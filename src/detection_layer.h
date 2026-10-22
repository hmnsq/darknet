#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer detection_layer;

detection_layer make_detection_layer(முழு batch, முழு inputs, முழு n, முழு size, முழு classes, முழு coords, முழு rescore);
வெறுமை forward_detection_layer(மாறிலி detection_layer l, network net);
வெறுமை backward_detection_layer(மாறிலி detection_layer l, network net);

#ifdef GPU
வெறுமை forward_detection_layer_gpu(மாறிலி detection_layer l, network net);
வெறுமை backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
