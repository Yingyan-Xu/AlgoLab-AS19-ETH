#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

int kruskal(const int n, const vector<tuple<int, int, int>> &edges,
		vector<pair<int, int>> &mst, const pair<int, int> &p)
{
	boost::disjoint_sets_with_storage<> uf(n);
	int component = n;
	int len = 0;
	for(auto t : edges) {
		int d = get<0>(t);
		int u = get<1>(t);
		int v = get<2>(t);
		if(uf.find_set(u) != uf.find_set(v) && make_pair(u, v) != p) {
			uf.link(u, v);
			len += d;
			mst.push_back({u, v});
			if(--component == 1) break;
		} 
	}

	assert(component == 1);
	return len;
}

void testcase()
{
	int n, idx; cin >> n >> idx;

	vector<tuple<int, int, int>> edges;
	for(int i=0; i<n-1; i++) {
		for(int j=i+1; j<n; j++) {
			int x; cin >> x;
			edges.push_back(make_tuple(x, i, j));
		}
	}
	sort(edges.begin(), edges.end());

	vector<pair<int, int>> mst;
	kruskal(n, edges, mst, {-1, -1});

	int res = numeric_limits<int>::max();
	vector<pair<int, int>> _;
	for(auto p : mst) {
		int cur = kruskal(n, edges, _, p);
		res = min(cur, res);
	}

	cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
