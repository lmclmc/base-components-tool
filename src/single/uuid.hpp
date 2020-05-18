#ifndef UUID_HPP_
#define UUID_HPP_

#include <random>

namespace lmc
{
class UUID
{
public:
  static uint64_t generateUuid()
  {
    uint64_t uuid = std::chrono::system_clock::now().time_since_epoch().count();
    std::random_device rd;
    uuid = (uuid << 16) | rd();
    return uuid;
  }
};
}

#endif