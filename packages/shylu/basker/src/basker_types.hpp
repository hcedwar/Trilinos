#ifndef BASKER_TYPES_HPP
#define BASKER_TYPES_HPP

//MACRO TURN ON FUCNTIONS
#define BASKER_KOKKOS         //Use Kokkos
#define BASKER_ATOMIC         //Use Atomics (OLD)
#define BASKER_ATOMIC_2       //Use Atomics (OLD)
#define BASKER_NO_LAMBDA      //Do not use lambda
#define BASKER_2DL            //Use a 2D L
#define BASKER_MULTIPLE_UPPER //Use multiple threads for upper
#define BASKER_MULTIPLE_LOWER //Use multiple threads for lower
#define HAVE_AMESOS           //Use Amesos orderings
#define BASKER_SCOTCH         //Use Scotch


//MACRO TYPE
#define BASKER_INT            Int
#define BASKER_ENTRY          Entry
#define BASKER_BOOL           bool
#define BASKER_TRUE           true
#define BASKER_FALSE          false
#define BASKER_NO_OP          ((void)0)
#define BASKER_MAX_IDX        -1         //What we check against
#define BASKER_ERROR          -1         //What we return with an error

#define BASKER_EPSILON       1e-6


//MACRO BTF METHOD
#define BASKER_BTF_MAX_PERCENT  1.00
#define BASKER_BTF_LARGE        500  //Made smaller for unit test
//#define BASKER_BTF_LARGE        100

//MACRO SYSTEM FUNCTIONS
#ifdef BASKER_DEBUG
//#define ASSERT(a)             BASKER_NO_OP
#define ASSERT(a)             assert(a)
#else
//#define ASSERT(a)             BASKER_NO_OP
#define   ASSERT(a)           assert(a)
#endif

#define BASKER_ASSERT(a,s)       \
  {                              \
    if(!(a))                     \
      {printf("\n\n%s\n\n", s);} \
    ASSERT(a);                   \
  } 


//Note:  Should see if Kokkos has a fast memory cpy in place of for-loop
//MACRO ARRAY FUNCTIONS
#ifdef BASKER_KOKKOS
//Execution Space
#include <Kokkos_Core.hpp>
#define BASKER_EXE_SPACE     Kokkos::OpenMP
//ReMacro Basker Classes
#define BASKER_SOLVER        Basker<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
#define BASKER_MATRIX        BaskerMatrix<BASKER_INT, BASKER_ENTRY, BASKER_EXE_SPACE>
#define BASKER_MATRIX_VIEW   BaskerMatrixView<BASKER_INT, BASKER_ENTRY, BASKER_EXE_SPACE>
#define BASKER_STATS         BaskerStats<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
//ReMacro Basker Structs
#define BASKER_TREE          basker_tree<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
#define BASKER_SYMBOLIC_TREE basker_symbolic_tree<BASKER_INT, BASKER_ENTRY, BASKER_EXE_SPACE>
#define BASKER_THREAD        basker_thread<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
//Macro Arrays
#define KOKKOS_NOINIT             Kokkos::ViewAllocateWithoutInitializing
#define INT_1DARRAY               Kokkos::View<BASKER_INT*,          BASKER_EXE_SPACE>
#define INT_2DARRAY               Kokkos::View<INT_1DARRAY*,         BASKER_EXE_SPACE> 
#define ENTRY_1DARRAY             Kokkos::View<BASKER_ENTRY*,        BASKER_EXE_SPACE>
#define ENTRY_2DARRAY             Kokkos::View<ENTRY_1DARRAY*,       BASKER_EXE_SPACE>  
#define BOOL_1DARRAY              Kokkos::View<BASKER_BOOL*,         BASKER_EXE_SPACE>
#define BOOL_2DARRAY              Kokkos::View<BOOL_1DARRAY*,        BASKER_EXE_SPACE>
#define MATRIX_1DARRAY            Kokkos::View<BASKER_MATRIX*,       BASKER_EXE_SPACE>
#define MATRIX_2DARRAY            Kokkos::View<MATRIX_1DARRAY*,      BASKER_EXE_SPACE>
#define MATRIX_VIEW_1DARRAY       Kokkos::View<BASKER_MATRIX_VIEW*,  BASKER_EXE_SPACE>
#define MATRIX_VIEW_2DARRAY       Kokkos::View<MATRIX_VIEW_1DARRAY*, BASKER_EXE_SPACE>
#define THREAD_1DARRAY            Kokkos::View<BASKER_THREAD*,       BASKER_EXE_SPACE>
#define THREAD_2DARRAY            Kokkos::View<THREAD_1DARRAY*,      BASKER_EXE_SPACE>
//Macro Memory Calls
//MALLOC
#define MALLOC_INT_1DARRAY(a,s)         \
  { \
    BASKER_ASSERT(s>0, "init_1d"); \
    a = INT_1DARRAY(KOKKOS_NOINIT("int_1d"),s); \
  }
#define MALLOC_INT_2DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0,"int_2d"); \
    a = INT_2DARRAY("int_2d",s); \
  }
#define MALLOC_ENTRY_1DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "entry_1d"); \
    a = ENTRY_1DARRAY(KOKKOS_NOINIT("entry_1d"),s); \
  }
#define MALLOC_ENTRY_2DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "entry_2d"); \
    a = ENTRY_2DARRAY("entry_2d",s); \
  }
#define MALLOC_BOOL_1DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "bool_1d"); \
    a = BOOL_1DARRAY(KOKKOS_NOINIT("bool_1d"), s); \
  }
#define MALLOC_BOOL_2DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "bool_2d"); \
    a = BOOL_2DARRAY("bool_2d", s); \
  }
#define MALLOC_MATRIX_1DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "matrix_1d"); \
    a = MATRIX_1DARRAY("matrix_1d",s); \
  }
#define MALLOC_MATRIX_2DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "matrix_2d"); \
    a = MATRIX_2DARRAY("matrix_2d",s); \
  }
#define MALLOC_MATRIX_VIEW_1DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "matrix_view_1d"); \
    a = MATRIX_VIEW_1DARRAY("matrix_view_1d",s); \
  }
#define MALLOC_MATRIX_VIEW_2DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "matrix_view_2d"); \
    a = MATRIX_VIEW_2DARRAY("matrix_view_2d",s); \
  }
#define MALLOC_THREAD_1DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "thread_1d"); \
    a = THREAD_1DARRAY("thread_1d",s); \
  }
#define MALLOC_THREAD_2DARRAY(a,s) \
  { \
    BASKER_ASSERT(s>0, "thread_2d"); \
    a = THREAD_2DARRAY("thread_2d",s); \
  }
//RESIZE (with copy)
#define RESIZE_1DARRAY(a,os,s)           Kokkos::resize(a,s)
#define RESIZE_2DARRAY(a,os1,os2s1,s2)   Kokkos::resize(a,s1,s2)
#define RESIZE_INT_1DARRAY(a,os,s)       RESIZE_1DARRAY(a,os,s)
#define RESIZE_ENTRY_1DARRAY(a,os,s)     RESIZE_1DARRAY(a,os,s)
//REALLOC (no copy)
#define REALLOC_1DARRAY(a,os,s)          Kokkos::realloc(a,s)
#define REALLOC_2DRRAAY(a,os1,os2,s1,s2) Kokkos::realloc(a,s1,s2)
#define REALLOC_INT_1DARRAY(a,os,s)      REALLOC_1DARRAY(a,os,s)
#define REALLOC_ENTRY_1DARRAY(a,os,s)    REALLOC_1DARRAY(a,os,s)
//Set values
#define SET_INT_1DARRAY(a, b, s)    	\
  {						\
  MALLOC_INT_1DARRAY(a,s);			     \
  for(BASKER_INT MACRO_I = 0; MACRO_I< s; MACRO_I++)	\
    a[MACRO_I] = b[MACRO_I];				\
  }
       
#define SET_ENTRY_1DARRAY(a, b,s)		\
  {						\
  MALLOC_ENTRY_1DARRAY(a,s);			    \
  for(BASKER_INT MACRO_I =0; MACRO_I <s; MACRO_I++)	\
    a[MACRO_I] = b[MACRO_I];				\
  }
  
#define SET_BOOL_1DARRAY(a, b, s)		\
  {						\
  MALLOC_BOOL_1DARRY(a,s);			    \
  for(BASKER_INT MACRO_I =0; MACRO_I <s; MACRO_I++)	\
    a[MACRO_I] = b[MACRO_I];				\
  }

#define FREE(a)                        BASKER_NO_OP

#else
//Execution Space
#define BASKER_EXE_SPACE     void*
//ReMacro Basker Classes
#define BASKER_SOLVER        Basker<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
#define BASKER_MATRIX        BaskerMatrix<BASKER_INT, BASKER_ENTRY, BASKER_EXE_SPACE>
#define BASKER_MATRIX_VIEW   BaskerMatrixView<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
#define BASKER_STATS         BaskerStats<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
//ReMacor Basker Structs
#define BASKER_TREE          basker_tree<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
#define BASKER_SYMBOLIC_TREE basker_symbolic_tree<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
#define BASKER_THREAD        basker_thread<BASKER_INT,BASKER_ENTRY,BASKER_EXE_SPACE>
//Array Types
#define INT_1DARRAY          BASKER_INT*
#define INT_2DARRAY          BASKER_INT**
#define ENTRY_1DARRAY        BASKER_ENTRY*
#define ENTRY_2DARRAY        BASKER_ENTRY**
#define BOOL_1DARRAY         BASKER_BOOL*
#define BOOL_2DARRAY         BASKER_BOOL**
#define MATRIX_1DARRAY       BASKER_MATRIX*
#define MATRIX_2DARRAY       BASKER_MATRIX**
#define MATRIX_VIEW_1DARRAY  BASKER_MATRIX_VIEW*
#define MATRIX_VIEW_2DARRAY  BASKER_MATRIX_VIEW**
#define THREAD_1DARRAY       BASKER_THREAD*
#define THREAD_2DARRAY       BASKER_THREAD**

//Macro Memory Calls
//Malloc
#define MALLOC_INT_1DARRAY(a,s)          a = new BASKER_INT         [s]
#define MALLOC_INT_2DARRAY(a,s)          a = new INT_1DARRAY        [s]
#define MALLOC_ENTRY_1DARRAY(a,s)        a = new BASKER_ENTRY       [s]
#define MALLOC_ENTRY_2DARRAY(a,s)        a = new ENTRY_1DARRAY      [s]
#define MALLOC_BOOL_1DARRAY(a,s)         a = new BASKER_BOOL        [s]
#define MALLOC_BOOL_2DARRAY(a,s)         a = new BOOL_1DARRAY       [s]
#define MALLOC_MATRIX_1DARRAY(a,s)       a = new BASKER_MATRIX      [s]
#define MALLOC_MATRIX_2DARRAY(a,s)       a = new MATRIX_1DARRAY     [s]
#define MALLOC_MATRIX_VIEW_1DARRAY(a,s)  a = new BASKER_MATRIX_VIEW [s]
#define MALLOC_MATRIX_VIEW_2DARRAY(a,s)  a = new MATRIX_VIEW_1DARRAY[s]
#define MALLOC_THREAD_1DARRAY(a,s)       a = new BASKER_THREAD      [s]
#define MALLOC_THREAD_2DARRAY(a,s)       a = new THREAD_1DARRAY     [s]
//Resize (copy old data) (come back and add)
#define RESIZE_1DARRAY(a,os,s)               BASKER_NO_OP
#define RESIZE_2DARRAY(a,os1,os2,s1,s2)      BASKER_NO_OP
#define RESIZE_INT_1DARRAY(a,os,s)           BASKER_NO_OP
#define RESIZE_ENTRY_1DARRAY(a,os,s)         BASKER_NO_OP
//Realloc (dont copy old data)
#define REALLOC_1DARRAY(a,os,s)              BASKER_NO_OP
#define REALLOC_2DARRAY(a,os1,os2,s1,s2)     BASKER_NO_OP
#define REALLOC_INT_1DARRAY(a,os,s)          BASKER_NO_OP
#define REALLOC_ENTRY_1DARRAY(a,os,s)        BASKER_NO_OP
//Set functions
#define SET_INT_1DARRAY(a,b,s)           a = b
#define SET_ENTRY_1DARRAY(a,b,s)         a = b
#define SET_ENTRY_1DARRAY(a,b,s)         a = b  
#define FREE(a)                    delete [] a

#endif //end ifdef BASKER_KOKKOS

//Inline command
#ifdef BASKER_KOKKOS
#define BASKER_INLINE   KOKKOS_INLINE_FUNCTION
#else
#define BASKER_INLINE    
//#define BASKER_INLINE   _forceinline
//#define BASKER_INLINE inline
//#dfinee BASKER_INLINE #pragma forceinline
#endif

#define BASKER_FINLINE  inline

//Time Macro
#ifdef BASKER_TIME
#ifdef BASKER_KOKKOS
#define BASKER_KOKKOS_TIME
#else
#define BASKER_OMP_TIME
#endif
#endif

#endif //end basker_types_hpp
