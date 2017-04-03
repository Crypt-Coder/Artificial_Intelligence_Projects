#include <bits/stdc++.h>
#include <unistd.h>			// Using this library to use usleep function
#define SLEEP_TIME 1000000  // Set the sleep time(micro sec) to be passed to usleep
using namespace std;

/*
 * A* Algorithm
 * Open List: priority_queue
 * Closed List: map
 * Heuristic: Manhatten Distance + Path traversed to reach here --> h() + g()
 * Initial State: Provide a initial state in same manner as goal state
 * as represented below
 */
 
/*
 * Initializing the goal state
 * You can change the goal state as you wish just by changing the string below
 * Default goal state is as follows:
 * 1 2 3
 * 4 5 6
 * 7 8  
 * This is represented in a string as "12345678 "
 */
string goal = "12345678 ";

/*
 * This DS is used to maintain closed list in 
 * the A* algorithm
 * Key --> Check whether state was visited
 * Value --> Store the parent of current state (used to backtrack to find 
 * the path back to the initial state to display all the states at the end)
 */
map<string,string> m;

/*
 * Helper function to print the string in original grid form
 */
void printer(string game) {
	for(int i = 0;i < 9;i++) {
		cout << game[i] << " ";
		if ((i+1)%3 == 0)
			cout << endl;
	}
	cout << endl;
}

/*
 * Used to calculate the heuristic value of the current state
 * Heuristic value is the Manhatten Distance of each node in that state
 */
int calculate_heuristic(string curr) {
	int heuristic = 0,val;
	for(int i = 0;i < 3;i++) {
		for(int j = 0;j < 3;j++) {
			val = goal[i*3+j] - '0';
			if (val == ' ' - '0')	break;
			for(int ii = 0;ii < 3;ii++) {
				for(int jj = 0;jj < 3;jj++) {
					if (val == (curr[ii*3+jj] - '0')) {
						heuristic += abs(i - ii) + abs(j - jj);
						break;
					}
				}
			}
		}
	}
	return heuristic;
}

/*
 * Code to implement A* algorithm
 * Returns  0 if goal state reached
 * Returns -1 if failed
 */
int A_star(string curr) 
{
	/* 
	 * Priority queue to implement open list
	 * Format: (h() + g(), g(), current state, parent of current state)
	 * Note: The heuristic's are stored in negative coz priority_queue returns max value
	 */
	priority_queue<pair<pair<int, int>, pair<string, string> > > q;

	q.push(make_pair(make_pair(-(calculate_heuristic(curr) + 1), 1), make_pair(curr, curr)));
	while(!q.empty()) {

		int heuristic = q.top().first.first;
		int path_val  = q.top().first.second; 
		string state  = q.top().second.first;
		string parent = q.top().second.second;
		string next_parent = state;
		int spaceind = 0;
		q.pop();

		/*
		 * Check if current state is already present in the closed list
		 * If present discard current state and move to the next state
		 */
		if (m.find(state) != m.end())
			continue;
		/*
		 * Place the state in closed list, to mark it as visited
		 * The parent_state is stored as value for backtracking to the path later
		 */
		m[state] = parent;

		/* If heuristic is 0, means goal state is reached
		 * Here we have to check only h()
		 */
		if ((heuristic + path_val) == 0) {
			return 0;
		}
		// Find index of space in the current state
		for(int i = 0;i < 9;i++) {
			if (state[i] == ' ') {
				spaceind = i;
			}
		}
		// Start Generating its child states

		/*
		 * Move space Up
		 * 1 2 3 		  1   3
		 * 6   4   --->   6 2 4
		 * 8 7 5 		  8 7 5
		 * Check if condition space is not initially in first row
		 */
		if (spaceind - 3 > 0) {
			swap(state[spaceind], state[spaceind-3]);
			// Add the child state to open list
			q.push(make_pair(make_pair(-(calculate_heuristic(state) + path_val + 1), path_val + 1), make_pair(state, next_parent)));
			// Get back to original state
			swap(state[spaceind], state[spaceind-3]);
		}
		/*
		 * Move space Down
		 * 1   3 		  1 2 3
		 * 6 2 4   --->   6   4
		 * 8 7 5 		  8 7 5
		 * Check if condition space is not initially in last row
		 */
		if (spaceind + 3 < 9) {
			swap(state[spaceind], state[spaceind+3]);
			// Add the child state to open list
			q.push(make_pair(make_pair(-(calculate_heuristic(state) + path_val + 1), path_val + 1), make_pair(state, next_parent)));
			// Get back to original state
			swap(state[spaceind], state[spaceind+3]);
		}
		/*
		 * Move space left
		 * 1 2 3 		  1 2 3
		 * 6 4     --->   6   4
		 * 8 7 5 		  8 7 5
		 * Check if condition space is not initially in first column
		 */
		if (spaceind % 3 != 0) {
			swap(state[spaceind], state[spaceind-1]);
			// Add the child state to open list
			q.push(make_pair(make_pair(-(calculate_heuristic(state) + path_val + 1), path_val + 1), make_pair(state, next_parent)));
			// Get back to original state
			swap(state[spaceind], state[spaceind-1]);	
		}
		/*
		 * Move space right
		 * 1 2 3 		  1 2 3
		 * 6   4   --->   6 4
		 * 8 7 5 		  8 7 5
		 * Check if condition space is not initially in last column
		 */
		if ((spaceind + 1) % 3 != 0) {
			swap(state[spaceind], state[spaceind+1]);
			// Add the child state to open list
			q.push(make_pair(make_pair(-(calculate_heuristic(state) + path_val + 1), path_val + 1), make_pair(state, next_parent)));
			// Get back to original state
			swap(state[spaceind], state[spaceind+1]);
		}
	}
	/*
	 * If code reaches here, there is a problem in the input provided
	 * Possibly the input is not solvable
	 * Source:- http://math.stackexchange.com/questions/293527/how-to-check-if-a-8-puzzle-is-solvable
	 */
	return -1;
}
/*
 * Helper to print the path using backtracking from goal
 * state to initial state using the closed list
 */
int print_path(string state) {

	if (m[state] == state) {
		usleep(SLEEP_TIME);
		printer(state);
		return 1;
	}
	int val = print_path(m[state]);
	usleep(SLEEP_TIME);
	printer(state);
	return 1 + val;
}
/*
 * Driver Code
 */
int main(int argc, char const *argv[])
{
	int total_states = 0;
	char *curr = NULL;
	curr = (char *)malloc(sizeof(char)*9);
	scanf("%[^\n]s",curr);
	
	if (A_star(curr) == -1) {
		cout << "Found Some Issue while solving\n\n";
		return 0;
	}
	/*
	 * Print the path from initial to goal state
	 * Recursive function to print the path in correct order(rather than reverse)
	 */
	total_states = print_path(goal);
	cout << "Total No. of states: " << total_states << endl;
	return 0;
}