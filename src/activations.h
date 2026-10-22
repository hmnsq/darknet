#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#ଅନ୍ତର୍ଭାଵ "darknet.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "math.h"

ACTIVATION get_activation(ଅକ୍ଷର *s);

ଅକ୍ଷର *get_activation_string(ACTIVATION a);
ଦଶମିକ activate(ଦଶମିକ x, ACTIVATION a);
ଦଶମିକ gradient(ଦଶମିକ x, ACTIVATION a);
ଶୂନ୍ୟ gradient_array(ଅଚଳ ଦଶମିକ *x, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ ACTIVATION a, ଦଶମିକ *delta);
ଶୂନ୍ୟ activate_array(ଦଶମିକ *x, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ ACTIVATION a);
#ifdef GPU
ଶୂନ୍ୟ activate_array_gpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ACTIVATION a);
ଶୂନ୍ୟ gradient_array_gpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ACTIVATION a, ଦଶମିକ *delta);
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ stair_activate(ଦଶମିକ x)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = floor(x);
    ଯଦି (n%2 == 0) ଫେରନ୍ତୁ floor(x/2.);
    ଅନ୍ୟଥା ଫେରନ୍ତୁ (x - n) + floor(x/2.);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ hardtan_activate(ଦଶମିକ x)
{
    ଯଦି (x < -1) ଫେରନ୍ତୁ -1;
    ଯଦି (x > 1) ଫେରନ୍ତୁ 1;
    ଫେରନ୍ତୁ x;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ linear_activate(ଦଶମିକ x){ଫେରନ୍ତୁ x;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ logistic_activate(ଦଶମିକ x){ଫେରନ୍ତୁ 1./(1. + exp(-x));}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ loggy_activate(ଦଶମିକ x){ଫେରନ୍ତୁ 2./(1. + exp(-x)) - 1;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ relu_activate(ଦଶମିକ x){ଫେରନ୍ତୁ x*(x>0);}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ elu_activate(ଦଶମିକ x){ଫେରନ୍ତୁ (x >= 0)*x + (x < 0)*(exp(x)-1);}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ selu_activate(ଦଶମିକ x){ଫେରନ୍ତୁ (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ relie_activate(ଦଶମିକ x){ଫେରନ୍ତୁ (x>0) ? x : .01*x;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ ramp_activate(ଦଶମିକ x){ଫେରନ୍ତୁ x*(x>0)+.1*x;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ leaky_activate(ଦଶମିକ x){ଫେରନ୍ତୁ (x>0) ? x : .1*x;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ tanh_activate(ଦଶମିକ x){ଫେରନ୍ତୁ (exp(2*x)-1)/(exp(2*x)+1);}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ plse_activate(ଦଶମିକ x)
{
    ଯଦି(x < -4) ଫେରନ୍ତୁ .01 * (x + 4);
    ଯଦି(x > 4)  ଫେରନ୍ତୁ .01 * (x - 4) + 1;
    ଫେରନ୍ତୁ .125*x + .5;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ lhtan_activate(ଦଶମିକ x)
{
    ଯଦି(x < 0) ଫେରନ୍ତୁ .001*x;
    ଯଦି(x > 1) ଫେରନ୍ତୁ .001*(x-1) + 1;
    ଫେରନ୍ତୁ x;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ lhtan_gradient(ଦଶମିକ x)
{
    ଯଦି(x > 0 && x < 1) ଫେରନ୍ତୁ 1;
    ଫେରନ୍ତୁ .001;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ hardtan_gradient(ଦଶମିକ x)
{
    ଯଦି (x > -1 && x < 1) ଫେରନ୍ତୁ 1;
    ଫେରନ୍ତୁ 0;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ linear_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ 1;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ logistic_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (1-x)*x;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ loggy_gradient(ଦଶମିକ x)
{
    ଦଶମିକ y = (x+1.)/2.;
    ଫେରନ୍ତୁ 2*(1-y)*y;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ stair_gradient(ଦଶମିକ x)
{
    ଯଦି (floor(x) == x) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ 1;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ relu_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (x>0);}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ elu_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (x >= 0) + (x < 0)*(x + 1);}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ selu_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ relie_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (x>0) ? 1 : .01;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ ramp_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (x>0)+.1;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ leaky_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (x>0) ? 1 : .1;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ tanh_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ 1-x*x;}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ plse_gradient(ଦଶମିକ x){ଫେରନ୍ତୁ (x < 0 || x > 1) ? .01 : .125;}

#endif

