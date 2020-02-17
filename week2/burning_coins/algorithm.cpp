#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int n;
VI coins;
VVI dp;

int cdp(int i, int j)
{
	if(i > j) return 0;
	int &ans = dp[i][j];
	if(ans != -1) return ans;

	if((n-(j-i+1))%2 == 0) 
		return ans = max(cdp(i+1, j)+coins[i],
						cdp(i, j-1)+coins[j]);
	else return ans = min(cdp(i+1, j), cdp(i, j-1));
}

void testcase()
{
	cin >> n;

	coins.clear();
	coins.resize(n, 0);
	for(int i=0; i<n; i++) cin >> coins[i];

	dp.clear();
	dp.resize(n, VI(n, -1));
	cout << cdp(0, n-1) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
