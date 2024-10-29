Why bother with HPC?

1. Do things faster.
	- HPC is everywhere. TO do things efficiently.   
2. Do thisngs bigger.
        - Increase the size of the problem increses the time to solution and 
	memory requirement. 
We need to write code that executes really fast and scales in size. Whar happens if the problem gets really large from MB to PetaB.

FLOPs - floating point operations/sec

Task Parallelism

Data Parallelism

Paradigms in Parallelism 
1. Shared memory (OpenMP, Pthreads)
   - processing elements see the same centralized memory area. 
   - pragma based (#pragma omp (directive) (clause)).
   - a team of threads of executions are spawned by a process at the beginning of a parallel region 
   - they then join or converge tohether at the end of the parallel region. 

Decomposing the domain (Round robin(cyclic) and block decomposition) round robin is not good for cache coherence because multiple cache lines
are loaded unto the cache line.


variables declared before a parallel region are by default SHARED! 

each variable declared by a thread 

OMP_GET_NUME_THREAD()

OMP_SET_NUM_THREAD()


Race condition is being explained:
A = A+1
is actually 
Load A from memeory to register 
Add  1 to A whichi is in register 
Store A + 1 to memory

Writing to a memory area is always dangerous 

first_private() - initialize the private instance to the value of the shared instance.  

static variables(relevant in c++) are shared by default coz its in the heap. 

OpenMP are most often used to parallelize a for loop. 


#	pragma omp 
{
	...some codes here 

# 	pragma omp for 
	for (i = 0; i < n; i++)

}

beware of critical sections


#	pragma omp atomic 
 	-This is the directive when you need to perform simple updates on shared variables and want
	avoid the overhead of a full critical section. 

	-supports the following `+=	, ` -=  ` , ` *= `, `/=`, `++` and `--`.
 
 cache coherence - a condition when the a processor updated a cached value, the cached value 
		 	of other processors are also updated.

	 Ways to ensure Cache coherence
 
	1.Snooping cache coherence.
	2.Directory-based cache coherence- for large systems the problem associated with snooping cache coherence is solved using a data structure known as directory.


 
