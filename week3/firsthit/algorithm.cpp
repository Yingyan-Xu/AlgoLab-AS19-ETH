#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n)
{
	long x, y, a, b;
	cin >> x >> y >> a >> b;
	R r(P(x,y), P(a,b));

	vector<S> seg;
	for(int i=0; i<n; i++) {
		long r, s, t, u;
		cin >> r >> s >> t >> u;
		seg.push_back(S(P(r,s), P(t,u)));
	}

	random_shuffle(seg.begin(), seg.end());
	
	K::FT res = numeric_limits<double>::max();
	P final;
	bool intersect = false;
	for(auto s : seg) {
		if(CGAL::squared_distance(r.source(), s) >= res) continue;
		
		if(CGAL::do_intersect(r, s)) {
			intersect = true;
			auto o = CGAL::intersection(r, s);
			if(const P* op = boost::get<P>(&*o)) {
				K::FT cur = CGAL::squared_distance(r.source(), *op);
				if(cur < res) {
					res = cur;
					final = *op;
				}
			}
			else if (const S* os = boost::get<S>(&*o)) {
				if(CGAL::collinear_are_ordered_along_line(r.source(), (*os).source(), (*os).target())) {
					K::FT cur = CGAL::squared_distance(r.source(), (*os).source());
					if(cur < res) {
						res = cur;
						final = (*os).source();
					}
				}
				else {
					K::FT cur = CGAL::squared_distance(r.source(), (*os).target());
					if(cur < res) {
						res = cur;
						final = (*os).target();
					}
				}
			}
			else cout << "error\n";
		}
	}
	if(intersect) {
		cout << floor_to_double(final.x()) << " "
			<< floor_to_double(final.y()) << "\n";
	}
	else cout << "no\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << setiosflags(ios::fixed) << setprecision(0);
    
    int n; cin >> n;
    while(n != 0) {
    	testcase(n);
    	cin >> n;
    }

    return 0;
}
