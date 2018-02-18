"""
Module contains classes for various items that Hero can use.
"""
import abc

from . import hero as _Hero
from . import game_settings
from .enumerations import InstantItemType, SpellType, BodyPlace
from .existing_thing import ExistingThing
from ..event_history import EventHistory
from .game_settings import GameSettings


class AbstractItem(ExistingThing, metaclass=abc.ABCMeta):
    """Abstract class for item

    Item can be `use`-d. This can affect Hero instance
    or have global effects (e.g. "Teleport" spell triggering
    a map change).
    """

    @abc.abstractmethod
    def use(self):
        pass


class Spell(AbstractItem):
    """Item that can be "casted" using Hero-s mana"""

    def __init__(self, tp, pos):
        super().__init__(pos)
        self.type = tp

    def use(self):
        """If we have sufficient mana, cast this spell and remove it from inventory

        Everything is hardcoded in this method (manacost, heal value, ...).
        """
        hero = _Hero.Hero()
        if self.type == SpellType.HEAL:
            if hero.mana < GameSettings.HEAL_SCROLL_MANACOST:
                EventHistory.add_event("Not enough mana to use healing spell")
                return
            hero.mana -= GameSettings.HEAL_SCROLL_MANACOST
            hero.hp += GameSettings.HEAL_SCROLL_AMOUNT
            hero.inventory.remove_item(self)
            EventHistory.add_event('Used health spell')
        elif self.type == SpellType.TELEPORT:
            # Teleport straight to the exit, effectively starting a new map
            if hero.mana < GameSettings.TELEPORT_SCROLL_MANACOST:
                EventHistory.add_event("Not enough mana to use teleport spell")
                return
            hero.mana -= GameSettings.TELEPORT_SCROLL_MANACOST
            hero.inventory.remove_item(self)
            game_settings.GameSettings.current_map.exit_from_map()
            EventHistory.add_event('Used teleport spell')
        else:
            assert(False)

    def get_visual_map_representation(self):
        if self.type == SpellType.HEAL:
            return 'e'
        elif self.type == SpellType.TELEPORT:
            return 't'
        else:
            assert(False)

    def __str__(self):
        return 'Spell({})'.format(self.type.name)


class InstantItem(AbstractItem):
    """Item that can be consumed unconditionally"""

    def get_visual_map_representation(self):
        if self.type == InstantItemType.HEAL_POTION:
            return 'h'
        elif self.type == InstantItemType.MANA_POTION:
            return 'm'
        else:
            assert(False)

    def __init__(self, tp, pos):
        super().__init__(pos)
        self.type = tp

    def use(self):
        """Consume item, get it's benefits"""
        hero = _Hero.Hero()
        if self.type == InstantItemType.HEAL_POTION:
            hero.hp += GameSettings.HEAL_POTION_AMOUNT
            EventHistory.add_event('Used health potion')
        elif self.type == InstantItemType.MANA_POTION:
            hero.mana += GameSettings.MANA_POTION_AMOUNT
            EventHistory.add_event('Used mana potion')
        else:
            assert(False)
        hero.inventory.remove_item(self)

    def __str__(self):
        return 'InstantItem({})'.format(self.type.name)


class WearableItem(AbstractItem):
    """Item that can be worn in one of the BodyPlace-s

    Items have no requirements and give only bonuses.
    """

    def get_visual_map_representation(self):
        if self.body_place == BodyPlace.BODY:
            return 'B'
        elif self.body_place == BodyPlace.HEAD:
            return 'H'
        elif self.body_place == BodyPlace.LEGS:
            return 'F'
        elif self.body_place == BodyPlace.LEFT_HAND:
            return 'L'
        elif self.body_place == BodyPlace.RIGHT_HAND:
            return 'R'
        else:
            assert(False)

    def __init__(self, body_place,
                 strength_increase, dexterity_increase,
                 intelligence_increase, charisma_increase,
                 pos):
        super().__init__(pos)
        self.charisma_increase = charisma_increase
        self.intelligence_increase = intelligence_increase
        self.dexterity_increase = dexterity_increase
        self.strength_increase = strength_increase
        self.body_place = body_place

    def use(self):
        """Wear this item, possibly replacing what was worn in the target body place

        This method updates all characteristics, so that Inventory knows
        nothing about them.
        """
        hero = _Hero.Hero()
        inventory = hero.inventory

        cur_worn = inventory.get_item_at_bodyplace(self.body_place)
        if cur_worn is not None:
            hero.charisma -= cur_worn.charisma_increase
            hero.intelligence -= cur_worn.intelligence_increase
            hero.dexterity -= cur_worn.dexterity_increase
            hero.strength -= cur_worn.strength_increase

        inventory.wear_item(self.body_place, self)
        hero.charisma += self.charisma_increase
        hero.intelligence += self.intelligence_increase
        hero.dexterity += self.dexterity_increase
        hero.strength += self.strength_increase

        EventHistory.add_event('Now wearing {} at {}'.format(str(self), self.body_place.name))

    def __str__(self):
        return 'WearableItem({})'.format(self.body_place.name)

