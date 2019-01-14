# :mortar_board: CPSC 453 - Introduction to Computer Graphics - Fall 2018 - Assignment 5 - Real-Time Rendering

* Slightly modified README from 'final version'
* 20/20 + Bonus(+2)

[Evan Quan](https://github.com/EvanQuan)

[SeungBin Yim](https://github.com/yimsbean)

![](screenshot.png)

## About

Operating System: Fedora 28 (x86-64)

Compiler: clang++ 6.0.1

Used template code provided on D2L.

All textures were retrieved [here](https://www.solarsystemscope.com/textures/)

Deimos and phobos texture retrieved [here](http://planetpixelemporium.com/mars.html)

## Compiling and Running

Execute the `run.sh` file to compile and run the program.
```bash
$ bash run.sh
```
Alternatively, execute the `makefile` to compile,
```bash
$ make
```
and run the program with:
```bash
$ ./Boilerplate.out
```

## Controls

Press **Esc** to exit the program.

Click and drag the **left mouse button** to move the camera around.

Scroll **up** to zoom out.

Scroll **down** to zoom in.

Press **right arrow** to speed up the animation.

Press **left arrow** to slow down the animation.

Press **p** to toggle pausing and un-pausing.

Press **s** to reset the scene. All the planet begin lined up.

Press **c** to reset the camera. All the planets continue moving as they were.

## Bonus Features

- Extra planets
    - Mercury
    - Venus
    - Jupiter
    - Saturn
    - Uranus
    - Neptune
- Extra dwarf planets
    - Pluto (JUSTICE FOR PLUTO)
    - Ceres
    - Haumea
    - Makemake
    - Eris
- Mars has 2 moons, Deimos and Phobos
- The Earth has animated cloud textures that move over the Earth's atmosphere.
- Venus has animated atmosphere textures that move over Venus
- Ability to slow down, speed up, and reverse time.
- Ability to change camera focus on different planets.

| Focus     | Key |
|:---------:|:---:|
| Sun       | `   |
| Mercury   | 1   |
| Venus     | 2   |
| Earth     | 3   |
| Mars      | 4   |
| Ceres     | 5   |
| Jupiter   | 6   |
| Saturn    | 7   |
| Uranus    | 8   |
| Neptune   | 9   |
| Pluto     | 0   |
| Haumea    | -   |
| Makemake  | =   |
| Eris      | Backspace |
