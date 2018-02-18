"""
Module that draws user interface for the game.

From the MVC point of view, this is the View.
"""
import curses
import abc
import logging

from .event_history import EventHistory
from .position import Position
from .world.game_settings import GameSettings
from .world.enumerations import TileType, BodyPlace
from .world.hero import Hero


class TextViewRectangle(metaclass=abc.ABCMeta):
    """Basic block of console UI"""

    def __init__(self, size_pos):
        """`size_pos` is a Position(x=size by x coordinate, y=size by y coordinate)"""
        self.size_pos = size_pos

    @abc.abstractmethod
    def get_subrectangle(self, upper_left_pos, size_pos):
        pass

    @abc.abstractmethod
    def place_char(self, x, y, char):
        pass

    def place_string(self, x, y, s):
        for i, ch in enumerate(s):
            self.place_char(x + i, y, ch)


class CursesTextViewRectangle(TextViewRectangle):
    """Concrete UI block using `curses` library"""

    def place_char(self, x, y, char):
        abs_y = self.upper_left_pos.y + y
        abs_x = self.upper_left_pos.x + x
        logging.debug('Trying to place char "{}" at ({}, {})'.format(char, abs_x, abs_y))
        max_y, max_x = self.win.getmaxyx()
        if y < max_y and x < max_x:
            self.win.addch(y, x, char)

    def place_string(self, x, y, s, need_underline=False):
        abs_y = self.upper_left_pos.y + y
        abs_x = self.upper_left_pos.x + x
        logging.debug('Trying to place string "{}" at ({}, {})'.format(s, abs_x, abs_y))
        max_y, max_x = self.win.getmaxyx()
        if x + len(s) >= max_x or y >= max_y:
            return
        if need_underline:
            self.win.addstr(y, x, s, curses.A_UNDERLINE)
        else:
            self.win.addstr(y, x, s)

    def __init__(self, size_pos, absolute_upper_left_pos, curses_win):
        super().__init__(size_pos)
        self.upper_left_pos = absolute_upper_left_pos
        self.win = curses_win

    def get_subrectangle(self, relative_upper_left_pos, size_pos):
        abs_upper_left = Position(self.upper_left_pos.x + relative_upper_left_pos.x,
                                  self.upper_left_pos.y + relative_upper_left_pos.y)
        win = self.win.subwin(size_pos.y, size_pos.x, relative_upper_left_pos.y, relative_upper_left_pos.x)
        logging.debug('Creating subrectangle ({}, {})-({}, {})'.format(abs_upper_left.x,
                                                                       abs_upper_left.y,
                                                                       abs_upper_left.x + size_pos.x,
                                                                       abs_upper_left.y + size_pos.y))
        return CursesTextViewRectangle(size_pos, abs_upper_left, win)


class UIMaster:
    """Class responsible (from the viewpoint of controller) for all UI actions"""

    def __init__(self, stdscr):
        self.main_window = MainInterfaceElement()
        self.stdscr = stdscr
        curses.curs_set(0)

    def redraw(self):
        curses.update_lines_cols()
        logging.debug('Viewport size is ({}, {})'.format(curses.COLS, curses.LINES))
        size_pos = Position(x=curses.COLS, y=curses.LINES)
        rect = CursesTextViewRectangle(size_pos, Position(x=0, y=0), self.stdscr)
        self.stdscr.clear()
        self.main_window.draw_on_rectangle(rect)
        self.stdscr.refresh()


class IUserInterfaceElement(metaclass=abc.ABCMeta):
    """Interface for all UI elements"""

    @abc.abstractmethod
    def draw_on_rectangle(self, rect):
        pass


class MainInterfaceElement(IUserInterfaceElement):
    """Main UI class, responsible for layout of all other windows

    Layout:

    HERO|MAP
    +INV|
    ENTO|______
    RY  |EVENTS
        |
    """

    def __init__(self):
        self.map_element = MapInterfaceElement()
        self.hero_element = HeroInterfaceElement()
        self.event_element = LogInterfaceElement()

    def draw_on_rectangle(self, rect):
        rect_size = rect.size_pos

        num_rows, num_cols = rect_size.y, rect_size.x

        COLS_FOR_HERO = 40
        ROWS_FOR_EVENTS = 20

        hero_cols, hero_rows = COLS_FOR_HERO, num_rows
        event_cols, event_rows = num_cols - hero_cols, ROWS_FOR_EVENTS
        map_cols, map_rows = num_cols - hero_cols, num_rows - event_rows

        rect_hero = rect.get_subrectangle(Position(x=0, y=0),
                                          Position(x=hero_cols, y=hero_rows))
        rect_map = rect.get_subrectangle(Position(x=hero_cols, y=0),
                                         Position(x=map_cols, y=map_rows))
        rect_event = rect.get_subrectangle(Position(x=hero_cols, y=map_rows),
                                           Position(x=event_cols, y=event_rows))

        self.hero_element.draw_on_rectangle(rect_hero)
        self.map_element.draw_on_rectangle(rect_map)
        self.event_element.draw_on_rectangle(rect_event)


class MapInterfaceElement(IUserInterfaceElement):
    """Class responsible for drawing a map (with hero, monsters and items)"""

    def draw_on_rectangle(self, rect):
        cur_map = GameSettings.current_map
        width, height = cur_map.dim_width, cur_map.dim_height

        # Draw border
        for i in range(height + 2):
            for j in range(width + 2):
                if i == 0 or j == 0 or i == height + 1 or j == width + 1:
                    rect.place_char(j, i, '#')
                    if i == height + 1 and j == cur_map.exit_column + 1:
                        rect.place_char(j, i, 'E')
                    continue

        all_tiles = cur_map.get_all_tiles()
        for tile in all_tiles:
            x, y = tile.pos.x, tile.pos.y
            char_to_place = None
            if tile.type == TileType.WALL:
                char_to_place = '#'
            elif tile.type == TileType.EMPTY:
                char_to_place = '.'
            elif tile.type == TileType.EXISTING_THING:
                char_to_place = tile.thing_ref.get_visual_map_representation()
            else:
                assert(False)

            logging.debug('UI map: tile {}, x={}, y={}'.format(tile, x, y))
            rect.place_char(1 + x, 1 + y, char_to_place)


class HeroInterfaceElement(IUserInterfaceElement):
    """Class responsible for drawing hero's attributes and inventory"""

    def draw_on_rectangle(self, rect):
        hero = Hero()
        inv = hero.inventory
        rect.place_string(0, 0, 'Hero-s attributes')
        rect.place_string(0, 1, 'Hp: {}/{}'.format(hero.hp, hero.max_hp))
        rect.place_string(0, 2, 'Mana: {}/{}'.format(hero.mana, hero.max_mana))
        rect.place_string(0, 3, 'Strength: {}'.format(hero.strength))
        rect.place_string(0, 4, 'Dexterity: {}'.format(hero.dexterity))
        rect.place_string(0, 5, 'Intelligence: {}'.format(hero.intelligence))
        rect.place_string(0, 6, 'Charisma: {}'.format(hero.charisma))
        rect.place_string(0, 7, 'Experience: {}'.format(hero.experience))

        worn_items_base = 9
        rect.place_string(0, worn_items_base, 'Worn items')
        rect.place_string(0, worn_items_base + 1, 'Head: {}'.format(inv.get_item_at_bodyplace(BodyPlace.HEAD)))
        rect.place_string(0, worn_items_base + 2, 'Left hand: {}'.format(inv.get_item_at_bodyplace(BodyPlace.LEFT_HAND)))
        rect.place_string(0, worn_items_base + 3, 'Right hand: {}'.format(inv.get_item_at_bodyplace(BodyPlace.RIGHT_HAND)))
        rect.place_string(0, worn_items_base + 4, 'Body: {}'.format(inv.get_item_at_bodyplace(BodyPlace.BODY)))
        rect.place_string(0, worn_items_base + 5, 'Legs: {}'.format(inv.get_item_at_bodyplace(BodyPlace.LEGS)))

        inventory_base_y = 16
        rect.place_string(0, inventory_base_y, 'Inventory')
        items_around = inv.get_all_items_around_selected(8)
        for i, item in enumerate(items_around):
            item, is_selected, idx = item
            rect.place_string(0, inventory_base_y + 1 + i, '{}. {}'.format(idx, item), is_selected)

        rect.place_string(0, inventory_base_y + 1 + len(items_around), '_' * 10)

class LogInterfaceElement(IUserInterfaceElement):
    """Class responsible for drawing event log"""

    def draw_on_rectangle(self, rect):
        last_events = EventHistory.get_last_n_events(15)

        for i, event in enumerate(last_events):
            event = '|' + event
            if len(event) >= rect.size_pos.x:
                event = event[:rect.size_pos.x - 3] + '...'
            rect.place_string(0, i, event)
