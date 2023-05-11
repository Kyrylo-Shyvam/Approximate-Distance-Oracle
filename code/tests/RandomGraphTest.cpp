#include "../src/Oracle.hpp"
#include "../src/OracleFib.hpp"
#include "AdjacencyList.hpp"
#include "UFDS.hpp"
#include "Utilities.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

std::vector<std::vector<float>> RunOracle(AdjacencyList<int, float> adjList, int K)
{
    int N = adjList.GetSize();

    auto start = Clock::now();
    Oracle<int, float> oracle(adjList, K);
    auto dur = Clock::now() - start;
    std::cout << "Oracle generated in: " << dur.count() << " ms." << std::endl;

    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0.f));
    start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            float c = oracle.Query(i, j);
            // assert(c <= (2 * K - 1) * dmat[i][j]);
            dmat[i][j] = c;
        }
    }
    dur = Clock::now() - start;
    std::cout << "Average oracle query time: " << "," <<  dur.count()/ (N * N) << " ms." << std::endl;
    return dmat;
}

std::vector<std::vector<float>> RunOracleFib(AdjacencyList<int, float> adjList, int K)
{
    int N = adjList.GetSize();

    auto start = Clock::now();
    OracleFib<int, float> oracle(adjList, K);
    auto dur = Clock::now() - start;
    std::cout << "Oracle generated in: " << dur.count() << " ms." << std::endl;

    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0.f));
    start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            float c = oracle.Query(i, j);
            // assert(c <= (2 * K - 1) * dmat[i][j]);
            dmat[i][j] = c;
        }
    }
    dur = Clock::now() - start;
    std::cout << "Average oracle query time: " << "," <<  dur.count()/ (N * N) << " ms." << std::endl;
    return dmat;
}

std::vector<std::vector<float>> RunDjikstra(AdjacencyList<int, float> adjList, int K)
{
    int N = adjList.GetSize();

    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0.f));
    auto start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        dmat[i] = adjList.GetDistance(i);
    }
    auto dur = Clock::now() - start;
    std::cout << "Average bruteforce query time: " <<  dur.count() / (N * N) << " ms." << std::endl;
    return dmat;
}

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
    auto adjList = GenGraph<double>(seed, MN, MX, MM, MND, MXD);
    std::chrono::duration<double, std::milli> dur = Clock::now() - start;

    std::cout << "Graph of " << adjList.GetSize() << " vertices and " << adjList.GetEdgeCount() << " edges. Generated in: " << dur.count() << " ms." << std::endl;

    const int N = adjList.GetSize();

    std::vector<std::vector<double>> dmat(N, std::vector<double>(N, 0.f));
    start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        dmat[i] = adjList.GetDistance(i);
    }
    dur = Clock::now() - start;
    std::cout << "Average bruteforce query time: " <<  dur.count() / (N * N) << " ms." << std::endl;

    start = Clock::now();
    Oracle<int, double> oracle(adjList, K);
    dur = Clock::now() - start;
    std::cout << "Oracle generated in: " << dur.count() << " ms." << std::endl;

    start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            float c = oracle.Query(i, j);
            assert(c <= (2 * K - 1) * dmat[i][j]);
            dmat[i][j] = c;
        }
    }
    dur = Clock::now() - start;
    std::cout << "Average oracle query time: " <<  dur.count()/ (N * N) << " ms." << std::endl;

    start = Clock::now();
    OracleFib<int, double> oracleFib(adjList, K);
    dur = Clock::now() - start;
    std::cout << "OracleFib generated in: " << dur.count() << " ms." << std::endl;

    start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            float c = oracleFib.Query(i, j);
            assert(c <= (2 * K - 1) * dmat[i][j]);
            dmat[i][j] = c;
        }
    }
    dur = Clock::now() - start;
    std::cout << "AverageFib oracle query time: " <<  dur.count()/ (N * N) << " ms." << std::endl;

    return 0;
}
