#ifndef MOVE_HPP
#define MOVE_HPP

#include "coordinate.hpp"

namespace chess
{
  struct Move
  {
    coordinate firstSquare;
    coordinate lastSquare;

    bool operator <(const Move& d) 
    {
      return (firstSquare.file * firstSquare.rank < lastSquare.file * lastSquare.rank);
    }

    bool operator > (const Move& d)
    {
  return (firstSquare.file * firstSquare.rank < lastSquare.file * lastSquare.rank);
    }
    bool operator== (const Move& mov)
    {
      return (firstSquare.file == mov.firstSquare.file   &&
              firstSquare.rank == mov.firstSquare.rank   &&
              lastSquare.file == mov.lastSquare.file     &&
              lastSquare.rank == mov.lastSquare.rank       );
    }
  };

  inline bool operator <(const Move& d, const Move& e) 
  {
    if     (d.firstSquare.file == e.firstSquare.file   &&
            d.firstSquare.rank == e.firstSquare.rank   &&
            d.lastSquare.file == e.lastSquare.file     &&
            d.lastSquare.rank == e.lastSquare.rank       );
            return false;
    return (d.firstSquare.file * (d.firstSquare.rank+1) < e.lastSquare.file * (e.lastSquare.rank+1));
  }

  inline bool operator > (const Move& d, const Move& e)
  {
    if     (d.firstSquare.file == e.firstSquare.file   &&
            d.firstSquare.rank == e.firstSquare.rank   &&
            d.lastSquare.file == e.lastSquare.file     &&
            d.lastSquare.rank == e.lastSquare.rank       );
            return false;
    return (d.firstSquare.file * (d.firstSquare.rank+1) > e.lastSquare.file * (e.lastSquare.rank+1));}

  inline bool operator== (const Move& mov1, const Move& mov2)
  {
    return (mov1.firstSquare.file == mov2.firstSquare.file   &&
            mov1.firstSquare.rank == mov2.firstSquare.rank   &&
            mov1.lastSquare.file == mov2.lastSquare.file     &&
            mov1.lastSquare.rank == mov2.lastSquare.rank       );
  }
}

#endif
