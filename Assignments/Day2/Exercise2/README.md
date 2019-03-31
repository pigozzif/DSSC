## Exercise 2

I parallelized the provided loop with a work-sharing construct of the form `#pragma omp parallel for schedule(...)`, where `...` has to be filled with the appropriate schedule. Then, I created a bash script which exports the environment variable OMP_NUM_THREADS to 10, compiles (with the flag `-fopenmp`) and runs the program, and launched it on the Ulysses cluster on the appropriate queue. Examples outputs are shown below (some parts have been omitted due to space limitations, but we can still infer the overall pattern).

```sequential:
0:********************************************************************************************************************************
1:                                                                                                                                                    
2:                                                                                                                                                    
3:                                                                                                                                                    
4:                                                                                                                                                    
5:                                                                                                                                                    
6:                                                                                                                                                    
7:                                                                                                                                                   
8:                                                                                                                                                   
9:

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
0: *        * * * * * * * * * * * * * * * * * * * * *  ** ** ** ** * * * * * * * * * ** * * ** * * * * * * * * * * * * * * *** * 
1:*                                                                                                                                                   
2:	 *                                                                                                                                            
3:     *                                                                                                                                              
4:    *                                                                                                                                               
5:         *                                                                                                                                          
6:        *                                                                                                                                           
7:	*                                                                                                                                
8:  *                                                                                                                                                 
9:   *       * * * * * * * * * * * * * * * * * * * * **  *  *  *  * * * * * * * * * *  * * *  * * * * * * * * * * * * * * *   * 

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



