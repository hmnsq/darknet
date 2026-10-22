#ifndef IM2COL_H
#define IM2COL_H

فراغ im2col_cpu(عائم* data_im,
        صحيح channels, صحيح height, صحيح width,
        صحيح ksize, صحيح stride, صحيح pad, عائم* data_col);

#ifdef GPU

فراغ im2col_gpu(عائم *im,
         صحيح channels, صحيح height, صحيح width,
         صحيح ksize, صحيح stride, صحيح pad,عائم *data_col);

#endif
#endif
