#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "math.h"

ACTIVATION get_activation(অক্ষর *s);

অক্ষর *get_activation_string(ACTIVATION a);
দশমিক activate(দশমিক x, ACTIVATION a);
দশমিক gradient(দশমিক x, ACTIVATION a);
শূন্য gradient_array(ধ্রুবক দশমিক *x, ধ্রুবক পূর্ণসংখ্যা n, ধ্রুবক ACTIVATION a, দশমিক *delta);
শূন্য activate_array(দশমিক *x, ধ্রুবক পূর্ণসংখ্যা n, ধ্রুবক ACTIVATION a);
#ifdef GPU
শূন্য activate_array_gpu(দশমিক *x, পূর্ণসংখ্যা n, ACTIVATION a);
শূন্য gradient_array_gpu(দশমিক *x, পূর্ণসংখ্যা n, ACTIVATION a, দশমিক *delta);
#endif

স্থির অন্তঃরেখ দশমিক stair_activate(দশমিক x)
{
    পূর্ণসংখ্যা n = floor(x);
    যদি (n%2 == 0) ফেরত floor(x/2.);
    অন্যথা ফেরত (x - n) + floor(x/2.);
}
স্থির অন্তঃরেখ দশমিক hardtan_activate(দশমিক x)
{
    যদি (x < -1) ফেরত -1;
    যদি (x > 1) ফেরত 1;
    ফেরত x;
}
স্থির অন্তঃরেখ দশমিক linear_activate(দশমিক x){ফেরত x;}
স্থির অন্তঃরেখ দশমিক logistic_activate(দশমিক x){ফেরত 1./(1. + exp(-x));}
স্থির অন্তঃরেখ দশমিক loggy_activate(দশমিক x){ফেরত 2./(1. + exp(-x)) - 1;}
স্থির অন্তঃরেখ দশমিক relu_activate(দশমিক x){ফেরত x*(x>0);}
স্থির অন্তঃরেখ দশমিক elu_activate(দশমিক x){ফেরত (x >= 0)*x + (x < 0)*(exp(x)-1);}
স্থির অন্তঃরেখ দশমিক selu_activate(দশমিক x){ফেরত (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
স্থির অন্তঃরেখ দশমিক relie_activate(দশমিক x){ফেরত (x>0) ? x : .01*x;}
স্থির অন্তঃরেখ দশমিক ramp_activate(দশমিক x){ফেরত x*(x>0)+.1*x;}
স্থির অন্তঃরেখ দশমিক leaky_activate(দশমিক x){ফেরত (x>0) ? x : .1*x;}
স্থির অন্তঃরেখ দশমিক tanh_activate(দশমিক x){ফেরত (exp(2*x)-1)/(exp(2*x)+1);}
স্থির অন্তঃরেখ দশমিক plse_activate(দশমিক x)
{
    যদি(x < -4) ফেরত .01 * (x + 4);
    যদি(x > 4)  ফেরত .01 * (x - 4) + 1;
    ফেরত .125*x + .5;
}

স্থির অন্তঃরেখ দশমিক lhtan_activate(দশমিক x)
{
    যদি(x < 0) ফেরত .001*x;
    যদি(x > 1) ফেরত .001*(x-1) + 1;
    ফেরত x;
}
স্থির অন্তঃরেখ দশমিক lhtan_gradient(দশমিক x)
{
    যদি(x > 0 && x < 1) ফেরত 1;
    ফেরত .001;
}

স্থির অন্তঃরেখ দশমিক hardtan_gradient(দশমিক x)
{
    যদি (x > -1 && x < 1) ফেরত 1;
    ফেরত 0;
}
স্থির অন্তঃরেখ দশমিক linear_gradient(দশমিক x){ফেরত 1;}
স্থির অন্তঃরেখ দশমিক logistic_gradient(দশমিক x){ফেরত (1-x)*x;}
স্থির অন্তঃরেখ দশমিক loggy_gradient(দশমিক x)
{
    দশমিক y = (x+1.)/2.;
    ফেরত 2*(1-y)*y;
}
স্থির অন্তঃরেখ দশমিক stair_gradient(দশমিক x)
{
    যদি (floor(x) == x) ফেরত 0;
    ফেরত 1;
}
স্থির অন্তঃরেখ দশমিক relu_gradient(দশমিক x){ফেরত (x>0);}
স্থির অন্তঃরেখ দশমিক elu_gradient(দশমিক x){ফেরত (x >= 0) + (x < 0)*(x + 1);}
স্থির অন্তঃরেখ দশমিক selu_gradient(দশমিক x){ফেরত (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
স্থির অন্তঃরেখ দশমিক relie_gradient(দশমিক x){ফেরত (x>0) ? 1 : .01;}
স্থির অন্তঃরেখ দশমিক ramp_gradient(দশমিক x){ফেরত (x>0)+.1;}
স্থির অন্তঃরেখ দশমিক leaky_gradient(দশমিক x){ফেরত (x>0) ? 1 : .1;}
স্থির অন্তঃরেখ দশমিক tanh_gradient(দশমিক x){ফেরত 1-x*x;}
স্থির অন্তঃরেখ দশমিক plse_gradient(দশমিক x){ফেরত (x < 0 || x > 1) ? .01 : .125;}

#endif

