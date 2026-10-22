#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ပေႃး(l.type == DROPOUT){
        ပေႃး(l.rand)           free(l.rand);
#ifdef GPU
        ပေႃး(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ပွၵ်ႈ;
    }
    ပေႃး(l.cweights)           free(l.cweights);
    ပေႃး(l.indexes)            free(l.indexes);
    ပေႃး(l.input_layers)       free(l.input_layers);
    ပေႃး(l.input_sizes)        free(l.input_sizes);
    ပေႃး(l.map)                free(l.map);
    ပေႃး(l.rand)               free(l.rand);
    ပေႃး(l.cost)               free(l.cost);
    ပေႃး(l.state)              free(l.state);
    ပေႃး(l.prev_state)         free(l.prev_state);
    ပေႃး(l.forgot_state)       free(l.forgot_state);
    ပေႃး(l.forgot_delta)       free(l.forgot_delta);
    ပေႃး(l.state_delta)        free(l.state_delta);
    ပေႃး(l.concat)             free(l.concat);
    ပေႃး(l.concat_delta)       free(l.concat_delta);
    ပေႃး(l.binary_weights)     free(l.binary_weights);
    ပေႃး(l.biases)             free(l.biases);
    ပေႃး(l.bias_updates)       free(l.bias_updates);
    ပေႃး(l.scales)             free(l.scales);
    ပေႃး(l.scale_updates)      free(l.scale_updates);
    ပေႃး(l.weights)            free(l.weights);
    ပေႃး(l.weight_updates)     free(l.weight_updates);
    ပေႃး(l.delta)              free(l.delta);
    ပေႃး(l.output)             free(l.output);
    ပေႃး(l.squared)            free(l.squared);
    ပေႃး(l.norms)              free(l.norms);
    ပေႃး(l.spatial_mean)       free(l.spatial_mean);
    ပေႃး(l.mean)               free(l.mean);
    ပေႃး(l.variance)           free(l.variance);
    ပေႃး(l.mean_delta)         free(l.mean_delta);
    ပေႃး(l.variance_delta)     free(l.variance_delta);
    ပေႃး(l.rolling_mean)       free(l.rolling_mean);
    ပေႃး(l.rolling_variance)   free(l.rolling_variance);
    ပေႃး(l.x)                  free(l.x);
    ပေႃး(l.x_norm)             free(l.x_norm);
    ပေႃး(l.m)                  free(l.m);
    ပေႃး(l.v)                  free(l.v);
    ပေႃး(l.z_cpu)              free(l.z_cpu);
    ပေႃး(l.r_cpu)              free(l.r_cpu);
    ပေႃး(l.h_cpu)              free(l.h_cpu);
    ပေႃး(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ပေႃး(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ပေႃး(l.z_gpu)                   cuda_free(l.z_gpu);
    ပေႃး(l.r_gpu)                   cuda_free(l.r_gpu);
    ပေႃး(l.h_gpu)                   cuda_free(l.h_gpu);
    ပေႃး(l.m_gpu)                   cuda_free(l.m_gpu);
    ပေႃး(l.v_gpu)                   cuda_free(l.v_gpu);
    ပေႃး(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ပေႃး(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ပေႃး(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ပေႃး(l.state_gpu)               cuda_free(l.state_gpu);
    ပေႃး(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ပေႃး(l.gate_gpu)                cuda_free(l.gate_gpu);
    ပေႃး(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ပေႃး(l.save_gpu)                cuda_free(l.save_gpu);
    ပေႃး(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ပေႃး(l.concat_gpu)              cuda_free(l.concat_gpu);
    ပေႃး(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ပေႃး(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ပေႃး(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ပေႃး(l.mean_gpu)                cuda_free(l.mean_gpu);
    ပေႃး(l.variance_gpu)            cuda_free(l.variance_gpu);
    ပေႃး(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ပေႃး(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ပေႃး(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ပေႃး(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ပေႃး(l.x_gpu)                   cuda_free(l.x_gpu);
    ပေႃး(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ပေႃး(l.weights_gpu)             cuda_free(l.weights_gpu);
    ပေႃး(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ပေႃး(l.biases_gpu)              cuda_free(l.biases_gpu);
    ပေႃး(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ပေႃး(l.scales_gpu)              cuda_free(l.scales_gpu);
    ပေႃး(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ပေႃး(l.output_gpu)              cuda_free(l.output_gpu);
    ပေႃး(l.delta_gpu)               cuda_free(l.delta_gpu);
    ပေႃး(l.rand_gpu)                cuda_free(l.rand_gpu);
    ပေႃး(l.squared_gpu)             cuda_free(l.squared_gpu);
    ပေႃး(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
