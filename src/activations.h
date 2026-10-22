#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#અન્તર્ભાવ "darknet.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "math.h"

ACTIVATION get_activation(અક્ષર *s);

અક્ષર *get_activation_string(ACTIVATION a);
દશાંશ activate(દશાંશ x, ACTIVATION a);
દશાંશ gradient(દશાંશ x, ACTIVATION a);
શૂન્ય gradient_array(અચલ દશાંશ *x, અચલ પૂર્ણાંક n, અચલ ACTIVATION a, દશાંશ *delta);
શૂન્ય activate_array(દશાંશ *x, અચલ પૂર્ણાંક n, અચલ ACTIVATION a);
#ifdef GPU
શૂન્ય activate_array_gpu(દશાંશ *x, પૂર્ણાંક n, ACTIVATION a);
શૂન્ય gradient_array_gpu(દશાંશ *x, પૂર્ણાંક n, ACTIVATION a, દશાંશ *delta);
#endif

સ્થિર અંતઃરેખ દશાંશ stair_activate(દશાંશ x)
{
    પૂર્ણાંક n = floor(x);
    જો (n%2 == 0) પાછા floor(x/2.);
    નહીંતર પાછા (x - n) + floor(x/2.);
}
સ્થિર અંતઃરેખ દશાંશ hardtan_activate(દશાંશ x)
{
    જો (x < -1) પાછા -1;
    જો (x > 1) પાછા 1;
    પાછા x;
}
સ્થિર અંતઃરેખ દશાંશ linear_activate(દશાંશ x){પાછા x;}
સ્થિર અંતઃરેખ દશાંશ logistic_activate(દશાંશ x){પાછા 1./(1. + exp(-x));}
સ્થિર અંતઃરેખ દશાંશ loggy_activate(દશાંશ x){પાછા 2./(1. + exp(-x)) - 1;}
સ્થિર અંતઃરેખ દશાંશ relu_activate(દશાંશ x){પાછા x*(x>0);}
સ્થિર અંતઃરેખ દશાંશ elu_activate(દશાંશ x){પાછા (x >= 0)*x + (x < 0)*(exp(x)-1);}
સ્થિર અંતઃરેખ દશાંશ selu_activate(દશાંશ x){પાછા (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
સ્થિર અંતઃરેખ દશાંશ relie_activate(દશાંશ x){પાછા (x>0) ? x : .01*x;}
સ્થિર અંતઃરેખ દશાંશ ramp_activate(દશાંશ x){પાછા x*(x>0)+.1*x;}
સ્થિર અંતઃરેખ દશાંશ leaky_activate(દશાંશ x){પાછા (x>0) ? x : .1*x;}
સ્થિર અંતઃરેખ દશાંશ tanh_activate(દશાંશ x){પાછા (exp(2*x)-1)/(exp(2*x)+1);}
સ્થિર અંતઃરેખ દશાંશ plse_activate(દશાંશ x)
{
    જો(x < -4) પાછા .01 * (x + 4);
    જો(x > 4)  પાછા .01 * (x - 4) + 1;
    પાછા .125*x + .5;
}

સ્થિર અંતઃરેખ દશાંશ lhtan_activate(દશાંશ x)
{
    જો(x < 0) પાછા .001*x;
    જો(x > 1) પાછા .001*(x-1) + 1;
    પાછા x;
}
સ્થિર અંતઃરેખ દશાંશ lhtan_gradient(દશાંશ x)
{
    જો(x > 0 && x < 1) પાછા 1;
    પાછા .001;
}

સ્થિર અંતઃરેખ દશાંશ hardtan_gradient(દશાંશ x)
{
    જો (x > -1 && x < 1) પાછા 1;
    પાછા 0;
}
સ્થિર અંતઃરેખ દશાંશ linear_gradient(દશાંશ x){પાછા 1;}
સ્થિર અંતઃરેખ દશાંશ logistic_gradient(દશાંશ x){પાછા (1-x)*x;}
સ્થિર અંતઃરેખ દશાંશ loggy_gradient(દશાંશ x)
{
    દશાંશ y = (x+1.)/2.;
    પાછા 2*(1-y)*y;
}
સ્થિર અંતઃરેખ દશાંશ stair_gradient(દશાંશ x)
{
    જો (floor(x) == x) પાછા 0;
    પાછા 1;
}
સ્થિર અંતઃરેખ દશાંશ relu_gradient(દશાંશ x){પાછા (x>0);}
સ્થિર અંતઃરેખ દશાંશ elu_gradient(દશાંશ x){પાછા (x >= 0) + (x < 0)*(x + 1);}
સ્થિર અંતઃરેખ દશાંશ selu_gradient(દશાંશ x){પાછા (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
સ્થિર અંતઃરેખ દશાંશ relie_gradient(દશાંશ x){પાછા (x>0) ? 1 : .01;}
સ્થિર અંતઃરેખ દશાંશ ramp_gradient(દશાંશ x){પાછા (x>0)+.1;}
સ્થિર અંતઃરેખ દશાંશ leaky_gradient(દશાંશ x){પાછા (x>0) ? 1 : .1;}
સ્થિર અંતઃરેખ દશાંશ tanh_gradient(દશાંશ x){પાછા 1-x*x;}
સ્થિર અંતઃરેખ દશાંશ plse_gradient(દશાંશ x){પાછા (x < 0 || x > 1) ? .01 : .125;}

#endif

