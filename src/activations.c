#अन्तर्भाव "activations.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

वर्ण *get_activation_string(ACTIVATION a)
{
    विकल्प(a){
        पक्ष LOGISTIC:
            फर्काऊ "logistic";
        पक्ष LOGGY:
            फर्काऊ "loggy";
        पक्ष RELU:
            फर्काऊ "relu";
        पक्ष ELU:
            फर्काऊ "elu";
        पक्ष SELU:
            फर्काऊ "selu";
        पक्ष RELIE:
            फर्काऊ "relie";
        पक्ष RAMP:
            फर्काऊ "ramp";
        पक्ष LINEAR:
            फर्काऊ "linear";
        पक्ष TANH:
            फर्काऊ "tanh";
        पक्ष PLSE:
            फर्काऊ "plse";
        पक्ष LEAKY:
            फर्काऊ "leaky";
        पक्ष STAIR:
            फर्काऊ "stair";
        पक्ष HARDTAN:
            फर्काऊ "hardtan";
        पक्ष LHTAN:
            फर्काऊ "lhtan";
        स्वभाव:
            रोक;
    }
    फर्काऊ "relu";
}

ACTIVATION get_activation(वर्ण *s)
{
    यदि (म_भेद(s, "logistic")==0) फर्काऊ LOGISTIC;
    यदि (म_भेद(s, "loggy")==0) फर्काऊ LOGGY;
    यदि (म_भेद(s, "relu")==0) फर्काऊ RELU;
    यदि (म_भेद(s, "elu")==0) फर्काऊ ELU;
    यदि (म_भेद(s, "selu")==0) फर्काऊ SELU;
    यदि (म_भेद(s, "relie")==0) फर्काऊ RELIE;
    यदि (म_भेद(s, "plse")==0) फर्काऊ PLSE;
    यदि (म_भेद(s, "hardtan")==0) फर्काऊ HARDTAN;
    यदि (म_भेद(s, "lhtan")==0) फर्काऊ LHTAN;
    यदि (म_भेद(s, "linear")==0) फर्काऊ LINEAR;
    यदि (म_भेद(s, "ramp")==0) फर्काऊ RAMP;
    यदि (म_भेद(s, "leaky")==0) फर्काऊ LEAKY;
    यदि (म_भेद(s, "tanh")==0) फर्काऊ TANH;
    यदि (म_भेद(s, "stair")==0) फर्काऊ STAIR;
    ख_लिखो(मानक_त्रुटि, "Couldn't find activation function %s, going with ReLU\n", s);
    फर्काऊ RELU;
}

भिन्न activate(भिन्न x, ACTIVATION a)
{
    विकल्प(a){
        पक्ष LINEAR:
            फर्काऊ linear_activate(x);
        पक्ष LOGISTIC:
            फर्काऊ logistic_activate(x);
        पक्ष LOGGY:
            फर्काऊ loggy_activate(x);
        पक्ष RELU:
            फर्काऊ relu_activate(x);
        पक्ष ELU:
            फर्काऊ elu_activate(x);
        पक्ष SELU:
            फर्काऊ selu_activate(x);
        पक्ष RELIE:
            फर्काऊ relie_activate(x);
        पक्ष RAMP:
            फर्काऊ ramp_activate(x);
        पक्ष LEAKY:
            फर्काऊ leaky_activate(x);
        पक्ष TANH:
            फर्काऊ tanh_activate(x);
        पक्ष PLSE:
            फर्काऊ plse_activate(x);
        पक्ष STAIR:
            फर्काऊ stair_activate(x);
        पक्ष HARDTAN:
            फर्काऊ hardtan_activate(x);
        पक्ष LHTAN:
            फर्काऊ lhtan_activate(x);
    }
    फर्काऊ 0;
}

शून्य activate_array(भिन्न *x, अचर पूर्णाङ्क n, अचर ACTIVATION a)
{
    पूर्णाङ्क i;
    लागि(i = 0; i < n; ++i){
        x[i] = activate(x[i], a);
    }
}

भिन्न gradient(भिन्न x, ACTIVATION a)
{
    विकल्प(a){
        पक्ष LINEAR:
            फर्काऊ linear_gradient(x);
        पक्ष LOGISTIC:
            फर्काऊ logistic_gradient(x);
        पक्ष LOGGY:
            फर्काऊ loggy_gradient(x);
        पक्ष RELU:
            फर्काऊ relu_gradient(x);
        पक्ष ELU:
            फर्काऊ elu_gradient(x);
        पक्ष SELU:
            फर्काऊ selu_gradient(x);
        पक्ष RELIE:
            फर्काऊ relie_gradient(x);
        पक्ष RAMP:
            फर्काऊ ramp_gradient(x);
        पक्ष LEAKY:
            फर्काऊ leaky_gradient(x);
        पक्ष TANH:
            फर्काऊ tanh_gradient(x);
        पक्ष PLSE:
            फर्काऊ plse_gradient(x);
        पक्ष STAIR:
            फर्काऊ stair_gradient(x);
        पक्ष HARDTAN:
            फर्काऊ hardtan_gradient(x);
        पक्ष LHTAN:
            फर्काऊ lhtan_gradient(x);
    }
    फर्काऊ 0;
}

शून्य gradient_array(अचर भिन्न *x, अचर पूर्णाङ्क n, अचर ACTIVATION a, भिन्न *delta)
{
    पूर्णाङ्क i;
    लागि(i = 0; i < n; ++i){
        delta[i] *= gradient(x[i], a);
    }
} 

