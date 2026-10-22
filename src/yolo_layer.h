#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#અન્તર્ભાવ "darknet.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_yolo_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક n, પૂર્ણાંક total, પૂર્ણાંક *mask, પૂર્ણાંક classes);
શૂન્ય forward_yolo_layer(અચલ layer l, network net);
શૂન્ય backward_yolo_layer(અચલ layer l, network net);
શૂન્ય resize_yolo_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h);
પૂર્ણાંક yolo_અંક_detections(layer l, દશાંશ thresh);

#ifdef GPU
શૂન્ય forward_yolo_layer_gpu(અચલ layer l, network net);
શૂન્ય backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
