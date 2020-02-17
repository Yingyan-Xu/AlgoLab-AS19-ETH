#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

const int MAXN = 1000 + 2;
const int MAX = 100 * 1000 + 2;

int n;
int vol[MAXN];
int wei[MAXN];
int dp[MAXN][MAXN];

int cdp(int i, int j)
{
	if(i == 0 && j == 0) return 0;
	else if(i == 0 || j == 0) return MAX;
	int &ans = dp[i][j];
	if(ans != -1) return ans;

	ans = min((vol[i]-1)*(wei[j]-1)+cdp(i-1, j-1),
			min((vol[i]-1)*(wei[j]-1)+cdp(i, j-1),
				(vol[i]-1)*(wei[j]-1)+cdp(i-1, j)));

	return ans;
}

void testcase()
{
	cin >> n;

	for(int i=1; i<=n; i++) cin >> vol[i];
	for(int i=1; i<=n; i++) cin >> wei[i];

	for(int i=0; i<=n; i++)
		for(int j=0; j<=n; j++)
			dp[i][j] = -1;

	cout << cdp(n, n) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
