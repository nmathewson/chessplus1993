//	      		Chess Plus V1.0 Board Class
//				cpboard.cpp
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include "mouse.h"
#include "chessp.h"

// GRAPHIC functions
#define OS -23

#define IN1(x,y,xt,yt,xb,yb) ((x) > (xt)) && ((x) < (xb)) \
			 && ((y) > (yt)) && ((y) < (yb))

#define IN2(x,xt,xb) ((x) > (xt)) && ((x) < (xb))

void board::sline(int bx, int by, int x, int y, int xt, int yt)
{
  line(sx(x,bx), sy(y,by), sx(xt, bx), sy(yt, by));
}

void board::sline(int bx, int by, int x, int y,
		  int xt, int yt, int xt2, int yt2)
{
  moveto(sx(x,bx), sy(y,by) );
  lineto(sx(xt,bx), sy(yt,by) );
  lineto(sx(xt2,bx), sy(yt2,by) );
}

void board::clear(int x, int y)
{
  if ((x+y)%2 == 0)
     setfillstyle(1,DARKGRAY);
  else
     setfillstyle(1,WHITE);

  if (y==0 && (x==0 || (x==(dim-1)) ))
     setfillstyle(1,LIGHTRED);

  if (y==(dim-1) && (x==0 || (x==(dim-1)) ))
     setfillstyle(1,LIGHTGREEN);

  bar(xsize * x, ysize * y, xsize * (x+1) - 1, ysize * (y+1) - 1);
}

void board::draw(int x, int y)
{
  square sq = board[x+y*dim];
  int c;

if (sq.info)  {

  if (sq.color())
    c = GREEN;
  else
    c = RED;

  setfillstyle(1,c);
  setcolor(c);

  if (sq.is_pawn())
    {
     ellipse(sx(24,x),sy(24,y),0,360,sx(16),sy(16));
     floodfill(sx(24,x),sy(24,y),c);
    }
  else
    {
     ellipse(sx(24,x),sy(24,y),0,360,sx(8),sy(8));     // draw circle

     if (sq.can_jump())                                // jump?
	sline(x,y,24,16,24,32);
     else if (sq.can_bomb())                           // bomb?
       {
	sline(x,y,18,18,30,30);
	sline(x,y,18,30,30,18);
       }
     else if (sq.is_king() == 1)                       // king?
	;
     else                                              // normal?
	floodfill(sx(24,x), sy(24,y), c);

//			SMOVE
     if (sq.smove & 1)       			       // S0
	sline(x,y,24,16,24,6);
     if (sq.smove & 2)                                // S1
	sline(x,y,30,18,42,6);
     if (sq.smove & 4)                                // S2
	sline(x,y,32,24,42,24);
     if (sq.smove & 8)                                // S3
	sline(x,y,30,30,42,42);
     if (sq.smove & 16)                               // S4
	sline(x,y,24,32,24,42);
     if (sq.smove & 32)                               // S5
	sline(x,y,18,30,6,42);
     if (sq.smove & 64)                               // S6
	sline(x,y,16,24,6,24);
     if (sq.smove & 128)                              // S7
	sline(x,y,18,18,6,6);

//			LMOVE
     if (sq.lmove & 1)       			       // L0
	sline(x,y,24,16,24,0);
     if (sq.lmove & 2)                                // L1
	sline(x,y,30,18,47,0);
     if (sq.lmove & 4)                                // L2
	sline(x,y,32,24,47,24);
     if (sq.lmove & 8)                                // L3
	sline(x,y,30,30,47,47);
     if (sq.lmove & 16)                               // L4
	sline(x,y,24,32,24,47);
     if (sq.lmove & 32)                               // L5
	sline(x,y,18,30,0,47);
     if (sq.lmove & 64)                               // L6
	sline(x,y,16,24,0,24);
     if (sq.lmove & 128)                              // L7
	sline(x,y,18,18,0,0);

//			KMOVE
     if (sq.kmove & 1)				       // K0
	sline(x,y,30,18,
		  36,12,
		  36,0);
     if (sq.kmove & 2)				       // K1
	sline(x,y,30,18,
		  36,12,
		  47,12);
     if (sq.kmove & 4)                                 // K2
	sline(x,y,30,30,
		  36,36,
		  47,36);
     if (sq.kmove & 8)				       // K3
	sline(x,y,30,30,
		  36,36,
		  36,47);
     if (sq.kmove & 16)    			       // K4
	sline(x,y,18,30,
		  12,36,
		  12,47);
     if (sq.kmove & 32)      			       // K5
	sline(x,y,18,30,
		  12,36,
		  0,36);
     if (sq.kmove & 64)       			       // K6
	sline(x,y,18,18,
		  12,12,
		  0,12);
     if (sq.kmove & 128)                               // K7
	sline(x,y,18,18,
		  12,12,
		  12,0);
    };

 }
}

void board::scores(void)
{
  char str[4];

  setfillstyle(1,BLACK);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
  settextjustify(LEFT_TEXT, TOP_TEXT);

  bar(520,0,619,68);				// erase old scores

  setcolor(RED);
  itoa(score[0],str,10);
  outtextxy(520,1,str);

  setcolor(GREEN);
  itoa(score[1],str,10);
  outtextxy(520,32,str);
}

void board::drawbox(int x, int y)
{
  if (to_move)
     setcolor(GREEN);
  else
     setcolor(RED);

  rectangle(x*xsize,y*ysize,(x+1)*xsize-1,(y+1)*ysize-1);
  rectangle(x*xsize+1,y*ysize+1,(x+1)*xsize-2,(y+1)*ysize-2);
}

void board::drawcontrols(void)
{
//			BOXES
  setcolor(LIGHTGRAY);
  rectangle(416,168+OS,620,225+OS);
  rectangle(416,110+OS,620,168+OS);
  rectangle(484,110+OS,552,225+OS);

// 			CONTROLS
  setcolor(LIGHTBLUE);
  ellipse(450,139+OS,0,90,24,24);		 // rot 1
  ellipse(518,139+OS,270,90,24,24);		 // rot 2
  ellipse(586,139+OS,180,90,24,24);		 // rot 3

  line(420,173+OS,480,221+OS);			 // bomb
  line(420,221+OS,480,173+OS);

  line(500,197+OS,536,197+OS);			 // move
  line(522,181+OS,538,197+OS);
  line(522,213+OS,538,197+OS);

  rectangle(570,185+OS,602,209+OS);	       	 // create
  setfillstyle(1,LIGHTBLUE);
  bar(556,172+OS,570,185+OS);
  bar(602,172+OS,616,185+OS);
  bar(556,209+OS,570,221+OS);
  bar(602,209+OS,616,221+OS);

//			LABELS
  if (device == 0)
    {
     setcolor(LIGHTGRAY);
     settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
     settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
     outtextxy(482,108+OS,"Ins");
     outtextxy(550,108+OS,"Home");
     outtextxy(618,108+OS,"PgUp");
     outtextxy(482,240+OS,"Del");
     outtextxy(550,240+OS,"End");
     outtextxy(618,240+OS,"PgDn");
    };

}

void board::draw_template(int king)
{
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  setcolor(WHITE);

  ellipse(520,374,0,360,17,20);

  line(420,290,508,360);			// 7
  line(532,388,620,458);			// 3
  line(420,458,508,388);                        // 1
  line(532,360,620,290);			// 9
  line(420,374,503,374);			// 4
  line(537,374,620,374);			// 6
  line(520,290,520,354);			// 8
  line(520,394,520,458);			// 2

  line(470,290,470,332);			// W
  line(420,332,470,332);			// Q
  line(570,290,570,332);			// E
  line(620,332,570,332);			// R
  line(620,417,570,417);			// F
  line(570,458,570,417);			// D
  line(470,458,470,417);			// S
  line(420,417,470,417);			// A

  if (to_move)
     setcolor(GREEN);
  else
     setcolor(RED);

  if ((! king) && (device == 0))
     outtextxy(520,374,"5");

  if ((! king) && (device == 1))
     rectangle(510,364,530,384);

  if (device == 0)
    {
     outtextxy(420,290,"7");
     outtextxy(470,290,"W");
     outtextxy(520,290,"8");
     outtextxy(570,290,"E");
     outtextxy(620,290,"9");
     outtextxy(420,332,"Q");
     outtextxy(620,332,"R");
     outtextxy(420,374,"4");
     outtextxy(620,374,"6");
     outtextxy(420,417,"A");
     outtextxy(620,417,"F");
     outtextxy(420,458,"1");
     outtextxy(470,458,"S");
     outtextxy(520,458,"2");
     outtextxy(570,458,"D");
     outtextxy(620,458,"3");
    }
  else
    {
     rectangle(401,270,420,290);	// 7
     rectangle(460,270,480,290);        // W
     rectangle(510,270,530,290);        // 8
     rectangle(560,270,580,290);        // E
     rectangle(620,270,639,290);        // 9
     rectangle(401,322,420,342);	// Q
     rectangle(620,322,639,342);	// R
     rectangle(401,364,420,384);	// 4
     rectangle(620,364,639,384);        // 6
     rectangle(401,407,420,427);	// A
     rectangle(620,407,639,427);	// F
     rectangle(401,458,420,478);	// 1
     rectangle(460,458,480,478);	// S
     rectangle(510,458,530,478);	// 2
     rectangle(560,458,580,478);	// D
     rectangle(639,458,620,478);	// 3

    }
}

void board::erase_template(void)
{
 setfillstyle(1,BLACK);
 bar(400,230,639,480);
}

void board::writecost(int cost)
{
  char str[4];
  char ostr[10];

  strcpy(ostr, "Cost: ");
  itoa(cost, str, 10);
  strcat(ostr, str);

  setfillstyle(1,BLACK);			// Erase old
  bar(400,233,610,260);

  setcolor(WHITE);
  settextjustify(LEFT_TEXT, TOP_TEXT);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

  outtextxy(400,235,ostr);
  rectangle(530,235,550,255);
  outtextxy(555,235,"Done");

}

// WRITE FUNCTIONS

void board::writefrom(int x, int y)
{
  char str[10];
  char xs[3];
  char ys[3];

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  settextjustify(CENTER_TEXT, TOP_TEXT);
  if (to_move)
     setcolor(GREEN);
  else
     setcolor(RED);

  itoa(x, xs, 10);
  itoa(y, ys, 10);
  strcpy(str,"(");
  strcat(str,xs);
  strcat(str,", ");
  strcat(str,ys);
  strcat(str,")");

  outtextxy(520,224,str);
}

void board::writemove(int movetype)
{
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  settextjustify(CENTER_TEXT, TOP_TEXT);
  if (to_move)
     setcolor(GREEN);
  else
     setcolor(RED);

  switch (movetype)
    {
     case ROT1 : outtextxy(520,234,"Rotate 90");
		 break;
     case ROT2 : outtextxy(520,234,"Rotate 180");
		 break;
     case ROT3 : outtextxy(520,234,"Rotate 270");
		 break;
     case BOMB : outtextxy(520,234,"Explode");
		 break;
     case MOVE : outtextxy(520,234,"to");
		 break;
     case MAKE : outtextxy(520,234,"Create");
		 break;
    };
}

void board::writeto(int x, int y)
{
  char str[10];
  char xs[3];
  char ys[3];

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  settextjustify(CENTER_TEXT, TOP_TEXT);
  if (to_move)
     setcolor(GREEN);
  else
     setcolor(RED);

  itoa(x, xs, 10);
  itoa(y, ys, 10);
  strcpy(str,"(");
  strcat(str,xs);
  strcat(str,", ");
  strcat(str,ys);
  strcat(str,")");

  outtextxy(520,244,str);
}

void board::writemsg(int message)
{
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  settextjustify(CENTER_TEXT, TOP_TEXT);
  setcolor(LIGHTBLUE);

  switch (message)
    {
     case 1 : outtextxy(520,264,"Are you sure? (Y/N)");
	      break;
     case 2 : outtextxy(520,264,"Not your piece!");
	      delay(1500);
	      break;
     case 3 : outtextxy(520,264,"It can't jump.");
	      delay(1500);
	      break;
     case 4 : outtextxy(520,264,"It's not a bomb.");
	      delay(1500);
	      break;
     case 5 : outtextxy(520,264,"Can't go that way.");
	      delay(1500);
	      break;
     case 6 : outtextxy(520,264,"Cannot create there.");
	      delay(1500);
	      break;
     case 7 : outtextxy(520,264,"No piece there!");
	      delay(1500);
	      break;
     case 8 : outtextxy(520,264,"That's your piece!");
	      delay(1500);
	      break;
     case 9 : setcolor(LIGHTGRAY);
	      outtextxy(520,254,"Confirm?");
	      break;
     case 10: outtextxy(520,264,"Pawns can't turn.");
	      delay(1500);
	      break;
     case 11: outtextxy(520,264,"Not enough points!");
	      delay(1500);
	      break;
     case 12: outtextxy(520,264,"That space is full.");
	      delay(1500);
	      break;
     case 13: outtextxy(520,264,"Too far to jump.");
	      delay(1500);
	      break;
     case 14: outtextxy(520,264,"Can't go that far.");
	      delay(1500);
	      break;
    }
}

void board::clear_write_area(void)
{
  setfillstyle(1,BLACK);
  bar(410,223,639,274);
}

// INPUT FUNCTIONS

int board::askquit()
{
  char ch;
  writemsg(1);
  do
    ch = getch();
  while ( (ch != 'y') && (ch != 'n') );
  return (ch == 'y');
}

int board::getloc(int &x, int &y)
{
  if (device == 1)
     return getloc_m(x, y);

  char ch, chn;
  ch = ' ';

  do
  {
     chn = ' ';
     x = boxx;
     y = boxy;
     drawbox(x, y);
     ch = getch();
     if (ch == 0)
	chn = getch();
     switch (chn)
     {
	case 72 : if (boxy > 0)
		     boxy--;
		  break;
	case 80 : if (boxy < (dim-1))
		     boxy++;
		  break;
	case 75 : if (boxx > 0)
		     boxx--;
		  break;
	case 77 : if (boxx < (dim-1))
		     boxx++;
		  break;
     }
     switch (ch)
     {
	case '8' : if (boxy > 0)
		      boxy--;
		   break;
	case '2' : if (boxy < (dim-1))
		      boxy++;
		   break;
	case '4' : if (boxx > 0)
		      boxx--;
		   break;
	case '6' : if (boxx < (dim-1))
		      boxx++;
		   break;
     }
     clear(x,y);
     draw(x,y);
  } while ((ch != 13) && (ch != 27) && (ch != 'q'));
  x = boxx;
  y = boxy;
  if (ch == 27)
     return ESC;
  if (ch == 'q')
    {
     if (askquit())
	return TYPE_Q;
     else
	return ESC;
    };
  return 0;
}

int board::getloc_m(int &x, int &y)
{
  int xc, yc, b;

  delay(100);

  showmouse();

  do
    {
     showmouse();
     do
       {
	getmouse(xc,yc,b);
       }
     while (b == 0);
     x = (xc / xsize);
     y = (yc / ysize);
     hidemouse();

     if (b == 2)
	return ESC;

     if ((xc > 619) && (yc < 16))
       {
	if (askquit())
	   return TYPE_Q;
	else
	   return ESC;
       };
    }
  while ((x < 0) || (x > (dim-1))
      || (y < 0) || (y > (dim-1)));

  hidemouse();

  return 0;
}

int board::getmove(int &movetype)
{
  if (device == 1)
     return getmove_m(movetype);

  char ch, chn;

  movetype = -1;

  do
    {
     chn = ' ';
     ch=getch();
     if (ch==0)
	chn=getch();
     switch (chn)
     {
	case 82 : movetype = ROT1;
		  break;
	case 71 : movetype = ROT2;
		  break;
	case 73 : movetype = ROT3;
		  break;
	case 83 : movetype = BOMB;
		  break;
	case 79 : movetype = MOVE;
		  break;
	case 81 : movetype = MAKE;
		  break;
     }
    }
  while ((movetype < 0) && (ch != 27) && (ch != 'q'));
  if (ch == 27)
    return ESC;
  if (ch == 'q')
    {
     if (askquit())
	return TYPE_Q;
     else
	return ESC;
    };

  return 0;
}

int board::getmove_m(int &movetype)
{
  int x, y, b;

  delay(150);

  showmouse();

  do
    {
     showmouse();
     do
       {
	getmouse(x,y,b);
       }
     while (b == 0);
     hidemouse();
    }
  while (((x < 416) || (x > 620)
       || (y < 110) || (y > 225)) && (b != 2));

  hidemouse();

  if (b == 2)
    return ESC;

  if ((y < 168) && (x < 484))
    movetype = ROT1;
  if ((y < 168) && (x > 484) && (x < 552))
    movetype = ROT2;
  if ((y < 168) && (x > 552))
    movetype = ROT3;
  if ((y > 168) && (x < 484))
    movetype = BOMB;
  if ((y > 168) && (x > 484) && (x < 552))
    movetype = MOVE;
  if ((y > 168) && (x > 552))
    movetype = MAKE;

  return 0;
}

// CANDO series

int board::cando(int movetype, int x, int y, int xe, int ye)
{
  int t;

  switch (movetype)
    {
     case ROT1 :
     case ROT2 :
     case ROT3 : t = canrot(x,y);
		 break;
     case BOMB : t = canbomb(x,y);
		 break;
     case MOVE : t = canmove(x,y,xe,ye);
		 break;
     case MAKE : t = canmake(x,y);
		 break;
    }

  return t;

}

int board::canrot(int x, int y)
{
  square &sq = board[x+y*dim];

  if (! sq.is_there())					// is it there?
    {
     writemsg(7);
     return 0;
    }

  if (sq.color() != to_move)				// is it yours?
    {
     writemsg(2);
     return 0;
    }

  if (sq.is_pawn())					// is it a pawn?
    {
     writemsg(10);
     return 0;
    }

  return 1;
}

int board::canbomb(int x, int y)
{
  square &sq = board[x+y*dim];

  if (! sq.is_there())					// is it there?
    {
     writemsg(7);
     return 0;
    }

  if (sq.color() != to_move)				// is it yours?
    {
     writemsg(2);
     return 0;
    }

  if (! sq.can_bomb())					// is it a bomb?
    {
     writemsg(4);
     return 0;
    }

  return 1;
}

int board::canmove(int xs, int ys, int xe, int ye)
{
  int calc, bit, dist;

  square &from = board[xs+ys*dim];
  square &to = board[xe+ye*dim];

  if (! from.is_there() )				// is there
    {
     writemsg(7);
     return 0;
    }

  if (from.color() != to_move)				// is yours?
    {
     writemsg(2);
     return 0;
    }

  if ((to.is_there() == 1) && (to.color() == to_move))	// is TO yours?
    {
     writemsg(8);
     return 0;
    }

  if (from.is_pawn())					// PAWN HANDLER
    {
     if ((to_move == 0) && (ye != ys + 1))		// red direction
	{
	 writemsg(5);
	 return 0;
	}

     if ((to_move == 1) && (ye != ys - 1))		// green direction
	{
	 writemsg(5);
	 return 0;
	}

     if ((to.is_there() == 1) && (abs(xs-xe) != 1))     // capture motion
	{
	 writemsg(5);
	 return 0;
	}

     if ((to.is_there() == 0) && (xs != xe))	        // movement motion
	{
	 writemsg(5);
	 return 0;
	}
     return 1;
    }

  if ( ((abs(xs-xe)==2)&&(abs(ys-ye)==1))||
       ((abs(xs-xe)==1)&&(abs(ys-ye)==2)) )		// knight motion
    {
     calc = 0;
     if (abs(xs-xe) > abs(ys-ye))			// Xlong = 1
	calc++;

     if (xs < xe) 					// X+    = 2
	calc += 2;

     if (ys < ye) 					// Y+	 = 3
	calc += 4;

     switch (calc)
       {
	case 0 : bit = 128;
		 calc = xs+ys*dim - dim - 1;
		 break;
	case 1 : bit = 64;
		 calc = xs+ys*dim - dim - 1;
		 break;
	case 2 : bit = 1;
		 calc = xs+ys*dim - dim + 1;
		 break;
	case 3 : bit = 2;
		 calc = xs+ys*dim - dim + 1;
		 break;
	case 4 : bit = 16;
		 calc = xs+ys*dim + dim - 1;
		 break;
	case 5 : bit = 32;
		 calc = xs+ys*dim + dim - 1;
		 break;
	case 6 : bit = 8;
		 calc = xs+ys*dim + dim + 1;
		 break;
	case 7 : bit = 4;
		 calc = xs+ys*dim + dim + 1;
		 break;
       };

     if (! from.can_kmove(bit))				// check direction
       {
	writemsg(5);
	return 0;
       }

     if (from.can_jump() )				// if jump, OK
	return 1;

     if (board[calc].is_there() )			// if not, check
       {
	writemsg(3);
	return 0;
       }

     return 1;
    }

  if ( (abs(xs-xe) != abs(ys-ye)) &&
       (xs != xe) && (ys != ye) )
    {
     writemsg(5);
     return 0;
    }

  dist = abs(xs-xe);

  if (dist == 0)
     dist = abs(ys-ye);

  calc = (xe-xs) / dist + 1;				// x-:0, x0:1, x+:2

  calc += ( (ye-ys) / dist + 1) * 3;			// y-:0, y0:3, y+:6

  switch (calc)
    {
     case 0 : bit = 128;
	      calc = (-dim) - 1;
	      break;
     case 1 : bit = 1;
	      calc = -dim;
	      break;
     case 2 : bit = 2;
	      calc = (-dim) + 1;
	      break;
     case 3 : bit = 64;
	      calc = -1;
	      break;
//   case 4 : no movement, doesn't exist.
     case 5 : bit = 4;
	      calc = 1;
	      break;
     case 6 : bit = 32;
	      calc = dim - 1;
	      break;
     case 7 : bit = 16;
	      calc = dim;
	      break;
     case 8 : bit = 8;
	      calc = dim + 1;
	      break;
    }

  if (from.can_move(bit) == 0)				// can it go that way
    {
     writemsg(5);
     return 0;
    }

  if ((from.can_move(bit) == 1) && (dist > 1))		// no long
    {
     writemsg(14);
     return 0;
    }

  if (dist == 1)
     return 1;

  if ( (from.can_jump()) && dist == 2)
     return 1;

  for(bit = xs+ys*dim + calc;				// start 1 square in
      bit != xe+ye*dim; 				// stop at end
      bit += calc)					// inc by value
    {
     if ( board[bit].is_there() )		        // anything there?
	{
	 if (from.can_jump())
	    writemsg(14);
	 if (! from.can_jump())
	    writemsg(3);
	 return 0;
	}
     }

  return 1;
}

int board::canmake(int x, int y)
{
  if (y !=(dim-1)*to_move)				// in the back
    {
     writemsg(6);
     return 0;
    }

  if ( (x != 0) && (x != dim-1) )			// in the corners
    {
     writemsg(6);
     return 0;
    }

  if (board[x+y*dim].is_there())			// is it full?
    {
     writemsg(12);
     return 0;
    }

  if (score[to_move] == 0)				// enough points?
    {
     writemsg(11);
     return 0;
    }

  return 1;
}


// DOTURN series

int board::doturn(int movetype, int x, int y, int xe, int ye)
{
  int r;

  switch (movetype)
    {
     case ROT1 :
     case ROT2 :
     case ROT3 : board[x+y*dim].rotate(movetype+1);
		 clear(x,y);
		 draw(x,y);
		 r = NOTOVER;
		 break;
     case BOMB : r = dobomb(x,y);
		 break;
     case MOVE : r = domove(x,y,xe,ye);
		 break;
     case MAKE : r = create(x,y);
		 break;
    }
  scores();
  return r;
}

int board::domove(int x, int y, int xe, int ye)
{
  int kf;

  score[to_move] += board[xe+ye*dim].value();
  kf = board[xe+ye*dim].is_king();
  if (kf == 1)
     kf = to_move + 2;

  if ((board[x+y*dim].is_pawn() == 1) && ((ye == dim-1)||(ye ==0)))
    {
     board[x+y*dim].kill();
     score[to_move] += 5;
    }

  board[xe+ye*dim] = board[x+y*dim];
  board[x+y*dim].kill();
  clear(x,y);
  clear(xe,ye);
  draw(xe,ye);
  return kf;
}

int board::dobomb(int x, int y)
{
  int kf[2];                                   		// if king taken
  int i, j;

  kf[0] = kf[1] = 0;

  for(j = y-1; j <= y+1; j++)
     for(i = x-1; i <= x+1; i++)
	if (! ((j<0) || (i<0) || (j==dim) || (i==dim)) )
	   if (board[i+j*dim].is_there())
	     {
	      if (board[i+j*dim].is_king())		// is king taken?
		kf[ board[i+j*dim].color() ^ 1 ]++;	// ++ opponents kf
	      score[ board[i+j*dim].color() ^ 1 ] +=
		     board[i+j*dim].value();
	      clear(i,j);
	      board[i+j*dim].kill();
	     };

  scores();

  if ((kf[0]) && (kf[1]))
     return DRAW_GAME;
  if (kf[0])
     return RED_WINS;
  if (kf[1])
     return GREEN_WINS;
  return NOTOVER;
}

char board::getplace(void)
{

  int x, y, b;

  delay(200);
  showmouse();
  showmouse();

  do
    getmouse(x,y,b);
  while (b == 0);

  hidemouse();

  if (b == 2)
    return 27;

  if (IN1(x,y,530,235,550,255))
    return 13;

  if (IN2(x,401,420))
    {
     if (IN2(y,270,290))
	return '7';
     if (IN2(y,322,342))
	return 'q';
     if (IN2(y,364,384))
	return '4';
     if (IN2(y,407,427))
	return 'a';
     if (IN2(y,458,478))
	return '1';
    };

  if (IN2(x,460,480))
    {
     if (IN2(y,270,290))
	return 'w';
     if (IN2(y,458,478))
	return 's';
    };

  if (IN2(x,510,530))
    {
     if (IN2(y,270,290))
	return '8';
     if (IN2(y,364,384))
	return '5';
     if (IN2(y,458,478))
	return '2';
    };

  if (IN2(x,560,580))
    {
     if (IN2(y,270,290))
	return 'e';
     if (IN2(y,458,478))
	return 'd';
    };

  if (IN2(x,620,639))
    {
     if (IN2(y,270,290))
	return '9';
     if (IN2(y,322,342))
	return 'r';
     if (IN2(y,364,384))
	return '6';
     if (IN2(y,407,427))
	return 'f';
     if (IN2(y,458,478))
	return '3';
    };

  return 'x';

}


int board::create(int x, int y, int king)
{
  int cost, mask;
  char ch;
  square & sq = board[x+y*dim];

  if (score[to_move] == 0)
     return ESC;

  clear_write_area();
  draw_template(king);

  sq.kill();
  sq.info = 1 + to_move * 2 + king * 4;
  cost = 0;

  do
    {
     cost = sq.value();
     writecost(cost);
     clear(x,y);
     draw(x,y);
     if (device == 1)
	ch = getplace();
     else
	ch = getch();
     if (! ch)
	getch();

	     mask = 0;					// clear mask

     switch (ch)
       {
//				INFO
	case '5' : if (! king)
		     switch (sq.info & 24)
			{
			 case 0  :
			 case 8  : sq.info += 8;
				   break;
			 case 16 : sq.info -= 16;
				   break;
			};
		   break;

//				KMOVES
	case 'e' : sq.kmove ^= 1;
		   break;
	case 'r' : sq.kmove ^= 2;
		   break;
	case 'f' : sq.kmove ^= 4;
		   break;
	case 'd' : sq.kmove ^= 8;
		   break;
	case 's' : sq.kmove ^= 16;
		   break;
	case 'a' : sq.kmove ^= 32;
		   break;
	case 'q' : sq.kmove ^= 64;
		   break;
	case 'w' : sq.kmove ^= 128;
		   break;

//				Straight MOVES
	case '8' : mask = 1;
		   break;
	case '9' : mask = 2;
		   break;
	case '6' : mask = 4;
		   break;
	case '3' : mask = 8;
		   break;
	case '2' : mask = 16;
		   break;
	case '1' : mask = 32;
		   break;
	case '4' : mask = 64;
		   break;
	case '7' : mask = 128;
		   break;
       };

     if (mask)
       {
	if (sq.smove & mask)
	  {
	   sq.smove -= mask;
	   sq.lmove += mask;
	  }
	else if (sq.lmove & mask)
	   sq.lmove -= mask;
	else
	   sq.smove += mask;
       }


    }
  while ( ((ch != 13) && (ch != 27))
	|| (cost > score[to_move]) || (king && (cost == 0)) );

  score[to_move] -= cost;
  scores();

  erase_template();

  if ((cost == 0) || (ch == 27))
    {
     clear(x,y);
     sq.kill();
     return ESC;
    };

  return OK;
}

// CONSTRUCTOR/DESCTUCTOR

board::board(int size, int rscore, int gscore, int dev)
{
  device = dev;
  hidemouse();

  board = new square[size * size];

  score[0] = rscore;
  score[1] = gscore;
  to_move = 1;
  dim = size;
  boxx = boxy = 0;
  xsize = 400 / dim;
  ysize = 480 / dim;

  setbkcolor(BLACK);
  clearviewport();

  int i, j;
  for(i = 0; i < dim; i++)
     for(j = 0; j < dim; j++)
       {
	board[j+i*dim].kill();
	clear(j,i);
       };

  for(j = 0; j < dim; j++)
    {
     board[j + dim].pawn(0);
     draw(j,1);
    }

  for(j = 0; j < dim; j++)
    {
     board[j+dim*(dim-2)].pawn(1);
     draw(j,dim-2);
    }

  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
  settextjustify(TOP_TEXT, RIGHT_TEXT);
  setcolor(RED);
  outtextxy(515,1,"Red:");
  setcolor(GREEN);
  outtextxy(520,32,"Green:");

  scores();

  if (device == 1)
    {
     setfillstyle(1,DARKGRAY);
     bar(620,0,639,15);
     setcolor(LIGHTGRAY);
     setfillstyle(1,LIGHTGRAY);
     fillellipse(630,4,4,3);
     bar(629,10,631,13);
    };

  drawcontrols();

  if (dim % 2 == 1)				// if odd board
    {
     i = dim / 2;  	                        // center
     create(i,dim-1,1);				// green king
     to_move = 0;
     create(i,0,1);				// red king

     for(j = 0; j < i; j++)			// left of center
       {
	to_move = 1;
	create(j,dim-1);
	to_move = 0;
	create(j,0);
       };

     for(j = i+1; j < dim; j++)			// right of center
       {
	to_move = 1;
	create(j,dim-1);
	to_move = 0;
	create(j,0);
       };
    }
  else						// even board
    {
     i = dim / 2;
     create(i,dim-1,1);				// green king
     to_move = 0;
     create(i-1,0,1);				// red king

     to_move = 1;
     create(i-1,dim-1);				// green flank
     to_move = 0;
     create(i,0);				// red flank

     for(j = 0; j < i - 1; j++)			// left of center
       {
	to_move = 1;
	create(j,dim-1);
	to_move = 0;
	create(j,0);
       };

     for(j = i+1; j < dim; j++)			// right of center
       {
	to_move = 1;
	create(j,dim-1);
	to_move = 0;
	create(j,0);
       };
    };

  to_move = 1;
}

board::~board(void)
{
  delete board;
}

int board::move(void)
{
  int rflag;					// repeat flag
  int move, x, y, endx, endy;
  int kflag;					// king flag
  int cflag;					// can do flag
  char ch;
  int dx, dy, b;

  do
    {
     clear_write_area();
     kflag = rflag = cflag = 0;

     delay(200);

     rflag = getloc(x,y);
     if (rflag == 2)
	break;
     if (rflag)
	continue;

     writefrom(x,y);

     rflag = getmove(move);
     if (rflag == 2)
	break;
     if (rflag)
	continue;

     writemove(move);

     if (move == MOVE)
	rflag = getloc(endx,endy);

     if (rflag == 2)
	break;
     if (rflag)
	continue;

     if (move == MOVE)
	writeto(endx,endy);

     writemsg(9);

     if (device == 0)
	do
	  {
	   ch = getch();				// Yes/No move
	   if (ch==27)
	      rflag = 1;
	  }
	while ( (ch!=13) && (ch!=' ') && (ch!=27) );
     else
       {
	delay(250);
	showmouse();
	do
	   getmouse(dx,dy,b);
	while (b == 0);
	if (b != 2)
	   ch = 13;
	if (b == 2)
	   rflag = 1;
       };

     if (rflag == 1)
	continue;

     cflag = cando(move,x,y,endx,endy);  		// check move
     if (! cflag)
	rflag = 1;

     if (! cflag)
	continue;

     kflag = doturn(move,x,y,endx,endy);

     if ((move == MAKE) && (kflag == ESC))
	rflag = 1;

    }
  while (rflag == 1);

  if (rflag == 2)
     return QUIT;

  to_move ^= 1;

  return kflag;

}