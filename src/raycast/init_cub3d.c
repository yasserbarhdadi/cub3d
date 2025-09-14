/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cub3d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:54:52 by yabarhda          #+#    #+#             */
/*   Updated: 2025/09/14 11:30:50 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

int player_color;
int map_color;

static int touch(t_data *data, int x, int y)
{
	if (y < 0 || y >= data->map->size)
		return (-1);
	if (x < 0 || x >= (int)ft_strlen(data->grid[y]))
		return (-1);
	if (data->grid[y][x] == '1')
		return (1);
	return (0);
}

// Dedicated player wall collision check
static int touch_player(t_data *data, float x, float y)
{
	// Player radius (minimum distance from wall)
	const float radius = 5.0f; // pixels
	int min_x = (int)((x - radius) / BLOCK);
	int max_x = (int)((x + radius) / BLOCK);
	int min_y = (int)((y - radius) / BLOCK);
	int max_y = (int)((y + radius) / BLOCK);
	for (int gy = min_y; gy <= max_y; gy++) {
		if (gy < 0 || gy >= data->map->size)
			return 1;
		for (int gx = min_x; gx <= max_x; gx++) {
			if (gx < 0 || gx >= (int)ft_strlen(data->grid[gy]))
				return 1;
			if (data->grid[gy][gx] == '1')
				return 1;
		}
	}
	return 0;
}

// Get texture color at specific coordinates
static int get_texture_color(t_img *texture, int x, int y)
{
	if (!texture || !texture->pixels || x < 0 || y < 0 || x >= texture->width || y >= texture->height)
		return 0; // Return black if invalid
	
	int offset = (y * texture->size_line) + (x * (texture->bpp / 8));
	
	// Read as 32-bit value - format is 0x00RRGGBB
	unsigned int *pixel_ptr = (unsigned int *)(texture->pixels + offset);
	unsigned int raw_pixel = *pixel_ptr;
	
	// Extract RGB directly - the format is already 0x00RRGGBB
	unsigned char r = (raw_pixel >> 16) & 0xFF;
	unsigned char g = (raw_pixel >> 8) & 0xFF;
	unsigned char b = raw_pixel & 0xFF;
	
	return (r << 16) | (g << 8) | b;
}

static void put_pixel(t_data *data, int x, int y, int color)
{
	if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
		return ;
	int offset = (y * data->img->size_line) + (x * 4);
	data->img->pixels[offset] = (color) & 0xFF; // blue
	data->img->pixels[offset + 1] = (color >> 8) & 0xFF; // green
	data->img->pixels[offset + 2] = (color >> 16) & 0xFF; // red
}

// Structure to hold ray casting results
typedef struct s_ray_hit
{
	float perp_dist;     // Perpendicular distance to wall
	int hit_vertical;    // 1 if vertical wall, 0 if horizontal
	int wall_side;       // 0=North, 1=South, 2=West, 3=East
	float wall_x;        // Exact hit position on the wall (0.0 to 1.0)
} t_ray_hit;

// Cast ray and return detailed hit information
static t_ray_hit cast_ray_detailed(t_data *data, float ray_angle)
{
	t_ray_hit hit = {0};
	float tMaxX = 0, tMaxY = 0;
	int step_x = 0, step_y = 0;
	float ray_dir_x = cos(ray_angle);
	float ray_dir_y = sin(ray_angle);
	float safe_rx = (fabs(ray_dir_x) < 1e-6f) ? 1e-6f : ray_dir_x;
	float safe_ry = (fabs(ray_dir_y) < 1e-6f) ? 1e-6f : ray_dir_y;
	float tDeltaX = BLOCK / fabs(safe_rx);
	float tDeltaY = BLOCK / fabs(safe_ry);
	int grid_x = (int)(data->player.x / BLOCK);
	int grid_y = (int)(data->player.y / BLOCK);

	// Out-of-bounds or wall start: return max distance
	if (grid_y < 0 || grid_y >= data->map->size ||
		grid_x < 0 || grid_x >= (int)ft_strlen(data->grid[grid_y]) ||
		data->grid[grid_y][grid_x] == '1') {
		hit.perp_dist = 1e6f;
		hit.hit_vertical = 0;
		hit.wall_side = 0;
		hit.wall_x = 0.0f;
		return hit;
	}

	if (ray_dir_x > 0)
	{
		tMaxX = ((grid_x + 1) * BLOCK - data->player.x) / safe_rx;
		step_x = 1;
	}
	else if (ray_dir_x < 0)
	{
		tMaxX = (data->player.x - (grid_x * BLOCK)) / -safe_rx;
		step_x = -1;
	}
	else
		tMaxX = 99999;
	if (ray_dir_y > 0)
	{
		tMaxY = ((grid_y + 1) * BLOCK - data->player.y) / safe_ry;
		step_y = 1;
	}
	else if (ray_dir_y < 0)
	{
		tMaxY = (data->player.y - grid_y * BLOCK) / -safe_ry;
		step_y = -1;
	}
	else
		tMaxY = 99999;
	
	// Traverse until we hit a wall
	while (!touch(data, grid_x, grid_y))
	{
		// Out-of-bounds: return max distance
		if (grid_y < 0 || grid_y >= data->map->size ||
			grid_x < 0 || grid_x >= (int)ft_strlen(data->grid[grid_y])) {
			hit.perp_dist = 1e6f;
			hit.hit_vertical = 0;
			hit.wall_side = 0;
			hit.wall_x = 0.0f;
			return hit;
		}
		if (tMaxX < tMaxY)
		{
			grid_x += step_x;
			tMaxX += tDeltaX;
			hit.hit_vertical = 1;
		}
		else
		{
			grid_y += step_y;
			tMaxY += tDeltaY;
			hit.hit_vertical = 0;
		}
	}
	
	// Calculate exact hit point and wall side
	float t_hit = hit.hit_vertical ? (tMaxX - tDeltaX) : (tMaxY - tDeltaY);
	float hit_x = data->player.x + ray_dir_x * t_hit;
	float hit_y = data->player.y + ray_dir_y * t_hit;
	
	// Determine which side of the wall was hit and calculate wall_x
	if (hit.hit_vertical)
	{
		// Vertical wall (North or South face)
		if (step_x > 0) {
			hit.wall_side = 2; // West face (coming from west, hitting east-facing wall)
		} else {
			hit.wall_side = 3; // East face (coming from east, hitting west-facing wall)
		}
		hit.wall_x = fmod(hit_y, BLOCK) / BLOCK; // Y position along the wall
	}
	else
	{
		// Horizontal wall (East or West face)  
		if (step_y > 0) {
			hit.wall_side = 0; // North face (coming from north, hitting south-facing wall)
		} else {
			hit.wall_side = 1; // South face (coming from south, hitting north-facing wall)
		}
		hit.wall_x = fmod(hit_x, BLOCK) / BLOCK; // X position along the wall
	}
	
	// Ensure wall_x is between 0 and 1
	if (hit.wall_x < 0) hit.wall_x += 1.0f;
	if (hit.wall_x > 1.0f) hit.wall_x -= 1.0f;
	
	// Calculate perpendicular distance with fisheye correction
	float direct_dist = t_hit;
	float angle_diff = ray_angle - data->player.angle;
	hit.perp_dist = direct_dist * cos(angle_diff);
	if (hit.perp_dist < 1e-3f) hit.perp_dist = 1e-3f; // avoid div by zero
	
	return hit;
}

// Helper: Cast ray and return perpendicular distance to wall
// Returns perpendicular distance and sets *hit_vertical (1=vertical, 0=horizontal)
static float cast_ray_perp_dist(t_data *data, float ray_angle, int *hit_vertical)
{
	float tMaxX = 0, tMaxY = 0;
	int step_x = 0, step_y = 0;
	float ray_dir_x = cos(ray_angle);
	float ray_dir_y = sin(ray_angle);
	float safe_rx = (fabs(ray_dir_x) < 1e-6f) ? 1e-6f : ray_dir_x;
	float safe_ry = (fabs(ray_dir_y) < 1e-6f) ? 1e-6f : ray_dir_y;
	float tDeltaX = BLOCK / fabs(safe_rx);
	float tDeltaY = BLOCK / fabs(safe_ry);
	int grid_x = (int)(data->player.x / BLOCK);
	int grid_y = (int)(data->player.y / BLOCK);

	// Out-of-bounds or wall start: return max distance
	if (grid_y < 0 || grid_y >= data->map->size ||
		grid_x < 0 || grid_x >= (int)ft_strlen(data->grid[grid_y]) ||
		data->grid[grid_y][grid_x] == '1') {
		if (hit_vertical) *hit_vertical = 0;
		return 1e6f;
	}

	*hit_vertical = 0;
	if (ray_dir_x > 0)
	{
		tMaxX = ((grid_x + 1) * BLOCK - data->player.x) / safe_rx;
		step_x = 1;
	}
	else if (ray_dir_x < 0)
	{
		tMaxX = (data->player.x - (grid_x * BLOCK)) / -safe_rx;
		step_x = -1;
	}
	else
		tMaxX = 99999;
	if (ray_dir_y > 0)
	{
		tMaxY = ((grid_y + 1) * BLOCK - data->player.y) / safe_ry;
		step_y = 1;
	}
	else if (ray_dir_y < 0)
	{
		tMaxY = (data->player.y - grid_y * BLOCK) / -safe_ry;
		step_y = -1;
	}
	else
		tMaxY = 99999;
	while (!touch(data, grid_x, grid_y))
	{
		// Out-of-bounds: return max distance
		if (grid_y < 0 || grid_y >= data->map->size ||
			grid_x < 0 || grid_x >= (int)ft_strlen(data->grid[grid_y])) {
			*hit_vertical = 0;
			return 1e6f;
		}
		if (tMaxX < tMaxY)
		{
			grid_x += step_x;
			tMaxX += tDeltaX;
			*hit_vertical = 1;
		}
		else
		{
			grid_y += step_y;
			tMaxY += tDeltaY;
			*hit_vertical = 0;
		}
	}
	float t_hit = *hit_vertical ? (tMaxX - tDeltaX) : (tMaxY - tDeltaY);
	float direct_dist = t_hit * sqrt(ray_dir_x * ray_dir_x + ray_dir_y * ray_dir_y);
	// Fisheye correction: project onto player view
	float angle_diff = ray_angle - data->player.angle;
	float perp_dist = direct_dist * cos(angle_diff);
	if (perp_dist < 1e-3f) perp_dist = 1e-3f; // avoid div by zero
	return perp_dist;
}
// Draw 3D walls using raycasting with proper texture mapping
static void draw_walls(t_data *data)
{
	// Camera plane method for perspective-correct raycasting
	const float fov_rad = (90.0f * PI) / 180.0f;
	const int num_rays = WIDTH;
	float camera_x;
	float ray_angle;
	
	for (int x = 0; x < num_rays; ++x)
	{
		camera_x = 2.0f * x / (float)num_rays - 1.0f;
		ray_angle = data->player.angle + atan(camera_x * tan(fov_rad / 2.0f));
		
		// Get detailed ray hit information
		t_ray_hit hit = cast_ray_detailed(data, ray_angle);
		
		// Calculate wall height on screen
		int wall_height = (int)((BLOCK * HEIGHT) / hit.perp_dist);
		int y_start = (HEIGHT - wall_height) / 2;
		int y_end = (HEIGHT + wall_height) / 2;
		
		// Select the appropriate texture based on wall side
		t_img *texture = NULL;
		switch (hit.wall_side)
		{
			case 0: texture = data->texture->north; break;  // North face
			case 1: texture = data->texture->south; break;  // South face
			case 2: texture = data->texture->west; break;   // West face
			case 3: texture = data->texture->east; break;   // East face
		}
		
		// Calculate texture X coordinate
		int tex_x = 0;
		if (texture && texture->width > 0)
		{
			tex_x = (int)(hit.wall_x * texture->width);
			// Ensure tex_x is within bounds
			if (tex_x < 0) tex_x = 0;
			if (tex_x >= texture->width) tex_x = texture->width - 1;
		}
		
		// Draw the wall column
		for (int y = y_start; y < y_end; y++)
		{
			if (y >= 0 && y < HEIGHT)
			{
				int color;
				
				if (texture && texture->pixels)
				{
					// Calculate texture Y coordinate
					float wall_pos = (float)(y - y_start) / (float)wall_height;
					int tex_y = (int)(wall_pos * texture->height);
					
					// Ensure tex_y is within bounds
					if (tex_y < 0) tex_y = 0;
					if (tex_y >= texture->height) tex_y = texture->height - 1;
					
					// Get texture color
					color = get_texture_color(texture, tex_x, tex_y);
				}
				else
				{
					// Fallback colors if no texture
					if (hit.hit_vertical)
						color = decode_rgb(120, 120, 120); // darker for vertical
					else
						color = decode_rgb(200, 200, 200); // lighter for horizontal
				}
				
				put_pixel(data, x, y, color);
			}
		}
	}
}

static int	clean_exit(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img->img);
	// mlx_destroy_image(data->mlx, data->texture->north->img);
	// mlx_destroy_image(data->mlx, data->texture->south->img);
	// mlx_destroy_image(data->mlx, data->texture->west->img);
	// mlx_destroy_image(data->mlx, data->texture->east->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	ft_malloc(-42);
	exit(0);
}

static int	key_press(int key, t_data *data)
{
	if (key == KEY_ESC)
		clean_exit(data);
	if (key == KEY_W)
		data->player.key_w = true;
	if (key == KEY_A)
		data->player.key_a = true;
	if (key == KEY_S)
		data->player.key_s = true;
	if (key == KEY_D)
		data->player.key_d = true;
	if (key == KEY_RIGHT)
		data->player.key_right = true;
	if (key == KEY_LEFT)
		data->player.key_left = true;
	return (0);
}

static int	key_release(int key, t_data *data)
{
	if (key == KEY_W)
		data->player.key_w = false;
	if (key == KEY_A)
		data->player.key_a = false;
	if (key == KEY_S)
		data->player.key_s = false;
	if (key == KEY_D)
		data->player.key_d = false;
	if (key == KEY_RIGHT)
		data->player.key_right = false;
	if (key == KEY_LEFT)
		data->player.key_left = false;
	return (0);
}

static void draw_box(t_data *data, int x, int y, int color, int size)
{
	for (int i = 0; i <= size; i++)
		put_pixel(data, x + i, y, color);
	for (int i = 0; i <= size; i++)
		put_pixel(data, x, y + i, color);
	for (int i = 0; i <= size; i++)
		put_pixel(data, x + i, y + size, color);
	for (int i = 0; i <= size; i++)
		put_pixel(data, x + size, y + i, color);
}

static void move_player(t_data *data)
{
	float speed = 1.25f;
	float rot_speed = 0.03f;

	if (data->player.angle > PI * 2)
		data->player.angle = 0;
	if (data->player.angle < 0)
		data->player.angle = PI * 2;

	float cos_angle = cos(data->player.angle);
	float sin_angle = sin(data->player.angle);

	float next_x, next_y;
	// Forward
	if (data->player.key_w)
	{
		next_x = data->player.x + cos_angle * speed;
		next_y = data->player.y + sin_angle * speed;
		// Check X axis
		if (!touch_player(data, next_x, data->player.y))
			data->player.x = next_x;
		// Check Y axis
		if (!touch_player(data, data->player.x, next_y))
			data->player.y = next_y;
	}
	// Left
	if (data->player.key_a)
	{
		next_x = data->player.x + cos(data->player.angle + (3 * PI) / 2) * speed;
		next_y = data->player.y + sin(data->player.angle + (3 * PI) / 2) * speed;
		if (!touch_player(data, next_x, data->player.y))
			data->player.x = next_x;
		if (!touch_player(data, data->player.x, next_y))
			data->player.y = next_y;
	}
	// Backward
	if (data->player.key_s)
	{
		next_x = data->player.x - cos_angle * speed;
		next_y = data->player.y - sin_angle * speed;
		if (!touch_player(data, next_x, data->player.y))
			data->player.x = next_x;
		if (!touch_player(data, data->player.x, next_y))
			data->player.y = next_y;
	}
	// Right
	if (data->player.key_d)
	{
		next_x = data->player.x - cos(data->player.angle + (3 * PI) / 2) * speed;
		next_y = data->player.y - sin(data->player.angle + (3 * PI) / 2) * speed;
		if (!touch_player(data, next_x, data->player.y))
			data->player.x = next_x;
		if (!touch_player(data, data->player.x, next_y))
			data->player.y = next_y;
	}
	if (data->player.key_right) data->player.angle += rot_speed;
	if (data->player.key_left) data->player.angle -= rot_speed;

	// Clamp player position to stay out of walls and away from boundaries
	int grid_x = (int)(data->player.x / BLOCK);
	int grid_y = (int)(data->player.y / BLOCK);
	float offset = 2.0f; // pixels away from wall
	if (touch_player(data, data->player.x, data->player.y)) {
		// Try to nudge player out of wall
		if (!touch_player(data, (grid_x + 1) * BLOCK - offset, data->player.y))
			data->player.x = (grid_x + 1) * BLOCK - offset;
		else if (!touch_player(data, grid_x * BLOCK + offset, data->player.y))
			data->player.x = grid_x * BLOCK + offset;
		if (!touch_player(data, data->player.x, (grid_y + 1) * BLOCK - offset))
			data->player.y = (grid_y + 1) * BLOCK - offset;
		else if (!touch_player(data, data->player.x, grid_y * BLOCK + offset))
			data->player.y = grid_y * BLOCK + offset;
	}
}

static void clear_window(t_data *data)
{
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			put_pixel(data, x, y, 0);
}

static void	draw_map(t_data *data, int color)
{
	int x_axis = 0;
	int y_axis = 0;
	for (int y = 0; data->grid[y]; y++)
	{
		x_axis = 0;
		for (int x = 0; data->grid[y][x]; x++)
		{
			if (data->grid[y][x] == '1')
				draw_box(data, x_axis, y_axis, color, BLOCK);
			x_axis += BLOCK;
		}
		y_axis += BLOCK;
	}
}

static int get_ray_len(t_data *data, float cos_angle, float sin_angle)
{
	int i = 0;
	while (1337)
	{
		float x = data->player.x + (cos_angle * i);
		float y = data->player.y + (sin_angle * i);
		int ret = touch(data, x, y);
		if (ret == -1)
			return (0);
		else if (ret == 1)
			break ;
		i++;
	}
	return (i);
}

static void	draw_line(t_data *data, float angle)
{
	float tMaxX = 0;
	float tMaxY = 0;
	int step_x = 0;
	int step_y = 0;
	float ray_dir_x = cos(angle);
	float ray_dir_y = sin(angle);
	// Avoid division by zero: treat zero components as very small
	float safe_rx;
	float safe_ry;
	if (fabs(ray_dir_x) < 1e-6f)
		safe_rx = 1e-6f;
	else
		safe_rx = ray_dir_x;
	if (fabs(ray_dir_y) < 1e-6f)
		safe_ry = 1e-6f;
	else
		safe_ry = ray_dir_y;
	float tDeltaX = BLOCK / fabs(safe_rx);
	float tDeltaY = BLOCK / fabs(safe_ry);
	int grid_x = (int)(data->player.x / BLOCK);
	int grid_y = (int)(data->player.y / BLOCK);
	if (ray_dir_x > 0)
	{
		tMaxX = ((grid_x + 1) * BLOCK - data->player.x) / safe_rx;
		step_x = 1;
	}
	else if (ray_dir_x < 0)
	{
		tMaxX = (data->player.x - (grid_x * BLOCK)) / -safe_rx;
		step_x = -1;
	}
	else if (ray_dir_x == 0)
		tMaxX = 99999;
	if (ray_dir_y > 0)
	{
		tMaxY = ((grid_y + 1) * BLOCK - data->player.y) / safe_ry;
		step_y = 1;
	}
	else if (ray_dir_y < 0)
	{
		tMaxY = (data->player.y - grid_y * BLOCK) / -safe_ry;
		step_y = -1;
	}
	else if (ray_dir_y == 0)
		tMaxY = 99999;
	int hit_vertical = 0; // 1 if last step was along X, else 0 for Y
	while (!touch(data, grid_x, grid_y))
	{
		if (tMaxX < tMaxY)
		{
			grid_x += step_x;
			tMaxX += tDeltaX;
			hit_vertical = 1;
		}
		else
		{
			grid_y += step_y;
			tMaxY += tDeltaY;
			hit_vertical = 0;
		}
	}
	// Compute exact collision point
	float c_x = 0;
	float c_y = 0;
	if (hit_vertical)
	{
		float t_hit = tMaxX - tDeltaX;
		// Choose correct edge based on direction of travel
		if (step_x == 1)
			c_x = grid_x * BLOCK;           // entered from left, hit left edge
		else
			c_x = (grid_x + 1) * BLOCK;     // entered from right, hit right edge
		c_y = data->player.y + safe_ry * t_hit;
	}
	else
	{
		float t_hit = tMaxY - tDeltaY;
		if (step_y == 1)
			c_y = grid_y * BLOCK;           // entered from top, hit top edge
		else
			c_y = (grid_y + 1) * BLOCK;     // entered from bottom, hit bottom edge
		c_x = data->player.x + safe_rx * t_hit;
	}
	float dx = c_x - data->player.x;
	float dy = c_y - data->player.y;

	int steps = (int)fmax(fabs(dx), fabs(dy));
	if (steps < 1) steps = 1; // guard

	float x_inc = dx / steps;
	float y_inc = dy / steps;

	float x = data->player.x;
	float y = data->player.y;
	for (int i = 0; i <= steps; i++)
	{
		put_pixel(data, (int)x, (int)y, decode_rgb(255, 0, 0));
		x += x_inc;
		y += y_inc;
	}
}

static void draw_player(t_data *data)
{
	draw_box(data, data->player.x, data->player.y, player_color, 16);
	// Cast rays across a forward-facing field of view centered on player.angle
	const float fov_rad = (90.0f * PI) / 180.0f; // 60° FOV
	const int num_rays = WIDTH;                    // adjust as needed
	const float start = data->player.angle - (fov_rad * 0.5f);
	const float step = fov_rad / (float)num_rays;
	for (int i = 0; i <= num_rays; ++i)
	{
		float ang = start + (i * step);
		draw_line(data, ang);
	}
}

static void draw_background(t_data *data)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		if (y < HEIGHT / 2)
		{
			for (int x = 0; x < WIDTH; x++)
				put_pixel(data, x, y, decode_rgb(data->texture->ceiling[0], data->texture->ceiling[1], data->texture->ceiling[2]));
		}
		else
		{
			for (int x = 0; x < WIDTH; x++)
				put_pixel(data, x, y, decode_rgb(data->texture->floor[0], data->texture->floor[1], data->texture->floor[2]));
		}
	}
}

static int	on_game_update(t_data *data)
{
	clear_window(data);
	draw_background(data);
	move_player(data);
	// draw_map(data, map_color);
	draw_walls(data); // <-- 3D wall rendering
	// draw_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img->img, 0, 0);
	return (0);
}

static void get_player_3d_position(t_data *data)
{
	int x_axis = 0;
	int y_axis = 0;
	for (int y = 0; data->grid[y]; y++)
	{
		x_axis = 0;
		for (int x = 0; data->grid[y][x]; x++)
		{
			if (x == data->player.x && y == data->player.y)
			{
				data->player.x = x_axis;
				data->player.y = y_axis;
				return ;
			}
			x_axis += BLOCK;
		}
		y_axis += BLOCK;
	}	
}

static void load_textures(t_data *data)
{
	// Allocate texture structure
	// data->texture = ft_malloc(sizeof(t_texture));
	
	// Allocate individual texture images
	data->texture->north = ft_malloc(sizeof(t_img));
	data->texture->south = ft_malloc(sizeof(t_img));
	data->texture->west = ft_malloc(sizeof(t_img));
	data->texture->east = ft_malloc(sizeof(t_img));
	
	// Set file paths (these should come from your map parsing)
	data->texture->north->file = "./assets/north.xpm";
	data->texture->south->file = "./assets/south.xpm";
	data->texture->west->file = "./assets/west.xpm";
	data->texture->east->file = "./assets/east.xpm";
	
	// Load north texture
	data->texture->north->img = mlx_xpm_file_to_image(data->mlx, data->texture->north->file, 
		&data->texture->north->width, &data->texture->north->height);
	if (data->texture->north->img)
		data->texture->north->pixels = mlx_get_data_addr(data->texture->north->img, 
			&data->texture->north->bpp, &data->texture->north->size_line, &data->texture->north->endian);
	
	// Load south texture
	data->texture->south->img = mlx_xpm_file_to_image(data->mlx, data->texture->south->file, 
		&data->texture->south->width, &data->texture->south->height);
	if (data->texture->south->img)
		data->texture->south->pixels = mlx_get_data_addr(data->texture->south->img, 
			&data->texture->south->bpp, &data->texture->south->size_line, &data->texture->south->endian);
	
	// Load west texture
	data->texture->west->img = mlx_xpm_file_to_image(data->mlx, data->texture->west->file, 
		&data->texture->west->width, &data->texture->west->height);
	if (data->texture->west->img)
		data->texture->west->pixels = mlx_get_data_addr(data->texture->west->img, 
			&data->texture->west->bpp, &data->texture->west->size_line, &data->texture->west->endian);
	
	// Load east texture
	data->texture->east->img = mlx_xpm_file_to_image(data->mlx, data->texture->east->file, 
		&data->texture->east->width, &data->texture->east->height);
	if (data->texture->east->img)
		data->texture->east->pixels = mlx_get_data_addr(data->texture->east->img, 
			&data->texture->east->bpp, &data->texture->east->size_line, &data->texture->east->endian);
	
	// Initialize ceiling and floor colors
	// data->texture->ceiling = ft_malloc(3);
	// data->texture->floor = ft_malloc(3);
	// data->texture->ceiling[0] = 42; // R
	// data->texture->ceiling[1] = 42; // G
	// data->texture->ceiling[2] = 42; // B
	// data->texture->floor[0] = 42;   // R
	// data->texture->floor[1] = 42;   // G
	// data->texture->floor[2] = 42;   // B
}

void	init_cub3d(t_data *data)
{
	player_color = decode_rgb(0, 255, 30);
	map_color = decode_rgb(128, 128, 128);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3D");
	data->img = ft_malloc(sizeof(t_img));
	data->img->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->img->pixels = mlx_get_data_addr(data->img->img, &data->img->bpp, &data->img->size_line, &data->img->endian);
	data->grid = ft_lsttoarr(data->map);
	get_player_3d_position(data);
	load_textures(data);
	mlx_hook(data->win, 17, 1L << 0, clean_exit, data);
	mlx_hook(data->win, 2, 1L << 0, key_press, data);
	mlx_hook(data->win, 3, 1L << 1, key_release, data);
	mlx_loop_hook(data->mlx, on_game_update, data);
	mlx_loop(data->mlx);
}
