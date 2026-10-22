#include "layer.h"
#include "cuda.h"

#include <stdlib.h>

void free_layer(layer l)
{
    ᬬᬾᬦ᭄(l.type == DROPOUT){
        ᬬᬾᬦ᭄(l.rand)           free(l.rand);
#ifdef GPU
        ᬬᬾᬦ᭄(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        ᬯᬮᬶ;
    }
    ᬬᬾᬦ᭄(l.cweights)           free(l.cweights);
    ᬬᬾᬦ᭄(l.indexes)            free(l.indexes);
    ᬬᬾᬦ᭄(l.input_layers)       free(l.input_layers);
    ᬬᬾᬦ᭄(l.input_sizes)        free(l.input_sizes);
    ᬬᬾᬦ᭄(l.map)                free(l.map);
    ᬬᬾᬦ᭄(l.rand)               free(l.rand);
    ᬬᬾᬦ᭄(l.cost)               free(l.cost);
    ᬬᬾᬦ᭄(l.state)              free(l.state);
    ᬬᬾᬦ᭄(l.prev_state)         free(l.prev_state);
    ᬬᬾᬦ᭄(l.forgot_state)       free(l.forgot_state);
    ᬬᬾᬦ᭄(l.forgot_delta)       free(l.forgot_delta);
    ᬬᬾᬦ᭄(l.state_delta)        free(l.state_delta);
    ᬬᬾᬦ᭄(l.concat)             free(l.concat);
    ᬬᬾᬦ᭄(l.concat_delta)       free(l.concat_delta);
    ᬬᬾᬦ᭄(l.binary_weights)     free(l.binary_weights);
    ᬬᬾᬦ᭄(l.biases)             free(l.biases);
    ᬬᬾᬦ᭄(l.bias_updates)       free(l.bias_updates);
    ᬬᬾᬦ᭄(l.scales)             free(l.scales);
    ᬬᬾᬦ᭄(l.scale_updates)      free(l.scale_updates);
    ᬬᬾᬦ᭄(l.weights)            free(l.weights);
    ᬬᬾᬦ᭄(l.weight_updates)     free(l.weight_updates);
    ᬬᬾᬦ᭄(l.delta)              free(l.delta);
    ᬬᬾᬦ᭄(l.output)             free(l.output);
    ᬬᬾᬦ᭄(l.squared)            free(l.squared);
    ᬬᬾᬦ᭄(l.norms)              free(l.norms);
    ᬬᬾᬦ᭄(l.spatial_mean)       free(l.spatial_mean);
    ᬬᬾᬦ᭄(l.mean)               free(l.mean);
    ᬬᬾᬦ᭄(l.variance)           free(l.variance);
    ᬬᬾᬦ᭄(l.mean_delta)         free(l.mean_delta);
    ᬬᬾᬦ᭄(l.variance_delta)     free(l.variance_delta);
    ᬬᬾᬦ᭄(l.rolling_mean)       free(l.rolling_mean);
    ᬬᬾᬦ᭄(l.rolling_variance)   free(l.rolling_variance);
    ᬬᬾᬦ᭄(l.x)                  free(l.x);
    ᬬᬾᬦ᭄(l.x_norm)             free(l.x_norm);
    ᬬᬾᬦ᭄(l.m)                  free(l.m);
    ᬬᬾᬦ᭄(l.v)                  free(l.v);
    ᬬᬾᬦ᭄(l.z_cpu)              free(l.z_cpu);
    ᬬᬾᬦ᭄(l.r_cpu)              free(l.r_cpu);
    ᬬᬾᬦ᭄(l.h_cpu)              free(l.h_cpu);
    ᬬᬾᬦ᭄(l.binary_input)       free(l.binary_input);

#ifdef GPU
    ᬬᬾᬦ᭄(l.indexes_gpu)           cuda_free((float *)l.indexes_gpu);

    ᬬᬾᬦ᭄(l.z_gpu)                   cuda_free(l.z_gpu);
    ᬬᬾᬦ᭄(l.r_gpu)                   cuda_free(l.r_gpu);
    ᬬᬾᬦ᭄(l.h_gpu)                   cuda_free(l.h_gpu);
    ᬬᬾᬦ᭄(l.m_gpu)                   cuda_free(l.m_gpu);
    ᬬᬾᬦ᭄(l.v_gpu)                   cuda_free(l.v_gpu);
    ᬬᬾᬦ᭄(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    ᬬᬾᬦ᭄(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    ᬬᬾᬦ᭄(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    ᬬᬾᬦ᭄(l.state_gpu)               cuda_free(l.state_gpu);
    ᬬᬾᬦ᭄(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    ᬬᬾᬦ᭄(l.gate_gpu)                cuda_free(l.gate_gpu);
    ᬬᬾᬦ᭄(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    ᬬᬾᬦ᭄(l.save_gpu)                cuda_free(l.save_gpu);
    ᬬᬾᬦ᭄(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    ᬬᬾᬦ᭄(l.concat_gpu)              cuda_free(l.concat_gpu);
    ᬬᬾᬦ᭄(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    ᬬᬾᬦ᭄(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    ᬬᬾᬦ᭄(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    ᬬᬾᬦ᭄(l.mean_gpu)                cuda_free(l.mean_gpu);
    ᬬᬾᬦ᭄(l.variance_gpu)            cuda_free(l.variance_gpu);
    ᬬᬾᬦ᭄(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    ᬬᬾᬦ᭄(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    ᬬᬾᬦ᭄(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    ᬬᬾᬦ᭄(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    ᬬᬾᬦ᭄(l.x_gpu)                   cuda_free(l.x_gpu);
    ᬬᬾᬦ᭄(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    ᬬᬾᬦ᭄(l.weights_gpu)             cuda_free(l.weights_gpu);
    ᬬᬾᬦ᭄(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    ᬬᬾᬦ᭄(l.biases_gpu)              cuda_free(l.biases_gpu);
    ᬬᬾᬦ᭄(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    ᬬᬾᬦ᭄(l.scales_gpu)              cuda_free(l.scales_gpu);
    ᬬᬾᬦ᭄(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    ᬬᬾᬦ᭄(l.output_gpu)              cuda_free(l.output_gpu);
    ᬬᬾᬦ᭄(l.delta_gpu)               cuda_free(l.delta_gpu);
    ᬬᬾᬦ᭄(l.rand_gpu)                cuda_free(l.rand_gpu);
    ᬬᬾᬦ᭄(l.squared_gpu)             cuda_free(l.squared_gpu);
    ᬬᬾᬦ᭄(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
