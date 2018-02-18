"""
Basic block of map.
"""


class Tile:
    def __init__(self, tp, pos, thing_ref=None):
        self.type = tp
        self._pos = pos
        self.thing_ref = thing_ref

    @property
    def pos(self):
        if self.thing_ref is not None:
            return self.thing_ref.pos
        return self._pos

    @pos.setter
    def pos(self, value):
        if self.thing_ref is not None:
            self.thing_ref.pos = value
        self._pos = value

    def __repr__(self):
        return 'Tile({}, {}, {})'.format(self.type, self.pos, self.thing_ref)
