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
	int n, m, k, l;
	cin >> n >> m >> k >> l;

	vector<int> police(n, 0);
	for(int i=0; i<k; i++) {
		int x; cin >> x;
		police[x]++;
	}

	vector<int> photo(n, 0);
	for(int i=0; i<l; i++) {
		int x; cin >> x;
		photo[x]++;
	}

	graph G(2 * n);
	edge_adder adder(G);

	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_target = boost::add_vertex(G);

  	int res = 0;
  	for(int i=0; i<n; i++) {
  		int tmp = min(police[i], photo[i]);
  		res += tmp;
  		police[i] -= tmp;
  		photo[i] -= tmp;

  		if(police[i] > 0) {
  			adder.add_edge(v_source, i, police[i]);
  			adder.add_edge(n+i, v_target, police[i]);
  		}

  		if(photo[i] > 0) {
  			adder.add_edge(i, n+i, photo[i]);
  		}
  	}

  	for(int i=0; i<m; i++) {
  		int u, v; cin >> u >> v;
  		adder.add_edge(u, v, 500);
  		adder.add_edge(n+u, n+v, 1);
  	}

  	long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  	cout << res + flow << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
