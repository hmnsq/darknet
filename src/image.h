#اگر_تعریف_نشده IMAGE_H
#تعریف_کن IMAGE_H

#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <float.h>
#ضمن <string.h>
#ضمن <math.h>
#ضمن "box.h"
#ضمن "darknet.h"

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

#اگر_تعریف_شده OPENCV
فراغ *open_video_جریان(ثابت حرف *f, صحيح c, صحيح w, صحيح h, صحيح fps);
image get_image_from_جریان(فراغ *p);
image بارگیری_image_cv(حرف *filename, صحيح channels);
صحيح show_image_cv(image im, ثابت حرف* name, صحيح ms);
#پایان_اگر

عائم get_color(صحيح c, صحيح x, صحيح max);
فراغ draw_box(image a, صحيح x1, صحيح y1, صحيح x2, صحيح y2, عائم r, عائم g, عائم b);
فراغ draw_bbox(image a, box bbox, صحيح w, عائم r, عائم g, عائم b);
فراغ write_label(image a, صحيح r, صحيح c, image *characters, حرف *string, عائم *rgb);
image image_فاصله(image a, image b);
فراغ scale_image(image m, عائم s);
image rotate_crop_image(image im, عائم rad, عائم s, صحيح w, صحيح h, عائم dx, عائم dy, عائم aspect);
image random_crop_image(image im, صحيح w, صحيح h);
image random_augment_image(image im, عائم angle, عائم aspect, صحيح کم, صحيح بالا, صحيح w, صحيح h);
augment_آرگومانها random_augment_آرگومانها(image im, عائم angle, عائم aspect, صحيح کم, صحيح بالا, صحيح w, صحيح h);
فراغ letterbox_image_into(image im, صحيح w, صحيح h, image boxed);
image resize_max(image im, صحيح max);
فراغ translate_image(image m, عائم s);
فراغ embed_image(image منبع, image dest, صحيح dx, صحيح dy);
فراغ قراربده_image(image im, صحيح w, صحيح h, صحيح dx, صحيح dy, image canvas);
فراغ saturate_image(image im, عائم sat);
فراغ exposure_image(image im, عائم sat);
فراغ distort_image(image im, عائم hue, عائم sat, عائم val);
فراغ saturate_exposure_image(image im, عائم sat, عائم exposure);
فراغ rgb_مقصد_hsv(image im);
فراغ hsv_مقصد_rgb(image im);
فراغ yuv_مقصد_rgb(image im);
فراغ rgb_مقصد_yuv(image im);


image collapse_image_layers(image منبع, صحيح border);
image collapse_images_horz(image *ims, صحيح n);
image collapse_images_vert(image *ims, صحيح n);

فراغ show_image_normalized(image im, ثابت حرف *name);
فراغ show_images(image *ims, صحيح n, حرف *پنجره);
فراغ show_image_layers(image p, حرف *name);
فراغ show_image_collapsed(image p, حرف *name);

فراغ print_image(image m);

image make_خالی_image(صحيح w, صحيح h, صحيح c);
فراغ copy_image_into(image src, image dest);

image get_image_layer(image m, صحيح l);

#اگر_تعریف_شده __cplusplus
}
#پایان_اگر

#پایان_اگر

