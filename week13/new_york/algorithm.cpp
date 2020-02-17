#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <set>

using namespace std;
int len, risk;

void dfs(int start, int level, 
    vector<int> &path, multiset<int> &curTemp, set<int> &res,
    const vector<vector<int>> &edges, const vector<int> &temp)
{

    path[level] = start;
    // cout << level << endl;

    int idx = -1;
    if(curTemp.size() == len) {
        int low = *curTemp.begin();
        int high = *curTemp.rbegin();
        idx = path[level-len+1];
        if(high - low <= risk) res.insert(idx);
        curTemp.erase(curTemp.lower_bound(temp[idx]));
    }
    for(auto next : edges[start]) {
        curTemp.insert(temp[next]);
        dfs(next, level+1, path, curTemp, res, edges, temp);
    }

    if(idx != -1) curTemp.insert(temp[idx]);
    curTemp.erase(curTemp.lower_bound(temp[start]));
}

void testcase()
{
    int n;//#point, length, risk
    cin >> n >> len >> risk;
    // cout << "n " << n << endl;

    vector<int> temp;
    for(int i=0; i<n; i++) {
        int h; cin >> h;
        temp.push_back(h);
    }

    vector<vector<int>> edges(n);
    for(int i=0; i<n-1; i++) {
        int u, v; cin >> u >> v;
        edges[u].push_back(v);
    }

    multiset<int> curTemp;
    set<int> res;
    vector<int> path(n);
    curTemp.insert(temp[0]);
    dfs(0, 0, path, curTemp, res, edges, temp);

    if(!res.empty()) {
        for(auto r : res)
            cout << r << " ";
    }
    else cout << "Abort mission";
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
