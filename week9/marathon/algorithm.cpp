#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = capacity; 
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   
    w_map[rev_e] = cost;  
  }
};

void testcase()
{
	int n, m, s, f;
	cin >> n >> m >> s >> f;

	graph G(n);
	edge_adder adder(G);
	auto c_map = boost::get(boost::edge_capacity, G);
	auto w_map = boost::get(boost::edge_weight, G);

	for(int i=0; i<m; i++) {
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		adder.add_edge(a, b, c, d);
	}

	vector<long> dist_map(n);

  	boost::dijkstra_shortest_paths(G, f,
    	boost::distance_map(boost::make_iterator_property_map(
      		dist_map.begin(), boost::get(boost::vertex_index, G))));

	vector<int> vis(n, false); 
	queue<int> Q; 
	vis[s] = true; 
	Q.push(s);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		long cur_dist = dist_map[u];
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			long next_dist = dist_map[v];
			long edgeLen = w_map[*ebeg];
			if(next_dist + edgeLen > cur_dist) c_map[*ebeg] = 0;

			if (vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

	long flow = boost::push_relabel_max_flow(G, s, f);
	cout << flow << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
