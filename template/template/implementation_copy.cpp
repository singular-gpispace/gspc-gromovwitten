
#include <Singular/libsingular.h>


#include <interface/template_interface.hpp>
#include <interface/WorkflowResult.hpp>
#include <interface/ValuesOnPorts.hpp>
#include <interface/Workflow.hpp>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "config.hpp"
#include "singular_functions.hpp"
#include <chrono>
#include <tuple>
//#include "Singular/lists.h"



NO_NAME_MANGLING

#include <iostream>
#include <string>
#include <vector>

// Function to compute standard basis using Singular library
std::vector<std::string> singular_template_compute_StdBasis(const std::string& input_filename,
  const std::string& needed_library,
  const std::string& base_filename) {
  // Initialize Singular environment
  init_singular(config::singularLibrary().string());
  load_singular_library(needed_library);

  // Deserialize input
  std::pair<int, lists> input = deserialize(input_filename, worker());
  std::string out_filename1;

  ScopedLeftv args(input.first, lCopy(input.second));
  std::string function_name2 = "stdBasis";

  // Access the elements of the input list
  lists Token = (lists)(args.leftV()->data);
  if (Token == nullptr) {
    std::cerr << "Error: Token list is null." << std::endl;
    return {};  // Return an empty vector on error
  }

  int L_size = lSize(Token) + 1;
  std::cout << "Size of J in the transition: " << L_size << std::endl;

  for (int i = 0; i < L_size; i++) {
    sleftv& listElement = Token->m[i];
    if (listElement.data == nullptr) {
      std::cout << "Input: NULL" << std::endl;
    }
    else if (i == 3) {
      std::cout << "INPUT1: " << listElement.String() << std::endl;
      out_filename1 = listElement.String();
    }
  }

  // Example output filename for debug purposes
  std::cout << "Base filename: " << base_filename << std::endl;
  std::cout << "Output filename: " << out_filename1 << std::endl;
  std::cout << "Output filename: " << typeof(out_filename1) << std::endl;

  // Parse `out_filename1` to extract contents between braces
  std::vector<std::string> result;
  std::string current;
  int braceCount = 0;

  for (char c : out_filename1) {
    if (c == '{') {
      braceCount++;
      current += c;
    }
    else if (c == '}') {
      braceCount--;
      current += c;
    }
    else if (c == ',' && braceCount == 0) {
      if (!current.empty()) {
        result.push_back(current);
        current.clear();
      }
    }
    else {
      current += c;
    }
  }

  // Add the last token if not empty
  if (!current.empty()) {
    result.push_back(current);
  }

  return result;
}


std::string singular_template_compute_parseInput(const std::string& input) {

  return input;
}


int main() {
  // Example input parameters
  std::string input_filename = "../example_dir/temp/input.txt";
  std::string needed_library = "stdBasis";
  std::string base_filename = "base_output";
  // Call the function and get the result
  std::vector<std::string> result = singular_template_compute_StdBasis(input_filename, needed_library, base_filename);

  // Print the parsed output
  std::cout << "Parsed Output:" << std::endl;
  for (const auto& entry : result) {
    std::cout << entry << std::endl;
  }

  return 0;
}