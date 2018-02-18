"""
Configuration for the game.
"""


class GameSettings:
    """Static class for game settings and current state of the game"""

    INITIAL_EXPERIENCE = 0
    INITIAL_STRENGTH = 10
    INITIAL_DEXTERITY = 4
    INITIAL_INTELLIGENCE = 5
    INITIAL_CHARISMA = 1

    HITPOINTS_PER_STRENGTH = 100
    MANAPOINTS_PER_INT = 50
    ARMOR_PER_DEX = 1.5

    HEAL_POTION_AMOUNT = 60
    MANA_POTION_AMOUNT = 30

    HEAL_SCROLL_MANACOST = 20
    HEAL_SCROLL_AMOUNT = 50

    TELEPORT_SCROLL_MANACOST = 70

    current_map = None
