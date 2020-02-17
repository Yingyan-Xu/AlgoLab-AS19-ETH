//
//  main.cpp
//  hit
//
//  Created by ayan on 2019/10/5.
//  Copyright © 2019 ayan. All rights reserved.
//

#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <type_traits>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

void testcase(int n_seg)
{
    long p1x, p1y, p2x, p2y;
    std::cin >> p1x >> p1y >> p2x >> p2y;
    P p1(p1x, p1y), p2(p2x, p2y);
    R r(p1, p2);
    
    std::vector<S> seg;
    bool flag = false;
    int i = 0, j = 0;
    for(i=0; i<n_seg; i++){
        std::cin >> p1x >> p1y >> p2x >> p2y;
        P p3(p1x, p1y), p4(p2x, p2y);
        if(CGAL::do_intersect(r, S(p3, p4))){
            flag = true;
            break;
        }
    }
    std::string tmp;
    for(j=0; j<n_seg-i; j++) getline(std::cin, tmp);
    if(flag) std::cout << "yes\n";
    else std::cout << "no\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    
    int n_seg = 0;
    std::cin >> n_seg;
    while(n_seg != 0){
        testcase(n_seg);
        std::cin >> n_seg;
    }
    
    return 0;
}

