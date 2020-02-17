#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase()
{
    int n, m, c; //warehouse, stadium, contour line
    cin >> n >> m >> c;

    vector<pair<int, int>> warehouses;
    vector<K::Point_2> t_warehouse;
    // t_warehouse.reserve(n);
    for(int i=0; i<n; i++) {
        long x, y; int s, a;
        cin >> x >> y >> s >> a;
        t_warehouse.push_back(K::Point_2(x, y));
        warehouses.push_back({s, a});

    }

    vector<pair<int, int>> stadiums;
    vector<K::Point_2> t_stadium;
    // t_stadium.reserve(m);
    for(int i=0; i<m; i++) {
        long x, y; int d, u;
        cin >> x >> y >> d >> u;
        t_stadium.push_back(K::Point_2(x, y));
        stadiums.push_back({d, u});
    }

    vector<vector<int>> revenue(n);
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            int r; cin >> r;
            revenue[i].push_back(r);
        }
    }

    vector<pair<K::Point_2, int>> contour;
    for(int i=0; i<c; i++) {
    	long x, y, r;
    	cin >> x >> y >> r;
    	K::FT rr = r * r;
    	contour.push_back(make_pair(K::Point_2(x, y), rr));
    }

    Triangulation t;
    t.insert(t_warehouse.begin(), t_warehouse.end());
    t.insert(t_stadium.begin(), t_stadium.end());

    int cnt = 0;
    vector<vector<int>> n_contour(n, vector<int>(m, 0));
    for(int i=0; i<c; i++) {
    	K::Point_2 p = contour[i].first;
    	K::FT rr = contour[i].second;
    	auto v = t.nearest_vertex(p);
    	K::FT d = CGAL::squared_distance(p, v->point());
    	if(d > rr) continue; cnt++;
    	K::Circle_2 c(p, rr);
    	for(int w=0; w<n; w++) {
    		for(int s=0; s<m; s++) {
    			auto pw = t_warehouse[w];
    			auto ps = t_stadium[s];
    			if(c.oriented_side(pw) != c.oriented_side(ps))
    				n_contour[w][s]++;
    		}
    	}
    	if(cnt >= 100) break;
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);

    int c_cnt = 0;
    //#alcohl bound: m
    for(int i=0; i<m; i++) {
        for(int j=0;j<n; j++) {
            int varId = i * n + j;
            lp.set_a(varId, c_cnt, warehouses[j].second);
        }
        lp.set_b(c_cnt, 100 * stadiums[i].second);
        c_cnt++;
    }

    //#supply bound: n
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            int varId = j * n + i;
            lp.set_a(varId, c_cnt, 1);
        }
        lp.set_b(c_cnt, warehouses[i].first);
        c_cnt++;
    }

    //#demand bound: 2 * m
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            int varId = i * n + j;
            lp.set_a(varId, c_cnt, 1);
        }
        lp.set_b(c_cnt, stadiums[i].first);
        c_cnt++;
        for(int j=0; j<n; j++) {
            int varId = i * n + j;
            lp.set_a(varId, c_cnt, -1);
        }
        lp.set_b(c_cnt, -stadiums[i].first);
        c_cnt++;
    }

    //objective: max profit; #var: n * m
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            int varId = j * n + i;
            int profit = 100 * revenue[i][j] - n_contour[i][j];
            // cout << i << " " << j << " " << profit << endl;
            lp.set_c(varId, -profit);
        }
    }

    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);

    // cout << s << endl;
    
    if(s.is_infeasible()) cout << "RIOT!\n";
    else cout << floor_to_double(-s.objective_value() / 100.0) << endl;
} 

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << setiosflags(ios::fixed) << setprecision(0);

    int t; cin >> t;
    while(t--) testcase();

    return 0;
}
