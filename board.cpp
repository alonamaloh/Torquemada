#include "board.hpp"

decltype(NW) * const Board::forward_directions[2][2] = {{NW, NE}, {SW, SE}};
decltype(NW) * const Board::backward_directions[2][2] = {{SE, SW}, {NE, NW}};
decltype(NW) * const Board::all_directions[4] = { NE, NW, SW, SE };
