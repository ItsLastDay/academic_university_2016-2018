import itertools
import copy

class BTreeNode:

    @staticmethod
    def create_empty():
        return BTreeNode(True, None, None, None)

    def __init__(self, is_empty, le, rg, x):
        self.le = le
        self.rg = rg
        self.val = x
        self.is_empty = is_empty

    def add(self, elem):
        # Recursively add into one of the subtrees.
        # New empty nodes are created.
        if self.is_empty:
            return BTreeNode(False, 
                    self.create_empty(), 
                    self.create_empty(),
                    elem)

        if elem == self.val:
            return self
        elif elem < self.val:
            self.le = self.le.add(elem)
        else:
            self.rg = self.rg.add(elem)

        return self


    def __iter__(self):
        if self.is_empty:
            return iter([])

        return itertools.chain(iter(self.le), iter([self.val]), iter(self.rg))

    def __contains__(self, elem):
        if self.is_empty:
            return False

        if self.val == elem:
            return True
        if elem < self.val:
            return elem in self.le
        else:
            return elem in self.rg


class BTree:
    def __init__(self):
        self.root = BTreeNode.create_empty()

    def __contains__(self, x):
        return x in self.root

    def __iter__(self):
        return iter(self.root)

    def __add__(self, other):
        if isinstance(other, (BTree, int)):
            cp = copy.deepcopy(self)
            cp += other
            return cp
        else:
            return NotImplemented

    def __iadd__(self, other):
        if isinstance(other, int):
            self.root = self.root.add(other)
        elif isinstance(other, BTree):
            for elem in iter(other):
                self += elem
        else:
            return NotImplemented

        return self


tree = BTree()
for i in range(100):
    tree = tree + i

other = BTree()
other += -501
tree += -500
tree += other

gtree = tree + tree
print(list(other))

print(10 in tree)
print(-500 in tree)
print(-501 in tree)

for i in iter(tree):
    print(i)
