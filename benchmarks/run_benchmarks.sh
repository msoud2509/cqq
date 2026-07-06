#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
build_dir="$repo_root/build"
benchmark_binary="$build_dir/cqq_benchmarks"
benchmarks_md="$repo_root/BENCHMARKS.md"

if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
    cmake -S "$repo_root" -B "$build_dir" -DCMAKE_BUILD_TYPE=Release
fi

cmake --build "$build_dir" --target cqq_benchmarks

benchmark_output_file="$(mktemp)"
trap 'rm -f "$benchmark_output_file"' EXIT

"$benchmark_binary" | tee "$benchmark_output_file"

format_table() {
    python3 - "$1" <<'PY'
from pathlib import Path
import re
import sys

output_path = Path(sys.argv[1])

row_pattern = re.compile(
    r'^BM_Simulation/'
    r'(?P<qubits>\d+)/'
    r'(?P<cregs>\d+)/'
    r'(?P<gates>\d+)/'
    r'(?P<shots>\d+)/'
    r'(?P<seed>\d+)\s+'
    r'(?P<time_ns>[0-9.e+-]+)\s+ns\s+'
    r'(?P<cpu_ns>[0-9.e+-]+)\s+ns\s+'
    r'(?P<iterations>\d+)$'
)

def humanize(ns_text: str) -> str:
    ns = float(ns_text)
    magnitude = abs(ns)

    if magnitude >= 1e9:
        return f'{ns / 1e9:.1f} s'
    if magnitude >= 1e6:
        return f'{ns / 1e6:.1f} ms'
    if magnitude >= 1e3:
        return f'{ns / 1e3:.1f} us'
    return f'{ns:.0f} ns'


def format_value(ns_text: str) -> str:
    return f'{ns_text} ns ({humanize(ns_text)})'


rows = []
for line in output_path.read_text().splitlines():
    match = row_pattern.match(line.strip())
    if not match:
        continue

    groups = match.groupdict()

    rows.append(
        [
            'Random circuit simulation',
            groups['qubits'],
            groups['cregs'],
            groups['gates'],
            groups['shots'],
            groups['seed'],
            format_value(groups['time_ns']),
            format_value(groups['cpu_ns']),
            groups['iterations'],
        ]
    )

if not rows:
    raise SystemExit('No benchmark rows found in benchmark output.')

print('| Benchmark | Qubits | Classical regs | Gates | Shots | Seed | Time | CPU | Iterations |')
print('| --- | --- | --- | --- | --- | --- | --- | --- | --- |')
for row in rows:
    print('| ' + ' | '.join(row) + ' |')
PY
}

read -r -p "Apply these results to BENCHMARKS.md? [y/N] " apply_results
case "$apply_results" in
    y|Y)
        current_version="$(python3 - "$benchmarks_md" <<'PY'
from pathlib import Path
import sys

path = Path(sys.argv[1])
lines = path.read_text().splitlines()
seen_log = False

for line in lines:
    if line.strip() == '## Benchmark Log':
        seen_log = True
        continue
    if seen_log and line.startswith('## '):
        version = line[3:].strip()
        print(version if version.startswith('v') else f'v{version}')
        break
else:
    print('')
PY
)"

        if [[ -n "$current_version" ]]; then
            echo "Current version: ${current_version}"
        else
            echo "Current version: none"
        fi
        read -r -p "New version number: " new_version

        normalized_new_version="$(python3 - "$new_version" <<'PY'
import sys

version = sys.argv[1].strip()
if not version:
    raise SystemExit('Version number cannot be empty.')

print(version if version.startswith('v') else f'v{version}')
PY
)"

        markdown_table="$(format_table "$benchmark_output_file")"

        python3 - "$benchmarks_md" "$normalized_new_version" "$markdown_table" <<'PY'
from pathlib import Path
import sys

markdown_path = Path(sys.argv[1])
new_version = sys.argv[2].strip()
benchmark_table = sys.argv[3].rstrip()

if not new_version:
    raise SystemExit('Version number cannot be empty.')

content = markdown_path.read_text()
marker = '## Benchmark Log\n'
index = content.find(marker)

if index == -1:
    raise SystemExit('Could not find the Benchmark Log section in BENCHMARKS.md.')

insert_at = index + len(marker)
new_entry = (
    f'\n## {new_version}\n\n'
    f'*Add description here*\n\n'
    f'{benchmark_table}\n'
)

markdown_path.write_text(content[:insert_at] + new_entry + content[insert_at:])
PY

        echo "Updated BENCHMARKS.md with version ${normalized_new_version}."
        ;;
    *)
        echo "Skipped updating BENCHMARKS.md."
        ;;
esac
