#ifndef IMAGE_H
#define IMAGE_H

#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <float.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব "box.h"
#অন্তর্ভাব "darknet.h"

#ifdef __cplusplus
বাহ্যিক "C" {
#endif

#ifdef OPENCV
শূন্য *open_video_stream(ধ্রুবক অক্ষর *f, পূর্ণসংখ্যা c, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা fps);
image get_image_from_stream(শূন্য *p);
image load_image_cv(অক্ষর *filename, পূর্ণসংখ্যা channels);
পূর্ণসংখ্যা show_image_cv(image im, ধ্রুবক অক্ষর* name, পূর্ণসংখ্যা ms);
#endif

দশমিক get_color(পূর্ণসংখ্যা c, পূর্ণসংখ্যা x, পূর্ণসংখ্যা max);
শূন্য draw_box(image a, পূর্ণসংখ্যা x1, পূর্ণসংখ্যা y1, পূর্ণসংখ্যা x2, পূর্ণসংখ্যা y2, দশমিক r, দশমিক g, দশমিক b);
শূন্য draw_bbox(image a, box bbox, পূর্ণসংখ্যা w, দশমিক r, দশমিক g, দশমিক b);
শূন্য write_label(image a, পূর্ণসংখ্যা r, পূর্ণসংখ্যা c, image *characters, অক্ষর *string, দশমিক *rgb);
image image_distance(image a, image b);
শূন্য scale_image(image m, দশমিক s);
image rotate_crop_image(image im, দশমিক rad, দশমিক s, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, দশমিক dx, দশমিক dy, দশমিক aspect);
image random_crop_image(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
image random_augment_image(image im, দশমিক angle, দশমিক aspect, পূর্ণসংখ্যা নিচের, পূর্ণসংখ্যা উপরের, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
augment_args random_augment_args(image im, দশমিক angle, দশমিক aspect, পূর্ণসংখ্যা নিচের, পূর্ণসংখ্যা উপরের, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
শূন্য letterbox_image_into(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, image boxed);
image resize_max(image im, পূর্ণসংখ্যা max);
শূন্য translate_image(image m, দশমিক s);
শূন্য embed_image(image source, image dest, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy);
শূন্য place_image(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy, image canvas);
শূন্য saturate_image(image im, দশমিক sat);
শূন্য exposure_image(image im, দশমিক sat);
শূন্য distort_image(image im, দশমিক hue, দশমিক sat, দশমিক val);
শূন্য saturate_exposure_image(image im, দশমিক sat, দশমিক exposure);
শূন্য rgb_to_hsv(image im);
শূন্য hsv_to_rgb(image im);
শূন্য yuv_to_rgb(image im);
শূন্য rgb_to_yuv(image im);


image collapse_image_layers(image source, পূর্ণসংখ্যা border);
image collapse_images_horz(image *ims, পূর্ণসংখ্যা n);
image collapse_images_vert(image *ims, পূর্ণসংখ্যা n);

শূন্য show_image_normalized(image im, ধ্রুবক অক্ষর *name);
শূন্য show_images(image *ims, পূর্ণসংখ্যা n, অক্ষর *window);
শূন্য show_image_layers(image p, অক্ষর *name);
শূন্য show_image_collapsed(image p, অক্ষর *name);

শূন্য print_image(image m);

image make_empty_image(পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c);
শূন্য copy_image_into(image src, image dest);

image get_image_layer(image m, পূর্ণসংখ্যা l);

#ifdef __cplusplus
}
#endif

#endif

