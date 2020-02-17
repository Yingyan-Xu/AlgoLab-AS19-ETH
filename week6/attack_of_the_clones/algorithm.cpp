#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

using namespace std;

void testcase()
{
    int n; long long m; //#Jedi, #seg
    cin >> n >> m;
    // cout << "n " << n << " m " << m << endl;

    vector<pair<long long, long long>> seg;
    for(int i=0; i<n; i++) {
        long long a, b; cin >> a >> b;
        a--; b--;
        if(a > b) b += m;
        seg.push_back({a, b});
        seg.push_back({a+m, b+m});
    }

    sort(seg.begin(), seg.end(), 
        [](const pair<long long, long long>& p1, const pair<long long, long long>& p2)->bool{
            if(p1.second == p2.second) return p1.first > p2.first;
            return p1.second < p2.second;
        });

    // for(auto p : seg) {
    //     cout << p.first << " " << p.second << "\n";
    // }
    // cout << "--------------\n";

    int res = 1;
    int cur = 1;
    long long end = seg[0].second;
    queue<pair<long long, long long>> Q;
    // cout << "in queue: " << seg[0].first << " " << seg[0].second << "\n";
    Q.push(seg[0]);
    for(int i=1; i<2*n; i++) {
        if(seg[i].first <= end) continue;
        Q.push(seg[i]);
        // cout << "in queue: " << seg[i].first << " " << seg[i].second << "\n";
        cur++;
        end = seg[i].second;
        auto jedi = Q.front();
        long long start = jedi.first;
        bool conflict = (end - m >= start && end % m >= start % m)
                    || (end - start >= m);
        if(conflict) {
            Q.pop();
            // cout << "out queue: " << jedi.first << " " << jedi.second << "\n";
            cur--;
        }
        res = max(res, cur);
    }

    cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
