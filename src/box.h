#ifndef BOX_H
#define BOX_H
#অন্তর্ভাব "darknet.h"

প্রকারলেখ কাঠামো{
    দশমিক dx, dy, dw, dh;
} dbox;

দশমিক box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
