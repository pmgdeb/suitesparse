//------------------------------------------------------------------------------
// GrB_init: initialize GraphBLAS
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// GrB_init must called before any other GraphBLAS operation.  GrB_finalize
// must be called as the last GraphBLAS operation.

// GrB_init defines the mode that GraphBLAS will use:  blocking or
// non-blocking.  With blocking mode, all operations finish before returning to
// the user application.  With non-blocking mode, operations can be left
// pending, and are computed only when needed.

// The GrB_wait function forces all pending operations to complete.  Blocking
// mode is as if the GrB_wait operation is called whenever a GraphBLAS
// operation returns to the user.

// The non-blocking mode can have side effects if user-defined functions have
// side effects or if they rely on global variables, which are not under the
// control of GraphBLAS.  Suppose the user creates a user-defined operator that
// accesses a global variable.  That operator is then used in a GraphBLAS
// operation, which is left pending.  If the user then changes the global
// variable, the pending operations will be eventually computed with this
// different value.

// Worse yet, a user-defined operator can be freed before it is needed to
// finish a pending operation.  To avoid this, call GrB_wait before modifying
// any global variables relied upon by user-defined operators and before
// freeing any user-defined types, operators, monoids, or semirings.

// This version of SuiteSparse:GraphBLAS does not actually require a call to
// GrB_init.  All required global variables are statically initialized to their
// proper values.  However, for best practice, GrB_init should be called prior
// to any other call to GraphBLAS functions.

#include "GB.h"

GrB_Info GrB_init           // start up GraphBLAS
(
    const GrB_Mode mode     // blocking or non-blocking mode
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    WHERE ("GrB_init (mode)") ;

    if (! (mode == GrB_BLOCKING || mode == GrB_NONBLOCKING))
    {
        return (ERROR (GrB_INVALID_VALUE, (LOG,
            "Unknown mode: %d; must be %d (GrB_NONBLOCKING) or %d"
            " (GrB_BLOCKING)", mode, GrB_NONBLOCKING, GrB_BLOCKING))) ;
    }

    //--------------------------------------------------------------------------
    // initialize GraphBLAS
    //--------------------------------------------------------------------------

    // error status
    GB_thread_local.info = GrB_SUCCESS ;
    GB_thread_local.row = 0 ;
    GB_thread_local.col = 0 ;
    GB_thread_local.is_matrix = 0 ;
    GB_thread_local.file = __FILE__ ;
    GB_thread_local.line = __LINE__ ;
    GB_thread_local.details [0] = '\0' ;
    GB_thread_local.report  [0] = '\0' ;

    // queue of matrices for nonblocking mode
    GB_thread_local.queue_head = NULL ;

    // mode: blocking or nonblocking
    GB_thread_local.mode = mode ;       // default is non-blocking

    // malloc tracking
    GB_thread_local.nmalloc = 0 ;
    GB_thread_local.malloc_debug = false ;
    GB_thread_local.malloc_debug_count = 0 ;

    // workspace
    GB_thread_local.Mark = NULL ;       // initialized space
    GB_thread_local.Mark_flag = 1 ;     // current watermark in Mark [...]
    GB_thread_local.Mark_size = 0 ;     // current size of Mark array
    GB_thread_local.Work = NULL ;       // uninitialized space
    GB_thread_local.Work_size = 0 ;     // current size of Work array
    GB_thread_local.Flag = NULL ;       // initialized space
    GB_thread_local.Flag_size = 0 ;     // current size of Flag array

    return (REPORT_SUCCESS) ;
}

