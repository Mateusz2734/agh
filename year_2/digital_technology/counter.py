from tabulate import tabulate
import pandas as pd


def replace_nth(input_string, substring, replacement, n):
    start_index = input_string.find(substring)
    while start_index >= 0 and n > 1:
        start_index = input_string.find(substring, start_index + len(substring))
        n -= 1

    if start_index >= 0:
        return (
            input_string[:start_index]
            + replacement
            + input_string[start_index + len(substring) :]
        )
    else:
        return input_string


def fmt(table):
    rows = []

    to_del = [9, 8, 7, 5, 4, 3]
    for row in table.split("\n"):
        if "|" in row:
            row = replace_nth(row, "|", "||", 10)
            for i in to_del:
                row = replace_nth(row, "|", "", i)

        elif "+" in row:
            row = replace_nth(row, "+", "++", 10)
            for i in to_del:
                row = replace_nth(row, "+", "", i)
        rows.append(row)
    return "\n".join(rows)


def q0_q1_to_t(q0, q1):
    return q0 ^ q1


def nth_bit(number, n):
    return (number >> (n - 1)) & 1


if __name__ == "__main__":
    df = pd.DataFrame()
    seq = [x for x in range(15, -1, -1)]
    seq1 = [x for x in range(14, -1, -1)] + [0]

    df["DEC"] = seq
    df["A0"] = [nth_bit(x, 4) for x in seq]
    df["B0"] = [nth_bit(x, 3) for x in seq]
    df["C0"] = [nth_bit(x, 2) for x in seq]
    df["D0"] = [nth_bit(x, 1) for x in seq]

    df["A1"] = [nth_bit(x, 4) for x in seq1]
    df["B1"] = [nth_bit(x, 3) for x in seq1]
    df["C1"] = [nth_bit(x, 2) for x in seq1]
    df["D1"] = [nth_bit(x, 1) for x in seq1]

    df["TA"] = [q0_q1_to_t(df["A0"][i], df["A1"][i]) for i in range(len(df))]
    df["TB"] = [q0_q1_to_t(df["B0"][i], df["B1"][i]) for i in range(len(df))]
    df["TC"] = [q0_q1_to_t(df["C0"][i], df["C1"][i]) for i in range(len(df))]
    df["TD"] = [q0_q1_to_t(df["D0"][i], df["D1"][i]) for i in range(len(df))]

    print()
    print(fmt(tabulate(df, headers="keys", tablefmt="pretty", showindex=False)))
    print()
