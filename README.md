# THE LO3BA — a raycasting 3D engine in C (cub3D)

**A first-person 3D maze in the style of Wolfenstein 3D, rendered in real time from a flat 2D map, written in C with no game engine.** Every frame is computed column by column with a DDA raycasting algorithm and drawn with MiniLibX.

*"Lo3ba" (لعبة) means "game" in Moroccan Darija. This is my take on 1337/42's cub3D project.*

![C](https://img.shields.io/badge/C-00599c?logo=c&logoColor=white)
![MiniLibX](https://img.shields.io/badge/MiniLibX-X11-555)
![Linux](https://img.shields.io/badge/Linux-333?logo=linux&logoColor=white)
![Raycasting](https://img.shields.io/badge/Raycasting-DDA-8a6935)

## How it works

The world is a grid of walls (`1`) and floor (`0`). For every one of the **1500 columns** of the window, the engine:

1. **Casts a ray** from the player across a **60° field of view**.
2. **Walks the grid with DDA** (Digital Differential Analysis): it jumps from one grid line to the next instead of moving in tiny steps, so it finds the first wall exactly and quickly.
3. **Corrects the fish-eye effect** by multiplying the distance by `cos(ray angle − player angle)`, so straight walls look straight.
4. **Draws a vertical slice**: closer walls are taller. It picks the north/south/east/west texture based on which side of the wall the ray hit.

```text
 top-down map                       what the player sees
 1 1 1 1 1 1 1                     ┌───────────────────────┐
 1 0 0 0 0 0 1      cast 1500      │  sky (ceiling color)  │
 1 0 1 1 0 0 1  ── rays over 60° ─►│ ▌▌▌█████▌▌▌▌██████▌▌▌ │  ← one textured column per ray
 1 0 0 N→ ·· 1      (DDA per ray)  │  floor (floor color)  │
 1 1 1 1 1 1 1                     └───────────────────────┘
```

## Features

- Real-time 3D rendering with DDA raycasting and fish-eye correction
- Four wall textures (north, south, east, west) loaded from `.xpm` files
- Custom floor and ceiling colors
- Smooth movement and rotation, with wall collisions
- **Strict map validation**, with a clear error message for every invalid input: wrong file extension, missing textures or colors, invalid characters, a map not closed by walls, a missing or extra player spawn, duplicate texture lines

## Build & run

Requires Linux with X11 and [MiniLibX](https://github.com/42Paris/minilibx-linux) installed in `/usr/local/lib`.

```bash
git clone https://github.com/Abnemili/THE-LO3BA.git
cd THE-LO3BA
make              # builds ./lo3ba
./lo3ba maps/map.cub
```

`make clean`, `make fclean` and `make re` are also available.

### Controls

| Key | Action |
| --- | --- |
| `W` / `S` | Move forward / backward |
| `A` / `D` | Strafe left / right |
| `←` / `→` | Turn the camera |
| `ESC` | Quit |

## Map format (`.cub`)

```text
NO Assets/1.xpm        # north wall texture
SO Assets/2.xpm        # south wall texture
WE Assets/3.xpm        # west wall texture
EA Assets/4.xpm        # east wall texture

F 0,0,0                # floor color (R,G,B 0–255)
C 153,204,255          # ceiling color

111111
100001
101N01                 # 1 = wall · 0 = floor · N/S/E/W = player start and facing
100001
111111
```

The map must be completely surrounded by walls, with exactly one player start.

## Project structure

```text
parsing/     reads and validates the .cub file (textures, colors, map, player)
src/
  init/        window, images and game state setup
  ray_cast/    DDA raycasting and wall projection
  textures/    texture loading and per-column sampling
  movment/     input handling, movement and collisions
  game_loading/ main loop and rendering
get_line/    get_next_line, used to read the map file
```

## What I learned

- The math behind a 3D renderer: angles, trigonometry, perspective projection and the fish-eye correction
- Grid traversal with DDA, and why it beats stepping along the ray pixel by pixel
- Defensive parsing in C: validating every line of untrusted input and reporting precise errors
- Managing memory and graphics resources in C, and cleaning them up on exit

---

*Built as part of the 1337 (42 Network) curriculum.*
