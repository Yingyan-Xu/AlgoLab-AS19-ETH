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

double floor_to_double(const SolT& x)
{
    double a = floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

double ceil_to_double(const SolT& x)
{
    double a = ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void testcase(int p, int a, int b)
{
	if(p == 1){
		Program lp (CGAL::SMALLER, true, 0, false, 0);

		const int X = 0;
		const int Y = 1;
		lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
		lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
		lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

		lp.set_c(X, a); 
		lp.set_c(Y, -b);

		Solution s = CGAL::solve_linear_program(lp, ET());
		assert(s.solves_linear_program(lp));

		if(s.is_infeasible()) cout << "no\n";
		else if(s.is_unbounded()) cout << "unbounded\n";
		else cout << floor_to_double(-s.objective_value()) << "\n";
	}

	else{
		Program lp (CGAL::LARGER, false, 0, true, 0);

		const int X = 0;
		const int Y = 1;
		const int Z = 2;
		lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
		lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b);
		lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);

		lp.set_c(X, a); 
		lp.set_c(Y, b);
		lp.set_c(Z, 1);

		Solution s = CGAL::solve_linear_program(lp, ET());
		assert(s.solves_linear_program(lp));

		if(s.is_infeasible()) cout << "no\n";
		else if(s.is_unbounded()) cout << "unbounded\n";
		else cout << ceil_to_double(s.objective_value()) << "\n";
	}
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int p, a, b;
    cin >> p;
    while(p != 0){
	    cin >> a >> b;
	    testcase(p, a, b);
	    cin >> p;
	}

	return 0;
}
