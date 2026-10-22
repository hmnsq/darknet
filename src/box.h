#ifndef BOX_H
#define BOX_H
#અન્તર્ભાવ "darknet.h"

પ્રકારવ્યાખ્યા સંરચના{
    દશાંશ dx, dy, dw, dh;
} dbox;

દશાંશ box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
