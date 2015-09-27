// Author: Robert H. Lee
// Created: Fall 2015
// The motivation for this example is utilization of MPI_THREAD_FUNNELED.
// This allows a normally blocking MPI command to run in the background 
// while other threads can process work in parallel. 
// MPI_THREAD_FUNNELED means that all MPI calls are sent to the main thread, 
// (and serialized), even if MPI calls are invoked by other threads. 
// Therefore we want other threads to be doing the main work, and then any 
// other thread can make the MPI calls (which will all be funneled to the main 
// thread).

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

// Here we execute a for loop, where we want data associated with each loop
// to be transmitted by an MPI process, but we are able to process the entire 
// for loop all at once. So, instead of waiting on the blocking MPI call at the 
// end of each loop (or even using an MPI call to handle all data all at once 
// at the end of the whole for loop), we can transmit the data as it is done in 
// each loop, and continue working on the rest of the data. The idea is to 
// achieve high overlap of communication and computation. 

int main(int argc, char** argv)
{
  // Initiate the parallel region so that multiple threads can be used.
  #pragma omp parallel
  {
    // We do not want the computational work on the main thread; we only want 
    // the main thread to handle the MPI communication. The master thread has 
    // omp_get_thread_num() == 0.
    if(omp_get_thread_num() != 0)
    {
      // Now we only want one non-master thread to do work, hence 'single'. If 
      // we did not use single, redundant, same computations will be done.
      // The 'nowait' allows us to continue work without waiting on other 
      // things inside the loop (in this case, 'task').
      #pragma omp single nowait
      {
        for(int i = 0; i < 5; ++i)
        {
          // Our 'work'.
          // sleep(1);
          fprintf(stdout, "Hello. %d (thread: %d)\n", i, omp_get_thread_num());
          
          // This would be where a blocking call would be placed. Placed inside 
          // the task, the call would be placed upon the master thread.
          // 'task' gives up the code inside to the general available thread 
          // pool.
          #pragma omp task
          {
            // A blocking call, such as MPI_Allgatherv().
            sleep(1);
            fprintf(stdout, "Master. %d (thread: %d)\n", i, 
              omp_get_thread_num());
          }
        }
      }
    }
  }

  return EXIT_SUCCESS;
}