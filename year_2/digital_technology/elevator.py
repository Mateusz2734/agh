from tabulate import tabulate
import pandas as pd

states = ["GO", "GC", "1O", "1C", "2O", "2C", "INIT"]
states_bin = {"GO": "1100", "GC": "0100", "1O": "1010", "1C": "0010", "2O": "1001", "2C": "0001", "INIT": "0000"}

df = pd.read_csv("data.csv")
df["CURR_BIN"] = df["CURR"].map(states_bin)
df["NEXT_BIN"] = df["NEXT"].map(states_bin)
df["INPUTS (G12)"] = df["INPUTS (G12)"].astype(str).str.zfill(3)

df=df[["CURR", "CURR_BIN", "INPUTS (G12)", "NEXT", "NEXT_BIN"]]

print(tabulate(df, headers="keys", tablefmt="grid", showindex=False, colalign=("center", "center", "center", "center", "center")))

lines = []
for k in range(4):
    zeros = []
    ones = []
    for _, row in df.iterrows():
        i = int(str(row["CURR_BIN"]), 2)
        j = int(str(row["INPUTS (G12)"]), 2)
        if row["NEXT_BIN"][k] == "1":
            ones.append(j*16+i)
        else:
            zeros.append(j*16+i)
    dontcares = [x for x in range(16*8) if x not in zeros and x not in ones]

    lines.append((k, ",".join([str(elem) for elem in ones]), ",".join([str(elem) for elem in dontcares]), "\n"))

with open("output.txt", "w") as f:
    for line in lines:
        f.write(f"K={line[0]}\n")
        f.write(f"1s: {line[1]}\n")
        f.write(f"DCs: {line[2]}\n")
        f.write("\n")
    f.write("\n")

words = []
for _, row in df.iterrows():
    words.append(f"{row['CURR_BIN']}{row['INPUTS (G12)']}{row['NEXT_BIN']}")

with open("word_generator.txt", "w") as f:
    f.write("CURRENT_STATE(4)|INPUTS(3)|NEXT_STATE(4)\n")
    f.write("\n".join(words))
    f.write("\n")