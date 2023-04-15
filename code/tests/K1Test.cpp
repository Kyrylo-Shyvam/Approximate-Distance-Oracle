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
        std::cout << "usage: K1Test <seed> <minimum-n> <maximum-n> <max-m> <min-dist> <max-dist>" << std::endl;
        return -1;
    }

    const int seed = atoi(argv[1]);
    const int MN = atoi(argv[2]);
    const int MX = atoi(argv[3]);
    const int MM = atoi(argv[4]);
    const float MND = strtof(argv[5], nullptr);
    const float MXD = strtof(argv[6], nullptr);

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

    Oracle<int, float, 1> oracle(adjList);
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            assert(oracle.Query(i, j) == dmat[i][j]);
        }
    }

    return 0;
}
