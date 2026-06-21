#!/usr/bin/env python3
import argparse
import subprocess
import os
import sys

# Parametry badań

# Badanie A – 6 rozmiarów grafów, gęstość 50%
STUDY_A_VERTICES = [50, 100, 200, 300, 500, 750]
STUDY_A_DENSITY  = 50

# Badanie B – stały rozmiar, 4 gęstości
STUDY_B_VERTICES = 300
STUDY_B_DENSITIES = [25, 50, 75, 99]

# Liczba powtórzeń każdego pomiaru
ITERATIONS = 50

# Problemy i algorytmy (zgodnie z Parameters.h)
PROBLEMS = {
    "mst": {"id": 0, "algorithm_id": 1},  # Prim
    "sp":  {"id": 1, "algorithm_id": 3},  # Dijkstra
}

# Reprezentacje: 0 = obie naraz
STRUCTURE_ALL = 0


def run_benchmark(binary: str, problem_id: int, algo_id: int,
                  vertices: int, density: int, iterations: int,
                  results_file: str) -> None:
    """Wywołuje pojedynczy pomiar benchmarkowy."""
    cmd = [
        binary,
        "--benchmark",
        "-p", str(problem_id),
        "-a", str(algo_id),
        "-s", str(STRUCTURE_ALL),
        "-l", str(vertices),
        "-d", str(density),
        "-n", str(iterations),
        "-r", results_file,
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.stdout:
        print(result.stdout, end="")
    if result.returncode != 0:
        print(f"  [BŁĄD] kod wyjścia {result.returncode}", file=sys.stderr)
        if result.stderr:
            print(result.stderr, file=sys.stderr)


def main():
    parser = argparse.ArgumentParser(description="Uruchamia badania projektu 2.")
    parser.add_argument("--binary",  default="./build/src/project",
                        help="Ścieżka do skompilowanego programu")
    parser.add_argument("--output",  default="results/results.csv",
                        help="Plik CSV z wynikami")
    parser.add_argument("--study",   choices=["A", "B", "all"], default="all",
                        help="Które badanie uruchomić")
    args = parser.parse_args()

    # Upewnij się że katalog wyjściowy istnieje
    os.makedirs(os.path.dirname(args.output) if os.path.dirname(args.output) else ".", exist_ok=True)

    # Oddzielne pliki dla badania A i B
    base, ext = os.path.splitext(args.output)
    output_a = base + "_A" + ext
    output_b = base + "_B" + ext

    binary = args.binary
    if not os.path.isfile(binary):
        print(f"Błąd: nie znaleziono binarnego: {binary}", file=sys.stderr)
        sys.exit(1)

    # Badanie A – wpływ rozmiaru grafu
    if args.study in ("A", "all"):
        print("\n" + "="*60)
        print(f"BADANIE A – wpływ rozmiaru (gęstość={STUDY_A_DENSITY}%)")
        print("="*60)
        for prob_name, prob in PROBLEMS.items():
            print(f"\n  Problem: {prob_name.upper()}")
            for V in STUDY_A_VERTICES:
                print(f"    V={V:>4}  ", end="", flush=True)
                run_benchmark(binary, prob["id"], prob["algorithm_id"],
                              V, STUDY_A_DENSITY, ITERATIONS, output_a)

    # Badanie B – wpływ gęstości grafu
    if args.study in ("B", "all"):
        print("\n" + "="*60)
        print(f"BADANIE B – wpływ gęstości (V={STUDY_B_VERTICES})")
        print("="*60)
        for prob_name, prob in PROBLEMS.items():
            print(f"\n  Problem: {prob_name.upper()}")
            for d in STUDY_B_DENSITIES:
                print(f"    density={d:>2}%  ", end="", flush=True)
                run_benchmark(binary, prob["id"], prob["algorithm_id"],
                              STUDY_B_VERTICES, d, ITERATIONS, output_b)

    print(f"\nWyniki zapisano do:")
    if args.study in ("A", "all"):
        print(f"  Badanie A: {output_a}")
    if args.study in ("B", "all"):
        print(f"  Badanie B: {output_b}")


if __name__ == "__main__":
    main()
