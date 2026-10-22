#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    если(l.type == DROPOUT){
        если(l.rand)           free(l.rand);
#ifdef GPU
        если(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        вернуть;
    }
    если(l.cweights)           free(l.cweights);
    если(l.indexes)            free(l.indexes);
    если(l.input_layers)       free(l.input_layers);
    если(l.input_sizes)        free(l.input_sizes);
    если(l.map)                free(l.map);
    если(l.rand)               free(l.rand);
    если(l.cost)               free(l.cost);
    если(l.state)              free(l.state);
    если(l.prev_state)         free(l.prev_state);
    если(l.forgot_state)       free(l.forgot_state);
    если(l.forgot_delta)       free(l.forgot_delta);
    если(l.state_delta)        free(l.state_delta);
    если(l.concat)             free(l.concat);
    если(l.concat_delta)       free(l.concat_delta);
    если(l.binary_weights)     free(l.binary_weights);
    если(l.biases)             free(l.biases);
    если(l.bias_updates)       free(l.bias_updates);
    если(l.scales)             free(l.scales);
    если(l.scale_updates)      free(l.scale_updates);
    если(l.weights)            free(l.weights);
    если(l.weight_updates)     free(l.weight_updates);
    если(l.delta)              free(l.delta);
    если(l.output)             free(l.output);
    если(l.squared)            free(l.squared);
    если(l.norms)              free(l.norms);
    если(l.spatial_mean)       free(l.spatial_mean);
    если(l.mean)               free(l.mean);
    если(l.variance)           free(l.variance);
    если(l.mean_delta)         free(l.mean_delta);
    если(l.variance_delta)     free(l.variance_delta);
    если(l.rolling_mean)       free(l.rolling_mean);
    если(l.rolling_variance)   free(l.rolling_variance);
    если(l.x)                  free(l.x);
    если(l.x_norm)             free(l.x_norm);
    если(l.m)                  free(l.m);
    если(l.v)                  free(l.v);
    если(l.z_cpu)              free(l.z_cpu);
    если(l.r_cpu)              free(l.r_cpu);
    если(l.h_cpu)              free(l.h_cpu);
    если(l.binary_input)       free(l.binary_input);

#ifdef GPU
    если(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    если(l.z_gpu)                   cuda_free(l.z_gpu);
    если(l.r_gpu)                   cuda_free(l.r_gpu);
    если(l.h_gpu)                   cuda_free(l.h_gpu);
    если(l.m_gpu)                   cuda_free(l.m_gpu);
    если(l.v_gpu)                   cuda_free(l.v_gpu);
    если(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    если(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    если(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    если(l.state_gpu)               cuda_free(l.state_gpu);
    если(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    если(l.gate_gpu)                cuda_free(l.gate_gpu);
    если(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    если(l.save_gpu)                cuda_free(l.save_gpu);
    если(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    если(l.concat_gpu)              cuda_free(l.concat_gpu);
    если(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    если(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    если(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    если(l.mean_gpu)                cuda_free(l.mean_gpu);
    если(l.variance_gpu)            cuda_free(l.variance_gpu);
    если(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    если(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    если(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    если(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    если(l.x_gpu)                   cuda_free(l.x_gpu);
    если(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    если(l.weights_gpu)             cuda_free(l.weights_gpu);
    если(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    если(l.biases_gpu)              cuda_free(l.biases_gpu);
    если(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    если(l.scales_gpu)              cuda_free(l.scales_gpu);
    если(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    если(l.output_gpu)              cuda_free(l.output_gpu);
    если(l.delta_gpu)               cuda_free(l.delta_gpu);
    если(l.rand_gpu)                cuda_free(l.rand_gpu);
    если(l.squared_gpu)             cuda_free(l.squared_gpu);
    если(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
