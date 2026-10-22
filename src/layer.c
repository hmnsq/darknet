#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    もし(l.type == DROPOUT){
        もし(l.rand)           free(l.rand);
#ifdef GPU
        もし(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        戻る;
    }
    もし(l.cweights)           free(l.cweights);
    もし(l.indexes)            free(l.indexes);
    もし(l.input_layers)       free(l.input_layers);
    もし(l.input_sizes)        free(l.input_sizes);
    もし(l.map)                free(l.map);
    もし(l.rand)               free(l.rand);
    もし(l.cost)               free(l.cost);
    もし(l.state)              free(l.state);
    もし(l.prev_state)         free(l.prev_state);
    もし(l.forgot_state)       free(l.forgot_state);
    もし(l.forgot_delta)       free(l.forgot_delta);
    もし(l.state_delta)        free(l.state_delta);
    もし(l.concat)             free(l.concat);
    もし(l.concat_delta)       free(l.concat_delta);
    もし(l.binary_weights)     free(l.binary_weights);
    もし(l.biases)             free(l.biases);
    もし(l.bias_updates)       free(l.bias_updates);
    もし(l.scales)             free(l.scales);
    もし(l.scale_updates)      free(l.scale_updates);
    もし(l.weights)            free(l.weights);
    もし(l.weight_updates)     free(l.weight_updates);
    もし(l.delta)              free(l.delta);
    もし(l.output)             free(l.output);
    もし(l.squared)            free(l.squared);
    もし(l.norms)              free(l.norms);
    もし(l.spatial_mean)       free(l.spatial_mean);
    もし(l.mean)               free(l.mean);
    もし(l.variance)           free(l.variance);
    もし(l.mean_delta)         free(l.mean_delta);
    もし(l.variance_delta)     free(l.variance_delta);
    もし(l.rolling_mean)       free(l.rolling_mean);
    もし(l.rolling_variance)   free(l.rolling_variance);
    もし(l.x)                  free(l.x);
    もし(l.x_norm)             free(l.x_norm);
    もし(l.m)                  free(l.m);
    もし(l.v)                  free(l.v);
    もし(l.z_cpu)              free(l.z_cpu);
    もし(l.r_cpu)              free(l.r_cpu);
    もし(l.h_cpu)              free(l.h_cpu);
    もし(l.binary_input)       free(l.binary_input);

#ifdef GPU
    もし(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    もし(l.z_gpu)                   cuda_free(l.z_gpu);
    もし(l.r_gpu)                   cuda_free(l.r_gpu);
    もし(l.h_gpu)                   cuda_free(l.h_gpu);
    もし(l.m_gpu)                   cuda_free(l.m_gpu);
    もし(l.v_gpu)                   cuda_free(l.v_gpu);
    もし(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    もし(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    もし(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    もし(l.state_gpu)               cuda_free(l.state_gpu);
    もし(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    もし(l.gate_gpu)                cuda_free(l.gate_gpu);
    もし(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    もし(l.save_gpu)                cuda_free(l.save_gpu);
    もし(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    もし(l.concat_gpu)              cuda_free(l.concat_gpu);
    もし(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    もし(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    もし(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    もし(l.mean_gpu)                cuda_free(l.mean_gpu);
    もし(l.variance_gpu)            cuda_free(l.variance_gpu);
    もし(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    もし(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    もし(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    もし(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    もし(l.x_gpu)                   cuda_free(l.x_gpu);
    もし(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    もし(l.weights_gpu)             cuda_free(l.weights_gpu);
    もし(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    もし(l.biases_gpu)              cuda_free(l.biases_gpu);
    もし(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    もし(l.scales_gpu)              cuda_free(l.scales_gpu);
    もし(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    もし(l.output_gpu)              cuda_free(l.output_gpu);
    もし(l.delta_gpu)               cuda_free(l.delta_gpu);
    もし(l.rand_gpu)                cuda_free(l.rand_gpu);
    もし(l.squared_gpu)             cuda_free(l.squared_gpu);
    もし(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
