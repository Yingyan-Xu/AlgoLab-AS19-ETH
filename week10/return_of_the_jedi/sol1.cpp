#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator 			out_edge_it;
typedef boost::graph_traits<weighted_graph>::edge_iterator 						edge_it;


void testcase()
{
	int n, i; cin >> n >> i;

	weighted_graph G(n);
	weight_map weights = boost::get(boost::edge_weight, G);
	edge_desc e;

	vector<vector<int>> costmap(n, vector<int>(n, 0));
	for(int i=0; i<n-1; i++) {
		for(int j=i+1; j<n; j++) {
			int c; cin >> c;
			e = boost::add_edge(i, j, G).first; weights[e] = c;
			costmap[i][j] = c;
		}
	}

	vector<edge_desc> mst;
	boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

	weighted_graph MST(n);
	weight_map w_mst = boost::get(boost::edge_weight, MST);
	edge_desc e_mst;

	int old_cost = 0;
	vector<vector<bool>> inMST(n, vector<bool>(n, false));
	for (vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
	  	vertex_desc u = boost::source(*it, G);
	  	vertex_desc v = boost::target(*it, G);
	  	e_mst = boost::add_edge(u, v, MST).first; w_mst[e_mst] = weights[*it];
	  	old_cost += weights[*it];
	  	inMST[u][v] = true;
	}

	vector<vector<int>> maxCost(n, vector<int>(n, -1));
	for(int u=0; u<n; u++) {
		queue<int> Q;
		Q.push(u);
		vector<bool> visited(n, false);
		while(!Q.empty()) {
			int cur = Q.front(); Q.pop();
			visited[cur] = true;
			out_edge_it oe_beg, oe_end;
			for(boost::tie(oe_beg, oe_end) = boost::out_edges(cur, MST); oe_beg != oe_end; ++oe_beg) {
				assert(boost::source(*oe_beg, MST) == cur);
				int v = boost::target(*oe_beg, MST);
				if(!visited[v]) {
					maxCost[u][v] = max(maxCost[u][cur], w_mst[*oe_beg]);
					Q.push(v);
				}
			}
		}
	}

	int minDelta = numeric_limits<int>::max();
	edge_it e_beg, e_end;
    for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg) {
       	vertex_desc u = boost::source(*e_beg, G);
	  	vertex_desc v = boost::target(*e_beg, G);
	  	if(inMST[u][v]) continue;
	  	int curDelta = weights[*e_beg] - maxCost[u][v];
	  	assert(curDelta >= 0);
	  	minDelta = min(minDelta, curDelta);
    }

    cout << old_cost + minDelta << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
