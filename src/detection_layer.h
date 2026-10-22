#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer detection_layer;

detection_layer make_detection_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક n, પૂર્ણાંક size, પૂર્ણાંક classes, પૂર્ણાંક coords, પૂર્ણાંક rescore);
શૂન્ય forward_detection_layer(અચલ detection_layer l, network net);
શૂન્ય backward_detection_layer(અચલ detection_layer l, network net);

#ifdef GPU
શૂન્ય forward_detection_layer_gpu(અચલ detection_layer l, network net);
શૂન્ય backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
