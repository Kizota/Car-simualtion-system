#ifndef IINFOTRACKER_HPP
#define IINFOTRACKER_HPP

#include <string>

enum InfoType
{
    SPEED,
    TEMP,
    PRESSURE
};

class IDataTracker
{
    public:
    virtual bool UpdateData(std::string) = 0;
};


#endif 
