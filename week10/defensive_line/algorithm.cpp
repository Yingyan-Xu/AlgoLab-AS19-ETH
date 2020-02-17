#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

const int MAX = 1000000001;
const int impossible = -2;

int n, m, k;
vector<int> defenders;
vector<int> intvls;
vector<vector<int>> dp;

int cdp(int i, int j)
{
	if(i == 0 && j > 0)  return impossible;
	if(j == 0) return 0;
	int &ans = dp[i][j];
	if(ans != -1) return ans;

	ans = impossible;
	ans = max(ans, cdp(i-1, j));

	int a = intvls[i];
	if(a != -1 && cdp(a, j-1) != impossible) {
		ans = max(ans, cdp(a, j-1) + i-a);
	}

	return ans;
}

void testcase()
{
	cin >> n >> m >> k;

	defenders.clear();
	defenders.resize(n+1, 0);
	defenders[n] = MAX;
	for(int i=0; i<n; i++) cin >> defenders[i];

	//sliding window to find all intervals
	//[a, b) -> intval[b] = a
	intvls.clear();
	intvls.resize(n+1, -1);
	int l = 0, r = 1;
	int sum = defenders[0];
	while(l < r && r <= n) {
		if(sum == k) {
			intvls[r] = l;
			sum -= defenders[l++];
			sum += defenders[r++];
		}
		else if(sum < k) {
			sum += defenders[r++];
		}
		else {
			sum -= defenders[l++];
			if(l == r)
				sum += defenders[r++];
		}
	}

	dp.clear();
	dp.resize(n+1, vector<int>(m+1, -1));

	int res = cdp(n, m);
	if(res == impossible) cout << "fail\n";
	else cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
