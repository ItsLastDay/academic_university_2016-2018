"""
Game-specific exceptions.
"""


class GameOverException(Exception):
    """Exception indicating that the game is over"""
    pass


class ExitFromMapException(Exception):
    """Exception indicating that hero exits this map"""
    pass

