#ifndef GRAPH_H
#define GRAPH_H

#include "boost/heap/fibonacci_heap.hpp"

#include <limits>
#include <vector>
#include <queue>
#include <unordered_set>

template<typename NodeType, typename DistanceType>
class AdjacencyList
{
    public:
        using Edges = std::vector<std::pair<NodeType, DistanceType>>;
        using Matrix = std::vector<Edges>;

        AdjacencyList(std::size_t v) : sz(v), edgeCount(0) { mat.assign(v, std::vector<std::pair<NodeType, DistanceType>>()); }

        void AddUndirectedEdge(NodeType u, NodeType v, DistanceType d)
        {
            mat[u].push_back(std::make_pair(v, d));
            mat[v].push_back(std::make_pair(u, d));
            edgeCount++;
        }

        const std::vector<std::pair<NodeType, DistanceType>>& GetAllEdges(NodeType v) { return mat[v]; }
        const Matrix& GetMatrix() const { return mat; }
        std::size_t GetSize() const { return sz; }
        std::size_t GetEdgeCount() const { return edgeCount; }

        // Use some shortest path algorithm
        std::pair<NodeType, DistanceType> GetNearest(NodeType u, std::vector<NodeType> v) const
        {
            std::unordered_set<NodeType> vs(v.begin(), v.end());

            std::vector<DistanceType> dist(sz, std::numeric_limits<DistanceType>::max());
            dist[u] = 0;

            std::priority_queue<std::pair<DistanceType, NodeType>, std::vector<std::pair<DistanceType, NodeType>>, std::greater<std::pair<DistanceType, NodeType>>> q;
            q.push({0, u});

            while(!q.empty())
            {
				auto temp = q.top();
				auto d = temp.first;auto a =temp.second;
                q.pop();

                if(dist[a] < d) continue;
                if(vs.count(a)) return {a, d};

 				for(auto temp1: mat[a])
                {
					auto b = temp1.first;auto d1 =temp1.second;
                    if(d1 + d < dist[b])
                    {
                        dist[b] = d1 + d;
                        q.push({d1 + d, b});
                    }
                }
            }
            return {-1, 0.f};
        }

        std::pair<NodeType, DistanceType> GetNearestFib(NodeType u, std::vector<NodeType> v) const
        {
            std::unordered_set<NodeType> vs(v.begin(), v.end());

            std::vector<DistanceType> dist(sz, std::numeric_limits<DistanceType>::max());
            dist[u] = 0;

            using Heap = boost::heap::fibonacci_heap<std::pair<DistanceType, NodeType>, 
                  boost::heap::compare<std::greater<std::pair<DistanceType, NodeType>>>>;

            // std::vector<bool> has_handle(sz, false);
            std::vector<typename Heap::handle_type> handles(sz);

            Heap q;
            handles[u] = q.emplace(0, u);

            for(NodeType i = 0; i < sz; i++)
            {
                if(i == u) continue;
                handles[i] = q.emplace(std::numeric_limits<DistanceType>::max(), i);
            }

            while(!q.empty())
            {
 				auto temp = q.top();
				auto d = temp.first;auto a =temp.second;
                q.pop();

                if(vs.count(a)) return {a, d};

 				for(auto temp1: mat[a])
                {
					auto b = temp1.first;auto d1 =temp1.second;
                    if(d1 + d < dist[b])
                    {
                        dist[b] = d1 + d;
                        *handles[b] = { d1 + d, b };
                        q.decrease(handles[b]);
                    }
                }
            }
            return {-1, 0.f};
        }

        DistanceType GetDistance(NodeType u, NodeType v) const
        {
            return GetNearest(u, {v}).second;
        }

        DistanceType GetDistanceFib(NodeType u, NodeType v) const
        {
            return GetNearestFib(u, {v}).second;
        }

    private:
        std::size_t sz;
        std::size_t edgeCount;
        Matrix mat;
};

#endif
