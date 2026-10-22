#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV

#ಅನ್ತರ್ಭಾವ "stdio.h"
#ಅನ್ತರ್ಭಾವ "stdlib.h"
#ಅನ್ತರ್ಭಾವ "opencv2/opencv.hpp"
#ಅನ್ತರ್ಭಾವ "image.h"

ಬಳಸಿ ನಾಮಾಕಾಶ cv;

ಬಾಹ್ಯ "C" {

IplImage *image_ಗಮ್ಯ_ipl(image im)
{
    ಪೂರ್ಣಾಂಕ x,y,c;
    IplImage *disp = cvCreateImage(cv_ಗಾತ್ರ(im.w,im.h), IPL_ಆಳ_8_U, im.c);
    ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = disp->ಅಗಲ_ಹೆಜ್ಜೆ;
    ಗಾಗಿ(y = 0; y < im.h; ++y){
        ಗಾಗಿ(x = 0; x < im.w; ++x){
            ಗಾಗಿ(c= 0; c < im.c; ++c){
                ದಶಮಾಂಶ val = im.ದತ್ತಾಂಶ[c*im.h*im.w + y*im.w + x];
                disp->image_ದತ್ತಾಂಶ[y*ಹೆಜ್ಜೆ + x*im.c + c] = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)(val*255);
            }
        }
    }
    ಹಿಂತಿರುಗಿ disp;
}

image ipl_ಗಮ್ಯ_image(IplImage* src)
{
    ಪೂರ್ಣಾಂಕ h = src->ಎತ್ತರ;
    ಪೂರ್ಣಾಂಕ w = src->ಅಗಲ;
    ಪೂರ್ಣಾಂಕ c = src->nChannels;
    image im = make_image(w, h, c);
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)src->image_ದತ್ತಾಂಶ;
    ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = src->ಅಗಲ_ಹೆಜ್ಜೆ;
    ಪೂರ್ಣಾಂಕ i, j, k;

    ಗಾಗಿ(i = 0; i < h; ++i){
        ಗಾಗಿ(k= 0; k < c; ++k){
            ಗಾಗಿ(j = 0; j < w; ++j){
                im.ದತ್ತಾಂಶ[k*w*h + i*w + j] = ದತ್ತಾಂಶ[i*ಹೆಜ್ಜೆ + j*c + k]/255.;
            }
        }
    }
    ಹಿಂತಿರುಗಿ im;
}

Mat image_ಗಮ್ಯ_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ಆದರೆ(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_ಗಮ್ಯ_ipl(copy);
    Mat m = cvarr_ಗಮ್ಯ_Mat(ipl, ನಿಜ);
    cvReleaseImage(&ipl);
    free_image(copy);
    ಹಿಂತಿರುಗಿ m;
}

image mat_ಗಮ್ಯ_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_ಗಮ್ಯ_image(&ipl);
    rgbgr_image(im);
    ಹಿಂತಿರುಗಿ im;
}

ಶೂನ್ಯ *open_video_ಹರಿವು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *f, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ fps)
{
    VideoCapture *cap;
    ಆದರೆ(f) cap = ಹೊಸ VideoCapture(f);
    ಇಲ್ಲದಿದ್ದರೆ cap = ಹೊಸ VideoCapture(c);
    ಆದರೆ(!cap->isOpened()) ಹಿಂತಿರುಗಿ 0;
    ಆದರೆ(w) cap->ಗಣ(CV_CAP_PROP_ಚೌಕಟ್ಟು_ಅಗಲ, w);
    ಆದರೆ(h) cap->ಗಣ(CV_CAP_PROP_ಚೌಕಟ್ಟು_ಎತ್ತರ, w);
    ಆದರೆ(fps) cap->ಗಣ(CV_CAP_PROP_FPS, w);
    ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *) cap;
}

image get_image_from_ಹರಿವು(ಶೂನ್ಯ *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ಆದರೆ(m.ಖಾಲಿಗಳು()) ಹಿಂತಿರುಗಿ make_ಖಾಲಿಗಳು_image(0,0,0);
    ಹಿಂತಿರುಗಿ mat_ಗಮ್ಯ_image(m);
}

image ತುಂಬು_image_cv(ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ channels)
{
    ಪೂರ್ಣಾಂಕ flag = -1;
    ಆದರೆ (channels == 0) flag = -1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (channels == 1) flag = 0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (channels == 3) flag = 1;
    ಇಲ್ಲದಿದ್ದರೆ {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ಆದರೆ(!m.ದತ್ತಾಂಶ){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        ಅಕ್ಷರ buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ಹಿಂತಿರುಗಿ make_image(10,10,3);
        //exit(0);
    }
    image im = mat_ಗಮ್ಯ_image(m);
    ಹಿಂತಿರುಗಿ im;
}

ಪೂರ್ಣಾಂಕ show_image_cv(image im, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name, ಪೂರ್ಣಾಂಕ ms)
{
    Mat m = image_ಗಮ್ಯ_mat(im);
    imshow(name, m);
    ಪೂರ್ಣಾಂಕ c = wait_ಕೀಲಿ(ms);
    ಆದರೆ (c != -1) c = c%256;
    ಹಿಂತಿರುಗಿ c;
}

ಶೂನ್ಯ make_ಕಿಟಕಿ(ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ fullscreen)
{
    named_ಕಿಟಕಿ(name, ಕಿಟಕಿ_ಸಾಮಾನ್ಯ); 
    ಆದರೆ (fullscreen) {
        ಗಣ_ಕಿಟಕಿ_Property(name, CV_WND_PROP_FULLSCREEN, CV_ಕಿಟಕಿ_FULLSCREEN);
    } ಇಲ್ಲದಿದ್ದರೆ {
        resize_ಕಿಟಕಿ(name, w, h);
        ಆದರೆ(strcmp(name, "Demo") == 0) move_ಕಿಟಕಿ(name, 0, 0);
    }
}

}

#ಆದರೆ_ಅಂತ್ಯ
