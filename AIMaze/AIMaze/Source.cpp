//MazeMaster
//This program will pull in a maze file from an outside source. It will then introduce
// an agent to the maze. The agent will use a breadth first (queue) and width first (stack)
// algorithm to make its way through the maze.
//
//Programmer: Ryan Shoemake
//
//Start: 9/13/2018
//

#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <queue>

using namespace std;

//I put this variable in here to be able to change the size of the map with just one variable.
const int MAX_NUM_GRID = 10;

struct Maze {
	string mark;
	int row;
	int col;
	bool visited = false;
};

struct MapNode {
	string marker;
	int row;
	int col;
	MapNode* north;
	MapNode* south;
	MapNode* east;
	MapNode* west;
};

struct Agent {
	string mark;
	int row;
	int col;
};

bool GetUserInput(ifstream &YourFile);
void ReadFile(ifstream &YourFile, Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID]);
void PrintMaze(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID]);
int FindEntrance(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID]);
void GrowTree(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID], queue<Maze>& myqueue, int row, int col);
void GrowTree(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID], stack<Maze>& mystack, int row, int col);



int main(int argc, char *argv[]) {
	ifstream YourFile;
	Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID];
	MapNode *myNode = NULL;
	stack<Maze> mystack;
	queue<Maze> myqueue;

	// First we are going to check if the user sent in the name of the file using the command line.
	if (argc == 2) {
		bool done = false;
		//Continue
		string YourInput = argv[1];
		while (!done) {
			YourFile.open(YourInput.c_str());
			//Error checking to make sure the file opens.
			if (YourFile.is_open()) {
				cout << "Opening " << YourInput << " was successful.\n";
				done = true;
			}
			else {
				cout << "Failure to open " << YourInput << " !\n";
				//system("pause");
				//return 1;
			}
		}
	}
	// They did not use the command line. They will need to be prompted to enter a file name.
	else {
		if (!GetUserInput(YourFile))
			return 1;
	}

	//This is main. Functions that will be called in order of
	// Read in the user file ( file and map)
	// Print out the maze to show the user the correct maze ( map )
	// And then I think I'll do a Navigate ( map, agent)
	//		Inside navigate I think I'll have a findEntrance <------------ And maybe later have this search the entire map
	// 

	ReadFile(YourFile, yourMaze);
	PrintMaze(yourMaze);

	system("pause");

	//Getting the coords for entrance.
	int row = FindEntrance(yourMaze);
	int col = 0;
	
	//QUEUE---------------------------------------------------------------------------------------------------------------
	bool solution = false;
	myqueue.push(yourMaze[row][col]);
	while (!myqueue.empty()) {
		PrintMaze(yourMaze);
		if (myqueue.front().mark == "X") {
			solution = true;
			break;
		}
		row = myqueue.front().row;
		col = myqueue.front().col;
		myqueue.pop();
		GrowTree(yourMaze, myqueue, row, col);
		//if (myqueue.front().mark == "X") {
			//solution = true;
			//break;
		//}
		//else
			//myqueue.pop();
		//cout << "Not Done yet\n";
		
	}
	if (solution) {
		cout << "Solution found with queue at " << myqueue.front().row << " " << myqueue.front().col << endl;
		yourMaze[myqueue.front().row][myqueue.front().col].mark = "*";
		PrintMaze(yourMaze);
	}
	else
		cout << "No Solution\n";
	system("pause");
	//QUEUE DONE------------------------------------------------------------------------------------------------
	
	//Reset the input file.
	YourFile.clear();
	YourFile.seekg(0, ios::beg);
	//Rebuild the map.
	ReadFile(YourFile, yourMaze);
	PrintMaze(yourMaze);
	cout << "Reset the map for stack\n\n";
	system("pause");
	
	
	//STACK-----------------------------------------------------------------------------------------------------
	//Reseting the coords for entrance.
	row = FindEntrance(yourMaze);
	col = 0;
	//

	solution = false;
	mystack.push(yourMaze[row][col]);
	while (!mystack.empty()) {
		PrintMaze(yourMaze);
		if (mystack.top().mark == "X") {
			solution = true;
			break;
		}
		row = mystack.top().row;
		col = mystack.top().col;
		mystack.pop();
		GrowTree(yourMaze, mystack, row, col);
		//if (mystack.top().mark == "X") {
			//solution = true;
			//break;
		//}
		//cout << "Not Done yet\n";
		//system("pause");

	}
	if (solution) {
		cout << "Solution found with stack at " << mystack.top().row << " " << mystack.top().col << endl;
		yourMaze[mystack.top().row][mystack.top().col].mark = "*";
		PrintMaze(yourMaze);
	}
	else
		cout << "No Solution\n";
	//STACK END-------------------------------------------------------------------------------------------------------
	
	system("pause");
	//Close files and get out.
	YourFile.close();
	return 0;
}

bool GetUserInput(ifstream &YourFile) {
	string YourInput;
	bool done = false;
	while (!done) {
		cout << "Please enter a file name that will be read.\n";
		cin >> YourInput;
		YourFile.open(YourInput.c_str());
		if (YourFile.is_open()) {
			cout << "Opening " << YourInput << " was successful.\n";
			done = true;
			return true;
		}
		else {
			cout << "Failure to open " << YourInput << " !\n";
			//system("pause");
			//return false;
		}
	}
}

void ReadFile(ifstream &YourFile, Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID]) {

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			YourFile >> yourMaze[i][j].mark;
			yourMaze[i][j].row = i;
			yourMaze[i][j].col = j;
		}
	}

}

void PrintMaze(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++)
			cout << yourMaze[i][j].mark << "  ";
		cout << "\n";
	}
	cout << endl;
}

int FindEntrance(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID]) {
	bool done = false;
	int x = 0;
	while (!done) {
		if (yourMaze[x][0].mark == "E")
			return x;
		x++;
	}
}

void GrowTree(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID], queue<Maze>& myqueue,int row, int col) {

	if (yourMaze[row - 1][col].mark == "X" ||
		(yourMaze[row - 1][col].mark == "P" &&
			yourMaze[row - 1][col].visited == false)) {
		//cout << "Growing North!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		myqueue.push(yourMaze[row - 1][col]);
		//Root->north = new MapNode;
	}
	if (yourMaze[row][col + 1].mark == "X" ||
		(yourMaze[row][col + 1].mark == "P" &&
			yourMaze[row][col + 1].visited == false)) {
		//cout << "Growing East!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		myqueue.push(yourMaze[row][col + 1]);
		//Root->east = new MapNode;
	}
	if (yourMaze[row + 1][col].mark == "X" ||
		(yourMaze[row + 1][col].mark == "P" &&
			yourMaze[row + 1][col].visited == false)) {
		//cout << "Growing South!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		myqueue.push(yourMaze[row + 1][col]);
		//Root->south = new MapNode;
	}
	if (yourMaze[row][col - 1].mark == "X" ||
		(yourMaze[row][col - 1].mark == "P" &&
			yourMaze[row][col - 1].visited == false)) {
		//cout << "Growing West!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		myqueue.push(yourMaze[row][col - 1]);
		//Root->west = new MapNode;
	}
}

void GrowTree(Maze yourMaze[MAX_NUM_GRID][MAX_NUM_GRID], stack<Maze>& mystack, int row, int col) {

	if (yourMaze[row - 1][col].mark == "X" ||
		(yourMaze[row - 1][col].mark == "P" &&
			yourMaze[row - 1][col].visited == false)) {
		//cout << "Growing North!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		mystack.push(yourMaze[row - 1][col]);
		//Root->north = new MapNode;
	}
	if (yourMaze[row][col + 1].mark == "X" ||
		(yourMaze[row][col + 1].mark == "P" &&
			yourMaze[row][col + 1].visited == false)) {
		//cout << "Growing East!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		mystack.push(yourMaze[row][col + 1]);
		//Root->east = new MapNode;
	}
	if (yourMaze[row + 1][col].mark == "X" ||
		(yourMaze[row + 1][col].mark == "P" &&
			yourMaze[row + 1][col].visited == false)) {
		//cout << "Growing South!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		mystack.push(yourMaze[row + 1][col]);
		//Root->south = new MapNode;
	}
	if (yourMaze[row][col - 1].mark == "X" ||
		(yourMaze[row][col - 1].mark == "P" &&
			yourMaze[row][col - 1].visited == false)) {
		//cout << "Growing West!\n";
		//yourMaze[myqueue.front().row][myqueue.front().col].visited = true;
		yourMaze[row][col].mark = "*";
		mystack.push(yourMaze[row][col - 1]);
		//Root->west = new MapNode;
	}
}
