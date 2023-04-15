#ifndef ORACLE_H
#define ORACLE_H

#include <vector>
#include <cmath>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <assert.h>

#include "AdjacencyList.hpp"

template<typename NodeType, typename DistanceType, int K>
class Oracle
{
    using AdjList = AdjacencyList<NodeType, DistanceType>;

    public:
        // WARN: only works for connected graphs
        Oracle(AdjList lst) : graph(lst)
        {
            A.assign(K + 1, std::vector<NodeType>());

            // used for fast lookup for calculating A[i] - A[i + 1]
            AC.assign(K + 1, std::vector<bool>(graph.GetSize(), false));

            B.assign(graph.GetSize(), std::unordered_map<NodeType, DistanceType>());

            P.assign(K + 1, std::vector<std::pair<NodeType, DistanceType>>(graph.GetSize(), std::make_pair(-1, std::numeric_limits<DistanceType>::max())));

            for(NodeType i = 0; i < graph.GetSize(); i++)
            {
                A[0].push_back(i);
                AC[0][i] = true;
            }

            double prob = std::pow((double)graph.GetSize(), -1.00 / K);
            std::bernoulli_distribution dist(prob);

            std::random_device rd;
            std::mt19937 gen(rd()); // seed

            for(int i = 1; i < K; i++)
            {
                do {
                    for(NodeType x: A[i - 1])
                    {
                        if(dist(gen))
                        {
                            A[i].push_back(x);
                            AC[i][x] = true;
                        }
                    }
                } while(A[i].size() == 0); // HACK: is this correct? we dont want to get an empty list before A[K] but can this end up in infinite loop
            }

            std::vector<std::unordered_map<NodeType, DistanceType>> C(graph.GetSize());
            for(int i = K - 1; i >= 0; i--)
            {
                for(NodeType x = 0; x < graph.GetSize(); x++)
                {
                    // Find nearest to x in A[i]
                    auto [piv, div] = graph.GetNearest(x, A[i]);
                    if(div == P[i + 1][x].second) piv = P[i + 1][x].first;

                    P[i][x] = {piv, div};
                }

                for(NodeType x: A[i])
                {
                    // skip if A[i + 1] contains x
                    if(AC[i + 1][x]) continue;

                    // grow shortest path tree from x
                    C[x] = GenCluster(x, i);
                }
            }

            for(NodeType w = 0; w < graph.GetSize(); w++)
            {
                for(auto [v, d]: C[w])
                {
                    B[v][w] = d;
                }
            }
        }

        std::unordered_map<NodeType, DistanceType> GenCluster(NodeType w, int pi)
        {
            std::vector<DistanceType> dist(graph.GetSize(), std::numeric_limits<DistanceType>::max());
            dist[w] = 0;

            std::priority_queue<std::pair<DistanceType, NodeType>> q;
            q.push({0, w});

            std::unordered_map<NodeType, DistanceType> res;
            while(!q.empty())
            {
                auto [d, a] = q.top();
                q.pop();

                if(dist[a] < d) continue;

                res[a] = d;
                for(auto [b, d1]: graph.GetAllEdges(a))
                {
                    if(d1 + d < P[pi][b].second)
                    {
                        dist[b] = d1 + d;
                        q.push({d1 + d, b});
                    }
                }
            }
            return res;
        }

        DistanceType Query(NodeType u, NodeType v)
        {
            int i = 0;
            NodeType w = u;

            while(B[v].count(w) == 0)
            {
                i++;
                std::swap(u, v);
                w = P[i][u].first;
            }

            // WARN: I am not sure this is how it should work
            assert(B[u].count(w) > 0);
            assert(B[v].count(w) > 0);
            return B[u][w] + B[v][w];
        }

    private:
        AdjList graph;
        std::vector<std::vector<NodeType>> A;
        std::vector<std::vector<bool>> AC;
        std::vector<std::vector<std::pair<NodeType, DistanceType>>> P;
        std::vector<std::unordered_map<NodeType, DistanceType>> B;
};

#endif
