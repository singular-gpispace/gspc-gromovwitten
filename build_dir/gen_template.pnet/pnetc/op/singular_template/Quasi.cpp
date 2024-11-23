#include <pnetc/op/singular_template/Quasi.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <interface/feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Quasi
        ( const int& weightmax
        , std::string& answer
        )
      {
#line 438 "/home/atraore/gpi/try_gpi/gpispace/template/workflow/template.xpnet"


  std::vector<fmpq_t*> Iq = RESOLVE_INTERFACE_FUNCTION( stringToVectorFmpq) (answer);
          std::cout << "answer= "  << answer<<std::endl;

    // Maximum weight

    // Initialize FLINT polynomial context with 3 variables
    slong nv = 3;  // Number of variables
    fmpq_mpoly_ctx_t ctx;
    RESOLVE_INTERFACE_FUNCTION( myctx_init) (ctx,nv);
    // Variable to hold the result
    fmpq_mpoly_t result;
        RESOLVE_INTERFACE_FUNCTION( my_init) (result,ctx);

    std::cout << "quasimodular_form as string: " ;

        RESOLVE_INTERFACE_FUNCTION( quasimodular_form) (result,Iq,weightmax,ctx);


    RESOLVE_INTERFACE_FUNCTION(my_mpoly_pretty) (result,ctx);

    // Convert polynomial result to string
   answer =RESOLVE_INTERFACE_FUNCTION(fmpq_mpolyToString) (result,ctx);

    std::cout << "Polynomial as string: " << answer << std::endl;

    // Clear polynomials and context
        RESOLVE_INTERFACE_FUNCTION(my_mpoly_clear) (result,ctx);
        RESOLVE_INTERFACE_FUNCTION( my_ctx_clear) (ctx);


         
      }
    }
  }
}