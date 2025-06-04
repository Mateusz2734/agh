import json

import matplotlib.pyplot as plt


def load_data(path):
    with open(path, "r") as f:
        return [json.loads(line) for line in f]


file1 = "docs/ref.jsonl"
file2 = "docs/opt.jsonl"
file3 = "docs/nmul.jsonl"

data1 = load_data(file1)
data2 = load_data(file2)
data3 = load_data(file3)

data1.sort(key=lambda x: x["size"])
data2.sort(key=lambda x: x["size"])
data3.sort(key=lambda x: x["size"])

sizes = [d["size"] for d in data1]
time1 = [d["time"] for d in data1]
gflops1 = [d["gflops"] for d in data1]

time2 = [d["time"] for d in data2]
gflops2 = [d["gflops"] for d in data2]

time3 = [d["time"] for d in data3]

# Time plot
plt.figure(figsize=(10, 6))
plt.plot(sizes, time1, label="Reference", marker="o")
plt.plot(sizes, time3, label="Reference (no multiplier optimization)", marker="s")
plt.plot(sizes, time2, label="Optimized", marker="s")
plt.xlabel("Matrix Size")
plt.ylabel("Time (s)")
plt.title("Time vs Matrix Size")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("docs/time.png")

# Gflops plot
plt.figure(figsize=(10, 6))
plt.plot(sizes, gflops1, label="Reference", marker="o")
plt.plot(sizes, gflops2, label="Optimized", marker="s")
plt.xlabel("Matrix Size")
plt.ylabel("Gflops")
plt.title("Gflops vs Matrix Size")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("docs/gflops.png")
