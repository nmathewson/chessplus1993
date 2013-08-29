//				MOUSE.CPP

#include "mouse.h"
#include <dos.h>

int initmouse(void)
{
 long far *mvector;
 union REGS in, out;

 mvector = (long far *) (long far *) MK_FP(0x0000, 0x00cc);

 if (*mvector)
   {
    in.x.ax = 0;
    int86(0x33, &in, &out);
    return (out.x.ax);
   }
 else
    return 0;
}

void showmouse(void)
{
 long far *mvector;
 union REGS in, out;

 mvector = (long far *) MK_FP(0x0000, 0x00cc);

 if (*mvector)
   {
    in.x.ax = 1;
    int86(0x33, &in, &out);
   }
}

void hidemouse(void)
{
 long far *mvector;
 union REGS in, out;

 mvector = (long far *) MK_FP(0x0000, 0x00cc);

 if (*mvector)
   {
    in.x.ax = 2;
    int86(0x33, &in, &out);
   }
}

void getmouse(int &mx, int &my, int &button)
{
 long far *mvector;
 union REGS in, out;

 mvector = (long far *) MK_FP(0x0000, 0x00cc);

 if (*mvector)
   {
    in.x.ax = 3;
    in.x.bx = in.x.cx = in.x.dx = 0;
    int86(0x33, &in, &out);
    mx = out.x.cx;
    my = out.x.dx;
    button = out.x.bx;
   }
}

