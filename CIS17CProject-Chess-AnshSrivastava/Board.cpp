#include "Board.hpp"

int chess::getFileFromChar(char c)
{
  std::map<char, int> charToFile =
  {
    {'A', chess::A},
    {'B', chess::B},
    {'C', chess::C},
    {'D', chess::D},
    {'E', chess::E},
    {'F', chess::F},
    {'G', chess::G},
    {'H', chess::H},
    {'a', chess::A},
    {'b', chess::B},
    {'c', chess::C},
    {'d', chess::D},
    {'e', chess::E},
    {'f', chess::F},
    {'g', chess::G},
    {'h', chess::H}
  };
  return charToFile[c];
}

int chess::getRankFromChar(char c)
{
  std::map<char, int> charToRank = 
  {
    {'1', 1},
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8}
  };
  return charToRank[c];
}

chess::Player chess::pieceToPlayer(int piece)
{
  std::map<int, chess::Player> pToPlayer = 
  {
    {chess::EMPTY, chess::Player::NOPLAYER},
    {chess::WP, chess::Player::WHITE},
    {chess::WB, chess::Player::WHITE},
    {chess::WR, chess::Player::WHITE},
    {chess::WN, chess::Player::WHITE},
    {chess::WK, chess::Player::WHITE},
    {chess::WQ, chess::Player::WHITE},
    {chess::BP, chess::Player::BLACK},
    {chess::BR, chess::Player::BLACK},
    {chess::BN, chess::Player::BLACK},
    {chess::BB, chess::Player::BLACK},
    {chess::BK, chess::Player::BLACK},
    {chess::BQ, chess::Player::BLACK}
  };
  return pToPlayer[piece];
}

chess::Board::Board()
{
  canEnpassant = true;
  canCastleKingside.first = true;
  canCastleKingside.second = true;
  canCastleQueenside.first = true;
  canCastleQueenside.second = true;
  std::vector<int> temp;
  for (int i=0; i<9; ++i)
    temp.push_back(0);
  for (int i=0; i<9; ++i)
    board.push_back(temp);
  currentMove.push(Player::BLACK);
  currentMove.push(Player::WHITE);
  auto sortQueue = [](std::queue<Player> q)
  {
      std::vector<Player> pVector;
      while (!q.empty())
      {
        pVector.push_back(q.front());
        q.pop();
      }
      std::sort(pVector.begin(), pVector.end(), [] (Player a, Player b)
          {
            if (a == Player::WHITE && b == Player::BLACK)
              return true;
            return false;
          });
      std::queue<Player> ret (std::deque<Player> (pVector.begin(), pVector.end()));
      return ret;
  };
  currentMove = sortQueue(currentMove);
  for (std::vector<std::vector<int>>::iterator it = board.begin() + 1; it != board.end(); ++it)
  {
    (*it)[2] = WP;
    (*it)[7] = BP;
  }
  board[A][1] = WR;
  board[A][8] = BR;

  board[B][1] = WN;
  board[B][8] = BN;

  board[G][1] = WN;
  board[G][8] = BN;

  board[H][1] = WR;
  board[H][8] = BR;

  board[F][1] = WB;
  board[F][8] = BB;

  board[C][1] = WB;
  board[C][8] = BB;

  board[D][1] = WQ;
  board[D][8] = BQ;

  board[E][1] = WK;
  board[E][8] = BK;
}

std::string chess::Board::getPieceName(int num)
{
  std::map<int, std::string> pieceNames =
  {
    {WP, "WP"},
    {BP, "BP"},
    {WR, "WR"},
    {BR, "BR"},
    {WN, "WN"},
    {BN, "BN"},
    {WB, "WB"},
    {BB, "BB"},
    {WQ, "WQ"},
    {BQ, "BQ"},
    {WK, "WK"},
    {BK, "BK"},
    {EMPTY, "  "}
  };
  return pieceNames[num];
}

void chess::Board::printBoard()
{
    for (std::vector<std::vector<int>>::iterator it = board.end() - 1; it!= board.begin(); --it)
   {
      for (std::vector<int>::iterator jt = it->begin(); jt != it->end(); ++ jt)
      {
        int i = std::distance(board.begin(), it);
        int j = std::distance(it->begin(), jt);
        if (j==0)
        {
          if (i==8)
            std::cout << std::setw(2) << "|8" << std::setfill(' ');
          else
            std::cout << std::setw(2) << i << std::setfill(' ');
        }
        std::cout << std::setw(4) << getPieceName(board[j][i]) << std::setfill('|');  
      }
      std::cout << '\n';
   } 
  std::cout <<  "     " << std::setfill(' ')
            << std::setw(4) << "A" << std::setfill(' ')
            << std::setw(4) << "B" << std::setfill(' ')
            << std::setw(4) << "C" << std::setfill(' ')
            << std::setw(4) << "D" << std::setfill(' ')
            << std::setw(4) << "E" << std::setfill(' ')
            << std::setw(4) << "F" << std::setfill(' ')
            << std::setw(4) << "G" << std::setfill(' ')
            << std::setw(4) << "H" << std::setfill(' ') << "\n\n\n";
  std::cout << "Piece Score for White: " << getPieceScore().first 
            << "\nPiece Score for Black: " << getPieceScore().second
            << '\n';
}

void chess::Board::forceMove(Move mov)
{
  // Check the rules
  // if (!validateMove(mov))
  //  return false;
  // Push to the stack
  moves.push(mov);
  // Make the move
  coordinate initialSquare = mov.firstSquare;
  coordinate finalSquare = mov.lastSquare; // Pawn Promotion
  if (board[initialSquare.file][initialSquare.rank] == WP && finalSquare.rank ==8)
  {
    std::string s;
    while (true)
    {
      std::cout << "Do you want a bishop, knight, rook, or queen :(B/N/R/Q) "; 
      s = chess::cin();
      if (s=="B" || s == "N" || s == "R" || s=="Q")
        break;
      else
        std::cout << "Please type in B,N,R, or Q.\n";
    }
    if (s=="Q")
      board[initialSquare.file][initialSquare.rank] = WQ;
    if (s=="B")
      board[initialSquare.file][initialSquare.rank] = WB;
    if (s=="R")
      board[initialSquare.file][initialSquare.rank] = WR;
    if (s=="N")
      board[initialSquare.file][initialSquare.rank] = WN;
  }

  if (board[initialSquare.file][initialSquare.rank] == BP && finalSquare.rank == 1)
  {
    std::string s;

    while (true)
    {
      std::cout << "Do you want a bishop, knight, rook, or queen :(B/N/R/Q) "; 
      s = chess::cin();
      if (s=="B" || s == "N" || s == "R" || s=="Q")
        break;
      else
        std::cout << "Please type in B,N,R, or Q.\n";
    }
    if (s=="Q")
      board[initialSquare.file][initialSquare.rank] = BQ;
    if (s=="B")
      board[initialSquare.file][initialSquare.rank] = BB;
    if (s=="R")
      board[initialSquare.file][initialSquare.rank] = BR;
    if (s=="N")
      board[initialSquare.file][initialSquare.rank] = BN;
  }

  std::swap(board[initialSquare.file][initialSquare.rank], board[finalSquare.file][finalSquare.rank]);
  if (board[initialSquare.file][initialSquare.rank] != EMPTY)
    capturedPieces.push_back(board[initialSquare.file][initialSquare.rank]);
  board[initialSquare.file][initialSquare.rank] = EMPTY;
  
  if ((initialSquare.file == E && initialSquare.rank == 1) || 
      (finalSquare.file == E && finalSquare.rank == 1))
      {
        canCastleKingside.first = false;
        canCastleQueenside.first = false;
      }
  if ((initialSquare.file == H && initialSquare.rank == 1) || 
      (finalSquare.file == H && finalSquare.rank == 1))
      canCastleKingside.first = false;
  if ((initialSquare.file == H && initialSquare.rank == 8) || 
      (finalSquare.file == H && finalSquare.rank == 8))
      canCastleKingside.second = false;
  if ((initialSquare.file == A && initialSquare.rank == 1) || 
      (finalSquare.file == A && finalSquare.rank == 1))
      canCastleQueenside.first = false;
  if ((initialSquare.file == A && initialSquare.rank == 8) || 
      (finalSquare.file == A && finalSquare.rank == 8))
      canCastleQueenside.second = false;
  fillSet();
}

bool chess::Board::makeAMove(Move mov)
{
  fillSet();
  for (std::multiset<Move>::iterator it = legalMoves.begin();
       it != legalMoves.end();
       ++it)
  {
    Move x = *it;
    if (x.firstSquare.rank == mov.firstSquare.rank &&
        x.firstSquare.file == mov.firstSquare.file &&
        x.lastSquare.rank == mov.lastSquare.rank   &&
        x.lastSquare.file == mov.lastSquare.file     )
    {
    forceMove(mov);
    moves.push(mov);  
    currentMove.push(currentMove.front()); 
    currentMove.pop();
    fillSet();
    //for (Move xy : legalMoves){ std::cout << xy.firstSquare.file << xy.firstSquare.rank << "->" << xy.lastSquare.file << xy.lastSquare.rank << "\t"; }
    //std::cout << '\n';
    return true;
    }
  }
  fillSet();
  return false;
}

bool chess::Board::validateMove(chess::Move mov)
{
  int initialPiece = getPiece(mov.firstSquare.file, mov.firstSquare.rank);
  int finalPiece = getPiece(mov.lastSquare.file, mov.lastSquare.rank);

  auto validRookMove = [&]()
  {
    if (!(mov.firstSquare.file == mov.lastSquare.file || mov.firstSquare.rank == mov.lastSquare.rank))
      return false;

      // Moving Vertically
    if (mov.firstSquare.file == mov.lastSquare.file)
    {
      // Moving Up or Moving Down
      if (mov.firstSquare.rank > mov.lastSquare.rank)
      {
        // Moving Down
        for (int j = mov.firstSquare.rank -1; j>mov.lastSquare.rank; --j)
          if (getPiece(mov.firstSquare.file, j) != EMPTY)
            return false;
      }
      if (mov.firstSquare.rank < mov.lastSquare.rank)
      {
        // Moving Up
        for (int j = mov.firstSquare.rank +1; j<mov.lastSquare.rank; ++j)
          if (getPiece(mov.firstSquare.file, j) != EMPTY)
            return false;
      }
    }
      // Moving Horizontally
    if (mov.firstSquare.rank == mov.lastSquare.rank)
    {
      // Moving Left or Right
      if (mov.firstSquare.file > mov.lastSquare.file)
      {
        // Moving Left
        for (int j = mov.firstSquare.file -1; j>mov.lastSquare.file; --j)
          if (getPiece(j, mov.firstSquare.rank) != EMPTY)
            return false;
      }
      if (mov.firstSquare.file < mov.lastSquare.file)
      {
        // Moving Right
        for (int j = mov.firstSquare.file+1; j<mov.lastSquare.file; ++j)
          if (getPiece(j, mov.firstSquare.rank) != EMPTY)
            return false;
      }
    }
    return true;
  };

  auto validBishopMove = [&]()
  {
    int dx = mov.lastSquare.file - mov.firstSquare.file;  
    int dy = mov.lastSquare.rank - mov.firstSquare.rank;
    if (std::abs(dy) != std::abs(dx))
      return false;
    int norm = std::abs(dx);
    dx = dx/norm;
    dy = dy/norm;
    for (int i=1; i<norm; ++i)
      if (getPiece(mov.firstSquare.file + i*dx, mov.firstSquare.rank + i * dy) != EMPTY)
        return false;
    return true;
  };

  auto validKnightMove = [&] ()
  {
    int dx = std::abs(mov.lastSquare.file - mov.firstSquare.file);  
    int dy = std::abs(mov.lastSquare.rank - mov.firstSquare.rank);

    if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1))
      return true;
    else
      return false;
  };

  auto validPawnMove = [&] ()
  {
    int dx = mov.lastSquare.file - mov.firstSquare.file;  
    int dy = mov.lastSquare.rank - mov.firstSquare.rank;
    // Check y-direction
    if ((initialPiece == WP && dy<0)||(initialPiece == BP && dy>0))
      return false;
    // Pawn can only move 1 or 2 squares in the y-direction under any circumstances
    if (abs(dy) > 2 || dy == 0)
      return false;
    // Incase abs(dy) = 2 during the pawn's first move
    if (abs(dy) == 2)
      {
       if (abs(dx) != 0)
          return false;
       if (initialPiece == WP && mov.firstSquare.rank ==2 && finalPiece == EMPTY)
         return true;
       if (initialPiece == BP && mov.firstSquare.rank == 7 && finalPiece == EMPTY)
         return true;
       return false;
      }
    // Pawn can only move 1 square max in the x-direction
    if (abs(dx) > 1)
      return false;
    if (abs(dx) == 1)
    {
      if (initialPiece == WP)
      {
        if (pieceToPlayer(finalPiece) != Player::BLACK)
          return false;
      }
      else if(initialPiece == BP)
      {
        if (pieceToPlayer(finalPiece) != Player::WHITE)
          return false;
      }
    }
    else if (finalPiece != chess::EMPTY)
      return false;
    return true;
  };

  // You cannot move to the same square you are in
  if (mov.firstSquare.file == mov.lastSquare.file && mov.firstSquare.rank == mov.lastSquare.rank)
  {
    return false;
  }

  // You cannot capture your own piece
  if (chess::pieceToPlayer(initialPiece) == chess::pieceToPlayer(finalPiece))
  {
    return false;
  }

  // Rooks can only move horizontally and vertically
  if (initialPiece==WR || initialPiece==BR)  
    {
      if (!validRookMove())
      {
        return false;
      }
    }
  
  // Bishops can only move diagonally
  if (initialPiece==WB || initialPiece == BB)
  {
    if (!validBishopMove())
    {
      return false;
    }
  }

  // Queen can move like a bishop and a rook
  if (initialPiece==WQ || initialPiece==BQ)
  {
    if (!(validBishopMove() || validRookMove()))
      {
        return false;
      }
  }

  // Knights can only move in L-shapes
  if (initialPiece==WN || initialPiece==BN)
  {
    if (!validKnightMove())
    {
      return false;
    }
  }

  // Kings can move like queen but only 1 square at a time
  if (initialPiece == WK || initialPiece == BK)
  {
    int dx = std::abs(mov.lastSquare.file - mov.firstSquare.file);  
    int dy = std::abs(mov.lastSquare.rank - mov.firstSquare.rank);
    if (! ((validRookMove() || validBishopMove()) && ((abs(dx)==1 && abs(dy) == 0) || (abs(dy)==1 && abs(dx) == 0)|| (abs(dx) == 1 && abs(dy) == 1))))
      return false;
  }

  // Pawns
  if (initialPiece == WP || initialPiece == BP)
  {
    if (!validPawnMove())
    {
      return false;
    }
  }
  return true;
}


bool chess::Board::blackIsUnderCheck()
{
  coordinate c;
  for (std::vector<std::vector<int>>::iterator it = board.begin() + 1;
      it != board.end(); ++it)
  {
    // for (std::vector<int>::iterator jt = it->begin() + 1; 
    //     jt != it->end(); ++jt)
    std::vector<int>::iterator jt = std::find(it->begin(), it->end(), BK);
    if (*jt  == BK && jt != it->end())
      {
        c.file = it - board.begin();
        c.rank = jt - it->begin();
      }
  }
  return isSquareThreatenedForBlack(c);
}

bool chess::Board::whiteIsUnderCheck()
{
  coordinate c;
  for (std::vector<std::vector<int>>::iterator it = board.begin() + 1;
      it != board.end(); ++it)
  {
    // for (std::vector<int>::iterator jt = it->begin() + 1; 
    //     jt != it->end(); ++jt)
    std::vector<int>::iterator jt = std::find(it->begin(), it->end(), WK);
    if (*jt  == WK && jt != it->end())
      {
        c.file = it - board.begin();
        c.rank = jt - it->begin();
      }
  }
  return isSquareThreatenedForWhite(c);
}

bool chess::Board::isSquareThreatenedForWhite(chess::coordinate c)
{
  auto getPieceAt = [&] (int i, int j)
  {
    if (c.file + i > 8 || c.file + i < 1)
      return 0;
    if (c.rank + j > 8 || c.rank + j < 1)
      return 0;
    return board[c.file + i][c.rank + j];
  };

  // Check diagonals
    // top right direction
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(j, j) != EMPTY && getPieceAt(j, j) != WK)  
      {
        if (getPieceAt(j,j) == BB || getPieceAt(j,j) == BQ)
          return true;
        else
          break;
      }
    }
    // Check for pawns
    if (getPieceAt(1, 1) == BP || getPieceAt(-1, 1) == BP)
      return true;
    // top left direction
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(-j, j) != EMPTY && getPieceAt(-j, j) != WK)  
      {
        if (getPieceAt(-j,j) == BB || getPieceAt(-j,j) == BQ)
          return true;
        else
          break;
      }
    }

    // bottom left direction
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(-j, -j) != EMPTY && getPieceAt(-j, -j) != WK)  
      {
        if (getPieceAt(-j, -j) == BB || getPieceAt(-j, -j) == BQ)
          return true;
        else
          break;
      }
    }
     
    // bottom right direction
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(j, -j) != EMPTY && getPieceAt(j, -j) != WK)  
      {
        if (getPieceAt(j, -j) == BB || getPieceAt(-j, -j) == BQ)
          return true;
        else
          break;
      } 
    }
     
  // Check files and ranks
    // Left
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(-j, 0) != EMPTY && getPieceAt(-j, 0) != WK)  
      {
        if (getPieceAt(-j, 0) == BR || getPieceAt(-j, 0) == BQ)
          return true;
        else
          break;
      } 
    }

    // Right
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(j, 0) != EMPTY && getPieceAt(j, 0) != WK)  
      {
        if (getPieceAt(j, 0) == BR || getPieceAt(j, 0) == BQ)
          return true;
        else
          break;
      } 
    }

    // Up
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(0, j) != EMPTY && getPieceAt(0, j) != WK)  
      {
        if (getPieceAt(0, j) == BR || getPieceAt(0, j) == BQ)
          return true;
        else
          break;
      } 
    }

    // Down
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(0, -j) != EMPTY && getPieceAt(0, -j) != WK)  
      {
        if (getPieceAt(0, -j) == BR || getPieceAt(0, -j) == BQ)
          return true;
        else
          break;
      } 
    }

  // Check Knight Squares
  if (getPieceAt(2, 1) == BN || getPieceAt(2, -1) == BN || getPieceAt(-2, 1) == BN || getPieceAt(-2, -1) == BN || getPieceAt(1, 2) == BN || getPieceAt(-1, 2) == BN || getPieceAt(1, -2)==BN || getPieceAt(-1, -2) == BN) 
    return true;
  return false;
}


bool chess::Board::isSquareThreatenedForBlack(chess::coordinate c)
{
  auto getPieceAt = [&] (int i, int j)
  {
    if (c.file + i > 8 || c.file + i < 1)
      return 0;
    if (c.rank + j > 8 || c.rank + j < 1)
      return 0;
    return board[c.file + i][c.rank + j];
  };

  // Check diagonals
    // top right direction
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(j, j) != EMPTY && getPieceAt(j, j) != BK)  
      {
        if (getPieceAt(j,j) == WB || getPieceAt(j,j) == WQ)
          return true;
        else
          break;
      }
    }

    // Top left direction
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(-j, j) != EMPTY && getPieceAt(-j, j) != BK)  
      {
        if (getPieceAt(-j,j) == WB || getPieceAt(-j,j) == WQ)
          return true;
        else
          break;
      }
    }

    // Bottom left direction
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(-j, -j) != EMPTY  && getPieceAt(-j, -j) != BK)  
      {
        if (getPieceAt(-j, -j) == WB || getPieceAt(-j, -j) == WQ)
          return true;
        else
          break;
      }
    }
     
    // bottom right direction
    for (int j=1; j<9; ++j)
   {
      if (getPieceAt(j, -j) != EMPTY  && getPieceAt(j, -j) != BK)  
      {
        if (getPieceAt(j, -j) == WB || getPieceAt(j, -j) == WQ)
          return true;
        else
          break;
      } 
    }
     
    // Check for pawns
    if (getPieceAt(1, -1) == WP || getPieceAt(-1, -1) == WP)
      return true;
  // Check files and ranks
    // Left
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(-j, 0) != EMPTY  && getPieceAt(-j, 0) != BK)    
      {
        if (getPieceAt(-j, 0) == WR || getPieceAt(-j, 0) == WQ)
          return true;
        else
          break;
      } 
    }

    // Right
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(j, 0) != EMPTY  && getPieceAt(j, 0) != BK)  
      {
        if (getPieceAt(j, 0) == WR || getPieceAt(j, 0) == WQ)
          return true;
        else
          break;
      } 
    }

    // Up
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(0, j) != EMPTY && getPieceAt(0, j) != BK)  
      {
        if (getPieceAt(0, j) == WR || getPieceAt(0, j) == WQ)
          return true;
        else
          break;
      } 
    }

    // Down
    for (int j=1; j<9; ++j)
    {
      if (getPieceAt(0, -j) != EMPTY  && getPieceAt(0, -j) != BK)  
      {
        if (getPieceAt(0, -j) == WR || getPieceAt(0, -j) == WQ)
          return true;
        else
          break;
      } 
    }

  // Check Knight Squares
  if (getPieceAt(2, 1) == WN || getPieceAt(2, -1) == WN || getPieceAt(-2, 1) == WN || getPieceAt(-2, -1) == WN || getPiece(1, 2) == WN || getPieceAt(-1, 2) == WN || getPieceAt(1, -2) == WN || getPieceAt(-1, -2) == WN) 
    return true;
  return false;
}

void chess::Board::fillSet()
{
  legalMoves.clear();
  for (int i=1; i<9; ++i)
  {
    for (int j=1; j<9; ++j)
    {
      if (pieceToPlayer(board[i][j]) == currentMove.front())
      {
        for (int k=1; k<9; ++k)
        {
          for (int l=1; l<9; ++l)
          {
            Move mov;
            coordinate initialSquare;
            coordinate finalSquare;
            initialSquare.file = i;
            initialSquare.rank = j;
            finalSquare.file = k;
            finalSquare.rank = l;
            mov.firstSquare = initialSquare;
            mov.lastSquare = finalSquare;
            if (initialSquare.rank == finalSquare.rank && 
                initialSquare.file == finalSquare.file)
              continue;
            if (validateMove(mov))
            {
              int a  = board[mov.firstSquare.file][mov.firstSquare.rank];
              int b = board[mov.lastSquare.file][mov.lastSquare.rank];
              if (currentMove.front() != pieceToPlayer(a))
                continue;
              board[mov.lastSquare.file][mov.lastSquare.rank] = a;
              board[mov.firstSquare.file][mov.firstSquare.rank] = chess::EMPTY;
              if ((currentMove.front() == Player::WHITE && !whiteIsUnderCheck()) || (currentMove.front() == Player::BLACK && !blackIsUnderCheck())) 
                legalMoves.insert(mov);
              board[mov.firstSquare.file][mov.firstSquare.rank] = a;
              board[mov.lastSquare.file][mov.lastSquare.rank] = b;
            }
          }
        }
      } 
    }
  }  
}

bool chess::Board::isGameOver()
{
  fillSet();
  if (legalMoves.empty())
  {
    if (currentMove.front() == Player::WHITE)
    {
      if (whiteIsUnderCheck())
         std::cout << "\nCheckmate. Black won!!!!!!!\n";
      else
         std::cout << "\nStalemate. Game is a draw......\n";
      
    }
    else
    {
      if (blackIsUnderCheck())
         std::cout << "\nCheckmate. White won!!!!!\n";
      else
         std::cout << "\nStalemate. Game is a draw.....\n";
    }
    return true;
  }            
    return false;
}

bool chess::Board::castleKingside()
{
  if (currentMove.front() == Player::WHITE)
  {
    coordinate E1;
    E1.file= chess::E;
    E1.rank = 1;

    coordinate F1;
    F1.file= chess::F;
    F1.rank = 1;

    coordinate G1;
    G1.file= chess::G;
    G1.rank = 1;

    coordinate H1;
    H1.file= chess::H;
    H1.rank = 1;

    if (!canCastleKingside.first || whiteIsUnderCheck() || board[F][1] != EMPTY || board[G][1] != EMPTY || isSquareThreatenedForWhite(F1) || isSquareThreatenedForWhite(G1) || board[H][1] != WR)
      return false;
    Move kingSwap;
    kingSwap.firstSquare = E1;
    kingSwap.lastSquare = G1;
    Move RookSwap;
    RookSwap.firstSquare = H1;
    RookSwap.lastSquare = F1;
    forceMove(kingSwap);
    forceMove(RookSwap);
    canCastleKingside.first = false;
    currentMove.push(currentMove.front());
    currentMove.pop();
  }
  else if (currentMove.front() == Player::BLACK)
  {
    coordinate E8;
    E8.file= chess::E;
    E8.rank = 8;

    coordinate F8;
    F8.file= chess::F;
    F8.rank = 8;

    coordinate G8;
    G8.file= chess::G;
    G8.rank = 8;

    coordinate H8;
    H8.file= chess::H;
    H8.rank = 8;

    if (!canCastleKingside.second || blackIsUnderCheck() || board[F][8] != EMPTY || board[G][8] != EMPTY || isSquareThreatenedForBlack(F8) || isSquareThreatenedForBlack(G8) || board[H][8] != BR)
      return false;
    Move kingSwap;
    kingSwap.firstSquare = E8;
    kingSwap.lastSquare = G8;
    Move RookSwap;
    RookSwap.firstSquare = H8;
    RookSwap.lastSquare = F8;
    forceMove(kingSwap);
    forceMove(RookSwap);
    canCastleKingside.second = false;
    currentMove.push(currentMove.front());
    currentMove.pop();
  }
  fillSet();
  return true;
}


bool chess::Board::castleQueenside()
{
  if (currentMove.front() == Player::WHITE)
  {
    coordinate A1;
    A1.file= chess::A;
    A1.rank = 1;

    coordinate B1;
    B1.file= chess::B;
    B1.rank = 1;

    coordinate C1;
    C1.file= chess::C;
    C1.rank = 1;

    coordinate D1;
    D1.file= chess::D;
    D1.rank = 1;

    coordinate E1;
    E1.file= chess::E;
    E1.rank = 1;

    if (!canCastleQueenside.first || 
        whiteIsUnderCheck() || 
        board[B][1] != EMPTY || 
        board[C][1] != EMPTY || 
        board[D][1] != EMPTY || 
        isSquareThreatenedForWhite(D1) || 
        isSquareThreatenedForWhite(C1) || 
        board[A][1] != WR)
      return false;
    Move kingSwap;
    kingSwap.firstSquare = E1;
    kingSwap.lastSquare = C1;
    Move RookSwap;
    RookSwap.firstSquare = A1;
    RookSwap.lastSquare = D1;
    forceMove(kingSwap);
    forceMove(RookSwap);
    canCastleQueenside.first = false;
    currentMove.push(currentMove.front());
    currentMove.pop();
  }
  else if (currentMove.front() == Player::BLACK)
  {
    coordinate A8;
    A8.file= chess::A;
    A8.rank = 8;

    coordinate B8;
    B8.file= chess::B;
    B8.rank = 8;

    coordinate C8;
    C8.file= chess::C;
    C8.rank = 8;

    coordinate D8;
    D8.file= chess::D;
    D8.rank = 8;

    coordinate E8;
    E8.file= chess::E;
    E8.rank = 8;

    if (!canCastleQueenside.second || 
        blackIsUnderCheck() || 
        board[B][8] != EMPTY || 
        board[C][8] != EMPTY || 
        board[D][8] != EMPTY || 
        isSquareThreatenedForBlack(D8) || 
        isSquareThreatenedForBlack(C8) || 
        board[A][8] != BR)
      return false;
    Move kingSwap;
    kingSwap.firstSquare = E8;
    kingSwap.lastSquare = C8;
    Move RookSwap;
    RookSwap.firstSquare = A8;
    RookSwap.lastSquare = D8;
    forceMove(kingSwap);
    forceMove(RookSwap);
    canCastleQueenside.second = false;
    currentMove.push(currentMove.front());
    currentMove.pop();
  }
  fillSet();
  return true;
}

bool chess::Board::enpassant()
{
  if (!canEnpassant)
    return false;
  Move lastMove = moves.top();
  if (std::abs(lastMove.firstSquare.rank - lastMove.lastSquare.rank) != 2)
    return false;
  if (currentMove.front() == Player::WHITE)
  {
    if (board[lastMove.lastSquare.file][lastMove.lastSquare.rank] != BP && 
        std::abs(lastMove.firstSquare.rank - lastMove.lastSquare.rank) != 2)
        return false;
    if (whiteIsUnderCheck()) 
        return false;
    if (board[lastMove.lastSquare.file+1][lastMove.lastSquare.rank] != WP &&
        board[lastMove.lastSquare.file-1][lastMove.lastSquare.rank] != WP) 
        return false;
    if (board[lastMove.lastSquare.file+1][lastMove.lastSquare.rank] == WP)
    {
        capturedPieces.push_back(board[lastMove.lastSquare.file][lastMove.lastSquare.rank]);
        board[lastMove.lastSquare.file+1][lastMove.lastSquare.rank] = EMPTY; 
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank + 1] = WP;
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank] = EMPTY;
        canEnpassant = false;
        currentMove.push(currentMove.front());
        currentMove.pop();
        fillSet();
        return true;
    }   
    if (board[lastMove.lastSquare.file-1][lastMove.lastSquare.rank] == WP)
    {
        capturedPieces.push_back(board[lastMove.lastSquare.file][lastMove.lastSquare.rank]);
        board[lastMove.lastSquare.file-1][lastMove.lastSquare.rank] = EMPTY; 
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank + 1] = WP;
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank] = EMPTY;
        canEnpassant = false;
        currentMove.push(currentMove.front());
        currentMove.pop();
        fillSet();
        return true;
    }   
  }
  else if (currentMove.front() == Player::BLACK)
  {
    if (board[lastMove.lastSquare.file][lastMove.lastSquare.rank] != WP
        && std::abs(lastMove.firstSquare.rank - lastMove.lastSquare.rank) != 2)
        return false;
    if (blackIsUnderCheck()) 
        return false;
    if (board[lastMove.lastSquare.file+1][lastMove.lastSquare.rank] != BP &&
        board[lastMove.lastSquare.file-1][lastMove.lastSquare.rank] != BP) 
        return false;
    if (board[lastMove.lastSquare.file+1][lastMove.lastSquare.rank] == BP)
    {
        capturedPieces.push_back(board[lastMove.lastSquare.file][lastMove.lastSquare.rank]);
        board[lastMove.lastSquare.file+1][lastMove.lastSquare.rank] = EMPTY; 
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank + 1] = BP;
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank] = EMPTY;
        canEnpassant = false;
        currentMove.push(currentMove.front());
        currentMove.pop();
        fillSet();
        return true;
    }   
    if (board[lastMove.lastSquare.file-1][lastMove.lastSquare.rank] == BP)
    {
        capturedPieces.push_back(board[lastMove.lastSquare.file][lastMove.lastSquare.rank]);
        board[lastMove.lastSquare.file-1][lastMove.lastSquare.rank] = EMPTY; 
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank - 1] = BP;
        board[lastMove.lastSquare.file][lastMove.lastSquare.rank] = EMPTY;
        canEnpassant = false;
        currentMove.push(currentMove.front());
        currentMove.pop();
        fillSet();
        return true;
    }
  }
   return true;
}

std::pair<int, int> chess::Board::getPieceScore()
{
  std::map<int, int> pieceScores = {
      {WP, 1},
      {WR, 5},
      {WN, 3},
      {WB, 3},
      {WQ, 9},
      {BP, 1},
      {BR, 5},
      {BN, 3},
      {BB, 3},
      {BQ, 9}
    };
  int blackScore = 0;
  int whiteScore = 0;
  for (std::list<int>::iterator it = capturedPieces.begin();
      it != capturedPieces.end(); ++it)
  {
    if (pieceToPlayer(*it) == Player::WHITE)
      blackScore += pieceScores[*it];
    if (pieceToPlayer(*it) == Player::BLACK)
      whiteScore += pieceScores[*it];
  }
  std::pair <int, int> ret;
  ret.first = whiteScore;
  ret.second = blackScore;
  return ret;
}