#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা crop_height, পূর্ণসংখ্যা crop_width, পূর্ণসংখ্যা flip, দশমিক angle, দশমিক saturation, দশমিক exposure);
শূন্য forward_crop_layer(ধ্রুবক crop_layer l, network net);
শূন্য resize_crop_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);

#ifdef GPU
শূন্য forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

