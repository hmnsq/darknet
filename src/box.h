#ifndef BOX_H
#define BOX_H
#അന്തര്ഭാവ "darknet.h"

തരനിർവചനം ഘടന{
    ദശാംശം dx, dy, dw, dh;
} dbox;

ദശാംശം box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#endif
