#ضمن "layer.h"
#ضمن "cuda.h"

#ضمن <stdlib.h>

فراغ free_layer(layer l)
{
    إذا(l.type == DROPOUT){
        إذا(l.rand)           حرر(l.rand);
#اگر_تعریف_شده GPU
        إذا(l.rand_gpu)             cuda_free(l.rand_gpu);
#پایان_اگر
        أرجع;
    }
    إذا(l.cweights)           حرر(l.cweights);
    إذا(l.indexes)            حرر(l.indexes);
    إذا(l.درون_داد_layers)       حرر(l.درون_داد_layers);
    إذا(l.درون_داد_sizes)        حرر(l.درون_داد_sizes);
    إذا(l.map)                حرر(l.map);
    إذا(l.rand)               حرر(l.rand);
    إذا(l.هزینه_2)               حرر(l.هزینه_2);
    إذا(l.state)              حرر(l.state);
    إذا(l.prev_state)         حرر(l.prev_state);
    إذا(l.forgot_state)       حرر(l.forgot_state);
    إذا(l.forgot_delta)       حرر(l.forgot_delta);
    إذا(l.state_delta)        حرر(l.state_delta);
    إذا(l.concat)             حرر(l.concat);
    إذا(l.concat_delta)       حرر(l.concat_delta);
    إذا(l.دودویی_weights)     حرر(l.دودویی_weights);
    إذا(l.biases)             حرر(l.biases);
    إذا(l.bias_updates)       حرر(l.bias_updates);
    إذا(l.scales)             حرر(l.scales);
    إذا(l.scale_updates)      حرر(l.scale_updates);
    إذا(l.weights)            حرر(l.weights);
    إذا(l.وزن_updates)     حرر(l.وزن_updates);
    إذا(l.delta)              حرر(l.delta);
    إذا(l.output)             حرر(l.output);
    إذا(l.squared)            حرر(l.squared);
    إذا(l.norms)              حرر(l.norms);
    إذا(l.spatial_میانگین)       حرر(l.spatial_میانگین);
    إذا(l.میانگین)               حرر(l.میانگین);
    إذا(l.variance)           حرر(l.variance);
    إذا(l.میانگین_delta)         حرر(l.میانگین_delta);
    إذا(l.variance_delta)     حرر(l.variance_delta);
    إذا(l.rolling_میانگین)       حرر(l.rolling_میانگین);
    إذا(l.rolling_variance)   حرر(l.rolling_variance);
    إذا(l.x)                  حرر(l.x);
    إذا(l.x_norm)             حرر(l.x_norm);
    إذا(l.m)                  حرر(l.m);
    إذا(l.v)                  حرر(l.v);
    إذا(l.z_cpu)              حرر(l.z_cpu);
    إذا(l.r_cpu)              حرر(l.r_cpu);
    إذا(l.h_cpu)              حرر(l.h_cpu);
    إذا(l.دودویی_درون_داد)       حرر(l.دودویی_درون_داد);

#اگر_تعریف_شده GPU
    إذا(l.indexes_gpu)           cuda_free((عائم *)l.indexes_gpu);

    إذا(l.z_gpu)                   cuda_free(l.z_gpu);
    إذا(l.r_gpu)                   cuda_free(l.r_gpu);
    إذا(l.h_gpu)                   cuda_free(l.h_gpu);
    إذا(l.m_gpu)                   cuda_free(l.m_gpu);
    إذا(l.v_gpu)                   cuda_free(l.v_gpu);
    إذا(l.prev_state_gpu)          cuda_free(l.prev_state_gpu);
    إذا(l.forgot_state_gpu)        cuda_free(l.forgot_state_gpu);
    إذا(l.forgot_delta_gpu)        cuda_free(l.forgot_delta_gpu);
    إذا(l.state_gpu)               cuda_free(l.state_gpu);
    إذا(l.state_delta_gpu)         cuda_free(l.state_delta_gpu);
    إذا(l.gate_gpu)                cuda_free(l.gate_gpu);
    إذا(l.gate_delta_gpu)          cuda_free(l.gate_delta_gpu);
    إذا(l.save_gpu)                cuda_free(l.save_gpu);
    إذا(l.save_delta_gpu)          cuda_free(l.save_delta_gpu);
    إذا(l.concat_gpu)              cuda_free(l.concat_gpu);
    إذا(l.concat_delta_gpu)        cuda_free(l.concat_delta_gpu);
    إذا(l.دودویی_درون_داد_gpu)        cuda_free(l.دودویی_درون_داد_gpu);
    إذا(l.دودویی_weights_gpu)      cuda_free(l.دودویی_weights_gpu);
    إذا(l.میانگین_gpu)                cuda_free(l.میانگین_gpu);
    إذا(l.variance_gpu)            cuda_free(l.variance_gpu);
    إذا(l.rolling_میانگین_gpu)        cuda_free(l.rolling_میانگین_gpu);
    إذا(l.rolling_variance_gpu)    cuda_free(l.rolling_variance_gpu);
    إذا(l.variance_delta_gpu)      cuda_free(l.variance_delta_gpu);
    إذا(l.میانگین_delta_gpu)          cuda_free(l.میانگین_delta_gpu);
    إذا(l.x_gpu)                   cuda_free(l.x_gpu);
    إذا(l.x_norm_gpu)              cuda_free(l.x_norm_gpu);
    إذا(l.weights_gpu)             cuda_free(l.weights_gpu);
    إذا(l.وزن_updates_gpu)      cuda_free(l.وزن_updates_gpu);
    إذا(l.biases_gpu)              cuda_free(l.biases_gpu);
    إذا(l.bias_updates_gpu)        cuda_free(l.bias_updates_gpu);
    إذا(l.scales_gpu)              cuda_free(l.scales_gpu);
    إذا(l.scale_updates_gpu)       cuda_free(l.scale_updates_gpu);
    إذا(l.output_gpu)              cuda_free(l.output_gpu);
    إذا(l.delta_gpu)               cuda_free(l.delta_gpu);
    إذا(l.rand_gpu)                cuda_free(l.rand_gpu);
    إذا(l.squared_gpu)             cuda_free(l.squared_gpu);
    إذا(l.norms_gpu)               cuda_free(l.norms_gpu);
#پایان_اگر
}
