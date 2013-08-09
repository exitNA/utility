#ifndef GRAPH_H_
#define GRAPH_H_

#include <map>
#include <vector>


template<typename VertexT, typename DataT>
struct Edge
{
    VertexT from;
    VertexT to;
    DataT   weight;
};


template<typename VertexT, typename DataT>
struct EdgeList
{
    std::map<VertexT, DataT> edges;
};


template<typename VertexT, typename DataT>
class Graph
{
    std::map<VertexT, EdgeList> _vertices;

public:
    EdgeList operator[](VertexT& vertex)
    {
        return _vertices[vertex];
    }
};

#endif