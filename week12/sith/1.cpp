#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
typedef CGAL::Exact_predicates_inexact_constructions_kernel    			K;
typedef CGAL::Triangulation_vertex_base_with_info_2<pair<int, int>,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     			Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            			Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  			Triangulation;
typedef Triangulation::Vertex_handle									VH;
typedef Triangulation::Vertex_circulator								VC;

int MAX = 1 << 21;

int assign_component(const Triangulation &t, const K::FT &rr)
{
	int component = 0;
	int maxNum = 0;
	for(auto v=t.finite_vertices_begin(); v!=t.finite_vertices_end(); ++v) {
		if(v->info().second == 0) {
			v->info().second = ++component;
			vector<VH> stack;
			stack.push_back(v);
			int curNum = 1;
			do {
				VH u = stack.back();
				stack.pop_back();
				VC c = t.incident_vertices(u);
				do {
					if(!t.is_infinite(c) &&
						CGAL::squared_distance(c->point(), u->point()) <= rr) {
						if(c->info().second != 0) assert(c->info().second == component);
						if(c->info().second == 0) {
							c->info().second = component;
							stack.push_back(c);
							curNum++;
						}
					}
				} while(++c != t.incident_vertices(u));
			} while(!stack.empty());
			maxNum = max(maxNum, curNum);
		}
	}
	return maxNum;
}

void testcase()
{
	int n; long r;
	cin >> n >> r;

	typedef pair<K::Point_2, pair<int, int>> IPoint; //idx, component
	vector<IPoint> planet;
	planet.reserve(n);
	for(int i=0; i<n; i++) {
		int x, y; cin >> x >> y;
		planet.emplace_back(K::Point_2(x, y), make_pair(i, 0));
	}

	K::FT rr = r * r;
	int left = 0; 
	int right = n - 1;
	int res = 0;
	while(left + 1 < right) {
		int m = (left + right) / 2;
		Triangulation t;
		t.insert(planet.begin()+m, planet.end());
		int cur = assign_component(t, rr);
		res = max(res, min(m, cur));
		if(cur == m) {
			cout << m << "\n";
			return;
		}
		else if(cur < m) right = m;
		else left = m;
	}
	cout << res << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      