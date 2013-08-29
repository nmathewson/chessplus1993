//	                Chess Plus V1.0 Header File
//				chessp.h
// CONTENTS:	- defines
//		- classes
#ifndef CHESSP_H
#define CHESSP_H

//			     DEFINES
#define RD 0
#define GRN 1

#define ROT1 0
#define ROT2 1
#define ROT3 2
#define MOVE 3
#define BOMB 4
#define MAKE 5

#define NOTOVER 0
#define QUIT 1
#define RED_WINS 2
#define GREEN_WINS 3
#define DRAW_GAME 4

#define OK 0
#define ESC 1
#define TYPE_Q 2

//	                     CLASSES

class square;

class board
{
 private:
// GAME vars
   square *board; 			// dimension = dim^2
					// element = x + dim * y

   unsigned int score[2];
   unsigned int to_move;

// GRAPHIC vars
   unsigned int dim,			// dimension of board
		xsize, ysize;		// size of square

// BOX vars
   unsigned int boxx, boxy;             // Where's the box?

// DEVICE vars
   int device;				// 0 for KBD, 1 for MOUSE

// GRAPHIC functions
   int sx(int x, int bx = 0) 	{ return (bx * xsize) + (x * xsize) / 48; }
   int sy(int y, int by = 0) 	{ return (by * ysize) + (y * ysize) / 48; }
   void sline(int bx, int by, int x, int y, int xt, int yt);
   void sline(int bx, int by, int x, int y, int xt, int yt,
	      int xt2, int yt2);
   void clear(int x, int y);		// clear a square
   void draw(int x, int y);		// draw a square
   void scores(void);			// write scores
   void drawbox(int x, int y);		// draw around square
   void drawcontrols(void);		// draw and label controls
   void draw_template(int king);	// template for create
   void erase_template(void);
   void writecost(int cost);            // update cost


// WRITE functions
   void writefrom(int x, int y);        //-\
   void writemove(int movetype);        // I--> Write the move
   void writeto(int x, int y);          //-/
   void writemsg(int message);
   void clear_write_area(void);

// INPUT functions
   int askquit();                       // Ask question, get y/n.
   int getloc(int &x, int &y);
   int getloc_m(int &x, int &y);
   int getmove(int &movetype);
   int getmove_m(int &movetype);

// CANDO series
   int cando(int movetype, int x, int y, int xe = 0, int ye = 0);
   int canrot(int x, int y);
   int canbomb(int x, int y);
   int canmove(int xs, int ys, int xe, int ye);
   int canmake(int x, int y);

// DOTURN series
// Must         - alter graphics, scores
// 		- return NOTOVER, QUIT, RED_WINS, GREEN_WINS, DRAW_GAME
   int doturn(int movetype, int x, int y, int xe = 0, int ye = 0);
   int domove(int x, int y, int xe, int ye);
   int dobomb(int x, int y);
   char getplace(void);
   int create(int x, int y, int king = 0);

 public:
// CONSTRUCTOR
// Must:        - initialize graphic & box vars
//		- set & draw board
//     		- set & draw pawns
//		- write "red:" and "green:"
//		- set & draw scores
//              - call drawcontrols()
//		- create kings and all other pieces
//		- set to_move
   board(int size, int rscore, int gscore, int dev);
   ~board(void);

// INTEFACE
// Must:	- get square & write
//		- get movetype & write
//		- get second square if movetype == MOVE & write
//		- call cando()
//		- call doturn()
//		- clear write_area
//		- return NOTOVER, QUIT, RED_WINS, GREEN_WINS, DRAW_GAME
   int move();
};

class square
{
 protected:
   unsigned char info;
//   0          1       2          3         4         5
//  2  (1)     2  (2)  2  (4)     2  (8)    2  (16)   2   (32)
//  =          =       =          =         =         =
//  0 no piece 0 red   0 not king 0 no jump 0 no bomb 0 not pawn
//  1 piece    1 green 1 king     1 jump    1 bomb    1 pawn

   unsigned char smove;
   unsigned char lmove;
//     7       0     1
//    2 (128) 2 (1) 2 (2)
//       \    I    /
//        \   I   /
//         \  I  /
//          \ I /
//           \I/     2
//      6-----O-----2 (4)
//     2 (64)/I\
//          / I \
//         /  I  \
//        /   I   \
//       /    I    \
//     5      4      3
//    2 (32) 2 (16) 2 (8)

unsigned char kmove;
//         7      0
//        2      2
//         I     I
//         I     I    1
//     6---\     /---2
//    2     \   /
//           \ /
//            O
//           / \
//          /   \     2
//     5---/     \---2
//    2    I     I
//         I     I
//          4    3
//         2    2

 public:


// SET functions
   void pawn(int color);

// TEST functions
   int is_there(void)	{ return (info & 1); }
   int color(void) 	{ return (info & 2) / 2; }
   int is_king(void)	{ return (info & 4) / 4; }
   int can_jump(void)	{ return (info & 8) / 8; }
   int can_bomb(void)	{ return (info & 16) / 16; }
   int is_pawn(void)	{ return (info & 32) / 32; }
   int value(void);
   int can_move(int d);			// Can move in d directon?
   int can_kmove(int d);

// DO functions
   void kill(void);                     // Set to empty
   void operator=(square &sq);          // Set to square
   void rotate(int n);                  // rotate clockwize n * 90 deg.
   friend int board::create(int x, int y, int king);

// GRAPHIC functions
   friend void board::draw(int x, int y);

};


#endif