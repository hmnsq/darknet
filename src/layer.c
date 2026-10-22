#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ប្រសិនបើ(l.type == DROPOUT){
        ប្រសិនបើ(l.rand)           free(l.rand);
#ifdef GPU
        ប្រសិនបើ(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ត្រឡប់;
    }
    ប្រសិនបើ(l.cweights)           free(l.cweights);
    ប្រសិនបើ(l.indexes)            free(l.indexes);
    ប្រសិនបើ(l.input_layers)       free(l.input_layers);
    ប្រសិនបើ(l.input_sizes)        free(l.input_sizes);
    ប្រសិនបើ(l.map)                free(l.map);
    ប្រសិនបើ(l.rand)               free(l.rand);
    ប្រសិនបើ(l.cost)               free(l.cost);
    ប្រសិនបើ(l.state)              free(l.state);
    ប្រសិនបើ(l.prev_state)         free(l.prev_state);
    ប្រសិនបើ(l.forgot_state)       free(l.forgot_state);
    ប្រសិនបើ(l.forgot_delta)       free(l.forgot_delta);
    ប្រសិនបើ(l.state_delta)        free(l.state_delta);
    ប្រសិនបើ(l.concat)             free(l.concat);
    ប្រសិនបើ(l.concat_delta)       free(l.concat_delta);
    ប្រសិនបើ(l.binary_weights)     free(l.binary_weights);
    ប្រសិនបើ(l.biases)             free(l.biases);
    ប្រសិនបើ(l.bias_updates)       free(l.bias_updates);
    ប្រសិនបើ(l.scales)             free(l.scales);
    ប្រសិនបើ(l.scale_updates)      free(l.scale_updates);
    ប្រសិនបើ(l.weights)            free(l.weights);
    ប្រសិនបើ(l.weight_updates)     free(l.weight_updates);
    ប្រសិនបើ(l.delta)              free(l.delta);
    ប្រសិនបើ(l.output)             free(l.output);
    ប្រសិនបើ(l.squared)            free(l.squared);
    ប្រសិនបើ(l.norms)              free(l.norms);
    ប្រសិនបើ(l.spatial_mean)       free(l.spatial_mean);
    ប្រសិនបើ(l.mean)               free(l.mean);
    ប្រសិនបើ(l.variance)           free(l.variance);
    ប្រសិនបើ(l.mean_delta)         free(l.mean_delta);
    ប្រសិនបើ(l.variance_delta)     free(l.variance_delta);
    ប្រសិនបើ(l.rolling_mean)       free(l.rolling_mean);
    ប្រសិនបើ(l.rolling_variance)   free(l.rolling_variance);
    ប្រសិនបើ(l.x)                  free(l.x);
    ប្រសិនបើ(l.x_norm)             free(l.x_norm);
    ប្រសិនបើ(l.m)                  free(l.m);
    ប្រសិនបើ(l.v)                  free(l.v);
    ប្រសិនបើ(l.z_cpu)              free(l.z_cpu);
    ប្រសិនបើ(l.r_cpu)              free(l.r_cpu);
    ប្រសិនបើ(l.h_cpu)              free(l.h_cpu);
    ប្រសិនបើ(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ប្រសិនបើ(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ប្រសិនបើ(l.z_gpu)                   cuda_free(l.z_gpu);
    ប្រសិនបើ(l.r_gpu)                   cuda_free(l.r_gpu);
    ប្រសិនបើ(l.h_gpu)                   cuda_free(l.h_gpu);
    ប្រសិនបើ(l.m_gpu)                   cuda_free(l.m_gpu);
    ប្រសិនបើ(l.v_gpu)                   cuda_free(l.v_gpu);
    ប្រសិនបើ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ប្រសិនបើ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ប្រសិនបើ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ប្រសិនបើ(l.state_gpu)               cuda_free(l.state_gpu);
    ប្រសិនបើ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ប្រសិនបើ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ប្រសិនបើ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ប្រសិនបើ(l.save_gpu)                cuda_free(l.save_gpu);
    ប្រសិនបើ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ប្រសិនបើ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ប្រសិនបើ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ប្រសិនបើ(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ប្រសិនបើ(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ប្រសិនបើ(l.mean_gpu)                cuda_free(l.mean_gpu);
    ប្រសិនបើ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ប្រសិនបើ(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ប្រសិនបើ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ប្រសិនបើ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ប្រសិនបើ(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ប្រសិនបើ(l.x_gpu)                   cuda_free(l.x_gpu);
    ប្រសិនបើ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ប្រសិនបើ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ប្រសិនបើ(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ប្រសិនបើ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ប្រសិនបើ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ប្រសិនបើ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ប្រសិនបើ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ប្រសិនបើ(l.output_gpu)              cuda_free(l.output_gpu);
    ប្រសិនបើ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ប្រសិនបើ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ប្រសិនបើ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ប្រសិនបើ(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
