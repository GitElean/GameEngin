#include <vector>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "./STexture.cpp"
#include "./Systems.h"
#include "./Components.hpp"


class MovementSystems : public UpdateSystems {
  private:
    int counter = 0;

  public:
    MovementSystems(int c) : counter(c) {}

    void run(double dT) override {
      const auto view = scene->r.view<TransformComponent, MovementComponent>();
      for (const entt::entity e : view) {
        TransformComponent& t = view.get<TransformComponent>(e);
        MovementComponent& m = view.get<MovementComponent>(e);

        if (t.position.x <= 0)
        {
          m.velocity.x *= -1;
        }

        if (t.position.x >= 640)
        {
          m.velocity.x *= -1;
        }

        if (t.position.y <= 0)
        {
          m.velocity.y *= -1;
        }

        if (t.position.y >= 480)
        {
          m.velocity.y *= -1;
        }

        t.position.x += m.velocity.x * dT;
        t.position.y += m.velocity.y * dT;

        std::cout << "x: " << t.position.x << std::endl;
        std::cout << "y: " << t.position.y << std::endl;
        std::cout << "c: " << counter++ << std::endl;
      }
    }
};

class CubeSystems : public RenderSystems {
  public:
    void run(SDL_Renderer* renderer) override {
      SDL_SetRenderDrawColor(renderer, 255, 100, 100, 1);

      const auto view = scene->r.view<TransformComponent, ColliderComponent>();
      for (const entt::entity e : view) {
        const TransformComponent& t = view.get<TransformComponent>(e);
        const ColliderComponent& c = view.get<ColliderComponent>(e);
        const int x = t.position.x;
        const int y = t.position.y;
        const int w = c.size.x;
        const int h = c.size.y;

        SDL_Rect rect = { x, y, w, h };    
        SDL_RenderFillRect(renderer, &rect);
      }
    }
};

class HelloSystems : public SetupSystems {
  public:
    HelloSystems() {
      std::cout << "Hello Systems Constructor" << std::endl;
    }

    ~HelloSystems() {
      std::cout << "Hello Systems Destructor" << std::endl;
    }

    void run() override {
      std::cout << "Hello Systems!" << std::endl;
    }
};


class TilemapSystems : public SetupSystems, public RenderSystems {
  private:
    SDL_Renderer* renderer;

    constexpr static int tilesCount = 2;
    constexpr static int tilesWidth = 4;
    constexpr static int tilesHeight = 3;

    constexpr static int x = 0;
    constexpr static int y = 0;
    constexpr static int tileWidth = 24;
    constexpr static int tileHeigth = 24;

    constexpr static char mmap[tilesWidth][tilesHeight] = {
      {0, 0, 0},
      {0, 1, 0},
      {0, 0, 0},
      {0, 0, 0}
    };
    const std::string files[tilesCount] = {
      "assets/tiles/grass.png",
      "assets/tiles/water.png"
    };

    std::map<char, SDL_Texture*> tiles;
    SDL_Texture** tilemap;

  public:
    TilemapSystems(SDL_Renderer* r) : renderer(r) {
      std::cout << "Tile map Systems started" << std::endl;
      tilemap = new SDL_Texture*[tilesWidth * tilesHeight];
    }

    ~TilemapSystems() {
      /*
      for(int i = 0; i < tilesCount; i++) {
        SDL_FreeSurface(surfaces[i]);
      }
      */
    }

    // setup
    void run() override {
      SDL_Surface* surfaces[tilesCount];

      for(int i = 0; i < tilesCount; i++) {
        surfaces[i] = IMG_Load(files[i].c_str());
      }
      
      for(int i = 0; i < tilesWidth; i++) {
        for(int j = 0; j < tilesHeight; j++) {
          char surfaceIndex = mmap[i][j];
          auto found = tiles.find(surfaceIndex);
          if (found == tiles.end()) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surfaces[surfaceIndex]);
            tiles.insert(
              std::make_pair(
                surfaceIndex,
                texture
              )
            );
            tilemap[i*tilesWidth + j] = texture;
          } else {
            tilemap[i*tilesWidth + j] = found->second;
          }
        }
      }

      for(int i = 0; i < tilesCount; i++) {
        SDL_FreeSurface(surfaces[i]);
      }
    }

    void run(SDL_Renderer* r) override {
      SDL_Rect rect = { x, y, tileWidth, tileHeigth };

      for(int i = 0; i < tilesWidth; i++) {
        for(int j = 0; j < tilesHeight; j++) {
          SDL_RenderCopy(r, tilemap[i*tilesWidth + j], nullptr, &rect);
          rect.x += tileWidth;
        }
        rect.x = 0;
        rect.y += tileHeigth;
      }
    }
};


class AdvancedTilemapSystems : public SetupSystems, public RenderSystems {
  private:
    SDL_Renderer* renderer;
    SDL_Window* window;

    constexpr static int tilesCount = 2;
    constexpr static int x = 0;
    constexpr static int y = 0;
    constexpr static int tileWidth = 32;
    constexpr static int tileHeigth = 32;

    const std::string mmap = "assets/tilemaps/1.png";
    const std::string files[tilesCount] = {
      "assets/tiles/grass.png",
      "assets/tiles/water.png"
    };
    std::map<char, SDL_Texture*> tiles;
    SDL_Texture** tilemap;
    int tilesWidth;
    int tilesHeight;

  public:
    AdvancedTilemapSystems(SDL_Renderer* r, SDL_Window* w) : renderer(r), window(w) {
      std::cout << "Tile map Systems started" << std::endl;
    }

    ~AdvancedTilemapSystems() {
    }

    // setup
    void run() override {
      SDL_Surface* surfaces[tilesCount];

      for(int i = 0; i < tilesCount; i++) {
        surfaces[i] = IMG_Load(files[i].c_str());
      }
  
      STexture* t = new STexture(renderer, window);
      t->load(mmap);
      tilesWidth = t->getWidth();
      tilesHeight = t->getHeight();
      int totalTiles = tilesWidth * tilesHeight;

      tilemap = new SDL_Texture*[totalTiles];

      for(int i = 0; i < totalTiles; i++) {
        Uint32 currentColor = t->getPixel(i);
        int surfaceIndex = ((int)(currentColor >> 16) & 0xff) - 1;

        auto found = tiles.find(surfaceIndex);
        if (found == tiles.end()) {
          SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surfaces[surfaceIndex]);
          tiles.insert(
            std::make_pair(
              surfaceIndex,
              texture
            )
          );
          tilemap[i] = texture;
        } else {
          tilemap[i] = found->second;
        }
      }

      delete t;

      for(int i = 0; i < tilesCount; i++) {
        SDL_FreeSurface(surfaces[i]);
      }
    }

    void run(SDL_Renderer* r) override {
      SDL_Rect rect = { x, y, tileWidth, tileHeigth };

      for(int i = 0; i < tilesHeight; i++) {
        for(int j = 0; j < tilesWidth; j++) {
          SDL_RenderCopy(r, tilemap[i*tilesWidth + j], nullptr, &rect);
          rect.x += tileWidth;
        }
        rect.x = 0;
        rect.y += tileHeigth;
      }
    }
};




class AdvancedTileSetSystems : public SetupSystems, public RenderSystems {
  private:
    SDL_Renderer* renderer;
    SDL_Window* window;

    constexpr static int dstTileSize = 32;
    constexpr static int srcTileSize = 16;
    
    const std::string mapfile = "assets/tilemaps/guratilemap.png";
    const std::string layerfiles[4] = {
      "assets/tiles/tile1.png",
      "assets/tiles/tile2.png",
      "assets/tiles/tile3.png",
      "assets/tiles/tile4.png"
    };

    SDL_Texture* tilesets[4];

    Tile* tilemap;
    int tilemapWidth;
    int tilemapHeight;
    
  public:
    AdvancedTileSetSystems(SDL_Renderer* r, SDL_Window* w) : renderer(r), window(w) {}

    ~AdvancedTileSetSystems() {}

    // setup
    void run() override {
      for(int i = 0; i < 3; i++) {
        SDL_Surface* surface = IMG_Load(layerfiles[i].c_str());
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        tilesets[i] = texture;
        SDL_FreeSurface(surface);
      }

      STexture* t = new STexture(renderer, window);
      t->load(mapfile);
      tilemapWidth = t->getWidth();
      tilemapHeight = t->getHeight();      
      tilemap = new Tile[tilemapWidth * tilemapHeight];

      t->lockTexture();
      for(int y = 0; y < tilemapHeight; y++) {
        for(int x = 0; x < tilemapWidth; x++) {
          Uint32 currentColor = t->getPixel(x, y);
          int r = ((int)(currentColor >> 16) & 0xff);
          int g = ((int)(currentColor >> 8) & 0xff);
          // std::cout << "rg: " << r << ", " << g << std::endl;
          tilemap[y*tilemapWidth + x].bottom = { r, g * srcTileSize, 0};
        }
      }
      t->unlockTexture();
      delete t;
    }

    void run(SDL_Renderer* r) override {
      SDL_Rect dst = { 0, 0, dstTileSize, dstTileSize };

      for(int y = 0; y < tilemapHeight; y++) {
        for(int x = 0; x < tilemapWidth; x++) {
          Tile tile = tilemap[y*tilemapWidth + x];
          SDL_Rect src = { tile.bottom.x, tile.bottom.y, srcTileSize, srcTileSize };
          SDL_RenderCopy(r,
            tilesets[tile.bottom.index],
            &src,
            &dst
          );
          dst.x += dstTileSize;
        }
        dst.x = 0;
        dst.y += dstTileSize;
      }
    }
};






    // setup
    void run() override {
      for(int i = 0; i < 3; i++) {
        SDL_Surface* surface = IMG_Load(layerfiles[i].c_str());
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        tilesets[i] = texture;
        SDL_FreeSurface(surface);
      }

      STexture* t = new STexture(renderer, window);
      t->load(mapfile);
      tilemapWidth = t->getWidth();
      tilemapHeight = t->getHeight();      
      tilemap = new Tile[tilemapWidth * tilemapHeight];

      t->lockTexture();
      for(int y = 0; y < tilemapHeight; y++) {
        for(int x = 0; x < tilemapWidth; x++) {
          Uint32 currentColor = t->getPixel(x, y);
          int r = ((int)(currentColor >> 16) & 0xff);
          int g = ((int)(currentColor >> 8) & 0xff);
          
          autoTiling2(t, x, y, &tilemap[y*tilemapWidth + x]);
        }
      }
      t->unlockTexture();
      delete t;
    }

    // autoTiling(t, x, y, &tilemap[i]);

    void run(SDL_Renderer* r) override {
      SDL_Rect dst = { 0, 0, dstTileSize, dstTileSize };

      for(int y = 0; y < tilemapHeight; y++) {
        for(int x = 0; x < tilemapWidth; x++) {
          Tile tile = tilemap[y*tilemapWidth + x];
          SDL_Rect src = { tile.bottom.x, tile.bottom.y, srcTileSize, srcTileSize };
          SDL_RenderCopy(r,
            tilesets[tile.bottom.index],
            &src,
            &dst
          );
          if (tile.top.index != -1) {
            SDL_Rect src = { tile.top.x, tile.top.y, srcTileSize, srcTileSize };
            SDL_RenderCopy(r,
              tilesets[tile.top.index],
              &src,
              &dst
            );
          }
          dst.x += dstTileSize;
        }
        dst.x = 0;
        dst.y += dstTileSize;
      }
    }
};