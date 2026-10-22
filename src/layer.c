#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ᤀᤡ᤺(l.type == DROPOUT){
        ᤀᤡ᤺(l.rand)           free(l.rand);
#ifdef GPU
        ᤀᤡ᤺(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ᤀᤡᤴ;
    }
    ᤀᤡ᤺(l.cweights)           free(l.cweights);
    ᤀᤡ᤺(l.indexes)            free(l.indexes);
    ᤀᤡ᤺(l.input_layers)       free(l.input_layers);
    ᤀᤡ᤺(l.input_sizes)        free(l.input_sizes);
    ᤀᤡ᤺(l.map)                free(l.map);
    ᤀᤡ᤺(l.rand)               free(l.rand);
    ᤀᤡ᤺(l.cost)               free(l.cost);
    ᤀᤡ᤺(l.state)              free(l.state);
    ᤀᤡ᤺(l.prev_state)         free(l.prev_state);
    ᤀᤡ᤺(l.forgot_state)       free(l.forgot_state);
    ᤀᤡ᤺(l.forgot_delta)       free(l.forgot_delta);
    ᤀᤡ᤺(l.state_delta)        free(l.state_delta);
    ᤀᤡ᤺(l.concat)             free(l.concat);
    ᤀᤡ᤺(l.concat_delta)       free(l.concat_delta);
    ᤀᤡ᤺(l.binary_weights)     free(l.binary_weights);
    ᤀᤡ᤺(l.biases)             free(l.biases);
    ᤀᤡ᤺(l.bias_updates)       free(l.bias_updates);
    ᤀᤡ᤺(l.scales)             free(l.scales);
    ᤀᤡ᤺(l.scale_updates)      free(l.scale_updates);
    ᤀᤡ᤺(l.weights)            free(l.weights);
    ᤀᤡ᤺(l.weight_updates)     free(l.weight_updates);
    ᤀᤡ᤺(l.delta)              free(l.delta);
    ᤀᤡ᤺(l.output)             free(l.output);
    ᤀᤡ᤺(l.squared)            free(l.squared);
    ᤀᤡ᤺(l.norms)              free(l.norms);
    ᤀᤡ᤺(l.spatial_mean)       free(l.spatial_mean);
    ᤀᤡ᤺(l.mean)               free(l.mean);
    ᤀᤡ᤺(l.variance)           free(l.variance);
    ᤀᤡ᤺(l.mean_delta)         free(l.mean_delta);
    ᤀᤡ᤺(l.variance_delta)     free(l.variance_delta);
    ᤀᤡ᤺(l.rolling_mean)       free(l.rolling_mean);
    ᤀᤡ᤺(l.rolling_variance)   free(l.rolling_variance);
    ᤀᤡ᤺(l.x)                  free(l.x);
    ᤀᤡ᤺(l.x_norm)             free(l.x_norm);
    ᤀᤡ᤺(l.m)                  free(l.m);
    ᤀᤡ᤺(l.v)                  free(l.v);
    ᤀᤡ᤺(l.z_cpu)              free(l.z_cpu);
    ᤀᤡ᤺(l.r_cpu)              free(l.r_cpu);
    ᤀᤡ᤺(l.h_cpu)              free(l.h_cpu);
    ᤀᤡ᤺(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ᤀᤡ᤺(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ᤀᤡ᤺(l.z_gpu)                   cuda_free(l.z_gpu);
    ᤀᤡ᤺(l.r_gpu)                   cuda_free(l.r_gpu);
    ᤀᤡ᤺(l.h_gpu)                   cuda_free(l.h_gpu);
    ᤀᤡ᤺(l.m_gpu)                   cuda_free(l.m_gpu);
    ᤀᤡ᤺(l.v_gpu)                   cuda_free(l.v_gpu);
    ᤀᤡ᤺(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ᤀᤡ᤺(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ᤀᤡ᤺(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ᤀᤡ᤺(l.state_gpu)               cuda_free(l.state_gpu);
    ᤀᤡ᤺(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ᤀᤡ᤺(l.gate_gpu)                cuda_free(l.gate_gpu);
    ᤀᤡ᤺(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ᤀᤡ᤺(l.save_gpu)                cuda_free(l.save_gpu);
    ᤀᤡ᤺(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ᤀᤡ᤺(l.concat_gpu)              cuda_free(l.concat_gpu);
    ᤀᤡ᤺(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ᤀᤡ᤺(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ᤀᤡ᤺(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ᤀᤡ᤺(l.mean_gpu)                cuda_free(l.mean_gpu);
    ᤀᤡ᤺(l.variance_gpu)            cuda_free(l.variance_gpu);
    ᤀᤡ᤺(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ᤀᤡ᤺(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ᤀᤡ᤺(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ᤀᤡ᤺(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ᤀᤡ᤺(l.x_gpu)                   cuda_free(l.x_gpu);
    ᤀᤡ᤺(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ᤀᤡ᤺(l.weights_gpu)             cuda_free(l.weights_gpu);
    ᤀᤡ᤺(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ᤀᤡ᤺(l.biases_gpu)              cuda_free(l.biases_gpu);
    ᤀᤡ᤺(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ᤀᤡ᤺(l.scales_gpu)              cuda_free(l.scales_gpu);
    ᤀᤡ᤺(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ᤀᤡ᤺(l.output_gpu)              cuda_free(l.output_gpu);
    ᤀᤡ᤺(l.delta_gpu)               cuda_free(l.delta_gpu);
    ᤀᤡ᤺(l.rand_gpu)                cuda_free(l.rand_gpu);
    ᤀᤡ᤺(l.squared_gpu)             cuda_free(l.squared_gpu);
    ᤀᤡ᤺(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
