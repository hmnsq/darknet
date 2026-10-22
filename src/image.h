#अगर_अघोषित IMAGE_H
#घोषणा IMAGE_H

#समावेश <stdlib.h>
#समावेश <stdio.h>
#समावेश <float.h>
#समावेश <string.h>
#समावेश <math.h>
#समावेश "box.h"
#समावेश "darknet.h"

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

#अगर_घोषित OPENCV
व्योम *open_video_stream(स्थिर अक्षर *f, पूर्णांक c, पूर्णांक w, पूर्णांक h, पूर्णांक fps);
image get_image_from_stream(व्योम *p);
image load_image_cv(अक्षर *filename, पूर्णांक channels);
पूर्णांक show_image_cv(image im, स्थिर अक्षर* name, पूर्णांक ms);
#पूर्ण_अगर

भग्न get_color(पूर्णांक c, पूर्णांक x, पूर्णांक max);
व्योम draw_box(image a, पूर्णांक x1, पूर्णांक y1, पूर्णांक x2, पूर्णांक y2, भग्न r, भग्न g, भग्न b);
व्योम draw_bbox(image a, box bbox, पूर्णांक w, भग्न r, भग्न g, भग्न b);
व्योम write_label(image a, पूर्णांक r, पूर्णांक c, image *characters, अक्षर *string, भग्न *rgb);
image image_distance(image a, image b);
व्योम scale_image(image m, भग्न s);
image rotate_crop_image(image im, भग्न rad, भग्न s, पूर्णांक w, पूर्णांक h, भग्न dx, भग्न dy, भग्न aspect);
image random_crop_image(image im, पूर्णांक w, पूर्णांक h);
image random_augment_image(image im, भग्न angle, भग्न aspect, पूर्णांक निचला, पूर्णांक ऊपरी, पूर्णांक w, पूर्णांक h);
augment_args random_augment_args(image im, भग्न angle, भग्न aspect, पूर्णांक निचला, पूर्णांक ऊपरी, पूर्णांक w, पूर्णांक h);
व्योम letterbox_image_into(image im, पूर्णांक w, पूर्णांक h, image boxed);
image resize_max(image im, पूर्णांक max);
व्योम translate_image(image m, भग्न s);
व्योम embed_image(image source, image dest, पूर्णांक dx, पूर्णांक dy);
व्योम place_image(image im, पूर्णांक w, पूर्णांक h, पूर्णांक dx, पूर्णांक dy, image canvas);
व्योम saturate_image(image im, भग्न sat);
व्योम exposure_image(image im, भग्न sat);
व्योम distort_image(image im, भग्न hue, भग्न sat, भग्न val);
व्योम saturate_exposure_image(image im, भग्न sat, भग्न exposure);
व्योम rgb_to_hsv(image im);
व्योम hsv_to_rgb(image im);
व्योम yuv_to_rgb(image im);
व्योम rgb_to_yuv(image im);


image collapse_image_layers(image source, पूर्णांक border);
image collapse_images_horz(image *ims, पूर्णांक n);
image collapse_images_vert(image *ims, पूर्णांक n);

व्योम show_image_normalized(image im, स्थिर अक्षर *name);
व्योम show_images(image *ims, पूर्णांक n, अक्षर *window);
व्योम show_image_layers(image p, अक्षर *name);
व्योम show_image_collapsed(image p, अक्षर *name);

व्योम print_image(image m);

image make_empty_image(पूर्णांक w, पूर्णांक h, पूर्णांक c);
व्योम copy_image_into(image src, image dest);

image get_image_layer(image m, पूर्णांक l);

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर

