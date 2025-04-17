#include "board.hpp"

decltype(NW) * const Board::forward_directions[2][2] = {{NW, NE}, {SW, SE}};
decltype(NW) * const Board::backward_directions[2][2] = {{SE, SW}, {NE, NW}};
decltype(NW) * const Board::all_directions[4] = { NE, NW, SW, SE };

void Board::make_move(Move move) {
  Bitboard captures = enemies() & move;
  Bitboard from_xor_to = move ^ captures;
  bool pawn_moved = from_xor_to & own_pawns();
  bool capture = (captures != 0u);
  
  if (from_xor_to & kings)
    kings ^= from_xor_to;
  pieces[side_to_move] ^= from_xor_to;
  
  pieces[side_to_move ^ 1] &= ~captures;
  kings &= ~captures;
  
  static const Bitboard promotion_row[2] = {0xf0000000u, 0x0000000fu};
  kings |= own_pawns() & promotion_row[side_to_move];
  
  reversible_plies = (pawn_moved || capture) ? 0 : uint8_t(reversible_plies + 1);
  side_to_move = side_to_move ^ 1;
}

namespace {
  struct PawnCaptureGenerator {
    Bitboard can_capture[2];
    uint8_t side_to_move;
    
    PawnCaptureGenerator(Board const &b) : side_to_move(b.side_to_move) {
      for (int d = 0; d < 2; ++d) {
	auto backward = Board::backward_directions[side_to_move][d];
	can_capture[d] = (*backward)(b.enemies() & (*backward)(~b.occupied()));
      }
    }

    std::size_t generate(Move move, Bitboard from, Move *moves) {
      std::size_t n_moves = 0;
      
      for (int d = 0; d < 2; ++d) {
	auto forward = Board::forward_directions[side_to_move][d];
	if (from & can_capture[d]) {
	  Bitboard to = (*forward)((*forward)(from));
	  Move m = move ^ (from | (*forward)(from) | to);
	  if (to & (can_capture[0] | can_capture[1]))
	    n_moves += generate(m, (*forward)((*forward)(from)), moves + n_moves);
	  else
	    moves[n_moves++] = m;
	}
      }
      
      return n_moves;
    }
  };
  
  struct KingCaptureGenerator {
    Bitboard enemy;
    uint8_t side;

    KingCaptureGenerator(Board const &b)
      : enemy(b.enemies()),
        side(b.side_to_move)
    {}

    std::size_t generate(Move move,
                         Bitboard from,
                         Bitboard occ,
                         Move *out) const
    {
      std::size_t cnt = 0;

      for (int d = 0; d < 4; ++d) {
        auto dir = Board::all_directions[d];

        Bitboard x = from;
        while (true) {
          x = (*dir)(x);
          if (!x) break;
          if (x & occ) {
            if (!(x & (enemy & ~move))) break;
	    Bitboard y = x;
	    
	    while (1) {
	      y = (*dir)(y) & ~occ;
	      if (!y)
		break;
	      
              Move m2 = move ^ from ^ x ^ y;

              auto sub = generate(m2, y, occ, out + cnt);
              if (sub == 0) {
                out[cnt++] = m2;
              } else {
                cnt += sub;
              }
            }
            break;
          }
        }
      }

      return cnt;
    }
  };
}

std::size_t Board::generate_moves(Move *moves) const {
  std::size_t n_moves = 0;
  
  PawnCaptureGenerator pcg(*this);
  for (Bitboard from : Iterate(own_pawns() & (pcg.can_capture[0] | pcg.can_capture[1])))
    n_moves += pcg.generate(0u, from, moves + n_moves);
  
  KingCaptureGenerator kcg(*this);
  for (Bitboard from : Iterate(own_kings()))
    n_moves += kcg.generate(0u, from, occupied() ^ from, moves + n_moves);
  
  // "Ley de cantidad"
  int max_captured = 0;
  std::size_t write;
  for (std::size_t read = write = 0; read < n_moves; ++read) {
    int n_captured = std::popcount(moves[read] & enemies());
    if (n_captured < max_captured)
      continue;
    if (n_captured > max_captured) {
      max_captured = n_captured;
      write = 0;
    }
    if (n_captured == max_captured)
      moves[write++] = moves[read];
  }
  n_moves = write;
  
  // Remove duplicates
  if (max_captured >= 2) {
    std::sort(moves, moves + n_moves);
    std::size_t write;
    for (std::size_t read = write = 0; read < n_moves; ++read) {
      if (read == 0 || moves[read] != moves[read - 1])
	moves[write++] = moves[read];
    }
    n_moves = write;
  }
  
  if (max_captured > 0)
    return n_moves;
  
  Bitboard empty = ~occupied();
  
  // Pawn non-captures
  for (int d = 0; d < 2; ++d) {
    auto backward = backward_directions[side_to_move][d];
    auto forward = forward_directions[side_to_move][d];
    for (Bitboard from : Iterate(own_pawns() & (*backward)(empty)))
      moves[n_moves++] = from ^ (*forward)(from);
  }
  
  // King non-captures
  for (int d = 0; d < 4; ++d) {
    auto forward = all_directions[d];
    auto backward = all_directions[d ^ 2];
    for (Bitboard from : Iterate(own_kings() & (*backward)(~occupied()))) {
      for (Bitboard to = (*forward)(from); to; to = (*forward)(to) & empty)
	moves[n_moves++] = from ^ to;
    }
  }
  
  return n_moves;
}

