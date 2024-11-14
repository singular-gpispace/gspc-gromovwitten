
lists pOLY_List(poly f0) {
   
    int r = pLength(f0); // Get the number of elements in f0
    int k = 0;

    lists S = (lists)omAlloc0Bin(slists_bin);
    S->Init(r); // Initialize list S with size r
    for (k = 0; k <= r-1; k++) {
        S->m[k] = pHead(f0); // Add leading monomial of f0 to S
        f0 = pNext(f0); // Move to the next polynomial
    }

    lists res = (lists)omAllocBin(sleftv_bin); // Allocate memory for result
    res->rtyp = LIST_CMD; // Set the type of the result
    res->Data() = (void *)S;
    return res;
}
//for vectors
lists vector_List(poly f0){ //poly f0  is singular vector here 
    int r= nrows(f0); //nrows of f0
    
    int k=0;
    int j=0;
    lists S = (lists)omAlloc0Bin(slists_bin);
    
    int cc=0;
    poly term=NULL;//vector term
    for(k=0; k<=r-1; k++){
       for(j=0; j<=size(f0[k])-1; j++){
        term =f0[k][j]*gen(k); //??
      if(term!=0){
        //  S[cc]=term;
        // cc++;

         S->m[cc].rtyp = VECTOR_CMD;
         S->m[cc].data = term;
                cc++;

     }

       }
    
        
    }
    
    lists res = (lists)omAllocBin(sleftv_bin); // Allocate memory for result
    res->rtyp = LIST_CMD; // Set the type of the result
    res->Data() = (void *)S;
    return res;
}


poly LOT(poly g0, ideal S1) 
{
    std::cout << "LOT_g= " << pString(g0) << std::endl;
    std::cout << "LOT_S1= " << IDELEMS(S1) << std::endl;
    poly t = NULL;
    int i = 0;
    poly LOT = pCopy(g0);

    std::cout << "LOT_initialize: " << pString(LOT) << std::endl;
    
    while (g0!= NULL) 
    {
        for (i = 0; i < IDELEMS(S1); i++) 
        {
            t = pHead(g0); // Leading term of g0
          
            bool c = p_DivisibleBy(S1->m[i], t, currRing);
            std::cout << "Divisibility check result: " << c << std::endl;
          
            if (c == TRUE)
             { // Check normal form
                std::cout << "LOT_found _t :" << t << std::endl;

                LOT = p_Sub(LOT, pCopy(t), currRing);
                break;
            }
        }
        
        g0 = p_Sub(g0, pCopy(t), currRing);
        std::cout << "LOT_return :" << pString(LOT) << std::endl;
    }

    return LOT;
}



poly LOT_12(poly g0, ideal S1, lists J, int level) {
    int r = IDELEMS(S1);
    int i = 0;
    int j = 0;
    int isDivisible = 0;
    poly LOT = NULL; // Initialize the vector LOT
    poly t = NULL; // Initialize the vector t
    poly term = NULL; // Initialize the vector term
    int k = 0;

    // Get the module F0 from the list J at the specified level
    ideal F0 = (ideal)J->m[level].Data();

    // Loop through each term in the vector g0
    for (k = 0; k < pLength(g0); k++) {
        for (j = 0; j < pLength(g0->m[k]); j++) {
            // Create the term by multiplying with the generator
            // Replace `gen(k)` with the appropriate construction
            // Assuming gen(k) corresponds to the k-th standard basis element (1 at position k, 0 elsewhere)
            poly generator = p_One(currRing); // Create the 1 polynomial
            generator = p_SetExp(generator, k+1, 1, currRing); // Set the exponent corresponding to the k-th variable
            p_Setm(generator, currRing); // Set the monomial properly
            
            term = p_Mult_q(g0[k][j], generator, currRing); // Multiply the term with the generator
            
            ideal Gb_t = idInit(1, 1);
            Gb_t->m[0] = term;
            isDivisible = 0;

            // Check divisibility by each generator of S1
            for (i = 0; i < r; i++) {
                if ((Gb_t != NULL) && (kNF(Gb_t, NULL, F0->m[i]) == NULL)) { 
                    // If divisible, set the flag and break
                    isDivisible = 1;
                    break;
                }
            }

            // If not divisible, add it to the LOT
            if (!isDivisible) {
                poly first_poly = Gb_t->m[0]; 
                t = p_Copy(first_poly, currRing); // Copy the polynomial
                LOT = p_Add_q(LOT, t, currRing); // LOT = LOT + t
            }
            
            // Clean up the generator to prevent memory leaks
            p_Delete(&generator, currRing);
        }
    }

    return LOT;
}



//returns singular vector
poly LOT_2(poly g0, module S1, lists J, int level) { //poly g0 is singular vector
    int r = IDELEMS(S1);
    int i=0
    int k=0;
    int j=0;
    int isDivisible=0;
    poly LOT=NULL;//vector LOT
    poly t=NULL;//vector t
    poly term=NULL; //vector term
 
   //module F0=J[level];
   module F0=J->m[level].CopyD();//module F0=J[leve]
    for (k = 0; k <= nrows(g0)-1; k++) {
     for(j=0; j<=size(g0[k])-1; j++){

          term=g0[k][j]*gen(k);//??
        
          isDivisible=0;

        for (i = 0; i <= r-1; i++) {
            if ((term != 0)&& (kNF(term, module(F0[i])) == 0)) { 
                // g[k] is divisible by the generator S[i]
                isDivisible = 1;
                break;  //No need to check further generators for this 
            }
        }
       
        if (!isDivisible) {
            t = pCopy(term);  //t=term
            LOT= p_Add_q(LOT,t,currRing); //LOT=LOT +t
            
        }
     }
       
            
           
    }
    // Allocate memory for result
     poly res = (poly)omAllocBin(sleftv_bin); //poly res is singular vector
    res->rtyp = VECTOR_CMD; // Set result type
    res->Data() = (void *)LOT;
    return res;

    
}

lists oneSublift(ideal f, poly s) { //poly s is singular vector
    
    int r = IDELEMS(f);
    poly h = NULL;
    poly g = NULL;

    int i = 0;
    int k = 0;
    int q = 0;
    int i0 = 0;

    poly lm_s = pHead(s); // Get the leading monomial of s including coefficient
     poly g1=pOne();


    for (i = 0; i < r; i++) {
     
         pSetComp(g1,i);

        poly tempPoly = pp_Mul_qq(lm_s, g1, currRing); // Multiply m by e_i

        // Check if tempPoly equals the vector s
        if (p_EqualPolys(tempPoly, s, currRing)) {
            h = pp_Mul_qq(lm_s, f[i], currRing); // Compute h = m * f[i]
            
           
            break; // Exit loop once found
        }
        
        p_Delete(&tempPoly, currRing); // Clean up temporary polynomial if not needed
       
    }

    //g = h - pHead(h); // Leading term of h
    g=pp_Sub_qq(h,pHead(h),currRing);
    poly T0 = (poly)LOT(g, f)->Data(); // poly T0=LOT(g, f)

    lists T = (lists)pOLY_List(T0)->Data(); // list T=pOLY_List(T0)

    

    poly s_hat = pCopy(s); // vector s_hat=s
    poly t = NULL;
    poly m1 = NULL;
    poly m = NULL;
    poly s_v = NULL;//vector s_v
    int C = 0;
    int cl = 0;
    int t_size = IDELEMS(T);

    lists TT = (lists)omAlloc0Bin(slists_bin); // Allocate memory for list TT
    TT->Init(t_size); // Initialize TT with size t_size
      poly h1=pOne();


    for (k = 0; k < t_size; k++) {
        t = T[k];

        for (q = 0; q < r; q++) {
            pSetComp(h1, q); // Set component of h1

            C = (kNF(t, pHead(f0->m[lambda].Data())) == NULL); // Check if normal form is NULL

            if (C == 1) {
                // Perform polynomial division
                //m1 = p_PolyDiv(t, lead_terms[q], currRing); // Multivariate division
                m1=pp_Divide(t,pHead(f0->m[lambda].Data())), currRing);
                s_v = pp_Mul_qq(m1, h1, currRing);


                m = pCopy(m1);//m=m1;
               
                  TT->m[cl].rtyp = VECTOR_CMD;
                  TT->m[cl].data = p_Mul_qq(m, h1, currRing);//TT[cl]=m*f[lambda]
                cl++;
                break;
            }
        }
    }
  
 lists res = (lists)omAllocBin(sleftv_bin); // Allocate memory for result
    res->rtyp = LIST_CMD;
    res->Data() = (void *)TT;
    
    return res;
}




lists subLiftTree2(module f, poly s, lists J, int level) {//poly s is singular vector
    int r = IDELEMS(f);
    poly g=NULL;  //vector g
    poly h=NULL;  //vector h
    poly H=NULL; //vector H
    int i=0;
  
    //module f0 = J[level];
      module F0=J->m[level].CopyD();
   
   // number lc_s = leadcoef(s);
    poly lm_s =  pHead(s); //leading monomial including coefficient

    poly g1=pOne();

    for ( i= 0; i < r; i++) {
       pSetComp(g1,i);
       poly tempPoly = pp_Mul_qq(lm_s, g1, currRing); // Multiply m by e_i
        if (p_EqualPolys(tempPoly, s, currRing)) {
            h = pp_Mul_qq(lm_s, f[i]);
            H = pp_Mul_qq(lm_s, f0[i],currRing);
            break;
        }
         p_Delete(&tempPoly, currRing); 
    }

    g = pp_Sub_qq(h, pHead(H),currRing); // g = h - lead(H)



    poly t0 = (poly)LOT_2(g, f, J, level)->Data(); // vector t0=LOT_2(g,f,J,level)


    
    poly T0 = pp_Sub_qq(g, t0,currRing);//vector T0=g - LOT_2(g, f, J, level)

   //lists T = vector_List(T0); 
   lists T = (lists)vector_List(T0)->Data();

    
    int t_size = size(T);//??
    lists TT = (lists)omAlloc0Bin(slists_bin);
    TT->Init(t_size);

    
    poly s_hat = pCopy(s); // vector s_hat:=s
    poly t=NULL;//vector t
    poly s_v=NULL; //vector s_v
     poly m=NULL:
     poly m1=NULL;
    int lambda=0;
    int counter = 0;
    int k=0;
    for (k = 0; k < size(T); k++) {
        t = T[k];
        for (lambda = 0; lambda <= r-1; lambda++) {
            int a = p_Head(t)[nvars(basering) + 1];//?
            int b = p_Head(f0[lambda])[nvars(basering) + 1];//p_Head for leadsxp

            if (a == b) {
                int C = (kNF(t, lead(f0[lambda])) == 0);
                if (C == 1) {
                    // Compute m1 and update TT   
                    
                    m1 = pp_Divide(pp_Mul_qq(leadcoef(t), pHead(t),currRing), p_Mul_qq(leadcoef(f0[lambda]), pHead(f0[lambda])), currRing);
  
                    s_v = pp_Mul_qq(m1, gen(lambda),currRing);
                    m = pCopy(m1);//m=m1;
                    TT->m[counter].rtyp = VECTOR_CMD;
                    TT->m[counter].data = pp_Mul_qq(m, gen(lambda),currRing);
                    counter++;
                    break;
                }
            }
        }
    }

    lists res = (lists)omAllocBin(sleftv_bin); // Allocate memory for result

    res->rtyp = LIST_CMD;
    res->Data() = (void *)TT;
    
    return res;
}












std::pair<int, lists> SubLIFT_GPI(leftv args){
  //extract Token
  lists Token = (lists)(args->Data()); 
// extract tok
  lists tok =  (lists)(args->next->Data()); 
//extract TT
  lists TT = (lists)(args->next->next->Data()); 
  //Token.data
  lists tmp = (lists)(Token->m[3].Data());
  //tok.data
  lists tmp1 = (lists)(tok->m[3].Data());
  //TT.data
   lists tmp2 = (lists)(TT->m[3].Data());
   //TT.data[6]
  int counter=(int)(long)tmp2->m[5].Data();
  //column
   int colmn = (int)(long)tmp2->m[3].Data(); 

   //J=tok.data[1]
   lists J=(lists)(tok->m[0].Data());

   
  poly s=pCopy(tmp2->m[0].Data()); //  vector s = TT.data[1];

  leftv L = nullptr; leftv LL = nullptr;
   int r0=0; int c=0;int l_size=0;
   //f=tmp->m[0]
  if (tmp->m[0].Typ() == IDEAL_CMD) {
    module l=(module)J->m[1].Data(); //module l=J[2]
    l_size=IDELEMS(l);
    r0= IDELEMS((ideal)(tmp->m[0].Data()));
    c=l_size;


}
else if (tmp->m[0].Typ() == VECTOR_CMD) {
  int level=counter;
  module l=(module)J[counter]->Data();
    
    int l_size = IDELEMS(l); 
    
    int r0= IDELEMS(l); 

     module LL=(module)J[level+1]->Data();
     int c=IDELEMS(LL);


}
 
if(tmp->m[0].Typ() == IDEAL_CMD){
          lists lL=(lists)oneSublift(f,s)->Data();// list lL=oneSublift(f,s)
          int r=IDELEMS(lL);
        } else if(tmp->m[0].Typ() == VECTOR_CMD){
           // matrix A[r0][c] =Token.data[2];
           matrix A;
            A = (matrix)tmp1->m[1].Data(); //A= Tok.data[2]
            module M=A;
            int level=counter;
           
            lists lL=(lists)subLiftTree2(M,s,J,level)->Data(); //list lL=subLiftTree2(f,s)
            int r=IDELEMS(lL);
           
          }
        
        // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    
    int k=0;


  for (k = 0; k <r; k++) {
      // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
     Ld->Init(4);// type token 
      // fieldnames //Ld.fieldnames = list("generator", "Sparse_matrix_Lift"); 
    lists t=(lists)omAlloc0Bin(slists_bin);
    t->Init(2);
     t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
    t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("Sparse_matrix_SubLift");
     
     Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
     Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
     Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;


    matrix sM[r0][c];
    
           
    //vector s_lift =lL[k];//added c_n
    poly s_lift=(poly)lL->m[k]; //vector s_lift=lL[k]
     
    int l_k = leadexp(s_lift)[nvars(basering) + 1];//??
  
    sM[l_k, colmn] =pp_Mul_qq(-1, pHead(s_lift), currRing); 
    
     //data  Ld.data
    t=(lists)omAlloc0Bin(slists_bin);
    t->Init(7);

    //Ld.data[1]=(-1)*s_lift; 
    t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pp_Mul_qq(-1,s_lift, currRing);
    

    //Ld.data[2]= sM;
     t->m[1].rtyp=MATRIX_CMD; t->m[1].data=sM;

     //Ld.data[3] = l_k;
    t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)l_k;
    
    
      // Ld.data[4]=colmn;
    t->m[3].rtyp=INT_CMD;  t->m[3].data = (void*)(long)colmn;
   

    if (tmp->m[0].Typ() == IDEAL_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[1].Data();//J[2]

     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) {
      int level=counter;
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[level+1].Data(); //Ask Hans
     }
   
   // Ld.data[6]=counter;
    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
//Ld.data[7] = counter;
   t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);
   Ld->m[3].rtyp=LIST_CMD; Ld->m[3].data=t;//data is in postion 4
     // fieldnames in token LLT
    lists t0=(lists)omAlloc0Bin(slists_bin);
    t0->Init(r);
    // LLT.fieldnames[k] = "generator"; 
     t0->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
     LLT->m[1].rtyp=LIST_CMD; output->m[1].data=t0;
     LLT->m[0].rtyp=RING_CMD; output->m[0].data=currRing;
     LLT->m[2].rtyp=RING_CMD; output->m[2].data=currRing;



     //LLT.data[k] = Ld; 
     
    lists t0 = (lists)LLT->m[1].data;
    if (t0 == nullptr) {
        t0 = (lists)omAlloc0Bin(slists_bin);
        t0->Init(r); // Initialize with r elements to store each Ld
        LLT->m[1].rtyp = LIST_CMD;
        LLT->m[1].data = t0;
    }
   
    t0->m[k].rtyp=LIST_CMD; t0->m[k].data=Ld;
   
  }
     //int cmd_nr;
     //blackboxIsCmd("token",cmd_nr); // get the type id of "token"
     
     return {r,LLT};
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
    
   

    // Debugging: Print worker ID
    std::string ids = worker();
    //std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> Lead;
    std::pair<int, lists> All_Lead;
    
   
    input = deserialize(input_filename, ids);
    All_Lead = deserialize(all_lead, ids);
  
    
   
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
    
    
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
   out = SubLIFT_GPI(args.leftV());  // Call SubLIFT_GPI with the raw pointer
    auto end_computation = std::chrono::high_resolution_clock::now();
   auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
 
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
   
    
   
     std::vector<std::string> vec;
     int total_generator;
   
    //Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
 

   auto total_runtime=computation_time;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}




//Uptodate Version from here:


lists pOLY_List(poly f0)
 {
  // Get the length of the polynomial
    int r = pLength(f0);
    std::cout << "input_pOLYList size: " << r << std::endl;

    // Handle the case of a zero polynomial
    if (r == 0) 
    {
        std::cout << "f0 is a zero polynomial, returning empty list." << std::endl;
       lists S = (lists)omAlloc0Bin(slists_bin);
      
        S->Init(0);
        return S;
    }

    // Allocate memory for the list and initialize it
    lists S = (lists)omAlloc0Bin(slists_bin);
    S->Init(r);

    // Populate the list with terms from the polynomial
    for (int k = 0; k < r; k++)
     {
       
        S->m[k].rtyp = POLY_CMD; // Set the type of the entry
        S->m[k].data = pHead(f0); // Get the current term (leading monomial)
        
        // Debugging output
        std::cout << "Adding term: " << S->m[k].data << " at index " << k << std::endl;
        
        f0 = pNext(f0); // Move to the next term in the polynomial
    }

    // Final output of the populated list size
    std::cout << "Final size of pOLY_List: " << lSize(S) << std::endl;
    return S; // Return the populated list
}











//for vectors
lists vector_List(poly f0) //poly f0 is singular vector
{ 
    int r = pLength(f0); 
    lists S = (lists)omAlloc0Bin(slists_bin); // Allocate memory for the list
    S->Init(r); // Initialize the list to hold r elements

    int cc = 0; // Counter for the elements added to S

    for (int i = 0; i < r; i++) 
    { // Loop through each element of the vector f0
        poly term = pCopy(f0); // Start with the current polynomial

        while (term != NULL) 
        { // Traverse through each monomial in the polynomial
            poly term_copy = p_Copy(pHead(term), currRing); // Copy the leading monomial

            if (term_copy != NULL)
             {
                S->m[cc].rtyp = VECTOR_CMD; // Set the type of the entry
                S->m[cc].data = term_copy; // Store the copied term in the list
                cc++; // Increment the counter
            }

            term = pNext(term); // Move to the next monomial
        }

        f0 = pNext(f0); // Move to the next polynomial in the vector
    }

    // Adjust the list size to the number of elements added
    S->Init(cc); // Initialize the list with the count of elements added
    return S;
}







poly LOT(poly g0, ideal S1) 
{
    std::cout << "LOT_g= " << pString(g0) << std::endl;
    std::cout << "LOT_S1= " << IDELEMS(S1) << std::endl;
    poly t = NULL;
    int i = 0;
    poly LOT = pCopy(g0);

    std::cout << "LOT_initialize: " << pString(LOT) << std::endl;
    
    while (g0!= NULL) 
    {
        for (i = 0; i < IDELEMS(S1); i++) 
        {
            t = pHead(g0); // Leading term of g0
          
            bool c = p_DivisibleBy(S1->m[i], t, currRing);
            std::cout << "Divisibility check result: " << c << std::endl;
          
            if (c == TRUE)
             { // Check normal form
                std::cout << "LOT_found _t :" << t << std::endl;

                LOT = p_Sub(LOT, pCopy(t), currRing);
                break;
            }
        }
        
        g0 = p_Sub(g0, pCopy(t), currRing);
        std::cout << "LOT_return :" << pString(LOT) << std::endl;
    }

    return LOT;
}


//returns singular vector
poly LOT_2(poly g0, ideal S1, lists J, int level) {
    // Convert g0 to a list of polynomials (terms)
    lists termList = vector_List(g0);
    
    poly LOT = NULL;  // Resulting polynomial to hold non-divisible terms

    int r = IDELEMS(S1);  // Get the number of generators in F0
    ideal F0 = (ideal)J->m[level].Data(); // Get the module F0 from J[level]

    int termCount = lSize(termList);  // Get the number of terms in termList
   ideal Gb_t=NULL;
    // Iterate over each term in termList
    for (int i = 0; i < termCount; i++) {
        poly term = (poly)termList->m[i].Data();  // Get the i-th term from the list
        bool isDivisible = false;

        // Check divisibility of the term against each leading term in F0
        for (int j = 0; j < r; j++) {
            poly leadingTermF0 = pHead(F0->m[j]);  // Get the leading term of F0[j]

            Gb_t = idInit(1, 1); // Initialize a new ideal Gb_t
            Gb_t->m[0] = term; // Set the polynomial term in Gb_t

            // Check if the term is divisible by the leading term of F0[j]
            if ((Gb_t!=NULL)&& kNF(Gb_t, NULL, leadingTermF0) == NULL) {
                isDivisible = true;  // Mark as divisible
                omFreeBin(Gb_t, ideal_bin); // Free memory for Gb_t
                break;  // No need to check further generators
            }
                 omFreeBin(Gb_t, ideal_bin); 
          
        }

        // If the term is not divisible by any generator, add it to LOT
        if (!isDivisible) {
          
           // poly termCopy = p_Copy(term, currRing);  // Copy the term
            LOT = p_Add_q(LOT, term, currRing);  // Add the term to LOT
        }
    }

    // Free memory allocated for termList
    
    omFreeBin(termList, slists_bin);
    

    return LOT;  // Return the list of non-divisible terms
}








lists oneSublift(ideal f, poly s) //poly s is singular vector
{ 
    
    int r = IDELEMS(f);
    std::cout << "oneSublift_ideal: " << r << std::endl;
    std::cout << "oneSublift_vector: " << s << std::endl;
    poly h = NULL;
    poly g = NULL;

   // int i = 0;
    int k = 0;
    int q = 0;
   
   //For s= c*x^a*gen(i);
   //poly lm_s=c*x^a;
    poly lm_s = pHead(s); // Get the leading monomial of s including coefficient
    
   pWrite(s);
   
    pSetComp(lm_s,0);pSetmComp(lm_s);

    int g1= p_GetComp(s,currRing);
     
   std::cout << "GetComp: " << g1 << std::endl;
   h=pp_Mult_qq(lm_s,f->m[g1],currRing);
  //std::cout << "h=lm_s*f[g1]: " << h << std::endl;
    

    //g = h - pHead(h); // Leading term of h
    g=p_Sub(h,pHead(h),currRing);
    //std::cout << "g =h-LT(h): " << pLength(g) << std::endl;
    poly lOT=LOT(g, f); //call function LOT
    std::cout << "lOT_onesubLift: " << pLength(lOT) << std::endl;
    //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(g,lOT,currRing);
    std::cout << "size _g-LOT(g, f): " << pLength(T0) << std::endl;
    lists T=pOLY_List(T0);  

    
    std::cout << "pOLY_List(T0): " << lSize(T) << std::endl;
    //poly s_hat = pCopy(s); // vector s_hat=s
    
    poly m1 = NULL;
    poly m = NULL;
    poly s_v = NULL;//vector s_v
    int C = 0;
    int cl = 0;
    int t_size = lSize(T);
    std::cout << "size of T: " << t_size << std::endl;
    lists TT = (lists)omAlloc0Bin(slists_bin); // Allocate memory for list TT
    TT->Init(t_size); // Initialize TT with size t_size
     


  for (k = 0; k < t_size; k++) 
  {
      poly t = (poly)T->m[k].Data();// poly t=T[k]

      for (q = 0; q < r; q++)
      {
           
        ideal Gb_t=idInit(1,1);
        Gb_t->m[0]=t;
        C = (kNF(Gb_t,NULL, pHead(f->m[q])) == NULL); 


          if (C == 1) 
          {
            // Perform polynomial division
            //m1 = p_PolyDiv(t, lead_terms[q], currRing); 
            m1=pp_Divide(t,pHead(f->m[q]), currRing);
                 
            m = pCopy(m1); //m=m1;
            p_SetComp(m,q,currRing);
            p_SetmComp(m,currRing);
            s_v=m;
            TT->m[cl].rtyp = VECTOR_CMD;
            TT->m[cl].data=s_v;  //TT[cl]=m*f[lambda]
              cl++;
              break;
          }
      }
  }
  


 return TT;
}




lists subLiftTree2(ideal f, poly s, lists J, int level) {//poly s is singular vector  
    //ideal f is singular module
    int r = IDELEMS(f);
    poly g=NULL;  //vector g
    poly h=NULL;  //vector h
    poly H=NULL; //vector H
    //int i=0;
  
    //module f0 = J[level];
      ideal f0=(ideal)J->m[level].Data();//
   
   // number lc_s = leadcoef(s);
    poly lm_s =  pHead(s); //leading monomial including coefficient

    int g1= p_GetComp(s,currRing);


        
    
        h = pp_Mult_qq(lm_s, f->m[g1],currRing);
        H=pp_Mult_qq(lm_s, f0->m[g1],currRing);
   
     
  

    g = p_Sub(h, pHead(H),currRing); // g = h - lead(H)


// vector t0=LOT_2(g,f,J,level)
    poly t0=LOT_2(g,f,J,level);//poly t0 = (poly)LOT_2(g, f, J, level)->next; 


    
    poly T0 = p_Sub(g, t0,currRing);//vector T0=g - LOT_2(g, f, J, level)

   
      lists T = vector_List(T0); //lists T = (lists)vector_List(T0)->next;

    
    int t_size = lSize(T);
    lists TT = (lists)omAlloc0Bin(slists_bin);
    TT->Init(t_size);

    
    //poly s_hat = pCopy(s); // vector s_hat:=s
    
    poly s_v=NULL; //vector s_v
     poly m=NULL;
     poly m1=NULL;
    int lambda=0;
    int counter = 0;
    int k=0;
    for (k = 0; k < lSize(T); k++) {
        poly t = (poly)T->m[k].Data(); //vector t=T[k]
        for (lambda = 0; lambda < r; lambda++) {
            int a = p_GetComp(t,currRing);
            int b = p_GetComp(f0->m[lambda],currRing);//p_Head for leadsxp
           ideal Gb_t=idInit(1,1);
           Gb_t->m[0]=t;
            if (a == b) {
                int C = (kNF(Gb_t,NULL, pHead(f0->m[lambda])) == 0);
                if (C == 1) {
                    // Compute m1 and update TT   
                    
                    m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);
                     m = pCopy(m1);//m=m1;
                   p_SetComp(m,lambda,currRing);
                   p_SetmComp(m,currRing);
                   s_v=m;
                   // s_v = pp_Mult_qq(m1, poly(lambda),currRing);
               
                  
                      // Store the result in TT
                  TT->m[counter].rtyp =VECTOR_CMD;
                  TT->m[counter].data= s_v;//TT[cl]=m*f[lambda]
              
                    counter++;
                    break;
                }
            }
        }
    }

      


    return TT;
}

std::pair<int, lists> SubLIFT_GPI(leftv args){

  
  //extract Token
 
  lists Token = (lists)(args->Data()); 
// extract tok

  lists tok =  (lists)(args->next->Data()); 
//extract TT

  lists TT = (lists)(args->next->next->Data()); 
  //Token.data
  lists tmp = (lists)(Token->m[3].Data());
  //tok.data
  lists tmp1 = (lists)(tok->m[3].Data());
  //TT.data
   lists tmp2 = (lists)(TT->m[3].Data());
   //TT.data[6]
  int counter=(int)(long)tmp2->m[5].Data();
  //column
   int colmn = (int)(long)tmp2->m[3].Data(); 
     //std::cout << "SubLIFT_GPI_Counter: " << counter << std::endl;
   //J=tok.data[1]
   lists J=(lists)(tmp1->m[0].Data());
   //std::cout << "SubLIFT_GPI_LIST: " << J << std::endl;
  
  poly s=(poly)tmp2->m[0].Data(); //  vector s = TT.data[1];
    //std::cout << "SubLIFT_GPI_vector: " << s << std::endl;
  
   int r0=0; int c=0;int l_size=0;
    int L_size=lSize(J);
       std::cout << "Size of J: " << L_size << std::endl;
   ideal l=NULL;
   ideal LL=NULL;
  if (tmp->m[0].Typ() == IDEAL_CMD) {
    l =(ideal)J->m[1].Data(); //module l=J[2]
    //std::cout << "SubLIFT_GPI_Module: " << l << std::endl;
    l_size=IDELEMS(l);
    //std::cout << "SubLIFT_GPI_Module_size: " << l_size << std::endl;
    r0= IDELEMS((ideal)(tmp->m[0].Data())); 
    c=l_size;
      r0=r0;


}
else if (tmp->m[0].Typ() == VECTOR_CMD) {
  int level=counter;
   l=(ideal)J->m[counter-1].Data(); //module l=J[counter]
    
    int l_size = IDELEMS(l); 
    l_size=l_size;
    int r0= IDELEMS(l); 
     r0=r0;
     LL=(ideal)J->m[level].Data(); //module LL=J[counter+1]
     int c=IDELEMS(LL);
     c=c;


}
 lists lL=NULL;
 int r=0;
if(tmp->m[0].Typ() == IDEAL_CMD){
   ideal f=(ideal)(tmp->m[0].Data());
   // std::cout << "SubLIFT_GPI_Input: " << f << std::endl;
     //std::cout << "SubLIFT_GPI_Size(f): " << IDELEMS(f) << std::endl;
      lL=oneSublift(f,s); // lists lL=(lists)oneSublift(f,s).Data();
       //std::cout << "SubLIFT_GPI_oneSublift: " << IDELEMS(f) << std::endl;
          r = lSize(lL);
          r=r;
           std::cout << "SubLIFT_GPI_oneSublift_size: " << r << std::endl;
        } else if(tmp->m[0].Typ() == VECTOR_CMD){
           // matrix A[r0][c] =Token.data[2];
           matrix A;
            A = (matrix)tmp1->m[1].Data(); //A= Tok.data[2]
           // ideal M = id_Copy((matrix)A);//module M=A
            ideal M=id_Matrix2Module(A,currRing);
            int level=counter;
          
            lL=subLiftTree2(M,s,J,level);// lists lL=(lists)subLiftTree2(M,s,J,level).Data(); 
            r = lSize(lL);
            r=r;
           
          }
        
        // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    
    int k=0;


  for (k = 0; k <r; k++) {
      // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
     Ld->Init(4);// type token 
      // fieldnames //Ld.fieldnames = list("generator", "Sparse_matrix_Lift"); 
    lists t=(lists)omAlloc0Bin(slists_bin);
    t->Init(2);
     t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
    t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("Sparse_matrix_SubLift");
     
     Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
     Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
     Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;


    //matrix sM[r0][c];
    matrix sM= mpNew(r0,c);
  //sM = (matrix)omAllocBin(smatrix_bin); // Allocate memory for the matrix

    //vector s_lift =lL[k];//added c_n
    poly s_lift=(poly)lL->m[k].Data(); //vector s_lift=lL[k]
     
    //int l_k = p_Head(s_lift)[nvars(basering) + 1];// p_Head for leadexp 
    int l_k = p_GetComp(s_lift, currRing);
  
    //sM[l_k,colmn] =pCopy(pp_Mult_qq(-1, pHead(s_lift), currRing)); 
    MATELEM(sM,l_k,colmn)=pCopy(pp_Mult_qq(poly(-1),pHead(s_lift), currRing)); //should include -1
    
     //data  Ld.data
    t=(lists)omAlloc0Bin(slists_bin);
    t->Init(7);

    //Ld.data[1]=(-1)*s_lift; 
    t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pp_Mult_qq(poly(-1),s_lift, currRing);//should include -1
    

    //Ld.data[2]= sM;
     t->m[1].rtyp=MATRIX_CMD; t->m[1].data=sM;

     //Ld.data[3] = l_k;
    t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)l_k;
    
    
      // Ld.data[4]=colmn;
    t->m[3].rtyp=INT_CMD;  t->m[3].data = (void*)(long)colmn;
   

    if (tmp->m[0].Typ() == IDEAL_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[1].Data();//J[2]

     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) {
      int level=counter;
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[level].Data(); // Ld.data[5]=J[level+1];
     }
   
   // Ld.data[6]=counter;
    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
//Ld.data[7] = counter;
   t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);
   Ld->m[3].rtyp=LIST_CMD; Ld->m[3].data=t;//data is in postion 4
     // fieldnames in token LLT
  // Set field names for LLT
lists field_names = (lists)omAlloc0Bin(slists_bin);
field_names->Init(r);
for (int s = 0; s < r; s++) {
    field_names->m[s].rtyp = STRING_CMD;
    field_names->m[s].data = strdup("generator");  // Set fieldnames[k] to "generator"
}
 // Assign fieldnames to LLT
LLT->m[1].rtyp = LIST_CMD;
LLT->m[1].data = field_names;

LLT->m[0].rtyp=RING_CMD; LLT->m[0].data=currRing;
LLT->m[2].rtyp=RING_CMD; LLT->m[2].data=currRing;

     //LLT.data[k] = Ld; 
     
    
   // Set data for LLT
lists t0 = (lists)omAlloc0Bin(slists_bin);
t0->Init(r);
for (int s = 0; s < r; s++) {
    t0->m[s].rtyp = LIST_CMD;
    t0->m[s].data = lCopy(Ld);  // Set fieldnames[k] to "generator"
}
 // Assign fieldnames to LLT
LLT->m[3].rtyp = LIST_CMD;
LLT->m[3].data = t0;

   
  }
  //  lists t1 = (lists)omAlloc0Bin(slists_bin);
  //  t1->Init(r+1);
  //  for(int i=0;i<r; i++){
  //  t1->m[i]=LLT->m[i];
  //  t1->m[r].data=strdup("total_gen");t1->m[r].rtyp=STRING_CMD;
  //  t1->m[i]=LLT->m[i];
  //  t1->m[r].rtyp=INT_CMD;
  //  t1->m[r].data=r;
  //  }
 
   //LLT.fieldnames[size(LLT.fieldnames) + 1] = "total_number_generator";  
    //LLT.data[size(LLT.data) + 1] = r;  
   
     //int cmd_nr;
     //blackboxIsCmd("token",cmd_nr); // get the type id of "token"
     
     return {r,LLT};
  }



NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int, long> singular_template_SUBLIFT(const std::string& input_filename,
                                                                 const std::string& all_lead,
                                                                const std::string& lead,
                                                               
                                                  const std::string& needed_library,
                                                   const std::string& base_filename)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print out function parameters

  // std::cout << "Input Filename_SubLIFT: " << input_filename << std::endl;
  //  std::cout << "AllLEAD _SubLIFT: " << all_lead << std::endl;
  // std::cout << "LIFT in _SubLIFT: " << lead << std::endl;
  // std::cout << "Needed Library_SubLIFT: " << needed_library << std::endl;
  // std::cout << "Base Filename_SubLIFT: " << base_filename << std::endl;

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
    
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
    
    //std::cout<<"function_name_SubLIFT:"<< std:: endl;
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = SubLIFT_GPI(args.leftV());  // Call  SubLIFT_GPI with the raw pointer
    auto end_computation = std::chrono::high_resolution_clock::now();
   auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
  std::cout << "SubLIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    //std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
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
