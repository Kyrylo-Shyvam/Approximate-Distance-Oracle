#ifndef GRAPH_H
#define GRAPH_H

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

        AdjacencyList(std::size_t v) { mat.assign(v, std::vector<std::pair<NodeType, DistanceType>>()); sz = v; }

        void AddUndirectedEdge(NodeType u, NodeType v, DistanceType d)
        {
            mat[u].push_back(std::make_pair(v, d));
            mat[v].push_back(std::make_pair(u, d));
        }

        const std::vector<std::pair<NodeType, DistanceType>>& GetAllEdges(NodeType v) { return mat[v]; }
        const Matrix& GetMatrix() const { return mat; }
        std::size_t GetSize() const { return sz; }

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
                auto [d, a] = q.top();
                q.pop();

                if(dist[a] < d) continue;
                if(vs.count(a)) return {a, d};

                for(auto [b, d1]: mat[a])
                {
                    if(d1 + d < dist[b])
                    {
                        dist[b] = d1 + d;
                        q.push({d1 + d, b});
                    }
                }
            }
            return {-1, 0.f};
        }

        DistanceType GetDistance(NodeType u, NodeType v) const
        {
            return GetNearest(u, {v}).second;
        }

    private:
        std::size_t sz;
        Matrix mat;
};

#endif
