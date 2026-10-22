#ifndef BOX_H
#define BOX_H
#ଅନ୍ତର୍ଭାଵ "darknet.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା{
    ଦଶମିକ dx, dy, dw, dh;
} dbox;

ଦଶମିକ box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
