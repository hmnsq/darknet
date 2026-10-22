#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#include "darknet.h"
#include "cuda.h"
#include "math.h"

ACTIVATION get_activation(char *s);

char *get_activation_string(ACTIVATION a);
float activate(float x, ACTIVATION a);
float gradient(float x, ACTIVATION a);
void gradient_array(const float *x, const 整数 n, const ACTIVATION a, float *delta);
void activate_array(float *x, const 整数 n, const ACTIVATION a);
#ifdef GPU
void activate_array_gpu(float *x, 整数 n, ACTIVATION a);
void gradient_array_gpu(float *x, 整数 n, ACTIVATION a, float *delta);
#endif

static inline float stair_activate(float x)
{
    整数 n = floor(x);
    如果 (n%2 == 0) 返回 floor(x/2.);
    否则 返回 (x - n) + floor(x/2.);
}
static inline float hardtan_activate(float x)
{
    如果 (x < -1) 返回 -1;
    如果 (x > 1) 返回 1;
    返回 x;
}
static inline float linear_activate(float x){返回 x;}
static inline float logistic_activate(float x){返回 1./(1. + exp(-x));}
static inline float loggy_activate(float x){返回 2./(1. + exp(-x)) - 1;}
static inline float relu_activate(float x){返回 x*(x>0);}
static inline float elu_activate(float x){返回 (x >= 0)*x + (x < 0)*(exp(x)-1);}
static inline float selu_activate(float x){返回 (x >= 0)*1.0507*x + (x < 0)*1.0507*1.6732*(exp(x)-1);}
static inline float relie_activate(float x){返回 (x>0) ? x : .01*x;}
static inline float ramp_activate(float x){返回 x*(x>0)+.1*x;}
static inline float leaky_activate(float x){返回 (x>0) ? x : .1*x;}
static inline float tanh_activate(float x){返回 (exp(2*x)-1)/(exp(2*x)+1);}
static inline float plse_activate(float x)
{
    如果(x < -4) 返回 .01 * (x + 4);
    如果(x > 4)  返回 .01 * (x - 4) + 1;
    返回 .125*x + .5;
}

static inline float lhtan_activate(float x)
{
    如果(x < 0) 返回 .001*x;
    如果(x > 1) 返回 .001*(x-1) + 1;
    返回 x;
}
static inline float lhtan_gradient(float x)
{
    如果(x > 0 && x < 1) 返回 1;
    返回 .001;
}

static inline float hardtan_gradient(float x)
{
    如果 (x > -1 && x < 1) 返回 1;
    返回 0;
}
static inline float linear_gradient(float x){返回 1;}
static inline float logistic_gradient(float x){返回 (1-x)*x;}
static inline float loggy_gradient(float x)
{
    float y = (x+1.)/2.;
    返回 2*(1-y)*y;
}
static inline float stair_gradient(float x)
{
    如果 (floor(x) == x) 返回 0;
    返回 1;
}
static inline float relu_gradient(float x){返回 (x>0);}
static inline float elu_gradient(float x){返回 (x >= 0) + (x < 0)*(x + 1);}
static inline float selu_gradient(float x){返回 (x >= 0)*1.0507 + (x < 0)*(x + 1.0507*1.6732);}
static inline float relie_gradient(float x){返回 (x>0) ? 1 : .01;}
static inline float ramp_gradient(float x){返回 (x>0)+.1;}
static inline float leaky_gradient(float x){返回 (x>0) ? 1 : .1;}
static inline float tanh_gradient(float x){返回 1-x*x;}
static inline float plse_gradient(float x){返回 (x < 0 || x > 1) ? .01 : .125;}

#endif

