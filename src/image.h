#ifndef IMAGE_H
#define IMAGE_H

#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <float.h>
#ضمن <string.h>
#ضمن <math.h>
#ضمن "box.h"
#ضمن "darknet.h"

#ifdef __cplusplus
خارجي "C" {
#endif

#ifdef OPENCV
فراغ *open_video_stream(ثابت حرف *f, صحيح c, صحيح w, صحيح h, صحيح fps);
image get_image_from_stream(فراغ *p);
image load_image_cv(حرف *filename, صحيح channels);
صحيح show_image_cv(image im, ثابت حرف* name, صحيح ms);
#endif

عائم get_color(صحيح c, صحيح x, صحيح max);
فراغ draw_box(image a, صحيح x1, صحيح y1, صحيح x2, صحيح y2, عائم r, عائم g, عائم b);
فراغ draw_bbox(image a, box bbox, صحيح w, عائم r, عائم g, عائم b);
فراغ write_label(image a, صحيح r, صحيح c, image *characters, حرف *string, عائم *rgb);
image image_distance(image a, image b);
فراغ scale_image(image m, عائم s);
image rotate_crop_image(image im, عائم rad, عائم s, صحيح w, صحيح h, عائم dx, عائم dy, عائم aspect);
image random_crop_image(image im, صحيح w, صحيح h);
image random_augment_image(image im, عائم angle, عائم aspect, صحيح أدنى, صحيح أعلى, صحيح w, صحيح h);
augment_args random_augment_args(image im, عائم angle, عائم aspect, صحيح أدنى, صحيح أعلى, صحيح w, صحيح h);
فراغ letterbox_image_into(image im, صحيح w, صحيح h, image boxed);
image resize_max(image im, صحيح max);
فراغ translate_image(image m, عائم s);
فراغ embed_image(image source, image dest, صحيح dx, صحيح dy);
فراغ place_image(image im, صحيح w, صحيح h, صحيح dx, صحيح dy, image canvas);
فراغ saturate_image(image im, عائم sat);
فراغ exposure_image(image im, عائم sat);
فراغ distort_image(image im, عائم hue, عائم sat, عائم val);
فراغ saturate_exposure_image(image im, عائم sat, عائم exposure);
فراغ rgb_to_hsv(image im);
فراغ hsv_to_rgb(image im);
فراغ yuv_to_rgb(image im);
فراغ rgb_to_yuv(image im);


image collapse_image_layers(image source, صحيح border);
image collapse_images_horz(image *ims, صحيح n);
image collapse_images_vert(image *ims, صحيح n);

فراغ show_image_normalized(image im, ثابت حرف *name);
فراغ show_images(image *ims, صحيح n, حرف *window);
فراغ show_image_layers(image p, حرف *name);
فراغ show_image_collapsed(image p, حرف *name);

فراغ print_image(image m);

image make_empty_image(صحيح w, صحيح h, صحيح c);
فراغ copy_image_into(image src, image dest);

image get_image_layer(image m, صحيح l);

#ifdef __cplusplus
}
#endif

#endif

