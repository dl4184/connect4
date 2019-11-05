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

#include "Solver.hpp"
#include <iostream>
#include <sys/time.h>
#include <vector>
#include <algorithm>

using namespace GameSolver::Connect4;
using namespace std;



/**
 Returns the best column to play for a given state of the board.

 @params
    - Position &P - class storing a Connect 4 position
    - Solver &solver - class for determining the score of a postion using negamax
    - bool weak - weak solver - false

 @returns column (integer) that is the best play for a given state

*/
int best_col(Position &P, Solver &solver, bool weak) {
    int b_col = -1;
    int best_ind = 90;
    int tmp_ind, score;

    vector<int>::iterator iter;
    vector<int> score_table;

    for (int i = 1; i < 43; i++)
        score_table.push_back(i);
    score_table.push_back(0);
    for (int i = -42; i < 0; i++)
        score_table.push_back(i);


    for (int col = 0; col < 7; col++) {
        if (P.isWinningMove(col)) {
            b_col = col;
            break;
        } else if (P.canPlay(col)) {
            P.playCol(col);

            score = solver.solve(P, weak);
            iter = find(score_table.begin(), score_table.end(), score);
            tmp_ind = distance(score_table.begin(), iter);

            if (tmp_ind < best_ind) {
                best_ind = tmp_ind;
                b_col = col;
            }
            P.undo(col);
        }
    }
    return b_col;
}


/**
 * Main function.
 * First the player gives a sequence of moves (with indecies from 1 to 7).
 * Based on the board state engine outputs the best move.
 */
int main(int argc, char **argv) {

    Position P;
    Solver solver;
    bool weak = false;


    // loading the opening book
    string opening_book = "C:\\Magistrsko_delo\\connect4\\7x6.book";
    solver.loadBook(opening_book);


    // give the state of the board as move sequence (e.g. "4343" means player first played 4th colum then the second
    // player played the 3rd column and so on). If there is only newline then the board is empty
    cout << "Provide the state of the board as a sequence of moves:" << endl;
    string line;
    getline(cin, line);

    if (line.size() > 0) {
        if (P.play(line) != line.size()) {
            int col = stoi(line) - 1;
            if (P.isWinningMove(col)) {
                cout << "WIN" << endl;
                return 0;
            } else {
                cerr << "Invalid move " << (P.nbMoves() + 1) << " \"" << line << " \"" << endl;
                return -1;
            }
        }
    }


    int b_col = best_col(P, solver, weak);
    cout << "Best next move is:" << endl;
    cout << (b_col + 1) << endl;
    return (b_col + 1);
}
