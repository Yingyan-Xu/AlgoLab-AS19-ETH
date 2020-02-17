#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_handle VH;

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase()
{
	int a, s, b; //asteroid, shot, bounty
	cin >> a >> s >> b;

	int e; cin >> e; //energy

	vector<pair<K::Point_2, int>> asteroid;
	asteroid.reserve(a);
	for(int i=0; i<a; i++) {
		long x, y; int d;
		cin >> x >> y >> d;
		// cout << x << " " <<  y << " " << d << endl;
		asteroid.push_back({K::Point_2(x, y), d});
	}

	vector<K::Point_2> shot;
	for(int i=0; i<s; i++) {
		long x, y; cin >> x >> y;
		// cout << x << " " <<  y << " " << endl;
		shot.push_back(K::Point_2(x, y));
	}

	vector<K::Point_2> bounty;
	bounty.reserve(b);
	for(int i=0; i<b; i++) {
		long x, y; cin >> x >> y;
		// cout << x << " " <<  y << " " << endl;
		bounty.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(bounty.begin(), bounty.end());

	Program lp (CGAL::LARGER, true, 0, false, 0); 

	for(int i=0; i<s; i++) {
		K::Point_2 curShot = shot[i];
		K::FT maxRR = numeric_limits<double>::max();
		if(b > 0) {
			VH nearest_bounty = t.nearest_vertex(curShot);
			K::Point_2 bp = nearest_bounty->point();
			maxRR = CGAL::squared_distance(curShot, bp);
		}
		for(int j=0; j<a; j++) {
			K::Point_2 curP = asteroid[j].first;
			K::FT d2 = CGAL::squared_distance(curShot, curP);
			if(d2 < maxRR) {
				double coeff = 1. / max(d2, K::FT(1));
				lp.set_a(i, j, coeff);
			}
		}
	}

	for(int i=0; i<s; i++) {
		lp.set_a(i, a, -1);
	}
	lp.set_b(a, -e);

	for(int i=0; i<a; i++) {
		lp.set_b(i, asteroid[i].second);
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
  	assert(sol.solves_linear_program(lp));

  	if(sol.is_infeasible()) cout << "n\n";
  	else cout << "y\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
