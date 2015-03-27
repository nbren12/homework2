/* Parallel sample sort
 */
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>


static int compare(const void *a, const void *b)
{
  int *da = (int *)a;
  int *db = (int *)b;

  if (*da > *db)
    return 1;
  else if (*da < *db)
    return -1;
  else
    return 0;
}

int main( int argc, char *argv[])
{
  int rank, world_size;
  int i, j, N, S;
  int *vec, *samples, *sample_idxs;
  int *all_samples;
  int *bins, nsamp;
  int randint;


  int root = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  /* Number of random numbers per processor (this should be increased
   * for actual tests or could be passed in through the command line */
  N = 100;

  /* Number of entries to send to root process. */
  S = 10;
  nsamp = S * world_size;


  /* Allocations for all processes */
  vec = calloc(N, sizeof(int));
  samples = calloc(N, sizeof(int));
  sample_idxs = calloc(N, sizeof(int));
  bins = calloc(world_size, sizeof(int));

  /* Only need to allocate on root process */
  if (rank == root) all_samples = calloc(nsamp, sizeof(int));

  /* seed random number generator differently on every core */
  srand((unsigned int) (rank + 393919));

  /* fill vector with random integers */
  for (i = 0; i < N; ++i) {
    vec[i] = rand();
  }
  printf("rank: %d, first entry: %d\n", rank, vec[0]);


  /* randomly sample s entries from vector or select local splitters,
   * i.e., every N/P-th entry of the sorted vector */

  j = 0; 
  do {
    randint = rand() % N;

    /* This logic ensures that the random draws are unique */
    char isuniq = 1;
    for (i = 0; i < j; i++) {
      if ( sample_idxs[i] == randint) {
	isuniq = 0;
	break;
      }
    }

    if (isuniq) {
      sample_idxs[j] = randint;
      samples[j] = vec[randint];
      ++j;
    }
  } while(j < S);
  

  /* for (i = 0; i < nbins; ++i) printf("%d\n", bins[i]); */
  /* every processor communicates the selected entries
   * to the root processor; use for instance an MPI_Gather */
  MPI_Gather(samples, S, MPI_INT, all_samples, S, MPI_INT, root, MPI_COMM_WORLD);

  /* root processor does a sort, determinates splitters that
   * split the data into P buckets of approximately the same size */
  if ( rank == root ){
    qsort(all_samples, nsamp, sizeof(int), compare);
    for (i=0; i < world_size ; ++i) bins[i] = all_samples[(i+1) * S];
    free(all_samples);
  }

  /* root process broadcasts splitters */
  MPI_Bcast(bins, world_size, MPI_INT, root, MPI_COMM_WORLD);


  /* every processor uses the obtained splitters to decide
   * which integers need to be sent to which other processor (local bins) */

  /* send and receive: either you use MPI_AlltoallV, or
   * (and that might be easier), use an MPI_Alltoall to share
   * with every processor how many integers it should expect,
   * and then use MPI_Send and MPI_Recv to exchange the data */

  /* sort locally */
  qsort(vec, N, sizeof(int), compare);

  /* every processor writes its result to a file */
  free(vec);
  free(samples);
  free(sample_idxs);
  free(bins);

  MPI_Finalize();
  return 0;
}

