#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

void print(const vector<int> &v)
{
	for(auto i : v) cout << i << " ";
	cout << endl;
}

void printVVI(const vector<vector<int>> &v) {
	for(int i=0; i<v.size(); i++) {
		for(auto j : v[i]) cout << j << " ";
		cout << endl;
	}
	cout << "-------------\n";
}

void assign(int start, int end, int &num, vector<int> &cur,
			const vector<vector<int>> &on, const vector<vector<int>> &off,
			vector< pair<vector<int>, int> > &v)
{
	if(start >= end) {
		v.push_back(make_pair(cur, num));
		return;
	}

	int m = cur.size();
	for(int i=0; i<m; i++)
		cur[i] += on[start][i];
	assign(start+1, end, num, cur, on, off, v);
	for(int i=0; i<m; i++)
		cur[i] = cur[i] - on[start][i] + off[start][i];
	num++;
	assign(start+1, end, num, cur, on, off, v);
	num--;
	for(int i=0; i<m; i++)
		cur[i] -= off[start][i];
}

void testcase()
{
	//#switch, room
	int n, m; cin >> n >> m;

	vector<int> target;
	for(int i=0; i<m; i++) {
		int b; cin >> b;
		target.push_back(b);
	}

	vector<vector<int>> on(n, vector<int>(m, 0));
	vector<vector<int>> off(n, vector<int>(m, 0));	
	for(int i=0; i<n; i++) {
		for(int j=0; j<m; j++) {
			cin >> on[i][j] >> off[i][j];
		}
	}

	int num = 0, num2 = 0;
	vector< pair<vector<int>, int> > v, v2;
	vector<int> cur(m, 0), cur2(m, 0);
	assign(0, n/2, num, cur, on, off, v);
	assign(n/2, n, num2, cur2, on, off, v2);

	sort(v2.begin(), v2.end());

	vector<int> curHalf(m);
	int res = 101;
	for(int i=0; i<v.size(); i++) {
		for(int j=0; j<m; j++) {
			curHalf[j] = target[j] - v[i].first[j];
		}
		auto it = lower_bound(v2.begin(), v2.end(), make_pair(curHalf, 0));
		if(it->first == curHalf) 
			res = min(res, v[i].second + it->second);
	}

	if(res > n) cout << "impossible\n";
	else cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
