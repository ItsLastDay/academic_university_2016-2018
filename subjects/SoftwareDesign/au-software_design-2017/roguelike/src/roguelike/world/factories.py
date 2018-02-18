"""
Module consists of Factory classes, that build game objects.
"""
import logging
import random

from .items import InstantItem, Spell, WearableItem
from .monster import MonsterType, Monster
from .game_settings import GameSettings
from .enumerations import TileType, InstantItemType, BodyPlace, SpellType
from .map import Map
from .hero import Hero
from roguelike.world.tile import Tile
from ..position import Position


def get_free_position():
    cur_map = GameSettings.current_map

    all_free_positions = []
    for tile in cur_map.get_all_tiles():
        if tile.type == TileType.EMPTY:
            all_free_positions.append(tile.pos)

    if all_free_positions:
        return random.choice(all_free_positions)

    return None


class MonsterFactory:
    """Creates monsters and places them on current map"""

    @staticmethod
    def generate_random_monster():
        pos = get_free_position()
        if pos is None:
            logging.info('No place for new monster')
            return
        monster_type = random.choice([MonsterType.ZOMBIE, MonsterType.DRAGON, MonsterType.FLY])
        attack_low = Hero().experience // 1000
        attack_high = max(attack_low, Hero().max_hp)
        attack = random.randint(attack_low, attack_high)
        monster = Monster(monster_type, attack, 3, pos)
        GameSettings.current_map.add_tile(Tile(TileType.EXISTING_THING, None, monster))
        logging.info('New monster "{}" generated on position {}'.format(monster, pos))
        return monster


class ItemFactory:
    """Creates items and places them on current map"""

    @staticmethod
    def generate_random_item():
        pos = get_free_position()
        if pos is None:
            logging.info('No place for new item')
            return

        item = None
        if random.randint(1, 100) <= 60:
            item_type = random.choice([InstantItemType.HEAL_POTION, InstantItemType.MANA_POTION])
            item = InstantItem(item_type, pos)
        else:
            if random.randint(1, 100) <= 60:
                item_type = random.choice(list(BodyPlace))
                str_inc = random.randint(1, 4 + Hero().experience // 1000)
                dex_inc = random.randint(1, 4 + Hero().experience // 1000)
                int_inc = random.randint(1, 4 + Hero().experience // 1000)
                char_inc = random.randint(1, 4 + Hero().experience // 1000)
                item = WearableItem(item_type, str_inc, dex_inc, int_inc, char_inc, pos)
            else:
                item_type = random.choice([SpellType.HEAL, SpellType.TELEPORT])
                item = Spell(item_type, pos)
        GameSettings.current_map.add_tile(Tile(TileType.EXISTING_THING, None, item))
        logging.info('New item "{}" generated on position {}'.format(item, pos))
        return item


class MapFactory:
    """Creates maps"""

    @staticmethod
    def place_hero(enter_column, tiles):
        for tile in tiles:
            if tile.pos == Position(y=0, x=enter_column):
                tile.type = TileType.EXISTING_THING
                tile.pos = None
                tile.thing_ref = Hero()
                Hero().pos = Position(y=0, x=enter_column)
                logging.info('Hero placed at {}'.format(tile.pos))

    @staticmethod
    def generate_tiles(height, width, enter_col, exit_col):
        is_free = [[False for j in range(width)] for i in range(height)]
        is_free[0][enter_col] = True
        is_free[height - 1][exit_col] = True

        for i in range(height):
            for j in range(width):
                if not is_free[i][j]:
                    if random.randint(1, 100) <= 20:
                        is_free[i][j] = True

        par = [i for i in range(width * height)]

        def get_num_form_pair(y, x):
            return y * width + x

        def get_par(idx):
            if par[idx] == idx:
                return idx
            par[idx] = get_par(par[idx])
            return par[idx]

        def merge(p1, p2):
            logging.debug('DSU: merge {} and {}'.format(p1, p2))
            par1 = get_par(get_num_form_pair(*p1))
            par2 = get_par(get_num_form_pair(*p2))
            par[par1] = par2

        possible_shifts = ((-1, 0), (1, 0), (0, -1), (0, 1))
        for i in range(height):
            for j in range(width):
                if not is_free[i][j]:
                    continue

                for dx, dy in possible_shifts:
                    y = i + dy
                    x = j + dx
                    if 0 <= x < width and 0 <= y < height and is_free[y][x]:
                        merge((i, j), (y, x))

        any_changes = True
        while any_changes:
            any_changes = False
            nonfree_pairs = []
            for i in range(height):
                for j in range(width):
                    if not is_free[i][j]:
                        nonfree_pairs.append((i, j))
            logging.debug('Map generator: nonfree pairs {}'.format(nonfree_pairs))
            random.shuffle(nonfree_pairs)

            for p in nonfree_pairs:
                neighbouring_classes = []
                for dx, dy in possible_shifts:
                    y = p[0] + dy
                    x = p[1] + dx
                    if 0 <= x < width and 0 <= y < height and is_free[y][x]:
                        neighbouring_classes.append(get_par(get_num_form_pair(y, x)))

                if len(set(neighbouring_classes)) != 1:
                    is_free[p[0]][p[1]] = True
                    logging.debug('DSU: erasing wall at {}'.format(p))
                    any_changes = True
                    for dx, dy in possible_shifts:
                        y = p[0] + dy
                        x = p[1] + dx
                        if 0 <= x < width and 0 <= y < height and is_free[y][x]:
                            merge((y, x), p)

        for i in range(width * height):
            get_par(i)

        def split_by_width(par, width):
            for i in range(0, len(par), width):
                yield ' '.join(map(str, par[i:i+width]))

        free_arr_repr = '\n'.join(map(lambda x: ' '.join(map(lambda y: str(int(y)), x)), is_free))
        logging.debug('"Free" array from map initialization: {}'.format(free_arr_repr))
        par_arr_repr = '\n'.join(split_by_width(par, width))
        logging.debug('"Parents" array from map initialization: {}'.format(par_arr_repr))

        tiles = []
        for i in range(height):
            for j in range(width):
                tiles.append(Tile(TileType.EMPTY if is_free[i][j] else TileType.WALL,
                                  Position(y=i, x=j)))

        if get_par(get_num_form_pair(0, enter_col)) != get_par(get_num_form_pair(height - 1, exit_col)):
            return MapFactory.generate_tiles(height, width, enter_col, exit_col)

        return tiles

    @staticmethod
    def generate_random_map():
        dim_width = random.randint(20, 65)
        dim_height = random.randint(15, 20)
        exit_column = random.randint(0, dim_width - 1)
        enter_column = random.randint(0, dim_width - 1)
        tiles = MapFactory.generate_tiles(dim_height, dim_width, enter_column, exit_column)

        MapFactory.place_hero(enter_column, tiles)
        logging.info('New map of size ({}, {}) generated'.format(dim_height, dim_width))
        logging.info('Enter column: {}'.format(enter_column))
        logging.debug('Map tiles: {}'.format(tiles))
        logging.debug('Hero position: {}'.format(Hero().pos))
        return Map(tiles, dim_height, dim_width, exit_column)
