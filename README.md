

## What is CrossSkelet?

CrossSkelet is a cross-platform fork of [Skeletux](https://github.com/LWSS/Skeletux). Now it supports GNU/Linux and Windows.

Skeletux is an experimental base for making your own CS:GO cheat on Linux.
It is mostly aimed towards developers.

---
## Features
* Supporting both Windows and Linux
* Fully dynamic runtime created Panorama GUI that does not touch any files and is even removed on unloading.
* Custom ConVar settings
* A few simple features (ESP, Bunnyhop)


#### Dependencies:
* A Compiler that supports C++17
* CMake  >= 2.8



#### How to Use
Build like any cmake project

##### Linux-based:
```bash
cmake . && make 

```

Afterwards you can inject with the load script
```bash
sudo ./load
```

##### Windows-based(using VS):

File->Open->CMake->CMakeLists.txt, than open CMakeLists file in VS and press ctrl+s, now you can build it like a standart windows project(CMake->Build all).
 
##### Windows-based(console):
TODO



You can open/close the Menu with `Insert`

## Screenshot

![menu](https://i.imgur.com/XtzVXti.jpg)


## Thanks

* [LWSS](https://github.com/LWSS) and [Fuzion](https://github.com/LWSS/Fuzion) contributors - [Skeletux](https://github.com/LWSS/Skeletux) and [Fuzion](https://github.com/LWSS/Fuzion) projects, also LWSS gives me some help in pm
* [McSwaggens](https://github.com/McSwaggens), [luk1337](https://github.com/luk1337), [AnterGhost](https://github.com/AnterGhost), [nvllvs](https://github.com/nvllvs) and another [AimTux](https://github.com/AimTuxOfficial) contributors.
* [Atex](https://www.unknowncheats.me/forum/members/602638.html) - linux basehook project
* [aixxe](https://github.com/aixxe) - cstrike-basehook-linux project
* [MarkHC](https://github.com/MarkHC) and CSGOSimple contributors - CSGOSimple project
* [vladislavik36](https://github.com/vladislavik36) - some help with CrossSkelet
* disabilitor - some gui ideas

## TODO

* Mac OS support

## Known Issues

* [ToggleUI] - Skeletux Panel not layed out yet. Try again.