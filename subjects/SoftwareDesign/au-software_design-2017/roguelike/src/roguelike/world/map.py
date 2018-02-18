"""
Module related to map, on which all action takes place.
"""
import logging

from roguelike.world.tile import Tile
from .enumerations import TileType
from .items import AbstractItem
from .exceptions import ExitFromMapException
from .hero import Hero
from .monster import Monster
from ..event_history import EventHistory
from ..position import Position


class Map:
    """Map of dungeon, with monsters and items on it"""

    def __init__(self, tiles, dim_height, dim_width, exit_column):
        self.dim_width = dim_width
        self.tiles = tiles
        self.dim_height = dim_height
        self.exit_column = exit_column

    def get_all_tiles(self):
        return self.tiles

    def move_thing(self, tile_thing, shift_dir):
        # Assume that move is only called after `check_feasible_move`
        former_pos = tile_thing.pos
        new_pos = Position(tile_thing.pos.x + shift_dir.dx,
                           tile_thing.pos.y + shift_dir.dy)
        logging.debug('Moving {} from {} to {}'.format(tile_thing, former_pos, new_pos))
        empty_tile = self._get_empty_tile(new_pos)

        # Next comes collision resolution code. It can be refactored using
        # bigger class structure + some kind of visitor.

        # Hero steps out of dungeon
        if isinstance(tile_thing.thing_ref, Hero) and new_pos.x == self.exit_column\
                and new_pos.y == self.dim_height - 1:
            self.exit_from_map()

        # Monster steps on hero
        if isinstance(tile_thing.thing_ref, Monster) and self.get_hero_tile().pos == new_pos:
            tile_thing.thing_ref.attack_hero(Hero())
            self.remove_tile(tile_thing)

        # Monster steps on item
        if isinstance(tile_thing.thing_ref, Monster):
            tile_at = self._get_tile_at(new_pos)
            if tile_at is not None and isinstance(tile_at.thing_ref, AbstractItem):
                self.remove_tile(tile_at, True)

        # Monster steps on monster
        if isinstance(tile_thing.thing_ref, Monster):
            tile_at = self._get_tile_at(new_pos)
            if tile_at is not None and isinstance(tile_at.thing_ref, Monster):
                self.remove_tile(tile_at, True)

        # Hero steps on monster
        if isinstance(tile_thing.thing_ref, Hero):
            tile_at = self._get_tile_at(new_pos)
            if tile_at is not None and isinstance(tile_at.thing_ref, Monster):
                tile_at.thing_ref.attack_hero(Hero())
                self.remove_tile(tile_at, True)

        # Hero steps on item
        if isinstance(tile_thing.thing_ref, Hero):
            tile_at = self._get_tile_at(new_pos)
            if tile_at is not None and isinstance(tile_at.thing_ref, AbstractItem):
                Hero().inventory.add_item(tile_at.thing_ref)
                EventHistory.add_event('Item {} acquired'.format(tile_at.thing_ref))
                self.remove_tile(tile_at, True)

        tile_thing.pos = new_pos
        if empty_tile is not None:
            self.remove_tile(empty_tile)

        self.add_tile(Tile(TileType.EMPTY, former_pos))

    def check_feasible_move(self, pos, shift_dir):
        # If monster steps on item, it will disappear. Let it be a feature.
        new_position = Position(pos.x + shift_dir.dx, pos.y + shift_dir.dy)
        tile_at = self._get_tile_at(new_position)
        return (0 <= pos.x + shift_dir.dx < self.dim_width and
                0 <= pos.y + shift_dir.dy < self.dim_height and
                tile_at.type != TileType.WALL)

    def add_tile(self, tile):
        for prev_tile in self.tiles:
            if prev_tile.pos == tile.pos:
                if not (prev_tile.type == TileType.EMPTY):
                    logging.debug('Invariant violated: placing new tile above non-empty')
                    logging.debug('Trying to place {}'.format(tile))
                    logging.debug('Violated tile position: {}'.format(tile.pos))
                    logging.debug('Tiles state: {}'.format('\n'.join(map(str, sorted(self.tiles, key=lambda x: x.pos)))))
                    assert(False)
                self.remove_tile(prev_tile)
                break
        self.tiles.append(tile)

    def remove_tile(self, tile, without_empty=False):
        self.tiles.remove(tile)
        if not without_empty and self._has_nothing_on_tile(tile.pos) and tile.type != TileType.EMPTY:
            self.add_tile(Tile(TileType.EMPTY, tile.pos))

    def exit_from_map(self):
        raise ExitFromMapException()

    def is_empty_tile(self, pos):
        return self._get_empty_tile(pos) is not None

    def _get_tile_at(self, pos):
        for tile in self.tiles:
            if tile.pos == pos:
                return tile

        return None

    def _get_empty_tile(self, pos):
        for tile in self.tiles:
            if tile.type == TileType.EMPTY:
                if tile.pos == pos:
                    return tile

        return None

    def get_hero_tile(self):
        for tile in self.tiles:
            if tile.type == TileType.EXISTING_THING and isinstance(tile.thing_ref, Hero):
                return tile

        return None

    def _has_nothing_on_tile(self, pos):
        for tile in self.tiles:
            if tile.pos == pos:
                return False

        return True
