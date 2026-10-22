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
शून्य *open_video_stream(अचर वर्ण *f, पूर्णाङ्क c, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क fps);
image get_image_from_stream(शून्य *p);
image load_image_cv(वर्ण *filename, पूर्णाङ्क channels);
पूर्णाङ्क show_image_cv(image im, अचर वर्ण* name, पूर्णाङ्क ms);
#endif

भिन्न get_color(पूर्णाङ्क c, पूर्णाङ्क x, पूर्णाङ्क max);
शून्य draw_box(image a, पूर्णाङ्क x1, पूर्णाङ्क y1, पूर्णाङ्क x2, पूर्णाङ्क y2, भिन्न r, भिन्न g, भिन्न b);
शून्य draw_bbox(image a, box bbox, पूर्णाङ्क w, भिन्न r, भिन्न g, भिन्न b);
शून्य write_label(image a, पूर्णाङ्क r, पूर्णाङ्क c, image *characters, वर्ण *string, भिन्न *rgb);
image image_distance(image a, image b);
शून्य scale_image(image m, भिन्न s);
image rotate_crop_image(image im, भिन्न rad, भिन्न s, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न dx, भिन्न dy, भिन्न aspect);
image random_crop_image(image im, पूर्णाङ्क w, पूर्णाङ्क h);
image random_augment_image(image im, भिन्न angle, भिन्न aspect, पूर्णाङ्क तल्लो, पूर्णाङ्क माथिल्लो, पूर्णाङ्क w, पूर्णाङ्क h);
augment_args random_augment_args(image im, भिन्न angle, भिन्न aspect, पूर्णाङ्क तल्लो, पूर्णाङ्क माथिल्लो, पूर्णाङ्क w, पूर्णाङ्क h);
शून्य letterbox_image_into(image im, पूर्णाङ्क w, पूर्णाङ्क h, image boxed);
image resize_max(image im, पूर्णाङ्क max);
शून्य translate_image(image m, भिन्न s);
शून्य embed_image(image source, image dest, पूर्णाङ्क dx, पूर्णाङ्क dy);
शून्य place_image(image im, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क dx, पूर्णाङ्क dy, image canvas);
शून्य saturate_image(image im, भिन्न sat);
शून्य exposure_image(image im, भिन्न sat);
शून्य distort_image(image im, भिन्न hue, भिन्न sat, भिन्न val);
शून्य saturate_exposure_image(image im, भिन्न sat, भिन्न exposure);
शून्य rgb_to_hsv(image im);
शून्य hsv_to_rgb(image im);
शून्य yuv_to_rgb(image im);
शून्य rgb_to_yuv(image im);


image collapse_image_layers(image source, पूर्णाङ्क border);
image collapse_images_horz(image *ims, पूर्णाङ्क n);
image collapse_images_vert(image *ims, पूर्णाङ्क n);

शून्य show_image_normalized(image im, अचर वर्ण *name);
शून्य show_images(image *ims, पूर्णाङ्क n, वर्ण *window);
शून्य show_image_layers(image p, वर्ण *name);
शून्य show_image_collapsed(image p, वर्ण *name);

शून्य print_image(image m);

image make_empty_image(पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c);
शून्य copy_image_into(image src, image dest);

image get_image_layer(image m, पूर्णाङ्क l);

#ifdef __cplusplus
}
#endif

#endif

