#ifndef COL2IM_H
#define COL2IM_H

فراغ col2im_cpu(عائم* data_col,
        صحيح channels, صحيح height, صحيح width,
        صحيح ksize, صحيح stride, صحيح pad, عائم* data_im);

#ifdef GPU
فراغ col2im_gpu(عائم *data_col,
        صحيح channels, صحيح height, صحيح width,
        صحيح ksize, صحيح stride, صحيح pad, عائم *data_im);
#endif
#endif
