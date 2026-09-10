#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <atomic>
#include <stdbool.h>

int shared_counter = 0;
int min(int first, int second)
{
    if (first > second)
        return second;
    else
        return first;
}

void Multiplication(const std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, int Size, int rowInc, volatile bool lock)
{
    while (true)
    {

        while (!__sync_bool_compare_and_swap(&lock, false, true))
            ;
        int var = shared_counter;
        shared_counter = shared_counter + rowInc;
        __sync_lock_release(&lock);
        int startRow = var;
        int endRow = min(var + rowInc, Size);
        for (int i = startRow; i < endRow; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                B[i][j] = 0;
                for (int k = 0; k < Size; k++)
                {
                    B[i][j] += A[i][k] * A[k][j];
                }
            }
        }
        if (var >= Size)
        {
            return;
        }
    }
}

int main()
{
    std::ofstream outFile("CAS.txt");

    if (!outFile.is_open())
    {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outFile.rdbuf());

    std::ifstream inputFile("inp.txt");
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    int N, K, rowInc;
    inputFile >> N >> K >> rowInc;

    std::vector<std::vector<int>> A(N, std::vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            inputFile >> A[i][j];
        }
    }
    inputFile.close();
    volatile bool lock = false;
    std::vector<std::vector<int>> B(N, std::vector<int>(N, 0));
    auto start_time = std::chrono::high_resolution_clock::now();
    std::thread threads1[K];
    for (int i = 0; i < K; ++i)
    {
        threads1[i] = std::thread(Multiplication, std::cref(A), std::ref(B), N, rowInc, lock);
    }

    for (int i = 0; i < K; ++i)
    {
        threads1[i].join();
    }
    std::cout << "Result Matrix B:" << std::endl;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            std::cout << B[i][j] << " ";
        }
        std::cout << std::endl;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time taken by Multiplication: " << duration.count() << " milliseconds" << std::endl;
    std::cout.rdbuf(coutbuf);
    outFile.close();
    return 0;
}
