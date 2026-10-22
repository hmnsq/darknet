#ifndef BOX_H
#define BOX_H
#அந்தர்பாவ "darknet.h"

வகைவரை கட்டமைப்பு{
    தசம dx, dy, dw, dh;
} dbox;

தசம box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
