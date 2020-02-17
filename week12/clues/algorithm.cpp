#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
typedef CGAL::Exact_predicates_inexact_constructions_kernel 				K;
typedef CGAL::Triangulation_vertex_base_with_info_2<pair<int, bool>,K>   	Vb;
typedef CGAL::Triangulation_face_base_2<K>                     				Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            				Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  				Triangulation;
typedef Triangulation::Vertex_handle										VH;

bool has_inference(const Triangulation &t, const K::FT &rr)
{
	for(auto e=t.finite_edges_begin(); e!=t.finite_edges_end(); ++e) 
		if(t.segment(e).squared_length() <= rr) return true;
	return false;
}

bool try_two_color(const Triangulation &t, const K::FT &rr)
{
	for(auto v=t.finite_vertices_begin(); v!=t.finite_vertices_end(); ++v) 
		v->info().first = 0;

	int component = 0;
	Triangulation t1, t2;
	for(auto v=t.finite_vertices_begin(); v!=t.finite_vertices_end(); ++v) {
		if(v->info().first == 0) {
			v->info().first = ++component;
			v->info().second = true;
			vector<VH> stack;
			stack.push_back(v);
			while(!stack.empty()) {
				VH cur = stack.back();
				stack.pop_back();
				auto c = t.incident_vertices(cur);
				do {
					if(t.is_infinite(c) ||
						CGAL::squared_distance(c->point(), cur->point()) > rr)
						continue;
					if(c->info().first != 0 && 
						c->info().second == cur->info().second)
						return false;
					if(c->info().first == 0) {
						c->info().first = component;
						c->info().second = !cur->info().second;
						stack.push_back(c);
					}
				} while(++c != t.incident_vertices(cur));
			}
		}
		if(v->info().second) t1.insert(v->point());
		else t2.insert(v->point());
	}

	return !has_inference(t1, rr) && !has_inference(t2, rr);
}

void testcase()
{
	int n, m; K::FT r;
	cin >> n >> m >> r;

	vector<K::Point_2> stations;
	stations.reserve(n);
	for(int i=0; i<n; i++) {
		long x, y; cin >> x >> y;
		stations.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(stations.begin(), stations.end());

	K::FT rr = CGAL::square(r);
	bool flag = try_two_color(t, rr);

	for(int i=0; i<m; i++) {
		long x0, y0, x1, y1; 
		cin >> x0 >> y0 >> x1 >> y1;

		if(!flag) {cout << "n"; continue;}

		K::Point_2 holmes(x0, y0), watson(x1, y1);
		K::FT init_d = CGAL::squared_distance(holmes, watson);
		if(init_d <= rr) {cout << "y"; continue;}

		auto v = t.nearest_vertex(holmes);
		auto v2 = t.nearest_vertex(watson);
		if(CGAL::squared_distance(holmes, v->point()) <= rr &&
			CGAL::squared_distance(watson, v2->point()) <= rr &&
			v->info().first == v2->info().first)
			cout << "y";
		else cout << "n";
	}
	cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
