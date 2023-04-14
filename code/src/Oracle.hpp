#ifndef ORACLE_H
#define ORACLE_H

#include <vector>
#include <cmath>
#include <random>

#include "AdjacencyList.hpp"

template<typename NodeType, typename DistanceType, int K>
class Oracle
{
    using AdjList = AdjacencyList<NodeType, DistanceType>;

    public:
        Oracle(AdjList lst) : graph(lst)
        {
            std::vector<std::vector<NodeType>> A(K + 1, std::vector<NodeType>());
            for(NodeType i = 0; i < graph.GetSize(); i++) A[0].push_back(i);

            double prob = std::pow((double)graph.GetSize(), -1.00 / K);
            std::bernoulli_distribution dist(prob);

            std::random_device rd;
            std::mt19937 gen(rd()); // seed

            for(int i = 1; i < K; i++)
            {
                do {
                    for(NodeType x: A[i - 1])
                    {
                        if(dist(gen)) A[i].push_back(x);
                    }
                } while(A[i].size() == 0); // TODO: is this correct? we dont want to get an empty list before A[K] but can this end up in infinite loop
            }

            for(int i = K - 1; i >= 0; i--)
            {
                for(NodeType x = 0; x < graph.GetSize(); x++)
                {
                    // Find nearest to x in A[i]
                }
            }
        }

    private:
        AdjList graph;
};

#endif
