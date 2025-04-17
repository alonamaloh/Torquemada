#include "board.hpp"
#include <random>

Move random_move(Board const &b) {
  Move moves[128];
  std::size_t n_moves = b.generate_moves(moves);
  if (n_moves == 0)
    return 0;

  static std::mt19937_64 rng;
  
  return moves[rng() % n_moves];
}

int main() {
  for (int game = 0; game < 1000; ++game) {
    Board b;
    while (1) {
      std::cout << "-----\n" << b << '\n';
      
      Move m = random_move(b);
      if (m == 0)
	break;
      b.make_move(m);
    }
  }
}
