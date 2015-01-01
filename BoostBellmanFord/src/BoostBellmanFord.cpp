//============================================================================
// Name        : BoostBellmanFord.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>

using namespace std;
using namespace boost;

template < typename Graph, typename ParentMap >
struct edge_writer
{
	const Graph & m_g;
	ParentMap m_parent;

	edge_writer(const Graph & g, const ParentMap & p) : m_g(g), m_parent(p)
	{
	}

	template < typename Edge >
	void operator() ( ostream & out, const Edge & e ) const
	{
		out << "[label=\"" << get(edge_weight, m_g, e) << "\"";
		typename graph_traits < Graph >::vertex_descriptor u = source(e, m_g), v = target(e, m_g);
		if (m_parent[v] == u)
			out << ", color=\"black\"";
		else
			out << ", color=\"grey\"";
		out << "]";
	}

};

template < typename Graph, typename Parent >
edge_writer < Graph, Parent >
make_edge_writer(const Graph & g, const Parent & p)
{
	return edge_writer < Graph, Parent > (g, p);
}

struct EdgeProperties
{
	int weight;
};

int main() {
	enum { u, v, x, y, z, N };
	char name[] = { 'u', 'v', 'x', 'y', 'z' };
	typedef pair< int, int > E;
	const int n_edges = 10;
	E edge_array[] = { E(u, y), E(u, x), E(u, v), E(v, u), E(x, y), E(x, v), E(y, v), E(y, z), E(z, u), E(z, x)  };
	int weight[n_edges] = { -4, 8, 5, -2, 9, -3, 7, 2, 6, 7 };

	// template <class OutEdgeListS = vecS, // a Sequence or an AssociativeContainer
	//            class VertexListS = vecS, // a Sequence or a RandomAccessContainer
	//            class DirectedS = directedS,
	//            class VertexProperty = no_property,
	//            class EdgeProperty = no_property,
	//            class GraphProperty = no_property,
	//            class EdgeListS = listS>
	typedef adjacency_list < vecS, vecS, directedS, no_property, EdgeProperties > Graph;
    Graph g(edge_array, edge_array + n_edges, N);
    graph_traits <Graph >::edge_iterator ei, ei_end;
    property_map< Graph, int EdgeProperties::* >::type weight_pmap = get(&EdgeProperties::weight, g);
    int i = 0;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++i)
    	weight_pmap[*ei] = weight[i];

    std::vector<int> distance(N, (std::numeric_limits < short >::max)() );
    std::vector<std::size_t> parent(N);

    for (i = 0; i < N; ++i)
    	parent[i] = i;
    distance[z] = 0;

    bool r = bellman_ford_shortest_paths
    			(g, int(N), weight_map(weight_pmap).distance_map(&distance[0]).predecessor_map(&parent[0]));

    if (r)
    	for (i = 0; i < N; ++i)
    		std::cout << name[i] << ": " << std::setw(3) << distance[i]
    		      << " " << name[parent[i]] << std::endl;
    else
    	std::cout << "negative cycle" << std::endl;

    std::ofstream dot_file ("figs\bellman-eg.dot");
    dot_file << "digraph D {\n"
    		 << "  rankdir=LR\n"
    		 << "  size=\"5,3\"\n"
    		 << "  ratio=\"fill\"\n"
    		 << "  edge[style=\"bold\"]\n" << "   node[shape=\"circle\"]\n";

    {


    }

    dot_file << "}";

	return 0;
}
