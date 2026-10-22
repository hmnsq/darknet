#اگر_غیر_تعریف_شدہ YOLO_LAYER_H
#تعریف_کرو YOLO_LAYER_H

#شامل "darknet.h"
#شامل "layer.h"
#شامل "network.h"

layer make_yolo_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح کل, صحیح *نقاب, صحیح classes);
خالی forward_yolo_layer(ثابت layer l, جال net);
خالی backward_yolo_layer(ثابت layer l, جال net);
خالی resize_yolo_layer(layer *l, صحیح w, صحیح h);
صحیح yolo_عدد_detections(layer l, اعشاری thresh);

#اگر_تعریف_شدہ GPU
خالی forward_yolo_layer_gpu(ثابت layer l, جال net);
خالی backward_yolo_layer_gpu(layer l, جال net);
#اگر_ختم

#اگر_ختم
