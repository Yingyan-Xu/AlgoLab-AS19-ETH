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
typedef CGAL::Exact_predicates_inexact_constructions_kernel 	K;
typedef std::size_t                                            	Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   	Vb;
typedef CGAL::Triangulation_face_base_2<K>                     	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            	Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  	Triangulation;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

int n; 

int getCompNum(const Triangulation &t, const K::FT &rr)
{
	EdgeV edges;
  	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    	Index i1 = e->first->vertex((e->second+1)%3)->info();
    	Index i2 = e->first->vertex((e->second+2)%3)->info();
    	if (i1 > i2) std::swap(i1, i2);
    	edges.emplace_back(i1, i2, t.segment(e).squared_length());
  	}

  	sort(edges.begin(), edges.end(),
	    [](const Edge& e1, const Edge& e2) -> bool {
	      	return std::get<2>(e1) < std::get<2>(e2);
        });

  	boost::disjoint_sets_with_storage<> uf(n);
  	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    	Index c1 = uf.find_set(std::get<0>(*e));
    	Index c2 = uf.find_set(std::get<1>(*e));
    	K::FT d = get<2>(*e);
    	if(d > rr) break;
    	if (c1 != c2) {
      		uf.link(c1, c2);
    	}
  	}

  	vector<int> compNum(n, 0);
  	vector<bool> vis(n, false);
  	for(auto v=t.finite_vertices_begin(); v!=t.finite_vertices_end(); ++v) {
  		Index c = uf.find_set(v->info());
  		compNum[c]++;
  	}

  	int res = 0;
  	for(int i=0; i<compNum.size(); i++) {
  		res = max(res, compNum[i]);
  	}

  	return res;
}

void testcase()
{
	K::FT r;
	cin >> n >> r;
	K::FT rr = CGAL::square(r);

	typedef pair<K::Point_2,Index> IPoint;
  	vector<IPoint> points;
  	points.reserve(n);
  	for (Index i = 0; i < n; ++i) {
    	int x, y;
    	cin >> x >> y;
    	points.emplace_back(K::Point_2(x, y), i);
  	}

  	int left = 0, right = n;
  	int res = 0;
  	while(left + 1 < right) {
  		int m = (left + right) / 2;
  		Triangulation t;
  		t.insert(points.begin()+m, points.end());
  		int cur = getCompNum(t, rr);
  		res = max(res, min(cur, m));
  		if(cur == m) {
  			cout << cur << "\n"; return;
  		}
  		if(cur < m) right = m;
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
