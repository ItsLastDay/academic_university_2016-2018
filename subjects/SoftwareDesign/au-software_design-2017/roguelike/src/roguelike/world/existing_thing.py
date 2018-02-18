"""
Module contains base class for all objects that can reside on the map.
"""
import abc


class ExistingThing(metaclass=abc.ABCMeta):
    """An object that can be on a map"""

    def __init__(self, pos):
        """pos : Position"""
        self.pos = pos

    @abc.abstractmethod
    def get_visual_map_representation(self):
        pass
