#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#شامل "darknet.h"
#شامل "cuda.h"
#شامل "math.h"

ACTIVATION get_activation(نویسه *s);

نویسه *get_activation_string(ACTIVATION a);
اعشاری activate(اعشاری x, ACTIVATION a);
اعشاری gradient(اعشاری x, ACTIVATION a);
تهی gradient_array(ثابت اعشاری *x, ثابت صحیح n, ثابت ACTIVATION a, اعشاری *delta);
تهی activate_array(اعشاری *x, ثابت صحیح n, ثابت ACTIVATION a);
#ifdef GPU
تهی activate_array_gpu(اعشاری *x, صحیح n, ACTIVATION a);
تهی gradient_array_gpu(اعشاری *x, صحیح n, ACTIVATION a, اعشاری *delta);
#endif

ایستا درونخطی اعشاری stair_activate(اعشاری x)
{
    صحیح n = floor(x);
    اگر (n%2 == 0) بازگشت floor(x/2.);
    وگرنه بازگشت (x - n) + floor(x/2.);
}
ایستا درونخطی اعشاری hardtan_activate(اعشاری x)
{
    اگر (x < -1) بازگشت -1;
    اگر (x > 1) بازگشت 1;
    بازگشت x;
}
ایستا درونخطی اعشاری linear_activate(اعشاری x){بازگشت x;}
ایستا درونخطی اعشاری logistic_activate(اعشاری x){بازگشت 1./(1. + exp(-x));}
ایستا درونخطی اعشاری loggy_activate(اعشاری x){بازگشت 2./(1. + exp(-x)) - 1;}
ایستا درونخطی اعشاری relu_activate(اعشاری x){بازگشت x*(x>0);}
ایستا درونخطی اعشاری elu_activate(اعشاری x){بازگشت (x >= 0)*x + (x < 0)*(exp(x)-1);}
ایستا درونخطی اعشاری selu_activate(اعشاری x){بازگشت (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
ایستا درونخطی اعشاری relie_activate(اعشاری x){بازگشت (x>0) ? x : .01*x;}
ایستا درونخطی اعشاری ramp_activate(اعشاری x){بازگشت x*(x>0)+.1*x;}
ایستا درونخطی اعشاری leaky_activate(اعشاری x){بازگشت (x>0) ? x : .1*x;}
ایستا درونخطی اعشاری tanh_activate(اعشاری x){بازگشت (exp(2*x)-1)/(exp(2*x)+1);}
ایستا درونخطی اعشاری plse_activate(اعشاری x)
{
    اگر(x < -4) بازگشت .01 * (x + 4);
    اگر(x > 4)  بازگشت .01 * (x - 4) + 1;
    بازگشت .125*x + .5;
}

ایستا درونخطی اعشاری lhtan_activate(اعشاری x)
{
    اگر(x < 0) بازگشت .001*x;
    اگر(x > 1) بازگشت .001*(x-1) + 1;
    بازگشت x;
}
ایستا درونخطی اعشاری lhtan_gradient(اعشاری x)
{
    اگر(x > 0 && x < 1) بازگشت 1;
    بازگشت .001;
}

ایستا درونخطی اعشاری hardtan_gradient(اعشاری x)
{
    اگر (x > -1 && x < 1) بازگشت 1;
    بازگشت 0;
}
ایستا درونخطی اعشاری linear_gradient(اعشاری x){بازگشت 1;}
ایستا درونخطی اعشاری logistic_gradient(اعشاری x){بازگشت (1-x)*x;}
ایستا درونخطی اعشاری loggy_gradient(اعشاری x)
{
    اعشاری y = (x+1.)/2.;
    بازگشت 2*(1-y)*y;
}
ایستا درونخطی اعشاری stair_gradient(اعشاری x)
{
    اگر (floor(x) == x) بازگشت 0;
    بازگشت 1;
}
ایستا درونخطی اعشاری relu_gradient(اعشاری x){بازگشت (x>0);}
ایستا درونخطی اعشاری elu_gradient(اعشاری x){بازگشت (x >= 0) + (x < 0)*(x + 1);}
ایستا درونخطی اعشاری selu_gradient(اعشاری x){بازگشت (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
ایستا درونخطی اعشاری relie_gradient(اعشاری x){بازگشت (x>0) ? 1 : .01;}
ایستا درونخطی اعشاری ramp_gradient(اعشاری x){بازگشت (x>0)+.1;}
ایستا درونخطی اعشاری leaky_gradient(اعشاری x){بازگشت (x>0) ? 1 : .1;}
ایستا درونخطی اعشاری tanh_gradient(اعشاری x){بازگشت 1-x*x;}
ایستا درونخطی اعشاری plse_gradient(اعشاری x){بازگشت (x < 0 || x > 1) ? .01 : .125;}

#endif

