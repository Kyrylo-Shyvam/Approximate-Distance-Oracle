#include "../src/AdjacencyList.hpp"

#include <iostream>

int main()
{
    AdjacencyList<int, float> lst1(5);
    lst1.AddUndirectedEdge(0, 1, 10.f);
    lst1.AddUndirectedEdge(1, 2, 10.f);
    lst1.AddUndirectedEdge(4, 2, 5.f);

    for(int i = 0; i < lst1.GetSize(); i++)
    {
        std::cout << i << ":";
        for(auto& x: lst1.GetAllEdges(i))
        {
            std::cout << " [" << x.first << ", " << x.second << "]";
        }
        std::cout << std::endl;
    }

    AdjacencyList<unsigned int, float> lst2(5);
    lst2.AddUndirectedEdge(0, 1, 10.f);
    lst2.AddUndirectedEdge(1, 2, 10.f);
    lst2.AddUndirectedEdge(4, 2, 5.f);

    for(int i = 0; i < lst2.GetSize(); i++)
    {
        std::cout << i << ":";
        for(auto& x: lst2.GetAllEdges(i))
        {
            std::cout << " [" << x.first << ", " << x.second << "]";
        }
        std::cout << std::endl;
    }

    return 0;
}
