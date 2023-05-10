#ifndef ORACLE_FIB_H
#define ORACLE_FIB_H

#include "boost/heap/fibonacci_heap.hpp"

#include <functional>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <assert.h>

#include "AdjacencyList.hpp"

template<typename NodeType, typename DistanceType>
class OracleFib
{
    using AdjList = AdjacencyList<NodeType, DistanceType>;

    public:
        // WARN: only works for connected graphs
        OracleFib(AdjList lst, int K) : graph(lst), K(K)
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

#ifdef PRINT_EXTRA
            for(int i = 0; i <= K; i++)
            {
                std::cout << "A[" << i << "]:";
                for(NodeType x: A[i])
                {
                    std::cout << " " << x;
                }
                std::cout << std::endl;
            }
#endif

            std::vector<std::unordered_map<NodeType, DistanceType>> C(graph.GetSize());
            for(int i = K - 1; i >= 0; i--)
            {
#ifdef PRINT_EXTRA
                std::cout << i << ":";
#endif

                std::vector<std::pair<NodeType, DistanceType>> T = SSSPTree(i);

                for(NodeType x = 0; x < graph.GetSize(); x++)
                {
                    // Find nearest to x in A[i]
  					auto temp = T[x];
					auto piv = temp.first; auto div = temp.second;
                    if(div == P[i + 1][x].second) piv = P[i + 1][x].first;

#ifdef PRINT_EXTRA
                    std::cout << " {" << x << ": " << piv << ", " << div << "}";
#endif

                    P[i][x] = {piv, div};
                }
#ifdef PRINT_EXTRA
                std::cout << std::endl;
#endif

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
#ifdef PRINT_EXTRA
                std::cout << w << ":";
#endif
                for(auto temp1: C[w])
                {
					auto v = temp1.first;auto d = temp1.second;
#ifdef PRINT_EXTRA
                    std::cout << " {" << v << ":" << d << "}";
#endif
                    B[v][w] = d;
                }
#ifdef PRINT_EXTRA
                std::cout << std::endl;
#endif
            }
        }

        // HACK: Do we need to care about P_i(v) = P_{i + 1}(v) in case of multiple witnesses, shouldn't priority_queue maintain that?
        std::vector<std::pair<NodeType, DistanceType>> SSSPTree(int i)
        {
            std::vector<DistanceType> dist(graph.GetSize(), std::numeric_limits<DistanceType>::max());

            using Heap = boost::heap::fibonacci_heap<std::pair<DistanceType, NodeType>, 
                  boost::heap::compare<std::greater<std::pair<DistanceType, NodeType>>>>;

            std::vector<typename Heap::handle_type> handles(graph.GetSize());
            Heap q;

            std::vector<std::pair<NodeType, DistanceType>> res(graph.GetSize(), {-1, std::numeric_limits<DistanceType>::max()});

            for(std::size_t j = 0; j < graph.GetSize(); j++)
            {
                handles[j] = q.emplace(std::numeric_limits<DistanceType>::max(), j);
            }

            for(NodeType x: A[i])
            {
                dist[x] = 0;
                
                *handles[x] = { 0, x };
                q.decrease(handles[x]);

                res[x] = {x, 0};
            }

            while(!q.empty())
            {
				auto temp = q.top();
				auto d = temp.first;auto a = temp.second;
                q.pop();

                // if(dist[a] < d) continue;

 				for(auto temp1: graph.GetAllEdges(a))
                {
					auto b = temp1.first;auto d1 = temp1.second;
                    if(d1 + d < dist[b] || (d1 + d == dist[b] && res[b].first > res[a].first))
                    {
                        dist[b] = d1 + d;

                        *handles[b] = { d1 + d, b };
                        q.decrease(handles[b]);

                        res[b] = { res[a].first, d1 + d };
                    }
                }
            }
            return res;
        }

        std::unordered_map<NodeType, DistanceType> GenCluster(NodeType w, int pi)
        {
            std::vector<DistanceType> dist(graph.GetSize(), std::numeric_limits<DistanceType>::max());
            dist[w] = 0;

            using Heap = boost::heap::fibonacci_heap<std::pair<DistanceType, NodeType>, 
                  boost::heap::compare<std::greater<std::pair<DistanceType, NodeType>>>>;

            std::vector<typename Heap::handle_type> handles(graph.GetSize());

            Heap q;
            handles[w] = q.emplace(0, w);

            for(NodeType i = 0; i < graph.GetSize(); i++)
            {
                if(i == w) continue;
                handles[i] = q.emplace(std::numeric_limits<DistanceType>::max(), i);
            }

            std::unordered_map<NodeType, DistanceType> res;
            while(!q.empty())
            {
				auto temp = q.top();
				auto d = temp.first;auto a = temp.second;
                q.pop();

                // if(dist[a] < d) continue;

                res[a] = d;
 				for(auto temp1: graph.GetAllEdges(a))
                {
					auto b = temp1.first;auto d1 = temp1.second;
                    if(d1 + d < dist[b] && d1 + d < P[pi + 1][b].second)
                    {
                        dist[b] = d1 + d;
                        *handles[b] = { d1 + d, b };
                        q.decrease(handles[b]);
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
                assert(i <= K);
                std::swap(u, v);
                w = P[i][u].first;
            }

            // WARN: I am not sure this is how it should work
            assert(B[u].count(w) > 0);
            assert(B[v].count(w) > 0);
            return B[u][w] + B[v][w];
        }

    private:
        int K;
        AdjList graph;
        std::vector<std::vector<NodeType>> A;
        std::vector<std::vector<bool>> AC;
        std::vector<std::vector<std::pair<NodeType, DistanceType>>> P;
        std::vector<std::unordered_map<NodeType, DistanceType>> B;
};

#endif
