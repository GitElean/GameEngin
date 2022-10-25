#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>     

class STexture{
    public:
        STexture(SDL_Renderer* gRenderer, SDL_Window* gWindow);
        ~STexture();
        void load(std::string path);
        void setup();
        void render(int x, int y, int w, int h, SDL_Rect* clip=NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        int getWidth();
        int getHeight();

        void free();

        int getPitch();

    private:
        SDL_Renderer* renderer;
        SDL_Window* window;

        int tWidth = 0;
        int tHeight = 0;

        Uint32 format;  //ventana
        SDL_PixelFormat* mappingFormat;  //textura

        void* mPixels; 

        int mPitch;

        
        SDL_Texture* mTexture;
};

STexture::STexture(SDL_Renderer* gRenderer, SDL_Window* gWindow){

    mTexture = NULL;
	tWidth = 0;
	tHeight = 0;
	mPixels = NULL;
	mPitch = 0;


    renderer = gRenderer;
    window = gWindow;

    format = SDL_GetWindowPixelFormat(window);
    mappingFormat = SDL_AllocFormat(format);

}

STexture::~STexture(){
    SDL_FreeFormat(mappingFormat);//liberar memoria
}
 

void STexture::load(std::string path){
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, format, 0);

    mTexture = SDL_CreateTextureFromSurface(renderer, format, SDL_TEXTUREACCESS_STREAMING,
                                            formattedSurface->w, formattedSurface->h);
    //  newTexture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
    tWidth = formattedSurface->w;  
    tHeight = formattedSurface->h;
    mPitch = 0;


    SDL_LockTexture(mTexture, nullptr, &mPixels, &mPitch);

    memcpy(mPixels, formattedSurface->pixels,(formattedSurface -> pitch* formattedSurface -> h));


    SDL_UnlockTexture(mTexture);

    SDL_FreeSurface(loadedSurface);

    SDL_FreeSurface(formattedSurface);

       /*
    SDL_Surface* surface = IMG_Load("./assets/kroniireference1.png");
    

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr , &rec2);

    SDL_DestroyTexture(texture);*/
}

int STexture::getPitch(){
    
    return mPitch;
}



void STexture::render(int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{

    SDL_Rect renderQuad = {x, y, w, h};
    SDL_RenderCopyEx(renderer, mTexture, clip , &renderQuad, angle, center, flip);
}