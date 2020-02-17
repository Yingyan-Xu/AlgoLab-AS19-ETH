#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>

using namespace std;

void testcase()
{
	int n, m; cin >> n >> m;

	int maxStrength = 0;
	map<int, int, greater<int>> strength;
	for(int i=0; i<n; i++) {
		int s; cin >> s;
		maxStrength = max(maxStrength, s);
		strength[s]++;
	}
	
	int maxWeight = 0;
	map<int, int, greater<int>> weight;
	for(int i=0; i<m; i++) {
		int w; cin >> w;
		maxWeight = max(maxWeight, w);
		weight[w]++;
	}

	// cout << "strength\n";
	// for(auto m : strength) {
	// 	cout << m.first << " " << m.second << "\n";
	// }

	if(maxWeight > maxStrength) {
		cout << "impossible\n";
		return;
	}

	int n_turn = 0;
	while(m > 0) {
		// cout << "turn # " << n_turn + 1 << "\n";
		for(auto frnd : strength) {
			int strgth = frnd.first;
			int n_friend = frnd.second;
			if(n_friend == 0) continue;

			for(auto box : weight) {
				int wt = box.first;
				int n_box = box.second;

				if(wt > strgth || n_box == 0) continue;
				int dec = min(n_friend, n_box);
				// cout << "move " << dec << " boxes with weight " << wt << "\n";
				weight[wt] -= dec;
				n_friend -= dec;
				m -= dec;
			}
		}
		n_turn++;
	}

	cout << 3 * n_turn - 1 << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
