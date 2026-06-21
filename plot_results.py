#!/usr/bin/env python3
import argparse
import os
import sys

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker


# Stałe
STRUCT_LABELS = {
    "adjacencyList":   "Lista sąsiedztwa",
    "incidenceMatrix": "Macierz incydencji",
}

PROBLEM_LABELS = {
    "mst": "MST (Prim)",
    "sp":  "SP (Dijkstra)",
}

COLORS = {
    "adjacencyList":   "#2196F3",
    "incidenceMatrix": "#F44336",
}

MARKERS = {
    "adjacencyList":   "o",
    "incidenceMatrix": "s",
}


def load_csv(path: str) -> pd.DataFrame:
    df = pd.read_csv(path)

    # Nowy format: jeden wiersz na iterację (kolumna time_us)
    # Agregujemy do min/avg/max per (problem, algorithm, structure, vertices, edges, density)
    if "time_us" in df.columns:
        group_cols = ["problem", "algorithm", "structure", "vertices", "edges", "density"]
        df = (df.groupby(group_cols)["time_us"]
                .agg(min_us="min", avg_us="mean", max_us="max")
                .reset_index())
        df["iterations"] = df.groupby(group_cols[:-1])["min_us"].transform("count")

    required = {"problem", "algorithm", "structure", "vertices", "edges",
                "density", "min_us", "avg_us", "max_us"}
    missing = required - set(df.columns)
    if missing:
        print(f"Błąd: brakujące kolumny w CSV: {missing}", file=sys.stderr)
        sys.exit(1)
    return df


def plot_study_a(df: pd.DataFrame, out_dir: str) -> None:
    """Badanie A: czas vs liczba wierzchołków (gęstość 50%)."""
    data = df[df["density"] == 50].copy()
    if data.empty:
        print("Brak danych Badania A (gęstość=50%) – pomijam.", file=sys.stderr)
        return

    for prob in data["problem"].unique():
        prob_data = data[data["problem"] == prob]

        # Rysuj dwa wykresy: liniowy i log-log
        for log_scale in (False, True):
            fig, ax = plt.subplots(figsize=(8, 5))

            for struct in prob_data["structure"].unique():
                s = prob_data[prob_data["structure"] == struct].sort_values("vertices")
                label = STRUCT_LABELS.get(struct, struct)
                color = COLORS.get(struct, "gray")
                marker = MARKERS.get(struct, "o")

                ax.plot(s["vertices"], s["avg_us"],
                        label=f"{label} – śr.",
                        color=color, marker=marker, linewidth=2)
                ax.fill_between(s["vertices"], s["min_us"], s["max_us"],
                                alpha=0.15, color=color, label=f"{label} – min/max")

            suffix = ""
            if log_scale:
                ax.set_yscale("log")
                ax.set_xscale("log")
                suffix = "_log"

            ax.set_title(f"Badanie A – {PROBLEM_LABELS.get(prob, prob)}\n"
                         f"Wpływ rozmiaru grafu (gęstość 50%)"
                         + (" [skala log]" if log_scale else ""))
            ax.set_xlabel("Liczba wierzchołków")
            ax.set_ylabel("Czas [µs]")
            ax.legend(fontsize=9)
            if not log_scale:
                ax.yaxis.set_major_formatter(ticker.FuncFormatter(
                    lambda x, _: f"{int(x):,}".replace(",", " ")))
            ax.grid(True, linestyle="--", alpha=0.4)
            fig.tight_layout()

            fname = os.path.join(out_dir, f"study_A_{prob}{suffix}.png")
            fig.savefig(fname, dpi=150)
            plt.close(fig)
            print(f"  Zapisano: {fname}")


def plot_study_b(df: pd.DataFrame, out_dir: str) -> None:
    """Badanie B: czas vs gęstość (stały rozmiar)."""
    candidates = df[df["density"] != 50]["vertices"]
    if candidates.empty:
        candidates = df["vertices"]
    fixed_v = int(candidates.mode().iloc[0]) if not candidates.empty else None

    data = df[df["vertices"] == fixed_v].copy() if fixed_v else df.copy()
    if data.empty:
        print("Brak danych Badania B – pomijam.", file=sys.stderr)
        return

    for prob in data["problem"].unique():
        prob_data = data[data["problem"] == prob]
        fig, ax = plt.subplots(figsize=(8, 5))

        for struct in prob_data["structure"].unique():
            s = prob_data[prob_data["structure"] == struct].sort_values("density")
            label = STRUCT_LABELS.get(struct, struct)
            color = COLORS.get(struct, "gray")
            marker = MARKERS.get(struct, "o")

            ax.plot(s["density"], s["avg_us"],
                    label=f"{label} – śr.",
                    color=color, marker=marker, linewidth=2)
            ax.fill_between(s["density"], s["min_us"], s["max_us"],
                            alpha=0.15, color=color, label=f"{label} – min/max")

        ax.set_title(f"Badanie B – {PROBLEM_LABELS.get(prob, prob)}\n"
                     f"Wpływ gęstości (V={fixed_v})")
        ax.set_xlabel("Gęstość grafu [%]")
        ax.set_ylabel("Czas [µs]")
        ax.legend(fontsize=9)
        ax.yaxis.set_major_formatter(ticker.FuncFormatter(
            lambda x, _: f"{int(x):,}".replace(",", " ")))
        ax.grid(True, linestyle="--", alpha=0.4)
        fig.tight_layout()

        fname = os.path.join(out_dir, f"study_B_{prob}.png")
        fig.savefig(fname, dpi=150)
        plt.close(fig)
        print(f"  Zapisano: {fname}")


def plot_comparison_bar(df: pd.DataFrame, out_dir: str) -> None:
    """Wykres słupkowy z log-skalą: porównanie obu reprezentacji per problem."""
    summary = (df.groupby(["problem", "structure"])["avg_us"]
                 .mean()
                 .unstack("structure")
                 .rename(columns=STRUCT_LABELS))

    if summary.empty:
        return

    # Liniowy
    ax = summary.plot(kind="bar", figsize=(8, 5),
                      color=[COLORS["adjacencyList"], COLORS["incidenceMatrix"]],
                      edgecolor="white", width=0.6)
    ax.set_title("Porównanie reprezentacji – średni czas [µs]")
    ax.set_xlabel("Problem")
    ax.set_ylabel("Śr. czas [µs]")
    ax.set_xticklabels([PROBLEM_LABELS.get(p, p) for p in summary.index],
                       rotation=0)
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(
        lambda x, _: f"{int(x):,}".replace(",", " ")))
    ax.legend(title="Reprezentacja")
    ax.grid(True, axis="y", linestyle="--", alpha=0.4)
    plt.tight_layout()
    fname = os.path.join(out_dir, "comparison_bar.png")
    plt.savefig(fname, dpi=150)
    plt.close()
    print(f"  Zapisano: {fname}")

    # Log-skala 
    ax2 = summary.plot(kind="bar", figsize=(8, 5),
                       color=[COLORS["adjacencyList"], COLORS["incidenceMatrix"]],
                       edgecolor="white", width=0.6)
    ax2.set_yscale("log")
    ax2.set_title("Porównanie reprezentacji – średni czas [µs, skala log]")
    ax2.set_xlabel("Problem")
    ax2.set_ylabel("Śr. czas [µs] (log)")
    ax2.set_xticklabels([PROBLEM_LABELS.get(p, p) for p in summary.index],
                        rotation=0)
    ax2.legend(title="Reprezentacja")
    ax2.grid(True, axis="y", linestyle="--", alpha=0.4)
    plt.tight_layout()
    fname2 = os.path.join(out_dir, "comparison_bar_log.png")
    plt.savefig(fname2, dpi=150)
    plt.close()
    print(f"  Zapisano: {fname2}")


def print_summary_table(df: pd.DataFrame) -> None:
    """Wydrukuj tabelę podsumowującą na stdout."""
    summary = (df.groupby(["problem", "algorithm", "structure"])[["min_us", "avg_us", "max_us"]]
                 .agg(["mean"])
                 .round(1))
    summary.columns = ["min_mean_µs", "avg_mean_µs", "max_mean_µs"]
    print("\n--- Tabela podsumowująca (średnia z wszystkich pomiarów) ---")
    print(summary.to_string())
    print()


def main():
    parser = argparse.ArgumentParser(description="Generuje wykresy z wyników benchmarków.")
    parser.add_argument("--input",  default="results/results.csv",
                        help="Plik CSV z wynikami")
    parser.add_argument("--outdir", default="results/plots",
                        help="Katalog na wykresy PNG")
    args = parser.parse_args()

    if not os.path.isfile(args.input):
        print(f"Błąd: nie znaleziono pliku: {args.input}", file=sys.stderr)
        sys.exit(1)

    os.makedirs(args.outdir, exist_ok=True)

    df = load_csv(args.input)
    print(f"Wczytano {len(df)} wierszy z {args.input}")
    print_summary_table(df)

    print("Generuję wykresy...")
    plot_study_a(df, args.outdir)
    plot_study_b(df, args.outdir)
    plot_comparison_bar(df, args.outdir)
    print("Gotowe.")


if __name__ == "__main__":
    main()
