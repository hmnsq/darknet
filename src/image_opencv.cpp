#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    පූර්ණ_සංඛ්‍යා x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    පූර්ණ_සංඛ්‍යා step = disp->widthStep;
    සඳහා(y = 0; y < im.h; ++y){
        සඳහා(x = 0; x < im.w; ++x){
            සඳහා(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned var_na)(val*255);
            }
        }
    }
    ආපසු disp;
}

image ipl_to_image(IplImage* src)
{
    පූර්ණ_සංඛ්‍යා h = src->height;
    පූර්ණ_සංඛ්‍යා w = src->width;
    පූර්ණ_සංඛ්‍යා c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned var_na *data = (unsigned var_na *)src->imageData;
    පූර්ණ_සංඛ්‍යා step = src->widthStep;
    පූර්ණ_සංඛ්‍යා i, j, k;

    සඳහා(i = 0; i < h; ++i){
        සඳහා(k= 0; k < c; ++k){
            සඳහා(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    ආපසු im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    නම්(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    ආපසු m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    ආපසු im;
}

void *open_video_stream(const var_na *f, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා fps)
{
    VideoCapture *cap;
    නම්(f) cap = new VideoCapture(f);
    නැතහොත් cap = new VideoCapture(c);
    නම්(!cap->isOpened()) ආපසු 0;
    නම්(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    නම්(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    නම්(fps) cap->set(CV_CAP_PROP_FPS, w);
    ආපසු (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    නම්(m.empty()) ආපසු make_empty_image(0,0,0);
    ආපසු mat_to_image(m);
}

image load_image_cv(var_na *filename, පූර්ණ_සංඛ්‍යා channels)
{
    පූර්ණ_සංඛ්‍යා flag = -1;
    නම් (channels == 0) flag = -1;
    නැතහොත් නම් (channels == 1) flag = 0;
    නැතහොත් නම් (channels == 3) flag = 1;
    නැතහොත් {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    නම්(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        var_na buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ආපසු make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    ආපසු im;
}

පූර්ණ_සංඛ්‍යා show_image_cv(image im, const var_na* name, පූර්ණ_සංඛ්‍යා ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    පූර්ණ_සංඛ්‍යා c = waitKey(ms);
    නම් (c != -1) c = c%256;
    ආපසු c;
}

void make_window(var_na *name, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    නම් (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } නැතහොත් {
        resizeWindow(name, w, h);
        නම්(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
