#pragma once

#include <cstdint>
#include <iostream>

using Bitboard = std::uint32_t; // one bit per light square, 0..31
  
constexpr Bitboard NE(Bitboard b) {
  return ((b & 0x0E0E0E0E) << 3) | ((b & 0x00F0F0F0) << 4);
}
  
constexpr Bitboard NW(Bitboard b) {
  return ((b & 0x0F0F0F0F) << 4) | ((b & 0x00707070) << 5);
}
  
constexpr Bitboard SE(Bitboard b) {
  return ((b & 0xF0F0F0F0) >> 4) | ((b & 0x0E0E0E0E) >> 5);
}
  
constexpr Bitboard SW(Bitboard b) {
  return ((b & 0x70707070) >> 3) | ((b & 0x0F0F0F0F) >> 4);
}
  
class Iterate {
  Bitboard x;
    
public:
  explicit constexpr Iterate(Bitboard x) : x(x) {}
    
  class iterator {
    Bitboard b;
  public:
    explicit constexpr iterator(Bitboard b) : b(b) {}
    constexpr bool operator!=(const iterator& o) const { return b != o.b; }
    constexpr Bitboard operator*() const { return b & -b; }
    constexpr iterator& operator++() { b &= b - 1; return *this; }
  };
    
  constexpr iterator begin() const { return iterator(x); }
  constexpr iterator end() const { return iterator(0); }
};
  
// ────────────────────────────────────────────────────────────
// Very small ASCII renderer – handy while unit‑testing.
// ────────────────────────────────────────────────────────────
inline std::ostream& print(std::ostream &os, Bitboard b) {
  for (int row = 7; row >= 0; --row) {
    if (row % 2 == 0)
      os << "  ";
    for (int col = 3; col >= 0; --col)
      os << "_*"[(b >> (4 * row + col)) & 1] << "   ";
    os << '\n';
  }
  os << std::flush;
  return os;
}
