#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; 
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase()
{
	int Z, J; cin >> Z >> J;

	graph G(Z+J);
	edge_adder adder(G);

	const vertex_desc v_source = boost::add_vertex(G);
	const vertex_desc v_target = boost::add_vertex(G);

	for(int i=0; i<Z; i++) {
		int c; cin >> c;
		adder.add_edge(i, v_target, c);
		// cout << i << " -> " << v_target << " " << c <<  endl;
	}

	for(int i=0; i<J; i++) {
		int r; cin >> r;
		adder.add_edge(v_source, Z+i, r);
		// cout << v_source << " -> " << Z+i << " " << r <<  endl;
	}

	for(int i=0; i<J; i++) {
		int N; cin >> N;
		for(int j=0; j<N; j++) {
			int zone; cin >> zone;
			adder.add_edge(Z+i, zone, 5000);
			// cout << Z+i << " -> " << j <<  endl;
		}
	}

	long flow = boost::push_relabel_max_flow(G, v_source, v_target);

  	const auto rc_map = boost::get(boost::edge_residual_capacity, G);

  	int res = 0;
  	out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        res += rc_map[*e];     
    }

  	cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
