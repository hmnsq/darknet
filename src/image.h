#ifndef IMAGE_H
#define IMAGE_H

#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ "box.h"
#અન્તર્ભાવ "darknet.h"

#ifdef __cplusplus
બાહ્ય "C" {
#endif

#ifdef OPENCV
શૂન્ય *open_video_stream(અચલ અક્ષર *f, પૂર્ણાંક c, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક fps);
image get_image_from_stream(શૂન્ય *p);
image load_image_cv(અક્ષર *filename, પૂર્ણાંક channels);
પૂર્ણાંક show_image_cv(image im, અચલ અક્ષર* name, પૂર્ણાંક ms);
#endif

દશાંશ get_color(પૂર્ણાંક c, પૂર્ણાંક x, પૂર્ણાંક max);
શૂન્ય draw_box(image a, પૂર્ણાંક x1, પૂર્ણાંક y1, પૂર્ણાંક x2, પૂર્ણાંક y2, દશાંશ r, દશાંશ g, દશાંશ b);
શૂન્ય draw_bbox(image a, box bbox, પૂર્ણાંક w, દશાંશ r, દશાંશ g, દશાંશ b);
શૂન્ય write_label(image a, પૂર્ણાંક r, પૂર્ણાંક c, image *characters, અક્ષર *string, દશાંશ *rgb);
image image_distance(image a, image b);
શૂન્ય scale_image(image m, દશાંશ s);
image rotate_crop_image(image im, દશાંશ rad, દશાંશ s, પૂર્ણાંક w, પૂર્ણાંક h, દશાંશ dx, દશાંશ dy, દશાંશ aspect);
image random_crop_image(image im, પૂર્ણાંક w, પૂર્ણાંક h);
image random_augment_image(image im, દશાંશ angle, દશાંશ aspect, પૂર્ણાંક નીચલું, પૂર્ણાંક ઉપલું, પૂર્ણાંક w, પૂર્ણાંક h);
augment_args random_augment_args(image im, દશાંશ angle, દશાંશ aspect, પૂર્ણાંક નીચલું, પૂર્ણાંક ઉપલું, પૂર્ણાંક w, પૂર્ણાંક h);
શૂન્ય letterbox_image_into(image im, પૂર્ણાંક w, પૂર્ણાંક h, image boxed);
image resize_max(image im, પૂર્ણાંક max);
શૂન્ય translate_image(image m, દશાંશ s);
શૂન્ય embed_image(image source, image dest, પૂર્ણાંક dx, પૂર્ણાંક dy);
શૂન્ય place_image(image im, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક dx, પૂર્ણાંક dy, image canvas);
શૂન્ય saturate_image(image im, દશાંશ sat);
શૂન્ય exposure_image(image im, દશાંશ sat);
શૂન્ય distort_image(image im, દશાંશ hue, દશાંશ sat, દશાંશ val);
શૂન્ય saturate_exposure_image(image im, દશાંશ sat, દશાંશ exposure);
શૂન્ય rgb_to_hsv(image im);
શૂન્ય hsv_to_rgb(image im);
શૂન્ય yuv_to_rgb(image im);
શૂન્ય rgb_to_yuv(image im);


image collapse_image_layers(image source, પૂર્ણાંક border);
image collapse_images_horz(image *ims, પૂર્ણાંક n);
image collapse_images_vert(image *ims, પૂર્ણાંક n);

શૂન્ય show_image_normalized(image im, અચલ અક્ષર *name);
શૂન્ય show_images(image *ims, પૂર્ણાંક n, અક્ષર *window);
શૂન્ય show_image_layers(image p, અક્ષર *name);
શૂન્ય show_image_collapsed(image p, અક્ષર *name);

શૂન્ય print_image(image m);

image make_empty_image(પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c);
શૂન્ય copy_image_into(image src, image dest);

image get_image_layer(image m, પૂર્ણાંક l);

#ifdef __cplusplus
}
#endif

#endif

