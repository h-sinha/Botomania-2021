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
int WIN = 1000;
int DRAW = 0;
int LOSS = -1000;

struct coord{
	int x, y, z;
};
coord* same[6][6][6];

std::vector<coord*> get_legal_moves(int hexagons[6][6][6])
{
	std::vector<bool> tmp(6);
	std::vector<std::vector<bool> > vc(6, tmp);
	std::vector<std::vector<std::vector<bool> > > done(6, vc);
	std::vector<coord*> legal_moves;
	
	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 6; ++j){
			for (int k = 0; k < 6; ++k){
				if(hexagons[i][j][k] == 0 && !done[i][j][j]){
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

pair<int, coord*> minimax_optimization(int hexagons[6][6][6], int marker, int depth, int alpha, int beta){
	// Initialize best move
	coord* best_move = new coord({-1, -1, -1});
	int best_score = (marker == OP_MARKER) ? LOSS : WIN;

	std::vector<coord*> legal_moves = get_legal_moves(hexagons);
	if(int(legal_moves.size()) == 0){
		return make_pair(best_score, best_move);
	}
	
	return make_pair(best_score, best_move);
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
    minimax_optimization(hexagons, MY_MARKER, 0, LOSS, WIN);
	return 0;
}