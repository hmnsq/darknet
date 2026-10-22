#ifndef IM2COL_H
#define IM2COL_H

शून्य im2col_cpu(दशांश* data_im,
        पूर्णांक channels, पूर्णांक height, पूर्णांक width,
        पूर्णांक ksize, पूर्णांक stride, पूर्णांक pad, दशांश* data_col);

#ifdef GPU

शून्य im2col_gpu(दशांश *im,
         पूर्णांक channels, पूर्णांक height, पूर्णांक width,
         पूर्णांक ksize, पूर्णांक stride, पूर्णांक pad,दशांश *data_col);

#endif
#endif
