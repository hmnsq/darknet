#ifndef IM2COL_H
#define IM2COL_H

শূন্য im2col_cpu(ভিন্ন* data_im,
        পূর্ণাঙ্ক channels, পূর্ণাঙ্ক height, পূর্ণাঙ্ক width,
        পূর্ণাঙ্ক ksize, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক pad, ভিন্ন* data_col);

#ifdef GPU

শূন্য im2col_gpu(ভিন্ন *im,
         পূর্ণাঙ্ক channels, পূর্ণাঙ্ক height, পূর্ণাঙ্ক width,
         পূর্ণাঙ্ক ksize, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক pad,ভিন্ন *data_col);

#endif
#endif
