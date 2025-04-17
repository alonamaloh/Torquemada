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
  for (int i = 1; i <= 11; ++i)
    std::cout << i << ' ' << perft(board, i) << std::endl;

  std::cout << "-----\n";
  
  board.pieces[0] = 0x00000007u;
  board.pieces[1] = 0x00000008u;
  board.kings = 0x0000000fu;
  for (int i = 1; i <= 7; ++i)
    std::cout << i << ' ' << perft(board, i) << std::endl;
  
}
