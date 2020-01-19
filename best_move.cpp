/*
 * This file is part of Connect4 Game Solver <http://connect4.gamesolver.org>
 * Copyright (C) 2017-2019 Pascal Pons <contact@gamesolver.org>
 *
 * Connect4 Game Solver is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Connect4 Game Solver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Connect4 Game Solver. If not, see <http://www.gnu.org/licenses/>.
 */


#include <cstdlib>
#include <ctime>
#include "Solver.hpp"
#include <sstream>
#include <algorithm>

using namespace GameSolver::Connect4;
using namespace std;
//using position_t = uint64_t;



/**
 Returns the best column to play for a given state of the board.

 @params
    - Position &P - class storing a Connect 4 position
    - Solver &solver - class for determining the score of a postion using negamax
    - bool weak - weak solver - false

 @returns column (integer) that is the best play for a given state

*/
int best_col(Position &P, Solver &solver, bool weak) {
    int scores[7] = {43, 43, 43, 43, 43, 43, 43};

    for (int col = 0; col < 7; col++) {
        if (P.isWinningMove(col)) {
            return col;
        } else if (P.canPlay(col)) {
            P.playCol(col);

            scores[col] = solver.solve(P, weak);
            cout << "Score " << col << ": " << scores[col] << endl;
            P.undo(col);
        }
    }

    int min_el = *min_element(scores, scores + 7);
    vector<int> best_ind;
    for (int col = 0; col < 7; col++)
        if (min_el == scores[col]) {
            best_ind.push_back(col);
            cout << col << endl;
        }
    srand((unsigned) time(0));
    int ri = rand() % best_ind.size();

    return best_ind[ri];
}

/**
 *
 * @param str a string which we split by space " "
 * @return split words as a vector of strings
 */

vector<string> split(string str) {
    vector<string> res;
    string word = "";
    for (auto x : str) {
        if (x == ' ') {
            res.push_back(word);
            word = "";
        } else
            word = word + x;
    }
    res.push_back(word);

    return res;
}


/**
 * Main function.
 * We provide the params describing the state of the board (current position, mask and number of moves e.g. 44040192 65011712 5)
 * Based on the board state engine outputs the best move.
 */
int main(int argc, char **argv) {

    Position P;
    Solver solver;
    bool weak = false;


    // loading the opening book
    string opening_book = "C:\\Magistrsko_delo\\connect4\\7x6.book";
    solver.loadBook(opening_book);


    // provide the state of the board described with current position, mask and number of moves
    cout << "Provide parameters describing the state of the board:" << endl;
    string line;
    getline(cin, line);

    vector<string> param = split(line);
    uint64_t v1, v2;
    istringstream p1(param[0]);
    istringstream p2(param[1]);

    p1 >> v1;
    p2 >> v2;

    P.set_param(v1, v2, stoi(param[2]));

    int b_col = best_col(P, solver, weak);
    cout << "Best next move is:" << endl;
    cout << (b_col + 1) << endl;
    return (b_col + 1);
}