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
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<CGAL::Gmpz> &x)
{
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase()
{
	int xs, ys, n;
	cin >> xs >> ys >> n;

	Program lp (CGAL::LARGER, false, 0, false, 0);

	int px = 0, py = 1, ptime = 2;
	for(int i=0; i<n; i++) {
		long a, b, c; int v;
		cin >> a >> b >> c >> v;
		//not cross the line
		long sgn = a * xs + b * ys + c;
		if(sgn < 0) {a = -a; b = -b; c = -c;}
		lp.set_a(px, i, a);
		lp.set_a(py, i, b);
		lp.set_b(i, -c);

		//preparation time at least ptime
		long coeff_time = - v * sqrt(a * a + b * b);
		lp.set_a(px, n+i, a);
		lp.set_a(py, n+i, b);
		lp.set_a(ptime, n+i, coeff_time);
		lp.set_b(n+i, -c);
	}

	lp.set_l(ptime, true, 1);
	lp.set_c(ptime, -1);

	CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
	assert(s.solves_linear_program(lp));

	// cout << s << endl;

	cout << floor_to_double(-s.objective_value()) << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
