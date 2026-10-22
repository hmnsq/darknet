#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BOX_H
#ವ್ಯಾಖ್ಯಾನಿಸು BOX_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ{
    ದಶಮಾಂಶ dx, dy, dw, dh;
} dbox;

ದಶಮಾಂಶ box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);

#ಆದರೆ_ಅಂತ್ಯ
