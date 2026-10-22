#ضمن <stdio.h>
#ضمن <math.h>
فراغ col2im_add_pixel(عائم *im, صحيح height, صحيح width, صحيح channels,
                        صحيح row, صحيح col, صحيح channel, صحيح pad, عائم val)
{
    row -= pad;
    col -= pad;

    إذا (row < 0 || col < 0 ||
        row >= height || col >= width) إرجاع;
    im[col + width*(row + height*channel)] += val;
}
//This one might be too, can't remember.
فراغ col2im_cpu(عائم* data_col,
         صحيح channels,  صحيح height,  صحيح width,
         صحيح ksize,  صحيح stride, صحيح pad, عائم* data_im) 
{
    صحيح c,h,w;
    صحيح height_col = (height + 2*pad - ksize) / stride + 1;
    صحيح width_col = (width + 2*pad - ksize) / stride + 1;

    صحيح channels_col = channels * ksize * ksize;
    لكل (c = 0; c < channels_col; ++c) {
        صحيح w_offset = c % ksize;
        صحيح h_offset = (c / ksize) % ksize;
        صحيح c_im = c / ksize / ksize;
        لكل (h = 0; h < height_col; ++h) {
            لكل (w = 0; w < width_col; ++w) {
                صحيح im_row = h_offset + h * stride;
                صحيح im_col = w_offset + w * stride;
                صحيح col_index = (c * height_col + h) * width_col + w;
                مزدوج val = data_col[col_index];
                col2im_add_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad, val);
            }
        }
    }
}

