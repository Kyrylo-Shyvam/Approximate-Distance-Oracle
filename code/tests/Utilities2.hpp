#ifndef UTILITIES2_H
#define UTILITIES2_H

#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <algorithm>
#include <vector>

#include "AdjacencyList2.hpp"
#include "UFDS.hpp"

inline bool IsConnected(AdjacencyList<int, float> graph)
{
    int n = graph.GetSize();
    UFDS uf(n);

    for(int i = 0; i < n; i++)
    {
        for(auto [x, d]: graph.GetAllEdges(i)) uf.Union(i, x);
    }
    return uf.Components().size() == 1;
}

inline AdjacencyList<int, float> GenGraph(int seed, int MN, int MX, int MM, int MND, int MXD)
{
    std::mt19937 gen(seed);
    int N = std::uniform_int_distribution<>(MN, MX)(gen);

    std::uniform_int_distribution<> rnd(0, std::numeric_limits<int>::max());
    std::uniform_real_distribution<> drnd(MND, MXD);

    AdjacencyList<int, float> adjList(N);

    UFDS uf(N);
    std::vector<std::pair<int, int>> notdone;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < i; j++)
        {
            notdone.push_back({j, i});
        }
    }

    // make a tree
    for(int i = 0; i < N - 1; i++)
    {
        std::vector<int> comps(uf.Components().begin(), uf.Components().end());

        int x = rnd(gen) % comps.size();
        int y = rnd(gen) % comps.size();
        while(x == y) y = rnd(gen) % comps.size();

        x = comps[x];
        y = comps[y];

        uf.Union(x, y);
        adjList.AddUndirectedEdge(x, y, drnd(gen));

        notdone.erase(std::find(notdone.begin(), 
                    notdone.end(), 
                    std::make_pair(std::min(x, y), std::max(x, y))));
    }
    assert(IsConnected(adjList));


    int M = (std::min(MM, (N * (N - 1)) / 2) - N + 1);
    for(int i = 0; i < M; i++)
    {
        int j = rnd(gen) % notdone.size();

        auto [x, y] = notdone[j];

        adjList.AddUndirectedEdge(x, y, drnd(gen));
        notdone.erase(notdone.begin() + j);
    }
    return adjList;
}

inline void  FW(AdjacencyList<int, float> adjList)
{
	int sizeMatrix = adjList.GetSize();
	int edgesMatrix = adjList.GetEdgeCount();
	auto matrix  = adjList.GetMatrix();

	std::vector<std::vector<double>> dp(sizeMatrix,
			std::vector<double>(sizeMatrix,-1));
	int vertexNo = 0;
	for(auto edge : matrix)
	{

		for(auto v : edge) 
		{
			dp[v.first][vertexNo] = v.second;
			dp[vertexNo][v.first] = v.second;
		}
		vertexNo++;
	}
	for (int k = 0; k < sizeMatrix; ++k) {
		for (int i = 0; i < sizeMatrix; ++i) {
			for (int j = 0; j < sizeMatrix; ++j) {
				dp[i][j] = fmin(dp[i][j], dp[i][k] + dp[k][j]);
			}
		}
	}
	//std::cout << "FW" << sizeMatrix;

}
#endif
