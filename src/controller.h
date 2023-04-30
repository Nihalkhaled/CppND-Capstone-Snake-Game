#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include"gameHandler.h"

class Controller:public GameHandler {
 public:
    void HandleInput() ;
    void SetTargetFrameDuration(std::size_t &duration);
    void CtrlInput(void);
    void runThread() override;
    void SetSharedSnakePtr(std::shared_ptr<Snake>  &ptr);
   // void SetSharedSnakePtr(Snake s);
    bool GetRunningFlagstatus () const;

 private:
  std::size_t Controller_target_frame_duration;
  bool running{true} ;
  std::shared_ptr<Snake> snakeptr;
  
  void ChangeDirection(Snake::Direction input,
                       Snake::Direction opposite);
   
};

#endif