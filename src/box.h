#ifndef BOX_H
#define BOX_H
#شامل "darknet.h"

تعریفنوع ساختار{
    اعشاری dx, dy, dw, dh;
} dbox;

اعشاری box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
