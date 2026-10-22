#ಅನ್ತರ್ಭಾವ "activations.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

ಅಕ್ಷರ *get_activation_string(ACTIVATION a)
{
    ಆಯ್ಕೆ(a){
        ಸ್ಥಿತಿ LOGISTIC:
            ಹಿಂತಿರುಗಿ "logistic";
        ಸ್ಥಿತಿ LOGGY:
            ಹಿಂತಿರುಗಿ "loggy";
        ಸ್ಥಿತಿ RELU:
            ಹಿಂತಿರುಗಿ "relu";
        ಸ್ಥಿತಿ ELU:
            ಹಿಂತಿರುಗಿ "elu";
        ಸ್ಥಿತಿ SELU:
            ಹಿಂತಿರುಗಿ "selu";
        ಸ್ಥಿತಿ RELIE:
            ಹಿಂತಿರುಗಿ "relie";
        ಸ್ಥಿತಿ RAMP:
            ಹಿಂತಿರುಗಿ "ramp";
        ಸ್ಥಿತಿ ರೇಖೀಯ:
            ಹಿಂತಿರುಗಿ "linear";
        ಸ್ಥಿತಿ TANH:
            ಹಿಂತಿರುಗಿ "tanh";
        ಸ್ಥಿತಿ PLSE:
            ಹಿಂತಿರುಗಿ "plse";
        ಸ್ಥಿತಿ LEAKY:
            ಹಿಂತಿರುಗಿ "leaky";
        ಸ್ಥಿತಿ STAIR:
            ಹಿಂತಿರುಗಿ "stair";
        ಸ್ಥಿತಿ HARDTAN:
            ಹಿಂತಿರುಗಿ "hardtan";
        ಸ್ಥಿತಿ LHTAN:
            ಹಿಂತಿರುಗಿ "lhtan";
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ "relu";
}

ACTIVATION get_activation(ಅಕ್ಷರ *s)
{
    ಆದರೆ (strcmp(s, "logistic")==0) ಹಿಂತಿರುಗಿ LOGISTIC;
    ಆದರೆ (strcmp(s, "loggy")==0) ಹಿಂತಿರುಗಿ LOGGY;
    ಆದರೆ (strcmp(s, "relu")==0) ಹಿಂತಿರುಗಿ RELU;
    ಆದರೆ (strcmp(s, "elu")==0) ಹಿಂತಿರುಗಿ ELU;
    ಆದರೆ (strcmp(s, "selu")==0) ಹಿಂತಿರುಗಿ SELU;
    ಆದರೆ (strcmp(s, "relie")==0) ಹಿಂತಿರುಗಿ RELIE;
    ಆದರೆ (strcmp(s, "plse")==0) ಹಿಂತಿರುಗಿ PLSE;
    ಆದರೆ (strcmp(s, "hardtan")==0) ಹಿಂತಿರುಗಿ HARDTAN;
    ಆದರೆ (strcmp(s, "lhtan")==0) ಹಿಂತಿರುಗಿ LHTAN;
    ಆದರೆ (strcmp(s, "linear")==0) ಹಿಂತಿರುಗಿ ರೇಖೀಯ;
    ಆದರೆ (strcmp(s, "ramp")==0) ಹಿಂತಿರುಗಿ RAMP;
    ಆದರೆ (strcmp(s, "leaky")==0) ಹಿಂತಿರುಗಿ LEAKY;
    ಆದರೆ (strcmp(s, "tanh")==0) ಹಿಂತಿರುಗಿ TANH;
    ಆದರೆ (strcmp(s, "stair")==0) ಹಿಂತಿರುಗಿ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ಹಿಂತಿರುಗಿ RELU;
}

ದಶಮಾಂಶ activate(ದಶಮಾಂಶ x, ACTIVATION a)
{
    ಆಯ್ಕೆ(a){
        ಸ್ಥಿತಿ ರೇಖೀಯ:
            ಹಿಂತಿರುಗಿ ರೇಖೀಯ_activate(x);
        ಸ್ಥಿತಿ LOGISTIC:
            ಹಿಂತಿರುಗಿ logistic_activate(x);
        ಸ್ಥಿತಿ LOGGY:
            ಹಿಂತಿರುಗಿ loggy_activate(x);
        ಸ್ಥಿತಿ RELU:
            ಹಿಂತಿರುಗಿ relu_activate(x);
        ಸ್ಥಿತಿ ELU:
            ಹಿಂತಿರುಗಿ elu_activate(x);
        ಸ್ಥಿತಿ SELU:
            ಹಿಂತಿರುಗಿ selu_activate(x);
        ಸ್ಥಿತಿ RELIE:
            ಹಿಂತಿರುಗಿ relie_activate(x);
        ಸ್ಥಿತಿ RAMP:
            ಹಿಂತಿರುಗಿ ramp_activate(x);
        ಸ್ಥಿತಿ LEAKY:
            ಹಿಂತಿರುಗಿ leaky_activate(x);
        ಸ್ಥಿತಿ TANH:
            ಹಿಂತಿರುಗಿ tanh_activate(x);
        ಸ್ಥಿತಿ PLSE:
            ಹಿಂತಿರುಗಿ plse_activate(x);
        ಸ್ಥಿತಿ STAIR:
            ಹಿಂತಿರುಗಿ stair_activate(x);
        ಸ್ಥಿತಿ HARDTAN:
            ಹಿಂತಿರುಗಿ hardtan_activate(x);
        ಸ್ಥಿತಿ LHTAN:
            ಹಿಂತಿರುಗಿ lhtan_activate(x);
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ activate_array(ದಶಮಾಂಶ *x, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n, ಸ್ಥಿರಾಂಕ ACTIVATION a)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

ದಶಮಾಂಶ gradient(ದಶಮಾಂಶ x, ACTIVATION a)
{
    ಆಯ್ಕೆ(a){
        ಸ್ಥಿತಿ ರೇಖೀಯ:
            ಹಿಂತಿರುಗಿ ರೇಖೀಯ_gradient(x);
        ಸ್ಥಿತಿ LOGISTIC:
            ಹಿಂತಿರುಗಿ logistic_gradient(x);
        ಸ್ಥಿತಿ LOGGY:
            ಹಿಂತಿರುಗಿ loggy_gradient(x);
        ಸ್ಥಿತಿ RELU:
            ಹಿಂತಿರುಗಿ relu_gradient(x);
        ಸ್ಥಿತಿ ELU:
            ಹಿಂತಿರುಗಿ elu_gradient(x);
        ಸ್ಥಿತಿ SELU:
            ಹಿಂತಿರುಗಿ selu_gradient(x);
        ಸ್ಥಿತಿ RELIE:
            ಹಿಂತಿರುಗಿ relie_gradient(x);
        ಸ್ಥಿತಿ RAMP:
            ಹಿಂತಿರುಗಿ ramp_gradient(x);
        ಸ್ಥಿತಿ LEAKY:
            ಹಿಂತಿರುಗಿ leaky_gradient(x);
        ಸ್ಥಿತಿ TANH:
            ಹಿಂತಿರುಗಿ tanh_gradient(x);
        ಸ್ಥಿತಿ PLSE:
            ಹಿಂತಿರುಗಿ plse_gradient(x);
        ಸ್ಥಿತಿ STAIR:
            ಹಿಂತಿರುಗಿ stair_gradient(x);
        ಸ್ಥಿತಿ HARDTAN:
            ಹಿಂತಿರುಗಿ hardtan_gradient(x);
        ಸ್ಥಿತಿ LHTAN:
            ಹಿಂತಿರುಗಿ lhtan_gradient(x);
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ gradient_array(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *x, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n, ಸ್ಥಿರಾಂಕ ACTIVATION a, ದಶಮಾಂಶ *delta)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

