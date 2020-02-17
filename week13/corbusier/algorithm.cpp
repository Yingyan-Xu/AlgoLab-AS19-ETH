#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <set>

using namespace std;

const int MAXN = 1000 + 2;

int n, idx, k; 
int height[MAXN];

void assign(int start, int end, long &cur, set<int> &sum)
{
	if(start == end) {
		sum.insert(cur % k); return;
	}

	cur += height[start];
	assign(start+1, end, cur, sum);
	cur -= height[start];
	if(cur != 0) assign(start+1, end, cur, sum);
}

void testcase()
{
	cin >> n >> idx >> k;

	for(int i=0; i<n; i++) cin >> height[i];

	long cur = 0, cur2 = 0;
	set<int> sum, sum2;
	assign(0, n/2, cur, sum);
	assign(n/2, n, cur2, sum2);
	for(int half : sum) {
		int target = (idx + k - half) % k;
		auto it = sum2.find(target);
		if(it != sum2.end()) {
			cout << "yes\n"; return;
		}
	}

	cout << "no\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
