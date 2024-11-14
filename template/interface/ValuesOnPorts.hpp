#pragma once

#include <we/type/value.hpp>

#include <map>
#include <string>

namespace template_module
{
    class ValuesOnPorts
    {
        public:
            using Key = std::string;
            using Value = pnet::type::value::value_type;
            using Map = std::multimap<Key, Value>;

            ValuesOnPorts (Map map);

            Map const& map() const;

    protected:
        Map _values_on_ports;
  };
}

namespace aggregate_sum
{
  class SumValuesOnPorts
  {
  public:
     using Key = std::string;
     using Value = pnet::type::value::value_type;
     using Map = std::multimap<Key, Value>;

     SumValuesOnPorts (Map map);

     Map const& map() const;

  protected:
     Map _values_on_ports;
  };
}
