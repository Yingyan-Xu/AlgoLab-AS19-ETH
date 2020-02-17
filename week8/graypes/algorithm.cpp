#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;

double ceil_to_double(const EK::FT& x)
{
	double a = ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

void testcase(int n)
{
	vector<IK::Point_2> pts;
	pts.reserve(n);
	for (size_t i=0; i<n; ++i) {
	    long x, y;
	    std::cin >> x >> y;
	    pts.push_back(IK::Point_2(x, y));
	}

    Triangulation t;
	t.insert(pts.begin(), pts.end());

	IK::FT minEdge = numeric_limits<IK::FT>::max();
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    	minEdge = min(minEdge, t.segment(e).squared_length());
    }

    cout << ceil_to_double(CGAL::sqrt(minEdge) / 2 * 100) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int n; cin >> n;
    while(n != 0) {
    	testcase(n);
    	cin >> n;
    }

    return 0;
}
