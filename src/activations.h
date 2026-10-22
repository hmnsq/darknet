#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#ضمن "darknet.h"
#ضمن "cuda.h"
#ضمن "math.h"

ACTIVATION get_activation(حرف *s);

حرف *get_activation_string(ACTIVATION a);
عائم activate(عائم x, ACTIVATION a);
عائم gradient(عائم x, ACTIVATION a);
فراغ gradient_array(ثابت عائم *x, ثابت صحيح n, ثابت ACTIVATION a, عائم *delta);
فراغ activate_array(عائم *x, ثابت صحيح n, ثابت ACTIVATION a);
#ifdef GPU
فراغ activate_array_gpu(عائم *x, صحيح n, ACTIVATION a);
فراغ gradient_array_gpu(عائم *x, صحيح n, ACTIVATION a, عائم *delta);
#endif

ساكن مضمن عائم stair_activate(عائم x)
{
    صحيح n = floor(x);
    ܐܢ (n%2 == 0) ܦܢܐ floor(x/2.);
    ܐܚܪܢܐ ܦܢܐ (x - n) + floor(x/2.);
}
ساكن مضمن عائم hardtan_activate(عائم x)
{
    ܐܢ (x < -1) ܦܢܐ -1;
    ܐܢ (x > 1) ܦܢܐ 1;
    ܦܢܐ x;
}
ساكن مضمن عائم linear_activate(عائم x){ܦܢܐ x;}
ساكن مضمن عائم logistic_activate(عائم x){ܦܢܐ 1./(1. + exp(-x));}
ساكن مضمن عائم loggy_activate(عائم x){ܦܢܐ 2./(1. + exp(-x)) - 1;}
ساكن مضمن عائم relu_activate(عائم x){ܦܢܐ x*(x>0);}
ساكن مضمن عائم elu_activate(عائم x){ܦܢܐ (x >= 0)*x + (x < 0)*(exp(x)-1);}
ساكن مضمن عائم selu_activate(عائم x){ܦܢܐ (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
ساكن مضمن عائم relie_activate(عائم x){ܦܢܐ (x>0) ? x : .01*x;}
ساكن مضمن عائم ramp_activate(عائم x){ܦܢܐ x*(x>0)+.1*x;}
ساكن مضمن عائم leaky_activate(عائم x){ܦܢܐ (x>0) ? x : .1*x;}
ساكن مضمن عائم tanh_activate(عائم x){ܦܢܐ (exp(2*x)-1)/(exp(2*x)+1);}
ساكن مضمن عائم plse_activate(عائم x)
{
    ܐܢ(x < -4) ܦܢܐ .01 * (x + 4);
    ܐܢ(x > 4)  ܦܢܐ .01 * (x - 4) + 1;
    ܦܢܐ .125*x + .5;
}

ساكن مضمن عائم lhtan_activate(عائم x)
{
    ܐܢ(x < 0) ܦܢܐ .001*x;
    ܐܢ(x > 1) ܦܢܐ .001*(x-1) + 1;
    ܦܢܐ x;
}
ساكن مضمن عائم lhtan_gradient(عائم x)
{
    ܐܢ(x > 0 && x < 1) ܦܢܐ 1;
    ܦܢܐ .001;
}

ساكن مضمن عائم hardtan_gradient(عائم x)
{
    ܐܢ (x > -1 && x < 1) ܦܢܐ 1;
    ܦܢܐ 0;
}
ساكن مضمن عائم linear_gradient(عائم x){ܦܢܐ 1;}
ساكن مضمن عائم logistic_gradient(عائم x){ܦܢܐ (1-x)*x;}
ساكن مضمن عائم loggy_gradient(عائم x)
{
    عائم y = (x+1.)/2.;
    ܦܢܐ 2*(1-y)*y;
}
ساكن مضمن عائم stair_gradient(عائم x)
{
    ܐܢ (floor(x) == x) ܦܢܐ 0;
    ܦܢܐ 1;
}
ساكن مضمن عائم relu_gradient(عائم x){ܦܢܐ (x>0);}
ساكن مضمن عائم elu_gradient(عائم x){ܦܢܐ (x >= 0) + (x < 0)*(x + 1);}
ساكن مضمن عائم selu_gradient(عائم x){ܦܢܐ (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
ساكن مضمن عائم relie_gradient(عائم x){ܦܢܐ (x>0) ? 1 : .01;}
ساكن مضمن عائم ramp_gradient(عائم x){ܦܢܐ (x>0)+.1;}
ساكن مضمن عائم leaky_gradient(عائم x){ܦܢܐ (x>0) ? 1 : .1;}
ساكن مضمن عائم tanh_gradient(عائم x){ܦܢܐ 1-x*x;}
ساكن مضمن عائم plse_gradient(عائم x){ܦܢܐ (x < 0 || x > 1) ? .01 : .125;}

#endif

