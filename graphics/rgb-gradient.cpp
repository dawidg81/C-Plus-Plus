// -*- compile-command: "g++ main.cpp -o app -lmingw32 -lSDL2main -lSDL2" -*- for windows mingw
#include <cstdlib>
#include <ostream>
#include <random>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <functional>
#include <cstring>
#include "font8x8_basic.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

uint32_t pixbuf[SCREEN_WIDTH * SCREEN_HEIGHT]; // one dimensional array of pixels

void draw_menu() {
  for (int y = 0; y < SCREEN_HEIGHT; y++)
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      uint8_t r = x % 256, g = y % 256, b = 0;
      pixbuf[y * SCREEN_WIDTH + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
    }
}

int main(){
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow(
					"SoftRaster",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					640, 480,
					SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
					);
  if(!window){
    std::cerr << "SDL Create Window Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(
					      window, -1, SDL_RENDERER_ACCELERATED
					      );

  if(!renderer){
    std::cerr << "SDL Create Renderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_Texture* texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
  );

  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
    }

    draw_menu();

    SDL_UpdateTexture(texture, NULL, pixbuf, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
