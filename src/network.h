// Oh boy, why am I about to do this....
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಜಾಲ_H
#ವ್ಯಾಖ್ಯಾನಿಸು ಜಾಲ_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "data.h"
#ಅನ್ತರ್ಭಾವ "tree.h"


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ pull_ಜಾಲ_output(ಜಾಲ *net);
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ compare_networks(ಜಾಲ *n1, ಜಾಲ *n2, ದತ್ತಾಂಶ d);
ಅಕ್ಷರ *get_layer_string(LAYER_TYPE a);

ಜಾಲ *make_ಜಾಲ(ಪೂರ್ಣಾಂಕ n);


ದಶಮಾಂಶ ಜಾಲ_accuracy_multi(ಜಾಲ *net, ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ n);
ಪೂರ್ಣಾಂಕ get_predicted_class_ಜಾಲ(ಜಾಲ *net);
ಶೂನ್ಯ print_ಜಾಲ(ಜಾಲ *net);
ಪೂರ್ಣಾಂಕ resize_ಜಾಲ(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಶೂನ್ಯ calc_ಜಾಲ_ವೆಚ್ಚ(ಜಾಲ *net);

#ಆದರೆ_ಅಂತ್ಯ

