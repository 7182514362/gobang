#include "LTexture.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

void Button::setPosition(int x, int y)
{
  mPosition.x = x;
  mPosition.y = y;
}

void Button::handleEvent(SDL_Event &e)
{
  // If mouse event happened
  if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN ||
      e.type == SDL_MOUSEBUTTONDOWN)
  {
    // Get mouse position
    SDL_GetMouseState(&mPosition.x, &mPosition.y);
  }
  // Set mouse over sprite
  switch (e.type)
  {
  case SDL_MOUSEBUTTONDOWN:
    mCurrentSprite = (mCurrentSprite == BUTTON_SPRITE_AIM_CROSS)
                         ? BUTTON_SPRITE_AIM_CIRCLE
                         : BUTTON_SPRITE_AIM_CROSS;
    break;
  case SDL_MOUSEBUTTONUP:
    break;
  }
  render();
}

void Button::render()
{
  // Show current button sprite
  mButtonSpriteSheetTexture.at(mCurrentSprite)
      ->render(mPosition.x, mPosition.y);
}

bool LTexture::loadFromFile(const char *path)
{
  // Get rid of preexisting texture
  free();
  if (!mRenderer)
  {
    std::cerr << "LTexture has not been properly initialized!\n";
  }
  // The final texture
  SDL_Texture *newTexture = nullptr;
  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path);
  if (loadedSurface == nullptr)
  {
    std::cerr << "Unable to load image " << path << "! SDL_image Error:" << IMG_GetError() << std::endl;
    exit(1);
  }
  else
  {
    // Color key image, The color key defines a pixel value that will be
    // treated as transparent in a blit.
    SDL_SetColorKey(loadedSurface, SDL_TRUE,
                    SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
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
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }
    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }
  // Return success
  mTexture = newTexture;
  return mTexture != nullptr;
}

bool LTexture::loadFromRenderedText(const char *textureText,
                                   SDL_Color textColor)
{
  // Get rid of preexisting texture
  free();
  if (!mFont || !mRenderer)
  {
    return false;
  }
  // Render text surface only once
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(mFont, textureText, textColor);
  if (!textSurface)
  {
    std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
  }
  else
  {
    // Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
    if (!mTexture)
    {
      std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
      exit(1);
    }
    else
    {
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }
    // Get rid of old surface
    SDL_FreeSurface(textSurface);
  }

  // Return success
  return mTexture != nullptr;
}

void LTexture::free()
{
  // Free texture if it exists
  if (mTexture != nullptr)
  {
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
  }
}

void LTexture::render(int x, int y, SDL_Rect *clip, Rotation *rot,
                     SDL_RendererFlip flip)
{
  // Set rendering space and render to screen
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};
  // Set clip rendering dimensions
  if (clip != nullptr)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  // render to screen
  SDL_RenderCopy(mRenderer, mTexture, clip, &renderQuad);
  // SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad,
  // rot->angle,rot->center, flip);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  // Modulate texture
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
  // Set blending function
  SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
  // Modulate texture alpha
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

/////////////////////////////////////////// TextureBase //////////////////////////////////////////

bool TextureBase::loadFromFile(const char *path)
{

  // Get rid of preexisting texture
  freeTexture();
  if (!mRenderer)
  {
    std::cerr << "Renderer has not been properly initialized!\n";
    exit(1);
  }
  // The final texture
  SDL_Texture *newTexture = nullptr;
  //newTexture=IMG_LoadTexture(mRenderer,path);
  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path);
  if (loadedSurface == nullptr)
  {
    std::cerr << "Unable to load image " << path << "! SDL_image Error:" << IMG_GetError() << std::endl;
    exit(1);
  }
  else
  {
    // Color key image, The color key defines a pixel value that will be
    // treated as transparent in a blit.
    SDL_SetColorKey(loadedSurface, SDL_TRUE,
                    SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
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
      mTextureW = loadedSurface->w;
      mTextureH = loadedSurface->h;
    }
    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }
  // Return success
  mTextureList->push_back(newTexture);

  return true;
}

void TextureBase::freeTexture()
{
  // Free texture if it exists
  for (auto i : *mTextureList)
  {
    SDL_DestroyTexture(i);
  }
  /*
  if (!mTextureList->empty())
  {
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
    mTextureW = 0;
    mTextureH = 0;
  }
  */
}

void TextureBase::render()
{
  SDL_Rect renderQuad = {mPosX, mPosY, mTextureW, mTextureH};

  // render to screen
  SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderQuad);
}

void TextureBase::setPos(int x, int y)
{
  mPosX = x;
  mPosY = y;
}