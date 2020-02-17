//
//  almost-antenna.cpp
//  
//
//  Created by ayan on 2019/10/6.
//

#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>
#include <limits>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 P;

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a >= x+1) a -= 1;
    while (a < x) a += 1;
    return a;
}

void testcase(int n_citizen)
{
    std::vector<P> p;
    for(int i=0; i<n_citizen; i++){
        long x=0, y=0;
        std::cin >> x >> y;
        p.push_back(P(x, y));
    }
    Min_circle mc(p.begin(), p.end(), true);
    Traits::Circle c = mc.circle();
    
    
    K::FT minr = c.squared_radius();
    Min_circle::Support_point_iterator iter;
    for(iter=mc.support_points_begin(); iter!=mc.support_points_end(); iter++){
        //std::vector<P>::iterator it = find(p.begin(), p.end(), *iter);
        //p.erase(it);
        for(int i=0; i<n_citizen; i++){
            if(p[i] == *iter) {p.erase(p.begin()+i); break;}
        }
        Min_circle tmp(p.begin(), p.end(), true);
        Traits::Circle c_tmp = tmp.circle();
        K::FT rds = c_tmp.squared_radius();
        if(rds < minr) minr = rds;
        p.push_back(*iter);
    }
    std::cout << long(ceil_to_double(CGAL::sqrt(minr))) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    
    int n_citizen = 0; //num of citizen
    std::cin >> n_citizen;
    while(n_citizen != 0){
        testcase(n_citizen);
        std::cin >> n_citizen;
    }
    
    return 0;
}
