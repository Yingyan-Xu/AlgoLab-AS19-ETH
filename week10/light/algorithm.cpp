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
typedef CGAL::Exact_predicates_inexact_constructions_kernel 	K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   	Vb;
typedef CGAL::Triangulation_face_base_2<K>                     	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            	Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                 	Triangulation;

void testcase()
{
	//#participant, lamp
	int m, n; cin >> m >> n;

	vector<pair<K::Point_2, K::FT>> par;
	for(int i=0; i<m; i++) {
		long x, y; K::FT r;
		cin >> x >> y >> r;
		par.push_back(make_pair(K::Point_2(x, y), r));
	}

	K::FT h; cin >> h;

	vector<K::Point_2> lamps;
	lamps.reserve(n);
	for(int i=0; i<n; i++) {
		long x, y; cin >> x >> y;
		lamps.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(lamps.begin(), lamps.end());

	int maxIdx = 0;
	vector<int> idx(m);
	for(int i=0; i<m; i++) {
		K::Point_2 cur_p = par[i].first;
		K::FT cur_r = par[i].second;

		auto v = t.nearest_vertex(cur_p);
		K::FT init_d = CGAL::squared_distance(cur_p, v->point());
		if(init_d >= CGAL::square(cur_r+h)) {
			idx[i] = n; maxIdx = n; continue;
		}
		
		for(int j=0; j<n; j++) {
			K::Point_2 lamp_p = lamps[j];
			K::FT cur_d = CGAL::squared_distance(lamp_p, cur_p);
			if(cur_d < CGAL::square(cur_r+h)) {
				idx[i] = j; maxIdx = max(maxIdx, j); break;
			}
		}
	}

	for(int i=0; i<m; i++) {
		if(idx[i] == maxIdx) cout << i << " ";
	}
	cout << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
