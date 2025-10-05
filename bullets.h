typedef struct {
   int x, y;
   int active;
   int is_enemy_bullet;
} Bullet;

Bullet bullets[MAX_BULLETS];

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
                   bullets[i].active = 0;
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
