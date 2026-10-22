#ضمن "layer.h"
#ضمن "cuda.h"

#ضمن <stdlib.h>

فراغ free_layer(layer l)
{
    אם(l.type == DROPOUT){
        אם(l.rand)           حرر(l.rand);
#ifdef GPU
        אם(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        החזר;
    }
    אם(l.cweights)           حرر(l.cweights);
    אם(l.indexes)            حرر(l.indexes);
    אם(l.input_layers)       حرر(l.input_layers);
    אם(l.input_sizes)        حرر(l.input_sizes);
    אם(l.map)                حرر(l.map);
    אם(l.rand)               حرر(l.rand);
    אם(l.cost)               حرر(l.cost);
    אם(l.state)              حرر(l.state);
    אם(l.prev_state)         حرر(l.prev_state);
    אם(l.forgot_state)       حرر(l.forgot_state);
    אם(l.forgot_delta)       حرر(l.forgot_delta);
    אם(l.state_delta)        حرر(l.state_delta);
    אם(l.concat)             حرر(l.concat);
    אם(l.concat_delta)       حرر(l.concat_delta);
    אם(l.binary_weights)     حرر(l.binary_weights);
    אם(l.biases)             حرر(l.biases);
    אם(l.bias_updates)       حرر(l.bias_updates);
    אם(l.scales)             حرر(l.scales);
    אם(l.scale_updates)      حرر(l.scale_updates);
    אם(l.weights)            حرر(l.weights);
    אם(l.weight_updates)     حرر(l.weight_updates);
    אם(l.delta)              حرر(l.delta);
    אם(l.output)             حرر(l.output);
    אם(l.squared)            حرر(l.squared);
    אם(l.norms)              حرر(l.norms);
    אם(l.spatial_mean)       حرر(l.spatial_mean);
    אם(l.mean)               حرر(l.mean);
    אם(l.variance)           حرر(l.variance);
    אם(l.mean_delta)         حرر(l.mean_delta);
    אם(l.variance_delta)     حرر(l.variance_delta);
    אם(l.rolling_mean)       حرر(l.rolling_mean);
    אם(l.rolling_variance)   حرر(l.rolling_variance);
    אם(l.x)                  حرر(l.x);
    אם(l.x_norm)             حرر(l.x_norm);
    אם(l.m)                  حرر(l.m);
    אם(l.v)                  حرر(l.v);
    אם(l.z_cpu)              حرر(l.z_cpu);
    אם(l.r_cpu)              حرر(l.r_cpu);
    אם(l.h_cpu)              حرر(l.h_cpu);
    אם(l.binary_input)       حرر(l.binary_input);

#ifdef GPU
    אם(l.indexes_gpu)           cuda_free((عائم *)l.indexes_gpu);

    אם(l.z_gpu)                   cuda_free(l.z_gpu);
    אם(l.r_gpu)                   cuda_free(l.r_gpu);
    אם(l.h_gpu)                   cuda_free(l.h_gpu);
    אם(l.m_gpu)                   cuda_free(l.m_gpu);
    אם(l.v_gpu)                   cuda_free(l.v_gpu);
    אם(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    אם(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    אם(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    אם(l.state_gpu)               cuda_free(l.state_gpu);
    אם(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    אם(l.gate_gpu)                cuda_free(l.gate_gpu);
    אם(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    אם(l.save_gpu)                cuda_free(l.save_gpu);
    אם(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    אם(l.concat_gpu)              cuda_free(l.concat_gpu);
    אם(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    אם(l.binary_input_gpu)        cuda_free(l.binary_input_gpu);
    אם(l.binary_weights_gpu)      cuda_free(l.binary_weights_gpu);
    אם(l.mean_gpu)                cuda_free(l.mean_gpu);
    אם(l.variance_gpu)            cuda_free(l.variance_gpu);
    אם(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    אם(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    אם(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    אם(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    אם(l.x_gpu)                   cuda_free(l.x_gpu);
    אם(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    אם(l.weights_gpu)             cuda_free(l.weights_gpu);
    אם(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    אם(l.biases_gpu)              cuda_free(l.biases_gpu);
    אם(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    אם(l.scales_gpu)              cuda_free(l.scales_gpu);
    אם(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    אם(l.output_gpu)              cuda_free(l.output_gpu);
    אם(l.delta_gpu)               cuda_free(l.delta_gpu);
    אם(l.rand_gpu)                cuda_free(l.rand_gpu);
    אם(l.squared_gpu)             cuda_free(l.squared_gpu);
    אם(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
