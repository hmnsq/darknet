#ifndef BOX_H
#define BOX_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ{
    ಭಿನ್ನ dx, dy, dw, dh;
} dbox;

ಭಿನ್ನ box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
