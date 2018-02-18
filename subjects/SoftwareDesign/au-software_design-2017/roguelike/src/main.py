#!/usr/bin/env python3.6
import logging
import logging.config
from curses import wrapper

from roguelike.world.exceptions import GameOverException
from roguelike.runner import GameRunner

log_config = {
        'version': 1,
        'disable_existing_loggers': True,
        'formatters': {
            'detailed': {
                'class': 'logging.Formatter',
                'format': '%(asctime)s %(name)-15s %(levelname)-8s %(processName)-10s %(message)s'
            },
            'simple': {
                'class': 'logging.Formatter',
                'format': '%(name)-15s %(levelname)-8s %(processName)-10s %(message)s'
            }
        },
        'handlers': {
            'file_debug': {
                'class': 'logging.FileHandler',
                'filename': 'roguelike_debug.log',
                'mode': 'w',
                'formatter': 'detailed',
            },
            'file_info': {
                'class': 'logging.FileHandler',
                'filename': 'roguelike_info.log',
                'level': 'INFO',
                'mode': 'w',
                'formatter': 'simple',
            },
        },
        'root': {
            'level': 'DEBUG',
            'handlers': ['file_debug', 'file_info']
        },
    }


def main(stdscr):
    logging.config.dictConfig(log_config)
    game = GameRunner(stdscr)
    while True:
        game.listen_for_key_and_draw_interface()


if __name__ == '__main__':
    try:
        wrapper(main)
    except GameOverException:
        print('Game over')
