#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

//CGAL triangulation
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

double floor_to_double(const K::FT& x)
{
	double a = floor(CGAL::to_double(x));
	while(a > x) a -= 1;
	while(a+1 <= x) a += 1;
	return a; 
}

void testcase(size_t n)
{
	cout << setiosflags(ios::fixed) << setprecision(0);

	vector<K::Point_2> exist;
	exist.reserve(n);
	for(size_t i=0; i<n; ++i){
		int x, y; cin >> x >> y;
		exist.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(exist.begin(), exist.end());

	size_t m; //num of new restaurants
	cin >> m;
	for(size_t i=0; i<m; i++){
		int x, y; cin >> x >> y;
		K::Point_2 new_res(x, y);
		auto v = t.nearest_vertex(new_res);
		K::FT dist = CGAL::squared_distance(v->point(), new_res);
		cout << floor_to_double(dist) << "\n";
	}
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    size_t n; //num of existing restaurants
    cin >> n;
    while(n != 0){
    	testcase(n);
    	cin >> n;
    }

    return 0;
}
