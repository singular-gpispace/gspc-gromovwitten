#include <pnetc/op/feynman_module/Quasi.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <feynman.hpp>

namespace pnetc
{
  namespace op
  {
    namespace feynman_module
    {
      void Quasi
        ( const int& weightmax
        , std::string& answer
        )
      {
#line 360 "/home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet"


                std::cout << "weightmax: " << weightmax << std::endl;

             std::vector<fmpq_t*>Iq = stringToVectorFmpq(answer);
    // Maximum weight

    // Initialize FLINT polynomial context with 3 variables
    slong nv = 3;  // Number of variables
    fmpq_mpoly_ctx_t ctx;
    fmpq_mpoly_ctx_init(ctx, nv, ORD_DEGLEX);

    // Variable to hold the result
    fmpq_mpoly_t result;
    fmpq_mpoly_init(result, ctx);

    // Call the quasimodular_form function with Iq and weightmax
    quasimodular_form(result, Iq, weightmax, ctx);

    // Print the result in a pretty format
    std::cout << "Quasimodular form result: ";
    fmpq_mpoly_print_pretty(result, NULL, ctx);
    std::cout << std::endl;

    // Convert polynomial result to string
    std::string fey = fmpq_mpolyToString(result, ctx);
    std::cout << "Polynomial as string: " << fey << std::endl;

    answer=fey;

    // Clear polynomials and context
    fmpq_mpoly_clear(result, ctx);
    fmpq_mpoly_ctx_clear(ctx);


   
         
      }
    }
  }
}