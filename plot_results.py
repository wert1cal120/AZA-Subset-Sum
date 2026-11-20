#!/usr/bin/env python3
"""Generate six plots from the Subset Sum experiments CSV."""

from __future__ import annotations

import argparse
import csv
import statistics
import sys
from collections import defaultdict
from pathlib import Path

try:
    import matplotlib.pyplot as plt
except ImportError:
    print("Matplotlib is required. Install it with: python3 -m pip install matplotlib", file=sys.stderr)
    raise SystemExit(1)


TESTS = {
    "possible": "Solvable tests",
    "random": "Random tests",
    "impossible": "Unsatisfiable tests",
}
ALGORITHMS = {"A": "Algorithm A", "B": "Algorithm B"}
REQUIRED_COLUMNS = {"n", "test", "algorithm", "status", "calls", "runtime_ms"}


def default_csv_path(script_dir: Path) -> Path:
    candidates = [script_dir / "results.csv", script_dir / "cmake-build-debug" / "results.csv"]
    for path in candidates:
        if path.is_file():
            return path
    return candidates[0]


def normalize_test(value: str) -> str:
    name = value.strip().lower()
    aliases = {
        "solvable": "possible",
        "possible": "possible",
        "random": "random",
        "mixed": "random",
        "unsatisfiable": "impossible",
        "impossible": "impossible",
    }
    return aliases.get(name, name)


def normalize_algorithm(value: str) -> str:
    name = value.strip().upper()
    if name in {"A", "BASELINE", "BASELINE ALG", "BASELINE ALGORITHM"}:
        return "A"
    if name in {"B", "IMPROVED", "IMPROVED ALG", "IMPROVED ALGORITHM"}:
        return "B"
    return name


def is_timeout(value: str) -> bool:
    return "TIMEOUT" in value.strip().upper()


def read_results(csv_path: Path):
    # Each key collects the three trials for one test size and algorithm.
    groups = defaultdict(lambda: {"calls": [], "runtime_ms": [], "timeout": False, "rows": 0})

    try:
        with csv_path.open("r", newline="", encoding="utf-8-sig") as source:
            reader = csv.DictReader(source)
            if reader.fieldnames is None:
                raise ValueError("CSV is empty or has no header row")

            missing = REQUIRED_COLUMNS - set(reader.fieldnames)
            if missing:
                raise ValueError("CSV is missing columns: " + ", ".join(sorted(missing)))

            for line_number, row in enumerate(reader, start=2):
                test = normalize_test(row["test"])
                algorithm = normalize_algorithm(row["algorithm"])
                if test not in TESTS:
                    raise ValueError(f"Unknown test type '{row['test']}' on CSV line {line_number}")
                if algorithm not in ALGORITHMS:
                    raise ValueError(f"Unknown algorithm '{row['algorithm']}' on CSV line {line_number}")

                n = int(row["n"])
                key = (test, algorithm, n)
                group = groups[key]
                group["rows"] += 1

                if is_timeout(row["status"]):
                    group["timeout"] = True
                    continue

                group["calls"].append(int(row["calls"]))
                group["runtime_ms"].append(float(row["runtime_ms"]))

    except OSError as error:
        raise ValueError(f"Cannot read CSV file '{csv_path}': {error}") from error

    if not groups:
        raise ValueError("CSV contains no experiment rows")

    sizes = sorted({n for _, _, n in groups})
    return groups, sizes


def median_for(groups, test: str, algorithm: str, n: int, metric: str):
    group = groups.get((test, algorithm, n))
    if group is None or group["timeout"] or not group[metric]:
        return None
    return statistics.median(group[metric])


def make_plot(groups, sizes, test: str, metric: str, ylabel: str, output_dir: Path):
    figure, axis = plt.subplots(figsize=(7.5, 4.8))
    has_data = False

    for algorithm, label in ALGORITHMS.items():
        values = [median_for(groups, test, algorithm, n, metric) for n in sizes]
        if any(value is not None for value in values):
            has_data = True
        plotted = [float("nan") if value is None else value for value in values]
        axis.plot(sizes, plotted, marker="o", linewidth=2, label=label)

        for position, n in enumerate(sizes):
            group = groups.get((test, algorithm, n))
            if group is not None and group["timeout"]:
                y_fraction = 0.96 if algorithm == "A" else 0.87
                axis.text(
                    n,
                    y_fraction,
                    f"{label}: TIMEOUT",
                    transform=axis.get_xaxis_transform(),
                    ha="center",
                    va="top",
                    fontsize=8,
                    color="crimson",
                )

    axis.set_title(f"{TESTS[test]} — {ylabel.lower()} by input size")
    axis.set_xlabel("Input size n")
    axis.set_ylabel(ylabel)
    axis.set_xticks(sizes)
    axis.grid(True, which="both", linestyle="--", alpha=0.35)
    axis.legend()

    if metric == "calls":
        axis.set_yscale("log")
    if not has_data:
        axis.text(0.5, 0.5, "No completed runs", transform=axis.transAxes, ha="center", va="center")

    figure.tight_layout()
    output_path = output_dir / f"{metric}_{test}.png"
    figure.savefig(output_path, dpi=200)
    plt.close(figure)
    return output_path


def main() -> int:
    script_dir = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description="Plot Subset Sum experiment results from CSV.")
    parser.add_argument(
        "csv_file",
        nargs="?",
        type=Path,
        default=default_csv_path(script_dir),
        help="CSV path (default: results.csv next to the script or in cmake-build-debug)",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=None,
        help="directory for the six PNG charts (default: plots next to the CSV)",
    )
    args = parser.parse_args()
    csv_path = args.csv_file.expanduser().resolve()
    output_dir = (args.output_dir or csv_path.parent / "plots").expanduser().resolve()

    try:
        groups, sizes = read_results(csv_path)
        output_dir.mkdir(parents=True, exist_ok=True)

        written = []
        for test in TESTS:
            written.append(make_plot(groups, sizes, test, "calls", "Recursive calls", output_dir))
        for test in TESTS:
            written.append(make_plot(groups, sizes, test, "runtime_ms", "Runtime (ms)", output_dir))

    except (ValueError, OSError) as error:
        print(error, file=sys.stderr)
        return 1

    print(f"Read results from: {csv_path}")
    print(f"Saved six charts to: {output_dir}")
    for path in written:
        print(f"  {path.name}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
