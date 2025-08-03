#ifndef LOAD_TEXTURES_H
#define LOAD_TEXTURES_H

extern GLuint textures[64];
extern GLuint textureID;

GLuint load_texture(const char* texture_filename);
void load_all_tex();

#endif
