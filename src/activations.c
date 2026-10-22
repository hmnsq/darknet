#ಅನ್ತರ್ಭಾವ "activations.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

ವರ್ಣ *get_activation_string(ACTIVATION a)
{
    ವಿಕಲ್ಪ(a){
        ಪಕ್ಷ LOGISTIC:
            ಪ್ರತ್ಯಾಗಮ "logistic";
        ಪಕ್ಷ LOGGY:
            ಪ್ರತ್ಯಾಗಮ "loggy";
        ಪಕ್ಷ RELU:
            ಪ್ರತ್ಯಾಗಮ "relu";
        ಪಕ್ಷ ELU:
            ಪ್ರತ್ಯಾಗಮ "elu";
        ಪಕ್ಷ SELU:
            ಪ್ರತ್ಯಾಗಮ "selu";
        ಪಕ್ಷ RELIE:
            ಪ್ರತ್ಯಾಗಮ "relie";
        ಪಕ್ಷ RAMP:
            ಪ್ರತ್ಯಾಗಮ "ramp";
        ಪಕ್ಷ LINEAR:
            ಪ್ರತ್ಯಾಗಮ "linear";
        ಪಕ್ಷ TANH:
            ಪ್ರತ್ಯಾಗಮ "tanh";
        ಪಕ್ಷ PLSE:
            ಪ್ರತ್ಯಾಗಮ "plse";
        ಪಕ್ಷ LEAKY:
            ಪ್ರತ್ಯಾಗಮ "leaky";
        ಪಕ್ಷ STAIR:
            ಪ್ರತ್ಯಾಗಮ "stair";
        ಪಕ್ಷ HARDTAN:
            ಪ್ರತ್ಯಾಗಮ "hardtan";
        ಪಕ್ಷ LHTAN:
            ಪ್ರತ್ಯಾಗಮ "lhtan";
        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ "relu";
}

ACTIVATION get_activation(ವರ್ಣ *s)
{
    ಯದಿ (strcmp(s, "logistic")==0) ಪ್ರತ್ಯಾಗಮ LOGISTIC;
    ಯದಿ (strcmp(s, "loggy")==0) ಪ್ರತ್ಯಾಗಮ LOGGY;
    ಯದಿ (strcmp(s, "relu")==0) ಪ್ರತ್ಯಾಗಮ RELU;
    ಯದಿ (strcmp(s, "elu")==0) ಪ್ರತ್ಯಾಗಮ ELU;
    ಯದಿ (strcmp(s, "selu")==0) ಪ್ರತ್ಯಾಗಮ SELU;
    ಯದಿ (strcmp(s, "relie")==0) ಪ್ರತ್ಯಾಗಮ RELIE;
    ಯದಿ (strcmp(s, "plse")==0) ಪ್ರತ್ಯಾಗಮ PLSE;
    ಯದಿ (strcmp(s, "hardtan")==0) ಪ್ರತ್ಯಾಗಮ HARDTAN;
    ಯದಿ (strcmp(s, "lhtan")==0) ಪ್ರತ್ಯಾಗಮ LHTAN;
    ಯದಿ (strcmp(s, "linear")==0) ಪ್ರತ್ಯಾಗಮ LINEAR;
    ಯದಿ (strcmp(s, "ramp")==0) ಪ್ರತ್ಯಾಗಮ RAMP;
    ಯದಿ (strcmp(s, "leaky")==0) ಪ್ರತ್ಯಾಗಮ LEAKY;
    ಯದಿ (strcmp(s, "tanh")==0) ಪ್ರತ್ಯಾಗಮ TANH;
    ಯದಿ (strcmp(s, "stair")==0) ಪ್ರತ್ಯಾಗಮ STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    ಪ್ರತ್ಯಾಗಮ RELU;
}

ಭಿನ್ನ activate(ಭಿನ್ನ x, ACTIVATION a)
{
    ವಿಕಲ್ಪ(a){
        ಪಕ್ಷ LINEAR:
            ಪ್ರತ್ಯಾಗಮ linear_activate(x);
        ಪಕ್ಷ LOGISTIC:
            ಪ್ರತ್ಯಾಗಮ logistic_activate(x);
        ಪಕ್ಷ LOGGY:
            ಪ್ರತ್ಯಾಗಮ loggy_activate(x);
        ಪಕ್ಷ RELU:
            ಪ್ರತ್ಯಾಗಮ relu_activate(x);
        ಪಕ್ಷ ELU:
            ಪ್ರತ್ಯಾಗಮ elu_activate(x);
        ಪಕ್ಷ SELU:
            ಪ್ರತ್ಯಾಗಮ selu_activate(x);
        ಪಕ್ಷ RELIE:
            ಪ್ರತ್ಯಾಗಮ relie_activate(x);
        ಪಕ್ಷ RAMP:
            ಪ್ರತ್ಯಾಗಮ ramp_activate(x);
        ಪಕ್ಷ LEAKY:
            ಪ್ರತ್ಯಾಗಮ leaky_activate(x);
        ಪಕ್ಷ TANH:
            ಪ್ರತ್ಯಾಗಮ tanh_activate(x);
        ಪಕ್ಷ PLSE:
            ಪ್ರತ್ಯಾಗಮ plse_activate(x);
        ಪಕ್ಷ STAIR:
            ಪ್ರತ್ಯಾಗಮ stair_activate(x);
        ಪಕ್ಷ HARDTAN:
            ಪ್ರತ್ಯಾಗಮ hardtan_activate(x);
        ಪಕ್ಷ LHTAN:
            ಪ್ರತ್ಯಾಗಮ lhtan_activate(x);
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ activate_array(ಭಿನ್ನ *x, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n, ಧ್ರುವ ACTIVATION a)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

ಭಿನ್ನ gradient(ಭಿನ್ನ x, ACTIVATION a)
{
    ವಿಕಲ್ಪ(a){
        ಪಕ್ಷ LINEAR:
            ಪ್ರತ್ಯಾಗಮ linear_gradient(x);
        ಪಕ್ಷ LOGISTIC:
            ಪ್ರತ್ಯಾಗಮ logistic_gradient(x);
        ಪಕ್ಷ LOGGY:
            ಪ್ರತ್ಯಾಗಮ loggy_gradient(x);
        ಪಕ್ಷ RELU:
            ಪ್ರತ್ಯಾಗಮ relu_gradient(x);
        ಪಕ್ಷ ELU:
            ಪ್ರತ್ಯಾಗಮ elu_gradient(x);
        ಪಕ್ಷ SELU:
            ಪ್ರತ್ಯಾಗಮ selu_gradient(x);
        ಪಕ್ಷ RELIE:
            ಪ್ರತ್ಯಾಗಮ relie_gradient(x);
        ಪಕ್ಷ RAMP:
            ಪ್ರತ್ಯಾಗಮ ramp_gradient(x);
        ಪಕ್ಷ LEAKY:
            ಪ್ರತ್ಯಾಗಮ leaky_gradient(x);
        ಪಕ್ಷ TANH:
            ಪ್ರತ್ಯಾಗಮ tanh_gradient(x);
        ಪಕ್ಷ PLSE:
            ಪ್ರತ್ಯಾಗಮ plse_gradient(x);
        ಪಕ್ಷ STAIR:
            ಪ್ರತ್ಯಾಗಮ stair_gradient(x);
        ಪಕ್ಷ HARDTAN:
            ಪ್ರತ್ಯಾಗಮ hardtan_gradient(x);
        ಪಕ್ಷ LHTAN:
            ಪ್ರತ್ಯಾಗಮ lhtan_gradient(x);
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ gradient_array(ಧ್ರುವ ಭಿನ್ನ *x, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n, ಧ್ರುವ ACTIVATION a, ಭಿನ್ನ *delta)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

