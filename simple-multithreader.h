#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include<pthread.h>
#include <chrono>

#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}

typedef struct{
    int low;
    int high;
    std::function<void(int)> l1;
} threadargs;

typedef struct{
    int low1;
    int high1;
    int low2;
    int high2;
    std::function<void(int,int)> l1;
} threadargs_2;


void* routine(void* ptr){
    threadargs *t =(threadargs *)ptr;
    int l =t->low;
    int h = t->high;
    for(int i=l;i<h;i++){
        // pthread_mutex_lock(&mutex);
        t->l1(i);
        // pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* routine2(void* ptr){
    threadargs_2 *t = (threadargs_2 *)ptr;
    for(int j = t->low1; j<t->high1; j++){
        for(int m = t->low2; m<t->high2; m++){
            t->l1(j,m);
        }
    }
    return NULL;
}

void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
    
      std::chrono::time_point<std::chrono::steady_clock> start;
  start = std::chrono::steady_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> end;

    // clock_t begin = clock();
  
    if(high<1024){
        int ans =0;
        for(int i=low;i<high;i++){
            lambda(i);
        }
    }
    else{
        // pthread_mutex_init(&mutex, NULL);
        pthread_t th[numThreads];
        threadargs args[numThreads];   

        int flag =0;

        int chunk = (high-low) / numThreads ;
        // printf("chunk: %d\n",chunk);
        // fflush(stdout);
        int extra = (high-low) % (numThreads);
        // printf("extra: %d\n",extra);
        // fflush(stdout);

        if(numThreads> (high-low)){
            chunk= 1;
            extra= 0;
            flag= 1;
        }

        for(int i=0; i<numThreads; i++){
            args[i].low = i*chunk; 
            args[i].high=args[i].low + chunk ;
            if(i== numThreads-1){
                args[i].high += extra;
            }
            args[i].l1 = lambda;
            if(flag==1 && i>=(high-low)){
                args[i].low = 0;
                args[i].high = 0;
                args[i].l1 = lambda;
            }
            if(pthread_create(th+i, NULL, routine, (void*) &args[i])!=0){
                perror("Could not create thread");
                exit(1);
            };
        }
        for(int i=0;i<numThreads;i++){
            if(pthread_join(th[i], NULL) !=0){
                perror("Could not join thread");
                exit(2);
            };
        }

        end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "parallel_for execution time: ";
  std::cout << elapsed_seconds.count() << "seconds\n";

        // clock_t end = clock();
        // double time_taken = (double)(end -begin) /CLOCKS_PER_SEC ;
        // printf("Time taken to execute parallel_for: %f seconds\n", time_taken);
        // fflush(stdout);

        // pthread_mutex_destroy(&mutex);
    }
}

void parallel_for(int low1, int high1,  int low2, int high2, std::function<void(int, int)>  &&lambda, int numThreads){
    
   std::chrono::time_point<std::chrono::steady_clock> start;
  start = std::chrono::steady_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> end;


    // clock_t begin = clock();
    
    if(high1-low1 <16 && high2-low2<16){
        for(int i=low1;i<high1;i++){
            for(int j=low2;j<high2;j++){
                lambda(i,j);
            }
        }
    }
    else{
        pthread_t th2[numThreads];
        threadargs_2 args2[numThreads];

        int flag =0;
        int chunk2 = (high1-low1)/numThreads;

        int extra2 = (high1- low1)% numThreads;
        if(numThreads>(high1 - low1)){
            chunk2 =1;
            extra2 =0;
            flag =1;
        }
        // printf("chunk2: %d\n", chunk2);
        // printf("extra2: %d\n", extra2);
        for(int i=0;i<numThreads;i++){
            args2[i].low1 =  i*chunk2 ;
            args2[i].high1 = args2[i].low1 + chunk2;
            args2[i].low2 = low2;
            args2[i].high2 = high2;
            args2[i].l1 = lambda;
            if(i == numThreads -1){
                args2[i].high1 += extra2;
            }
            if(flag==1 && i>=(high1-low1)){
                args2[i].low1=0;
                args2[i].high1 =0;
                args2[i].low2=0;
                args2[i].high2 = 0;
            }
            if(pthread_create(&th2[i],NULL, routine2, (void*)&args2[i])!=0){
                perror("could not create thread");
                exit(3);
            };
        }
        for(int i=0;i<numThreads;i++){
            if(pthread_join(th2[i], NULL)!=0){
                perror("could not join thread");
                exit(4);
            };
        }
    }

end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "parallel_for execution time: ";
  std::cout << elapsed_seconds.count() << "s\n";

    // clock_t end = clock();
    //     double time_taken = (double)(end -begin) /CLOCKS_PER_SEC ;
    //     printf("Time taken to execute parallel_for: %f seconds\n", time_taken);
    //     fflush(stdout);

}


#endif

int main(int argc, char **argv) {
  /* 
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be 
   * explicity captured if they are used inside lambda.
   */
  int x=5,y=1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/[/*by value*/ x, /*by reference*/ &y](void) {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
 
  auto /*name*/ lambda2 = [/*nothing captured*/]() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

#define main user_main


