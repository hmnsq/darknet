#ifndef BOX_H
#define BOX_H
#अन्तर्भाव "darknet.h"

प्रकारनिर्देश संरचना{
    भिन्न dx, dy, dw, dh;
} dbox;

भिन्न box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
