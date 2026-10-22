#اگر_غیر_تعریف_شدہ IMAGE_H
#تعریف_کرو IMAGE_H

#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <float.h>
#شامل <string.h>
#شامل <math.h>
#شامل "box.h"
#شامل "darknet.h"

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

#اگر_تعریف_شدہ OPENCV
خالی *open_video_دھارا(ثابت حرف *f, صحیح c, صحیح w, صحیح h, صحیح fps);
image get_image_from_دھارا(خالی *p);
image بھرو_image_cv(حرف *filename, صحیح channels);
صحیح show_image_cv(image im, ثابت حرف* name, صحیح ms);
#اگر_ختم

اعشاری get_color(صحیح c, صحیح x, صحیح max);
خالی draw_box(image a, صحیح x1, صحیح y1, صحیح x2, صحیح y2, اعشاری r, اعشاری g, اعشاری b);
خالی draw_bbox(image a, box bbox, صحیح w, اعشاری r, اعشاری g, اعشاری b);
خالی write_label(image a, صحیح r, صحیح c, image *characters, حرف *string, اعشاری *rgb);
image image_فاصلہ(image a, image b);
خالی scale_image(image m, اعشاری s);
image rotate_crop_image(image im, اعشاری rad, اعشاری s, صحیح w, صحیح h, اعشاری dx, اعشاری dy, اعشاری aspect);
image random_crop_image(image im, صحیح w, صحیح h);
image random_augment_image(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بلند, صحیح w, صحیح h);
augment_دلائل random_augment_دلائل(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بلند, صحیح w, صحیح h);
خالی letterbox_image_into(image im, صحیح w, صحیح h, image boxed);
image resize_max(image im, صحیح max);
خالی translate_image(image m, اعشاری s);
خالی embed_image(image ماخذ, image dest, صحیح dx, صحیح dy);
خالی رکھو_image(image im, صحیح w, صحیح h, صحیح dx, صحیح dy, image canvas);
خالی saturate_image(image im, اعشاری sat);
خالی exposure_image(image im, اعشاری sat);
خالی distort_image(image im, اعشاری hue, اعشاری sat, اعشاری val);
خالی saturate_exposure_image(image im, اعشاری sat, اعشاری exposure);
خالی rgb_مقصد_hsv(image im);
خالی hsv_مقصد_rgb(image im);
خالی yuv_مقصد_rgb(image im);
خالی rgb_مقصد_yuv(image im);


image collapse_image_layers(image ماخذ, صحیح border);
image collapse_images_horz(image *ims, صحیح n);
image collapse_images_vert(image *ims, صحیح n);

خالی show_image_normalized(image im, ثابت حرف *name);
خالی show_images(image *ims, صحیح n, حرف *کھڑکی);
خالی show_image_layers(image p, حرف *name);
خالی show_image_collapsed(image p, حرف *name);

خالی print_image(image m);

image make_خالی_خانے_image(صحیح w, صحیح h, صحیح c);
خالی copy_image_into(image src, image dest);

image get_image_layer(image m, صحیح l);

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

#اگر_ختم

