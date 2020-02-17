#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <iomanip>

using namespace std;

int n, k, m;

double solve(int day, int money, vector<vector<double>> &dp, 
			const vector<double> &prob)
{
	if(day > n) return 0;
	if(money >= m) return 1;

	double &ans = dp[day][money]; 
	if(ans != -1) return ans;

	ans = 0;
	for(int i=0; i<=money; i++) {
		ans = max(ans, prob[day] * solve(day+1, money+i, dp, prob)
					+ (1-prob[day]) * solve(day+1, money-i, dp, prob));
	}
	return ans;
}

void testcase()
{
	cin >> n >> k >> m;

	vector<double> prob;
	for(int i=0; i<n; i++) {
		double x; cin >> x;
		prob.push_back(x);
	}

	vector<vector<double>> dp(n+1, vector<double>(m+1, -1));

	cout << solve(0, k, dp, prob) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(5);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
