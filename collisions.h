
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