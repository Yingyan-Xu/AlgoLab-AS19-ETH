#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

struct boat
{
	int l, p;
	boat(int l, int p) : l(l), p(p) {}
};

void testcase()
{
	int n; cin >> n;

	vector<boat> v;
	for(int i=0; i<n; i++) {
		int l, p; cin >> l >> p;
		v.push_back(boat(l, p));
	}

	sort(v.begin(), v.end(), 
		[](const boat &b1, const boat &b2)->bool {
			return b1.p < b2.p;
		});

	int end = v[0].p;
	int num = 1;
	int i = 1;
	while(i < n) {
		if(end > v[i].p) {i++; continue;}
		if(v[i].p - v[i].l >= end) {
			end = v[i].p;
			num++; i++;
			continue;
		} 
		int tmp_end = end + v[i].l;
		int next = i + 1;
		for(int j=i+1; j<n; j++) {
			int cur;
			if(v[j].p - v[j].l >= end) cur = v[j].p;
			else cur = end + v[j].l;
			if(cur < tmp_end) {
				tmp_end = cur;
				next = j + 1;
			}
		}
		end = tmp_end;
		i = next;
		num++;
	}

	cout << num << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
