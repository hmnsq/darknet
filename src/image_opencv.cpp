#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    ຈຳນວນເຕັມ x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    ຈຳນວນເຕັມ step = disp->widthStep;
    ສຳລັບ(y = 0; y < im.h; ++y){
        ສຳລັບ(x = 0; x < im.w; ++x){
            ສຳລັບ(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned var_na)(val*255);
            }
        }
    }
    ສົ່ງຄືນ disp;
}

image ipl_to_image(IplImage* src)
{
    ຈຳນວນເຕັມ h = src->height;
    ຈຳນວນເຕັມ w = src->width;
    ຈຳນວນເຕັມ c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned var_na *data = (unsigned var_na *)src->imageData;
    ຈຳນວນເຕັມ step = src->widthStep;
    ຈຳນວນເຕັມ i, j, k;

    ສຳລັບ(i = 0; i < h; ++i){
        ສຳລັບ(k= 0; k < c; ++k){
            ສຳລັບ(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    ສົ່ງຄືນ im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ຖ້າ(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    ສົ່ງຄືນ m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    ສົ່ງຄືນ im;
}

void *open_video_stream(const var_na *f, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ fps)
{
    VideoCapture *cap;
    ຖ້າ(f) cap = new VideoCapture(f);
    ບໍ່ດັ່ງນັ້ນ cap = new VideoCapture(c);
    ຖ້າ(!cap->isOpened()) ສົ່ງຄືນ 0;
    ຖ້າ(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    ຖ້າ(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    ຖ້າ(fps) cap->set(CV_CAP_PROP_FPS, w);
    ສົ່ງຄືນ (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ຖ້າ(m.empty()) ສົ່ງຄືນ make_empty_image(0,0,0);
    ສົ່ງຄືນ mat_to_image(m);
}

image load_image_cv(var_na *filename, ຈຳນວນເຕັມ channels)
{
    ຈຳນວນເຕັມ flag = -1;
    ຖ້າ (channels == 0) flag = -1;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (channels == 1) flag = 0;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (channels == 3) flag = 1;
    ບໍ່ດັ່ງນັ້ນ {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ຖ້າ(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        var_na buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ສົ່ງຄືນ make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    ສົ່ງຄືນ im;
}

ຈຳນວນເຕັມ show_image_cv(image im, const var_na* name, ຈຳນວນເຕັມ ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    ຈຳນວນເຕັມ c = waitKey(ms);
    ຖ້າ (c != -1) c = c%256;
    ສົ່ງຄືນ c;
}

void make_window(var_na *name, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    ຖ້າ (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } ບໍ່ດັ່ງນັ້ນ {
        resizeWindow(name, w, h);
        ຖ້າ(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
