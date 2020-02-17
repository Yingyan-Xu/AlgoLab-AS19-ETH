#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

void testcase()
{
	int n; cin >> n;
	
	vector<int> bombs;
	for(int i=0; i<n; i++) {
		int h; cin >> h;
		bombs.push_back(h);
	}

	vector<int> copy(n, -1);
	copy[0] = bombs[0];
	for(int i=0; i<=(n-3)/2; i++) {
		copy[2*i+1] = min(copy[i], bombs[2*i+1]);
		copy[2*i+2] = min(copy[i], bombs[2*i+2]);
	}

	sort(copy.begin(), copy.end());

	for(int i=0; i<n; i++) {
		if(i >= copy[i]) {
			cout << "no\n"; return;
		}
	}
	cout << "yes\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
