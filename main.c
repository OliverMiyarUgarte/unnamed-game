#include <allegro.h>
#include <stdio.h>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

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

  clear_to_color(screen, makecol(255, 0, 255));

  //Creates a border on the left and right side of the screen
  rectfill(screen, 0, 0, SCREEN_WIDTH/4, SCREEN_HEIGHT, makecol(59, 68, 75));
  rectfill(screen, (SCREEN_WIDTH*3)/4, 0, SCREEN_WIDTH, SCREEN_HEIGHT, makecol(59, 68, 75));

  int player_x = 100, player_y = 100;
  while (1) {
        int val;
        val = readkey();

        if ((val & 0xff) == 'w') {
            player_x += 5;
            allegro_message("You pressed 'w'\n");

        }

        circlefill(screen, player_x, player_y, 5, makecol(255, 0, 255));
    
        rest(16);  
  }
 
  readkey();
  allegro_exit();

  return 0;
}
END_OF_MAIN()