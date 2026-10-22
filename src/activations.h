#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#अन्तर्भाव "darknet.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "math.h"

ACTIVATION get_activation(वर्ण *s);

वर्ण *get_activation_string(ACTIVATION a);
भिन्न activate(भिन्न x, ACTIVATION a);
भिन्न gradient(भिन्न x, ACTIVATION a);
शून्य gradient_array(अचर भिन्न *x, अचर पूर्णाङ्क n, अचर ACTIVATION a, भिन्न *delta);
शून्य activate_array(भिन्न *x, अचर पूर्णाङ्क n, अचर ACTIVATION a);
#ifdef GPU
शून्य activate_array_gpu(भिन्न *x, पूर्णाङ्क n, ACTIVATION a);
शून्य gradient_array_gpu(भिन्न *x, पूर्णाङ्क n, ACTIVATION a, भिन्न *delta);
#endif

स्थिर अन्तःस्थ भिन्न stair_activate(भिन्न x)
{
    पूर्णाङ्क n = न्यूनमान(x);
    यदि (n%2 == 0) फर्काऊ न्यूनमान(x/2.);
    अन्यथा फर्काऊ (x - n) + न्यूनमान(x/2.);
}
स्थिर अन्तःस्थ भिन्न hardtan_activate(भिन्न x)
{
    यदि (x < -1) फर्काऊ -1;
    यदि (x > 1) फर्काऊ 1;
    फर्काऊ x;
}
स्थिर अन्तःस्थ भिन्न linear_activate(भिन्न x){फर्काऊ x;}
स्थिर अन्तःस्थ भिन्न logistic_activate(भिन्न x){फर्काऊ 1./(1. + exp(-x));}
स्थिर अन्तःस्थ भिन्न loggy_activate(भिन्न x){फर्काऊ 2./(1. + exp(-x)) - 1;}
स्थिर अन्तःस्थ भिन्न relu_activate(भिन्न x){फर्काऊ x*(x>0);}
स्थिर अन्तःस्थ भिन्न elu_activate(भिन्न x){फर्काऊ (x >= 0)*x + (x < 0)*(exp(x)-1);}
स्थिर अन्तःस्थ भिन्न selu_activate(भिन्न x){फर्काऊ (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
स्थिर अन्तःस्थ भिन्न relie_activate(भिन्न x){फर्काऊ (x>0) ? x : .01*x;}
स्थिर अन्तःस्थ भिन्न ramp_activate(भिन्न x){फर्काऊ x*(x>0)+.1*x;}
स्थिर अन्तःस्थ भिन्न leaky_activate(भिन्न x){फर्काऊ (x>0) ? x : .1*x;}
स्थिर अन्तःस्थ भिन्न tanh_activate(भिन्न x){फर्काऊ (exp(2*x)-1)/(exp(2*x)+1);}
स्थिर अन्तःस्थ भिन्न plse_activate(भिन्न x)
{
    यदि(x < -4) फर्काऊ .01 * (x + 4);
    यदि(x > 4)  फर्काऊ .01 * (x - 4) + 1;
    फर्काऊ .125*x + .5;
}

स्थिर अन्तःस्थ भिन्न lhtan_activate(भिन्न x)
{
    यदि(x < 0) फर्काऊ .001*x;
    यदि(x > 1) फर्काऊ .001*(x-1) + 1;
    फर्काऊ x;
}
स्थिर अन्तःस्थ भिन्न lhtan_gradient(भिन्न x)
{
    यदि(x > 0 && x < 1) फर्काऊ 1;
    फर्काऊ .001;
}

स्थिर अन्तःस्थ भिन्न hardtan_gradient(भिन्न x)
{
    यदि (x > -1 && x < 1) फर्काऊ 1;
    फर्काऊ 0;
}
स्थिर अन्तःस्थ भिन्न linear_gradient(भिन्न x){फर्काऊ 1;}
स्थिर अन्तःस्थ भिन्न logistic_gradient(भिन्न x){फर्काऊ (1-x)*x;}
स्थिर अन्तःस्थ भिन्न loggy_gradient(भिन्न x)
{
    भिन्न y = (x+1.)/2.;
    फर्काऊ 2*(1-y)*y;
}
स्थिर अन्तःस्थ भिन्न stair_gradient(भिन्न x)
{
    यदि (न्यूनमान(x) == x) फर्काऊ 0;
    फर्काऊ 1;
}
स्थिर अन्तःस्थ भिन्न relu_gradient(भिन्न x){फर्काऊ (x>0);}
स्थिर अन्तःस्थ भिन्न elu_gradient(भिन्न x){फर्काऊ (x >= 0) + (x < 0)*(x + 1);}
स्थिर अन्तःस्थ भिन्न selu_gradient(भिन्न x){फर्काऊ (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
स्थिर अन्तःस्थ भिन्न relie_gradient(भिन्न x){फर्काऊ (x>0) ? 1 : .01;}
स्थिर अन्तःस्थ भिन्न ramp_gradient(भिन्न x){फर्काऊ (x>0)+.1;}
स्थिर अन्तःस्थ भिन्न leaky_gradient(भिन्न x){फर्काऊ (x>0) ? 1 : .1;}
स्थिर अन्तःस्थ भिन्न tanh_gradient(भिन्न x){फर्काऊ 1-x*x;}
स्थिर अन्तःस्थ भिन्न plse_gradient(भिन्न x){फर्काऊ (x < 0 || x > 1) ? .01 : .125;}

#endif

