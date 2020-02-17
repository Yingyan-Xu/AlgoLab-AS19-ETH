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
	int n; cin >> n;

	vector<int> freq(n);
	for(int i=0; i<n; i++) {
		cin >> freq[i];
	}

	map<int, int> lookup;
	vector<int> position;
	for(int i=0; i<n; i++) {
		for(int j=0; j<freq[i]; j++) {
			int pos; cin >> pos;
			lookup[pos] = i;
			position.push_back(pos);
		}
	}
	sort(position.begin(), position.end());

	vector<int> covered(n, 0);
	int numCovered = 1;
	covered[lookup[position[0]]]++;
	int left = 0, right = 0;
	int res = numeric_limits<int>::max();
	while(left < position.size()) {
		while(numCovered < n && right < position.size()) {
			right++;
			if(++covered[lookup[position[right]]] == 1)
				numCovered++;
		}
		if(numCovered < n) break;
		while(left < right && numCovered == n) {
			if(--covered[lookup[position[left]]] == 0)
				numCovered--;
			left++;
		}
		if(right < position.size() && left-1 >= 0) 
			res = min(res, position[right] - position[left-1] + 1);
	}

	cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
