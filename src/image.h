#నిర్వచితం_కాకపోతే IMAGE_H
#నిర్వచించు IMAGE_H

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <float.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <math.h>
#అన్తర్భావ "box.h"
#అన్తర్భావ "darknet.h"

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

#నిర్వచితమైతే OPENCV
శూన్య *open_video_ప్రవాహం(స్థిరరాశి అక్షరం *f, పూర్ణాంకం c, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం fps);
image get_image_from_ప్రవాహం(శూన్య *p);
image ఎక్కించు_image_cv(అక్షరం *filename, పూర్ణాంకం channels);
పూర్ణాంకం show_image_cv(image im, స్థిరరాశి అక్షరం* name, పూర్ణాంకం ms);
#అయితే_ముగింపు

దశాంశం get_color(పూర్ణాంకం c, పూర్ణాంకం x, పూర్ణాంకం max);
శూన్య draw_box(image a, పూర్ణాంకం x1, పూర్ణాంకం y1, పూర్ణాంకం x2, పూర్ణాంకం y2, దశాంశం r, దశాంశం g, దశాంశం b);
శూన్య draw_bbox(image a, box bbox, పూర్ణాంకం w, దశాంశం r, దశాంశం g, దశాంశం b);
శూన్య write_label(image a, పూర్ణాంకం r, పూర్ణాంకం c, image *characters, అక్షరం *string, దశాంశం *rgb);
image image_దూరం(image a, image b);
శూన్య scale_image(image m, దశాంశం s);
image rotate_crop_image(image im, దశాంశం rad, దశాంశం s, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం dx, దశాంశం dy, దశాంశం aspect);
image random_crop_image(image im, పూర్ణాంకం w, పూర్ణాంకం h);
image random_augment_image(image im, దశాంశం angle, దశాంశం aspect, పూర్ణాంకం దిగువ, పూర్ణాంకం ఎగువ, పూర్ణాంకం w, పూర్ణాంకం h);
augment_వాదనలు random_augment_వాదనలు(image im, దశాంశం angle, దశాంశం aspect, పూర్ణాంకం దిగువ, పూర్ణాంకం ఎగువ, పూర్ణాంకం w, పూర్ణాంకం h);
శూన్య letterbox_image_into(image im, పూర్ణాంకం w, పూర్ణాంకం h, image boxed);
image resize_max(image im, పూర్ణాంకం max);
శూన్య translate_image(image m, దశాంశం s);
శూన్య embed_image(image ఆధారం, image dest, పూర్ణాంకం dx, పూర్ణాంకం dy);
శూన్య ఉంచు_image(image im, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం dx, పూర్ణాంకం dy, image canvas);
శూన్య saturate_image(image im, దశాంశం sat);
శూన్య exposure_image(image im, దశాంశం sat);
శూన్య distort_image(image im, దశాంశం hue, దశాంశం sat, దశాంశం val);
శూన్య saturate_exposure_image(image im, దశాంశం sat, దశాంశం exposure);
శూన్య rgb_గమ్యం_hsv(image im);
శూన్య hsv_గమ్యం_rgb(image im);
శూన్య yuv_గమ్యం_rgb(image im);
శూన్య rgb_గమ్యం_yuv(image im);


image collapse_image_layers(image ఆధారం, పూర్ణాంకం border);
image collapse_images_horz(image *ims, పూర్ణాంకం n);
image collapse_images_vert(image *ims, పూర్ణాంకం n);

శూన్య show_image_normalized(image im, స్థిరరాశి అక్షరం *name);
శూన్య show_images(image *ims, పూర్ణాంకం n, అక్షరం *కిటికీ);
శూన్య show_image_layers(image p, అక్షరం *name);
శూన్య show_image_collapsed(image p, అక్షరం *name);

శూన్య print_image(image m);

image make_ఖాళీలు_image(పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c);
శూన్య copy_image_into(image src, image dest);

image get_image_layer(image m, పూర్ణాంకం l);

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు

