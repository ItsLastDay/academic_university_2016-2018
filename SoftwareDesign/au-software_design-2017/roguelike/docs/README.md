# Documentation for Tropka: a simple Rogue-like game

Table of Contents
=================

   * [Documentation for Rogue-like game](#documentation-for-rogue-like-game)  
      * [What is this project about?](#what-is-this-project-about)  
      * [How to run](#how-to-run)  
      * [Gameplay](#gameplay)  
         * [Keyboard controls](#keyboard-controls)  
         * [Attributes](#attributes)  
         * [Monsters](#monsters)  
         * [Items](#items)  
            * [Consumable items](#consumable-items)  
            * [Spells](#spells)  
            * [Wearable items](#wearable-items)  
         * [Inventory](#inventory)  

Table of contents created using [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)  

## What is this project about?
Many years ago, the game titled Rogue started a new genre of computer games - [Rogue-like](https://en.wikipedia.org/wiki/Roguelike).  
This project, named **Tropka**, implements a simple game in this genre. You control the Hero (denoted as `@`). You are supposed to navigate through the dungeon, fight monsters, collect items and have fun.

User interface is console. Here is a screenshot of gameplay:
![Screenshot](https://github.com/ItsLastDay/au-software_design-2017/blob/roguelike/roguelike/docs/basic_screenshot.png)

## How to run
Just run (from the `src/` folder)
```
python3.6 main.py
```
No additional dependencies needed, but make sure you have Python**3.6**.  
Tested on Ubuntu 16.04, but it should work on Winodws as well.


## Gameplay  
As a hero, you navigate in a dungeon full of monsters and items. The greed for more power and experience is endless.  
When you have gotten everything from one dungeon (or, on the contrary, you need to run from mighty monsters ASAP), you can move to a cell next to letter `E` on the bottom of the map. This will take you to another randomly-generated map.  
When the hero steps on an item, it is added to the inventory.  
If a monster steps on the hero (or vice-versa), that monster attacks hero (inflicting damage) and perishes.

### Keyboard controls
`(left | right | up | down) arrow` - move hero  
`p` - move monsters (without moving hero)  
`q` - change inventory selection one point above  
`a` - change inventory selection one point below  
`w` - use currently selected item  
`h` - shout something  

### Attributes
`strength` - affects maximum hitpoints   
`intelligence` - affects maximum mana  
`dexterity` - affects armor (monster attack reduction)  
`charisma` - affects chance that monster is charmed by you and won't attack

### Monsters
`zombie` - represented as `^`  
`dragon` - represented as `!`  
`fly` - represented as `<`  

### Items  
#### Consumable items  
`health potion` - represented as `h`. Heals for certain amount  
`mana potion` - represented as `m`. Replenishes mana for certain amount  
#### Spells  
`teleport` - represented as `t`. Instantly takes Hero to the next dungeon, has manacost  
`heal` - represented as `e`. Heals for certain amount, has manacost  
#### Wearable items  
Hero has several slots for items (below are pairs "`type` - visual representation for items of this type"):   
`Body` - `B`  
`Head` - `H`  
`Left hand` - `L`  
`Right hand` - `R`  
`Legs` - `F`  
Items on these slots are interchangable: e.g., any item of type `B` can be worn on body, but only at most one at a time. Items add characteristics to hero. If you issue a command to wear some item, and the slot is already filled with another item, that item is moved to the inventory.

### Inventory
Inventory has infinite capacity. Every moment you see only a portion of inventory, which is near the current selected item (the underlined one).
