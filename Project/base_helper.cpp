#include "base_helper.h"

bool is_adjacent(const std::pair<int, int> &first, const std::pair<int, int> &second,
		 int left_x, int left_y, int right_x, int right_y){
  bool first_in = true, second_in = true;
  //If either point is on the boundary, then regardless of where
  //the other point is, the two are adjacent.
  if(((first.first == left_x || first.first == right_x) &&
      (first.second <= right_y && first.second >= left_y)) || 
     ((first.second == left_y || first.second == right_y) &&
      (first.first <= right_x && first.first >= left_x))){
    return true;
  }
  else if(((second.first == left_x || second.first == right_x) &&
	   (second.second <= right_y && second.second >= left_y)) ||
	  ((second.second == left_y || second.second == right_y) &&
	   (second.first <= right_x && second.first >= left_x))){
    return true;
  }
  //check if first is inside or outside of the boundary.
  if(first.first > left_x && first.first < right_x && 
     first.second > left_y && first.second < right_y){
    first_in = true;
  }
  else{
    first_in = false;
  }
  
  //check if second is inside or outside of the boundary.
  if(second.first > left_x && second.first < right_x &&
     second.second > left_y && second.second < right_y){
    second_in = true;
  }
  else{
    second_in = false;
  }
  //If they are both in or both out, then true, if not false.
  if(first_in && second_in){
    return true;
  }
  else if(!first_in && !second_in){
    return true;
  }
  else{
    return false;
  }
  

}

int check_border(std::pair<int, int> &check, int lx, int ly,
		 int rx, int ry){

  if(((check.first == lx || check.first == rx) &&
      (check.second <= ry && check.second >= ly)) ||
     ((check.second == ly || check.second == ry) &&
      (check.first <= rx && check.first >= lx))){
    return 0;
  }
  else if(check.first < rx && check.first > lx &&
	  check.second < ry && check.second > ly){
    return 1;
  }
  else{
    return 2;
  }

}

double calculate_distance(const std::vector<int> &route, 
			  const std::vector< std::pair<int, int> > &coordinate){
  double distance = 0;
  for(unsigned int i = 0; i < route.size() - 1; ++i){
    distance = distance + sqrt(pow(coordinate[route[i]].first - coordinate[route[i + 1]].first, 2) +
			       pow(coordinate[route[i]].second - coordinate[route[i + 1]].second, 2));
  }
  distance = distance + sqrt(pow(coordinate[route[route.size() - 1]].first -
				 coordinate[0].first, 2) +
			     pow(coordinate[route[route.size() - 1]].second -
				 coordinate[0].second, 2));
  return distance;

}

double calculate_distance_from_list(const std::vector< std::pair<int, int> > &coordinate, const std::vector <int> &route){
  double distance = 0;
  for(unsigned int i = 0; i < route.size() - 1; ++i){
    distance = distance + sqrt(pow(coordinate[route[i]].first - coordinate[route[i + 1]].first, 2) +
			       pow(coordinate[route[i]].second - coordinate[route[i + 1]].second, 2));
  }
  return distance;
}

bool promising(const std::vector<int> &route_so_far, const std::vector< std::vector<double> > 
	       &distances, double distance_so_far, double best_distance){
  std::vector<bool> visited(distances.size(), false);
  
  if(distances.size() - route_so_far.size() <= 3){
    return true;
  }
  std::pair<double, double> first_point (-1, -1);
  double new_bound = distance_so_far;
  for(unsigned int i = 0; i < route_so_far.size(); ++i){
    visited[route_so_far[i]] = true;
  }
  for(unsigned int i = 0; i < distances.size(); ++i){
    if(!visited[i]){
      std::pair<double, double> best_pair (-1, -1);
      for(unsigned int j = 0; j < distances.size(); ++j){
	if(!visited[j]){
	  if(best_pair.first == -1){
	    best_pair.first = distances[i][j];
	  }
	  else if(best_pair.second == -1){
	    best_pair.second = distances[i][j];
	  }
	  else if(best_pair.first > distances[i][j] && best_pair.second > distances[i][j]){
	    if(best_pair.first <= best_pair.second){
	      best_pair.second = distances[i][j];
	    }
	    else{
	      best_pair.first = distances[i][j];
	    }
	  }
	  else{
	    if(best_pair.first > distances[i][j]){
	      best_pair.first = distances[i][j];
	    }
	    else if(best_pair.second > distances[i][j]){
	      best_pair.second = distances[i][j];
	    }
	  }
	  
	}
	
      }
      
      new_bound = new_bound + (best_pair.first + best_pair.second)/2;
    }
  }
  
  if(new_bound > best_distance && new_bound != -1){
    return false; 
  }
  else{
    return true;
  }
}

void gen_permutations(std::vector<int> &path_so_far, std::queue<int> &unvisited_vertices,
		      std::vector<int> &best_path, std::vector< std::vector<double> > &distances,
		      double &best_distance, std::vector< std::pair<int, int> > &coordinate_list){
  double distance_so_far = -1;
  
  if(unvisited_vertices.empty()){
    if(calculate_distance(path_so_far, coordinate_list) < best_distance){
      best_distance = calculate_distance(path_so_far, coordinate_list);
      best_path = path_so_far;
    }
    return;
    //check to see if better than the one we already have.
  }
  for(unsigned int i = 0; i < unvisited_vertices.size(); ++i){
    path_so_far.push_back(unvisited_vertices.front()); 
    unvisited_vertices.pop();
    if(!path_so_far.empty()){
      distance_so_far = calculate_distance_from_list(coordinate_list, path_so_far);

    }
    if(promising(path_so_far, distances, distance_so_far, best_distance)){
      gen_permutations(path_so_far, unvisited_vertices, best_path, distances, best_distance, 
		       coordinate_list);
    }
    unvisited_vertices.push(path_so_far.back());
    path_so_far.pop_back();
  }

}

