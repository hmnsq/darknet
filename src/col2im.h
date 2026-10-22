#ifndef COL2IM_H
#define COL2IM_H

تهی col2im_cpu(اعشاری* data_col,
        صحیح channels, صحیح height, صحیح width,
        صحیح ksize, صحیح stride, صحیح pad, اعشاری* data_im);

#ifdef GPU
تهی col2im_gpu(اعشاری *data_col,
        صحیح channels, صحیح height, صحیح width,
        صحیح ksize, صحیح stride, صحیح pad, اعشاری *data_im);
#endif
#endif
