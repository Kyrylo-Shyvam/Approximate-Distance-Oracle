#ifndef GUTILITIES_H
#define GUTILITIES_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <random>
#include <algorithm>
#include <vector>
#include <string>

#include "AdjacencyList.hpp"
#include "UFDS.hpp"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;

    while (getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;

    split(s, delim, elems);

    return elems;
}

inline bool IsConnected(AdjacencyList<int, float> graph)
{
    int n = graph.GetSize();
    UFDS uf(n);

    for(int i = 0; i < n; i++)
    {
        for(auto temp: graph.GetAllEdges(i)) uf.Union(i, temp.first);
    }
    return uf.Components().size() == 1;
}

inline AdjacencyList<int, float> GenGraph(std::string fname)
{
    AdjacencyList<int, float> adjList(1);
	std::string line,pname;
	std::ifstream fin(fname);
	std::vector<std::string> nbrs;
	int size,edges;
	int k=0;
	while(1)
	{
		getline(fin,line);
		if(line[0]=='%')
			continue;
		if(!k)
		{
			nbrs=split(line,' ');
			size=atoi(nbrs[0].c_str());
			edges=atoi(nbrs[2].c_str());
		    std::cout << size << std::endl;

			AdjacencyList<int, float> X(size);
			adjList = X;
			k=1;
			break;
		}
	}
	while(getline(fin,line))
	{
		if(line[0]=='%')
			continue;
		nbrs=split(line,' ');
		int x = atoi(nbrs[0].c_str());
		int y = atoi(nbrs[1].c_str());
		adjList.AddUndirectedEdge(x-1, y-1,rand()%4+1);
	}

    assert(IsConnected(adjList));
    return adjList;
}

inline std::vector<std::vector<double>>  FW(AdjacencyList<int, float> adjList)
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
	return dp;
}
#endif

