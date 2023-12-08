#ifndef IMESSAGER_HPP
#define IMESSAGER_HPP
namespace msg
{
    class ISender
    {
    public:
        virtual void SendMessage() = 0;
    };

}
#endif