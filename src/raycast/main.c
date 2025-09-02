#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "../../../../Documents/mlx/mlx.h"
#include <math.h>

#define WIDTH 1280
#define HEIGHT 720
#define PI 3.14159265
typedef struct s_player {
    int player_x_pos;
    int player_y_pos;
    bool up;
    bool down;
    bool left;
    bool right;
} t_player;

typedef struct s_game {
    void *mlx;
    void *win;
    void *img;
    char *ptr;
    int bpp;
    int size_line;
    int endian;
    char **map;
    t_player player;
} t_game;

void init_player(t_player *player) {
    player->player_x_pos = WIDTH / 2;
    player->player_y_pos = HEIGHT / 2;
    player->up = player->down = player->left = player->right = false;
}

void put_pix(int x, int y, int col, t_game *access) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return;
    int tmp = y * access->size_line + x * access->bpp / 8;
    access->ptr[tmp] = (col) & 0xFF;
    access->ptr[tmp + 1] = (col >> 8) & 0xFF;
    access->ptr[tmp + 2] = (col >> 16) & 0xFF;
}

void draw_sq(int x, int y, int size, int color, t_game *access) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            put_pix(x + i, y + j, color, access);
    }
}
    }
    



void draw_walls(t_game *access , int col) {
    int i = 0 , j = 0;
    while(i < WIDTH) {
        j = 0;
        while(j < HEIGHT) {
            put_pix(0 , j , col , access);
            // put_pix(0 + 1 , j , col , access);
            // put_pix(0 + 2 , j , col , access);
            // put_pix(0 + 3, j , col , access);

            put_pix(i , 0 , col , access);
            // put_pix(i , 0 + 1, col , access);
            // put_pix(i , 0  + 2, col , access);
            // put_pix(i , 0  + 3, col , access);
            put_pix(WIDTH - 1 , j , col,access);
            put_pix(i , HEIGHT -1 , col  , access);
            j++;
        }
        // put_pix(0 , j -1 , col , access);
        i++;
    }
}

// void draw_something()

void draw_boxe_1(t_game *access , int col) {
    int i = 1 , j = 1;
    while(i < 280) {
        j = 1;
        while(j < 280 ) {
            put_pix(i , j , col , access);
            j++;
        }
        i++;
    } 
}
void draw_boxe_2(t_game *access , int col) {
    int i = 1 , j = 1;
    while(i < 280) {
        j = 1;
        while(j < 280 ) {
            put_pix(WIDTH - i - 1 , HEIGHT - j -1 , col , access);
            j++;
        }
        i++;
    } 
}
int key_press(int key, t_game *game) {
    if (key == 65307)
        exit(0);
    else if (key == 119)
        game->player.up = true;
    else if (key == 115)
        game->player.down = true;
    else if (key == 97)
        game->player.left = true;
    else if (key == 100)
        game->player.right = true;
    return 0;
}

int key_release(int key, t_game *game) {
    if (key == 119)
        game->player.up = false;
    else if (key == 115)
        game->player.down = false;
    else if (key == 97)
        game->player.left = false;
    else if (key == 100)
        game->player.right = false;
    return 0;
}

void move_player(t_player *player) {
    int speed = 3;
    if (player->up) player->player_y_pos -= speed;
    if (player->down) player->player_y_pos += speed;
    if (player->left) player->player_x_pos -= speed;
    if (player->right) player->player_x_pos += speed;
}

// void draw_line(int x0, int y0, int x1, int y1, int color, t_game *g) {
//     int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
//     int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
//     int err = dx + dy, e2;

//     while (1) {
//         put_pix(x0, y0, color, g);
//         if (x0 == x1 && y0 == y1) break;
//         e2 = 2 * err;
//         if (e2 >= dy) { err += dy; x0 += sx; }
//         if (e2 <= dx) { err += dx; y0 += sy; }
//     }
// }

char  **init_map(void) {
    char **map = malloc(sizeof(char) * 10);
    map[0] = "1111111111";
    map[1] = "1000000001";
    map[2] = "1000000001";
    map[3] = "1000000001";
    map[4] = "1000000001";
    map[5] = "1000000001";
    map[6] = "1000000001";
    map[7] = "1000000001";
    map[8] = "1000000001";
    map[9] = "1111111111";
    map[10] = NULL;
    return map;
}

void draw_map(t_game *access) {
    char **map = access->map;
    int col = 0x0000FF;
    for (int i = 0; map[i]; i++)
    {
        for (int j = 0; map[i][j]; j++)
        {
                if(map[i][j] == '1') {
                    draw_sq(i * 64, j * 64, 64 , col , access);
                }
        }   
    }
}

int render(t_game *game) {
    // int cx = game->player.player_x_pos + 5; 
    // int cy = game->player.player_y_pos + 5;
    // double angle = 0;          
    // int length = 100;         
    // int ex = cx + cos(angle) * length;
    // int ey = cy + sin(angle) * length;
    
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->ptr = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    
    move_player(&game->player);
    init_map();
    draw_sq(game->player.player_x_pos, game->player.player_y_pos, 20, 0x0FB64B6, game);
    draw_sq(game->player.player_x_pos + 1 , game->player.player_y_pos + 1, 20 , 0x00FF00 , game);
    // draw_line(cx, cy, ex, ey, 0xFF0000, game);
    draw_walls(game  , 0x00FF00);
    draw_boxe_1(game , 0x0737373);
    draw_boxe_2(game , 0x0737373);

   //draw_radar(game);

    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    mlx_destroy_image(game->mlx, game->img);
    return 0;
}

void init_game(t_game *game) {
    game->mlx = mlx_init();
    game->map = init_map();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "CUBE3DOFFICIAL");

    init_player(&game->player);

    mlx_hook(game->win, 2, 1L << 0, key_press, game);
    mlx_hook(game->win, 3, 1L << 1, key_release, game);
    mlx_loop_hook(game->mlx, render, game);
}

int main() {
    t_game game;
    init_game(&game);
    mlx_loop(game.mlx);
    return 0;
}

