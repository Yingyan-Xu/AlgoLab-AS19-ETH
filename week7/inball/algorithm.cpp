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

void testcase(int n, int d)
{
	Program lp (CGAL::SMALLER, false, 0, false, 0); 

	for(int i=0; i<n; i++) {
		int sum = 0;
		for(int j=1; j<=d; j++) {
			int a; cin >> a;
			sum += a*a;
			lp.set_a(j, i, a);
		}
		int b; cin >> b;
		lp.set_a(0, i, sqrt(sum));
		lp.set_b(i, b);
	}

	lp.set_c(0, -1);
	lp.set_l(0, true, 0);

	Solution s = CGAL::solve_linear_program(lp, ET());
  	assert(s.solves_linear_program(lp));

  	if(s.is_unbounded()) cout << "inf\n";
  	else if(s.is_infeasible()) cout << "none\n";
  	else cout << - s.objective_value().numerator()
  				/ s.objective_value().denominator() << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int n; cin >> n;
    while(n != 0) {
    	int d; cin >> d;
    	testcase(n, d);
    	cin >> n;
    }

    return 0;
}
