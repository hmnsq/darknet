#جے_بنا_تعریف BOX_H
#تعریف_کرو BOX_H
#ضمن "darknet.h"

تعريفنوع بنية{
    عائم dx, dy, dw, dh;
} dbox;

عائم box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#جے_انت
