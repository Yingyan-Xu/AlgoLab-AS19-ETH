#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<graph>::vertex_descriptor		vertex_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator		out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase()
{
	int g, s, l;
	cin >> g >> s >> l;

	graph G(g+s);
	edge_adder adder(G);
	auto rc_map = boost::get(boost::edge_residual_capacity, G);

	const vertex_desc v_source = boost::add_vertex(G);
	const vertex_desc v_target = boost::add_vertex(G);

	for(int i=0; i<g; i++) {
		adder.add_edge(v_source, i, 1);
	}

	for(int j=0; j<s; j++) {
		adder.add_edge(g+j, v_target, 1);
	}

	for(int i=0; i<l; i++) {
		int u, v; cin >> u >> v;
		adder.add_edge(u, g+v, 1);
	}

	int flow = boost::push_relabel_max_flow(G, v_source, v_target);

	vector<int> vis(g+s+2, false); 
	queue<int> Q; 
	vis[v_source] = true; 
	Q.push(v_source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			if (rc_map[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

	int n_g = 0, n_s = 0;
	vector<int> v_g, v_s;
	for(int i=0; i<g; i++) {
		if(!vis[i]) {
			n_g++; v_g.push_back(i);
		}
	}

	for(int i=0; i<s; i++) {
		if(vis[g+i]) {
			n_s++; v_s.push_back(i);
		}
	}

	cout << n_g << " " << n_s << "\n";
	for(int i=0; i<v_g.size(); i++) cout << v_g[i] << " ";
	for(int i=0; i<v_s.size(); i++) cout << v_s[i] << " ";
	if(n_g != 0 || n_s != 0) cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
