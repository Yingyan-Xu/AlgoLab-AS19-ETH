#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <iomanip>
#include <tuple>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
typedef CGAL::Exact_predicates_inexact_constructions_kernel 	K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   	Vb;
typedef CGAL::Triangulation_face_base_2<K>                     	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            	Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  	Triangulation;

typedef tuple<int,int,K::FT> Edge;
typedef vector<Edge> EdgeV;

void testcase()
{
	int n, m; K::FT p;
	cin >> n >> m >> p;

	vector<pair<K::Point_2, int>> jammers;
	jammers.reserve(n);
	for(int i=0; i<n; i++) {
		long x, y; cin >> x >> y;
		jammers.emplace_back(K::Point_2(x, y), i);
	}

	Triangulation t;
	t.insert(jammers.begin(), jammers.end());

	EdgeV edges;
  	edges.reserve(3*n); 
  	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    	int i1 = e->first->vertex((e->second+1)%3)->info();
    	int i2 = e->first->vertex((e->second+2)%3)->info();
    	edges.emplace_back(i1, i2, t.segment(e).squared_length());
  	}
  	sort(edges.begin(), edges.end(),
	    [](const Edge& e1, const Edge& e2) -> bool {
	      	return get<2>(e1) < get<2>(e2);
        });

  	boost::disjoint_sets_with_storage<> uf(n);
  	int n_components = n;
  	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    	int c1 = uf.find_set(get<0>(*e));
    	int c2 = uf.find_set(get<1>(*e));
    	K::FT d = get<2>(*e);
    	if(d > p) break;
    	if (c1 != c2) {
      		uf.link(c1, c2);
      	if (--n_components == 1) break;
    	}
  	}

  	
  	boost::disjoint_sets_with_storage<> ufa(n), ufb(n);
  	K::FT a = 0, b = 0;
  	auto ita = edges.begin();
  	auto itb = edges.begin();
  	for(int i=0; i<m; i++) {
  		long x0, y0, x1, y1;
  		cin >> x0 >> y0 >> x1 >> y1;

  		K::Point_2 start(x0, y0), end(x1, y1);
  		auto vs = t.nearest_vertex(start);
  		auto ve = t.nearest_vertex(end);
  		K::FT init_d = 4 * max(CGAL::squared_distance(start, vs->point()),
  								CGAL::squared_distance(end, ve->point()));

  		int c1 = vs->info();
  		int c2 = ve->info();
  		if(init_d <= p && 
  			uf.find_set(c1) == uf.find_set(c2)) {
  			cout << "y";
  			b = max(b, init_d);

  			for(; itb!=edges.end() && 
  				ufb.find_set(c1) != ufb.find_set(c2); itb++)
  				ufb.link(get<0>(*itb), get<1>(*itb));
  		}
  		else cout << "n";
  		a = max(a, init_d);
  		for(; ita!=edges.end() && 
  				ufa.find_set(c1) != ufa.find_set(c2); ita++)
  				ufa.link(get<0>(*ita), get<1>(*ita));
  	}
  	if(ita != edges.begin()) a = max(a, get<2>(*(--ita)));
  	if(itb != edges.begin()) b = max(b, get<2>(*(--itb)));

  	cout << "\n" << a << "\n" << b << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << setiosflags(ios::fixed) << setprecision(0);
    
    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
