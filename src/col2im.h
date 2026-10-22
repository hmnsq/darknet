#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        පූර්ණ_සංඛ්‍යා channels, පූර්ණ_සංඛ්‍යා height, පූර්ණ_සංඛ්‍යා width,
        පූර්ණ_සංඛ්‍යා ksize, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        පූර්ණ_සංඛ්‍යා channels, පූර්ණ_සංඛ්‍යා height, පූර්ණ_සංඛ්‍යා width,
        පූර්ණ_සංඛ්‍යා ksize, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා pad, float *data_im);
#endif
#endif
