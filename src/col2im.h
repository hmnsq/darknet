#ifndef COL2IM_H
#define COL2IM_H

शून्य col2im_cpu(दशांश* data_col,
        पूर्णांक channels, पूर्णांक height, पूर्णांक width,
        पूर्णांक ksize, पूर्णांक stride, पूर्णांक pad, दशांश* data_im);

#ifdef GPU
शून्य col2im_gpu(दशांश *data_col,
        पूर्णांक channels, पूर्णांक height, पूर्णांक width,
        पूर्णांक ksize, पूर्णांक stride, पूर्णांक pad, दशांश *data_im);
#endif
#endif
