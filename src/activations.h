#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#অন্তর্ভাৱ "darknet.h"
#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ "math.h"

ACTIVATION get_activation(ৱর্ণ *s);

ৱর্ণ *get_activation_string(ACTIVATION a);
ভিন্ন activate(ভিন্ন x, ACTIVATION a);
ভিন্ন gradient(ভিন্ন x, ACTIVATION a);
শূন্য gradient_array(ধ্রুৱ ভিন্ন *x, ধ্রুৱ পূর্ণাঙ্ক n, ধ্রুৱ ACTIVATION a, ভিন্ন *delta);
শূন্য activate_array(ভিন্ন *x, ধ্রুৱ পূর্ণাঙ্ক n, ধ্রুৱ ACTIVATION a);
#ifdef GPU
শূন্য activate_array_gpu(ভিন্ন *x, পূর্ণাঙ্ক n, ACTIVATION a);
শূন্য gradient_array_gpu(ভিন্ন *x, পূর্ণাঙ্ক n, ACTIVATION a, ভিন্ন *delta);
#endif

স্থির অন্তঃস্থ ভিন্ন stair_activate(ভিন্ন x)
{
    পূর্ণাঙ্ক n = floor(x);
    যদি (n%2 == 0) ঘূৰাই floor(x/2.);
    অন্যথা ঘূৰাই (x - n) + floor(x/2.);
}
স্থির অন্তঃস্থ ভিন্ন hardtan_activate(ভিন্ন x)
{
    যদি (x < -1) ঘূৰাই -1;
    যদি (x > 1) ঘূৰাই 1;
    ঘূৰাই x;
}
স্থির অন্তঃস্থ ভিন্ন linear_activate(ভিন্ন x){ঘূৰাই x;}
স্থির অন্তঃস্থ ভিন্ন logistic_activate(ভিন্ন x){ঘূৰাই 1./(1. + exp(-x));}
স্থির অন্তঃস্থ ভিন্ন loggy_activate(ভিন্ন x){ঘূৰাই 2./(1. + exp(-x)) - 1;}
স্থির অন্তঃস্থ ভিন্ন relu_activate(ভিন্ন x){ঘূৰাই x*(x>0);}
স্থির অন্তঃস্থ ভিন্ন elu_activate(ভিন্ন x){ঘূৰাই (x >= 0)*x + (x < 0)*(exp(x)-1);}
স্থির অন্তঃস্থ ভিন্ন selu_activate(ভিন্ন x){ঘূৰাই (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
স্থির অন্তঃস্থ ভিন্ন relie_activate(ভিন্ন x){ঘূৰাই (x>0) ? x : .01*x;}
স্থির অন্তঃস্থ ভিন্ন ramp_activate(ভিন্ন x){ঘূৰাই x*(x>0)+.1*x;}
স্থির অন্তঃস্থ ভিন্ন leaky_activate(ভিন্ন x){ঘূৰাই (x>0) ? x : .1*x;}
স্থির অন্তঃস্থ ভিন্ন tanh_activate(ভিন্ন x){ঘূৰাই (exp(2*x)-1)/(exp(2*x)+1);}
স্থির অন্তঃস্থ ভিন্ন plse_activate(ভিন্ন x)
{
    যদি(x < -4) ঘূৰাই .01 * (x + 4);
    যদি(x > 4)  ঘূৰাই .01 * (x - 4) + 1;
    ঘূৰাই .125*x + .5;
}

স্থির অন্তঃস্থ ভিন্ন lhtan_activate(ভিন্ন x)
{
    যদি(x < 0) ঘূৰাই .001*x;
    যদি(x > 1) ঘূৰাই .001*(x-1) + 1;
    ঘূৰাই x;
}
স্থির অন্তঃস্থ ভিন্ন lhtan_gradient(ভিন্ন x)
{
    যদি(x > 0 && x < 1) ঘূৰাই 1;
    ঘূৰাই .001;
}

স্থির অন্তঃস্থ ভিন্ন hardtan_gradient(ভিন্ন x)
{
    যদি (x > -1 && x < 1) ঘূৰাই 1;
    ঘূৰাই 0;
}
স্থির অন্তঃস্থ ভিন্ন linear_gradient(ভিন্ন x){ঘূৰাই 1;}
স্থির অন্তঃস্থ ভিন্ন logistic_gradient(ভিন্ন x){ঘূৰাই (1-x)*x;}
স্থির অন্তঃস্থ ভিন্ন loggy_gradient(ভিন্ন x)
{
    ভিন্ন y = (x+1.)/2.;
    ঘূৰাই 2*(1-y)*y;
}
স্থির অন্তঃস্থ ভিন্ন stair_gradient(ভিন্ন x)
{
    যদি (floor(x) == x) ঘূৰাই 0;
    ঘূৰাই 1;
}
স্থির অন্তঃস্থ ভিন্ন relu_gradient(ভিন্ন x){ঘূৰাই (x>0);}
স্থির অন্তঃস্থ ভিন্ন elu_gradient(ভিন্ন x){ঘূৰাই (x >= 0) + (x < 0)*(x + 1);}
স্থির অন্তঃস্থ ভিন্ন selu_gradient(ভিন্ন x){ঘূৰাই (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
স্থির অন্তঃস্থ ভিন্ন relie_gradient(ভিন্ন x){ঘূৰাই (x>0) ? 1 : .01;}
স্থির অন্তঃস্থ ভিন্ন ramp_gradient(ভিন্ন x){ঘূৰাই (x>0)+.1;}
স্থির অন্তঃস্থ ভিন্ন leaky_gradient(ভিন্ন x){ঘূৰাই (x>0) ? 1 : .1;}
স্থির অন্তঃস্থ ভিন্ন tanh_gradient(ভিন্ন x){ঘূৰাই 1-x*x;}
স্থির অন্তঃস্থ ভিন্ন plse_gradient(ভিন্ন x){ঘূৰাই (x < 0 || x > 1) ? .01 : .125;}

#endif

