#include "../src/Oracle.hpp"
#include "AdjacencyList.hpp"
#include "UFDS.hpp"
#include "Utilities.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char* argv[])
{
    if(argc <= 7)
    {
        std::cout << "usage: RandomGraphTest K <seed> <minimum-n> <maximum-n> <max-m> <min-dist> <max-dist>" << std::endl;
        return -1;
    }

    const int K = atoi(argv[1]);
    const int seed = atoi(argv[2]);
    const int MN = atoi(argv[3]);
    const int MX = atoi(argv[4]);
    const int MM = atoi(argv[5]);
    const float MND = strtof(argv[6], nullptr);
    const float MXD = strtof(argv[7], nullptr);

    auto start = Clock::now();
    auto adjList = GenGraph(seed, MN, MX, MM, MND, MXD);
    std::chrono::duration<double, std::milli> dur = Clock::now() - start;

    std::cout << "Graph of " << adjList.GetSize() << " vertices and " << adjList.GetEdgeCount() << " edges. Generated in: " << dur.count() << " ms." << std::endl;

    const int N = adjList.GetSize();

    double avgTime = 0;
    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0.f));
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            start = Clock::now();
            dmat[i][j] = adjList.GetDistance(i, j);
            dur = Clock::now() - start;
            avgTime += dur.count();
        }
    }
    std::cout << "Average bruteforce query time: " <<avgTime <<","<<  avgTime / (N * N) << " ms." << std::endl;

    // avgTime = 0.00;
    // for(int i = 0; i < N; i++)
    // {
    //     for(int j = 0; j < N; j++)
    //     {
    //         start = Clock::now();
    //         float d = adjList.GetDistanceFib(i, j);
    //         dur = Clock::now() - start;
    //         assert(dmat[i][j] == d);
    //         avgTime += dur.count();
    //     }
    // }
    // std::cout << "Average fib heap query time: " << avgTime / (N * N) << " ms." << std::endl;

    // std::cout << "Generated. Testing..." << std::endl;
    start = Clock::now();
    Oracle<int, float> oracle(adjList, K);
    dur = Clock::now() - start;
    std::cout << "Oracle generated in: " << dur.count() << " ms." << std::endl;

    avgTime = 0;
	double totTime = dur.count();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            start = Clock::now();
            assert(oracle.Query(i, j) <= (2 * K - 1) * dmat[i][j]);
            dur = Clock::now() - start;
            avgTime += dur.count();
        }
    }
    std::cout << "Average oracle query time: " << avgTime << "," <<  avgTime / (N * N) << " ms." << std::endl;
    std::cout << "Average oracle query+preprocessing time: " << (avgTime+totTime) / (N * N) << " ms." << std::endl;

    return 0;
}
