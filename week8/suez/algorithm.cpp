#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

double ceil_to_double(const SolT& x)
{
    double a = ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void testcase()
{
	cout << setiosflags(ios::fixed) << setprecision(0);
	
	//n: num of new posters
	//m: num of old posters
	//h: height
	//w: width
	int n, m, h, w;
	cin >> n >> m >> h >> w;
	
	vector<pair<int, int>> new_poster;
	for(int i=0; i<n; i++){
		long x, y; cin >> x >> y;
		new_poster.push_back({x, y});
	}

	vector<pair<int, int>> old_poster;
	for(int i=0; i<m; i++){
		long x, y; cin >> x >> y;
		old_poster.push_back({x, y});
	}

	Program lp (CGAL::SMALLER, true, 1, false, 0);

	int nr = 0; //record number of constraints
	for(int i=0; i<n; i++){
		long x1 = new_poster[i].first;
		long y1 = new_poster[i].second;
		//constraints with other new posters
		for(int ii=i+1; ii<n; ii++){
			if(ii == i) continue;
			long x2 = new_poster[ii].first;
			long y2 = new_poster[ii].second;
			long d_x = abs(x1 - x2);
			long d_y = abs(y1 - y2);
			if(d_x*h >= d_y*w){
				lp.set_a(i, nr, w); lp.set_a(ii, nr, w); lp.set_b(nr, 2*d_x);
			}
			else{
				lp.set_a(i, nr, h); lp.set_a(ii, nr, h); lp.set_b(nr, 2*d_y);
			}
			nr++;
		}
		//constraints with old posters
		long min_d_x = numeric_limits<int>::max();
		long min_d_y = numeric_limits<int>::max();
		bool flag_x = false, flag_y = false;
		for(int j=0; j<m; j++){
			long x2 = old_poster[j].first;
			long y2 = old_poster[j].second;
			long d_x = abs(x1 - x2);
			long d_y = abs(y1 - y2);
			if(d_x < min_d_x && d_x*h >= d_y*w){
				min_d_x = d_x;
				flag_x = true;
			}
			else if(d_y < min_d_y && d_x*h < d_y*w){
				min_d_y = d_y;
				flag_y = true;
			}
		}
		if(flag_x) {lp.set_a(i, nr, w); lp.set_b(nr, 2*min_d_x - w); nr++;}
		if(flag_y) {lp.set_a(i, nr, h); lp.set_b(nr, 2*min_d_y - h); nr++;}
		lp.set_c(i, -2*(h+w));
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));
	
	cout << ceil_to_double(-s.objective_value()) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();
    
    return 0;
}
