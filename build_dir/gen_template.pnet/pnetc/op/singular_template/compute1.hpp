#ifndef _PNETC_OP_singular_template_compute1
#define _PNETC_OP_singular_template_compute1

#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void compute1
        ( const std::string& input
        , std::string& graph
        , int& degree
        , int& edges
        , int& g
        , int& deg
        , unsigned long& total
        , std::string& genus
        , std::string& loop
        );
    }
  }
}
#endif
