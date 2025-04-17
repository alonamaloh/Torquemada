#include "board.hpp"
#include <random>

long perft(Board const &b, int depth) {
  Move moves[128];
  std::size_t n_moves = b.generate_moves(moves);

  if (depth == 1)
    return n_moves;

  long result = 0;
  for (std::size_t i = 0; i < n_moves; ++i) {
    Move move = moves[i];
    Board copy = b;
    copy.make_move(move);
    result += perft(copy, depth - 1);
  }
  
  return result;
}

int main() {
  Board board;
  for (int i = 1; i <= 12; ++i)
    std::cout << i << ' ' << perft(board, i) << std::endl;
}
