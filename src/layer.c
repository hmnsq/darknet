#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ᯀᯮᯝ(l.type == DROPOUT){
        ᯀᯮᯝ(l.rand)           free(l.rand);
#ifdef GPU
        ᯀᯮᯝ(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ᯀᯮᯞ;
    }
    ᯀᯮᯝ(l.cweights)           free(l.cweights);
    ᯀᯮᯝ(l.indexes)            free(l.indexes);
    ᯀᯮᯝ(l.input_layers)       free(l.input_layers);
    ᯀᯮᯝ(l.input_sizes)        free(l.input_sizes);
    ᯀᯮᯝ(l.map)                free(l.map);
    ᯀᯮᯝ(l.rand)               free(l.rand);
    ᯀᯮᯝ(l.cost)               free(l.cost);
    ᯀᯮᯝ(l.state)              free(l.state);
    ᯀᯮᯝ(l.prev_state)         free(l.prev_state);
    ᯀᯮᯝ(l.forgot_state)       free(l.forgot_state);
    ᯀᯮᯝ(l.forgot_delta)       free(l.forgot_delta);
    ᯀᯮᯝ(l.state_delta)        free(l.state_delta);
    ᯀᯮᯝ(l.concat)             free(l.concat);
    ᯀᯮᯝ(l.concat_delta)       free(l.concat_delta);
    ᯀᯮᯝ(l.binary_weights)     free(l.binary_weights);
    ᯀᯮᯝ(l.biases)             free(l.biases);
    ᯀᯮᯝ(l.bias_updates)       free(l.bias_updates);
    ᯀᯮᯝ(l.scales)             free(l.scales);
    ᯀᯮᯝ(l.scale_updates)      free(l.scale_updates);
    ᯀᯮᯝ(l.weights)            free(l.weights);
    ᯀᯮᯝ(l.weight_updates)     free(l.weight_updates);
    ᯀᯮᯝ(l.delta)              free(l.delta);
    ᯀᯮᯝ(l.output)             free(l.output);
    ᯀᯮᯝ(l.squared)            free(l.squared);
    ᯀᯮᯝ(l.norms)              free(l.norms);
    ᯀᯮᯝ(l.spatial_mean)       free(l.spatial_mean);
    ᯀᯮᯝ(l.mean)               free(l.mean);
    ᯀᯮᯝ(l.variance)           free(l.variance);
    ᯀᯮᯝ(l.mean_delta)         free(l.mean_delta);
    ᯀᯮᯝ(l.variance_delta)     free(l.variance_delta);
    ᯀᯮᯝ(l.rolling_mean)       free(l.rolling_mean);
    ᯀᯮᯝ(l.rolling_variance)   free(l.rolling_variance);
    ᯀᯮᯝ(l.x)                  free(l.x);
    ᯀᯮᯝ(l.x_norm)             free(l.x_norm);
    ᯀᯮᯝ(l.m)                  free(l.m);
    ᯀᯮᯝ(l.v)                  free(l.v);
    ᯀᯮᯝ(l.z_cpu)              free(l.z_cpu);
    ᯀᯮᯝ(l.r_cpu)              free(l.r_cpu);
    ᯀᯮᯝ(l.h_cpu)              free(l.h_cpu);
    ᯀᯮᯝ(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ᯀᯮᯝ(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ᯀᯮᯝ(l.z_gpu)                   cuda_free(l.z_gpu);
    ᯀᯮᯝ(l.r_gpu)                   cuda_free(l.r_gpu);
    ᯀᯮᯝ(l.h_gpu)                   cuda_free(l.h_gpu);
    ᯀᯮᯝ(l.m_gpu)                   cuda_free(l.m_gpu);
    ᯀᯮᯝ(l.v_gpu)                   cuda_free(l.v_gpu);
    ᯀᯮᯝ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ᯀᯮᯝ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ᯀᯮᯝ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ᯀᯮᯝ(l.state_gpu)               cuda_free(l.state_gpu);
    ᯀᯮᯝ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ᯀᯮᯝ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ᯀᯮᯝ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ᯀᯮᯝ(l.save_gpu)                cuda_free(l.save_gpu);
    ᯀᯮᯝ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ᯀᯮᯝ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ᯀᯮᯝ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ᯀᯮᯝ(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ᯀᯮᯝ(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ᯀᯮᯝ(l.mean_gpu)                cuda_free(l.mean_gpu);
    ᯀᯮᯝ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ᯀᯮᯝ(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ᯀᯮᯝ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ᯀᯮᯝ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ᯀᯮᯝ(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ᯀᯮᯝ(l.x_gpu)                   cuda_free(l.x_gpu);
    ᯀᯮᯝ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ᯀᯮᯝ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ᯀᯮᯝ(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ᯀᯮᯝ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ᯀᯮᯝ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ᯀᯮᯝ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ᯀᯮᯝ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ᯀᯮᯝ(l.output_gpu)              cuda_free(l.output_gpu);
    ᯀᯮᯝ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ᯀᯮᯝ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ᯀᯮᯝ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ᯀᯮᯝ(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
