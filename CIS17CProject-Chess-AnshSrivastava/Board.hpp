#ifndef BOARD_HPP 
#define BOARD_HPP 

#include "coordinate.hpp"
#include "Move.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <set>
#include <cmath>
#include <utility>
#include <map>
#include <list>

namespace chess
{
  enum { EMPTY, WP, BP, WK, BK, WQ, BQ, WB, BB, WN, BN, WR, BR};
  enum Player { BLACK, WHITE, NOPLAYER};

  Player pieceToPlayer(int piece);
  int getFileFromChar(char c);
  int getRankFromChar(char c);

  class Board
  {
     private:
      std::queue<Player> currentMove;
      std::vector<std::vector<int>> board;
      std::string getPieceName(int);
      bool validateMove(Move move);
      std::stack<Move> moves;
      std::multiset<Move> legalMoves;
      std::pair <bool, bool> canCastleKingside;
      std::pair <bool, bool> canCastleQueenside;
      std::list <int> capturedPieces;
      bool canEnpassant;
      void forceMove(Move move);
    public:
      int getPlayer() { return currentMove.front(); }
      bool isGameOver();
      Board();
      void printBoard();
      bool makeAMove(Move move);
      void fillSet();
      int getPiece(int file, int rank) { return board[file][rank]; }
      bool whiteIsUnderCheck();
      bool blackIsUnderCheck();
      bool isSquareThreatenedForWhite(chess::coordinate c);
      bool isSquareThreatenedForBlack(chess::coordinate c);
      bool castleKingside();
      bool castleQueenside();
      bool enpassant();
      std::pair<int, int> getPieceScore();
  };
  inline std::string cin()
  {
    std::string ret;
    std::getline(std::cin, ret);
    return ret;
  }
}

#endif
