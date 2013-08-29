//			Chess Plus	Square Class
//  				cpsquare.cpp
#include "chessp.h"

// SET functions

void square::pawn(int color)
{
  smove = lmove = kmove = 0;
  info = 33 + (color * 2);			// PIECE + PAWN + COLOR
}

// TEST functions

int square::value(void)
{
  if (!(info & 1)) 				// Nothing there? 0 pts
     return 0;
  if (info & 32)                               // Is it a pawn? 2 pts
     return 2;

//			INFO
  int i, val;
  val = (info & 8) / 4;				// Does it jump? 2 pts
  val += (info & 16) / 8;			// Does it bomb? 2 pts

//			SMOVE
  for(i = 1; i < 256; i *= 2)			// i = 1,2,4,...64,128
    if (smove & i)				// for each smove bit
      val += 1;					// 1 point
//			LMOVE
  for(i = 1; i < 256; i *= 2)			// i = 1,2,4,...64,128
    if (lmove & i)				// for each lmove bit
      val += 2;					// 2 point

//			KMOVE
  for(i = 1; i < 256; i *= 2)			// i = 1,2,4,...64,128
    if (kmove & i)				// for each kmove bit
      val += 1;					// 1 point

  return val;
}

int square::can_kmove(int d)
{
  return (kmove & d);
}

int square::can_move(int d)
{
// values: 	0: no movement
//		1: short movement
//		2: long movement
  if (lmove & d)	 			// LONG
     return 2;

  if (smove & d)	 			// SHORT
     return 1;

  return 0;					// NONE
}

// DO functions

void square::kill(void)
{
  info = smove = lmove = kmove = 0;
}

void square::operator=(square &sq)
{
  info = sq.info;
  smove = sq.smove;
  lmove = sq.lmove;
  kmove = sq.kmove;
}

void square::rotate(int n)
{
  int i;

  for(i = 0; i < n; i++)
    {
     smove = (smove & 196) / 64 + (smove & 63) * 4;
     lmove = (lmove & 196) / 64 + (lmove & 63) * 4;
     kmove = (kmove & 196) / 64 + (kmove & 63) * 4;
    };
}



