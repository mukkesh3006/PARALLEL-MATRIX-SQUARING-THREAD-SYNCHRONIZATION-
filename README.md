# Parallel Matrix Squaring & Thread Synchronization

## Overview

This project focuses on a shared counter `C` (`shared_counter` in the implementation) that is incremented by multiple threads. To avoid synchronization issues when threads compete to increment the counter by a value of `rowInc`, different mutual exclusion and atomic synchronization algorithms are used:

- **TAS (Test-and-Set)**
- **CAS (Compare-and-Swap)**
- **Bounded CAS**
- **Atomic Increment**

The execution time is measured for each algorithm.

## Input Format

The program reads input from a file named `inp.txt`.

The format of `inp.txt` is:

```text
N K rowInc
a11 a12 ... a1N
a21 a22 ... a2N
...
aN1 aN2 ... aNN
```

## Input Parameters

- **N**: Size of the square matrix (`N × N`)
- **K**: Number of threads
- **rowInc**: Value by which the shared counter is increased each time

## Output

The program produces:

- **Time of execution** required to complete square multiplication for the respective algorithm
- **Square matrix** after multiplication

## Note

> **For Bounded CAS, `% Size` is used, so the `wait` array size should be greater than or equal to `Size`.**

## Experiments

Four experiments are performed:

1. **Time vs. Size of Input (`N`)**
2. **Time vs. `rowInc`**
3. **Time vs. Number of Threads (`K`)**
4. **Time vs. Algorithms**

The corresponding **graphs and observations** are included for each experiment.

## Algorithms

### 1. Test-and-Set (TAS)

- Uses an atomic flag to manage access to the critical section.
- Only one thread can set the flag (lock) to `true` at a time.
- Other threads spin-wait until the flag is released.
- This approach is simple but can lead to busy-waiting.

### 2. Compare-and-Swap (CAS)

- Uses an atomic integer variable to manage access to the critical section.
- Threads try to atomically change the lock variable from `0` to `1`.
- If successful, the thread enters the critical section.
- Otherwise, the thread retries.

### 3. Bounded CAS

- An extension of CAS with an additional waiting mechanism for competing threads.
- Allows a thread that finishes its work to select the next waiting thread.
- Helps provide fairness and avoid thread starvation.

### 4. Atomic Increment

- Uses an atomic integer variable to divide work among threads.
- Each thread atomically increments the shared counter to determine its assigned rows.
- This approach avoids contention for a single lock while dynamically distributing work.

## Source Files

The project contains four source code files, one for each algorithm:

| Algorithm | Source File |
|-----------|-------------|
| **TAS** | `TAS.cpp` |
| **CAS** | `CAS.cpp` |
| **Bounded CAS** | `CAS_BOUNDED.cpp` |
| **Atomic Increment** | `ATOMIC.cpp` |

## Output Files

Each algorithm has a corresponding output file:

- `TAS.txt`
- `CAS.txt`
- `BCAS.txt`
- `ATINC.txt`
## Performance Analysis

The execution time of the four synchronization algorithms is compared by varying:

- **Input matrix size (`N`)**
- **Row increment (`rowInc`)**
- **Number of threads (`K`)**
- **Synchronization algorithm**

The performance results are analyzed using the corresponding graphs and observations.
