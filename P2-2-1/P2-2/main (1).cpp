// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int pOm, Om;
    int run;
    int state;
    int ball, arrow;
    int start;
    int lives;
    // You can add other properties for the player here
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
int get_action(GameInputs inputs)
{
    Player.pOm = Player.Om;
    if(inputs.b1){
        Player.Om = 1;
    }else{
        Player.Om = 0;
    }
    
    if(inputs.b2){
        return ACTION_BUTTON;
    }
    if(inputs.b3){
        Player.run += 1;
        if (Player.run > 2){
            Player.run = 0;
        }
    }
    
    double x = inputs.ax;
    double y = inputs.ay;
    if(abs(x) > abs(y)){
        if(x<-0.4){
            return GO_RIGHT;
        }else{
            return GO_LEFT;
        }
    }else{
        if(y<-0.4){
            return GO_DOWN;
        }else{
            return GO_UP;
        }
    }

}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2

void go_up(){
    MapItem* item = get_north(Player.x, Player.y);
    MapItem* check = get_here(Player.x, Player.y);
    if((item->walkable||item == NULL||Player.Om) && Player.run == 0){
        Player.py = Player.y;
        Player.y = Player.y + 1;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 1){
        Player.py = Player.y;
        Player.y = Player.y + 3;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 2){
        Player.py = Player.y;
        Player.y = Player.y + 1;
        wait(0.5);
    }
    if(check -> type == FIRE){
        Player.lives = Player.lives - 1;
        if(Player.lives <1){
            uLCD.locate(2,4);
            uLCD.textbackground_color(BLACK);
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.printf("YOU");
            uLCD.locate(2,6);
            uLCD.printf("LOST");
            wait(3);
            speaker.period(1.0/100.0); 
            speaker =0.25; 
            wait(.05);
            speaker=0.0; 
            wait(0.5);
            Player.state =0;
            Player.has_key = 0;
            Player.start = 0;
            Player.ball = 0;
            Player.arrow = 0;
            Player.lives = 6;
            set_active_map(0);
        }
    }
}

void go_down(){
    MapItem* item = get_south(Player.x, Player.y);
    MapItem* check = get_here(Player.x, Player.y);
    if((item->walkable||item == NULL||Player.Om) && Player.run == 0){
        Player.py = Player.y;
        Player.y = Player.y - 1;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 1){
        Player.py = Player.y;
        Player.y = Player.y - 3;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 2){
        Player.py = Player.y;
        Player.y = Player.y - 1;
        wait(0.5);
    }
    if(check -> type == FIRE){
        Player.lives = Player.lives - 1;
        if(Player.lives <1){
            uLCD.locate(2,4);
            uLCD.textbackground_color(BLACK);
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.printf("YOU");
            uLCD.locate(2,6);
            uLCD.printf("LOST");
            wait(3);
            speaker.period(1.0/100.0); 
            speaker =0.25; 
            wait(.05);
            speaker=0.0; 
            wait(0.5);
            Player.state =0;
            Player.has_key = 0;
            Player.start = 0;
            Player.ball = 0;
            Player.arrow = 0;
            Player.lives = 6;
            set_active_map(0);
        }
    }
}

void go_right(){
    MapItem* item = get_east(Player.x, Player.y);
    MapItem* check = get_here(Player.x, Player.y);
    if((item->walkable||item == NULL||Player.Om) && Player.run == 0){
        Player.px = Player.x;
        Player.x = Player.x + 1;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 1){
        Player.px = Player.x;
        Player.x = Player.x + 3;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 2){
        Player.px = Player.x;
        Player.x = Player.x + 1;
        wait(0.5);
    }
    if(check -> type == FIRE){
        Player.lives = Player.lives - 1;
        if(Player.lives <1){
            uLCD.locate(2,4);
            uLCD.textbackground_color(BLACK);
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.printf("YOU");
            uLCD.locate(2,6);
            uLCD.printf("LOST");
            wait(3);
            speaker.period(1.0/100.0); 
            speaker =0.25; 
            wait(.05);
            speaker=0.0; 
            wait(0.5);
            Player.state =0;
            Player.has_key = 0;
            Player.start = 0;
            Player.ball = 0;
            Player.arrow = 0;
            Player.lives = 6;
            set_active_map(0);
        }
    }
}

void go_left(){
    MapItem* item = get_west(Player.x, Player.y);
    MapItem* check = get_here(Player.x, Player.y);
    if((item->walkable||item == NULL||Player.Om) && Player.run == 0){
        Player.px = Player.x;
        Player.x = Player.x - 1;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 1){
        Player.px = Player.x;
        Player.x = Player.x - 3;
    }else if((item->walkable||item == NULL||Player.Om) && Player.run == 2){
        Player.px = Player.x;
        Player.x = Player.x - 1;
        wait(0.5);
    }
    if(check -> type == FIRE){
        Player.lives = Player.lives - 1;
        if(Player.lives <1){
            uLCD.locate(2,4);
            uLCD.textbackground_color(BLACK);
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.printf("YOU");
            uLCD.locate(2,6);
            uLCD.printf("LOST");
            wait(3);
            speaker.period(1.0/100.0); 
            speaker =0.25; 
            wait(.05);
            speaker=0.0; 
            wait(0.5);
            Player.state =0;
            Player.has_key = 0;
            Player.start = 0;
            Player.ball = 0;
            Player.arrow = 0;
            Player.lives = 6;
            set_active_map(0);
        }
    }
}

int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP: 
            go_up();
            break;
        case GO_LEFT:
            go_left();
            break;            
        case GO_DOWN:
            go_down();   
            break;
        case GO_RIGHT:
            go_right();
            break;
        case ACTION_BUTTON: 
            MapItem* up = get_north(Player.x, Player.y);
            MapItem* down = get_south(Player.x, Player.y);
            MapItem* left = get_west(Player.x, Player.y);
            MapItem* right = get_east(Player.x, Player.y);
            MapItem* here = get_here(Player.x, Player.y);
            if (up->type == DOOR ||down->type == DOOR ||right->type == DOOR ||left->type == DOOR  ){
                set_active_map(1);
            }
            if (up->type == NPC ||down->type == NPC ||right->type == NPC ||left->type == NPC  ){
                if (!Player.state){
                    const char* line[] = {"Welcome, warrior", "bring the key", "I will teach", "you spells"};
                    long_speech(line, 4);
                    speaker.period(1.0/150.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    speaker.period(1.0/200.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    speaker.period(1.0/300.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    
                    Player.state = 1;
                }else if(Player.state == 1&&!Player.has_key){
                    const char* line[] = {"you don't have", "the key"};
                    long_speech(line, 2);
                    speaker.period(1.0/300.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    speaker.period(1.0/150.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    speaker.period(1.0/300.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    speaker.period(1.0/400.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                }else if(Player.state == 1&&Player.has_key){
                    const char* line[] = {"Great! you have", "the key", "I'll teach","you the spell","to fight","the boss","you can now","use the spell","when you fight","stand next", "to the pattern", "and press the", "middle and","top botton"};
                    long_speech(line, 14);
                    Player.state = 2;
                    speaker.period(1.0/220.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    speaker.period(1.0/160.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                    speaker.period(1.0/1000.0); 
                    speaker =0.25; 
                    wait(.02);
                    speaker=0.0; 
                    wait(0.5);
                }
            }
            if (up->type == KEY ||down->type == KEY ||right->type == KEY ||left->type == KEY  ){
                Player.has_key = 1;
                map_erase(10, 16);
            }
            if ((up->type == BALL ||down->type == BALL ||right->type == BALL ||left->type == BALL ||here->type == BALL) && Player.state == 1){
                const char* line[] = {"go learn the","spell"};
                long_speech(line, 2); 
            }
            if ((up->type == BALL ||down->type == BALL ||right->type == BALL ||left->type == BALL ||here->type == BALL) && Player.state == 0){
                const char* line[] = {"go get the","key"};
                long_speech(line, 2); 
            }
            if ((up->type == BALL ||down->type == BALL ||right->type == BALL ||left->type == BALL ||here->type == BALL) && Player.state == 2 && !Player.arrow){
                const char* line[] = {"Fire ball!", "now use", "fire arrow to", "kill the boss"};
                long_speech(line, 4);
                Player.ball = 1;
            }
            if ((up->type == BALL ||down->type == BALL ||right->type == BALL ||left->type == BALL ||here->type == BALL) && Player.state == 3 && !Player.arrow){
                const char* line[] = {"Fire ball!", "now use", "fire arrow to", "kill the boss"};
                long_speech(line, 4);
                Player.ball = 1;
            }
            if ((up->type == BALL ||down->type == BALL ||right->type == BALL ||left->type == BALL ||here->type == BALL) && Player.state == 2 && Player.arrow && !Player.ball){
                const char* line[] = {"Fire ball!", "now you", "killed the", "boss","go fight the","next one"};
                long_speech(line, 6);
                Player.arrow = 0;
                Player.state = 3;
            }
            if ((up->type == BALL ||down->type == BALL ||right->type == BALL ||left->type == BALL ||here->type == BALL) && Player.state == 3 && Player.arrow && !Player.ball){
                const char* line[] = {"Fire ball!", "now you", "killed the", "boss"};
                long_speech(line, 4);
                Player.ball = 1;
                Player.state = 3;
                uLCD.text_width(2); //4X size text
                uLCD.text_height(2);
                uLCD.locate(1,1);
                uLCD.color(RED);
                uLCD.printf("YOU WIN");
                wait(1);
                speaker.period(1.0/300.0); 
                speaker =0.25; 
                wait(.02);
                speaker=0.0; 
                wait(0.5);
                speaker.period(1.0/300.0); 
                speaker =0.25; 
                wait(.02);
                speaker=0.0; 
                wait(0.5);
                speaker.period(1.0/300.0); 
                speaker =0.25; 
                wait(.02);
                speaker=0.0; 
                wait(0.5);
                Player.state =0;
                Player.has_key = 0;
                Player.start = 0;
                Player.ball = 0;
                Player.arrow = 0;
                Player.lives = 6;
                set_active_map(0);
            }
            if ((up->type == ARROW ||down->type == ARROW ||right->type == ARROW ||left->type == ARROW ||here->type == ARROW) && Player.state == 1){
                const char* line[] = {"go learn the","spell"};
                long_speech(line, 2); 
            }
            if ((up->type == ARROW ||down->type == ARROW ||right->type == ARROW ||left->type == ARROW ||here->type == ARROW) && Player.state == 0){
                const char* line[] = {"go get the","key"};
                long_speech(line, 2); 
            }
            if ((up->type == ARROW ||down->type == ARROW ||right->type == ARROW ||left->type == ARROW ||here->type == ARROW) && Player.state == 2 && !Player.ball){
                const char* line[] = {"Fire arrow!", "now use", "fire ball to", "kill the boss"};
                long_speech(line, 4);
                Player.arrow = 1;
            }
            if ((up->type == ARROW ||down->type == ARROW ||right->type == ARROW ||left->type == ARROW ||here->type == ARROW) && Player.state == 3 && !Player.ball){
                const char* line[] = {"Fire arrow!", "now use", "fire ball to", "kill the boss"};
                long_speech(line, 4);
                Player.arrow = 1;
            }
            if ((up->type == ARROW ||down->type == ARROW ||right->type == ARROW ||left->type == ARROW ||here->type == ARROW) && Player.state == 2 && !Player.arrow && Player.ball){
                const char* line[] = {"Fire arrow!", "now you", "killed the", "boss","go fight the","next one"};
                long_speech(line, 6);
                Player.ball = 0;
                Player.state = 3;
            }
            if ((up->type == ARROW ||down->type == ARROW ||right->type == ARROW ||left->type == ARROW ||here->type == ARROW) && Player.state == 3 && !Player.arrow && Player.ball){
                const char* line[] = {"Fire arrow!", "now you", "killed the", "boss"};
                long_speech(line, 4);
                Player.arrow = 1;
                Player.state = 3;
                uLCD.text_width(4); //4X size text
                uLCD.text_height(4);
                uLCD.locate(1,1);
                uLCD.color(RED);
                uLCD.printf("YOU WIN");
                wait(1);
                speaker.period(1.0/300.0); 
                speaker =0.25; 
                wait(.02);
                speaker=0.0; 
                wait(0.5);
                speaker.period(1.0/300.0); 
                speaker =0.25; 
                wait(.02);
                speaker=0.0; 
                wait(0.5);
                speaker.period(1.0/300.0); 
                speaker =0.25; 
                wait(.02);
                speaker=0.0; 
                wait(0.5);
                Player.state =0;
                Player.has_key = 0;
                Player.start = 0;
                Player.ball = 0;
                Player.arrow = 0;
                Player.lives = 6;
                set_active_map(0);
            }
            break;
        case MENU_BUTTON:
            break;
        default:        break;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    if(Player.start == 1){
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }
    }else {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(2,4);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(RED);
        uLCD.printf("START");
        GameInputs inputs;
        speaker.period(1.0/100.0); 
        speaker =0.25; 
        wait(.02);
        speaker=0.0; 
        wait(0.5);
        speaker.period(1.0/500.0); 
        speaker =0.25; 
        wait(.02);
        speaker=0.0; 
        wait(0.5);
        speaker.period(1.0/800.0); 
        speaker =0.25; 
        wait(.02);
        speaker=0.0; 
        wait(0.5);
        
        if(inputs.b3){
            Player.start = 1;
            Player.lives = 6;
        }
    }

    // Draw status bars    
    draw_upper_status(Player.x, Player.y, Player.Om, Player.lives);
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
void init_main_map()
{
    // "Random" plants
    Map* map[2];
    map[0] = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
        
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    add_door(15, 16);
    add_npc(5, 15);
    add_key(10, 16);
    pc.printf("Walls done!\r\n");

    print_map();
    
    map[1] = set_active_map(1);
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_fire(i % map_width(), i / map_width());
    }
    add_boss(6, 10);
    add_arrow(6, 12);
    add_ball(6, 8);
    add_boss(10, 10);
    add_arrow(10, 12);
    add_ball(10, 8);
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
//   uLCD.filled_rectangle(64, 64, 74, 74, RED); //DELETE OR COMMENT THIS LINE

    // Initialize the maps
    maps_init();
    init_main_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;

    // Initial drawing
    draw_game(true);
    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actually do the game update:
        // 1. Read inputs     
        GameInputs input = read_inputs();   
        // 2. Determine action (get_action)
        int action = get_action(input);        
        // 3. Update game (update_game)
        update_game(action);
        // 3b. Check for game over
        
        // 4. Draw frame (draw_game)
        draw_game(true);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }

}
