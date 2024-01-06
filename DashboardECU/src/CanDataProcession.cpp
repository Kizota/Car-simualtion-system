#include "CanDataProcession.hpp"

CanData::CanData() : msgId(0), len(0)
{
  command[8] = {0};
}

CanData::CanData(unsigned long msgId, byte len) : msgId(msgId), len(len)
{
  command[8] = {0};
}