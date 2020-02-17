#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <tuple>

using namespace std;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct point
{
	int x, y, z, i;
	point(int x, int y, int z, int i) : 
		x(x), y(y), z(z), i(i) {}
};

bool possible(int d, int h, const vector<point> &pts)
{
	Program lp (CGAL::SMALLER, false, 0, false, 0);

	for(auto p : pts) {

 		lp.set_a(0, p.i, 1);
		int coeff = 1;

		for(int x=0; x<=d; x++) {
			for(int y=0; y<=d-x; y++) {
				for(int z=0; z<=d-x-y; z++) {
					double num = pow(p.x, x) * pow(p.y, y) * pow(p.z, z);
					num = p.i < h ? num : -num;
					lp.set_a(coeff, p.i, num);
					++coeff;
				}
			}
		}
	}
	lp.set_c(0, -1);
	lp.set_l(0, true, 0);
	lp.set_u(0, true, 1);

	
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
	assert(s.solves_linear_program(lp));

	return (!s.is_infeasible() && s.objective_value() != 0);

}

void testcase()
{
	int h, t; cin >> h >> t;

	vector<point> pts;
	for(int i=0; i<h+t; i++) {
		int x, y, z; 
		cin >> x >> y >> z;
		pts.push_back(point(x, y, z, i));
	}

	for(int d=0; d<=30; d++) {
		if(possible(d, h, pts)) {
			cout << d << "\n";
			return;
		}
	}
	cout << "Impossible!\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
