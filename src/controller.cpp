#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

  void Controller::runThread()
{
    threads.emplace_back(std::thread(&Controller::CtrlInput, this));


}
void Controller::SetTargetFrameDuration(std::size_t &duration)
{
   Controller_target_frame_duration = duration;

}
 void Controller::SetSharedSnakePtr(std::shared_ptr<Snake>  &ptr)
 {
   snakeptr = ptr;
 }

void Controller::CtrlInput(void)
{
  	Uint32 frame_start;
  	Uint32 frame_end;
  	Uint32 frame_duration;
    
    while (running) 
    {
    	
      std::this_thread::sleep_for(std::chrono::milliseconds(1));

    	frame_start = SDL_GetTicks();

        this->HandleInput();
    

   		frame_end = SDL_GetTicks();


       frame_duration = frame_end - frame_start;

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < Controller_target_frame_duration) {
      SDL_Delay(Controller_target_frame_duration - frame_duration);
    }
  }
}
void Controller::HandleInput() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) 
    {
      std::unique_lock<std::mutex> lck(_mtxRunningFlag);	
      
      running = false;
      lck.unlock();
      
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}
bool Controller::GetRunningFlagstatus () const {return running;}

void Controller::ChangeDirection(Snake::Direction input,
                                 Snake::Direction opposite)  {
  if (snakeptr->direction != opposite || snakeptr->size == 1){
    snakeptr->direction = input;
  }
  return;
}
