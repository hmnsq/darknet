#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ᨀᨘ(l.type == DROPOUT){
        ᨀᨘ(l.rand)           free(l.rand);
#ifdef GPU
        ᨀᨘ(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ᨀᨘᨒ;
    }
    ᨀᨘ(l.cweights)           free(l.cweights);
    ᨀᨘ(l.indexes)            free(l.indexes);
    ᨀᨘ(l.input_layers)       free(l.input_layers);
    ᨀᨘ(l.input_sizes)        free(l.input_sizes);
    ᨀᨘ(l.map)                free(l.map);
    ᨀᨘ(l.rand)               free(l.rand);
    ᨀᨘ(l.cost)               free(l.cost);
    ᨀᨘ(l.state)              free(l.state);
    ᨀᨘ(l.prev_state)         free(l.prev_state);
    ᨀᨘ(l.forgot_state)       free(l.forgot_state);
    ᨀᨘ(l.forgot_delta)       free(l.forgot_delta);
    ᨀᨘ(l.state_delta)        free(l.state_delta);
    ᨀᨘ(l.concat)             free(l.concat);
    ᨀᨘ(l.concat_delta)       free(l.concat_delta);
    ᨀᨘ(l.binary_weights)     free(l.binary_weights);
    ᨀᨘ(l.biases)             free(l.biases);
    ᨀᨘ(l.bias_updates)       free(l.bias_updates);
    ᨀᨘ(l.scales)             free(l.scales);
    ᨀᨘ(l.scale_updates)      free(l.scale_updates);
    ᨀᨘ(l.weights)            free(l.weights);
    ᨀᨘ(l.weight_updates)     free(l.weight_updates);
    ᨀᨘ(l.delta)              free(l.delta);
    ᨀᨘ(l.output)             free(l.output);
    ᨀᨘ(l.squared)            free(l.squared);
    ᨀᨘ(l.norms)              free(l.norms);
    ᨀᨘ(l.spatial_mean)       free(l.spatial_mean);
    ᨀᨘ(l.mean)               free(l.mean);
    ᨀᨘ(l.variance)           free(l.variance);
    ᨀᨘ(l.mean_delta)         free(l.mean_delta);
    ᨀᨘ(l.variance_delta)     free(l.variance_delta);
    ᨀᨘ(l.rolling_mean)       free(l.rolling_mean);
    ᨀᨘ(l.rolling_variance)   free(l.rolling_variance);
    ᨀᨘ(l.x)                  free(l.x);
    ᨀᨘ(l.x_norm)             free(l.x_norm);
    ᨀᨘ(l.m)                  free(l.m);
    ᨀᨘ(l.v)                  free(l.v);
    ᨀᨘ(l.z_cpu)              free(l.z_cpu);
    ᨀᨘ(l.r_cpu)              free(l.r_cpu);
    ᨀᨘ(l.h_cpu)              free(l.h_cpu);
    ᨀᨘ(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ᨀᨘ(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ᨀᨘ(l.z_gpu)                   cuda_free(l.z_gpu);
    ᨀᨘ(l.r_gpu)                   cuda_free(l.r_gpu);
    ᨀᨘ(l.h_gpu)                   cuda_free(l.h_gpu);
    ᨀᨘ(l.m_gpu)                   cuda_free(l.m_gpu);
    ᨀᨘ(l.v_gpu)                   cuda_free(l.v_gpu);
    ᨀᨘ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ᨀᨘ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ᨀᨘ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ᨀᨘ(l.state_gpu)               cuda_free(l.state_gpu);
    ᨀᨘ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ᨀᨘ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ᨀᨘ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ᨀᨘ(l.save_gpu)                cuda_free(l.save_gpu);
    ᨀᨘ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ᨀᨘ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ᨀᨘ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ᨀᨘ(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ᨀᨘ(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ᨀᨘ(l.mean_gpu)                cuda_free(l.mean_gpu);
    ᨀᨘ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ᨀᨘ(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ᨀᨘ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ᨀᨘ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ᨀᨘ(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ᨀᨘ(l.x_gpu)                   cuda_free(l.x_gpu);
    ᨀᨘ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ᨀᨘ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ᨀᨘ(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ᨀᨘ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ᨀᨘ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ᨀᨘ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ᨀᨘ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ᨀᨘ(l.output_gpu)              cuda_free(l.output_gpu);
    ᨀᨘ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ᨀᨘ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ᨀᨘ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ᨀᨘ(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
