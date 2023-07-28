class SegmentTree:
    def __init__(self, T):
        self.n = len(T)
        self.tree = [0 for _ in range(2 * self.n)]

        self.build_tree(T)

    def build_tree(self, T):
        n = self.n

        for i in range(n):
            self.tree[n + i] = T[i]

        for i in range(n - 1, 0, -1):
            self.tree[i] = self.tree[i << 1] + self.tree[i << 1 | 1]

    def update(self, i, x):
        n = self.n

        self.tree[i + n] = x
        i += n

        while i > 1:
            self.tree[i >> 1] = self.tree[i] + self.tree[i ^ 1]
            i >>= 1

    # [l, r)
    def sum(self, l, r):
        res = 0

        l += self.n
        r += self.n

        while l < r:
            if l & 1:
                res += self.tree[l]
                l += 1

            if r & 1:
                r -= 1
                res += self.tree[r]

            l >>= 1
            r >>= 1

        return res
