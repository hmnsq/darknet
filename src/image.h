#جے_بنا_تعریف IMAGE_H
#تعریف_کرو IMAGE_H

#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <float.h>
#ضمن <string.h>
#ضمن <math.h>
#ضمن "box.h"
#ضمن "darknet.h"

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

#جے_تعریف_شدہ OPENCV
فراغ *open_video_دھارا(ثابت حرف *f, صحيح c, صحيح w, صحيح h, صحيح fps);
image get_image_from_دھارا(فراغ *p);
image لدو_image_cv(حرف *filename, صحيح channels);
صحيح show_image_cv(image im, ثابت حرف* name, صحيح ms);
#جے_انت

عائم get_color(صحيح c, صحيح x, صحيح max);
فراغ draw_box(image a, صحيح x1, صحيح y1, صحيح x2, صحيح y2, عائم r, عائم g, عائم b);
فراغ draw_bbox(image a, box bbox, صحيح w, عائم r, عائم g, عائم b);
فراغ write_label(image a, صحيح r, صحيح c, image *characters, حرف *string, عائم *rgb);
image image_فاصلہ(image a, image b);
فراغ scale_image(image m, عائم s);
image rotate_crop_image(image im, عائم rad, عائم s, صحيح w, صحيح h, عائم dx, عائم dy, عائم aspect);
image random_crop_image(image im, صحيح w, صحيح h);
image random_augment_image(image im, عائم angle, عائم aspect, صحيح کم, صحيح بلند, صحيح w, صحيح h);
augment_دلیلاں random_augment_دلیلاں(image im, عائم angle, عائم aspect, صحيح کم, صحيح بلند, صحيح w, صحيح h);
فراغ letterbox_image_into(image im, صحيح w, صحيح h, image boxed);
image resize_max(image im, صحيح max);
فراغ translate_image(image m, عائم s);
فراغ embed_image(image سروت, image dest, صحيح dx, صحيح dy);
فراغ رکھو_image(image im, صحيح w, صحيح h, صحيح dx, صحيح dy, image canvas);
فراغ saturate_image(image im, عائم sat);
فراغ exposure_image(image im, عائم sat);
فراغ distort_image(image im, عائم hue, عائم sat, عائم val);
فراغ saturate_exposure_image(image im, عائم sat, عائم exposure);
فراغ rgb_منزل_hsv(image im);
فراغ hsv_منزل_rgb(image im);
فراغ yuv_منزل_rgb(image im);
فراغ rgb_منزل_yuv(image im);


image collapse_image_layers(image سروت, صحيح border);
image collapse_images_horz(image *ims, صحيح n);
image collapse_images_vert(image *ims, صحيح n);

فراغ show_image_normalized(image im, ثابت حرف *name);
فراغ show_images(image *ims, صحيح n, حرف *کھڑکی);
فراغ show_image_layers(image p, حرف *name);
فراغ show_image_collapsed(image p, حرف *name);

فراغ print_image(image m);

image make_خالی_تھاواں_image(صحيح w, صحيح h, صحيح c);
فراغ copy_image_into(image src, image dest);

image get_image_layer(image m, صحيح l);

#جے_تعریف_شدہ __cplusplus
}
#جے_انت

#جے_انت

