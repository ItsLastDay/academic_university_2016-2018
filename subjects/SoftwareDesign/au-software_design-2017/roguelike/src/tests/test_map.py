import unittest

from roguelike.world.exceptions import ExitFromMapException
from roguelike.world.map import Map
from roguelike.world.tile import Tile
from roguelike.world.items import InstantItemType, InstantItem
from roguelike.world.monster import Monster
from roguelike.world.enumerations import TileType, MonsterType
from roguelike.position import Position, ShiftDirection
from roguelike.world.hero import Hero


class MapTest(unittest.TestCase):
    """Tests for Map functionality"""

    def setUp(self):
        Hero().pos = Position(0, 0)
        Hero().inventory.items = []
        tiles = [
            Tile(TileType.EXISTING_THING, None, Hero()),
            Tile(TileType.EMPTY, Position(1, 0)),
            Tile(TileType.WALL, Position(2, 0)),
            Tile(TileType.EXISTING_THING, None, InstantItem(InstantItemType.HEAL_POTION, Position(3, 0))),
            Tile(TileType.EMPTY, Position(0, 1)),
            Tile(TileType.EMPTY, Position(1, 1)),
            Tile(TileType.EMPTY, Position(3, 1)),
            Tile(TileType.EXISTING_THING, None,
                 Monster(MonsterType.ZOMBIE, 2, 0, Position(2, 1))),
        ]
        self.map = Map(tiles, 2, 4, 0)

    def test_hero_steps_on_item(self):
        hero_tile = self.map.get_hero_tile()
        self.map.move_thing(hero_tile, ShiftDirection(dx=3, dy=0))
        self.assertEqual(1, len(Hero().inventory.items))
        self.assertEqual(self.map.dim_width * self.map.dim_height,
                         len(self.map.get_all_tiles()))
        self.map.move_thing(hero_tile, ShiftDirection(dx=-3, dy=0))
        self.assertEqual(self.map.dim_width * self.map.dim_height,
                         len(self.map.get_all_tiles()))

    def test_monster_steps_on_item(self):
        zombie = self.map.tiles[-1]
        self.map.move_thing(zombie, ShiftDirection(dx=1, dy=-1))
        self.assertEqual(self.map.dim_width * self.map.dim_height,
                         len(self.map.get_all_tiles()))
        self.assertEqual(0, len(Hero().inventory.items))
        self.map.move_thing(zombie, ShiftDirection(dx=-1, dy=1))
        self.assertTrue(self.map.is_empty_tile(Position(3, 0)))

    def test_add_tile_above_empty(self):
        # We have ability to lay tiles above EMPTY ones. Map should remove them.
        self.map.add_tile(Tile(TileType.WALL, Position(1, 0)))
        self.assertIsNone(self.map._get_empty_tile(Position(1, 0)))
        self.assertEqual(self.map.dim_width * self.map.dim_height,
                         len(self.map.get_all_tiles()))

    def test_move_thing(self):
        zombie = self.map.tiles[-1]
        self.map.move_thing(zombie, ShiftDirection(dx=-1, dy=0))
        self.assertEqual(Position(1, 1), zombie.pos)

        has_empty_in_former_place = False
        for tile in self.map.tiles:
            if tile.type == TileType.EMPTY:
                self.assertNotEqual(Position(1, 1), tile.pos)
                has_empty_in_former_place |= tile.pos == Position(2, 1)

        self.assertTrue(has_empty_in_former_place)
        self.assertEqual(self.map.dim_height * self.map.dim_width,
                         len(self.map.get_all_tiles()))

    def test_check_feasible_move(self):
        self.assertTrue(self.map.check_feasible_move(Position(0, 0),
                                                     ShiftDirection(dx=0, dy=1)))
        self.assertFalse(self.map.check_feasible_move(Position(1, 0),
                                                      ShiftDirection(dx=1, dy=0)))
        # Hero can step on monsters, and vice-versa
        self.assertTrue(self.map.check_feasible_move(Position(0, 0),
                                                     ShiftDirection(dx=2, dy=1)))
        self.assertTrue(self.map.check_feasible_move(Position(2, 1),
                                                     ShiftDirection(dx=-2, dy=-1)))

    def test_is_empty(self):
        self.assertTrue(self.map.is_empty_tile(Position(1, 0)))
        self.assertFalse(self.map.is_empty_tile(Position(0, 0)))

    def test_get_hero(self):
        self.assertIsNotNone(self.map.get_hero_tile())

    def test_remove_tile_empty(self):
        self.map.remove_tile(self.map.tiles[1])
        self.assertEqual(self.map.dim_height * self.map.dim_width - 1,
                         len(self.map.get_all_tiles()))
        self.assertFalse(self.map.is_empty_tile(Position(1, 0)))
        self.assertTrue(self.map._has_nothing_on_tile(Position(1, 0)))

    def test_remove_tile_nonempty(self):
        self.map.remove_tile(self.map.tiles[-1])
        self.assertEqual(self.map.dim_height * self.map.dim_width,
                         len(self.map.get_all_tiles()))
        self.assertTrue(self.map.is_empty_tile(Position(2, 1)))
        self.assertFalse(self.map._has_nothing_on_tile(Position(2, 1)))

    def test_hero_exits(self):
        with self.assertRaises(ExitFromMapException):
            self.map.move_thing(self.map.get_hero_tile(), ShiftDirection(dx=0, dy=1))

    def test_monster_attacks_hero_and_perishes(self):
        zombie = self.map.tiles[-1]
        self.map.move_thing(zombie, ShiftDirection(dx=-1, dy=0))
        self.map.move_thing(zombie, ShiftDirection(dx=-1, dy=0))
        self.map.move_thing(zombie, ShiftDirection(dx=0, dy=-1))
        self.assertEqual(self.map.dim_width * self.map.dim_height,
                         len(self.map.get_all_tiles()))
        self.assertEqual(5, len(list(filter(lambda tile: tile.type == TileType.EMPTY,
                                   self.map.get_all_tiles()))))

