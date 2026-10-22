#ifndef IMAGE_H
#define IMAGE_H

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "darknet.h"

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

#ifdef OPENCV
ಶೂನ್ಯ *open_video_stream(ಧ್ರುವ ವರ್ಣ *f, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ fps);
image get_image_from_stream(ಶೂನ್ಯ *p);
image load_image_cv(ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ channels);
ಪೂರ್ಣಾಙ್ಕ show_image_cv(image im, ಧ್ರುವ ವರ್ಣ* name, ಪೂರ್ಣಾಙ್ಕ ms);
#endif

ಭಿನ್ನ get_color(ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ x, ಪೂರ್ಣಾಙ್ಕ max);
ಶೂನ್ಯ draw_box(image a, ಪೂರ್ಣಾಙ್ಕ x1, ಪೂರ್ಣಾಙ್ಕ y1, ಪೂರ್ಣಾಙ್ಕ x2, ಪೂರ್ಣಾಙ್ಕ y2, ಭಿನ್ನ r, ಭಿನ್ನ g, ಭಿನ್ನ b);
ಶೂನ್ಯ draw_bbox(image a, box bbox, ಪೂರ್ಣಾಙ್ಕ w, ಭಿನ್ನ r, ಭಿನ್ನ g, ಭಿನ್ನ b);
ಶೂನ್ಯ write_label(image a, ಪೂರ್ಣಾಙ್ಕ r, ಪೂರ್ಣಾಙ್ಕ c, image *characters, ವರ್ಣ *string, ಭಿನ್ನ *rgb);
image image_distance(image a, image b);
ಶೂನ್ಯ scale_image(image m, ಭಿನ್ನ s);
image rotate_crop_image(image im, ಭಿನ್ನ rad, ಭಿನ್ನ s, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ dx, ಭಿನ್ನ dy, ಭಿನ್ನ aspect);
image random_crop_image(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
image random_augment_image(image im, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಪೂರ್ಣಾಙ್ಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಙ್ಕ ಮೇಲಿನ, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
augment_args random_augment_args(image im, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಪೂರ್ಣಾಙ್ಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಙ್ಕ ಮೇಲಿನ, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಶೂನ್ಯ letterbox_image_into(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, image boxed);
image resize_max(image im, ಪೂರ್ಣಾಙ್ಕ max);
ಶೂನ್ಯ translate_image(image m, ಭಿನ್ನ s);
ಶೂನ್ಯ embed_image(image source, image dest, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy);
ಶೂನ್ಯ place_image(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy, image canvas);
ಶೂನ್ಯ saturate_image(image im, ಭಿನ್ನ sat);
ಶೂನ್ಯ exposure_image(image im, ಭಿನ್ನ sat);
ಶೂನ್ಯ distort_image(image im, ಭಿನ್ನ hue, ಭಿನ್ನ sat, ಭಿನ್ನ val);
ಶೂನ್ಯ saturate_exposure_image(image im, ಭಿನ್ನ sat, ಭಿನ್ನ exposure);
ಶೂನ್ಯ rgb_to_hsv(image im);
ಶೂನ್ಯ hsv_to_rgb(image im);
ಶೂನ್ಯ yuv_to_rgb(image im);
ಶೂನ್ಯ rgb_to_yuv(image im);


image collapse_image_layers(image source, ಪೂರ್ಣಾಙ್ಕ border);
image collapse_images_horz(image *ims, ಪೂರ್ಣಾಙ್ಕ n);
image collapse_images_vert(image *ims, ಪೂರ್ಣಾಙ್ಕ n);

ಶೂನ್ಯ show_image_normalized(image im, ಧ್ರುವ ವರ್ಣ *name);
ಶೂನ್ಯ show_images(image *ims, ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ *window);
ಶೂನ್ಯ show_image_layers(image p, ವರ್ಣ *name);
ಶೂನ್ಯ show_image_collapsed(image p, ವರ್ಣ *name);

ಶೂನ್ಯ print_image(image m);

image make_empty_image(ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c);
ಶೂನ್ಯ copy_image_into(image src, image dest);

image get_image_layer(image m, ಪೂರ್ಣಾಙ್ಕ l);

#ifdef __cplusplus
}
#endif

#endif

