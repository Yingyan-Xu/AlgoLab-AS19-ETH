#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

int binary(const int i, const vector<pair<int, int>> &v)
{
	int cur = v[i].second;
	int l = i, r = v.size()-1;
	while(l + 1 < r) {
		int m = (l + r) / 2;
		if(v[m].first <= cur) l = m;
		else r = m;
	}
	if(v[r].first > cur) return r;
	else return v.size();
}

int solve(int i, int m, const vector<pair<int, int>> &v,
		vector<vector<int>> &memo)
{
	if(i >= v.size()) return 0;
	if(memo[i][m] != -1) return memo[i][m];

	if(m == 1) {
		int res = 0; int idx = i;
		for(; idx<v.size(); idx++) {
			res = max(res, v[idx].second - v[idx].first + 1);
		}
		memo[i][m] = res;
		return res;
	}

	int cur = v[i].second - v[i].first + 1;
	int next = binary(i, v);
	int res = max(cur + solve(next, m-1, v, memo),
				solve(i+1, m, v, memo));
	memo[i][m] = res;
	return res;
}

void testcase()
{
	int n, m, k; 
	cin >> n >> m >> k;

	vector<int> dValue;
	for(int i=0; i<n; i++) {
		int x; cin >> x;
		dValue.push_back(x);
	}

	vector<pair<int, int>> intvl;
	int l = 0, r = 0, sum = dValue[0];
	while(r < n) {
		if(sum == k) {
			intvl.push_back({l, r});
			sum -= dValue[l];
			l++; r++;
			sum += dValue[r];
		}
		else if(sum < k) {
			r++;
			sum += dValue[r];
		}
		else {
			sum -= dValue[l];
			l++; 
			if(l > r) {
				r = l;
				sum = dValue[r];
			}
		}
	}

	if(intvl.empty()) {
		cout << "fail\n";
		return;
	}

	sort(intvl.begin(), intvl.end(),
		[](const pair<int, int> &p1, const pair<int, int> &p2)->bool{
			return p1.second < p2.second;
		});

	int n_intvl = 1, end = intvl[0].second;
	for(int i=1; i<intvl.size(); i++) {
		if(intvl[i].first <= end) continue;
		end = intvl[i].second;
		n_intvl++;
	}

	if(n_intvl < m) {
		cout << "fail\n";
		return;
	}

	//memo[i][j]: max stratege value for j attacker given interval 0-i
	vector<vector<int>> memo(intvl.size(), vector<int>(m+1, -1));
	solve(0, m, intvl, memo);
	cout << solve(0, m, intvl, memo) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
