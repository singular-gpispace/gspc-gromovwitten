#ifndef FEYNMAN_HPP
#define FEYNMAN_HPP

#include <list>
#include <tuple>
#include <sstream> // for std::istringstream
/* #include "../../../code/boost/include/boost/variant.hpp"
#include "../../../code/util-generic/split.hpp"
#include "../../../code/we/type/value.hpp" */
#include <stdio.h>
#include <flint/fmpz_mpoly.h>
#include <stdlib.h> // for malloc and free
#include <flint/fmpz.h>
#include <flint/fmpq_mpoly.h>


#include <fstream>
#include <sys/resource.h>
#include <iostream>
#include <chrono>
#include <functional> // Include for std::function
#include <vector>     // Include for std::vector
#include <numeric>
#include <unordered_set> // for std::unordered_set
/*
using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>; */
using vector2d = std::vector<std::vector<int>>;
using list_type = std::list<std::string>; // Define list_type as std::list<std::string>
using graph = std::vector<std::pair<int, int>>;

std::string vectorToStringInt(const std::vector<int>& vec);
std::vector<int> stringToVectorInt(const std::string& str);
vector2d gen_block(int d, int n);
std::vector<int> next_partition(std::vector<int> a);
vector2d iterate(std::vector<int> xa);
std::vector<std::vector<int>> find_equal_pairs(const std::vector<std::pair<int, int>>& ve);
std::vector<std::vector<int>> generate_permutation(const std::vector<int>& l, const std::vector<std::vector<int>>& indices);
vector2d iterate_permutation(const graph& ve, const std::vector<int>& a);
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(std::vector<std::pair<int, int>> G, const std::vector<int> a);

void constterm(const int k, const int j, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
void proterm(const int k, const int j, int a, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
unsigned long feynman_integral_type(std::vector<std::pair<int, int>> Gv, int factor, std::vector<int> av);
unsigned long feynman_integral_branch_type(std::vector<std::pair<int, int>> Gv, std::vector<int> a);
unsigned long feynman_integral_degree(std::vector<std::pair<int, int>> Gv, int d);
void invsfunction(const int j, const int aa, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void sfunction(const int w, const int j, const int aa, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void loopterm(const int  z, const int a, const int aa, fmpq_mpoly_t p, const fmpq_mpoly_ctx_t ctx);
void constterm(const int k, const int j, const int i1, const int i2, mp_limb_signed_t N, const int aa, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void proterm(const int k, const int j, const int i1, const int i2, const int a, const int aa, mp_limb_signed_t N, fmpq_mpoly_t result, const fmpq_mpoly_ctx_t ctx);
void get_coefficient(fmpq_mpoly_t coeff, const fmpq_mpoly_t poly, const mp_limb_signed_t* var_indices, const mp_limb_t* exps, mp_limb_signed_t length, const fmpq_mpoly_ctx_t ctx);
void filter_term(fmpq_mpoly_t result, const fmpq_mpoly_t poly, const std::vector<slong>& variables, const std::vector<int>& power, const fmpq_mpoly_ctx_t ctx);
void feynman_integral_branch_type(fmpq_t myfey, graph& Gv, const std::vector<int>& av, const std::vector<int>& g = std::vector<int>(), const int aa = 0, const std::vector<int>& l = std::vector<int>());
void feynman_integral_degree(fmpq_t result, graph Gv, const int& d, const std::vector<int>& g = std::vector<int>(), const int aa = 0, const std::vector<int>& l = std::vector<int>());
std::string fmpqToString(const fmpq_t f);
void stringToFmpq(fmpq_t f, const std::string& str);
#endif // FEYNMAN_HPP