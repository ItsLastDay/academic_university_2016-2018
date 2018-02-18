import random

"""
My version of quadtree, which slightly differs from Wikipedia definition.
"""

def does_not_intersect(x1, x2):
    max_of_min = max(x1[0], x2[0])
    min_of_max = min(x1[1], x2[1])

    return max_of_min > min_of_max

class QuadTreeNode:
    def __init__(self, lower_left, upper_right, points):
        self.points = points
        self.lower_left = lower_left
        self.upper_right = upper_right
        self.nw = None
        self.ne = None
        self.sw = None
        self.se = None

    def query(self, lower_left, upper_right):
        # Our node lies inside given rectangle.
        if (self.lower_left[0] >= lower_left[0] and
            self.lower_left[1] >= lower_left[1] and
            self.upper_right[0] <= upper_right[0] and
            self.upper_right[1] <= upper_right[1]):
            return self.points

        # Our node does not intersect given rectangle.
        if does_not_intersect((self.lower_left[0], self.upper_right[0]), (lower_left[0], upper_right[0])):
            return []
        if does_not_intersect((self.lower_left[1], self.upper_right[1]), (lower_left[1], upper_right[1])):
            return []

        res = []
        any_child = False
        for child in (self.nw, self.ne, self.sw, self.se):
            if child is not None:
                res.extend(child.query(lower_left, upper_right))
                any_child = True

        # When there are no children, there can still be some suitable points.
        if not any_child:
            for point in self.points:
                if (lower_left[0] <= point['x'] <= upper_right[0] and
                        lower_left[1] <= point['y'] <= upper_right[1]):
                    res.append(point)
            

        return res




def find_kth(seq, k):
    """
    Find k-th order statistics using randomization like in QuickSort.
    Assume that all elements are distinct.
    """
    i, j = 0, len(seq) - 1

    while i < j:
        pivot_idx = random.randint(i, j)
        seq[pivot_idx], seq[j] = seq[j], seq[pivot_idx]

        idx_read = 0
        free_place_higher = j - 1

        while idx_read <= free_place_higher:
            if seq[idx_read] <= seq[j]:
                idx_read += 1
            else:
                seq[free_place_higher], seq[idx_read] = seq[idx_read], seq[free_place_higher]
                free_place_higher -= 1

        # [i, idx_read - 1] = lower or equal
        # [free_place_higher + 1, j - 1] = higher
        # [j] = equal
        seq[free_place_higher + 1], seq[j] = seq[j], seq[free_place_higher + 1]

        # [i, idx_read - 1] = lower or equal
        # [idx_read = free_place_higher + 1] = equal
        # [free_place_higher + 2, j] = higher
        sz_lower_eq = idx_read - i
        if sz_lower_eq == k - 1:
            return seq[idx_read]
        elif sz_lower_eq >= k:
            j = idx_read - 1
        else:
            k -= sz_lower_eq + 1
            i = idx_read + 1


    return seq[i]


class QuadTree:
    # Each point must be a dict with ['x'] and ['y']
    def __init__(self, points):
        self.root = self._build(points)

    def _build(self, points):
        if len(points) == 0:
            return None

        xs = [point['x'] for point in points]
        ys = [point['y'] for point in points]

        max_x = max(xs)
        min_x = min(xs)

        max_y = max(ys)
        min_y = min(ys)

        node = QuadTreeNode((min_x, min_y), (max_x, max_y), points)

        distinct_points = set((point['x'], point['y']) for point in points)

        if len(points) > 1 and len(distinct_points) > 1:
            median_x = find_kth(xs, (len(xs) + 1) // 2)
            median_y = find_kth(ys, (len(ys) + 1) // 2)

            nwest = [point for point in points if point['x'] < median_x and point['y'] >= median_y]
            swest = [point for point in points if point['x'] < median_x and point['y'] < median_y] 

            neast = [point for point in points if point['x'] >= median_x and point['y'] >= median_y]
            seast = [point for point in points if point['x'] >= median_x and point['y'] < median_y]

            if (len(nwest) == len(points) or
                len(swest) == len(points) or
                len(neast) == len(points) or
                len(seast) == len(points)):
                # Not subdividable.
                return node

            node.nw = self._build(nwest)
            node.sw = self._build(swest)
            node.ne = self._build(neast)
            node.se = self._build(seast)

        return node

    def query(self, lower_left, upper_right):
        """
        Return all points in [lower_left, upper_right] rectangle (both are points).
        """
        return self.root.query(lower_left, upper_right)
