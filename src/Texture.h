#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>

struct Rotation
{
    double angle;
    SDL_Point center;
};

class Texture
{
public:
    SDL_Texture *mTexture;
    SDL_Renderer *mRenderer;
    int w, h;
    unsigned char alpha;
    Uint32 colorKey;

    SDL_Rect srcRect;
    SDL_Rect dstRect;
    Rotation rotation;

    Texture(SDL_Renderer *);
    Texture() = delete;
    ~Texture();

    void loadFromFile(const char *path);
    void setAlpha(unsigned char);
};



#endif