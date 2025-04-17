#pragma once

#include "bitboard.hpp"
#include <cstdint>
#include <iosfwd>
#include <functional>

// Simple position: pieces[side] + kings + side_to_move + reversible_plies
// – fully spelled‑out names, now with centralized setters/getters to trigger hooks.

using Move = Bitboard;

struct Board {
  Bitboard pieces[2]        { 0x00000fffu, 0xfff00000u }; // opening pawns
  Bitboard kings            { 0 };                        // no kings yet
  uint8_t  side_to_move     { 0 };                        // 0 = white to move
  uint8_t  reversible_plies { 0 };                        // plies since last pawn push/capture

  Bitboard occupied()     const { return pieces[0] | pieces[1]; }
  Bitboard own_pieces()    const { return pieces[side_to_move]; }
  Bitboard enemies()  const { return pieces[side_to_move ^ 1]; }
  Bitboard own_pawns()     const { return own_pieces() & ~kings; }
  Bitboard own_kings()     const { return own_pieces() &  kings; }
  bool     draw25()        const { return reversible_plies >= 50; }

  void make_move(Move move);
  
  static std::uint64_t mix(std::uint64_t h, std::uint64_t x) {
    h += x;
    h *= 0xf1f1dd0843bad02dULL;
    h ^= h >> 32;
    return h;
  }

  std::uint64_t hash() const {
    std::uint64_t h = 0ULL;
    h = mix(h, pieces[0]);
    h = mix(h, pieces[1]);
    h = mix(h, kings);
    h = mix(h, reversible_plies);
    h = mix(h, side_to_move);
    return h;
  }

  static decltype(NW) * const forward_directions[2][2];
  static decltype(NW) * const backward_directions[2][2];
  static decltype(NE)* const all_directions[4];

  std::size_t generate_moves(Move *moves) const;  
};

std::ostream& operator<<(std::ostream& os, Board const& b);

