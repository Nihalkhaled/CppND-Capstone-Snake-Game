#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include"gameHandler.h"

class Game:public GameHandler {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  void runThread() override;
  bool GetGameStatus () const;

  int GetScore() const;
  int GetSize() const;
  
  

 private:
 //Snake snake;
  std::shared_ptr<Snake> ptrsnake;
  
  SDL_Point food;
  SDL_Point poison;
  
  std::size_t Game_target_frame_duration;
  bool running{true};
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void PlacePoison();
  void Update();
};

#endif