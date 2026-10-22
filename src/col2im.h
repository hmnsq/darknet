#ifndef COL2IM_H
#define COL2IM_H

শূন্য col2im_cpu(দশমিক* তথ্য_col,
        পূর্ণসংখ্যা channels, পূর্ণসংখ্যা height, পূর্ণসংখ্যা width,
        পূর্ণসংখ্যা ksize, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা pad, দশমিক* তথ্য_im);

#ifdef GPU
শূন্য col2im_gpu(দশমিক *তথ্য_col,
        পূর্ণসংখ্যা channels, পূর্ণসংখ্যা height, পূর্ণসংখ্যা width,
        পূর্ণসংখ্যা ksize, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা pad, দশমিক *তথ্য_im);
#endif
#endif
