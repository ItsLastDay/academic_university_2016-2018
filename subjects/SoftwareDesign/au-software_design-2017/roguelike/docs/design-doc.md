# Design document for Tropka: a simple rogue-like game


## Overview
*Tropka* is a single-player cross-platform computer game belonging to Rogue-like genre. Games of this genre have been played since the 1990x (maybe, even
earlier). Player is supposed to download the game, run it and survive (as a character) as long as he\she can. The are no multiplayer,
no save\load capabilities. User interface is console.

## Architectural drivers
Key points that affected Tropka's architecture:  
  - game graphics should be console, with possible upgrade to tile interface;  
  - game entities (e.g., monsters, items) should be extensible;  
  - user interaction must be extensible as well: adding new commands and new game features should be easy.


## Actors and use-cases  
I assume two types of users (*actors*):
  - **Hardcore games addict**. Let's call him Petya. Petya is 15-25 years old, not married. He is experienced in computer games, he've seen them all. What Petya misses is gameplay complexity coupled with interesting game world to explore. He is not afraid of installation or runtime issues. However, if the game is not challenging enough, he will throw it away quickly  
  Typical *use-case* for Petya (in form of a user story): "Right now I am tired of my favourite game X. 
I want to dive into a simple, yet deep game with unpredictable world and easy set-up";   
  - **Nostalgic game player**. Let's call him Ivan. Ivan is above 40 years old. He misses the good old times when the
  grass was greener and games were more interesting. He played the original Rogue back then. Now he seeks long-gone 
  emotions, playing various Rogue-like games. He does not spend much time on one game.  
  Typical user story for Ivan: "I have a free evening. I'm looking for a Roguelike game that is easy to start playing".
  
## Composition
The component structure is inspired by Model-View-Controller architectural pattern. On the whole, there are three large parts:
   - *Model*: all objects from the domain (roguelike game), as well as game rules;
   - *Controller*: entities that are repsonsible for interacting with user and with model. They have knowledge about 
   what can possibly happen in the game (e.g., about hero's death);
   - *View*: user-facing part. Knows how to represent data from the Model.  
   
Model should not know about View, but can possibly know about Controller. Controller directs how View and Model act. View need to know only about the Model.  

## Logical structure   
   - **The model**. Every aspect of the game can be extended to provide richer playing experience for users. This was desirable, because
   target audience seeks interesting gameplay. Model is represented as files in `./src/roguelike/world` folder. They include the following classes:
     - `GameSettings`: global configuration of the game (singleton);  
     - `ExistingThing`: base class for all objects that can possibly reside on a map (monsters, items, etc.);  
     - `Hero, Inventory`: describe current state of main character and its inventory. Since
     the hero is unique, those classes are effectively singleton;  
     - `Monster`: a single monster in the game. Each monster has `MonsterType` (this can potentially be transformed into
     class hierarchy of `Monster`-s subclasses);  
     - `AbstractItem` hierarchy (`WearableItem`, `InstantItem`, `Spell`): various types of items in the game. This
     ierarchy can be extended just like Monster-s;  
     - `___Factory`: factory classes for items and monsters (and their sublcasses). This can be extended to ierarchy
     of Factories;  
     - `Tile`: basic building block of map. Tiles can be turned into richer ierarchy, as above;  
     - `Map`: globally unique objects that knows about all tiles and how to manage them (e.g. who  can move on who).  
   - **The view**. The view is structured so that it can be manipulated seamlessly, adding new UI elements or changing the visual
   representation altogether. It resides in a single file `./src/roguelike/user_interface.py`. It consists of a handful of classes:
     - `UIMaster`: a facade for all UI operations;  
     - `TextViewRectangle`: basic abstraction of a part of a screen. It can be implemented in various ways, which leads to 
     extensibility of graphics;  
     - `IUserInterfaceElement`: interface for all UI elements. Elements know only about `TextViewRectangle` and about their 
     subelements. Thus, UI is modular is extensible;  
     - `MainInterfaceElement, MapInterfaceElement, HeroInterfaceElement, LogInterfaceElement`: concrete UI elements
     that represent various parts of the game. All elements are subelements of `MainInterfaceElement`. They can
     be further dissected;   
   - **The controller**. Controller is meant to be extensible. User interaction is simple and minimalistic, so that new players feel 
   comfortable after a couple of minutes. Controller may look complicated, but it is like that to allow extensions:
     - `IInputController`: interface of interaction with user. Multiple modes of interaction can
     exist. As of now, only `DungeonController` is implemented, which manipulates Hero through the dungeon;  
     - `UserInteractor` is repsonsible for choosing the appropriate InputController. When there is only
     one controller, this choice is obvious;  
     - `IGameOperations`: interface with all relevant actions in the game. As with InputController, currently
     there is only one implementation: `ControllerGameOperations`. It is the thing that accesses model and directs
     it;  
     - `GameRunner`: a class that instantiates all needed entities (controller, view, etc.) and can perform
     a single tick of the event loop.  
   
## Patterns  
*Factory* pattern is used to create game objects (items, monsters, map). Hero is represented as a *Singleton*.  
Only one `Map` object can be active at a time - you can say that it is *Singleton* too.  

## Algorithms  
Most algorithms in the implementation are straightforward. However, map generation turned out to be a tough task.  
Here is the outline of the algorithm:  

  0. Fix map dimensions, enter and exit cells.
  1. Randomly generate walls on a map (a certain percentage).
  2. While there exists a wall that separates two or more empty places from reaching each other: delete the wall. 
  Reachability is checked with [Disjoint Set Union](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) datastructure.  
  3. If on the resulting map the enter cell is not reachable from the exit cell, then go to step 1. Otherwise, the map is done.

Also, in current implementation, Map operations (like chechking the validity of a move) is done in the most straightforward way, without any optimizations. For current scale, this is not an issue.
  
  
## Interaction  
I am lazy to draw finite automaton diagrams, so the following text describes how certain objects interact:  
  - `GameRunner` is created once on start of the program. It creates `UIMaster` and `UserInteractor`, who in
  turn create needed descendant of `IGameOperations` and `IUserInterfaceElement`. These structures live
  throughout the game process, and interact with each other on user input;  
  - when each `Monster` is created, it knows where it stands on a map. It is bound to Map while it is alive.
  It takes commands to move from the Map. Once the monster has moved to the same tile as hero, it is asked
  to attack hero. Then, the monster dies;  
  - each `Item` is created with a position on a map. Item lies until either Monster steps on it (in that case,
  item perishes) or Hero steps on it (then it is transferred to Hero-s inventory). Once in inventory, this
  item can be issued `use` command - then it does what it should do on using (e.g. Spell performs its action and
  perishes);  
  - `Map` is generated randomly. It accepts commands from Controller. Once the hero enters the exit cell (or otherwise
  leaves the map), it is thrown away and new map is generated;  
  - `Tile`s are created and deleted on `Map`s wish.
