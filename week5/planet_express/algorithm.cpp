#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  	boost::no_property, boost::property<boost::edge_weight_t, int> > graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void testcase()
{
	int n, m, k, t;
	cin >> n >> m >> k >> t;

	vector<int> tele;
	for(int i=0; i<t; i++) {
		int x; cin >> x;
		tele.push_back(x);
	}

	graph G(n);
	weight_map weights = boost::get(boost::edge_weight, G);
	edge_desc e;

	for(int i=0; i<m; i++) {
		int u, v, c;
		cin >> u >> v >> c;
		e = boost::add_edge(v, u, G).first;
		weights[e] = c;
	}

  	vector<int> scc_map(n);
  	int nscc = boost::strong_components(G,
    	boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

  	// cout << "nscc " << nscc << endl;

  	vector<vector<int>> scc_v(nscc); //vertices in scc i
  	for(int i=0; i<t; i++) {
  		int curTele = tele[i];
  		int curScc = scc_map[curTele];
  		scc_v[curScc].push_back(curTele);
  		// cout << "tele " << curTele << " belong to " << curScc << endl;
  	}

  	int v_cnt = boost::num_vertices(G);
  	for(int i=0; i<nscc; i++) {
  		if(scc_v[i].size() <= 1) continue;
  		int weight = scc_v[i].size() - 1;
  		for(auto v : scc_v[i]) {
  			e = boost::add_edge(v, v_cnt, G).first;
  			weights[e] = weight;
  			e = boost::add_edge(v_cnt, v, G).first;
  			weights[e] = 0;
  		}
  		v_cnt++;
  	}
  	// cout << "#vertices " << v_cnt << endl;

  	vector<int> dist_map(v_cnt);
  	boost::dijkstra_shortest_paths(G, n-1,
    	boost::distance_map(boost::make_iterator_property_map(
      		dist_map.begin(), boost::get(boost::vertex_index, G))));

  	int res = numeric_limits<int>::max();
  	for(int i=0; i<k; i++) {
  		// cout << "dist to " << i << " " << dist_map[i] << endl;
  		res = min(res, dist_map[i]);
  	}

  	if(res <= pow(10, 6)) cout << res << "\n";
  	else cout << "no\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
