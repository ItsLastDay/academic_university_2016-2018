"""
Module that interacts with user and interprets input.

From the MVC point of view, this is part of Controller.
"""
import random
import abc

from .position import ShiftDirection, ShiftDirection1d
from .world.exceptions import ExitFromMapException


class IInputController(metaclass=abc.ABCMeta):
    """Interface for all interactions with user.
    """

    @abc.abstractmethod
    def accept_key(self, key_pressed):
        pass


class DungeonController(IInputController):
    """Implementation of interacting with user in dungeon.
    """

    def __init__(self, game_operations):
        self.game_ops = game_operations

    def accept_key(self, key_pressed):
        try:
            if key_pressed == 'KEY_LEFT':
                shift = ShiftDirection(dx=-1, dy=0)
                self.game_ops.move_hero(shift)
            elif key_pressed == 'KEY_RIGHT':
                shift = ShiftDirection(dx=1, dy=0)
                self.game_ops.move_hero(shift)
            elif key_pressed == 'KEY_DOWN':
                shift = ShiftDirection(dx=0, dy=1)
                self.game_ops.move_hero(shift)
            elif key_pressed == 'KEY_UP':
                shift = ShiftDirection(dx=0, dy=-1)
                self.game_ops.move_hero(shift)
            elif key_pressed == 'w':
                self.game_ops.use_selected_inventory_item()
            elif key_pressed == 'q':
                self.game_ops.change_selection_inventory(ShiftDirection1d(-1))
            elif key_pressed == 'a':
                self.game_ops.change_selection_inventory(ShiftDirection1d(1))
            elif key_pressed == 'p':
                self.game_ops.move_all_monsters()
            elif key_pressed == 'h':
                phrases = [
                    'Hello, world!',
                    'Is anybody here?',
                    'I want treasure!'
                ]
                self.game_ops.add_event_msg('Hero shouts: "{}"'.format(random.choice(phrases)))
            else:
                self.game_ops.add_event_msg('Hero thinks about existence while pressing {}'.format(key_pressed))
        except ExitFromMapException:
            self.game_ops.reset_map()


class UserInteractor:
    """A class responsible (from GameRunner's point of view) for user interaction.
    """

    def __init__(self, game_operations):
        self.delegate_interactor = DungeonController(game_operations)

    def accept_key(self, key_pressed):
        self.delegate_interactor.accept_key(key_pressed)
