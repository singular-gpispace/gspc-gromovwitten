





matrix lcm_mod(ideal G) { //ideal G is Singular module

    int a=0;
    int b=0;
    int i=0;
    int j=0;
    int r = IDELEMS(G);
    matrix l=mpNew(r,r);
    poly s10=NULL;
    poly t10=NULL;
    for (a = 0; a < r; a++) {
        for (b = 0; b < r; b++) {
           
            //i = leadexp(G[a])[nvars(basering) + 1];
            i= p_GetComp(G->m[a],currRing);
            //j = leadexp(G[b])[nvars(basering) + 1];
            j= p_GetComp(G->m[b],currRing);
           s10 = pHead(G->m[a]); //Should be leadmonomial
           t10 = pHead(G->m[b]);//Should be leadmonomial
            poly lcm_poly =  p_Lcm(s10, t10, currRing);

            if (i == j) {
               // l[a, b] = lcm(leadmonomial(G[a]), leadmonomial(G[b])) / lead(t10);
                
                MATELEM(l, a, b) = pp_Divide(lcm_poly, t10, currRing);

            } else {
                // If i is not equal to j, set l[a, b] to 0
                MATELEM(l,a,b)= NULL;
            }
        }
    }
    

    return(l);
}





//First Level Leadsyz
ideal leadSyz(ideal f) {

    int a=0;
    int b=0;
    int i=0;
    int j=0;
    int k=0;
    poly s=NULL; //vector 
    poly t=NULL;//vector t
    int r = IDELEMS(f);
    ideal L=NULL;   // Initialize module L
    matrix m=mpNew(r,r);

    for (a = 0; a < r; a++) {
        for (b = 0; b < r; b++) {
          // MATELEM(m,a,b) = lcm(leadmonom(f->m[b]), leadmonom(f->[a])) / pHead(f->m[b]);
          MATELEM(m,a,b) = pp_Divide(p_Lcm(pHead(f->m[b]), pHead(f->m[a]),currRing), pHead(f->m[b]),currRing);
        }
    }

    int cc = 0;
    

    for (i = 1; i < r; i++) {
       
        
        for (j = 0; j < i; j++) {


               p_SetComp(MATELEM(m,j,i),i,currRing);
                   p_SetmComp(MATELEM(m,j,i),currRing);
            t =  MATELEM(m,j,i);//t=m[j,i]*gen(i)
         
            // Check divisibility conditions and remove a generator if necessary
            for (k = 0; k < IDELEMS(L); k++) {
              
               
                 
                 
                    s = L->m[k];//s=L[k]
               
               
                // Check if s divides t
           ideal Gb_t=idInit(1,1);
           Gb_t->m[0]=t;
                 if (kNF(Gb_t,NULL, s) == 0) {
                  
                    t = 0;
                    break;
                 } else {
         ideal Gb_s=idInit(1,1);
           Gb_s->m[0]=s;
                    // Check if t divides s
                    if (kNF(Gb_s,NULL, t) == 0) {                  
                        p_DeleteComp(L->m, k, currRing);


                        // L = delete(L, k); //Ask Hans
                         k=k-1;
                           cc=cc-1;
                                         
                        
                      
                    }
                }
            }
            
            // If t survived the checks and no generator was deleted, add it to the module L
            if (t != NULL) {
           
              
                L->m[cc]=t;//get back from Gb_t
                
               
                   cc++;

               
            }
             
        } 
    }// timer-t1;
      // print(" timer of leadsyz");
return(L); 
    

}

//iterative leadsyz
ideal Sec_leadSyz(ideal f0) { //ideal f is singular module
    int i=0;
    int j=0;
    int k=0;
    poly s=NULL; //poly s is singular vector
    poly t=NULL; //poly t is singular vector
    int r = IDELEMS(f0);
    ideal L=NULL;   // Initialize module L

    //module f0 = f; // Use the input module directly as input is already previous leadsyz

   matrix m=mpNew(r,r);
    m = lcm_mod(f0);//calling function lcm_mod

    int cc = 0;
 //i=2 in SIngular
    for (i = 1; i < r; i++) {
        for (j = 0; j < i; j++) {
            if ( MATELEM(m,j,i)!= NULL) {//how to access m[j,i]
             
                 p_SetComp( MATELEM(m,j,i),i,currRing);
                   p_SetmComp( MATELEM(m,j,i),currRing);
                t =   MATELEM(m,j,i);//t=m[j,i]*gen(i)
            }

            for (k = 0; k < IDELEMS(L); k++) {
                s = L->m[k];//s=L[k]
                     ideal Gb_t=idInit(1,1);
                  Gb_t->m[0]=t;
                if (kNF(Gb_t,NULL, s) == 0) {
                    t = NULL;
                    break;
                } else {
                   ideal Gb_s=idInit(1,1);
                    Gb_s->m[0]=s;
                    if (kNF(Gb_s,NULL, t) == 0)  {
                        p_DeleteComp(L->m, k, currRing);

                       // L = delete(L, k);
                        k = k - 1;
                        cc = cc - 1;
                    }
                }
            }

             if (t != NULL) {
                L->m[cc] = t;//get back from Gb_t
                cc++;
            }
        }
    }
    return (L);
}








std::pair<int, lists> leadsyz_GpI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
    ideal f = (ideal)(tmp->m[0].Data());
    
    int counter = 0;
    int ro = 0;
    ideal LT=NULL;
    
    // Check if f is an ideal or a vector
    if (tmp->m[0].Typ() == IDEAL_CMD) {
        // If it's an ideal, call leadSyz
        LT = leadSyz(f);
        ro = IDELEMS(f); 
        ro=ro; // Set row count to the number of elements in ideal
        counter = 0;//counter=1
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        // If it's a vector, handle it using Sec_leadSyz
        ideal mM = (ideal)(tmp->m[4].Data()); 
        ro =IDELEMS(mM);  // Set row count for matrix
        ro=ro;
        LT = Sec_leadSyz(mM);
        counter = (int)(long)tmp->m[6].Data();
    }

    int r = IDELEMS(LT);  // Number of lead syzygies
    lists LLT = (lists)omAlloc0Bin(slists_bin);  // Allocate memory for LLT token
    LLT->Init(r + 1);  // Initialize LLT with r generators and one extra for total number of generators

    for (int k = 0; k < r; k++) {
        lists Ld = (lists)omAlloc0Bin(slists_bin);  // Create a new token Ld
        Ld->Init(7);  // Initialize Ld with 7 data slots

        // Set field names for Ld
        lists fieldnames = (lists)omAlloc0Bin(slists_bin);
        fieldnames->Init(2);
        fieldnames->m[0].rtyp = STRING_CMD; fieldnames->m[0].data = strdup("generator");
        fieldnames->m[1].rtyp = STRING_CMD; fieldnames->m[1].data = strdup("Lead_Syz_Sparse_matrix");
        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = fieldnames;

        // Create the matrix sM
        matrix sM = mpNew(ro, r);
        poly s_lift = LT->m[k];

        // Extract the lead exponent l_k
        int l_k = p_GetComp(s_lift, currRing);

        // sM[l_k, k] = leadcoef(s_lift) * leadmonomial(s_lift);
        MATELEM(sM, l_k,r) = pp_Mult_qq(pHead(s_lift), pCopy(s_lift),currRing);

        // Assign values to Ld
        Ld->m[0].rtyp = VECTOR_CMD; Ld->m[0].data = s_lift;  // s_lift
        Ld->m[2].rtyp = MATRIX_CMD; Ld->m[2].data = sM;      // sM matrix
        Ld->m[3].rtyp = INT_CMD; Ld->m[3].data = (void*)(long)l_k;  // l_k
        Ld->m[4].rtyp = INT_CMD; Ld->m[4].data = (void*)(long)k;  // column k
        Ld->m[5].rtyp = LIST_CMD; Ld->m[5].data = LT;        // LT
        Ld->m[6].rtyp = INT_CMD; Ld->m[6].data = (void*)(long)counter;  // counter
        Ld->m[7].rtyp = INT_CMD; Ld->m[7].data = (void*)(long)counter;  // counter
        
        // Add Ld to LLT
        LLT->m[k].rtyp = LIST_CMD; LLT->m[k].data = Ld;
    }

    // Add the total number of generators to LLT
    LLT->m[r].rtyp = INT_CMD; LLT->m[r].data = (void*)(long)r;
    
    // Add field name for total number of generators
    lists field_names = (lists)omAlloc0Bin(slists_bin);
    field_names->Init(r + 1);
    for (int i = 0; i < r; i++) {
        field_names->m[i].rtyp = STRING_CMD;
        field_names->m[i].data = strdup("generator");
    }
    field_names->m[r].rtyp = STRING_CMD;
    field_names->m[r].data = strdup("total_number_generator");
    
    LLT->m[1].rtyp = LIST_CMD; LLT->m[1].data = field_names;

    return {r, LLT};  // Return r (number of generators) and LLT
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
  
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = leadsyz_GpI(args.leftV());
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













