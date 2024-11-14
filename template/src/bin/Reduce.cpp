#include "Singular/libsingular.h"



//kernel level for matrix addition
static BOOLEAN jjPLUS_MA(leftv res, leftv u, leftv v) {
    // Retrieve matrices from the leftv structures
    matrix A = (matrix)u->Data();
    matrix B = (matrix)v->Data();

    // Perform matrix addition using mp_Add
    matrix result = (matrix)mp_Add(A, B, currRing);

    
    if (result == NULL) {
        // Handle error: matrix sizes not compatible
        Werror("Matrix size not compatible (%dx%d, %dx%d)",
               MATROWS(A), MATCOLS(A), MATROWS(B), MATCOLS(B));
        return TRUE; 
    }

    // Store the result in the 'res' leftv structure
    res->data = (char *)result;

    
    return jjPLUSMINUS_Gen(res, u, v);
}



// Function to initialize the module and register your function
extern "C" int mod_init(SModulFunctions* psModulFunctions) {
    psModulFunctions->iiAddCproc("", "jjPLUS_MA", FALSE, jjPLUS_MA);
    return MAX_TOK;
}




// Implementation_Kernel_Level


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


std::string  singular_template_compute_StdBasis(   std::string const& input_filename 
										, std::string const& needed_library
            							, std::string const& base_filename)
{
	init_singular (config::singularLibrary().string());
	load_singular_library(needed_library);
	std::pair<int,lists> input;
	std::pair<int, lists> out;
	std::string ids;
	std::string out_filename;
	ids = worker();
	//std::cout << ids << " in singular_..._compute" << std::endl;
	input = deserialize(input_filename,ids);
  
	ScopedLeftv args( input.first, lCopy(input.second));
  std::string function_name2 = "stdBasis";
	out = call_user_proc(function_name2, needed_library, args);
  out_filename = serialize(out.second, base_filename);
    
	return out_filename;
}



NO_NAME_MANGLING
std::string singular_template_Init( std::string const& input) {

    return input;
}






NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_leadSyzGPI(const std::string& input_filename,
                                                     const std::string& needed_library,
                                                     const std::string& base_filename)
{
    // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print out function parameters
    // std::cout << "Function Parameters:" << std::endl;
    // std::cout << "Input Filename: " << input_filename << std::endl;
    // std::cout << "Needed Library: " << needed_library << std::endl;
    // std::cout << "Base Filename: " << base_filename << std::endl;

    // Debugging: Print worker ID
    std::string ids = worker();
    //std::cout << ids << " in singular_template_Leadvector" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    //auto start_deserialize = std::chrono::high_resolution_clock::now();
    input = deserialize(input_filename, ids);
   
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    // Debugging: Print input data
    //std::cout << "Input Data:" << input.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));

    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "leadsyz_GpI";
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = call_user_proc(function_name, needed_library, args);
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
  auto end_computation = std::chrono::high_resolution_clock::now();
  auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
  //std::cout << "LEADSYZ_Runtime: " << computation_time << " milliseconds" << std::endl;
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
     //auto start_serialize = std::chrono::high_resolution_clock::now();
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output_LEADSYZ:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
  //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
 //auto total_runtime= computation_time_deserialize+computation_time+ serialization_time;
auto total_runtime=computation_time;

   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}





NO_NAME_MANGLING


std::pair<std::vector<std::string>, int> singular_template_all_leadsyz_GpI( std::string const& input_filename 
										, std::string const& needed_library
            							, std::string const& base_filename)
{
	init_singular (config::singularLibrary().string());
	load_singular_library(needed_library);
	std::pair<int,lists> input;
	std::pair<int, lists> out;
	std::string ids;
	std::string out_filename;
	ids = worker();
	//std::cout << ids << " in singular_..._compute" << std::endl;
	input = deserialize(input_filename,ids);
  
	ScopedLeftv args( input.first, lCopy(input.second));
  std::string function_name = "all_leadsyz_GpI";
	out = call_user_proc(function_name, needed_library, args);
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};
}


NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_LIFT(const std::string& input_filename,
                                                                 const std::string& all_lead,
                                                                const std::string& lead,
                                                               
                                                  const std::string& needed_library,
                                                   const std::string& base_filename)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print out function parameters
    // std::cout << "Function Parameters:" << std::endl;
    // std::cout << "Input Filename_LIFT: " << input_filename << std::endl;
    // std::cout << "Input Filename_LIFT: " << lead_syz << std::endl;
    // std::cout << "Needed Library_LIFT: " << needed_library << std::endl;
    // std::cout << "Base Filename_LIFT: " << base_filename << std::endl;

    // Debugging: Print worker ID
    std::string ids = worker();
    //std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> Lead;
    std::pair<int, lists> All_Lead;
    
    //auto start_deserialize = std::chrono::high_resolution_clock::now();
    input = deserialize(input_filename, ids);
    All_Lead = deserialize(all_lead, ids);
    Lead = deserialize(lead, ids);
   
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    // Debugging: Print input data
    // std::cout << "Input Data_LIFT:" << input.second << std::endl;
    // std::cout << "Lead_Syz:" << Lead_Syz.second << std::endl;
    // std::cout << "L0:" << L0.second << std::endl;
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "LIFT_GPI";
    //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = call_user_proc(function_name, needed_library, args);
  auto end_computation = std::chrono::high_resolution_clock::now();
  auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
  //std::cout << "LIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;
    //auto start_serialize = std::chrono::high_resolution_clock::now();
    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
  // End timing for serialization
   //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds

  //auto total_runtime= computation_time_deserialize+computation_time+ serialization_time;
   auto total_runtime= computation_time;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}


NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_SUBLIFT(const std::string& input_filename,
                                                                 const std::string& all_lead,
                                                                const std::string& lead,
                                                               
                                                  const std::string& needed_library,
                                                   const std::string& base_filename)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print out function parameters
    // std::cout << "Function Parameters:" << std::endl;
    // std::cout << "Input Filename_LIFT: " << input_filename << std::endl;
    // std::cout << "Input Filename_LIFT: " << lead_syz << std::endl;
    // std::cout << "Needed Library_LIFT: " << needed_library << std::endl;
    // std::cout << "Base Filename_LIFT: " << base_filename << std::endl;

    // Debugging: Print worker ID
    std::string ids = worker();
    //std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> Lead;
    std::pair<int, lists> All_Lead;
    
    //auto start_deserialize = std::chrono::high_resolution_clock::now();
    input = deserialize(input_filename, ids);
    All_Lead = deserialize(all_lead, ids);
    Lead = deserialize(lead, ids);
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    
    // Debugging: Print input data
    // std::cout << "Input Data_LIFT:" << input.second << std::endl;
    // std::cout << "Lead_Syz:" << Lead_Syz.second << std::endl;
    // std::cout << "L0:" << L0.second << std::endl;
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "SubLIFT_GPI";
    //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = call_user_proc(function_name, needed_library, args);
    auto end_computation = std::chrono::high_resolution_clock::now();
   auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
  //std::cout << "SubLIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;
   //auto start_serialize = std::chrono::high_resolution_clock::now();
    //Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
  // End timing for serialization
   //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
   //auto total_runtime=computation_time_deserialize+computation_time+serialization_time;

   auto total_runtime=computation_time;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}


leftv reduce_GPI(leftv arg1) {
    
    lists input = (lists)(arg1->Data()); //extract input
    lists Tok = (lists)(arg1->next->Data()); // extract Tok
    lists tok = (lists)(arg1->next->next->Data()); //extract tok

    lists tmp = (lists)(input->m[3].Data()); // input.data
   

     int r=0; int c=0;
     leftv L = nullptr; leftv LL = nullptr;
   
    if (tmp->m[0].Typ() == IDEAL_CMD) {
        // Handle case where `f` is an ideal
       r = IDELEMS((ideal)(tmp->m[0].Data()));
       lists tmpL = (lists)(Tok->m[3].Data()); // Tok.data
        L=&tmpL->m[4];
        r=r;
       c = IDELEMS((ideal)L->Data());
       c=c;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        // Handle case where `f` is a vector
        ideal t=(ideal)(tmp->m[4].Data());
        r = IDELEMS(t);
        r=r;
        lists tmpLL = (lists)(Tok->m[3].Data()); // Tok.data
        LL=&tmpLL->m[4]; //Tok.data[5]
       c = IDELEMS((ideal)LL->Data());
       c=c;
    }
     lists tmpL1 = (lists)(Tok->m[3].Data()); // Tok.data
       int counter=(int)(long)tmpL1->m[5].Data();//Tok.data[6]
  
    matrix A;
    matrix B;
    //leftv L2=(ideal)tmpL1->m[1];
    lists tmpl=(lists)(tok->m[3].Data()); //tok.data
    //leftv l=(ideal)(tmpl->m[1]);
    A = (matrix)tmpL1->m[1].Data(); // Tok.data[2]
    B = (matrix)tmpl->m[1].Data(); // tok.data[2]

    // Perform the matrix addition using Singular's API function
    matrix C = mp_Add(A, B, currRing);

  //  // Prepare the output token
   slists* output = new slists();  // Allocate memory for output
    output->Init(4);// type token
    // fieldnames
   slists* t = new slists(); 
    t->Init(2);
    t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
    t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("FirstSyz_matrix");
    output->m[1].rtyp=LIST_CMD; output->m[1].data=t;

    //data
    t->Init(7);
    t->m[0].rtyp = Tok->m[0].rtyp;t->m[0].data=Tok->m[0].CopyD(); // copy Tok.data[1]
    t->m[1].rtyp=MATRIX_CMD; t->m[1].data=C;
    t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)r;
    t->m[3].rtyp=INT_CMD; t->m[3].data = (void*)(long)c;
    
   

   if (tmp->m[0].Typ() == IDEAL_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= L->CopyD();//Tok.data[5]

     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= LL->CopyD();//Tok.data[5]
     }

    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
   t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);

   output->m[3].rtyp=LIST_CMD; output->m[3].data=t;//data is in position 4
    leftv result = new sleftv; // Allocating memory for result
     result->data = output;
     return result;
}

NO_NAME_MANGLING
std::pair<std::string, long> singular_template_reduce(const std::string& Red, 
                                                      const std::string& input_filename,
                                                      const std::string& lift,
                                                      const std::string& needed_library,
                                                      const std::string& base_filename) 
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);

    //std::pair<int, lists> out;
    std::string ids;
    std::string out_filename;
    ids = worker();
    
    std::pair<int, lists> Gb = deserialize(Red, ids);
    std::pair<int, lists> input = deserialize(input_filename, ids);
    std::pair<int, lists> lift_syz = deserialize(lift, ids);

    // Prepare arguments
    ScopedLeftv arg1(Gb.first, lCopy(Gb.second));
    ScopedLeftv args(arg1, input.first, lCopy(input.second));
    ScopedLeftv arg(args, lift_syz.first, lCopy(lift_syz.second));
    
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();

  leftv result = reduce_GPI(arg1.leftV()); 




    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
   lists output = (lists)result->data; 
  out_filename = serialize(output, base_filename);
    auto total_runtime = computation_time;
    
    return {out_filename, total_runtime};
}