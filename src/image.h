#ifndef IMAGE_H
#define IMAGE_H

#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <float.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ "box.h"
#অন্তর্ভাৱ "darknet.h"

#ifdef __cplusplus
বাহ্য "C" {
#endif

#ifdef OPENCV
শূন্য *open_video_stream(ধ্রুৱ ৱর্ণ *f, পূর্ণাঙ্ক c, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক fps);
image get_image_from_stream(শূন্য *p);
image load_image_cv(ৱর্ণ *filename, পূর্ণাঙ্ক channels);
পূর্ণাঙ্ক show_image_cv(image im, ধ্রুৱ ৱর্ণ* name, পূর্ণাঙ্ক ms);
#endif

ভিন্ন get_color(পূর্ণাঙ্ক c, পূর্ণাঙ্ক x, পূর্ণাঙ্ক max);
শূন্য draw_box(image a, পূর্ণাঙ্ক x1, পূর্ণাঙ্ক y1, পূর্ণাঙ্ক x2, পূর্ণাঙ্ক y2, ভিন্ন r, ভিন্ন g, ভিন্ন b);
শূন্য draw_bbox(image a, box bbox, পূর্ণাঙ্ক w, ভিন্ন r, ভিন্ন g, ভিন্ন b);
শূন্য write_label(image a, পূর্ণাঙ্ক r, পূর্ণাঙ্ক c, image *characters, ৱর্ণ *string, ভিন্ন *rgb);
image image_distance(image a, image b);
শূন্য scale_image(image m, ভিন্ন s);
image rotate_crop_image(image im, ভিন্ন rad, ভিন্ন s, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন dx, ভিন্ন dy, ভিন্ন aspect);
image random_crop_image(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
image random_augment_image(image im, ভিন্ন angle, ভিন্ন aspect, পূর্ণাঙ্ক নিচেৰ, পূর্ণাঙ্ক উপৰেৰ, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
augment_args random_augment_args(image im, ভিন্ন angle, ভিন্ন aspect, পূর্ণাঙ্ক নিচেৰ, পূর্ণাঙ্ক উপৰেৰ, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
শূন্য letterbox_image_into(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, image boxed);
image resize_max(image im, পূর্ণাঙ্ক max);
শূন্য translate_image(image m, ভিন্ন s);
শূন্য embed_image(image source, image dest, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy);
শূন্য place_image(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy, image canvas);
শূন্য saturate_image(image im, ভিন্ন sat);
শূন্য exposure_image(image im, ভিন্ন sat);
শূন্য distort_image(image im, ভিন্ন hue, ভিন্ন sat, ভিন্ন val);
শূন্য saturate_exposure_image(image im, ভিন্ন sat, ভিন্ন exposure);
শূন্য rgb_to_hsv(image im);
শূন্য hsv_to_rgb(image im);
শূন্য yuv_to_rgb(image im);
শূন্য rgb_to_yuv(image im);


image collapse_image_layers(image source, পূর্ণাঙ্ক border);
image collapse_images_horz(image *ims, পূর্ণাঙ্ক n);
image collapse_images_vert(image *ims, পূর্ণাঙ্ক n);

শূন্য show_image_normalized(image im, ধ্রুৱ ৱর্ণ *name);
শূন্য show_images(image *ims, পূর্ণাঙ্ক n, ৱর্ণ *window);
শূন্য show_image_layers(image p, ৱর্ণ *name);
শূন্য show_image_collapsed(image p, ৱর্ণ *name);

শূন্য print_image(image m);

image make_empty_image(পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c);
শূন্য copy_image_into(image src, image dest);

image get_image_layer(image m, পূর্ণাঙ্ক l);

#ifdef __cplusplus
}
#endif

#endif

