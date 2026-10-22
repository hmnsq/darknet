#که_نه_تعریف_شوی IMAGE_H
#تعریف_کړه IMAGE_H

#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <float.h>
#ضمن <string.h>
#ضمن <math.h>
#ضمن "box.h"
#ضمن "darknet.h"

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

#که_تعریف_شوی OPENCV
فراغ *open_video_بهیر(ثابت حرف *f, صحيح c, صحيح w, صحيح h, صحيح fps);
image get_image_from_بهیر(فراغ *p);
image بارول_image_cv(حرف *filename, صحيح channels);
صحيح show_image_cv(image im, ثابت حرف* name, صحيح ms);
#د_که_پای

عائم get_color(صحيح c, صحيح x, صحيح max);
فراغ draw_box(image a, صحيح x1, صحيح y1, صحيح x2, صحيح y2, عائم r, عائم g, عائم b);
فراغ draw_bbox(image a, box bbox, صحيح w, عائم r, عائم g, عائم b);
فراغ write_label(image a, صحيح r, صحيح c, image *characters, حرف *string, عائم *rgb);
image image_واټن(image a, image b);
فراغ scale_image(image m, عائم s);
image rotate_crop_image(image im, عائم rad, عائم s, صحيح w, صحيح h, عائم dx, عائم dy, عائم aspect);
image random_crop_image(image im, صحيح w, صحيح h);
image random_augment_image(image im, عائم angle, عائم aspect, صحيح ټیټ, صحيح لوړ, صحيح w, صحيح h);
augment_دلیلونه random_augment_دلیلونه(image im, عائم angle, عائم aspect, صحيح ټیټ, صحيح لوړ, صحيح w, صحيح h);
فراغ letterbox_image_into(image im, صحيح w, صحيح h, image boxed);
image resize_max(image im, صحيح max);
فراغ translate_image(image m, عائم s);
فراغ embed_image(image سرچینه, image dest, صحيح dx, صحيح dy);
فراغ ځای_پرځای_کړه_image(image im, صحيح w, صحيح h, صحيح dx, صحيح dy, image canvas);
فراغ saturate_image(image im, عائم sat);
فراغ exposure_image(image im, عائم sat);
فراغ distort_image(image im, عائم hue, عائم sat, عائم val);
فراغ saturate_exposure_image(image im, عائم sat, عائم exposure);
فراغ rgb_منزل_hsv(image im);
فراغ hsv_منزل_rgb(image im);
فراغ yuv_منزل_rgb(image im);
فراغ rgb_منزل_yuv(image im);


image collapse_image_layers(image سرچینه, صحيح border);
image collapse_images_horz(image *ims, صحيح n);
image collapse_images_vert(image *ims, صحيح n);

فراغ show_image_normalized(image im, ثابت حرف *name);
فراغ show_images(image *ims, صحيح n, حرف *کړکۍ);
فراغ show_image_layers(image p, حرف *name);
فراغ show_image_collapsed(image p, حرف *name);

فراغ print_image(image m);

image make_تشه_image(صحيح w, صحيح h, صحيح c);
فراغ copy_image_into(image src, image dest);

image get_image_layer(image m, صحيح l);

#که_تعریف_شوی __cplusplus
}
#د_که_پای

#د_که_پای

