#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

template<typename NodeType, typename DistanceType>
class AdjacencyList
{
    public:
        using Matrix = std::vector<std::vector<std::pair<NodeType, DistanceType>>>;

        AdjacencyList(std::size_t v) { mat.assign(v, std::vector<std::pair<NodeType, DistanceType>>()); sz = v; }

        void AddUndirectedEdge(NodeType u, NodeType v, DistanceType d)
        {
            mat[u].push_back(std::make_pair(v, d));
            mat[v].push_back(std::make_pair(u, d));
        }

        const std::vector<std::pair<NodeType, DistanceType>>& GetAllEdges(NodeType v) { return mat[v]; }

        const Matrix& GetMatrix() const { return mat; }
        std::size_t GetSize() const { return sz; }

    private:
        std::size_t sz;
        Matrix mat;
};

#endif
