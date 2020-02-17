#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

inline int to_num(char a) {return a-'A';}

void testcase()
{
	int h, w; //num of lines, letters
	cin >> h >> w;

	string crime;
	cin >> crime;
	vector<int> target(26, 0); //record num of times each char occurs in the crime scene
	for(int i=0; i<crime.length(); i++){
		target[to_num(crime[i])]++;
	}

	vector<string> front;
	vector<string> back;
	for(int i=0; i<h; i++){
		string line;
		cin >> line;
		front.push_back(line);
	}
	for(int i=0; i<h; i++){
		string line;
		cin >> line;
		back.push_back(line);
	}

	vector<vector<int>> times(26, vector<int>(26));
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			int f = to_num(front[i][j]);
			int b = to_num(back[i][w-j-1]);
			times[f][b]++;
		}
	}

	graph G(26 * 26 + 26);
	edge_adder adder(G);
  	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_target = boost::add_vertex(G);

	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			int f = to_num(front[i][j]);
			int b = to_num(back[i][w-j-1]);
			int from = f * 26 + b;
			int weight = times[f][b];
			if(weight != 0){ //cannot use boost::edge().second as judgement, it leads to TL!!!
				adder.add_edge(v_source, from, weight);
				adder.add_edge(from, 26 * 26 + f, weight);
				adder.add_edge(from, 26 * 26 + b, weight);
				times[f][b] = 0;
			}
		}
	}

	for(int i=0; i<26; i++){
		if(target[i] == 0) continue;
		adder.add_edge(26 * 26 + i, v_target, target[i]);
	}

	long flow = boost::push_relabel_max_flow(G, v_source, v_target);

	if(flow == crime.length()) cout << "Yes\n";
	else cout << "No\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t; cin >> t;

    while(t--) testcase();

    return 0;
}
