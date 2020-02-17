#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

void print(const vector<vector<int>> &v)
{
	for(int i=0; i<v.size(); i++) {
		for(int j=0; j<v[i].size(); j++) {
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
}

void assign(int l, int r, const vector<int> &planks, 
	vector<vector<int>> &v, vector<int> &tmp)
{
	if(l >= r) {
		v.push_back(tmp);
		return;
	}

	for(int i=0; i<4; i++) {
		tmp[i] += planks[l];
		assign(l+1, r, planks, v, tmp);
		tmp[i] -= planks[l];
	}
}

void testcase()
{
	int n; cin >> n;

	int sum = 0;
	vector<int> planks;
	for(int i=0; i<n; i++) {
		int x; cin >> x;
		planks.push_back(x);
		sum += x;
	}

	if(sum % 4 != 0) {
		cout << "0\n"; return;
	}

	int l = sum / 4;

	vector<vector<int>> v0, v1;
	vector<int> tmp0(4, 0), tmp1(4, 0);
	assign(0, n/2, planks, v0, tmp0);
	assign(n/2, n, planks, v1, tmp1);

	sort(v1.begin(), v1.end());
	for(auto v : v1) {
		sort(v.begin(), v.end());
	}

	long long res = 0;
	vector<int> target(4);
	for(int i=0; i<v0.size(); i++) {
		for(int j=0; j<4; j++) {
			target[j] = l - v0[i][j];
		}
		sort(target.begin(), target.end());
		auto bound = equal_range(v1.begin(), v1.end(), target);
		res += bound.second - bound.first;
	}

	cout << res / 24 << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
