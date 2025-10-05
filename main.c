#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define PLAYER_RADIUS 5
#define MAX_BULLETS 10
#define MAX_ENEMIES 5
#define BULLET_COOLDOWN 5
#define ENEMY_RADIUS 8
#define BULLET_RADIUS 2
#define ENEMY_SHOOT_COOLDOWN 90

int player_x = 100, player_y = 100;
int game_over = 0;
int bullet_cooldown = 0; 

#include "bullets.h"
#include "enemies.h"
#include "collisions.h"


int main(void) {
   if (allegro_init() != 0) {
       printf("Error initializing Allegro!\n");
       return 1;
   }
  
   install_keyboard();
   install_timer();


   set_color_depth(8);
   if (set_gfx_mode(GFX_AUTODETECT, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) != 0) {
       printf("Error setting graphics mode!\n");
       return 1;
   }


   BITMAP* buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
   if (!buffer) {
       printf("Error creating off-screen buffer!\n");
       return 1;
   }


   PALETTE palette;
   generate_332_palette(palette);
   set_palette(palette);


   init_bullets();
   init_enemies();


   while (1) {
       if (game_over) {
           textout_ex(screen, font, "Game Over! Press ESC to exit.", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, makecol(255, 0, 0), -1);
           if (key[KEY_ESC]) {
               break;
           }
           rest(16);
           continue;
       }


       clear_to_color(buffer, makecol(3, 27, 117));


       if (key[KEY_UP] && player_y - PLAYER_RADIUS > 0) { player_y -= 4; }
       if (key[KEY_DOWN] && player_y + PLAYER_RADIUS < SCREEN_HEIGHT) { player_y += 4; }
       if (key[KEY_LEFT] && player_x - PLAYER_RADIUS > SCREEN_WIDTH / 4) { player_x -= 4; }
       if (key[KEY_RIGHT] && player_x + PLAYER_RADIUS < (SCREEN_WIDTH * 3) / 4) { player_x += 4; }
       if (key[KEY_SPACE] && bullet_cooldown == 0) { shoot_bullet(player_x, player_y + 5, 0); }
      
       if (key[KEY_ESC]) { break; }


       //player_y += 1;
       if (player_y + PLAYER_RADIUS >= SCREEN_HEIGHT) {
           player_y = SCREEN_HEIGHT - PLAYER_RADIUS;
       }


       spawn_enemy();
       update_bullets();
       update_enemies();
       check_bullet_enemy_collisions();
       check_player_bullet_collision();
       check_player_enemy_collision();


       circlefill(buffer, player_x, player_y, PLAYER_RADIUS, makecol(115, 245, 213));


       draw_bullets(buffer);
       draw_enemies(buffer);


       rectfill(buffer, 0, 0, SCREEN_WIDTH / 4, SCREEN_HEIGHT, makecol(59, 68, 75));
       rectfill(buffer, (SCREEN_WIDTH * 3) / 4, 0, SCREEN_WIDTH, SCREEN_HEIGHT, makecol(59, 68, 75));
       show_enemy_counter(buffer);


       blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


       rest(16);
   }


   destroy_bitmap(buffer);
   allegro_exit();
   return 0;
}
END_OF_MAIN()
