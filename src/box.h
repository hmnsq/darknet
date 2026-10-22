#अगर_अघोषित BOX_H
#घोषणा BOX_H
#समावेश "darknet.h"

प्रकार काष्ठा{
    भग्न dx, dy, dw, dh;
} dbox;

भग्न box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#पूर्ण_अगर
