#ifndef BOX_H
#define BOX_H
#अन्तर्भाव "darknet.h"

प्रकारव्याख्या संरचना{
    दशांश dx, dy, dw, dh;
} dbox;

दशांश box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
