#include <utility>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <cmath> 

//Requires: first and and second are valid coordinate pairs.  That right_x, right_y,
//are both greater than left_x and left_y respectively.
//Effects: Returns true if the two points are adjacent, false otherwise.
bool is_adjacent(const std::pair<int, int> &first, const std::pair<int, int> &second,
		 int left_x, int left_y, int right_x, int right_y);

//Requires: lx, ly are less than rx, ry respectively.  check is a valid point.
//Effects: returns 0 if point is on line, 1 if inside the border, and 2 if outside the border.
int check_border(std::pair<int, int> &check, int lx, int ly,
		 int rx, int ry);

//Requires: route contains at least one integer that refers to a valid coordinate.
//Effects: calculates the cycle distance given route.
double calculate_distance(const std::vector<int> &route, 
			  const std::vector< std::pair<int, int> > &coordinate);

//Requires: route contains at least one integer that refers to a valid coordinate
//Effects: calculates the distance of the route, not the cycle, expressed by route.
double calculate_distance_from_list(const std::vector< std::pair<int, int> > &coordinate, 
				    const std::vector <int> &route);

//Requires: Distances is full of the distances to and from each point specified in route_so_far
//Effects: returns true if the current path is a promising solution, false if the lower bound
//calculated results in a path greater than the best_distance.
bool promising(const std::vector<int> &route_so_far, const std::vector< std::vector<double> > 
	       &distances, double distance_so_far, double best_distance);

//Requires: The union of path_so_far and unvisited_vertices is the full list of vertices to create
//a hamiltonian cycle.
//Effects: Will find the best distance given the full list of vertices and write it to best_path.
//Modifies: path_so_far, unvisited_vertices, best_path, best_distance
void gen_permutations(std::vector<int> &path_so_far, std::queue<int> &unvisited_vertices,
		      std::vector<int> &best_path, std::vector< std::vector<double> > &distances,
		      double &best_distance, std::vector< std::pair<int, int> > &coordinate_list);
