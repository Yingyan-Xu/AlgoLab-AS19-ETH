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

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

double floor_to_double(const SolT& x)
{
    double a = floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase(int n, int m)
{
	vector<pair<int, int>> nutrients;
	for(int i=0; i<n; i++){
		int min_n, max_n;
		cin >> min_n >> max_n;
		nutrients.push_back(make_pair(min_n, max_n));
	}

	vector<pair<int, vector<int>>> food;
	for(int i=0; i<m; i++){
		int price;
		cin >> price;
		vector<int> nutrient_in_i(n);
		for(int j=0; j<n; j++){
			cin >> nutrient_in_i[j];
		}
		food.push_back(make_pair(price, nutrient_in_i));
	}

	Program lp (CGAL::SMALLER, true, 0, false, 0);

	vector<int> amount(m);
	for(int i=0; i<m; i++) amount[i] = i;

	//set constraints
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			lp.set_a(amount[j], 2*i, food[j].second[i]);
			lp.set_a(amount[j], 2*i+1, -food[j].second[i]);
		}
		lp.set_b(2*i, nutrients[i].second); // <= max
		lp.set_b(2*i+1, -nutrients[i].first); // >= min
	}

	//set objective 
	for(int i=0; i<m; i++){
		lp.set_c(amount[i], food[i].first);
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));

	if(s.is_infeasible()) cout << "No such diet.\n";
	else cout << floor_to_double(s.objective_value()) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int n, m; //nutrients, food
    cin >> n >> m;
    while(n != 0 && m != 0){
    	testcase(n, m);
    	cin >> n >> m;
    }

    return 0;
}
