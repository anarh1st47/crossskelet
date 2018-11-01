

## What is CrossSkelet?

CrossSkelet is a cross-platform fork of Skeletux. Now it supports GNU/Linux and Windows.

Skeletux is an experimental base for making your own CS:GO cheat on Linux.

It is mostly aimed towards developers.


## Features
* Fully dynamic runtime created Panorama GUI that does not touch any files and is even removed on unloading.
* Custom ConVar settings
* A few simple features (ESP, Bunnyhop)


#### Dependencies:
* A Compiler that supports C++17
* CMake  >= 2.8

===================

#### How to Use

Build like any cmake project

Linux-based:
```bash
cmake . && make 

```

Afterwards you can inject with the load script
```bash
sudo ./load
```

Windows-based(using VS):

File->Open->CMake->CMakeLists.txt, than open CMakeLists file in VS and press ctrl+s, now you can build it like a standart windows project(CMake->Build all).
 
Windows-based(console):
TODO



You can open/close the Menu with `Insert`

## Screenshots

This looks like garbage lol
![menu](https://i.imgur.com/lATYWiH.jpg)


## Thanks
LWSS and Fuzion contributors - Skeletux and Fuzion projects
McSwaggens, luk1337, AnterGhost, nvllvs, aixxe and another AimTux contributors.
Atex - linux basehook project
MarkHC and CSGOSimple contributors - CSGOSimple project
vladislavik36 - some help with CrossSkelet

## TODO

(todo before release)
Merge pattern scanners
Verify usercmd
Menu redisign
Refactoring

(todo after release)
Mac OS support