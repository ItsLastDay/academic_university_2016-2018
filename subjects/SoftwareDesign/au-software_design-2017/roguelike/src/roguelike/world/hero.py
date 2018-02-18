"""
Everything related to main Hero.
"""
from ..position import Position
from . import game_settings
from .exceptions import GameOverException


class Inventory:
    """Hero's inventory"""

    def __init__(self):
        self.items = []
        self.item_by_bodyplace = dict()
        self.selected_idx = 0

    def change_selection(self, shift_dir):
        """Change selected item according to shift_dir:ShiftDirection1d"""
        self.selected_idx += shift_dir.dx

        if self.items:
            self.selected_idx %= len(self.items)
        elif self.selected_idx < 0:
            self.selected_idx = 0

    def get_all_items_around_selected(self, n):
        list_items = []
        for idx in range(self.selected_idx-n, self.selected_idx+n+1):
            if idx < 0 or idx >= len(self.items):
                continue
            list_items.append((self.items[idx], idx == self.selected_idx, idx))

        return list_items

    def add_item(self, item):
        """Add item to inventory"""
        self.items.append(item)

    def remove_item(self, item):
        if item in self.items:
            idx = self.items.index(item)
            self.items.remove(item)
            if idx <= self.selected_idx:
                self.selected_idx = max(self.selected_idx - 1, 0)

    def get_selected_item(self):
        return self.items[self.selected_idx]

    def get_item_at_bodyplace(self, body_place):
        return self.item_by_bodyplace.get(body_place, None)

    def use_item(self, item):
        item.use()

    def wear_item(self, body_place, item):
        self.takeoff_item(body_place)
        self.item_by_bodyplace[body_place] = item
        self.remove_item(item)

    def takeoff_item(self, body_place):
        cur_worn = self.get_item_at_bodyplace(body_place)
        if cur_worn is not None:
            self.add_item(cur_worn)
        self.item_by_bodyplace[body_place] = None


class HeroBorgMeta(type):
    initialized_dict = dict()
    has_initialized = False

    def __call__(cls, *args, **kwargs):
        obj = super().__call__(*args, **kwargs)
        if not HeroBorgMeta.has_initialized:
            obj.__init__(*args, **kwargs)
            obj.pos = Position(0, 0)
            HeroBorgMeta.has_initialized = True
            HeroBorgMeta.initialized_dict = obj.__dict__
        else:
            obj.__dict__ = HeroBorgMeta.initialized_dict
        return obj


class Hero(metaclass=HeroBorgMeta):
    """Borg class representing current state of main character"""

    def get_visual_map_representation(self):
        return '@'

    def __init__(self):
        self._max_hp = 0
        self._hp = 0
        self._max_mana = 0
        self._mana = 0
        self._armor = 0
        self.experience = 0

        self._strength = game_settings.GameSettings.INITIAL_STRENGTH
        self._dexterity = game_settings.GameSettings.INITIAL_DEXTERITY
        self._intelligence = game_settings.GameSettings.INITIAL_INTELLIGENCE
        self._charisma = game_settings.GameSettings.INITIAL_CHARISMA
        self._update_stats()

        self.hp += 10 ** 18
        self.mana += 10 ** 18

        self._inventory = Inventory()

    @property
    def armor(self):
        return self._armor

    @property
    def max_hp(self):
        return self._max_hp

    @property
    def max_mana(self):
        return self._max_mana

    @property
    def inventory(self):
        return self._inventory

    def _update_stats(self):
        self._max_hp = self._strength * game_settings.GameSettings.HITPOINTS_PER_STRENGTH
        self._max_mana = self._intelligence * game_settings.GameSettings.MANAPOINTS_PER_INT
        self._armor = self._dexterity * game_settings.GameSettings.ARMOR_PER_DEX

    @property
    def hp(self):
        return self._hp

    @hp.setter
    def hp(self, value):
        self._hp = min(self._max_hp, value)
        if self._hp <= 0:
            raise GameOverException()

    @property
    def mana(self):
        return self._mana

    @mana.setter
    def mana(self, value):
        self._mana = min(self._max_mana, value)

    @property
    def strength(self):
        return self._strength

    @strength.setter
    def strength(self, value):
        self._strength = value
        self._update_stats()

    @property
    def dexterity(self):
        return self._dexterity

    @dexterity.setter
    def dexterity(self, value):
        self._dexterity = value
        self._update_stats()

    @property
    def intelligence(self):
        return self._intelligence

    @intelligence.setter
    def intelligence(self, value):
        self._intelligence = value
        self._update_stats()

    @property
    def charisma(self):
        return self._charisma

    @charisma.setter
    def charisma(self, value):
        self._charisma = value
        self._update_stats()
