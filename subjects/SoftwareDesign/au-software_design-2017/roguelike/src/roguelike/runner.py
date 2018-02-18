"""
Main module of the game. Can perform one tick of event loop.
"""
import logging
from .user_interface import UIMaster
from .controller import UserInteractor
from .game_operations import ControllerGameOperations


class GameRunner:
    """Initiator of all game objects, listens for user input.
    """

    def __init__(self, stdscr):
        game_ops = ControllerGameOperations()
        self.interactor = UserInteractor(game_ops)
        self.drawer = UIMaster(stdscr)
        self.stdscr = stdscr
        game_ops.reset_map()
        self.drawer.redraw()

    def listen_for_key_and_draw_interface(self):
        key = self.stdscr.getkey()
        logging.info('Key {} was pressed'.format(key))
        self.interactor.accept_key(key)
        self.drawer.redraw()
