#include "CanDataProcession.hpp"

CanData::CanData() : msgId(NO_MESSGAGE), len(0)
{
  command[8] = {0};
}

CanData::CanData(unsigned long msgId, byte len) : msgId(msgId), len(len)
{
  command[8] = {0};
}