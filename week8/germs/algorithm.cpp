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
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Triangulation_vertex_base_with_info_2<IK::FT, IK> Vb;
typedef CGAL::Triangulation_face_base_2<IK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK,Tds>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

double ceil_to_double(const EK::FT& x)
{
	double a = ceil(CGAL::to_double(x));
  	while (a < x) a += 1;
  	while (a-1 >= x) a -= 1;
  	return a;
}

EK::FT get_t(const EK::FT& s)
{
	return CGAL::sqrt(CGAL::sqrt(s/4) - 0.5);
}

void testcase(int n)
{
	long l, b, r, t;
	cin >> l >> b >> r >> t;

	vector<IK::Point_2> pts;
	pts.reserve(n);
	for(int i=0; i<n; i++) {
		long x, y;
		cin >> x >> y;
		pts.push_back(IK::Point_2(x, y));
	}

	Triangulation trg;
	trg.insert(pts.begin(), pts.end());

	for(auto v=trg.finite_vertices_begin(); v!=trg.finite_vertices_end(); ++v) {
		IK::FT& info = v->info();
		IK::Point_2 p = v->point();
		IK::FT boundaryDist = min(min(p.x() - l, r - p.x()), min(p.y() - b, t - p.y()));
		info = 4 * boundaryDist * boundaryDist;
	}

	for(auto e=trg.finite_edges_begin(); e!=trg.finite_edges_end(); ++e) {
		auto v1 = e->first->vertex(trg.cw(e->second));
		auto v2 = e->first->vertex(trg.ccw(e->second));
		IK::FT& info_v1 = v1->info();
		IK::FT& info_v2 = v2->info();
		IK::FT edgeLen = trg.segment(e).squared_length();

		info_v1 = min(info_v1, edgeLen);
		info_v2 = min(info_v2, edgeLen);
	}

	vector<IK::FT> S;
	for(auto v=trg.finite_vertices_begin(); v!=trg.finite_vertices_end(); ++v) {
		S.push_back(v->info());
	}
	sort(S.begin(), S.end());

	EK::FT first = get_t(EK::FT(S[0]));
	EK::FT median = get_t(EK::FT(S[n/2]));
	EK::FT last = get_t(EK::FT(S[n-1]));

	cout << ceil_to_double(first) << " "
		 << ceil_to_double(median) << " "
		 << ceil_to_double(last) << "\n";
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
