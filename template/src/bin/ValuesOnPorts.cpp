#include <interface/ValuesOnPorts.hpp>

namespace template_module
{
  ValuesOnPorts::ValuesOnPorts (Map map) : _values_on_ports (map) {}

  ValuesOnPorts::Map const& ValuesOnPorts::map() const
  {
     return _values_on_ports;
  }
}
namespace aggregate_sum
{
  SumValuesOnPorts::SumValuesOnPorts (Map map) : _values_on_ports (map) {}

  SumValuesOnPorts::Map const& SumValuesOnPorts::map() const
  {
     return _values_on_ports;
  }
}
