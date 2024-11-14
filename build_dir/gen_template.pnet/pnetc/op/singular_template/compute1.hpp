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
        , const std::string& library_name
        , const std::string& base_filename
        , std::string& output
        );
    }
  }
}
#endif
