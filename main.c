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


typedef struct {
   int x, y;
   int active;
   int is_enemy_bullet;
} Bullet;


typedef struct {
   int x, y;
   int active;
   int shoot_cooldown;
   int health; 
} Enemy;




Bullet bullets[MAX_BULLETS];
Enemy enemies[MAX_ENEMIES];


int player_x = 100, player_y = 100;
int game_over = 0;
int bullet_cooldown = 0; 


void init_bullets() {
   for (int i = 0; i < MAX_BULLETS; i++) {
       bullets[i].active = 0;
       bullets[i].is_enemy_bullet = 0;
   }
}


void shoot_bullet(int x, int y, int is_enemy_bullet) {
   if (is_enemy_bullet == 0 && bullet_cooldown > 0) {
       return;
   }
  
   for (int i = 0; i < MAX_BULLETS; i++) {
       if (!bullets[i].active) {
           bullets[i].x = x;
           bullets[i].y = y;
           bullets[i].active = 1;
           bullets[i].is_enemy_bullet = is_enemy_bullet;
          
           if (is_enemy_bullet == 0) {
               bullet_cooldown = BULLET_COOLDOWN;
           }
           break;
       }
   }
}


void update_bullets() {
   if (bullet_cooldown > 0) {
       bullet_cooldown--;
   }
  
   for (int i = 0; i < MAX_BULLETS; i++) {
       if (bullets[i].active) {
           if (bullets[i].is_enemy_bullet) {
               bullets[i].y += 3;
               if (bullets[i].y > SCREEN_HEIGHT) {
                   bullets[i].active = 0;
               }
           }
           else {
               bullets[i].y -= 4;
               if (bullets[i].y < 0) {
                   bullets[i].active = 0;sudo git clone git@github.com:OliverMiyarUgarte/unnamed-game.git
Cloning into 'unnamed-game'...
git@github.com: Permission denied (publickey).
fatal: Could not read from remote repository.

Please make sure you have the correct access rights
               }
           }
       }
   }
}


void draw_bullets(BITMAP* buffer) {
   for (int i = 0; i < MAX_BULLETS; i++) {
       if (bullets[i].active) {
           if (bullets[i].is_enemy_bullet) {
               circlefill(buffer, bullets[i].x, bullets[i].y, BULLET_RADIUS, makecol(255, 50, 50));
           } else {
               circlefill(buffer, bullets[i].x, bullets[i].y, BULLET_RADIUS, makecol(254, 242, 85));
           }
       }
   }
}


void init_enemies() {
   for (int i = 0; i < MAX_ENEMIES; i++) {
       enemies[i].active = 0;
       enemies[i].shoot_cooldown = rand() % ENEMY_SHOOT_COOLDOWN; 
   }
}


void spawn_enemy() {
   static int spawn_timer = 0;
   spawn_timer++;
  
   if (spawn_timer >= 30) {
       spawn_timer = 0;
       for (int i = 0; i < MAX_ENEMIES; i++) {
           if (!enemies[i].active) {
               enemies[i].x = rand() % ((SCREEN_WIDTH * 3 / 4) - (SCREEN_WIDTH / 4)) + (SCREEN_WIDTH / 4);
               enemies[i].y = 0;  
               enemies[i].active = 1;
               enemies[i].shoot_cooldown = rand() % ENEMY_SHOOT_COOLDOWN;
               enemies[i].health = 3;
               break;
           }
       }
   }
}




void update_enemies() {
   for (int i = 0; i < MAX_ENEMIES; i++) {
       if (enemies[i].active) {
           int direction = rand() % 2;
           enemies[i].y += 1;
           if(direction){enemies[i].x += rand() % 4 - 1;}else{enemies[i].x -= rand() % 4 - 1;}   
          
           if (enemies[i].shoot_cooldown > 0) {
               enemies[i].shoot_cooldown--;
           } else {
                shoot_bullet(enemies[i].x, enemies[i].y + ENEMY_RADIUS, 1);
               enemies[i].shoot_cooldown = ENEMY_SHOOT_COOLDOWN + (rand() % 30);
           }
          
           if (enemies[i].y > SCREEN_HEIGHT) {
               enemies[i].active = 0;
           }
       }
   }
}


void draw_enemies(BITMAP* buffer) {
   for (int i = 0; i < MAX_ENEMIES; i++) {
       if (enemies[i].active) {
           circlefill(buffer, enemies[i].x, enemies[i].y, ENEMY_RADIUS, makecol(114, 12, 10));


           int health_bar_width = ENEMY_RADIUS * 2;
           int health_bar_height = 2;
           int health_bar_color = makecol(0, 255, 0); 
           if (enemies[i].health == 2) health_bar_color = makecol(255, 255, 0);
           if (enemies[i].health == 1) health_bar_color = makecol(255, 0, 0);


           rectfill(buffer, enemies[i].x - ENEMY_RADIUS, enemies[i].y - ENEMY_RADIUS - 5,
                    enemies[i].x - ENEMY_RADIUS + (enemies[i].health * health_bar_width / 3),
                    enemies[i].y - ENEMY_RADIUS - 3, health_bar_color);
       }
   }
}


int enemy_counter = 0;


void check_bullet_enemy_collisions() {
   for (int i = 0; i < MAX_BULLETS; i++) {
       if (bullets[i].active && !bullets[i].is_enemy_bullet) {
           for (int j = 0; j < MAX_ENEMIES; j++) {
               if (enemies[j].active) {
                   int dx = bullets[i].x - enemies[j].x;
                   int dy = bullets[i].y - enemies[j].y;
                   int distance_squared = dx * dx + dy * dy;
                   int collision_distance = (BULLET_RADIUS + ENEMY_RADIUS) * (BULLET_RADIUS + ENEMY_RADIUS);
                   if (distance_squared < collision_distance) { 
                       bullets[i].active = 0;
                       enemies[j].health--;  
                      
                       if (enemies[j].health <= 0) {
                           enemies[j].active = 0;
                           enemy_counter++;
                       }
                   }
               }
           }
       }
   }
}


void show_enemy_counter(BITMAP* buffer){
   for(int i = 0; i < MAX_ENEMIES; i++){
       textprintf_ex(buffer, font, 10, 10, makecol(0, 0, 0), -1, "Score: %d", enemy_counter);


       if(enemy_counter == 25){
           game_over = 1;
       }
   }
}


void check_player_bullet_collision() {
   for (int i = 0; i < MAX_BULLETS; i++) {
       if (bullets[i].active && bullets[i].is_enemy_bullet) {
           int dx = player_x - bullets[i].x;
           int dy = player_y - bullets[i].y;
           int distance_squared = dx * dx + dy * dy;
           int collision_distance = (PLAYER_RADIUS + BULLET_RADIUS) * (PLAYER_RADIUS + BULLET_RADIUS);
          
           if (distance_squared < collision_distance) {
               bullets[i].active = 0; 
               game_over = 1;
               return; 
           }
       }
   }
}


void check_player_enemy_collision() {
   for (int i = 0; i < MAX_ENEMIES; i++) {
       if (enemies[i].active) {
           int dx = player_x - enemies[i].x;
           int dy = player_y - enemies[i].y;
           int distance_squared = dx * dx + dy * dy;
           int collision_distance = (PLAYER_RADIUS + ENEMY_RADIUS) * (PLAYER_RADIUS + ENEMY_RADIUS);
          
           if (distance_squared < collision_distance) {
               game_over = 1;
           }
       }
   }
}


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
