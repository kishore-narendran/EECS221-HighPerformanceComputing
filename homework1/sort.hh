/**
 *  \file sort.hh
 *
 *  \brief Interface to sorting arrays of keys ('keytype' values).
 */

#if !defined (INC_SORT_HH)
#define INC_SORT_HH /*!< sort.hh already included */

/** 'keytype' is the primitive type for sorting keys */
typedef unsigned long keytype;

/**
 *  Sorts an input array containing N keys, A[0:N-1]. The sorted
 *  output overwrites the input array.
 */
void sequentialSort (int N, keytype* A);

/**
 *  Sorts an input array containing N keys, A[0:N-1]. The sorted
 *  output overwrites the input array. This is the routine YOU will
 *  implement; see 'parallel-qsort.cc'.
 */
void parallelSort (int N, keytype* A);

/** Returns a new uninitialized array of length N */
keytype* newKeys (int N);

/** Returns a new copy of A[0:N-1] */
keytype* newCopy (int N, const keytype* A);

/**
 *  Checks whether A[0:N-1] is in fact sorted, and if not, aborts the
 *  program.
 */
void assertIsSorted (int N, const keytype* A);

/**
 *  Checks whether A[0:N-1] == B[0:N-1]. If not, aborts the program.
 */
void assertIsEqual (int N, const keytype* A, const keytype* B);

#endif

/* eof */
