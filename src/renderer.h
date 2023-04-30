#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include <memory>
#include <mutex>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(std::shared_ptr<Snake> ptrsnake, SDL_Point const &food, SDL_Point const &poison);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  
  std::mutex _mtxRenderFood;        // mutex that protects the shared Food object from being corrupted throught the game 
  std::mutex _mtxRenderPoison;      // mutex that protects the shared Poison object from being corrupted throught the game 
};

#endif