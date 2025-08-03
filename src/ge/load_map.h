#ifndef LOAD_MAP_H
#define LOAD_MAP_H

struct p_initial_pos{
    float pX, pY, pZ, pA;
    char map[200], map_name[256], filename[256];
};

extern struct p_initial_pos pip;

void open_map();
int render_map();

#endif
