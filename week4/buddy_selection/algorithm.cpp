#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int num_common(int i, int j, const vector<vector<string>>& v)
{
	int res = 0, len = v[i].size();
	int i1 = 0, i2 = 0;
	while(i1 < len && i2 < len) {
		if(v[i][i1] == v[j][i2]) {
			res++; i1++; i2++;
		}
		else if(v[i][i1] < v[j][i2]) i1++;
		else i2++;
	}

	return res;
}

void testcase()
{
	int n, c, f; cin >> n >> c >> f;

	assert(n % 2 == 0);

	vector<vector<string>> character(n);
	for(int i=0; i<n; i++) {
		for(int j=0; j<c; j++) {
			string str; cin >> str;
			character[i].push_back(str);
		}
		sort(character[i].begin(), character[i].end());
	}

	graph G(4);
	for(int i=0; i<n-1; i++) {
		for(int j=i+1; j<n; j++) {
			if(num_common(i, j, character) > f)
				boost::add_edge(i, j, G);
		}
	}

	std::vector<vertex_desc> mate_map(n);  // exterior property map

  	boost::edmonds_maximum_cardinality_matching(G,
    	boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  	int matching_size = boost::matching_size(G,
    	boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  	if(matching_size == n / 2) cout << "not optimal\n";
  	else cout << "optimal\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
