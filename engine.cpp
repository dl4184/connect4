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
#include <algorithm>
#include <vector>

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
    int score = 43;
    int tmp_score;

    for (int col = 0; col < 7; col++) {
        if (P.isWinningMove(col)) {
            b_col = col;
            break;
        } else if (P.canPlay(col)) {
            P.playCol(col);

            tmp_score = solver.solve(P, weak);
            cout<<"Score for col. "<< (col+1) <<": "<<tmp_score<<endl;
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
 * First you decide if the first player will be the engine or the second.
 * After that you give numbers from 1 to 7 as an input, where numbers represent the column in which the token is played.
 * Program returns:
 *  1 - if the first player has won
 *  2 - if the second player has won
 *  3 - if there was a draw
 */
int main(int argc, char **argv) {
    string line;
    Position P;
    int b_col;
    Solver solver;
    bool weak = false;


    // loading the opening book
    string opening_book = "C:\\Magistrsko_delo\\connect4\\7x6.book";
    solver.loadBook(opening_book);

    // change this to true if you want to see the board state
    bool print_board = false;

    // given the input we detirmine if the first player is the engine
    cout << "Is first player engine? (y/n)" << endl;
    string first_engine;
    getline(cin, first_engine);
    bool first = first_engine[0] == 'y';


    // if engine is first we play the first move
    if (first) {
        b_col = best_col(P, solver, weak);
        cout << "Best move: " << (b_col + 1) << endl;
        P.playCol(b_col);

        if (print_board) {
            P.print_board();
            cout << endl;
        }
    }


    // we read the standard input and play tokens accordingly
    for (int l = 1; getline(cin, line); l++) {
        if (P.play(line) != line.size()) {
            int col = stoi(line) - 1;
            if (P.isWinningMove(col)) {
                cout << "WIN" << endl;
                return first + 1;
            } else if (P.nbMoves() == 42) // we played the last move and no one won -> it is a draw
                return 3;
            else
                cerr << "Line " << l << ": Invalid move " << (P.nbMoves() + 1) << " \"" << line << endl;
        } else {
            b_col = best_col(P, solver, weak);

            if (print_board) {
                P.print_board();
                cout << endl;
            }

            cout << "Best move: " << (b_col + 1) << endl;
            if (P.isWinningMove(b_col)) {
                cout << "WIN" << endl;
                return (not first) + 1;
            }

            P.playCol(b_col);

            // we played the last move and no one won -> it is a draw
            if (P.nbMoves() == 42)
                return 3;


            if (print_board) {
                P.print_board();
                cout << endl;
            }

        }

    }
}
