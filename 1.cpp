#include <bits/stdc++.h>
using namespace std;
int OP_MARKER;
int MY_MARKER;
int WIN = 1000;
int DRAW = 0;
int LOSS = -1000;

struct move{
	int x, y, z;
};
move* same[6][6][6];

move* get_legal_moves(int hexagons[6][6][6])
{
	std::vector<bool> tmp;
	std::vector<std::vector<bool> v;> tmp;
	std::vector<std::vector<std::vector<bool> > > done(6, vc);

	std::vector<move*> legal_moves;
	
	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 6; ++j){
			for (int k = 0; k < 6; ++k){
				if(hexagons[i][j][k] == 0 && !done[i][j][j]){
					legal_moves.push_back(new move({i, j, k}));
					done[i][j][k] = 1;
					done[same[i][j][k].x][same[i][j][k].y][same[i][j][k].z] = 1;
				}
			}
		}
	}
	return legal_moves;
}

pair<int, move*> minimax_optimization(int hexagons[6][6][6], int marker, int depth, int alpha, int beta){
	// Initialize best move
	move* best_move = new move({-1, -1, -1});
	int best_score = (marker == OP_MARKER) ? LOSS : WIN;

	// // If we hit a terminal state (leaf node), return the best score and move
	// if (board_is_full(board) || DRAW != get_board_state(board, OP_MARKER))
	// {
	// 	best_score = get_board_state(board, OP_MARKER);
	// 	return std::make_pair(best_score, best_move);
	// }

	std::vector<std::pair<int, int>> legal_moves = get_legal_moves(board);
	if(int(legal_moves.size()) == 0){
		return best_score;
	}
	// for (int i = 0; i < legal_moves.size(); i++)
	// {
	// 	std::pair<int, int> curr_move = legal_moves[i];
	// 	board[curr_move.first][curr_move.second] = marker;

	// 	// Maximizing player's turn
	// 	if (marker == OP_MARKER)
	// 	{
	// 		int score = minimax_optimization(board, MY_MARKER, depth + 1, alpha, beta).first;

	// 		// Get the best scoring move
	// 		if (best_score < score)
	// 		{
	// 			best_score = score - depth * 10;
	// 			best_move = curr_move;

	// 			// Check if this branch's best move is worse than the best
	// 			// option of a previously search branch. If it is, skip it
	// 			alpha = std::max(alpha, best_score);
	// 			board[curr_move.first][curr_move.second] = EMPTY_SPACE;
	// 			if (beta <= alpha) 
	// 			{ 
	// 				break; 
	// 			}
	// 		}

	// 	} // Minimizing opponent's turn
	// 	else
	// 	{
	// 		int score = minimax_optimization(board, OP_MARKER, depth + 1, alpha, beta).first;

	// 		if (best_score > score)
	// 		{
	// 			best_score = score + depth * 10;
	// 			best_move = curr_move;

	// 			// Check if this branch's best move is worse than the best
	// 			// option of a previously search branch. If it is, skip it
	// 			beta = std::min(beta, best_score);
	// 			board[curr_move.first][curr_move.second] = EMPTY_SPACE;
	// 			if (beta <= alpha) 
	// 			{ 
	// 				break; 
	// 			}
	// 		}

	// 	}

	// 	board[curr_move.first][curr_move.second] = EMPTY_SPACE; // Undo move

	// }

	// return std::make_pair(best_score, best_move);
	return best_move;
}
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> MY_MARKER;
	OP_MARKER = 3 - MY_MARKER;

	int num_edge_pairs;
    cin >> num_edge_pairs;

    int x[3], y[3];
    for (int i = 0; i < num_edge_pairs; ++i){
    	for (int j = 0; j < 3; ++j)cin >> x[j];
    	for (int j = 0; j < 3; ++j)cin >> y[j];
    	same[x[0]][x[1]][x[2]] = new move({y[0], y[1], y[2]});
        same[y[0]][y[1]][y[2]] = new move({x[0], x[1], x[2]});
    }

	int hexagons[6][6][6];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
            	cin >> hexagons[i][j][k];
        	}
        }
    }
    minimax_optimization(hexagons, MY_MARKER, 0, LOSS, WIN);
	return 0;
}