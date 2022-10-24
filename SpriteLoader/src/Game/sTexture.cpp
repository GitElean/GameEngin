#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>     

class sTexture{
    public:
        sTexture(SDL_Renderer* gRenderer, SDL_Window* gWindow);
        ~sTexture();
        load(std::string path);
        setup();
        render(SDL_Rect* dst, SDL_Rect* clip=NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    private:
        SDL_Renderer* renderer;
        SDL_Window* window;

}