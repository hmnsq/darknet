#ifndef IMAGE_H
#define IMAGE_H

#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <float.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <math.h>
#அந்தர்பாவ "box.h"
#அந்தர்பாவ "darknet.h"

#ifdef __cplusplus
வெளி "C" {
#endif

#ifdef OPENCV
வெறுமை *open_video_stream(மாறிலி எழுத்து *f, முழு c, முழு w, முழு h, முழு fps);
image get_image_from_stream(வெறுமை *p);
image load_image_cv(எழுத்து *filename, முழு channels);
முழு show_image_cv(image im, மாறிலி எழுத்து* name, முழு ms);
#endif

தசம get_color(முழு c, முழு x, முழு max);
வெறுமை draw_box(image a, முழு x1, முழு y1, முழு x2, முழு y2, தசம r, தசம g, தசம b);
வெறுமை draw_bbox(image a, box bbox, முழு w, தசம r, தசம g, தசம b);
வெறுமை write_label(image a, முழு r, முழு c, image *characters, எழுத்து *string, தசம *rgb);
image image_distance(image a, image b);
வெறுமை scale_image(image m, தசம s);
image rotate_crop_image(image im, தசம rad, தசம s, முழு w, முழு h, தசம dx, தசம dy, தசம aspect);
image random_crop_image(image im, முழு w, முழு h);
image random_augment_image(image im, தசம angle, தசம aspect, முழு கீழ், முழு உயர்வு, முழு w, முழு h);
augment_args random_augment_args(image im, தசம angle, தசம aspect, முழு கீழ், முழு உயர்வு, முழு w, முழு h);
வெறுமை letterbox_image_into(image im, முழு w, முழு h, image boxed);
image resize_max(image im, முழு max);
வெறுமை translate_image(image m, தசம s);
வெறுமை embed_image(image source, image dest, முழு dx, முழு dy);
வெறுமை place_image(image im, முழு w, முழு h, முழு dx, முழு dy, image canvas);
வெறுமை saturate_image(image im, தசம sat);
வெறுமை exposure_image(image im, தசம sat);
வெறுமை distort_image(image im, தசம hue, தசம sat, தசம val);
வெறுமை saturate_exposure_image(image im, தசம sat, தசம exposure);
வெறுமை rgb_to_hsv(image im);
வெறுமை hsv_to_rgb(image im);
வெறுமை yuv_to_rgb(image im);
வெறுமை rgb_to_yuv(image im);


image collapse_image_layers(image source, முழு border);
image collapse_images_horz(image *ims, முழு n);
image collapse_images_vert(image *ims, முழு n);

வெறுமை show_image_normalized(image im, மாறிலி எழுத்து *name);
வெறுமை show_images(image *ims, முழு n, எழுத்து *window);
வெறுமை show_image_layers(image p, எழுத்து *name);
வெறுமை show_image_collapsed(image p, எழுத்து *name);

வெறுமை print_image(image m);

image make_empty_image(முழு w, முழு h, முழு c);
வெறுமை copy_image_into(image src, image dest);

image get_image_layer(image m, முழு l);

#ifdef __cplusplus
}
#endif

#endif

