#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

const int GRID_SIZE = 9;
const int BOX_SIZE = 3;

class Grid {
public:
    vector<vector<int>> grid;

    Grid() : grid(GRID_SIZE, vector<int>(GRID_SIZE, 0)) {}

    vector<int> getRow(int row) {
        return grid[row];
    }

    vector<int> getColumn(int col) {
        vector<int> column(GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; ++i) {
            column[i] = grid[i][col];
        }
        return column;
    }

    vector<int> getBox(int row, int col) {
        vector<int> box;
        int boxRowStart = (row / BOX_SIZE) * BOX_SIZE;
        int boxColStart = (col / BOX_SIZE) * BOX_SIZE;

        for (int i = 0; i < BOX_SIZE; ++i) {
            for (int j = 0; j < BOX_SIZE; ++j) {
                box.push_back(grid[boxRowStart + i][boxColStart + j]);
            }
        }
        return box;
    }

    // clone
    Grid clone() const {
        Grid newGrid;
        newGrid.grid = grid;
        return newGrid;
    }

    // serialize
    string serialize() const {
        stringstream ss;
        for (const auto& row : grid) {
            for (int num : row) {
                ss << num;
            }
        }
        return ss.str();
    }

    // deserialize
    static Grid deserialize(const string &data) {
        Grid newGrid;
        if (data.length() != GRID_SIZE * GRID_SIZE) {
            throw invalid_argument("Invalid serialized data length.");
        }
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                newGrid.grid[i][j] = data[i * GRID_SIZE + j] - '0';
            }
        }
        return newGrid;
    }

    // compare
    bool operator==(const Grid &other) const {
        return grid == other.grid;
    }
};

class Sudoku : public Grid {
public:
    Sudoku() : Grid() {}

    void parse(const string &input) {
        if (input.length() != GRID_SIZE * GRID_SIZE) {
            throw invalid_argument("Input must be a string of length 81.");
        }
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                char ch = input[i * GRID_SIZE + j];
                grid[i][j] = ch - '0'; // Convert char to int
            }
        }
    }

    vector<set<int>> getInference() {
        vector<set<int>> candidates(GRID_SIZE * GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (grid[i][j] == 0) {
                    set<int> usedNumbers;

                    // get used numbers
                    vector<int> row = getRow(i);
                    vector<int> col = getColumn(j);
                    vector<int> box = getBox(i, j);

                    for (int num : row) usedNumbers.insert(num);
                    for (int num : col) usedNumbers.insert(num);
                    for (int num : box) usedNumbers.insert(num);

                    // get candidates
                    for (int num = 1; num <= 9; ++num) {
                        if (usedNumbers.find(num) == usedNumbers.end()) {
                            candidates[i * GRID_SIZE + j].insert(num);
                        }
                    }
                }
            }
        }
        return candidates;
    }
};

// test function
void testSudoku(const string &input, const string &expectedSerialization, const vector<set<int>> &expectedInference) {
    Sudoku sudoku;
    try {
    	// test parse
        sudoku.parse(input);
        string serialized = sudoku.serialize();
        cout << "Serialized: " << serialized << endl;
        cout << "Expected: " << expectedSerialization << endl;
        if (serialized == expectedSerialization) {
            cout << "Serialization Test passed!" << endl;
        } else {
            cout << "Serialization Test failed!" << endl;
        }

        // test getInference 
        vector<set<int>> candidates = sudoku.getInference();
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
            if (candidates[i] == expectedInference[i]) {
                cout << "Inference Test for cell " << i << " passed!" << endl;
            } else {
                cout << "Inference Test for cell " << i << " failed!" << endl;
            }
        }

    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}

int main() {
    // test case 
    testSudoku("017903600000080000900000507072010430000402070064370250701000065000030000005601720", 
                "017903600000080000900000507072010430000402070064370250701000065000030000005601720",
                { {2, 4, 5, 8},{},{},{},{2, 4, 5},{},{},{4, 8},{2, 4, 8},{2, 3, 4, 5, 6},{2, 3, 4, 5},{3, 6},{1, 2, 5, 7},{},{4, 5, 6, 7},{1, 3, 9},{1, 4, 9},{1, 2, 3, 4, 9},{},{2, 3, 4, 8},{3, 6, 8},{1, 2},{2, 4, 6},{4, 6},{},{1, 4, 8},{},{5, 8},{},{},{5, 8},{},{5, 6, 8, 9},{},{},{6, 8, 9},{1, 3, 5, 8},{3, 5, 8, 9},{3, 8, 9},{},{5, 6, 9},{},{1, 8, 9},{},{1, 6, 8, 9},{1, 8},{},{},{},{},{8, 9},{},{},{1, 8, 9},{},{2, 3, 4, 8, 9},{},{2, 8},{2, 4, 9},{4, 8, 9},{3, 8, 9},{},{},{2, 4, 6, 8},{2, 4, 8, 9},{6, 8, 9},{2, 5, 7, 8},{},{4, 5, 7, 8, 9},{1, 8, 9},{1, 4, 8, 9},{1, 4, 8, 9},{3, 4, 8},{3, 4, 8, 9},{},{},{4, 9},{},{},{},{3, 4, 8, 9} });

    
    return 0;
}

