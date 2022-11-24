#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

//Texture wrapper class
class STexture
{
	public:
		//Initializes variables
		STexture(SDL_Renderer* gRenderer, SDL_Window* gWindow);
		STexture(SDL_Renderer* gRenderer);

		//Deallocates memory
		~STexture();

		//Loads image at specified path
		void load(std::string path);

		void executeShader(Uint32(*func)(Uint32));

		//Deallocates texture
		void free();
		
		// Renders texture at given point
		void render(int x, int y, int w = 0, int h = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		void renderWithShader(
      int x, int y, int w, int h,
      Uint32(*func)(Uint32)
    );

    // Get a single color (using current mapping)
		Uint32 color(Uint8 red, Uint8 green, Uint8 blue);

		//Gets image dimensions
		int getWidth();
		int getHeight();

		//Pixel manipulators
		bool lockTexture();
		bool unlockTexture();
		Uint32* getPixels();
		int getPitch();
    int getPixelCount();
    Uint32 getPixel(int x, int y);
    Uint32 getPixel(int xy);

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		void* mPixels;
		int mPitch;
    Uint32 format;
    SDL_PixelFormat* mappingFormat;

		//Image dimensions
		int mWidth;
		int mHeight;

    // renderer
    SDL_Renderer *renderer;
};

STexture::STexture(SDL_Renderer* gRenderer, SDL_Window* gWindow)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;

  renderer = gRenderer;

  format = SDL_GetWindowPixelFormat(gWindow);
  mappingFormat = SDL_AllocFormat(format);
}

STexture::STexture(SDL_Renderer* gRenderer)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;

  renderer = gRenderer;

  format = SDL_PIXELFORMAT_RGBA8888;
  mappingFormat = SDL_AllocFormat(format);
}

STexture::~STexture()
{
  SDL_FreeFormat(mappingFormat);
  SDL_DestroyTexture(mTexture);

  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  mPixels = NULL;
  mPitch = 0;
}

void STexture::executeShader(Uint32(*func)(Uint32))
{
	// texture must be loaded first

	lockTexture();
  int pixelCount = getPixelCount();
  Uint32* pixels = getPixels();

  //Color key pixels
  
  for( int i = 0; i < pixelCount; ++i)
  {
    pixels[i] = func(pixels[i]);
  }
  
  unlockTexture();
}

void STexture::load(std::string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, format, 0);
  newTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, loadedSurface->w, loadedSurface->h);
  // newTexture = SDL_CreateTextureFromSurface(renderer, formattedSurface);

  SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);

  // Copy loaded/formatted surface pixels
  memcpy(mPixels, formattedSurface->pixels, mPitch * formattedSurface->h);

  SDL_UnlockTexture(newTexture);

  mPixels = NULL;
  mWidth = loadedSurface->w;
  mHeight = loadedSurface->h;
  
  SDL_FreeSurface(formattedSurface);
  SDL_FreeSurface(loadedSurface);
  
	mTexture = newTexture;
}

Uint32 STexture::color(Uint8 red, Uint8 green, Uint8 blue)
{
  return SDL_MapRGB(mappingFormat, red, green, blue);
}

void STexture::render(int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
  int rWidth = mWidth;
  int rHeight = mHeight;

  if (w != 0) {
    rWidth = w;
  }

  if (h != 0) {
    rHeight = h;
  }

	SDL_Rect renderQuad = { x, y, rWidth, rHeight };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}

void STexture::renderWithShader(int x, int y, int w, int h, Uint32(*func)(Uint32))
{
	SDL_Rect renderQuad = { x, y, w, h };

	lockTexture();
  Uint32* pixels = getPixels();  

  void* copyPixels;
  int copyPitch;
  SDL_Texture* copyTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, mWidth, mHeight);

  SDL_LockTexture(copyTexture, NULL, &copyPixels, &copyPitch);
  Uint32* copyPixelsInt = (Uint32*) copyPixels;

  for(int i = 0; i < (mWidth * mHeight); ++i)
  {
    copyPixelsInt[i] = func(pixels[i]);
  }

  SDL_UnlockTexture(copyTexture);
  unlockTexture();

	SDL_RenderCopy(renderer, copyTexture, NULL, &renderQuad);
}


int STexture::getWidth()
{
	return mWidth;
}

int STexture::getHeight()
{
	return mHeight;
}

bool STexture::lockTexture()
{
	if(mPixels != NULL)
	{
    // texture is already locked
    return false;
	}

  SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch);
	return true;
}

bool STexture::unlockTexture()
{
	if(mPixels == NULL)
	{
    // texture is not locked
    return true;
	}

  SDL_UnlockTexture(mTexture);
  mPixels = NULL;
  mPitch = 0;

  return true;
}

Uint32* STexture::getPixels()
{
	return (Uint32*) mPixels;
}

int STexture::getPitch()
{
	return mPitch;
}

int STexture::getPixelCount()
{
	return (getPitch() / 4) * getHeight();
}

Uint32 STexture::getPixel(int x, int y)
{
  //lockTexture();
  Uint32* pixels = getPixels();
  Uint32 pixel = pixels[(y * getPitch() / 4) + x];
  //unlockTexture();

	return pixel;
}

Uint32 STexture::getPixel(int xy)
{
  lockTexture();
  Uint32* pixels = getPixels();
  Uint32 pixel = pixels[xy];
  unlockTexture();

	return pixel;
}
