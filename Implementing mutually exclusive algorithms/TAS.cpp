#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <atomic>

std::atomic_flag lock = ATOMIC_FLAG_INIT;

int shared_counter = 0;
int min(int first, int second)
{
    if (first > second)
        return second;
    else
        return first;
}

void Multiplication(const std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B, int Size, int rowInc)
{

    while (true)
    {
        while (lock.test_and_set(std::memory_order_acquire))
            ;
        int var = shared_counter;
        shared_counter = shared_counter + rowInc;
        lock.clear(std::memory_order_release);

        int startRow = var;
        int endRow = min(var + rowInc, Size);
        //   printf("%d\n",var);
        for (int i = startRow; i < endRow; i++)
        {
            // printf("%d\n",i);
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
    std::ofstream outFile("TAS.txt");

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
    std::vector<std::vector<int>> B(N, std::vector<int>(N, 0));
    auto start_time = std::chrono::high_resolution_clock::now();
    std::thread threads1[K];
    for (int i = 0; i < K; ++i)
    {
        threads1[i] = std::thread(Multiplication, std::cref(A), std::ref(B), N, rowInc);
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
