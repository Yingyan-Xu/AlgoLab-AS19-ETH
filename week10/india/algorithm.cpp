#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

// BGL includes
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

int INF = numeric_limits<int>::max();

void testcase()
{
	int c, g, b, k, a;
	cin >> c >> g >> b >> k >> a;

	graph G(c);
	edge_adder adder(G);

	for(int i=0; i<g; i++) {
		int x, y, d, e;
		cin >> x >> y >> d >> e;
		adder.add_edge(x, y, e, d);
	}

	const vertex_desc v_target = boost::add_vertex(G);
	adder.add_edge(a, v_target, INF, 0);

	auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
	boost::successive_shortest_path_nonnegative_weights(G, k, v_target);
    int initCost = boost::find_flow_cost(G);
    int initFlow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(k,G), G); e != eend; ++e) {
        initFlow += c_map[*e] - rc_map[*e];     
    }

    if(initCost <= b) {
    	cout << initFlow << "\n"; return;
    }

    int l = 0, r = initFlow;
    while(l + 1 < r) {
    	int m = (l + r) / 2;
    	edge_desc edge = boost::edge(a, v_target, G).first;
    	c_map[edge] = m;
    	boost::successive_shortest_path_nonnegative_weights(G, k, v_target);
    	int curCost = boost::find_flow_cost(G);
    	if(curCost <= b) l = m;
    	else r = m;
    }

    cout << l << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
