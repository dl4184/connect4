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
    int b_col = -1;
    int score = 43;
    int tmp_score;

    for (int col = 0; col < 7; col++) {
        if (P.isWinningMove(col)) {
            b_col = col;
            break;
        } else if (P.canPlay(col)) {
            P.playCol(col);

            tmp_score = solver.solve(P, weak);
            //cout<<"Score "<< col <<": "<<tmp_score<<endl;
            if (tmp_score < score) {
                b_col = col;
                score = tmp_score;
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
    cout << "State of the board" << endl;
    string line;
    getline(cin, line);

    if (line.size()>0){
        if (P.play(line) != line.size()) {
            int col = stoi(line) - 1;
            if (P.isWinningMove(col)) {
                cout << "WIN" << endl;
                return 1;
            }
            else
                cerr << "Invalid move " << (P.nbMoves() + 1) << " \"" << line << endl;
        }
    }


    int b_col = best_col(P, solver, weak);
    cout << (b_col + 1) << endl;
    return 0;
}
