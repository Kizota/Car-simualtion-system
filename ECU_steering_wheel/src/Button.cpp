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
}

void Button::SetReadMode(TaskMode mode)
{
  readTaskHandler->SetMode(mode);
}

// thread safe sema
bool Button::IsPressed()
{
  bool isPressed = false;
  // only read when is not updated

  if (xSemaphoreTake(readMutex, portMAX_DELAY))
  {
    isPressed = (state == LOW);
    xSemaphoreGive(readMutex);
  }

  return isPressed;
}
