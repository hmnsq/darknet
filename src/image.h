#ifndef IMAGE_H
#define IMAGE_H

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ "box.h"
#ଅନ୍ତର୍ଭାଵ "darknet.h"

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

#ifdef OPENCV
ଶୂନ୍ୟ *open_video_stream(ଅଚଳ ଅକ୍ଷର *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fps);
image get_image_from_stream(ଶୂନ୍ୟ *p);
image load_image_cv(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_image_cv(image im, ଅଚଳ ଅକ୍ଷର* name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ms);
#endif

ଦଶମିକ get_color(ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max);
ଶୂନ୍ୟ draw_box(image a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y2, ଦଶମିକ r, ଦଶମିକ g, ଦଶମିକ b);
ଶୂନ୍ୟ draw_bbox(image a, box bbox, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ଦଶମିକ r, ଦଶମିକ g, ଦଶମିକ b);
ଶୂନ୍ୟ write_label(image a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା r, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, image *characters, ଅକ୍ଷର *string, ଦଶମିକ *rgb);
image image_distance(image a, image b);
ଶୂନ୍ୟ scale_image(image m, ଦଶମିକ s);
image rotate_crop_image(image im, ଦଶମିକ rad, ଦଶମିକ s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ dx, ଦଶମିକ dy, ଦଶମିକ aspect);
image random_crop_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
image random_augment_image(image im, ଦଶମିକ angle, ଦଶମିକ aspect, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ତଳ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
augment_args random_augment_args(image im, ଦଶମିକ angle, ଦଶମିକ aspect, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ତଳ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ଶୂନ୍ୟ letterbox_image_into(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, image boxed);
image resize_max(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max);
ଶୂନ୍ୟ translate_image(image m, ଦଶମିକ s);
ଶୂନ୍ୟ embed_image(image source, image dest, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy);
ଶୂନ୍ୟ place_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy, image canvas);
ଶୂନ୍ୟ saturate_image(image im, ଦଶମିକ sat);
ଶୂନ୍ୟ exposure_image(image im, ଦଶମିକ sat);
ଶୂନ୍ୟ distort_image(image im, ଦଶମିକ hue, ଦଶମିକ sat, ଦଶମିକ val);
ଶୂନ୍ୟ saturate_exposure_image(image im, ଦଶମିକ sat, ଦଶମିକ exposure);
ଶୂନ୍ୟ rgb_to_hsv(image im);
ଶୂନ୍ୟ hsv_to_rgb(image im);
ଶୂନ୍ୟ yuv_to_rgb(image im);
ଶୂନ୍ୟ rgb_to_yuv(image im);


image collapse_image_layers(image source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା border);
image collapse_images_horz(image *ims, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
image collapse_images_vert(image *ims, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);

ଶୂନ୍ୟ show_image_normalized(image im, ଅଚଳ ଅକ୍ଷର *name);
ଶୂନ୍ୟ show_images(image *ims, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର *window);
ଶୂନ୍ୟ show_image_layers(image p, ଅକ୍ଷର *name);
ଶୂନ୍ୟ show_image_collapsed(image p, ଅକ୍ଷର *name);

ଶୂନ୍ୟ print_image(image m);

image make_empty_image(ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c);
ଶୂନ୍ୟ copy_image_into(image src, image dest);

image get_image_layer(image m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା l);

#ifdef __cplusplus
}
#endif

#endif

