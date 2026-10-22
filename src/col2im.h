#अगर_अघोषित COL2IM_H
#घोषणा COL2IM_H

व्योम col2im_cpu(भग्न* तथ्य_col,
        पूर्णांक channels, पूर्णांक height, पूर्णांक width,
        पूर्णांक ksize, पूर्णांक stride, पूर्णांक pad, भग्न* तथ्य_im);

#अगर_घोषित GPU
व्योम col2im_gpu(भग्न *तथ्य_col,
        पूर्णांक channels, पूर्णांक height, पूर्णांक width,
        पूर्णांक ksize, पूर्णांक stride, पूर्णांक pad, भग्न *तथ्य_im);
#पूर्ण_अगर
#पूर्ण_अगर
