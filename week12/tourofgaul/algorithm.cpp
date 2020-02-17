#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int MAX = 128;

void testcase()
{
	int n, m; cin >> n >> m;

	graph G(n);
	edge_adder adder(G);

	const vertex_desc v_source = boost::add_vertex(G);
	const vertex_desc v_target = boost::add_vertex(G);

	for(int i=0; i<n-1; i++) {
		int c; cin >> c;
		adder.add_edge(i, i+1, c, MAX);
		adder.add_edge(v_source, i, c, 0);
		adder.add_edge(i+1, v_target, c, 0);
	}

	map<tuple<int, int, int>, int> lookup;
	for(int i=0; i<m; i++) {
		int a, b, d;
		cin >> a >> b >> d;
		if(lookup.count(make_tuple(a, b, d)) != 0) {
			lookup[make_tuple(a, b, d)]++;
		}
		else {
			lookup[make_tuple(a, b, d)] = 1;
		}
	}

	for(auto edge : lookup) {
		int a = get<0>(edge.first);
		int b = get<1>(edge.first);
		int c = edge.second;
		int d = get<2>(edge.first);
		adder.add_edge(a, b, c, MAX*(b-a)-d);
	}

	auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

	boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];     
    }

    cout << flow * MAX - cost << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
