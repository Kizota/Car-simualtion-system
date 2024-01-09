#ifndef WSDATAPROCESSION_HPP
#define WSDATAPROCESSION_HPP

#include <string>

enum ClientType
{
    DASHBOARD,
    TEST =2,
};

class IWSSender
{
    public:
   virtual bool SendMessage(ClientType,std::string) = 0;
};

#endif 