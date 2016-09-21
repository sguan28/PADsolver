#include <iostream>
#include <cstdlib>
#include <vector>
#include <getopt.h>
#include <string>

using namespace std;

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

int main(){

	vector<vector<char>> board;
	vector<char> col;
	//adjust values to change board
	int height = 5;
	int width = 6;

	for (int i = 0; i < height; ++i){
		col.push_back('a');
	}
	for (int i = 0; i < width; ++i){
		board.push_back(col);
	}
	//cout << board[0].size() << endl;

	cout << "testing (0,0):\n";
	cout << "u: " << prune(board, 0, 0, 'u') << endl;
	cout << "d: " << prune(board, 0, 0, 'd') << endl;
	cout << "l: " << prune(board, 0, 0, 'l') << endl;
	cout << "r: " << prune(board, 0, 0, 'r') << endl;

	cout << "testing (end,end):\n";
	cout << "u: " << prune(board, width - 1, height - 1, 'u') << endl;
	cout << "d: " << prune(board, width - 1, height - 1, 'd') << endl;
	cout << "l: " << prune(board, width - 1, height - 1, 'l') << endl;
	cout << "r: " << prune(board, width - 1, height - 1, 'r') << endl;

	return 0;
}