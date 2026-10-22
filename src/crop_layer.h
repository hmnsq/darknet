#నిర్వచితం_కాకపోతే CROP_LAYER_H
#నిర్వచించు CROP_LAYER_H

#అన్తర్భావ "image.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం crop_ఎత్తు, పూర్ణాంకం crop_వెడల్పు, పూర్ణాంకం flip, దశాంశం angle, దశాంశం saturation, దశాంశం exposure);
శూన్య forward_crop_layer(స్థిరరాశి crop_layer l, జాలం net);
శూన్య resize_crop_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h);

#నిర్వచితమైతే GPU
శూన్య forward_crop_layer_gpu(crop_layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

