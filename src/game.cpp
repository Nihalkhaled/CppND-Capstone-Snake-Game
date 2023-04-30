#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : ptrsnake(std::make_shared<Snake> (grid_width, grid_height)),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) 
{
  PlaceFood();
  PlacePoison();
}

void Game::runThread()
{
  // launch Game Update function in a thread
  threads.emplace_back(std::thread(&Game::Update, this));
  
}
void Game::Run(Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  
  Uint32 title_timestamp = SDL_GetTicks();
  
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
 
 controller.SetSharedSnakePtr(ptrsnake);
  /* Set TargetFrameDuration for the controlller */
  controller.SetTargetFrameDuration(target_frame_duration);
  /*Controller run thread */
  controller.runThread();
  
  /* save the Game target frame duration */
  Game_target_frame_duration = target_frame_duration;
  /* Game run thread */
  runThread();
   
   while (running) 
    {
    	
      std::this_thread::sleep_for(std::chrono::milliseconds(1));

    	frame_start = SDL_GetTicks();

        renderer.Render(ptrsnake, food, poison);

   		frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
       frame_count++;
       frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) 
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < Game_target_frame_duration) {
      SDL_Delay(Game_target_frame_duration - frame_duration);
    }
    
    //Lock shared Running game flag object in order to not be corrupted while setting it
   std::unique_lock<std::mutex> lck(_mtxRunningFlag);	
    running = controller.GetRunningFlagstatus();
  }

}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!ptrsnake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}
  void Game::PlacePoison() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
      if (!ptrsnake->SnakeCell(x, y)) {
      poison.x = x;
      poison.y = y;
      return;
    }
  }
 }


void Game::Update(){
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  
  
  while (running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    frame_start = SDL_GetTicks();

   
    int frame_count = 0;
    std::unique_lock<std::mutex> lck(_mtxSnake); //Lock shared snake object in order to not be corrupted while using it
    if (!ptrsnake->alive) return;

    
    ptrsnake->Update();

    int new_x = static_cast<int>(ptrsnake->head_x);
    int new_y = static_cast<int>(ptrsnake->head_y);

    // Check if there's food over here
    std::unique_lock<std::mutex> lckFood(_mtxFood); //make lock before using the shared objects food 
    std::unique_lock<std::mutex> lckPoison(_mtxPoison); //Lock shared Poison object in order to not be corrupted while setting it
    if (food.x == new_x && food.y == new_y)
    {
      score++;
      PlaceFood();
      // Grow snake and increase speed.
      ptrsnake->GrowBody();
      ptrsnake->speed += 0.02;
    }
    else if(poison.x == new_x && poison.y == new_y)
    {
      if (score > 0)
          score--;

      PlacePoison();
      // Grow snake and increase speed.
      ptrsnake->ShrinkBody();
   }
    //Unlock snake mutex 
    lckFood.unlock();
    lckPoison.unlock();
    lck.unlock();
    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;


    if (frame_duration < Game_target_frame_duration) 
     SDL_Delay(Game_target_frame_duration - frame_duration);
    
 }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const 
{ 
 // std::unique_lock<std::mutex> lck(_mtxSnake); //make lock before using the shared object snake
  return ptrsnake->size; 
}