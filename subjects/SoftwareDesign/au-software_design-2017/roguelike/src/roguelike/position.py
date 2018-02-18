"""
Value classes for representing position\direction on a map.
"""
from collections import namedtuple

# Value class for representing position (e.g. on a map)
Position = namedtuple('Position', ['x', 'y'])

# Value class for representing shift of a position
ShiftDirection = namedtuple('ShiftDirection', ['dx', 'dy'])

# Value class for representing shift of a value
ShiftDirection1d = namedtuple('ShiftDirection1d', ['dx'])
