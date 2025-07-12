#ifndef MAIN_H
#define MAIN_H

struct flagstruct{
    int fflag, dflag;
};

struct player_config{
    char player_name[64], language_id[64];
    int res_x, res_y;
    int fullscreen;
};

struct flagstruct flags;
struct player_config player_conf;

#endif
