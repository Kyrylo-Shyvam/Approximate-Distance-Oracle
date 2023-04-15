#ifndef UTILITIES_H
#define UTILITIES_H

#include <cassert>
#include <iostream>
#include <limits>
#include <random>

#include "AdjacencyList.hpp"
#include "UFDS.hpp"

inline AdjacencyList<int, float> GenGraph(int seed, int MN, int MX, int MM, int MND, int MXD)
{
    std::mt19937 gen(seed);
    unsigned int N = std::uniform_int_distribution<>(MN, MX)(gen);

    std::uniform_int_distribution<> rnd(0, std::numeric_limits<int>::max());
    std::uniform_real_distribution<> drnd(MND, MXD);

    AdjacencyList<int, float> adjList(N);

    UFDS uf(N);
    std::set<std::pair<int, int>> done;

    // make a tree
    for(int i = 0; i < N - 1; i++)
    {
        auto comps = uf.Components();

        int x = rnd(gen) % comps.size();
        int y = rnd(gen) % comps.size();
        while(x == y) y = rnd(gen) % comps.size();

        uf.Union(x, y);
        adjList.AddUndirectedEdge(x, y, drnd(gen));

        done.insert({std::min(x, y), std::max(x, y)});
    }

    int M = rnd(gen) % (MM - N + 1);
    for(int i = 0; i < M; i++)
    {
        int x = rnd(gen) % N;
        int y = rnd(gen) % N;
        if(x == y || done.count({std::min(x, y), std::max(x, y)}))
        {
            i--;
            continue;
        }

        adjList.AddUndirectedEdge(x, y, drnd(gen));
        done.insert({std::min(x, y), std::max(x, y)});
    }

    return adjList;
}

#endif
