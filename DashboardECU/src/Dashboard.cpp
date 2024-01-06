#include "DashBoard.hpp"

Dashboard::Dashboard()
{
   taskHandler = new RealTime::TaskHandler("Dashboard",Dashboard::DisplayData ,this);
}







