#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "../lib/stb_image.h"
#include "load_textures.h"

GLuint textures[64];

GLuint load_texture(const char* texture_filename){
    GLuint textureID;
    glGenTextures(1, &textureID);
    if(!textureID){
        printf("Error: glGenTextures devolvió 0\n");
        return 0;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texture_filename, &width, &height, &nrChannels, 0);
    if(!data){
        printf("Error loading image: %s\n", texture_filename);
        return 0;
    }

    GLenum format;
    if(nrChannels == 1) format = GL_RED;
    else if(nrChannels == 3) format = GL_RGB;
    else if(nrChannels == 4) format = GL_RGBA;
    else{
        printf("Unsupported channels: %d\n", nrChannels);
        stbi_image_free(data);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    GLenum err = glGetError();
    if(err != GL_NO_ERROR){
        printf("glTexImage2D error: %u\n", err);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    err = glGetError();
    if(err != GL_NO_ERROR) printf("glGenerateMipmap error: %u\n", err);

    stbi_image_free(data);
    return textureID;
}

void load_all_tex(){
    textures[1] = load_texture("build/textures/wall/concrete.png");
}
