#నిర్వచితం_కాకపోతే BOX_H
#నిర్వచించు BOX_H
#అన్తర్భావ "darknet.h"

రకనిర్వచనం నిర్మాణం{
    దశాంశం dx, dy, dw, dh;
} dbox;

దశాంశం box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#అయితే_ముగింపు
