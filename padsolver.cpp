#include <iostream>
#include <cstdlib>
#include <vector>
#include <getopt.h>
#include <string>

using namespace std;

/***************************************************************
Functions:
1) Finds specified # combo given max move count
Future Functions:
1) Include combo # of a specific color
2) Include specific shape of combo (rows, 5s, crosses, etc.)
3) Include cascades
4) Include enhanced orbs
***************************************************************/

void solve(vector<vector<char>> board, vector<char>* solution, int max_moves, int curr_moves);
bool check(vector<vector<char>> board);
bool prune();

int main(int argc, char* argv[]){
	bool show_board = 1;
	int board_w = 0, board_h = 0, max_moves = 0, combo_count = 0, x_coor = 0, y_coor = 0;
	char mode;
	vector<char> temp_row_vec;
	vector<char> solution;
	vector<char> temp_sol;
	vector<vector<char>> board;

	//include parameters for future functions later
	cout << "Enter d (default) or c (custom): ";
	cin >> mode;
	if (mode == 'd'){
		board_w = 6;
		board_h = 5;
	}
	else if (mode == 'c'){
		cout << "Board width: ";
		cin >> board_w;
		cout << "Board height: ";
		cin >> board_h;
		cout << "Show board? 1 = Yes, 0 = No: ";
		cin >> show_board;
	}
	else {
		cerr << "Error: specify d (default) or c (custom)\n";
		exit(1);
	}

	//making blank board
	for (int i = 0; i < board_w; ++i){
		temp_row_vec.push_back(' ');
	}
	for (int i = 0; i < board_h; ++i){
		board.push_back(temp_row_vec);
	}

	cout << "Max number of moves: ";
	cin >> max_moves;

	cout << "Set up the board (in caps):\n";

	//NOTE: figure out how to represent enhanced orbs
	int setup_counter = 0;
	char temp_orb = ' ';
	for (int i = 0; i < board_h; ++ i){
		for (int j = 0; j < board_w; ++j){
			cout << "R = red, B = blue, G = green, L = light, D = dark, J = jammer";
			cout << " P = poison, M = mortal poison, E = erase previous orb\n";

			//displays board is specified
			if (show_board == 1){
				for (int k = 0; k < board_h; ++k){
					for (int l = 0; l < board_w; ++l){
						cout << board[k][l] << " ";
					}
					cout << "\n";
				}
			}

			//keeps asking to enter an orb for current spot until correct orb is inputted
			while (temp_orb != 'R' && temp_orb != 'B' && temp_orb != 'G' && temp_orb != 'L' && 
				   temp_orb != 'D' && temp_orb != 'J' && temp_orb != 'P' && temp_orb != 'M' && 
				   temp_orb != 'E'){

				cout << "Enter orb: ";
				cin >> temp_orb;

				if (temp_orb != 'R' && temp_orb != 'B' && temp_orb != 'G' && temp_orb != 'L' && 
				    temp_orb != 'D' && temp_orb != 'J' && temp_orb != 'P' && temp_orb != 'M' && 
				    temp_orb != 'E'){

					cout << "Error: bad orb, try again\n"; 
				}
			}

			//If you wanna erase previous orb
			if (temp_orb == 'E'){
				if (j == 0){
					board[i - 1][board_w - 1] = ' ';
					--i;
					j += board_w - 1;
				}
				else {
					board[i][j - 1] = ' ';
					--j;
				}
			}
			else {
				board[i][j] = temp_orb;
			}

			temp_orb = ' ';
		}
	}

	//display final board
	//NOTE: cannot take back last orb input; maybe have option for that?
	if (show_board == 1){
		cout << "Initial Board:\n";
		for (int k = 0; k < board_h; ++k){
			for (int l = 0; l < board_w; ++l){
				cout << board[k][l] << " ";
			}
			cout << "\n";
		}
	}

	return 0;
};

void solve(vector<vector<char>> board, vector<char>* solution, vector<char> curr_sol,
		   int max_moves, int curr_moves, int* sol_moves, int combo_count, int x_coor,
		   int y_coor){
	//if over max moves specified, stop current path
	if (curr_moves > max_moves){
		return;
	}
	//check if current board is valid board for solution
	//update solution if curr solution is better
	//priority: combo count > moves needed
	//NOTE: can only count match 3s at the moment
	if (check(board, &combo_count)){
		if (curr_moves < sol_moves){
			solution = curr_sol;
			sol_moves = curr_moves;
		}
	}

	//does one move: up, down, left or right
	//col#,row# (x,y)
	char temp_char = board[x_coor][y_coor];
	if (prune()){
		//move right
		board[x_coor][y_coor] = board[x_coor + 1][y_coor];
		board[x_coor + 1][y_coor] = temp_char;
		solve(board, &solution, curr_sol, max_moves, curr_moves + 1, sol_moves, combo_count, x_coor + 1, y_coor);
		board[x_coor + 1][y_coor] = board[x_coor][y_coor];
		board[x_coor][y_coor] = temp_char;
	}
	if (prune()){
		//move left
		board[x_coor][y_coor] = board[x_coor - 1][y_coor];
		board[x_coor - 1][y_coor] = temp_char;
		solve(board, &solution, curr_sol, max_moves, curr_moves + 1, sol_moves, combo_count, x_coor - 1, y_coor);
		board[x_coor - 1][y_coor] = board[x_coor][y_coor];
		board[x_coor][y_coor] = temp_char;
	}
	if (prune()){
		//move up
		board[x_coor][y_coor] = board[x_coor][y_coor + 1];
		board[x_coor][y_coor + 1] = temp_char;
		solve(board, &solution, curr_sol, max_moves, curr_moves + 1, sol_moves, combo_count, x_coor, y_coor + 1);
		board[x_coor][y_coor + 1] = board[x_coor][y_coor];
		board[x_coor][y_coor] = temp_char;
	}
	if (prune()){
		//move down
		board[x_coor][y_coor] = board[x_coor][y_coor - 1];
		board[x_coor][y_coor - 1] = temp_char;
		solve(board, &solution, curr_sol, max_moves, curr_moves + 1, sol_moves, combo_count, x_coor, y_coor - 1);
		board[x_coor][y_coor - 1] = board[x_coor][y_coor];
		board[x_coor][y_coor] = temp_char;
	}
	return;
}

bool check(vector<vector<char>> board, int* combo_count){
	

	return true;
}

bool prune(vector<vector<char>> board, int x_coor, int y_coor, char direc){

	if (direc == 'r' && x_coor == board.size() - 1){
		return false;
	}
	else if (direc == 'l' && x_coor == 0){
		return false;
	}
	else if (direc == 'u' && y_coor == 0){
		return false;
	}
	else if (direc == 'd' && y_coor == board[0].size() - 1){
		return false;
	}

	return true;
}
