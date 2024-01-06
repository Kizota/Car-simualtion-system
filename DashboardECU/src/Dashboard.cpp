#include "DashBoard.hpp"

Dashboard::Dashboard() : speed(0), temperature(0), pressure(0)
{
  displayMutex = xSemaphoreCreateMutex();
  taskHandler = new RealTime::TaskHandler("Dashboard", Dashboard::DisplayData, this);
}
