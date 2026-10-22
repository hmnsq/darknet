#ifndef BOX_H
#define BOX_H
#অন্তর্ভাৱ "darknet.h"

প্রকারনির্দেশ সংরচনা{
    ভিন্ন dx, dy, dw, dh;
} dbox;

ভিন্ন box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
