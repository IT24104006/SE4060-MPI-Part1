import csv
import statistics as st
import matplotlib
matplotlib.use("Agg")   # no display needed on the server
import matplotlib.pyplot as plt

def load(path):
    d = {}
    with open(path) as f:
        for row in csv.DictReader(f):
            d.setdefault(int(row["procs"]), []).append(float(row["time"]))
    return d

datasets = [
    ("Exercise 2: Sum of 1..10,000,000", load("../exercise2/sum_results.csv")),
    ("Exercise 3: Monte Carlo Pi (10,000,000 samples)", load("../exercise3/pi_results.csv")),
]

# print a table you can paste into your report
for title, d in datasets:
    procs = sorted(d)
    t1 = st.mean(d[1])
    print(title)
    print("procs  avg_time(s)  stdev(s)  speedup")
    for p in procs:
        m = st.mean(d[p])
        print(f"{p:5d}  {m:11.6f}  {st.stdev(d[p]):8.6f}  {t1 / m:7.2f}")
    print()

# Graph 1: time vs number of processors
fig, axes = plt.subplots(1, 2, figsize=(11, 4))
for ax, (title, d) in zip(axes, datasets):
    procs = sorted(d)
    means = [st.mean(d[p]) for p in procs]
    errs = [st.stdev(d[p]) for p in procs]
    ax.errorbar(procs, means, yerr=errs, marker="o", capsize=4)
    ax.set_xscale("log", base=2)
    ax.set_xticks(procs)
    ax.set_xticklabels(procs)
    ax.set_xlabel("Number of processes")
    ax.set_ylabel("Time (s)")
    ax.set_title(title, fontsize=10)
    ax.grid(True, alpha=0.3)
fig.tight_layout()
fig.savefig("time_vs_processes.png", dpi=150)

# Graph 2: speedup
fig, axes = plt.subplots(1, 2, figsize=(11, 4))
for ax, (title, d) in zip(axes, datasets):
    procs = sorted(d)
    t1 = st.mean(d[1])
    speedup = [t1 / st.mean(d[p]) for p in procs]
    ax.plot(procs, speedup, marker="o", label="Measured")
    ax.plot(procs, procs, linestyle="--", color="gray", label="Ideal")
    ax.set_xscale("log", base=2)
    ax.set_xticks(procs)
    ax.set_xticklabels(procs)
    ax.set_xlabel("Number of processes")
    ax.set_ylabel("Speedup (T1 / Tp)")
    ax.set_title(title, fontsize=10)
    ax.legend()
    ax.grid(True, alpha=0.3)
fig.tight_layout()
fig.savefig("speedup.png", dpi=150)
print("Saved time_vs_processes.png and speedup.png")
