#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <iomanip>
#include <type_traits>
#include <set>

using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Direction_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
typedef K::Direction_2 D;

//sort the rays by y0
struct CustomCompare
{
    bool operator()(const int& x, const int& y)
    {
        return x < y;
    }

    bool operator()(const pair<int, R>& r1, const pair<int, R>& r2)
    {
        return r1.second.source().y() > r2.second.source().y();
    }
};

void testcase()
{
    int n;
    cin >> n;

    set<pair<int, R>, CustomCompare> ray;
    for(int i=0; i<n; i++){
        long y0, x1, y1;
        cin >> y0 >> x1 >> y1;
        P p1(0, y0), p2(x1, y1);
        ray.insert(make_pair(i, R(p1, p2)));
    }

    vector<bool> flag(n, true);

    //positive k
    D abs_k = D(R(P(0,0), P(0,1)));
    set<pair<int, R>, CustomCompare>::iterator it = ray.begin();
    for(it; it != ray.end(); it++){
    	D cur_k = it->second.direction();
    	D cur_abs = D(R(P(0,0), P(cur_k.dx(), abs(cur_k.dy()))));
    	if(cur_k.dy() > 0 and cur_k > abs_k) flag[it->first] = false;
    	else{
    		if(cur_abs < abs_k) abs_k = cur_abs;
    	}
    }

    //negative k
    abs_k = D(R(P(0,0), P(0,1)));
    it = ray.end(); it--;
    bool is_negative = true;
    for(it; it != ray.begin(); it--){
    	D cur_k = it->second.direction();
    	D cur_abs = D(R(P(0,0), P(cur_k.dx(), abs(cur_k.dy()))));
    	if(cur_k.dy() < 0 and cur_abs > abs_k) flag[it->first] = false;
    	else if(cur_k.dy() < 0 and (cur_abs == abs_k and !is_negative)) flag[it->first] = false;
    	else{
    		if(cur_abs <= abs_k) {
    			abs_k = cur_abs; 
    			if(cur_k.dy() < 0) is_negative = true;
    			else is_negative = false;
    		}
    	}
    }

    //once more for ray.begin()
    D cur_k = it->second.direction();
	D cur_abs = D(R(P(0,0), P(cur_k.dx(), abs(cur_k.dy()))));
	if(cur_k.dy() < 0 and cur_abs >= abs_k) flag[it->first] = false;
	else{
		if(cur_abs < abs_k) abs_k = cur_abs;
	}

    //print res
    for(int i=0; i<n; i++){
    	if(flag[i]) cout << i << " ";
    }
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;
    
    while(t--)
        testcase();
    
    return 0;
}