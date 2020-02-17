#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
	int n, m, s; //num of vertices, edges, stores
	cin >> n >> m >> s;

	graph G(n);
	edge_adder adder(G);

	//record num of stores in each vertex
	vector<int> num_store(n, 0);
	for(int i=0; i<s; i++){
		int store;
		cin >> store;
		num_store[store]++;
	}

	for(int i=0; i<m; i++){
		int a, b;
		cin >> a >> b;
		adder.add_edge(a, b, 1);
		adder.add_edge(b, a, 1);
	}

	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_target = boost::add_vertex(G);
  	adder.add_edge(v_source, 0, s);
  	for(int i=0; i<n; i++){
  		if(num_store[i] > 0)
  			adder.add_edge(i, v_target, num_store[i]);
  	}

  	long flow = boost::push_relabel_max_flow(G, v_source, v_target);

  	if(flow == s) cout << "yes\n";
  	else cout << "no\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; //num of testcases
    cin >> t;

    while(t--) testcase();

    return 0;
}
