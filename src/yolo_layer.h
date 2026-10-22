#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YOLO_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು YOLO_LAYER_H

#ಅನ್ತರ್ಭಾವ "darknet.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_yolo_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಒಟ್ಟು, ಪೂರ್ಣಾಂಕ *ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ classes);
ಶೂನ್ಯ forward_yolo_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_yolo_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_yolo_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಪೂರ್ಣಾಂಕ yolo_ಅಂಕೆ_detections(layer l, ದಶಮಾಂಶ thresh);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_yolo_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_yolo_layer_gpu(layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
