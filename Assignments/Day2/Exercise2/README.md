## Exercise 2

The code written for this exercise can be easily compiled with `make`. Moreover, the tests can be reproduced by submitting the script `script.sh` to a queue, possibly with 10 cores.

I parallelized the provided loop with a work-sharing construct of the form `#pragma omp parallel for schedule(...)`, where `...` has to be filled with the appropriate schedule. Then, I created a bash script which exports the environment variable OMP_NUM_THREADS to 10, compiles (with the flag `-fopenmp`) and runs the program, and launched it on the Ulysses cluster on the appropriate queue. Examples outputs are shown below (some parts have been omitted due to space limitations, but we can still infer the overall pattern).

```
schedule(static):
0:*************************                                                                                                                           
1:                         *************************                                                                                                  
2:                                                  *************************                                                                         
3:                                                                           *************************                                                
4:                                                                                                    *************************                       
5:                                                                                                                             **
6:                                                                                                                                                    
7:                                                                                                                                                    
8:                                                                                                                                                    
9:                     

schedule(static, 1):
0:*         *         *         *         *         *         *         *         *         *         *         *         *        
1: *         *         *         *         *         *         *         *         *         *         *         *         *         
2:  *         *         *         *         *         *         *         *         *         *         *         *         *         
3:   *         *         *         *         *         *         *         *         *         *         *         *         *        
4:    *         *         *         *         *         *         *         *         *         *         *         *         *         
5:     *         *         *         *         *         *         *         *         *         *         *         *         *         
6:	*         *         *         *         *         *         *         *         *         *         *         *         *         
7:	 *         *         *         *         *         *         *         *         *         *         *         *         *         
8:        *         *         *         *         *         *         *         *         *         *         *         *         
9:         *         *         *         *         *         *         *         *         *         *         *         *  

schedule(static, 10):
0:**********                                                                                          **********                                      
1:          **********                                                                                          **********                            
2:                    **********                                                                                          ****
3:                              **********                                                                                          
4:                                        **********                                                                                         
5:                                                  **********                                                                                        
6:                                                            **********                                                                              
7:                                                                      **********                                                                    
8:                                                                                **********                                                          
9:                                                                                          **********                                           

schedule(dynamic):
0:*          *  * *  *           *     *                 *  *  *       *          *         *   *    *     *                             
1: *                     *   * *  * *    *   *       *    *	  *     * *           *   *            *          *         * *      
2:    *                                             *	   *                  * *                               *	*          *    *             
3:   *      * *  *  * *   *   *       *       *   *              *    *                 *     *  *	 *          *	   *   *          
4:	*                          *	   *   * *    *              *                 *            * *              *                     
5:        *    *   *   *    *        *    *            *        *  **       *        *                  *   * *          *   *   
6:         *                                       *         *           *	 *  *    *     *          *	 *    *         * *                   
7:	 *                              *                     *            * *             *                       *                          
8:     *                                                                       *             *     *                   *			*     
9:  *                   *  *    *           *   *	*                          *              *          * *          *           *  

schedule(dynamic, 1):
0:     *          *	 *              *   *       *           *              *         *             *      *        *         * 
1:             *                                *      ****	  *	 *   *               *    *                  *                 
2:         *	   *           *         *     *                     *           **   *       *      *     *     *           *   
3:	 *                *	 *    *           *  *     *	   *          *                * * *    *    *      *   *      *      * 
4:           *   *    **    *       *            *             *                     *                            *           
5:  *   *                  * *    *    *  *           *     *         *             *	   *    *           *             *	  *	 
6:          *   *	*          *          *                                 *         *         *          *            *       
7:            *      *               *       *     *         *         *  *            *                 *	*        *    *  
8:    *   *         *         *            *                               *            *             *   *           *    *  
9:** *                          *                             *  *  *   *   *      *        *                      *                  

schedule(dynamic, 10):
0:                    **********                                                                      **********          *******
1:**********                                                                                                                                          
2:                                                                      **********                              **********         
3:                                                  **********                                                                                        
4:                                                                                          **********                                                
5:                                                            **********                                                                              
6:                              **********                                                                                                            
7:                                                                                **********                                                          
8:          **********                                                                                                                                
9:                                        **********                                                                             
```
The comments are rather straight-forward.
  In the case of a static schedule, we are distributing the threads one chunk after the other, as can be seen from the replicating pattern. Notice that, if no chunk number is given (the second parameter to the schedule directive), OpenMP assigns to each thread a block of size number_of_iterations / number_of_threads, in our case 250 / 10 = 25.
  In the case of a dynamic schedule, the different tasks are pushed to a queue from which the threads grab once they have finished their current task, and this process goes on until all the tasks have been fulfilled. As a result, we can see a more irregular pattern in the allocation of jobs, since different iterations may have different durations (even for a simple loop like ours). 
