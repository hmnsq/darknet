#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    만약(l.type == DROPOUT){
        만약(l.rand)           free(l.rand);
#ifdef GPU
        만약(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        반환;
    }
    만약(l.cweights)           free(l.cweights);
    만약(l.indexes)            free(l.indexes);
    만약(l.input_layers)       free(l.input_layers);
    만약(l.input_sizes)        free(l.input_sizes);
    만약(l.map)                free(l.map);
    만약(l.rand)               free(l.rand);
    만약(l.cost)               free(l.cost);
    만약(l.state)              free(l.state);
    만약(l.prev_state)         free(l.prev_state);
    만약(l.forgot_state)       free(l.forgot_state);
    만약(l.forgot_delta)       free(l.forgot_delta);
    만약(l.state_delta)        free(l.state_delta);
    만약(l.concat)             free(l.concat);
    만약(l.concat_delta)       free(l.concat_delta);
    만약(l.binary_weights)     free(l.binary_weights);
    만약(l.biases)             free(l.biases);
    만약(l.bias_updates)       free(l.bias_updates);
    만약(l.scales)             free(l.scales);
    만약(l.scale_updates)      free(l.scale_updates);
    만약(l.weights)            free(l.weights);
    만약(l.weight_updates)     free(l.weight_updates);
    만약(l.delta)              free(l.delta);
    만약(l.output)             free(l.output);
    만약(l.squared)            free(l.squared);
    만약(l.norms)              free(l.norms);
    만약(l.spatial_mean)       free(l.spatial_mean);
    만약(l.mean)               free(l.mean);
    만약(l.variance)           free(l.variance);
    만약(l.mean_delta)         free(l.mean_delta);
    만약(l.variance_delta)     free(l.variance_delta);
    만약(l.rolling_mean)       free(l.rolling_mean);
    만약(l.rolling_variance)   free(l.rolling_variance);
    만약(l.x)                  free(l.x);
    만약(l.x_norm)             free(l.x_norm);
    만약(l.m)                  free(l.m);
    만약(l.v)                  free(l.v);
    만약(l.z_cpu)              free(l.z_cpu);
    만약(l.r_cpu)              free(l.r_cpu);
    만약(l.h_cpu)              free(l.h_cpu);
    만약(l.binary_input)       free(l.binary_input);

#ifdef GPU
    만약(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    만약(l.z_gpu)                   cuda_free(l.z_gpu);
    만약(l.r_gpu)                   cuda_free(l.r_gpu);
    만약(l.h_gpu)                   cuda_free(l.h_gpu);
    만약(l.m_gpu)                   cuda_free(l.m_gpu);
    만약(l.v_gpu)                   cuda_free(l.v_gpu);
    만약(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    만약(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    만약(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    만약(l.state_gpu)               cuda_free(l.state_gpu);
    만약(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    만약(l.gate_gpu)                cuda_free(l.gate_gpu);
    만약(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    만약(l.save_gpu)                cuda_free(l.save_gpu);
    만약(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    만약(l.concat_gpu)              cuda_free(l.concat_gpu);
    만약(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    만약(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    만약(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    만약(l.mean_gpu)                cuda_free(l.mean_gpu);
    만약(l.variance_gpu)            cuda_free(l.variance_gpu);
    만약(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    만약(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    만약(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    만약(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    만약(l.x_gpu)                   cuda_free(l.x_gpu);
    만약(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    만약(l.weights_gpu)             cuda_free(l.weights_gpu);
    만약(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    만약(l.biases_gpu)              cuda_free(l.biases_gpu);
    만약(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    만약(l.scales_gpu)              cuda_free(l.scales_gpu);
    만약(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    만약(l.output_gpu)              cuda_free(l.output_gpu);
    만약(l.delta_gpu)               cuda_free(l.delta_gpu);
    만약(l.rand_gpu)                cuda_free(l.rand_gpu);
    만약(l.squared_gpu)             cuda_free(l.squared_gpu);
    만약(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
