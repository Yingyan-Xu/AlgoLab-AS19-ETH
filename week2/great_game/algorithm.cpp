#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int n, m, r, b; 
VVI dp_max, dp_min;

int solve_min(int idx, int pos, const VVI &trans);

int solve_max(int idx, int pos, const VVI &trans)
{
	int &ans = dp_max[idx][pos];
	if(ans != -1) return ans;
	if(pos == trans.size()-1) return ans = 0;

	ans = 0;
	for(int next : trans[pos]) {
		ans = max(ans, 1+solve_min(idx, next, trans));
	}
	return ans;
}

int solve_min(int idx, int pos, const VVI &trans)
{
	int &ans = dp_min[idx][pos];
	if(ans != -1) return ans;
	if(pos == trans.size()-1) return ans = 0;

	ans = trans.size();
	for(int next : trans[pos]) {
		ans = min(ans, 1+solve_max(idx, next, trans));
	}
	return ans;
}

void testcase()
{
	cin >> n >> m;

	cin >> r >> b;
	r--; b--;

	VVI trans(n);
	for(int i=0; i<m; i++) {
		int u, v; cin >> u >> v;
		u--; v--;
		trans[u].push_back(v);
	}

	dp_max.clear();
	dp_max.resize(2, VI(n, -1));
	dp_min.clear();
	dp_min.resize(2, VI(n, -1));

	int sherlock = solve_min(0, r, trans);
	int moriarty = solve_min(1, b, trans);

	if(sherlock < moriarty) cout << 0 << endl;
	else if(sherlock > moriarty) cout << 1 << endl;
	else if(sherlock == moriarty && sherlock % 2 == 1)
		cout << 0 << endl;
	else cout << 1 << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
