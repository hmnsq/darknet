#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        පූර්ණ_සංඛ්‍යා channels, පූර්ණ_සංඛ්‍යා height, පූර්ණ_සංඛ්‍යා width,
        පූර්ණ_සංඛ්‍යා ksize, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         පූර්ණ_සංඛ්‍යා channels, පූර්ණ_සංඛ්‍යා height, පූර්ණ_සංඛ්‍යා width,
         පූර්ණ_සංඛ්‍යා ksize, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා pad,float *data_col);

#endif
#endif
