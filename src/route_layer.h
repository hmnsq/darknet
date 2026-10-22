#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#অন্তর্ভাব "network.h"
#অন্তর্ভাব "layer.h"

প্রকারলেখ layer route_layer;

route_layer make_route_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা *প্রবেশ_layers, পূর্ণসংখ্যা *প্রবেশ_আকার);
শূন্য forward_route_layer(ধ্রুবক route_layer l, network net);
শূন্য backward_route_layer(ধ্রুবক route_layer l, network net);
শূন্য resize_route_layer(route_layer *l, network *net);

#ifdef GPU
শূন্য forward_route_layer_gpu(ধ্রুবক route_layer l, network net);
শূন্য backward_route_layer_gpu(ধ্রুবক route_layer l, network net);
#endif

#endif
