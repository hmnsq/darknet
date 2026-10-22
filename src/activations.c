#अन्तर्भाव "activations.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

वर्ण *get_activation_string(ACTIVATION a)
{
    विकल्प(a){
        पक्ष LOGISTIC:
            प्रत्यागम "logistic";
        पक्ष LOGGY:
            प्रत्यागम "loggy";
        पक्ष RELU:
            प्रत्यागम "relu";
        पक्ष ELU:
            प्रत्यागम "elu";
        पक्ष SELU:
            प्रत्यागम "selu";
        पक्ष RELIE:
            प्रत्यागम "relie";
        पक्ष RAMP:
            प्रत्यागम "ramp";
        पक्ष LINEAR:
            प्रत्यागम "linear";
        पक्ष TANH:
            प्रत्यागम "tanh";
        पक्ष PLSE:
            प्रत्यागम "plse";
        पक्ष LEAKY:
            प्रत्यागम "leaky";
        पक्ष STAIR:
            प्रत्यागम "stair";
        पक्ष HARDTAN:
            प्रत्यागम "hardtan";
        पक्ष LHTAN:
            प्रत्यागम "lhtan";
        स्वभाव:
            भङ्ग;
    }
    प्रत्यागम "relu";
}

ACTIVATION get_activation(वर्ण *s)
{
    यदि (strcmp(s, "logistic")==0) प्रत्यागम LOGISTIC;
    यदि (strcmp(s, "loggy")==0) प्रत्यागम LOGGY;
    यदि (strcmp(s, "relu")==0) प्रत्यागम RELU;
    यदि (strcmp(s, "elu")==0) प्रत्यागम ELU;
    यदि (strcmp(s, "selu")==0) प्रत्यागम SELU;
    यदि (strcmp(s, "relie")==0) प्रत्यागम RELIE;
    यदि (strcmp(s, "plse")==0) प्रत्यागम PLSE;
    यदि (strcmp(s, "hardtan")==0) प्रत्यागम HARDTAN;
    यदि (strcmp(s, "lhtan")==0) प्रत्यागम LHTAN;
    यदि (strcmp(s, "linear")==0) प्रत्यागम LINEAR;
    यदि (strcmp(s, "ramp")==0) प्रत्यागम RAMP;
    यदि (strcmp(s, "leaky")==0) प्रत्यागम LEAKY;
    यदि (strcmp(s, "tanh")==0) प्रत्यागम TANH;
    यदि (strcmp(s, "stair")==0) प्रत्यागम STAIR;
    fprintf(stderr, "Couldn't find activation function %s, going with ReLU\n", s);
    प्रत्यागम RELU;
}

भिन्न activate(भिन्न x, ACTIVATION a)
{
    विकल्प(a){
        पक्ष LINEAR:
            प्रत्यागम linear_activate(x);
        पक्ष LOGISTIC:
            प्रत्यागम logistic_activate(x);
        पक्ष LOGGY:
            प्रत्यागम loggy_activate(x);
        पक्ष RELU:
            प्रत्यागम relu_activate(x);
        पक्ष ELU:
            प्रत्यागम elu_activate(x);
        पक्ष SELU:
            प्रत्यागम selu_activate(x);
        पक्ष RELIE:
            प्रत्यागम relie_activate(x);
        पक्ष RAMP:
            प्रत्यागम ramp_activate(x);
        पक्ष LEAKY:
            प्रत्यागम leaky_activate(x);
        पक्ष TANH:
            प्रत्यागम tanh_activate(x);
        पक्ष PLSE:
            प्रत्यागम plse_activate(x);
        पक्ष STAIR:
            प्रत्यागम stair_activate(x);
        पक्ष HARDTAN:
            प्रत्यागम hardtan_activate(x);
        पक्ष LHTAN:
            प्रत्यागम lhtan_activate(x);
    }
    प्रत्यागम 0;
}

शून्य activate_array(भिन्न *x, ध्रुव पूर्णाङ्क n, ध्रुव ACTIVATION a)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

भिन्न gradient(भिन्न x, ACTIVATION a)
{
    विकल्प(a){
        पक्ष LINEAR:
            प्रत्यागम linear_gradient(x);
        पक्ष LOGISTIC:
            प्रत्यागम logistic_gradient(x);
        पक्ष LOGGY:
            प्रत्यागम loggy_gradient(x);
        पक्ष RELU:
            प्रत्यागम relu_gradient(x);
        पक्ष ELU:
            प्रत्यागम elu_gradient(x);
        पक्ष SELU:
            प्रत्यागम selu_gradient(x);
        पक्ष RELIE:
            प्रत्यागम relie_gradient(x);
        पक्ष RAMP:
            प्रत्यागम ramp_gradient(x);
        पक्ष LEAKY:
            प्रत्यागम leaky_gradient(x);
        पक्ष TANH:
            प्रत्यागम tanh_gradient(x);
        पक्ष PLSE:
            प्रत्यागम plse_gradient(x);
        पक्ष STAIR:
            प्रत्यागम stair_gradient(x);
        पक्ष HARDTAN:
            प्रत्यागम hardtan_gradient(x);
        पक्ष LHTAN:
            प्रत्यागम lhtan_gradient(x);
    }
    प्रत्यागम 0;
}

शून्य gradient_array(ध्रुव भिन्न *x, ध्रुव पूर्णाङ्क n, ध्रुव ACTIVATION a, भिन्न *delta)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

