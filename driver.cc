/**
 *  \file driver.cc
 *  \brief HW 1: OpenMP
 *
 *  This program
 *
 *  - creates an input array of keys to sort, where the caller gives
 *    the array size as a command-line input;
 *
 *  - sorts it sequentially, noting the execution time;
 *
 *  - sorts it using YOUR parallel implementation, also noting the
 *    execution time;
 *
 *  - checks that the two sorts produce the same result;
 *
 *  - outputs the execution times and effective sorting rate (i.e.,
 *    keys per second).
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "timer.c"

#include "sort.hh"

/* ============================================================
 */

int
main (int argc, char* argv[])
{
  int N = -1;

  if (argc == 2) {
    N = atoi (argv[1]);
    assert (N > 0);
  } else {
    fprintf (stderr, "usage: %s <n>\n", argv[0]);
    fprintf (stderr, "where <n> is the length of the list to sort.\n");
    return -1;
  }

  stopwatch_init ();
  struct stopwatch_t* timer = stopwatch_create (); assert (timer);

  /* Create an input array of length N, initialized to random values */
  keytype* A_in = newKeys (N);
  for (int i = 0; i < N; ++i)
    A_in[i] = lrand48 ();

  printf ("\nN == %d\n\n", N);

  /* Sort sequentially */
  keytype* A_seq = newCopy (N, A_in);
  stopwatch_start (timer);
  sequentialSort (N, A_seq);
  long double t_seq = stopwatch_stop (timer);
  printf ("Sequential: %Lg seconds ==> %Lg million keys per second\n",
	  t_seq, 1e-6 * N / t_seq);
  assertIsSorted (N, A_seq);

  /* Sort in parallel, calling YOUR routine. */
  keytype* A_par = newCopy (N, A_in);
  stopwatch_start (timer);
  parallelSort (N, A_par);
  long double t_qs = stopwatch_stop (timer);
  printf ("Parallel sort: %Lg seconds ==> %Lg million keys per second\n",
	  t_qs, 1e-6 * N / t_qs);
  assertIsSorted (N, A_par);
  assertIsEqual (N, A_par, A_seq);

  /* Cleanup */
  printf ("\n");
  free (A_par);
  free (A_seq);
  free (A_in);
  stopwatch_destroy (timer);
  return 0;
}

/* eof */
