## Raycasting Formulas and Steps

### 1. Ray Direction Calculation
For each vertical stripe (column) on the screen:

	cameraX = 2 * x / screenWidth - 1
	rayDirX = playerDirX + cameraPlaneX * cameraX
	rayDirY = playerDirY + cameraPlaneY * cameraX

### 2. Delta Distance
Distance the ray travels from one x-side to the next, and from one y-side to the next:

	deltaDistX = |1 / rayDirX|
	deltaDistY = |1 / rayDirY|

### 3. Step and Initial Side Distance
Determine step direction and initial side distance:

	if (rayDirX < 0)
		stepX = -1
		sideDistX = (playerX - mapX) * deltaDistX
	else
		stepX = 1
		sideDistX = (mapX + 1.0 - playerX) * deltaDistX

	(Repeat similarly for Y)

### 4. DDA (Digital Differential Analyzer) Loop
Step through the map grid:

	while (wall not hit)
		if (sideDistX < sideDistY)
			sideDistX += deltaDistX
			mapX += stepX
			side = 0
		else
			sideDistY += deltaDistY
			mapY += stepY
			side = 1

### 5. Perpendicular Wall Distance
Calculate distance to the wall to avoid fish-eye effect:

	if (side == 0)
		perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX
	else
		perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY

### 6. Line Height for Wall Slice

	lineHeight = screenHeight / perpWallDist

### 7. Draw Start/End

	drawStart = -lineHeight / 2 + screenHeight / 2
	drawEnd = lineHeight / 2 + screenHeight / 2

---
These formulas are the backbone of the raycasting rendering process. Use them for each column of the screen to determine where and how tall to draw each wall slice.
## Player Movement According to Facing Angle

### Formula Used
To move the player forward or backward based on their facing angle:

	x_new = x_old + (speed * cos(angle))
	y_new = y_old + (speed * sin(angle))

Where:
- (x_old, y_old) is the player's current position
- speed is the movement speed (positive for forward, negative for backward)
- angle is the player's facing direction in radians

To move left or right (strafing), use an angle perpendicular to the facing direction:

	x_new = x_old + (speed * cos(angle + PI/2))
	y_new = y_old + (speed * sin(angle + PI/2))

### Why These Formulas?
These formulas use trigonometry to convert the player's facing angle and movement speed into changes in the x and y coordinates. This allows the player to move smoothly in the direction they are facing, regardless of their orientation. Strafing uses a perpendicular angle to the facing direction, enabling side-to-side movement.

This approach is standard in 2D games and simulations for directional movement, ensuring that movement is always relative to where the player is looking, not just the screen axes.
readme

## Player Antenna Drawing (Directional Line)

### Purpose
Draws a line (antenna) from the player's position in the direction they are facing, visually indicating orientation.

### Formula Used
For each pixel along the antenna:

	x = x0 + i * cos(angle)
	y = y0 + i * sin(angle)

Where:
- (x0, y0) is the player's position (data->player.x, data->player.y)
- angle is the player's angle in radians (data->player.angle)
- i is the distance from the player (0 to antenna length)

### Implementation Example
```c
for (int i = 0; i < 20; i++) {
	put_pixel(data,
		data->player.x + (i * cos(data->player.angle)),
		data->player.y + (i * sin(data->player.angle)),
		0xFFFF0000);
}
```

### Notes
- Angle must be in radians for cos() and sin().
- Cast coordinates to int if needed for pixel drawing.
- The antenna always points in the direction the player is facing.
- Uses trigonometry to convert angle and distance to (x, y).
