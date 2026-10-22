#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    නම්(l.type == DROPOUT){
        නම්(l.rand)           free(l.rand);
#ifdef GPU
        නම්(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ආපසු;
    }
    නම්(l.cweights)           free(l.cweights);
    නම්(l.indexes)            free(l.indexes);
    නම්(l.input_layers)       free(l.input_layers);
    නම්(l.input_sizes)        free(l.input_sizes);
    නම්(l.map)                free(l.map);
    නම්(l.rand)               free(l.rand);
    නම්(l.cost)               free(l.cost);
    නම්(l.state)              free(l.state);
    නම්(l.prev_state)         free(l.prev_state);
    නම්(l.forgot_state)       free(l.forgot_state);
    නම්(l.forgot_delta)       free(l.forgot_delta);
    නම්(l.state_delta)        free(l.state_delta);
    නම්(l.concat)             free(l.concat);
    නම්(l.concat_delta)       free(l.concat_delta);
    නම්(l.binary_weights)     free(l.binary_weights);
    නම්(l.biases)             free(l.biases);
    නම්(l.bias_updates)       free(l.bias_updates);
    නම්(l.scales)             free(l.scales);
    නම්(l.scale_updates)      free(l.scale_updates);
    නම්(l.weights)            free(l.weights);
    නම්(l.weight_updates)     free(l.weight_updates);
    නම්(l.delta)              free(l.delta);
    නම්(l.output)             free(l.output);
    නම්(l.squared)            free(l.squared);
    නම්(l.norms)              free(l.norms);
    නම්(l.spatial_mean)       free(l.spatial_mean);
    නම්(l.mean)               free(l.mean);
    නම්(l.variance)           free(l.variance);
    නම්(l.mean_delta)         free(l.mean_delta);
    නම්(l.variance_delta)     free(l.variance_delta);
    නම්(l.rolling_mean)       free(l.rolling_mean);
    නම්(l.rolling_variance)   free(l.rolling_variance);
    නම්(l.x)                  free(l.x);
    නම්(l.x_norm)             free(l.x_norm);
    නම්(l.m)                  free(l.m);
    නම්(l.v)                  free(l.v);
    නම්(l.z_cpu)              free(l.z_cpu);
    නම්(l.r_cpu)              free(l.r_cpu);
    නම්(l.h_cpu)              free(l.h_cpu);
    නම්(l.binary_input)       free(l.binary_input);

#ifdef GPU
    නම්(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    නම්(l.z_gpu)                   cuda_free(l.z_gpu);
    නම්(l.r_gpu)                   cuda_free(l.r_gpu);
    නම්(l.h_gpu)                   cuda_free(l.h_gpu);
    නම්(l.m_gpu)                   cuda_free(l.m_gpu);
    නම්(l.v_gpu)                   cuda_free(l.v_gpu);
    නම්(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    නම්(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    නම්(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    නම්(l.state_gpu)               cuda_free(l.state_gpu);
    නම්(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    නම්(l.gate_gpu)                cuda_free(l.gate_gpu);
    නම්(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    නම්(l.save_gpu)                cuda_free(l.save_gpu);
    නම්(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    නම්(l.concat_gpu)              cuda_free(l.concat_gpu);
    නම්(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    නම්(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    නම්(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    නම්(l.mean_gpu)                cuda_free(l.mean_gpu);
    නම්(l.variance_gpu)            cuda_free(l.variance_gpu);
    නම්(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    නම්(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    නම්(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    නම්(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    නම්(l.x_gpu)                   cuda_free(l.x_gpu);
    නම්(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    නම්(l.weights_gpu)             cuda_free(l.weights_gpu);
    නම්(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    නම්(l.biases_gpu)              cuda_free(l.biases_gpu);
    නම්(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    නම්(l.scales_gpu)              cuda_free(l.scales_gpu);
    නම්(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    නම්(l.output_gpu)              cuda_free(l.output_gpu);
    නම්(l.delta_gpu)               cuda_free(l.delta_gpu);
    නම්(l.rand_gpu)                cuda_free(l.rand_gpu);
    නම්(l.squared_gpu)             cuda_free(l.squared_gpu);
    නම්(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
