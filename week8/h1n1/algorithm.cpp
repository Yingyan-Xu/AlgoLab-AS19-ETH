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
#include <CGAL/Triangulation_face_base_with_info_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Face_handle FH;
typedef Triangulation::Vertex_handle VH;

void testcase(int n)
{
	vector<K::Point_2> pts;
	pts.reserve(n);
	for(int i=0; i<n; i++) {
		long x, y; cin >> x >> y;
		pts.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(pts.begin(), pts.end());

	priority_queue<FH> Q;
	for(auto f=t.all_faces_begin(); f!=t.all_faces_end(); ++f) {
		if(t.is_infinite(f)) {
			VH v0, v1;
			if(!t.is_infinite(f->neighbor(0))) {
				v0 = f->vertex(1); v1 = f->vertex(2);
			}
			else if(!t.is_infinite(f->neighbor(1))) {
				v0 = f->vertex(2); v1 = f->vertex(0);
			}
			else {
				v0 = f->vertex(0); v1 = f->vertex(1);
			}
			K::FT d = CGAL::squared_distance(v0->point(), v1->point());
			f->info() = d;
			Q.push(f);
		}
		else f->info() = 0;
	}

	while(!Q.empty()) {
		FH cur_f = Q.top(); Q.pop();
		K::FT cur_d = cur_f->info();
		for(int i=0; i<3; i++) {
			FH next_f = cur_f->neighbor(i);
			if(t.is_infinite(next_f)) continue;
			K::FT sharedEdge = CGAL::squared_distance(cur_f->vertex((i+1)%3)->point(),
												cur_f->vertex((i+2)%3)->point());
			K::FT next_d = min(cur_d, sharedEdge);
			if(next_d > next_f->info()) {
				next_f->info() = next_d;
				Q.push(next_f);
			}
		}
	}

	int m; cin >> m;
	for(int i=0; i<m; i++) {
		long x, y; K::FT d;
		cin >> x >> y >> d;
  
		K::Point_2 p(x, y);
		VH nearest_v = t.nearest_vertex(p);
		K::FT init_d = CGAL::squared_distance(p, nearest_v->point());
		if(init_d < d) {
			cout << "n"; continue;
		}

		FH init_f = t.locate(p);
		if(t.is_infinite(init_f)) {
			cout << "y"; continue;
		}

		if(init_f->info() >= 4*d) cout << "y";
		else cout << "n";
	}
	cout << "\n";
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
