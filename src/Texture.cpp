#include "Texture.h"
#include <iostream>

Texture::Texture(SDL_Renderer *r) : mTexture(nullptr), mRenderer(r), w(0), h(0),
                                    srcRect({0, 0, 0, 0}), colorKey(0),
                                    dstRect({0, 0, 0, 0}), rotation({0.0, {0, 0}}) {}

Texture::~Texture()
{
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
};

void Texture::loadFromFile(const char *path)
{
    if (!mRenderer)
    {
        std::cerr << "Texture has not been properly initialized!\n";
    }
    // The final texture
    SDL_Texture *newTexture = nullptr;
    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        std::cerr << "Unable to load image " << path << "! SDL_image Error:" << IMG_GetError() << std::endl;
        exit(1);
    }
    else
    {
        // Color key image, The color key defines a pixel value that will be
        // treated as transparent in a blit.
        // SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF)
        colorKey = SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF);
        SDL_SetColorKey(loadedSurface, SDL_TRUE,colorKey);
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
        if (newTexture == nullptr)
        {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
            exit(1);
        }
        else
        {
            // Get image dimensions
            w = loadedSurface->w;
            h = loadedSurface->h;
            dstRect.w=w;
            dstRect.h=h;
        }
        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    // Return success
    mTexture = newTexture;
}

void Texture::setAlpha(unsigned char aAlpha){
    SDL_SetTextureAlphaMod(mTexture,aAlpha);
}