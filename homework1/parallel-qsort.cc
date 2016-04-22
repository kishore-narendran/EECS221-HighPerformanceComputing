/**
 *  \file parallel-qsort.cc
 *
 *  \brief Implement your parallel quicksort using OpenMP in this
 *  file, given an initial sequential implementation.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.hh"

/**
 *  Pivots the keys of A[0:N-1] around a given pivot value. The number
 *  of keys less than the pivot is returned in *p_n_lt; the number
 *  equal in *p_n_eq; and the number greater in *p_n_gt. The
 *  rearranged keys are stored back in A as follows:
 *
 * - The first *p_n_lt elements of A are all the keys less than the
 *   pivot. That is, they appear in A[0:(*p_n_lt)-1].
 *
 * - The next *p_n_eq elements of A are all keys equal to the
 *   pivot. That is, they appear in A[(*p_n_lt):(*p_n_lt)+(*p_n_eq)-1].
 *
 * - The last *p_n_gt elements of A are all keys greater than the
 *   pivot. That is, they appear in
 *   A[(*p_n_lt)+(*p_n_eq):(*p_n_lt)+(*p_n_eq)+(*p_n_gt)-1].
 */
void partition (keytype pivot, int N, keytype* A,
		int* p_n_lt, int* p_n_eq, int* p_n_gt)
{
  /* Count how many elements of A are less than (lt), equal to (eq),
     or greater than (gt) the pivot value. */
  int n_lt = 0, n_eq = 0, n_gt = 0;
	#pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    if (A[i] < pivot) ++n_lt;
    else if (A[i] == pivot) ++n_eq;
    else ++n_gt;
  }

  keytype* A_orig = newCopy (N, A);

  /* Next, rearrange A so that:
   *   A_lt == A[0:n_lt-1] == subset of A < pivot
   *   A_eq == A[n_lt:(n_lt+n_eq-1)] == subset of A == pivot
   *   A_gt == A[(n_lt+n_eq):(N-1)] == subset of A > pivot
   */
  int i_lt = 0; /* next open slot in A_lt */
  int i_eq = n_lt; /* next open slot in A_eq */
  int i_gt = n_lt + n_eq; /* next open slot in A_gt */
	#pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    keytype ai = A_orig[i];
    if (ai < pivot)
      A[i_lt++] = ai;
    else if (ai > pivot)
      A[i_gt++] = ai;
    else
      A[i_eq++] = ai;
  }
  assert (i_lt == n_lt);
  assert (i_eq == (n_lt+n_eq));
  assert (i_gt == N);

  free (A_orig);

  if (p_n_lt) *p_n_lt = n_lt;
  if (p_n_eq) *p_n_eq = n_eq;
  if (p_n_gt) *p_n_gt = n_gt;
}

void
quickSort (int N, keytype* A)
{
  const int G = 1250000; /* base case size, a tuning parameter */
  if (N < G)
    sequentialSort (N, A);
  else {
    // Choose pivot at random
    keytype pivot = A[rand () % N];

    // Partition around the pivot. Upon completion, n_less, n_equal,
    // and n_greater should each be the number of keys less than,
    // equal to, or greater than the pivot, respectively. Moreover, the array
    int n_less = -1, n_equal = -1, n_greater = -1;
    partition (pivot, N, A, &n_less, &n_equal, &n_greater);
    assert (n_less >= 0 && n_equal >= 0 && n_greater >= 0);
		#pragma omp task
    quickSort (n_less, A);
    quickSort (n_greater, A + n_less + n_equal);
		#pragma omp taskwait
  }
}

void
parallelSort (int N, keytype* A)
{
	#pragma omp parallel
	#pragma omp single
  quickSort (N, A);
}

/* eof */
