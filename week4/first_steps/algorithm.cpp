#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property,                         
  boost::property<boost::edge_weight_t, int>  
> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

int kruskal(const weighted_graph &G) {
  vector<edge_desc> mst;

  boost::kruskal_minimum_spanning_tree(G, back_inserter(mst));

  int res = 0;
  for (vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    res += boost::get(boost::edge_weight_t(), G, *it);
  }
  return res; 
}


int max_dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int dmax = 0;
  for(int i=0; i<n; i++) dmax = max(dmax, dist_map[i]);
  return dmax;
}

void testcase()
{
	int vertex, edge; //num of vertices, edges
	cin >> vertex >> edge;

	weighted_graph G(vertex);
	weight_map weights = boost::get(boost::edge_weight, G);

	edge_desc e;
	for(int i=0; i<edge; i++){
		int v1, v2, w; //1st vertex, 2 vertex, weight
		cin >> v1 >> v2 >> w;
		e = boost::add_edge(v1, v2, G).first; weights[e]=w;
	}

	//print
	/*edge_it e_beg, e_end;
	for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg) {
       cout << boost::source(*e_beg, G) << " " << boost::target(*e_beg, G) << " " << weights[*e_beg] << "\n";
    }
    */

    cout << kruskal(G) << " " << max_dijkstra_dist(G, 0) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; //num of test cases;
    cin >> t;
    while(t--)
    	testcase();

    return 0;
}
