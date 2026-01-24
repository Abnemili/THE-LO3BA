# Cub3D

A simple raycasting engine inspired by Wolfenstein 3D, built as part of the 42/1337 curriculum.

## About

This project is a 3D maze game using raycasting techniques. Navigate through textured walls in first-person view using basic movement and rotation controls.

## Features

- Real-time 3D rendering using raycasting
- Textured walls (North, South, East, West)
- Player movement (WASD) and rotation (arrow keys)
- Configurable maps with .cub files
- Custom floor and ceiling colors

## Requirements

- MinilibX library
- GCC compiler
- Linux/Unix environment

## Installation

```bash
git clone <your-repo>
cd cub3d
make
```

## Usage

```bash
./cub3D maps/map.cub
```

### Controls

- `W` - Move forward
- `S` - Move backward
- `A` - Strafe left
- `D` - Strafe right
- `←/→` - Rotate camera
- `ESC` - Exit game

## Map Configuration

Create a `.cub` file with the following format:

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm

F 220,100,0
C 225,30,0

111111
100001
101N01
100001
111111
```

- `NO/SO/WE/EA` - Texture paths
- `F/C` - Floor/Ceiling RGB colors (0-255)
- Map: `1` = wall, `0` = empty space, `N/S/E/W` = player spawn

## Technical Details

- Resolution: 1500x800
- FOV: 320 rays
- Tile size: 64 pixels
- Uses DDA algorithm for raycasting

---

*42/1337 Network Project*
