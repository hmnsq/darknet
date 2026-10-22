#شامل "layer.h"
#شامل "cuda.h"

#شامل <stdlib.h>

تهی free_layer(layer l)
{
    اگر(l.type == DROPOUT){
        اگر(l.rand)           آزاد(l.rand);
#ifdef GPU
        اگر(l.rand_gpu)             cuda_free(l.rand_gpu);
#endif
        بازگشت;
    }
    اگر(l.cweights)           آزاد(l.cweights);
    اگر(l.indexes)            آزاد(l.indexes);
    اگر(l.درون_داد_layers)       آزاد(l.درون_داد_layers);
    اگر(l.درون_داد_sizes)        آزاد(l.درون_داد_sizes);
    اگر(l.map)                آزاد(l.map);
    اگر(l.rand)               آزاد(l.rand);
    اگر(l.cost)               آزاد(l.cost);
    اگر(l.state)              آزاد(l.state);
    اگر(l.prev_state)         آزاد(l.prev_state);
    اگر(l.forgot_state)       آزاد(l.forgot_state);
    اگر(l.forgot_delta)       آزاد(l.forgot_delta);
    اگر(l.state_delta)        آزاد(l.state_delta);
    اگر(l.concat)             آزاد(l.concat);
    اگر(l.concat_delta)       آزاد(l.concat_delta);
    اگر(l.دودویی_weights)     آزاد(l.دودویی_weights);
    اگر(l.biases)             آزاد(l.biases);
    اگر(l.bias_updates)       آزاد(l.bias_updates);
    اگر(l.scales)             آزاد(l.scales);
    اگر(l.scale_updates)      آزاد(l.scale_updates);
    اگر(l.weights)            آزاد(l.weights);
    اگر(l.weight_updates)     آزاد(l.weight_updates);
    اگر(l.delta)              آزاد(l.delta);
    اگر(l.output)             آزاد(l.output);
    اگر(l.squared)            آزاد(l.squared);
    اگر(l.norms)              آزاد(l.norms);
    اگر(l.spatial_mean)       آزاد(l.spatial_mean);
    اگر(l.mean)               آزاد(l.mean);
    اگر(l.variance)           آزاد(l.variance);
    اگر(l.mean_delta)         آزاد(l.mean_delta);
    اگر(l.variance_delta)     آزاد(l.variance_delta);
    اگر(l.rolling_mean)       آزاد(l.rolling_mean);
    اگر(l.rolling_variance)   آزاد(l.rolling_variance);
    اگر(l.x)                  آزاد(l.x);
    اگر(l.x_norm)             آزاد(l.x_norm);
    اگر(l.m)                  آزاد(l.m);
    اگر(l.v)                  آزاد(l.v);
    اگر(l.z_cpu)              آزاد(l.z_cpu);
    اگر(l.r_cpu)              آزاد(l.r_cpu);
    اگر(l.h_cpu)              آزاد(l.h_cpu);
    اگر(l.دودویی_درون_داد)       آزاد(l.دودویی_درون_داد);

#ifdef GPU
    اگر(l.indexes_gpu)           cuda_free((اعشاری *)l.indexes_gpu);

    اگر(l.z_gpu)                   cuda_free(l.z_gpu);
    اگر(l.r_gpu)                   cuda_free(l.r_gpu);
    اگر(l.h_gpu)                   cuda_free(l.h_gpu);
    اگر(l.m_gpu)                   cuda_free(l.m_gpu);
    اگر(l.v_gpu)                   cuda_free(l.v_gpu);
    اگر(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    اگر(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    اگر(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    اگر(l.state_gpu)               cuda_free(l.state_gpu);
    اگر(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    اگر(l.gate_gpu)                cuda_free(l.gate_gpu);
    اگر(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    اگر(l.save_gpu)                cuda_free(l.save_gpu);
    اگر(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    اگر(l.concat_gpu)              cuda_free(l.concat_gpu);
    اگر(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    اگر(l.دودویی_درون_داد_gpu)        cuda_free(l.دودویی_درون_داد_gpu);
    اگر(l.دودویی_weights_gpu)      cuda_free(l.دودویی_weights_gpu);
    اگر(l.mean_gpu)                cuda_free(l.mean_gpu);
    اگر(l.variance_gpu)            cuda_free(l.variance_gpu);
    اگر(l.rolling_mean_gpu)        cuda_free(l.rolling_mean_gpu);
    اگر(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    اگر(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    اگر(l.mean_delta_gpu)          cuda_free(l.mean_delta_gpu);
    اگر(l.x_gpu)                   cuda_free(l.x_gpu);
    اگر(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    اگر(l.weights_gpu)             cuda_free(l.weights_gpu);
    اگر(l.weight_updates_gpu)      cuda_free(l.weight_updates_gpu);
    اگر(l.biases_gpu)              cuda_free(l.biases_gpu);
    اگر(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    اگر(l.scales_gpu)              cuda_free(l.scales_gpu);
    اگر(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    اگر(l.output_gpu)              cuda_free(l.output_gpu);
    اگر(l.delta_gpu)               cuda_free(l.delta_gpu);
    اگر(l.rand_gpu)                cuda_free(l.rand_gpu);
    اگر(l.squared_gpu)             cuda_free(l.squared_gpu);
    اگر(l.norms_gpu)               cuda_free(l.norms_gpu);
#endif
}
