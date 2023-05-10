#include "../src/Oracle.hpp"
#include "AdjacencyList.hpp"
#include "UFDS.hpp"
#include "GraphUtilities.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <random>
#include <chrono>
#include <string.h>

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char* argv[])
{
    if(argc !=3 )
    {
        std::cout << "usage: ./GraphTest <K> <location> " << std::endl;
        return -1;
    }

    const int K = atoi(argv[1]);
	std::string fname;
	fname = argv[2];
	//std::ifstream fin(fname);

    auto start = Clock::now();
    auto adjList = GenGraph(fname);
    std::chrono::duration<double, std::milli> dur = Clock::now() - start;

    std::cout << "" << adjList.GetSize() << "," << adjList.GetEdgeCount() << "," << dur.count() << ",";


    const int N = adjList.GetSize();

    double avgTime = 0;
    std::vector<std::vector<float>> dmat(N, std::vector<float>(N, 0.f));
	for(int j = 0; j < N; j++)
	{
		start = Clock::now();
		dmat[j] = adjList.GetDistance(j);
		dur = Clock::now() - start;
		avgTime += dur.count();
		if(j%1000 == 0)
			std::cout << j<<std::endl;
	}
    std::cout << avgTime <<",";// << avgTime / (N * N) << ",";

    // std::cout << "Generated. Testing..." << std::endl;
    start = Clock::now();
    Oracle<int, float> oracle(adjList, K);
    dur = Clock::now() - start;
    std::cout << "" << dur.count() << ",";

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
    std::cout << avgTime<<"," ;//<< avgTime / (N * N) << ",";
    std::cout << "" << (avgTime+totTime) / (N * N) << ",";
/*
	start = Clock::now();
	auto dp = FW(adjList);
    dur = Clock::now() - start;
    std::cout << "" << dur.count() ;//<< ","<<dur.count()/(N*N)<<std::endl;

*/
    return 0;
}

