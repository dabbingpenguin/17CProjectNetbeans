#ifndef COORDINATE_HPP
#define COORDINATE_HPP

namespace chess
{
enum { BLANK, A, B, C, D, E, F, G, H};

struct coordinate
{
  int rank;
  int file;
  coordinate()
  {
   file = 0;
   rank = 0;
  }
};
}

#endif
