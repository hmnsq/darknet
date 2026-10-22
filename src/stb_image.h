/* stb_image - v2.19 - public domain image loader - http://nothings.org/stb
                                  no warranty implied; use at your own risk

   Do this:
      #define STB_IMAGE_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.

   // i.e. it should look like this:
   #include ...
   #include ...
   #include ...
   #define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"

   You can #define STBI_ASSERT(x) before the #include to avoid using assert.h.
   And #define STBI_MALLOC, STBI_REALLOC, and STBI_FREE to avoid using malloc,realloc,free


   QUICK NOTES:
      Primarily of interest to game developers and other people who can
          avoid problematic images and only need the trivial interface

      JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
      PNG 1/2/4/8/16-bit-per-channel

      TGA (not sure what subset, if a subset)
      BMP non-1bpp, non-RLE
      PSD (composited view only, no extra channels, 8/16 bit-per-channel)

      GIF (*comp always reports as 4-channel)
      HDR (radiance rgbE format)
      PIC (Softimage PIC)
      PNM (PPM and PGM binary only)

      Animated GIF still needs a proper API, but here's one way to do it:
          http://gist.github.com/urraka/685d9a6340b26b830d49

      - decode from memory or through FILE (define STBI_NO_STDIO to remove code)
      - decode from arbitrary I/O callbacks
      - SIMD acceleration on x86/x64 (SSE2) and ARM (NEON)

   Full documentation under "DOCUMENTATION" below.


LICENSE

  See end of file for license information.

RECENT REVISION HISTORY:

      2.19  (2018-02-11) fix warning
      2.18  (2018-01-30) fix warnings
      2.17  (2018-01-29) bugfix, 1-bit BMP, 16-bitness query, fix warnings
      2.16  (2017-07-23) all functions have 16-bit variants; optimizations; bugfixes
      2.15  (2017-03-18) fix png-1,2,4; all Imagenet JPGs; no runtime SSE detection on GCC
      2.14  (2017-03-03) remove deprecated STBI_JPEG_OLD; fixes for Imagenet JPGs
      2.13  (2016-12-04) experimental 16-bit API, only for PNG so far; fixes
      2.12  (2016-04-02) fix typo in 2.11 PSD fix that caused crashes
      2.11  (2016-04-02) 16-bit PNGS; enable SSE2 in non-gcc x64
                         RGB-format JPEG; remove white matting in PSD;
                         allocate large structures on the stack;
                         correct channel count for PNG & BMP
      2.10  (2016-01-22) avoid warning introduced in 2.09
      2.09  (2016-01-16) 16-bit TGA; comments in PNM files; STBI_REALLOC_SIZED

   See end of file for full revision history.


 ============================    Contributors    =========================

 Image formats                          Extensions, features
    Sean Barrett (jpeg, png, bmp)          Jetro Lauha (stbi_info)
    Nicolas Schulz (hdr, psd)              Martin "SpartanJ" Golini (stbi_info)
    Jonathan Dummer (tga)                  James "moose2000" Brown (iPhone PNG)
    Jean-Marc Lienher (gif)                Ben "Disch" Wenger (io callbacks)
    Tom Seddon (pic)                       Omar Cornut (1/2/4-bit PNG)
    Thatcher Ulrich (psd)                  Nicolas Guillemot (vertical flip)
    Ken Miller (pgm, ppm)                  Richard Mitton (16-bit PSD)
    github:urraka (animated gif)           Junggon Kim (PNM comments)
    Christopher Forseth (animated gif)     Daniel Gibson (16-bit TGA)
                                           socks-the-fox (16-bit PNG)
                                           Jeremy Sawicki (handle all ImageNet JPGs)
 Optimizations & bugfixes                  Mikhail Morozov (1-bit BMP)
    Fabian "ryg" Giesen                    Anael Seghezzi (is-16-bit query)
    Arseny Kapoulkine
    John-Mark Allen

 Bug & warning fixes
    Marc LeBlanc            David Woo          Guillaume George   Martins Mozeiko
    Christpher Lloyd        Jerry Jansson      Joseph Thomson     Phil Jordan
    Dave Moore              Roy Eltham         Hayaki Saito       Nathan Reed
    Won Chun                Luke Graham        Johan Duparc       Nick Verigakis
    the Horde3D community   Thomas Ruf         Ronny Chevalier    github:rlyeh
    Janez Zemva             John Bartholomew   Michal Cichon      github:romigrou
    Jonathan Blow           Ken Hamada         Tero Hanninen      github:svdijk
    Laurent Gomila          Cort Stratton      Sergio Gonzalez    github:snagar
    Aruelien Pocheville     Thibault Reuille   Cass Everitt       github:Zelex
    Ryamond Barbiero        Paul Du Bois       Engin Manap        github:grim210
    Aldo Culquicondor       Philipp Wiesemann  Dale Weiler        github:sammyhw
    Oriol Ferrer Mesia      Josh Tobin         Matthew Gregan     github:phprus
    Julian Raschke          Gregory Mullen     Baldur Karlsson    github:poppolopoppo
    Christian Floisand      Kevin Schmidt                         github:darealshinji
    Blazej Dariusz Roszkowski                                     github:Michaelangel007
*/

#اگر_غیر_تعریف_شدہ STBI_INCLUDE_STB_IMAGE_H
#تعریف_کرو STBI_INCLUDE_STB_IMAGE_H

// DOCUMENTATION
//
// Limitations:
//    - no 12-bit-per-channel JPEG
//    - no JPEGs with arithmetic coding
//    - GIF always returns *comp=4
//
// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data)
//
// Standard parameters:
//    int *x                 -- outputs image width in pixels
//    int *y                 -- outputs image height in pixels
//    int *channels_in_file  -- outputs # of image components in image file
//    int desired_channels   -- if non-zero, # of image components requested in result
//
// The return value from an image loader is an 'unsigned char *' which points
// to the pixel data, or NULL on an allocation failure or if the image is
// corrupt or invalid. The pixel data consists of *y scanlines of *x pixels,
// with each pixel consisting of N interleaved 8-bit components; the first
// pixel pointed to is top-left-most in the image. There is no padding between
// image scanlines or between pixels, regardless of format. The number of
// components N is 'desired_channels' if desired_channels is non-zero, or
// *channels_in_file otherwise. If desired_channels is non-zero,
// *channels_in_file has the number of components that _would_ have been
// output otherwise. E.g. if you set desired_channels to 4, you will always
// get RGBA output, but you can check *channels_in_file to see if it's trivially
// opaque because e.g. there were only 3 channels in the source image.
//
// An output image with N components has the following components interleaved
// in this order in each pixel:
//
//     N=#comp     components
//       1           grey
//       2           grey, alpha
//       3           red, green, blue
//       4           red, green, blue, alpha
//
// If image loading fails for any reason, the return value will be NULL,
// and *x, *y, *channels_in_file will be unchanged. The function
// stbi_failure_reason() can be queried for an extremely brief, end-user
// unfriendly explanation of why the load failed. Define STBI_NO_FAILURE_STRINGS
// to avoid compiling these strings at all, and STBI_FAILURE_USERMSG to get slightly
// more user-friendly ones.
//
// Paletted PNG, BMP, GIF, and PIC images are automatically depalettized.
//
// ===========================================================================
//
// Philosophy
//
// stb libraries are designed with the following priorities:
//
//    1. easy to use
//    2. easy to maintain
//    3. good performance
//
// Sometimes I let "good performance" creep up in priority over "easy to maintain",
// and for best performance I may provide less-easy-to-use APIs that give higher
// performance, in addition to the easy to use ones. Nevertheless, it's important
// to keep in mind that from the standpoint of you, a client of this library,
// all you care about is #1 and #3, and stb libraries DO NOT emphasize #3 above all.
//
// Some secondary priorities arise directly from the first two, some of which
// make more explicit reasons why performance can't be emphasized.
//
//    - Portable ("ease of use")
//    - Small source code footprint ("easy to maintain")
//    - No dependencies ("ease of use")
//
// ===========================================================================
//
// I/O callbacks
//
// I/O callbacks allow you to read from arbitrary sources, like packaged
// files or some other source. Data read from callbacks are processed
// through a small internal buffer (currently 128 bytes) to try to reduce
// overhead.
//
// The three functions you must define are "read" (reads some bytes of data),
// "skip" (skips some bytes of data), "eof" (reports if the stream is at the end).
//
// ===========================================================================
//
// SIMD support
//
// The JPEG decoder will try to automatically use SIMD kernels on x86 when
// supported by the compiler. For ARM Neon support, you must explicitly
// request it.
//
// (The old do-it-yourself SIMD API is no longer supported in the current
// code.)
//
// On x86, SSE2 will automatically be used when available based on a run-time
// test; if not, the generic C versions are used as a fall-back. On ARM targets,
// the typical path is to have separate builds for NEON and non-NEON devices
// (at least this is true for iOS and Android). Therefore, the NEON support is
// toggled by a build flag: define STBI_NEON to get NEON loops.
//
// If for some reason you do not want to use any of SIMD code, or if
// you have issues compiling it, you can disable it entirely by
// defining STBI_NO_SIMD.
//
// ===========================================================================
//
// HDR image support   (disable by defining STBI_NO_HDR)
//
// stb_image now supports loading HDR images in general, and currently
// the Radiance .HDR file format, although the support is provided
// generically. You can still load any file through the existing interface;
// if you attempt to load an HDR file, it will be automatically remapped to
// LDR, assuming gamma 2.2 and an arbitrary scale factor defaulting to 1;
// both of these constants can be reconfigured through this interface:
//
//     stbi_hdr_to_ldr_gamma(2.2f);
//     stbi_hdr_to_ldr_scale(1.0f);
//
// (note, do not use _inverse_ constants; stbi_image will invert them
// appropriately).
//
// Additionally, there is a new, parallel interface for loading files as
// (linear) floats to preserve the full dynamic range:
//
//    float *data = stbi_loadf(filename, &x, &y, &n, 0);
//
// If you load LDR images through this interface, those images will
// be promoted to floating point values, run through the inverse of
// constants corresponding to the above:
//
//     stbi_ldr_to_hdr_scale(1.0f);
//     stbi_ldr_to_hdr_gamma(2.2f);
//
// Finally, given a filename (or an open file or memory block--see header
// file for details) containing image data, you can query for the "most
// appropriate" interface to use (that is, whether the image is HDR or
// not), using:
//
//     stbi_is_hdr(char *filename);
//
// ===========================================================================
//
// iPhone PNG support:
//
// By default we convert iphone-formatted PNGs back to RGB, even though
// they are internally encoded differently. You can disable this conversion
// by by calling stbi_convert_iphone_png_to_rgb(0), in which case
// you will always just get the native iphone "format" through (which
// is BGR stored in RGB).
//
// Call stbi_set_unpremultiply_on_load(1) as well to force a divide per
// pixel to remove any premultiplied alpha *only* if the image file explicitly
// says there's premultiplied data (currently only happens in iPhone images,
// and only if iPhone convert-to-rgb processing is on).
//
// ===========================================================================
//
// ADDITIONAL CONFIGURATION
//
//  - You can suppress implementation of any of the decoders to reduce
//    your code footprint by #defining one or more of the following
//    symbols before creating the implementation.
//
//        STBI_NO_JPEG
//        STBI_NO_PNG
//        STBI_NO_BMP
//        STBI_NO_PSD
//        STBI_NO_TGA
//        STBI_NO_GIF
//        STBI_NO_HDR
//        STBI_NO_PIC
//        STBI_NO_PNM   (.ppm and .pgm)
//
//  - You can request *only* certain decoders and suppress all other ones
//    (this will be more forward-compatible, as addition of new decoders
//    doesn't require you to disable them explicitly):
//
//        STBI_ONLY_JPEG
//        STBI_ONLY_PNG
//        STBI_ONLY_BMP
//        STBI_ONLY_PSD
//        STBI_ONLY_TGA
//        STBI_ONLY_GIF
//        STBI_ONLY_HDR
//        STBI_ONLY_PIC
//        STBI_ONLY_PNM   (.ppm and .pgm)
//
//   - If you use STBI_NO_PNG (or _ONLY_ without PNG), and you still
//     want the zlib decoder to be available, #define STBI_SUPPORT_ZLIB
//


#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
#شامل <stdio.h>
#اگر_ختم // STBI_NO_STDIO

#تعریف_کرو STBI_نسخہ 1

شمار
{
   STBI_default = 0, // only used for desired_channels

   STBI_grey       = 1,
   STBI_grey_alpha = 2,
   STBI_rgb        = 3,
   STBI_rgb_alpha  = 4
};

قسمتعریف غیرعلامتی حرف stbi_uc;
قسمتعریف غیرعلامتی مختصر stbi_us;

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

#اگر_تعریف_شدہ STB_IMAGE_STATIC
#تعریف_کرو STBIDEF ساکن
#ورنہ
#تعریف_کرو STBIDEF خارجی
#اگر_ختم

//////////////////////////////////////////////////////////////////////////////
//
// PRIMARY API - works on images of any type
//

//
// load image by filename, open file, or memory buffer
//

قسمتعریف ساخت
{
   صحیح      (*اقرأ)  (خالی *user,حرف *ڈیٹا,صحیح سائز);   // fill 'data' with 'size' bytes.  return number of bytes actually read
   خالی     (*skip)  (خالی *user,صحیح n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
   صحیح      (*eof)   (خالی *user);                       // returns nonzero if we are at end of file/data
} stbi_io_callbacks;

////////////////////////////////////
//
// 8-bits-per-channel interface
//

STBIDEF stbi_uc *stbi_بھرو_from_حافظہ   (stbi_uc           ثابت *بفر, صحیح len   , صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
STBIDEF stbi_uc *stbi_بھرو_from_callbacks(stbi_io_callbacks ثابت *clbk  , خالی *user, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
#اگر_غیر_تعریف_شدہ STBI_NO_GIF
STBIDEF stbi_uc *stbi_بھرو_gif_from_حافظہ(stbi_uc ثابت *بفر, صحیح len, صحیح **delays, صحیح *x, صحیح *y, صحیح *z, صحیح *comp, صحیح req_comp);
#اگر_ختم


#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
STBIDEF stbi_uc *stbi_بھرو            (حرف ثابت *filename, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
STBIDEF stbi_uc *stbi_بھرو_from_file  (FILE *f, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
// for stbi_load_from_file, file pointer is left pointing immediately after image
#اگر_ختم

////////////////////////////////////
//
// 16-bits-per-channel interface
//

STBIDEF stbi_us *stbi_بھرو_16_from_حافظہ   (stbi_uc ثابت *بفر, صحیح len, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
STBIDEF stbi_us *stbi_بھرو_16_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
STBIDEF stbi_us *stbi_بھرو_16          (حرف ثابت *filename, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
STBIDEF stbi_us *stbi_بھرو_from_file_16(FILE *f, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
#اگر_ختم

////////////////////////////////////
//
// float-per-channel interface
//
#اگر_غیر_تعریف_شدہ STBI_NO_خطی
   STBIDEF اعشاری *stbi_loadf_from_حافظہ     (stbi_uc ثابت *بفر, صحیح len, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
   STBIDEF اعشاری *stbi_loadf_from_callbacks  (stbi_io_callbacks ثابت *clbk, خالی *user, صحیح *x, صحیح *y,  صحیح *channels_اندر_file, صحیح desired_channels);

   #اگر_غیر_تعریف_شدہ STBI_NO_STDIO
   STBIDEF اعشاری *stbi_loadf            (حرف ثابت *filename, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
   STBIDEF اعشاری *stbi_loadf_from_file  (FILE *f, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels);
   #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_HDR
   STBIDEF خالی   stbi_hdr_مقصد_ldr_gamma(اعشاری gamma);
   STBIDEF خالی   stbi_hdr_مقصد_ldr_scale(اعشاری scale);
#اگر_ختم // STBI_NO_HDR

#اگر_غیر_تعریف_شدہ STBI_NO_خطی
   STBIDEF خالی   stbi_ldr_مقصد_hdr_gamma(اعشاری gamma);
   STBIDEF خالی   stbi_ldr_مقصد_hdr_scale(اعشاری scale);
#اگر_ختم // STBI_NO_LINEAR

// stbi_is_hdr is always defined, but always returns false if STBI_NO_HDR
STBIDEF صحیح    stbi_is_hdr_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user);
STBIDEF صحیح    stbi_is_hdr_from_حافظہ(stbi_uc ثابت *بفر, صحیح len);
#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
STBIDEF صحیح      stbi_is_hdr          (حرف ثابت *filename);
STBIDEF صحیح      stbi_is_hdr_from_file(FILE *f);
#اگر_ختم // STBI_NO_STDIO


// get a VERY brief reason for failure
// NOT THREADSAFE
STBIDEF ثابت حرف *stbi_failure_reason  (خالی);

// free the loaded image -- this is just free()
STBIDEF خالی     stbi_image_free      (خالی *retval_from_stbi_بھرو);

// get image dimensions & components without fully decoding
STBIDEF صحیح      stbi_info_from_حافظہ(stbi_uc ثابت *بفر, صحیح len, صحیح *x, صحیح *y, صحیح *comp);
STBIDEF صحیح      stbi_info_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user, صحیح *x, صحیح *y, صحیح *comp);
STBIDEF صحیح      stbi_is_16_ثنائی_ہندسہ_from_حافظہ(stbi_uc ثابت *بفر, صحیح len);
STBIDEF صحیح      stbi_is_16_ثنائی_ہندسہ_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user);

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
STBIDEF صحیح      stbi_info               (حرف ثابت *filename,     صحیح *x, صحیح *y, صحیح *comp);
STBIDEF صحیح      stbi_info_from_file     (FILE *f,                  صحیح *x, صحیح *y, صحیح *comp);
STBIDEF صحیح      stbi_is_16_ثنائی_ہندسہ          (حرف ثابت *filename);
STBIDEF صحیح      stbi_is_16_ثنائی_ہندسہ_from_file(FILE *f);
#اگر_ختم



// for image formats that explicitly notate that they have premultiplied alpha,
// we just return the colors as stored in the file. set this flag to force
// unpremultiplication. results are undefined if the unpremultiply overflow.
STBIDEF خالی stbi_سیٹ_unpremultiply_on_بھرو(صحیح flag_true_if_should_unpremultiply);

// indicate whether we should process iphone images back to canonical format,
// or just pass them through "as-is"
STBIDEF خالی stbi_convert_iphone_png_مقصد_rgb(صحیح flag_true_if_should_convert);

// flip the image vertically, so the first pixel in the output array is the bottom left
STBIDEF خالی stbi_سیٹ_flip_vertically_on_بھرو(صحیح flag_true_if_should_flip);

// ZLIB client - used by PNG, available for other purposes

STBIDEF حرف *stbi_zlib_decode_malloc_guesssize(ثابت حرف *بفر, صحیح len, صحیح initial_سائز, صحیح *outlen);
STBIDEF حرف *stbi_zlib_decode_malloc_guesssize_headerflag(ثابت حرف *بفر, صحیح len, صحیح initial_سائز, صحیح *outlen, صحیح parse_سرخی);
STBIDEF حرف *stbi_zlib_decode_malloc(ثابت حرف *بفر, صحیح len, صحیح *outlen);
STBIDEF صحیح   stbi_zlib_decode_بفر(حرف *obuffer, صحیح olen, ثابت حرف *ibuffer, صحیح ilen);

STBIDEF حرف *stbi_zlib_decode_noheader_malloc(ثابت حرف *بفر, صحیح len, صحیح *outlen);
STBIDEF صحیح   stbi_zlib_decode_noheader_بفر(حرف *obuffer, صحیح olen, ثابت حرف *ibuffer, صحیح ilen);


#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

//
//
////   end header file   /////////////////////////////////////////////////////
#اگر_ختم // STBI_INCLUDE_STB_IMAGE_H

#اگر_تعریف_شدہ STB_IMAGE_IMPLEMENTATION

#اگر defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
   #اگر_غیر_تعریف_شدہ STBI_ONLY_JPEG
   #تعریف_کرو STBI_NO_JPEG
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_PNG
   #تعریف_کرو STBI_NO_PNG
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_BMP
   #تعریف_کرو STBI_NO_BMP
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_PSD
   #تعریف_کرو STBI_NO_PSD
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_TGA
   #تعریف_کرو STBI_NO_TGA
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_GIF
   #تعریف_کرو STBI_NO_GIF
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_HDR
   #تعریف_کرو STBI_NO_HDR
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_PIC
   #تعریف_کرو STBI_NO_PIC
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_ONLY_PNM
   #تعریف_کرو STBI_NO_PNM
   #اگر_ختم
#اگر_ختم

#اگر defined(STBI_NO_PNG) && !defined(STBI_SUPPORT_ZLIB) && !defined(STBI_NO_ZLIB)
#تعریف_کرو STBI_NO_ZLIB
#اگر_ختم


#شامل <stdarg.h>
#شامل <stddef.h> // ptrdiff_t on osx
#شامل <stdlib.h>
#شامل <string.h>
#شامل <limits.h>

#اگر !defined(STBI_NO_خطی) || !defined(STBI_NO_HDR)
#شامل <math.h>  // ldexp, pow
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
#شامل <stdio.h>
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_ASSERT
#شامل <assert.h>
#تعریف_کرو STBI_ASSERT(x) أكد(x)
#اگر_ختم


#اگر_غیر_تعریف_شدہ _MSC_VER
   #اگر_تعریف_شدہ __cplusplus
   #تعریف_کرو stbi_inline اندرونی
   #ورنہ
   #تعریف_کرو stbi_inline
   #اگر_ختم
#ورنہ
   #تعریف_کرو stbi_inline __forceinline
#اگر_ختم


#اگر_تعریف_شدہ _MSC_VER
قسمتعریف غیرعلامتی مختصر stbi__uint16;
قسمتعریف   علامتی مختصر stbi__int16;
قسمتعریف غیرعلامتی صحیح   stbi__uint32;
قسمتعریف   علامتی صحیح   stbi__int32;
#ورنہ
#شامل <stdint.h>
قسمتعریف uint16_t stbi__uint16;
قسمتعریف int16_t  stbi__int16;
قسمتعریف uint32_t stbi__uint32;
قسمتعریف int32_t  stbi__int32;
#اگر_ختم

// should produce compiler error if size is wrong
قسمتعریف غیرعلامتی حرف validate_uint32[حجم(stbi__uint32)==4 ? 1 : -1];

#اگر_تعریف_شدہ _MSC_VER
#تعریف_کرو STBI_NOTUSED(v)  (خالی)(v)
#ورنہ
#تعریف_کرو STBI_NOTUSED(v)  (خالی)حجم(v)
#اگر_ختم

#اگر_تعریف_شدہ _MSC_VER
#تعریف_کرو STBI_HAS_LROTL
#اگر_ختم

#اگر_تعریف_شدہ STBI_HAS_LROTL
   #تعریف_کرو stbi_lrot(x,y)  _lrotl(x,y)
#ورنہ
   #تعریف_کرو stbi_lrot(x,y)  (((x) << (y)) | ((x) >> (32 - (y))))
#اگر_ختم

#اگر defined(STBI_MALLOC) && defined(STBI_FREE) && (defined(STBI_REALLOC) || defined(STBI_REALLOC_SIZED))
// ok
#وگرنہ_اگر !defined(STBI_MALLOC) && !defined(STBI_FREE) && !defined(STBI_REALLOC) && !defined(STBI_REALLOC_SIZED)
// ok
#ورنہ
#غلطی "Must define all or none of STBI_MALLOC, STBI_FREE, and STBI_REALLOC (or STBI_REALLOC_SIZED)."
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_MALLOC
#تعریف_کرو STBI_MALLOC(sz)           حافظہ_دو(sz)
#تعریف_کرو STBI_REALLOC(p,newsz)     realloc(p,newsz)
#تعریف_کرو STBI_FREE(p)              آزاد(p)
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_REALLOC_SIZED
#تعریف_کرو STBI_REALLOC_SIZED(p,oldsz,newsz) STBI_REALLOC(p,newsz)
#اگر_ختم

// x86/x64 detection
#اگر defined(__x86_64__) || defined(_M_X64)
#تعریف_کرو STBI_X64_ہدف
#وگرنہ_اگر defined(__i386) || defined(_M_IX86)
#تعریف_کرو STBI_X86_ہدف
#اگر_ختم

#اگر defined(__GNUC__) && defined(STBI_X86_ہدف) && !defined(__SSE2__) && !defined(STBI_NO_SIMD)
// gcc doesn't support sse2 intrinsics unless you compile with -msse2,
// which in turn means it gets to use SSE2 everywhere. This is unfortunate,
// but previous attempts to provide the SSE2 functions with runtime
// detection caused numerous issues. The way architecture extensions are
// exposed in GCC/Clang is, sadly, not really suited for one-file libs.
// New behavior: if compiled with -msse2, we use SSE2 without any
// detection; if not, we don't use it at all.
#تعریف_کرو STBI_NO_SIMD
#اگر_ختم

#اگر defined(__MINGW32__) && defined(STBI_X86_ہدف) && !defined(STBI_MINGW_ENABLE_SSE2) && !defined(STBI_NO_SIMD)
// Note that __MINGW32__ doesn't actually mean 32-bit, so we have to avoid STBI__X64_TARGET
//
// 32-bit MinGW wants ESP to be 16-byte aligned, but this is not in the
// Windows ABI and VC++ as well as Windows DLLs don't maintain that invariant.
// As a result, enabling SSE2 on 32-bit MinGW is dangerous when not
// simultaneously enabling "-mstackrealign".
//
// See https://github.com/nothings/stb/issues/81 for more information.
//
// So default to no SSE2 on 32-bit MinGW. If you've read this far and added
// -mstackrealign to your build settings, feel free to #define STBI_MINGW_ENABLE_SSE2.
#تعریف_کرو STBI_NO_SIMD
#اگر_ختم

#اگر !defined(STBI_NO_SIMD) && (defined(STBI_X86_ہدف) || defined(STBI_X64_ہدف))
#تعریف_کرو STBI_SSE2
#شامل <emmintrin.h>

#اگر_تعریف_شدہ _MSC_VER

#اگر _MSC_VER >= 1400  // not VC6
#شامل <intrin.h> // __cpuid
ساکن صحیح stbi__cpuid3(خالی)
{
   صحیح info[4];
   __cpuid(info,1);
   واپس info[3];
}
#ورنہ
ساکن صحیح stbi__cpuid3(خالی)
{
   صحیح res;
   __asm {
      mov  eax,1
      cpuid
      mov  res,edx
   }
   واپس res;
}
#اگر_ختم

#تعریف_کرو STBI_SIMD_ALIGN(type, name) __declspec(align(16)) type name

ساکن صحیح stbi_sse2_دستیاب(خالی)
{
   صحیح info3 = stbi__cpuid3();
   واپس ((info3 >> 26) & 1) != 0;
}
#ورنہ // assume GCC-style if not VC++
#تعریف_کرو STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))

ساکن صحیح stbi_sse2_دستیاب(خالی)
{
   // If we're even attempting to compile this on GCC/Clang, that means
   // -msse2 is on, which means the compiler is allowed to use SSE2
   // instructions at will, and so are we.
   واپس 1;
}
#اگر_ختم
#اگر_ختم

// ARM NEON
#اگر defined(STBI_NO_SIMD) && defined(STBI_NEON)
#تعریف_منسوخ STBI_NEON
#اگر_ختم

#اگر_تعریف_شدہ STBI_NEON
#شامل <arm_neon.h>
// assume GCC or Clang on ARM targets
#تعریف_کرو STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_SIMD_ALIGN
#تعریف_کرو STBI_SIMD_ALIGN(type, name) type name
#اگر_ختم

///////////////////////////////////////////////
//
//  stbi__context struct and start_xxx functions

// stbi__context structure is our basic context used by all images, so it
// contains all the IO context, plus some basic image information
قسمتعریف ساخت
{
   stbi__uint32 img_x, img_y;
   صحیح img_n, img_برآمد_n;

   stbi_io_callbacks io;
   خالی *io_user_ڈیٹا;

   صحیح read_from_callbacks;
   صحیح buflen;
   stbi_uc بفر_آغاز[128];

   stbi_uc *img_بفر, *img_بفر_انتہا;
   stbi_uc *img_بفر_original, *img_بفر_original_انتہا;
} stbi__context;


ساکن خالی stbi_refill_بفر(stbi__context *s);

// initialize a memory-decode context
ساکن خالی stbi_آغاز_mem(stbi__context *s, stbi_uc ثابت *بفر, صحیح len)
{
   s->io.اقرأ = تہی;
   s->read_from_callbacks = 0;
   s->img_بفر = s->img_بفر_original = (stbi_uc *) بفر;
   s->img_بفر_انتہا = s->img_بفر_original_انتہا = (stbi_uc *) بفر+len;
}

// initialize a callback-based context
ساکن خالی stbi_آغاز_callbacks(stbi__context *s, stbi_io_callbacks *c, خالی *user)
{
   s->io = *c;
   s->io_user_ڈیٹا = user;
   s->buflen = حجم(s->بفر_آغاز);
   s->read_from_callbacks = 1;
   s->img_بفر_original = s->بفر_آغاز;
   stbi_refill_بفر(s);
   s->img_بفر_original_انتہا = s->img_بفر_انتہا;
}

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO

ساکن صحیح stbi__stdio_read(خالی *user, حرف *ڈیٹا, صحیح سائز)
{
   واپس (صحیح) fread(ڈیٹا,1,سائز,(FILE*) user);
}

ساکن خالی stbi__stdio_skip(خالی *user, صحیح n)
{
   fseek((FILE*) user, n, SEEK_CUR);
}

ساکن صحیح stbi__stdio_eof(خالی *user)
{
   واپس feof((FILE*) user);
}

ساکن stbi_io_callbacks stbi__stdio_callbacks =
{
   stbi__stdio_read,
   stbi__stdio_skip,
   stbi__stdio_eof,
};

ساکن خالی stbi_آغاز_file(stbi__context *s, FILE *f)
{
   stbi_آغاز_callbacks(s, &stbi__stdio_callbacks, (خالی *) f);
}

//static void stop_file(stbi__context *s) { }

#اگر_ختم // !STBI_NO_STDIO

ساکن خالی stbi__rewind(stbi__context *s)
{
   // conceptually rewind SHOULD rewind to the beginning of the stream,
   // but we just rewind to the beginning of the initial buffer, because
   // we only use it after doing 'test', which only ever looks at at most 92 bytes
   s->img_بفر = s->img_بفر_original;
   s->img_بفر_انتہا = s->img_بفر_original_انتہا;
}

شمار
{
   STBI_ترتیب_RGB,
   STBI_ترتیب_BGR
};

قسمتعریف ساخت
{
   صحیح بٹ_فی_channel;
   صحیح عدد_channels;
   صحیح channel_ترتیب;
} stbi_نتیجہ_info;

#اگر_غیر_تعریف_شدہ STBI_NO_JPEG
ساکن صحیح      stbi__jpeg_test(stbi__context *s);
ساکن خالی    *stbi_jpeg_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن صحیح      stbi__jpeg_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_PNG
ساکن صحیح      stbi__png_test(stbi__context *s);
ساکن خالی    *stbi_png_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن صحیح      stbi__png_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
ساکن صحیح      stbi__png_is16(stbi__context *s);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_BMP
ساکن صحیح      stbi__bmp_test(stbi__context *s);
ساکن خالی    *stbi_bmp_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن صحیح      stbi__bmp_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_TGA
ساکن صحیح      stbi__tga_test(stbi__context *s);
ساکن خالی    *stbi_tga_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن صحیح      stbi__tga_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_PSD
ساکن صحیح      stbi__psd_test(stbi__context *s);
ساکن خالی    *stbi_psd_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri, صحیح bpc);
ساکن صحیح      stbi__psd_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
ساکن صحیح      stbi__psd_is16(stbi__context *s);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_HDR
ساکن صحیح      stbi__hdr_test(stbi__context *s);
ساکن اعشاری   *stbi_hdr_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن صحیح      stbi__hdr_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_PIC
ساکن صحیح      stbi__pic_test(stbi__context *s);
ساکن خالی    *stbi_pic_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن صحیح      stbi__pic_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_GIF
ساکن صحیح      stbi__gif_test(stbi__context *s);
ساکن خالی    *stbi_gif_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن خالی    *stbi_بھرو_gif_main(stbi__context *s, صحیح **delays, صحیح *x, صحیح *y, صحیح *z, صحیح *comp, صحیح req_comp);
ساکن صحیح      stbi__gif_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_PNM
ساکن صحیح      stbi__pnm_test(stbi__context *s);
ساکن خالی    *stbi_pnm_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri);
ساکن صحیح      stbi__pnm_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp);
#اگر_ختم

// this is not threadsafe
ساکن ثابت حرف *stbi__g_failure_reason;

STBIDEF ثابت حرف *stbi_failure_reason(خالی)
{
   واپس stbi__g_failure_reason;
}

ساکن صحیح stbi__err(ثابت حرف *str)
{
   stbi__g_failure_reason = str;
   واپس 0;
}

ساکن خالی *stbi__malloc(size_t سائز)
{
    واپس STBI_MALLOC(سائز);
}

// stb_image uses ints pervasively, including for offset calculations.
// therefore the largest decoded image size we can support with the
// current code, even on 64-bit targets, is INT_MAX. this is not a
// significant limitation for the intended use case.
//
// we do, however, need to make sure our size calculations don't
// overflow. hence a few helper functions for size calculations that
// multiply integers together, making sure that they're non-negative
// and no overflow occurs.

// return 1 if the sum is valid, 0 on overflow.
// negative terms are considered invalid.
ساکن صحیح stbi__addsizes_valid(صحیح a, صحیح b)
{
   اگر (b < 0) واپس 0;
   // now 0 <= b <= INT_MAX, hence also
   // 0 <= INT_MAX - b <= INTMAX.
   // And "a + b <= INT_MAX" (which might overflow) is the
   // same as a <= INT_MAX - b (no overflow)
   واپس a <= زیادہ_سے_زیادہ_عدد - b;
}

// returns 1 if the product is valid, 0 on overflow.
// negative factors are considered invalid.
ساکن صحیح stbi__mul2sizes_valid(صحیح a, صحیح b)
{
   اگر (a < 0 || b < 0) واپس 0;
   اگر (b == 0) واپس 1; // mul-by-0 is always safe
   // portable way to check for no overflows in a*b
   واپس a <= زیادہ_سے_زیادہ_عدد/b;
}

// returns 1 if "a*b + add" has no negative terms/factors and doesn't overflow
ساکن صحیح stbi__mad2sizes_valid(صحیح a, صحیح b, صحیح جوڑو)
{
   واپس stbi__mul2sizes_valid(a, b) && stbi__addsizes_valid(a*b, جوڑو);
}

// returns 1 if "a*b*c + add" has no negative terms/factors and doesn't overflow
ساکن صحیح stbi__mad3sizes_valid(صحیح a, صحیح b, صحیح c, صحیح جوڑو)
{
   واپس stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__addsizes_valid(a*b*c, جوڑو);
}

// returns 1 if "a*b*c*d + add" has no negative terms/factors and doesn't overflow
#اگر !defined(STBI_NO_خطی) || !defined(STBI_NO_HDR)
ساکن صحیح stbi__mad4sizes_valid(صحیح a, صحیح b, صحیح c, صحیح d, صحیح جوڑو)
{
   واپس stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__mul2sizes_valid(a*b*c, d) && stbi__addsizes_valid(a*b*c*d, جوڑو);
}
#اگر_ختم

// mallocs with size overflow checking
ساکن خالی *stbi__malloc_mad2(صحیح a, صحیح b, صحیح جوڑو)
{
   اگر (!stbi__mad2sizes_valid(a, b, جوڑو)) واپس تہی;
   واپس stbi__malloc(a*b + جوڑو);
}

ساکن خالی *stbi__malloc_mad3(صحیح a, صحیح b, صحیح c, صحیح جوڑو)
{
   اگر (!stbi__mad3sizes_valid(a, b, c, جوڑو)) واپس تہی;
   واپس stbi__malloc(a*b*c + جوڑو);
}

#اگر !defined(STBI_NO_خطی) || !defined(STBI_NO_HDR)
ساکن خالی *stbi__malloc_mad4(صحیح a, صحیح b, صحیح c, صحیح d, صحیح جوڑو)
{
   اگر (!stbi__mad4sizes_valid(a, b, c, d, جوڑو)) واپس تہی;
   واپس stbi__malloc(a*b*c*d + جوڑو);
}
#اگر_ختم

// stbi__err - error
// stbi__errpf - error returning pointer to float
// stbi__errpuc - error returning pointer to unsigned char

#اگر_تعریف_شدہ STBI_NO_FAILURE_لڑیاں
   #تعریف_کرو stbi__err(x,y)  0
#وگرنہ_اگر defined(STBI_FAILURE_USERMSG)
   #تعریف_کرو stbi__err(x,y)  stbi__err(y)
#ورنہ
   #تعریف_کرو stbi__err(x,y)  stbi__err(x)
#اگر_ختم

#تعریف_کرو stbi__errpf(x,y)   ((اعشاری *)(size_t) (stbi__err(x,y)?تہی:تہی))
#تعریف_کرو stbi__errpuc(x,y)  ((غیرعلامتی حرف *)(size_t) (stbi__err(x,y)?تہی:تہی))

STBIDEF خالی stbi_image_free(خالی *retval_from_stbi_بھرو)
{
   STBI_FREE(retval_from_stbi_بھرو);
}

#اگر_غیر_تعریف_شدہ STBI_NO_خطی
ساکن اعشاری   *stbi_ldr_مقصد_hdr(stbi_uc *ڈیٹا, صحیح x, صحیح y, صحیح comp);
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_HDR
ساکن stbi_uc *stbi_hdr_مقصد_ldr(اعشاری   *ڈیٹا, صحیح x, صحیح y, صحیح comp);
#اگر_ختم

ساکن صحیح stbi_vertically_flip_on_بھرو = 0;

STBIDEF خالی stbi_سیٹ_flip_vertically_on_بھرو(صحیح flag_true_if_should_flip)
{
    stbi_vertically_flip_on_بھرو = flag_true_if_should_flip;
}

ساکن خالی *stbi_بھرو_main(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri, صحیح bpc)
{
   memset(ri, 0, حجم(*ri)); // make sure it's initialized if we add new fields
   ri->بٹ_فی_channel = 8; // default is 8 so most paths don't have to be changed
   ri->channel_ترتیب = STBI_ترتیب_RGB; // all current input & output are this, but this is here so we can add BGR order
   ri->عدد_channels = 0;

   #اگر_غیر_تعریف_شدہ STBI_NO_JPEG
   اگر (stbi__jpeg_test(s)) واپس stbi_jpeg_بھرو(s,x,y,comp,req_comp, ri);
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_NO_PNG
   اگر (stbi__png_test(s))  واپس stbi_png_بھرو(s,x,y,comp,req_comp, ri);
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_NO_BMP
   اگر (stbi__bmp_test(s))  واپس stbi_bmp_بھرو(s,x,y,comp,req_comp, ri);
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_NO_GIF
   اگر (stbi__gif_test(s))  واپس stbi_gif_بھرو(s,x,y,comp,req_comp, ri);
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_NO_PSD
   اگر (stbi__psd_test(s))  واپس stbi_psd_بھرو(s,x,y,comp,req_comp, ri, bpc);
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_NO_PIC
   اگر (stbi__pic_test(s))  واپس stbi_pic_بھرو(s,x,y,comp,req_comp, ri);
   #اگر_ختم
   #اگر_غیر_تعریف_شدہ STBI_NO_PNM
   اگر (stbi__pnm_test(s))  واپس stbi_pnm_بھرو(s,x,y,comp,req_comp, ri);
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_HDR
   اگر (stbi__hdr_test(s)) {
      اعشاری *hdr = stbi_hdr_بھرو(s, x,y,comp,req_comp, ri);
      واپس stbi_hdr_مقصد_ldr(hdr, *x, *y, req_comp ? req_comp : *comp);
   }
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_TGA
   // test tga last because it's a crappy test!
   اگر (stbi__tga_test(s))
      واپس stbi_tga_بھرو(s,x,y,comp,req_comp, ri);
   #اگر_ختم

   واپس stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
}

ساکن stbi_uc *stbi_convert_16_مقصد_8(stbi__uint16 *orig, صحیح w, صحیح h, صحیح channels)
{
   صحیح i;
   صحیح img_len = w * h * channels;
   stbi_uc *reduced;

   reduced = (stbi_uc *) stbi__malloc(img_len);
   اگر (reduced == تہی) واپس stbi__errpuc("outofmem", "Out of memory");

   برائے (i = 0; i < img_len; ++i)
      reduced[i] = (stbi_uc)((orig[i] >> 8) & 0xFF); // top half of each byte is sufficient approx of 16->8 bit scaling

   STBI_FREE(orig);
   واپس reduced;
}

ساکن stbi__uint16 *stbi_convert_8_مقصد_16(stbi_uc *orig, صحیح w, صحیح h, صحیح channels)
{
   صحیح i;
   صحیح img_len = w * h * channels;
   stbi__uint16 *enlarged;

   enlarged = (stbi__uint16 *) stbi__malloc(img_len*2);
   اگر (enlarged == تہی) واپس (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");

   برائے (i = 0; i < img_len; ++i)
      enlarged[i] = (stbi__uint16)((orig[i] << 8) + orig[i]); // replicate to high and low byte, maps 0->0, 255->0xffff

   STBI_FREE(orig);
   واپس enlarged;
}

ساکن خالی stbi__vertical_flip(خالی *image, صحیح w, صحیح h, صحیح بائٹیں_فی_pixel)
{
   صحیح صف;
   size_t بائٹیں_فی_صف = (size_t)w * بائٹیں_فی_pixel;
   stbi_uc عارضی[2048];
   stbi_uc *بائٹیں = (stbi_uc *)image;

   برائے (صف = 0; صف < (h>>1); صف++) {
      stbi_uc *row0 = بائٹیں + صف*بائٹیں_فی_صف;
      stbi_uc *row1 = بائٹیں + (h - صف - 1)*بائٹیں_فی_صف;
      // swap row0 with row1
      size_t بائٹیں_بایاں = بائٹیں_فی_صف;
      جب (بائٹیں_بایاں) {
         size_t بائٹیں_copy = (بائٹیں_بایاں < حجم(عارضی)) ? بائٹیں_بایاں : حجم(عارضی);
         حافظہ_نقل(عارضی, row0, بائٹیں_copy);
         حافظہ_نقل(row0, row1, بائٹیں_copy);
         حافظہ_نقل(row1, عارضی, بائٹیں_copy);
         row0 += بائٹیں_copy;
         row1 += بائٹیں_copy;
         بائٹیں_بایاں -= بائٹیں_copy;
      }
   }
}

ساکن خالی stbi__vertical_flip_slices(خالی *image, صحیح w, صحیح h, صحیح z, صحیح بائٹیں_فی_pixel)
{
   صحیح قاش;
   صحیح قاش_سائز = w * h * بائٹیں_فی_pixel;

   stbi_uc *بائٹیں = (stbi_uc *)image;
   برائے (قاش = 0; قاش < z; ++قاش) {
      stbi__vertical_flip(بائٹیں, w, h, بائٹیں_فی_pixel); 
      بائٹیں += قاش_سائز; 
   }
}

ساکن غیرعلامتی حرف *stbi_بھرو_and_postprocess_8bit(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi_نتیجہ_info ri;
   خالی *نتیجہ = stbi_بھرو_main(s, x, y, comp, req_comp, &ri, 8);

   اگر (نتیجہ == تہی)
      واپس تہی;

   اگر (ri.بٹ_فی_channel != 8) {
      STBI_ASSERT(ri.بٹ_فی_channel == 16);
      نتیجہ = stbi_convert_16_مقصد_8((stbi__uint16 *) نتیجہ, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.بٹ_فی_channel = 8;
   }

   // @TODO: move stbi__convert_format to here

   اگر (stbi_vertically_flip_on_بھرو) {
      صحیح channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(نتیجہ, *x, *y, channels * حجم(stbi_uc));
   }

   واپس (غیرعلامتی حرف *) نتیجہ;
}

ساکن stbi__uint16 *stbi_بھرو_and_postprocess_16bit(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi_نتیجہ_info ri;
   خالی *نتیجہ = stbi_بھرو_main(s, x, y, comp, req_comp, &ri, 16);

   اگر (نتیجہ == تہی)
      واپس تہی;

   اگر (ri.بٹ_فی_channel != 16) {
      STBI_ASSERT(ri.بٹ_فی_channel == 8);
      نتیجہ = stbi_convert_8_مقصد_16((stbi_uc *) نتیجہ, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.بٹ_فی_channel = 16;
   }

   // @TODO: move stbi__convert_format16 to here
   // @TODO: special case RGB-to-Y (and RGBA-to-YA) for 8-bit-to-16-bit case to keep more precision

   اگر (stbi_vertically_flip_on_بھرو) {
      صحیح channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(نتیجہ, *x, *y, channels * حجم(stbi__uint16));
   }

   واپس (stbi__uint16 *) نتیجہ;
}

#اگر !defined(STBI_NO_HDR) || !defined(STBI_NO_خطی)
ساکن خالی stbi__float_postprocess(اعشاری *نتیجہ, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   اگر (stbi_vertically_flip_on_بھرو && نتیجہ != تہی) {
      صحیح channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(نتیجہ, *x, *y, channels * حجم(اعشاری));
   }
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO

ساکن FILE *stbi__fopen(حرف ثابت *filename, حرف ثابت *منوال)
{
   FILE *f;
#اگر defined(_MSC_VER) && _MSC_VER >= 1400
   اگر (0 != fopen_s(&f, filename, منوال))
      f=0;
#ورنہ
   f = fopen(filename, منوال);
#اگر_ختم
   واپس f;
}


STBIDEF stbi_uc *stbi_بھرو(حرف ثابت *filename, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   غیرعلامتی حرف *نتیجہ;
   اگر (!f) واپس stbi__errpuc("can't fopen", "Unable to open file");
   نتیجہ = stbi_بھرو_from_file(f,x,y,comp,req_comp);
   fclose(f);
   واپس نتیجہ;
}

STBIDEF stbi_uc *stbi_بھرو_from_file(FILE *f, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   غیرعلامتی حرف *نتیجہ;
   stbi__context s;
   stbi_آغاز_file(&s,f);
   نتیجہ = stbi_بھرو_and_postprocess_8bit(&s,x,y,comp,req_comp);
   اگر (نتیجہ) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (صحیح) (s.img_بفر_انتہا - s.img_بفر), SEEK_CUR);
   }
   واپس نتیجہ;
}

STBIDEF stbi__uint16 *stbi_بھرو_from_file_16(FILE *f, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi__uint16 *نتیجہ;
   stbi__context s;
   stbi_آغاز_file(&s,f);
   نتیجہ = stbi_بھرو_and_postprocess_16bit(&s,x,y,comp,req_comp);
   اگر (نتیجہ) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (صحیح) (s.img_بفر_انتہا - s.img_بفر), SEEK_CUR);
   }
   واپس نتیجہ;
}

STBIDEF stbi_us *stbi_بھرو_16(حرف ثابت *filename, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   stbi__uint16 *نتیجہ;
   اگر (!f) واپس (stbi_us *) stbi__errpuc("can't fopen", "Unable to open file");
   نتیجہ = stbi_بھرو_from_file_16(f,x,y,comp,req_comp);
   fclose(f);
   واپس نتیجہ;
}


#اگر_ختم //!STBI_NO_STDIO

STBIDEF stbi_us *stbi_بھرو_16_from_حافظہ(stbi_uc ثابت *بفر, صحیح len, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels)
{
   stbi__context s;
   stbi_آغاز_mem(&s,بفر,len);
   واپس stbi_بھرو_and_postprocess_16bit(&s,x,y,channels_اندر_file,desired_channels);
}

STBIDEF stbi_us *stbi_بھرو_16_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user, صحیح *x, صحیح *y, صحیح *channels_اندر_file, صحیح desired_channels)
{
   stbi__context s;
   stbi_آغاز_callbacks(&s, (stbi_io_callbacks *)clbk, user);
   واپس stbi_بھرو_and_postprocess_16bit(&s,x,y,channels_اندر_file,desired_channels);
}

STBIDEF stbi_uc *stbi_بھرو_from_حافظہ(stbi_uc ثابت *بفر, صحیح len, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi__context s;
   stbi_آغاز_mem(&s,بفر,len);
   واپس stbi_بھرو_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

STBIDEF stbi_uc *stbi_بھرو_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi__context s;
   stbi_آغاز_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   واپس stbi_بھرو_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

#اگر_غیر_تعریف_شدہ STBI_NO_GIF
STBIDEF stbi_uc *stbi_بھرو_gif_from_حافظہ(stbi_uc ثابت *بفر, صحیح len, صحیح **delays, صحیح *x, صحیح *y, صحیح *z, صحیح *comp, صحیح req_comp)
{
   غیرعلامتی حرف *نتیجہ;
   stbi__context s; 
   stbi_آغاز_mem(&s,بفر,len); 
   
   نتیجہ = (غیرعلامتی حرف*) stbi_بھرو_gif_main(&s, delays, x, y, z, comp, req_comp);
   اگر (stbi_vertically_flip_on_بھرو) {
      stbi__vertical_flip_slices( نتیجہ, *x, *y, *z, *comp ); 
   }

   واپس نتیجہ; 
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_خطی
ساکن اعشاری *stbi__loadf_main(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   غیرعلامتی حرف *ڈیٹا;
   #اگر_غیر_تعریف_شدہ STBI_NO_HDR
   اگر (stbi__hdr_test(s)) {
      stbi_نتیجہ_info ri;
      اعشاری *hdr_ڈیٹا = stbi_hdr_بھرو(s,x,y,comp,req_comp, &ri);
      اگر (hdr_ڈیٹا)
         stbi__float_postprocess(hdr_ڈیٹا,x,y,comp,req_comp);
      واپس hdr_ڈیٹا;
   }
   #اگر_ختم
   ڈیٹا = stbi_بھرو_and_postprocess_8bit(s, x, y, comp, req_comp);
   اگر (ڈیٹا)
      واپس stbi_ldr_مقصد_hdr(ڈیٹا, *x, *y, req_comp ? req_comp : *comp);
   واپس stbi__errpf("unknown image type", "Image not of any known type, or corrupt");
}

STBIDEF اعشاری *stbi_loadf_from_حافظہ(stbi_uc ثابت *بفر, صحیح len, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi__context s;
   stbi_آغاز_mem(&s,بفر,len);
   واپس stbi__loadf_main(&s,x,y,comp,req_comp);
}

STBIDEF اعشاری *stbi_loadf_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi__context s;
   stbi_آغاز_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   واپس stbi__loadf_main(&s,x,y,comp,req_comp);
}

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
STBIDEF اعشاری *stbi_loadf(حرف ثابت *filename, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   اعشاری *نتیجہ;
   FILE *f = stbi__fopen(filename, "rb");
   اگر (!f) واپس stbi__errpf("can't fopen", "Unable to open file");
   نتیجہ = stbi_loadf_from_file(f,x,y,comp,req_comp);
   fclose(f);
   واپس نتیجہ;
}

STBIDEF اعشاری *stbi_loadf_from_file(FILE *f, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp)
{
   stbi__context s;
   stbi_آغاز_file(&s,f);
   واپس stbi__loadf_main(&s,x,y,comp,req_comp);
}
#اگر_ختم // !STBI_NO_STDIO

#اگر_ختم // !STBI_NO_LINEAR

// these is-hdr-or-not is defined independent of whether STBI_NO_LINEAR is
// defined, for API simplicity; if STBI_NO_LINEAR is defined, it always
// reports false!

STBIDEF صحیح stbi_is_hdr_from_حافظہ(stbi_uc ثابت *بفر, صحیح len)
{
   #اگر_غیر_تعریف_شدہ STBI_NO_HDR
   stbi__context s;
   stbi_آغاز_mem(&s,بفر,len);
   واپس stbi__hdr_test(&s);
   #ورنہ
   STBI_NOTUSED(بفر);
   STBI_NOTUSED(len);
   واپس 0;
   #اگر_ختم
}

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
STBIDEF صحیح      stbi_is_hdr          (حرف ثابت *filename)
{
   FILE *f = stbi__fopen(filename, "rb");
   صحیح نتیجہ=0;
   اگر (f) {
      نتیجہ = stbi_is_hdr_from_file(f);
      fclose(f);
   }
   واپس نتیجہ;
}

STBIDEF صحیح stbi_is_hdr_from_file(FILE *f)
{
   #اگر_غیر_تعریف_شدہ STBI_NO_HDR
   طویل pos = ftell(f);
   صحیح res;
   stbi__context s;
   stbi_آغاز_file(&s,f);
   res = stbi__hdr_test(&s);
   fseek(f, pos, SEEK_SET);
   واپس res;
   #ورنہ
   STBI_NOTUSED(f);
   واپس 0;
   #اگر_ختم
}
#اگر_ختم // !STBI_NO_STDIO

STBIDEF صحیح      stbi_is_hdr_from_callbacks(stbi_io_callbacks ثابت *clbk, خالی *user)
{
   #اگر_غیر_تعریف_شدہ STBI_NO_HDR
   stbi__context s;
   stbi_آغاز_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   واپس stbi__hdr_test(&s);
   #ورنہ
   STBI_NOTUSED(clbk);
   STBI_NOTUSED(user);
   واپس 0;
   #اگر_ختم
}

#اگر_غیر_تعریف_شدہ STBI_NO_خطی
ساکن اعشاری stbi__l2h_gamma=2.2f, stbi__l2h_scale=1.0f;

STBIDEF خالی   stbi_ldr_مقصد_hdr_gamma(اعشاری gamma) { stbi__l2h_gamma = gamma; }
STBIDEF خالی   stbi_ldr_مقصد_hdr_scale(اعشاری scale) { stbi__l2h_scale = scale; }
#اگر_ختم

ساکن اعشاری stbi__h2l_gamma_i=1.0f/2.2f, stbi__h2l_scale_i=1.0f;

STBIDEF خالی   stbi_hdr_مقصد_ldr_gamma(اعشاری gamma) { stbi__h2l_gamma_i = 1/gamma; }
STBIDEF خالی   stbi_hdr_مقصد_ldr_scale(اعشاری scale) { stbi__h2l_scale_i = 1/scale; }


//////////////////////////////////////////////////////////////////////////////
//
// Common code used by all image loaders
//

شمار
{
   STBI_جائزہ_بھرو=0,
   STBI_جائزہ_type,
   STBI_جائزہ_سرخی
};

ساکن خالی stbi_refill_بفر(stbi__context *s)
{
   صحیح n = (s->io.اقرأ)(s->io_user_ڈیٹا,(حرف*)s->بفر_آغاز,s->buflen);
   اگر (n == 0) {
      // at end of file, treat same as if from memory, but need to handle case
      // where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file
      s->read_from_callbacks = 0;
      s->img_بفر = s->بفر_آغاز;
      s->img_بفر_انتہا = s->بفر_آغاز+1;
      *s->img_بفر = 0;
   } ورنہ {
      s->img_بفر = s->بفر_آغاز;
      s->img_بفر_انتہا = s->بفر_آغاز + n;
   }
}

stbi_inline ساکن stbi_uc stbi__get8(stbi__context *s)
{
   اگر (s->img_بفر < s->img_بفر_انتہا)
      واپس *s->img_بفر++;
   اگر (s->read_from_callbacks) {
      stbi_refill_بفر(s);
      واپس *s->img_بفر++;
   }
   واپس 0;
}

stbi_inline ساکن صحیح stbi__at_eof(stbi__context *s)
{
   اگر (s->io.اقرأ) {
      اگر (!(s->io.eof)(s->io_user_ڈیٹا)) واپس 0;
      // if feof() is true, check if buffer = end
      // special case: we've only got the special 0 character at the end
      اگر (s->read_from_callbacks == 0) واپس 1;
   }

   واپس s->img_بفر >= s->img_بفر_انتہا;
}

ساکن خالی stbi__skip(stbi__context *s, صحیح n)
{
   اگر (n < 0) {
      s->img_بفر = s->img_بفر_انتہا;
      واپس;
   }
   اگر (s->io.اقرأ) {
      صحیح blen = (صحیح) (s->img_بفر_انتہا - s->img_بفر);
      اگر (blen < n) {
         s->img_بفر = s->img_بفر_انتہا;
         (s->io.skip)(s->io_user_ڈیٹا, n - blen);
         واپس;
      }
   }
   s->img_بفر += n;
}

ساکن صحیح stbi__getn(stbi__context *s, stbi_uc *بفر, صحیح n)
{
   اگر (s->io.اقرأ) {
      صحیح blen = (صحیح) (s->img_بفر_انتہا - s->img_بفر);
      اگر (blen < n) {
         صحیح res, گنتی;

         حافظہ_نقل(بفر, s->img_بفر, blen);

         گنتی = (s->io.اقرأ)(s->io_user_ڈیٹا, (حرف*) بفر + blen, n - blen);
         res = (گنتی == (n-blen));
         s->img_بفر = s->img_بفر_انتہا;
         واپس res;
      }
   }

   اگر (s->img_بفر+n <= s->img_بفر_انتہا) {
      حافظہ_نقل(بفر, s->img_بفر, n);
      s->img_بفر += n;
      واپس 1;
   } ورنہ
      واپس 0;
}

ساکن صحیح stbi__get16be(stbi__context *s)
{
   صحیح z = stbi__get8(s);
   واپس (z << 8) + stbi__get8(s);
}

ساکن stbi__uint32 stbi__get32be(stbi__context *s)
{
   stbi__uint32 z = stbi__get16be(s);
   واپس (z << 16) + stbi__get16be(s);
}

#اگر defined(STBI_NO_BMP) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF)
// nothing
#ورنہ
ساکن صحیح stbi__get16le(stbi__context *s)
{
   صحیح z = stbi__get8(s);
   واپس z + (stbi__get8(s) << 8);
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_BMP
ساکن stbi__uint32 stbi__get32le(stbi__context *s)
{
   stbi__uint32 z = stbi__get16le(s);
   واپس z + (stbi__get16le(s) << 16);
}
#اگر_ختم

#تعریف_کرو STBI__BYTECAST(x)  ((stbi_uc) ((x) & 255))  // truncate int to byte without warnings


//////////////////////////////////////////////////////////////////////////////
//
//  generic converter from built-in img_n to req_comp
//    individual types do this automatically as much as possible (e.g. jpeg
//    does all cases internally since it needs to colorspace convert anyway,
//    and it never has alpha, so very few cases ). png can automatically
//    interleave an alpha=255 channel, but falls back to this for other cases
//
//  assume data buffer is malloced, so malloc a new one and free that one
//  only failure mode is malloc failing

ساکن stbi_uc stbi__compute_y(صحیح r, صحیح g, صحیح b)
{
   واپس (stbi_uc) (((r*77) + (g*150) +  (29*b)) >> 8);
}

ساکن غیرعلامتی حرف *stbi__convert_format(غیرعلامتی حرف *ڈیٹا, صحیح img_n, صحیح req_comp, غیرعلامتی صحیح x, غیرعلامتی صحیح y)
{
   صحیح i,j;
   غیرعلامتی حرف *good;

   اگر (req_comp == img_n) واپس ڈیٹا;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (غیرعلامتی حرف *) stbi__malloc_mad3(req_comp, x, y, 0);
   اگر (good == تہی) {
      STBI_FREE(ڈیٹا);
      واپس stbi__errpuc("outofmem", "Out of memory");
   }

   برائے (j=0; j < (صحیح) y; ++j) {
      غیرعلامتی حرف *src  = ڈیٹا + j * x * img_n   ;
      غیرعلامتی حرف *dest = good + j * x * req_comp;

      #تعریف_کرو STBI__COMBO(a,b)  ((a)*8+(b))
      #تعریف_کرو STBI__CASE(a,b)   حالت STBI__COMBO(a,b): برائے(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      انتخاب (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=255;                                     } توڑو;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } توڑو;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=255;                     } توڑو;
         STBI__CASE(2,1) { dest[0]=src[0];                                                  } توڑو;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } توڑو;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                  } توڑو;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255;        } توڑو;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } توڑو;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = 255;    } توڑو;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } توڑو;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = src[3]; } توڑو;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                    } توڑو;
         طےشدہ: STBI_ASSERT(0);
      }
      #تعریف_منسوخ STBI__CASE
   }

   STBI_FREE(ڈیٹا);
   واپس good;
}

ساکن stbi__uint16 stbi__compute_y_16(صحیح r, صحیح g, صحیح b)
{
   واپس (stbi__uint16) (((r*77) + (g*150) +  (29*b)) >> 8);
}

ساکن stbi__uint16 *stbi__convert_format16(stbi__uint16 *ڈیٹا, صحیح img_n, صحیح req_comp, غیرعلامتی صحیح x, غیرعلامتی صحیح y)
{
   صحیح i,j;
   stbi__uint16 *good;

   اگر (req_comp == img_n) واپس ڈیٹا;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (stbi__uint16 *) stbi__malloc(req_comp * x * y * 2);
   اگر (good == تہی) {
      STBI_FREE(ڈیٹا);
      واپس (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");
   }

   برائے (j=0; j < (صحیح) y; ++j) {
      stbi__uint16 *src  = ڈیٹا + j * x * img_n   ;
      stbi__uint16 *dest = good + j * x * req_comp;

      #تعریف_کرو STBI__COMBO(a,b)  ((a)*8+(b))
      #تعریف_کرو STBI__CASE(a,b)   حالت STBI__COMBO(a,b): برائے(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      انتخاب (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=0xffff;                                     } توڑو;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } توڑو;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=0xffff;                     } توڑو;
         STBI__CASE(2,1) { dest[0]=src[0];                                                     } توڑو;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } توڑو;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                     } توڑو;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=0xffff;        } توڑو;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } توڑو;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = 0xffff; } توڑو;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } توڑو;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = src[3]; } توڑو;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                       } توڑو;
         طےشدہ: STBI_ASSERT(0);
      }
      #تعریف_منسوخ STBI__CASE
   }

   STBI_FREE(ڈیٹا);
   واپس good;
}

#اگر_غیر_تعریف_شدہ STBI_NO_خطی
ساکن اعشاری   *stbi_ldr_مقصد_hdr(stbi_uc *ڈیٹا, صحیح x, صحیح y, صحیح comp)
{
   صحیح i,k,n;
   اعشاری *output;
   اگر (!ڈیٹا) واپس تہی;
   output = (اعشاری *) stbi__malloc_mad4(x, y, comp, حجم(اعشاری), 0);
   اگر (output == تہی) { STBI_FREE(ڈیٹا); واپس stbi__errpf("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   اگر (comp & 1) n = comp; ورنہ n = comp-1;
   برائے (i=0; i < x*y; ++i) {
      برائے (k=0; k < n; ++k) {
         output[i*comp + k] = (اعشاری) (قوت(ڈیٹا[i*comp+k]/255.0f, stbi__l2h_gamma) * stbi__l2h_scale);
      }
      اگر (k < comp) output[i*comp + k] = ڈیٹا[i*comp+k]/255.0f;
   }
   STBI_FREE(ڈیٹا);
   واپس output;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_HDR
#تعریف_کرو stbi__float2int(x)   ((صحیح) (x))
ساکن stbi_uc *stbi_hdr_مقصد_ldr(اعشاری   *ڈیٹا, صحیح x, صحیح y, صحیح comp)
{
   صحیح i,k,n;
   stbi_uc *output;
   اگر (!ڈیٹا) واپس تہی;
   output = (stbi_uc *) stbi__malloc_mad3(x, y, comp, 0);
   اگر (output == تہی) { STBI_FREE(ڈیٹا); واپس stbi__errpuc("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   اگر (comp & 1) n = comp; ورنہ n = comp-1;
   برائے (i=0; i < x*y; ++i) {
      برائے (k=0; k < n; ++k) {
         اعشاری z = (اعشاری) قوت(ڈیٹا[i*comp+k]*stbi__h2l_scale_i, stbi__h2l_gamma_i) * 255 + 0.5f;
         اگر (z < 0) z = 0;
         اگر (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
      اگر (k < comp) {
         اعشاری z = ڈیٹا[i*comp+k] * 255 + 0.5f;
         اگر (z < 0) z = 0;
         اگر (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
   }
   STBI_FREE(ڈیٹا);
   واپس output;
}
#اگر_ختم

//////////////////////////////////////////////////////////////////////////////
//
//  "baseline" JPEG/JFIF decoder
//
//    simple implementation
//      - doesn't support delayed output of y-dimension
//      - simple interface (only one output format: 8-bit interleaved RGB)
//      - doesn't try to recover corrupt jpegs
//      - doesn't allow partial loading, loading multiple at once
//      - still fast on x86 (copying globals into locals doesn't help x86)
//      - allocates lots of intermediate memory (full size of all components)
//        - non-interleaved case requires this anyway
//        - allows good upsampling (see next)
//    high-quality
//      - upsampled channels are bilinearly interpolated, even across blocks
//      - quality integer IDCT derived from IJG's 'slow'
//    performance
//      - fast huffman; reasonable integer IDCT
//      - some SIMD kernels for common paths on targets with SSE2/NEON
//      - uses a lot of intermediate memory, could cache poorly

#اگر_غیر_تعریف_شدہ STBI_NO_JPEG

// huffman decoding acceleration
#تعریف_کرو FAST_بٹ   9  // larger handles more cases; smaller stomps less cache

قسمتعریف ساخت
{
   stbi_uc  fast[1 << FAST_بٹ];
   // weirdly, repacking this into AoS is a 10% speed loss, instead of a win
   stbi__uint16 رمز[256];
   stbi_uc  اقدار[256];
   stbi_uc  سائز[257];
   غیرعلامتی صحیح maxcode[18];
   صحیح    delta[17];   // old 'firstsymbol' - old 'firstcode'
} stbi__huffman;

قسمتعریف ساخت
{
   stbi__context *s;
   stbi__huffman huff_dc[4];
   stbi__huffman huff_ac[4];
   stbi__uint16 dequant[4][64];
   stbi__int16 fast_ac[4][1 << FAST_بٹ];

// sizes for components, interleaved MCUs
   صحیح img_h_max, img_v_max;
   صحیح img_mcu_x, img_mcu_y;
   صحیح img_mcu_w, img_mcu_h;

// definition of jpeg image component
   ساخت
   {
      صحیح id;
      صحیح h,v;
      صحیح tq;
      صحیح hd,ha;
      صحیح dc_pred;

      صحیح x,y,w2,h2;
      stbi_uc *ڈیٹا;
      خالی *raw_ڈیٹا, *raw_coeff;
      stbi_uc *linebuf;
      مختصر   *coeff;   // progressive only
      صحیح      coeff_w, coeff_h; // number of 8x8 coefficient blocks
   } img_comp[4];

   stbi__uint32   رمز_بفر; // jpeg entropy-coded buffer
   صحیح            رمز_بٹ;   // number of valid bits
   غیرعلامتی حرف  marker;      // marker seen while filling entropy buffer
   صحیح            nomore;      // flag if we saw a marker so must stop

   صحیح            progressive;
   صحیح            spec_آغاز;
   صحیح            spec_انتہا;
   صحیح            succ_بلند;
   صحیح            succ_کم;
   صحیح            eob_تسلسل;
   صحیح            jfif;
   صحیح            app14_color_transform; // Adobe APP14 tag
   صحیح            rgb;

   صحیح جائزہ_n, ترتیب[4];
   صحیح restart_interval, todo;

// kernels
   خالی (*idct_بلاک_kernel)(stbi_uc *برآمد, صحیح برآمد_stride, مختصر ڈیٹا[64]);
   خالی (*YCb_Cr_مقصد_RGB_kernel)(stbi_uc *برآمد, ثابت stbi_uc *y, ثابت stbi_uc *pcb, ثابت stbi_uc *pcr, صحیح گنتی, صحیح قدم);
   stbi_uc *(*resample_صف_hv_2_kernel)(stbi_uc *برآمد, stbi_uc *اندر_near, stbi_uc *اندر_far, صحیح w, صحیح hs);
} stbi__jpeg;

ساکن صحیح stbi__build_huffman(stbi__huffman *h, صحیح *گنتی)
{
   صحیح i,j,k=0;
   غیرعلامتی صحیح رمز;
   // build size list for each symbol (from JPEG spec)
   برائے (i=0; i < 16; ++i)
      برائے (j=0; j < گنتی[i]; ++j)
         h->سائز[k++] = (stbi_uc) (i+1);
   h->سائز[k] = 0;

   // compute actual symbols (from jpeg spec)
   رمز = 0;
   k = 0;
   برائے(j=1; j <= 16; ++j) {
      // compute delta to add to code to compute symbol id
      h->delta[j] = k - رمز;
      اگر (h->سائز[k] == j) {
         جب (h->سائز[k] == j)
            h->رمز[k++] = (stbi__uint16) (رمز++);
         اگر (رمز-1 >= (1u << j)) واپس stbi__err("bad code lengths","Corrupt JPEG");
      }
      // compute largest code + 1 for this size, preshifted as needed later
      h->maxcode[j] = رمز << (16-j);
      رمز <<= 1;
   }
   h->maxcode[j] = 0xffffffff;

   // build non-spec acceleration table; 255 is flag for not-accelerated
   memset(h->fast, 255, 1 << FAST_بٹ);
   برائے (i=0; i < k; ++i) {
      صحیح s = h->سائز[i];
      اگر (s <= FAST_بٹ) {
         صحیح c = h->رمز[i] << (FAST_بٹ-s);
         صحیح m = 1 << (FAST_بٹ-s);
         برائے (j=0; j < m; ++j) {
            h->fast[c+j] = (stbi_uc) i;
         }
      }
   }
   واپس 1;
}

// build a table that decodes both magnitude and value of small ACs in
// one go.
ساکن خالی stbi__build_fast_ac(stbi__int16 *fast_ac, stbi__huffman *h)
{
   صحیح i;
   برائے (i=0; i < (1 << FAST_بٹ); ++i) {
      stbi_uc fast = h->fast[i];
      fast_ac[i] = 0;
      اگر (fast < 255) {
         صحیح rs = h->اقدار[fast];
         صحیح تسلسل = (rs >> 4) & 15;
         صحیح magbits = rs & 15;
         صحیح len = h->سائز[fast];

         اگر (magbits && len + magbits <= FAST_بٹ) {
            // magnitude code followed by receive_extend code
            صحیح k = ((i << len) & ((1 << FAST_بٹ) - 1)) >> (FAST_بٹ - magbits);
            صحیح m = 1 << (magbits - 1);
            اگر (k < m) k += (~0U << magbits) + 1;
            // if the result is small enough, we can fit it in fast_ac table
            اگر (k >= -128 && k <= 127)
               fast_ac[i] = (stbi__int16) ((k * 256) + (تسلسل * 16) + (len + magbits));
         }
      }
   }
}

ساکن خالی stbi_grow_بفر_unsafe(stbi__jpeg *j)
{
   کرو {
      غیرعلامتی صحیح b = j->nomore ? 0 : stbi__get8(j->s);
      اگر (b == 0xff) {
         صحیح c = stbi__get8(j->s);
         جب (c == 0xff) c = stbi__get8(j->s); // consume fill bytes
         اگر (c != 0) {
            j->marker = (غیرعلامتی حرف) c;
            j->nomore = 1;
            واپس;
         }
      }
      j->رمز_بفر |= b << (24 - j->رمز_بٹ);
      j->رمز_بٹ += 8;
   } جب (j->رمز_بٹ <= 24);
}

// (1 << n) - 1
ساکن ثابت stbi__uint32 stbi__bmask[17]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};

// decode a jpeg huffman value from the bitstream
stbi_inline ساکن صحیح stbi__jpeg_huff_decode(stbi__jpeg *j, stbi__huffman *h)
{
   غیرعلامتی صحیح عارضی;
   صحیح c,k;

   اگر (j->رمز_بٹ < 16) stbi_grow_بفر_unsafe(j);

   // look at the top FAST_BITS and determine what symbol ID it is,
   // if the code is <= FAST_BITS
   c = (j->رمز_بفر >> (32 - FAST_بٹ)) & ((1 << FAST_بٹ)-1);
   k = h->fast[c];
   اگر (k < 255) {
      صحیح s = h->سائز[k];
      اگر (s > j->رمز_بٹ)
         واپس -1;
      j->رمز_بفر <<= s;
      j->رمز_بٹ -= s;
      واپس h->اقدار[k];
   }

   // naive test is to shift the code_buffer down so k bits are
   // valid, then test against maxcode. To speed this up, we've
   // preshifted maxcode left so that it has (16-k) 0s at the
   // end; in other words, regardless of the number of bits, it
   // wants to be compared against something shifted to have 16;
   // that way we don't need to shift inside the loop.
   عارضی = j->رمز_بفر >> 16;
   برائے (k=FAST_بٹ+1 ; ; ++k)
      اگر (عارضی < h->maxcode[k])
         توڑو;
   اگر (k == 17) {
      // error! code not found
      j->رمز_بٹ -= 16;
      واپس -1;
   }

   اگر (k > j->رمز_بٹ)
      واپس -1;

   // convert the huffman code to the symbol id
   c = ((j->رمز_بفر >> (32 - k)) & stbi__bmask[k]) + h->delta[k];
   STBI_ASSERT((((j->رمز_بفر) >> (32 - h->سائز[c])) & stbi__bmask[h->سائز[c]]) == h->رمز[c]);

   // convert the id to a symbol
   j->رمز_بٹ -= k;
   j->رمز_بفر <<= k;
   واپس h->اقدار[c];
}

// bias[n] = (-1<<n) + 1
ساکن ثابت صحیح stbi__jbias[16] = {0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767};

// combined JPEG 'receive' and JPEG 'extend', since baseline
// always extends everything it receives.
stbi_inline ساکن صحیح stbi__extend_receive(stbi__jpeg *j, صحیح n)
{
   غیرعلامتی صحیح k;
   صحیح sgn;
   اگر (j->رمز_بٹ < n) stbi_grow_بفر_unsafe(j);

   sgn = (stbi__int32)j->رمز_بفر >> 31; // sign bit is always in MSB
   k = stbi_lrot(j->رمز_بفر, n);
   STBI_ASSERT(n >= 0 && n < (صحیح) (حجم(stbi__bmask)/حجم(*stbi__bmask)));
   j->رمز_بفر = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->رمز_بٹ -= n;
   واپس k + (stbi__jbias[n] & ~sgn);
}

// get some unsigned bits
stbi_inline ساکن صحیح stbi_jpeg_get_بٹ(stbi__jpeg *j, صحیح n)
{
   غیرعلامتی صحیح k;
   اگر (j->رمز_بٹ < n) stbi_grow_بفر_unsafe(j);
   k = stbi_lrot(j->رمز_بفر, n);
   j->رمز_بفر = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->رمز_بٹ -= n;
   واپس k;
}

stbi_inline ساکن صحیح stbi_jpeg_get_ثنائی_ہندسہ(stbi__jpeg *j)
{
   غیرعلامتی صحیح k;
   اگر (j->رمز_بٹ < 1) stbi_grow_بفر_unsafe(j);
   k = j->رمز_بفر;
   j->رمز_بفر <<= 1;
   --j->رمز_بٹ;
   واپس k & 0x80000000;
}

// given a value that's at position X in the zigzag stream,
// where does it appear in the 8x8 matrix coded as row-major?
ساکن ثابت stbi_uc stbi__jpeg_dezigzag[64+15] =
{
    0,  1,  8, 16,  9,  2,  3, 10,
   17, 24, 32, 25, 18, 11,  4,  5,
   12, 19, 26, 33, 40, 48, 41, 34,
   27, 20, 13,  6,  7, 14, 21, 28,
   35, 42, 49, 56, 57, 50, 43, 36,
   29, 22, 15, 23, 30, 37, 44, 51,
   58, 59, 52, 45, 38, 31, 39, 46,
   53, 60, 61, 54, 47, 55, 62, 63,
   // let corrupt input sample past end
   63, 63, 63, 63, 63, 63, 63, 63,
   63, 63, 63, 63, 63, 63, 63
};

// decode one 64-entry block--
ساکن صحیح stbi_jpeg_decode_بلاک(stbi__jpeg *j, مختصر ڈیٹا[64], stbi__huffman *hdc, stbi__huffman *hac, stbi__int16 *fac, صحیح b, stbi__uint16 *dequant)
{
   صحیح diff,dc,k;
   صحیح t;

   اگر (j->رمز_بٹ < 16) stbi_grow_بفر_unsafe(j);
   t = stbi__jpeg_huff_decode(j, hdc);
   اگر (t < 0) واپس stbi__err("bad huffman code","Corrupt JPEG");

   // 0 all the ac values now so we can do it 32-bits at a time
   memset(ڈیٹا,0,64*حجم(ڈیٹا[0]));

   diff = t ? stbi__extend_receive(j, t) : 0;
   dc = j->img_comp[b].dc_pred + diff;
   j->img_comp[b].dc_pred = dc;
   ڈیٹا[0] = (مختصر) (dc * dequant[0]);

   // decode AC components, see JPEG spec
   k = 1;
   کرو {
      غیرعلامتی صحیح zig;
      صحیح c,r,s;
      اگر (j->رمز_بٹ < 16) stbi_grow_بفر_unsafe(j);
      c = (j->رمز_بفر >> (32 - FAST_بٹ)) & ((1 << FAST_بٹ)-1);
      r = fac[c];
      اگر (r) { // fast-AC path
         k += (r >> 4) & 15; //تسلسل
         s = r & 15; // combined length
         j->رمز_بفر <<= s;
         j->رمز_بٹ -= s;
         // decode into unzigzag'd location
         zig = stbi__jpeg_dezigzag[k++];
         ڈیٹا[zig] = (مختصر) ((r >> 8) * dequant[zig]);
      } ورنہ {
         صحیح rs = stbi__jpeg_huff_decode(j, hac);
         اگر (rs < 0) واپس stbi__err("bad huffman code","Corrupt JPEG");
         s = rs & 15;
         r = rs >> 4;
         اگر (s == 0) {
            اگر (rs != 0xf0) توڑو; // end block
            k += 16;
         } ورنہ {
            k += r;
            // decode into unzigzag'd location
            zig = stbi__jpeg_dezigzag[k++];
            ڈیٹا[zig] = (مختصر) (stbi__extend_receive(j,s) * dequant[zig]);
         }
      }
   } جب (k < 64);
   واپس 1;
}

ساکن صحیح stbi_jpeg_decode_بلاک_prog_dc(stbi__jpeg *j, مختصر ڈیٹا[64], stbi__huffman *hdc, صحیح b)
{
   صحیح diff,dc;
   صحیح t;
   اگر (j->spec_انتہا != 0) واپس stbi__err("can't merge dc and ac", "Corrupt JPEG");

   اگر (j->رمز_بٹ < 16) stbi_grow_بفر_unsafe(j);

   اگر (j->succ_بلند == 0) {
      // first scan for DC coefficient, must be first
      memset(ڈیٹا,0,64*حجم(ڈیٹا[0])); // 0 all the ac values now
      t = stbi__jpeg_huff_decode(j, hdc);
      diff = t ? stbi__extend_receive(j, t) : 0;

      dc = j->img_comp[b].dc_pred + diff;
      j->img_comp[b].dc_pred = dc;
      ڈیٹا[0] = (مختصر) (dc << j->succ_کم);
   } ورنہ {
      // refinement scan for DC coefficient
      اگر (stbi_jpeg_get_ثنائی_ہندسہ(j))
         ڈیٹا[0] += (مختصر) (1 << j->succ_کم);
   }
   واپس 1;
}

// @OPTIMIZE: store non-zigzagged during the decode passes,
// and only de-zigzag when dequantizing
ساکن صحیح stbi_jpeg_decode_بلاک_prog_ac(stbi__jpeg *j, مختصر ڈیٹا[64], stbi__huffman *hac, stbi__int16 *fac)
{
   صحیح k;
   اگر (j->spec_آغاز == 0) واپس stbi__err("can't merge dc and ac", "Corrupt JPEG");

   اگر (j->succ_بلند == 0) {
      صحیح سرکاؤ = j->succ_کم;

      اگر (j->eob_تسلسل) {
         --j->eob_تسلسل;
         واپس 1;
      }

      k = j->spec_آغاز;
      کرو {
         غیرعلامتی صحیح zig;
         صحیح c,r,s;
         اگر (j->رمز_بٹ < 16) stbi_grow_بفر_unsafe(j);
         c = (j->رمز_بفر >> (32 - FAST_بٹ)) & ((1 << FAST_بٹ)-1);
         r = fac[c];
         اگر (r) { // fast-AC path
            k += (r >> 4) & 15; //تسلسل
            s = r & 15; // combined length
            j->رمز_بفر <<= s;
            j->رمز_بٹ -= s;
            zig = stbi__jpeg_dezigzag[k++];
            ڈیٹا[zig] = (مختصر) ((r >> 8) << سرکاؤ);
         } ورنہ {
            صحیح rs = stbi__jpeg_huff_decode(j, hac);
            اگر (rs < 0) واپس stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            اگر (s == 0) {
               اگر (r < 15) {
                  j->eob_تسلسل = (1 << r);
                  اگر (r)
                     j->eob_تسلسل += stbi_jpeg_get_بٹ(j, r);
                  --j->eob_تسلسل;
                  توڑو;
               }
               k += 16;
            } ورنہ {
               k += r;
               zig = stbi__jpeg_dezigzag[k++];
               ڈیٹا[zig] = (مختصر) (stbi__extend_receive(j,s) << سرکاؤ);
            }
         }
      } جب (k <= j->spec_انتہا);
   } ورنہ {
      // refinement scan for these AC coefficients

      مختصر ثنائی_ہندسہ = (مختصر) (1 << j->succ_کم);

      اگر (j->eob_تسلسل) {
         --j->eob_تسلسل;
         برائے (k = j->spec_آغاز; k <= j->spec_انتہا; ++k) {
            مختصر *p = &ڈیٹا[stbi__jpeg_dezigzag[k]];
            اگر (*p != 0)
               اگر (stbi_jpeg_get_ثنائی_ہندسہ(j))
                  اگر ((*p & ثنائی_ہندسہ)==0) {
                     اگر (*p > 0)
                        *p += ثنائی_ہندسہ;
                     ورنہ
                        *p -= ثنائی_ہندسہ;
                  }
         }
      } ورنہ {
         k = j->spec_آغاز;
         کرو {
            صحیح r,s;
            صحیح rs = stbi__jpeg_huff_decode(j, hac); // @OPTIMIZE see if we can use the fast path here, advance-by-r is so slow, eh
            اگر (rs < 0) واپس stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            اگر (s == 0) {
               اگر (r < 15) {
                  j->eob_تسلسل = (1 << r) - 1;
                  اگر (r)
                     j->eob_تسلسل += stbi_jpeg_get_بٹ(j, r);
                  r = 64; // force end of block
               } ورنہ {
                  // r=15 s=0 should write 16 0s, so we just do
                  // a run of 15 0s and then write s (which is 0),
                  // so we don't have to do anything special here
               }
            } ورنہ {
               اگر (s != 1) واپس stbi__err("bad huffman code", "Corrupt JPEG");
               // sign bit
               اگر (stbi_jpeg_get_ثنائی_ہندسہ(j))
                  s = ثنائی_ہندسہ;
               ورنہ
                  s = -ثنائی_ہندسہ;
            }

            // advance by r
            جب (k <= j->spec_انتہا) {
               مختصر *p = &ڈیٹا[stbi__jpeg_dezigzag[k++]];
               اگر (*p != 0) {
                  اگر (stbi_jpeg_get_ثنائی_ہندسہ(j))
                     اگر ((*p & ثنائی_ہندسہ)==0) {
                        اگر (*p > 0)
                           *p += ثنائی_ہندسہ;
                        ورنہ
                           *p -= ثنائی_ہندسہ;
                     }
               } ورنہ {
                  اگر (r == 0) {
                     *p = (مختصر) s;
                     توڑو;
                  }
                  --r;
               }
            }
         } جب (k <= j->spec_انتہا);
      }
   }
   واپس 1;
}

// take a -128..127 value and stbi__clamp it and convert to 0..255
stbi_inline ساکن stbi_uc stbi__clamp(صحیح x)
{
   // trick to use a single test to catch both cases
   اگر ((غیرعلامتی صحیح) x > 255) {
      اگر (x < 0) واپس 0;
      اگر (x > 255) واپس 255;
   }
   واپس (stbi_uc) x;
}

#تعریف_کرو stbi__f2f(x)  ((صحیح) (((x) * 4096 + 0.5)))
#تعریف_کرو stbi__fsh(x)  ((x) * 4096)

// derived from jidctint -- DCT_ISLOW
#تعریف_کرو STBI__IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7) \
   صحیح t0,t1,t2,t3,p1,p2,p3,p4,p5,x0,x1,x2,x3; \
   p2 = s2;                                    \
   p3 = s6;                                    \
   p1 = (p2+p3) * stbi__f2f(0.5411961f);       \
   t2 = p1 + p3*stbi__f2f(-1.847759065f);      \
   t3 = p1 + p2*stbi__f2f( 0.765366865f);      \
   p2 = s0;                                    \
   p3 = s4;                                    \
   t0 = stbi__fsh(p2+p3);                      \
   t1 = stbi__fsh(p2-p3);                      \
   x0 = t0+t3;                                 \
   x3 = t0-t3;                                 \
   x1 = t1+t2;                                 \
   x2 = t1-t2;                                 \
   t0 = s7;                                    \
   t1 = s5;                                    \
   t2 = s3;                                    \
   t3 = s1;                                    \
   p3 = t0+t2;                                 \
   p4 = t1+t3;                                 \
   p1 = t0+t3;                                 \
   p2 = t1+t2;                                 \
   p5 = (p3+p4)*stbi__f2f( 1.175875602f);      \
   t0 = t0*stbi__f2f( 0.298631336f);           \
   t1 = t1*stbi__f2f( 2.053119869f);           \
   t2 = t2*stbi__f2f( 3.072711026f);           \
   t3 = t3*stbi__f2f( 1.501321110f);           \
   p1 = p5 + p1*stbi__f2f(-0.899976223f);      \
   p2 = p5 + p2*stbi__f2f(-2.562915447f);      \
   p3 = p3*stbi__f2f(-1.961570560f);           \
   p4 = p4*stbi__f2f(-0.390180644f);           \
   t3 += p1+p4;                                \
   t2 += p2+p3;                                \
   t1 += p2+p4;                                \
   t0 += p1+p3;

ساکن خالی stbi_idct_بلاک(stbi_uc *برآمد, صحیح برآمد_stride, مختصر ڈیٹا[64])
{
   صحیح i,val[64],*v=val;
   stbi_uc *o;
   مختصر *d = ڈیٹا;

   // columns
   برائے (i=0; i < 8; ++i,++d, ++v) {
      // if all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing
      اگر (d[ 8]==0 && d[16]==0 && d[24]==0 && d[32]==0
           && d[40]==0 && d[48]==0 && d[56]==0) {
         //    no shortcut                 0     seconds
         //    (1|2|3|4|5|6|7)==0          0     seconds
         //    all separate               -0.047 seconds
         //    1 && 2|3 && 4|5 && 6|7:    -0.047 seconds
         صحیح dcterm = d[0]*4;
         v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
      } ورنہ {
         STBI__IDCT_1D(d[ 0],d[ 8],d[16],d[24],d[32],d[40],d[48],d[56])
         // constants scaled things up by 1<<12; let's bring them back
         // down, but keep 2 extra bits of precision
         x0 += 512; x1 += 512; x2 += 512; x3 += 512;
         v[ 0] = (x0+t3) >> 10;
         v[56] = (x0-t3) >> 10;
         v[ 8] = (x1+t2) >> 10;
         v[48] = (x1-t2) >> 10;
         v[16] = (x2+t1) >> 10;
         v[40] = (x2-t1) >> 10;
         v[24] = (x3+t0) >> 10;
         v[32] = (x3-t0) >> 10;
      }
   }

   برائے (i=0, v=val, o=برآمد; i < 8; ++i,v+=8,o+=برآمد_stride) {
      // no fast case since the first 1D IDCT spread components out
      STBI__IDCT_1D(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7])
      // constants scaled things up by 1<<12, plus we had 1<<2 from first
      // loop, plus horizontal and vertical each scale by sqrt(8) so together
      // we've got an extra 1<<3, so 1<<17 total we need to remove.
      // so we want to round that, which means adding 0.5 * 1<<17,
      // aka 65536. Also, we'll end up with -128 to 127 that we want
      // to encode as 0..255 by adding 128, so we'll add that before the shift
      x0 += 65536 + (128<<17);
      x1 += 65536 + (128<<17);
      x2 += 65536 + (128<<17);
      x3 += 65536 + (128<<17);
      // tried computing the shifts into temps, or'ing the temps to see
      // if any were out of range, but that was slower
      o[0] = stbi__clamp((x0+t3) >> 17);
      o[7] = stbi__clamp((x0-t3) >> 17);
      o[1] = stbi__clamp((x1+t2) >> 17);
      o[6] = stbi__clamp((x1-t2) >> 17);
      o[2] = stbi__clamp((x2+t1) >> 17);
      o[5] = stbi__clamp((x2-t1) >> 17);
      o[3] = stbi__clamp((x3+t0) >> 17);
      o[4] = stbi__clamp((x3-t0) >> 17);
   }
}

#اگر_تعریف_شدہ STBI_SSE2
// sse2 integer IDCT. not the fastest possible implementation but it
// produces bit-identical results to the generic C version so it's
// fully "transparent".
ساکن خالی stbi__idct_simd(stbi_uc *برآمد, صحیح برآمد_stride, مختصر ڈیٹا[64])
{
   // This is constructed to match our regular (generic) integer IDCT exactly.
   __m128i row0, row1, row2, row3, row4, row5, row6, row7;
   __m128i tmp;

   // dot product constant: even elems=x, odd elems=y
   #تعریف_کرو dct_const(x,y)  _mm_setr_epi16((x),(y),(x),(y),(x),(y),(x),(y))

   // out(0) = c0[even]*x + c0[odd]*y   (c0, x, y 16-bit, out 32-bit)
   // out(1) = c1[even]*x + c1[odd]*y
   #تعریف_کرو dct_rot(out0,out1, x,y,c0,c1) \
      __m128i c0##lo = _mm_unpacklo_epi16((x),(y)); \
      __m128i c0##hi = _mm_unpackhi_epi16((x),(y)); \
      __m128i out0##_l = _mm_madd_epi16(c0##lo, c0); \
      __m128i out0##_h = _mm_madd_epi16(c0##hi, c0); \
      __m128i out1##_l = _mm_madd_epi16(c0##lo, c1); \
      __m128i out1##_h = _mm_madd_epi16(c0##hi, c1)

   // out = in << 12  (in 16-bit, out 32-bit)
   #تعریف_کرو dct_widen(برآمد, اندر) \
      __m128i برآمد##_l = _mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), (اندر)), 4); \
      __m128i برآمد##_h = _mm_srai_epi32(_mm_unpackhi_epi16(_mm_setzero_si128(), (اندر)), 4)

   // wide add
   #تعریف_کرو dct_wadd(برآمد, a, b) \
      __m128i برآمد##_l = mm_جوڑو_epi32(a##_l, b##_l); \
      __m128i برآمد##_h = mm_جوڑو_epi32(a##_h, b##_h)

   // wide sub
   #تعریف_کرو dct_wsub(برآمد, a, b) \
      __m128i برآمد##_l = _mm_sub_epi32(a##_l, b##_l); \
      __m128i برآمد##_h = _mm_sub_epi32(a##_h, b##_h)

   // butterfly a/b, add bias, then shift by "s" and pack
   #تعریف_کرو dct_bfly32o(out0, out1, a,b,bias,s) \
      { \
         __m128i abiased_l = mm_جوڑو_epi32(a##_l, bias); \
         __m128i abiased_h = mm_جوڑو_epi32(a##_h, bias); \
         dct_wadd(مجموع, abiased, b); \
         dct_wsub(dif, abiased, b); \
         out0 = _mm_packs_epi32(_mm_srai_epi32(مجموع_l, s), _mm_srai_epi32(مجموع_h, s)); \
         out1 = _mm_packs_epi32(_mm_srai_epi32(dif_l, s), _mm_srai_epi32(dif_h, s)); \
      }

   // 8-bit interleave step (for transposes)
   #تعریف_کرو dct_interleave8(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi8(a, b); \
      b = _mm_unpackhi_epi8(tmp, b)

   // 16-bit interleave step (for transposes)
   #تعریف_کرو dct_interleave16(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi16(a, b); \
      b = _mm_unpackhi_epi16(tmp, b)

   #تعریف_کرو dct_pass(bias,سرکاؤ) \
      { \
         /* even part */ \
         dct_rot(t2e,t3e, row2,row6, rot0_0,rot0_1); \
         __m128i sum04 = mm_جوڑو_epi16(row0, row4); \
         __m128i dif04 = _mm_sub_epi16(row0, row4); \
         dct_widen(t0e, sum04); \
         dct_widen(t1e, dif04); \
         dct_wadd(x0, t0e, t3e); \
         dct_wsub(x3, t0e, t3e); \
         dct_wadd(x1, t1e, t2e); \
         dct_wsub(x2, t1e, t2e); \
         /* odd part */ \
         dct_rot(y0o,y2o, row7,row3, rot2_0,rot2_1); \
         dct_rot(y1o,y3o, row5,row1, rot3_0,rot3_1); \
         __m128i sum17 = mm_جوڑو_epi16(row1, row7); \
         __m128i sum35 = mm_جوڑو_epi16(row3, row5); \
         dct_rot(y4o,y5o, sum17,sum35, rot1_0,rot1_1); \
         dct_wadd(x4, y0o, y4o); \
         dct_wadd(x5, y1o, y5o); \
         dct_wadd(x6, y2o, y5o); \
         dct_wadd(x7, y3o, y4o); \
         dct_bfly32o(row0,row7, x0,x7,bias,سرکاؤ); \
         dct_bfly32o(row1,row6, x1,x6,bias,سرکاؤ); \
         dct_bfly32o(row2,row5, x2,x5,bias,سرکاؤ); \
         dct_bfly32o(row3,row4, x3,x4,bias,سرکاؤ); \
      }

   __m128i rot0_0 = dct_const(stbi__f2f(0.5411961f), stbi__f2f(0.5411961f) + stbi__f2f(-1.847759065f));
   __m128i rot0_1 = dct_const(stbi__f2f(0.5411961f) + stbi__f2f( 0.765366865f), stbi__f2f(0.5411961f));
   __m128i rot1_0 = dct_const(stbi__f2f(1.175875602f) + stbi__f2f(-0.899976223f), stbi__f2f(1.175875602f));
   __m128i rot1_1 = dct_const(stbi__f2f(1.175875602f), stbi__f2f(1.175875602f) + stbi__f2f(-2.562915447f));
   __m128i rot2_0 = dct_const(stbi__f2f(-1.961570560f) + stbi__f2f( 0.298631336f), stbi__f2f(-1.961570560f));
   __m128i rot2_1 = dct_const(stbi__f2f(-1.961570560f), stbi__f2f(-1.961570560f) + stbi__f2f( 3.072711026f));
   __m128i rot3_0 = dct_const(stbi__f2f(-0.390180644f) + stbi__f2f( 2.053119869f), stbi__f2f(-0.390180644f));
   __m128i rot3_1 = dct_const(stbi__f2f(-0.390180644f), stbi__f2f(-0.390180644f) + stbi__f2f( 1.501321110f));

   // rounding biases in column/row passes, see stbi__idct_block for explanation.
   __m128i bias_0 = _mm_set1_epi32(512);
   __m128i bias_1 = _mm_set1_epi32(65536 + (128<<17));

   //بھرو
   row0 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 0*8));
   row1 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 1*8));
   row2 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 2*8));
   row3 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 3*8));
   row4 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 4*8));
   row5 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 5*8));
   row6 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 6*8));
   row7 = mm_بھرو_si128((ثابت __m128i *) (ڈیٹا + 7*8));

   // column pass
   dct_pass(bias_0, 10);

   {
      // 16bit 8x8 transpose pass 1
      dct_interleave16(row0, row4);
      dct_interleave16(row1, row5);
      dct_interleave16(row2, row6);
      dct_interleave16(row3, row7);

      // transpose pass 2
      dct_interleave16(row0, row2);
      dct_interleave16(row1, row3);
      dct_interleave16(row4, row6);
      dct_interleave16(row5, row7);

      // transpose pass 3
      dct_interleave16(row0, row1);
      dct_interleave16(row2, row3);
      dct_interleave16(row4, row5);
      dct_interleave16(row6, row7);
   }

   // row pass
   dct_pass(bias_1, 17);

   {
      // pack
      __m128i p0 = _mm_packus_epi16(row0, row1); // a0a1a2a3...a7b0b1b2b3...b7
      __m128i p1 = _mm_packus_epi16(row2, row3);
      __m128i p2 = _mm_packus_epi16(row4, row5);
      __m128i p3 = _mm_packus_epi16(row6, row7);

      // 8bit 8x8 transpose pass 1
      dct_interleave8(p0, p2); // a0e0a1e1...
      dct_interleave8(p1, p3); // c0g0c1g1...

      // transpose pass 2
      dct_interleave8(p0, p1); // a0c0e0g0...
      dct_interleave8(p2, p3); // b0d0f0h0...

      // transpose pass 3
      dct_interleave8(p0, p2); // a0b0c0d0...
      dct_interleave8(p1, p3); // a4b4c4d4...

      // store
      _mm_storel_epi64((__m128i *) برآمد, p0); برآمد += برآمد_stride;
      _mm_storel_epi64((__m128i *) برآمد, _mm_shuffle_epi32(p0, 0x4e)); برآمد += برآمد_stride;
      _mm_storel_epi64((__m128i *) برآمد, p2); برآمد += برآمد_stride;
      _mm_storel_epi64((__m128i *) برآمد, _mm_shuffle_epi32(p2, 0x4e)); برآمد += برآمد_stride;
      _mm_storel_epi64((__m128i *) برآمد, p1); برآمد += برآمد_stride;
      _mm_storel_epi64((__m128i *) برآمد, _mm_shuffle_epi32(p1, 0x4e)); برآمد += برآمد_stride;
      _mm_storel_epi64((__m128i *) برآمد, p3); برآمد += برآمد_stride;
      _mm_storel_epi64((__m128i *) برآمد, _mm_shuffle_epi32(p3, 0x4e));
   }

#تعریف_منسوخ dct_const
#تعریف_منسوخ dct_rot
#تعریف_منسوخ dct_widen
#تعریف_منسوخ dct_wadd
#تعریف_منسوخ dct_wsub
#تعریف_منسوخ dct_bfly32o
#تعریف_منسوخ dct_interleave8
#تعریف_منسوخ dct_interleave16
#تعریف_منسوخ dct_pass
}

#اگر_ختم // STBI_SSE2

#اگر_تعریف_شدہ STBI_NEON

// NEON integer IDCT. should produce bit-identical
// results to the generic C version.
ساکن خالی stbi__idct_simd(stbi_uc *برآمد, صحیح برآمد_stride, مختصر ڈیٹا[64])
{
   int16x8_t row0, row1, row2, row3, row4, row5, row6, row7;

   int16x4_t rot0_0 = vdup_n_s16(stbi__f2f(0.5411961f));
   int16x4_t rot0_1 = vdup_n_s16(stbi__f2f(-1.847759065f));
   int16x4_t rot0_2 = vdup_n_s16(stbi__f2f( 0.765366865f));
   int16x4_t rot1_0 = vdup_n_s16(stbi__f2f( 1.175875602f));
   int16x4_t rot1_1 = vdup_n_s16(stbi__f2f(-0.899976223f));
   int16x4_t rot1_2 = vdup_n_s16(stbi__f2f(-2.562915447f));
   int16x4_t rot2_0 = vdup_n_s16(stbi__f2f(-1.961570560f));
   int16x4_t rot2_1 = vdup_n_s16(stbi__f2f(-0.390180644f));
   int16x4_t rot3_0 = vdup_n_s16(stbi__f2f( 0.298631336f));
   int16x4_t rot3_1 = vdup_n_s16(stbi__f2f( 2.053119869f));
   int16x4_t rot3_2 = vdup_n_s16(stbi__f2f( 3.072711026f));
   int16x4_t rot3_3 = vdup_n_s16(stbi__f2f( 1.501321110f));

#تعریف_کرو dct_long_mul(برآمد, inq, coeff) \
   int32x4_t برآمد##_l = vmull_s16(vget_کم_s16(inq), coeff); \
   int32x4_t برآمد##_h = vmull_s16(vget_بلند_s16(inq), coeff)

#تعریف_کرو dct_long_mac(برآمد, acc, inq, coeff) \
   int32x4_t برآمد##_l = vmlal_s16(acc##_l, vget_کم_s16(inq), coeff); \
   int32x4_t برآمد##_h = vmlal_s16(acc##_h, vget_بلند_s16(inq), coeff)

#تعریف_کرو dct_widen(برآمد, inq) \
   int32x4_t برآمد##_l = vshll_n_s16(vget_کم_s16(inq), 12); \
   int32x4_t برآمد##_h = vshll_n_s16(vget_بلند_s16(inq), 12)

// wide add
#تعریف_کرو dct_wadd(برآمد, a, b) \
   int32x4_t برآمد##_l = vaddq_s32(a##_l, b##_l); \
   int32x4_t برآمد##_h = vaddq_s32(a##_h, b##_h)

// wide sub
#تعریف_کرو dct_wsub(برآمد, a, b) \
   int32x4_t برآمد##_l = vsubq_s32(a##_l, b##_l); \
   int32x4_t برآمد##_h = vsubq_s32(a##_h, b##_h)

// butterfly a/b, then shift using "shiftop" by "s" and pack
#تعریف_کرو dct_bfly32o(out0,out1, a,b,shiftop,s) \
   { \
      dct_wadd(مجموع, a, b); \
      dct_wsub(dif, a, b); \
      out0 = vcombine_s16(shiftop(مجموع_l, s), shiftop(مجموع_h, s)); \
      out1 = vcombine_s16(shiftop(dif_l, s), shiftop(dif_h, s)); \
   }

#تعریف_کرو dct_pass(shiftop, سرکاؤ) \
   { \
      /* even part */ \
      int16x8_t sum26 = vaddq_s16(row2, row6); \
      dct_long_mul(p1e, sum26, rot0_0); \
      dct_long_mac(t2e, p1e, row6, rot0_1); \
      dct_long_mac(t3e, p1e, row2, rot0_2); \
      int16x8_t sum04 = vaddq_s16(row0, row4); \
      int16x8_t dif04 = vsubq_s16(row0, row4); \
      dct_widen(t0e, sum04); \
      dct_widen(t1e, dif04); \
      dct_wadd(x0, t0e, t3e); \
      dct_wsub(x3, t0e, t3e); \
      dct_wadd(x1, t1e, t2e); \
      dct_wsub(x2, t1e, t2e); \
      /* odd part */ \
      int16x8_t sum15 = vaddq_s16(row1, row5); \
      int16x8_t sum17 = vaddq_s16(row1, row7); \
      int16x8_t sum35 = vaddq_s16(row3, row5); \
      int16x8_t sum37 = vaddq_s16(row3, row7); \
      int16x8_t sumodd = vaddq_s16(sum17, sum35); \
      dct_long_mul(p5o, sumodd, rot1_0); \
      dct_long_mac(p1o, p5o, sum17, rot1_1); \
      dct_long_mac(p2o, p5o, sum35, rot1_2); \
      dct_long_mul(p3o, sum37, rot2_0); \
      dct_long_mul(p4o, sum15, rot2_1); \
      dct_wadd(sump13o, p1o, p3o); \
      dct_wadd(sump24o, p2o, p4o); \
      dct_wadd(sump23o, p2o, p3o); \
      dct_wadd(sump14o, p1o, p4o); \
      dct_long_mac(x4, sump13o, row7, rot3_0); \
      dct_long_mac(x5, sump24o, row5, rot3_1); \
      dct_long_mac(x6, sump23o, row3, rot3_2); \
      dct_long_mac(x7, sump14o, row1, rot3_3); \
      dct_bfly32o(row0,row7, x0,x7,shiftop,سرکاؤ); \
      dct_bfly32o(row1,row6, x1,x6,shiftop,سرکاؤ); \
      dct_bfly32o(row2,row5, x2,x5,shiftop,سرکاؤ); \
      dct_bfly32o(row3,row4, x3,x4,shiftop,سرکاؤ); \
   }

   //بھرو
   row0 = vld1q_s16(ڈیٹا + 0*8);
   row1 = vld1q_s16(ڈیٹا + 1*8);
   row2 = vld1q_s16(ڈیٹا + 2*8);
   row3 = vld1q_s16(ڈیٹا + 3*8);
   row4 = vld1q_s16(ڈیٹا + 4*8);
   row5 = vld1q_s16(ڈیٹا + 5*8);
   row6 = vld1q_s16(ڈیٹا + 6*8);
   row7 = vld1q_s16(ڈیٹا + 7*8);

   // add DC bias
   row0 = vaddq_s16(row0, vsetq_lane_s16(1024, vdupq_n_s16(0), 0));

   // column pass
   dct_pass(vrshrn_n_s32, 10);

   // 16bit 8x8 transpose
   {
// these three map to a single VTRN.16, VTRN.32, and VSWP, respectively.
// whether compilers actually get this is another story, sadly.
#تعریف_کرو dct_trn16(x, y) { int16x8x2_t t = vtrnq_s16(x, y); x = t.val[0]; y = t.val[1]; }
#تعریف_کرو dct_trn32(x, y) { int32x4x2_t t = vtrnq_s32(vreinterpretq_s32_s16(x), vreinterpretq_s32_s16(y)); x = vreinterpretq_s16_s32(t.val[0]); y = vreinterpretq_s16_s32(t.val[1]); }
#تعریف_کرو dct_trn64(x, y) { int16x8_t x0 = x; int16x8_t y0 = y; x = vcombine_s16(vget_کم_s16(x0), vget_کم_s16(y0)); y = vcombine_s16(vget_بلند_s16(x0), vget_بلند_s16(y0)); }

      // pass 1
      dct_trn16(row0, row1); // a0b0a2b2a4b4a6b6
      dct_trn16(row2, row3);
      dct_trn16(row4, row5);
      dct_trn16(row6, row7);

      // pass 2
      dct_trn32(row0, row2); // a0b0c0d0a4b4c4d4
      dct_trn32(row1, row3);
      dct_trn32(row4, row6);
      dct_trn32(row5, row7);

      // pass 3
      dct_trn64(row0, row4); // a0b0c0d0e0f0g0h0
      dct_trn64(row1, row5);
      dct_trn64(row2, row6);
      dct_trn64(row3, row7);

#تعریف_منسوخ dct_trn16
#تعریف_منسوخ dct_trn32
#تعریف_منسوخ dct_trn64
   }

   // row pass
   // vrshrn_n_s32 only supports shifts up to 16, we need
   // 17. so do a non-rounding shift of 16 first then follow
   // up with a rounding shift by 1.
   dct_pass(vshrn_n_s32, 16);

   {
      // pack and round
      uint8x8_t p0 = vqrshrun_n_s16(row0, 1);
      uint8x8_t p1 = vqrshrun_n_s16(row1, 1);
      uint8x8_t p2 = vqrshrun_n_s16(row2, 1);
      uint8x8_t p3 = vqrshrun_n_s16(row3, 1);
      uint8x8_t p4 = vqrshrun_n_s16(row4, 1);
      uint8x8_t p5 = vqrshrun_n_s16(row5, 1);
      uint8x8_t p6 = vqrshrun_n_s16(row6, 1);
      uint8x8_t p7 = vqrshrun_n_s16(row7, 1);

      // again, these can translate into one instruction, but often don't.
#تعریف_کرو dct_trn8_8(x, y) { uint8x8x2_t t = vtrn_u8(x, y); x = t.val[0]; y = t.val[1]; }
#تعریف_کرو dct_trn8_16(x, y) { uint16x4x2_t t = vtrn_u16(vreinterpret_u16_u8(x), vreinterpret_u16_u8(y)); x = vreinterpret_u8_u16(t.val[0]); y = vreinterpret_u8_u16(t.val[1]); }
#تعریف_کرو dct_trn8_32(x, y) { uint32x2x2_t t = vtrn_u32(vreinterpret_u32_u8(x), vreinterpret_u32_u8(y)); x = vreinterpret_u8_u32(t.val[0]); y = vreinterpret_u8_u32(t.val[1]); }

      // sadly can't use interleaved stores here since we only write
      // 8 bytes to each scan line!

      // 8x8 8-bit transpose pass 1
      dct_trn8_8(p0, p1);
      dct_trn8_8(p2, p3);
      dct_trn8_8(p4, p5);
      dct_trn8_8(p6, p7);

      // pass 2
      dct_trn8_16(p0, p2);
      dct_trn8_16(p1, p3);
      dct_trn8_16(p4, p6);
      dct_trn8_16(p5, p7);

      // pass 3
      dct_trn8_32(p0, p4);
      dct_trn8_32(p1, p5);
      dct_trn8_32(p2, p6);
      dct_trn8_32(p3, p7);

      // store
      vst1_u8(برآمد, p0); برآمد += برآمد_stride;
      vst1_u8(برآمد, p1); برآمد += برآمد_stride;
      vst1_u8(برآمد, p2); برآمد += برآمد_stride;
      vst1_u8(برآمد, p3); برآمد += برآمد_stride;
      vst1_u8(برآمد, p4); برآمد += برآمد_stride;
      vst1_u8(برآمد, p5); برآمد += برآمد_stride;
      vst1_u8(برآمد, p6); برآمد += برآمد_stride;
      vst1_u8(برآمد, p7);

#تعریف_منسوخ dct_trn8_8
#تعریف_منسوخ dct_trn8_16
#تعریف_منسوخ dct_trn8_32
   }

#تعریف_منسوخ dct_long_mul
#تعریف_منسوخ dct_long_mac
#تعریف_منسوخ dct_widen
#تعریف_منسوخ dct_wadd
#تعریف_منسوخ dct_wsub
#تعریف_منسوخ dct_bfly32o
#تعریف_منسوخ dct_pass
}

#اگر_ختم // STBI_NEON

#تعریف_کرو STBI__MARKER_none  0xff
// if there's a pending marker from the entropy stream, return that
// otherwise, fetch from the stream and get a marker. if there's no
// marker, return 0xff, which is never a valid marker value
ساکن stbi_uc stbi__get_marker(stbi__jpeg *j)
{
   stbi_uc x;
   اگر (j->marker != STBI__MARKER_none) { x = j->marker; j->marker = STBI__MARKER_none; واپس x; }
   x = stbi__get8(j->s);
   اگر (x != 0xff) واپس STBI__MARKER_none;
   جب (x == 0xff)
      x = stbi__get8(j->s); // consume repeated 0xff fill bytes
   واپس x;
}

// in each scan, we'll have scan_n components, and the order
// of the components is specified by order[]
#تعریف_کرو STBI__RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)

// after a restart interval, stbi__jpeg_reset the entropy decoder and
// the dc prediction
ساکن خالی stbi__jpeg_reset(stbi__jpeg *j)
{
   j->رمز_بٹ = 0;
   j->رمز_بفر = 0;
   j->nomore = 0;
   j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = j->img_comp[3].dc_pred = 0;
   j->marker = STBI__MARKER_none;
   j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
   j->eob_تسلسل = 0;
   // no more than 1<<31 MCUs if no restart_interal? that's plenty safe,
   // since we don't even allow 1<<30 pixels
}

ساکن صحیح stbi_parse_entropy_coded_ڈیٹا(stbi__jpeg *z)
{
   stbi__jpeg_reset(z);
   اگر (!z->progressive) {
      اگر (z->جائزہ_n == 1) {
         صحیح i,j;
         STBI_SIMD_ALIGN(مختصر, ڈیٹا[64]);
         صحیح n = z->ترتیب[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         صحیح w = (z->img_comp[n].x+7) >> 3;
         صحیح h = (z->img_comp[n].y+7) >> 3;
         برائے (j=0; j < h; ++j) {
            برائے (i=0; i < w; ++i) {
               صحیح ha = z->img_comp[n].ha;
               اگر (!stbi_jpeg_decode_بلاک(z, ڈیٹا, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) واپس 0;
               z->idct_بلاک_kernel(z->img_comp[n].ڈیٹا+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, ڈیٹا);
               // every data block is an MCU, so countdown the restart interval
               اگر (--z->todo <= 0) {
                  اگر (z->رمز_بٹ < 24) stbi_grow_بفر_unsafe(z);
                  // if it's NOT a restart, then just bail, so we get corrupt data
                  // rather than no data
                  اگر (!STBI__RESTART(z->marker)) واپس 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         واپس 1;
      } ورنہ { // interleaved
         صحیح i,j,k,x,y;
         STBI_SIMD_ALIGN(مختصر, ڈیٹا[64]);
         برائے (j=0; j < z->img_mcu_y; ++j) {
            برائے (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               برائے (k=0; k < z->جائزہ_n; ++k) {
                  صحیح n = z->ترتیب[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  برائے (y=0; y < z->img_comp[n].v; ++y) {
                     برائے (x=0; x < z->img_comp[n].h; ++x) {
                        صحیح x2 = (i*z->img_comp[n].h + x)*8;
                        صحیح y2 = (j*z->img_comp[n].v + y)*8;
                        صحیح ha = z->img_comp[n].ha;
                        اگر (!stbi_jpeg_decode_بلاک(z, ڈیٹا, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) واپس 0;
                        z->idct_بلاک_kernel(z->img_comp[n].ڈیٹا+z->img_comp[n].w2*y2+x2, z->img_comp[n].w2, ڈیٹا);
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               اگر (--z->todo <= 0) {
                  اگر (z->رمز_بٹ < 24) stbi_grow_بفر_unsafe(z);
                  اگر (!STBI__RESTART(z->marker)) واپس 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         واپس 1;
      }
   } ورنہ {
      اگر (z->جائزہ_n == 1) {
         صحیح i,j;
         صحیح n = z->ترتیب[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         صحیح w = (z->img_comp[n].x+7) >> 3;
         صحیح h = (z->img_comp[n].y+7) >> 3;
         برائے (j=0; j < h; ++j) {
            برائے (i=0; i < w; ++i) {
               مختصر *ڈیٹا = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               اگر (z->spec_آغاز == 0) {
                  اگر (!stbi_jpeg_decode_بلاک_prog_dc(z, ڈیٹا, &z->huff_dc[z->img_comp[n].hd], n))
                     واپس 0;
               } ورنہ {
                  صحیح ha = z->img_comp[n].ha;
                  اگر (!stbi_jpeg_decode_بلاک_prog_ac(z, ڈیٹا, &z->huff_ac[ha], z->fast_ac[ha]))
                     واپس 0;
               }
               // every data block is an MCU, so countdown the restart interval
               اگر (--z->todo <= 0) {
                  اگر (z->رمز_بٹ < 24) stbi_grow_بفر_unsafe(z);
                  اگر (!STBI__RESTART(z->marker)) واپس 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         واپس 1;
      } ورنہ { // interleaved
         صحیح i,j,k,x,y;
         برائے (j=0; j < z->img_mcu_y; ++j) {
            برائے (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               برائے (k=0; k < z->جائزہ_n; ++k) {
                  صحیح n = z->ترتیب[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  برائے (y=0; y < z->img_comp[n].v; ++y) {
                     برائے (x=0; x < z->img_comp[n].h; ++x) {
                        صحیح x2 = (i*z->img_comp[n].h + x);
                        صحیح y2 = (j*z->img_comp[n].v + y);
                        مختصر *ڈیٹا = z->img_comp[n].coeff + 64 * (x2 + y2 * z->img_comp[n].coeff_w);
                        اگر (!stbi_jpeg_decode_بلاک_prog_dc(z, ڈیٹا, &z->huff_dc[z->img_comp[n].hd], n))
                           واپس 0;
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               اگر (--z->todo <= 0) {
                  اگر (z->رمز_بٹ < 24) stbi_grow_بفر_unsafe(z);
                  اگر (!STBI__RESTART(z->marker)) واپس 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         واپس 1;
      }
   }
}

ساکن خالی stbi__jpeg_dequantize(مختصر *ڈیٹا, stbi__uint16 *dequant)
{
   صحیح i;
   برائے (i=0; i < 64; ++i)
      ڈیٹا[i] *= dequant[i];
}

ساکن خالی stbi_jpeg_اختتام(stbi__jpeg *z)
{
   اگر (z->progressive) {
      // dequantize and idct the data
      صحیح i,j,n;
      برائے (n=0; n < z->s->img_n; ++n) {
         صحیح w = (z->img_comp[n].x+7) >> 3;
         صحیح h = (z->img_comp[n].y+7) >> 3;
         برائے (j=0; j < h; ++j) {
            برائے (i=0; i < w; ++i) {
               مختصر *ڈیٹا = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               stbi__jpeg_dequantize(ڈیٹا, z->dequant[z->img_comp[n].tq]);
               z->idct_بلاک_kernel(z->img_comp[n].ڈیٹا+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, ڈیٹا);
            }
         }
      }
   }
}

ساکن صحیح stbi__process_marker(stbi__jpeg *z, صحیح m)
{
   صحیح L;
   انتخاب (m) {
      حالت STBI__MARKER_none: // no marker found
         واپس stbi__err("expected marker","Corrupt JPEG");

      حالت 0xDD: // DRI - specify restart interval
         اگر (stbi__get16be(z->s) != 4) واپس stbi__err("bad DRI len","Corrupt JPEG");
         z->restart_interval = stbi__get16be(z->s);
         واپس 1;

      حالت 0xDB: // DQT - define quantization table
         L = stbi__get16be(z->s)-2;
         جب (L > 0) {
            صحیح q = stbi__get8(z->s);
            صحیح p = q >> 4, sixteen = (p != 0);
            صحیح t = q & 15,i;
            اگر (p != 0 && p != 1) واپس stbi__err("bad DQT type","Corrupt JPEG");
            اگر (t > 3) واپس stbi__err("bad DQT table","Corrupt JPEG");

            برائے (i=0; i < 64; ++i)
               z->dequant[t][stbi__jpeg_dezigzag[i]] = (stbi__uint16)(sixteen ? stbi__get16be(z->s) : stbi__get8(z->s));
            L -= (sixteen ? 129 : 65);
         }
         واپس L==0;

      حالت 0xC4: // DHT - define huffman table
         L = stbi__get16be(z->s)-2;
         جب (L > 0) {
            stbi_uc *v;
            صحیح sizes[16],i,n=0;
            صحیح q = stbi__get8(z->s);
            صحیح tc = q >> 4;
            صحیح th = q & 15;
            اگر (tc > 1 || th > 3) واپس stbi__err("bad DHT header","Corrupt JPEG");
            برائے (i=0; i < 16; ++i) {
               sizes[i] = stbi__get8(z->s);
               n += sizes[i];
            }
            L -= 17;
            اگر (tc == 0) {
               اگر (!stbi__build_huffman(z->huff_dc+th, sizes)) واپس 0;
               v = z->huff_dc[th].اقدار;
            } ورنہ {
               اگر (!stbi__build_huffman(z->huff_ac+th, sizes)) واپس 0;
               v = z->huff_ac[th].اقدار;
            }
            برائے (i=0; i < n; ++i)
               v[i] = stbi__get8(z->s);
            اگر (tc != 0)
               stbi__build_fast_ac(z->fast_ac[th], z->huff_ac + th);
            L -= n;
         }
         واپس L==0;
   }

   // check for comment block or APP blocks
   اگر ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
      L = stbi__get16be(z->s);
      اگر (L < 2) {
         اگر (m == 0xFE)
            واپس stbi__err("bad COM len","Corrupt JPEG");
         ورنہ
            واپس stbi__err("bad APP len","Corrupt JPEG");
      }
      L -= 2;

      اگر (m == 0xE0 && L >= 5) { // JFIF APP0 segment
         ساکن ثابت غیرعلامتی حرف لیبل[5] = {'J','F','I','F','\0'};
         صحیح ok = 1;
         صحیح i;
         برائے (i=0; i < 5; ++i)
            اگر (stbi__get8(z->s) != لیبل[i])
               ok = 0;
         L -= 5;
         اگر (ok)
            z->jfif = 1;
      } ورنہ اگر (m == 0xEE && L >= 12) { // Adobe APP14 segment
         ساکن ثابت غیرعلامتی حرف لیبل[6] = {'A','d','o','b','e','\0'};
         صحیح ok = 1;
         صحیح i;
         برائے (i=0; i < 6; ++i)
            اگر (stbi__get8(z->s) != لیبل[i])
               ok = 0;
         L -= 6;
         اگر (ok) {
            stbi__get8(z->s); //نسخہ
            stbi__get16be(z->s); // flags0
            stbi__get16be(z->s); // flags1
            z->app14_color_transform = stbi__get8(z->s); // color transform
            L -= 6;
         }
      }

      stbi__skip(z->s, L);
      واپس 1;
   }

   واپس stbi__err("unknown marker","Corrupt JPEG");
}

// after we see SOS
ساکن صحیح stbi_process_جائزہ_سرخی(stbi__jpeg *z)
{
   صحیح i;
   صحیح Ls = stbi__get16be(z->s);
   z->جائزہ_n = stbi__get8(z->s);
   اگر (z->جائزہ_n < 1 || z->جائزہ_n > 4 || z->جائزہ_n > (صحیح) z->s->img_n) واپس stbi__err("bad SOS component count","Corrupt JPEG");
   اگر (Ls != 6+2*z->جائزہ_n) واپس stbi__err("bad SOS len","Corrupt JPEG");
   برائے (i=0; i < z->جائزہ_n; ++i) {
      صحیح id = stbi__get8(z->s), which;
      صحیح q = stbi__get8(z->s);
      برائے (which = 0; which < z->s->img_n; ++which)
         اگر (z->img_comp[which].id == id)
            توڑو;
      اگر (which == z->s->img_n) واپس 0; // no match
      z->img_comp[which].hd = q >> 4;   اگر (z->img_comp[which].hd > 3) واپس stbi__err("bad DC huff","Corrupt JPEG");
      z->img_comp[which].ha = q & 15;   اگر (z->img_comp[which].ha > 3) واپس stbi__err("bad AC huff","Corrupt JPEG");
      z->ترتیب[i] = which;
   }

   {
      صحیح aa;
      z->spec_آغاز = stbi__get8(z->s);
      z->spec_انتہا   = stbi__get8(z->s); // should be 63, but might be 0
      aa = stbi__get8(z->s);
      z->succ_بلند = (aa >> 4);
      z->succ_کم  = (aa & 15);
      اگر (z->progressive) {
         اگر (z->spec_آغاز > 63 || z->spec_انتہا > 63  || z->spec_آغاز > z->spec_انتہا || z->succ_بلند > 13 || z->succ_کم > 13)
            واپس stbi__err("bad SOS", "Corrupt JPEG");
      } ورنہ {
         اگر (z->spec_آغاز != 0) واپس stbi__err("bad SOS","Corrupt JPEG");
         اگر (z->succ_بلند != 0 || z->succ_کم != 0) واپس stbi__err("bad SOS","Corrupt JPEG");
         z->spec_انتہا = 63;
      }
   }

   واپس 1;
}

ساکن صحیح stbi__free_jpeg_components(stbi__jpeg *z, صحیح ncomp, صحیح why)
{
   صحیح i;
   برائے (i=0; i < ncomp; ++i) {
      اگر (z->img_comp[i].raw_ڈیٹا) {
         STBI_FREE(z->img_comp[i].raw_ڈیٹا);
         z->img_comp[i].raw_ڈیٹا = تہی;
         z->img_comp[i].ڈیٹا = تہی;
      }
      اگر (z->img_comp[i].raw_coeff) {
         STBI_FREE(z->img_comp[i].raw_coeff);
         z->img_comp[i].raw_coeff = 0;
         z->img_comp[i].coeff = 0;
      }
      اگر (z->img_comp[i].linebuf) {
         STBI_FREE(z->img_comp[i].linebuf);
         z->img_comp[i].linebuf = تہی;
      }
   }
   واپس why;
}

ساکن صحیح stbi_process_چوکھٹا_سرخی(stbi__jpeg *z, صحیح جائزہ)
{
   stbi__context *s = z->s;
   صحیح Lf,p,i,q, h_max=1,v_max=1,c;
   Lf = stbi__get16be(s);         اگر (Lf < 11) واپس stbi__err("bad SOF len","Corrupt JPEG"); // JPEG
   p  = stbi__get8(s);            اگر (p != 8) واپس stbi__err("only 8-bit","JPEG format not supported: 8-bit only"); // JPEG baseline
   s->img_y = stbi__get16be(s);   اگر (s->img_y == 0) واپس stbi__err("no header height", "JPEG format not supported: delayed height"); // Legal, but we don't handle it--but neither does IJG
   s->img_x = stbi__get16be(s);   اگر (s->img_x == 0) واپس stbi__err("0 width","Corrupt JPEG"); // JPEG requires
   c = stbi__get8(s);
   اگر (c != 3 && c != 1 && c != 4) واپس stbi__err("bad component count","Corrupt JPEG");
   s->img_n = c;
   برائے (i=0; i < c; ++i) {
      z->img_comp[i].ڈیٹا = تہی;
      z->img_comp[i].linebuf = تہی;
   }

   اگر (Lf != 8+3*s->img_n) واپس stbi__err("bad SOF len","Corrupt JPEG");

   z->rgb = 0;
   برائے (i=0; i < s->img_n; ++i) {
      ساکن ثابت غیرعلامتی حرف rgb[3] = { 'R', 'G', 'B' };
      z->img_comp[i].id = stbi__get8(s);
      اگر (s->img_n == 3 && z->img_comp[i].id == rgb[i])
         ++z->rgb;
      q = stbi__get8(s);
      z->img_comp[i].h = (q >> 4);  اگر (!z->img_comp[i].h || z->img_comp[i].h > 4) واپس stbi__err("bad H","Corrupt JPEG");
      z->img_comp[i].v = q & 15;    اگر (!z->img_comp[i].v || z->img_comp[i].v > 4) واپس stbi__err("bad V","Corrupt JPEG");
      z->img_comp[i].tq = stbi__get8(s);  اگر (z->img_comp[i].tq > 3) واپس stbi__err("bad TQ","Corrupt JPEG");
   }

   اگر (جائزہ != STBI_جائزہ_بھرو) واپس 1;

   اگر (!stbi__mad3sizes_valid(s->img_x, s->img_y, s->img_n, 0)) واپس stbi__err("too large", "Image too large to decode");

   برائے (i=0; i < s->img_n; ++i) {
      اگر (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
      اگر (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
   }

   // compute interleaved mcu info
   z->img_h_max = h_max;
   z->img_v_max = v_max;
   z->img_mcu_w = h_max * 8;
   z->img_mcu_h = v_max * 8;
   // these sizes can't be more than 17 bits
   z->img_mcu_x = (s->img_x + z->img_mcu_w-1) / z->img_mcu_w;
   z->img_mcu_y = (s->img_y + z->img_mcu_h-1) / z->img_mcu_h;

   برائے (i=0; i < s->img_n; ++i) {
      // number of effective pixels (e.g. for non-interleaved MCU)
      z->img_comp[i].x = (s->img_x * z->img_comp[i].h + h_max-1) / h_max;
      z->img_comp[i].y = (s->img_y * z->img_comp[i].v + v_max-1) / v_max;
      // to simplify generation, we'll allocate enough memory to decode
      // the bogus oversized data from using interleaved MCUs and their
      // big blocks (e.g. a 16x16 iMCU on an image of width 33); we won't
      // discard the extra data until colorspace conversion
      //
      // img_mcu_x, img_mcu_y: <=17 bits; comp[i].h and .v are <=4 (checked earlier)
      // so these muls can't overflow with 32-bit ints (which we require)
      z->img_comp[i].w2 = z->img_mcu_x * z->img_comp[i].h * 8;
      z->img_comp[i].h2 = z->img_mcu_y * z->img_comp[i].v * 8;
      z->img_comp[i].coeff = 0;
      z->img_comp[i].raw_coeff = 0;
      z->img_comp[i].linebuf = تہی;
      z->img_comp[i].raw_ڈیٹا = stbi__malloc_mad2(z->img_comp[i].w2, z->img_comp[i].h2, 15);
      اگر (z->img_comp[i].raw_ڈیٹا == تہی)
         واپس stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
      // align blocks for idct using mmx/sse
      z->img_comp[i].ڈیٹا = (stbi_uc*) (((size_t) z->img_comp[i].raw_ڈیٹا + 15) & ~15);
      اگر (z->progressive) {
         // w2, h2 are multiples of 8 (see above)
         z->img_comp[i].coeff_w = z->img_comp[i].w2 / 8;
         z->img_comp[i].coeff_h = z->img_comp[i].h2 / 8;
         z->img_comp[i].raw_coeff = stbi__malloc_mad3(z->img_comp[i].w2, z->img_comp[i].h2, حجم(مختصر), 15);
         اگر (z->img_comp[i].raw_coeff == تہی)
            واپس stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
         z->img_comp[i].coeff = (مختصر*) (((size_t) z->img_comp[i].raw_coeff + 15) & ~15);
      }
   }

   واپس 1;
}

// use comparisons since in some cases we handle more than one case (e.g. SOF)
#تعریف_کرو stbi__DNL(x)         ((x) == 0xdc)
#تعریف_کرو stbi__SOI(x)         ((x) == 0xd8)
#تعریف_کرو stbi__EOI(x)         ((x) == 0xd9)
#تعریف_کرو stbi__SOF(x)         ((x) == 0xc0 || (x) == 0xc1 || (x) == 0xc2)
#تعریف_کرو stbi__SOS(x)         ((x) == 0xda)

#تعریف_کرو stbi__SOF_progressive(x)   ((x) == 0xc2)

ساکن صحیح stbi_decode_jpeg_سرخی(stbi__jpeg *z, صحیح جائزہ)
{
   صحیح m;
   z->jfif = 0;
   z->app14_color_transform = -1; // valid values are 0,1,2
   z->marker = STBI__MARKER_none; // initialize cached marker to empty
   m = stbi__get_marker(z);
   اگر (!stbi__SOI(m)) واپس stbi__err("no SOI","Corrupt JPEG");
   اگر (جائزہ == STBI_جائزہ_type) واپس 1;
   m = stbi__get_marker(z);
   جب (!stbi__SOF(m)) {
      اگر (!stbi__process_marker(z,m)) واپس 0;
      m = stbi__get_marker(z);
      جب (m == STBI__MARKER_none) {
         // some files have extra padding after their blocks, so ok, we'll scan
         اگر (stbi__at_eof(z->s)) واپس stbi__err("no SOF", "Corrupt JPEG");
         m = stbi__get_marker(z);
      }
   }
   z->progressive = stbi__SOF_progressive(m);
   اگر (!stbi_process_چوکھٹا_سرخی(z, جائزہ)) واپس 0;
   واپس 1;
}

// decode image to YCbCr format
ساکن صحیح stbi__decode_jpeg_image(stbi__jpeg *j)
{
   صحیح m;
   برائے (m = 0; m < 4; m++) {
      j->img_comp[m].raw_ڈیٹا = تہی;
      j->img_comp[m].raw_coeff = تہی;
   }
   j->restart_interval = 0;
   اگر (!stbi_decode_jpeg_سرخی(j, STBI_جائزہ_بھرو)) واپس 0;
   m = stbi__get_marker(j);
   جب (!stbi__EOI(m)) {
      اگر (stbi__SOS(m)) {
         اگر (!stbi_process_جائزہ_سرخی(j)) واپس 0;
         اگر (!stbi_parse_entropy_coded_ڈیٹا(j)) واپس 0;
         اگر (j->marker == STBI__MARKER_none ) {
            // handle 0s at the end of image data from IP Kamera 9060
            جب (!stbi__at_eof(j->s)) {
               صحیح x = stbi__get8(j->s);
               اگر (x == 255) {
                  j->marker = stbi__get8(j->s);
                  توڑو;
               }
            }
            // if we reach eof without hitting a marker, stbi__get_marker() below will fail and we'll eventually return 0
         }
      } ورنہ اگر (stbi__DNL(m)) {
         صحیح Ld = stbi__get16be(j->s);
         stbi__uint32 NL = stbi__get16be(j->s);
         اگر (Ld != 4) واپس stbi__err("bad DNL len", "Corrupt JPEG");
         اگر (NL != j->s->img_y) واپس stbi__err("bad DNL height", "Corrupt JPEG");
      } ورنہ {
         اگر (!stbi__process_marker(j, m)) واپس 0;
      }
      m = stbi__get_marker(j);
   }
   اگر (j->progressive)
      stbi_jpeg_اختتام(j);
   واپس 1;
}

// static jfif-centered resampling (across block boundaries)

قسمتعریف stbi_uc *(*resample_صف_func)(stbi_uc *برآمد, stbi_uc *in0, stbi_uc *in1,
                                    صحیح w, صحیح hs);

#تعریف_کرو stbi__div4(x) ((stbi_uc) ((x) >> 2))

ساکن stbi_uc *resample_صف_1(stbi_uc *برآمد, stbi_uc *اندر_near, stbi_uc *اندر_far, صحیح w, صحیح hs)
{
   STBI_NOTUSED(برآمد);
   STBI_NOTUSED(اندر_far);
   STBI_NOTUSED(w);
   STBI_NOTUSED(hs);
   واپس اندر_near;
}

ساکن stbi_uc* stbi_resample_صف_v_2(stbi_uc *برآمد, stbi_uc *اندر_near, stbi_uc *اندر_far, صحیح w, صحیح hs)
{
   // need to generate two samples vertically for every one in input
   صحیح i;
   STBI_NOTUSED(hs);
   برائے (i=0; i < w; ++i)
      برآمد[i] = stbi__div4(3*اندر_near[i] + اندر_far[i] + 2);
   واپس برآمد;
}

ساکن stbi_uc*  stbi_resample_صف_h_2(stbi_uc *برآمد, stbi_uc *اندر_near, stbi_uc *اندر_far, صحیح w, صحیح hs)
{
   // need to generate two samples horizontally for every one in input
   صحیح i;
   stbi_uc *داخلہ = اندر_near;

   اگر (w == 1) {
      // if only one sample, can't do any interpolation
      برآمد[0] = برآمد[1] = داخلہ[0];
      واپس برآمد;
   }

   برآمد[0] = داخلہ[0];
   برآمد[1] = stbi__div4(داخلہ[0]*3 + داخلہ[1] + 2);
   برائے (i=1; i < w-1; ++i) {
      صحیح n = 3*داخلہ[i]+2;
      برآمد[i*2+0] = stbi__div4(n+داخلہ[i-1]);
      برآمد[i*2+1] = stbi__div4(n+داخلہ[i+1]);
   }
   برآمد[i*2+0] = stbi__div4(داخلہ[w-2]*3 + داخلہ[w-1] + 2);
   برآمد[i*2+1] = داخلہ[w-1];

   STBI_NOTUSED(اندر_far);
   STBI_NOTUSED(hs);

   واپس برآمد;
}

#تعریف_کرو stbi__div16(x) ((stbi_uc) ((x) >> 4))

ساکن stbi_uc *stbi_resample_صف_hv_2(stbi_uc *برآمد, stbi_uc *اندر_near, stbi_uc *اندر_far, صحیح w, صحیح hs)
{
   // need to generate 2x2 samples for every one in input
   صحیح i,t0,t1;
   اگر (w == 1) {
      برآمد[0] = برآمد[1] = stbi__div4(3*اندر_near[0] + اندر_far[0] + 2);
      واپس برآمد;
   }

   t1 = 3*اندر_near[0] + اندر_far[0];
   برآمد[0] = stbi__div4(t1+2);
   برائے (i=1; i < w; ++i) {
      t0 = t1;
      t1 = 3*اندر_near[i]+اندر_far[i];
      برآمد[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      برآمد[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   برآمد[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   واپس برآمد;
}

#اگر defined(STBI_SSE2) || defined(STBI_NEON)
ساکن stbi_uc *stbi_resample_صف_hv_2_simd(stbi_uc *برآمد, stbi_uc *اندر_near, stbi_uc *اندر_far, صحیح w, صحیح hs)
{
   // need to generate 2x2 samples for every one in input
   صحیح i=0,t0,t1;

   اگر (w == 1) {
      برآمد[0] = برآمد[1] = stbi__div4(3*اندر_near[0] + اندر_far[0] + 2);
      واپس برآمد;
   }

   t1 = 3*اندر_near[0] + اندر_far[0];
   // process groups of 8 pixels for as long as we can.
   // note we can't handle the last pixel in a row in this loop
   // because we need to handle the filter boundary conditions.
   برائے (; i < ((w-1) & ~7); i += 8) {
#اگر defined(STBI_SSE2)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      __m128i zero  = _mm_setzero_si128();
      __m128i farb  = _mm_loadl_epi64((__m128i *) (اندر_far + i));
      __m128i nearb = _mm_loadl_epi64((__m128i *) (اندر_near + i));
      __m128i farw  = _mm_unpacklo_epi8(farb, zero);
      __m128i nearw = _mm_unpacklo_epi8(nearb, zero);
      __m128i diff  = _mm_sub_epi16(farw, nearw);
      __m128i nears = _mm_slli_epi16(nearw, 2);
      __m128i curr  = mm_جوڑو_epi16(nears, diff); // current row

      // horizontal filter works the same based on shifted vers of current
      // row. "prev" is current row shifted right by 1 pixel; we need to
      // insert the previous pixel value (from t1).
      // "next" is current row shifted left by 1 pixel, with first pixel
      // of next block of 8 pixels added in.
      __m128i prv0 = _mm_slli_si128(curr, 2);
      __m128i nxt0 = _mm_srli_si128(curr, 2);
      __m128i پچھلا = mm_داخل_کرو_epi16(prv0, t1, 0);
      __m128i اگلا = mm_داخل_کرو_epi16(nxt0, 3*اندر_near[i+8] + اندر_far[i+8], 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      __m128i bias  = _mm_set1_epi16(8);
      __m128i curs = _mm_slli_epi16(curr, 2);
      __m128i prvd = _mm_sub_epi16(پچھلا, curr);
      __m128i nxtd = _mm_sub_epi16(اگلا, curr);
      __m128i curb = mm_جوڑو_epi16(curs, bias);
      __m128i even = mm_جوڑو_epi16(prvd, curb);
      __m128i odd  = mm_جوڑو_epi16(nxtd, curb);

      // interleave even and odd pixels, then undo scaling.
      __m128i int0 = _mm_unpacklo_epi16(even, odd);
      __m128i int1 = _mm_unpackhi_epi16(even, odd);
      __m128i de0  = _mm_srli_epi16(int0, 4);
      __m128i de1  = _mm_srli_epi16(int1, 4);

      // pack and write output
      __m128i outv = _mm_packus_epi16(de0, de1);
      _mm_storeu_si128((__m128i *) (برآمد + i*2), outv);
#وگرنہ_اگر defined(STBI_NEON)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      uint8x8_t farb  = vld1_u8(اندر_far + i);
      uint8x8_t nearb = vld1_u8(اندر_near + i);
      int16x8_t diff  = vreinterpretq_s16_u16(vsubl_u8(farb, nearb));
      int16x8_t nears = vreinterpretq_s16_u16(vshll_n_u8(nearb, 2));
      int16x8_t curr  = vaddq_s16(nears, diff); // current row

      // horizontal filter works the same based on shifted vers of current
      // row. "prev" is current row shifted right by 1 pixel; we need to
      // insert the previous pixel value (from t1).
      // "next" is current row shifted left by 1 pixel, with first pixel
      // of next block of 8 pixels added in.
      int16x8_t prv0 = vextq_s16(curr, curr, 7);
      int16x8_t nxt0 = vextq_s16(curr, curr, 1);
      int16x8_t پچھلا = vsetq_lane_s16(t1, prv0, 0);
      int16x8_t اگلا = vsetq_lane_s16(3*اندر_near[i+8] + اندر_far[i+8], nxt0, 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      int16x8_t curs = vshlq_n_s16(curr, 2);
      int16x8_t prvd = vsubq_s16(پچھلا, curr);
      int16x8_t nxtd = vsubq_s16(اگلا, curr);
      int16x8_t even = vaddq_s16(curs, prvd);
      int16x8_t odd  = vaddq_s16(curs, nxtd);

      // undo scaling and round, then store with even/odd phases interleaved
      uint8x8x2_t o;
      o.val[0] = vqrshrun_n_s16(even, 4);
      o.val[1] = vqrshrun_n_s16(odd,  4);
      vst2_u8(برآمد + i*2, o);
#اگر_ختم

      // "previous" value for next iter
      t1 = 3*اندر_near[i+7] + اندر_far[i+7];
   }

   t0 = t1;
   t1 = 3*اندر_near[i] + اندر_far[i];
   برآمد[i*2] = stbi__div16(3*t1 + t0 + 8);

   برائے (++i; i < w; ++i) {
      t0 = t1;
      t1 = 3*اندر_near[i]+اندر_far[i];
      برآمد[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      برآمد[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   برآمد[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   واپس برآمد;
}
#اگر_ختم

ساکن stbi_uc *stbi_resample_صف_generic(stbi_uc *برآمد, stbi_uc *اندر_near, stbi_uc *اندر_far, صحیح w, صحیح hs)
{
   // resample with nearest-neighbor
   صحیح i,j;
   STBI_NOTUSED(اندر_far);
   برائے (i=0; i < w; ++i)
      برائے (j=0; j < hs; ++j)
         برآمد[i*hs+j] = اندر_near[i];
   واپس برآمد;
}

// this is a reduced-precision calculation of YCbCr-to-RGB introduced
// to make sure the code produces the same results in both SIMD and scalar
#تعریف_کرو stbi__float2fixed(x)  (((صحیح) ((x) * 4096.0f + 0.5f)) << 8)
ساکن خالی stbi_YCb_Cr_مقصد_RGB_صف(stbi_uc *برآمد, ثابت stbi_uc *y, ثابت stbi_uc *pcb, ثابت stbi_uc *pcr, صحیح گنتی, صحیح قدم)
{
   صحیح i;
   برائے (i=0; i < گنتی; ++i) {
      صحیح y_fixed = (y[i] << 20) + (1<<19); // rounding
      صحیح r,g,b;
      صحیح cr = pcr[i] - 128;
      صحیح cb = pcb[i] - 128;
      r = y_fixed +  cr* stbi__float2fixed(1.40200f);
      g = y_fixed + (cr*-stbi__float2fixed(0.71414f)) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                     +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      اگر ((غیرعلامتی) r > 255) { اگر (r < 0) r = 0; ورنہ r = 255; }
      اگر ((غیرعلامتی) g > 255) { اگر (g < 0) g = 0; ورنہ g = 255; }
      اگر ((غیرعلامتی) b > 255) { اگر (b < 0) b = 0; ورنہ b = 255; }
      برآمد[0] = (stbi_uc)r;
      برآمد[1] = (stbi_uc)g;
      برآمد[2] = (stbi_uc)b;
      برآمد[3] = 255;
      برآمد += قدم;
   }
}

#اگر defined(STBI_SSE2) || defined(STBI_NEON)
ساکن خالی stbi_YCb_Cr_مقصد_RGB_simd(stbi_uc *برآمد, stbi_uc ثابت *y, stbi_uc ثابت *pcb, stbi_uc ثابت *pcr, صحیح گنتی, صحیح قدم)
{
   صحیح i = 0;

#اگر_تعریف_شدہ STBI_SSE2
   // step == 3 is pretty ugly on the final interleave, and i'm not convinced
   // it's useful in practice (you wouldn't use it for textures, for example).
   // so just accelerate step == 4 case.
   اگر (قدم == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      __m128i signflip  = _mm_set1_epi8(-0x80);
      __m128i cr_const0 = _mm_set1_epi16(   (مختصر) ( 1.40200f*4096.0f+0.5f));
      __m128i cr_const1 = _mm_set1_epi16( - (مختصر) ( 0.71414f*4096.0f+0.5f));
      __m128i cb_const0 = _mm_set1_epi16( - (مختصر) ( 0.34414f*4096.0f+0.5f));
      __m128i cb_const1 = _mm_set1_epi16(   (مختصر) ( 1.77200f*4096.0f+0.5f));
      __m128i y_bias = _mm_set1_epi8((حرف) (غیرعلامتی حرف) 128);
      __m128i xw = _mm_set1_epi16(255); // alpha channel

      برائے (; i+7 < گنتی; i += 8) {
         //بھرو
         __m128i y_بائٹیں = _mm_loadl_epi64((__m128i *) (y+i));
         __m128i cr_بائٹیں = _mm_loadl_epi64((__m128i *) (pcr+i));
         __m128i cb_بائٹیں = _mm_loadl_epi64((__m128i *) (pcb+i));
         __m128i cr_biased = _mm_xor_si128(cr_بائٹیں, signflip); // -128
         __m128i cb_biased = _mm_xor_si128(cb_بائٹیں, signflip); // -128

         // unpack to short (and left-shift cr, cb by 8)
         __m128i yw  = _mm_unpacklo_epi8(y_bias, y_بائٹیں);
         __m128i crw = _mm_unpacklo_epi8(_mm_setzero_si128(), cr_biased);
         __m128i cbw = _mm_unpacklo_epi8(_mm_setzero_si128(), cb_biased);

         // color transform
         __m128i yws = _mm_srli_epi16(yw, 4);
         __m128i cr0 = _mm_mulhi_epi16(cr_const0, crw);
         __m128i cb0 = _mm_mulhi_epi16(cb_const0, cbw);
         __m128i cb1 = _mm_mulhi_epi16(cbw, cb_const1);
         __m128i cr1 = _mm_mulhi_epi16(crw, cr_const1);
         __m128i rws = mm_جوڑو_epi16(cr0, yws);
         __m128i gwt = mm_جوڑو_epi16(cb0, yws);
         __m128i bws = mm_جوڑو_epi16(yws, cb1);
         __m128i gws = mm_جوڑو_epi16(gwt, cr1);

         // descale
         __m128i rw = _mm_srai_epi16(rws, 4);
         __m128i bw = _mm_srai_epi16(bws, 4);
         __m128i gw = _mm_srai_epi16(gws, 4);

         // back to byte, set up for transpose
         __m128i brb = _mm_packus_epi16(rw, bw);
         __m128i gxb = _mm_packus_epi16(gw, xw);

         // transpose to interleave channels
         __m128i t0 = _mm_unpacklo_epi8(brb, gxb);
         __m128i t1 = _mm_unpackhi_epi8(brb, gxb);
         __m128i o0 = _mm_unpacklo_epi16(t0, t1);
         __m128i o1 = _mm_unpackhi_epi16(t0, t1);

         // store
         _mm_storeu_si128((__m128i *) (برآمد + 0), o0);
         _mm_storeu_si128((__m128i *) (برآمد + 16), o1);
         برآمد += 32;
      }
   }
#اگر_ختم

#اگر_تعریف_شدہ STBI_NEON
   // in this version, step=3 support would be easy to add. but is there demand?
   اگر (قدم == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      uint8x8_t signflip = vdup_n_u8(0x80);
      int16x8_t cr_const0 = vdupq_n_s16(   (مختصر) ( 1.40200f*4096.0f+0.5f));
      int16x8_t cr_const1 = vdupq_n_s16( - (مختصر) ( 0.71414f*4096.0f+0.5f));
      int16x8_t cb_const0 = vdupq_n_s16( - (مختصر) ( 0.34414f*4096.0f+0.5f));
      int16x8_t cb_const1 = vdupq_n_s16(   (مختصر) ( 1.77200f*4096.0f+0.5f));

      برائے (; i+7 < گنتی; i += 8) {
         //بھرو
         uint8x8_t y_بائٹیں  = vld1_u8(y + i);
         uint8x8_t cr_بائٹیں = vld1_u8(pcr + i);
         uint8x8_t cb_بائٹیں = vld1_u8(pcb + i);
         int8x8_t cr_biased = vreinterpret_s8_u8(vsub_u8(cr_بائٹیں, signflip));
         int8x8_t cb_biased = vreinterpret_s8_u8(vsub_u8(cb_بائٹیں, signflip));

         // expand to s16
         int16x8_t yws = vreinterpretq_s16_u16(vshll_n_u8(y_بائٹیں, 4));
         int16x8_t crw = vshll_n_s8(cr_biased, 7);
         int16x8_t cbw = vshll_n_s8(cb_biased, 7);

         // color transform
         int16x8_t cr0 = vqdmulhq_s16(crw, cr_const0);
         int16x8_t cb0 = vqdmulhq_s16(cbw, cb_const0);
         int16x8_t cr1 = vqdmulhq_s16(crw, cr_const1);
         int16x8_t cb1 = vqdmulhq_s16(cbw, cb_const1);
         int16x8_t rws = vaddq_s16(yws, cr0);
         int16x8_t gws = vaddq_s16(vaddq_s16(yws, cb0), cr1);
         int16x8_t bws = vaddq_s16(yws, cb1);

         // undo scaling, round, convert to byte
         uint8x8x4_t o;
         o.val[0] = vqrshrun_n_s16(rws, 4);
         o.val[1] = vqrshrun_n_s16(gws, 4);
         o.val[2] = vqrshrun_n_s16(bws, 4);
         o.val[3] = vdup_n_u8(255);

         // store, interleaving r/g/b/a
         vst4_u8(برآمد, o);
         برآمد += 8*4;
      }
   }
#اگر_ختم

   برائے (; i < گنتی; ++i) {
      صحیح y_fixed = (y[i] << 20) + (1<<19); // rounding
      صحیح r,g,b;
      صحیح cr = pcr[i] - 128;
      صحیح cb = pcb[i] - 128;
      r = y_fixed + cr* stbi__float2fixed(1.40200f);
      g = y_fixed + cr*-stbi__float2fixed(0.71414f) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                   +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      اگر ((غیرعلامتی) r > 255) { اگر (r < 0) r = 0; ورنہ r = 255; }
      اگر ((غیرعلامتی) g > 255) { اگر (g < 0) g = 0; ورنہ g = 255; }
      اگر ((غیرعلامتی) b > 255) { اگر (b < 0) b = 0; ورنہ b = 255; }
      برآمد[0] = (stbi_uc)r;
      برآمد[1] = (stbi_uc)g;
      برآمد[2] = (stbi_uc)b;
      برآمد[3] = 255;
      برآمد += قدم;
   }
}
#اگر_ختم

// set up the kernels
ساکن خالی stbi_تیاری_jpeg(stbi__jpeg *j)
{
   j->idct_بلاک_kernel = stbi_idct_بلاک;
   j->YCb_Cr_مقصد_RGB_kernel = stbi_YCb_Cr_مقصد_RGB_صف;
   j->resample_صف_hv_2_kernel = stbi_resample_صف_hv_2;

#اگر_تعریف_شدہ STBI_SSE2
   اگر (stbi_sse2_دستیاب()) {
      j->idct_بلاک_kernel = stbi__idct_simd;
      j->YCb_Cr_مقصد_RGB_kernel = stbi_YCb_Cr_مقصد_RGB_simd;
      j->resample_صف_hv_2_kernel = stbi_resample_صف_hv_2_simd;
   }
#اگر_ختم

#اگر_تعریف_شدہ STBI_NEON
   j->idct_بلاک_kernel = stbi__idct_simd;
   j->YCb_Cr_مقصد_RGB_kernel = stbi_YCb_Cr_مقصد_RGB_simd;
   j->resample_صف_hv_2_kernel = stbi_resample_صف_hv_2_simd;
#اگر_ختم
}

// clean up the temporary component buffers
ساکن خالی stbi__cleanup_jpeg(stbi__jpeg *j)
{
   stbi__free_jpeg_components(j, j->s->img_n, 0);
}

قسمتعریف ساخت
{
   resample_صف_func resample;
   stbi_uc *line0,*line1;
   صحیح hs,vs;   // expansion factor in each axis
   صحیح w_lores; // horizontal pixels pre-expansion
   صحیح ystep;   // how far through vertical expansion we are
   صحیح ypos;    // which pre-expansion row we're on
} stbi__resample;

// fast 0..255 * 0..255 => 0..255 rounded multiplication
ساکن stbi_uc stbi__blinn_8x8(stbi_uc x, stbi_uc y)
{
   غیرعلامتی صحیح t = x*y + 128;
   واپس (stbi_uc) ((t + (t >>8)) >> 8);
}

ساکن stbi_uc *بھرو_jpeg_image(stbi__jpeg *z, صحیح *برآمد_x, صحیح *برآمد_y, صحیح *comp, صحیح req_comp)
{
   صحیح n, decode_n, is_rgb;
   z->s->img_n = 0; // make stbi__cleanup_jpeg safe

   // validate req_comp
   اگر (req_comp < 0 || req_comp > 4) واپس stbi__errpuc("bad req_comp", "Internal error");

   // load a jpeg image from whichever source, but leave in YCbCr format
   اگر (!stbi__decode_jpeg_image(z)) { stbi__cleanup_jpeg(z); واپس تہی; }

   // determine actual number of components to generate
   n = req_comp ? req_comp : z->s->img_n >= 3 ? 3 : 1;

   is_rgb = z->s->img_n == 3 && (z->rgb == 3 || (z->app14_color_transform == 0 && !z->jfif));

   اگر (z->s->img_n == 3 && n < 3 && !is_rgb)
      decode_n = 1;
   ورنہ
      decode_n = z->s->img_n;

   // resample and color-convert
   {
      صحیح k;
      غیرعلامتی صحیح i,j;
      stbi_uc *output;
      stbi_uc *coutput[4];

      stbi__resample res_comp[4];

      برائے (k=0; k < decode_n; ++k) {
         stbi__resample *r = &res_comp[k];

         // allocate line buffer big enough for upsampling off the edges
         // with upsample factor of 4
         z->img_comp[k].linebuf = (stbi_uc *) stbi__malloc(z->s->img_x + 3);
         اگر (!z->img_comp[k].linebuf) { stbi__cleanup_jpeg(z); واپس stbi__errpuc("outofmem", "Out of memory"); }

         r->hs      = z->img_h_max / z->img_comp[k].h;
         r->vs      = z->img_v_max / z->img_comp[k].v;
         r->ystep   = r->vs >> 1;
         r->w_lores = (z->s->img_x + r->hs-1) / r->hs;
         r->ypos    = 0;
         r->line0   = r->line1 = z->img_comp[k].ڈیٹا;

         اگر      (r->hs == 1 && r->vs == 1) r->resample = resample_صف_1;
         ورنہ اگر (r->hs == 1 && r->vs == 2) r->resample = stbi_resample_صف_v_2;
         ورنہ اگر (r->hs == 2 && r->vs == 1) r->resample = stbi_resample_صف_h_2;
         ورنہ اگر (r->hs == 2 && r->vs == 2) r->resample = z->resample_صف_hv_2_kernel;
         ورنہ                               r->resample = stbi_resample_صف_generic;
      }

      // can't error after this so, this is safe
      output = (stbi_uc *) stbi__malloc_mad3(n, z->s->img_x, z->s->img_y, 1);
      اگر (!output) { stbi__cleanup_jpeg(z); واپس stbi__errpuc("outofmem", "Out of memory"); }

      // now go ahead and resample
      برائے (j=0; j < z->s->img_y; ++j) {
         stbi_uc *برآمد = output + n * z->s->img_x * j;
         برائے (k=0; k < decode_n; ++k) {
            stbi__resample *r = &res_comp[k];
            صحیح y_bot = r->ystep >= (r->vs >> 1);
            coutput[k] = r->resample(z->img_comp[k].linebuf,
                                     y_bot ? r->line1 : r->line0,
                                     y_bot ? r->line0 : r->line1,
                                     r->w_lores, r->hs);
            اگر (++r->ystep >= r->vs) {
               r->ystep = 0;
               r->line0 = r->line1;
               اگر (++r->ypos < z->img_comp[k].y)
                  r->line1 += z->img_comp[k].w2;
            }
         }
         اگر (n >= 3) {
            stbi_uc *y = coutput[0];
            اگر (z->s->img_n == 3) {
               اگر (is_rgb) {
                  برائے (i=0; i < z->s->img_x; ++i) {
                     برآمد[0] = y[i];
                     برآمد[1] = coutput[1][i];
                     برآمد[2] = coutput[2][i];
                     برآمد[3] = 255;
                     برآمد += n;
                  }
               } ورنہ {
                  z->YCb_Cr_مقصد_RGB_kernel(برآمد, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } ورنہ اگر (z->s->img_n == 4) {
               اگر (z->app14_color_transform == 0) { // CMYK
                  برائے (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     برآمد[0] = stbi__blinn_8x8(coutput[0][i], m);
                     برآمد[1] = stbi__blinn_8x8(coutput[1][i], m);
                     برآمد[2] = stbi__blinn_8x8(coutput[2][i], m);
                     برآمد[3] = 255;
                     برآمد += n;
                  }
               } ورنہ اگر (z->app14_color_transform == 2) { // YCCK
                  z->YCb_Cr_مقصد_RGB_kernel(برآمد, y, coutput[1], coutput[2], z->s->img_x, n);
                  برائے (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     برآمد[0] = stbi__blinn_8x8(255 - برآمد[0], m);
                     برآمد[1] = stbi__blinn_8x8(255 - برآمد[1], m);
                     برآمد[2] = stbi__blinn_8x8(255 - برآمد[2], m);
                     برآمد += n;
                  }
               } ورنہ { // YCbCr + alpha?  Ignore the fourth channel for now
                  z->YCb_Cr_مقصد_RGB_kernel(برآمد, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } ورنہ
               برائے (i=0; i < z->s->img_x; ++i) {
                  برآمد[0] = برآمد[1] = برآمد[2] = y[i];
                  برآمد[3] = 255; // not used if n==3
                  برآمد += n;
               }
         } ورنہ {
            اگر (is_rgb) {
               اگر (n == 1)
                  برائے (i=0; i < z->s->img_x; ++i)
                     *برآمد++ = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
               ورنہ {
                  برائے (i=0; i < z->s->img_x; ++i, برآمد += 2) {
                     برآمد[0] = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
                     برآمد[1] = 255;
                  }
               }
            } ورنہ اگر (z->s->img_n == 4 && z->app14_color_transform == 0) {
               برائے (i=0; i < z->s->img_x; ++i) {
                  stbi_uc m = coutput[3][i];
                  stbi_uc r = stbi__blinn_8x8(coutput[0][i], m);
                  stbi_uc g = stbi__blinn_8x8(coutput[1][i], m);
                  stbi_uc b = stbi__blinn_8x8(coutput[2][i], m);
                  برآمد[0] = stbi__compute_y(r, g, b);
                  برآمد[1] = 255;
                  برآمد += n;
               }
            } ورنہ اگر (z->s->img_n == 4 && z->app14_color_transform == 2) {
               برائے (i=0; i < z->s->img_x; ++i) {
                  برآمد[0] = stbi__blinn_8x8(255 - coutput[0][i], coutput[3][i]);
                  برآمد[1] = 255;
                  برآمد += n;
               }
            } ورنہ {
               stbi_uc *y = coutput[0];
               اگر (n == 1)
                  برائے (i=0; i < z->s->img_x; ++i) برآمد[i] = y[i];
               ورنہ
                  برائے (i=0; i < z->s->img_x; ++i) *برآمد++ = y[i], *برآمد++ = 255;
            }
         }
      }
      stbi__cleanup_jpeg(z);
      *برآمد_x = z->s->img_x;
      *برآمد_y = z->s->img_y;
      اگر (comp) *comp = z->s->img_n >= 3 ? 3 : 1; // report original components, not output
      واپس output;
   }
}

ساکن خالی *stbi_jpeg_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   غیرعلامتی حرف* نتیجہ;
   stbi__jpeg* j = (stbi__jpeg*) stbi__malloc(حجم(stbi__jpeg));
   STBI_NOTUSED(ri);
   j->s = s;
   stbi_تیاری_jpeg(j);
   نتیجہ = بھرو_jpeg_image(j, x,y,comp,req_comp);
   STBI_FREE(j);
   واپس نتیجہ;
}

ساکن صحیح stbi__jpeg_test(stbi__context *s)
{
   صحیح r;
   stbi__jpeg* j = (stbi__jpeg*)stbi__malloc(حجم(stbi__jpeg));
   j->s = s;
   stbi_تیاری_jpeg(j);
   r = stbi_decode_jpeg_سرخی(j, STBI_جائزہ_type);
   stbi__rewind(s);
   STBI_FREE(j);
   واپس r;
}

ساکن صحیح stbi__jpeg_info_raw(stbi__jpeg *j, صحیح *x, صحیح *y, صحیح *comp)
{
   اگر (!stbi_decode_jpeg_سرخی(j, STBI_جائزہ_سرخی)) {
      stbi__rewind( j->s );
      واپس 0;
   }
   اگر (x) *x = j->s->img_x;
   اگر (y) *y = j->s->img_y;
   اگر (comp) *comp = j->s->img_n >= 3 ? 3 : 1;
   واپس 1;
}

ساکن صحیح stbi__jpeg_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   صحیح نتیجہ;
   stbi__jpeg* j = (stbi__jpeg*) (stbi__malloc(حجم(stbi__jpeg)));
   j->s = s;
   نتیجہ = stbi__jpeg_info_raw(j, x, y, comp);
   STBI_FREE(j);
   واپس نتیجہ;
}
#اگر_ختم

// public domain zlib decode    v0.2  Sean Barrett 2006-11-18
//    simple implementation
//      - all input must be provided in an upfront buffer
//      - all output is written to a single output buffer (can malloc/realloc)
//    performance
//      - fast huffman

#اگر_غیر_تعریف_شدہ STBI_NO_ZLIB

// fast-way is faster to check than jpeg huffman, but slow way is slower
#تعریف_کرو STBI_ZFAST_بٹ  9 // accelerate all cases in default tables
#تعریف_کرو STBI_ZFAST_نقاب  ((1 << STBI_ZFAST_بٹ) - 1)

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
قسمتعریف ساخت
{
   stbi__uint16 fast[1 << STBI_ZFAST_بٹ];
   stbi__uint16 firstcode[16];
   صحیح maxcode[17];
   stbi__uint16 firstsymbol[16];
   stbi_uc  سائز[288];
   stbi__uint16 قدر[288];
} stbi__zhuffman;

stbi_inline ساکن صحیح stbi__bitreverse16(صحیح n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  واپس n;
}

stbi_inline ساکن صحیح stbi_ثنائی_ہندسہ_الٹ(صحیح v, صحیح بٹ)
{
   STBI_ASSERT(بٹ <= 16);
   // to bit reverse n bits, reverse 16 and shift
   // e.g. 11 bits, bit reverse and shift away 5
   واپس stbi__bitreverse16(v) >> (16-بٹ);
}

ساکن صحیح stbi__zbuild_huffman(stbi__zhuffman *z, ثابت stbi_uc *sizelist, صحیح عدد)
{
   صحیح i,k=0;
   صحیح رمز, اگلا_رمز[16], sizes[17];

   // DEFLATE spec for generating codes
   memset(sizes, 0, حجم(sizes));
   memset(z->fast, 0, حجم(z->fast));
   برائے (i=0; i < عدد; ++i)
      ++sizes[sizelist[i]];
   sizes[0] = 0;
   برائے (i=1; i < 16; ++i)
      اگر (sizes[i] > (1 << i))
         واپس stbi__err("bad sizes", "Corrupt PNG");
   رمز = 0;
   برائے (i=1; i < 16; ++i) {
      اگلا_رمز[i] = رمز;
      z->firstcode[i] = (stbi__uint16) رمز;
      z->firstsymbol[i] = (stbi__uint16) k;
      رمز = (رمز + sizes[i]);
      اگر (sizes[i])
         اگر (رمز-1 >= (1 << i)) واپس stbi__err("bad codelengths","Corrupt PNG");
      z->maxcode[i] = رمز << (16-i); // preshift for inner loop
      رمز <<= 1;
      k += sizes[i];
   }
   z->maxcode[16] = 0x10000; // sentinel
   برائے (i=0; i < عدد; ++i) {
      صحیح s = sizelist[i];
      اگر (s) {
         صحیح c = اگلا_رمز[s] - z->firstcode[s] + z->firstsymbol[s];
         stbi__uint16 fastv = (stbi__uint16) ((s << 9) | i);
         z->سائز [c] = (stbi_uc     ) s;
         z->قدر[c] = (stbi__uint16) i;
         اگر (s <= STBI_ZFAST_بٹ) {
            صحیح j = stbi_ثنائی_ہندسہ_الٹ(اگلا_رمز[s],s);
            جب (j < (1 << STBI_ZFAST_بٹ)) {
               z->fast[j] = fastv;
               j += (1 << s);
            }
         }
         ++اگلا_رمز[s];
      }
   }
   واپس 1;
}

// zlib-from-memory implementation for PNG reading
//    because PNG allows splitting the zlib stream arbitrarily,
//    and it's annoying structurally to have PNG call ZLIB call PNG,
//    we require PNG read all the IDATs and combine them into a single
//    memory buffer

قسمتعریف ساخت
{
   stbi_uc *zbuffer, *zbuffer_انتہا;
   صحیح عدد_بٹ;
   stbi__uint32 رمز_بفر;

   حرف *zout;
   حرف *zout_آغاز;
   حرف *zout_انتہا;
   صحیح   z_expandable;

   stbi__zhuffman z_لمبائی, z_فاصلہ;
} stbi__zbuf;

stbi_inline ساکن stbi_uc stbi__zget8(stbi__zbuf *z)
{
   اگر (z->zbuffer >= z->zbuffer_انتہا) واپس 0;
   واپس *z->zbuffer++;
}

ساکن خالی stbi_fill_بٹ(stbi__zbuf *z)
{
   کرو {
      STBI_ASSERT(z->رمز_بفر < (1U << z->عدد_بٹ));
      z->رمز_بفر |= (غیرعلامتی صحیح) stbi__zget8(z) << z->عدد_بٹ;
      z->عدد_بٹ += 8;
   } جب (z->عدد_بٹ <= 24);
}

stbi_inline ساکن غیرعلامتی صحیح stbi__zreceive(stbi__zbuf *z, صحیح n)
{
   غیرعلامتی صحیح k;
   اگر (z->عدد_بٹ < n) stbi_fill_بٹ(z);
   k = z->رمز_بفر & ((1 << n) - 1);
   z->رمز_بفر >>= n;
   z->عدد_بٹ -= n;
   واپس k;
}

ساکن صحیح stbi__zhuffman_decode_slowpath(stbi__zbuf *a, stbi__zhuffman *z)
{
   صحیح b,s,k;
   // not resolved by fast table, so compute it the slow way
   // use jpeg approach, which requires MSbits at top
   k = stbi_ثنائی_ہندسہ_الٹ(a->رمز_بفر, 16);
   برائے (s=STBI_ZFAST_بٹ+1; ; ++s)
      اگر (k < z->maxcode[s])
         توڑو;
   اگر (s == 16) واپس -1; // invalid code!
   // code size is s, so:
   b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
   STBI_ASSERT(z->سائز[b] == s);
   a->رمز_بفر >>= s;
   a->عدد_بٹ -= s;
   واپس z->قدر[b];
}

stbi_inline ساکن صحیح stbi__zhuffman_decode(stbi__zbuf *a, stbi__zhuffman *z)
{
   صحیح b,s;
   اگر (a->عدد_بٹ < 16) stbi_fill_بٹ(a);
   b = z->fast[a->رمز_بفر & STBI_ZFAST_نقاب];
   اگر (b) {
      s = b >> 9;
      a->رمز_بفر >>= s;
      a->عدد_بٹ -= s;
      واپس b & 511;
   }
   واپس stbi__zhuffman_decode_slowpath(a, z);
}

ساکن صحیح stbi__zexpand(stbi__zbuf *z, حرف *zout, صحیح n)  // need to make room for n bytes
{
   حرف *q;
   صحیح cur, حد_بندی, old_حد_بندی;
   z->zout = zout;
   اگر (!z->z_expandable) واپس stbi__err("output buffer limit","Corrupt PNG");
   cur   = (صحیح) (z->zout     - z->zout_آغاز);
   حد_بندی = old_حد_بندی = (صحیح) (z->zout_انتہا - z->zout_آغاز);
   جب (cur + n > حد_بندی)
      حد_بندی *= 2;
   q = (حرف *) STBI_REALLOC_SIZED(z->zout_آغاز, old_حد_بندی, حد_بندی);
   STBI_NOTUSED(old_حد_بندی);
   اگر (q == تہی) واپس stbi__err("outofmem", "Out of memory");
   z->zout_آغاز = q;
   z->zout       = q + cur;
   z->zout_انتہا   = q + حد_بندی;
   واپس 1;
}

ساکن ثابت صحیح stbi_zlength_بنیاد[31] = {
   3,4,5,6,7,8,9,10,11,13,
   15,17,19,23,27,31,35,43,51,59,
   67,83,99,115,131,163,195,227,258,0,0 };

ساکن ثابت صحیح stbi__zlength_extra[31]=
{ 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };

ساکن ثابت صحیح stbi_zdist_بنیاد[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0};

ساکن ثابت صحیح stbi__zdist_extra[32] =
{ 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

ساکن صحیح stbi_parse_huffman_بلاک(stbi__zbuf *a)
{
   حرف *zout = a->zout;
   برائے(;;) {
      صحیح z = stbi__zhuffman_decode(a, &a->z_لمبائی);
      اگر (z < 256) {
         اگر (z < 0) واپس stbi__err("bad huffman code","Corrupt PNG"); // error in huffman codes
         اگر (zout >= a->zout_انتہا) {
            اگر (!stbi__zexpand(a, zout, 1)) واپس 0;
            zout = a->zout;
         }
         *zout++ = (حرف) z;
      } ورنہ {
         stbi_uc *p;
         صحیح len,dist;
         اگر (z == 256) {
            a->zout = zout;
            واپس 1;
         }
         z -= 257;
         len = stbi_zlength_بنیاد[z];
         اگر (stbi__zlength_extra[z]) len += stbi__zreceive(a, stbi__zlength_extra[z]);
         z = stbi__zhuffman_decode(a, &a->z_فاصلہ);
         اگر (z < 0) واپس stbi__err("bad huffman code","Corrupt PNG");
         dist = stbi_zdist_بنیاد[z];
         اگر (stbi__zdist_extra[z]) dist += stbi__zreceive(a, stbi__zdist_extra[z]);
         اگر (zout - a->zout_آغاز < dist) واپس stbi__err("bad dist","Corrupt PNG");
         اگر (zout + len > a->zout_انتہا) {
            اگر (!stbi__zexpand(a, zout, len)) واپس 0;
            zout = a->zout;
         }
         p = (stbi_uc *) (zout - dist);
         اگر (dist == 1) { // run of one byte; common in images.
            stbi_uc v = *p;
            اگر (len) { کرو *zout++ = v; جب (--len); }
         } ورنہ {
            اگر (len) { کرو *zout++ = *p++; جب (--len); }
         }
      }
   }
}

ساکن صحیح stbi__compute_huffman_codes(stbi__zbuf *a)
{
   ساکن ثابت stbi_uc لمبائی_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
   stbi__zhuffman z_codelength;
   stbi_uc lencodes[286+32+137];//padding for maximum single op
   stbi_uc codelength_sizes[19];
   صحیح i,n;

   صحیح hlit  = stbi__zreceive(a,5) + 257;
   صحیح hdist = stbi__zreceive(a,5) + 1;
   صحیح hclen = stbi__zreceive(a,4) + 4;
   صحیح ntot  = hlit + hdist;

   memset(codelength_sizes, 0, حجم(codelength_sizes));
   برائے (i=0; i < hclen; ++i) {
      صحیح s = stbi__zreceive(a,3);
      codelength_sizes[لمبائی_dezigzag[i]] = (stbi_uc) s;
   }
   اگر (!stbi__zbuild_huffman(&z_codelength, codelength_sizes, 19)) واپس 0;

   n = 0;
   جب (n < ntot) {
      صحیح c = stbi__zhuffman_decode(a, &z_codelength);
      اگر (c < 0 || c >= 19) واپس stbi__err("bad codelengths", "Corrupt PNG");
      اگر (c < 16)
         lencodes[n++] = (stbi_uc) c;
      ورنہ {
         stbi_uc fill = 0;
         اگر (c == 16) {
            c = stbi__zreceive(a,2)+3;
            اگر (n == 0) واپس stbi__err("bad codelengths", "Corrupt PNG");
            fill = lencodes[n-1];
         } ورنہ اگر (c == 17)
            c = stbi__zreceive(a,3)+3;
         ورنہ {
            STBI_ASSERT(c == 18);
            c = stbi__zreceive(a,7)+11;
         }
         اگر (ntot - n < c) واپس stbi__err("bad codelengths", "Corrupt PNG");
         memset(lencodes+n, fill, c);
         n += c;
      }
   }
   اگر (n != ntot) واپس stbi__err("bad codelengths","Corrupt PNG");
   اگر (!stbi__zbuild_huffman(&a->z_لمبائی, lencodes, hlit)) واپس 0;
   اگر (!stbi__zbuild_huffman(&a->z_فاصلہ, lencodes+hlit, hdist)) واپس 0;
   واپس 1;
}

ساکن صحیح stbi_parse_uncompressed_بلاک(stbi__zbuf *a)
{
   stbi_uc سرخی[4];
   صحیح len,nlen,k;
   اگر (a->عدد_بٹ & 7)
      stbi__zreceive(a, a->عدد_بٹ & 7); // discard
   // drain the bit-packed data into header
   k = 0;
   جب (a->عدد_بٹ > 0) {
      سرخی[k++] = (stbi_uc) (a->رمز_بفر & 255); // suppress MSVC run-time check
      a->رمز_بفر >>= 8;
      a->عدد_بٹ -= 8;
   }
   STBI_ASSERT(a->عدد_بٹ == 0);
   // now fill header the normal way
   جب (k < 4)
      سرخی[k++] = stbi__zget8(a);
   len  = سرخی[1] * 256 + سرخی[0];
   nlen = سرخی[3] * 256 + سرخی[2];
   اگر (nlen != (len ^ 0xffff)) واپس stbi__err("zlib corrupt","Corrupt PNG");
   اگر (a->zbuffer + len > a->zbuffer_انتہا) واپس stbi__err("read past buffer","Corrupt PNG");
   اگر (a->zout + len > a->zout_انتہا)
      اگر (!stbi__zexpand(a, a->zout, len)) واپس 0;
   حافظہ_نقل(a->zout, a->zbuffer, len);
   a->zbuffer += len;
   a->zout += len;
   واپس 1;
}

ساکن صحیح stbi_parse_zlib_سرخی(stbi__zbuf *a)
{
   صحیح cmf   = stbi__zget8(a);
   صحیح cm    = cmf & 15;
   /* int cinfo = cmf >> 4; */
   صحیح flg   = stbi__zget8(a);
   اگر ((cmf*256+flg) % 31 != 0) واپس stbi__err("bad zlib header","Corrupt PNG"); // zlib spec
   اگر (flg & 32) واپس stbi__err("no preset dict","Corrupt PNG"); // preset dictionary not allowed in png
   اگر (cm != 8) واپس stbi__err("bad compression","Corrupt PNG"); // DEFLATE required for png
   // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
   واپس 1;
}

ساکن ثابت stbi_uc stbi_zdefault_لمبائی[288] =
{
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8
};
ساکن ثابت stbi_uc stbi_zdefault_فاصلہ[32] =
{
   5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};
/*
Init algorithm:
{
   int i;   // use <= to match clearly with spec
   for (i=0; i <= 143; ++i)     stbi__zdefault_length[i]   = 8;
   for (   ; i <= 255; ++i)     stbi__zdefault_length[i]   = 9;
   for (   ; i <= 279; ++i)     stbi__zdefault_length[i]   = 7;
   for (   ; i <= 287; ++i)     stbi__zdefault_length[i]   = 8;

   for (i=0; i <=  31; ++i)     stbi__zdefault_distance[i] = 5;
}
*/

ساکن صحیح stbi__parse_zlib(stbi__zbuf *a, صحیح parse_سرخی)
{
   صحیح final, type;
   اگر (parse_سرخی)
      اگر (!stbi_parse_zlib_سرخی(a)) واپس 0;
   a->عدد_بٹ = 0;
   a->رمز_بفر = 0;
   کرو {
      final = stbi__zreceive(a,1);
      type = stbi__zreceive(a,2);
      اگر (type == 0) {
         اگر (!stbi_parse_uncompressed_بلاک(a)) واپس 0;
      } ورنہ اگر (type == 3) {
         واپس 0;
      } ورنہ {
         اگر (type == 1) {
            // use fixed code lengths
            اگر (!stbi__zbuild_huffman(&a->z_لمبائی  , stbi_zdefault_لمبائی  , 288)) واپس 0;
            اگر (!stbi__zbuild_huffman(&a->z_فاصلہ, stbi_zdefault_فاصلہ,  32)) واپس 0;
         } ورنہ {
            اگر (!stbi__compute_huffman_codes(a)) واپس 0;
         }
         اگر (!stbi_parse_huffman_بلاک(a)) واپس 0;
      }
   } جب (!final);
   واپس 1;
}

ساکن صحیح stbi__do_zlib(stbi__zbuf *a, حرف *obuf, صحیح olen, صحیح قوت_نما, صحیح parse_سرخی)
{
   a->zout_آغاز = obuf;
   a->zout       = obuf;
   a->zout_انتہا   = obuf + olen;
   a->z_expandable = قوت_نما;

   واپس stbi__parse_zlib(a, parse_سرخی);
}

STBIDEF حرف *stbi_zlib_decode_malloc_guesssize(ثابت حرف *بفر, صحیح len, صحیح initial_سائز, صحیح *outlen)
{
   stbi__zbuf a;
   حرف *p = (حرف *) stbi__malloc(initial_سائز);
   اگر (p == تہی) واپس تہی;
   a.zbuffer = (stbi_uc *) بفر;
   a.zbuffer_انتہا = (stbi_uc *) بفر + len;
   اگر (stbi__do_zlib(&a, p, initial_سائز, 1, 1)) {
      اگر (outlen) *outlen = (صحیح) (a.zout - a.zout_آغاز);
      واپس a.zout_آغاز;
   } ورنہ {
      STBI_FREE(a.zout_آغاز);
      واپس تہی;
   }
}

STBIDEF حرف *stbi_zlib_decode_malloc(حرف ثابت *بفر, صحیح len, صحیح *outlen)
{
   واپس stbi_zlib_decode_malloc_guesssize(بفر, len, 16384, outlen);
}

STBIDEF حرف *stbi_zlib_decode_malloc_guesssize_headerflag(ثابت حرف *بفر, صحیح len, صحیح initial_سائز, صحیح *outlen, صحیح parse_سرخی)
{
   stbi__zbuf a;
   حرف *p = (حرف *) stbi__malloc(initial_سائز);
   اگر (p == تہی) واپس تہی;
   a.zbuffer = (stbi_uc *) بفر;
   a.zbuffer_انتہا = (stbi_uc *) بفر + len;
   اگر (stbi__do_zlib(&a, p, initial_سائز, 1, parse_سرخی)) {
      اگر (outlen) *outlen = (صحیح) (a.zout - a.zout_آغاز);
      واپس a.zout_آغاز;
   } ورنہ {
      STBI_FREE(a.zout_آغاز);
      واپس تہی;
   }
}

STBIDEF صحیح stbi_zlib_decode_بفر(حرف *obuffer, صحیح olen, حرف ثابت *ibuffer, صحیح ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_انتہا = (stbi_uc *) ibuffer + ilen;
   اگر (stbi__do_zlib(&a, obuffer, olen, 0, 1))
      واپس (صحیح) (a.zout - a.zout_آغاز);
   ورنہ
      واپس -1;
}

STBIDEF حرف *stbi_zlib_decode_noheader_malloc(حرف ثابت *بفر, صحیح len, صحیح *outlen)
{
   stbi__zbuf a;
   حرف *p = (حرف *) stbi__malloc(16384);
   اگر (p == تہی) واپس تہی;
   a.zbuffer = (stbi_uc *) بفر;
   a.zbuffer_انتہا = (stbi_uc *) بفر+len;
   اگر (stbi__do_zlib(&a, p, 16384, 1, 0)) {
      اگر (outlen) *outlen = (صحیح) (a.zout - a.zout_آغاز);
      واپس a.zout_آغاز;
   } ورنہ {
      STBI_FREE(a.zout_آغاز);
      واپس تہی;
   }
}

STBIDEF صحیح stbi_zlib_decode_noheader_بفر(حرف *obuffer, صحیح olen, ثابت حرف *ibuffer, صحیح ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_انتہا = (stbi_uc *) ibuffer + ilen;
   اگر (stbi__do_zlib(&a, obuffer, olen, 0, 0))
      واپس (صحیح) (a.zout - a.zout_آغاز);
   ورنہ
      واپس -1;
}
#اگر_ختم

// public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
//    simple implementation
//      - only 8-bit samples
//      - no CRC checking
//      - allocates lots of intermediate memory
//        - avoids problem of streaming data between subsystems
//        - avoids explicit window management
//    performance
//      - uses stb_zlib, a PD zlib implementation with fast huffman decoding

#اگر_غیر_تعریف_شدہ STBI_NO_PNG
قسمتعریف ساخت
{
   stbi__uint32 لمبائی;
   stbi__uint32 type;
} stbi__pngchunk;

ساکن stbi__pngchunk stbi_get_chunk_سرخی(stbi__context *s)
{
   stbi__pngchunk c;
   c.لمبائی = stbi__get32be(s);
   c.type   = stbi__get32be(s);
   واپس c;
}

ساکن صحیح stbi_check_png_سرخی(stbi__context *s)
{
   ساکن ثابت stbi_uc png_sig[8] = { 137,80,78,71,13,10,26,10 };
   صحیح i;
   برائے (i=0; i < 8; ++i)
      اگر (stbi__get8(s) != png_sig[i]) واپس stbi__err("bad png sig","Not a PNG");
   واپس 1;
}

قسمتعریف ساخت
{
   stbi__context *s;
   stbi_uc *idata, *expanded, *برآمد;
   صحیح گہرائی;
} stbi__png;


شمار {
   STBI__F_none=0,
   STBI__F_sub=1,
   STBI_F_اوپر=2,
   STBI__F_avg=3,
   STBI__F_paeth=4,
   // synthetic filters used for first scanline to avoid needing a dummy row of 0s
   STBI_F_avg_پہلا,
   STBI_F_paeth_پہلا
};

ساکن stbi_uc پہلا_صف_filter[5] =
{
   STBI__F_none,
   STBI__F_sub,
   STBI__F_none,
   STBI_F_avg_پہلا,
   STBI_F_paeth_پہلا
};

ساکن صحیح stbi__paeth(صحیح a, صحیح b, صحیح c)
{
   صحیح p = a + b - c;
   صحیح pa = مطلق_قدر(p-a);
   صحیح pb = مطلق_قدر(p-b);
   صحیح pc = مطلق_قدر(p-c);
   اگر (pa <= pb && pa <= pc) واپس a;
   اگر (pb <= pc) واپس b;
   واپس c;
}

ساکن ثابت stbi_uc stbi_گہرائی_scale_جدول[9] = { 0, 0xff, 0x55, 0, 0x11, 0,0,0, 0x01 };

// create the png data from post-deflated data
ساکن صحیح stbi__create_png_image_raw(stbi__png *a, stbi_uc *raw, stbi__uint32 raw_len, صحیح برآمد_n, stbi__uint32 x, stbi__uint32 y, صحیح گہرائی, صحیح color)
{
   صحیح بائٹیں = (گہرائی == 16? 2 : 1);
   stbi__context *s = a->s;
   stbi__uint32 i,j,stride = x*برآمد_n*بائٹیں;
   stbi__uint32 img_len, img_چوڑائی_بائٹیں;
   صحیح k;
   صحیح img_n = s->img_n; // copy it into a local for later

   صحیح output_بائٹیں = برآمد_n*بائٹیں;
   صحیح filter_بائٹیں = img_n*بائٹیں;
   صحیح چوڑائی = x;

   STBI_ASSERT(برآمد_n == s->img_n || برآمد_n == s->img_n+1);
   a->برآمد = (stbi_uc *) stbi__malloc_mad3(x, y, output_بائٹیں, 0); // extra bytes to write off the end into
   اگر (!a->برآمد) واپس stbi__err("outofmem", "Out of memory");

   اگر (!stbi__mad3sizes_valid(img_n, x, گہرائی, 7)) واپس stbi__err("too large", "Corrupt PNG");
   img_چوڑائی_بائٹیں = (((img_n * x * گہرائی) + 7) >> 3);
   img_len = (img_چوڑائی_بائٹیں + 1) * y;

   // we used to check for exact match between raw_len and img_len on non-interlaced PNGs,
   // but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),
   // so just check for raw_len < img_len always.
   اگر (raw_len < img_len) واپس stbi__err("not enough pixels","Corrupt PNG");

   برائے (j=0; j < y; ++j) {
      stbi_uc *cur = a->برآمد + stride*j;
      stbi_uc *پیشگی;
      صحیح filter = *raw++;

      اگر (filter > 4)
         واپس stbi__err("invalid filter","Corrupt PNG");

      اگر (گہرائی < 8) {
         STBI_ASSERT(img_چوڑائی_بائٹیں <= x);
         cur += x*برآمد_n - img_چوڑائی_بائٹیں; // store output to the rightmost img_len bytes, so we can decode in place
         filter_بائٹیں = 1;
         چوڑائی = img_چوڑائی_بائٹیں;
      }
      پیشگی = cur - stride; // bugfix: need to compute this after 'cur +=' computation above

      // if first row, use special filter that doesn't sample previous row
      اگر (j == 0) filter = پہلا_صف_filter[filter];

      // handle first byte explicitly
      برائے (k=0; k < filter_بائٹیں; ++k) {
         انتخاب (filter) {
            حالت STBI__F_none       : cur[k] = raw[k]; توڑو;
            حالت STBI__F_sub        : cur[k] = raw[k]; توڑو;
            حالت STBI_F_اوپر         : cur[k] = STBI__BYTECAST(raw[k] + پیشگی[k]); توڑو;
            حالت STBI__F_avg        : cur[k] = STBI__BYTECAST(raw[k] + (پیشگی[k]>>1)); توڑو;
            حالت STBI__F_paeth      : cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(0,پیشگی[k],0)); توڑو;
            حالت STBI_F_avg_پہلا  : cur[k] = raw[k]; توڑو;
            حالت STBI_F_paeth_پہلا: cur[k] = raw[k]; توڑو;
         }
      }

      اگر (گہرائی == 8) {
         اگر (img_n != برآمد_n)
            cur[img_n] = 255; // first pixel
         raw += img_n;
         cur += برآمد_n;
         پیشگی += برآمد_n;
      } ورنہ اگر (گہرائی == 16) {
         اگر (img_n != برآمد_n) {
            cur[filter_بائٹیں]   = 255; // first pixel top byte
            cur[filter_بائٹیں+1] = 255; // first pixel bottom byte
         }
         raw += filter_بائٹیں;
         cur += output_بائٹیں;
         پیشگی += output_بائٹیں;
      } ورنہ {
         raw += 1;
         cur += 1;
         پیشگی += 1;
      }

      // this is a little gross, so that we don't switch per-pixel or per-component
      اگر (گہرائی < 8 || img_n == برآمد_n) {
         صحیح nk = (چوڑائی - 1)*filter_بائٹیں;
         #تعریف_کرو STBI__CASE(f) \
             حالت f:     \
                برائے (k=0; k < nk; ++k)
         انتخاب (filter) {
            // "none" filter turns into a memcpy here; make that explicit.
            حالت STBI__F_none:         حافظہ_نقل(cur, raw, nk); توڑو;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k-filter_بائٹیں]); } توڑو;
            STBI__CASE(STBI_F_اوپر)           { cur[k] = STBI__BYTECAST(raw[k] + پیشگی[k]); } توڑو;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((پیشگی[k] + cur[k-filter_بائٹیں])>>1)); } توڑو;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_بائٹیں],پیشگی[k],پیشگی[k-filter_بائٹیں])); } توڑو;
            STBI__CASE(STBI_F_avg_پہلا)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k-filter_بائٹیں] >> 1)); } توڑو;
            STBI__CASE(STBI_F_paeth_پہلا)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_بائٹیں],0,0)); } توڑو;
         }
         #تعریف_منسوخ STBI__CASE
         raw += nk;
      } ورنہ {
         STBI_ASSERT(img_n+1 == برآمد_n);
         #تعریف_کرو STBI__CASE(f) \
             حالت f:     \
                برائے (i=x-1; i >= 1; --i, cur[filter_بائٹیں]=255,raw+=filter_بائٹیں,cur+=output_بائٹیں,پیشگی+=output_بائٹیں) \
                   برائے (k=0; k < filter_بائٹیں; ++k)
         انتخاب (filter) {
            STBI__CASE(STBI__F_none)         { cur[k] = raw[k]; } توڑو;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k- output_بائٹیں]); } توڑو;
            STBI__CASE(STBI_F_اوپر)           { cur[k] = STBI__BYTECAST(raw[k] + پیشگی[k]); } توڑو;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((پیشگی[k] + cur[k- output_بائٹیں])>>1)); } توڑو;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_بائٹیں],پیشگی[k],پیشگی[k- output_بائٹیں])); } توڑو;
            STBI__CASE(STBI_F_avg_پہلا)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k- output_بائٹیں] >> 1)); } توڑو;
            STBI__CASE(STBI_F_paeth_پہلا)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_بائٹیں],0,0)); } توڑو;
         }
         #تعریف_منسوخ STBI__CASE

         // the loop above sets the high byte of the pixels' alpha, but for
         // 16 bit png files we also need the low byte set. we'll do that here.
         اگر (گہرائی == 16) {
            cur = a->برآمد + stride*j; // start at the beginning of the row again
            برائے (i=0; i < x; ++i,cur+=output_بائٹیں) {
               cur[filter_بائٹیں+1] = 255;
            }
         }
      }
   }

   // we make a separate pass to expand bits to pixels; for performance,
   // this could run two scanlines behind the above code, so it won't
   // intefere with filtering but will still be in the cache.
   اگر (گہرائی < 8) {
      برائے (j=0; j < y; ++j) {
         stbi_uc *cur = a->برآمد + stride*j;
         stbi_uc *اندر  = a->برآمد + stride*j + x*برآمد_n - img_چوڑائی_بائٹیں;
         // unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit
         // png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop
         stbi_uc scale = (color == 0) ? stbi_گہرائی_scale_جدول[گہرائی] : 1; // scale grayscale values to 0..255 range

         // note that the final byte might overshoot and write more data than desired.
         // we can allocate enough data that this never writes out of memory, but it
         // could also overwrite the next scanline. can it overwrite non-empty data
         // on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.
         // so we need to explicitly clamp the final ones

         اگر (گہرائی == 4) {
            برائے (k=x*img_n; k >= 2; k-=2, ++اندر) {
               *cur++ = scale * ((*اندر >> 4)       );
               *cur++ = scale * ((*اندر     ) & 0x0f);
            }
            اگر (k > 0) *cur++ = scale * ((*اندر >> 4)       );
         } ورنہ اگر (گہرائی == 2) {
            برائے (k=x*img_n; k >= 4; k-=4, ++اندر) {
               *cur++ = scale * ((*اندر >> 6)       );
               *cur++ = scale * ((*اندر >> 4) & 0x03);
               *cur++ = scale * ((*اندر >> 2) & 0x03);
               *cur++ = scale * ((*اندر     ) & 0x03);
            }
            اگر (k > 0) *cur++ = scale * ((*اندر >> 6)       );
            اگر (k > 1) *cur++ = scale * ((*اندر >> 4) & 0x03);
            اگر (k > 2) *cur++ = scale * ((*اندر >> 2) & 0x03);
         } ورنہ اگر (گہرائی == 1) {
            برائے (k=x*img_n; k >= 8; k-=8, ++اندر) {
               *cur++ = scale * ((*اندر >> 7)       );
               *cur++ = scale * ((*اندر >> 6) & 0x01);
               *cur++ = scale * ((*اندر >> 5) & 0x01);
               *cur++ = scale * ((*اندر >> 4) & 0x01);
               *cur++ = scale * ((*اندر >> 3) & 0x01);
               *cur++ = scale * ((*اندر >> 2) & 0x01);
               *cur++ = scale * ((*اندر >> 1) & 0x01);
               *cur++ = scale * ((*اندر     ) & 0x01);
            }
            اگر (k > 0) *cur++ = scale * ((*اندر >> 7)       );
            اگر (k > 1) *cur++ = scale * ((*اندر >> 6) & 0x01);
            اگر (k > 2) *cur++ = scale * ((*اندر >> 5) & 0x01);
            اگر (k > 3) *cur++ = scale * ((*اندر >> 4) & 0x01);
            اگر (k > 4) *cur++ = scale * ((*اندر >> 3) & 0x01);
            اگر (k > 5) *cur++ = scale * ((*اندر >> 2) & 0x01);
            اگر (k > 6) *cur++ = scale * ((*اندر >> 1) & 0x01);
         }
         اگر (img_n != برآمد_n) {
            صحیح q;
            // insert alpha = 255
            cur = a->برآمد + stride*j;
            اگر (img_n == 1) {
               برائے (q=x-1; q >= 0; --q) {
                  cur[q*2+1] = 255;
                  cur[q*2+0] = cur[q];
               }
            } ورنہ {
               STBI_ASSERT(img_n == 3);
               برائے (q=x-1; q >= 0; --q) {
                  cur[q*4+3] = 255;
                  cur[q*4+2] = cur[q*3+2];
                  cur[q*4+1] = cur[q*3+1];
                  cur[q*4+0] = cur[q*3+0];
               }
            }
         }
      }
   } ورنہ اگر (گہرائی == 16) {
      // force the image data from big-endian to platform-native.
      // this is done in a separate pass due to the decoding relying
      // on the data being untouched, but could probably be done
      // per-line during decode if care is taken.
      stbi_uc *cur = a->برآمد;
      stbi__uint16 *cur16 = (stbi__uint16*)cur;

      برائے(i=0; i < x*y*برآمد_n; ++i,cur16++,cur+=2) {
         *cur16 = (cur[0] << 8) | cur[1];
      }
   }

   واپس 1;
}

ساکن صحیح stbi__create_png_image(stbi__png *a, stbi_uc *image_ڈیٹا_2, stbi__uint32 image_ڈیٹا_len, صحیح برآمد_n, صحیح گہرائی, صحیح color, صحیح interlaced)
{
   صحیح بائٹیں = (گہرائی == 16 ? 2 : 1);
   صحیح برآمد_بائٹیں = برآمد_n * بائٹیں;
   stbi_uc *final;
   صحیح p;
   اگر (!interlaced)
      واپس stbi__create_png_image_raw(a, image_ڈیٹا_2, image_ڈیٹا_len, برآمد_n, a->s->img_x, a->s->img_y, گہرائی, color);

   // de-interlacing
   final = (stbi_uc *) stbi__malloc_mad3(a->s->img_x, a->s->img_y, برآمد_بائٹیں, 0);
   برائے (p=0; p < 7; ++p) {
      صحیح xorig[] = { 0,4,0,2,0,1,0 };
      صحیح yorig[] = { 0,0,4,0,2,0,1 };
      صحیح xspc[]  = { 8,8,4,4,2,2,1 };
      صحیح yspc[]  = { 8,8,8,4,4,2,2 };
      صحیح i,j,x,y;
      // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
      x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
      y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
      اگر (x && y) {
         stbi__uint32 img_len = ((((a->s->img_n * x * گہرائی) + 7) >> 3) + 1) * y;
         اگر (!stbi__create_png_image_raw(a, image_ڈیٹا_2, image_ڈیٹا_len, برآمد_n, x, y, گہرائی, color)) {
            STBI_FREE(final);
            واپس 0;
         }
         برائے (j=0; j < y; ++j) {
            برائے (i=0; i < x; ++i) {
               صحیح برآمد_y = j*yspc[p]+yorig[p];
               صحیح برآمد_x = i*xspc[p]+xorig[p];
               حافظہ_نقل(final + برآمد_y*a->s->img_x*برآمد_بائٹیں + برآمد_x*برآمد_بائٹیں,
                      a->برآمد + (j*x+i)*برآمد_بائٹیں, برآمد_بائٹیں);
            }
         }
         STBI_FREE(a->برآمد);
         image_ڈیٹا_2 += img_len;
         image_ڈیٹا_len -= img_len;
      }
   }
   a->برآمد = final;

   واپس 1;
}

ساکن صحیح stbi__compute_transparency(stbi__png *z, stbi_uc tc[3], صحیح برآمد_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_گنتی = s->img_x * s->img_y;
   stbi_uc *p = z->برآمد;

   // compute color-based transparency, assuming we've
   // already got 255 as the alpha value in the output
   STBI_ASSERT(برآمد_n == 2 || برآمد_n == 4);

   اگر (برآمد_n == 2) {
      برائے (i=0; i < pixel_گنتی; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 255);
         p += 2;
      }
   } ورنہ {
      برائے (i=0; i < pixel_گنتی; ++i) {
         اگر (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   واپس 1;
}

ساکن صحیح stbi__compute_transparency16(stbi__png *z, stbi__uint16 tc[3], صحیح برآمد_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_گنتی = s->img_x * s->img_y;
   stbi__uint16 *p = (stbi__uint16*) z->برآمد;

   // compute color-based transparency, assuming we've
   // already got 65535 as the alpha value in the output
   STBI_ASSERT(برآمد_n == 2 || برآمد_n == 4);

   اگر (برآمد_n == 2) {
      برائے (i = 0; i < pixel_گنتی; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 65535);
         p += 2;
      }
   } ورنہ {
      برائے (i = 0; i < pixel_گنتی; ++i) {
         اگر (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   واپس 1;
}

ساکن صحیح stbi__expand_png_palette(stbi__png *a, stbi_uc *palette, صحیح len, صحیح pal_img_n)
{
   stbi__uint32 i, pixel_گنتی = a->s->img_x * a->s->img_y;
   stbi_uc *p, *عارضی_برآمد, *orig = a->برآمد;

   p = (stbi_uc *) stbi__malloc_mad2(pixel_گنتی, pal_img_n, 0);
   اگر (p == تہی) واپس stbi__err("outofmem", "Out of memory");

   // between here and free(out) below, exitting would leak
   عارضی_برآمد = p;

   اگر (pal_img_n == 3) {
      برائے (i=0; i < pixel_گنتی; ++i) {
         صحیح n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p += 3;
      }
   } ورنہ {
      برائے (i=0; i < pixel_گنتی; ++i) {
         صحیح n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p[3] = palette[n+3];
         p += 4;
      }
   }
   STBI_FREE(a->برآمد);
   a->برآمد = عارضی_برآمد;

   STBI_NOTUSED(len);

   واپس 1;
}

ساکن صحیح stbi_unpremultiply_on_بھرو = 0;
ساکن صحیح stbi__de_iphone_flag = 0;

STBIDEF خالی stbi_سیٹ_unpremultiply_on_بھرو(صحیح flag_true_if_should_unpremultiply)
{
   stbi_unpremultiply_on_بھرو = flag_true_if_should_unpremultiply;
}

STBIDEF خالی stbi_convert_iphone_png_مقصد_rgb(صحیح flag_true_if_should_convert)
{
   stbi__de_iphone_flag = flag_true_if_should_convert;
}

ساکن خالی stbi__de_iphone(stbi__png *z)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_گنتی = s->img_x * s->img_y;
   stbi_uc *p = z->برآمد;

   اگر (s->img_برآمد_n == 3) {  // convert bgr to rgb
      برائے (i=0; i < pixel_گنتی; ++i) {
         stbi_uc t = p[0];
         p[0] = p[2];
         p[2] = t;
         p += 3;
      }
   } ورنہ {
      STBI_ASSERT(s->img_برآمد_n == 4);
      اگر (stbi_unpremultiply_on_بھرو) {
         // convert bgr to rgb and unpremultiply
         برائے (i=0; i < pixel_گنتی; ++i) {
            stbi_uc a = p[3];
            stbi_uc t = p[0];
            اگر (a) {
               stbi_uc half = a / 2;
               p[0] = (p[2] * 255 + half) / a;
               p[1] = (p[1] * 255 + half) / a;
               p[2] = ( t   * 255 + half) / a;
            } ورنہ {
               p[0] = p[2];
               p[2] = t;
            }
            p += 4;
         }
      } ورنہ {
         // convert bgr to rgb
         برائے (i=0; i < pixel_گنتی; ++i) {
            stbi_uc t = p[0];
            p[0] = p[2];
            p[2] = t;
            p += 4;
         }
      }
   }
}

#تعریف_کرو STBI__PNG_TYPE(a,b,c,d)  (((غیرعلامتی) (a) << 24) + ((غیرعلامتی) (b) << 16) + ((غیرعلامتی) (c) << 8) + (غیرعلامتی) (d))

ساکن صحیح stbi__parse_png_file(stbi__png *z, صحیح جائزہ, صحیح req_comp)
{
   stbi_uc palette[1024], pal_img_n=0;
   stbi_uc has_trans=0, tc[3];
   stbi__uint16 tc16[3];
   stbi__uint32 ioff=0, idata_حد_بندی=0, i, pal_len=0;
   صحیح پہلا=1,k,interlace=0, color=0, is_iphone=0;
   stbi__context *s = z->s;

   z->expanded = تہی;
   z->idata = تہی;
   z->برآمد = تہی;

   اگر (!stbi_check_png_سرخی(s)) واپس 0;

   اگر (جائزہ == STBI_جائزہ_type) واپس 1;

   برائے (;;) {
      stbi__pngchunk c = stbi_get_chunk_سرخی(s);
      انتخاب (c.type) {
         حالت STBI__PNG_TYPE('C','g','B','I'):
            is_iphone = 1;
            stbi__skip(s, c.لمبائی);
            توڑو;
         حالت STBI__PNG_TYPE('I','H','D','R'): {
            صحیح comp,filter;
            اگر (!پہلا) واپس stbi__err("multiple IHDR","Corrupt PNG");
            پہلا = 0;
            اگر (c.لمبائی != 13) واپس stbi__err("bad IHDR len","Corrupt PNG");
            s->img_x = stbi__get32be(s); اگر (s->img_x > (1 << 24)) واپس stbi__err("too large","Very large image (corrupt?)");
            s->img_y = stbi__get32be(s); اگر (s->img_y > (1 << 24)) واپس stbi__err("too large","Very large image (corrupt?)");
            z->گہرائی = stbi__get8(s);  اگر (z->گہرائی != 1 && z->گہرائی != 2 && z->گہرائی != 4 && z->گہرائی != 8 && z->گہرائی != 16)  واپس stbi__err("1/2/4/8/16-bit only","PNG not supported: 1/2/4/8/16-bit only");
            color = stbi__get8(s);  اگر (color > 6)         واپس stbi__err("bad ctype","Corrupt PNG");
            اگر (color == 3 && z->گہرائی == 16)                  واپس stbi__err("bad ctype","Corrupt PNG");
            اگر (color == 3) pal_img_n = 3; ورنہ اگر (color & 1) واپس stbi__err("bad ctype","Corrupt PNG");
            comp  = stbi__get8(s);  اگر (comp) واپس stbi__err("bad comp method","Corrupt PNG");
            filter= stbi__get8(s);  اگر (filter) واپس stbi__err("bad filter method","Corrupt PNG");
            interlace = stbi__get8(s); اگر (interlace>1) واپس stbi__err("bad interlace method","Corrupt PNG");
            اگر (!s->img_x || !s->img_y) واپس stbi__err("0-pixel image","Corrupt PNG");
            اگر (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               اگر ((1 << 30) / s->img_x / s->img_n < s->img_y) واپس stbi__err("too large", "Image too large to decode");
               اگر (جائزہ == STBI_جائزہ_سرخی) واپس 1;
            } ورنہ {
               // if paletted, then pal_n is our final components, and
               // img_n is # components to decompress/filter.
               s->img_n = 1;
               اگر ((1 << 30) / s->img_x / 4 < s->img_y) واپس stbi__err("too large","Corrupt PNG");
               // if SCAN_header, have to scan to see if we have a tRNS
            }
            توڑو;
         }

         حالت STBI__PNG_TYPE('P','L','T','E'):  {
            اگر (پہلا) واپس stbi__err("first not IHDR", "Corrupt PNG");
            اگر (c.لمبائی > 256*3) واپس stbi__err("invalid PLTE","Corrupt PNG");
            pal_len = c.لمبائی / 3;
            اگر (pal_len * 3 != c.لمبائی) واپس stbi__err("invalid PLTE","Corrupt PNG");
            برائے (i=0; i < pal_len; ++i) {
               palette[i*4+0] = stbi__get8(s);
               palette[i*4+1] = stbi__get8(s);
               palette[i*4+2] = stbi__get8(s);
               palette[i*4+3] = 255;
            }
            توڑو;
         }

         حالت STBI__PNG_TYPE('t','R','N','S'): {
            اگر (پہلا) واپس stbi__err("first not IHDR", "Corrupt PNG");
            اگر (z->idata) واپس stbi__err("tRNS after IDAT","Corrupt PNG");
            اگر (pal_img_n) {
               اگر (جائزہ == STBI_جائزہ_سرخی) { s->img_n = 4; واپس 1; }
               اگر (pal_len == 0) واپس stbi__err("tRNS before PLTE","Corrupt PNG");
               اگر (c.لمبائی > pal_len) واپس stbi__err("bad tRNS len","Corrupt PNG");
               pal_img_n = 4;
               برائے (i=0; i < c.لمبائی; ++i)
                  palette[i*4+3] = stbi__get8(s);
            } ورنہ {
               اگر (!(s->img_n & 1)) واپس stbi__err("tRNS with alpha","Corrupt PNG");
               اگر (c.لمبائی != (stbi__uint32) s->img_n*2) واپس stbi__err("bad tRNS len","Corrupt PNG");
               has_trans = 1;
               اگر (z->گہرائی == 16) {
                  برائے (k = 0; k < s->img_n; ++k) tc16[k] = (stbi__uint16)stbi__get16be(s); // copy the values as-is
               } ورنہ {
                  برائے (k = 0; k < s->img_n; ++k) tc[k] = (stbi_uc)(stbi__get16be(s) & 255) * stbi_گہرائی_scale_جدول[z->گہرائی]; // non 8-bit images will be larger
               }
            }
            توڑو;
         }

         حالت STBI__PNG_TYPE('I','D','A','T'): {
            اگر (پہلا) واپس stbi__err("first not IHDR", "Corrupt PNG");
            اگر (pal_img_n && !pal_len) واپس stbi__err("no PLTE","Corrupt PNG");
            اگر (جائزہ == STBI_جائزہ_سرخی) { s->img_n = pal_img_n; واپس 1; }
            اگر ((صحیح)(ioff + c.لمبائی) < (صحیح)ioff) واپس 0;
            اگر (ioff + c.لمبائی > idata_حد_بندی) {
               stbi__uint32 idata_حد_بندی_old = idata_حد_بندی;
               stbi_uc *p;
               اگر (idata_حد_بندی == 0) idata_حد_بندی = c.لمبائی > 4096 ? c.لمبائی : 4096;
               جب (ioff + c.لمبائی > idata_حد_بندی)
                  idata_حد_بندی *= 2;
               STBI_NOTUSED(idata_حد_بندی_old);
               p = (stbi_uc *) STBI_REALLOC_SIZED(z->idata, idata_حد_بندی_old, idata_حد_بندی); اگر (p == تہی) واپس stbi__err("outofmem", "Out of memory");
               z->idata = p;
            }
            اگر (!stbi__getn(s, z->idata+ioff,c.لمبائی)) واپس stbi__err("outofdata","Corrupt PNG");
            ioff += c.لمبائی;
            توڑو;
         }

         حالت STBI__PNG_TYPE('I','E','N','D'): {
            stbi__uint32 raw_len, bpl;
            اگر (پہلا) واپس stbi__err("first not IHDR", "Corrupt PNG");
            اگر (جائزہ != STBI_جائزہ_بھرو) واپس 1;
            اگر (z->idata == تہی) واپس stbi__err("no IDAT","Corrupt PNG");
            // initial guess for decoded data size to avoid unnecessary reallocs
            bpl = (s->img_x * z->گہرائی + 7) / 8; // bytes per line, per component
            raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
            z->expanded = (stbi_uc *) stbi_zlib_decode_malloc_guesssize_headerflag((حرف *) z->idata, ioff, raw_len, (صحیح *) &raw_len, !is_iphone);
            اگر (z->expanded == تہی) واپس 0; // zlib should set error
            STBI_FREE(z->idata); z->idata = تہی;
            اگر ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_برآمد_n = s->img_n+1;
            ورنہ
               s->img_برآمد_n = s->img_n;
            اگر (!stbi__create_png_image(z, z->expanded, raw_len, s->img_برآمد_n, z->گہرائی, color, interlace)) واپس 0;
            اگر (has_trans) {
               اگر (z->گہرائی == 16) {
                  اگر (!stbi__compute_transparency16(z, tc16, s->img_برآمد_n)) واپس 0;
               } ورنہ {
                  اگر (!stbi__compute_transparency(z, tc, s->img_برآمد_n)) واپس 0;
               }
            }
            اگر (is_iphone && stbi__de_iphone_flag && s->img_برآمد_n > 2)
               stbi__de_iphone(z);
            اگر (pal_img_n) {
               // pal_img_n == 3 or 4
               s->img_n = pal_img_n; // record the actual colors we had
               s->img_برآمد_n = pal_img_n;
               اگر (req_comp >= 3) s->img_برآمد_n = req_comp;
               اگر (!stbi__expand_png_palette(z, palette, pal_len, s->img_برآمد_n))
                  واپس 0;
            } ورنہ اگر (has_trans) {
               // non-paletted image with tRNS -> source image has (constant) alpha
               ++s->img_n;
            }
            STBI_FREE(z->expanded); z->expanded = تہی;
            واپس 1;
         }

         طےشدہ:
            // if critical, fail
            اگر (پہلا) واپس stbi__err("first not IHDR", "Corrupt PNG");
            اگر ((c.type & (1 << 29)) == 0) {
               #اگر_غیر_تعریف_شدہ STBI_NO_FAILURE_لڑیاں
               // not threadsafe
               ساکن حرف invalid_chunk[] = "XXXX PNG chunk not known";
               invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
               invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
               invalid_chunk[2] = STBI__BYTECAST(c.type >>  8);
               invalid_chunk[3] = STBI__BYTECAST(c.type >>  0);
               #اگر_ختم
               واپس stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
            }
            stbi__skip(s, c.لمبائی);
            توڑو;
      }
      // end of PNG chunk, read and skip CRC
      stbi__get32be(s);
   }
}

ساکن خالی *stbi__do_png(stbi__png *p, صحیح *x, صحیح *y, صحیح *n, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   خالی *نتیجہ=تہی;
   اگر (req_comp < 0 || req_comp > 4) واپس stbi__errpuc("bad req_comp", "Internal error");
   اگر (stbi__parse_png_file(p, STBI_جائزہ_بھرو, req_comp)) {
      اگر (p->گہرائی < 8)
         ri->بٹ_فی_channel = 8;
      ورنہ
         ri->بٹ_فی_channel = p->گہرائی;
      نتیجہ = p->برآمد;
      p->برآمد = تہی;
      اگر (req_comp && req_comp != p->s->img_برآمد_n) {
         اگر (ri->بٹ_فی_channel == 8)
            نتیجہ = stbi__convert_format((غیرعلامتی حرف *) نتیجہ, p->s->img_برآمد_n, req_comp, p->s->img_x, p->s->img_y);
         ورنہ
            نتیجہ = stbi__convert_format16((stbi__uint16 *) نتیجہ, p->s->img_برآمد_n, req_comp, p->s->img_x, p->s->img_y);
         p->s->img_برآمد_n = req_comp;
         اگر (نتیجہ == تہی) واپس نتیجہ;
      }
      *x = p->s->img_x;
      *y = p->s->img_y;
      اگر (n) *n = p->s->img_n;
   }
   STBI_FREE(p->برآمد);      p->برآمد      = تہی;
   STBI_FREE(p->expanded); p->expanded = تہی;
   STBI_FREE(p->idata);    p->idata    = تہی;

   واپس نتیجہ;
}

ساکن خالی *stbi_png_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   stbi__png p;
   p.s = s;
   واپس stbi__do_png(&p, x,y,comp,req_comp, ri);
}

ساکن صحیح stbi__png_test(stbi__context *s)
{
   صحیح r;
   r = stbi_check_png_سرخی(s);
   stbi__rewind(s);
   واپس r;
}

ساکن صحیح stbi__png_info_raw(stbi__png *p, صحیح *x, صحیح *y, صحیح *comp)
{
   اگر (!stbi__parse_png_file(p, STBI_جائزہ_سرخی, 0)) {
      stbi__rewind( p->s );
      واپس 0;
   }
   اگر (x) *x = p->s->img_x;
   اگر (y) *y = p->s->img_y;
   اگر (comp) *comp = p->s->img_n;
   واپس 1;
}

ساکن صحیح stbi__png_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   stbi__png p;
   p.s = s;
   واپس stbi__png_info_raw(&p, x, y, comp);
}

ساکن صحیح stbi__png_is16(stbi__context *s)
{
   stbi__png p;
   p.s = s;
   اگر (!stbi__png_info_raw(&p, تہی, تہی, تہی))
	   واپس 0;
   اگر (p.گہرائی != 16) {
      stbi__rewind(p.s);
      واپس 0;
   }
   واپس 1;
}
#اگر_ختم

// Microsoft/Windows BMP image

#اگر_غیر_تعریف_شدہ STBI_NO_BMP
ساکن صحیح stbi__bmp_test_raw(stbi__context *s)
{
   صحیح r;
   صحیح sz;
   اگر (stbi__get8(s) != 'B') واپس 0;
   اگر (stbi__get8(s) != 'M') واپس 0;
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   stbi__get32le(s); // discard data offset
   sz = stbi__get32le(s);
   r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
   واپس r;
}

ساکن صحیح stbi__bmp_test(stbi__context *s)
{
   صحیح r = stbi__bmp_test_raw(s);
   stbi__rewind(s);
   واپس r;
}


// returns 0..31 for the highest set bit
ساکن صحیح stbi_بلند_ثنائی_ہندسہ(غیرعلامتی صحیح z)
{
   صحیح n=0;
   اگر (z == 0) واپس -1;
   اگر (z >= 0x10000) n += 16, z >>= 16;
   اگر (z >= 0x00100) n +=  8, z >>=  8;
   اگر (z >= 0x00010) n +=  4, z >>=  4;
   اگر (z >= 0x00004) n +=  2, z >>=  2;
   اگر (z >= 0x00002) n +=  1, z >>=  1;
   واپس n;
}

ساکن صحیح stbi__bitcount(غیرعلامتی صحیح a)
{
   a = (a & 0x55555555) + ((a >>  1) & 0x55555555); // max 2
   a = (a & 0x33333333) + ((a >>  2) & 0x33333333); // max 4
   a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
   a = (a + (a >> 8)); // max 16 per 8 bits
   a = (a + (a >> 16)); // max 32 per 8 bits
   واپس a & 0xff;
}

// extract an arbitrarily-aligned N-bit value (N=bits)
// from v, and then make it 8-bits long and fractionally
// extend it to full full range.
ساکن صحیح stbi__shiftsigned(صحیح v, صحیح سرکاؤ, صحیح بٹ)
{
   ساکن غیرعلامتی صحیح mul_جدول[9] = {
      0,
      0xff/*0b11111111*/, 0x55/*0b01010101*/, 0x49/*0b01001001*/, 0x11/*0b00010001*/,
      0x21/*0b00100001*/, 0x41/*0b01000001*/, 0x81/*0b10000001*/, 0x01/*0b00000001*/,
   };
   ساکن غیرعلامتی صحیح سرکاؤ_جدول[9] = {
      0, 0,0,1,0,2,4,6,0,
   };
   اگر (سرکاؤ < 0)
      v <<= -سرکاؤ;
   ورنہ
      v >>= سرکاؤ;
   STBI_ASSERT(v >= 0 && v < 256);
   v >>= (8-بٹ);
   STBI_ASSERT(بٹ >= 0 && بٹ <= 8);
   واپس (صحیح) ((غیرعلامتی) v * mul_جدول[بٹ]) >> سرکاؤ_جدول[بٹ];
}

قسمتعریف ساخت
{
   صحیح bpp, انحراف, hsz;
   غیرعلامتی صحیح mr,mg,mb,ma, all_a;
} stbi_bmp_ڈیٹا;

ساکن خالی *stbi_bmp_parse_سرخی(stbi__context *s, stbi_bmp_ڈیٹا *info)
{
   صحیح hsz;
   اگر (stbi__get8(s) != 'B' || stbi__get8(s) != 'M') واپس stbi__errpuc("not BMP", "Corrupt BMP");
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   info->انحراف = stbi__get32le(s);
   info->hsz = hsz = stbi__get32le(s);
   info->mr = info->mg = info->mb = info->ma = 0;

   اگر (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) واپس stbi__errpuc("unknown BMP", "BMP type not supported: unknown");
   اگر (hsz == 12) {
      s->img_x = stbi__get16le(s);
      s->img_y = stbi__get16le(s);
   } ورنہ {
      s->img_x = stbi__get32le(s);
      s->img_y = stbi__get32le(s);
   }
   اگر (stbi__get16le(s) != 1) واپس stbi__errpuc("bad BMP", "bad BMP");
   info->bpp = stbi__get16le(s);
   اگر (hsz != 12) {
      صحیح compress = stbi__get32le(s);
      اگر (compress == 1 || compress == 2) واپس stbi__errpuc("BMP RLE", "BMP type not supported: RLE");
      stbi__get32le(s); // discard sizeof
      stbi__get32le(s); // discard hres
      stbi__get32le(s); // discard vres
      stbi__get32le(s); // discard colorsused
      stbi__get32le(s); // discard max important
      اگر (hsz == 40 || hsz == 56) {
         اگر (hsz == 56) {
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
         }
         اگر (info->bpp == 16 || info->bpp == 32) {
            اگر (compress == 0) {
               اگر (info->bpp == 32) {
                  info->mr = 0xffu << 16;
                  info->mg = 0xffu <<  8;
                  info->mb = 0xffu <<  0;
                  info->ma = 0xffu << 24;
                  info->all_a = 0; // if all_a is 0 at end, then we loaded alpha channel but it was all 0
               } ورنہ {
                  info->mr = 31u << 10;
                  info->mg = 31u <<  5;
                  info->mb = 31u <<  0;
               }
            } ورنہ اگر (compress == 3) {
               info->mr = stbi__get32le(s);
               info->mg = stbi__get32le(s);
               info->mb = stbi__get32le(s);
               // not documented, but generated by photoshop and handled by mspaint
               اگر (info->mr == info->mg && info->mg == info->mb) {
                  // ?!?!?
                  واپس stbi__errpuc("bad BMP", "bad BMP");
               }
            } ورنہ
               واپس stbi__errpuc("bad BMP", "bad BMP");
         }
      } ورنہ {
         صحیح i;
         اگر (hsz != 108 && hsz != 124)
            واپس stbi__errpuc("bad BMP", "bad BMP");
         info->mr = stbi__get32le(s);
         info->mg = stbi__get32le(s);
         info->mb = stbi__get32le(s);
         info->ma = stbi__get32le(s);
         stbi__get32le(s); // discard color space
         برائے (i=0; i < 12; ++i)
            stbi__get32le(s); // discard color space parameters
         اگر (hsz == 124) {
            stbi__get32le(s); // discard rendering intent
            stbi__get32le(s); // discard offset of profile data
            stbi__get32le(s); // discard size of profile data
            stbi__get32le(s); // discard reserved
         }
      }
   }
   واپس (خالی *) 1;
}


ساکن خالی *stbi_bmp_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   stbi_uc *برآمد;
   غیرعلامتی صحیح mr=0,mg=0,mb=0,ma=0, all_a;
   stbi_uc pal[256][4];
   صحیح psize=0,i,j,چوڑائی;
   صحیح flip_vertically, pad, ہدف;
   stbi_bmp_ڈیٹا info;
   STBI_NOTUSED(ri);

   info.all_a = 255;
   اگر (stbi_bmp_parse_سرخی(s, &info) == تہی)
      واپس تہی; // error code already set

   flip_vertically = ((صحیح) s->img_y) > 0;
   s->img_y = مطلق_قدر((صحیح) s->img_y);

   mr = info.mr;
   mg = info.mg;
   mb = info.mb;
   ma = info.ma;
   all_a = info.all_a;

   اگر (info.hsz == 12) {
      اگر (info.bpp < 24)
         psize = (info.انحراف - 14 - 24) / 3;
   } ورنہ {
      اگر (info.bpp < 16)
         psize = (info.انحراف - 14 - info.hsz) >> 2;
   }

   s->img_n = ma ? 4 : 3;
   اگر (req_comp && req_comp >= 3) // we can directly decode 3 or 4
      ہدف = req_comp;
   ورنہ
      ہدف = s->img_n; // if they want monochrome, we'll post-convert

   // sanity-check size
   اگر (!stbi__mad3sizes_valid(ہدف, s->img_x, s->img_y, 0))
      واپس stbi__errpuc("too large", "Corrupt BMP");

   برآمد = (stbi_uc *) stbi__malloc_mad3(ہدف, s->img_x, s->img_y, 0);
   اگر (!برآمد) واپس stbi__errpuc("outofmem", "Out of memory");
   اگر (info.bpp < 16) {
      صحیح z=0;
      اگر (psize == 0 || psize > 256) { STBI_FREE(برآمد); واپس stbi__errpuc("invalid", "Corrupt BMP"); }
      برائے (i=0; i < psize; ++i) {
         pal[i][2] = stbi__get8(s);
         pal[i][1] = stbi__get8(s);
         pal[i][0] = stbi__get8(s);
         اگر (info.hsz != 12) stbi__get8(s);
         pal[i][3] = 255;
      }
      stbi__skip(s, info.انحراف - 14 - info.hsz - psize * (info.hsz == 12 ? 3 : 4));
      اگر (info.bpp == 1) چوڑائی = (s->img_x + 7) >> 3;
      ورنہ اگر (info.bpp == 4) چوڑائی = (s->img_x + 1) >> 1;
      ورنہ اگر (info.bpp == 8) چوڑائی = s->img_x;
      ورنہ { STBI_FREE(برآمد); واپس stbi__errpuc("bad bpp", "Corrupt BMP"); }
      pad = (-چوڑائی)&3;
      اگر (info.bpp == 1) {
         برائے (j=0; j < (صحیح) s->img_y; ++j) {
            صحیح ثنائی_ہندسہ_انحراف = 7, v = stbi__get8(s);
            برائے (i=0; i < (صحیح) s->img_x; ++i) {
               صحیح color = (v>>ثنائی_ہندسہ_انحراف)&0x1;
               برآمد[z++] = pal[color][0];
               برآمد[z++] = pal[color][1];
               برآمد[z++] = pal[color][2];
               اگر((--ثنائی_ہندسہ_انحراف) < 0) {
                  ثنائی_ہندسہ_انحراف = 7;
                  v = stbi__get8(s);
               }
            }
            stbi__skip(s, pad);
         }
      } ورنہ {
         برائے (j=0; j < (صحیح) s->img_y; ++j) {
            برائے (i=0; i < (صحیح) s->img_x; i += 2) {
               صحیح v=stbi__get8(s),v2=0;
               اگر (info.bpp == 4) {
                  v2 = v & 15;
                  v >>= 4;
               }
               برآمد[z++] = pal[v][0];
               برآمد[z++] = pal[v][1];
               برآمد[z++] = pal[v][2];
               اگر (ہدف == 4) برآمد[z++] = 255;
               اگر (i+1 == (صحیح) s->img_x) توڑو;
               v = (info.bpp == 8) ? stbi__get8(s) : v2;
               برآمد[z++] = pal[v][0];
               برآمد[z++] = pal[v][1];
               برآمد[z++] = pal[v][2];
               اگر (ہدف == 4) برآمد[z++] = 255;
            }
            stbi__skip(s, pad);
         }
      }
   } ورنہ {
      صحیح rshift=0,gshift=0,bshift=0,ashift=0,rcount=0,gcount=0,bcount=0,acount=0;
      صحیح z = 0;
      صحیح easy=0;
      stbi__skip(s, info.انحراف - 14 - info.hsz);
      اگر (info.bpp == 24) چوڑائی = 3 * s->img_x;
      ورنہ اگر (info.bpp == 16) چوڑائی = 2*s->img_x;
      ورنہ /* bpp = 32 and pad = 0 */ چوڑائی=0;
      pad = (-چوڑائی) & 3;
      اگر (info.bpp == 24) {
         easy = 1;
      } ورنہ اگر (info.bpp == 32) {
         اگر (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
            easy = 2;
      }
      اگر (!easy) {
         اگر (!mr || !mg || !mb) { STBI_FREE(برآمد); واپس stbi__errpuc("bad masks", "Corrupt BMP"); }
         // right shift amt to put high bit in position #7
         rshift = stbi_بلند_ثنائی_ہندسہ(mr)-7; rcount = stbi__bitcount(mr);
         gshift = stbi_بلند_ثنائی_ہندسہ(mg)-7; gcount = stbi__bitcount(mg);
         bshift = stbi_بلند_ثنائی_ہندسہ(mb)-7; bcount = stbi__bitcount(mb);
         ashift = stbi_بلند_ثنائی_ہندسہ(ma)-7; acount = stbi__bitcount(ma);
      }
      برائے (j=0; j < (صحیح) s->img_y; ++j) {
         اگر (easy) {
            برائے (i=0; i < (صحیح) s->img_x; ++i) {
               غیرعلامتی حرف a;
               برآمد[z+2] = stbi__get8(s);
               برآمد[z+1] = stbi__get8(s);
               برآمد[z+0] = stbi__get8(s);
               z += 3;
               a = (easy == 2 ? stbi__get8(s) : 255);
               all_a |= a;
               اگر (ہدف == 4) برآمد[z++] = a;
            }
         } ورنہ {
            صحیح bpp = info.bpp;
            برائے (i=0; i < (صحیح) s->img_x; ++i) {
               stbi__uint32 v = (bpp == 16 ? (stbi__uint32) stbi__get16le(s) : stbi__get32le(s));
               غیرعلامتی صحیح a;
               برآمد[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
               برآمد[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
               برآمد[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));
               a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
               all_a |= a;
               اگر (ہدف == 4) برآمد[z++] = STBI__BYTECAST(a);
            }
         }
         stbi__skip(s, pad);
      }
   }

   // if alpha channel is all 0s, replace with all 255s
   اگر (ہدف == 4 && all_a == 0)
      برائے (i=4*s->img_x*s->img_y-1; i >= 0; i -= 4)
         برآمد[i] = 255;

   اگر (flip_vertically) {
      stbi_uc t;
      برائے (j=0; j < (صحیح) s->img_y>>1; ++j) {
         stbi_uc *p1 = برآمد +      j     *s->img_x*ہدف;
         stbi_uc *p2 = برآمد + (s->img_y-1-j)*s->img_x*ہدف;
         برائے (i=0; i < (صحیح) s->img_x*ہدف; ++i) {
            t = p1[i], p1[i] = p2[i], p2[i] = t;
         }
      }
   }

   اگر (req_comp && req_comp != ہدف) {
      برآمد = stbi__convert_format(برآمد, ہدف, req_comp, s->img_x, s->img_y);
      اگر (برآمد == تہی) واپس برآمد; // stbi__convert_format frees input on failure
   }

   *x = s->img_x;
   *y = s->img_y;
   اگر (comp) *comp = s->img_n;
   واپس برآمد;
}
#اگر_ختم

// Targa Truevision - TGA
// by Jonathan Dummer
#اگر_غیر_تعریف_شدہ STBI_NO_TGA
// returns STBI_rgb or whatever, 0 on error
ساکن صحیح stbi__tga_get_comp(صحیح بٹ_فی_pixel, صحیح is_grey, صحیح* is_rgb16)
{
   // only RGB or RGBA (incl. 16bit) or grey allowed
   اگر (is_rgb16) *is_rgb16 = 0;
   انتخاب(بٹ_فی_pixel) {
      حالت 8:  واپس STBI_grey;
      حالت 16: اگر(is_grey) واپس STBI_grey_alpha;
               // fallthrough
      حالت 15: اگر(is_rgb16) *is_rgb16 = 1;
               واپس STBI_rgb;
      حالت 24: // fallthrough
      حالت 32: واپس بٹ_فی_pixel/8;
      طےشدہ: واپس 0;
   }
}

ساکن صحیح stbi__tga_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
    صحیح tga_w, tga_h, tga_comp, tga_image_type, tga_بٹ_فی_pixel, tga_colormap_bpp;
    صحیح sz, tga_colormap_type;
    stbi__get8(s);                   // discard Offset
    tga_colormap_type = stbi__get8(s); // colormap type
    اگر( tga_colormap_type > 1 ) {
        stbi__rewind(s);
        واپس 0;      // only RGB or indexed allowed
    }
    tga_image_type = stbi__get8(s); // image type
    اگر ( tga_colormap_type == 1 ) { // colormapped (paletted) image
        اگر (tga_image_type != 1 && tga_image_type != 9) {
            stbi__rewind(s);
            واپس 0;
        }
        stbi__skip(s,4);       // skip index of first colormap entry and number of entries
        sz = stbi__get8(s);    //   check bits per palette color entry
        اگر ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) {
            stbi__rewind(s);
            واپس 0;
        }
        stbi__skip(s,4);       // skip image x and y origin
        tga_colormap_bpp = sz;
    } ورنہ { // "normal" image w/o colormap - only RGB or grey allowed, +/- RLE
        اگر ( (tga_image_type != 2) && (tga_image_type != 3) && (tga_image_type != 10) && (tga_image_type != 11) ) {
            stbi__rewind(s);
            واپس 0; // only RGB or grey allowed, +/- RLE
        }
        stbi__skip(s,9); // skip colormap specification and image x/y origin
        tga_colormap_bpp = 0;
    }
    tga_w = stbi__get16le(s);
    اگر( tga_w < 1 ) {
        stbi__rewind(s);
        واپس 0;   // test width
    }
    tga_h = stbi__get16le(s);
    اگر( tga_h < 1 ) {
        stbi__rewind(s);
        واپس 0;   // test height
    }
    tga_بٹ_فی_pixel = stbi__get8(s); // bits per pixel
    stbi__get8(s); // ignore alpha bits
    اگر (tga_colormap_bpp != 0) {
        اگر((tga_بٹ_فی_pixel != 8) && (tga_بٹ_فی_pixel != 16)) {
            // when using a colormap, tga_bits_per_pixel is the size of the indexes
            // I don't think anything but 8 or 16bit indexes makes sense
            stbi__rewind(s);
            واپس 0;
        }
        tga_comp = stbi__tga_get_comp(tga_colormap_bpp, 0, تہی);
    } ورنہ {
        tga_comp = stbi__tga_get_comp(tga_بٹ_فی_pixel, (tga_image_type == 3) || (tga_image_type == 11), تہی);
    }
    اگر(!tga_comp) {
      stbi__rewind(s);
      واپس 0;
    }
    اگر (x) *x = tga_w;
    اگر (y) *y = tga_h;
    اگر (comp) *comp = tga_comp;
    واپس 1;                   // seems to have passed everything
}

ساکن صحیح stbi__tga_test(stbi__context *s)
{
   صحیح res = 0;
   صحیح sz, tga_color_type;
   stbi__get8(s);      //   discard Offset
   tga_color_type = stbi__get8(s);   //   color type
   اگر ( tga_color_type > 1 ) جاؤ error_انتہا;   //   only RGB or indexed allowed
   sz = stbi__get8(s);   //   image type
   اگر ( tga_color_type == 1 ) { // colormapped (paletted) image
      اگر (sz != 1 && sz != 9) جاؤ error_انتہا; // colortype 1 demands image type 1 or 9
      stbi__skip(s,4);       // skip index of first colormap entry and number of entries
      sz = stbi__get8(s);    //   check bits per palette color entry
      اگر ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) جاؤ error_انتہا;
      stbi__skip(s,4);       // skip image x and y origin
   } ورنہ { // "normal" image w/o colormap
      اگر ( (sz != 2) && (sz != 3) && (sz != 10) && (sz != 11) ) جاؤ error_انتہا; // only RGB or grey allowed, +/- RLE
      stbi__skip(s,9); // skip colormap specification and image x/y origin
   }
   اگر ( stbi__get16le(s) < 1 ) جاؤ error_انتہا;      //   test width
   اگر ( stbi__get16le(s) < 1 ) جاؤ error_انتہا;      //   test height
   sz = stbi__get8(s);   //   bits per pixel
   اگر ( (tga_color_type == 1) && (sz != 8) && (sz != 16) ) جاؤ error_انتہا; // for colormapped images, bpp is size of an index
   اگر ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) جاؤ error_انتہا;

   res = 1; // if we got this far, everything's good and we can return 1 instead of 0

error_انتہا:
   stbi__rewind(s);
   واپس res;
}

// read 16bit value and convert to 24bit RGB
ساکن خالی stbi__tga_read_rgb16(stbi__context *s, stbi_uc* برآمد)
{
   stbi__uint16 px = (stbi__uint16)stbi__get16le(s);
   stbi__uint16 five_ثنائی_ہندسہ_نقاب = 31;
   // we have 3 channels with 5bits each
   صحیح r = (px >> 10) & five_ثنائی_ہندسہ_نقاب;
   صحیح g = (px >> 5) & five_ثنائی_ہندسہ_نقاب;
   صحیح b = px & five_ثنائی_ہندسہ_نقاب;
   // Note that this saves the data in RGB(A) order, so it doesn't need to be swapped later
   برآمد[0] = (stbi_uc)((r * 255)/31);
   برآمد[1] = (stbi_uc)((g * 255)/31);
   برآمد[2] = (stbi_uc)((b * 255)/31);

   // some people claim that the most significant bit might be used for alpha
   // (possibly if an alpha-bit is set in the "image descriptor byte")
   // but that only made 16bit test images completely translucent..
   // so let's treat all 15 and 16bit TGAs as RGB with no alpha.
}

ساکن خالی *stbi_tga_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   //   read in the TGA header stuff
   صحیح tga_انحراف = stbi__get8(s);
   صحیح tga_اشاریہ_دار = stbi__get8(s);
   صحیح tga_image_type = stbi__get8(s);
   صحیح tga_is_RLE = 0;
   صحیح tga_palette_آغاز = stbi__get16le(s);
   صحیح tga_palette_len = stbi__get16le(s);
   صحیح tga_palette_بٹ = stbi__get8(s);
   صحیح tga_x_origin = stbi__get16le(s);
   صحیح tga_y_origin = stbi__get16le(s);
   صحیح tga_چوڑائی = stbi__get16le(s);
   صحیح tga_اونچائی = stbi__get16le(s);
   صحیح tga_بٹ_فی_pixel = stbi__get8(s);
   صحیح tga_comp, tga_rgb16=0;
   صحیح tga_inverted = stbi__get8(s);
   // int tga_alpha_bits = tga_inverted & 15; // the 4 lowest bits - unused (useless?)
   //   image data
   غیرعلامتی حرف *tga_ڈیٹا;
   غیرعلامتی حرف *tga_palette = تہی;
   صحیح i, j;
   غیرعلامتی حرف raw_ڈیٹا[4] = {0};
   صحیح RLE_گنتی = 0;
   صحیح RLE_repeating = 0;
   صحیح read_اگلا_pixel = 1;
   STBI_NOTUSED(ri);

   //   do a tiny bit of precessing
   اگر ( tga_image_type >= 8 )
   {
      tga_image_type -= 8;
      tga_is_RLE = 1;
   }
   tga_inverted = 1 - ((tga_inverted >> 5) & 1);

   //   If I'm paletted, then I'll use the number of bits from the palette
   اگر ( tga_اشاریہ_دار ) tga_comp = stbi__tga_get_comp(tga_palette_بٹ, 0, &tga_rgb16);
   ورنہ tga_comp = stbi__tga_get_comp(tga_بٹ_فی_pixel, (tga_image_type == 3), &tga_rgb16);

   اگر(!tga_comp) // shouldn't really happen, stbi__tga_test() should have ensured basic consistency
      واپس stbi__errpuc("bad format", "Can't find out TGA pixelformat");

   //   tga info
   *x = tga_چوڑائی;
   *y = tga_اونچائی;
   اگر (comp) *comp = tga_comp;

   اگر (!stbi__mad3sizes_valid(tga_چوڑائی, tga_اونچائی, tga_comp, 0))
      واپس stbi__errpuc("too large", "Corrupt TGA");

   tga_ڈیٹا = (غیرعلامتی حرف*)stbi__malloc_mad3(tga_چوڑائی, tga_اونچائی, tga_comp, 0);
   اگر (!tga_ڈیٹا) واپس stbi__errpuc("outofmem", "Out of memory");

   // skip to the data's starting position (offset usually = 0)
   stbi__skip(s, tga_انحراف );

   اگر ( !tga_اشاریہ_دار && !tga_is_RLE && !tga_rgb16 ) {
      برائے (i=0; i < tga_اونچائی; ++i) {
         صحیح صف = tga_inverted ? tga_اونچائی -i - 1 : i;
         stbi_uc *tga_صف = tga_ڈیٹا + صف*tga_چوڑائی*tga_comp;
         stbi__getn(s, tga_صف, tga_چوڑائی * tga_comp);
      }
   } ورنہ  {
      //   do I need to load a palette?
      اگر ( tga_اشاریہ_دار)
      {
         //   any data to skip? (offset usually = 0)
         stbi__skip(s, tga_palette_آغاز );
         //   load the palette
         tga_palette = (غیرعلامتی حرف*)stbi__malloc_mad2(tga_palette_len, tga_comp, 0);
         اگر (!tga_palette) {
            STBI_FREE(tga_ڈیٹا);
            واپس stbi__errpuc("outofmem", "Out of memory");
         }
         اگر (tga_rgb16) {
            stbi_uc *pal_اندراج = tga_palette;
            STBI_ASSERT(tga_comp == STBI_rgb);
            برائے (i=0; i < tga_palette_len; ++i) {
               stbi__tga_read_rgb16(s, pal_اندراج);
               pal_اندراج += tga_comp;
            }
         } ورنہ اگر (!stbi__getn(s, tga_palette, tga_palette_len * tga_comp)) {
               STBI_FREE(tga_ڈیٹا);
               STBI_FREE(tga_palette);
               واپس stbi__errpuc("bad palette", "Corrupt TGA");
         }
      }
      //   load the data
      برائے (i=0; i < tga_چوڑائی * tga_اونچائی; ++i)
      {
         //   if I'm in RLE mode, do I need to get a RLE stbi__pngchunk?
         اگر ( tga_is_RLE )
         {
            اگر ( RLE_گنتی == 0 )
            {
               //   yep, get the next byte as a RLE command
               صحیح RLE_cmd = stbi__get8(s);
               RLE_گنتی = 1 + (RLE_cmd & 127);
               RLE_repeating = RLE_cmd >> 7;
               read_اگلا_pixel = 1;
            } ورنہ اگر ( !RLE_repeating )
            {
               read_اگلا_pixel = 1;
            }
         } ورنہ
         {
            read_اگلا_pixel = 1;
         }
         //   OK, if I need to read a pixel, do it now
         اگر ( read_اگلا_pixel )
         {
            //   load however much data we did have
            اگر ( tga_اشاریہ_دار )
            {
               // read in index, then perform the lookup
               صحیح pal_idx = (tga_بٹ_فی_pixel == 8) ? stbi__get8(s) : stbi__get16le(s);
               اگر ( pal_idx >= tga_palette_len ) {
                  // invalid index
                  pal_idx = 0;
               }
               pal_idx *= tga_comp;
               برائے (j = 0; j < tga_comp; ++j) {
                  raw_ڈیٹا[j] = tga_palette[pal_idx+j];
               }
            } ورنہ اگر(tga_rgb16) {
               STBI_ASSERT(tga_comp == STBI_rgb);
               stbi__tga_read_rgb16(s, raw_ڈیٹا);
            } ورنہ {
               //   read in the data raw
               برائے (j = 0; j < tga_comp; ++j) {
                  raw_ڈیٹا[j] = stbi__get8(s);
               }
            }
            //   clear the reading flag for the next pixel
            read_اگلا_pixel = 0;
         } // end of reading a pixel

         // copy data
         برائے (j = 0; j < tga_comp; ++j)
           tga_ڈیٹا[i*tga_comp+j] = raw_ڈیٹا[j];

         //   in case we're in RLE mode, keep counting down
         --RLE_گنتی;
      }
      //   do I need to invert the image?
      اگر ( tga_inverted )
      {
         برائے (j = 0; j*2 < tga_اونچائی; ++j)
         {
            صحیح index1 = j * tga_چوڑائی * tga_comp;
            صحیح index2 = (tga_اونچائی - 1 - j) * tga_چوڑائی * tga_comp;
            برائے (i = tga_چوڑائی * tga_comp; i > 0; --i)
            {
               غیرعلامتی حرف عارضی = tga_ڈیٹا[index1];
               tga_ڈیٹا[index1] = tga_ڈیٹا[index2];
               tga_ڈیٹا[index2] = عارضی;
               ++index1;
               ++index2;
            }
         }
      }
      //   clear my palette, if I had one
      اگر ( tga_palette != تہی )
      {
         STBI_FREE( tga_palette );
      }
   }

   // swap RGB - if the source data was RGB16, it already is in the right order
   اگر (tga_comp >= 3 && !tga_rgb16)
   {
      غیرعلامتی حرف* tga_pixel = tga_ڈیٹا;
      برائے (i=0; i < tga_چوڑائی * tga_اونچائی; ++i)
      {
         غیرعلامتی حرف عارضی = tga_pixel[0];
         tga_pixel[0] = tga_pixel[2];
         tga_pixel[2] = عارضی;
         tga_pixel += tga_comp;
      }
   }

   // convert to target component count
   اگر (req_comp && req_comp != tga_comp)
      tga_ڈیٹا = stbi__convert_format(tga_ڈیٹا, tga_comp, req_comp, tga_چوڑائی, tga_اونچائی);

   //   the things I do to get rid of an error message, and yet keep
   //   Microsoft's C compilers happy... [8^(
   tga_palette_آغاز = tga_palette_len = tga_palette_بٹ =
         tga_x_origin = tga_y_origin = 0;
   //   OK, done
   واپس tga_ڈیٹا;
}
#اگر_ختم

// *************************************************************************************************
// Photoshop PSD loader -- PD by Thatcher Ulrich, integration by Nicolas Schulz, tweaked by STB

#اگر_غیر_تعریف_شدہ STBI_NO_PSD
ساکن صحیح stbi__psd_test(stbi__context *s)
{
   صحیح r = (stbi__get32be(s) == 0x38425053);
   stbi__rewind(s);
   واپس r;
}

ساکن صحیح stbi__psd_decode_rle(stbi__context *s, stbi_uc *p, صحیح pixel_گنتی_2)
{
   صحیح گنتی, nleft, len;

   گنتی = 0;
   جب ((nleft = pixel_گنتی_2 - گنتی) > 0) {
      len = stbi__get8(s);
      اگر (len == 128) {
         // No-op.
      } ورنہ اگر (len < 128) {
         // Copy next len+1 bytes literally.
         len++;
         اگر (len > nleft) واپس 0; // corrupt data
         گنتی += len;
         جب (len) {
            *p = stbi__get8(s);
            p += 4;
            len--;
         }
      } ورنہ اگر (len > 128) {
         stbi_uc   val;
         // Next -len+1 bytes in the dest are replicated from next source byte.
         // (Interpret len as a negative 8-bit int.)
         len = 257 - len;
         اگر (len > nleft) واپس 0; // corrupt data
         val = stbi__get8(s);
         گنتی += len;
         جب (len) {
            *p = val;
            p += 4;
            len--;
         }
      }
   }

   واپس 1;
}

ساکن خالی *stbi_psd_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri, صحیح bpc)
{
   صحیح pixel_گنتی_2;
   صحیح channel_گنتی, compression;
   صحیح channel, i;
   صحیح bitdepth;
   صحیح w,h;
   stbi_uc *برآمد;
   STBI_NOTUSED(ri);

   // Check identifier
   اگر (stbi__get32be(s) != 0x38425053)   // "8BPS"
      واپس stbi__errpuc("not PSD", "Corrupt PSD image");

   // Check file type version.
   اگر (stbi__get16be(s) != 1)
      واپس stbi__errpuc("wrong version", "Unsupported version of PSD image");

   // Skip 6 reserved bytes.
   stbi__skip(s, 6 );

   // Read the number of channels (R, G, B, A, etc).
   channel_گنتی = stbi__get16be(s);
   اگر (channel_گنتی < 0 || channel_گنتی > 16)
      واپس stbi__errpuc("wrong channel count", "Unsupported number of channels in PSD image");

   // Read the rows and columns of the image.
   h = stbi__get32be(s);
   w = stbi__get32be(s);

   // Make sure the depth is 8 bits.
   bitdepth = stbi__get16be(s);
   اگر (bitdepth != 8 && bitdepth != 16)
      واپس stbi__errpuc("unsupported bit depth", "PSD bit depth is not 8 or 16 bit");

   // Make sure the color mode is RGB.
   // Valid options are:
   //   0: Bitmap
   //   1: Grayscale
   //   2: Indexed color
   //   3: RGB color
   //   4: CMYK color
   //   7: Multichannel
   //   8: Duotone
   //   9: Lab color
   اگر (stbi__get16be(s) != 3)
      واپس stbi__errpuc("wrong color format", "PSD is not in RGB color format");

   // Skip the Mode Data.  (It's the palette for indexed color; other info for other modes.)
   stbi__skip(s,stbi__get32be(s) );

   // Skip the image resources.  (resolution, pen tool paths, etc)
   stbi__skip(s, stbi__get32be(s) );

   // Skip the reserved data.
   stbi__skip(s, stbi__get32be(s) );

   // Find out if the data is compressed.
   // Known values:
   //   0: no compression
   //   1: RLE compressed
   compression = stbi__get16be(s);
   اگر (compression > 1)
      واپس stbi__errpuc("bad compression", "PSD has an unknown compression format");

   // Check size
   اگر (!stbi__mad3sizes_valid(4, w, h, 0))
      واپس stbi__errpuc("too large", "Corrupt PSD");

   // Create the destination image.

   اگر (!compression && bitdepth == 16 && bpc == 16) {
      برآمد = (stbi_uc *) stbi__malloc_mad3(8, w, h, 0);
      ri->بٹ_فی_channel = 16;
   } ورنہ
      برآمد = (stbi_uc *) stbi__malloc(4 * w*h);

   اگر (!برآمد) واپس stbi__errpuc("outofmem", "Out of memory");
   pixel_گنتی_2 = w*h;

   // Initialize the data to zero.
   //memset( out, 0, pixelCount * 4 );

   // Finally, the image data.
   اگر (compression) {
      // RLE as used by .PSD and .TIFF
      // Loop until you get the number of unpacked bytes you are expecting:
      //     Read the next source byte into n.
      //     If n is between 0 and 127 inclusive, copy the next n+1 bytes literally.
      //     Else if n is between -127 and -1 inclusive, copy the next byte -n+1 times.
      //     Else if n is 128, noop.
      // Endloop

      // The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
      // which we're going to just skip.
      stbi__skip(s, h * channel_گنتی * 2 );

      // Read the RLE data by channel.
      برائے (channel = 0; channel < 4; channel++) {
         stbi_uc *p;

         p = برآمد+channel;
         اگر (channel >= channel_گنتی) {
            // Fill this channel with default data.
            برائے (i = 0; i < pixel_گنتی_2; i++, p += 4)
               *p = (channel == 3 ? 255 : 0);
         } ورنہ {
            // Read the RLE data.
            اگر (!stbi__psd_decode_rle(s, p, pixel_گنتی_2)) {
               STBI_FREE(برآمد);
               واپس stbi__errpuc("corrupt", "bad RLE data");
            }
         }
      }

   } ورنہ {
      // We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
      // where each channel consists of an 8-bit (or 16-bit) value for each pixel in the image.

      // Read the data by channel.
      برائے (channel = 0; channel < 4; channel++) {
         اگر (channel >= channel_گنتی) {
            // Fill this channel with default data.
            اگر (bitdepth == 16 && bpc == 16) {
               stbi__uint16 *q = ((stbi__uint16 *) برآمد) + channel;
               stbi__uint16 val = channel == 3 ? 65535 : 0;
               برائے (i = 0; i < pixel_گنتی_2; i++, q += 4)
                  *q = val;
            } ورنہ {
               stbi_uc *p = برآمد+channel;
               stbi_uc val = channel == 3 ? 255 : 0;
               برائے (i = 0; i < pixel_گنتی_2; i++, p += 4)
                  *p = val;
            }
         } ورنہ {
            اگر (ri->بٹ_فی_channel == 16) {    // output bpc
               stbi__uint16 *q = ((stbi__uint16 *) برآمد) + channel;
               برائے (i = 0; i < pixel_گنتی_2; i++, q += 4)
                  *q = (stbi__uint16) stbi__get16be(s);
            } ورنہ {
               stbi_uc *p = برآمد+channel;
               اگر (bitdepth == 16) {  // input bpc
                  برائے (i = 0; i < pixel_گنتی_2; i++, p += 4)
                     *p = (stbi_uc) (stbi__get16be(s) >> 8);
               } ورنہ {
                  برائے (i = 0; i < pixel_گنتی_2; i++, p += 4)
                     *p = stbi__get8(s);
               }
            }
         }
      }
   }

   // remove weird white matte from PSD
   اگر (channel_گنتی >= 4) {
      اگر (ri->بٹ_فی_channel == 16) {
         برائے (i=0; i < w*h; ++i) {
            stbi__uint16 *pixel = (stbi__uint16 *) برآمد + 4*i;
            اگر (pixel[3] != 0 && pixel[3] != 65535) {
               اعشاری a = pixel[3] / 65535.0f;
               اعشاری ra = 1.0f / a;
               اعشاری inv_a = 65535.0f * (1 - ra);
               pixel[0] = (stbi__uint16) (pixel[0]*ra + inv_a);
               pixel[1] = (stbi__uint16) (pixel[1]*ra + inv_a);
               pixel[2] = (stbi__uint16) (pixel[2]*ra + inv_a);
            }
         }
      } ورنہ {
         برائے (i=0; i < w*h; ++i) {
            غیرعلامتی حرف *pixel = برآمد + 4*i;
            اگر (pixel[3] != 0 && pixel[3] != 255) {
               اعشاری a = pixel[3] / 255.0f;
               اعشاری ra = 1.0f / a;
               اعشاری inv_a = 255.0f * (1 - ra);
               pixel[0] = (غیرعلامتی حرف) (pixel[0]*ra + inv_a);
               pixel[1] = (غیرعلامتی حرف) (pixel[1]*ra + inv_a);
               pixel[2] = (غیرعلامتی حرف) (pixel[2]*ra + inv_a);
            }
         }
      }
   }

   // convert to desired output format
   اگر (req_comp && req_comp != 4) {
      اگر (ri->بٹ_فی_channel == 16)
         برآمد = (stbi_uc *) stbi__convert_format16((stbi__uint16 *) برآمد, 4, req_comp, w, h);
      ورنہ
         برآمد = stbi__convert_format(برآمد, 4, req_comp, w, h);
      اگر (برآمد == تہی) واپس برآمد; // stbi__convert_format frees input on failure
   }

   اگر (comp) *comp = 4;
   *y = h;
   *x = w;

   واپس برآمد;
}
#اگر_ختم

// *************************************************************************************************
// Softimage PIC loader
// by Tom Seddon
//
// See http://softimage.wiki.softimage.com/index.php/INFO:_PIC_file_format
// See http://ozviz.wasp.uwa.edu.au/~pbourke/dataformats/softimagepic/

#اگر_غیر_تعریف_شدہ STBI_NO_PIC
ساکن صحیح stbi__pic_is4(stbi__context *s,ثابت حرف *str)
{
   صحیح i;
   برائے (i=0; i<4; ++i)
      اگر (stbi__get8(s) != (stbi_uc)str[i])
         واپس 0;

   واپس 1;
}

ساکن صحیح stbi__pic_test_core(stbi__context *s)
{
   صحیح i;

   اگر (!stbi__pic_is4(s,"\x53\x80\xF6\x34"))
      واپس 0;

   برائے(i=0;i<84;++i)
      stbi__get8(s);

   اگر (!stbi__pic_is4(s,"PICT"))
      واپس 0;

   واپس 1;
}

قسمتعریف ساخت
{
   stbi_uc سائز,type,channel;
} stbi__pic_packet;

ساکن stbi_uc *stbi__readval(stbi__context *s, صحیح channel, stbi_uc *dest)
{
   صحیح نقاب=0x80, i;

   برائے (i=0; i<4; ++i, نقاب>>=1) {
      اگر (channel & نقاب) {
         اگر (stbi__at_eof(s)) واپس stbi__errpuc("bad file","PIC file too short");
         dest[i]=stbi__get8(s);
      }
   }

   واپس dest;
}

ساکن خالی stbi__copyval(صحیح channel,stbi_uc *dest,ثابت stbi_uc *src)
{
   صحیح نقاب=0x80,i;

   برائے (i=0;i<4; ++i, نقاب>>=1)
      اگر (channel&نقاب)
         dest[i]=src[i];
}

ساکن stbi_uc *stbi_pic_بھرو_core(stbi__context *s,صحیح چوڑائی,صحیح اونچائی,صحیح *comp, stbi_uc *نتیجہ)
{
   صحیح act_comp=0,عدد_packets=0,y,chained;
   stbi__pic_packet packets[10];

   // this will (should...) cater for even some bizarre stuff like having data
    // for the same channel in multiple packets.
   کرو {
      stbi__pic_packet *packet;

      اگر (عدد_packets==حجم(packets)/حجم(packets[0]))
         واپس stbi__errpuc("bad format","too many packets");

      packet = &packets[عدد_packets++];

      chained = stbi__get8(s);
      packet->سائز    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);

      act_comp |= packet->channel;

      اگر (stbi__at_eof(s))          واپس stbi__errpuc("bad file","file too short (reading packets)");
      اگر (packet->سائز != 8)  واپس stbi__errpuc("bad format","packet isn't 8bpp");
   } جب (chained);

   *comp = (act_comp & 0x10 ? 4 : 3); // has alpha channel?

   برائے(y=0; y<اونچائی; ++y) {
      صحیح packet_idx;

      برائے(packet_idx=0; packet_idx < عدد_packets; ++packet_idx) {
         stbi__pic_packet *packet = &packets[packet_idx];
         stbi_uc *dest = نتیجہ+y*چوڑائی*4;

         انتخاب (packet->type) {
            طےشدہ:
               واپس stbi__errpuc("bad format","packet has bad compression type");

            حالت 0: {//uncompressed
               صحیح x;

               برائے(x=0;x<چوڑائی;++x, dest+=4)
                  اگر (!stbi__readval(s,packet->channel,dest))
                     واپس 0;
               توڑو;
            }

            حالت 1://Pure RLE
               {
                  صحیح بایاں=چوڑائی, i;

                  جب (بایاں>0) {
                     stbi_uc گنتی,قدر[4];

                     گنتی=stbi__get8(s);
                     اگر (stbi__at_eof(s))   واپس stbi__errpuc("bad file","file too short (pure read count)");

                     اگر (گنتی > بایاں)
                        گنتی = (stbi_uc) بایاں;

                     اگر (!stbi__readval(s,packet->channel,قدر))  واپس 0;

                     برائے(i=0; i<گنتی; ++i,dest+=4)
                        stbi__copyval(packet->channel,dest,قدر);
                     بایاں -= گنتی;
                  }
               }
               توڑو;

            حالت 2: {//Mixed RLE
               صحیح بایاں=چوڑائی;
               جب (بایاں>0) {
                  صحیح گنتی = stbi__get8(s), i;
                  اگر (stbi__at_eof(s))  واپس stbi__errpuc("bad file","file too short (mixed read count)");

                  اگر (گنتی >= 128) { // Repeated
                     stbi_uc قدر[4];

                     اگر (گنتی==128)
                        گنتی = stbi__get16be(s);
                     ورنہ
                        گنتی -= 127;
                     اگر (گنتی > بایاں)
                        واپس stbi__errpuc("bad file","scanline overrun");

                     اگر (!stbi__readval(s,packet->channel,قدر))
                        واپس 0;

                     برائے(i=0;i<گنتی;++i, dest += 4)
                        stbi__copyval(packet->channel,dest,قدر);
                  } ورنہ { // Raw
                     ++گنتی;
                     اگر (گنتی>بایاں) واپس stbi__errpuc("bad file","scanline overrun");

                     برائے(i=0;i<گنتی;++i, dest+=4)
                        اگر (!stbi__readval(s,packet->channel,dest))
                           واپس 0;
                  }
                  بایاں-=گنتی;
               }
               توڑو;
            }
         }
      }
   }

   واپس نتیجہ;
}

ساکن خالی *stbi_pic_بھرو(stbi__context *s,صحیح *px,صحیح *py,صحیح *comp,صحیح req_comp, stbi_نتیجہ_info *ri)
{
   stbi_uc *نتیجہ;
   صحیح i, x,y, اندرونی_comp;
   STBI_NOTUSED(ri);

   اگر (!comp) comp = &اندرونی_comp;

   برائے (i=0; i<92; ++i)
      stbi__get8(s);

   x = stbi__get16be(s);
   y = stbi__get16be(s);
   اگر (stbi__at_eof(s))  واپس stbi__errpuc("bad file","file too short (pic header)");
   اگر (!stbi__mad3sizes_valid(x, y, 4, 0)) واپس stbi__errpuc("too large", "PIC image too large to decode");

   stbi__get32be(s); //skip `ratio'
   stbi__get16be(s); //skip `fields'
   stbi__get16be(s); //skip `pad'

   // intermediate buffer is RGBA
   نتیجہ = (stbi_uc *) stbi__malloc_mad3(x, y, 4, 0);
   memset(نتیجہ, 0xff, x*y*4);

   اگر (!stbi_pic_بھرو_core(s,x,y,comp, نتیجہ)) {
      STBI_FREE(نتیجہ);
      نتیجہ=0;
   }
   *px = x;
   *py = y;
   اگر (req_comp == 0) req_comp = *comp;
   نتیجہ=stbi__convert_format(نتیجہ,4,req_comp,x,y);

   واپس نتیجہ;
}

ساکن صحیح stbi__pic_test(stbi__context *s)
{
   صحیح r = stbi__pic_test_core(s);
   stbi__rewind(s);
   واپس r;
}
#اگر_ختم

// *************************************************************************************************
// GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb

#اگر_غیر_تعریف_شدہ STBI_NO_GIF
قسمتعریف ساخت
{
   stbi__int16 سابقہ;
   stbi_uc پہلا;
   stbi_uc suffix;
} stbi__gif_lzw;

قسمتعریف ساخت
{
   صحیح w,h;
   stbi_uc *برآمد;                 // output buffer (always 4 components)
   stbi_uc *background;          // The current "background" as far as a gif is concerned
   stbi_uc *history; 
   صحیح flags, bgindex, تناسب, transparent, eflags;
   stbi_uc  pal[256][4];
   stbi_uc lpal[256][4];
   stbi__gif_lzw codes[8192];
   stbi_uc *color_جدول;
   صحیح parse, قدم;
   صحیح lflags;
   صحیح آغاز_x, آغاز_y;
   صحیح max_x, max_y;
   صحیح cur_x, cur_y;
   صحیح سطر_سائز;
   صحیح delay;
} stbi__gif;

ساکن صحیح stbi__gif_test_raw(stbi__context *s)
{
   صحیح sz;
   اگر (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8') واپس 0;
   sz = stbi__get8(s);
   اگر (sz != '9' && sz != '7') واپس 0;
   اگر (stbi__get8(s) != 'a') واپس 0;
   واپس 1;
}

ساکن صحیح stbi__gif_test(stbi__context *s)
{
   صحیح r = stbi__gif_test_raw(s);
   stbi__rewind(s);
   واپس r;
}

ساکن خالی stbi__gif_parse_colortable(stbi__context *s, stbi_uc pal[256][4], صحیح عدد_entries, صحیح transp)
{
   صحیح i;
   برائے (i=0; i < عدد_entries; ++i) {
      pal[i][2] = stbi__get8(s);
      pal[i][1] = stbi__get8(s);
      pal[i][0] = stbi__get8(s);
      pal[i][3] = transp == i ? 0 : 255;
   }
}

ساکن صحیح stbi_gif_سرخی(stbi__context *s, stbi__gif *g, صحیح *comp, صحیح is_info)
{
   stbi_uc نسخہ;
   اگر (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8')
      واپس stbi__err("not GIF", "Corrupt GIF");

   نسخہ = stbi__get8(s);
   اگر (نسخہ != '7' && نسخہ != '9')    واپس stbi__err("not GIF", "Corrupt GIF");
   اگر (stbi__get8(s) != 'a')                واپس stbi__err("not GIF", "Corrupt GIF");

   stbi__g_failure_reason = "";
   g->w = stbi__get16le(s);
   g->h = stbi__get16le(s);
   g->flags = stbi__get8(s);
   g->bgindex = stbi__get8(s);
   g->تناسب = stbi__get8(s);
   g->transparent = -1;

   اگر (comp != 0) *comp = 4;  // can't actually tell whether it's 3 or 4 until we parse the comments

   اگر (is_info) واپس 1;

   اگر (g->flags & 0x80)
      stbi__gif_parse_colortable(s,g->pal, 2 << (g->flags & 7), -1);

   واپس 1;
}

ساکن صحیح stbi__gif_info_raw(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   stbi__gif* g = (stbi__gif*) stbi__malloc(حجم(stbi__gif));
   اگر (!stbi_gif_سرخی(s, g, comp, 1)) {
      STBI_FREE(g);
      stbi__rewind( s );
      واپس 0;
   }
   اگر (x) *x = g->w;
   اگر (y) *y = g->h;
   STBI_FREE(g);
   واپس 1;
}

ساکن خالی stbi_برآمد_gif_رمز(stbi__gif *g, stbi__uint16 رمز)
{
   stbi_uc *p, *c;
   صحیح idx; 

   // recurse to decode the prefixes, since the linked-list is backwards,
   // and working backwards through an interleaved image would be nasty
   اگر (g->codes[رمز].سابقہ >= 0)
      stbi_برآمد_gif_رمز(g, g->codes[رمز].سابقہ);

   اگر (g->cur_y >= g->max_y) واپس;

   idx = g->cur_x + g->cur_y; 
   p = &g->برآمد[idx];
   g->history[idx / 4] = 1;  

   c = &g->color_جدول[g->codes[رمز].suffix * 4];
   اگر (c[3] > 128) { // don't render transparent pixels; 
      p[0] = c[2];
      p[1] = c[1];
      p[2] = c[0];
      p[3] = c[3];
   }
   g->cur_x += 4;

   اگر (g->cur_x >= g->max_x) {
      g->cur_x = g->آغاز_x;
      g->cur_y += g->قدم;

      جب (g->cur_y >= g->max_y && g->parse > 0) {
         g->قدم = (1 << g->parse) * g->سطر_سائز;
         g->cur_y = g->آغاز_y + (g->قدم >> 1);
         --g->parse;
      }
   }
}

ساکن stbi_uc *stbi__process_gif_raster(stbi__context *s, stbi__gif *g)
{
   stbi_uc lzw_cs;
   stbi__int32 len, init_رمز;
   stbi__uint32 پہلا;
   stbi__int32 codesize, codemask, avail, oldcode, بٹ, valid_بٹ, clear;
   stbi__gif_lzw *p;

   lzw_cs = stbi__get8(s);
   اگر (lzw_cs > 12) واپس تہی;
   clear = 1 << lzw_cs;
   پہلا = 1;
   codesize = lzw_cs + 1;
   codemask = (1 << codesize) - 1;
   بٹ = 0;
   valid_بٹ = 0;
   برائے (init_رمز = 0; init_رمز < clear; init_رمز++) {
      g->codes[init_رمز].سابقہ = -1;
      g->codes[init_رمز].پہلا = (stbi_uc) init_رمز;
      g->codes[init_رمز].suffix = (stbi_uc) init_رمز;
   }

   // support no starting clear code
   avail = clear+2;
   oldcode = -1;

   len = 0;
   برائے(;;) {
      اگر (valid_بٹ < codesize) {
         اگر (len == 0) {
            len = stbi__get8(s); // start new block
            اگر (len == 0)
               واپس g->برآمد;
         }
         --len;
         بٹ |= (stbi__int32) stbi__get8(s) << valid_بٹ;
         valid_بٹ += 8;
      } ورنہ {
         stbi__int32 رمز = بٹ & codemask;
         بٹ >>= codesize;
         valid_بٹ -= codesize;
         // @OPTIMIZE: is there some way we can accelerate the non-clear path?
         اگر (رمز == clear) {  // clear code
            codesize = lzw_cs + 1;
            codemask = (1 << codesize) - 1;
            avail = clear + 2;
            oldcode = -1;
            پہلا = 0;
         } ورنہ اگر (رمز == clear + 1) { // end of stream code
            stbi__skip(s, len);
            جب ((len = stbi__get8(s)) > 0)
               stbi__skip(s,len);
            واپس g->برآمد;
         } ورنہ اگر (رمز <= avail) {
            اگر (پہلا) {
               واپس stbi__errpuc("no clear code", "Corrupt GIF");
            }

            اگر (oldcode >= 0) {
               p = &g->codes[avail++];
               اگر (avail > 8192) {
                  واپس stbi__errpuc("too many codes", "Corrupt GIF");
               }

               p->سابقہ = (stbi__int16) oldcode;
               p->پہلا = g->codes[oldcode].پہلا;
               p->suffix = (رمز == avail) ? p->پہلا : g->codes[رمز].پہلا;
            } ورنہ اگر (رمز == avail)
               واپس stbi__errpuc("illegal code in raster", "Corrupt GIF");

            stbi_برآمد_gif_رمز(g, (stbi__uint16) رمز);

            اگر ((avail & codemask) == 0 && avail <= 0x0FFF) {
               codesize++;
               codemask = (1 << codesize) - 1;
            }

            oldcode = رمز;
         } ورنہ {
            واپس stbi__errpuc("illegal code in raster", "Corrupt GIF");
         }
      }
   }
}

// this function is designed to support animated gifs, although stb_image doesn't support it
// two back is the image from two frames ago, used for a very specific disposal format
ساکن stbi_uc *stbi_gif_بھرو_اگلا(stbi__context *s, stbi__gif *g, صحیح *comp, صحیح req_comp, stbi_uc *two_عقب)
{
   صحیح dispose; 
   صحیح پہلا_چوکھٹا; 
   صحیح pi; 
   صحیح pcount; 

   // on first frame, any non-written pixels get the background colour (non-transparent)
   پہلا_چوکھٹا = 0; 
   اگر (g->برآمد == 0) {
      اگر (!stbi_gif_سرخی(s, g, comp,0))     واپس 0; // stbi__g_failure_reason set by stbi__gif_header
      g->برآمد = (stbi_uc *) stbi__malloc(4 * g->w * g->h);
      g->background = (stbi_uc *) stbi__malloc(4 * g->w * g->h); 
      g->history = (stbi_uc *) stbi__malloc(g->w * g->h); 
      اگر (g->برآمد == 0)                      واپس stbi__errpuc("outofmem", "Out of memory");

      // image is treated as "tranparent" at the start - ie, nothing overwrites the current background; 
      // background colour is only used for pixels that are not rendered first frame, after that "background"
      // color refers to teh color that was there the previous frame. 
      memset( g->برآمد, 0x00, 4 * g->w * g->h ); 
      memset( g->background, 0x00, 4 * g->w * g->h ); // state of the background (starts transparent)
      memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame
      پہلا_چوکھٹا = 1; 
   } ورنہ {
      // second frame - how do we dispoase of the previous one?
      dispose = (g->eflags & 0x1C) >> 2; 
      pcount = g->w * g->h; 

      اگر ((dispose == 3) && (two_عقب == 0)) {
         dispose = 2; // if I don't have an image to revert back to, default to the old background
      }

      اگر (dispose == 3) { // use previous graphic
         برائے (pi = 0; pi < pcount; ++pi) {
            اگر (g->history[pi]) {
               حافظہ_نقل( &g->برآمد[pi * 4], &two_عقب[pi * 4], 4 ); 
            }
         }
      } ورنہ اگر (dispose == 2) { 
         // restore what was changed last frame to background before that frame; 
         برائے (pi = 0; pi < pcount; ++pi) {
            اگر (g->history[pi]) {
               حافظہ_نقل( &g->برآمد[pi * 4], &g->background[pi * 4], 4 ); 
            }
         }
      } ورنہ {
         // This is a non-disposal case eithe way, so just 
         // leave the pixels as is, and they will become the new background
         // 1: do not dispose
         // 0:  not specified.
      }

      // background is what out is after the undoing of the previou frame; 
      حافظہ_نقل( g->background, g->برآمد, 4 * g->w * g->h ); 
   }

   // clear my history; 
   memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame

   برائے (;;) {
      صحیح لیبل = stbi__get8(s); 
      انتخاب (لیبل) {
         حالت 0x2C: /* Image Descriptor */
         {
            stbi__int32 x, y, w, h;
            stbi_uc *o;

            x = stbi__get16le(s);
            y = stbi__get16le(s);
            w = stbi__get16le(s);
            h = stbi__get16le(s);
            اگر (((x + w) > (g->w)) || ((y + h) > (g->h)))
               واپس stbi__errpuc("bad Image Descriptor", "Corrupt GIF");

            g->سطر_سائز = g->w * 4;
            g->آغاز_x = x * 4;
            g->آغاز_y = y * g->سطر_سائز;
            g->max_x   = g->آغاز_x + w * 4;
            g->max_y   = g->آغاز_y + h * g->سطر_سائز;
            g->cur_x   = g->آغاز_x;
            g->cur_y   = g->آغاز_y;

            g->lflags = stbi__get8(s);

            اگر (g->lflags & 0x40) {
               g->قدم = 8 * g->سطر_سائز; // first interlaced spacing
               g->parse = 3;
            } ورنہ {
               g->قدم = g->سطر_سائز;
               g->parse = 0;
            }

            اگر (g->lflags & 0x80) {
               stbi__gif_parse_colortable(s,g->lpal, 2 << (g->lflags & 7), g->eflags & 0x01 ? g->transparent : -1);
               g->color_جدول = (stbi_uc *) g->lpal;
            } ورنہ اگر (g->flags & 0x80) {
               g->color_جدول = (stbi_uc *) g->pal;
            } ورنہ
               واپس stbi__errpuc("missing color table", "Corrupt GIF");            
            
            o = stbi__process_gif_raster(s, g);
            اگر (o == تہی) واپس تہی;

            // if this was the first frame, 
            pcount = g->w * g->h; 
            اگر (پہلا_چوکھٹا && (g->bgindex > 0)) {
               // if first frame, any pixel not drawn to gets the background color
               برائے (pi = 0; pi < pcount; ++pi) {
                  اگر (g->history[pi] == 0) {
                     g->pal[g->bgindex][3] = 255; // just in case it was made transparent, undo that; It will be reset next frame if need be; 
                     حافظہ_نقل( &g->برآمد[pi * 4], &g->pal[g->bgindex], 4 ); 
                  }
               }
            }

            واپس o;
         }

         حالت 0x21: // Comment Extension.
         {
            صحیح len;
            صحیح ext = stbi__get8(s); 
            اگر (ext == 0xF9) { // Graphic Control Extension.
               len = stbi__get8(s);
               اگر (len == 4) {
                  g->eflags = stbi__get8(s);
                  g->delay = 10 * stbi__get16le(s); // delay - 1/100th of a second, saving as 1/1000ths.

                  // unset old transparent
                  اگر (g->transparent >= 0) {
                     g->pal[g->transparent][3] = 255; 
                  } 
                  اگر (g->eflags & 0x01) {
                     g->transparent = stbi__get8(s);
                     اگر (g->transparent >= 0) {
                        g->pal[g->transparent][3] = 0; 
                     }
                  } ورنہ {
                     // don't need transparent
                     stbi__skip(s, 1); 
                     g->transparent = -1; 
                  }
               } ورنہ {
                  stbi__skip(s, len);
                  توڑو;
               }
            } 
            جب ((len = stbi__get8(s)) != 0) {
               stbi__skip(s, len);
            }
            توڑو;
         }

         حالت 0x3B: // gif stream termination code
            واپس (stbi_uc *) s; // using '1' causes warning on some compilers

         طےشدہ:
            واپس stbi__errpuc("unknown code", "Corrupt GIF");
      }
   }
}

ساکن خالی *stbi_بھرو_gif_main(stbi__context *s, صحیح **delays, صحیح *x, صحیح *y, صحیح *z, صحیح *comp, صحیح req_comp)
{
   اگر (stbi__gif_test(s)) {
      صحیح layers = 0; 
      stbi_uc *u = 0;
      stbi_uc *برآمد = 0;
      stbi_uc *two_عقب = 0; 
      stbi__gif g;
      صحیح stride; 
      memset(&g, 0, حجم(g));
      اگر (delays) {
         *delays = 0; 
      }

      کرو {
         u = stbi_gif_بھرو_اگلا(s, &g, comp, req_comp, two_عقب);
         اگر (u == (stbi_uc *) s) u = 0;  // end of animated gif marker

         اگر (u) {
            *x = g.w;
            *y = g.h;
            ++layers; 
            stride = g.w * g.h * 4; 
         
            اگر (برآمد) {
               برآمد = (stbi_uc*) STBI_REALLOC( برآمد, layers * stride ); 
               اگر (delays) {
                  *delays = (صحیح*) STBI_REALLOC( *delays, حجم(صحیح) * layers ); 
               }
            } ورنہ {
               برآمد = (stbi_uc*)stbi__malloc( layers * stride ); 
               اگر (delays) {
                  *delays = (صحیح*) stbi__malloc( layers * حجم(صحیح) ); 
               }
            }
            حافظہ_نقل( برآمد + ((layers - 1) * stride), u, stride ); 
            اگر (layers >= 2) {
               two_عقب = برآمد - 2 * stride; 
            }

            اگر (delays) {
               (*delays)[layers - 1U] = g.delay; 
            }
         }
      } جب (u != 0); 

      // free temp buffer; 
      STBI_FREE(g.برآمد); 
      STBI_FREE(g.history); 
      STBI_FREE(g.background); 

      // do the final conversion after loading everything; 
      اگر (req_comp && req_comp != 4)
         برآمد = stbi__convert_format(برآمد, 4, req_comp, layers * g.w, g.h);

      *z = layers; 
      واپس برآمد;
   } ورنہ {
      واپس stbi__errpuc("not GIF", "Image was not as a gif type."); 
   }
}

ساکن خالی *stbi_gif_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   stbi_uc *u = 0;
   stbi__gif g;
   memset(&g, 0, حجم(g));

   u = stbi_gif_بھرو_اگلا(s, &g, comp, req_comp, 0);
   اگر (u == (stbi_uc *) s) u = 0;  // end of animated gif marker
   اگر (u) {
      *x = g.w;
      *y = g.h;

      // moved conversion to after successful load so that the same
      // can be done for multiple frames. 
      اگر (req_comp && req_comp != 4)
         u = stbi__convert_format(u, 4, req_comp, g.w, g.h);
   }

   // free buffers needed for multiple frame loading; 
   STBI_FREE(g.history);
   STBI_FREE(g.background); 

   واپس u;
}

ساکن صحیح stbi__gif_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   واپس stbi__gif_info_raw(s,x,y,comp);
}
#اگر_ختم

// *************************************************************************************************
// Radiance RGBE HDR loader
// originally by Nicolas Schulz
#اگر_غیر_تعریف_شدہ STBI_NO_HDR
ساکن صحیح stbi__hdr_test_core(stbi__context *s, ثابت حرف *signature)
{
   صحیح i;
   برائے (i=0; signature[i]; ++i)
      اگر (stbi__get8(s) != signature[i])
          واپس 0;
   stbi__rewind(s);
   واپس 1;
}

ساکن صحیح stbi__hdr_test(stbi__context* s)
{
   صحیح r = stbi__hdr_test_core(s, "#?RADIANCE\n");
   stbi__rewind(s);
   اگر(!r) {
       r = stbi__hdr_test_core(s, "#?RGBE\n");
       stbi__rewind(s);
   }
   واپس r;
}

#تعریف_کرو STBI__HDR_BUFLEN  1024
ساکن حرف *stbi__hdr_gettoken(stbi__context *z, حرف *بفر)
{
   صحیح len=0;
   حرف c = '\0';

   c = (حرف) stbi__get8(z);

   جب (!stbi__at_eof(z) && c != '\n') {
      بفر[len++] = c;
      اگر (len == STBI__HDR_BUFLEN-1) {
         // flush to end of line
         جب (!stbi__at_eof(z) && stbi__get8(z) != '\n')
            ;
         توڑو;
      }
      c = (حرف) stbi__get8(z);
   }

   بفر[len] = 0;
   واپس بفر;
}

ساکن خالی stbi__hdr_convert(اعشاری *output, stbi_uc *داخلہ, صحیح req_comp)
{
   اگر ( داخلہ[3] != 0 ) {
      اعشاری f1;
      // Exponent
      f1 = (اعشاری) ldexp(1.0f, داخلہ[3] - (صحیح)(128 + 8));
      اگر (req_comp <= 2)
         output[0] = (داخلہ[0] + داخلہ[1] + داخلہ[2]) * f1 / 3;
      ورنہ {
         output[0] = داخلہ[0] * f1;
         output[1] = داخلہ[1] * f1;
         output[2] = داخلہ[2] * f1;
      }
      اگر (req_comp == 2) output[1] = 1;
      اگر (req_comp == 4) output[3] = 1;
   } ورنہ {
      انتخاب (req_comp) {
         حالت 4: output[3] = 1; /* fallthrough */
         حالت 3: output[0] = output[1] = output[2] = 0;
                 توڑو;
         حالت 2: output[1] = 1; /* fallthrough */
         حالت 1: output[0] = 0;
                 توڑو;
      }
   }
}

ساکن اعشاری *stbi_hdr_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   حرف بفر[STBI__HDR_BUFLEN];
   حرف *token;
   صحیح valid = 0;
   صحیح چوڑائی, اونچائی;
   stbi_uc *scanline;
   اعشاری *hdr_ڈیٹا;
   صحیح len;
   غیرعلامتی حرف گنتی, قدر;
   صحیح i, j, k, c1,c2, z;
   ثابت حرف *سرخی_Token;
   STBI_NOTUSED(ri);

   // Check identifier
   سرخی_Token = stbi__hdr_gettoken(s,بفر);
   اگر (strcmp(سرخی_Token, "#?RADIANCE") != 0 && strcmp(سرخی_Token, "#?RGBE") != 0)
      واپس stbi__errpf("not HDR", "Corrupt HDR image");

   // Parse header
   برائے(;;) {
      token = stbi__hdr_gettoken(s,بفر);
      اگر (token[0] == 0) توڑو;
      اگر (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   اگر (!valid)    واپس stbi__errpf("unsupported format", "Unsupported HDR format");

   // Parse width and height
   // can't use sscanf() if we're not using stdio!
   token = stbi__hdr_gettoken(s,بفر);
   اگر (لڑی_موازنہ(token, "-Y ", 3))  واپس stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   اونچائی = (صحیح) strtol(token, &token, 10);
   جب (*token == ' ') ++token;
   اگر (لڑی_موازنہ(token, "+X ", 3))  واپس stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   چوڑائی = (صحیح) strtol(token, تہی, 10);

   *x = چوڑائی;
   *y = اونچائی;

   اگر (comp) *comp = 3;
   اگر (req_comp == 0) req_comp = 3;

   اگر (!stbi__mad4sizes_valid(چوڑائی, اونچائی, req_comp, حجم(اعشاری), 0))
      واپس stbi__errpf("too large", "HDR image is too large");

   // Read data
   hdr_ڈیٹا = (اعشاری *) stbi__malloc_mad4(چوڑائی, اونچائی, req_comp, حجم(اعشاری), 0);
   اگر (!hdr_ڈیٹا)
      واپس stbi__errpf("outofmem", "Out of memory");

   // Load image data
   // image data is stored as some number of sca
   اگر ( چوڑائی < 8 || چوڑائی >= 32768) {
      // Read flat data
      برائے (j=0; j < اونچائی; ++j) {
         برائے (i=0; i < چوڑائی; ++i) {
            stbi_uc rgbe[4];
           main_decode_loop:
            stbi__getn(s, rgbe, 4);
            stbi__hdr_convert(hdr_ڈیٹا + j * چوڑائی * req_comp + i * req_comp, rgbe, req_comp);
         }
      }
   } ورنہ {
      // Read RLE-encoded data
      scanline = تہی;

      برائے (j = 0; j < اونچائی; ++j) {
         c1 = stbi__get8(s);
         c2 = stbi__get8(s);
         len = stbi__get8(s);
         اگر (c1 != 2 || c2 != 2 || (len & 0x80)) {
            // not run-length encoded, so we have to actually use THIS data as a decoded
            // pixel (note this can't be a valid pixel--one of RGB must be >= 128)
            stbi_uc rgbe[4];
            rgbe[0] = (stbi_uc) c1;
            rgbe[1] = (stbi_uc) c2;
            rgbe[2] = (stbi_uc) len;
            rgbe[3] = (stbi_uc) stbi__get8(s);
            stbi__hdr_convert(hdr_ڈیٹا, rgbe, req_comp);
            i = 1;
            j = 0;
            STBI_FREE(scanline);
            جاؤ main_decode_loop; // yes, this makes no sense
         }
         len <<= 8;
         len |= stbi__get8(s);
         اگر (len != چوڑائی) { STBI_FREE(hdr_ڈیٹا); STBI_FREE(scanline); واپس stbi__errpf("invalid decoded scanline length", "corrupt HDR"); }
         اگر (scanline == تہی) {
            scanline = (stbi_uc *) stbi__malloc_mad2(چوڑائی, 4, 0);
            اگر (!scanline) {
               STBI_FREE(hdr_ڈیٹا);
               واپس stbi__errpf("outofmem", "Out of memory");
            }
         }

         برائے (k = 0; k < 4; ++k) {
            صحیح nleft;
            i = 0;
            جب ((nleft = چوڑائی - i) > 0) {
               گنتی = stbi__get8(s);
               اگر (گنتی > 128) {
                  // Run
                  قدر = stbi__get8(s);
                  گنتی -= 128;
                  اگر (گنتی > nleft) { STBI_FREE(hdr_ڈیٹا); STBI_FREE(scanline); واپس stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  برائے (z = 0; z < گنتی; ++z)
                     scanline[i++ * 4 + k] = قدر;
               } ورنہ {
                  // Dump
                  اگر (گنتی > nleft) { STBI_FREE(hdr_ڈیٹا); STBI_FREE(scanline); واپس stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  برائے (z = 0; z < گنتی; ++z)
                     scanline[i++ * 4 + k] = stbi__get8(s);
               }
            }
         }
         برائے (i=0; i < چوڑائی; ++i)
            stbi__hdr_convert(hdr_ڈیٹا+(j*چوڑائی + i)*req_comp, scanline + i*4, req_comp);
      }
      اگر (scanline)
         STBI_FREE(scanline);
   }

   واپس hdr_ڈیٹا;
}

ساکن صحیح stbi__hdr_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   حرف بفر[STBI__HDR_BUFLEN];
   حرف *token;
   صحیح valid = 0;
   صحیح dummy;

   اگر (!x) x = &dummy;
   اگر (!y) y = &dummy;
   اگر (!comp) comp = &dummy;

   اگر (stbi__hdr_test(s) == 0) {
       stbi__rewind( s );
       واپس 0;
   }

   برائے(;;) {
      token = stbi__hdr_gettoken(s,بفر);
      اگر (token[0] == 0) توڑو;
      اگر (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   اگر (!valid) {
       stbi__rewind( s );
       واپس 0;
   }
   token = stbi__hdr_gettoken(s,بفر);
   اگر (لڑی_موازنہ(token, "-Y ", 3)) {
       stbi__rewind( s );
       واپس 0;
   }
   token += 3;
   *y = (صحیح) strtol(token, &token, 10);
   جب (*token == ' ') ++token;
   اگر (لڑی_موازنہ(token, "+X ", 3)) {
       stbi__rewind( s );
       واپس 0;
   }
   token += 3;
   *x = (صحیح) strtol(token, تہی, 10);
   *comp = 3;
   واپس 1;
}
#اگر_ختم // STBI_NO_HDR

#اگر_غیر_تعریف_شدہ STBI_NO_BMP
ساکن صحیح stbi__bmp_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   خالی *p;
   stbi_bmp_ڈیٹا info;

   info.all_a = 255;
   p = stbi_bmp_parse_سرخی(s, &info);
   stbi__rewind( s );
   اگر (p == تہی)
      واپس 0;
   اگر (x) *x = s->img_x;
   اگر (y) *y = s->img_y;
   اگر (comp) *comp = info.ma ? 4 : 3;
   واپس 1;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_PSD
ساکن صحیح stbi__psd_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   صحیح channel_گنتی, dummy, گہرائی;
   اگر (!x) x = &dummy;
   اگر (!y) y = &dummy;
   اگر (!comp) comp = &dummy;
   اگر (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       واپس 0;
   }
   اگر (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       واپس 0;
   }
   stbi__skip(s, 6);
   channel_گنتی = stbi__get16be(s);
   اگر (channel_گنتی < 0 || channel_گنتی > 16) {
       stbi__rewind( s );
       واپس 0;
   }
   *y = stbi__get32be(s);
   *x = stbi__get32be(s);
   گہرائی = stbi__get16be(s);
   اگر (گہرائی != 8 && گہرائی != 16) {
       stbi__rewind( s );
       واپس 0;
   }
   اگر (stbi__get16be(s) != 3) {
       stbi__rewind( s );
       واپس 0;
   }
   *comp = 4;
   واپس 1;
}

ساکن صحیح stbi__psd_is16(stbi__context *s)
{
   صحیح channel_گنتی, گہرائی;
   اگر (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       واپس 0;
   }
   اگر (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       واپس 0;
   }
   stbi__skip(s, 6);
   channel_گنتی = stbi__get16be(s);
   اگر (channel_گنتی < 0 || channel_گنتی > 16) {
       stbi__rewind( s );
       واپس 0;
   }
   (خالی) stbi__get32be(s);
   (خالی) stbi__get32be(s);
   گہرائی = stbi__get16be(s);
   اگر (گہرائی != 16) {
       stbi__rewind( s );
       واپس 0;
   }
   واپس 1;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ STBI_NO_PIC
ساکن صحیح stbi__pic_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   صحیح act_comp=0,عدد_packets=0,chained,dummy;
   stbi__pic_packet packets[10];

   اگر (!x) x = &dummy;
   اگر (!y) y = &dummy;
   اگر (!comp) comp = &dummy;

   اگر (!stbi__pic_is4(s,"\x53\x80\xF6\x34")) {
      stbi__rewind(s);
      واپس 0;
   }

   stbi__skip(s, 88);

   *x = stbi__get16be(s);
   *y = stbi__get16be(s);
   اگر (stbi__at_eof(s)) {
      stbi__rewind( s);
      واپس 0;
   }
   اگر ( (*x) != 0 && (1 << 28) / (*x) < (*y)) {
      stbi__rewind( s );
      واپس 0;
   }

   stbi__skip(s, 8);

   کرو {
      stbi__pic_packet *packet;

      اگر (عدد_packets==حجم(packets)/حجم(packets[0]))
         واپس 0;

      packet = &packets[عدد_packets++];
      chained = stbi__get8(s);
      packet->سائز    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);
      act_comp |= packet->channel;

      اگر (stbi__at_eof(s)) {
          stbi__rewind( s );
          واپس 0;
      }
      اگر (packet->سائز != 8) {
          stbi__rewind( s );
          واپس 0;
      }
   } جب (chained);

   *comp = (act_comp & 0x10 ? 4 : 3);

   واپس 1;
}
#اگر_ختم

// *************************************************************************************************
// Portable Gray Map and Portable Pixel Map loader
// by Ken Miller
//
// PGM: http://netpbm.sourceforge.net/doc/pgm.html
// PPM: http://netpbm.sourceforge.net/doc/ppm.html
//
// Known limitations:
//    Does not support comments in the header section
//    Does not support ASCII image data (formats P2 and P3)
//    Does not support 16-bit-per-channel

#اگر_غیر_تعریف_شدہ STBI_NO_PNM

ساکن صحیح      stbi__pnm_test(stbi__context *s)
{
   حرف p, t;
   p = (حرف) stbi__get8(s);
   t = (حرف) stbi__get8(s);
   اگر (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind( s );
       واپس 0;
   }
   واپس 1;
}

ساکن خالی *stbi_pnm_بھرو(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp, صحیح req_comp, stbi_نتیجہ_info *ri)
{
   stbi_uc *برآمد;
   STBI_NOTUSED(ri);

   اگر (!stbi__pnm_info(s, (صحیح *)&s->img_x, (صحیح *)&s->img_y, (صحیح *)&s->img_n))
      واپس 0;

   *x = s->img_x;
   *y = s->img_y;
   اگر (comp) *comp = s->img_n;

   اگر (!stbi__mad3sizes_valid(s->img_n, s->img_x, s->img_y, 0))
      واپس stbi__errpuc("too large", "PNM too large");

   برآمد = (stbi_uc *) stbi__malloc_mad3(s->img_n, s->img_x, s->img_y, 0);
   اگر (!برآمد) واپس stbi__errpuc("outofmem", "Out of memory");
   stbi__getn(s, برآمد, s->img_n * s->img_x * s->img_y);

   اگر (req_comp && req_comp != s->img_n) {
      برآمد = stbi__convert_format(برآمد, s->img_n, req_comp, s->img_x, s->img_y);
      اگر (برآمد == تہی) واپس برآمد; // stbi__convert_format frees input on failure
   }
   واپس برآمد;
}

ساکن صحیح      stbi__pnm_isspace(حرف c)
{
   واپس c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

ساکن خالی     stbi__pnm_skip_whitespace(stbi__context *s, حرف *c)
{
   برائے (;;) {
      جب (!stbi__at_eof(s) && stbi__pnm_isspace(*c))
         *c = (حرف) stbi__get8(s);

      اگر (stbi__at_eof(s) || *c != '#')
         توڑو;

      جب (!stbi__at_eof(s) && *c != '\n' && *c != '\r' )
         *c = (حرف) stbi__get8(s);
   }
}

ساکن صحیح      stbi__pnm_isdigit(حرف c)
{
   واپس c >= '0' && c <= '9';
}

ساکن صحیح      stbi__pnm_getinteger(stbi__context *s, حرف *c)
{
   صحیح قدر = 0;

   جب (!stbi__at_eof(s) && stbi__pnm_isdigit(*c)) {
      قدر = قدر*10 + (*c - '0');
      *c = (حرف) stbi__get8(s);
   }

   واپس قدر;
}

ساکن صحیح      stbi__pnm_info(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   صحیح maxv, dummy;
   حرف c, p, t;

   اگر (!x) x = &dummy;
   اگر (!y) y = &dummy;
   اگر (!comp) comp = &dummy;

   stbi__rewind(s);

   // Get identifier
   p = (حرف) stbi__get8(s);
   t = (حرف) stbi__get8(s);
   اگر (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind(s);
       واپس 0;
   }

   *comp = (t == '6') ? 3 : 1;  // '5' is 1-component .pgm; '6' is 3-component .ppm

   c = (حرف) stbi__get8(s);
   stbi__pnm_skip_whitespace(s, &c);

   *x = stbi__pnm_getinteger(s, &c); // read width
   stbi__pnm_skip_whitespace(s, &c);

   *y = stbi__pnm_getinteger(s, &c); // read height
   stbi__pnm_skip_whitespace(s, &c);

   maxv = stbi__pnm_getinteger(s, &c);  // read max value

   اگر (maxv > 255)
      واپس stbi__err("max value > 255", "PPM image not 8-bit");
   ورنہ
      واپس 1;
}
#اگر_ختم

ساکن صحیح stbi__info_main(stbi__context *s, صحیح *x, صحیح *y, صحیح *comp)
{
   #اگر_غیر_تعریف_شدہ STBI_NO_JPEG
   اگر (stbi__jpeg_info(s, x, y, comp)) واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_PNG
   اگر (stbi__png_info(s, x, y, comp))  واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_GIF
   اگر (stbi__gif_info(s, x, y, comp))  واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_BMP
   اگر (stbi__bmp_info(s, x, y, comp))  واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_PSD
   اگر (stbi__psd_info(s, x, y, comp))  واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_PIC
   اگر (stbi__pic_info(s, x, y, comp))  واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_PNM
   اگر (stbi__pnm_info(s, x, y, comp))  واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_HDR
   اگر (stbi__hdr_info(s, x, y, comp))  واپس 1;
   #اگر_ختم

   // test tga last because it's a crappy test!
   #اگر_غیر_تعریف_شدہ STBI_NO_TGA
   اگر (stbi__tga_info(s, x, y, comp))
       واپس 1;
   #اگر_ختم
   واپس stbi__err("unknown image type", "Image not of any known type, or corrupt");
}

ساکن صحیح stbi__is_16_main(stbi__context *s)
{
   #اگر_غیر_تعریف_شدہ STBI_NO_PNG
   اگر (stbi__png_is16(s))  واپس 1;
   #اگر_ختم

   #اگر_غیر_تعریف_شدہ STBI_NO_PSD
   اگر (stbi__psd_is16(s))  واپس 1;
   #اگر_ختم

   واپس 0;
}

#اگر_غیر_تعریف_شدہ STBI_NO_STDIO
STBIDEF صحیح stbi_info(حرف ثابت *filename, صحیح *x, صحیح *y, صحیح *comp)
{
    FILE *f = stbi__fopen(filename, "rb");
    صحیح نتیجہ;
    اگر (!f) واپس stbi__err("can't fopen", "Unable to open file");
    نتیجہ = stbi_info_from_file(f, x, y, comp);
    fclose(f);
    واپس نتیجہ;
}

STBIDEF صحیح stbi_info_from_file(FILE *f, صحیح *x, صحیح *y, صحیح *comp)
{
   صحیح r;
   stbi__context s;
   طویل pos = ftell(f);
   stbi_آغاز_file(&s, f);
   r = stbi__info_main(&s,x,y,comp);
   fseek(f,pos,SEEK_SET);
   واپس r;
}

STBIDEF صحیح stbi_is_16_ثنائی_ہندسہ(حرف ثابت *filename)
{
    FILE *f = stbi__fopen(filename, "rb");
    صحیح نتیجہ;
    اگر (!f) واپس stbi__err("can't fopen", "Unable to open file");
    نتیجہ = stbi_is_16_ثنائی_ہندسہ_from_file(f);
    fclose(f);
    واپس نتیجہ;
}

STBIDEF صحیح stbi_is_16_ثنائی_ہندسہ_from_file(FILE *f)
{
   صحیح r;
   stbi__context s;
   طویل pos = ftell(f);
   stbi_آغاز_file(&s, f);
   r = stbi__is_16_main(&s);
   fseek(f,pos,SEEK_SET);
   واپس r;
}
#اگر_ختم // !STBI_NO_STDIO

STBIDEF صحیح stbi_info_from_حافظہ(stbi_uc ثابت *بفر, صحیح len, صحیح *x, صحیح *y, صحیح *comp)
{
   stbi__context s;
   stbi_آغاز_mem(&s,بفر,len);
   واپس stbi__info_main(&s,x,y,comp);
}

STBIDEF صحیح stbi_info_from_callbacks(stbi_io_callbacks ثابت *c, خالی *user, صحیح *x, صحیح *y, صحیح *comp)
{
   stbi__context s;
   stbi_آغاز_callbacks(&s, (stbi_io_callbacks *) c, user);
   واپس stbi__info_main(&s,x,y,comp);
}

STBIDEF صحیح stbi_is_16_ثنائی_ہندسہ_from_حافظہ(stbi_uc ثابت *بفر, صحیح len)
{
   stbi__context s;
   stbi_آغاز_mem(&s,بفر,len);
   واپس stbi__is_16_main(&s);
}

STBIDEF صحیح stbi_is_16_ثنائی_ہندسہ_from_callbacks(stbi_io_callbacks ثابت *c, خالی *user)
{
   stbi__context s;
   stbi_آغاز_callbacks(&s, (stbi_io_callbacks *) c, user);
   واپس stbi__is_16_main(&s);
}

#اگر_ختم // STB_IMAGE_IMPLEMENTATION

/*
   revision history:
      2.19  (2018-02-11) fix warning
      2.18  (2018-01-30) fix warnings
      2.17  (2018-01-29) change sbti__shiftsigned to avoid clang -O2 bug
                         1-bit BMP
                         *_is_16_bit api
                         avoid warnings
      2.16  (2017-07-23) all functions have 16-bit variants;
                         STBI_NO_STDIO works again;
                         compilation fixes;
                         fix rounding in unpremultiply;
                         optimize vertical flip;
                         disable raw_len validation;
                         documentation fixes
      2.15  (2017-03-18) fix png-1,2,4 bug; now all Imagenet JPGs decode;
                         warning fixes; disable run-time SSE detection on gcc;
                         uniform handling of optional "return" values;
                         thread-safe initialization of zlib tables
      2.14  (2017-03-03) remove deprecated STBI_JPEG_OLD; fixes for Imagenet JPGs
      2.13  (2016-11-29) add 16-bit API, only supported for PNG right now
      2.12  (2016-04-02) fix typo in 2.11 PSD fix that caused crashes
      2.11  (2016-04-02) allocate large structures on the stack
                         remove white matting for transparent PSD
                         fix reported channel count for PNG & BMP
                         re-enable SSE2 in non-gcc 64-bit
                         support RGB-formatted JPEG
                         read 16-bit PNGs (only as 8-bit)
      2.10  (2016-01-22) avoid warning introduced in 2.09 by STBI_REALLOC_SIZED
      2.09  (2016-01-16) allow comments in PNM files
                         16-bit-per-pixel TGA (not bit-per-component)
                         info() for TGA could break due to .hdr handling
                         info() for BMP to shares code instead of sloppy parse
                         can use STBI_REALLOC_SIZED if allocator doesn't support realloc
                         code cleanup
      2.08  (2015-09-13) fix to 2.07 cleanup, reading RGB PSD as RGBA
      2.07  (2015-09-13) fix compiler warnings
                         partial animated GIF support
                         limited 16-bpc PSD support
                         #ifdef unused functions
                         bug with < 92 byte PIC,PNM,HDR,TGA
      2.06  (2015-04-19) fix bug where PSD returns wrong '*comp' value
      2.05  (2015-04-19) fix bug in progressive JPEG handling, fix warning
      2.04  (2015-04-15) try to re-enable SIMD on MinGW 64-bit
      2.03  (2015-04-12) extra corruption checking (mmozeiko)
                         stbi_set_flip_vertically_on_load (nguillemot)
                         fix NEON support; fix mingw support
      2.02  (2015-01-19) fix incorrect assert, fix warning
      2.01  (2015-01-17) fix various warnings; suppress SIMD on gcc 32-bit without -msse2
      2.00b (2014-12-25) fix STBI_MALLOC in progressive JPEG
      2.00  (2014-12-25) optimize JPG, including x86 SSE2 & NEON SIMD (ryg)
                         progressive JPEG (stb)
                         PGM/PPM support (Ken Miller)
                         STBI_MALLOC,STBI_REALLOC,STBI_FREE
                         GIF bugfix -- seemingly never worked
                         STBI_NO_*, STBI_ONLY_*
      1.48  (2014-12-14) fix incorrectly-named assert()
      1.47  (2014-12-14) 1/2/4-bit PNG support, both direct and paletted (Omar Cornut & stb)
                         optimize PNG (ryg)
                         fix bug in interlaced PNG with user-specified channel count (stb)
      1.46  (2014-08-26)
              fix broken tRNS chunk (colorkey-style transparency) in non-paletted PNG
      1.45  (2014-08-16)
              fix MSVC-ARM internal compiler error by wrapping malloc
      1.44  (2014-08-07)
              various warning fixes from Ronny Chevalier
      1.43  (2014-07-15)
              fix MSVC-only compiler problem in code changed in 1.42
      1.42  (2014-07-09)
              don't define _CRT_SECURE_NO_WARNINGS (affects user code)
              fixes to stbi__cleanup_jpeg path
              added STBI_ASSERT to avoid requiring assert.h
      1.41  (2014-06-25)
              fix search&replace from 1.36 that messed up comments/error messages
      1.40  (2014-06-22)
              fix gcc struct-initialization warning
      1.39  (2014-06-15)
              fix to TGA optimization when req_comp != number of components in TGA;
              fix to GIF loading because BMP wasn't rewinding (whoops, no GIFs in my test suite)
              add support for BMP version 5 (more ignored fields)
      1.38  (2014-06-06)
              suppress MSVC warnings on integer casts truncating values
              fix accidental rename of 'skip' field of I/O
      1.37  (2014-06-04)
              remove duplicate typedef
      1.36  (2014-06-03)
              convert to header file single-file library
              if de-iphone isn't set, load iphone images color-swapped instead of returning NULL
      1.35  (2014-05-27)
              various warnings
              fix broken STBI_SIMD path
              fix bug where stbi_load_from_file no longer left file pointer in correct place
              fix broken non-easy path for 32-bit BMP (possibly never used)
              TGA optimization by Arseny Kapoulkine
      1.34  (unknown)
              use STBI_NOTUSED in stbi__resample_row_generic(), fix one more leak in tga failure case
      1.33  (2011-07-14)
              make stbi_is_hdr work in STBI_NO_HDR (as specified), minor compiler-friendly improvements
      1.32  (2011-07-13)
              support for "info" function for all supported filetypes (SpartanJ)
      1.31  (2011-06-20)
              a few more leak fixes, bug in PNG handling (SpartanJ)
      1.30  (2011-06-11)
              added ability to load files via callbacks to accomidate custom input streams (Ben Wenger)
              removed deprecated format-specific test/load functions
              removed support for installable file formats (stbi_loader) -- would have been broken for IO callbacks anyway
              error cases in bmp and tga give messages and don't leak (Raymond Barbiero, grisha)
              fix inefficiency in decoding 32-bit BMP (David Woo)
      1.29  (2010-08-16)
              various warning fixes from Aurelien Pocheville
      1.28  (2010-08-01)
              fix bug in GIF palette transparency (SpartanJ)
      1.27  (2010-08-01)
              cast-to-stbi_uc to fix warnings
      1.26  (2010-07-24)
              fix bug in file buffering for PNG reported by SpartanJ
      1.25  (2010-07-17)
              refix trans_data warning (Won Chun)
      1.24  (2010-07-12)
              perf improvements reading from files on platforms with lock-heavy fgetc()
              minor perf improvements for jpeg
              deprecated type-specific functions so we'll get feedback if they're needed
              attempt to fix trans_data warning (Won Chun)
      1.23    fixed bug in iPhone support
      1.22  (2010-07-10)
              removed image *writing* support
              stbi_info support from Jetro Lauha
              GIF support from Jean-Marc Lienher
              iPhone PNG-extensions from James Brown
              warning-fixes from Nicolas Schulz and Janez Zemva (i.stbi__err. Janez (U+017D)emva)
      1.21    fix use of 'stbi_uc' in header (reported by jon blow)
      1.20    added support for Softimage PIC, by Tom Seddon
      1.19    bug in interlaced PNG corruption check (found by ryg)
      1.18  (2008-08-02)
              fix a threading bug (local mutable static)
      1.17    support interlaced PNG
      1.16    major bugfix - stbi__convert_format converted one too many pixels
      1.15    initialize some fields for thread safety
      1.14    fix threadsafe conversion bug
              header-file-only version (#define STBI_HEADER_FILE_ONLY before including)
      1.13    threadsafe
      1.12    const qualifiers in the API
      1.11    Support installable IDCT, colorspace conversion routines
      1.10    Fixes for 64-bit (don't use "unsigned long")
              optimized upsampling by Fabian "ryg" Giesen
      1.09    Fix format-conversion for PSD code (bad global variables!)
      1.08    Thatcher Ulrich's PSD code integrated by Nicolas Schulz
      1.07    attempt to fix C++ warning/errors again
      1.06    attempt to fix C++ warning/errors again
      1.05    fix TGA loading to return correct *comp and use good luminance calc
      1.04    default float alpha is 1, not 255; use 'void *' for stbi_image_free
      1.03    bugfixes to STBI_NO_STDIO, STBI_NO_HDR
      1.02    support for (subset of) HDR files, float interface for preferred access to them
      1.01    fix bug: possible bug in handling right-side up bmps... not sure
              fix bug: the stbi__bmp_load() and stbi__tga_load() functions didn't work at all
      1.00    interface to zlib that skips zlib header
      0.99    correct handling of alpha in palette
      0.98    TGA loader by lonesock; dynamically add loaders (untested)
      0.97    jpeg errors on too large a file; also catch another malloc failure
      0.96    fix detection of invalid v value - particleman@mollyrocket forum
      0.95    during header scan, seek to markers in case of padding
      0.94    STBI_NO_STDIO to disable stdio usage; rename all #defines the same
      0.93    handle jpegtran output; verbose errors
      0.92    read 4,8,16,24,32-bit BMP files of several formats
      0.91    output 24-bit Windows 3.0 BMP files
      0.90    fix a few more warnings; bump version number to approach 1.0
      0.61    bugfixes due to Marc LeBlanc, Christopher Lloyd
      0.60    fix compiling as c++
      0.59    fix warnings: merge Dave Moore's -Wall fixes
      0.58    fix bug: zlib uncompressed mode len/nlen was wrong endian
      0.57    fix bug: jpg last huffman symbol before marker was >9 bits but less than 16 available
      0.56    fix bug: zlib uncompressed mode len vs. nlen
      0.55    fix bug: restart_interval not initialized to 0
      0.54    allow NULL for 'int *comp'
      0.53    fix bug in png 3->4; speedup png decoding
      0.52    png handles req_comp=3,4 directly; minor cleanup; jpeg comments
      0.51    obey req_comp requests, 1-component jpegs return as 1-component,
              on 'test' only check type, not whether we support this variant
      0.50  (2006-11-19)
              first released version
*/


/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
