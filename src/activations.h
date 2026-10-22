#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#include "darknet.h"
#include "cuda.h"
#include "math.h"

ACTIVATION get_activation(var_na *s);

var_na *get_activation_string(ACTIVATION a);
float activate(float x, ACTIVATION a);
float gradient(float x, ACTIVATION a);
void gradient_array(const float *x, const puur_naa_nka n, const ACTIVATION a, float *delta);
void activate_array(float *x, const puur_naa_nka n, const ACTIVATION a);
#ifdef GPU
void activate_array_gpu(float *x, puur_naa_nka n, ACTIVATION a);
void gradient_array_gpu(float *x, puur_naa_nka n, ACTIVATION a, float *delta);
#endif

static anta_hstha float stair_activate(float x)
{
    puur_naa_nka n = floor(x);
    ᮚᮦᮔ᮪ (n%2 == 0) ᮝᮜᮤ floor(x/2.);
    ᮜᮤᮚ ᮝᮜᮤ (x - n) + floor(x/2.);
}
static anta_hstha float hardtan_activate(float x)
{
    ᮚᮦᮔ᮪ (x < -1) ᮝᮜᮤ -1;
    ᮚᮦᮔ᮪ (x > 1) ᮝᮜᮤ 1;
    ᮝᮜᮤ x;
}
static anta_hstha float linear_activate(float x){ᮝᮜᮤ x;}
static anta_hstha float logistic_activate(float x){ᮝᮜᮤ 1./(1. + exp(-x));}
static anta_hstha float loggy_activate(float x){ᮝᮜᮤ 2./(1. + exp(-x)) - 1;}
static anta_hstha float relu_activate(float x){ᮝᮜᮤ x*(x>0);}
static anta_hstha float elu_activate(float x){ᮝᮜᮤ (x >= 0)*x + (x < 0)*(exp(x)-1);}
static anta_hstha float selu_activate(float x){ᮝᮜᮤ (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
static anta_hstha float relie_activate(float x){ᮝᮜᮤ (x>0) ? x : .01*x;}
static anta_hstha float ramp_activate(float x){ᮝᮜᮤ x*(x>0)+.1*x;}
static anta_hstha float leaky_activate(float x){ᮝᮜᮤ (x>0) ? x : .1*x;}
static anta_hstha float tanh_activate(float x){ᮝᮜᮤ (exp(2*x)-1)/(exp(2*x)+1);}
static anta_hstha float plse_activate(float x)
{
    ᮚᮦᮔ᮪(x < -4) ᮝᮜᮤ .01 * (x + 4);
    ᮚᮦᮔ᮪(x > 4)  ᮝᮜᮤ .01 * (x - 4) + 1;
    ᮝᮜᮤ .125*x + .5;
}

static anta_hstha float lhtan_activate(float x)
{
    ᮚᮦᮔ᮪(x < 0) ᮝᮜᮤ .001*x;
    ᮚᮦᮔ᮪(x > 1) ᮝᮜᮤ .001*(x-1) + 1;
    ᮝᮜᮤ x;
}
static anta_hstha float lhtan_gradient(float x)
{
    ᮚᮦᮔ᮪(x > 0 && x < 1) ᮝᮜᮤ 1;
    ᮝᮜᮤ .001;
}

static anta_hstha float hardtan_gradient(float x)
{
    ᮚᮦᮔ᮪ (x > -1 && x < 1) ᮝᮜᮤ 1;
    ᮝᮜᮤ 0;
}
static anta_hstha float linear_gradient(float x){ᮝᮜᮤ 1;}
static anta_hstha float logistic_gradient(float x){ᮝᮜᮤ (1-x)*x;}
static anta_hstha float loggy_gradient(float x)
{
    float y = (x+1.)/2.;
    ᮝᮜᮤ 2*(1-y)*y;
}
static anta_hstha float stair_gradient(float x)
{
    ᮚᮦᮔ᮪ (floor(x) == x) ᮝᮜᮤ 0;
    ᮝᮜᮤ 1;
}
static anta_hstha float relu_gradient(float x){ᮝᮜᮤ (x>0);}
static anta_hstha float elu_gradient(float x){ᮝᮜᮤ (x >= 0) + (x < 0)*(x + 1);}
static anta_hstha float selu_gradient(float x){ᮝᮜᮤ (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
static anta_hstha float relie_gradient(float x){ᮝᮜᮤ (x>0) ? 1 : .01;}
static anta_hstha float ramp_gradient(float x){ᮝᮜᮤ (x>0)+.1;}
static anta_hstha float leaky_gradient(float x){ᮝᮜᮤ (x>0) ? 1 : .1;}
static anta_hstha float tanh_gradient(float x){ᮝᮜᮤ 1-x*x;}
static anta_hstha float plse_gradient(float x){ᮝᮜᮤ (x < 0 || x > 1) ? .01 : .125;}

#endif

