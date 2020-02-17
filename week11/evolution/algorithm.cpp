#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>
#include <string>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int binary(const VI &path, const VI &ages, int target)
{
	int l = -1, r = path.size() - 1;
	while(l + 1 < r) {
		int m = (l + r) / 2;
		int curAge = ages[path[m]];
		if(target >= curAge) r = m;
		else l = m;
	}
	return path[r];
}

void dfs(int luca, const VVI &offsprings, VI &path, const VI &ages,
		const vector<vector<pair<int, int>>> &query, VI &res)
{
	path.push_back(luca);
	// cout << "cur sps " << luca << endl;

	for(int i=0; i<query[luca].size(); i++) {
		res[query[luca][i].second] = binary(path, ages, query[luca][i].first);
		// cout << "res " << res[query[luca][i].second] << endl;
	}

	if(!offsprings[luca].empty()) {
		for(auto off : offsprings[luca]) {
			dfs(off, offsprings, path, ages, query, res);
			path.pop_back();
		}
	}
}

void testcase()
{
	int n, q; cin >> n >> q;

	int luca; int maxAge = 0;
	map<string, int> sps_to_idx;
	vector<string> idx_to_name(n);
	VI ages;
	for(int i=0; i<n; i++){
		string name; int age;
		cin >> name >> age;
		sps_to_idx[name] = i;
		idx_to_name[i] = name;
		ages.push_back(age);
		if(age > maxAge) {
			maxAge = age;
			luca = i;
		}
	}

	VVI offsprings(n);
	for(int i=0; i<n-1; i++) {
		string s, p; cin >> s >> p;
		offsprings[sps_to_idx[p]].push_back(sps_to_idx[s]);
	}

	vector<vector<pair<int, int>>> query(n);
	for(int i=0; i<q; i++) {
		string name; int age;
		cin >> name >> age;
		query[sps_to_idx[name]].push_back({age, i});
	}

	VI res(q), path;
	dfs(luca, offsprings, path, ages, query, res);

	for(int i=0; i<q; i++) {
		if(i != q-1) cout << idx_to_name[res[i]] << " ";
		else cout << idx_to_name[res[i]] << "\n";
	}
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
