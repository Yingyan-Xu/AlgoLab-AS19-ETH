#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#define MAX 1000000001

typedef vector<int> VI;
typedef vector<VI> VVI;

VVI dp;

void testcase()
{
	int n, m; long k;
	cin >> n >> m >> k;

	VI defender(n+1);
	defender[n] = MAX;
	for(int i=0; i<n; i++) {
		int x; cin >> x;
		defender[i] = x;
	}

	//sliding window to find all intervals
	//if [a,b) is a valid intvl, intvl[b] = a
	//else intvl[b] = -1;
	VI intvl(n+1, -1);
	int l = 0, r = 1;
	int cnt = 0;
	int sum = defender[0];
	while(l < r && r <= n) {
		if(sum == k) {
			intvl[r] = l;
			sum -= defender[l++];
			sum += defender[r++];
			cnt++;
		}
		else if(sum < k) {
			sum += defender[r++];
		}
		else {
			sum -= defender[l++];
			if(l >= r)
				sum += defender[r++];
		}
	}

	if(cnt < m) {
		cout << "fail\n"; return;
	}

	//dp[i][j]: max number of attacked defenders
	//with i attackers and j defenders
	dp.clear();
	dp.resize(m+1, VI(n+1, -1));

	//base case: no attackers
	for(int i=0; i<=n; i++) dp[0][i] = 0;

	for(int i=1; i<=m; i++) {
		for(int j=1; j<=n; j++) {
			dp[i][j] = dp[i][j-1];
			int a = intvl[j];

			if(a != -1) {
				//no valid solution for i-1 attackers with a defenders
				if(dp[i-1][a] == -1) continue;
				int c = j - a;
				dp[i][j] = max(dp[i][j], c + dp[i-1][a]);
			}
		}
		//no valid solution for i attackers already
		if(dp[i][n] == -1) break;
	}

	if(dp[m][n] == -1) cout << "fail\n";
	else cout << dp[m][n] << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
