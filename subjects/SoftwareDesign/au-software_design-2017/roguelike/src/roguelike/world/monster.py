"""
Game monsters that can attack hero.
"""
import random

from ..event_history import EventHistory
from .enumerations import MonsterType, TileType
from .existing_thing import ExistingThing
from .game_settings import GameSettings
from ..position import ShiftDirection
from roguelike.world.tile import Tile

possible_shifts = [
    ShiftDirection(dx=1, dy=0),
    ShiftDirection(dx=0, dy=1),
    ShiftDirection(dx=0, dy=-1),
    ShiftDirection(dx=-1, dy=0),
]


class Monster(ExistingThing):
    """A monster in dungeon"""

    def __init__(self, tp, attack, dexterity, pos):
        super().__init__(pos)
        self.attack = attack
        self.dexterity = dexterity
        self.type = tp

    def make_turn(self, tile):
        cur_map = GameSettings.current_map
        random.shuffle(possible_shifts)

        for shift_dir in possible_shifts:
            if cur_map.check_feasible_move(self.pos, shift_dir):
                cur_map.move_thing(tile, shift_dir)
                return

    def attack_hero(self, hero):
        EventHistory.add_event('Monster {} attacks hero'.format(self.type))
        if random.randint(1, 100) <= hero.charisma:
            EventHistory.add_event('Monster was convinced to leave by charismatic hero')
            return

        attack_points = self.attack
        if random.randint(1, 100) <= self.dexterity:
            attack_points *= 2
            EventHistory.add_event('Monster-s dexterity plays for him: attack increase x2')

        attack_points -= hero.armor
        EventHistory.add_event('Monster hits player for {} hitpoints'.format(attack_points))
        hero.experience += 100
        hero.hp -= attack_points

    def get_visual_map_representation(self):
        if self.type == MonsterType.ZOMBIE:
            return '^'
        elif self.type == MonsterType.DRAGON:
            return '!'
        elif self.type == MonsterType.FLY:
            return '<'
        else:
            assert(False)

    def __str__(self):
        return 'Monster({})'.format(self.type.name)
