#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ IMAGE_H
#ವ್ಯಾಖ್ಯಾನಿಸು IMAGE_H

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "darknet.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV
ಶೂನ್ಯ *open_video_ಹರಿವು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *f, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ fps);
image get_image_from_ಹರಿವು(ಶೂನ್ಯ *p);
image ತುಂಬು_image_cv(ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ channels);
ಪೂರ್ಣಾಂಕ show_image_cv(image im, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name, ಪೂರ್ಣಾಂಕ ms);
#ಆದರೆ_ಅಂತ್ಯ

ದಶಮಾಂಶ get_color(ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ max);
ಶೂನ್ಯ draw_box(image a, ಪೂರ್ಣಾಂಕ x1, ಪೂರ್ಣಾಂಕ y1, ಪೂರ್ಣಾಂಕ x2, ಪೂರ್ಣಾಂಕ y2, ದಶಮಾಂಶ r, ದಶಮಾಂಶ g, ದಶಮಾಂಶ b);
ಶೂನ್ಯ draw_bbox(image a, box bbox, ಪೂರ್ಣಾಂಕ w, ದಶಮಾಂಶ r, ದಶಮಾಂಶ g, ದಶಮಾಂಶ b);
ಶೂನ್ಯ write_label(image a, ಪೂರ್ಣಾಂಕ r, ಪೂರ್ಣಾಂಕ c, image *characters, ಅಕ್ಷರ *string, ದಶಮಾಂಶ *rgb);
image image_ದೂರ(image a, image b);
ಶೂನ್ಯ scale_image(image m, ದಶಮಾಂಶ s);
image rotate_crop_image(image im, ದಶಮಾಂಶ rad, ದಶಮಾಂಶ s, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ dx, ದಶಮಾಂಶ dy, ದಶಮಾಂಶ aspect);
image random_crop_image(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
image random_augment_image(image im, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ಪೂರ್ಣಾಂಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಂಕ ಮೇಲಿನ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
augment_ವಾದಗಳು random_augment_ವಾದಗಳು(image im, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ಪೂರ್ಣಾಂಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಂಕ ಮೇಲಿನ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಶೂನ್ಯ letterbox_image_into(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, image boxed);
image resize_max(image im, ಪೂರ್ಣಾಂಕ max);
ಶೂನ್ಯ translate_image(image m, ದಶಮಾಂಶ s);
ಶೂನ್ಯ embed_image(image ಮೂಲ, image dest, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy);
ಶೂನ್ಯ ಇರಿಸು_image(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy, image canvas);
ಶೂನ್ಯ saturate_image(image im, ದಶಮಾಂಶ sat);
ಶೂನ್ಯ exposure_image(image im, ದಶಮಾಂಶ sat);
ಶೂನ್ಯ distort_image(image im, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ sat, ದಶಮಾಂಶ val);
ಶೂನ್ಯ saturate_exposure_image(image im, ದಶಮಾಂಶ sat, ದಶಮಾಂಶ exposure);
ಶೂನ್ಯ rgb_ಗಮ್ಯ_hsv(image im);
ಶೂನ್ಯ hsv_ಗಮ್ಯ_rgb(image im);
ಶೂನ್ಯ yuv_ಗಮ್ಯ_rgb(image im);
ಶೂನ್ಯ rgb_ಗಮ್ಯ_yuv(image im);


image collapse_image_layers(image ಮೂಲ, ಪೂರ್ಣಾಂಕ border);
image collapse_images_horz(image *ims, ಪೂರ್ಣಾಂಕ n);
image collapse_images_vert(image *ims, ಪೂರ್ಣಾಂಕ n);

ಶೂನ್ಯ show_image_normalized(image im, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
ಶೂನ್ಯ show_images(image *ims, ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ *ಕಿಟಕಿ);
ಶೂನ್ಯ show_image_layers(image p, ಅಕ್ಷರ *name);
ಶೂನ್ಯ show_image_collapsed(image p, ಅಕ್ಷರ *name);

ಶೂನ್ಯ print_image(image m);

image make_ಖಾಲಿಗಳು_image(ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c);
ಶೂನ್ಯ copy_image_into(image src, image dest);

image get_image_layer(image m, ಪೂರ್ಣಾಂಕ l);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

