#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <cmath>

using namespace std;

void testcase()
{
	int n; cin >> n;

	vector<int> parasols;
	for(int i=0; i<n; i++) {
		int x; cin >> x;
		parasols.push_back(x);
	}
	sort(parasols.begin(), parasols.end());

	vector<int>::iterator itb = parasols.begin();
	vector<int>::iterator ite = parasols.begin();
	vector<tuple<int, int, int>> res;
	while(ite != parasols.end()) {
		bool flag = false;
		while(*ite - *itb <= 200 && 
			ite != parasols.end()) {ite++; flag = true;}
		if(flag) ite--;
		int cur = ite - itb + 1;
		int dist = ceil((*ite - *itb) / 2.0);
		int coord1 = *itb + dist;
		int coord2 = *ite - dist;
		// cout << "cur left ptr " << *itb << " right ptr " << *ite << endl;
		// cout << "cur num " << cur << " max dist " << dist << " coord " << coord1 << " " << coord2 << endl;
 		if(coord1 == coord2) res.push_back({cur, dist, coord1});
		else {
			res.push_back({cur, dist, coord1});
			res.push_back({cur, dist, coord2});
		}
		if(ite != parasols.end()) ite++;
		while(*ite - *itb > 200) itb++;
	}

	sort(res.begin(), res.end(), 
		[](tuple<int, int, int> &t1, tuple<int, int, int> &t2)->bool{
			if(get<0>(t1) == get<0>(t2)) return get<1>(t1) < get<1>(t2);
			else return get<0>(t1) > get<0>(t2);
		});

	int maxNum = get<0>(*res.begin());
	int minDist = get<1>(*res.begin());
	cout << maxNum << " " << minDist << endl;

	vector<int> coord;
	for(int i=0; i<n; i++) {
		int curNum = get<0>(res[i]);
		int curDist = get<1>(res[i]);
		if(curNum == maxNum && curDist == minDist)
			coord.push_back(get<2>(res[i]));
		else break;
	}
	sort(coord.begin(), coord.end());
	for(int i=0; i<coord.size(); i++) {
		cout << coord[i];
		if(i != coord.size()-1)  cout << " ";
	}
	cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
