//C++ STL
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

//BGL header
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property,                         // no vertex property
  boost::property<boost::edge_weight_t, int>  // interior edge weight property
> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  return dist_map[t];
}


void testcase()
{
	//n: number of vertices
	//e: number of edges
	//s: number of species
	//a: start vertex
	//b: end vertex
	int n, e, s, a, b;
	cin >> n >> e >> s >> a >> b;

	vector<weighted_graph> G(s, weighted_graph(n));
	vector<weight_map> weights(s);
	vector<int> hive(s);

	//read input
	for(int i=0; i<e; i++){
		int v1, v2;
		cin >> v1 >> v2;

		vector<int> w(s);
		edge_desc e_desc;

		for(int j=0; j<s; j++){
			cin >> w[j];
			weights[j] = boost::get(boost::edge_weight, G[j]);
			e_desc = boost::add_edge(v1, v2, G[j]).first;
			weights[j][e_desc] = w[j];
		}
	}

	for(int i=0; i<s; i++) cin >> hive[i];

	//print
	// edge_it e_beg, e_end;
	// for(int i=0; i<s; i++){
	// 	cout << i << "\n";
	// 	for (boost::tie(e_beg, e_end) = boost::edges(G[i]); e_beg != e_end; ++e_beg) {
	// 		std::cout << boost::source(*e_beg, G[i]) << " " << boost::target(*e_beg, G[i]) << " " << weights[i][*e_beg] << "\n";
	// 	}
	// }

	//find MST
	vector<vector<edge_desc>> mst(s);
	for(int i=0; i<s; i++)
		boost::kruskal_minimum_spanning_tree(G[i], back_inserter(mst[i]));


	// for(int i=0; i<s; i++){
	// 	for(vector<edge_desc>::iterator it = mst[i].begin(); it != mst[i].end(); ++it) {
	// 	    cout << boost::source(*it, G[i]) << " " << boost::target(*it, G[i]) << "\n";
	// 	}
	// }

	//construct a new graph
	weighted_graph MST_G(n);
	edge_it e_beg, e_end;
    for (boost::tie(e_beg, e_end) = boost::edges(G[0]); e_beg != e_end; ++e_beg) {
	    int min_weight = 2000000;
	    for(int i=0; i<s; i++){
	    	for(vector<edge_desc>::iterator it = mst[i].begin(); it != mst[i].end(); it++){
	    		if(boost::source(*it, G[i]) == boost::source(*e_beg, G[0]) and boost::target(*it, G[i]) == boost::target(*e_beg, G[0]))
	    			min_weight = min(min_weight, weights[i][*it]);
	    	}
	    }
	    //cout << boost::source(*e_beg, G[0]) << " " << boost::target(*e_beg, G[0]) << " " << min_weight << "\n";
	    boost::add_edge(boost::source(*e_beg, G[0]), boost::target(*e_beg, G[0]), min_weight, MST_G);
	}

	//find the shortest path
	cout << dijkstra_dist(MST_G, a, b) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; //num of test cases
    cin >> t;
    while(t--){
    	testcase();
    	//cout << "--------------\n";
    }

    return 0;
}
