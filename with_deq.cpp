#include <bits/stdc++.h>
using namespace std;

#define DEBUG
#ifdef DEBUG
#define debug(...) __f(#__VA_ARGS__, __VA_ARGS__)
	template <typename Arg1>
	void __f(const char* name, Arg1&& arg1){
		cerr << name << " : " << arg1 << std::endl;
	}
	template <typename Arg1, typename... Args>
	void __f(const char* names, Arg1&& arg1, Args&&... args){
		const char* comma = strchr(names + 1, ','); cerr.write(names, comma - names) << " : " << arg1<<" | ";__f(comma+1, args...);
	}
#else
#define debug(...)
#endif

int OP_MARKER;
int MY_MARKER;
int WIN = 3600;
int DRAW = 0;
int LOSS = -3600;
std::chrono::steady_clock::time_point start_time;
long double timelimit = 1000;

struct coord{
	int x, y, z;
};
coord* same[6][6][6];
pair<int, coord*> ans;
	
deque<coord*> get_legal_moves(int hexagons[6][6][6])
{
	std::vector<bool> tmp(6);
	std::vector<std::vector<bool> > vc(6, tmp);
	std::vector<std::vector<std::vector<bool> > > done(6, vc);
	deque<coord*> legal_moves;
	
	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 6; ++j){
			for (int k = 0; k < 6; ++k){
				if(hexagons[i][j][k] == 0 && !done[i][j][k]){
					legal_moves.push_back(new coord({i, j, k}));
					done[i][j][k] = 1;
					if(same[i][j][k])
						done[same[i][j][k]->x][same[i][j][k]->y][same[i][j][k]->z] = 1;
				}
			}
		}
	}
	return legal_moves;
}

// returns number of new hexagons made in current move
int get_hex_made(int hexagons[6][6][6], coord* move){
	int count = 0, total = 0;
	for (int i = 0; i < 6; ++i){
		// not occupied by anyone;
		if(!hexagons[move->x][move->y][i])break;
		count++;
	}
	if(count == 6)total++;
	count = 0;
	if(same[move->x][move->y][move->z]){
		coord* nmove = same[move->x][move->y][move->z];
		for (int i = 0; i < 6; ++i){
			// not occupied by anyone;
			if(!hexagons[nmove->x][nmove->y][i])break;
			count++;
		}
	}
	if(count == 6)total++;
	return total;
}
void go(){
	cout << ans.second->x<<" "<<ans.second->y<<" "<<ans.second->z<<endl;
	exit(0);
}
void undo_move(int hexagons[6][6][6], deque<coord*>&legal_moves, coord* move, coord* nmove){
	hexagons[move->x][move->y][move->z] = 0;
	if(nmove)hexagons[nmove->x][nmove->y][nmove->z] = 0;
	legal_moves.push_back(move);
}
pair<int, coord*> minimax_optimization(int hexagons[6][6][6], deque<coord*>&legal_moves, int marker, int depth, int alpha, int beta, int my_score, int op_score){
	auto end = std::chrono::steady_clock::now();

	if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count() > timelimit)go();

	// Initialize best move
	coord* best_move = new coord({-1, -1, -1});
	int best_score = (marker == MY_MARKER) ? LOSS : WIN;
	// std::vector<coord*> legal_moves = get_legal_moves(hexagons);
	// No move available
	if(int(legal_moves.size()) == 0){
		if(marker == MY_MARKER)best_score = my_score;
		else best_score = op_score;
		return make_pair(best_score, best_move);
	}

	int cur_inc, score;
	if(depth <= 0){
		// TODO: think of a heuristic
		return make_pair(my_score - op_score, best_move);
	}
	int total_moves = legal_moves.size();
	int it = 0;
	coord* move;
	while(it < total_moves){
		it++;
	// for(auto move:legal_moves){
		auto end = std::chrono::steady_clock::now();
		if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count() > timelimit)go();
		move = legal_moves.front();
		legal_moves.pop_front();
		if(hexagons[move->x][move->y][move->z]){
			legal_moves.push_back(move);
			continue;
		}
		// make move
		hexagons[move->x][move->y][move->z] = marker;
		coord* nmove = NULL;
		if(same[move->x][move->y][move->z]){
			nmove = same[move->x][move->y][move->z];
			hexagons[nmove->x][nmove->y][nmove->z] = marker;
		}
		cur_inc = get_hex_made(hexagons, move);
		
		// Maximizing player's turn
		if (marker == MY_MARKER){
			// no new hex made
			if(!cur_inc)
				score = minimax_optimization(hexagons, legal_moves, OP_MARKER, depth - 1, alpha, beta, my_score, op_score).first;
			else
				score = minimax_optimization(hexagons, legal_moves, MY_MARKER, depth - 1, alpha, beta, my_score + cur_inc * 100, op_score).first;
			// Get the best scoring move
			if (best_score < score){
				best_score = score;
				best_move = move;

				// Check if this branch's best move is worse than the best
				// option of a previously search branch. If it is, skip it
				alpha = max(alpha, best_score);
				
				if (beta <= alpha){ 
					undo_move(hexagons, legal_moves, move, nmove);
					break; 
				}
			}

		} 
		// Minimizing opponent's turn
		else{
			if(!cur_inc)
				score = minimax_optimization(hexagons, legal_moves, MY_MARKER, depth - 1, alpha, beta, my_score, op_score).first;
			else
				score = minimax_optimization(hexagons, legal_moves, OP_MARKER, depth - 1, alpha, beta, my_score, op_score + cur_inc * 100).first;

			if (best_score > score){
				best_score = score;
				best_move = move;

				// Check if this branch's best move is worse than the best
				// option of a previously search branch. If it is, skip it
				beta = min(beta, best_score);

				if (beta <= alpha){ 
					undo_move(hexagons, legal_moves, move, nmove);
					break; 
				}
			}

		}
		undo_move(hexagons, legal_moves, move, nmove);
	}
	return make_pair(best_score, best_move);
}
int main()
{
	start_time = std::chrono::steady_clock::now();
	// start_time = time(NULL);
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
    	same[x[0]][x[1]][x[2]] = new coord({y[0], y[1], y[2]});
        same[y[0]][y[1]][y[2]] = new coord({x[0], x[1], x[2]});
    }
	int hexagons[6][6][6];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
            	cin >> hexagons[i][j][k];
        	}
        }
    }
    auto v = get_legal_moves(hexagons);
    ans = make_pair(0, new coord({v[v.size()-1]->x, v[v.size()-1]->y, v[v.size()-1]->z}));
    int cur_depth = 1;
    while(1){ 
    	auto end = std::chrono::steady_clock::now();
		if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count() > timelimit)break;
    	ans = minimax_optimization(hexagons, v, MY_MARKER, cur_depth, LOSS, WIN, 0, 0);
		cur_depth++;
	}
	// assert(v.size() > 0);
	// assert(ans.second->x !=  -1);
	cout << ans.second->x <<" "<<ans.second->y<<" "<<ans.second->z<<endl;
	return 0;
}