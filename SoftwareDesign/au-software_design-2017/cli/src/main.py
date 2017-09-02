#! /usr/bin/env python3

import logging.config

from cli.shell import Shell


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
                'filename': 'shell_debug.log',
                'mode': 'w',
                'formatter': 'detailed',
            },
            'file_info': {
                'class': 'logging.FileHandler',
                'filename': 'shell_info.log',
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


if __name__ == '__main__':
    logging.config.dictConfig(log_config)
    shell = Shell()
    shell.main_loop()
