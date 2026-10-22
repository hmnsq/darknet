#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    ཧྲིལ x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    ཧྲིལ step = disp->widthStep;
    ཆེད་དུ(y = 0; y < im.h; ++y){
        ཆེད་དུ(x = 0; x < im.w; ++x){
            ཆེད་དུ(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned var_na)(val*255);
            }
        }
    }
    ཕྱིར disp;
}

image ipl_to_image(IplImage* src)
{
    ཧྲིལ h = src->height;
    ཧྲིལ w = src->width;
    ཧྲིལ c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned var_na *data = (unsigned var_na *)src->imageData;
    ཧྲིལ step = src->widthStep;
    ཧྲིལ i, j, k;

    ཆེད་དུ(i = 0; i < h; ++i){
        ཆེད་དུ(k= 0; k < c; ++k){
            ཆེད་དུ(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    ཕྱིར im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    གལ་ཏེ(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    ཕྱིར m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    ཕྱིར im;
}

void *open_video_stream(const var_na *f, ཧྲིལ c, ཧྲིལ w, ཧྲིལ h, ཧྲིལ fps)
{
    VideoCapture *cap;
    གལ་ཏེ(f) cap = new VideoCapture(f);
    དེ་མིན cap = new VideoCapture(c);
    གལ་ཏེ(!cap->isOpened()) ཕྱིར 0;
    གལ་ཏེ(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    གལ་ཏེ(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    གལ་ཏེ(fps) cap->set(CV_CAP_PROP_FPS, w);
    ཕྱིར (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    གལ་ཏེ(m.empty()) ཕྱིར make_empty_image(0,0,0);
    ཕྱིར mat_to_image(m);
}

image load_image_cv(var_na *filename, ཧྲིལ channels)
{
    ཧྲིལ flag = -1;
    གལ་ཏེ (channels == 0) flag = -1;
    དེ་མིན གལ་ཏེ (channels == 1) flag = 0;
    དེ་མིན གལ་ཏེ (channels == 3) flag = 1;
    དེ་མིན {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    གལ་ཏེ(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        var_na buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ཕྱིར make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    ཕྱིར im;
}

ཧྲིལ show_image_cv(image im, const var_na* name, ཧྲིལ ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    ཧྲིལ c = waitKey(ms);
    གལ་ཏེ (c != -1) c = c%256;
    ཕྱིར c;
}

void make_window(var_na *name, ཧྲིལ w, ཧྲིལ h, ཧྲིལ fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    གལ་ཏེ (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } དེ་མིན {
        resizeWindow(name, w, h);
        གལ་ཏེ(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
