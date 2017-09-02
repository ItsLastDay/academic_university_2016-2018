"""
Module that contains enums: various types of game objects.
"""
from enum import Enum, auto


class BodyPlace(Enum):
    """Possible slots in Hero's inventory"""
    HEAD = auto()
    LEFT_HAND = auto()
    RIGHT_HAND = auto()
    BODY = auto()
    LEGS = auto()


class InstantItemType(Enum):
    """Types of consumable items"""
    HEAL_POTION = auto()
    MANA_POTION = auto()


class SpellType(Enum):
    """Types of spells"""
    TELEPORT = auto()
    HEAL = auto()


class TileType(Enum):
    """Types of tiles on map"""
    EXISTING_THING = auto()
    WALL = auto()
    EMPTY = auto()


class MonsterType(Enum):
    """Types of monsters"""
    ZOMBIE = auto()
    DRAGON = auto()
    FLY = auto()

