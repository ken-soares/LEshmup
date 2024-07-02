# ENGINE DESIGN DOCUMENT 
__This document is not a GDD (Game Design Document)__
hence, no information about lore, characters and specific mechanics about a game will be given.
The aim of this document is to allow anyone to pick up the source code and hack together some scenes to make
a Shoot 'Em Up game easily.

## Introduction 
This engine is not a general use game engine, but a Shoot 'Em Up specific top layer
for Raylib to facilitate level design and creative workflow.

Since this engine is based on Raylib, support for Mac, Linux, PC, IOS, Android and Nintendo Switch
is theoretically already setup
__though the engine doesn't attempt to facilitate platform support for mobile and console by default__.

## Design decisions

This engine does not implement strict OOD (Object-Oriented Design)
based on polymorphism and multiple inheritance layers nor a complete version ECS system but rather
a case-specific mix of both.

To avoid deep levels of inheritance, the command pattern is not implemented. Each scene handles its inputs in
its own way without polymorphism.

No `Actor` Superclass is defined for entities as players, enemies and backgrounds behave in a completely unique manner
most of the time, a total separation of each of these classes is most likely more beneficial even in the unlikely event
of needing to rewrite a line multiple times.

Global variables have been defined in a C-like manner in a separate header file called `Globals.h`. These 
global variables include constant screen dimensions (16:9 720p) and level scroll speed, which
can be used in all of the scenes including menus and dialog scences.

...

### todo section of design decision and implementation
The `main` function, entry point of the program implements a scene switching system which does not
dissociates different scene types (dialog, menu, level). As such, inheritance will be used to define a
base `Scene` class which all classes will inherit from. The creation of `LevelScene`, `DialogScene`, `MenuScene` 
classes  is still to be determined.

## TODOs
### Scenes
- Scene switching system [ ]
- Main Menu Scene (only mouse input with coordinates except for fullscreen) [ ]
- Dialog Scenes (only mouse click input except for fullscreen) [ ]
    - implement `.dlg` dialog files reading [ ]
-  Level Scenes (keyboard input only) [ ]

### Player
- Player controller [x]
- Player class [x]
- Player shooting system [ ]
- (optional) Allow Player to have multiple shot types [ ]
### Enemies
- create map loading from CSV files [ ]
- Enemy shooting system and patterns [ ]
### Organisation
- move globals from main to separate header file [x]
- moving player stuff to separate class [x]
- moving enemy and enemy definition in a class [x]

## advancement
references:
- https://youtu.be/CqDfZEX0Yhc (timestamp: 1:31:11)

benchmark LOC with `(Get-ChildItem -Recurse -File | Get-Content | Measure-Object -line).Lines`

## Licence
This engine's licence is still to be determined to this day,
an open source licence may be a good choice if source code is made public.