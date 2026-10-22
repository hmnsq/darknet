#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

প্রকারনির্দেশ layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক crop_height, পূর্ণাঙ্ক crop_width, পূর্ণাঙ্ক flip, ভিন্ন angle, ভিন্ন saturation, ভিন্ন exposure);
শূন্য forward_crop_layer(ধ্রুৱ crop_layer l, network net);
শূন্য resize_crop_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);

#ifdef GPU
শূন্য forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

