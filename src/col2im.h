#ifndef COL2IM_H
#define COL2IM_H

শূন্য col2im_cpu(ভিন্ন* data_col,
        পূর্ণাঙ্ক channels, পূর্ণাঙ্ক height, পূর্ণাঙ্ক width,
        পূর্ণাঙ্ক ksize, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক pad, ভিন্ন* data_im);

#ifdef GPU
শূন্য col2im_gpu(ভিন্ন *data_col,
        পূর্ণাঙ্ক channels, পূর্ণাঙ্ক height, পূর্ণাঙ্ক width,
        পূর্ণাঙ্ক ksize, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক pad, ভিন্ন *data_im);
#endif
#endif
