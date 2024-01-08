#ifndef WSDATAPROCESSION_HPP
#define WSDATAPROCESSION_HPP

#include <string>

enum ClientType
{
    DASHBOARD,
};

class IWSSender
{
    public:
   virtual bool SendMessage(ClientType,std::string) = 0;
};

#endif 