from random import shuffle, randrange

def make_maze(w = 16, h = 8):
    w //= 2
    h //= 2
    vis = [[0] * w + [1] for _ in range(h)] + [[1] * (w + 1)]
    ver = [["10"] * w + ['1'] for _ in range(h)] + [[]]
    hor = [["11"] * w + ['1'] for _ in range(h + 1)]

    def walk(x, y):
        vis[y][x] = 1

        d = [(x - 1, y), (x, y + 1), (x + 1, y), (x, y - 1)]
        shuffle(d)
        for (xx, yy) in d:
            if vis[yy][xx]: continue
            if xx == x: hor[max(y, yy)][x] = "10"
            if yy == y: ver[y][max(x, xx)] = "00"
            walk(xx, yy)

    walk(randrange(w), randrange(h))

    s = "1"*(2*w+3) + "\n"
    
    for (a, b) in zip(hor, ver):
        if len(a) > 0 and len(b) > 0:
            s += ''.join(["1"] + a + ["1"] + ['\n'] + ["1"] + b + ["1"] + ['\n'])
    s += "1"*(2*w+3) + "\n"
    s += "1"*(2*w+3) + "\n"
    return s


if __name__ == "__main__":
    lines = [line for line in make_maze(40, 40).split("\n") if line.strip()]

    num_lines = len(lines)
    num_cols = len(lines[0])

    knots_x = [0] + list(range(num_cols)) + [num_cols-1]
    knots_y = [0] + list(range(num_lines)) + [num_lines-1]

    print("knot_vectorx = ", knots_x)
    print("knot_vectory = ", knots_y)
    
    print("coeffs = [")
    for line in lines:
        print("    " + " ".join(list(map(lambda x: str(int(x)), line.strip()))) + ";")
    print("]")
