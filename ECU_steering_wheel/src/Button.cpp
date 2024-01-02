#include "Button.hpp"

Button::Button(std::string name, int pin, uint32_t interval) : name(name), pin(pin), preState(0), state(0)
{
  // configure button pin mode and timer
  pinMode(pin, INPUT_PULLUP);
  timer = new Timer(interval);

  // RTOS
  readMutex = xSemaphoreCreateMutex();
  readTaskHandler = new TaskHandler(name, Button::ReadSignal, this);
}

Button::~Button()
{
  // free the timer memory
  delete timer;

  // free mutex
  xSemaphoreGive(readMutex);
  vSemaphoreDelete(readMutex);

  // turn off reading task
  readTaskHandler->SetMode(OFF);
}

void Button::SetReadMode(TaskMode mode)
{
  readTaskHandler->SetMode(mode);
}

bool Button::IsPressed()
{
  bool state = isPressed;
  isPressed = false;

  return state;
}

// thread safe sema
bool Button::IsHold()
{
  bool IsHold = false;
  // only read when is not updated

  if (xSemaphoreTake(readMutex, portMAX_DELAY))
  {
    IsHold = (state == LOW);
    xSemaphoreGive(readMutex);
  }

  return IsHold;
}
