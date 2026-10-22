#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ IMAGE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IMAGE_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV
ਖਾਲੀ *open_video_ਧਾਰਾ(ਅਚਲ ਅੱਖਰ *f, ਪੂਰਨ c, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ fps);
image get_image_from_ਧਾਰਾ(ਖਾਲੀ *p);
image ਲੱਦੋ_image_cv(ਅੱਖਰ *filename, ਪੂਰਨ channels);
ਪੂਰਨ show_image_cv(image im, ਅਚਲ ਅੱਖਰ* name, ਪੂਰਨ ms);
#ਜੇ_ਅੰਤ

ਦਸ਼ਮਲਵ get_color(ਪੂਰਨ c, ਪੂਰਨ x, ਪੂਰਨ max);
ਖਾਲੀ draw_box(image a, ਪੂਰਨ x1, ਪੂਰਨ y1, ਪੂਰਨ x2, ਪੂਰਨ y2, ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b);
ਖਾਲੀ draw_bbox(image a, box bbox, ਪੂਰਨ w, ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b);
ਖਾਲੀ write_label(image a, ਪੂਰਨ r, ਪੂਰਨ c, image *characters, ਅੱਖਰ *string, ਦਸ਼ਮਲਵ *rgb);
image image_ਦੂਰੀ(image a, image b);
ਖਾਲੀ scale_image(image m, ਦਸ਼ਮਲਵ s);
image rotate_crop_image(image im, ਦਸ਼ਮਲਵ rad, ਦਸ਼ਮਲਵ s, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy, ਦਸ਼ਮਲਵ aspect);
image random_crop_image(image im, ਪੂਰਨ w, ਪੂਰਨ h);
image random_augment_image(image im, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਪੂਰਨ ਨੀਵਾਂ, ਪੂਰਨ ਉੱਚਾ, ਪੂਰਨ w, ਪੂਰਨ h);
augment_ਦਲੀਲਾਂ random_augment_ਦਲੀਲਾਂ(image im, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਪੂਰਨ ਨੀਵਾਂ, ਪੂਰਨ ਉੱਚਾ, ਪੂਰਨ w, ਪੂਰਨ h);
ਖਾਲੀ letterbox_image_into(image im, ਪੂਰਨ w, ਪੂਰਨ h, image boxed);
image resize_max(image im, ਪੂਰਨ max);
ਖਾਲੀ translate_image(image m, ਦਸ਼ਮਲਵ s);
ਖਾਲੀ embed_image(image ਸਰੋਤ, image dest, ਪੂਰਨ dx, ਪੂਰਨ dy);
ਖਾਲੀ ਰੱਖੋ_image(image im, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ dx, ਪੂਰਨ dy, image canvas);
ਖਾਲੀ saturate_image(image im, ਦਸ਼ਮਲਵ sat);
ਖਾਲੀ exposure_image(image im, ਦਸ਼ਮਲਵ sat);
ਖਾਲੀ distort_image(image im, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ sat, ਦਸ਼ਮਲਵ val);
ਖਾਲੀ saturate_exposure_image(image im, ਦਸ਼ਮਲਵ sat, ਦਸ਼ਮਲਵ exposure);
ਖਾਲੀ rgb_ਮੰਜ਼ਿਲ_hsv(image im);
ਖਾਲੀ hsv_ਮੰਜ਼ਿਲ_rgb(image im);
ਖਾਲੀ yuv_ਮੰਜ਼ਿਲ_rgb(image im);
ਖਾਲੀ rgb_ਮੰਜ਼ਿਲ_yuv(image im);


image collapse_image_layers(image ਸਰੋਤ, ਪੂਰਨ border);
image collapse_images_horz(image *ims, ਪੂਰਨ n);
image collapse_images_vert(image *ims, ਪੂਰਨ n);

ਖਾਲੀ show_image_normalized(image im, ਅਚਲ ਅੱਖਰ *name);
ਖਾਲੀ show_images(image *ims, ਪੂਰਨ n, ਅੱਖਰ *ਖਿੜਕੀ);
ਖਾਲੀ show_image_layers(image p, ਅੱਖਰ *name);
ਖਾਲੀ show_image_collapsed(image p, ਅੱਖਰ *name);

ਖਾਲੀ print_image(image m);

image make_ਖਾਲੀ_ਥਾਵਾਂ_image(ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c);
ਖਾਲੀ copy_image_into(image src, image dest);

image get_image_layer(image m, ਪੂਰਨ l);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

