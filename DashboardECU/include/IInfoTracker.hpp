#ifndef IINFOTRACKER_HPP
#define IINFOTRACKER_HPP

#include <iostream>
#include <utility>

enum InfoType
{
    SPEED,
    TEMP,
    PRESSURE
};

class IInfoTracker
{
    public:
    virtual bool UpdateInfo(std::pair<InfoType,void*>) = 0;
};


#endif 
