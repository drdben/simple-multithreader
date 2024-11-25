# simple-multithreader
SIMPLE MULIHREADER README Group 27
## Working
Any c++ programme that includes simple-multithreader.h can use multithreading by calling the parallel_for function with proper arguments (high,low,lambda function, number of threads) (or 2 highs and lows, depending on the implementation needed). 
## Features
- Supports both single and nested parallel loops
- Automatic work distribution across threads: By distributing work into chunks
- Built-in execution time measurement: using chrono
- Lambda function support for flexible parallel operations

## Performance Optimization
### Implementation includes several optimizations:
- For small workloads (arrays < 1024 elements or matrices < 16x16), it automatically switches to sequential execution to avoid thread overhead
- Work is distributed evenly across threads using chunk-based distribution
- Handles cases where the number of threads exceeds the workload size
- Execution time is measured and printed for *each parallel_for call*

## Error Handling
### The file includes error checking for thread creation and joining operations. In case of errors, appropriate error messages are displayed and the program exits with specific error codes:
- Exit code 1: Thread creation failure
- Exit code 2: Thread joining failure
- Exit code 3: Nested loop thread creation failure
- Exit code 4: Nested loop thread joining failure

## Dependencies
- C++11 or later
- pthread library
- iostream
- chrono (for timing measurements)
## Running Using existing testcases
download repo locally on linux/unix system, run 

```
make clean
make vector
./vector (arraysize) (numthreads)
> get output

make matrix
./matrix (size of square matrix) (numthreads)
> get output

make clean
```
## Contributions
1. Divyanshi, 2023209 (Simple Multithreader + ReadMe)
2. Arnav Gupta, 2023125 (Error Handling + Time Measurement system)

## Link to Private Repo:
https://github.com/drdben/simple-multithreader
