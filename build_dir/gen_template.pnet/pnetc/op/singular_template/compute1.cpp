#include <pnetc/op/singular_template/compute1.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <interface/feynman.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void compute1
        ( const std::string& input
        , const std::string& library_name
        , std::string& output
        )
      {
#line 51 "/home/atraore/gpi/try_gpi/Reduce/template/workflow/template.xpnet"

          
         std::cout<<" ***** myinput***** "<<input<<std::endl;

         std::vector<std::string> result;
    std::string current;
    int braceCount = 0;

    for (char c : input) {
        if (c == '{') {
            braceCount++;
            current += c;
        }
        else if (c == '}') {
            braceCount--;
            current += c;
        }
        else if (c == ',' && braceCount == 0) {
            // Add current token to result if not inside braces
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        }
        else {
            // Add character to current token
            current += c;
        }
    }

    // Add the last token
    if (!current.empty()) {
        result.push_back(current);
    }

    // Trim leading/trailing whitespace from each entry
    for (auto& entry : result) {
        entry.erase(0, entry.find_first_not_of(" \t"));
        entry.erase(entry.find_last_not_of(" \t") + 1);
    }

 for (size_t i = 0; i < result.size(); ++i) {
        std::cout << "result[" << i << "]=" << result[i] << std::endl;
    }

            		
      }
    }
  }
}