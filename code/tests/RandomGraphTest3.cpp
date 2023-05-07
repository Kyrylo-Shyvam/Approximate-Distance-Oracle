#include "../src/Oracle.hpp"
#include "../src/OracleFib.hpp"
#include "AdjacencyList.hpp"
#include "UFDS.hpp"
#include "Utilities.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <iomanip>

using Clock = std::chrono::high_resolution_clock;

std::vector<std::vector<float>> DoDjikstraFor(int s, int e, int n, AdjacencyList<int, float> adj, double& res)
{
    int N = n;

    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0.f));
    auto start = Clock::now();
    for(int i = s; i <= e; i++)
    {
        for(int j = 0; j < n; j++)
        {
            dmat[i][j] = adj.GetDistance(i, j);
        }
    }
    auto end = Clock::now();

    res = std::chrono::duration<double, std::milli>(end - start).count();
    return dmat;
}

int main(int argc, char* argv[])
{
    if(argc < 7)
    {
        std::cout << "usage: RandomGraphTest K <seed> <n> <m> <min-dist> <max-dist>" << std::endl;
        return -1;
    }

    const int K = atoi(argv[1]);
    const int seed = atoi(argv[2]);
    const int N = atoi(argv[3]);
    const int M = atoi(argv[4]);
    const float MND = strtof(argv[5], nullptr);
    const float MXD = strtof(argv[6], nullptr);

    auto start = Clock::now();
    auto adjList = GenGraph(seed, N, M, MND, MXD);
    std::chrono::duration<double, std::milli> dur = Clock::now() - start;

    std::cout << adjList.GetSize() << "," << adjList.GetEdgeCount() << "," << dur.count() << ",";

    unsigned int threadCount = std::thread::hardware_concurrency();
    if(threadCount == 0) threadCount = 2;

    std::vector<std::thread> threads;
    std::vector<double> threadTime(threadCount, 0);

    int ts = (N + threadCount - 1) / threadCount;
    for(int i = 0, j = 0; i < N; i += ts, j++)
    {
        int s = i;
        int e = std::min(N, ts - 1);

        threads.push_back(std::thread(DoDjikstraFor, s, e, N, adjList, std::ref(threadTime[j])));
    }

    for(int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

    double avgTime = std::accumulate(threadTime.begin(), threadTime.end(), 0.00) / threads.size();
    std::cout << avgTime << ",";

    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0));

    start = Clock::now();
    Oracle<int, float> oracle(adjList, K);
    dur = Clock::now() - start;
    std::cout << dur.count() << ",";

    avgTime = 0;
    start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            dmat[i][j] = oracle.Query(i, j);
        }
    }
    dur = Clock::now() - start;
    std::cout << dur.count() / (N * N) << ",";

    start = Clock::now();
    OracleFib<int, float> oracleFib(adjList, K);
    dur = Clock::now() - start;
    std::cout << dur.count() << ",";

    avgTime = 0;
    start = Clock::now();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            dmat[i][j] = oracleFib.Query(i, j);
        }
    }
    dur = Clock::now() - start;
    std::cout << dur.count() / (N * N) << ",";

	start = Clock::now();
	auto dp = FW(adjList);
    dur = Clock::now() - start;
    std::cout << dur.count() << std::endl;

    // for(int i = 0; i < N; i++)
    // {
    //     for(int j = 0; j < N; j++)
    //     {
    //         double q = dmat[i][j] / adjList.GetDistance(i, j);
    //         if(q < 1 || q > 2 * K - 1) {
    //             std::cerr << i << ' ' << j << ' ' << std::setprecision(8) << dmat[i][j] << " > (2 * K - 1) * " << dp[i][j] << std::endl;
    //             // return -1;
    //         }
    //     }
    // }

    return 0;
}
