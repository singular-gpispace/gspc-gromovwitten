#include <feynman/ValuesOnPorts.hpp>

namespace feynman
{
  ValuesOnPorts::ValuesOnPorts (Map map) : _values_on_ports (map) {}

  ValuesOnPorts::Map const& ValuesOnPorts::map() const
  {
     return _values_on_ports;
  }
}
