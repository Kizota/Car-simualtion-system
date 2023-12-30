#include "SWManager.hpp"

SWManager::SWManager(Button *indBt,Button *hgBmBt, JoyStick *joyStk) : indBt(indBt), hgBmBt(hgBmBt), joyStk(joyStk)
{
      commandMutex = xSemaphoreCreateMutex();
    
     //turn on read signal task
      indBt->SetReadMode(ON);
      hgBmBt->SetReadMode(ON);
}




