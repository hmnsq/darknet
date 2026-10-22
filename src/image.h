#ifndef IMAGE_H
#define IMAGE_H

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ "box.h"
#അന്തര്ഭാവ "darknet.h"

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

#ifdef OPENCV
ശൂന്യം *open_video_stream(സ്ഥിരം അക്ഷരം *f, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ fps);
image get_image_from_stream(ശൂന്യം *p);
image load_image_cv(അക്ഷരം *filename, പൂർണ്ണസംഖ്യ channels);
പൂർണ്ണസംഖ്യ show_image_cv(image im, സ്ഥിരം അക്ഷരം* name, പൂർണ്ണസംഖ്യ ms);
#endif

ദശാംശം get_color(പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ max);
ശൂന്യം draw_box(image a, പൂർണ്ണസംഖ്യ x1, പൂർണ്ണസംഖ്യ y1, പൂർണ്ണസംഖ്യ x2, പൂർണ്ണസംഖ്യ y2, ദശാംശം r, ദശാംശം g, ദശാംശം b);
ശൂന്യം draw_bbox(image a, box bbox, പൂർണ്ണസംഖ്യ w, ദശാംശം r, ദശാംശം g, ദശാംശം b);
ശൂന്യം write_label(image a, പൂർണ്ണസംഖ്യ r, പൂർണ്ണസംഖ്യ c, image *characters, അക്ഷരം *string, ദശാംശം *rgb);
image image_distance(image a, image b);
ശൂന്യം scale_image(image m, ദശാംശം s);
image rotate_crop_image(image im, ദശാംശം rad, ദശാംശം s, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം dx, ദശാംശം dy, ദശാംശം aspect);
image random_crop_image(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
image random_augment_image(image im, ദശാംശം angle, ദശാംശം aspect, പൂർണ്ണസംഖ്യ താഴെയുള്ള, പൂർണ്ണസംഖ്യ മുകളിലെ, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
augment_args random_augment_args(image im, ദശാംശം angle, ദശാംശം aspect, പൂർണ്ണസംഖ്യ താഴെയുള്ള, പൂർണ്ണസംഖ്യ മുകളിലെ, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
ശൂന്യം letterbox_image_into(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, image boxed);
image resize_max(image im, പൂർണ്ണസംഖ്യ max);
ശൂന്യം translate_image(image m, ദശാംശം s);
ശൂന്യം embed_image(image source, image dest, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy);
ശൂന്യം place_image(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy, image canvas);
ശൂന്യം saturate_image(image im, ദശാംശം sat);
ശൂന്യം exposure_image(image im, ദശാംശം sat);
ശൂന്യം distort_image(image im, ദശാംശം hue, ദശാംശം sat, ദശാംശം val);
ശൂന്യം saturate_exposure_image(image im, ദശാംശം sat, ദശാംശം exposure);
ശൂന്യം rgb_to_hsv(image im);
ശൂന്യം hsv_to_rgb(image im);
ശൂന്യം yuv_to_rgb(image im);
ശൂന്യം rgb_to_yuv(image im);


image collapse_image_layers(image source, പൂർണ്ണസംഖ്യ border);
image collapse_images_horz(image *ims, പൂർണ്ണസംഖ്യ n);
image collapse_images_vert(image *ims, പൂർണ്ണസംഖ്യ n);

ശൂന്യം show_image_normalized(image im, സ്ഥിരം അക്ഷരം *name);
ശൂന്യം show_images(image *ims, പൂർണ്ണസംഖ്യ n, അക്ഷരം *window);
ശൂന്യം show_image_layers(image p, അക്ഷരം *name);
ശൂന്യം show_image_collapsed(image p, അക്ഷരം *name);

ശൂന്യം print_image(image m);

image make_empty_image(പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c);
ശൂന്യം copy_image_into(image src, image dest);

image get_image_layer(image m, പൂർണ്ണസംഖ്യ l);

#ifdef __cplusplus
}
#endif

#endif

