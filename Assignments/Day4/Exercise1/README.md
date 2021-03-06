## Exercise 1

The results can be printed to stdout by running the bash script `script.sh`. The number of processes can be easily modified with the environment variable `PROCS`. The executable produced is `ring.x`.

In this exercise we were asked to implement an all-reduce communication pattern among a given number of processes. The high-level algorithm works as follows: 1) Each worker initializes two local buffers. One (`sum`) will be used to accumulate the result, while the other (`X`) will contain the numbers to be summed and is equal to the rank itself at the very beginning; 2) Each process sends its `X` to the process on the right, and receives an equivalent buffer from the process on the left, which will substitute `X` itself; 3) Each process locally augments `sum` with `X`; 4) Repeat steps 2-3 for every other process. At the end, the desired output should be that every worker has the same content for the buffer `sum`, namely the sum of all the rank identifiers across the current MPI world. 

Right and left processes are swiftly computed with the `PROC_RIGHT` and `PROC_LEFT` macros defined on top of the script. Moreover, with the usage of the `VECTOR` macro we can implement this program for sending in the ring an array of data, otherwise we are talking about scalar messages.

Notice that this all-reduce is a non-blocking implementation. The `MPI_Send` has been substituted by an `MPI_Isend`, which returns without necessarily accomplishing the communication. As a result, execution is optimized since we can overlap communication (the send) with the computation (summing the buffers), one of the major goals of parallel programming.
