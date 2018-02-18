"""
Module that encapsulates possible game reactions to user in put.

From the MVC point of view, this is part of the Controller.
"""
import random
import logging
import abc

from .world.enumerations import TileType
from .world.monster import Monster
from .world import factories
from . import event_history
from .world.hero import Hero
from .event_history import EventHistory
from .world.game_settings import GameSettings


class IGameOperations(metaclass=abc.ABCMeta):

    @abc.abstractmethod
    def move_hero(self, direction):
        pass

    @abc.abstractmethod
    def use_selected_inventory_item(self):
        pass

    @abc.abstractmethod
    def change_selection_inventory(self, direction):
        pass

    @abc.abstractmethod
    def add_event_msg(self, event_description):
        pass

    @abc.abstractmethod
    def move_all_monsters(self):
        pass

    @abc.abstractmethod
    def reset_map(self):
        pass


class ControllerGameOperations(IGameOperations):
    
    def move_hero(self, direction):
        cur_map = GameSettings.current_map
        hero_tile = cur_map.get_hero_tile()
        cur_pos = hero_tile.pos
        if cur_map.check_feasible_move(cur_pos, direction):
            cur_map.move_thing(hero_tile, direction)
            self.move_all_monsters()
        else:
            logging.info('Hero could not move from {} in direction {}'.format(cur_pos, direction))

    def use_selected_inventory_item(self):
        try:
            item = Hero().inventory.get_selected_item()
            item.use()
        except IndexError:
            self.add_event_msg('Could not use item: empty inventory')

    def change_selection_inventory(self, direction):
        Hero().inventory.change_selection(direction)

    def add_event_msg(self, event_description):
        EventHistory.add_event(event_description)

    def move_all_monsters(self):
        map = GameSettings.current_map
        for tile in map.get_all_tiles():
            if (tile.type == TileType.EXISTING_THING and
                    isinstance(tile.thing_ref, Monster)):
                tile.thing_ref.make_turn(tile)

    def reset_map(self):
        GameSettings.current_map = factories.MapFactory.generate_random_map()
        event_history.EventHistory.add_event('Hero enters a new map')

        for i in range(100):#random.randint(7, 10)):
            factories.ItemFactory.generate_random_item()

        for i in range(random.randint(5, 10)):
            factories.MonsterFactory.generate_random_monster()

