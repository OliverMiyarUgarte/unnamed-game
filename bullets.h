#include "fila.h"

typedef struct {
   int x, y;
   int active;
   int is_enemy_bullet;
   int speedx;
   int speedy;
   int radius;
   int cooldown;
   int damage;
} Bullet;

Bullet btypes[3];
Bullet bullets[MAX_BULLETS];
fila pbullets[2];

int atual = 0;
int ativa = 0;
int reload_time = 60;
int deucerto;

void init_bullets() {
   for (int i = 0; i < MAX_BULLETS; i++) {
       bullets[i].active = 0;
       bullets[i].is_enemy_bullet = 0;
   }


   Cria(&pbullets[0]);
   Cria(&pbullets[1]);

   //setup bullet types
   btypes[0].speedx = 0; 
   btypes[0].speedy = 4; 
   btypes[0].radius = 3;
   btypes[0].cooldown = 5;
   btypes[0].damage = 1;

   btypes[1].speedx = 0; 
   btypes[1].speedy = 2; 
   btypes[1].radius = 5;
   btypes[1].cooldown = 10;
   btypes[1].damage = 2;

   btypes[2].speedx = 0;
   btypes[2].speedy = 7; 
   btypes[2].radius = 1;
   btypes[2].cooldown = 2; 
   btypes[2].damage = 1;
}

void selectbullet(int i){
    int x;
    Insere(&pbullets[0], i, &x);
}

void deselectbullet(){
    fila aux;
    int bullet;
    int ok;
    int n = 0;
    while(!Vazia(&pbullets[0])){
        Retira(&pbullets[0], &bullet, &ok);
        Insere(&aux, bullet, &ok);
        n++;
    }

    while (n>1){
        Retira(&aux, &bullet, &ok);
        Insere(&pbullets[0], bullet, &ok);
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

           int deucerto = 1;

           if (is_enemy_bullet) {
               int rand_index = rand() % 3;
               bullets[i].speedx = btypes[rand_index].speedx;
               bullets[i].speedy = btypes[rand_index].speedy;
               bullets[i].radius = btypes[rand_index].radius;
               bullets[i].cooldown = btypes[rand_index].cooldown;
               bullets[i].damage = btypes[atual].damage;
           } else { // player bullet
               if (Vazia(&pbullets[0]) && Vazia(&pbullets[1])) { // no bullet selected
                    bullets[i].active = 0;
                   break;
               }

               if (!Vazia(&pbullets[ativa])) {
                   Retira(&pbullets[ativa], &atual, &deucerto);
                   Insere(&pbullets[1 - ativa], atual, &deucerto);
                    bullet_cooldown = btypes[atual].cooldown;
                    bullets[i].speedx = btypes[atual].speedx;
                    bullets[i].speedy = btypes[atual].speedy;
                    bullets[i].radius = btypes[atual].radius;
                    bullets[i].cooldown = btypes[atual].cooldown;
                    bullets[i].damage = btypes[atual].damage;
               } else {
                   ativa = 1 - ativa; 
                   bullet_cooldown = reload_time;
                   bullets[i].active = 0;
               }
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
               bullets[i].x += bullets[i].speedx;
               if (bullets[i].x > SCREEN_HEIGHT) {
                   bullets[i].active = 0;
               }
                bullets[i].y += bullets[i].speedy;
               if (bullets[i].y > SCREEN_WIDTH) {
                   bullets[i].active = 0;
               }
           }
           else {
               bullets[i].x -= bullets[i].speedx;
               if (bullets[i].x < 0) {
                   bullets[i].active = 0;
               }
               bullets[i].y -= bullets[i].speedy;
               if (bullets[i].y < 0) {
                   bullets[i].active = 0;
               }
           }
       }
   }
}


void draw_bullets(BITMAP* buffer, BITMAP* enemy_bullet, BITMAP* playerBullet) {
   for (int i = 0; i < MAX_BULLETS; i++) {
       if (bullets[i].active) {
           if (bullets[i].is_enemy_bullet) {
               circlefill(buffer, bullets[i].x, bullets[i].y, BULLET_RADIUS, makecol(255, 50, 50));
           } else {
               masked_blit(playerBullet, buffer, 0, 0, bullets[i].x, bullets[i].y, playerBullet->w, playerBullet->h);
           }
       }
   }
}
