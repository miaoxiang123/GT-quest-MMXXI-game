#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 * In this file you need to define how the map will be structured. IE how will
 * you put values into the map, pull them from the map. Remember a "Map" variable
 * is a hashtable plus two ints (see below) 
 * You will have more than one map variable, one will be the main map with it's own hashtable.
 * Then you'll have a second map with another hashtable
 * You should store objects into the hashtable with different properties (spells
 * etc)
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define BUCKETS 100

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map[2];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // convert 2 d to 1 d by x + y * width
    return X+Y*(map_width());
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return(key % BUCKETS);
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
    for (int i = 0; i < 2; i++){
        map[i].items = createHashTable(map_hash, BUCKETS);
        map[i].w = 50;
        map[i].h = 50;
    }
    active_map = 0;
}

Map* get_active_map()
{
    // There's only one map
    return &map[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return get_active_map();
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return get_active_map() -> w;
}

int map_height()
{
    return get_active_map() -> h;
}

int map_area()
{
    return map_width() * map_height();
}

MapItem* get_north(int x, int y)
{
    return get_here(x, y-1);
}

MapItem* get_south(int x, int y)
{
    return get_here(x, y+1);
}

MapItem* get_east(int x, int y)
{
    return get_here(x+1, y);
}

MapItem* get_west(int x, int y)
{
    return get_here(x-1, y);
}

MapItem* get_here(int x, int y)
{
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y));
}


void map_erase(int x, int y)
{
    MapItem* item = get_here(x, y);
    unsigned key = XY_KEY(x, y);
    if (item && item -> data != NULL){
        free(item-> data);
    }
    deleteItem(get_active_map()->items, key);
    
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_npc(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_npc;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_key(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_fire(int x, int y)
{
    MapItem* w2 = (MapItem*) malloc(sizeof(MapItem));
    w2->type = FIRE;
    w2->draw = draw_fire;
    w2->walkable = true;
    w2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w2);
    if (val) free(val); // If something is already there, free it
}

void add_boss(int x, int y)
{
    MapItem* w2 = (MapItem*) malloc(sizeof(MapItem));
    w2->type = BOSS;
    w2->draw = draw_boss;
    w2->walkable = true;
    w2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w2);
    if (val) free(val); // If something is already there, free it
}

void add_ball(int x, int y)
{
    MapItem* w2 = (MapItem*) malloc(sizeof(MapItem));
    w2->type = BALL;
    w2->draw = draw_ball;
    w2->walkable = true;
    w2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w2);
    if (val) free(val); // If something is already there, free it
}

void add_arrow(int x, int y)
{
    MapItem* w2 = (MapItem*) malloc(sizeof(MapItem));
    w2->type = ARROW;
    w2->draw = draw_arrow;
    w2->walkable = true;
    w2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w2);
    if (val) free(val); // If something is already there, free it
}