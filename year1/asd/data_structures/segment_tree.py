class SegmentTree:
    def __init__(self, T, operation=lambda a, b: a + b):
        self.n = len(T)
        self.tree = [0 for _ in range(2 * self.n)]
        self.operation = operation

        self.build_tree(T)

    # O(n)
    def build_tree(self, T):
        n = self.n

        for i in range(n):
            self.tree[n + i] = T[i]

        for i in range(n - 1, 0, -1):
            self.tree[i] = self.operation(self.tree[i << 1], self.tree[i << 1 | 1])

    # O(logn)
    def update(self, i, x):
        n = self.n

        self.tree[i + n] = x
        i += n

        while i > 1:
            self.tree[i >> 1] = self.operation(self.tree[i], self.tree[i ^ 1])
            i >>= 1

    # [l, r) 
    # O(logn)
    def query(self, l, r):
        res = 0

        l += self.n
        r += self.n

        while l < r:
            if l & 1:
                res = self.operation(res, self.tree[l])
                l += 1

            if r & 1:
                r -= 1
                res = self.operation(res, self.tree[r])

            l >>= 1
            r >>= 1

        return res
