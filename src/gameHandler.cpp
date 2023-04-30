#include "gameHandler.h"
#include <algorithm>

 GameHandler::~GameHandler()
 {
   //To prevent that the main terminates the program until the all threads finish their execution
   std::for_each(threads.begin(), threads.end(), [](std::thread &th) {th.join(); });

 }