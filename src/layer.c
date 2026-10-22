#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ຖ້າ(l.type == DROPOUT){
        ຖ້າ(l.rand)           free(l.rand);
#ifdef GPU
        ຖ້າ(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ສົ່ງຄືນ;
    }
    ຖ້າ(l.cweights)           free(l.cweights);
    ຖ້າ(l.indexes)            free(l.indexes);
    ຖ້າ(l.input_layers)       free(l.input_layers);
    ຖ້າ(l.input_sizes)        free(l.input_sizes);
    ຖ້າ(l.map)                free(l.map);
    ຖ້າ(l.rand)               free(l.rand);
    ຖ້າ(l.cost)               free(l.cost);
    ຖ້າ(l.state)              free(l.state);
    ຖ້າ(l.prev_state)         free(l.prev_state);
    ຖ້າ(l.forgot_state)       free(l.forgot_state);
    ຖ້າ(l.forgot_delta)       free(l.forgot_delta);
    ຖ້າ(l.state_delta)        free(l.state_delta);
    ຖ້າ(l.concat)             free(l.concat);
    ຖ້າ(l.concat_delta)       free(l.concat_delta);
    ຖ້າ(l.binary_weights)     free(l.binary_weights);
    ຖ້າ(l.biases)             free(l.biases);
    ຖ້າ(l.bias_updates)       free(l.bias_updates);
    ຖ້າ(l.scales)             free(l.scales);
    ຖ້າ(l.scale_updates)      free(l.scale_updates);
    ຖ້າ(l.weights)            free(l.weights);
    ຖ້າ(l.weight_updates)     free(l.weight_updates);
    ຖ້າ(l.delta)              free(l.delta);
    ຖ້າ(l.output)             free(l.output);
    ຖ້າ(l.squared)            free(l.squared);
    ຖ້າ(l.norms)              free(l.norms);
    ຖ້າ(l.spatial_mean)       free(l.spatial_mean);
    ຖ້າ(l.mean)               free(l.mean);
    ຖ້າ(l.variance)           free(l.variance);
    ຖ້າ(l.mean_delta)         free(l.mean_delta);
    ຖ້າ(l.variance_delta)     free(l.variance_delta);
    ຖ້າ(l.rolling_mean)       free(l.rolling_mean);
    ຖ້າ(l.rolling_variance)   free(l.rolling_variance);
    ຖ້າ(l.x)                  free(l.x);
    ຖ້າ(l.x_norm)             free(l.x_norm);
    ຖ້າ(l.m)                  free(l.m);
    ຖ້າ(l.v)                  free(l.v);
    ຖ້າ(l.z_cpu)              free(l.z_cpu);
    ຖ້າ(l.r_cpu)              free(l.r_cpu);
    ຖ້າ(l.h_cpu)              free(l.h_cpu);
    ຖ້າ(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ຖ້າ(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ຖ້າ(l.z_gpu)                   cuda_free(l.z_gpu);
    ຖ້າ(l.r_gpu)                   cuda_free(l.r_gpu);
    ຖ້າ(l.h_gpu)                   cuda_free(l.h_gpu);
    ຖ້າ(l.m_gpu)                   cuda_free(l.m_gpu);
    ຖ້າ(l.v_gpu)                   cuda_free(l.v_gpu);
    ຖ້າ(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ຖ້າ(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ຖ້າ(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ຖ້າ(l.state_gpu)               cuda_free(l.state_gpu);
    ຖ້າ(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ຖ້າ(l.gate_gpu)                cuda_free(l.gate_gpu);
    ຖ້າ(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ຖ້າ(l.save_gpu)                cuda_free(l.save_gpu);
    ຖ້າ(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ຖ້າ(l.concat_gpu)              cuda_free(l.concat_gpu);
    ຖ້າ(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ຖ້າ(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ຖ້າ(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ຖ້າ(l.mean_gpu)                cuda_free(l.mean_gpu);
    ຖ້າ(l.variance_gpu)            cuda_free(l.variance_gpu);
    ຖ້າ(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ຖ້າ(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ຖ້າ(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ຖ້າ(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ຖ້າ(l.x_gpu)                   cuda_free(l.x_gpu);
    ຖ້າ(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ຖ້າ(l.weights_gpu)             cuda_free(l.weights_gpu);
    ຖ້າ(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ຖ້າ(l.biases_gpu)              cuda_free(l.biases_gpu);
    ຖ້າ(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ຖ້າ(l.scales_gpu)              cuda_free(l.scales_gpu);
    ຖ້າ(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ຖ້າ(l.output_gpu)              cuda_free(l.output_gpu);
    ຖ້າ(l.delta_gpu)               cuda_free(l.delta_gpu);
    ຖ້າ(l.rand_gpu)                cuda_free(l.rand_gpu);
    ຖ້າ(l.squared_gpu)             cuda_free(l.squared_gpu);
    ຖ້າ(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
