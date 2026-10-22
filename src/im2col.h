#ifndef IM2COL_H
#define IM2COL_H

تهی im2col_cpu(اعشاری* data_im,
        صحیح channels, صحیح height, صحیح width,
        صحیح ksize, صحیح stride, صحیح pad, اعشاری* data_col);

#ifdef GPU

تهی im2col_gpu(اعشاری *im,
         صحیح channels, صحیح height, صحیح width,
         صحیح ksize, صحیح stride, صحیح pad,اعشاری *data_col);

#endif
#endif
