#include "SWManager.hpp"

SWManager::SWManager(Button *indBt,Button *hgBmBt, JoyStick *joyStk) : indBt(indBt), hgBmBt(hgBmBt), joyStk(joyStk), sender(nullptr)
{
     commandMutex = xSemaphoreCreateMutex();
    
     //turn on read signal task
      indBt->SetReadMode(ON);
      hgBmBt->SetReadMode(ON);

      //call read button task
      xTaskCreate(SWManager::HandleSettingMode,"Setting mode",stackDepth,this,1,NULL);
}

void SWManager::SetSender(ISender *newSender)
{
  sender = newSender;
}




