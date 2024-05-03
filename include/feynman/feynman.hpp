#ifndef FEYNMAN_HPP
#define FEYNMAN_HPP

#include <list>
#include <tuple>
#include <sstream> // for std::istringstream
/* #include "../../code/boost/include/boost/variant.hpp"
#include "../../code/util-generic/split.hpp"
#include "../../code/we/type/value.hpp" */
#include <stdio.h>
#include <flint/fmpz_mpoly.h>
#include <stdlib.h> // for malloc and free
#include <flint/fmpz.h>
#include <fstream>
#include <sys/resource.h>
#include <iostream>
#include <chrono>
#include <functional> // Include for std::function
#include <vector>     // Include for std::vector
#include <numeric>
#include <unordered_set> // for std::unordered_set

void constterm(const int k, const int j, mp_limb_signed_t N, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
void proterm(const int k, const int j, int a, mp_limb_signed_t N, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
int feynman(std::vector<std::pair<int, int>> Gv, std::vector<int> av);
#endif // FEYNMAN_HPP