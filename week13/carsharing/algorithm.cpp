#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>

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

int INF = numeric_limits<int>::max();

struct req
{
	int s, t, d, a, p;
	req(int s, int t, int d, int a, int p) : 
		s(s), t(t), d(d), a(a), p(p) {}
};

void testcase()
{
	int n, s; cin >> n >> s;

	vector<int> init;
	for(int i=0; i<s; i++) {
		int x; cin >> x;
		init.push_back(x);
	}

	vector<req> booking;
	vector<set<int>> times(s);
	for(int i=0; i<n; i++) {
		int s, t, d, a, p;
		cin >> s >> t >> d >> a >> p;
		booking.push_back(req(s-1, t-1, d, a, p));
		times[s-1].insert(d);
		times[t-1].insert(a);
	}

	for(int i=0; i<s; i++) {
		times[i].insert(0);
		times[i].insert(100000);
	}

	int v_idx = 0;
	map<pair<int, int>, int> to_idx;
	for(int i=0; i<s; i++) {
		for(auto time : times[i]) {
			to_idx[{i, time}] = v_idx;
			v_idx++;
		}
	}

	graph G(v_idx);
	edge_adder adder(G);
	auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

	const vertex_desc v_source = boost::add_vertex(G);
	const vertex_desc v_target = boost::add_vertex(G);

	for(int i=0; i<s; i++) {
		adder.add_edge(v_source, to_idx[{i, 0}], init[i], 0);
		adder.add_edge(to_idx[{i, 100000}], v_target, INF, 0);
		for(auto it=times[i].begin(); it!=times[i].end();) {
			int curT = *it; 
			int curV = to_idx[{i, curT}];
			it++;
			if(it != times[i].end()) {
				int nextT = *it; 
				int nextV = to_idx[{i, nextT}];
				adder.add_edge(curV, nextV, INF, 100*(nextT-curT));
				// cout << "weight/100 " << 100*(nextT-curT) << endl;
			}
		}
	}

	for(int i=0; i<n; i++) {
		int s = booking[i].s;
		int t = booking[i].t;
		int gap = booking[i].a - booking[i].d;
		int u = to_idx[{s, booking[i].d}];
		int v = to_idx[{t, booking[i].a}];
		adder.add_edge(u, v, 1, gap*100 - booking[i].p);
		// cout << "gap " << gap << " price " << booking[i].p << endl;
		// cout << "weight/100-price " << gap*100 - booking[i].p << endl;
	}

	boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];     
    }
    // cout << "flow " << flow << endl;
    // cout << "cost " << cost << endl;

    cout << 100 * 100000 * flow - cost << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
