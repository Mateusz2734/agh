from math import inf


class ITNode:
    def __init__(self, root, key, l_span, r_span):
        self.key = key
        self.left = None
        self.right = None
        self.parent = root
        self.intervals = []
        self.left_span = l_span
        self.right_span = r_span


class IntervalTree:
    def __init__(self, T):
        self.root = self.build_tree(T, None, 0, len(T) - 1, -inf, inf)

    def add_interval(self, interval):
        self._add(self.root, interval)

    def _add(self, root: ITNode, interval):
        if root.key is None:
            root.intervals.append(interval)
        elif root.key >= interval[1]:
            self._add(root.left, interval)
        elif root.key <= interval[0]:
            self._add(root.right, interval)
        elif interval[0] <= root.key <= interval[1]:
            if root.left_span >= interval[0] and root.right_span <= interval[1]:
                root.intervals.append(interval)
            else:
                self._add(root.left, interval)
                self._add(root.right, interval)

    def point_query(self, point):
        result = []
        root = self.root

        while root is not None:
            for i in range(len(root.intervals)):
                result.append(root.intervals[i])
            if root.key is None or root.key == point:
                return result
            if point < root.key:
                root = root.left
            else:
                root = root.right

    def build_tree(self, T, root, a, b, l_span, r_span):
        if b < a:
            return ITNode(root, None, l_span, r_span)
        mid = (a + b) // 2
        new_node = ITNode(root, T[mid], l_span, r_span)
        if mid - a - 1 >= 0:
            new_node.left = self.build_tree(
                T, new_node, a, mid - 1, new_node.left_span, new_node.key
            )
        else:
            new_node.left = ITNode(root, None, new_node.left_span, new_node.key)
        if b - mid - 1 >= 0:
            new_node.right = self.build_tree(
                T, new_node, mid + 1, b, new_node.key, new_node.right_span
            )
        else:
            new_node.right = ITNode(root, None, new_node.key, new_node.right_span)
        return new_node
