#ifndef IMAGE_H
#define IMAGE_H

#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <float.h>
#شامل <string.h>
#شامل <math.h>
#شامل "box.h"
#شامل "darknet.h"

#ifdef __cplusplus
بیرونی "C" {
#endif

#ifdef OPENCV
تهی *open_video_stream(ثابت نویسه *f, صحیح c, صحیح w, صحیح h, صحیح fps);
image get_image_from_stream(تهی *p);
image load_image_cv(نویسه *filename, صحیح channels);
صحیح show_image_cv(image im, ثابت نویسه* name, صحیح ms);
#endif

اعشاری get_color(صحیح c, صحیح x, صحیح max);
تهی draw_box(image a, صحیح x1, صحیح y1, صحیح x2, صحیح y2, اعشاری r, اعشاری g, اعشاری b);
تهی draw_bbox(image a, box bbox, صحیح w, اعشاری r, اعشاری g, اعشاری b);
تهی write_label(image a, صحیح r, صحیح c, image *characters, نویسه *string, اعشاری *rgb);
image image_distance(image a, image b);
تهی scale_image(image m, اعشاری s);
image rotate_crop_image(image im, اعشاری rad, اعشاری s, صحیح w, صحیح h, اعشاری dx, اعشاری dy, اعشاری aspect);
image random_crop_image(image im, صحیح w, صحیح h);
image random_augment_image(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بالا, صحیح w, صحیح h);
augment_args random_augment_args(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بالا, صحیح w, صحیح h);
تهی letterbox_image_into(image im, صحیح w, صحیح h, image boxed);
image resize_max(image im, صحیح max);
تهی translate_image(image m, اعشاری s);
تهی embed_image(image source, image dest, صحیح dx, صحیح dy);
تهی place_image(image im, صحیح w, صحیح h, صحیح dx, صحیح dy, image canvas);
تهی saturate_image(image im, اعشاری sat);
تهی exposure_image(image im, اعشاری sat);
تهی distort_image(image im, اعشاری hue, اعشاری sat, اعشاری val);
تهی saturate_exposure_image(image im, اعشاری sat, اعشاری exposure);
تهی rgb_to_hsv(image im);
تهی hsv_to_rgb(image im);
تهی yuv_to_rgb(image im);
تهی rgb_to_yuv(image im);


image collapse_image_layers(image source, صحیح border);
image collapse_images_horz(image *ims, صحیح n);
image collapse_images_vert(image *ims, صحیح n);

تهی show_image_normalized(image im, ثابت نویسه *name);
تهی show_images(image *ims, صحیح n, نویسه *window);
تهی show_image_layers(image p, نویسه *name);
تهی show_image_collapsed(image p, نویسه *name);

تهی print_image(image m);

image make_empty_image(صحیح w, صحیح h, صحیح c);
تهی copy_image_into(image src, image dest);

image get_image_layer(image m, صحیح l);

#ifdef __cplusplus
}
#endif

#endif

