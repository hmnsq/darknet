#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>
शून्य col2im_सङ्कलय_pixel(भिन्न *im, पूर्णाङ्क height, पूर्णाङ्क विस्तारः, पूर्णाङ्क channels,
                        पूर्णाङ्क row, पूर्णाङ्क col, पूर्णाङ्क channel, पूर्णाङ्क pad, भिन्न val)
{
    row -= pad;
    col -= pad;

    यदि (row < 0 || col < 0 ||
        row >= height || col >= विस्तारः) प्रत्यागम;
    im[col + विस्तारः*(row + height*channel)] += val;
}
//This one might be too, can't remember.
शून्य col2im_cpu(भिन्न* दत्त_col,
         पूर्णाङ्क channels,  पूर्णाङ्क height,  पूर्णाङ्क विस्तारः,
         पूर्णाङ्क ksize,  पूर्णाङ्क stride, पूर्णाङ्क pad, भिन्न* दत्त_im) 
{
    पूर्णाङ्क c,h,w;
    पूर्णाङ्क height_col = (height + 2*pad - ksize) / stride + 1;
    पूर्णाङ्क विस्तारः_col = (विस्तारः + 2*pad - ksize) / stride + 1;

    पूर्णाङ्क channels_col = channels * ksize * ksize;
    प्रति (c = 0; c < channels_col; ++c) {
        पूर्णाङ्क w_विस्थापनम् = c % ksize;
        पूर्णाङ्क h_विस्थापनम् = (c / ksize) % ksize;
        पूर्णाङ्क c_im = c / ksize / ksize;
        प्रति (h = 0; h < height_col; ++h) {
            प्रति (w = 0; w < विस्तारः_col; ++w) {
                पूर्णाङ्क im_row = h_विस्थापनम् + h * stride;
                पूर्णाङ्क im_col = w_विस्थापनम् + w * stride;
                पूर्णाङ्क col_सूचकांक = (c * height_col + h) * विस्तारः_col + w;
                द्विगुण val = दत्त_col[col_सूचकांक];
                col2im_सङ्कलय_pixel(दत्त_im, height, विस्तारः, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

