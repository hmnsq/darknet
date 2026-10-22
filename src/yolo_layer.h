#నిర్వచితం_కాకపోతే YOLO_LAYER_H
#నిర్వచించు YOLO_LAYER_H

#అన్తర్భావ "darknet.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_yolo_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం n, పూర్ణాంకం మొత్తం, పూర్ణాంకం *ముసుగు, పూర్ణాంకం classes);
శూన్య forward_yolo_layer(స్థిరరాశి layer l, జాలం net);
శూన్య backward_yolo_layer(స్థిరరాశి layer l, జాలం net);
శూన్య resize_yolo_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h);
పూర్ణాంకం yolo_అంకె_detections(layer l, దశాంశం thresh);

#నిర్వచితమైతే GPU
శూన్య forward_yolo_layer_gpu(స్థిరరాశి layer l, జాలం net);
శూన్య backward_yolo_layer_gpu(layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు
