#include "../src/Oracle.hpp"
#include "AdjacencyList.hpp"
#include "UFDS.hpp"
#include "Utilities.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <random>

int main(int argc, char* argv[])
{
    if(argc < 7)
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

    auto adjList = GenGraph(seed, MN, MX, MM, MND, MXD);
    const int N = adjList.GetSize();

    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0.f));
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            dmat[i][j] = adjList.GetDistance(i, j);
        }
    }

    // std::cout << "Generated. Testing..." << std::endl;
    Oracle<int, float> oracle(adjList, K);
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            assert(oracle.Query(i, j) <= (2 * K - 1) * dmat[i][j]);
        }
    }

    return 0;
}
