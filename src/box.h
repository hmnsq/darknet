#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BOX_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BOX_H
#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ{
    ਦਸ਼ਮਲਵ dx, dy, dw, dh;
} dbox;

ਦਸ਼ਮਲਵ box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#ਜੇ_ਅੰਤ
