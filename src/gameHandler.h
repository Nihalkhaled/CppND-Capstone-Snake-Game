#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H


#include <vector>
#include <thread>
#include <mutex>


class GameHandler
{
 public:
  	virtual void runThread() = 0;
    ~GameHandler();
    std::vector<std::thread> threads;  // It holds all threads that will be launched in the Game.
    std::mutex _mtxRunningFlag; // mutex that protects the Running flag from being corrupted throught the game 
    std::mutex _mtxSnake; 		// mutex that protects the shared snake object from being corrupted throught the game 
    std::mutex _mtxFood;        // mutex that protects the shared Food object from being corrupted throught the game 
    std::mutex _mtxPoison;      // mutex that protects the shared Poison object from being corrupted throught the game 


};

#endif