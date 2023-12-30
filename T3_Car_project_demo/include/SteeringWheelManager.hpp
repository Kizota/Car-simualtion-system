#ifndef STEERING_WHEEL_MANAGER_H
#define STEERING_WHEEL_MANAGER_H

#include "IMessager.hpp"

using namespace message;

class SteeringWheelManager
{
   private:

   ISender *sender;
   
   public:
   SteeringWheelManager(ISender *sender) : sender(sender)
   {
      
   }

   //stearing wheel has the responsibility of reading the button signal and send signal?
   
   

};
#endif