import unittest

from roguelike.world.exceptions import ExitFromMapException
from roguelike.position import Position
from roguelike.world.hero import Hero
from roguelike.world.items import *
from roguelike.world.game_settings import GameSettings


class MapMock:
    @staticmethod
    def exit_from_map():
        raise ExitFromMapException

GameSettings.current_map = MapMock

class ItemTest(unittest.TestCase):
    """Tests for items functionality"""

    def setUp(self):
        Hero().hp = 1
        Hero().mana = 100
        Hero().inventory.items = []
        Hero().inventory.item_by_bodyplace = dict()

    def test_use_heal_potion(self):
        heal = InstantItem(InstantItemType.HEAL_POTION, Position(0, 0))
        cur_hp = Hero().hp
        heal.use()
        self.assertEqual(cur_hp + GameSettings.HEAL_POTION_AMOUNT, Hero().hp)

    def test_use_mana_potion(self):
        mana = InstantItem(InstantItemType.MANA_POTION, Position(0, 0))
        cur_mana = Hero().mana
        mana.use()
        self.assertEqual(cur_mana + GameSettings.MANA_POTION_AMOUNT, Hero().mana)

    def test_use_heal_scroll(self):
        heal = Spell(SpellType.HEAL, Position(0, 0))
        cur_hp = Hero().hp
        heal.use()
        self.assertEqual(cur_hp + GameSettings.HEAL_SCROLL_AMOUNT, Hero().hp)

    def test_use_teleport_scroll(self):
        tp = Spell(SpellType.TELEPORT, Position(0, 0))
        with self.assertRaises(ExitFromMapException):
            tp.use()

    def test_wear_item(self):
        head = WearableItem(BodyPlace.HEAD, 1, 0, 0, 0, 0)
        cur_str = Hero().strength
        head.use()
        inv = Hero().inventory
        self.assertEqual(0, len(inv.items))
        self.assertIsNotNone(inv.get_item_at_bodyplace(BodyPlace.HEAD))
        self.assertEqual(cur_str + 1, Hero().strength)

    def test_wear_item_then_other(self):
        inv = Hero().inventory
        head = WearableItem(BodyPlace.HEAD, 1, 0, 0, 0, 0)
        cur_str = Hero().strength
        cur_dex = Hero().dexterity
        head.use()
        head = WearableItem(BodyPlace.HEAD, 0, 1, 0, 0, 0)
        head.use()
        self.assertEqual(1, len(inv.items))
        self.assertIsNotNone(inv.get_item_at_bodyplace(BodyPlace.HEAD))
        self.assertEqual(cur_str, Hero().strength)
        self.assertEqual(cur_dex + 1, Hero().dexterity)
