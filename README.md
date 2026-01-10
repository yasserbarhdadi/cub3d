## cub3D — Raycasting Shooter

A minimal Wolfenstein-style renderer written with MiniLibX. This document explains every moving part, the math behind it, how to build/run, and the data formats so you can reproduce or extend the project.

### Quick Start
- Build: `make`
- Run: `./cub3D maps/map.cub`
- Keys: `W/A/S/D` move, `←/→` rotate, `Esc` quit.

### Project Layout (key files)
- Core loop & rendering: [src/raycast/raycast.c](src/raycast/raycast.c), [src/raycast/texture_draw.c](src/raycast/texture_draw.c), [src/raycast/texture_utils.c](src/raycast/texture_utils.c), [src/raycast/draw_secondaries.c](src/raycast/draw_secondaries.c)
- Input & player: [src/raycast/player_movement.c](src/raycast/player_movement.c), [src/raycast/game_loop.c](src/raycast/game_loop.c)
- Parsing: [src/parse/parse_file.c](src/parse/parse_file.c), [src/parse/check_element.c](src/parse/check_element.c), [src/parse/map_check.c](src/parse/map_check.c), [src/parse/validate_map.c](src/parse/validate_map.c)
- Utils: [src/utils](src/utils)
- Entry: [src/main.c](src/main.c)
- Header/types: [inc/main.h](inc/main.h)

### Build & Dependencies
- Compiler flags: `-Wall -Wextra -Werror`
- Libraries: MiniLibX with X11 (`-lmlx -lXext -lX11 -lm`)
- Targets: `make`, `make clean`, `make fclean`, `make re`

### Data Formats
- **Map file (.cub)**
  - Texture paths: `NO`, `SO`, `WE`, `EA` each pointing to `.xpm`
  - Floor/Ceiling colors: `F r,g,b`, `C r,g,b` (0–255)
  - Map grid of `1` (wall), `0` (space), single player start `N/S/E/W`
- **Texture validation** happens during parsing: ensures `.xpm` extension, file exists, and is non-empty before loading via MLX.

### Parsing (.cub) — Detailed

Parsing lives in `src/parse/` and is designed to fail fast (prints an error via `ft_perror`, triggers `ft_malloc(-42)` cleanup, then `exit(1)`).

**High-level flow**
1) `parse_file(data, path)` (`src/parse/parse_file.c`)
   - Opens the file (`open(path, O_RDONLY)`), then runs the three phases below.
2) Elements phase: `parse_elements()` + `require_elements()` (`src/parse/parse_elements.c`)
   - Reads lines until all 6 elements are set: `NO/SO/WE/EA/F/C`.
3) Texture file checks: `check_texture_files()` (`src/parse/parse_texture_check.c`)
   - Validates the 4 texture paths (extension/existence/readability).
4) Map phase: `map_check()` (`src/parse/map_check.c`)
   - Reads the grid into a linked list (`t_map`) and then validates it (`validate_map`).

**File responsibilities (parse folder)**
- `parse_file.c`: orchestration only.
- `parse_elements.c`: reads the header part (textures + colors) until complete.
- `check_element.c`: dispatches a line to texture or color assignment.
- `element_texture.c`: parses and stores one texture path.
- `element_color_utils.c`: string helpers for color parsing.
- `valid_color_format.c`: validates the raw `r,g,b` formatting (digits/commas/spaces).
- `element_color.c`: parses `F`/`C` into 3 bytes (0–255 each).
- `parse_texture_check.c`: validates that the texture files are usable.
- `map_check.c`: reads the map grid, validates characters, and enforces “no content after map gap”.
- `map_nodes.c`: linked-list helpers (`add_node`, `new_node`).
- `validate_map.c`: runs the three validation steps.
- `validate_size.c`: computes the linked list size and stores it in each node.
- `validate_player.c`: finds exactly one `N/S/E/W`, sets player position/direction, replaces it with `'0'`.
- `validate_deep.c`: duplicates the map and calls border checks.
- `validate_borders.c`: verifies enclosure rules for walkable cells.
- `convert_map_to_array.c`: converts the linked list to `data->map_array` and computes `map_width/map_height` (used later by the engine).
- `utils.c`: shared parse helpers like `valid_map_char()` / `only_spaces()`.

## What is accepted and what is rejected

### 1) Elements parsing (NO/SO/WE/EA/F/C)

**Accepted**
- Empty/whitespace-only lines are skipped.
- Elements can appear in any order.
- Texture path can end with `\n` (it will be duplicated without the newline).
- Color values allow spaces around digits/commas as long as the overall format remains valid.

**Rejected (error cases)**
- Empty file: first `get_next_line(fd)` returns `NULL` → `"Empty file"`.
- Unknown identifier (not `NO/SO/WE/EA/F/C`) → `"Invalid map"`.
- Duplicate texture definition (`NO/SO/WE/EA` repeated) → `"Multiple definitions of textures"`.
- Duplicate color definition (`F` or `C` repeated) → `"Multiple definitions of floor colors"` (same message for both).
- Texture line with missing path (no `arr[1]`) → `"Invalid map"`.
- Texture line with extra non-newline tokens (`arr[2]` that is not just `\n`) → `"Invalid map"`.
- Color line with missing payload (no `arr[1]`) → `"Invalid map"`.
- Color formatting invalid (not exactly 2 commas, bad characters, starts/ends with comma) → `"Invalid map"`.
- Color split does not produce exactly 3 parts (more/less than 3) → `"Invalid map"`.
- Any color channel is not purely numeric (except trailing `\n`) → `"Invalid map"`.
- Any channel value out of range `[0..255]` → `"Invalid map"`.
- After the elements loop ends, if any of the 6 required values is missing → `"Invalid map"`.

### 2) Texture file validation
`check_texture_files()` validates each of `NO/WE/SO/EA`:
- Path must end with `.xpm` (minimum length 5) → otherwise `"Invalid texture file"`.
- File must open and be readable (a `read(fd, buf, 1)` must succeed) → otherwise `"Invalid texture file"`.

### 3) Map reading + character validation

**Accepted**
- Leading blank lines before the first map row are ignored.
- Map rows may contain: `1`, `0`, exactly one of `N/S/E/W`, and whitespace.
- Trailing blank lines after the map are allowed.

**Rejected (error cases)**
- Any non-whitespace character outside `01NSEW` → `"Invalid map"`.
- If a blank/space-only line appears *inside* the map, then any later non-blank line is rejected.
  - This prevents “two separated map blocks”.

### 4) Map validation (size, player, borders)

**Size (`get_map_size`)**
- Counts linked-list rows and stores the same `size` in each node (`t_map.size`).

**Player (`standard_check`)**
- Finds the first and only `N/S/E/W`.
- Sets:
  - `data->player.x = (x + 0.5) * BLOCK`
  - `data->player.y = (y + 0.5) * BLOCK`
  - `data->player.direction = 'N'|'S'|'E'|'W'`
- Replaces the map cell with `'0'` so walkable-area validation treats it as floor.

Rejected:
- More than one player start → `"Invalid map"`.
- No player start → `"Player not found"`.

**Border / enclosure (`deep_check` + `check_borders`)**
- Duplicates each row (`ft_strdup`) into a temporary `char **map`.
- For every `'0'` cell, checks its 4-neighbors exist and are either `'0'` or `'1'`.
  - Right neighbor must exist and be `0/1`.
  - Left neighbor must exist and be `0/1`.
  - Down neighbor row must exist, column must be in-bounds, and be `0/1`.
  - Up neighbor must exist, column must be in-bounds, and be `0/1`.

Rejected:
- Any `'0'` cell touching outside the map, a shorter row, or a non `0/1` cell (including spaces) → `"Invalid map"`.
- Any copied row that becomes empty (`!map[i][0]`) → `"Invalid map"`.

### Input & Movement
- Position updated by $x_{new} = x_{old} + v \cdot \cos(\theta)$, $y_{new} = y_{old} + v \cdot \sin(\theta)$
- Strafing uses $\theta + \frac{\pi}{2}$.
- Collision: rejects moves landing in a wall cell.

### Rendering Pipeline (per frame)
1) Clear image buffer and draw flat ceiling/floor ([draw_secondaries.c](src/raycast/draw_secondaries.c)).
2) For each screen column:
   - Compute ray angle across FOV.
   - Initialize DDA step based on ray direction.
   - DDA until hitting a wall grid cell.
   - Compute perpendicular distance to avoid fish-eye.
   - Derive wall slice height and vertical draw limits.
   - Sample texture column and blit to the screen buffer.
3) Push image to window (double-buffered via MLX image).

### Core Math (fish-eye free)
- Ray angle step: $\Delta \alpha = \frac{\text{FOV}}{\text{WIDTH}}$
- Delta distances: $\delta_x = \left|\frac{1}{\cos(\alpha)}\right|$, $\delta_y = \left|\frac{1}{\sin(\alpha)}\right|$
- DDA progression chooses the smaller of next $side\_x$ or $side\_y$ to step gridwise.
- Perpendicular distance used for scaling: $d_\perp = d_{hit} \cdot \cos(\alpha - \theta_{player})$
- Wall height: $h = \frac{\text{HEIGHT}}{d_\perp}$
- Vertical bounds: $start = \max\left(0, -\frac{h}{2} + \frac{\text{HEIGHT}}{2}\right)$, $end = \min\left(\text{HEIGHT}-1, \frac{h}{2} + \frac{\text{HEIGHT}}{2}\right)$
- Texture X coordinate: project hit point along the wall, mirror when needed for facing direction.

### Texture Sampling Notes
- Textures load once at startup (`init_textures`).
- For each column: `tex_x` from wall hit fraction; `tex_y` increases by `step = tex_height / wall_h` as we move down the slice.
- Perpendicular distance is used both for wall height and `tex_x` computation to keep textures stable when strafing.

### Controls & Angles
- Rotation adjusts player angle by a constant angular velocity (see [player_movement.c](src/raycast/player_movement.c)).
- Angles in radians; FOV is 60° ($\pi / 3$).

### Error Handling & Validation
- Parsing aborts on:
  - Missing or duplicate texture/color definitions.
  - Non-rectangular or open maps.
  - Invalid characters outside `01NSEW`.
  - Texture files missing, empty, or non-`.xpm`.
- Rendering guards against out-of-bounds writes in `put_pixel`.

### Minimal Visual Guide (ASCII)
```
Player (P), rays (.), walls (#)

#########
#..P....#   <- rays cast per column
#.......#
#########
```

### Extending the Project
- Add sprites: cast a secondary pass sorted by distance; reuse texture sampling helpers.
- Doors: treat door cells as semi-solid with partial depth tests.
- Minimap: reuse map array and `put_pixel` to draw top-down overlay.

### What changed (additions)
- Textured walls replacing flat colors via MLX `xpm` images.
- Texture validation during parsing (empty/format checks) to fail early.
- Fish-eye-safe sampling: perpendicular distance shared between scaling and UVs to reduce warping when strafing.
- Struct additions: texture image metadata (`t_img`) and draw helpers (`t_draw`) in [inc/main.h](inc/main.h).

### How to Recreate From Scratch
1) Set up MiniLibX and a Makefile linking X11.
2) Define core structs: player (pos, angle), ray (DDA data), textures (paths + MLX images), and a central `t_data`.
3) Parse `.cub` file: store texture paths, colors, and map array; validate borders and characters.
4) Initialize MLX window/image and hook events (keypress, keyrelease, loop hook).
5) Implement the ray loop as described above; test first with flat colors, then add textures via `mlx_xpm_file_to_image`.
6) Add collision-aware movement driven by keyboard state each frame.
7) Optimize: single image buffer per frame; avoid recomputing textures; guard all allocations and file opens.

### Compilation & Run Commands
```sh
make            # build
./cub3D maps/map.cub   # run game
make fclean     # remove binaries and objects
```

### Ray Math (project-specific, angle-based)
For each screen column `x`:
- Ray angle: $\alpha = \theta_{player} - \text{FOV}/2 + x \cdot \frac{\text{FOV}}{\text{WIDTH}}$ (radians).
- Direction components: $dir_x = \cos(\alpha)$, $dir_y = \sin(\alpha)$.
- Delta distances: $\delta_x = \left|\frac{1}{dir_x}\right|$, $\delta_y = \left|\frac{1}{dir_y}\right|$.
- Initial side distances depend on ray sign; DDA advances the smaller of `side_x` / `side_y` until a wall cell is hit, storing which side (x or y) was crossed.
- Perpendicular distance (fish-eye corrected): $d_\perp = d_{hit} \cdot \cos(\alpha - \theta_{player})$.
- Wall height: $h = \frac{\text{HEIGHT}}{d_\perp}$; clamp draw start/end to the screen.
- Texture X: project hit position along the wall; mirror based on facing to keep orientation consistent.
- Texture Y: advance by `step = tex_height / h` as you draw down the column.

### Movement Formulas (used here)
- Forward/back: $x' = x + v\cos(\theta)$, $y' = y + v\sin(\theta)$.
- Strafe: $x' = x + v\cos(\theta + \pi/2)$, $y' = y + v\sin(\theta + \pi/2)$.
- Rotation: adjust $\theta$ by a fixed angular velocity on keypress (`←/→`).

### (Optional) Direction Line Helper
Not rendered by default, but if you want a debug line from the player in the facing direction:
```c
for (int i = 0; i < 20; i++)
  put_pixel(data,
    data->player.x + i * cos(data->player.angle),
    data->player.y + i * sin(data->player.angle),
    0xFFFF0000);
```
This visualizes $x = x_0 + i\cos(\theta)$, $y = y_0 + i\sin(\theta)$.

