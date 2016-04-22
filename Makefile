CC = g++
CFLAGS =
COPTFLAGS = -O3 -g
LDFLAGS =

default:
	@echo "=================================================="
	@echo "To build your OpenMP code, use:"
	@echo "  make qsort-omp        # For Quicksort"
	@echo "  make mergesort-omp    # For Mergesort"
	@echo ""
	@echo "To clean this subdirectory (remove object files"
	@echo "and other junk), use:"
	@echo "  make clean"
	@echo "=================================================="

# Mergesort driver using OpenMP
mergesort-omp: driver.o sort.o parallel-mergesort.o
	$(CC) $(COPTFLAGS) -fopenmp -o $@ $^

mergesort: driver.o sort.o parallel-mergesort.o
		$(CC) $(COPTFLAGS) -o $@ $^

# Quicksort driver using OpenMP
qsort-omp: driver.o sort.o parallel-qsort.o
	$(CC) $(COPTFLAGS) -fopenmp -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -fopenmp -o $@ -c $<

clean:
	rm -f core *.o *~ qsort-omp mergesort-omp

# eof
