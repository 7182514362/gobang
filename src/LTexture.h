#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <vector>

enum ButtonSprite
{

    BUTTON_SPRITE_AIM_CROSS = 0,
    BUTTON_SPRITE_AIM_CIRCLE = 1
};
class LTexture;

// The mouse button
class Button
{
public:
    // Initializes internal variables
    Button(std::vector<LTexture *> &sheet) : mButtonSpriteSheetTexture(sheet),
                                            mPosition({0, 0}),
                                            mCurrentSprite(BUTTON_SPRITE_AIM_CROSS){};
    ~Button() = default;
    // Sets top left position
    void setPosition(int x, int y);
    // Handles mouse event
    void handleEvent(SDL_Event &e);
    // Shows button sprite
    void render();

private:
    // Top left position
    SDL_Point mPosition;
    // Currently used global sprite
    ButtonSprite mCurrentSprite;
    std::vector<LTexture *> mButtonSpriteSheetTexture;
};

typedef struct
{
    double angle = 0.0;
    SDL_Point *center = nullptr;
} Rotation;

// LTexture wrapper class
class LTexture
{
public:
    LTexture(const LTexture &) = delete;
    LTexture &operator=(const LTexture &) = delete;
    // Initializes variables
    LTexture(SDL_Renderer *pRenderer, TTF_Font *font = nullptr)
        : mTexture(nullptr),
          mRenderer(pRenderer),
          mFont(font),
          mWidth(0),
          mHeight(0){};
    // Deallocates memory
    ~LTexture() { free(); };
    // Loads image at specified path
    bool loadFromFile(const char *path);
    // Creates image from font string
    bool loadFromRenderedText(const char *textureText, SDL_Color textColor);
    // Deallocates texture
    void free();
    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    // Set blending
    void setBlendMode(SDL_BlendMode blending);
    // Set alpha modulation
    void setAlpha(Uint8 alpha);
    // Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = nullptr, Rotation *rot = nullptr,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
    // Gets image dimensions
    int getWidth() { return mWidth; };
    int getHeight() { return mHeight; };

    void setWidth(const int w) { mWidth = w; };
    void setHeight(const int h) { mHeight = h; };

private:
    // The actual hardware texture
    SDL_Texture *mTexture;
    SDL_Renderer *mRenderer;
    TTF_Font *mFont;
    // Image dimensions
    int mWidth;
    int mHeight;
};



class TextureBase
{
public:
    explicit TextureBase(SDL_Renderer *r = nullptr) : mRenderer(r), mTextureList(new std::vector<SDL_Texture *>()),
                                                      mTextureH(0), mTextureW(0), mPosX(0), mPosY(0){};
    ~TextureBase() { freeTexture(); };
    bool loadFromFile(const char *path);
    void freeTexture();

    virtual void render();

    void setPos(int x, int y);

protected:
    // Texture render position
    int mPosX;
    int mPosY;
    // Texture demisions
    int mTextureW;
    int mTextureH;
    SDL_Renderer *mRenderer;
    std::vector<SDL_Texture *> *mTextureList;
};

#endif
