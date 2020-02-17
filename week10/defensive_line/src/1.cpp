#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

int binary(int idx, const vector<pair<int, int>>& v)
{
	int l = idx;
	int r = v.size() - 1;
	while(l + 1 < r) {
		int m = (l + r) / 2;
		if(v[m].first <= v[idx].second) l = m;
		else r = m;
	}
	return r;
}

int dp(int idx, int m, const vector<pair<int, int>>& v, vector<vector<int>>& memo)
{
	if(memo[idx][m] != -1) return memo[idx][m];

	if(m == 0 || v.size() - idx < m) return 0;

	int cur = v[idx].second - v[idx].first + 1;
	if(m == 1) {
		int res = cur;
		if(idx + 1 < v.size()) {
			res = max(res, dp(idx+1, 1, v, memo));
		}
		memo[idx][m] = res;
		return res;
	}

	int res = 0;
	int i = binary(idx, v);
	if(v[i].first > v[idx].second) 
		res = max(res, cur + dp(i, m-1, v, memo));
	res = max(res, dp(idx + 1, m, v, memo));

	memo[idx][m] = res;
	return res;
}

void testcase()
{
	int n, m; //#defender, #attacker
	cin >> n >> m;
	long k; //attack strength
	cin >> k;

	vector<int> defender;
	for(int i=0; i<n; i++) {
		int v; cin >> v;
		defender.push_back(v);
	}

	//sliding window to find all intervals with sum=k
	int l = 0, r = 0, sum = defender[0];
	vector<pair<int, int>> interval;
	while(l <= r && r < n) {
		if(sum == k) {
			interval.push_back({l,r});
			sum -= defender[l]; l++;
			r++; if(r < n) sum += defender[r];
		}
		else if(sum > k) {
			sum -= defender[l]; l++;
			if(l > r) {
				r = l;
				if(r < n) sum += defender[r];
			}
		}
		else {
			r++;
			if(r < n) sum += defender[r];
		}
	}

	if(m > interval.size()) {
		cout << "fail\n"; return;
	}

	//greedy to decide fail or not
	vector<pair<int, int>> sorted;
	copy(interval.begin(), interval.end(), back_inserter(sorted));
	sort(sorted.begin(), sorted.end(), 
		[](const pair<int, int>& p1, const pair<int, int>& p2) -> bool {
			return p1.second < p2.second;
		});
	int n_itvl = 1;
	int end = sorted[0].second;
	for(int i=1; i<sorted.size(); i++) {
		if(sorted[i].first <= end) continue;
		n_itvl++;
		end = sorted[i].second;
	}
	if(n_itvl < m) {
		cout << "fail\n";
		return;
	}

	//top-down dp to get maximum
	//memo[i][j]: start from interval at index i, maximum num of defenders given m attackers
	vector<vector<int>> memo(interval.size(), vector<int>(m+1, -1));
	int res = dp(0, m, interval, memo);

	cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
