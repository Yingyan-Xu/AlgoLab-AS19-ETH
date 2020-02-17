//
//  antenna.cpp
//  
//
//  Created by ayan on 2019/10/6.
//
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 P;

double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a >= x+1) a -= 1;
    while (a < x) a += 1;
    return a;
}

void testcase(int n_citizen)
{
    //load points
    std::vector<P> p;
    for(int i=0; i<n_citizen; i++){
        long x=0, y=0;
        std::cin >> x >> y;
        p.push_back(P(x, y));
    }
    
    //min circle
    Min_circle mc(p.begin(), p.end(), true);
    Traits::Circle c = mc.circle();
    K::FT d = CGAL::sqrt(c.squared_radius());
    std::cout << long(ceil_to_double(d)) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    
    int n_citizen; //number of citizens;
    std::cin >> n_citizen;
    while(n_citizen != 0){
        testcase(n_citizen);
        std::cin >> n_citizen;
    }
    
    return 0;
}
