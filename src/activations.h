#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#include "darknet.h"
#include "cuda.h"
#include "math.h"

ACTIVATION get_activation(var_na *s);

var_na *get_activation_string(ACTIVATION a);
float activate(float x, ACTIVATION a);
float gradient(float x, ACTIVATION a);
void gradient_array(const float *x, const ကိန်းပြည့် n, const ACTIVATION a, float *delta);
void activate_array(float *x, const ကိန်းပြည့် n, const ACTIVATION a);
#ifdef GPU
void activate_array_gpu(float *x, ကိန်းပြည့် n, ACTIVATION a);
void gradient_array_gpu(float *x, ကိန်းပြည့် n, ACTIVATION a, float *delta);
#endif

static anta_hstha float stair_activate(float x)
{
    ကိန်းပြည့် n = floor(x);
    အကယ်၍ (n%2 == 0) ပြန်ပေး floor(x/2.);
    မဟုတ်ပါက ပြန်ပေး (x - n) + floor(x/2.);
}
static anta_hstha float hardtan_activate(float x)
{
    အကယ်၍ (x < -1) ပြန်ပေး -1;
    အကယ်၍ (x > 1) ပြန်ပေး 1;
    ပြန်ပေး x;
}
static anta_hstha float linear_activate(float x){ပြန်ပေး x;}
static anta_hstha float logistic_activate(float x){ပြန်ပေး 1./(1. + exp(-x));}
static anta_hstha float loggy_activate(float x){ပြန်ပေး 2./(1. + exp(-x)) - 1;}
static anta_hstha float relu_activate(float x){ပြန်ပေး x*(x>0);}
static anta_hstha float elu_activate(float x){ပြန်ပေး (x >= 0)*x + (x < 0)*(exp(x)-1);}
static anta_hstha float selu_activate(float x){ပြန်ပေး (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
static anta_hstha float relie_activate(float x){ပြန်ပေး (x>0) ? x : .01*x;}
static anta_hstha float ramp_activate(float x){ပြန်ပေး x*(x>0)+.1*x;}
static anta_hstha float leaky_activate(float x){ပြန်ပေး (x>0) ? x : .1*x;}
static anta_hstha float tanh_activate(float x){ပြန်ပေး (exp(2*x)-1)/(exp(2*x)+1);}
static anta_hstha float plse_activate(float x)
{
    အကယ်၍(x < -4) ပြန်ပေး .01 * (x + 4);
    အကယ်၍(x > 4)  ပြန်ပေး .01 * (x - 4) + 1;
    ပြန်ပေး .125*x + .5;
}

static anta_hstha float lhtan_activate(float x)
{
    အကယ်၍(x < 0) ပြန်ပေး .001*x;
    အကယ်၍(x > 1) ပြန်ပေး .001*(x-1) + 1;
    ပြန်ပေး x;
}
static anta_hstha float lhtan_gradient(float x)
{
    အကယ်၍(x > 0 && x < 1) ပြန်ပေး 1;
    ပြန်ပေး .001;
}

static anta_hstha float hardtan_gradient(float x)
{
    အကယ်၍ (x > -1 && x < 1) ပြန်ပေး 1;
    ပြန်ပေး 0;
}
static anta_hstha float linear_gradient(float x){ပြန်ပေး 1;}
static anta_hstha float logistic_gradient(float x){ပြန်ပေး (1-x)*x;}
static anta_hstha float loggy_gradient(float x)
{
    float y = (x+1.)/2.;
    ပြန်ပေး 2*(1-y)*y;
}
static anta_hstha float stair_gradient(float x)
{
    အကယ်၍ (floor(x) == x) ပြန်ပေး 0;
    ပြန်ပေး 1;
}
static anta_hstha float relu_gradient(float x){ပြန်ပေး (x>0);}
static anta_hstha float elu_gradient(float x){ပြန်ပေး (x >= 0) + (x < 0)*(x + 1);}
static anta_hstha float selu_gradient(float x){ပြန်ပေး (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
static anta_hstha float relie_gradient(float x){ပြန်ပေး (x>0) ? 1 : .01;}
static anta_hstha float ramp_gradient(float x){ပြန်ပေး (x>0)+.1;}
static anta_hstha float leaky_gradient(float x){ပြန်ပေး (x>0) ? 1 : .1;}
static anta_hstha float tanh_gradient(float x){ပြန်ပေး 1-x*x;}
static anta_hstha float plse_gradient(float x){ပြန်ပေး (x < 0 || x > 1) ? .01 : .125;}

#endif

