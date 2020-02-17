#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

typedef pair<int, int> PII;

const int MAXN = 1000 + 10;
const int MAXK = 4 * 1000 + 10;
vector<PII> edges[MAXN];
long dp[MAXN][MAXK];

long cdp(int i, int j)
{
	if(j == 0) return 0;
	long &ans = dp[i][j];
	if(ans != -1) return ans;

	if(edges[i].size() == 0) 
		return ans = cdp(0, j);

	ans = 0;
	for(int ii=0; ii<edges[i].size(); ii++) {
		int v = edges[i][ii].first;
		long p = edges[i][ii].second;
		ans = max(ans, p+cdp(v,j-1));
	}

	return ans;
}

void testcase()
{
	int n, m, k; long x;
	cin >> n >> m >> x >> k;

	for(int i=0; i<n; i++) edges[i].clear();
	for(int i=0; i<m; i++) {
		int u, v, p;
		cin >> u >> v >> p;
		edges[u].push_back({v, p});
	}

	for(int i=0; i<n; i++)
		for(int j=0; j<=k; j++)
			dp[i][j] = -1;

	for(int i=1; i<=k; i++) {
		if(cdp(0, i) >= x) {
			cout << i <<"\n"; return;
		}
	}

	cout << "Impossible\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
