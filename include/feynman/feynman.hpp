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
#include <flint/fmpq.h>
#include <flint/fmpq_mat.h>
#include <flint/fmpq_poly.h>
#include <flint/fmpq_mpoly.h>  // Add this for rational polynomial functions
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
#include <algorithm>
#include <set>
#include <map>

using Element = std::pair<std::pair<int, int>, std::pair<int, int>>;
using Sequence = std::vector<Element>;
using signature = std::vector<std::pair<int, std::vector<int>>>;
using graph = std::vector<std::pair<int, int>>;

/*
using pnet_value = pnet::type::value::value_type;
using pnet_list = std::list<pnet_value>;
using pnet_list2d = std::list<std::list<pnet_value>>; */
using vector2d = std::vector<std::vector<int>>;
using list_type = std::list<std::string>; // Define list_type as std::list<std::string>

// String and vector conversion functions
std::string vectorToStringULong(const std::vector<unsigned long>& vec);
std::vector<unsigned long> stringToVectorUlong(const std::string& str);
std::vector<unsigned long> sumOfVectors(const std::vector<unsigned long>& v1, const std::vector<unsigned long>& v2);
void updateVectorFromString(std::vector<unsigned long>& v, const std::string& s);
std::string updateAndConvertVector(std::vector<unsigned long>& v, const std::string& s);

// FLINT rational number conversion functions
std::string vectorToStringFmpq(const std::vector<fmpq_t*>& vec);
std::vector<fmpq_t*> stringToVectorFmpq(const std::string& str);
std::vector<fmpq_t*> sumOfVectorsFmpq(const std::vector<fmpq_t*>& v1, const std::vector<fmpq_t*>& v2);

// FLINT rational polynomial wrapper functions
void feynman_fmpq_mpoly_ctx_init(fmpq_mpoly_ctx_t ctx, slong nvars, const ordering_t ord);
void feynman_fmpq_mpoly_init(fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx);
void feynman_fmpq_mpoly_clear(fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx);
void feynman_fmpq_mpoly_ctx_clear(fmpq_mpoly_ctx_t ctx);
int feynman_fmpq_mpoly_print_pretty(const fmpq_mpoly_t poly, const char ** x, const fmpq_mpoly_ctx_t ctx);
std::string feynman_fmpq_mpolyToString(const fmpq_mpoly_t poly, const fmpq_mpoly_ctx_t ctx);
void feynman_quasimodular_form(fmpq_mpoly_t result, const std::vector<fmpq_t*>& Iq, int weightmax, const fmpq_mpoly_ctx_t ctx);

// Basic functions
vector2d gen_block(int d, int n);
int binomial(int n, int k);
std::vector<int> next_partition(std::vector<int> a);
vector2d iterate(std::vector<int> xa);
 int number_monomial(int weightmax);
std::vector<std::tuple<int, std::vector<int>>> signature_and_multiplicitie(std::vector<std::pair<int, int>> G, const std::vector<int> a);

// Feynman integral functions
void constterm(const int k, const int j, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
void proterm(const int k, const int j, int a, mp_limb_signed_t N, const int nv, fmpz_mpoly_t result, const fmpz_mpoly_ctx_t ctx);
unsigned long feynman_integral_type(std::vector<std::pair<int, int>> Gv, int factor, std::vector<int> av);

// Different signatures of feynman_integral_branch_type
unsigned long feynman_integral_branch_type(std::vector<std::pair<int, int>> Gv, std::vector<int> a);
void feynman_integral_branch_type(fmpq_t myfey, graph& Gv, const std::vector<int>& av, const std::vector<int>& g = std::vector<int>(), const std::vector<int>& l = std::vector<int>());
unsigned long feynman_integral_degree(std::vector<std::pair<int, int>> Gv, int d);

// Double edges functions
int nv(const std::vector<std::pair<int, int>>& ve);
std::vector<std::vector<int>> find_equal_pairs(const std::vector<std::pair<int, int>>& ve);
std::string vector_to_monomial(const graph& G, const std::vector<int>& v);
std::vector<std::vector<int>> generate_permutation(const std::vector<int>& l, const std::vector<std::vector<int>>& indices);
vector2d iterate_permutation(const graph& ve, const std::vector<int>& a);

// Functions from feynman_sum_psi.cpp
std::vector<int> extractIntegers(const std::string& graph);
unsigned long partialBinomialSum(int n, int d);
std::vector<int> stringToVectorInt(const std::string& str);
int sumVector(const std::vector<int>& vec);

// FLINT rational number functions
void fmpq_init(fmpq_t x);
void fmpq_clear(fmpq_t x);
void fmpq_add(fmpq_t res, const fmpq_t op1, const fmpq_t op2);
void fmpq_mul_ui(fmpq_t res, const fmpq_t op, unsigned long c);
std::string fmpqToString(const fmpq_t x);
void stringToFmpq(fmpq_t res, const std::string& str);

#endif // FEYNMAN_HPP