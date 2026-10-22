#ifndef IMAGE_H
#define IMAGE_H

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <float.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव "box.h"
#अन्तर्भाव "darknet.h"

#ifdef __cplusplus
बाह्य "C" {
#endif

#ifdef OPENCV
शून्य *open_video_stream(स्थिरांक अक्षर *f, पूर्णांक c, पूर्णांक w, पूर्णांक h, पूर्णांक fps);
image get_image_from_stream(शून्य *p);
image load_image_cv(अक्षर *filename, पूर्णांक channels);
पूर्णांक show_image_cv(image im, स्थिरांक अक्षर* name, पूर्णांक ms);
#endif

दशांश get_color(पूर्णांक c, पूर्णांक x, पूर्णांक max);
शून्य draw_box(image a, पूर्णांक x1, पूर्णांक y1, पूर्णांक x2, पूर्णांक y2, दशांश r, दशांश g, दशांश b);
शून्य draw_bbox(image a, box bbox, पूर्णांक w, दशांश r, दशांश g, दशांश b);
शून्य write_label(image a, पूर्णांक r, पूर्णांक c, image *characters, अक्षर *string, दशांश *rgb);
image image_distance(image a, image b);
शून्य scale_image(image m, दशांश s);
image rotate_crop_image(image im, दशांश rad, दशांश s, पूर्णांक w, पूर्णांक h, दशांश dx, दशांश dy, दशांश aspect);
image random_crop_image(image im, पूर्णांक w, पूर्णांक h);
image random_augment_image(image im, दशांश angle, दशांश aspect, पूर्णांक खालचा, पूर्णांक वरचा, पूर्णांक w, पूर्णांक h);
augment_args random_augment_args(image im, दशांश angle, दशांश aspect, पूर्णांक खालचा, पूर्णांक वरचा, पूर्णांक w, पूर्णांक h);
शून्य letterbox_image_into(image im, पूर्णांक w, पूर्णांक h, image boxed);
image resize_max(image im, पूर्णांक max);
शून्य translate_image(image m, दशांश s);
शून्य embed_image(image source, image dest, पूर्णांक dx, पूर्णांक dy);
शून्य place_image(image im, पूर्णांक w, पूर्णांक h, पूर्णांक dx, पूर्णांक dy, image canvas);
शून्य saturate_image(image im, दशांश sat);
शून्य exposure_image(image im, दशांश sat);
शून्य distort_image(image im, दशांश hue, दशांश sat, दशांश val);
शून्य saturate_exposure_image(image im, दशांश sat, दशांश exposure);
शून्य rgb_to_hsv(image im);
शून्य hsv_to_rgb(image im);
शून्य yuv_to_rgb(image im);
शून्य rgb_to_yuv(image im);


image collapse_image_layers(image source, पूर्णांक border);
image collapse_images_horz(image *ims, पूर्णांक n);
image collapse_images_vert(image *ims, पूर्णांक n);

शून्य show_image_normalized(image im, स्थिरांक अक्षर *name);
शून्य show_images(image *ims, पूर्णांक n, अक्षर *window);
शून्य show_image_layers(image p, अक्षर *name);
शून्य show_image_collapsed(image p, अक्षर *name);

शून्य print_image(image m);

image make_empty_image(पूर्णांक w, पूर्णांक h, पूर्णांक c);
शून्य copy_image_into(image src, image dest);

image get_image_layer(image m, पूर्णांक l);

#ifdef __cplusplus
}
#endif

#endif

