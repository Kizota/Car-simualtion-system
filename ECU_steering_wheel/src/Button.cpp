#include "Button.hpp"

Button::Button(std::string name, int pin, uint32_t interval) : name(name), pin(pin), preState(0), state(0)
{
  // configure button pin mode and timer
  pinMode(pin, INPUT);
  timer = new Timer(interval);

  // RTOS
  readMutex = xSemaphoreCreateMutex();
  readTaskHandler = new TaskHandler(name, Button::ReadSignal, this);
  readTaskHandler->AddMutex(readMutex);
}

Button::~Button()
{
  // free the timer memory
  delete timer;

  // REVIEW  free mutex how can be sure that mutex below to the current and free
  //  give the mutex if it is nor given yet
  //  turn off reading task
  readTaskHandler->SetMode(OFF);
}

void Button::SetReadMode(TaskMode mode)
{
  // give the mutex if it is nor given yet
  readTaskHandler->SetMode(mode);
}

bool Button::IsPressed()
{
  bool state = false;

  if (xSemaphoreTake(readMutex, portMAX_DELAY))
  {

    state = isPressed;
    if(isPressed)
    {
      isPressed = false;
    }
    xSemaphoreGive(readMutex);
  }
  return state;
}

// thread safe sema
bool Button::IsHold()
{
  bool IsHold = false;
  // only read when is not updated

  IsHold = (state == LOW);

  return IsHold;
}
