// 			Chess Plus V1.0 Main File
//				cpmain.cpp
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <graphics.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>
#include <fstream.h>
#include "chessp.h"
#include "mouse.h"

enum item { C, H, E, S1, S2, PLUS };

char *quotes[130];

void saywinner(int end);
int  beep(int time, int freq, int pause);
void title(void);
void shareware(void);
void on(item i);
void off(item i);
void getquotes(int &nquotes);
int getquote(int which, ifstream &in);

int main(int argc, char *argv[])
{
  int size, rpoints, gpoints, input, end_game, shareware_flag;
  char buffer[129];
  char *index;
  int nquotes;

  randomize();

  getquotes(nquotes);

  if (registerbgidriver(EGAVGA_driver) < 0) exit(1);
  if (registerbgifont(triplex_font) < 0) exit(1);
  int gdriver, gmode, errorcode;
  gdriver = VGA;
  gmode = VGAHI;
  initgraph(&gdriver, &gmode, "");
  errorcode = graphresult();
  if (errorcode != grOk)
     exit(1);

  shareware_flag = 2;

  if (argc > 1)
    {
     index = argv[1];
     while (*index)
       {
	*index = toupper(*index) + 1;
	index++;
       };
     if ( strcmp(argv[1], "BMFHSJB") == 0)
	shareware_flag = 1;
     if ( strcmp(argv[1], "CPTDP") == 0)
	shareware_flag = 0;
    };

  if (shareware_flag)
     title();

  if (shareware_flag == 2)
     shareware();

  if (shareware_flag == 0)
    {
     on(C);
     on(H);
     on(E);
     on(S1);
     on(S2);
     on(PLUS);
     getch();
    };

  cleardevice();
  gotoxy(0,0);
  puts("Chess Plus V 1.2, (C) 1992 Nick Mathewson\n");

  while(kbhit())
     getch();

  do
    {
     printf("Enter board size (5-12) :");
     gets(buffer);
     size = atoi(buffer);
    }
  while ( (size < 5) || (size > 12) );

  do
    {
     printf("Enter Red's starting points (10-200, %d recommended) :",
	     size * 6);
     gets(buffer);
     rpoints = atoi(buffer);
    }
  while ( (rpoints < 10) || (rpoints > 200) );

  do
    {
     printf("Enter Green's starting points (10-200, %d recommended) :",
	     size * 6);
     gets(buffer);
     gpoints = atoi(buffer);
    }
  while ( (gpoints < 10) || (gpoints > 200) );

  do
    {
     printf("(M)ouse or (K)eyboard?\n");
     *buffer = getch();
    }
  while ( (*buffer != 'M') && (*buffer != 'm')
       && (*buffer != 'K') && (*buffer != 'k') );

  if ((*buffer == 'm') || (*buffer == 'M'))
     input = 1;
  else
     input = 0;

  if ((input == 1) && (initmouse() == 0))
    {
     input = 0;
     printf("Mouse driver not found.  Changing to keyboard input.\n");
     getch();
    };

  board game(size, rpoints, gpoints, input);

  do
    end_game = game.move();
  while (! end_game );

  saywinner(end_game);

  getch();
  closegraph();

  puts("\n\n\n");
  puts(quotes[random(nquotes)]);

  return 0;
}

void saywinner(int end)
{
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 7);
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  switch(end)
    {
     case QUIT       : setcolor(LIGHTGRAY);
		       outtextxy(200,240,"Bye!");
		       break;
     case RED_WINS   : setcolor(LIGHTRED);
		       outtextxy(200,240,"Red wins!");
		       break;
     case GREEN_WINS : setcolor(LIGHTGREEN);
		       outtextxy(200,240,"Green wins!");
		       break;
     case DRAW_GAME  : setcolor(LIGHTBLUE);
		       settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
		       outtextxy(200,240,"The game is drawn.");
		       break;
    };
}

int beep(int length, int freq, int time)
{
  if (time > length)
     return 1;
  sound(freq);
  delay(time);
  nosound();
  delay(length - time);
  return 0;
}

void off(item i)
{
  setfillstyle(SOLID_FILL, BLACK);
  switch(i)
    {
     case C	: bar(0,0,127,128);
		  break;
     case H	: bar(128,0,255,128);
		  break;
     case E	: bar(256,0,383,128);
		  break;
     case S1	: bar(384,0,511,128);
		  break;
     case S2	: bar(512,0,639,128);
		  break;
     case PLUS	: bar(220,150,420,350);
    };
}

void on(item i)
{
  switch(i)
    {
     case C	: setcolor(CYAN);
		  ellipse(64,64,0,360,60,60);
		  line(125,30,64,64);
		  line(64,64,125,78);
		  setfillstyle(LINE_FILL, CYAN);
		  floodfill(60,60,CYAN);
		  break;
     case H	: setcolor(BLUE);
		  rectangle(148,20,236,108);
		  line(168,0,192,54);
		  line(192,54,216,0);
		  line(168,128,192,74);
		  line(192,74,216,128);
		  setfillstyle(INTERLEAVE_FILL, BLUE);
		  floodfill(192,64,BLUE);
		  break;
     case E	: setcolor(GREEN);
		  rectangle(266,20,364,108);
		  moveto(380,10);
		  lineto(320,37);
		  lineto(380,64);
		  lineto(320,91);
		  lineto(380,118);
		  setfillstyle(HATCH_FILL, GREEN);
		  floodfill(270,30,GREEN);
		  break;
     case S1	: setcolor(YELLOW);
		  moveto(510,10);
		  lineto(390,50);
		  lineto(510,50);
		  lineto(390,100);
		  break;
     case S2	: setcolor(LIGHTGRAY);
		  moveto(639,1);
		  lineto(520,55);
		  lineto(639,55);
		  lineto(520,125);
		  break;
     case PLUS	: setcolor(WHITE);
		  setfillstyle(WIDE_DOT_FILL, RED);
		  bar(293,150,352,350);
		  bar(220,216,420,282);
		  moveto(293,150);
		  lineto(352,150);
		  lineto(352,216);
		  lineto(420,216);
		  lineto(420,282);
		  lineto(352,282);
		  lineto(352,350);
		  lineto(293,350);
		  lineto(293,282);
		  lineto(220,282);
		  lineto(220,216);
		  lineto(293,216);
		  lineto(293,150);
    };
}

void title(void)		//           1/8: (200,50,50);
{				// stressed  1/8: (200,50,100);
  int i;
  clearviewport();
  on(PLUS);
  for(i = 1; i <=3; i++)
    {				//     ^
     beep(250,50,50);		// I-I-I * 4
     beep(250,50,50);		//xIxIxI
     beep(250,50,100);
    };
  beep(250,50,50);
  beep(250,50,50);
  beep(100,50,100);
  on(C);
  on(H);
  beep(170,50,50);
  on(E);
  beep(210,50,50);
  on(S1);
  on(S2);
  off(PLUS);
  beep(240,50,100);

  off(C);
  off(H);
  beep(250,50,50);
  off(E);
  beep(230,50,50);
  off(S1);
  off(S2);
  on(PLUS);
  beep(240,50,100);

  on(S2);
  beep(150,50,50);
  on(C);
  beep(240,50,50);
  on(S1);
  beep(180,50,100);
  on(H);
  beep(170,50,50);
  on(E);
  beep(240,50,50);
  off(PLUS);
  beep(250,50,100);

  off(S2);
  beep(240,50,50);
  off(C);
  beep(250,50,50);
  off(S1);
  beep(250,50,100);
  off(H);
  beep(250,50,50);
  off(E);
  beep(250,50,50);
  beep(170,50,100);

  on(E);
  beep(240,50,50);
  on(S1);
  beep(180,50,50);
  on(H);
  beep(240,50,100);
  on(S2);
  beep(150,50,50);
  on(C);
  beep(230,50,50);
  on(PLUS);
  beep(250,50,100);
				//    3      3    I--I-I   I
  for(i = 1; i <= 6; i++)       //  I-I-I  I-I-I  I  I-I   I
     beep(166,50,50);           // xIxIxI xIxIxI xI xIxI  xI
  beep(250,50,50);
  beep(125,50,50);
  beep(125,50,50);

  beep(250,50,100);
}

void shareware(void)
{   			// Message follows
   char message[] =
"   If you like this program, please send $10 to:\n"
"               N. Mathewson\n"
"               [1992 address redacted]\n"
"            (I can also be reached by electronic\n"
"            mail on a few Connecticuit BBS's)\n\n"
"                    HONEST MESSAGE!!!!\n"
"   I realize that it is completly rediculous to expect people to pay the\n"
"exorbitant registration fee for many works of shareware.  Let's face it:\n"
"  1. People should never have to pay MORE for LESS.\n"
"  2. There are COMMERCIAL games available for twenty to thirty dollars.\n"
"  3. Almost NO shareware products are as good as commercial ones.\n"
"For this reason, I am offering CHESS PLUS registration for $10.  If you can't\n"
"afford $10, send $5.  If you can't afford $5, send a postcard. (If you can't\n"
"afford $10, though, how did you get your computer?) Registered users receive\n"
"a password to allow them to skip this message.  If you want source code to\n"
"this program, say so - WITH your registration.  Note, though, that you MAY\n"
"NOT alter it, or re-release it under your own name.\n\n"
"Of course, there's no need to feel guilty if you don't want to register.\n\n"
"	CHESS PLUS V1.2 (C) 1993 N. Mathewson.\n";

   cleardevice();
   int a, b;
   char *c;

   a = b = 0;
   c = message;

   while (*c++)
     {
      a += (int) *c;
      a %= 1000;
      b ^= (int) *c;
     };

   if ((a != 617) || (b != 67))
     {
      printf("Internal error %d, %d\n\n",a,b);
      puts("Please do NOT change my program.\nIt isn't nice.\nI hate you.\n");
      exit(666);
     }
   puts(message);

   sleep(15);
   puts("\nPress any key to continue...\n");

   getch();
}

void getquotes(int &nquotes)
{
  int result = 1;
  nquotes = 0;

  ifstream in("chessp.cpq");
  if (in.fail())
     {
      puts("Can't find chessp.cpq in current directory!\n");
      exit(1);
     }

  do
    result = getquote(nquotes++, in);
  while (result);

  nquotes--;

  in.close();
}

int getquote(int which, ifstream &in)
{
  static char buffer[1024];
  char ch;
  int res, i = 0;

  res = 1;

  do
    {
     in.get(ch);
     buffer[i++] = ch;
    }
  while (ch != '@');

  if (--i == 0)
     res = 0;

  buffer[i] = '\0';

  quotes[which] = strdup(buffer);

  return res;
}
