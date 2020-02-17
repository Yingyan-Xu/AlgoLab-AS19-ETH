#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

using namespace std;

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

// Custom edge adder class, highly recommended
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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase()
{
	int n, m; cin >> n >> m;

	graph G(n);
	edge_adder adder(G);
	auto rc_map = boost::get(boost::edge_residual_capacity, G);

	for(int i=0; i<m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		adder.add_edge(a, b, c);
	}

	long long flow = numeric_limits<long>::max();
	int src, trg;
	for(int i=0; i<n; i++) {
		long long cur = boost::push_relabel_max_flow(G, i, (i+1)%n);
		if(cur < flow) {flow = cur, src = i, trg = (i+1)%n;}
	}

	flow = boost::push_relabel_max_flow(G, src, trg);
	vector<int> vis(n, false); 
	queue<int> Q; 
	vis[src] = true; 
	set<int> res;
	res.insert(src);
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			if (rc_map[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			res.insert(v);
			Q.push(v);
		}
	}

	cout << flow << "\n" << res.size();
	for(int v : res) cout << " " << v;
	cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
