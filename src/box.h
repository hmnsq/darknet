#اگر_غیر_تعریف_شدہ BOX_H
#تعریف_کرو BOX_H
#شامل "darknet.h"

قسمتعریف ساخت{
    اعشاری dx, dy, dw, dh;
} dbox;

اعشاری box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#اگر_ختم
