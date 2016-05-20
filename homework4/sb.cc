/**
 *  \file sb.cc
 *  \brief Saavedra-Barrera benchmark
 */

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "timer.c"

#define NIL -1

using namespace std;

  int
setup (int n_max, int* Index, int n, int s)
{
  assert (n <= n_max);
  int num_elems = 0;
  int cur = 0;
  for (int next = s; next < n; next += s) {
    Index[cur] = next;
    cur = next;
    ++num_elems;
  }
  assert (cur < n);
  Index[cur] = 0;
  return num_elems + 1;
}

  int
bench (int n, int* Index)
{
  int cur = 0;
  int k;
  for (k = 0; (k+4) <= n; k += 4) {
    cur = Index[cur];
    cur = Index[cur];
    cur = Index[cur];
    cur = Index[cur];
  }
  if (k < n)
    do
      cur = Index[cur];
    while (++k < n);
  return cur;
}

  int
main (int argc, char* argv[])
{
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " <n>" << endl;
    return -1;
  }

  stopwatch_init ();
  struct stopwatch_t* timer = stopwatch_create ();
  assert (timer);

  int n_max = atoi (argv[1]); assert (n_max >= 2);
  int* Index = new int[n_max]; assert (Index);

  for (int n = min (1024, n_max); n <= n_max; n *= 2) {
    for (int stride = 1; stride < n; stride *= 2) {
      cerr << "... n=" << n << ", s=" << stride << " ..." << endl;
      int num_reads = setup (n_max, Index, n, stride);

      // Determine how many trials are needed to get at least 1 sec
      cerr << "  Calibrating ...";
      long double t = 0;
      long int num_trials = 1;
      long int dummy = 0; // prevent compiler from optimizing this loop away
      while (t < 0.1) {
        stopwatch_start (timer);
        dummy += bench (num_reads * num_trials, Index);
        t = stopwatch_stop (timer);
        num_trials *= 2;
      }
      cerr << " done. [dummy=" << dummy << "]" << endl;

      cerr << "  Timing [num_trials=" << num_trials << "] ...";
      stopwatch_start (timer);
      dummy = bench (num_reads * num_trials, Index);
      t = stopwatch_stop (timer);
      cerr << " done. [dummy=" << dummy << "]" << endl;

      cout << n << ' ' << stride << ' ' << t / num_reads / num_trials << endl;
    } // stride
    cout << endl << endl;
  } // n

  delete[] Index;
  stopwatch_destroy (timer);
  return 0;
}

// eof
