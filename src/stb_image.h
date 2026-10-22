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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_INCLUDE_STB_IMAGE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_INCLUDE_STB_IMAGE_H

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


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ_ਅੰਤ // STBI_NO_STDIO

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_ਸੰਸਕਰਣ 1

ਗਿਣਤੀ
{
   STBI_default = 0, // only used for desired_channels

   STBI_grey       = 1,
   STBI_grey_alpha = 2,
   STBI_rgb        = 3,
   STBI_rgb_alpha  = 4
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ stbi_uc;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ stbi_us;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STB_IMAGE_STATIC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBIDEF ਸਥਿਰ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBIDEF ਬਾਹਰੀ
#ਜੇ_ਅੰਤ

//////////////////////////////////////////////////////////////////////////////
//
// PRIMARY API - works on images of any type
//

//
// load image by filename, open file, or memory buffer
//

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   ਪੂਰਨ      (*ਪਠ)  (ਖਾਲੀ *user,ਅੱਖਰ *ਅੰਕੜੇ,ਪੂਰਨ ਮਾਪ);   // fill 'data' with 'size' bytes.  return number of bytes actually read
   ਖਾਲੀ     (*skip)  (ਖਾਲੀ *user,ਪੂਰਨ n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
   ਪੂਰਨ      (*eof)   (ਖਾਲੀ *user);                       // returns nonzero if we are at end of file/data
} stbi_io_callbacks;

////////////////////////////////////
//
// 8-bits-per-channel interface
//

STBIDEF stbi_uc *stbi_ਲੱਦੋ_from_ਸਮ੍ਰਿਤੀ   (stbi_uc           ਅਚਲ *ਬਫਰ, ਪੂਰਨ len   , ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
STBIDEF stbi_uc *stbi_ਲੱਦੋ_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk  , ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_GIF
STBIDEF stbi_uc *stbi_ਲੱਦੋ_gif_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ **delays, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *z, ਪੂਰਨ *comp, ਪੂਰਨ req_comp);
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
STBIDEF stbi_uc *stbi_ਲੱਦੋ            (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
STBIDEF stbi_uc *stbi_ਲੱਦੋ_from_file  (FILE *f, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
// for stbi_load_from_file, file pointer is left pointing immediately after image
#ਜੇ_ਅੰਤ

////////////////////////////////////
//
// 16-bits-per-channel interface
//

STBIDEF stbi_us *stbi_ਲੱਦੋ_16_from_ਸਮ੍ਰਿਤੀ   (stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
STBIDEF stbi_us *stbi_ਲੱਦੋ_16_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
STBIDEF stbi_us *stbi_ਲੱਦੋ_16          (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
STBIDEF stbi_us *stbi_ਲੱਦੋ_from_file_16(FILE *f, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
#ਜੇ_ਅੰਤ

////////////////////////////////////
//
// float-per-channel interface
//
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_ਰੇਖਿਕ
   STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf_from_ਸਮ੍ਰਿਤੀ     (stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
   STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf_from_callbacks  (stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y,  ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
   STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf            (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
   STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf_from_file  (FILE *f, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels);
   #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
   STBIDEF ਖਾਲੀ   stbi_hdr_ਮੰਜ਼ਿਲ_ldr_gamma(ਦਸ਼ਮਲਵ gamma);
   STBIDEF ਖਾਲੀ   stbi_hdr_ਮੰਜ਼ਿਲ_ldr_scale(ਦਸ਼ਮਲਵ scale);
#ਜੇ_ਅੰਤ // STBI_NO_HDR

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_ਰੇਖਿਕ
   STBIDEF ਖਾਲੀ   stbi_ldr_ਮੰਜ਼ਿਲ_hdr_gamma(ਦਸ਼ਮਲਵ gamma);
   STBIDEF ਖਾਲੀ   stbi_ldr_ਮੰਜ਼ਿਲ_hdr_scale(ਦਸ਼ਮਲਵ scale);
#ਜੇ_ਅੰਤ // STBI_NO_LINEAR

// stbi_is_hdr is always defined, but always returns false if STBI_NO_HDR
STBIDEF ਪੂਰਨ    stbi_is_hdr_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user);
STBIDEF ਪੂਰਨ    stbi_is_hdr_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
STBIDEF ਪੂਰਨ      stbi_is_hdr          (ਅੱਖਰ ਅਚਲ *filename);
STBIDEF ਪੂਰਨ      stbi_is_hdr_from_file(FILE *f);
#ਜੇ_ਅੰਤ // STBI_NO_STDIO


// get a VERY brief reason for failure
// NOT THREADSAFE
STBIDEF ਅਚਲ ਅੱਖਰ *stbi_failure_reason  (ਖਾਲੀ);

// free the loaded image -- this is just free()
STBIDEF ਖਾਲੀ     stbi_image_free      (ਖਾਲੀ *retval_from_stbi_ਲੱਦੋ);

// get image dimensions & components without fully decoding
STBIDEF ਪੂਰਨ      stbi_info_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
STBIDEF ਪੂਰਨ      stbi_info_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
STBIDEF ਪੂਰਨ      stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len);
STBIDEF ਪੂਰਨ      stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
STBIDEF ਪੂਰਨ      stbi_info               (ਅੱਖਰ ਅਚਲ *filename,     ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
STBIDEF ਪੂਰਨ      stbi_info_from_file     (FILE *f,                  ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
STBIDEF ਪੂਰਨ      stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ          (ਅੱਖਰ ਅਚਲ *filename);
STBIDEF ਪੂਰਨ      stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ_from_file(FILE *f);
#ਜੇ_ਅੰਤ



// for image formats that explicitly notate that they have premultiplied alpha,
// we just return the colors as stored in the file. set this flag to force
// unpremultiplication. results are undefined if the unpremultiply overflow.
STBIDEF ਖਾਲੀ stbi_ਸੈੱਟ_unpremultiply_on_ਲੱਦੋ(ਪੂਰਨ flag_true_if_should_unpremultiply);

// indicate whether we should process iphone images back to canonical format,
// or just pass them through "as-is"
STBIDEF ਖਾਲੀ stbi_convert_iphone_png_ਮੰਜ਼ਿਲ_rgb(ਪੂਰਨ flag_true_if_should_convert);

// flip the image vertically, so the first pixel in the output array is the bottom left
STBIDEF ਖਾਲੀ stbi_ਸੈੱਟ_flip_vertically_on_ਲੱਦੋ(ਪੂਰਨ flag_true_if_should_flip);

// ZLIB client - used by PNG, available for other purposes

STBIDEF ਅੱਖਰ *stbi_zlib_decode_malloc_guesssize(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ initial_ਮਾਪ, ਪੂਰਨ *outlen);
STBIDEF ਅੱਖਰ *stbi_zlib_decode_malloc_guesssize_headerflag(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ initial_ਮਾਪ, ਪੂਰਨ *outlen, ਪੂਰਨ parse_ਸਿਰਲੇਖ);
STBIDEF ਅੱਖਰ *stbi_zlib_decode_malloc(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *outlen);
STBIDEF ਪੂਰਨ   stbi_zlib_decode_ਬਫਰ(ਅੱਖਰ *obuffer, ਪੂਰਨ olen, ਅਚਲ ਅੱਖਰ *ibuffer, ਪੂਰਨ ilen);

STBIDEF ਅੱਖਰ *stbi_zlib_decode_noheader_malloc(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *outlen);
STBIDEF ਪੂਰਨ   stbi_zlib_decode_noheader_ਬਫਰ(ਅੱਖਰ *obuffer, ਪੂਰਨ olen, ਅਚਲ ਅੱਖਰ *ibuffer, ਪੂਰਨ ilen);


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

//
//
////   end header file   /////////////////////////////////////////////////////
#ਜੇ_ਅੰਤ // STBI_INCLUDE_STB_IMAGE_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STB_IMAGE_IMPLEMENTATION

#ਜੇ defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_JPEG
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_JPEG
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_PNG
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_PNG
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_BMP
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_BMP
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_PSD
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_PSD
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_TGA
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_TGA
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_GIF
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_GIF
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_HDR
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_HDR
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_PIC
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_PIC
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ONLY_PNM
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_PNM
   #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ defined(STBI_NO_PNG) && !defined(STBI_SUPPORT_ZLIB) && !defined(STBI_NO_ZLIB)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_ZLIB
#ਜੇ_ਅੰਤ


#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h> // ptrdiff_t on osx
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਜੇ !defined(STBI_NO_ਰੇਖਿਕ) || !defined(STBI_NO_HDR)
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>  // ldexp, pow
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_ASSERT
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_ASSERT(x) ਪ੍ਰਤਿਜ੍ਞਾ(x)
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _MSC_VER
   #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi_inline ਸਤਰਅੰਦਰ
   #ਨਹੀਂਤਾਂ
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi_inline
   #ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi_inline __forceinline
#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ stbi__uint16;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ   ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ stbi__int16;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   stbi__uint32;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ   ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ   stbi__int32;
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t stbi__uint16;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ int16_t  stbi__int16;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t stbi__uint32;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ int32_t  stbi__int32;
#ਜੇ_ਅੰਤ

// should produce compiler error if size is wrong
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ validate_uint32[ਆਕਾਰ(stbi__uint32)==4 ? 1 : -1];

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NOTUSED(v)  (ਖਾਲੀ)(v)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NOTUSED(v)  (ਖਾਲੀ)ਆਕਾਰ(v)
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_HAS_LROTL
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_HAS_LROTL
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi_lrot(x,y)  _lrotl(x,y)
#ਨਹੀਂਤਾਂ
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi_lrot(x,y)  (((x) << (y)) | ((x) >> (32 - (y))))
#ਜੇ_ਅੰਤ

#ਜੇ defined(STBI_MALLOC) && defined(STBI_FREE) && (defined(STBI_REALLOC) || defined(STBI_REALLOC_SIZED))
// ok
#ਜਾਂਜੇ !defined(STBI_MALLOC) && !defined(STBI_FREE) && !defined(STBI_REALLOC) && !defined(STBI_REALLOC_SIZED)
// ok
#ਨਹੀਂਤਾਂ
#ਗਲਤੀ "Must define all or none of STBI_MALLOC, STBI_FREE, and STBI_REALLOC (or STBI_REALLOC_SIZED)."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_MALLOC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_MALLOC(sz)           ਸਮ੍ਰਿਤੀ_ਦਿਓ(sz)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_REALLOC(p,newsz)     realloc(p,newsz)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_FREE(p)              ਮੁਕਤ(p)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_REALLOC_SIZED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_REALLOC_SIZED(p,oldsz,newsz) STBI_REALLOC(p,newsz)
#ਜੇ_ਅੰਤ

// x86/x64 detection
#ਜੇ defined(__x86_64__) || defined(_M_X64)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_X64_ਨਿਸ਼ਾਨਾ
#ਜਾਂਜੇ defined(__i386) || defined(_M_IX86)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_X86_ਨਿਸ਼ਾਨਾ
#ਜੇ_ਅੰਤ

#ਜੇ defined(__GNUC__) && defined(STBI_X86_ਨਿਸ਼ਾਨਾ) && !defined(__SSE2__) && !defined(STBI_NO_SIMD)
// gcc doesn't support sse2 intrinsics unless you compile with -msse2,
// which in turn means it gets to use SSE2 everywhere. This is unfortunate,
// but previous attempts to provide the SSE2 functions with runtime
// detection caused numerous issues. The way architecture extensions are
// exposed in GCC/Clang is, sadly, not really suited for one-file libs.
// New behavior: if compiled with -msse2, we use SSE2 without any
// detection; if not, we don't use it at all.
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_SIMD
#ਜੇ_ਅੰਤ

#ਜੇ defined(__MINGW32__) && defined(STBI_X86_ਨਿਸ਼ਾਨਾ) && !defined(STBI_MINGW_ENABLE_SSE2) && !defined(STBI_NO_SIMD)
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_NO_SIMD
#ਜੇ_ਅੰਤ

#ਜੇ !defined(STBI_NO_SIMD) && (defined(STBI_X86_ਨਿਸ਼ਾਨਾ) || defined(STBI_X64_ਨਿਸ਼ਾਨਾ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_SSE2
#ਅਨ੍ਤਰ੍ਭਾਵ <emmintrin.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER

#ਜੇ _MSC_VER >= 1400  // not VC6
#ਅਨ੍ਤਰ੍ਭਾਵ <intrin.h> // __cpuid
ਸਥਿਰ ਪੂਰਨ stbi__cpuid3(ਖਾਲੀ)
{
   ਪੂਰਨ info[4];
   __cpuid(info,1);
   ਵਾਪਸ info[3];
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਪੂਰਨ stbi__cpuid3(ਖਾਲੀ)
{
   ਪੂਰਨ res;
   __asm {
      mov  eax,1
      cpuid
      mov  res,edx
   }
   ਵਾਪਸ res;
}
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_SIMD_ALIGN(type, name) __declspec(align(16)) type name

ਸਥਿਰ ਪੂਰਨ stbi_sse2_ਉਪਲਬਧ(ਖਾਲੀ)
{
   ਪੂਰਨ info3 = stbi__cpuid3();
   ਵਾਪਸ ((info3 >> 26) & 1) != 0;
}
#ਨਹੀਂਤਾਂ // assume GCC-style if not VC++
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))

ਸਥਿਰ ਪੂਰਨ stbi_sse2_ਉਪਲਬਧ(ਖਾਲੀ)
{
   // If we're even attempting to compile this on GCC/Clang, that means
   // -msse2 is on, which means the compiler is allowed to use SSE2
   // instructions at will, and so are we.
   ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

// ARM NEON
#ਜੇ defined(STBI_NO_SIMD) && defined(STBI_NEON)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ STBI_NEON
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_NEON
#ਅਨ੍ਤਰ੍ਭਾਵ <arm_neon.h>
// assume GCC or Clang on ARM targets
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_SIMD_ALIGN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_SIMD_ALIGN(type, name) type name
#ਜੇ_ਅੰਤ

///////////////////////////////////////////////
//
//  stbi__context struct and start_xxx functions

// stbi__context structure is our basic context used by all images, so it
// contains all the IO context, plus some basic image information
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi__uint32 img_x, img_y;
   ਪੂਰਨ img_n, img_ਬਾਹਰਲਾ_n;

   stbi_io_callbacks io;
   ਖਾਲੀ *io_user_ਅੰਕੜੇ;

   ਪੂਰਨ read_from_callbacks;
   ਪੂਰਨ buflen;
   stbi_uc ਬਫਰ_ਸ਼ੁਰੂ[128];

   stbi_uc *img_ਬਫਰ, *img_ਬਫਰ_ਸਿਰਾ;
   stbi_uc *img_ਬਫਰ_original, *img_ਬਫਰ_original_ਸਿਰਾ;
} stbi__context;


ਸਥਿਰ ਖਾਲੀ stbi_refill_ਬਫਰ(stbi__context *s);

// initialize a memory-decode context
ਸਥਿਰ ਖਾਲੀ stbi_ਸ਼ੁਰੂ_mem(stbi__context *s, stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len)
{
   s->io.ਪਠ = ਨਿਰਾ;
   s->read_from_callbacks = 0;
   s->img_ਬਫਰ = s->img_ਬਫਰ_original = (stbi_uc *) ਬਫਰ;
   s->img_ਬਫਰ_ਸਿਰਾ = s->img_ਬਫਰ_original_ਸਿਰਾ = (stbi_uc *) ਬਫਰ+len;
}

// initialize a callback-based context
ਸਥਿਰ ਖਾਲੀ stbi_ਸ਼ੁਰੂ_callbacks(stbi__context *s, stbi_io_callbacks *c, ਖਾਲੀ *user)
{
   s->io = *c;
   s->io_user_ਅੰਕੜੇ = user;
   s->buflen = ਆਕਾਰ(s->ਬਫਰ_ਸ਼ੁਰੂ);
   s->read_from_callbacks = 1;
   s->img_ਬਫਰ_original = s->ਬਫਰ_ਸ਼ੁਰੂ;
   stbi_refill_ਬਫਰ(s);
   s->img_ਬਫਰ_original_ਸਿਰਾ = s->img_ਬਫਰ_ਸਿਰਾ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO

ਸਥਿਰ ਪੂਰਨ stbi__stdio_read(ਖਾਲੀ *user, ਅੱਖਰ *ਅੰਕੜੇ, ਪੂਰਨ ਮਾਪ)
{
   ਵਾਪਸ (ਪੂਰਨ) fread(ਅੰਕੜੇ,1,ਮਾਪ,(FILE*) user);
}

ਸਥਿਰ ਖਾਲੀ stbi__stdio_skip(ਖਾਲੀ *user, ਪੂਰਨ n)
{
   fseek((FILE*) user, n, SEEK_CUR);
}

ਸਥਿਰ ਪੂਰਨ stbi__stdio_eof(ਖਾਲੀ *user)
{
   ਵਾਪਸ feof((FILE*) user);
}

ਸਥਿਰ stbi_io_callbacks stbi__stdio_callbacks =
{
   stbi__stdio_read,
   stbi__stdio_skip,
   stbi__stdio_eof,
};

ਸਥਿਰ ਖਾਲੀ stbi_ਸ਼ੁਰੂ_file(stbi__context *s, FILE *f)
{
   stbi_ਸ਼ੁਰੂ_callbacks(s, &stbi__stdio_callbacks, (ਖਾਲੀ *) f);
}

//static void stop_file(stbi__context *s) { }

#ਜੇ_ਅੰਤ // !STBI_NO_STDIO

ਸਥਿਰ ਖਾਲੀ stbi__rewind(stbi__context *s)
{
   // conceptually rewind SHOULD rewind to the beginning of the stream,
   // but we just rewind to the beginning of the initial buffer, because
   // we only use it after doing 'test', which only ever looks at at most 92 bytes
   s->img_ਬਫਰ = s->img_ਬਫਰ_original;
   s->img_ਬਫਰ_ਸਿਰਾ = s->img_ਬਫਰ_original_ਸਿਰਾ;
}

ਗਿਣਤੀ
{
   STBI_ਕ੍ਰਮ_RGB,
   STBI_ਕ੍ਰਮ_BGR
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   ਪੂਰਨ ਬਿੱਟ_ਪ੍ਰਤੀ_channel;
   ਪੂਰਨ ਅੰਕ_channels;
   ਪੂਰਨ channel_ਕ੍ਰਮ;
} stbi_ਨਤੀਜਾ_info;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_JPEG
ਸਥਿਰ ਪੂਰਨ      stbi__jpeg_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_jpeg_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਪੂਰਨ      stbi__jpeg_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNG
ਸਥਿਰ ਪੂਰਨ      stbi__png_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_png_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਪੂਰਨ      stbi__png_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
ਸਥਿਰ ਪੂਰਨ      stbi__png_is16(stbi__context *s);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_BMP
ਸਥਿਰ ਪੂਰਨ      stbi__bmp_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_bmp_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਪੂਰਨ      stbi__bmp_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_TGA
ਸਥਿਰ ਪੂਰਨ      stbi__tga_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_tga_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਪੂਰਨ      stbi__tga_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PSD
ਸਥਿਰ ਪੂਰਨ      stbi__psd_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_psd_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri, ਪੂਰਨ bpc);
ਸਥਿਰ ਪੂਰਨ      stbi__psd_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
ਸਥਿਰ ਪੂਰਨ      stbi__psd_is16(stbi__context *s);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
ਸਥਿਰ ਪੂਰਨ      stbi__hdr_test(stbi__context *s);
ਸਥਿਰ ਦਸ਼ਮਲਵ   *stbi_hdr_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਪੂਰਨ      stbi__hdr_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PIC
ਸਥਿਰ ਪੂਰਨ      stbi__pic_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_pic_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਪੂਰਨ      stbi__pic_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_GIF
ਸਥਿਰ ਪੂਰਨ      stbi__gif_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_gif_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਖਾਲੀ    *stbi_ਲੱਦੋ_gif_main(stbi__context *s, ਪੂਰਨ **delays, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *z, ਪੂਰਨ *comp, ਪੂਰਨ req_comp);
ਸਥਿਰ ਪੂਰਨ      stbi__gif_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNM
ਸਥਿਰ ਪੂਰਨ      stbi__pnm_test(stbi__context *s);
ਸਥਿਰ ਖਾਲੀ    *stbi_pnm_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri);
ਸਥਿਰ ਪੂਰਨ      stbi__pnm_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp);
#ਜੇ_ਅੰਤ

// this is not threadsafe
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *stbi__g_failure_reason;

STBIDEF ਅਚਲ ਅੱਖਰ *stbi_failure_reason(ਖਾਲੀ)
{
   ਵਾਪਸ stbi__g_failure_reason;
}

ਸਥਿਰ ਪੂਰਨ stbi__err(ਅਚਲ ਅੱਖਰ *str)
{
   stbi__g_failure_reason = str;
   ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ *stbi__malloc(size_t ਮਾਪ)
{
    ਵਾਪਸ STBI_MALLOC(ਮਾਪ);
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
ਸਥਿਰ ਪੂਰਨ stbi__addsizes_valid(ਪੂਰਨ a, ਪੂਰਨ b)
{
   ਜੇ (b < 0) ਵਾਪਸ 0;
   // now 0 <= b <= INT_MAX, hence also
   // 0 <= INT_MAX - b <= INTMAX.
   // And "a + b <= INT_MAX" (which might overflow) is the
   // same as a <= INT_MAX - b (no overflow)
   ਵਾਪਸ a <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ - b;
}

// returns 1 if the product is valid, 0 on overflow.
// negative factors are considered invalid.
ਸਥਿਰ ਪੂਰਨ stbi__mul2sizes_valid(ਪੂਰਨ a, ਪੂਰਨ b)
{
   ਜੇ (a < 0 || b < 0) ਵਾਪਸ 0;
   ਜੇ (b == 0) ਵਾਪਸ 1; // mul-by-0 is always safe
   // portable way to check for no overflows in a*b
   ਵਾਪਸ a <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ/b;
}

// returns 1 if "a*b + add" has no negative terms/factors and doesn't overflow
ਸਥਿਰ ਪੂਰਨ stbi__mad2sizes_valid(ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ ਜੋੜੋ)
{
   ਵਾਪਸ stbi__mul2sizes_valid(a, b) && stbi__addsizes_valid(a*b, ਜੋੜੋ);
}

// returns 1 if "a*b*c + add" has no negative terms/factors and doesn't overflow
ਸਥਿਰ ਪੂਰਨ stbi__mad3sizes_valid(ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ c, ਪੂਰਨ ਜੋੜੋ)
{
   ਵਾਪਸ stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__addsizes_valid(a*b*c, ਜੋੜੋ);
}

// returns 1 if "a*b*c*d + add" has no negative terms/factors and doesn't overflow
#ਜੇ !defined(STBI_NO_ਰੇਖਿਕ) || !defined(STBI_NO_HDR)
ਸਥਿਰ ਪੂਰਨ stbi__mad4sizes_valid(ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ c, ਪੂਰਨ d, ਪੂਰਨ ਜੋੜੋ)
{
   ਵਾਪਸ stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__mul2sizes_valid(a*b*c, d) && stbi__addsizes_valid(a*b*c*d, ਜੋੜੋ);
}
#ਜੇ_ਅੰਤ

// mallocs with size overflow checking
ਸਥਿਰ ਖਾਲੀ *stbi__malloc_mad2(ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ ਜੋੜੋ)
{
   ਜੇ (!stbi__mad2sizes_valid(a, b, ਜੋੜੋ)) ਵਾਪਸ ਨਿਰਾ;
   ਵਾਪਸ stbi__malloc(a*b + ਜੋੜੋ);
}

ਸਥਿਰ ਖਾਲੀ *stbi__malloc_mad3(ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ c, ਪੂਰਨ ਜੋੜੋ)
{
   ਜੇ (!stbi__mad3sizes_valid(a, b, c, ਜੋੜੋ)) ਵਾਪਸ ਨਿਰਾ;
   ਵਾਪਸ stbi__malloc(a*b*c + ਜੋੜੋ);
}

#ਜੇ !defined(STBI_NO_ਰੇਖਿਕ) || !defined(STBI_NO_HDR)
ਸਥਿਰ ਖਾਲੀ *stbi__malloc_mad4(ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ c, ਪੂਰਨ d, ਪੂਰਨ ਜੋੜੋ)
{
   ਜੇ (!stbi__mad4sizes_valid(a, b, c, d, ਜੋੜੋ)) ਵਾਪਸ ਨਿਰਾ;
   ਵਾਪਸ stbi__malloc(a*b*c*d + ਜੋੜੋ);
}
#ਜੇ_ਅੰਤ

// stbi__err - error
// stbi__errpf - error returning pointer to float
// stbi__errpuc - error returning pointer to unsigned char

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_NO_FAILURE_ਲੜੀਆਂ
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__err(x,y)  0
#ਜਾਂਜੇ defined(STBI_FAILURE_USERMSG)
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__err(x,y)  stbi__err(y)
#ਨਹੀਂਤਾਂ
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__err(x,y)  stbi__err(x)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__errpf(x,y)   ((ਦਸ਼ਮਲਵ *)(size_t) (stbi__err(x,y)?ਨਿਰਾ:ਨਿਰਾ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__errpuc(x,y)  ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)(size_t) (stbi__err(x,y)?ਨਿਰਾ:ਨਿਰਾ))

STBIDEF ਖਾਲੀ stbi_image_free(ਖਾਲੀ *retval_from_stbi_ਲੱਦੋ)
{
   STBI_FREE(retval_from_stbi_ਲੱਦੋ);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_ਰੇਖਿਕ
ਸਥਿਰ ਦਸ਼ਮਲਵ   *stbi_ldr_ਮੰਜ਼ਿਲ_hdr(stbi_uc *ਅੰਕੜੇ, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ comp);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
ਸਥਿਰ stbi_uc *stbi_hdr_ਮੰਜ਼ਿਲ_ldr(ਦਸ਼ਮਲਵ   *ਅੰਕੜੇ, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ comp);
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ stbi_vertically_flip_on_ਲੱਦੋ = 0;

STBIDEF ਖਾਲੀ stbi_ਸੈੱਟ_flip_vertically_on_ਲੱਦੋ(ਪੂਰਨ flag_true_if_should_flip)
{
    stbi_vertically_flip_on_ਲੱਦੋ = flag_true_if_should_flip;
}

ਸਥਿਰ ਖਾਲੀ *stbi_ਲੱਦੋ_main(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri, ਪੂਰਨ bpc)
{
   memset(ri, 0, ਆਕਾਰ(*ri)); // make sure it's initialized if we add new fields
   ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel = 8; // default is 8 so most paths don't have to be changed
   ri->channel_ਕ੍ਰਮ = STBI_ਕ੍ਰਮ_RGB; // all current input & output are this, but this is here so we can add BGR order
   ri->ਅੰਕ_channels = 0;

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_JPEG
   ਜੇ (stbi__jpeg_test(s)) ਵਾਪਸ stbi_jpeg_ਲੱਦੋ(s,x,y,comp,req_comp, ri);
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNG
   ਜੇ (stbi__png_test(s))  ਵਾਪਸ stbi_png_ਲੱਦੋ(s,x,y,comp,req_comp, ri);
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_BMP
   ਜੇ (stbi__bmp_test(s))  ਵਾਪਸ stbi_bmp_ਲੱਦੋ(s,x,y,comp,req_comp, ri);
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_GIF
   ਜੇ (stbi__gif_test(s))  ਵਾਪਸ stbi_gif_ਲੱਦੋ(s,x,y,comp,req_comp, ri);
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PSD
   ਜੇ (stbi__psd_test(s))  ਵਾਪਸ stbi_psd_ਲੱਦੋ(s,x,y,comp,req_comp, ri, bpc);
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PIC
   ਜੇ (stbi__pic_test(s))  ਵਾਪਸ stbi_pic_ਲੱਦੋ(s,x,y,comp,req_comp, ri);
   #ਜੇ_ਅੰਤ
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNM
   ਜੇ (stbi__pnm_test(s))  ਵਾਪਸ stbi_pnm_ਲੱਦੋ(s,x,y,comp,req_comp, ri);
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
   ਜੇ (stbi__hdr_test(s)) {
      ਦਸ਼ਮਲਵ *hdr = stbi_hdr_ਲੱਦੋ(s, x,y,comp,req_comp, ri);
      ਵਾਪਸ stbi_hdr_ਮੰਜ਼ਿਲ_ldr(hdr, *x, *y, req_comp ? req_comp : *comp);
   }
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_TGA
   // test tga last because it's a crappy test!
   ਜੇ (stbi__tga_test(s))
      ਵਾਪਸ stbi_tga_ਲੱਦੋ(s,x,y,comp,req_comp, ri);
   #ਜੇ_ਅੰਤ

   ਵਾਪਸ stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
}

ਸਥਿਰ stbi_uc *stbi_convert_16_ਮੰਜ਼ਿਲ_8(stbi__uint16 *orig, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ channels)
{
   ਪੂਰਨ i;
   ਪੂਰਨ img_len = w * h * channels;
   stbi_uc *reduced;

   reduced = (stbi_uc *) stbi__malloc(img_len);
   ਜੇ (reduced == ਨਿਰਾ) ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");

   ਲਈ (i = 0; i < img_len; ++i)
      reduced[i] = (stbi_uc)((orig[i] >> 8) & 0xFF); // top half of each byte is sufficient approx of 16->8 bit scaling

   STBI_FREE(orig);
   ਵਾਪਸ reduced;
}

ਸਥਿਰ stbi__uint16 *stbi_convert_8_ਮੰਜ਼ਿਲ_16(stbi_uc *orig, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ channels)
{
   ਪੂਰਨ i;
   ਪੂਰਨ img_len = w * h * channels;
   stbi__uint16 *enlarged;

   enlarged = (stbi__uint16 *) stbi__malloc(img_len*2);
   ਜੇ (enlarged == ਨਿਰਾ) ਵਾਪਸ (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");

   ਲਈ (i = 0; i < img_len; ++i)
      enlarged[i] = (stbi__uint16)((orig[i] << 8) + orig[i]); // replicate to high and low byte, maps 0->0, 255->0xffff

   STBI_FREE(orig);
   ਵਾਪਸ enlarged;
}

ਸਥਿਰ ਖਾਲੀ stbi__vertical_flip(ਖਾਲੀ *image, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel)
{
   ਪੂਰਨ ਪੰਕਤੀ;
   size_t ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀ = (size_t)w * ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel;
   stbi_uc ਆਰਜ਼ੀ[2048];
   stbi_uc *ਬਾਈਟਾਂ = (stbi_uc *)image;

   ਲਈ (ਪੰਕਤੀ = 0; ਪੰਕਤੀ < (h>>1); ਪੰਕਤੀ++) {
      stbi_uc *row0 = ਬਾਈਟਾਂ + ਪੰਕਤੀ*ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀ;
      stbi_uc *row1 = ਬਾਈਟਾਂ + (h - ਪੰਕਤੀ - 1)*ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀ;
      // swap row0 with row1
      size_t ਬਾਈਟਾਂ_ਖੱਬਾ = ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀ;
      ਜਦੋਂ (ਬਾਈਟਾਂ_ਖੱਬਾ) {
         size_t ਬਾਈਟਾਂ_copy = (ਬਾਈਟਾਂ_ਖੱਬਾ < ਆਕਾਰ(ਆਰਜ਼ੀ)) ? ਬਾਈਟਾਂ_ਖੱਬਾ : ਆਕਾਰ(ਆਰਜ਼ੀ);
         ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਆਰਜ਼ੀ, row0, ਬਾਈਟਾਂ_copy);
         ਸਮ੍ਰਿਤੀ_ਨਕਲ(row0, row1, ਬਾਈਟਾਂ_copy);
         ਸਮ੍ਰਿਤੀ_ਨਕਲ(row1, ਆਰਜ਼ੀ, ਬਾਈਟਾਂ_copy);
         row0 += ਬਾਈਟਾਂ_copy;
         row1 += ਬਾਈਟਾਂ_copy;
         ਬਾਈਟਾਂ_ਖੱਬਾ -= ਬਾਈਟਾਂ_copy;
      }
   }
}

ਸਥਿਰ ਖਾਲੀ stbi__vertical_flip_slices(ਖਾਲੀ *image, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ z, ਪੂਰਨ ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel)
{
   ਪੂਰਨ ਟੁਕੜਾ;
   ਪੂਰਨ ਟੁਕੜਾ_ਮਾਪ = w * h * ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel;

   stbi_uc *ਬਾਈਟਾਂ = (stbi_uc *)image;
   ਲਈ (ਟੁਕੜਾ = 0; ਟੁਕੜਾ < z; ++ਟੁਕੜਾ) {
      stbi__vertical_flip(ਬਾਈਟਾਂ, w, h, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel); 
      ਬਾਈਟਾਂ += ਟੁਕੜਾ_ਮਾਪ; 
   }
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *stbi_ਲੱਦੋ_and_postprocess_8bit(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi_ਨਤੀਜਾ_info ri;
   ਖਾਲੀ *ਨਤੀਜਾ = stbi_ਲੱਦੋ_main(s, x, y, comp, req_comp, &ri, 8);

   ਜੇ (ਨਤੀਜਾ == ਨਿਰਾ)
      ਵਾਪਸ ਨਿਰਾ;

   ਜੇ (ri.ਬਿੱਟ_ਪ੍ਰਤੀ_channel != 8) {
      STBI_ASSERT(ri.ਬਿੱਟ_ਪ੍ਰਤੀ_channel == 16);
      ਨਤੀਜਾ = stbi_convert_16_ਮੰਜ਼ਿਲ_8((stbi__uint16 *) ਨਤੀਜਾ, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.ਬਿੱਟ_ਪ੍ਰਤੀ_channel = 8;
   }

   // @TODO: move stbi__convert_format to here

   ਜੇ (stbi_vertically_flip_on_ਲੱਦੋ) {
      ਪੂਰਨ channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ਨਤੀਜਾ, *x, *y, channels * ਆਕਾਰ(stbi_uc));
   }

   ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਨਤੀਜਾ;
}

ਸਥਿਰ stbi__uint16 *stbi_ਲੱਦੋ_and_postprocess_16bit(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi_ਨਤੀਜਾ_info ri;
   ਖਾਲੀ *ਨਤੀਜਾ = stbi_ਲੱਦੋ_main(s, x, y, comp, req_comp, &ri, 16);

   ਜੇ (ਨਤੀਜਾ == ਨਿਰਾ)
      ਵਾਪਸ ਨਿਰਾ;

   ਜੇ (ri.ਬਿੱਟ_ਪ੍ਰਤੀ_channel != 16) {
      STBI_ASSERT(ri.ਬਿੱਟ_ਪ੍ਰਤੀ_channel == 8);
      ਨਤੀਜਾ = stbi_convert_8_ਮੰਜ਼ਿਲ_16((stbi_uc *) ਨਤੀਜਾ, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.ਬਿੱਟ_ਪ੍ਰਤੀ_channel = 16;
   }

   // @TODO: move stbi__convert_format16 to here
   // @TODO: special case RGB-to-Y (and RGBA-to-YA) for 8-bit-to-16-bit case to keep more precision

   ਜੇ (stbi_vertically_flip_on_ਲੱਦੋ) {
      ਪੂਰਨ channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ਨਤੀਜਾ, *x, *y, channels * ਆਕਾਰ(stbi__uint16));
   }

   ਵਾਪਸ (stbi__uint16 *) ਨਤੀਜਾ;
}

#ਜੇ !defined(STBI_NO_HDR) || !defined(STBI_NO_ਰੇਖਿਕ)
ਸਥਿਰ ਖਾਲੀ stbi__float_postprocess(ਦਸ਼ਮਲਵ *ਨਤੀਜਾ, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   ਜੇ (stbi_vertically_flip_on_ਲੱਦੋ && ਨਤੀਜਾ != ਨਿਰਾ) {
      ਪੂਰਨ channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ਨਤੀਜਾ, *x, *y, channels * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
   }
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO

ਸਥਿਰ FILE *stbi__fopen(ਅੱਖਰ ਅਚਲ *filename, ਅੱਖਰ ਅਚਲ *ਬਹੁਲਕ)
{
   FILE *f;
#ਜੇ defined(_MSC_VER) && _MSC_VER >= 1400
   ਜੇ (0 != fopen_s(&f, filename, ਬਹੁਲਕ))
      f=0;
#ਨਹੀਂਤਾਂ
   f = fopen(filename, ਬਹੁਲਕ);
#ਜੇ_ਅੰਤ
   ਵਾਪਸ f;
}


STBIDEF stbi_uc *stbi_ਲੱਦੋ(ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਨਤੀਜਾ;
   ਜੇ (!f) ਵਾਪਸ stbi__errpuc("can't fopen", "Unable to open file");
   ਨਤੀਜਾ = stbi_ਲੱਦੋ_from_file(f,x,y,comp,req_comp);
   fclose(f);
   ਵਾਪਸ ਨਤੀਜਾ;
}

STBIDEF stbi_uc *stbi_ਲੱਦੋ_from_file(FILE *f, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਨਤੀਜਾ;
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_file(&s,f);
   ਨਤੀਜਾ = stbi_ਲੱਦੋ_and_postprocess_8bit(&s,x,y,comp,req_comp);
   ਜੇ (ਨਤੀਜਾ) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (ਪੂਰਨ) (s.img_ਬਫਰ_ਸਿਰਾ - s.img_ਬਫਰ), SEEK_CUR);
   }
   ਵਾਪਸ ਨਤੀਜਾ;
}

STBIDEF stbi__uint16 *stbi_ਲੱਦੋ_from_file_16(FILE *f, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi__uint16 *ਨਤੀਜਾ;
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_file(&s,f);
   ਨਤੀਜਾ = stbi_ਲੱਦੋ_and_postprocess_16bit(&s,x,y,comp,req_comp);
   ਜੇ (ਨਤੀਜਾ) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (ਪੂਰਨ) (s.img_ਬਫਰ_ਸਿਰਾ - s.img_ਬਫਰ), SEEK_CUR);
   }
   ਵਾਪਸ ਨਤੀਜਾ;
}

STBIDEF stbi_us *stbi_ਲੱਦੋ_16(ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   stbi__uint16 *ਨਤੀਜਾ;
   ਜੇ (!f) ਵਾਪਸ (stbi_us *) stbi__errpuc("can't fopen", "Unable to open file");
   ਨਤੀਜਾ = stbi_ਲੱਦੋ_from_file_16(f,x,y,comp,req_comp);
   fclose(f);
   ਵਾਪਸ ਨਤੀਜਾ;
}


#ਜੇ_ਅੰਤ //!STBI_NO_STDIO

STBIDEF stbi_us *stbi_ਲੱਦੋ_16_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_mem(&s,ਬਫਰ,len);
   ਵਾਪਸ stbi_ਲੱਦੋ_and_postprocess_16bit(&s,x,y,channels_ਵਿੱਚ_file,desired_channels);
}

STBIDEF stbi_us *stbi_ਲੱਦੋ_16_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *channels_ਵਿੱਚ_file, ਪੂਰਨ desired_channels)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_callbacks(&s, (stbi_io_callbacks *)clbk, user);
   ਵਾਪਸ stbi_ਲੱਦੋ_and_postprocess_16bit(&s,x,y,channels_ਵਿੱਚ_file,desired_channels);
}

STBIDEF stbi_uc *stbi_ਲੱਦੋ_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_mem(&s,ਬਫਰ,len);
   ਵਾਪਸ stbi_ਲੱਦੋ_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

STBIDEF stbi_uc *stbi_ਲੱਦੋ_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   ਵਾਪਸ stbi_ਲੱਦੋ_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_GIF
STBIDEF stbi_uc *stbi_ਲੱਦੋ_gif_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ **delays, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *z, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਨਤੀਜਾ;
   stbi__context s; 
   stbi_ਸ਼ੁਰੂ_mem(&s,ਬਫਰ,len); 
   
   ਨਤੀਜਾ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*) stbi_ਲੱਦੋ_gif_main(&s, delays, x, y, z, comp, req_comp);
   ਜੇ (stbi_vertically_flip_on_ਲੱਦੋ) {
      stbi__vertical_flip_slices( ਨਤੀਜਾ, *x, *y, *z, *comp ); 
   }

   ਵਾਪਸ ਨਤੀਜਾ; 
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_ਰੇਖਿਕ
ਸਥਿਰ ਦਸ਼ਮਲਵ *stbi__loadf_main(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ;
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
   ਜੇ (stbi__hdr_test(s)) {
      stbi_ਨਤੀਜਾ_info ri;
      ਦਸ਼ਮਲਵ *hdr_ਅੰਕੜੇ = stbi_hdr_ਲੱਦੋ(s,x,y,comp,req_comp, &ri);
      ਜੇ (hdr_ਅੰਕੜੇ)
         stbi__float_postprocess(hdr_ਅੰਕੜੇ,x,y,comp,req_comp);
      ਵਾਪਸ hdr_ਅੰਕੜੇ;
   }
   #ਜੇ_ਅੰਤ
   ਅੰਕੜੇ = stbi_ਲੱਦੋ_and_postprocess_8bit(s, x, y, comp, req_comp);
   ਜੇ (ਅੰਕੜੇ)
      ਵਾਪਸ stbi_ldr_ਮੰਜ਼ਿਲ_hdr(ਅੰਕੜੇ, *x, *y, req_comp ? req_comp : *comp);
   ਵਾਪਸ stbi__errpf("unknown image type", "Image not of any known type, or corrupt");
}

STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_mem(&s,ਬਫਰ,len);
   ਵਾਪਸ stbi__loadf_main(&s,x,y,comp,req_comp);
}

STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   ਵਾਪਸ stbi__loadf_main(&s,x,y,comp,req_comp);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf(ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   ਦਸ਼ਮਲਵ *ਨਤੀਜਾ;
   FILE *f = stbi__fopen(filename, "rb");
   ਜੇ (!f) ਵਾਪਸ stbi__errpf("can't fopen", "Unable to open file");
   ਨਤੀਜਾ = stbi_loadf_from_file(f,x,y,comp,req_comp);
   fclose(f);
   ਵਾਪਸ ਨਤੀਜਾ;
}

STBIDEF ਦਸ਼ਮਲਵ *stbi_loadf_from_file(FILE *f, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_file(&s,f);
   ਵਾਪਸ stbi__loadf_main(&s,x,y,comp,req_comp);
}
#ਜੇ_ਅੰਤ // !STBI_NO_STDIO

#ਜੇ_ਅੰਤ // !STBI_NO_LINEAR

// these is-hdr-or-not is defined independent of whether STBI_NO_LINEAR is
// defined, for API simplicity; if STBI_NO_LINEAR is defined, it always
// reports false!

STBIDEF ਪੂਰਨ stbi_is_hdr_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len)
{
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_mem(&s,ਬਫਰ,len);
   ਵਾਪਸ stbi__hdr_test(&s);
   #ਨਹੀਂਤਾਂ
   STBI_NOTUSED(ਬਫਰ);
   STBI_NOTUSED(len);
   ਵਾਪਸ 0;
   #ਜੇ_ਅੰਤ
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
STBIDEF ਪੂਰਨ      stbi_is_hdr          (ਅੱਖਰ ਅਚਲ *filename)
{
   FILE *f = stbi__fopen(filename, "rb");
   ਪੂਰਨ ਨਤੀਜਾ=0;
   ਜੇ (f) {
      ਨਤੀਜਾ = stbi_is_hdr_from_file(f);
      fclose(f);
   }
   ਵਾਪਸ ਨਤੀਜਾ;
}

STBIDEF ਪੂਰਨ stbi_is_hdr_from_file(FILE *f)
{
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
   ਲੰਮਾ pos = ftell(f);
   ਪੂਰਨ res;
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_file(&s,f);
   res = stbi__hdr_test(&s);
   fseek(f, pos, SEEK_SET);
   ਵਾਪਸ res;
   #ਨਹੀਂਤਾਂ
   STBI_NOTUSED(f);
   ਵਾਪਸ 0;
   #ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ // !STBI_NO_STDIO

STBIDEF ਪੂਰਨ      stbi_is_hdr_from_callbacks(stbi_io_callbacks ਅਚਲ *clbk, ਖਾਲੀ *user)
{
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   ਵਾਪਸ stbi__hdr_test(&s);
   #ਨਹੀਂਤਾਂ
   STBI_NOTUSED(clbk);
   STBI_NOTUSED(user);
   ਵਾਪਸ 0;
   #ਜੇ_ਅੰਤ
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_ਰੇਖਿਕ
ਸਥਿਰ ਦਸ਼ਮਲਵ stbi__l2h_gamma=2.2f, stbi__l2h_scale=1.0f;

STBIDEF ਖਾਲੀ   stbi_ldr_ਮੰਜ਼ਿਲ_hdr_gamma(ਦਸ਼ਮਲਵ gamma) { stbi__l2h_gamma = gamma; }
STBIDEF ਖਾਲੀ   stbi_ldr_ਮੰਜ਼ਿਲ_hdr_scale(ਦਸ਼ਮਲਵ scale) { stbi__l2h_scale = scale; }
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਦਸ਼ਮਲਵ stbi__h2l_gamma_i=1.0f/2.2f, stbi__h2l_scale_i=1.0f;

STBIDEF ਖਾਲੀ   stbi_hdr_ਮੰਜ਼ਿਲ_ldr_gamma(ਦਸ਼ਮਲਵ gamma) { stbi__h2l_gamma_i = 1/gamma; }
STBIDEF ਖਾਲੀ   stbi_hdr_ਮੰਜ਼ਿਲ_ldr_scale(ਦਸ਼ਮਲਵ scale) { stbi__h2l_scale_i = 1/scale; }


//////////////////////////////////////////////////////////////////////////////
//
// Common code used by all image loaders
//

ਗਿਣਤੀ
{
   STBI_ਜਾਂਚ_ਲੱਦੋ=0,
   STBI_ਜਾਂਚ_type,
   STBI_ਜਾਂਚ_ਸਿਰਲੇਖ
};

ਸਥਿਰ ਖਾਲੀ stbi_refill_ਬਫਰ(stbi__context *s)
{
   ਪੂਰਨ n = (s->io.ਪਠ)(s->io_user_ਅੰਕੜੇ,(ਅੱਖਰ*)s->ਬਫਰ_ਸ਼ੁਰੂ,s->buflen);
   ਜੇ (n == 0) {
      // at end of file, treat same as if from memory, but need to handle case
      // where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file
      s->read_from_callbacks = 0;
      s->img_ਬਫਰ = s->ਬਫਰ_ਸ਼ੁਰੂ;
      s->img_ਬਫਰ_ਸਿਰਾ = s->ਬਫਰ_ਸ਼ੁਰੂ+1;
      *s->img_ਬਫਰ = 0;
   } ਨਹੀਂਤਾਂ {
      s->img_ਬਫਰ = s->ਬਫਰ_ਸ਼ੁਰੂ;
      s->img_ਬਫਰ_ਸਿਰਾ = s->ਬਫਰ_ਸ਼ੁਰੂ + n;
   }
}

stbi_inline ਸਥਿਰ stbi_uc stbi__get8(stbi__context *s)
{
   ਜੇ (s->img_ਬਫਰ < s->img_ਬਫਰ_ਸਿਰਾ)
      ਵਾਪਸ *s->img_ਬਫਰ++;
   ਜੇ (s->read_from_callbacks) {
      stbi_refill_ਬਫਰ(s);
      ਵਾਪਸ *s->img_ਬਫਰ++;
   }
   ਵਾਪਸ 0;
}

stbi_inline ਸਥਿਰ ਪੂਰਨ stbi__at_eof(stbi__context *s)
{
   ਜੇ (s->io.ਪਠ) {
      ਜੇ (!(s->io.eof)(s->io_user_ਅੰਕੜੇ)) ਵਾਪਸ 0;
      // if feof() is true, check if buffer = end
      // special case: we've only got the special 0 character at the end
      ਜੇ (s->read_from_callbacks == 0) ਵਾਪਸ 1;
   }

   ਵਾਪਸ s->img_ਬਫਰ >= s->img_ਬਫਰ_ਸਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ stbi__skip(stbi__context *s, ਪੂਰਨ n)
{
   ਜੇ (n < 0) {
      s->img_ਬਫਰ = s->img_ਬਫਰ_ਸਿਰਾ;
      ਵਾਪਸ;
   }
   ਜੇ (s->io.ਪਠ) {
      ਪੂਰਨ blen = (ਪੂਰਨ) (s->img_ਬਫਰ_ਸਿਰਾ - s->img_ਬਫਰ);
      ਜੇ (blen < n) {
         s->img_ਬਫਰ = s->img_ਬਫਰ_ਸਿਰਾ;
         (s->io.skip)(s->io_user_ਅੰਕੜੇ, n - blen);
         ਵਾਪਸ;
      }
   }
   s->img_ਬਫਰ += n;
}

ਸਥਿਰ ਪੂਰਨ stbi__getn(stbi__context *s, stbi_uc *ਬਫਰ, ਪੂਰਨ n)
{
   ਜੇ (s->io.ਪਠ) {
      ਪੂਰਨ blen = (ਪੂਰਨ) (s->img_ਬਫਰ_ਸਿਰਾ - s->img_ਬਫਰ);
      ਜੇ (blen < n) {
         ਪੂਰਨ res, ਗਿਣਤੀ_2;

         ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਬਫਰ, s->img_ਬਫਰ, blen);

         ਗਿਣਤੀ_2 = (s->io.ਪਠ)(s->io_user_ਅੰਕੜੇ, (ਅੱਖਰ*) ਬਫਰ + blen, n - blen);
         res = (ਗਿਣਤੀ_2 == (n-blen));
         s->img_ਬਫਰ = s->img_ਬਫਰ_ਸਿਰਾ;
         ਵਾਪਸ res;
      }
   }

   ਜੇ (s->img_ਬਫਰ+n <= s->img_ਬਫਰ_ਸਿਰਾ) {
      ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਬਫਰ, s->img_ਬਫਰ, n);
      s->img_ਬਫਰ += n;
      ਵਾਪਸ 1;
   } ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ stbi__get16be(stbi__context *s)
{
   ਪੂਰਨ z = stbi__get8(s);
   ਵਾਪਸ (z << 8) + stbi__get8(s);
}

ਸਥਿਰ stbi__uint32 stbi__get32be(stbi__context *s)
{
   stbi__uint32 z = stbi__get16be(s);
   ਵਾਪਸ (z << 16) + stbi__get16be(s);
}

#ਜੇ defined(STBI_NO_BMP) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF)
// nothing
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਪੂਰਨ stbi__get16le(stbi__context *s)
{
   ਪੂਰਨ z = stbi__get8(s);
   ਵਾਪਸ z + (stbi__get8(s) << 8);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_BMP
ਸਥਿਰ stbi__uint32 stbi__get32le(stbi__context *s)
{
   stbi__uint32 z = stbi__get16le(s);
   ਵਾਪਸ z + (stbi__get16le(s) << 16);
}
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__BYTECAST(x)  ((stbi_uc) ((x) & 255))  // truncate int to byte without warnings


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

ਸਥਿਰ stbi_uc stbi__compute_y(ਪੂਰਨ r, ਪੂਰਨ g, ਪੂਰਨ b)
{
   ਵਾਪਸ (stbi_uc) (((r*77) + (g*150) +  (29*b)) >> 8);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *stbi__convert_format(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ, ਪੂਰਨ img_n, ਪੂਰਨ req_comp, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ y)
{
   ਪੂਰਨ i,j;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *good;

   ਜੇ (req_comp == img_n) ਵਾਪਸ ਅੰਕੜੇ;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) stbi__malloc_mad3(req_comp, x, y, 0);
   ਜੇ (good == ਨਿਰਾ) {
      STBI_FREE(ਅੰਕੜੇ);
      ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");
   }

   ਲਈ (j=0; j < (ਪੂਰਨ) y; ++j) {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *src  = ਅੰਕੜੇ + j * x * img_n   ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *dest = good + j * x * req_comp;

      #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__COMBO(a,b)  ((a)*8+(b))
      #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__CASE(a,b)   ਹਾਲਤ STBI__COMBO(a,b): ਲਈ(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      ਚੋਣ (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=255;                                     } ਰੋਕੋ;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } ਰੋਕੋ;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=255;                     } ਰੋਕੋ;
         STBI__CASE(2,1) { dest[0]=src[0];                                                  } ਰੋਕੋ;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } ਰੋਕੋ;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                  } ਰੋਕੋ;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255;        } ਰੋਕੋ;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } ਰੋਕੋ;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = 255;    } ਰੋਕੋ;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } ਰੋਕੋ;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = src[3]; } ਰੋਕੋ;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                    } ਰੋਕੋ;
         ਮੂਲ: STBI_ASSERT(0);
      }
      #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ STBI__CASE
   }

   STBI_FREE(ਅੰਕੜੇ);
   ਵਾਪਸ good;
}

ਸਥਿਰ stbi__uint16 stbi__compute_y_16(ਪੂਰਨ r, ਪੂਰਨ g, ਪੂਰਨ b)
{
   ਵਾਪਸ (stbi__uint16) (((r*77) + (g*150) +  (29*b)) >> 8);
}

ਸਥਿਰ stbi__uint16 *stbi__convert_format16(stbi__uint16 *ਅੰਕੜੇ, ਪੂਰਨ img_n, ਪੂਰਨ req_comp, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ y)
{
   ਪੂਰਨ i,j;
   stbi__uint16 *good;

   ਜੇ (req_comp == img_n) ਵਾਪਸ ਅੰਕੜੇ;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (stbi__uint16 *) stbi__malloc(req_comp * x * y * 2);
   ਜੇ (good == ਨਿਰਾ) {
      STBI_FREE(ਅੰਕੜੇ);
      ਵਾਪਸ (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");
   }

   ਲਈ (j=0; j < (ਪੂਰਨ) y; ++j) {
      stbi__uint16 *src  = ਅੰਕੜੇ + j * x * img_n   ;
      stbi__uint16 *dest = good + j * x * req_comp;

      #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__COMBO(a,b)  ((a)*8+(b))
      #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__CASE(a,b)   ਹਾਲਤ STBI__COMBO(a,b): ਲਈ(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      ਚੋਣ (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=0xffff;                                     } ਰੋਕੋ;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } ਰੋਕੋ;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=0xffff;                     } ਰੋਕੋ;
         STBI__CASE(2,1) { dest[0]=src[0];                                                     } ਰੋਕੋ;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } ਰੋਕੋ;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                     } ਰੋਕੋ;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=0xffff;        } ਰੋਕੋ;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } ਰੋਕੋ;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = 0xffff; } ਰੋਕੋ;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } ਰੋਕੋ;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = src[3]; } ਰੋਕੋ;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                       } ਰੋਕੋ;
         ਮੂਲ: STBI_ASSERT(0);
      }
      #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ STBI__CASE
   }

   STBI_FREE(ਅੰਕੜੇ);
   ਵਾਪਸ good;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_ਰੇਖਿਕ
ਸਥਿਰ ਦਸ਼ਮਲਵ   *stbi_ldr_ਮੰਜ਼ਿਲ_hdr(stbi_uc *ਅੰਕੜੇ, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ comp)
{
   ਪੂਰਨ i,k,n;
   ਦਸ਼ਮਲਵ *output;
   ਜੇ (!ਅੰਕੜੇ) ਵਾਪਸ ਨਿਰਾ;
   output = (ਦਸ਼ਮਲਵ *) stbi__malloc_mad4(x, y, comp, ਆਕਾਰ(ਦਸ਼ਮਲਵ), 0);
   ਜੇ (output == ਨਿਰਾ) { STBI_FREE(ਅੰਕੜੇ); ਵਾਪਸ stbi__errpf("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   ਜੇ (comp & 1) n = comp; ਨਹੀਂਤਾਂ n = comp-1;
   ਲਈ (i=0; i < x*y; ++i) {
      ਲਈ (k=0; k < n; ++k) {
         output[i*comp + k] = (ਦਸ਼ਮਲਵ) (ਘਾਤ(ਅੰਕੜੇ[i*comp+k]/255.0f, stbi__l2h_gamma) * stbi__l2h_scale);
      }
      ਜੇ (k < comp) output[i*comp + k] = ਅੰਕੜੇ[i*comp+k]/255.0f;
   }
   STBI_FREE(ਅੰਕੜੇ);
   ਵਾਪਸ output;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__float2int(x)   ((ਪੂਰਨ) (x))
ਸਥਿਰ stbi_uc *stbi_hdr_ਮੰਜ਼ਿਲ_ldr(ਦਸ਼ਮਲਵ   *ਅੰਕੜੇ, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ comp)
{
   ਪੂਰਨ i,k,n;
   stbi_uc *output;
   ਜੇ (!ਅੰਕੜੇ) ਵਾਪਸ ਨਿਰਾ;
   output = (stbi_uc *) stbi__malloc_mad3(x, y, comp, 0);
   ਜੇ (output == ਨਿਰਾ) { STBI_FREE(ਅੰਕੜੇ); ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   ਜੇ (comp & 1) n = comp; ਨਹੀਂਤਾਂ n = comp-1;
   ਲਈ (i=0; i < x*y; ++i) {
      ਲਈ (k=0; k < n; ++k) {
         ਦਸ਼ਮਲਵ z = (ਦਸ਼ਮਲਵ) ਘਾਤ(ਅੰਕੜੇ[i*comp+k]*stbi__h2l_scale_i, stbi__h2l_gamma_i) * 255 + 0.5f;
         ਜੇ (z < 0) z = 0;
         ਜੇ (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
      ਜੇ (k < comp) {
         ਦਸ਼ਮਲਵ z = ਅੰਕੜੇ[i*comp+k] * 255 + 0.5f;
         ਜੇ (z < 0) z = 0;
         ਜੇ (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
   }
   STBI_FREE(ਅੰਕੜੇ);
   ਵਾਪਸ output;
}
#ਜੇ_ਅੰਤ

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_JPEG

// huffman decoding acceleration
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAST_ਬਿੱਟ   9  // larger handles more cases; smaller stomps less cache

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi_uc  fast[1 << FAST_ਬਿੱਟ];
   // weirdly, repacking this into AoS is a 10% speed loss, instead of a win
   stbi__uint16 ਕੋਡ[256];
   stbi_uc  ਮੁੱਲਾਂ[256];
   stbi_uc  ਮਾਪ[257];
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ maxcode[18];
   ਪੂਰਨ    delta[17];   // old 'firstsymbol' - old 'firstcode'
} stbi__huffman;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi__context *s;
   stbi__huffman huff_dc[4];
   stbi__huffman huff_ac[4];
   stbi__uint16 dequant[4][64];
   stbi__int16 fast_ac[4][1 << FAST_ਬਿੱਟ];

// sizes for components, interleaved MCUs
   ਪੂਰਨ img_h_max, img_v_max;
   ਪੂਰਨ img_mcu_x, img_mcu_y;
   ਪੂਰਨ img_mcu_w, img_mcu_h;

// definition of jpeg image component
   ਬਣਤਰ
   {
      ਪੂਰਨ id;
      ਪੂਰਨ h,v;
      ਪੂਰਨ tq;
      ਪੂਰਨ hd,ha;
      ਪੂਰਨ dc_pred;

      ਪੂਰਨ x,y,w2,h2;
      stbi_uc *ਅੰਕੜੇ;
      ਖਾਲੀ *raw_ਅੰਕੜੇ, *raw_coeff;
      stbi_uc *linebuf;
      ਛੋਟਾ   *coeff;   // progressive only
      ਪੂਰਨ      coeff_w, coeff_h; // number of 8x8 coefficient blocks
   } img_comp[4];

   stbi__uint32   ਕੋਡ_ਬਫਰ; // jpeg entropy-coded buffer
   ਪੂਰਨ            ਕੋਡ_ਬਿੱਟ;   // number of valid bits
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  marker;      // marker seen while filling entropy buffer
   ਪੂਰਨ            nomore;      // flag if we saw a marker so must stop

   ਪੂਰਨ            progressive;
   ਪੂਰਨ            spec_ਸ਼ੁਰੂ;
   ਪੂਰਨ            spec_ਸਿਰਾ;
   ਪੂਰਨ            succ_ਉੱਚਾ;
   ਪੂਰਨ            succ_ਨੀਵਾਂ;
   ਪੂਰਨ            eob_ਲੜੀਵਾਰ;
   ਪੂਰਨ            jfif;
   ਪੂਰਨ            app14_color_transform; // Adobe APP14 tag
   ਪੂਰਨ            rgb;

   ਪੂਰਨ ਜਾਂਚ_n, ਕ੍ਰਮ[4];
   ਪੂਰਨ restart_interval, todo;

// kernels
   ਖਾਲੀ (*idct_ਖੰਡ_kernel)(stbi_uc *ਬਾਹਰਲਾ, ਪੂਰਨ ਬਾਹਰਲਾ_stride, ਛੋਟਾ ਅੰਕੜੇ[64]);
   ਖਾਲੀ (*YCb_Cr_ਮੰਜ਼ਿਲ_RGB_kernel)(stbi_uc *ਬਾਹਰਲਾ, ਅਚਲ stbi_uc *y, ਅਚਲ stbi_uc *pcb, ਅਚਲ stbi_uc *pcr, ਪੂਰਨ ਗਿਣਤੀ_2, ਪੂਰਨ ਕਦਮ);
   stbi_uc *(*resample_ਪੰਕਤੀ_hv_2_kernel)(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *ਵਿੱਚ_near, stbi_uc *ਵਿੱਚ_far, ਪੂਰਨ w, ਪੂਰਨ hs);
} stbi__jpeg;

ਸਥਿਰ ਪੂਰਨ stbi__build_huffman(stbi__huffman *h, ਪੂਰਨ *ਗਿਣਤੀ_2)
{
   ਪੂਰਨ i,j,k=0;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੋਡ;
   // build size list for each symbol (from JPEG spec)
   ਲਈ (i=0; i < 16; ++i)
      ਲਈ (j=0; j < ਗਿਣਤੀ_2[i]; ++j)
         h->ਮਾਪ[k++] = (stbi_uc) (i+1);
   h->ਮਾਪ[k] = 0;

   // compute actual symbols (from jpeg spec)
   ਕੋਡ = 0;
   k = 0;
   ਲਈ(j=1; j <= 16; ++j) {
      // compute delta to add to code to compute symbol id
      h->delta[j] = k - ਕੋਡ;
      ਜੇ (h->ਮਾਪ[k] == j) {
         ਜਦੋਂ (h->ਮਾਪ[k] == j)
            h->ਕੋਡ[k++] = (stbi__uint16) (ਕੋਡ++);
         ਜੇ (ਕੋਡ-1 >= (1u << j)) ਵਾਪਸ stbi__err("bad code lengths","Corrupt JPEG");
      }
      // compute largest code + 1 for this size, preshifted as needed later
      h->maxcode[j] = ਕੋਡ << (16-j);
      ਕੋਡ <<= 1;
   }
   h->maxcode[j] = 0xffffffff;

   // build non-spec acceleration table; 255 is flag for not-accelerated
   memset(h->fast, 255, 1 << FAST_ਬਿੱਟ);
   ਲਈ (i=0; i < k; ++i) {
      ਪੂਰਨ s = h->ਮਾਪ[i];
      ਜੇ (s <= FAST_ਬਿੱਟ) {
         ਪੂਰਨ c = h->ਕੋਡ[i] << (FAST_ਬਿੱਟ-s);
         ਪੂਰਨ m = 1 << (FAST_ਬਿੱਟ-s);
         ਲਈ (j=0; j < m; ++j) {
            h->fast[c+j] = (stbi_uc) i;
         }
      }
   }
   ਵਾਪਸ 1;
}

// build a table that decodes both magnitude and value of small ACs in
// one go.
ਸਥਿਰ ਖਾਲੀ stbi__build_fast_ac(stbi__int16 *fast_ac, stbi__huffman *h)
{
   ਪੂਰਨ i;
   ਲਈ (i=0; i < (1 << FAST_ਬਿੱਟ); ++i) {
      stbi_uc fast = h->fast[i];
      fast_ac[i] = 0;
      ਜੇ (fast < 255) {
         ਪੂਰਨ rs = h->ਮੁੱਲਾਂ[fast];
         ਪੂਰਨ ਲੜੀਵਾਰ = (rs >> 4) & 15;
         ਪੂਰਨ magbits = rs & 15;
         ਪੂਰਨ len = h->ਮਾਪ[fast];

         ਜੇ (magbits && len + magbits <= FAST_ਬਿੱਟ) {
            // magnitude code followed by receive_extend code
            ਪੂਰਨ k = ((i << len) & ((1 << FAST_ਬਿੱਟ) - 1)) >> (FAST_ਬਿੱਟ - magbits);
            ਪੂਰਨ m = 1 << (magbits - 1);
            ਜੇ (k < m) k += (~0U << magbits) + 1;
            // if the result is small enough, we can fit it in fast_ac table
            ਜੇ (k >= -128 && k <= 127)
               fast_ac[i] = (stbi__int16) ((k * 256) + (ਲੜੀਵਾਰ * 16) + (len + magbits));
         }
      }
   }
}

ਸਥਿਰ ਖਾਲੀ stbi_grow_ਬਫਰ_unsafe(stbi__jpeg *j)
{
   ਕਰੋ {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b = j->nomore ? 0 : stbi__get8(j->s);
      ਜੇ (b == 0xff) {
         ਪੂਰਨ c = stbi__get8(j->s);
         ਜਦੋਂ (c == 0xff) c = stbi__get8(j->s); // consume fill bytes
         ਜੇ (c != 0) {
            j->marker = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) c;
            j->nomore = 1;
            ਵਾਪਸ;
         }
      }
      j->ਕੋਡ_ਬਫਰ |= b << (24 - j->ਕੋਡ_ਬਿੱਟ);
      j->ਕੋਡ_ਬਿੱਟ += 8;
   } ਜਦੋਂ (j->ਕੋਡ_ਬਿੱਟ <= 24);
}

// (1 << n) - 1
ਸਥਿਰ ਅਚਲ stbi__uint32 stbi__bmask[17]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};

// decode a jpeg huffman value from the bitstream
stbi_inline ਸਥਿਰ ਪੂਰਨ stbi__jpeg_huff_decode(stbi__jpeg *j, stbi__huffman *h)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਰਜ਼ੀ;
   ਪੂਰਨ c,k;

   ਜੇ (j->ਕੋਡ_ਬਿੱਟ < 16) stbi_grow_ਬਫਰ_unsafe(j);

   // look at the top FAST_BITS and determine what symbol ID it is,
   // if the code is <= FAST_BITS
   c = (j->ਕੋਡ_ਬਫਰ >> (32 - FAST_ਬਿੱਟ)) & ((1 << FAST_ਬਿੱਟ)-1);
   k = h->fast[c];
   ਜੇ (k < 255) {
      ਪੂਰਨ s = h->ਮਾਪ[k];
      ਜੇ (s > j->ਕੋਡ_ਬਿੱਟ)
         ਵਾਪਸ -1;
      j->ਕੋਡ_ਬਫਰ <<= s;
      j->ਕੋਡ_ਬਿੱਟ -= s;
      ਵਾਪਸ h->ਮੁੱਲਾਂ[k];
   }

   // naive test is to shift the code_buffer down so k bits are
   // valid, then test against maxcode. To speed this up, we've
   // preshifted maxcode left so that it has (16-k) 0s at the
   // end; in other words, regardless of the number of bits, it
   // wants to be compared against something shifted to have 16;
   // that way we don't need to shift inside the loop.
   ਆਰਜ਼ੀ = j->ਕੋਡ_ਬਫਰ >> 16;
   ਲਈ (k=FAST_ਬਿੱਟ+1 ; ; ++k)
      ਜੇ (ਆਰਜ਼ੀ < h->maxcode[k])
         ਰੋਕੋ;
   ਜੇ (k == 17) {
      // error! code not found
      j->ਕੋਡ_ਬਿੱਟ -= 16;
      ਵਾਪਸ -1;
   }

   ਜੇ (k > j->ਕੋਡ_ਬਿੱਟ)
      ਵਾਪਸ -1;

   // convert the huffman code to the symbol id
   c = ((j->ਕੋਡ_ਬਫਰ >> (32 - k)) & stbi__bmask[k]) + h->delta[k];
   STBI_ASSERT((((j->ਕੋਡ_ਬਫਰ) >> (32 - h->ਮਾਪ[c])) & stbi__bmask[h->ਮਾਪ[c]]) == h->ਕੋਡ[c]);

   // convert the id to a symbol
   j->ਕੋਡ_ਬਿੱਟ -= k;
   j->ਕੋਡ_ਬਫਰ <<= k;
   ਵਾਪਸ h->ਮੁੱਲਾਂ[c];
}

// bias[n] = (-1<<n) + 1
ਸਥਿਰ ਅਚਲ ਪੂਰਨ stbi__jbias[16] = {0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767};

// combined JPEG 'receive' and JPEG 'extend', since baseline
// always extends everything it receives.
stbi_inline ਸਥਿਰ ਪੂਰਨ stbi__extend_receive(stbi__jpeg *j, ਪੂਰਨ n)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k;
   ਪੂਰਨ sgn;
   ਜੇ (j->ਕੋਡ_ਬਿੱਟ < n) stbi_grow_ਬਫਰ_unsafe(j);

   sgn = (stbi__int32)j->ਕੋਡ_ਬਫਰ >> 31; // sign bit is always in MSB
   k = stbi_lrot(j->ਕੋਡ_ਬਫਰ, n);
   STBI_ASSERT(n >= 0 && n < (ਪੂਰਨ) (ਆਕਾਰ(stbi__bmask)/ਆਕਾਰ(*stbi__bmask)));
   j->ਕੋਡ_ਬਫਰ = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->ਕੋਡ_ਬਿੱਟ -= n;
   ਵਾਪਸ k + (stbi__jbias[n] & ~sgn);
}

// get some unsigned bits
stbi_inline ਸਥਿਰ ਪੂਰਨ stbi_jpeg_get_ਬਿੱਟ(stbi__jpeg *j, ਪੂਰਨ n)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k;
   ਜੇ (j->ਕੋਡ_ਬਿੱਟ < n) stbi_grow_ਬਫਰ_unsafe(j);
   k = stbi_lrot(j->ਕੋਡ_ਬਫਰ, n);
   j->ਕੋਡ_ਬਫਰ = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->ਕੋਡ_ਬਿੱਟ -= n;
   ਵਾਪਸ k;
}

stbi_inline ਸਥਿਰ ਪੂਰਨ stbi_jpeg_get_ਦੋਆਧਾਰੀ_ਅੰਕ(stbi__jpeg *j)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k;
   ਜੇ (j->ਕੋਡ_ਬਿੱਟ < 1) stbi_grow_ਬਫਰ_unsafe(j);
   k = j->ਕੋਡ_ਬਫਰ;
   j->ਕੋਡ_ਬਫਰ <<= 1;
   --j->ਕੋਡ_ਬਿੱਟ;
   ਵਾਪਸ k & 0x80000000;
}

// given a value that's at position X in the zigzag stream,
// where does it appear in the 8x8 matrix coded as row-major?
ਸਥਿਰ ਅਚਲ stbi_uc stbi__jpeg_dezigzag[64+15] =
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
ਸਥਿਰ ਪੂਰਨ stbi_jpeg_decode_ਖੰਡ(stbi__jpeg *j, ਛੋਟਾ ਅੰਕੜੇ[64], stbi__huffman *hdc, stbi__huffman *hac, stbi__int16 *fac, ਪੂਰਨ b, stbi__uint16 *dequant)
{
   ਪੂਰਨ diff,dc,k;
   ਪੂਰਨ t;

   ਜੇ (j->ਕੋਡ_ਬਿੱਟ < 16) stbi_grow_ਬਫਰ_unsafe(j);
   t = stbi__jpeg_huff_decode(j, hdc);
   ਜੇ (t < 0) ਵਾਪਸ stbi__err("bad huffman code","Corrupt JPEG");

   // 0 all the ac values now so we can do it 32-bits at a time
   memset(ਅੰਕੜੇ,0,64*ਆਕਾਰ(ਅੰਕੜੇ[0]));

   diff = t ? stbi__extend_receive(j, t) : 0;
   dc = j->img_comp[b].dc_pred + diff;
   j->img_comp[b].dc_pred = dc;
   ਅੰਕੜੇ[0] = (ਛੋਟਾ) (dc * dequant[0]);

   // decode AC components, see JPEG spec
   k = 1;
   ਕਰੋ {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ zig;
      ਪੂਰਨ c,r,s;
      ਜੇ (j->ਕੋਡ_ਬਿੱਟ < 16) stbi_grow_ਬਫਰ_unsafe(j);
      c = (j->ਕੋਡ_ਬਫਰ >> (32 - FAST_ਬਿੱਟ)) & ((1 << FAST_ਬਿੱਟ)-1);
      r = fac[c];
      ਜੇ (r) { // fast-AC path
         k += (r >> 4) & 15; //ਲੜੀਵਾਰ
         s = r & 15; // combined length
         j->ਕੋਡ_ਬਫਰ <<= s;
         j->ਕੋਡ_ਬਿੱਟ -= s;
         // decode into unzigzag'd location
         zig = stbi__jpeg_dezigzag[k++];
         ਅੰਕੜੇ[zig] = (ਛੋਟਾ) ((r >> 8) * dequant[zig]);
      } ਨਹੀਂਤਾਂ {
         ਪੂਰਨ rs = stbi__jpeg_huff_decode(j, hac);
         ਜੇ (rs < 0) ਵਾਪਸ stbi__err("bad huffman code","Corrupt JPEG");
         s = rs & 15;
         r = rs >> 4;
         ਜੇ (s == 0) {
            ਜੇ (rs != 0xf0) ਰੋਕੋ; // end block
            k += 16;
         } ਨਹੀਂਤਾਂ {
            k += r;
            // decode into unzigzag'd location
            zig = stbi__jpeg_dezigzag[k++];
            ਅੰਕੜੇ[zig] = (ਛੋਟਾ) (stbi__extend_receive(j,s) * dequant[zig]);
         }
      }
   } ਜਦੋਂ (k < 64);
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi_jpeg_decode_ਖੰਡ_prog_dc(stbi__jpeg *j, ਛੋਟਾ ਅੰਕੜੇ[64], stbi__huffman *hdc, ਪੂਰਨ b)
{
   ਪੂਰਨ diff,dc;
   ਪੂਰਨ t;
   ਜੇ (j->spec_ਸਿਰਾ != 0) ਵਾਪਸ stbi__err("can't merge dc and ac", "Corrupt JPEG");

   ਜੇ (j->ਕੋਡ_ਬਿੱਟ < 16) stbi_grow_ਬਫਰ_unsafe(j);

   ਜੇ (j->succ_ਉੱਚਾ == 0) {
      // first scan for DC coefficient, must be first
      memset(ਅੰਕੜੇ,0,64*ਆਕਾਰ(ਅੰਕੜੇ[0])); // 0 all the ac values now
      t = stbi__jpeg_huff_decode(j, hdc);
      diff = t ? stbi__extend_receive(j, t) : 0;

      dc = j->img_comp[b].dc_pred + diff;
      j->img_comp[b].dc_pred = dc;
      ਅੰਕੜੇ[0] = (ਛੋਟਾ) (dc << j->succ_ਨੀਵਾਂ);
   } ਨਹੀਂਤਾਂ {
      // refinement scan for DC coefficient
      ਜੇ (stbi_jpeg_get_ਦੋਆਧਾਰੀ_ਅੰਕ(j))
         ਅੰਕੜੇ[0] += (ਛੋਟਾ) (1 << j->succ_ਨੀਵਾਂ);
   }
   ਵਾਪਸ 1;
}

// @OPTIMIZE: store non-zigzagged during the decode passes,
// and only de-zigzag when dequantizing
ਸਥਿਰ ਪੂਰਨ stbi_jpeg_decode_ਖੰਡ_prog_ac(stbi__jpeg *j, ਛੋਟਾ ਅੰਕੜੇ[64], stbi__huffman *hac, stbi__int16 *fac)
{
   ਪੂਰਨ k;
   ਜੇ (j->spec_ਸ਼ੁਰੂ == 0) ਵਾਪਸ stbi__err("can't merge dc and ac", "Corrupt JPEG");

   ਜੇ (j->succ_ਉੱਚਾ == 0) {
      ਪੂਰਨ ਖਿਸਕਾਓ = j->succ_ਨੀਵਾਂ;

      ਜੇ (j->eob_ਲੜੀਵਾਰ) {
         --j->eob_ਲੜੀਵਾਰ;
         ਵਾਪਸ 1;
      }

      k = j->spec_ਸ਼ੁਰੂ;
      ਕਰੋ {
         ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ zig;
         ਪੂਰਨ c,r,s;
         ਜੇ (j->ਕੋਡ_ਬਿੱਟ < 16) stbi_grow_ਬਫਰ_unsafe(j);
         c = (j->ਕੋਡ_ਬਫਰ >> (32 - FAST_ਬਿੱਟ)) & ((1 << FAST_ਬਿੱਟ)-1);
         r = fac[c];
         ਜੇ (r) { // fast-AC path
            k += (r >> 4) & 15; //ਲੜੀਵਾਰ
            s = r & 15; // combined length
            j->ਕੋਡ_ਬਫਰ <<= s;
            j->ਕੋਡ_ਬਿੱਟ -= s;
            zig = stbi__jpeg_dezigzag[k++];
            ਅੰਕੜੇ[zig] = (ਛੋਟਾ) ((r >> 8) << ਖਿਸਕਾਓ);
         } ਨਹੀਂਤਾਂ {
            ਪੂਰਨ rs = stbi__jpeg_huff_decode(j, hac);
            ਜੇ (rs < 0) ਵਾਪਸ stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            ਜੇ (s == 0) {
               ਜੇ (r < 15) {
                  j->eob_ਲੜੀਵਾਰ = (1 << r);
                  ਜੇ (r)
                     j->eob_ਲੜੀਵਾਰ += stbi_jpeg_get_ਬਿੱਟ(j, r);
                  --j->eob_ਲੜੀਵਾਰ;
                  ਰੋਕੋ;
               }
               k += 16;
            } ਨਹੀਂਤਾਂ {
               k += r;
               zig = stbi__jpeg_dezigzag[k++];
               ਅੰਕੜੇ[zig] = (ਛੋਟਾ) (stbi__extend_receive(j,s) << ਖਿਸਕਾਓ);
            }
         }
      } ਜਦੋਂ (k <= j->spec_ਸਿਰਾ);
   } ਨਹੀਂਤਾਂ {
      // refinement scan for these AC coefficients

      ਛੋਟਾ ਦੋਆਧਾਰੀ_ਅੰਕ = (ਛੋਟਾ) (1 << j->succ_ਨੀਵਾਂ);

      ਜੇ (j->eob_ਲੜੀਵਾਰ) {
         --j->eob_ਲੜੀਵਾਰ;
         ਲਈ (k = j->spec_ਸ਼ੁਰੂ; k <= j->spec_ਸਿਰਾ; ++k) {
            ਛੋਟਾ *p = &ਅੰਕੜੇ[stbi__jpeg_dezigzag[k]];
            ਜੇ (*p != 0)
               ਜੇ (stbi_jpeg_get_ਦੋਆਧਾਰੀ_ਅੰਕ(j))
                  ਜੇ ((*p & ਦੋਆਧਾਰੀ_ਅੰਕ)==0) {
                     ਜੇ (*p > 0)
                        *p += ਦੋਆਧਾਰੀ_ਅੰਕ;
                     ਨਹੀਂਤਾਂ
                        *p -= ਦੋਆਧਾਰੀ_ਅੰਕ;
                  }
         }
      } ਨਹੀਂਤਾਂ {
         k = j->spec_ਸ਼ੁਰੂ;
         ਕਰੋ {
            ਪੂਰਨ r,s;
            ਪੂਰਨ rs = stbi__jpeg_huff_decode(j, hac); // @OPTIMIZE see if we can use the fast path here, advance-by-r is so slow, eh
            ਜੇ (rs < 0) ਵਾਪਸ stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            ਜੇ (s == 0) {
               ਜੇ (r < 15) {
                  j->eob_ਲੜੀਵਾਰ = (1 << r) - 1;
                  ਜੇ (r)
                     j->eob_ਲੜੀਵਾਰ += stbi_jpeg_get_ਬਿੱਟ(j, r);
                  r = 64; // force end of block
               } ਨਹੀਂਤਾਂ {
                  // r=15 s=0 should write 16 0s, so we just do
                  // a run of 15 0s and then write s (which is 0),
                  // so we don't have to do anything special here
               }
            } ਨਹੀਂਤਾਂ {
               ਜੇ (s != 1) ਵਾਪਸ stbi__err("bad huffman code", "Corrupt JPEG");
               // sign bit
               ਜੇ (stbi_jpeg_get_ਦੋਆਧਾਰੀ_ਅੰਕ(j))
                  s = ਦੋਆਧਾਰੀ_ਅੰਕ;
               ਨਹੀਂਤਾਂ
                  s = -ਦੋਆਧਾਰੀ_ਅੰਕ;
            }

            // advance by r
            ਜਦੋਂ (k <= j->spec_ਸਿਰਾ) {
               ਛੋਟਾ *p = &ਅੰਕੜੇ[stbi__jpeg_dezigzag[k++]];
               ਜੇ (*p != 0) {
                  ਜੇ (stbi_jpeg_get_ਦੋਆਧਾਰੀ_ਅੰਕ(j))
                     ਜੇ ((*p & ਦੋਆਧਾਰੀ_ਅੰਕ)==0) {
                        ਜੇ (*p > 0)
                           *p += ਦੋਆਧਾਰੀ_ਅੰਕ;
                        ਨਹੀਂਤਾਂ
                           *p -= ਦੋਆਧਾਰੀ_ਅੰਕ;
                     }
               } ਨਹੀਂਤਾਂ {
                  ਜੇ (r == 0) {
                     *p = (ਛੋਟਾ) s;
                     ਰੋਕੋ;
                  }
                  --r;
               }
            }
         } ਜਦੋਂ (k <= j->spec_ਸਿਰਾ);
      }
   }
   ਵਾਪਸ 1;
}

// take a -128..127 value and stbi__clamp it and convert to 0..255
stbi_inline ਸਥਿਰ stbi_uc stbi__clamp(ਪੂਰਨ x)
{
   // trick to use a single test to catch both cases
   ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) x > 255) {
      ਜੇ (x < 0) ਵਾਪਸ 0;
      ਜੇ (x > 255) ਵਾਪਸ 255;
   }
   ਵਾਪਸ (stbi_uc) x;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__f2f(x)  ((ਪੂਰਨ) (((x) * 4096 + 0.5)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__fsh(x)  ((x) * 4096)

// derived from jidctint -- DCT_ISLOW
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7) \
   ਪੂਰਨ t0,t1,t2,t3,p1,p2,p3,p4,p5,x0,x1,x2,x3; \
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

ਸਥਿਰ ਖਾਲੀ stbi_idct_ਖੰਡ(stbi_uc *ਬਾਹਰਲਾ, ਪੂਰਨ ਬਾਹਰਲਾ_stride, ਛੋਟਾ ਅੰਕੜੇ[64])
{
   ਪੂਰਨ i,val[64],*v=val;
   stbi_uc *o;
   ਛੋਟਾ *d = ਅੰਕੜੇ;

   // columns
   ਲਈ (i=0; i < 8; ++i,++d, ++v) {
      // if all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing
      ਜੇ (d[ 8]==0 && d[16]==0 && d[24]==0 && d[32]==0
           && d[40]==0 && d[48]==0 && d[56]==0) {
         //    no shortcut                 0     seconds
         //    (1|2|3|4|5|6|7)==0          0     seconds
         //    all separate               -0.047 seconds
         //    1 && 2|3 && 4|5 && 6|7:    -0.047 seconds
         ਪੂਰਨ dcterm = d[0]*4;
         v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
      } ਨਹੀਂਤਾਂ {
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

   ਲਈ (i=0, v=val, o=ਬਾਹਰਲਾ; i < 8; ++i,v+=8,o+=ਬਾਹਰਲਾ_stride) {
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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_SSE2
// sse2 integer IDCT. not the fastest possible implementation but it
// produces bit-identical results to the generic C version so it's
// fully "transparent".
ਸਥਿਰ ਖਾਲੀ stbi__idct_simd(stbi_uc *ਬਾਹਰਲਾ, ਪੂਰਨ ਬਾਹਰਲਾ_stride, ਛੋਟਾ ਅੰਕੜੇ[64])
{
   // This is constructed to match our regular (generic) integer IDCT exactly.
   __m128i row0, row1, row2, row3, row4, row5, row6, row7;
   __m128i tmp;

   // dot product constant: even elems=x, odd elems=y
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_const(x,y)  _mm_setr_epi16((x),(y),(x),(y),(x),(y),(x),(y))

   // out(0) = c0[even]*x + c0[odd]*y   (c0, x, y 16-bit, out 32-bit)
   // out(1) = c1[even]*x + c1[odd]*y
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_rot(out0,out1, x,y,c0,c1) \
      __m128i c0##lo = _mm_unpacklo_epi16((x),(y)); \
      __m128i c0##hi = _mm_unpackhi_epi16((x),(y)); \
      __m128i out0##_l = _mm_madd_epi16(c0##lo, c0); \
      __m128i out0##_h = _mm_madd_epi16(c0##hi, c0); \
      __m128i out1##_l = _mm_madd_epi16(c0##lo, c1); \
      __m128i out1##_h = _mm_madd_epi16(c0##hi, c1)

   // out = in << 12  (in 16-bit, out 32-bit)
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_widen(ਬਾਹਰਲਾ, ਵਿੱਚ) \
      __m128i ਬਾਹਰਲਾ##_l = _mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), (ਵਿੱਚ)), 4); \
      __m128i ਬਾਹਰਲਾ##_h = _mm_srai_epi32(_mm_unpackhi_epi16(_mm_setzero_si128(), (ਵਿੱਚ)), 4)

   // wide add
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_wadd(ਬਾਹਰਲਾ, a, b) \
      __m128i ਬਾਹਰਲਾ##_l = mm_ਜੋੜੋ_epi32(a##_l, b##_l); \
      __m128i ਬਾਹਰਲਾ##_h = mm_ਜੋੜੋ_epi32(a##_h, b##_h)

   // wide sub
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_wsub(ਬਾਹਰਲਾ, a, b) \
      __m128i ਬਾਹਰਲਾ##_l = _mm_sub_epi32(a##_l, b##_l); \
      __m128i ਬਾਹਰਲਾ##_h = _mm_sub_epi32(a##_h, b##_h)

   // butterfly a/b, add bias, then shift by "s" and pack
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_bfly32o(out0, out1, a,b,bias,s) \
      { \
         __m128i abiased_l = mm_ਜੋੜੋ_epi32(a##_l, bias); \
         __m128i abiased_h = mm_ਜੋੜੋ_epi32(a##_h, bias); \
         dct_wadd(ਜੋੜ, abiased, b); \
         dct_wsub(dif, abiased, b); \
         out0 = _mm_packs_epi32(_mm_srai_epi32(ਜੋੜ_l, s), _mm_srai_epi32(ਜੋੜ_h, s)); \
         out1 = _mm_packs_epi32(_mm_srai_epi32(dif_l, s), _mm_srai_epi32(dif_h, s)); \
      }

   // 8-bit interleave step (for transposes)
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_interleave8(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi8(a, b); \
      b = _mm_unpackhi_epi8(tmp, b)

   // 16-bit interleave step (for transposes)
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_interleave16(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi16(a, b); \
      b = _mm_unpackhi_epi16(tmp, b)

   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_pass(bias,ਖਿਸਕਾਓ) \
      { \
         /* even part */ \
         dct_rot(t2e,t3e, row2,row6, rot0_0,rot0_1); \
         __m128i sum04 = mm_ਜੋੜੋ_epi16(row0, row4); \
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
         __m128i sum17 = mm_ਜੋੜੋ_epi16(row1, row7); \
         __m128i sum35 = mm_ਜੋੜੋ_epi16(row3, row5); \
         dct_rot(y4o,y5o, sum17,sum35, rot1_0,rot1_1); \
         dct_wadd(x4, y0o, y4o); \
         dct_wadd(x5, y1o, y5o); \
         dct_wadd(x6, y2o, y5o); \
         dct_wadd(x7, y3o, y4o); \
         dct_bfly32o(row0,row7, x0,x7,bias,ਖਿਸਕਾਓ); \
         dct_bfly32o(row1,row6, x1,x6,bias,ਖਿਸਕਾਓ); \
         dct_bfly32o(row2,row5, x2,x5,bias,ਖਿਸਕਾਓ); \
         dct_bfly32o(row3,row4, x3,x4,bias,ਖਿਸਕਾਓ); \
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

   //ਲੱਦੋ
   row0 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 0*8));
   row1 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 1*8));
   row2 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 2*8));
   row3 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 3*8));
   row4 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 4*8));
   row5 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 5*8));
   row6 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 6*8));
   row7 = mm_ਲੱਦੋ_si128((ਅਚਲ __m128i *) (ਅੰਕੜੇ + 7*8));

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
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, p0); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, _mm_shuffle_epi32(p0, 0x4e)); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, p2); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, _mm_shuffle_epi32(p2, 0x4e)); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, p1); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, _mm_shuffle_epi32(p1, 0x4e)); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, p3); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      _mm_storel_epi64((__m128i *) ਬਾਹਰਲਾ, _mm_shuffle_epi32(p3, 0x4e));
   }

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_const
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_rot
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_widen
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_wadd
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_wsub
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_bfly32o
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_interleave8
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_interleave16
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_pass
}

#ਜੇ_ਅੰਤ // STBI_SSE2

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_NEON

// NEON integer IDCT. should produce bit-identical
// results to the generic C version.
ਸਥਿਰ ਖਾਲੀ stbi__idct_simd(stbi_uc *ਬਾਹਰਲਾ, ਪੂਰਨ ਬਾਹਰਲਾ_stride, ਛੋਟਾ ਅੰਕੜੇ[64])
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_long_mul(ਬਾਹਰਲਾ, inq, coeff) \
   int32x4_t ਬਾਹਰਲਾ##_l = vmull_s16(vget_ਨੀਵਾਂ_s16(inq), coeff); \
   int32x4_t ਬਾਹਰਲਾ##_h = vmull_s16(vget_ਉੱਚਾ_s16(inq), coeff)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_long_mac(ਬਾਹਰਲਾ, acc, inq, coeff) \
   int32x4_t ਬਾਹਰਲਾ##_l = vmlal_s16(acc##_l, vget_ਨੀਵਾਂ_s16(inq), coeff); \
   int32x4_t ਬਾਹਰਲਾ##_h = vmlal_s16(acc##_h, vget_ਉੱਚਾ_s16(inq), coeff)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_widen(ਬਾਹਰਲਾ, inq) \
   int32x4_t ਬਾਹਰਲਾ##_l = vshll_n_s16(vget_ਨੀਵਾਂ_s16(inq), 12); \
   int32x4_t ਬਾਹਰਲਾ##_h = vshll_n_s16(vget_ਉੱਚਾ_s16(inq), 12)

// wide add
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_wadd(ਬਾਹਰਲਾ, a, b) \
   int32x4_t ਬਾਹਰਲਾ##_l = vaddq_s32(a##_l, b##_l); \
   int32x4_t ਬਾਹਰਲਾ##_h = vaddq_s32(a##_h, b##_h)

// wide sub
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_wsub(ਬਾਹਰਲਾ, a, b) \
   int32x4_t ਬਾਹਰਲਾ##_l = vsubq_s32(a##_l, b##_l); \
   int32x4_t ਬਾਹਰਲਾ##_h = vsubq_s32(a##_h, b##_h)

// butterfly a/b, then shift using "shiftop" by "s" and pack
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_bfly32o(out0,out1, a,b,shiftop,s) \
   { \
      dct_wadd(ਜੋੜ, a, b); \
      dct_wsub(dif, a, b); \
      out0 = vcombine_s16(shiftop(ਜੋੜ_l, s), shiftop(ਜੋੜ_h, s)); \
      out1 = vcombine_s16(shiftop(dif_l, s), shiftop(dif_h, s)); \
   }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_pass(shiftop, ਖਿਸਕਾਓ) \
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
      dct_bfly32o(row0,row7, x0,x7,shiftop,ਖਿਸਕਾਓ); \
      dct_bfly32o(row1,row6, x1,x6,shiftop,ਖਿਸਕਾਓ); \
      dct_bfly32o(row2,row5, x2,x5,shiftop,ਖਿਸਕਾਓ); \
      dct_bfly32o(row3,row4, x3,x4,shiftop,ਖਿਸਕਾਓ); \
   }

   //ਲੱਦੋ
   row0 = vld1q_s16(ਅੰਕੜੇ + 0*8);
   row1 = vld1q_s16(ਅੰਕੜੇ + 1*8);
   row2 = vld1q_s16(ਅੰਕੜੇ + 2*8);
   row3 = vld1q_s16(ਅੰਕੜੇ + 3*8);
   row4 = vld1q_s16(ਅੰਕੜੇ + 4*8);
   row5 = vld1q_s16(ਅੰਕੜੇ + 5*8);
   row6 = vld1q_s16(ਅੰਕੜੇ + 6*8);
   row7 = vld1q_s16(ਅੰਕੜੇ + 7*8);

   // add DC bias
   row0 = vaddq_s16(row0, vsetq_lane_s16(1024, vdupq_n_s16(0), 0));

   // column pass
   dct_pass(vrshrn_n_s32, 10);

   // 16bit 8x8 transpose
   {
// these three map to a single VTRN.16, VTRN.32, and VSWP, respectively.
// whether compilers actually get this is another story, sadly.
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_trn16(x, y) { int16x8x2_t t = vtrnq_s16(x, y); x = t.val[0]; y = t.val[1]; }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_trn32(x, y) { int32x4x2_t t = vtrnq_s32(vreinterpretq_s32_s16(x), vreinterpretq_s32_s16(y)); x = vreinterpretq_s16_s32(t.val[0]); y = vreinterpretq_s16_s32(t.val[1]); }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_trn64(x, y) { int16x8_t x0 = x; int16x8_t y0 = y; x = vcombine_s16(vget_ਨੀਵਾਂ_s16(x0), vget_ਨੀਵਾਂ_s16(y0)); y = vcombine_s16(vget_ਉੱਚਾ_s16(x0), vget_ਉੱਚਾ_s16(y0)); }

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

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_trn16
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_trn32
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_trn64
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_trn8_8(x, y) { uint8x8x2_t t = vtrn_u8(x, y); x = t.val[0]; y = t.val[1]; }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_trn8_16(x, y) { uint16x4x2_t t = vtrn_u16(vreinterpret_u16_u8(x), vreinterpret_u16_u8(y)); x = vreinterpret_u8_u16(t.val[0]); y = vreinterpret_u8_u16(t.val[1]); }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dct_trn8_32(x, y) { uint32x2x2_t t = vtrn_u32(vreinterpret_u32_u8(x), vreinterpret_u32_u8(y)); x = vreinterpret_u8_u32(t.val[0]); y = vreinterpret_u8_u32(t.val[1]); }

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
      vst1_u8(ਬਾਹਰਲਾ, p0); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      vst1_u8(ਬਾਹਰਲਾ, p1); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      vst1_u8(ਬਾਹਰਲਾ, p2); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      vst1_u8(ਬਾਹਰਲਾ, p3); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      vst1_u8(ਬਾਹਰਲਾ, p4); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      vst1_u8(ਬਾਹਰਲਾ, p5); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      vst1_u8(ਬਾਹਰਲਾ, p6); ਬਾਹਰਲਾ += ਬਾਹਰਲਾ_stride;
      vst1_u8(ਬਾਹਰਲਾ, p7);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_trn8_8
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_trn8_16
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_trn8_32
   }

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_long_mul
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_long_mac
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_widen
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_wadd
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_wsub
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_bfly32o
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ dct_pass
}

#ਜੇ_ਅੰਤ // STBI_NEON

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__MARKER_none  0xff
// if there's a pending marker from the entropy stream, return that
// otherwise, fetch from the stream and get a marker. if there's no
// marker, return 0xff, which is never a valid marker value
ਸਥਿਰ stbi_uc stbi__get_marker(stbi__jpeg *j)
{
   stbi_uc x;
   ਜੇ (j->marker != STBI__MARKER_none) { x = j->marker; j->marker = STBI__MARKER_none; ਵਾਪਸ x; }
   x = stbi__get8(j->s);
   ਜੇ (x != 0xff) ਵਾਪਸ STBI__MARKER_none;
   ਜਦੋਂ (x == 0xff)
      x = stbi__get8(j->s); // consume repeated 0xff fill bytes
   ਵਾਪਸ x;
}

// in each scan, we'll have scan_n components, and the order
// of the components is specified by order[]
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)

// after a restart interval, stbi__jpeg_reset the entropy decoder and
// the dc prediction
ਸਥਿਰ ਖਾਲੀ stbi__jpeg_reset(stbi__jpeg *j)
{
   j->ਕੋਡ_ਬਿੱਟ = 0;
   j->ਕੋਡ_ਬਫਰ = 0;
   j->nomore = 0;
   j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = j->img_comp[3].dc_pred = 0;
   j->marker = STBI__MARKER_none;
   j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
   j->eob_ਲੜੀਵਾਰ = 0;
   // no more than 1<<31 MCUs if no restart_interal? that's plenty safe,
   // since we don't even allow 1<<30 pixels
}

ਸਥਿਰ ਪੂਰਨ stbi_parse_entropy_coded_ਅੰਕੜੇ(stbi__jpeg *z)
{
   stbi__jpeg_reset(z);
   ਜੇ (!z->progressive) {
      ਜੇ (z->ਜਾਂਚ_n == 1) {
         ਪੂਰਨ i,j;
         STBI_SIMD_ALIGN(ਛੋਟਾ, ਅੰਕੜੇ[64]);
         ਪੂਰਨ n = z->ਕ੍ਰਮ[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         ਪੂਰਨ w = (z->img_comp[n].x+7) >> 3;
         ਪੂਰਨ h = (z->img_comp[n].y+7) >> 3;
         ਲਈ (j=0; j < h; ++j) {
            ਲਈ (i=0; i < w; ++i) {
               ਪੂਰਨ ha = z->img_comp[n].ha;
               ਜੇ (!stbi_jpeg_decode_ਖੰਡ(z, ਅੰਕੜੇ, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) ਵਾਪਸ 0;
               z->idct_ਖੰਡ_kernel(z->img_comp[n].ਅੰਕੜੇ+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, ਅੰਕੜੇ);
               // every data block is an MCU, so countdown the restart interval
               ਜੇ (--z->todo <= 0) {
                  ਜੇ (z->ਕੋਡ_ਬਿੱਟ < 24) stbi_grow_ਬਫਰ_unsafe(z);
                  // if it's NOT a restart, then just bail, so we get corrupt data
                  // rather than no data
                  ਜੇ (!STBI__RESTART(z->marker)) ਵਾਪਸ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ਵਾਪਸ 1;
      } ਨਹੀਂਤਾਂ { // interleaved
         ਪੂਰਨ i,j,k,x,y;
         STBI_SIMD_ALIGN(ਛੋਟਾ, ਅੰਕੜੇ[64]);
         ਲਈ (j=0; j < z->img_mcu_y; ++j) {
            ਲਈ (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               ਲਈ (k=0; k < z->ਜਾਂਚ_n; ++k) {
                  ਪੂਰਨ n = z->ਕ੍ਰਮ[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  ਲਈ (y=0; y < z->img_comp[n].v; ++y) {
                     ਲਈ (x=0; x < z->img_comp[n].h; ++x) {
                        ਪੂਰਨ x2 = (i*z->img_comp[n].h + x)*8;
                        ਪੂਰਨ y2 = (j*z->img_comp[n].v + y)*8;
                        ਪੂਰਨ ha = z->img_comp[n].ha;
                        ਜੇ (!stbi_jpeg_decode_ਖੰਡ(z, ਅੰਕੜੇ, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) ਵਾਪਸ 0;
                        z->idct_ਖੰਡ_kernel(z->img_comp[n].ਅੰਕੜੇ+z->img_comp[n].w2*y2+x2, z->img_comp[n].w2, ਅੰਕੜੇ);
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               ਜੇ (--z->todo <= 0) {
                  ਜੇ (z->ਕੋਡ_ਬਿੱਟ < 24) stbi_grow_ਬਫਰ_unsafe(z);
                  ਜੇ (!STBI__RESTART(z->marker)) ਵਾਪਸ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ਵਾਪਸ 1;
      }
   } ਨਹੀਂਤਾਂ {
      ਜੇ (z->ਜਾਂਚ_n == 1) {
         ਪੂਰਨ i,j;
         ਪੂਰਨ n = z->ਕ੍ਰਮ[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         ਪੂਰਨ w = (z->img_comp[n].x+7) >> 3;
         ਪੂਰਨ h = (z->img_comp[n].y+7) >> 3;
         ਲਈ (j=0; j < h; ++j) {
            ਲਈ (i=0; i < w; ++i) {
               ਛੋਟਾ *ਅੰਕੜੇ = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               ਜੇ (z->spec_ਸ਼ੁਰੂ == 0) {
                  ਜੇ (!stbi_jpeg_decode_ਖੰਡ_prog_dc(z, ਅੰਕੜੇ, &z->huff_dc[z->img_comp[n].hd], n))
                     ਵਾਪਸ 0;
               } ਨਹੀਂਤਾਂ {
                  ਪੂਰਨ ha = z->img_comp[n].ha;
                  ਜੇ (!stbi_jpeg_decode_ਖੰਡ_prog_ac(z, ਅੰਕੜੇ, &z->huff_ac[ha], z->fast_ac[ha]))
                     ਵਾਪਸ 0;
               }
               // every data block is an MCU, so countdown the restart interval
               ਜੇ (--z->todo <= 0) {
                  ਜੇ (z->ਕੋਡ_ਬਿੱਟ < 24) stbi_grow_ਬਫਰ_unsafe(z);
                  ਜੇ (!STBI__RESTART(z->marker)) ਵਾਪਸ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ਵਾਪਸ 1;
      } ਨਹੀਂਤਾਂ { // interleaved
         ਪੂਰਨ i,j,k,x,y;
         ਲਈ (j=0; j < z->img_mcu_y; ++j) {
            ਲਈ (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               ਲਈ (k=0; k < z->ਜਾਂਚ_n; ++k) {
                  ਪੂਰਨ n = z->ਕ੍ਰਮ[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  ਲਈ (y=0; y < z->img_comp[n].v; ++y) {
                     ਲਈ (x=0; x < z->img_comp[n].h; ++x) {
                        ਪੂਰਨ x2 = (i*z->img_comp[n].h + x);
                        ਪੂਰਨ y2 = (j*z->img_comp[n].v + y);
                        ਛੋਟਾ *ਅੰਕੜੇ = z->img_comp[n].coeff + 64 * (x2 + y2 * z->img_comp[n].coeff_w);
                        ਜੇ (!stbi_jpeg_decode_ਖੰਡ_prog_dc(z, ਅੰਕੜੇ, &z->huff_dc[z->img_comp[n].hd], n))
                           ਵਾਪਸ 0;
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               ਜੇ (--z->todo <= 0) {
                  ਜੇ (z->ਕੋਡ_ਬਿੱਟ < 24) stbi_grow_ਬਫਰ_unsafe(z);
                  ਜੇ (!STBI__RESTART(z->marker)) ਵਾਪਸ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ਵਾਪਸ 1;
      }
   }
}

ਸਥਿਰ ਖਾਲੀ stbi__jpeg_dequantize(ਛੋਟਾ *ਅੰਕੜੇ, stbi__uint16 *dequant)
{
   ਪੂਰਨ i;
   ਲਈ (i=0; i < 64; ++i)
      ਅੰਕੜੇ[i] *= dequant[i];
}

ਸਥਿਰ ਖਾਲੀ stbi_jpeg_ਅੰਤ(stbi__jpeg *z)
{
   ਜੇ (z->progressive) {
      // dequantize and idct the data
      ਪੂਰਨ i,j,n;
      ਲਈ (n=0; n < z->s->img_n; ++n) {
         ਪੂਰਨ w = (z->img_comp[n].x+7) >> 3;
         ਪੂਰਨ h = (z->img_comp[n].y+7) >> 3;
         ਲਈ (j=0; j < h; ++j) {
            ਲਈ (i=0; i < w; ++i) {
               ਛੋਟਾ *ਅੰਕੜੇ = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               stbi__jpeg_dequantize(ਅੰਕੜੇ, z->dequant[z->img_comp[n].tq]);
               z->idct_ਖੰਡ_kernel(z->img_comp[n].ਅੰਕੜੇ+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, ਅੰਕੜੇ);
            }
         }
      }
   }
}

ਸਥਿਰ ਪੂਰਨ stbi__process_marker(stbi__jpeg *z, ਪੂਰਨ m)
{
   ਪੂਰਨ L;
   ਚੋਣ (m) {
      ਹਾਲਤ STBI__MARKER_none: // no marker found
         ਵਾਪਸ stbi__err("expected marker","Corrupt JPEG");

      ਹਾਲਤ 0xDD: // DRI - specify restart interval
         ਜੇ (stbi__get16be(z->s) != 4) ਵਾਪਸ stbi__err("bad DRI len","Corrupt JPEG");
         z->restart_interval = stbi__get16be(z->s);
         ਵਾਪਸ 1;

      ਹਾਲਤ 0xDB: // DQT - define quantization table
         L = stbi__get16be(z->s)-2;
         ਜਦੋਂ (L > 0) {
            ਪੂਰਨ q = stbi__get8(z->s);
            ਪੂਰਨ p = q >> 4, sixteen = (p != 0);
            ਪੂਰਨ t = q & 15,i;
            ਜੇ (p != 0 && p != 1) ਵਾਪਸ stbi__err("bad DQT type","Corrupt JPEG");
            ਜੇ (t > 3) ਵਾਪਸ stbi__err("bad DQT table","Corrupt JPEG");

            ਲਈ (i=0; i < 64; ++i)
               z->dequant[t][stbi__jpeg_dezigzag[i]] = (stbi__uint16)(sixteen ? stbi__get16be(z->s) : stbi__get8(z->s));
            L -= (sixteen ? 129 : 65);
         }
         ਵਾਪਸ L==0;

      ਹਾਲਤ 0xC4: // DHT - define huffman table
         L = stbi__get16be(z->s)-2;
         ਜਦੋਂ (L > 0) {
            stbi_uc *v;
            ਪੂਰਨ sizes[16],i,n=0;
            ਪੂਰਨ q = stbi__get8(z->s);
            ਪੂਰਨ tc = q >> 4;
            ਪੂਰਨ th = q & 15;
            ਜੇ (tc > 1 || th > 3) ਵਾਪਸ stbi__err("bad DHT header","Corrupt JPEG");
            ਲਈ (i=0; i < 16; ++i) {
               sizes[i] = stbi__get8(z->s);
               n += sizes[i];
            }
            L -= 17;
            ਜੇ (tc == 0) {
               ਜੇ (!stbi__build_huffman(z->huff_dc+th, sizes)) ਵਾਪਸ 0;
               v = z->huff_dc[th].ਮੁੱਲਾਂ;
            } ਨਹੀਂਤਾਂ {
               ਜੇ (!stbi__build_huffman(z->huff_ac+th, sizes)) ਵਾਪਸ 0;
               v = z->huff_ac[th].ਮੁੱਲਾਂ;
            }
            ਲਈ (i=0; i < n; ++i)
               v[i] = stbi__get8(z->s);
            ਜੇ (tc != 0)
               stbi__build_fast_ac(z->fast_ac[th], z->huff_ac + th);
            L -= n;
         }
         ਵਾਪਸ L==0;
   }

   // check for comment block or APP blocks
   ਜੇ ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
      L = stbi__get16be(z->s);
      ਜੇ (L < 2) {
         ਜੇ (m == 0xFE)
            ਵਾਪਸ stbi__err("bad COM len","Corrupt JPEG");
         ਨਹੀਂਤਾਂ
            ਵਾਪਸ stbi__err("bad APP len","Corrupt JPEG");
      }
      L -= 2;

      ਜੇ (m == 0xE0 && L >= 5) { // JFIF APP0 segment
         ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਟੈਗ[5] = {'J','F','I','F','\0'};
         ਪੂਰਨ ok = 1;
         ਪੂਰਨ i;
         ਲਈ (i=0; i < 5; ++i)
            ਜੇ (stbi__get8(z->s) != ਟੈਗ[i])
               ok = 0;
         L -= 5;
         ਜੇ (ok)
            z->jfif = 1;
      } ਨਹੀਂਤਾਂ ਜੇ (m == 0xEE && L >= 12) { // Adobe APP14 segment
         ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਟੈਗ[6] = {'A','d','o','b','e','\0'};
         ਪੂਰਨ ok = 1;
         ਪੂਰਨ i;
         ਲਈ (i=0; i < 6; ++i)
            ਜੇ (stbi__get8(z->s) != ਟੈਗ[i])
               ok = 0;
         L -= 6;
         ਜੇ (ok) {
            stbi__get8(z->s); //ਸੰਸਕਰਣ
            stbi__get16be(z->s); // flags0
            stbi__get16be(z->s); // flags1
            z->app14_color_transform = stbi__get8(z->s); // color transform
            L -= 6;
         }
      }

      stbi__skip(z->s, L);
      ਵਾਪਸ 1;
   }

   ਵਾਪਸ stbi__err("unknown marker","Corrupt JPEG");
}

// after we see SOS
ਸਥਿਰ ਪੂਰਨ stbi_process_ਜਾਂਚ_ਸਿਰਲੇਖ(stbi__jpeg *z)
{
   ਪੂਰਨ i;
   ਪੂਰਨ Ls = stbi__get16be(z->s);
   z->ਜਾਂਚ_n = stbi__get8(z->s);
   ਜੇ (z->ਜਾਂਚ_n < 1 || z->ਜਾਂਚ_n > 4 || z->ਜਾਂਚ_n > (ਪੂਰਨ) z->s->img_n) ਵਾਪਸ stbi__err("bad SOS component count","Corrupt JPEG");
   ਜੇ (Ls != 6+2*z->ਜਾਂਚ_n) ਵਾਪਸ stbi__err("bad SOS len","Corrupt JPEG");
   ਲਈ (i=0; i < z->ਜਾਂਚ_n; ++i) {
      ਪੂਰਨ id = stbi__get8(z->s), which;
      ਪੂਰਨ q = stbi__get8(z->s);
      ਲਈ (which = 0; which < z->s->img_n; ++which)
         ਜੇ (z->img_comp[which].id == id)
            ਰੋਕੋ;
      ਜੇ (which == z->s->img_n) ਵਾਪਸ 0; // no match
      z->img_comp[which].hd = q >> 4;   ਜੇ (z->img_comp[which].hd > 3) ਵਾਪਸ stbi__err("bad DC huff","Corrupt JPEG");
      z->img_comp[which].ha = q & 15;   ਜੇ (z->img_comp[which].ha > 3) ਵਾਪਸ stbi__err("bad AC huff","Corrupt JPEG");
      z->ਕ੍ਰਮ[i] = which;
   }

   {
      ਪੂਰਨ aa;
      z->spec_ਸ਼ੁਰੂ = stbi__get8(z->s);
      z->spec_ਸਿਰਾ   = stbi__get8(z->s); // should be 63, but might be 0
      aa = stbi__get8(z->s);
      z->succ_ਉੱਚਾ = (aa >> 4);
      z->succ_ਨੀਵਾਂ  = (aa & 15);
      ਜੇ (z->progressive) {
         ਜੇ (z->spec_ਸ਼ੁਰੂ > 63 || z->spec_ਸਿਰਾ > 63  || z->spec_ਸ਼ੁਰੂ > z->spec_ਸਿਰਾ || z->succ_ਉੱਚਾ > 13 || z->succ_ਨੀਵਾਂ > 13)
            ਵਾਪਸ stbi__err("bad SOS", "Corrupt JPEG");
      } ਨਹੀਂਤਾਂ {
         ਜੇ (z->spec_ਸ਼ੁਰੂ != 0) ਵਾਪਸ stbi__err("bad SOS","Corrupt JPEG");
         ਜੇ (z->succ_ਉੱਚਾ != 0 || z->succ_ਨੀਵਾਂ != 0) ਵਾਪਸ stbi__err("bad SOS","Corrupt JPEG");
         z->spec_ਸਿਰਾ = 63;
      }
   }

   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__free_jpeg_components(stbi__jpeg *z, ਪੂਰਨ ncomp, ਪੂਰਨ why)
{
   ਪੂਰਨ i;
   ਲਈ (i=0; i < ncomp; ++i) {
      ਜੇ (z->img_comp[i].raw_ਅੰਕੜੇ) {
         STBI_FREE(z->img_comp[i].raw_ਅੰਕੜੇ);
         z->img_comp[i].raw_ਅੰਕੜੇ = ਨਿਰਾ;
         z->img_comp[i].ਅੰਕੜੇ = ਨਿਰਾ;
      }
      ਜੇ (z->img_comp[i].raw_coeff) {
         STBI_FREE(z->img_comp[i].raw_coeff);
         z->img_comp[i].raw_coeff = 0;
         z->img_comp[i].coeff = 0;
      }
      ਜੇ (z->img_comp[i].linebuf) {
         STBI_FREE(z->img_comp[i].linebuf);
         z->img_comp[i].linebuf = ਨਿਰਾ;
      }
   }
   ਵਾਪਸ why;
}

ਸਥਿਰ ਪੂਰਨ stbi_process_ਚੌਖਟਾ_ਸਿਰਲੇਖ(stbi__jpeg *z, ਪੂਰਨ ਜਾਂਚ)
{
   stbi__context *s = z->s;
   ਪੂਰਨ Lf,p,i,q, h_max=1,v_max=1,c;
   Lf = stbi__get16be(s);         ਜੇ (Lf < 11) ਵਾਪਸ stbi__err("bad SOF len","Corrupt JPEG"); // JPEG
   p  = stbi__get8(s);            ਜੇ (p != 8) ਵਾਪਸ stbi__err("only 8-bit","JPEG format not supported: 8-bit only"); // JPEG baseline
   s->img_y = stbi__get16be(s);   ਜੇ (s->img_y == 0) ਵਾਪਸ stbi__err("no header height", "JPEG format not supported: delayed height"); // Legal, but we don't handle it--but neither does IJG
   s->img_x = stbi__get16be(s);   ਜੇ (s->img_x == 0) ਵਾਪਸ stbi__err("0 width","Corrupt JPEG"); // JPEG requires
   c = stbi__get8(s);
   ਜੇ (c != 3 && c != 1 && c != 4) ਵਾਪਸ stbi__err("bad component count","Corrupt JPEG");
   s->img_n = c;
   ਲਈ (i=0; i < c; ++i) {
      z->img_comp[i].ਅੰਕੜੇ = ਨਿਰਾ;
      z->img_comp[i].linebuf = ਨਿਰਾ;
   }

   ਜੇ (Lf != 8+3*s->img_n) ਵਾਪਸ stbi__err("bad SOF len","Corrupt JPEG");

   z->rgb = 0;
   ਲਈ (i=0; i < s->img_n; ++i) {
      ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ rgb[3] = { 'R', 'G', 'B' };
      z->img_comp[i].id = stbi__get8(s);
      ਜੇ (s->img_n == 3 && z->img_comp[i].id == rgb[i])
         ++z->rgb;
      q = stbi__get8(s);
      z->img_comp[i].h = (q >> 4);  ਜੇ (!z->img_comp[i].h || z->img_comp[i].h > 4) ਵਾਪਸ stbi__err("bad H","Corrupt JPEG");
      z->img_comp[i].v = q & 15;    ਜੇ (!z->img_comp[i].v || z->img_comp[i].v > 4) ਵਾਪਸ stbi__err("bad V","Corrupt JPEG");
      z->img_comp[i].tq = stbi__get8(s);  ਜੇ (z->img_comp[i].tq > 3) ਵਾਪਸ stbi__err("bad TQ","Corrupt JPEG");
   }

   ਜੇ (ਜਾਂਚ != STBI_ਜਾਂਚ_ਲੱਦੋ) ਵਾਪਸ 1;

   ਜੇ (!stbi__mad3sizes_valid(s->img_x, s->img_y, s->img_n, 0)) ਵਾਪਸ stbi__err("too large", "Image too large to decode");

   ਲਈ (i=0; i < s->img_n; ++i) {
      ਜੇ (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
      ਜੇ (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
   }

   // compute interleaved mcu info
   z->img_h_max = h_max;
   z->img_v_max = v_max;
   z->img_mcu_w = h_max * 8;
   z->img_mcu_h = v_max * 8;
   // these sizes can't be more than 17 bits
   z->img_mcu_x = (s->img_x + z->img_mcu_w-1) / z->img_mcu_w;
   z->img_mcu_y = (s->img_y + z->img_mcu_h-1) / z->img_mcu_h;

   ਲਈ (i=0; i < s->img_n; ++i) {
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
      z->img_comp[i].linebuf = ਨਿਰਾ;
      z->img_comp[i].raw_ਅੰਕੜੇ = stbi__malloc_mad2(z->img_comp[i].w2, z->img_comp[i].h2, 15);
      ਜੇ (z->img_comp[i].raw_ਅੰਕੜੇ == ਨਿਰਾ)
         ਵਾਪਸ stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
      // align blocks for idct using mmx/sse
      z->img_comp[i].ਅੰਕੜੇ = (stbi_uc*) (((size_t) z->img_comp[i].raw_ਅੰਕੜੇ + 15) & ~15);
      ਜੇ (z->progressive) {
         // w2, h2 are multiples of 8 (see above)
         z->img_comp[i].coeff_w = z->img_comp[i].w2 / 8;
         z->img_comp[i].coeff_h = z->img_comp[i].h2 / 8;
         z->img_comp[i].raw_coeff = stbi__malloc_mad3(z->img_comp[i].w2, z->img_comp[i].h2, ਆਕਾਰ(ਛੋਟਾ), 15);
         ਜੇ (z->img_comp[i].raw_coeff == ਨਿਰਾ)
            ਵਾਪਸ stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
         z->img_comp[i].coeff = (ਛੋਟਾ*) (((size_t) z->img_comp[i].raw_coeff + 15) & ~15);
      }
   }

   ਵਾਪਸ 1;
}

// use comparisons since in some cases we handle more than one case (e.g. SOF)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__DNL(x)         ((x) == 0xdc)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__SOI(x)         ((x) == 0xd8)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__EOI(x)         ((x) == 0xd9)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__SOF(x)         ((x) == 0xc0 || (x) == 0xc1 || (x) == 0xc2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__SOS(x)         ((x) == 0xda)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__SOF_progressive(x)   ((x) == 0xc2)

ਸਥਿਰ ਪੂਰਨ stbi_decode_jpeg_ਸਿਰਲੇਖ(stbi__jpeg *z, ਪੂਰਨ ਜਾਂਚ)
{
   ਪੂਰਨ m;
   z->jfif = 0;
   z->app14_color_transform = -1; // valid values are 0,1,2
   z->marker = STBI__MARKER_none; // initialize cached marker to empty
   m = stbi__get_marker(z);
   ਜੇ (!stbi__SOI(m)) ਵਾਪਸ stbi__err("no SOI","Corrupt JPEG");
   ਜੇ (ਜਾਂਚ == STBI_ਜਾਂਚ_type) ਵਾਪਸ 1;
   m = stbi__get_marker(z);
   ਜਦੋਂ (!stbi__SOF(m)) {
      ਜੇ (!stbi__process_marker(z,m)) ਵਾਪਸ 0;
      m = stbi__get_marker(z);
      ਜਦੋਂ (m == STBI__MARKER_none) {
         // some files have extra padding after their blocks, so ok, we'll scan
         ਜੇ (stbi__at_eof(z->s)) ਵਾਪਸ stbi__err("no SOF", "Corrupt JPEG");
         m = stbi__get_marker(z);
      }
   }
   z->progressive = stbi__SOF_progressive(m);
   ਜੇ (!stbi_process_ਚੌਖਟਾ_ਸਿਰਲੇਖ(z, ਜਾਂਚ)) ਵਾਪਸ 0;
   ਵਾਪਸ 1;
}

// decode image to YCbCr format
ਸਥਿਰ ਪੂਰਨ stbi__decode_jpeg_image(stbi__jpeg *j)
{
   ਪੂਰਨ m;
   ਲਈ (m = 0; m < 4; m++) {
      j->img_comp[m].raw_ਅੰਕੜੇ = ਨਿਰਾ;
      j->img_comp[m].raw_coeff = ਨਿਰਾ;
   }
   j->restart_interval = 0;
   ਜੇ (!stbi_decode_jpeg_ਸਿਰਲੇਖ(j, STBI_ਜਾਂਚ_ਲੱਦੋ)) ਵਾਪਸ 0;
   m = stbi__get_marker(j);
   ਜਦੋਂ (!stbi__EOI(m)) {
      ਜੇ (stbi__SOS(m)) {
         ਜੇ (!stbi_process_ਜਾਂਚ_ਸਿਰਲੇਖ(j)) ਵਾਪਸ 0;
         ਜੇ (!stbi_parse_entropy_coded_ਅੰਕੜੇ(j)) ਵਾਪਸ 0;
         ਜੇ (j->marker == STBI__MARKER_none ) {
            // handle 0s at the end of image data from IP Kamera 9060
            ਜਦੋਂ (!stbi__at_eof(j->s)) {
               ਪੂਰਨ x = stbi__get8(j->s);
               ਜੇ (x == 255) {
                  j->marker = stbi__get8(j->s);
                  ਰੋਕੋ;
               }
            }
            // if we reach eof without hitting a marker, stbi__get_marker() below will fail and we'll eventually return 0
         }
      } ਨਹੀਂਤਾਂ ਜੇ (stbi__DNL(m)) {
         ਪੂਰਨ Ld = stbi__get16be(j->s);
         stbi__uint32 NL = stbi__get16be(j->s);
         ਜੇ (Ld != 4) ਵਾਪਸ stbi__err("bad DNL len", "Corrupt JPEG");
         ਜੇ (NL != j->s->img_y) ਵਾਪਸ stbi__err("bad DNL height", "Corrupt JPEG");
      } ਨਹੀਂਤਾਂ {
         ਜੇ (!stbi__process_marker(j, m)) ਵਾਪਸ 0;
      }
      m = stbi__get_marker(j);
   }
   ਜੇ (j->progressive)
      stbi_jpeg_ਅੰਤ(j);
   ਵਾਪਸ 1;
}

// static jfif-centered resampling (across block boundaries)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ stbi_uc *(*resample_ਪੰਕਤੀ_func)(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *in0, stbi_uc *in1,
                                    ਪੂਰਨ w, ਪੂਰਨ hs);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__div4(x) ((stbi_uc) ((x) >> 2))

ਸਥਿਰ stbi_uc *resample_ਪੰਕਤੀ_1(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *ਵਿੱਚ_near, stbi_uc *ਵਿੱਚ_far, ਪੂਰਨ w, ਪੂਰਨ hs)
{
   STBI_NOTUSED(ਬਾਹਰਲਾ);
   STBI_NOTUSED(ਵਿੱਚ_far);
   STBI_NOTUSED(w);
   STBI_NOTUSED(hs);
   ਵਾਪਸ ਵਿੱਚ_near;
}

ਸਥਿਰ stbi_uc* stbi_resample_ਪੰਕਤੀ_v_2(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *ਵਿੱਚ_near, stbi_uc *ਵਿੱਚ_far, ਪੂਰਨ w, ਪੂਰਨ hs)
{
   // need to generate two samples vertically for every one in input
   ਪੂਰਨ i;
   STBI_NOTUSED(hs);
   ਲਈ (i=0; i < w; ++i)
      ਬਾਹਰਲਾ[i] = stbi__div4(3*ਵਿੱਚ_near[i] + ਵਿੱਚ_far[i] + 2);
   ਵਾਪਸ ਬਾਹਰਲਾ;
}

ਸਥਿਰ stbi_uc*  stbi_resample_ਪੰਕਤੀ_h_2(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *ਵਿੱਚ_near, stbi_uc *ਵਿੱਚ_far, ਪੂਰਨ w, ਪੂਰਨ hs)
{
   // need to generate two samples horizontally for every one in input
   ਪੂਰਨ i;
   stbi_uc *ਨਿਵੇਸ਼ = ਵਿੱਚ_near;

   ਜੇ (w == 1) {
      // if only one sample, can't do any interpolation
      ਬਾਹਰਲਾ[0] = ਬਾਹਰਲਾ[1] = ਨਿਵੇਸ਼[0];
      ਵਾਪਸ ਬਾਹਰਲਾ;
   }

   ਬਾਹਰਲਾ[0] = ਨਿਵੇਸ਼[0];
   ਬਾਹਰਲਾ[1] = stbi__div4(ਨਿਵੇਸ਼[0]*3 + ਨਿਵੇਸ਼[1] + 2);
   ਲਈ (i=1; i < w-1; ++i) {
      ਪੂਰਨ n = 3*ਨਿਵੇਸ਼[i]+2;
      ਬਾਹਰਲਾ[i*2+0] = stbi__div4(n+ਨਿਵੇਸ਼[i-1]);
      ਬਾਹਰਲਾ[i*2+1] = stbi__div4(n+ਨਿਵੇਸ਼[i+1]);
   }
   ਬਾਹਰਲਾ[i*2+0] = stbi__div4(ਨਿਵੇਸ਼[w-2]*3 + ਨਿਵੇਸ਼[w-1] + 2);
   ਬਾਹਰਲਾ[i*2+1] = ਨਿਵੇਸ਼[w-1];

   STBI_NOTUSED(ਵਿੱਚ_far);
   STBI_NOTUSED(hs);

   ਵਾਪਸ ਬਾਹਰਲਾ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__div16(x) ((stbi_uc) ((x) >> 4))

ਸਥਿਰ stbi_uc *stbi_resample_ਪੰਕਤੀ_hv_2(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *ਵਿੱਚ_near, stbi_uc *ਵਿੱਚ_far, ਪੂਰਨ w, ਪੂਰਨ hs)
{
   // need to generate 2x2 samples for every one in input
   ਪੂਰਨ i,t0,t1;
   ਜੇ (w == 1) {
      ਬਾਹਰਲਾ[0] = ਬਾਹਰਲਾ[1] = stbi__div4(3*ਵਿੱਚ_near[0] + ਵਿੱਚ_far[0] + 2);
      ਵਾਪਸ ਬਾਹਰਲਾ;
   }

   t1 = 3*ਵਿੱਚ_near[0] + ਵਿੱਚ_far[0];
   ਬਾਹਰਲਾ[0] = stbi__div4(t1+2);
   ਲਈ (i=1; i < w; ++i) {
      t0 = t1;
      t1 = 3*ਵਿੱਚ_near[i]+ਵਿੱਚ_far[i];
      ਬਾਹਰਲਾ[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      ਬਾਹਰਲਾ[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   ਬਾਹਰਲਾ[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   ਵਾਪਸ ਬਾਹਰਲਾ;
}

#ਜੇ defined(STBI_SSE2) || defined(STBI_NEON)
ਸਥਿਰ stbi_uc *stbi_resample_ਪੰਕਤੀ_hv_2_simd(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *ਵਿੱਚ_near, stbi_uc *ਵਿੱਚ_far, ਪੂਰਨ w, ਪੂਰਨ hs)
{
   // need to generate 2x2 samples for every one in input
   ਪੂਰਨ i=0,t0,t1;

   ਜੇ (w == 1) {
      ਬਾਹਰਲਾ[0] = ਬਾਹਰਲਾ[1] = stbi__div4(3*ਵਿੱਚ_near[0] + ਵਿੱਚ_far[0] + 2);
      ਵਾਪਸ ਬਾਹਰਲਾ;
   }

   t1 = 3*ਵਿੱਚ_near[0] + ਵਿੱਚ_far[0];
   // process groups of 8 pixels for as long as we can.
   // note we can't handle the last pixel in a row in this loop
   // because we need to handle the filter boundary conditions.
   ਲਈ (; i < ((w-1) & ~7); i += 8) {
#ਜੇ defined(STBI_SSE2)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      __m128i zero  = _mm_setzero_si128();
      __m128i farb  = _mm_loadl_epi64((__m128i *) (ਵਿੱਚ_far + i));
      __m128i nearb = _mm_loadl_epi64((__m128i *) (ਵਿੱਚ_near + i));
      __m128i farw  = _mm_unpacklo_epi8(farb, zero);
      __m128i nearw = _mm_unpacklo_epi8(nearb, zero);
      __m128i diff  = _mm_sub_epi16(farw, nearw);
      __m128i nears = _mm_slli_epi16(nearw, 2);
      __m128i curr  = mm_ਜੋੜੋ_epi16(nears, diff); // current row

      // horizontal filter works the same based on shifted vers of current
      // row. "prev" is current row shifted right by 1 pixel; we need to
      // insert the previous pixel value (from t1).
      // "next" is current row shifted left by 1 pixel, with first pixel
      // of next block of 8 pixels added in.
      __m128i prv0 = _mm_slli_si128(curr, 2);
      __m128i nxt0 = _mm_srli_si128(curr, 2);
      __m128i prev = mm_ਪਾਓ_epi16(prv0, t1, 0);
      __m128i ਅਗਲਾ = mm_ਪਾਓ_epi16(nxt0, 3*ਵਿੱਚ_near[i+8] + ਵਿੱਚ_far[i+8], 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      __m128i bias  = _mm_set1_epi16(8);
      __m128i curs = _mm_slli_epi16(curr, 2);
      __m128i prvd = _mm_sub_epi16(prev, curr);
      __m128i nxtd = _mm_sub_epi16(ਅਗਲਾ, curr);
      __m128i curb = mm_ਜੋੜੋ_epi16(curs, bias);
      __m128i even = mm_ਜੋੜੋ_epi16(prvd, curb);
      __m128i odd  = mm_ਜੋੜੋ_epi16(nxtd, curb);

      // interleave even and odd pixels, then undo scaling.
      __m128i int0 = _mm_unpacklo_epi16(even, odd);
      __m128i int1 = _mm_unpackhi_epi16(even, odd);
      __m128i de0  = _mm_srli_epi16(int0, 4);
      __m128i de1  = _mm_srli_epi16(int1, 4);

      // pack and write output
      __m128i outv = _mm_packus_epi16(de0, de1);
      _mm_storeu_si128((__m128i *) (ਬਾਹਰਲਾ + i*2), outv);
#ਜਾਂਜੇ defined(STBI_NEON)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      uint8x8_t farb  = vld1_u8(ਵਿੱਚ_far + i);
      uint8x8_t nearb = vld1_u8(ਵਿੱਚ_near + i);
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
      int16x8_t prev = vsetq_lane_s16(t1, prv0, 0);
      int16x8_t ਅਗਲਾ = vsetq_lane_s16(3*ਵਿੱਚ_near[i+8] + ਵਿੱਚ_far[i+8], nxt0, 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      int16x8_t curs = vshlq_n_s16(curr, 2);
      int16x8_t prvd = vsubq_s16(prev, curr);
      int16x8_t nxtd = vsubq_s16(ਅਗਲਾ, curr);
      int16x8_t even = vaddq_s16(curs, prvd);
      int16x8_t odd  = vaddq_s16(curs, nxtd);

      // undo scaling and round, then store with even/odd phases interleaved
      uint8x8x2_t o;
      o.val[0] = vqrshrun_n_s16(even, 4);
      o.val[1] = vqrshrun_n_s16(odd,  4);
      vst2_u8(ਬਾਹਰਲਾ + i*2, o);
#ਜੇ_ਅੰਤ

      // "previous" value for next iter
      t1 = 3*ਵਿੱਚ_near[i+7] + ਵਿੱਚ_far[i+7];
   }

   t0 = t1;
   t1 = 3*ਵਿੱਚ_near[i] + ਵਿੱਚ_far[i];
   ਬਾਹਰਲਾ[i*2] = stbi__div16(3*t1 + t0 + 8);

   ਲਈ (++i; i < w; ++i) {
      t0 = t1;
      t1 = 3*ਵਿੱਚ_near[i]+ਵਿੱਚ_far[i];
      ਬਾਹਰਲਾ[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      ਬਾਹਰਲਾ[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   ਬਾਹਰਲਾ[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   ਵਾਪਸ ਬਾਹਰਲਾ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ stbi_uc *stbi_resample_ਪੰਕਤੀ_generic(stbi_uc *ਬਾਹਰਲਾ, stbi_uc *ਵਿੱਚ_near, stbi_uc *ਵਿੱਚ_far, ਪੂਰਨ w, ਪੂਰਨ hs)
{
   // resample with nearest-neighbor
   ਪੂਰਨ i,j;
   STBI_NOTUSED(ਵਿੱਚ_far);
   ਲਈ (i=0; i < w; ++i)
      ਲਈ (j=0; j < hs; ++j)
         ਬਾਹਰਲਾ[i*hs+j] = ਵਿੱਚ_near[i];
   ਵਾਪਸ ਬਾਹਰਲਾ;
}

// this is a reduced-precision calculation of YCbCr-to-RGB introduced
// to make sure the code produces the same results in both SIMD and scalar
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stbi__float2fixed(x)  (((ਪੂਰਨ) ((x) * 4096.0f + 0.5f)) << 8)
ਸਥਿਰ ਖਾਲੀ stbi_YCb_Cr_ਮੰਜ਼ਿਲ_RGB_ਪੰਕਤੀ(stbi_uc *ਬਾਹਰਲਾ, ਅਚਲ stbi_uc *y, ਅਚਲ stbi_uc *pcb, ਅਚਲ stbi_uc *pcr, ਪੂਰਨ ਗਿਣਤੀ_2, ਪੂਰਨ ਕਦਮ)
{
   ਪੂਰਨ i;
   ਲਈ (i=0; i < ਗਿਣਤੀ_2; ++i) {
      ਪੂਰਨ y_fixed = (y[i] << 20) + (1<<19); // rounding
      ਪੂਰਨ r,g,b;
      ਪੂਰਨ cr = pcr[i] - 128;
      ਪੂਰਨ cb = pcb[i] - 128;
      r = y_fixed +  cr* stbi__float2fixed(1.40200f);
      g = y_fixed + (cr*-stbi__float2fixed(0.71414f)) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                     +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) r > 255) { ਜੇ (r < 0) r = 0; ਨਹੀਂਤਾਂ r = 255; }
      ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) g > 255) { ਜੇ (g < 0) g = 0; ਨਹੀਂਤਾਂ g = 255; }
      ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) b > 255) { ਜੇ (b < 0) b = 0; ਨਹੀਂਤਾਂ b = 255; }
      ਬਾਹਰਲਾ[0] = (stbi_uc)r;
      ਬਾਹਰਲਾ[1] = (stbi_uc)g;
      ਬਾਹਰਲਾ[2] = (stbi_uc)b;
      ਬਾਹਰਲਾ[3] = 255;
      ਬਾਹਰਲਾ += ਕਦਮ;
   }
}

#ਜੇ defined(STBI_SSE2) || defined(STBI_NEON)
ਸਥਿਰ ਖਾਲੀ stbi_YCb_Cr_ਮੰਜ਼ਿਲ_RGB_simd(stbi_uc *ਬਾਹਰਲਾ, stbi_uc ਅਚਲ *y, stbi_uc ਅਚਲ *pcb, stbi_uc ਅਚਲ *pcr, ਪੂਰਨ ਗਿਣਤੀ_2, ਪੂਰਨ ਕਦਮ)
{
   ਪੂਰਨ i = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_SSE2
   // step == 3 is pretty ugly on the final interleave, and i'm not convinced
   // it's useful in practice (you wouldn't use it for textures, for example).
   // so just accelerate step == 4 case.
   ਜੇ (ਕਦਮ == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      __m128i signflip  = _mm_set1_epi8(-0x80);
      __m128i cr_const0 = _mm_set1_epi16(   (ਛੋਟਾ) ( 1.40200f*4096.0f+0.5f));
      __m128i cr_const1 = _mm_set1_epi16( - (ਛੋਟਾ) ( 0.71414f*4096.0f+0.5f));
      __m128i cb_const0 = _mm_set1_epi16( - (ਛੋਟਾ) ( 0.34414f*4096.0f+0.5f));
      __m128i cb_const1 = _mm_set1_epi16(   (ਛੋਟਾ) ( 1.77200f*4096.0f+0.5f));
      __m128i y_bias = _mm_set1_epi8((ਅੱਖਰ) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) 128);
      __m128i xw = _mm_set1_epi16(255); // alpha channel

      ਲਈ (; i+7 < ਗਿਣਤੀ_2; i += 8) {
         //ਲੱਦੋ
         __m128i y_ਬਾਈਟਾਂ = _mm_loadl_epi64((__m128i *) (y+i));
         __m128i cr_ਬਾਈਟਾਂ = _mm_loadl_epi64((__m128i *) (pcr+i));
         __m128i cb_ਬਾਈਟਾਂ = _mm_loadl_epi64((__m128i *) (pcb+i));
         __m128i cr_biased = _mm_xor_si128(cr_ਬਾਈਟਾਂ, signflip); // -128
         __m128i cb_biased = _mm_xor_si128(cb_ਬਾਈਟਾਂ, signflip); // -128

         // unpack to short (and left-shift cr, cb by 8)
         __m128i yw  = _mm_unpacklo_epi8(y_bias, y_ਬਾਈਟਾਂ);
         __m128i crw = _mm_unpacklo_epi8(_mm_setzero_si128(), cr_biased);
         __m128i cbw = _mm_unpacklo_epi8(_mm_setzero_si128(), cb_biased);

         // color transform
         __m128i yws = _mm_srli_epi16(yw, 4);
         __m128i cr0 = _mm_mulhi_epi16(cr_const0, crw);
         __m128i cb0 = _mm_mulhi_epi16(cb_const0, cbw);
         __m128i cb1 = _mm_mulhi_epi16(cbw, cb_const1);
         __m128i cr1 = _mm_mulhi_epi16(crw, cr_const1);
         __m128i rws = mm_ਜੋੜੋ_epi16(cr0, yws);
         __m128i gwt = mm_ਜੋੜੋ_epi16(cb0, yws);
         __m128i bws = mm_ਜੋੜੋ_epi16(yws, cb1);
         __m128i gws = mm_ਜੋੜੋ_epi16(gwt, cr1);

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
         _mm_storeu_si128((__m128i *) (ਬਾਹਰਲਾ + 0), o0);
         _mm_storeu_si128((__m128i *) (ਬਾਹਰਲਾ + 16), o1);
         ਬਾਹਰਲਾ += 32;
      }
   }
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_NEON
   // in this version, step=3 support would be easy to add. but is there demand?
   ਜੇ (ਕਦਮ == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      uint8x8_t signflip = vdup_n_u8(0x80);
      int16x8_t cr_const0 = vdupq_n_s16(   (ਛੋਟਾ) ( 1.40200f*4096.0f+0.5f));
      int16x8_t cr_const1 = vdupq_n_s16( - (ਛੋਟਾ) ( 0.71414f*4096.0f+0.5f));
      int16x8_t cb_const0 = vdupq_n_s16( - (ਛੋਟਾ) ( 0.34414f*4096.0f+0.5f));
      int16x8_t cb_const1 = vdupq_n_s16(   (ਛੋਟਾ) ( 1.77200f*4096.0f+0.5f));

      ਲਈ (; i+7 < ਗਿਣਤੀ_2; i += 8) {
         //ਲੱਦੋ
         uint8x8_t y_ਬਾਈਟਾਂ  = vld1_u8(y + i);
         uint8x8_t cr_ਬਾਈਟਾਂ = vld1_u8(pcr + i);
         uint8x8_t cb_ਬਾਈਟਾਂ = vld1_u8(pcb + i);
         int8x8_t cr_biased = vreinterpret_s8_u8(vsub_u8(cr_ਬਾਈਟਾਂ, signflip));
         int8x8_t cb_biased = vreinterpret_s8_u8(vsub_u8(cb_ਬਾਈਟਾਂ, signflip));

         // expand to s16
         int16x8_t yws = vreinterpretq_s16_u16(vshll_n_u8(y_ਬਾਈਟਾਂ, 4));
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
         vst4_u8(ਬਾਹਰਲਾ, o);
         ਬਾਹਰਲਾ += 8*4;
      }
   }
#ਜੇ_ਅੰਤ

   ਲਈ (; i < ਗਿਣਤੀ_2; ++i) {
      ਪੂਰਨ y_fixed = (y[i] << 20) + (1<<19); // rounding
      ਪੂਰਨ r,g,b;
      ਪੂਰਨ cr = pcr[i] - 128;
      ਪੂਰਨ cb = pcb[i] - 128;
      r = y_fixed + cr* stbi__float2fixed(1.40200f);
      g = y_fixed + cr*-stbi__float2fixed(0.71414f) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                   +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) r > 255) { ਜੇ (r < 0) r = 0; ਨਹੀਂਤਾਂ r = 255; }
      ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) g > 255) { ਜੇ (g < 0) g = 0; ਨਹੀਂਤਾਂ g = 255; }
      ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) b > 255) { ਜੇ (b < 0) b = 0; ਨਹੀਂਤਾਂ b = 255; }
      ਬਾਹਰਲਾ[0] = (stbi_uc)r;
      ਬਾਹਰਲਾ[1] = (stbi_uc)g;
      ਬਾਹਰਲਾ[2] = (stbi_uc)b;
      ਬਾਹਰਲਾ[3] = 255;
      ਬਾਹਰਲਾ += ਕਦਮ;
   }
}
#ਜੇ_ਅੰਤ

// set up the kernels
ਸਥਿਰ ਖਾਲੀ stbi_ਤਿਆਰੀ_jpeg(stbi__jpeg *j)
{
   j->idct_ਖੰਡ_kernel = stbi_idct_ਖੰਡ;
   j->YCb_Cr_ਮੰਜ਼ਿਲ_RGB_kernel = stbi_YCb_Cr_ਮੰਜ਼ਿਲ_RGB_ਪੰਕਤੀ;
   j->resample_ਪੰਕਤੀ_hv_2_kernel = stbi_resample_ਪੰਕਤੀ_hv_2;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_SSE2
   ਜੇ (stbi_sse2_ਉਪਲਬਧ()) {
      j->idct_ਖੰਡ_kernel = stbi__idct_simd;
      j->YCb_Cr_ਮੰਜ਼ਿਲ_RGB_kernel = stbi_YCb_Cr_ਮੰਜ਼ਿਲ_RGB_simd;
      j->resample_ਪੰਕਤੀ_hv_2_kernel = stbi_resample_ਪੰਕਤੀ_hv_2_simd;
   }
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STBI_NEON
   j->idct_ਖੰਡ_kernel = stbi__idct_simd;
   j->YCb_Cr_ਮੰਜ਼ਿਲ_RGB_kernel = stbi_YCb_Cr_ਮੰਜ਼ਿਲ_RGB_simd;
   j->resample_ਪੰਕਤੀ_hv_2_kernel = stbi_resample_ਪੰਕਤੀ_hv_2_simd;
#ਜੇ_ਅੰਤ
}

// clean up the temporary component buffers
ਸਥਿਰ ਖਾਲੀ stbi__cleanup_jpeg(stbi__jpeg *j)
{
   stbi__free_jpeg_components(j, j->s->img_n, 0);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   resample_ਪੰਕਤੀ_func resample;
   stbi_uc *line0,*line1;
   ਪੂਰਨ hs,vs;   // expansion factor in each axis
   ਪੂਰਨ w_lores; // horizontal pixels pre-expansion
   ਪੂਰਨ ystep;   // how far through vertical expansion we are
   ਪੂਰਨ ypos;    // which pre-expansion row we're on
} stbi__resample;

// fast 0..255 * 0..255 => 0..255 rounded multiplication
ਸਥਿਰ stbi_uc stbi__blinn_8x8(stbi_uc x, stbi_uc y)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ t = x*y + 128;
   ਵਾਪਸ (stbi_uc) ((t + (t >>8)) >> 8);
}

ਸਥਿਰ stbi_uc *ਲੱਦੋ_jpeg_image(stbi__jpeg *z, ਪੂਰਨ *ਬਾਹਰਲਾ_x, ਪੂਰਨ *ਬਾਹਰਲਾ_y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   ਪੂਰਨ n, decode_n, is_rgb;
   z->s->img_n = 0; // make stbi__cleanup_jpeg safe

   // validate req_comp
   ਜੇ (req_comp < 0 || req_comp > 4) ਵਾਪਸ stbi__errpuc("bad req_comp", "Internal error");

   // load a jpeg image from whichever source, but leave in YCbCr format
   ਜੇ (!stbi__decode_jpeg_image(z)) { stbi__cleanup_jpeg(z); ਵਾਪਸ ਨਿਰਾ; }

   // determine actual number of components to generate
   n = req_comp ? req_comp : z->s->img_n >= 3 ? 3 : 1;

   is_rgb = z->s->img_n == 3 && (z->rgb == 3 || (z->app14_color_transform == 0 && !z->jfif));

   ਜੇ (z->s->img_n == 3 && n < 3 && !is_rgb)
      decode_n = 1;
   ਨਹੀਂਤਾਂ
      decode_n = z->s->img_n;

   // resample and color-convert
   {
      ਪੂਰਨ k;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i,j;
      stbi_uc *output;
      stbi_uc *coutput[4];

      stbi__resample res_comp[4];

      ਲਈ (k=0; k < decode_n; ++k) {
         stbi__resample *r = &res_comp[k];

         // allocate line buffer big enough for upsampling off the edges
         // with upsample factor of 4
         z->img_comp[k].linebuf = (stbi_uc *) stbi__malloc(z->s->img_x + 3);
         ਜੇ (!z->img_comp[k].linebuf) { stbi__cleanup_jpeg(z); ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory"); }

         r->hs      = z->img_h_max / z->img_comp[k].h;
         r->vs      = z->img_v_max / z->img_comp[k].v;
         r->ystep   = r->vs >> 1;
         r->w_lores = (z->s->img_x + r->hs-1) / r->hs;
         r->ypos    = 0;
         r->line0   = r->line1 = z->img_comp[k].ਅੰਕੜੇ;

         ਜੇ      (r->hs == 1 && r->vs == 1) r->resample = resample_ਪੰਕਤੀ_1;
         ਨਹੀਂਤਾਂ ਜੇ (r->hs == 1 && r->vs == 2) r->resample = stbi_resample_ਪੰਕਤੀ_v_2;
         ਨਹੀਂਤਾਂ ਜੇ (r->hs == 2 && r->vs == 1) r->resample = stbi_resample_ਪੰਕਤੀ_h_2;
         ਨਹੀਂਤਾਂ ਜੇ (r->hs == 2 && r->vs == 2) r->resample = z->resample_ਪੰਕਤੀ_hv_2_kernel;
         ਨਹੀਂਤਾਂ                               r->resample = stbi_resample_ਪੰਕਤੀ_generic;
      }

      // can't error after this so, this is safe
      output = (stbi_uc *) stbi__malloc_mad3(n, z->s->img_x, z->s->img_y, 1);
      ਜੇ (!output) { stbi__cleanup_jpeg(z); ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory"); }

      // now go ahead and resample
      ਲਈ (j=0; j < z->s->img_y; ++j) {
         stbi_uc *ਬਾਹਰਲਾ = output + n * z->s->img_x * j;
         ਲਈ (k=0; k < decode_n; ++k) {
            stbi__resample *r = &res_comp[k];
            ਪੂਰਨ y_bot = r->ystep >= (r->vs >> 1);
            coutput[k] = r->resample(z->img_comp[k].linebuf,
                                     y_bot ? r->line1 : r->line0,
                                     y_bot ? r->line0 : r->line1,
                                     r->w_lores, r->hs);
            ਜੇ (++r->ystep >= r->vs) {
               r->ystep = 0;
               r->line0 = r->line1;
               ਜੇ (++r->ypos < z->img_comp[k].y)
                  r->line1 += z->img_comp[k].w2;
            }
         }
         ਜੇ (n >= 3) {
            stbi_uc *y = coutput[0];
            ਜੇ (z->s->img_n == 3) {
               ਜੇ (is_rgb) {
                  ਲਈ (i=0; i < z->s->img_x; ++i) {
                     ਬਾਹਰਲਾ[0] = y[i];
                     ਬਾਹਰਲਾ[1] = coutput[1][i];
                     ਬਾਹਰਲਾ[2] = coutput[2][i];
                     ਬਾਹਰਲਾ[3] = 255;
                     ਬਾਹਰਲਾ += n;
                  }
               } ਨਹੀਂਤਾਂ {
                  z->YCb_Cr_ਮੰਜ਼ਿਲ_RGB_kernel(ਬਾਹਰਲਾ, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } ਨਹੀਂਤਾਂ ਜੇ (z->s->img_n == 4) {
               ਜੇ (z->app14_color_transform == 0) { // CMYK
                  ਲਈ (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     ਬਾਹਰਲਾ[0] = stbi__blinn_8x8(coutput[0][i], m);
                     ਬਾਹਰਲਾ[1] = stbi__blinn_8x8(coutput[1][i], m);
                     ਬਾਹਰਲਾ[2] = stbi__blinn_8x8(coutput[2][i], m);
                     ਬਾਹਰਲਾ[3] = 255;
                     ਬਾਹਰਲਾ += n;
                  }
               } ਨਹੀਂਤਾਂ ਜੇ (z->app14_color_transform == 2) { // YCCK
                  z->YCb_Cr_ਮੰਜ਼ਿਲ_RGB_kernel(ਬਾਹਰਲਾ, y, coutput[1], coutput[2], z->s->img_x, n);
                  ਲਈ (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     ਬਾਹਰਲਾ[0] = stbi__blinn_8x8(255 - ਬਾਹਰਲਾ[0], m);
                     ਬਾਹਰਲਾ[1] = stbi__blinn_8x8(255 - ਬਾਹਰਲਾ[1], m);
                     ਬਾਹਰਲਾ[2] = stbi__blinn_8x8(255 - ਬਾਹਰਲਾ[2], m);
                     ਬਾਹਰਲਾ += n;
                  }
               } ਨਹੀਂਤਾਂ { // YCbCr + alpha?  Ignore the fourth channel for now
                  z->YCb_Cr_ਮੰਜ਼ਿਲ_RGB_kernel(ਬਾਹਰਲਾ, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } ਨਹੀਂਤਾਂ
               ਲਈ (i=0; i < z->s->img_x; ++i) {
                  ਬਾਹਰਲਾ[0] = ਬਾਹਰਲਾ[1] = ਬਾਹਰਲਾ[2] = y[i];
                  ਬਾਹਰਲਾ[3] = 255; // not used if n==3
                  ਬਾਹਰਲਾ += n;
               }
         } ਨਹੀਂਤਾਂ {
            ਜੇ (is_rgb) {
               ਜੇ (n == 1)
                  ਲਈ (i=0; i < z->s->img_x; ++i)
                     *ਬਾਹਰਲਾ++ = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
               ਨਹੀਂਤਾਂ {
                  ਲਈ (i=0; i < z->s->img_x; ++i, ਬਾਹਰਲਾ += 2) {
                     ਬਾਹਰਲਾ[0] = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
                     ਬਾਹਰਲਾ[1] = 255;
                  }
               }
            } ਨਹੀਂਤਾਂ ਜੇ (z->s->img_n == 4 && z->app14_color_transform == 0) {
               ਲਈ (i=0; i < z->s->img_x; ++i) {
                  stbi_uc m = coutput[3][i];
                  stbi_uc r = stbi__blinn_8x8(coutput[0][i], m);
                  stbi_uc g = stbi__blinn_8x8(coutput[1][i], m);
                  stbi_uc b = stbi__blinn_8x8(coutput[2][i], m);
                  ਬਾਹਰਲਾ[0] = stbi__compute_y(r, g, b);
                  ਬਾਹਰਲਾ[1] = 255;
                  ਬਾਹਰਲਾ += n;
               }
            } ਨਹੀਂਤਾਂ ਜੇ (z->s->img_n == 4 && z->app14_color_transform == 2) {
               ਲਈ (i=0; i < z->s->img_x; ++i) {
                  ਬਾਹਰਲਾ[0] = stbi__blinn_8x8(255 - coutput[0][i], coutput[3][i]);
                  ਬਾਹਰਲਾ[1] = 255;
                  ਬਾਹਰਲਾ += n;
               }
            } ਨਹੀਂਤਾਂ {
               stbi_uc *y = coutput[0];
               ਜੇ (n == 1)
                  ਲਈ (i=0; i < z->s->img_x; ++i) ਬਾਹਰਲਾ[i] = y[i];
               ਨਹੀਂਤਾਂ
                  ਲਈ (i=0; i < z->s->img_x; ++i) *ਬਾਹਰਲਾ++ = y[i], *ਬਾਹਰਲਾ++ = 255;
            }
         }
      }
      stbi__cleanup_jpeg(z);
      *ਬਾਹਰਲਾ_x = z->s->img_x;
      *ਬਾਹਰਲਾ_y = z->s->img_y;
      ਜੇ (comp) *comp = z->s->img_n >= 3 ? 3 : 1; // report original components, not output
      ਵਾਪਸ output;
   }
}

ਸਥਿਰ ਖਾਲੀ *stbi_jpeg_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਨਤੀਜਾ;
   stbi__jpeg* j = (stbi__jpeg*) stbi__malloc(ਆਕਾਰ(stbi__jpeg));
   STBI_NOTUSED(ri);
   j->s = s;
   stbi_ਤਿਆਰੀ_jpeg(j);
   ਨਤੀਜਾ = ਲੱਦੋ_jpeg_image(j, x,y,comp,req_comp);
   STBI_FREE(j);
   ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ stbi__jpeg_test(stbi__context *s)
{
   ਪੂਰਨ r;
   stbi__jpeg* j = (stbi__jpeg*)stbi__malloc(ਆਕਾਰ(stbi__jpeg));
   j->s = s;
   stbi_ਤਿਆਰੀ_jpeg(j);
   r = stbi_decode_jpeg_ਸਿਰਲੇਖ(j, STBI_ਜਾਂਚ_type);
   stbi__rewind(s);
   STBI_FREE(j);
   ਵਾਪਸ r;
}

ਸਥਿਰ ਪੂਰਨ stbi__jpeg_info_raw(stbi__jpeg *j, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਜੇ (!stbi_decode_jpeg_ਸਿਰਲੇਖ(j, STBI_ਜਾਂਚ_ਸਿਰਲੇਖ)) {
      stbi__rewind( j->s );
      ਵਾਪਸ 0;
   }
   ਜੇ (x) *x = j->s->img_x;
   ਜੇ (y) *y = j->s->img_y;
   ਜੇ (comp) *comp = j->s->img_n >= 3 ? 3 : 1;
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__jpeg_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਪੂਰਨ ਨਤੀਜਾ;
   stbi__jpeg* j = (stbi__jpeg*) (stbi__malloc(ਆਕਾਰ(stbi__jpeg)));
   j->s = s;
   ਨਤੀਜਾ = stbi__jpeg_info_raw(j, x, y, comp);
   STBI_FREE(j);
   ਵਾਪਸ ਨਤੀਜਾ;
}
#ਜੇ_ਅੰਤ

// public domain zlib decode    v0.2  Sean Barrett 2006-11-18
//    simple implementation
//      - all input must be provided in an upfront buffer
//      - all output is written to a single output buffer (can malloc/realloc)
//    performance
//      - fast huffman

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_ZLIB

// fast-way is faster to check than jpeg huffman, but slow way is slower
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_ZFAST_ਬਿੱਟ  9 // accelerate all cases in default tables
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI_ZFAST_ਨਕਾਬ  ((1 << STBI_ZFAST_ਬਿੱਟ) - 1)

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi__uint16 fast[1 << STBI_ZFAST_ਬਿੱਟ];
   stbi__uint16 firstcode[16];
   ਪੂਰਨ maxcode[17];
   stbi__uint16 firstsymbol[16];
   stbi_uc  ਮਾਪ[288];
   stbi__uint16 ਮੁੱਲ[288];
} stbi__zhuffman;

stbi_inline ਸਥਿਰ ਪੂਰਨ stbi__bitreverse16(ਪੂਰਨ n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  ਵਾਪਸ n;
}

stbi_inline ਸਥਿਰ ਪੂਰਨ stbi_ਦੋਆਧਾਰੀ_ਅੰਕ_ਉਲਟ(ਪੂਰਨ v, ਪੂਰਨ ਬਿੱਟ)
{
   STBI_ASSERT(ਬਿੱਟ <= 16);
   // to bit reverse n bits, reverse 16 and shift
   // e.g. 11 bits, bit reverse and shift away 5
   ਵਾਪਸ stbi__bitreverse16(v) >> (16-ਬਿੱਟ);
}

ਸਥਿਰ ਪੂਰਨ stbi__zbuild_huffman(stbi__zhuffman *z, ਅਚਲ stbi_uc *sizelist, ਪੂਰਨ ਅੰਕ)
{
   ਪੂਰਨ i,k=0;
   ਪੂਰਨ ਕੋਡ, ਅਗਲਾ_ਕੋਡ[16], sizes[17];

   // DEFLATE spec for generating codes
   memset(sizes, 0, ਆਕਾਰ(sizes));
   memset(z->fast, 0, ਆਕਾਰ(z->fast));
   ਲਈ (i=0; i < ਅੰਕ; ++i)
      ++sizes[sizelist[i]];
   sizes[0] = 0;
   ਲਈ (i=1; i < 16; ++i)
      ਜੇ (sizes[i] > (1 << i))
         ਵਾਪਸ stbi__err("bad sizes", "Corrupt PNG");
   ਕੋਡ = 0;
   ਲਈ (i=1; i < 16; ++i) {
      ਅਗਲਾ_ਕੋਡ[i] = ਕੋਡ;
      z->firstcode[i] = (stbi__uint16) ਕੋਡ;
      z->firstsymbol[i] = (stbi__uint16) k;
      ਕੋਡ = (ਕੋਡ + sizes[i]);
      ਜੇ (sizes[i])
         ਜੇ (ਕੋਡ-1 >= (1 << i)) ਵਾਪਸ stbi__err("bad codelengths","Corrupt PNG");
      z->maxcode[i] = ਕੋਡ << (16-i); // preshift for inner loop
      ਕੋਡ <<= 1;
      k += sizes[i];
   }
   z->maxcode[16] = 0x10000; // sentinel
   ਲਈ (i=0; i < ਅੰਕ; ++i) {
      ਪੂਰਨ s = sizelist[i];
      ਜੇ (s) {
         ਪੂਰਨ c = ਅਗਲਾ_ਕੋਡ[s] - z->firstcode[s] + z->firstsymbol[s];
         stbi__uint16 fastv = (stbi__uint16) ((s << 9) | i);
         z->ਮਾਪ [c] = (stbi_uc     ) s;
         z->ਮੁੱਲ[c] = (stbi__uint16) i;
         ਜੇ (s <= STBI_ZFAST_ਬਿੱਟ) {
            ਪੂਰਨ j = stbi_ਦੋਆਧਾਰੀ_ਅੰਕ_ਉਲਟ(ਅਗਲਾ_ਕੋਡ[s],s);
            ਜਦੋਂ (j < (1 << STBI_ZFAST_ਬਿੱਟ)) {
               z->fast[j] = fastv;
               j += (1 << s);
            }
         }
         ++ਅਗਲਾ_ਕੋਡ[s];
      }
   }
   ਵਾਪਸ 1;
}

// zlib-from-memory implementation for PNG reading
//    because PNG allows splitting the zlib stream arbitrarily,
//    and it's annoying structurally to have PNG call ZLIB call PNG,
//    we require PNG read all the IDATs and combine them into a single
//    memory buffer

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi_uc *zbuffer, *zbuffer_ਸਿਰਾ;
   ਪੂਰਨ ਅੰਕ_ਬਿੱਟ;
   stbi__uint32 ਕੋਡ_ਬਫਰ;

   ਅੱਖਰ *zout;
   ਅੱਖਰ *zout_ਸ਼ੁਰੂ;
   ਅੱਖਰ *zout_ਸਿਰਾ;
   ਪੂਰਨ   z_expandable;

   stbi__zhuffman z_ਲੰਮਾਈ, z_ਦੂਰੀ;
} stbi__zbuf;

stbi_inline ਸਥਿਰ stbi_uc stbi__zget8(stbi__zbuf *z)
{
   ਜੇ (z->zbuffer >= z->zbuffer_ਸਿਰਾ) ਵਾਪਸ 0;
   ਵਾਪਸ *z->zbuffer++;
}

ਸਥਿਰ ਖਾਲੀ stbi_fill_ਬਿੱਟ(stbi__zbuf *z)
{
   ਕਰੋ {
      STBI_ASSERT(z->ਕੋਡ_ਬਫਰ < (1U << z->ਅੰਕ_ਬਿੱਟ));
      z->ਕੋਡ_ਬਫਰ |= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) stbi__zget8(z) << z->ਅੰਕ_ਬਿੱਟ;
      z->ਅੰਕ_ਬਿੱਟ += 8;
   } ਜਦੋਂ (z->ਅੰਕ_ਬਿੱਟ <= 24);
}

stbi_inline ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stbi__zreceive(stbi__zbuf *z, ਪੂਰਨ n)
{
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k;
   ਜੇ (z->ਅੰਕ_ਬਿੱਟ < n) stbi_fill_ਬਿੱਟ(z);
   k = z->ਕੋਡ_ਬਫਰ & ((1 << n) - 1);
   z->ਕੋਡ_ਬਫਰ >>= n;
   z->ਅੰਕ_ਬਿੱਟ -= n;
   ਵਾਪਸ k;
}

ਸਥਿਰ ਪੂਰਨ stbi__zhuffman_decode_slowpath(stbi__zbuf *a, stbi__zhuffman *z)
{
   ਪੂਰਨ b,s,k;
   // not resolved by fast table, so compute it the slow way
   // use jpeg approach, which requires MSbits at top
   k = stbi_ਦੋਆਧਾਰੀ_ਅੰਕ_ਉਲਟ(a->ਕੋਡ_ਬਫਰ, 16);
   ਲਈ (s=STBI_ZFAST_ਬਿੱਟ+1; ; ++s)
      ਜੇ (k < z->maxcode[s])
         ਰੋਕੋ;
   ਜੇ (s == 16) ਵਾਪਸ -1; // invalid code!
   // code size is s, so:
   b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
   STBI_ASSERT(z->ਮਾਪ[b] == s);
   a->ਕੋਡ_ਬਫਰ >>= s;
   a->ਅੰਕ_ਬਿੱਟ -= s;
   ਵਾਪਸ z->ਮੁੱਲ[b];
}

stbi_inline ਸਥਿਰ ਪੂਰਨ stbi__zhuffman_decode(stbi__zbuf *a, stbi__zhuffman *z)
{
   ਪੂਰਨ b,s;
   ਜੇ (a->ਅੰਕ_ਬਿੱਟ < 16) stbi_fill_ਬਿੱਟ(a);
   b = z->fast[a->ਕੋਡ_ਬਫਰ & STBI_ZFAST_ਨਕਾਬ];
   ਜੇ (b) {
      s = b >> 9;
      a->ਕੋਡ_ਬਫਰ >>= s;
      a->ਅੰਕ_ਬਿੱਟ -= s;
      ਵਾਪਸ b & 511;
   }
   ਵਾਪਸ stbi__zhuffman_decode_slowpath(a, z);
}

ਸਥਿਰ ਪੂਰਨ stbi__zexpand(stbi__zbuf *z, ਅੱਖਰ *zout, ਪੂਰਨ n)  // need to make room for n bytes
{
   ਅੱਖਰ *q;
   ਪੂਰਨ cur, ਹੱਦਬੰਦੀ, old_ਹੱਦਬੰਦੀ;
   z->zout = zout;
   ਜੇ (!z->z_expandable) ਵਾਪਸ stbi__err("output buffer limit","Corrupt PNG");
   cur   = (ਪੂਰਨ) (z->zout     - z->zout_ਸ਼ੁਰੂ);
   ਹੱਦਬੰਦੀ = old_ਹੱਦਬੰਦੀ = (ਪੂਰਨ) (z->zout_ਸਿਰਾ - z->zout_ਸ਼ੁਰੂ);
   ਜਦੋਂ (cur + n > ਹੱਦਬੰਦੀ)
      ਹੱਦਬੰਦੀ *= 2;
   q = (ਅੱਖਰ *) STBI_REALLOC_SIZED(z->zout_ਸ਼ੁਰੂ, old_ਹੱਦਬੰਦੀ, ਹੱਦਬੰਦੀ);
   STBI_NOTUSED(old_ਹੱਦਬੰਦੀ);
   ਜੇ (q == ਨਿਰਾ) ਵਾਪਸ stbi__err("outofmem", "Out of memory");
   z->zout_ਸ਼ੁਰੂ = q;
   z->zout       = q + cur;
   z->zout_ਸਿਰਾ   = q + ਹੱਦਬੰਦੀ;
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਅਚਲ ਪੂਰਨ stbi_zlength_ਅਧਾਰ[31] = {
   3,4,5,6,7,8,9,10,11,13,
   15,17,19,23,27,31,35,43,51,59,
   67,83,99,115,131,163,195,227,258,0,0 };

ਸਥਿਰ ਅਚਲ ਪੂਰਨ stbi__zlength_extra[31]=
{ 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };

ਸਥਿਰ ਅਚਲ ਪੂਰਨ stbi_zdist_ਅਧਾਰ[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0};

ਸਥਿਰ ਅਚਲ ਪੂਰਨ stbi__zdist_extra[32] =
{ 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

ਸਥਿਰ ਪੂਰਨ stbi_parse_huffman_ਖੰਡ(stbi__zbuf *a)
{
   ਅੱਖਰ *zout = a->zout;
   ਲਈ(;;) {
      ਪੂਰਨ z = stbi__zhuffman_decode(a, &a->z_ਲੰਮਾਈ);
      ਜੇ (z < 256) {
         ਜੇ (z < 0) ਵਾਪਸ stbi__err("bad huffman code","Corrupt PNG"); // error in huffman codes
         ਜੇ (zout >= a->zout_ਸਿਰਾ) {
            ਜੇ (!stbi__zexpand(a, zout, 1)) ਵਾਪਸ 0;
            zout = a->zout;
         }
         *zout++ = (ਅੱਖਰ) z;
      } ਨਹੀਂਤਾਂ {
         stbi_uc *p;
         ਪੂਰਨ len,dist;
         ਜੇ (z == 256) {
            a->zout = zout;
            ਵਾਪਸ 1;
         }
         z -= 257;
         len = stbi_zlength_ਅਧਾਰ[z];
         ਜੇ (stbi__zlength_extra[z]) len += stbi__zreceive(a, stbi__zlength_extra[z]);
         z = stbi__zhuffman_decode(a, &a->z_ਦੂਰੀ);
         ਜੇ (z < 0) ਵਾਪਸ stbi__err("bad huffman code","Corrupt PNG");
         dist = stbi_zdist_ਅਧਾਰ[z];
         ਜੇ (stbi__zdist_extra[z]) dist += stbi__zreceive(a, stbi__zdist_extra[z]);
         ਜੇ (zout - a->zout_ਸ਼ੁਰੂ < dist) ਵਾਪਸ stbi__err("bad dist","Corrupt PNG");
         ਜੇ (zout + len > a->zout_ਸਿਰਾ) {
            ਜੇ (!stbi__zexpand(a, zout, len)) ਵਾਪਸ 0;
            zout = a->zout;
         }
         p = (stbi_uc *) (zout - dist);
         ਜੇ (dist == 1) { // run of one byte; common in images.
            stbi_uc v = *p;
            ਜੇ (len) { ਕਰੋ *zout++ = v; ਜਦੋਂ (--len); }
         } ਨਹੀਂਤਾਂ {
            ਜੇ (len) { ਕਰੋ *zout++ = *p++; ਜਦੋਂ (--len); }
         }
      }
   }
}

ਸਥਿਰ ਪੂਰਨ stbi__compute_huffman_codes(stbi__zbuf *a)
{
   ਸਥਿਰ ਅਚਲ stbi_uc ਲੰਮਾਈ_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
   stbi__zhuffman z_codelength;
   stbi_uc lencodes[286+32+137];//padding for maximum single op
   stbi_uc codelength_sizes[19];
   ਪੂਰਨ i,n;

   ਪੂਰਨ hlit  = stbi__zreceive(a,5) + 257;
   ਪੂਰਨ hdist = stbi__zreceive(a,5) + 1;
   ਪੂਰਨ hclen = stbi__zreceive(a,4) + 4;
   ਪੂਰਨ ntot  = hlit + hdist;

   memset(codelength_sizes, 0, ਆਕਾਰ(codelength_sizes));
   ਲਈ (i=0; i < hclen; ++i) {
      ਪੂਰਨ s = stbi__zreceive(a,3);
      codelength_sizes[ਲੰਮਾਈ_dezigzag[i]] = (stbi_uc) s;
   }
   ਜੇ (!stbi__zbuild_huffman(&z_codelength, codelength_sizes, 19)) ਵਾਪਸ 0;

   n = 0;
   ਜਦੋਂ (n < ntot) {
      ਪੂਰਨ c = stbi__zhuffman_decode(a, &z_codelength);
      ਜੇ (c < 0 || c >= 19) ਵਾਪਸ stbi__err("bad codelengths", "Corrupt PNG");
      ਜੇ (c < 16)
         lencodes[n++] = (stbi_uc) c;
      ਨਹੀਂਤਾਂ {
         stbi_uc fill = 0;
         ਜੇ (c == 16) {
            c = stbi__zreceive(a,2)+3;
            ਜੇ (n == 0) ਵਾਪਸ stbi__err("bad codelengths", "Corrupt PNG");
            fill = lencodes[n-1];
         } ਨਹੀਂਤਾਂ ਜੇ (c == 17)
            c = stbi__zreceive(a,3)+3;
         ਨਹੀਂਤਾਂ {
            STBI_ASSERT(c == 18);
            c = stbi__zreceive(a,7)+11;
         }
         ਜੇ (ntot - n < c) ਵਾਪਸ stbi__err("bad codelengths", "Corrupt PNG");
         memset(lencodes+n, fill, c);
         n += c;
      }
   }
   ਜੇ (n != ntot) ਵਾਪਸ stbi__err("bad codelengths","Corrupt PNG");
   ਜੇ (!stbi__zbuild_huffman(&a->z_ਲੰਮਾਈ, lencodes, hlit)) ਵਾਪਸ 0;
   ਜੇ (!stbi__zbuild_huffman(&a->z_ਦੂਰੀ, lencodes+hlit, hdist)) ਵਾਪਸ 0;
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi_parse_uncompressed_ਖੰਡ(stbi__zbuf *a)
{
   stbi_uc ਸਿਰਲੇਖ[4];
   ਪੂਰਨ len,nlen,k;
   ਜੇ (a->ਅੰਕ_ਬਿੱਟ & 7)
      stbi__zreceive(a, a->ਅੰਕ_ਬਿੱਟ & 7); // discard
   // drain the bit-packed data into header
   k = 0;
   ਜਦੋਂ (a->ਅੰਕ_ਬਿੱਟ > 0) {
      ਸਿਰਲੇਖ[k++] = (stbi_uc) (a->ਕੋਡ_ਬਫਰ & 255); // suppress MSVC run-time check
      a->ਕੋਡ_ਬਫਰ >>= 8;
      a->ਅੰਕ_ਬਿੱਟ -= 8;
   }
   STBI_ASSERT(a->ਅੰਕ_ਬਿੱਟ == 0);
   // now fill header the normal way
   ਜਦੋਂ (k < 4)
      ਸਿਰਲੇਖ[k++] = stbi__zget8(a);
   len  = ਸਿਰਲੇਖ[1] * 256 + ਸਿਰਲੇਖ[0];
   nlen = ਸਿਰਲੇਖ[3] * 256 + ਸਿਰਲੇਖ[2];
   ਜੇ (nlen != (len ^ 0xffff)) ਵਾਪਸ stbi__err("zlib corrupt","Corrupt PNG");
   ਜੇ (a->zbuffer + len > a->zbuffer_ਸਿਰਾ) ਵਾਪਸ stbi__err("read past buffer","Corrupt PNG");
   ਜੇ (a->zout + len > a->zout_ਸਿਰਾ)
      ਜੇ (!stbi__zexpand(a, a->zout, len)) ਵਾਪਸ 0;
   ਸਮ੍ਰਿਤੀ_ਨਕਲ(a->zout, a->zbuffer, len);
   a->zbuffer += len;
   a->zout += len;
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi_parse_zlib_ਸਿਰਲੇਖ(stbi__zbuf *a)
{
   ਪੂਰਨ cmf   = stbi__zget8(a);
   ਪੂਰਨ cm    = cmf & 15;
   /* int cinfo = cmf >> 4; */
   ਪੂਰਨ flg   = stbi__zget8(a);
   ਜੇ ((cmf*256+flg) % 31 != 0) ਵਾਪਸ stbi__err("bad zlib header","Corrupt PNG"); // zlib spec
   ਜੇ (flg & 32) ਵਾਪਸ stbi__err("no preset dict","Corrupt PNG"); // preset dictionary not allowed in png
   ਜੇ (cm != 8) ਵਾਪਸ stbi__err("bad compression","Corrupt PNG"); // DEFLATE required for png
   // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਅਚਲ stbi_uc stbi_zdefault_ਲੰਮਾਈ[288] =
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
ਸਥਿਰ ਅਚਲ stbi_uc stbi_zdefault_ਦੂਰੀ[32] =
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

ਸਥਿਰ ਪੂਰਨ stbi__parse_zlib(stbi__zbuf *a, ਪੂਰਨ parse_ਸਿਰਲੇਖ)
{
   ਪੂਰਨ final, type;
   ਜੇ (parse_ਸਿਰਲੇਖ)
      ਜੇ (!stbi_parse_zlib_ਸਿਰਲੇਖ(a)) ਵਾਪਸ 0;
   a->ਅੰਕ_ਬਿੱਟ = 0;
   a->ਕੋਡ_ਬਫਰ = 0;
   ਕਰੋ {
      final = stbi__zreceive(a,1);
      type = stbi__zreceive(a,2);
      ਜੇ (type == 0) {
         ਜੇ (!stbi_parse_uncompressed_ਖੰਡ(a)) ਵਾਪਸ 0;
      } ਨਹੀਂਤਾਂ ਜੇ (type == 3) {
         ਵਾਪਸ 0;
      } ਨਹੀਂਤਾਂ {
         ਜੇ (type == 1) {
            // use fixed code lengths
            ਜੇ (!stbi__zbuild_huffman(&a->z_ਲੰਮਾਈ  , stbi_zdefault_ਲੰਮਾਈ  , 288)) ਵਾਪਸ 0;
            ਜੇ (!stbi__zbuild_huffman(&a->z_ਦੂਰੀ, stbi_zdefault_ਦੂਰੀ,  32)) ਵਾਪਸ 0;
         } ਨਹੀਂਤਾਂ {
            ਜੇ (!stbi__compute_huffman_codes(a)) ਵਾਪਸ 0;
         }
         ਜੇ (!stbi_parse_huffman_ਖੰਡ(a)) ਵਾਪਸ 0;
      }
   } ਜਦੋਂ (!final);
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__do_zlib(stbi__zbuf *a, ਅੱਖਰ *obuf, ਪੂਰਨ olen, ਪੂਰਨ ਘਾਤਾਂਕ, ਪੂਰਨ parse_ਸਿਰਲੇਖ)
{
   a->zout_ਸ਼ੁਰੂ = obuf;
   a->zout       = obuf;
   a->zout_ਸਿਰਾ   = obuf + olen;
   a->z_expandable = ਘਾਤਾਂਕ;

   ਵਾਪਸ stbi__parse_zlib(a, parse_ਸਿਰਲੇਖ);
}

STBIDEF ਅੱਖਰ *stbi_zlib_decode_malloc_guesssize(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ initial_ਮਾਪ, ਪੂਰਨ *outlen)
{
   stbi__zbuf a;
   ਅੱਖਰ *p = (ਅੱਖਰ *) stbi__malloc(initial_ਮਾਪ);
   ਜੇ (p == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;
   a.zbuffer = (stbi_uc *) ਬਫਰ;
   a.zbuffer_ਸਿਰਾ = (stbi_uc *) ਬਫਰ + len;
   ਜੇ (stbi__do_zlib(&a, p, initial_ਮਾਪ, 1, 1)) {
      ਜੇ (outlen) *outlen = (ਪੂਰਨ) (a.zout - a.zout_ਸ਼ੁਰੂ);
      ਵਾਪਸ a.zout_ਸ਼ੁਰੂ;
   } ਨਹੀਂਤਾਂ {
      STBI_FREE(a.zout_ਸ਼ੁਰੂ);
      ਵਾਪਸ ਨਿਰਾ;
   }
}

STBIDEF ਅੱਖਰ *stbi_zlib_decode_malloc(ਅੱਖਰ ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *outlen)
{
   ਵਾਪਸ stbi_zlib_decode_malloc_guesssize(ਬਫਰ, len, 16384, outlen);
}

STBIDEF ਅੱਖਰ *stbi_zlib_decode_malloc_guesssize_headerflag(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ initial_ਮਾਪ, ਪੂਰਨ *outlen, ਪੂਰਨ parse_ਸਿਰਲੇਖ)
{
   stbi__zbuf a;
   ਅੱਖਰ *p = (ਅੱਖਰ *) stbi__malloc(initial_ਮਾਪ);
   ਜੇ (p == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;
   a.zbuffer = (stbi_uc *) ਬਫਰ;
   a.zbuffer_ਸਿਰਾ = (stbi_uc *) ਬਫਰ + len;
   ਜੇ (stbi__do_zlib(&a, p, initial_ਮਾਪ, 1, parse_ਸਿਰਲੇਖ)) {
      ਜੇ (outlen) *outlen = (ਪੂਰਨ) (a.zout - a.zout_ਸ਼ੁਰੂ);
      ਵਾਪਸ a.zout_ਸ਼ੁਰੂ;
   } ਨਹੀਂਤਾਂ {
      STBI_FREE(a.zout_ਸ਼ੁਰੂ);
      ਵਾਪਸ ਨਿਰਾ;
   }
}

STBIDEF ਪੂਰਨ stbi_zlib_decode_ਬਫਰ(ਅੱਖਰ *obuffer, ਪੂਰਨ olen, ਅੱਖਰ ਅਚਲ *ibuffer, ਪੂਰਨ ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_ਸਿਰਾ = (stbi_uc *) ibuffer + ilen;
   ਜੇ (stbi__do_zlib(&a, obuffer, olen, 0, 1))
      ਵਾਪਸ (ਪੂਰਨ) (a.zout - a.zout_ਸ਼ੁਰੂ);
   ਨਹੀਂਤਾਂ
      ਵਾਪਸ -1;
}

STBIDEF ਅੱਖਰ *stbi_zlib_decode_noheader_malloc(ਅੱਖਰ ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *outlen)
{
   stbi__zbuf a;
   ਅੱਖਰ *p = (ਅੱਖਰ *) stbi__malloc(16384);
   ਜੇ (p == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;
   a.zbuffer = (stbi_uc *) ਬਫਰ;
   a.zbuffer_ਸਿਰਾ = (stbi_uc *) ਬਫਰ+len;
   ਜੇ (stbi__do_zlib(&a, p, 16384, 1, 0)) {
      ਜੇ (outlen) *outlen = (ਪੂਰਨ) (a.zout - a.zout_ਸ਼ੁਰੂ);
      ਵਾਪਸ a.zout_ਸ਼ੁਰੂ;
   } ਨਹੀਂਤਾਂ {
      STBI_FREE(a.zout_ਸ਼ੁਰੂ);
      ਵਾਪਸ ਨਿਰਾ;
   }
}

STBIDEF ਪੂਰਨ stbi_zlib_decode_noheader_ਬਫਰ(ਅੱਖਰ *obuffer, ਪੂਰਨ olen, ਅਚਲ ਅੱਖਰ *ibuffer, ਪੂਰਨ ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_ਸਿਰਾ = (stbi_uc *) ibuffer + ilen;
   ਜੇ (stbi__do_zlib(&a, obuffer, olen, 0, 0))
      ਵਾਪਸ (ਪੂਰਨ) (a.zout - a.zout_ਸ਼ੁਰੂ);
   ਨਹੀਂਤਾਂ
      ਵਾਪਸ -1;
}
#ਜੇ_ਅੰਤ

// public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
//    simple implementation
//      - only 8-bit samples
//      - no CRC checking
//      - allocates lots of intermediate memory
//        - avoids problem of streaming data between subsystems
//        - avoids explicit window management
//    performance
//      - uses stb_zlib, a PD zlib implementation with fast huffman decoding

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNG
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi__uint32 ਲੰਮਾਈ;
   stbi__uint32 type;
} stbi__pngchunk;

ਸਥਿਰ stbi__pngchunk stbi_get_chunk_ਸਿਰਲੇਖ(stbi__context *s)
{
   stbi__pngchunk c;
   c.ਲੰਮਾਈ = stbi__get32be(s);
   c.type   = stbi__get32be(s);
   ਵਾਪਸ c;
}

ਸਥਿਰ ਪੂਰਨ stbi_check_png_ਸਿਰਲੇਖ(stbi__context *s)
{
   ਸਥਿਰ ਅਚਲ stbi_uc png_sig[8] = { 137,80,78,71,13,10,26,10 };
   ਪੂਰਨ i;
   ਲਈ (i=0; i < 8; ++i)
      ਜੇ (stbi__get8(s) != png_sig[i]) ਵਾਪਸ stbi__err("bad png sig","Not a PNG");
   ਵਾਪਸ 1;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi__context *s;
   stbi_uc *idata, *expanded, *ਬਾਹਰਲਾ;
   ਪੂਰਨ ਡੂੰਘਾਈ;
} stbi__png;


ਗਿਣਤੀ {
   STBI__F_none=0,
   STBI__F_sub=1,
   STBI_F_ਉੱਪਰ=2,
   STBI__F_avg=3,
   STBI__F_paeth=4,
   // synthetic filters used for first scanline to avoid needing a dummy row of 0s
   STBI_F_avg_ਪਹਿਲਾ,
   STBI_F_paeth_ਪਹਿਲਾ
};

ਸਥਿਰ stbi_uc ਪਹਿਲਾ_ਪੰਕਤੀ_filter[5] =
{
   STBI__F_none,
   STBI__F_sub,
   STBI__F_none,
   STBI_F_avg_ਪਹਿਲਾ,
   STBI_F_paeth_ਪਹਿਲਾ
};

ਸਥਿਰ ਪੂਰਨ stbi__paeth(ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ c)
{
   ਪੂਰਨ p = a + b - c;
   ਪੂਰਨ pa = ਨਿਰਪੇਖ(p-a);
   ਪੂਰਨ pb = ਨਿਰਪੇਖ(p-b);
   ਪੂਰਨ pc = ਨਿਰਪੇਖ(p-c);
   ਜੇ (pa <= pb && pa <= pc) ਵਾਪਸ a;
   ਜੇ (pb <= pc) ਵਾਪਸ b;
   ਵਾਪਸ c;
}

ਸਥਿਰ ਅਚਲ stbi_uc stbi_ਡੂੰਘਾਈ_scale_ਸਾਰਣੀ[9] = { 0, 0xff, 0x55, 0, 0x11, 0,0,0, 0x01 };

// create the png data from post-deflated data
ਸਥਿਰ ਪੂਰਨ stbi__create_png_image_raw(stbi__png *a, stbi_uc *raw, stbi__uint32 raw_len, ਪੂਰਨ ਬਾਹਰਲਾ_n, stbi__uint32 x, stbi__uint32 y, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ color)
{
   ਪੂਰਨ ਬਾਈਟਾਂ = (ਡੂੰਘਾਈ == 16? 2 : 1);
   stbi__context *s = a->s;
   stbi__uint32 i,j,stride = x*ਬਾਹਰਲਾ_n*ਬਾਈਟਾਂ;
   stbi__uint32 img_len, img_ਚੌੜਾਈ_ਬਾਈਟਾਂ;
   ਪੂਰਨ k;
   ਪੂਰਨ img_n = s->img_n; // copy it into a local for later

   ਪੂਰਨ output_ਬਾਈਟਾਂ = ਬਾਹਰਲਾ_n*ਬਾਈਟਾਂ;
   ਪੂਰਨ filter_ਬਾਈਟਾਂ = img_n*ਬਾਈਟਾਂ;
   ਪੂਰਨ ਚੌੜਾਈ = x;

   STBI_ASSERT(ਬਾਹਰਲਾ_n == s->img_n || ਬਾਹਰਲਾ_n == s->img_n+1);
   a->ਬਾਹਰਲਾ = (stbi_uc *) stbi__malloc_mad3(x, y, output_ਬਾਈਟਾਂ, 0); // extra bytes to write off the end into
   ਜੇ (!a->ਬਾਹਰਲਾ) ਵਾਪਸ stbi__err("outofmem", "Out of memory");

   ਜੇ (!stbi__mad3sizes_valid(img_n, x, ਡੂੰਘਾਈ, 7)) ਵਾਪਸ stbi__err("too large", "Corrupt PNG");
   img_ਚੌੜਾਈ_ਬਾਈਟਾਂ = (((img_n * x * ਡੂੰਘਾਈ) + 7) >> 3);
   img_len = (img_ਚੌੜਾਈ_ਬਾਈਟਾਂ + 1) * y;

   // we used to check for exact match between raw_len and img_len on non-interlaced PNGs,
   // but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),
   // so just check for raw_len < img_len always.
   ਜੇ (raw_len < img_len) ਵਾਪਸ stbi__err("not enough pixels","Corrupt PNG");

   ਲਈ (j=0; j < y; ++j) {
      stbi_uc *cur = a->ਬਾਹਰਲਾ + stride*j;
      stbi_uc *ਪੂਰਵ;
      ਪੂਰਨ filter = *raw++;

      ਜੇ (filter > 4)
         ਵਾਪਸ stbi__err("invalid filter","Corrupt PNG");

      ਜੇ (ਡੂੰਘਾਈ < 8) {
         STBI_ASSERT(img_ਚੌੜਾਈ_ਬਾਈਟਾਂ <= x);
         cur += x*ਬਾਹਰਲਾ_n - img_ਚੌੜਾਈ_ਬਾਈਟਾਂ; // store output to the rightmost img_len bytes, so we can decode in place
         filter_ਬਾਈਟਾਂ = 1;
         ਚੌੜਾਈ = img_ਚੌੜਾਈ_ਬਾਈਟਾਂ;
      }
      ਪੂਰਵ = cur - stride; // bugfix: need to compute this after 'cur +=' computation above

      // if first row, use special filter that doesn't sample previous row
      ਜੇ (j == 0) filter = ਪਹਿਲਾ_ਪੰਕਤੀ_filter[filter];

      // handle first byte explicitly
      ਲਈ (k=0; k < filter_ਬਾਈਟਾਂ; ++k) {
         ਚੋਣ (filter) {
            ਹਾਲਤ STBI__F_none       : cur[k] = raw[k]; ਰੋਕੋ;
            ਹਾਲਤ STBI__F_sub        : cur[k] = raw[k]; ਰੋਕੋ;
            ਹਾਲਤ STBI_F_ਉੱਪਰ         : cur[k] = STBI__BYTECAST(raw[k] + ਪੂਰਵ[k]); ਰੋਕੋ;
            ਹਾਲਤ STBI__F_avg        : cur[k] = STBI__BYTECAST(raw[k] + (ਪੂਰਵ[k]>>1)); ਰੋਕੋ;
            ਹਾਲਤ STBI__F_paeth      : cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(0,ਪੂਰਵ[k],0)); ਰੋਕੋ;
            ਹਾਲਤ STBI_F_avg_ਪਹਿਲਾ  : cur[k] = raw[k]; ਰੋਕੋ;
            ਹਾਲਤ STBI_F_paeth_ਪਹਿਲਾ: cur[k] = raw[k]; ਰੋਕੋ;
         }
      }

      ਜੇ (ਡੂੰਘਾਈ == 8) {
         ਜੇ (img_n != ਬਾਹਰਲਾ_n)
            cur[img_n] = 255; // first pixel
         raw += img_n;
         cur += ਬਾਹਰਲਾ_n;
         ਪੂਰਵ += ਬਾਹਰਲਾ_n;
      } ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == 16) {
         ਜੇ (img_n != ਬਾਹਰਲਾ_n) {
            cur[filter_ਬਾਈਟਾਂ]   = 255; // first pixel top byte
            cur[filter_ਬਾਈਟਾਂ+1] = 255; // first pixel bottom byte
         }
         raw += filter_ਬਾਈਟਾਂ;
         cur += output_ਬਾਈਟਾਂ;
         ਪੂਰਵ += output_ਬਾਈਟਾਂ;
      } ਨਹੀਂਤਾਂ {
         raw += 1;
         cur += 1;
         ਪੂਰਵ += 1;
      }

      // this is a little gross, so that we don't switch per-pixel or per-component
      ਜੇ (ਡੂੰਘਾਈ < 8 || img_n == ਬਾਹਰਲਾ_n) {
         ਪੂਰਨ nk = (ਚੌੜਾਈ - 1)*filter_ਬਾਈਟਾਂ;
         #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__CASE(f) \
             ਹਾਲਤ f:     \
                ਲਈ (k=0; k < nk; ++k)
         ਚੋਣ (filter) {
            // "none" filter turns into a memcpy here; make that explicit.
            ਹਾਲਤ STBI__F_none:         ਸਮ੍ਰਿਤੀ_ਨਕਲ(cur, raw, nk); ਰੋਕੋ;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k-filter_ਬਾਈਟਾਂ]); } ਰੋਕੋ;
            STBI__CASE(STBI_F_ਉੱਪਰ)           { cur[k] = STBI__BYTECAST(raw[k] + ਪੂਰਵ[k]); } ਰੋਕੋ;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((ਪੂਰਵ[k] + cur[k-filter_ਬਾਈਟਾਂ])>>1)); } ਰੋਕੋ;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_ਬਾਈਟਾਂ],ਪੂਰਵ[k],ਪੂਰਵ[k-filter_ਬਾਈਟਾਂ])); } ਰੋਕੋ;
            STBI__CASE(STBI_F_avg_ਪਹਿਲਾ)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k-filter_ਬਾਈਟਾਂ] >> 1)); } ਰੋਕੋ;
            STBI__CASE(STBI_F_paeth_ਪਹਿਲਾ)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_ਬਾਈਟਾਂ],0,0)); } ਰੋਕੋ;
         }
         #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ STBI__CASE
         raw += nk;
      } ਨਹੀਂਤਾਂ {
         STBI_ASSERT(img_n+1 == ਬਾਹਰਲਾ_n);
         #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__CASE(f) \
             ਹਾਲਤ f:     \
                ਲਈ (i=x-1; i >= 1; --i, cur[filter_ਬਾਈਟਾਂ]=255,raw+=filter_ਬਾਈਟਾਂ,cur+=output_ਬਾਈਟਾਂ,ਪੂਰਵ+=output_ਬਾਈਟਾਂ) \
                   ਲਈ (k=0; k < filter_ਬਾਈਟਾਂ; ++k)
         ਚੋਣ (filter) {
            STBI__CASE(STBI__F_none)         { cur[k] = raw[k]; } ਰੋਕੋ;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k- output_ਬਾਈਟਾਂ]); } ਰੋਕੋ;
            STBI__CASE(STBI_F_ਉੱਪਰ)           { cur[k] = STBI__BYTECAST(raw[k] + ਪੂਰਵ[k]); } ਰੋਕੋ;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((ਪੂਰਵ[k] + cur[k- output_ਬਾਈਟਾਂ])>>1)); } ਰੋਕੋ;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_ਬਾਈਟਾਂ],ਪੂਰਵ[k],ਪੂਰਵ[k- output_ਬਾਈਟਾਂ])); } ਰੋਕੋ;
            STBI__CASE(STBI_F_avg_ਪਹਿਲਾ)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k- output_ਬਾਈਟਾਂ] >> 1)); } ਰੋਕੋ;
            STBI__CASE(STBI_F_paeth_ਪਹਿਲਾ)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_ਬਾਈਟਾਂ],0,0)); } ਰੋਕੋ;
         }
         #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ STBI__CASE

         // the loop above sets the high byte of the pixels' alpha, but for
         // 16 bit png files we also need the low byte set. we'll do that here.
         ਜੇ (ਡੂੰਘਾਈ == 16) {
            cur = a->ਬਾਹਰਲਾ + stride*j; // start at the beginning of the row again
            ਲਈ (i=0; i < x; ++i,cur+=output_ਬਾਈਟਾਂ) {
               cur[filter_ਬਾਈਟਾਂ+1] = 255;
            }
         }
      }
   }

   // we make a separate pass to expand bits to pixels; for performance,
   // this could run two scanlines behind the above code, so it won't
   // intefere with filtering but will still be in the cache.
   ਜੇ (ਡੂੰਘਾਈ < 8) {
      ਲਈ (j=0; j < y; ++j) {
         stbi_uc *cur = a->ਬਾਹਰਲਾ + stride*j;
         stbi_uc *ਵਿੱਚ  = a->ਬਾਹਰਲਾ + stride*j + x*ਬਾਹਰਲਾ_n - img_ਚੌੜਾਈ_ਬਾਈਟਾਂ;
         // unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit
         // png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop
         stbi_uc scale = (color == 0) ? stbi_ਡੂੰਘਾਈ_scale_ਸਾਰਣੀ[ਡੂੰਘਾਈ] : 1; // scale grayscale values to 0..255 range

         // note that the final byte might overshoot and write more data than desired.
         // we can allocate enough data that this never writes out of memory, but it
         // could also overwrite the next scanline. can it overwrite non-empty data
         // on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.
         // so we need to explicitly clamp the final ones

         ਜੇ (ਡੂੰਘਾਈ == 4) {
            ਲਈ (k=x*img_n; k >= 2; k-=2, ++ਵਿੱਚ) {
               *cur++ = scale * ((*ਵਿੱਚ >> 4)       );
               *cur++ = scale * ((*ਵਿੱਚ     ) & 0x0f);
            }
            ਜੇ (k > 0) *cur++ = scale * ((*ਵਿੱਚ >> 4)       );
         } ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == 2) {
            ਲਈ (k=x*img_n; k >= 4; k-=4, ++ਵਿੱਚ) {
               *cur++ = scale * ((*ਵਿੱਚ >> 6)       );
               *cur++ = scale * ((*ਵਿੱਚ >> 4) & 0x03);
               *cur++ = scale * ((*ਵਿੱਚ >> 2) & 0x03);
               *cur++ = scale * ((*ਵਿੱਚ     ) & 0x03);
            }
            ਜੇ (k > 0) *cur++ = scale * ((*ਵਿੱਚ >> 6)       );
            ਜੇ (k > 1) *cur++ = scale * ((*ਵਿੱਚ >> 4) & 0x03);
            ਜੇ (k > 2) *cur++ = scale * ((*ਵਿੱਚ >> 2) & 0x03);
         } ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == 1) {
            ਲਈ (k=x*img_n; k >= 8; k-=8, ++ਵਿੱਚ) {
               *cur++ = scale * ((*ਵਿੱਚ >> 7)       );
               *cur++ = scale * ((*ਵਿੱਚ >> 6) & 0x01);
               *cur++ = scale * ((*ਵਿੱਚ >> 5) & 0x01);
               *cur++ = scale * ((*ਵਿੱਚ >> 4) & 0x01);
               *cur++ = scale * ((*ਵਿੱਚ >> 3) & 0x01);
               *cur++ = scale * ((*ਵਿੱਚ >> 2) & 0x01);
               *cur++ = scale * ((*ਵਿੱਚ >> 1) & 0x01);
               *cur++ = scale * ((*ਵਿੱਚ     ) & 0x01);
            }
            ਜੇ (k > 0) *cur++ = scale * ((*ਵਿੱਚ >> 7)       );
            ਜੇ (k > 1) *cur++ = scale * ((*ਵਿੱਚ >> 6) & 0x01);
            ਜੇ (k > 2) *cur++ = scale * ((*ਵਿੱਚ >> 5) & 0x01);
            ਜੇ (k > 3) *cur++ = scale * ((*ਵਿੱਚ >> 4) & 0x01);
            ਜੇ (k > 4) *cur++ = scale * ((*ਵਿੱਚ >> 3) & 0x01);
            ਜੇ (k > 5) *cur++ = scale * ((*ਵਿੱਚ >> 2) & 0x01);
            ਜੇ (k > 6) *cur++ = scale * ((*ਵਿੱਚ >> 1) & 0x01);
         }
         ਜੇ (img_n != ਬਾਹਰਲਾ_n) {
            ਪੂਰਨ q;
            // insert alpha = 255
            cur = a->ਬਾਹਰਲਾ + stride*j;
            ਜੇ (img_n == 1) {
               ਲਈ (q=x-1; q >= 0; --q) {
                  cur[q*2+1] = 255;
                  cur[q*2+0] = cur[q];
               }
            } ਨਹੀਂਤਾਂ {
               STBI_ASSERT(img_n == 3);
               ਲਈ (q=x-1; q >= 0; --q) {
                  cur[q*4+3] = 255;
                  cur[q*4+2] = cur[q*3+2];
                  cur[q*4+1] = cur[q*3+1];
                  cur[q*4+0] = cur[q*3+0];
               }
            }
         }
      }
   } ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == 16) {
      // force the image data from big-endian to platform-native.
      // this is done in a separate pass due to the decoding relying
      // on the data being untouched, but could probably be done
      // per-line during decode if care is taken.
      stbi_uc *cur = a->ਬਾਹਰਲਾ;
      stbi__uint16 *cur16 = (stbi__uint16*)cur;

      ਲਈ(i=0; i < x*y*ਬਾਹਰਲਾ_n; ++i,cur16++,cur+=2) {
         *cur16 = (cur[0] << 8) | cur[1];
      }
   }

   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__create_png_image(stbi__png *a, stbi_uc *image_ਅੰਕੜੇ_2, stbi__uint32 image_ਅੰਕੜੇ_len, ਪੂਰਨ ਬਾਹਰਲਾ_n, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ color, ਪੂਰਨ interlaced)
{
   ਪੂਰਨ ਬਾਈਟਾਂ = (ਡੂੰਘਾਈ == 16 ? 2 : 1);
   ਪੂਰਨ ਬਾਹਰਲਾ_ਬਾਈਟਾਂ = ਬਾਹਰਲਾ_n * ਬਾਈਟਾਂ;
   stbi_uc *final;
   ਪੂਰਨ p;
   ਜੇ (!interlaced)
      ਵਾਪਸ stbi__create_png_image_raw(a, image_ਅੰਕੜੇ_2, image_ਅੰਕੜੇ_len, ਬਾਹਰਲਾ_n, a->s->img_x, a->s->img_y, ਡੂੰਘਾਈ, color);

   // de-interlacing
   final = (stbi_uc *) stbi__malloc_mad3(a->s->img_x, a->s->img_y, ਬਾਹਰਲਾ_ਬਾਈਟਾਂ, 0);
   ਲਈ (p=0; p < 7; ++p) {
      ਪੂਰਨ xorig[] = { 0,4,0,2,0,1,0 };
      ਪੂਰਨ yorig[] = { 0,0,4,0,2,0,1 };
      ਪੂਰਨ xspc[]  = { 8,8,4,4,2,2,1 };
      ਪੂਰਨ yspc[]  = { 8,8,8,4,4,2,2 };
      ਪੂਰਨ i,j,x,y;
      // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
      x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
      y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
      ਜੇ (x && y) {
         stbi__uint32 img_len = ((((a->s->img_n * x * ਡੂੰਘਾਈ) + 7) >> 3) + 1) * y;
         ਜੇ (!stbi__create_png_image_raw(a, image_ਅੰਕੜੇ_2, image_ਅੰਕੜੇ_len, ਬਾਹਰਲਾ_n, x, y, ਡੂੰਘਾਈ, color)) {
            STBI_FREE(final);
            ਵਾਪਸ 0;
         }
         ਲਈ (j=0; j < y; ++j) {
            ਲਈ (i=0; i < x; ++i) {
               ਪੂਰਨ ਬਾਹਰਲਾ_y = j*yspc[p]+yorig[p];
               ਪੂਰਨ ਬਾਹਰਲਾ_x = i*xspc[p]+xorig[p];
               ਸਮ੍ਰਿਤੀ_ਨਕਲ(final + ਬਾਹਰਲਾ_y*a->s->img_x*ਬਾਹਰਲਾ_ਬਾਈਟਾਂ + ਬਾਹਰਲਾ_x*ਬਾਹਰਲਾ_ਬਾਈਟਾਂ,
                      a->ਬਾਹਰਲਾ + (j*x+i)*ਬਾਹਰਲਾ_ਬਾਈਟਾਂ, ਬਾਹਰਲਾ_ਬਾਈਟਾਂ);
            }
         }
         STBI_FREE(a->ਬਾਹਰਲਾ);
         image_ਅੰਕੜੇ_2 += img_len;
         image_ਅੰਕੜੇ_len -= img_len;
      }
   }
   a->ਬਾਹਰਲਾ = final;

   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__compute_transparency(stbi__png *z, stbi_uc tc[3], ਪੂਰਨ ਬਾਹਰਲਾ_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ਗਿਣਤੀ = s->img_x * s->img_y;
   stbi_uc *p = z->ਬਾਹਰਲਾ;

   // compute color-based transparency, assuming we've
   // already got 255 as the alpha value in the output
   STBI_ASSERT(ਬਾਹਰਲਾ_n == 2 || ਬਾਹਰਲਾ_n == 4);

   ਜੇ (ਬਾਹਰਲਾ_n == 2) {
      ਲਈ (i=0; i < pixel_ਗਿਣਤੀ; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 255);
         p += 2;
      }
   } ਨਹੀਂਤਾਂ {
      ਲਈ (i=0; i < pixel_ਗਿਣਤੀ; ++i) {
         ਜੇ (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__compute_transparency16(stbi__png *z, stbi__uint16 tc[3], ਪੂਰਨ ਬਾਹਰਲਾ_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ਗਿਣਤੀ = s->img_x * s->img_y;
   stbi__uint16 *p = (stbi__uint16*) z->ਬਾਹਰਲਾ;

   // compute color-based transparency, assuming we've
   // already got 65535 as the alpha value in the output
   STBI_ASSERT(ਬਾਹਰਲਾ_n == 2 || ਬਾਹਰਲਾ_n == 4);

   ਜੇ (ਬਾਹਰਲਾ_n == 2) {
      ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 65535);
         p += 2;
      }
   } ਨਹੀਂਤਾਂ {
      ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ; ++i) {
         ਜੇ (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__expand_png_palette(stbi__png *a, stbi_uc *palette, ਪੂਰਨ len, ਪੂਰਨ pal_img_n)
{
   stbi__uint32 i, pixel_ਗਿਣਤੀ = a->s->img_x * a->s->img_y;
   stbi_uc *p, *ਆਰਜ਼ੀ_ਬਾਹਰਲਾ, *orig = a->ਬਾਹਰਲਾ;

   p = (stbi_uc *) stbi__malloc_mad2(pixel_ਗਿਣਤੀ, pal_img_n, 0);
   ਜੇ (p == ਨਿਰਾ) ਵਾਪਸ stbi__err("outofmem", "Out of memory");

   // between here and free(out) below, exitting would leak
   ਆਰਜ਼ੀ_ਬਾਹਰਲਾ = p;

   ਜੇ (pal_img_n == 3) {
      ਲਈ (i=0; i < pixel_ਗਿਣਤੀ; ++i) {
         ਪੂਰਨ n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p += 3;
      }
   } ਨਹੀਂਤਾਂ {
      ਲਈ (i=0; i < pixel_ਗਿਣਤੀ; ++i) {
         ਪੂਰਨ n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p[3] = palette[n+3];
         p += 4;
      }
   }
   STBI_FREE(a->ਬਾਹਰਲਾ);
   a->ਬਾਹਰਲਾ = ਆਰਜ਼ੀ_ਬਾਹਰਲਾ;

   STBI_NOTUSED(len);

   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi_unpremultiply_on_ਲੱਦੋ = 0;
ਸਥਿਰ ਪੂਰਨ stbi__de_iphone_flag = 0;

STBIDEF ਖਾਲੀ stbi_ਸੈੱਟ_unpremultiply_on_ਲੱਦੋ(ਪੂਰਨ flag_true_if_should_unpremultiply)
{
   stbi_unpremultiply_on_ਲੱਦੋ = flag_true_if_should_unpremultiply;
}

STBIDEF ਖਾਲੀ stbi_convert_iphone_png_ਮੰਜ਼ਿਲ_rgb(ਪੂਰਨ flag_true_if_should_convert)
{
   stbi__de_iphone_flag = flag_true_if_should_convert;
}

ਸਥਿਰ ਖਾਲੀ stbi__de_iphone(stbi__png *z)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ਗਿਣਤੀ = s->img_x * s->img_y;
   stbi_uc *p = z->ਬਾਹਰਲਾ;

   ਜੇ (s->img_ਬਾਹਰਲਾ_n == 3) {  // convert bgr to rgb
      ਲਈ (i=0; i < pixel_ਗਿਣਤੀ; ++i) {
         stbi_uc t = p[0];
         p[0] = p[2];
         p[2] = t;
         p += 3;
      }
   } ਨਹੀਂਤਾਂ {
      STBI_ASSERT(s->img_ਬਾਹਰਲਾ_n == 4);
      ਜੇ (stbi_unpremultiply_on_ਲੱਦੋ) {
         // convert bgr to rgb and unpremultiply
         ਲਈ (i=0; i < pixel_ਗਿਣਤੀ; ++i) {
            stbi_uc a = p[3];
            stbi_uc t = p[0];
            ਜੇ (a) {
               stbi_uc half = a / 2;
               p[0] = (p[2] * 255 + half) / a;
               p[1] = (p[1] * 255 + half) / a;
               p[2] = ( t   * 255 + half) / a;
            } ਨਹੀਂਤਾਂ {
               p[0] = p[2];
               p[2] = t;
            }
            p += 4;
         }
      } ਨਹੀਂਤਾਂ {
         // convert bgr to rgb
         ਲਈ (i=0; i < pixel_ਗਿਣਤੀ; ++i) {
            stbi_uc t = p[0];
            p[0] = p[2];
            p[2] = t;
            p += 4;
         }
      }
   }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__PNG_TYPE(a,b,c,d)  (((ਬਿਨਾਨਿਸ਼ਾਨੀ) (a) << 24) + ((ਬਿਨਾਨਿਸ਼ਾਨੀ) (b) << 16) + ((ਬਿਨਾਨਿਸ਼ਾਨੀ) (c) << 8) + (ਬਿਨਾਨਿਸ਼ਾਨੀ) (d))

ਸਥਿਰ ਪੂਰਨ stbi__parse_png_file(stbi__png *z, ਪੂਰਨ ਜਾਂਚ, ਪੂਰਨ req_comp)
{
   stbi_uc palette[1024], pal_img_n=0;
   stbi_uc has_trans=0, tc[3];
   stbi__uint16 tc16[3];
   stbi__uint32 ioff=0, idata_ਹੱਦਬੰਦੀ=0, i, pal_len=0;
   ਪੂਰਨ ਪਹਿਲਾ=1,k,interlace=0, color=0, is_iphone=0;
   stbi__context *s = z->s;

   z->expanded = ਨਿਰਾ;
   z->idata = ਨਿਰਾ;
   z->ਬਾਹਰਲਾ = ਨਿਰਾ;

   ਜੇ (!stbi_check_png_ਸਿਰਲੇਖ(s)) ਵਾਪਸ 0;

   ਜੇ (ਜਾਂਚ == STBI_ਜਾਂਚ_type) ਵਾਪਸ 1;

   ਲਈ (;;) {
      stbi__pngchunk c = stbi_get_chunk_ਸਿਰਲੇਖ(s);
      ਚੋਣ (c.type) {
         ਹਾਲਤ STBI__PNG_TYPE('C','g','B','I'):
            is_iphone = 1;
            stbi__skip(s, c.ਲੰਮਾਈ);
            ਰੋਕੋ;
         ਹਾਲਤ STBI__PNG_TYPE('I','H','D','R'): {
            ਪੂਰਨ comp,filter;
            ਜੇ (!ਪਹਿਲਾ) ਵਾਪਸ stbi__err("multiple IHDR","Corrupt PNG");
            ਪਹਿਲਾ = 0;
            ਜੇ (c.ਲੰਮਾਈ != 13) ਵਾਪਸ stbi__err("bad IHDR len","Corrupt PNG");
            s->img_x = stbi__get32be(s); ਜੇ (s->img_x > (1 << 24)) ਵਾਪਸ stbi__err("too large","Very large image (corrupt?)");
            s->img_y = stbi__get32be(s); ਜੇ (s->img_y > (1 << 24)) ਵਾਪਸ stbi__err("too large","Very large image (corrupt?)");
            z->ਡੂੰਘਾਈ = stbi__get8(s);  ਜੇ (z->ਡੂੰਘਾਈ != 1 && z->ਡੂੰਘਾਈ != 2 && z->ਡੂੰਘਾਈ != 4 && z->ਡੂੰਘਾਈ != 8 && z->ਡੂੰਘਾਈ != 16)  ਵਾਪਸ stbi__err("1/2/4/8/16-bit only","PNG not supported: 1/2/4/8/16-bit only");
            color = stbi__get8(s);  ਜੇ (color > 6)         ਵਾਪਸ stbi__err("bad ctype","Corrupt PNG");
            ਜੇ (color == 3 && z->ਡੂੰਘਾਈ == 16)                  ਵਾਪਸ stbi__err("bad ctype","Corrupt PNG");
            ਜੇ (color == 3) pal_img_n = 3; ਨਹੀਂਤਾਂ ਜੇ (color & 1) ਵਾਪਸ stbi__err("bad ctype","Corrupt PNG");
            comp  = stbi__get8(s);  ਜੇ (comp) ਵਾਪਸ stbi__err("bad comp method","Corrupt PNG");
            filter= stbi__get8(s);  ਜੇ (filter) ਵਾਪਸ stbi__err("bad filter method","Corrupt PNG");
            interlace = stbi__get8(s); ਜੇ (interlace>1) ਵਾਪਸ stbi__err("bad interlace method","Corrupt PNG");
            ਜੇ (!s->img_x || !s->img_y) ਵਾਪਸ stbi__err("0-pixel image","Corrupt PNG");
            ਜੇ (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               ਜੇ ((1 << 30) / s->img_x / s->img_n < s->img_y) ਵਾਪਸ stbi__err("too large", "Image too large to decode");
               ਜੇ (ਜਾਂਚ == STBI_ਜਾਂਚ_ਸਿਰਲੇਖ) ਵਾਪਸ 1;
            } ਨਹੀਂਤਾਂ {
               // if paletted, then pal_n is our final components, and
               // img_n is # components to decompress/filter.
               s->img_n = 1;
               ਜੇ ((1 << 30) / s->img_x / 4 < s->img_y) ਵਾਪਸ stbi__err("too large","Corrupt PNG");
               // if SCAN_header, have to scan to see if we have a tRNS
            }
            ਰੋਕੋ;
         }

         ਹਾਲਤ STBI__PNG_TYPE('P','L','T','E'):  {
            ਜੇ (ਪਹਿਲਾ) ਵਾਪਸ stbi__err("first not IHDR", "Corrupt PNG");
            ਜੇ (c.ਲੰਮਾਈ > 256*3) ਵਾਪਸ stbi__err("invalid PLTE","Corrupt PNG");
            pal_len = c.ਲੰਮਾਈ / 3;
            ਜੇ (pal_len * 3 != c.ਲੰਮਾਈ) ਵਾਪਸ stbi__err("invalid PLTE","Corrupt PNG");
            ਲਈ (i=0; i < pal_len; ++i) {
               palette[i*4+0] = stbi__get8(s);
               palette[i*4+1] = stbi__get8(s);
               palette[i*4+2] = stbi__get8(s);
               palette[i*4+3] = 255;
            }
            ਰੋਕੋ;
         }

         ਹਾਲਤ STBI__PNG_TYPE('t','R','N','S'): {
            ਜੇ (ਪਹਿਲਾ) ਵਾਪਸ stbi__err("first not IHDR", "Corrupt PNG");
            ਜੇ (z->idata) ਵਾਪਸ stbi__err("tRNS after IDAT","Corrupt PNG");
            ਜੇ (pal_img_n) {
               ਜੇ (ਜਾਂਚ == STBI_ਜਾਂਚ_ਸਿਰਲੇਖ) { s->img_n = 4; ਵਾਪਸ 1; }
               ਜੇ (pal_len == 0) ਵਾਪਸ stbi__err("tRNS before PLTE","Corrupt PNG");
               ਜੇ (c.ਲੰਮਾਈ > pal_len) ਵਾਪਸ stbi__err("bad tRNS len","Corrupt PNG");
               pal_img_n = 4;
               ਲਈ (i=0; i < c.ਲੰਮਾਈ; ++i)
                  palette[i*4+3] = stbi__get8(s);
            } ਨਹੀਂਤਾਂ {
               ਜੇ (!(s->img_n & 1)) ਵਾਪਸ stbi__err("tRNS with alpha","Corrupt PNG");
               ਜੇ (c.ਲੰਮਾਈ != (stbi__uint32) s->img_n*2) ਵਾਪਸ stbi__err("bad tRNS len","Corrupt PNG");
               has_trans = 1;
               ਜੇ (z->ਡੂੰਘਾਈ == 16) {
                  ਲਈ (k = 0; k < s->img_n; ++k) tc16[k] = (stbi__uint16)stbi__get16be(s); // copy the values as-is
               } ਨਹੀਂਤਾਂ {
                  ਲਈ (k = 0; k < s->img_n; ++k) tc[k] = (stbi_uc)(stbi__get16be(s) & 255) * stbi_ਡੂੰਘਾਈ_scale_ਸਾਰਣੀ[z->ਡੂੰਘਾਈ]; // non 8-bit images will be larger
               }
            }
            ਰੋਕੋ;
         }

         ਹਾਲਤ STBI__PNG_TYPE('I','D','A','T'): {
            ਜੇ (ਪਹਿਲਾ) ਵਾਪਸ stbi__err("first not IHDR", "Corrupt PNG");
            ਜੇ (pal_img_n && !pal_len) ਵਾਪਸ stbi__err("no PLTE","Corrupt PNG");
            ਜੇ (ਜਾਂਚ == STBI_ਜਾਂਚ_ਸਿਰਲੇਖ) { s->img_n = pal_img_n; ਵਾਪਸ 1; }
            ਜੇ ((ਪੂਰਨ)(ioff + c.ਲੰਮਾਈ) < (ਪੂਰਨ)ioff) ਵਾਪਸ 0;
            ਜੇ (ioff + c.ਲੰਮਾਈ > idata_ਹੱਦਬੰਦੀ) {
               stbi__uint32 idata_ਹੱਦਬੰਦੀ_old = idata_ਹੱਦਬੰਦੀ;
               stbi_uc *p;
               ਜੇ (idata_ਹੱਦਬੰਦੀ == 0) idata_ਹੱਦਬੰਦੀ = c.ਲੰਮਾਈ > 4096 ? c.ਲੰਮਾਈ : 4096;
               ਜਦੋਂ (ioff + c.ਲੰਮਾਈ > idata_ਹੱਦਬੰਦੀ)
                  idata_ਹੱਦਬੰਦੀ *= 2;
               STBI_NOTUSED(idata_ਹੱਦਬੰਦੀ_old);
               p = (stbi_uc *) STBI_REALLOC_SIZED(z->idata, idata_ਹੱਦਬੰਦੀ_old, idata_ਹੱਦਬੰਦੀ); ਜੇ (p == ਨਿਰਾ) ਵਾਪਸ stbi__err("outofmem", "Out of memory");
               z->idata = p;
            }
            ਜੇ (!stbi__getn(s, z->idata+ioff,c.ਲੰਮਾਈ)) ਵਾਪਸ stbi__err("outofdata","Corrupt PNG");
            ioff += c.ਲੰਮਾਈ;
            ਰੋਕੋ;
         }

         ਹਾਲਤ STBI__PNG_TYPE('I','E','N','D'): {
            stbi__uint32 raw_len, bpl;
            ਜੇ (ਪਹਿਲਾ) ਵਾਪਸ stbi__err("first not IHDR", "Corrupt PNG");
            ਜੇ (ਜਾਂਚ != STBI_ਜਾਂਚ_ਲੱਦੋ) ਵਾਪਸ 1;
            ਜੇ (z->idata == ਨਿਰਾ) ਵਾਪਸ stbi__err("no IDAT","Corrupt PNG");
            // initial guess for decoded data size to avoid unnecessary reallocs
            bpl = (s->img_x * z->ਡੂੰਘਾਈ + 7) / 8; // bytes per line, per component
            raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
            z->expanded = (stbi_uc *) stbi_zlib_decode_malloc_guesssize_headerflag((ਅੱਖਰ *) z->idata, ioff, raw_len, (ਪੂਰਨ *) &raw_len, !is_iphone);
            ਜੇ (z->expanded == ਨਿਰਾ) ਵਾਪਸ 0; // zlib should set error
            STBI_FREE(z->idata); z->idata = ਨਿਰਾ;
            ਜੇ ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_ਬਾਹਰਲਾ_n = s->img_n+1;
            ਨਹੀਂਤਾਂ
               s->img_ਬਾਹਰਲਾ_n = s->img_n;
            ਜੇ (!stbi__create_png_image(z, z->expanded, raw_len, s->img_ਬਾਹਰਲਾ_n, z->ਡੂੰਘਾਈ, color, interlace)) ਵਾਪਸ 0;
            ਜੇ (has_trans) {
               ਜੇ (z->ਡੂੰਘਾਈ == 16) {
                  ਜੇ (!stbi__compute_transparency16(z, tc16, s->img_ਬਾਹਰਲਾ_n)) ਵਾਪਸ 0;
               } ਨਹੀਂਤਾਂ {
                  ਜੇ (!stbi__compute_transparency(z, tc, s->img_ਬਾਹਰਲਾ_n)) ਵਾਪਸ 0;
               }
            }
            ਜੇ (is_iphone && stbi__de_iphone_flag && s->img_ਬਾਹਰਲਾ_n > 2)
               stbi__de_iphone(z);
            ਜੇ (pal_img_n) {
               // pal_img_n == 3 or 4
               s->img_n = pal_img_n; // record the actual colors we had
               s->img_ਬਾਹਰਲਾ_n = pal_img_n;
               ਜੇ (req_comp >= 3) s->img_ਬਾਹਰਲਾ_n = req_comp;
               ਜੇ (!stbi__expand_png_palette(z, palette, pal_len, s->img_ਬਾਹਰਲਾ_n))
                  ਵਾਪਸ 0;
            } ਨਹੀਂਤਾਂ ਜੇ (has_trans) {
               // non-paletted image with tRNS -> source image has (constant) alpha
               ++s->img_n;
            }
            STBI_FREE(z->expanded); z->expanded = ਨਿਰਾ;
            ਵਾਪਸ 1;
         }

         ਮੂਲ:
            // if critical, fail
            ਜੇ (ਪਹਿਲਾ) ਵਾਪਸ stbi__err("first not IHDR", "Corrupt PNG");
            ਜੇ ((c.type & (1 << 29)) == 0) {
               #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_FAILURE_ਲੜੀਆਂ
               // not threadsafe
               ਸਥਿਰ ਅੱਖਰ invalid_chunk[] = "XXXX PNG chunk not known";
               invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
               invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
               invalid_chunk[2] = STBI__BYTECAST(c.type >>  8);
               invalid_chunk[3] = STBI__BYTECAST(c.type >>  0);
               #ਜੇ_ਅੰਤ
               ਵਾਪਸ stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
            }
            stbi__skip(s, c.ਲੰਮਾਈ);
            ਰੋਕੋ;
      }
      // end of PNG chunk, read and skip CRC
      stbi__get32be(s);
   }
}

ਸਥਿਰ ਖਾਲੀ *stbi__do_png(stbi__png *p, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *n, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   ਖਾਲੀ *ਨਤੀਜਾ=ਨਿਰਾ;
   ਜੇ (req_comp < 0 || req_comp > 4) ਵਾਪਸ stbi__errpuc("bad req_comp", "Internal error");
   ਜੇ (stbi__parse_png_file(p, STBI_ਜਾਂਚ_ਲੱਦੋ, req_comp)) {
      ਜੇ (p->ਡੂੰਘਾਈ < 8)
         ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel = 8;
      ਨਹੀਂਤਾਂ
         ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel = p->ਡੂੰਘਾਈ;
      ਨਤੀਜਾ = p->ਬਾਹਰਲਾ;
      p->ਬਾਹਰਲਾ = ਨਿਰਾ;
      ਜੇ (req_comp && req_comp != p->s->img_ਬਾਹਰਲਾ_n) {
         ਜੇ (ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel == 8)
            ਨਤੀਜਾ = stbi__convert_format((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਨਤੀਜਾ, p->s->img_ਬਾਹਰਲਾ_n, req_comp, p->s->img_x, p->s->img_y);
         ਨਹੀਂਤਾਂ
            ਨਤੀਜਾ = stbi__convert_format16((stbi__uint16 *) ਨਤੀਜਾ, p->s->img_ਬਾਹਰਲਾ_n, req_comp, p->s->img_x, p->s->img_y);
         p->s->img_ਬਾਹਰਲਾ_n = req_comp;
         ਜੇ (ਨਤੀਜਾ == ਨਿਰਾ) ਵਾਪਸ ਨਤੀਜਾ;
      }
      *x = p->s->img_x;
      *y = p->s->img_y;
      ਜੇ (n) *n = p->s->img_n;
   }
   STBI_FREE(p->ਬਾਹਰਲਾ);      p->ਬਾਹਰਲਾ      = ਨਿਰਾ;
   STBI_FREE(p->expanded); p->expanded = ਨਿਰਾ;
   STBI_FREE(p->idata);    p->idata    = ਨਿਰਾ;

   ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ *stbi_png_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   stbi__png p;
   p.s = s;
   ਵਾਪਸ stbi__do_png(&p, x,y,comp,req_comp, ri);
}

ਸਥਿਰ ਪੂਰਨ stbi__png_test(stbi__context *s)
{
   ਪੂਰਨ r;
   r = stbi_check_png_ਸਿਰਲੇਖ(s);
   stbi__rewind(s);
   ਵਾਪਸ r;
}

ਸਥਿਰ ਪੂਰਨ stbi__png_info_raw(stbi__png *p, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਜੇ (!stbi__parse_png_file(p, STBI_ਜਾਂਚ_ਸਿਰਲੇਖ, 0)) {
      stbi__rewind( p->s );
      ਵਾਪਸ 0;
   }
   ਜੇ (x) *x = p->s->img_x;
   ਜੇ (y) *y = p->s->img_y;
   ਜੇ (comp) *comp = p->s->img_n;
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__png_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   stbi__png p;
   p.s = s;
   ਵਾਪਸ stbi__png_info_raw(&p, x, y, comp);
}

ਸਥਿਰ ਪੂਰਨ stbi__png_is16(stbi__context *s)
{
   stbi__png p;
   p.s = s;
   ਜੇ (!stbi__png_info_raw(&p, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ))
	   ਵਾਪਸ 0;
   ਜੇ (p.ਡੂੰਘਾਈ != 16) {
      stbi__rewind(p.s);
      ਵਾਪਸ 0;
   }
   ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ

// Microsoft/Windows BMP image

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_BMP
ਸਥਿਰ ਪੂਰਨ stbi__bmp_test_raw(stbi__context *s)
{
   ਪੂਰਨ r;
   ਪੂਰਨ sz;
   ਜੇ (stbi__get8(s) != 'B') ਵਾਪਸ 0;
   ਜੇ (stbi__get8(s) != 'M') ਵਾਪਸ 0;
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   stbi__get32le(s); // discard data offset
   sz = stbi__get32le(s);
   r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
   ਵਾਪਸ r;
}

ਸਥਿਰ ਪੂਰਨ stbi__bmp_test(stbi__context *s)
{
   ਪੂਰਨ r = stbi__bmp_test_raw(s);
   stbi__rewind(s);
   ਵਾਪਸ r;
}


// returns 0..31 for the highest set bit
ਸਥਿਰ ਪੂਰਨ stbi_ਉੱਚਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ z)
{
   ਪੂਰਨ n=0;
   ਜੇ (z == 0) ਵਾਪਸ -1;
   ਜੇ (z >= 0x10000) n += 16, z >>= 16;
   ਜੇ (z >= 0x00100) n +=  8, z >>=  8;
   ਜੇ (z >= 0x00010) n +=  4, z >>=  4;
   ਜੇ (z >= 0x00004) n +=  2, z >>=  2;
   ਜੇ (z >= 0x00002) n +=  1, z >>=  1;
   ਵਾਪਸ n;
}

ਸਥਿਰ ਪੂਰਨ stbi__bitcount(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a)
{
   a = (a & 0x55555555) + ((a >>  1) & 0x55555555); // max 2
   a = (a & 0x33333333) + ((a >>  2) & 0x33333333); // max 4
   a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
   a = (a + (a >> 8)); // max 16 per 8 bits
   a = (a + (a >> 16)); // max 32 per 8 bits
   ਵਾਪਸ a & 0xff;
}

// extract an arbitrarily-aligned N-bit value (N=bits)
// from v, and then make it 8-bits long and fractionally
// extend it to full full range.
ਸਥਿਰ ਪੂਰਨ stbi__shiftsigned(ਪੂਰਨ v, ਪੂਰਨ ਖਿਸਕਾਓ, ਪੂਰਨ ਬਿੱਟ)
{
   ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mul_ਸਾਰਣੀ[9] = {
      0,
      0xff/*0b11111111*/, 0x55/*0b01010101*/, 0x49/*0b01001001*/, 0x11/*0b00010001*/,
      0x21/*0b00100001*/, 0x41/*0b01000001*/, 0x81/*0b10000001*/, 0x01/*0b00000001*/,
   };
   ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ_ਸਾਰਣੀ[9] = {
      0, 0,0,1,0,2,4,6,0,
   };
   ਜੇ (ਖਿਸਕਾਓ < 0)
      v <<= -ਖਿਸਕਾਓ;
   ਨਹੀਂਤਾਂ
      v >>= ਖਿਸਕਾਓ;
   STBI_ASSERT(v >= 0 && v < 256);
   v >>= (8-ਬਿੱਟ);
   STBI_ASSERT(ਬਿੱਟ >= 0 && ਬਿੱਟ <= 8);
   ਵਾਪਸ (ਪੂਰਨ) ((ਬਿਨਾਨਿਸ਼ਾਨੀ) v * mul_ਸਾਰਣੀ[ਬਿੱਟ]) >> ਖਿਸਕਾਓ_ਸਾਰਣੀ[ਬਿੱਟ];
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   ਪੂਰਨ bpp, ਵਿੱਥ, hsz;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mr,mg,mb,ma, all_a;
} stbi_bmp_ਅੰਕੜੇ;

ਸਥਿਰ ਖਾਲੀ *stbi_bmp_parse_ਸਿਰਲੇਖ(stbi__context *s, stbi_bmp_ਅੰਕੜੇ *info)
{
   ਪੂਰਨ hsz;
   ਜੇ (stbi__get8(s) != 'B' || stbi__get8(s) != 'M') ਵਾਪਸ stbi__errpuc("not BMP", "Corrupt BMP");
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   info->ਵਿੱਥ = stbi__get32le(s);
   info->hsz = hsz = stbi__get32le(s);
   info->mr = info->mg = info->mb = info->ma = 0;

   ਜੇ (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) ਵਾਪਸ stbi__errpuc("unknown BMP", "BMP type not supported: unknown");
   ਜੇ (hsz == 12) {
      s->img_x = stbi__get16le(s);
      s->img_y = stbi__get16le(s);
   } ਨਹੀਂਤਾਂ {
      s->img_x = stbi__get32le(s);
      s->img_y = stbi__get32le(s);
   }
   ਜੇ (stbi__get16le(s) != 1) ਵਾਪਸ stbi__errpuc("bad BMP", "bad BMP");
   info->bpp = stbi__get16le(s);
   ਜੇ (hsz != 12) {
      ਪੂਰਨ compress = stbi__get32le(s);
      ਜੇ (compress == 1 || compress == 2) ਵਾਪਸ stbi__errpuc("BMP RLE", "BMP type not supported: RLE");
      stbi__get32le(s); // discard sizeof
      stbi__get32le(s); // discard hres
      stbi__get32le(s); // discard vres
      stbi__get32le(s); // discard colorsused
      stbi__get32le(s); // discard max important
      ਜੇ (hsz == 40 || hsz == 56) {
         ਜੇ (hsz == 56) {
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
         }
         ਜੇ (info->bpp == 16 || info->bpp == 32) {
            ਜੇ (compress == 0) {
               ਜੇ (info->bpp == 32) {
                  info->mr = 0xffu << 16;
                  info->mg = 0xffu <<  8;
                  info->mb = 0xffu <<  0;
                  info->ma = 0xffu << 24;
                  info->all_a = 0; // if all_a is 0 at end, then we loaded alpha channel but it was all 0
               } ਨਹੀਂਤਾਂ {
                  info->mr = 31u << 10;
                  info->mg = 31u <<  5;
                  info->mb = 31u <<  0;
               }
            } ਨਹੀਂਤਾਂ ਜੇ (compress == 3) {
               info->mr = stbi__get32le(s);
               info->mg = stbi__get32le(s);
               info->mb = stbi__get32le(s);
               // not documented, but generated by photoshop and handled by mspaint
               ਜੇ (info->mr == info->mg && info->mg == info->mb) {
                  // ?!?!?
                  ਵਾਪਸ stbi__errpuc("bad BMP", "bad BMP");
               }
            } ਨਹੀਂਤਾਂ
               ਵਾਪਸ stbi__errpuc("bad BMP", "bad BMP");
         }
      } ਨਹੀਂਤਾਂ {
         ਪੂਰਨ i;
         ਜੇ (hsz != 108 && hsz != 124)
            ਵਾਪਸ stbi__errpuc("bad BMP", "bad BMP");
         info->mr = stbi__get32le(s);
         info->mg = stbi__get32le(s);
         info->mb = stbi__get32le(s);
         info->ma = stbi__get32le(s);
         stbi__get32le(s); // discard color space
         ਲਈ (i=0; i < 12; ++i)
            stbi__get32le(s); // discard color space parameters
         ਜੇ (hsz == 124) {
            stbi__get32le(s); // discard rendering intent
            stbi__get32le(s); // discard offset of profile data
            stbi__get32le(s); // discard size of profile data
            stbi__get32le(s); // discard reserved
         }
      }
   }
   ਵਾਪਸ (ਖਾਲੀ *) 1;
}


ਸਥਿਰ ਖਾਲੀ *stbi_bmp_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   stbi_uc *ਬਾਹਰਲਾ;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mr=0,mg=0,mb=0,ma=0, all_a;
   stbi_uc pal[256][4];
   ਪੂਰਨ psize=0,i,j,ਚੌੜਾਈ;
   ਪੂਰਨ flip_vertically, pad, ਨਿਸ਼ਾਨਾ;
   stbi_bmp_ਅੰਕੜੇ info;
   STBI_NOTUSED(ri);

   info.all_a = 255;
   ਜੇ (stbi_bmp_parse_ਸਿਰਲੇਖ(s, &info) == ਨਿਰਾ)
      ਵਾਪਸ ਨਿਰਾ; // error code already set

   flip_vertically = ((ਪੂਰਨ) s->img_y) > 0;
   s->img_y = ਨਿਰਪੇਖ((ਪੂਰਨ) s->img_y);

   mr = info.mr;
   mg = info.mg;
   mb = info.mb;
   ma = info.ma;
   all_a = info.all_a;

   ਜੇ (info.hsz == 12) {
      ਜੇ (info.bpp < 24)
         psize = (info.ਵਿੱਥ - 14 - 24) / 3;
   } ਨਹੀਂਤਾਂ {
      ਜੇ (info.bpp < 16)
         psize = (info.ਵਿੱਥ - 14 - info.hsz) >> 2;
   }

   s->img_n = ma ? 4 : 3;
   ਜੇ (req_comp && req_comp >= 3) // we can directly decode 3 or 4
      ਨਿਸ਼ਾਨਾ = req_comp;
   ਨਹੀਂਤਾਂ
      ਨਿਸ਼ਾਨਾ = s->img_n; // if they want monochrome, we'll post-convert

   // sanity-check size
   ਜੇ (!stbi__mad3sizes_valid(ਨਿਸ਼ਾਨਾ, s->img_x, s->img_y, 0))
      ਵਾਪਸ stbi__errpuc("too large", "Corrupt BMP");

   ਬਾਹਰਲਾ = (stbi_uc *) stbi__malloc_mad3(ਨਿਸ਼ਾਨਾ, s->img_x, s->img_y, 0);
   ਜੇ (!ਬਾਹਰਲਾ) ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");
   ਜੇ (info.bpp < 16) {
      ਪੂਰਨ z=0;
      ਜੇ (psize == 0 || psize > 256) { STBI_FREE(ਬਾਹਰਲਾ); ਵਾਪਸ stbi__errpuc("invalid", "Corrupt BMP"); }
      ਲਈ (i=0; i < psize; ++i) {
         pal[i][2] = stbi__get8(s);
         pal[i][1] = stbi__get8(s);
         pal[i][0] = stbi__get8(s);
         ਜੇ (info.hsz != 12) stbi__get8(s);
         pal[i][3] = 255;
      }
      stbi__skip(s, info.ਵਿੱਥ - 14 - info.hsz - psize * (info.hsz == 12 ? 3 : 4));
      ਜੇ (info.bpp == 1) ਚੌੜਾਈ = (s->img_x + 7) >> 3;
      ਨਹੀਂਤਾਂ ਜੇ (info.bpp == 4) ਚੌੜਾਈ = (s->img_x + 1) >> 1;
      ਨਹੀਂਤਾਂ ਜੇ (info.bpp == 8) ਚੌੜਾਈ = s->img_x;
      ਨਹੀਂਤਾਂ { STBI_FREE(ਬਾਹਰਲਾ); ਵਾਪਸ stbi__errpuc("bad bpp", "Corrupt BMP"); }
      pad = (-ਚੌੜਾਈ)&3;
      ਜੇ (info.bpp == 1) {
         ਲਈ (j=0; j < (ਪੂਰਨ) s->img_y; ++j) {
            ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_ਵਿੱਥ = 7, v = stbi__get8(s);
            ਲਈ (i=0; i < (ਪੂਰਨ) s->img_x; ++i) {
               ਪੂਰਨ color = (v>>ਦੋਆਧਾਰੀ_ਅੰਕ_ਵਿੱਥ)&0x1;
               ਬਾਹਰਲਾ[z++] = pal[color][0];
               ਬਾਹਰਲਾ[z++] = pal[color][1];
               ਬਾਹਰਲਾ[z++] = pal[color][2];
               ਜੇ((--ਦੋਆਧਾਰੀ_ਅੰਕ_ਵਿੱਥ) < 0) {
                  ਦੋਆਧਾਰੀ_ਅੰਕ_ਵਿੱਥ = 7;
                  v = stbi__get8(s);
               }
            }
            stbi__skip(s, pad);
         }
      } ਨਹੀਂਤਾਂ {
         ਲਈ (j=0; j < (ਪੂਰਨ) s->img_y; ++j) {
            ਲਈ (i=0; i < (ਪੂਰਨ) s->img_x; i += 2) {
               ਪੂਰਨ v=stbi__get8(s),v2=0;
               ਜੇ (info.bpp == 4) {
                  v2 = v & 15;
                  v >>= 4;
               }
               ਬਾਹਰਲਾ[z++] = pal[v][0];
               ਬਾਹਰਲਾ[z++] = pal[v][1];
               ਬਾਹਰਲਾ[z++] = pal[v][2];
               ਜੇ (ਨਿਸ਼ਾਨਾ == 4) ਬਾਹਰਲਾ[z++] = 255;
               ਜੇ (i+1 == (ਪੂਰਨ) s->img_x) ਰੋਕੋ;
               v = (info.bpp == 8) ? stbi__get8(s) : v2;
               ਬਾਹਰਲਾ[z++] = pal[v][0];
               ਬਾਹਰਲਾ[z++] = pal[v][1];
               ਬਾਹਰਲਾ[z++] = pal[v][2];
               ਜੇ (ਨਿਸ਼ਾਨਾ == 4) ਬਾਹਰਲਾ[z++] = 255;
            }
            stbi__skip(s, pad);
         }
      }
   } ਨਹੀਂਤਾਂ {
      ਪੂਰਨ rshift=0,gshift=0,bshift=0,ashift=0,rcount=0,gcount=0,bcount=0,acount=0;
      ਪੂਰਨ z = 0;
      ਪੂਰਨ easy=0;
      stbi__skip(s, info.ਵਿੱਥ - 14 - info.hsz);
      ਜੇ (info.bpp == 24) ਚੌੜਾਈ = 3 * s->img_x;
      ਨਹੀਂਤਾਂ ਜੇ (info.bpp == 16) ਚੌੜਾਈ = 2*s->img_x;
      ਨਹੀਂਤਾਂ /* bpp = 32 and pad = 0 */ ਚੌੜਾਈ=0;
      pad = (-ਚੌੜਾਈ) & 3;
      ਜੇ (info.bpp == 24) {
         easy = 1;
      } ਨਹੀਂਤਾਂ ਜੇ (info.bpp == 32) {
         ਜੇ (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
            easy = 2;
      }
      ਜੇ (!easy) {
         ਜੇ (!mr || !mg || !mb) { STBI_FREE(ਬਾਹਰਲਾ); ਵਾਪਸ stbi__errpuc("bad masks", "Corrupt BMP"); }
         // right shift amt to put high bit in position #7
         rshift = stbi_ਉੱਚਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(mr)-7; rcount = stbi__bitcount(mr);
         gshift = stbi_ਉੱਚਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(mg)-7; gcount = stbi__bitcount(mg);
         bshift = stbi_ਉੱਚਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(mb)-7; bcount = stbi__bitcount(mb);
         ashift = stbi_ਉੱਚਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(ma)-7; acount = stbi__bitcount(ma);
      }
      ਲਈ (j=0; j < (ਪੂਰਨ) s->img_y; ++j) {
         ਜੇ (easy) {
            ਲਈ (i=0; i < (ਪੂਰਨ) s->img_x; ++i) {
               ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ a;
               ਬਾਹਰਲਾ[z+2] = stbi__get8(s);
               ਬਾਹਰਲਾ[z+1] = stbi__get8(s);
               ਬਾਹਰਲਾ[z+0] = stbi__get8(s);
               z += 3;
               a = (easy == 2 ? stbi__get8(s) : 255);
               all_a |= a;
               ਜੇ (ਨਿਸ਼ਾਨਾ == 4) ਬਾਹਰਲਾ[z++] = a;
            }
         } ਨਹੀਂਤਾਂ {
            ਪੂਰਨ bpp = info.bpp;
            ਲਈ (i=0; i < (ਪੂਰਨ) s->img_x; ++i) {
               stbi__uint32 v = (bpp == 16 ? (stbi__uint32) stbi__get16le(s) : stbi__get32le(s));
               ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a;
               ਬਾਹਰਲਾ[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
               ਬਾਹਰਲਾ[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
               ਬਾਹਰਲਾ[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));
               a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
               all_a |= a;
               ਜੇ (ਨਿਸ਼ਾਨਾ == 4) ਬਾਹਰਲਾ[z++] = STBI__BYTECAST(a);
            }
         }
         stbi__skip(s, pad);
      }
   }

   // if alpha channel is all 0s, replace with all 255s
   ਜੇ (ਨਿਸ਼ਾਨਾ == 4 && all_a == 0)
      ਲਈ (i=4*s->img_x*s->img_y-1; i >= 0; i -= 4)
         ਬਾਹਰਲਾ[i] = 255;

   ਜੇ (flip_vertically) {
      stbi_uc t;
      ਲਈ (j=0; j < (ਪੂਰਨ) s->img_y>>1; ++j) {
         stbi_uc *p1 = ਬਾਹਰਲਾ +      j     *s->img_x*ਨਿਸ਼ਾਨਾ;
         stbi_uc *p2 = ਬਾਹਰਲਾ + (s->img_y-1-j)*s->img_x*ਨਿਸ਼ਾਨਾ;
         ਲਈ (i=0; i < (ਪੂਰਨ) s->img_x*ਨਿਸ਼ਾਨਾ; ++i) {
            t = p1[i], p1[i] = p2[i], p2[i] = t;
         }
      }
   }

   ਜੇ (req_comp && req_comp != ਨਿਸ਼ਾਨਾ) {
      ਬਾਹਰਲਾ = stbi__convert_format(ਬਾਹਰਲਾ, ਨਿਸ਼ਾਨਾ, req_comp, s->img_x, s->img_y);
      ਜੇ (ਬਾਹਰਲਾ == ਨਿਰਾ) ਵਾਪਸ ਬਾਹਰਲਾ; // stbi__convert_format frees input on failure
   }

   *x = s->img_x;
   *y = s->img_y;
   ਜੇ (comp) *comp = s->img_n;
   ਵਾਪਸ ਬਾਹਰਲਾ;
}
#ਜੇ_ਅੰਤ

// Targa Truevision - TGA
// by Jonathan Dummer
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_TGA
// returns STBI_rgb or whatever, 0 on error
ਸਥਿਰ ਪੂਰਨ stbi__tga_get_comp(ਪੂਰਨ ਬਿੱਟ_ਪ੍ਰਤੀ_pixel, ਪੂਰਨ is_grey, ਪੂਰਨ* is_rgb16)
{
   // only RGB or RGBA (incl. 16bit) or grey allowed
   ਜੇ (is_rgb16) *is_rgb16 = 0;
   ਚੋਣ(ਬਿੱਟ_ਪ੍ਰਤੀ_pixel) {
      ਹਾਲਤ 8:  ਵਾਪਸ STBI_grey;
      ਹਾਲਤ 16: ਜੇ(is_grey) ਵਾਪਸ STBI_grey_alpha;
               // fallthrough
      ਹਾਲਤ 15: ਜੇ(is_rgb16) *is_rgb16 = 1;
               ਵਾਪਸ STBI_rgb;
      ਹਾਲਤ 24: // fallthrough
      ਹਾਲਤ 32: ਵਾਪਸ ਬਿੱਟ_ਪ੍ਰਤੀ_pixel/8;
      ਮੂਲ: ਵਾਪਸ 0;
   }
}

ਸਥਿਰ ਪੂਰਨ stbi__tga_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
    ਪੂਰਨ tga_w, tga_h, tga_comp, tga_image_type, tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel, tga_colormap_bpp;
    ਪੂਰਨ sz, tga_colormap_type;
    stbi__get8(s);                   // discard Offset
    tga_colormap_type = stbi__get8(s); // colormap type
    ਜੇ( tga_colormap_type > 1 ) {
        stbi__rewind(s);
        ਵਾਪਸ 0;      // only RGB or indexed allowed
    }
    tga_image_type = stbi__get8(s); // image type
    ਜੇ ( tga_colormap_type == 1 ) { // colormapped (paletted) image
        ਜੇ (tga_image_type != 1 && tga_image_type != 9) {
            stbi__rewind(s);
            ਵਾਪਸ 0;
        }
        stbi__skip(s,4);       // skip index of first colormap entry and number of entries
        sz = stbi__get8(s);    //   check bits per palette color entry
        ਜੇ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) {
            stbi__rewind(s);
            ਵਾਪਸ 0;
        }
        stbi__skip(s,4);       // skip image x and y origin
        tga_colormap_bpp = sz;
    } ਨਹੀਂਤਾਂ { // "normal" image w/o colormap - only RGB or grey allowed, +/- RLE
        ਜੇ ( (tga_image_type != 2) && (tga_image_type != 3) && (tga_image_type != 10) && (tga_image_type != 11) ) {
            stbi__rewind(s);
            ਵਾਪਸ 0; // only RGB or grey allowed, +/- RLE
        }
        stbi__skip(s,9); // skip colormap specification and image x/y origin
        tga_colormap_bpp = 0;
    }
    tga_w = stbi__get16le(s);
    ਜੇ( tga_w < 1 ) {
        stbi__rewind(s);
        ਵਾਪਸ 0;   // test width
    }
    tga_h = stbi__get16le(s);
    ਜੇ( tga_h < 1 ) {
        stbi__rewind(s);
        ਵਾਪਸ 0;   // test height
    }
    tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel = stbi__get8(s); // bits per pixel
    stbi__get8(s); // ignore alpha bits
    ਜੇ (tga_colormap_bpp != 0) {
        ਜੇ((tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel != 8) && (tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel != 16)) {
            // when using a colormap, tga_bits_per_pixel is the size of the indexes
            // I don't think anything but 8 or 16bit indexes makes sense
            stbi__rewind(s);
            ਵਾਪਸ 0;
        }
        tga_comp = stbi__tga_get_comp(tga_colormap_bpp, 0, ਨਿਰਾ);
    } ਨਹੀਂਤਾਂ {
        tga_comp = stbi__tga_get_comp(tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel, (tga_image_type == 3) || (tga_image_type == 11), ਨਿਰਾ);
    }
    ਜੇ(!tga_comp) {
      stbi__rewind(s);
      ਵਾਪਸ 0;
    }
    ਜੇ (x) *x = tga_w;
    ਜੇ (y) *y = tga_h;
    ਜੇ (comp) *comp = tga_comp;
    ਵਾਪਸ 1;                   // seems to have passed everything
}

ਸਥਿਰ ਪੂਰਨ stbi__tga_test(stbi__context *s)
{
   ਪੂਰਨ res = 0;
   ਪੂਰਨ sz, tga_color_type;
   stbi__get8(s);      //   discard Offset
   tga_color_type = stbi__get8(s);   //   color type
   ਜੇ ( tga_color_type > 1 ) ਜਾਓ error_ਸਿਰਾ;   //   only RGB or indexed allowed
   sz = stbi__get8(s);   //   image type
   ਜੇ ( tga_color_type == 1 ) { // colormapped (paletted) image
      ਜੇ (sz != 1 && sz != 9) ਜਾਓ error_ਸਿਰਾ; // colortype 1 demands image type 1 or 9
      stbi__skip(s,4);       // skip index of first colormap entry and number of entries
      sz = stbi__get8(s);    //   check bits per palette color entry
      ਜੇ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) ਜਾਓ error_ਸਿਰਾ;
      stbi__skip(s,4);       // skip image x and y origin
   } ਨਹੀਂਤਾਂ { // "normal" image w/o colormap
      ਜੇ ( (sz != 2) && (sz != 3) && (sz != 10) && (sz != 11) ) ਜਾਓ error_ਸਿਰਾ; // only RGB or grey allowed, +/- RLE
      stbi__skip(s,9); // skip colormap specification and image x/y origin
   }
   ਜੇ ( stbi__get16le(s) < 1 ) ਜਾਓ error_ਸਿਰਾ;      //   test width
   ਜੇ ( stbi__get16le(s) < 1 ) ਜਾਓ error_ਸਿਰਾ;      //   test height
   sz = stbi__get8(s);   //   bits per pixel
   ਜੇ ( (tga_color_type == 1) && (sz != 8) && (sz != 16) ) ਜਾਓ error_ਸਿਰਾ; // for colormapped images, bpp is size of an index
   ਜੇ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) ਜਾਓ error_ਸਿਰਾ;

   res = 1; // if we got this far, everything's good and we can return 1 instead of 0

error_ਸਿਰਾ:
   stbi__rewind(s);
   ਵਾਪਸ res;
}

// read 16bit value and convert to 24bit RGB
ਸਥਿਰ ਖਾਲੀ stbi__tga_read_rgb16(stbi__context *s, stbi_uc* ਬਾਹਰਲਾ)
{
   stbi__uint16 px = (stbi__uint16)stbi__get16le(s);
   stbi__uint16 five_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ = 31;
   // we have 3 channels with 5bits each
   ਪੂਰਨ r = (px >> 10) & five_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ;
   ਪੂਰਨ g = (px >> 5) & five_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ;
   ਪੂਰਨ b = px & five_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ;
   // Note that this saves the data in RGB(A) order, so it doesn't need to be swapped later
   ਬਾਹਰਲਾ[0] = (stbi_uc)((r * 255)/31);
   ਬਾਹਰਲਾ[1] = (stbi_uc)((g * 255)/31);
   ਬਾਹਰਲਾ[2] = (stbi_uc)((b * 255)/31);

   // some people claim that the most significant bit might be used for alpha
   // (possibly if an alpha-bit is set in the "image descriptor byte")
   // but that only made 16bit test images completely translucent..
   // so let's treat all 15 and 16bit TGAs as RGB with no alpha.
}

ਸਥਿਰ ਖਾਲੀ *stbi_tga_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   //   read in the TGA header stuff
   ਪੂਰਨ tga_ਵਿੱਥ = stbi__get8(s);
   ਪੂਰਨ tga_ਸੂਚਕਾਂਕਿਤ = stbi__get8(s);
   ਪੂਰਨ tga_image_type = stbi__get8(s);
   ਪੂਰਨ tga_is_RLE = 0;
   ਪੂਰਨ tga_palette_ਸ਼ੁਰੂ = stbi__get16le(s);
   ਪੂਰਨ tga_palette_len = stbi__get16le(s);
   ਪੂਰਨ tga_palette_ਬਿੱਟ = stbi__get8(s);
   ਪੂਰਨ tga_x_origin = stbi__get16le(s);
   ਪੂਰਨ tga_y_origin = stbi__get16le(s);
   ਪੂਰਨ tga_ਚੌੜਾਈ = stbi__get16le(s);
   ਪੂਰਨ tga_ਉਚਾਈ = stbi__get16le(s);
   ਪੂਰਨ tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel = stbi__get8(s);
   ਪੂਰਨ tga_comp, tga_rgb16=0;
   ਪੂਰਨ tga_inverted = stbi__get8(s);
   // int tga_alpha_bits = tga_inverted & 15; // the 4 lowest bits - unused (useless?)
   //   image data
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *tga_ਅੰਕੜੇ;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *tga_palette = ਨਿਰਾ;
   ਪੂਰਨ i, j;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ raw_ਅੰਕੜੇ[4] = {0};
   ਪੂਰਨ RLE_ਗਿਣਤੀ = 0;
   ਪੂਰਨ RLE_repeating = 0;
   ਪੂਰਨ read_ਅਗਲਾ_pixel = 1;
   STBI_NOTUSED(ri);

   //   do a tiny bit of precessing
   ਜੇ ( tga_image_type >= 8 )
   {
      tga_image_type -= 8;
      tga_is_RLE = 1;
   }
   tga_inverted = 1 - ((tga_inverted >> 5) & 1);

   //   If I'm paletted, then I'll use the number of bits from the palette
   ਜੇ ( tga_ਸੂਚਕਾਂਕਿਤ ) tga_comp = stbi__tga_get_comp(tga_palette_ਬਿੱਟ, 0, &tga_rgb16);
   ਨਹੀਂਤਾਂ tga_comp = stbi__tga_get_comp(tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel, (tga_image_type == 3), &tga_rgb16);

   ਜੇ(!tga_comp) // shouldn't really happen, stbi__tga_test() should have ensured basic consistency
      ਵਾਪਸ stbi__errpuc("bad format", "Can't find out TGA pixelformat");

   //   tga info
   *x = tga_ਚੌੜਾਈ;
   *y = tga_ਉਚਾਈ;
   ਜੇ (comp) *comp = tga_comp;

   ਜੇ (!stbi__mad3sizes_valid(tga_ਚੌੜਾਈ, tga_ਉਚਾਈ, tga_comp, 0))
      ਵਾਪਸ stbi__errpuc("too large", "Corrupt TGA");

   tga_ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)stbi__malloc_mad3(tga_ਚੌੜਾਈ, tga_ਉਚਾਈ, tga_comp, 0);
   ਜੇ (!tga_ਅੰਕੜੇ) ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");

   // skip to the data's starting position (offset usually = 0)
   stbi__skip(s, tga_ਵਿੱਥ );

   ਜੇ ( !tga_ਸੂਚਕਾਂਕਿਤ && !tga_is_RLE && !tga_rgb16 ) {
      ਲਈ (i=0; i < tga_ਉਚਾਈ; ++i) {
         ਪੂਰਨ ਪੰਕਤੀ = tga_inverted ? tga_ਉਚਾਈ -i - 1 : i;
         stbi_uc *tga_ਪੰਕਤੀ = tga_ਅੰਕੜੇ + ਪੰਕਤੀ*tga_ਚੌੜਾਈ*tga_comp;
         stbi__getn(s, tga_ਪੰਕਤੀ, tga_ਚੌੜਾਈ * tga_comp);
      }
   } ਨਹੀਂਤਾਂ  {
      //   do I need to load a palette?
      ਜੇ ( tga_ਸੂਚਕਾਂਕਿਤ)
      {
         //   any data to skip? (offset usually = 0)
         stbi__skip(s, tga_palette_ਸ਼ੁਰੂ );
         //   load the palette
         tga_palette = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)stbi__malloc_mad2(tga_palette_len, tga_comp, 0);
         ਜੇ (!tga_palette) {
            STBI_FREE(tga_ਅੰਕੜੇ);
            ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");
         }
         ਜੇ (tga_rgb16) {
            stbi_uc *pal_ਇੰਦਰਾਜ = tga_palette;
            STBI_ASSERT(tga_comp == STBI_rgb);
            ਲਈ (i=0; i < tga_palette_len; ++i) {
               stbi__tga_read_rgb16(s, pal_ਇੰਦਰਾਜ);
               pal_ਇੰਦਰਾਜ += tga_comp;
            }
         } ਨਹੀਂਤਾਂ ਜੇ (!stbi__getn(s, tga_palette, tga_palette_len * tga_comp)) {
               STBI_FREE(tga_ਅੰਕੜੇ);
               STBI_FREE(tga_palette);
               ਵਾਪਸ stbi__errpuc("bad palette", "Corrupt TGA");
         }
      }
      //   load the data
      ਲਈ (i=0; i < tga_ਚੌੜਾਈ * tga_ਉਚਾਈ; ++i)
      {
         //   if I'm in RLE mode, do I need to get a RLE stbi__pngchunk?
         ਜੇ ( tga_is_RLE )
         {
            ਜੇ ( RLE_ਗਿਣਤੀ == 0 )
            {
               //   yep, get the next byte as a RLE command
               ਪੂਰਨ RLE_cmd = stbi__get8(s);
               RLE_ਗਿਣਤੀ = 1 + (RLE_cmd & 127);
               RLE_repeating = RLE_cmd >> 7;
               read_ਅਗਲਾ_pixel = 1;
            } ਨਹੀਂਤਾਂ ਜੇ ( !RLE_repeating )
            {
               read_ਅਗਲਾ_pixel = 1;
            }
         } ਨਹੀਂਤਾਂ
         {
            read_ਅਗਲਾ_pixel = 1;
         }
         //   OK, if I need to read a pixel, do it now
         ਜੇ ( read_ਅਗਲਾ_pixel )
         {
            //   load however much data we did have
            ਜੇ ( tga_ਸੂਚਕਾਂਕਿਤ )
            {
               // read in index, then perform the lookup
               ਪੂਰਨ pal_idx = (tga_ਬਿੱਟ_ਪ੍ਰਤੀ_pixel == 8) ? stbi__get8(s) : stbi__get16le(s);
               ਜੇ ( pal_idx >= tga_palette_len ) {
                  // invalid index
                  pal_idx = 0;
               }
               pal_idx *= tga_comp;
               ਲਈ (j = 0; j < tga_comp; ++j) {
                  raw_ਅੰਕੜੇ[j] = tga_palette[pal_idx+j];
               }
            } ਨਹੀਂਤਾਂ ਜੇ(tga_rgb16) {
               STBI_ASSERT(tga_comp == STBI_rgb);
               stbi__tga_read_rgb16(s, raw_ਅੰਕੜੇ);
            } ਨਹੀਂਤਾਂ {
               //   read in the data raw
               ਲਈ (j = 0; j < tga_comp; ++j) {
                  raw_ਅੰਕੜੇ[j] = stbi__get8(s);
               }
            }
            //   clear the reading flag for the next pixel
            read_ਅਗਲਾ_pixel = 0;
         } // end of reading a pixel

         // copy data
         ਲਈ (j = 0; j < tga_comp; ++j)
           tga_ਅੰਕੜੇ[i*tga_comp+j] = raw_ਅੰਕੜੇ[j];

         //   in case we're in RLE mode, keep counting down
         --RLE_ਗਿਣਤੀ;
      }
      //   do I need to invert the image?
      ਜੇ ( tga_inverted )
      {
         ਲਈ (j = 0; j*2 < tga_ਉਚਾਈ; ++j)
         {
            ਪੂਰਨ index1 = j * tga_ਚੌੜਾਈ * tga_comp;
            ਪੂਰਨ index2 = (tga_ਉਚਾਈ - 1 - j) * tga_ਚੌੜਾਈ * tga_comp;
            ਲਈ (i = tga_ਚੌੜਾਈ * tga_comp; i > 0; --i)
            {
               ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਆਰਜ਼ੀ = tga_ਅੰਕੜੇ[index1];
               tga_ਅੰਕੜੇ[index1] = tga_ਅੰਕੜੇ[index2];
               tga_ਅੰਕੜੇ[index2] = ਆਰਜ਼ੀ;
               ++index1;
               ++index2;
            }
         }
      }
      //   clear my palette, if I had one
      ਜੇ ( tga_palette != ਨਿਰਾ )
      {
         STBI_FREE( tga_palette );
      }
   }

   // swap RGB - if the source data was RGB16, it already is in the right order
   ਜੇ (tga_comp >= 3 && !tga_rgb16)
   {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* tga_pixel = tga_ਅੰਕੜੇ;
      ਲਈ (i=0; i < tga_ਚੌੜਾਈ * tga_ਉਚਾਈ; ++i)
      {
         ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਆਰਜ਼ੀ = tga_pixel[0];
         tga_pixel[0] = tga_pixel[2];
         tga_pixel[2] = ਆਰਜ਼ੀ;
         tga_pixel += tga_comp;
      }
   }

   // convert to target component count
   ਜੇ (req_comp && req_comp != tga_comp)
      tga_ਅੰਕੜੇ = stbi__convert_format(tga_ਅੰਕੜੇ, tga_comp, req_comp, tga_ਚੌੜਾਈ, tga_ਉਚਾਈ);

   //   the things I do to get rid of an error message, and yet keep
   //   Microsoft's C compilers happy... [8^(
   tga_palette_ਸ਼ੁਰੂ = tga_palette_len = tga_palette_ਬਿੱਟ =
         tga_x_origin = tga_y_origin = 0;
   //   OK, done
   ਵਾਪਸ tga_ਅੰਕੜੇ;
}
#ਜੇ_ਅੰਤ

// *************************************************************************************************
// Photoshop PSD loader -- PD by Thatcher Ulrich, integration by Nicolas Schulz, tweaked by STB

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PSD
ਸਥਿਰ ਪੂਰਨ stbi__psd_test(stbi__context *s)
{
   ਪੂਰਨ r = (stbi__get32be(s) == 0x38425053);
   stbi__rewind(s);
   ਵਾਪਸ r;
}

ਸਥਿਰ ਪੂਰਨ stbi__psd_decode_rle(stbi__context *s, stbi_uc *p, ਪੂਰਨ pixel_ਗਿਣਤੀ_2)
{
   ਪੂਰਨ ਗਿਣਤੀ_2, nleft, len;

   ਗਿਣਤੀ_2 = 0;
   ਜਦੋਂ ((nleft = pixel_ਗਿਣਤੀ_2 - ਗਿਣਤੀ_2) > 0) {
      len = stbi__get8(s);
      ਜੇ (len == 128) {
         // No-op.
      } ਨਹੀਂਤਾਂ ਜੇ (len < 128) {
         // Copy next len+1 bytes literally.
         len++;
         ਜੇ (len > nleft) ਵਾਪਸ 0; // corrupt data
         ਗਿਣਤੀ_2 += len;
         ਜਦੋਂ (len) {
            *p = stbi__get8(s);
            p += 4;
            len--;
         }
      } ਨਹੀਂਤਾਂ ਜੇ (len > 128) {
         stbi_uc   val;
         // Next -len+1 bytes in the dest are replicated from next source byte.
         // (Interpret len as a negative 8-bit int.)
         len = 257 - len;
         ਜੇ (len > nleft) ਵਾਪਸ 0; // corrupt data
         val = stbi__get8(s);
         ਗਿਣਤੀ_2 += len;
         ਜਦੋਂ (len) {
            *p = val;
            p += 4;
            len--;
         }
      }
   }

   ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ *stbi_psd_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri, ਪੂਰਨ bpc)
{
   ਪੂਰਨ pixel_ਗਿਣਤੀ_2;
   ਪੂਰਨ channel_ਗਿਣਤੀ, compression;
   ਪੂਰਨ channel, i;
   ਪੂਰਨ bitdepth;
   ਪੂਰਨ w,h;
   stbi_uc *ਬਾਹਰਲਾ;
   STBI_NOTUSED(ri);

   // Check identifier
   ਜੇ (stbi__get32be(s) != 0x38425053)   // "8BPS"
      ਵਾਪਸ stbi__errpuc("not PSD", "Corrupt PSD image");

   // Check file type version.
   ਜੇ (stbi__get16be(s) != 1)
      ਵਾਪਸ stbi__errpuc("wrong version", "Unsupported version of PSD image");

   // Skip 6 reserved bytes.
   stbi__skip(s, 6 );

   // Read the number of channels (R, G, B, A, etc).
   channel_ਗਿਣਤੀ = stbi__get16be(s);
   ਜੇ (channel_ਗਿਣਤੀ < 0 || channel_ਗਿਣਤੀ > 16)
      ਵਾਪਸ stbi__errpuc("wrong channel count", "Unsupported number of channels in PSD image");

   // Read the rows and columns of the image.
   h = stbi__get32be(s);
   w = stbi__get32be(s);

   // Make sure the depth is 8 bits.
   bitdepth = stbi__get16be(s);
   ਜੇ (bitdepth != 8 && bitdepth != 16)
      ਵਾਪਸ stbi__errpuc("unsupported bit depth", "PSD bit depth is not 8 or 16 bit");

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
   ਜੇ (stbi__get16be(s) != 3)
      ਵਾਪਸ stbi__errpuc("wrong color format", "PSD is not in RGB color format");

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
   ਜੇ (compression > 1)
      ਵਾਪਸ stbi__errpuc("bad compression", "PSD has an unknown compression format");

   // Check size
   ਜੇ (!stbi__mad3sizes_valid(4, w, h, 0))
      ਵਾਪਸ stbi__errpuc("too large", "Corrupt PSD");

   // Create the destination image.

   ਜੇ (!compression && bitdepth == 16 && bpc == 16) {
      ਬਾਹਰਲਾ = (stbi_uc *) stbi__malloc_mad3(8, w, h, 0);
      ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel = 16;
   } ਨਹੀਂਤਾਂ
      ਬਾਹਰਲਾ = (stbi_uc *) stbi__malloc(4 * w*h);

   ਜੇ (!ਬਾਹਰਲਾ) ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");
   pixel_ਗਿਣਤੀ_2 = w*h;

   // Initialize the data to zero.
   //memset( out, 0, pixelCount * 4 );

   // Finally, the image data.
   ਜੇ (compression) {
      // RLE as used by .PSD and .TIFF
      // Loop until you get the number of unpacked bytes you are expecting:
      //     Read the next source byte into n.
      //     If n is between 0 and 127 inclusive, copy the next n+1 bytes literally.
      //     Else if n is between -127 and -1 inclusive, copy the next byte -n+1 times.
      //     Else if n is 128, noop.
      // Endloop

      // The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
      // which we're going to just skip.
      stbi__skip(s, h * channel_ਗਿਣਤੀ * 2 );

      // Read the RLE data by channel.
      ਲਈ (channel = 0; channel < 4; channel++) {
         stbi_uc *p;

         p = ਬਾਹਰਲਾ+channel;
         ਜੇ (channel >= channel_ਗਿਣਤੀ) {
            // Fill this channel with default data.
            ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ_2; i++, p += 4)
               *p = (channel == 3 ? 255 : 0);
         } ਨਹੀਂਤਾਂ {
            // Read the RLE data.
            ਜੇ (!stbi__psd_decode_rle(s, p, pixel_ਗਿਣਤੀ_2)) {
               STBI_FREE(ਬਾਹਰਲਾ);
               ਵਾਪਸ stbi__errpuc("corrupt", "bad RLE data");
            }
         }
      }

   } ਨਹੀਂਤਾਂ {
      // We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
      // where each channel consists of an 8-bit (or 16-bit) value for each pixel in the image.

      // Read the data by channel.
      ਲਈ (channel = 0; channel < 4; channel++) {
         ਜੇ (channel >= channel_ਗਿਣਤੀ) {
            // Fill this channel with default data.
            ਜੇ (bitdepth == 16 && bpc == 16) {
               stbi__uint16 *q = ((stbi__uint16 *) ਬਾਹਰਲਾ) + channel;
               stbi__uint16 val = channel == 3 ? 65535 : 0;
               ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ_2; i++, q += 4)
                  *q = val;
            } ਨਹੀਂਤਾਂ {
               stbi_uc *p = ਬਾਹਰਲਾ+channel;
               stbi_uc val = channel == 3 ? 255 : 0;
               ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ_2; i++, p += 4)
                  *p = val;
            }
         } ਨਹੀਂਤਾਂ {
            ਜੇ (ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel == 16) {    // output bpc
               stbi__uint16 *q = ((stbi__uint16 *) ਬਾਹਰਲਾ) + channel;
               ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ_2; i++, q += 4)
                  *q = (stbi__uint16) stbi__get16be(s);
            } ਨਹੀਂਤਾਂ {
               stbi_uc *p = ਬਾਹਰਲਾ+channel;
               ਜੇ (bitdepth == 16) {  // input bpc
                  ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ_2; i++, p += 4)
                     *p = (stbi_uc) (stbi__get16be(s) >> 8);
               } ਨਹੀਂਤਾਂ {
                  ਲਈ (i = 0; i < pixel_ਗਿਣਤੀ_2; i++, p += 4)
                     *p = stbi__get8(s);
               }
            }
         }
      }
   }

   // remove weird white matte from PSD
   ਜੇ (channel_ਗਿਣਤੀ >= 4) {
      ਜੇ (ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel == 16) {
         ਲਈ (i=0; i < w*h; ++i) {
            stbi__uint16 *pixel = (stbi__uint16 *) ਬਾਹਰਲਾ + 4*i;
            ਜੇ (pixel[3] != 0 && pixel[3] != 65535) {
               ਦਸ਼ਮਲਵ a = pixel[3] / 65535.0f;
               ਦਸ਼ਮਲਵ ra = 1.0f / a;
               ਦਸ਼ਮਲਵ inv_a = 65535.0f * (1 - ra);
               pixel[0] = (stbi__uint16) (pixel[0]*ra + inv_a);
               pixel[1] = (stbi__uint16) (pixel[1]*ra + inv_a);
               pixel[2] = (stbi__uint16) (pixel[2]*ra + inv_a);
            }
         }
      } ਨਹੀਂਤਾਂ {
         ਲਈ (i=0; i < w*h; ++i) {
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pixel = ਬਾਹਰਲਾ + 4*i;
            ਜੇ (pixel[3] != 0 && pixel[3] != 255) {
               ਦਸ਼ਮਲਵ a = pixel[3] / 255.0f;
               ਦਸ਼ਮਲਵ ra = 1.0f / a;
               ਦਸ਼ਮਲਵ inv_a = 255.0f * (1 - ra);
               pixel[0] = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (pixel[0]*ra + inv_a);
               pixel[1] = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (pixel[1]*ra + inv_a);
               pixel[2] = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (pixel[2]*ra + inv_a);
            }
         }
      }
   }

   // convert to desired output format
   ਜੇ (req_comp && req_comp != 4) {
      ਜੇ (ri->ਬਿੱਟ_ਪ੍ਰਤੀ_channel == 16)
         ਬਾਹਰਲਾ = (stbi_uc *) stbi__convert_format16((stbi__uint16 *) ਬਾਹਰਲਾ, 4, req_comp, w, h);
      ਨਹੀਂਤਾਂ
         ਬਾਹਰਲਾ = stbi__convert_format(ਬਾਹਰਲਾ, 4, req_comp, w, h);
      ਜੇ (ਬਾਹਰਲਾ == ਨਿਰਾ) ਵਾਪਸ ਬਾਹਰਲਾ; // stbi__convert_format frees input on failure
   }

   ਜੇ (comp) *comp = 4;
   *y = h;
   *x = w;

   ਵਾਪਸ ਬਾਹਰਲਾ;
}
#ਜੇ_ਅੰਤ

// *************************************************************************************************
// Softimage PIC loader
// by Tom Seddon
//
// See http://softimage.wiki.softimage.com/index.php/INFO:_PIC_file_format
// See http://ozviz.wasp.uwa.edu.au/~pbourke/dataformats/softimagepic/

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PIC
ਸਥਿਰ ਪੂਰਨ stbi__pic_is4(stbi__context *s,ਅਚਲ ਅੱਖਰ *str)
{
   ਪੂਰਨ i;
   ਲਈ (i=0; i<4; ++i)
      ਜੇ (stbi__get8(s) != (stbi_uc)str[i])
         ਵਾਪਸ 0;

   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__pic_test_core(stbi__context *s)
{
   ਪੂਰਨ i;

   ਜੇ (!stbi__pic_is4(s,"\x53\x80\xF6\x34"))
      ਵਾਪਸ 0;

   ਲਈ(i=0;i<84;++i)
      stbi__get8(s);

   ਜੇ (!stbi__pic_is4(s,"PICT"))
      ਵਾਪਸ 0;

   ਵਾਪਸ 1;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi_uc ਮਾਪ,type,channel;
} stbi__pic_packet;

ਸਥਿਰ stbi_uc *stbi__readval(stbi__context *s, ਪੂਰਨ channel, stbi_uc *dest)
{
   ਪੂਰਨ ਨਕਾਬ=0x80, i;

   ਲਈ (i=0; i<4; ++i, ਨਕਾਬ>>=1) {
      ਜੇ (channel & ਨਕਾਬ) {
         ਜੇ (stbi__at_eof(s)) ਵਾਪਸ stbi__errpuc("bad file","PIC file too short");
         dest[i]=stbi__get8(s);
      }
   }

   ਵਾਪਸ dest;
}

ਸਥਿਰ ਖਾਲੀ stbi__copyval(ਪੂਰਨ channel,stbi_uc *dest,ਅਚਲ stbi_uc *src)
{
   ਪੂਰਨ ਨਕਾਬ=0x80,i;

   ਲਈ (i=0;i<4; ++i, ਨਕਾਬ>>=1)
      ਜੇ (channel&ਨਕਾਬ)
         dest[i]=src[i];
}

ਸਥਿਰ stbi_uc *stbi_pic_ਲੱਦੋ_core(stbi__context *s,ਪੂਰਨ ਚੌੜਾਈ,ਪੂਰਨ ਉਚਾਈ,ਪੂਰਨ *comp, stbi_uc *ਨਤੀਜਾ)
{
   ਪੂਰਨ act_comp=0,ਅੰਕ_packets=0,y,chained;
   stbi__pic_packet packets[10];

   // this will (should...) cater for even some bizarre stuff like having data
    // for the same channel in multiple packets.
   ਕਰੋ {
      stbi__pic_packet *packet;

      ਜੇ (ਅੰਕ_packets==ਆਕਾਰ(packets)/ਆਕਾਰ(packets[0]))
         ਵਾਪਸ stbi__errpuc("bad format","too many packets");

      packet = &packets[ਅੰਕ_packets++];

      chained = stbi__get8(s);
      packet->ਮਾਪ    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);

      act_comp |= packet->channel;

      ਜੇ (stbi__at_eof(s))          ਵਾਪਸ stbi__errpuc("bad file","file too short (reading packets)");
      ਜੇ (packet->ਮਾਪ != 8)  ਵਾਪਸ stbi__errpuc("bad format","packet isn't 8bpp");
   } ਜਦੋਂ (chained);

   *comp = (act_comp & 0x10 ? 4 : 3); // has alpha channel?

   ਲਈ(y=0; y<ਉਚਾਈ; ++y) {
      ਪੂਰਨ packet_idx;

      ਲਈ(packet_idx=0; packet_idx < ਅੰਕ_packets; ++packet_idx) {
         stbi__pic_packet *packet = &packets[packet_idx];
         stbi_uc *dest = ਨਤੀਜਾ+y*ਚੌੜਾਈ*4;

         ਚੋਣ (packet->type) {
            ਮੂਲ:
               ਵਾਪਸ stbi__errpuc("bad format","packet has bad compression type");

            ਹਾਲਤ 0: {//uncompressed
               ਪੂਰਨ x;

               ਲਈ(x=0;x<ਚੌੜਾਈ;++x, dest+=4)
                  ਜੇ (!stbi__readval(s,packet->channel,dest))
                     ਵਾਪਸ 0;
               ਰੋਕੋ;
            }

            ਹਾਲਤ 1://Pure RLE
               {
                  ਪੂਰਨ ਖੱਬਾ=ਚੌੜਾਈ, i;

                  ਜਦੋਂ (ਖੱਬਾ>0) {
                     stbi_uc ਗਿਣਤੀ_2,ਮੁੱਲ[4];

                     ਗਿਣਤੀ_2=stbi__get8(s);
                     ਜੇ (stbi__at_eof(s))   ਵਾਪਸ stbi__errpuc("bad file","file too short (pure read count)");

                     ਜੇ (ਗਿਣਤੀ_2 > ਖੱਬਾ)
                        ਗਿਣਤੀ_2 = (stbi_uc) ਖੱਬਾ;

                     ਜੇ (!stbi__readval(s,packet->channel,ਮੁੱਲ))  ਵਾਪਸ 0;

                     ਲਈ(i=0; i<ਗਿਣਤੀ_2; ++i,dest+=4)
                        stbi__copyval(packet->channel,dest,ਮੁੱਲ);
                     ਖੱਬਾ -= ਗਿਣਤੀ_2;
                  }
               }
               ਰੋਕੋ;

            ਹਾਲਤ 2: {//Mixed RLE
               ਪੂਰਨ ਖੱਬਾ=ਚੌੜਾਈ;
               ਜਦੋਂ (ਖੱਬਾ>0) {
                  ਪੂਰਨ ਗਿਣਤੀ_2 = stbi__get8(s), i;
                  ਜੇ (stbi__at_eof(s))  ਵਾਪਸ stbi__errpuc("bad file","file too short (mixed read count)");

                  ਜੇ (ਗਿਣਤੀ_2 >= 128) { // Repeated
                     stbi_uc ਮੁੱਲ[4];

                     ਜੇ (ਗਿਣਤੀ_2==128)
                        ਗਿਣਤੀ_2 = stbi__get16be(s);
                     ਨਹੀਂਤਾਂ
                        ਗਿਣਤੀ_2 -= 127;
                     ਜੇ (ਗਿਣਤੀ_2 > ਖੱਬਾ)
                        ਵਾਪਸ stbi__errpuc("bad file","scanline overrun");

                     ਜੇ (!stbi__readval(s,packet->channel,ਮੁੱਲ))
                        ਵਾਪਸ 0;

                     ਲਈ(i=0;i<ਗਿਣਤੀ_2;++i, dest += 4)
                        stbi__copyval(packet->channel,dest,ਮੁੱਲ);
                  } ਨਹੀਂਤਾਂ { // Raw
                     ++ਗਿਣਤੀ_2;
                     ਜੇ (ਗਿਣਤੀ_2>ਖੱਬਾ) ਵਾਪਸ stbi__errpuc("bad file","scanline overrun");

                     ਲਈ(i=0;i<ਗਿਣਤੀ_2;++i, dest+=4)
                        ਜੇ (!stbi__readval(s,packet->channel,dest))
                           ਵਾਪਸ 0;
                  }
                  ਖੱਬਾ-=ਗਿਣਤੀ_2;
               }
               ਰੋਕੋ;
            }
         }
      }
   }

   ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ *stbi_pic_ਲੱਦੋ(stbi__context *s,ਪੂਰਨ *px,ਪੂਰਨ *py,ਪੂਰਨ *comp,ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   stbi_uc *ਨਤੀਜਾ;
   ਪੂਰਨ i, x,y, ਅੰਦਰੂਨੀ_comp;
   STBI_NOTUSED(ri);

   ਜੇ (!comp) comp = &ਅੰਦਰੂਨੀ_comp;

   ਲਈ (i=0; i<92; ++i)
      stbi__get8(s);

   x = stbi__get16be(s);
   y = stbi__get16be(s);
   ਜੇ (stbi__at_eof(s))  ਵਾਪਸ stbi__errpuc("bad file","file too short (pic header)");
   ਜੇ (!stbi__mad3sizes_valid(x, y, 4, 0)) ਵਾਪਸ stbi__errpuc("too large", "PIC image too large to decode");

   stbi__get32be(s); //skip `ratio'
   stbi__get16be(s); //skip `fields'
   stbi__get16be(s); //skip `pad'

   // intermediate buffer is RGBA
   ਨਤੀਜਾ = (stbi_uc *) stbi__malloc_mad3(x, y, 4, 0);
   memset(ਨਤੀਜਾ, 0xff, x*y*4);

   ਜੇ (!stbi_pic_ਲੱਦੋ_core(s,x,y,comp, ਨਤੀਜਾ)) {
      STBI_FREE(ਨਤੀਜਾ);
      ਨਤੀਜਾ=0;
   }
   *px = x;
   *py = y;
   ਜੇ (req_comp == 0) req_comp = *comp;
   ਨਤੀਜਾ=stbi__convert_format(ਨਤੀਜਾ,4,req_comp,x,y);

   ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ stbi__pic_test(stbi__context *s)
{
   ਪੂਰਨ r = stbi__pic_test_core(s);
   stbi__rewind(s);
   ਵਾਪਸ r;
}
#ਜੇ_ਅੰਤ

// *************************************************************************************************
// GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_GIF
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   stbi__int16 ਅਗੇਤਰ;
   stbi_uc ਪਹਿਲਾ;
   stbi_uc suffix;
} stbi__gif_lzw;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
   ਪੂਰਨ w,h;
   stbi_uc *ਬਾਹਰਲਾ;                 // output buffer (always 4 components)
   stbi_uc *background;          // The current "background" as far as a gif is concerned
   stbi_uc *history; 
   ਪੂਰਨ flags, bgindex, ਅਨੁਪਾਤ, transparent, eflags;
   stbi_uc  pal[256][4];
   stbi_uc lpal[256][4];
   stbi__gif_lzw codes[8192];
   stbi_uc *color_ਸਾਰਣੀ;
   ਪੂਰਨ parse, ਕਦਮ;
   ਪੂਰਨ lflags;
   ਪੂਰਨ ਸ਼ੁਰੂ_x, ਸ਼ੁਰੂ_y;
   ਪੂਰਨ max_x, max_y;
   ਪੂਰਨ cur_x, cur_y;
   ਪੂਰਨ ਸਤਰ_ਮਾਪ;
   ਪੂਰਨ delay;
} stbi__gif;

ਸਥਿਰ ਪੂਰਨ stbi__gif_test_raw(stbi__context *s)
{
   ਪੂਰਨ sz;
   ਜੇ (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8') ਵਾਪਸ 0;
   sz = stbi__get8(s);
   ਜੇ (sz != '9' && sz != '7') ਵਾਪਸ 0;
   ਜੇ (stbi__get8(s) != 'a') ਵਾਪਸ 0;
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__gif_test(stbi__context *s)
{
   ਪੂਰਨ r = stbi__gif_test_raw(s);
   stbi__rewind(s);
   ਵਾਪਸ r;
}

ਸਥਿਰ ਖਾਲੀ stbi__gif_parse_colortable(stbi__context *s, stbi_uc pal[256][4], ਪੂਰਨ ਅੰਕ_entries, ਪੂਰਨ transp)
{
   ਪੂਰਨ i;
   ਲਈ (i=0; i < ਅੰਕ_entries; ++i) {
      pal[i][2] = stbi__get8(s);
      pal[i][1] = stbi__get8(s);
      pal[i][0] = stbi__get8(s);
      pal[i][3] = transp == i ? 0 : 255;
   }
}

ਸਥਿਰ ਪੂਰਨ stbi_gif_ਸਿਰਲੇਖ(stbi__context *s, stbi__gif *g, ਪੂਰਨ *comp, ਪੂਰਨ is_info)
{
   stbi_uc ਸੰਸਕਰਣ;
   ਜੇ (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8')
      ਵਾਪਸ stbi__err("not GIF", "Corrupt GIF");

   ਸੰਸਕਰਣ = stbi__get8(s);
   ਜੇ (ਸੰਸਕਰਣ != '7' && ਸੰਸਕਰਣ != '9')    ਵਾਪਸ stbi__err("not GIF", "Corrupt GIF");
   ਜੇ (stbi__get8(s) != 'a')                ਵਾਪਸ stbi__err("not GIF", "Corrupt GIF");

   stbi__g_failure_reason = "";
   g->w = stbi__get16le(s);
   g->h = stbi__get16le(s);
   g->flags = stbi__get8(s);
   g->bgindex = stbi__get8(s);
   g->ਅਨੁਪਾਤ = stbi__get8(s);
   g->transparent = -1;

   ਜੇ (comp != 0) *comp = 4;  // can't actually tell whether it's 3 or 4 until we parse the comments

   ਜੇ (is_info) ਵਾਪਸ 1;

   ਜੇ (g->flags & 0x80)
      stbi__gif_parse_colortable(s,g->pal, 2 << (g->flags & 7), -1);

   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__gif_info_raw(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   stbi__gif* g = (stbi__gif*) stbi__malloc(ਆਕਾਰ(stbi__gif));
   ਜੇ (!stbi_gif_ਸਿਰਲੇਖ(s, g, comp, 1)) {
      STBI_FREE(g);
      stbi__rewind( s );
      ਵਾਪਸ 0;
   }
   ਜੇ (x) *x = g->w;
   ਜੇ (y) *y = g->h;
   STBI_FREE(g);
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ stbi_ਬਾਹਰਲਾ_gif_ਕੋਡ(stbi__gif *g, stbi__uint16 ਕੋਡ)
{
   stbi_uc *p, *c;
   ਪੂਰਨ idx; 

   // recurse to decode the prefixes, since the linked-list is backwards,
   // and working backwards through an interleaved image would be nasty
   ਜੇ (g->codes[ਕੋਡ].ਅਗੇਤਰ >= 0)
      stbi_ਬਾਹਰਲਾ_gif_ਕੋਡ(g, g->codes[ਕੋਡ].ਅਗੇਤਰ);

   ਜੇ (g->cur_y >= g->max_y) ਵਾਪਸ;

   idx = g->cur_x + g->cur_y; 
   p = &g->ਬਾਹਰਲਾ[idx];
   g->history[idx / 4] = 1;  

   c = &g->color_ਸਾਰਣੀ[g->codes[ਕੋਡ].suffix * 4];
   ਜੇ (c[3] > 128) { // don't render transparent pixels; 
      p[0] = c[2];
      p[1] = c[1];
      p[2] = c[0];
      p[3] = c[3];
   }
   g->cur_x += 4;

   ਜੇ (g->cur_x >= g->max_x) {
      g->cur_x = g->ਸ਼ੁਰੂ_x;
      g->cur_y += g->ਕਦਮ;

      ਜਦੋਂ (g->cur_y >= g->max_y && g->parse > 0) {
         g->ਕਦਮ = (1 << g->parse) * g->ਸਤਰ_ਮਾਪ;
         g->cur_y = g->ਸ਼ੁਰੂ_y + (g->ਕਦਮ >> 1);
         --g->parse;
      }
   }
}

ਸਥਿਰ stbi_uc *stbi__process_gif_raster(stbi__context *s, stbi__gif *g)
{
   stbi_uc lzw_cs;
   stbi__int32 len, init_ਕੋਡ;
   stbi__uint32 ਪਹਿਲਾ;
   stbi__int32 codesize, codemask, avail, oldcode, ਬਿੱਟ, valid_ਬਿੱਟ, clear;
   stbi__gif_lzw *p;

   lzw_cs = stbi__get8(s);
   ਜੇ (lzw_cs > 12) ਵਾਪਸ ਨਿਰਾ;
   clear = 1 << lzw_cs;
   ਪਹਿਲਾ = 1;
   codesize = lzw_cs + 1;
   codemask = (1 << codesize) - 1;
   ਬਿੱਟ = 0;
   valid_ਬਿੱਟ = 0;
   ਲਈ (init_ਕੋਡ = 0; init_ਕੋਡ < clear; init_ਕੋਡ++) {
      g->codes[init_ਕੋਡ].ਅਗੇਤਰ = -1;
      g->codes[init_ਕੋਡ].ਪਹਿਲਾ = (stbi_uc) init_ਕੋਡ;
      g->codes[init_ਕੋਡ].suffix = (stbi_uc) init_ਕੋਡ;
   }

   // support no starting clear code
   avail = clear+2;
   oldcode = -1;

   len = 0;
   ਲਈ(;;) {
      ਜੇ (valid_ਬਿੱਟ < codesize) {
         ਜੇ (len == 0) {
            len = stbi__get8(s); // start new block
            ਜੇ (len == 0)
               ਵਾਪਸ g->ਬਾਹਰਲਾ;
         }
         --len;
         ਬਿੱਟ |= (stbi__int32) stbi__get8(s) << valid_ਬਿੱਟ;
         valid_ਬਿੱਟ += 8;
      } ਨਹੀਂਤਾਂ {
         stbi__int32 ਕੋਡ = ਬਿੱਟ & codemask;
         ਬਿੱਟ >>= codesize;
         valid_ਬਿੱਟ -= codesize;
         // @OPTIMIZE: is there some way we can accelerate the non-clear path?
         ਜੇ (ਕੋਡ == clear) {  // clear code
            codesize = lzw_cs + 1;
            codemask = (1 << codesize) - 1;
            avail = clear + 2;
            oldcode = -1;
            ਪਹਿਲਾ = 0;
         } ਨਹੀਂਤਾਂ ਜੇ (ਕੋਡ == clear + 1) { // end of stream code
            stbi__skip(s, len);
            ਜਦੋਂ ((len = stbi__get8(s)) > 0)
               stbi__skip(s,len);
            ਵਾਪਸ g->ਬਾਹਰਲਾ;
         } ਨਹੀਂਤਾਂ ਜੇ (ਕੋਡ <= avail) {
            ਜੇ (ਪਹਿਲਾ) {
               ਵਾਪਸ stbi__errpuc("no clear code", "Corrupt GIF");
            }

            ਜੇ (oldcode >= 0) {
               p = &g->codes[avail++];
               ਜੇ (avail > 8192) {
                  ਵਾਪਸ stbi__errpuc("too many codes", "Corrupt GIF");
               }

               p->ਅਗੇਤਰ = (stbi__int16) oldcode;
               p->ਪਹਿਲਾ = g->codes[oldcode].ਪਹਿਲਾ;
               p->suffix = (ਕੋਡ == avail) ? p->ਪਹਿਲਾ : g->codes[ਕੋਡ].ਪਹਿਲਾ;
            } ਨਹੀਂਤਾਂ ਜੇ (ਕੋਡ == avail)
               ਵਾਪਸ stbi__errpuc("illegal code in raster", "Corrupt GIF");

            stbi_ਬਾਹਰਲਾ_gif_ਕੋਡ(g, (stbi__uint16) ਕੋਡ);

            ਜੇ ((avail & codemask) == 0 && avail <= 0x0FFF) {
               codesize++;
               codemask = (1 << codesize) - 1;
            }

            oldcode = ਕੋਡ;
         } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ stbi__errpuc("illegal code in raster", "Corrupt GIF");
         }
      }
   }
}

// this function is designed to support animated gifs, although stb_image doesn't support it
// two back is the image from two frames ago, used for a very specific disposal format
ਸਥਿਰ stbi_uc *stbi_gif_ਲੱਦੋ_ਅਗਲਾ(stbi__context *s, stbi__gif *g, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_uc *two_ਪਿੱਠ)
{
   ਪੂਰਨ dispose; 
   ਪੂਰਨ ਪਹਿਲਾ_ਚੌਖਟਾ; 
   ਪੂਰਨ pi; 
   ਪੂਰਨ pcount; 

   // on first frame, any non-written pixels get the background colour (non-transparent)
   ਪਹਿਲਾ_ਚੌਖਟਾ = 0; 
   ਜੇ (g->ਬਾਹਰਲਾ == 0) {
      ਜੇ (!stbi_gif_ਸਿਰਲੇਖ(s, g, comp,0))     ਵਾਪਸ 0; // stbi__g_failure_reason set by stbi__gif_header
      g->ਬਾਹਰਲਾ = (stbi_uc *) stbi__malloc(4 * g->w * g->h);
      g->background = (stbi_uc *) stbi__malloc(4 * g->w * g->h); 
      g->history = (stbi_uc *) stbi__malloc(g->w * g->h); 
      ਜੇ (g->ਬਾਹਰਲਾ == 0)                      ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");

      // image is treated as "tranparent" at the start - ie, nothing overwrites the current background; 
      // background colour is only used for pixels that are not rendered first frame, after that "background"
      // color refers to teh color that was there the previous frame. 
      memset( g->ਬਾਹਰਲਾ, 0x00, 4 * g->w * g->h ); 
      memset( g->background, 0x00, 4 * g->w * g->h ); // state of the background (starts transparent)
      memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame
      ਪਹਿਲਾ_ਚੌਖਟਾ = 1; 
   } ਨਹੀਂਤਾਂ {
      // second frame - how do we dispoase of the previous one?
      dispose = (g->eflags & 0x1C) >> 2; 
      pcount = g->w * g->h; 

      ਜੇ ((dispose == 3) && (two_ਪਿੱਠ == 0)) {
         dispose = 2; // if I don't have an image to revert back to, default to the old background
      }

      ਜੇ (dispose == 3) { // use previous graphic
         ਲਈ (pi = 0; pi < pcount; ++pi) {
            ਜੇ (g->history[pi]) {
               ਸਮ੍ਰਿਤੀ_ਨਕਲ( &g->ਬਾਹਰਲਾ[pi * 4], &two_ਪਿੱਠ[pi * 4], 4 ); 
            }
         }
      } ਨਹੀਂਤਾਂ ਜੇ (dispose == 2) { 
         // restore what was changed last frame to background before that frame; 
         ਲਈ (pi = 0; pi < pcount; ++pi) {
            ਜੇ (g->history[pi]) {
               ਸਮ੍ਰਿਤੀ_ਨਕਲ( &g->ਬਾਹਰਲਾ[pi * 4], &g->background[pi * 4], 4 ); 
            }
         }
      } ਨਹੀਂਤਾਂ {
         // This is a non-disposal case eithe way, so just 
         // leave the pixels as is, and they will become the new background
         // 1: do not dispose
         // 0:  not specified.
      }

      // background is what out is after the undoing of the previou frame; 
      ਸਮ੍ਰਿਤੀ_ਨਕਲ( g->background, g->ਬਾਹਰਲਾ, 4 * g->w * g->h ); 
   }

   // clear my history; 
   memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame

   ਲਈ (;;) {
      ਪੂਰਨ ਟੈਗ = stbi__get8(s); 
      ਚੋਣ (ਟੈਗ) {
         ਹਾਲਤ 0x2C: /* Image Descriptor */
         {
            stbi__int32 x, y, w, h;
            stbi_uc *o;

            x = stbi__get16le(s);
            y = stbi__get16le(s);
            w = stbi__get16le(s);
            h = stbi__get16le(s);
            ਜੇ (((x + w) > (g->w)) || ((y + h) > (g->h)))
               ਵਾਪਸ stbi__errpuc("bad Image Descriptor", "Corrupt GIF");

            g->ਸਤਰ_ਮਾਪ = g->w * 4;
            g->ਸ਼ੁਰੂ_x = x * 4;
            g->ਸ਼ੁਰੂ_y = y * g->ਸਤਰ_ਮਾਪ;
            g->max_x   = g->ਸ਼ੁਰੂ_x + w * 4;
            g->max_y   = g->ਸ਼ੁਰੂ_y + h * g->ਸਤਰ_ਮਾਪ;
            g->cur_x   = g->ਸ਼ੁਰੂ_x;
            g->cur_y   = g->ਸ਼ੁਰੂ_y;

            g->lflags = stbi__get8(s);

            ਜੇ (g->lflags & 0x40) {
               g->ਕਦਮ = 8 * g->ਸਤਰ_ਮਾਪ; // first interlaced spacing
               g->parse = 3;
            } ਨਹੀਂਤਾਂ {
               g->ਕਦਮ = g->ਸਤਰ_ਮਾਪ;
               g->parse = 0;
            }

            ਜੇ (g->lflags & 0x80) {
               stbi__gif_parse_colortable(s,g->lpal, 2 << (g->lflags & 7), g->eflags & 0x01 ? g->transparent : -1);
               g->color_ਸਾਰਣੀ = (stbi_uc *) g->lpal;
            } ਨਹੀਂਤਾਂ ਜੇ (g->flags & 0x80) {
               g->color_ਸਾਰਣੀ = (stbi_uc *) g->pal;
            } ਨਹੀਂਤਾਂ
               ਵਾਪਸ stbi__errpuc("missing color table", "Corrupt GIF");            
            
            o = stbi__process_gif_raster(s, g);
            ਜੇ (o == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

            // if this was the first frame, 
            pcount = g->w * g->h; 
            ਜੇ (ਪਹਿਲਾ_ਚੌਖਟਾ && (g->bgindex > 0)) {
               // if first frame, any pixel not drawn to gets the background color
               ਲਈ (pi = 0; pi < pcount; ++pi) {
                  ਜੇ (g->history[pi] == 0) {
                     g->pal[g->bgindex][3] = 255; // just in case it was made transparent, undo that; It will be reset next frame if need be; 
                     ਸਮ੍ਰਿਤੀ_ਨਕਲ( &g->ਬਾਹਰਲਾ[pi * 4], &g->pal[g->bgindex], 4 ); 
                  }
               }
            }

            ਵਾਪਸ o;
         }

         ਹਾਲਤ 0x21: // Comment Extension.
         {
            ਪੂਰਨ len;
            ਪੂਰਨ ext = stbi__get8(s); 
            ਜੇ (ext == 0xF9) { // Graphic Control Extension.
               len = stbi__get8(s);
               ਜੇ (len == 4) {
                  g->eflags = stbi__get8(s);
                  g->delay = 10 * stbi__get16le(s); // delay - 1/100th of a second, saving as 1/1000ths.

                  // unset old transparent
                  ਜੇ (g->transparent >= 0) {
                     g->pal[g->transparent][3] = 255; 
                  } 
                  ਜੇ (g->eflags & 0x01) {
                     g->transparent = stbi__get8(s);
                     ਜੇ (g->transparent >= 0) {
                        g->pal[g->transparent][3] = 0; 
                     }
                  } ਨਹੀਂਤਾਂ {
                     // don't need transparent
                     stbi__skip(s, 1); 
                     g->transparent = -1; 
                  }
               } ਨਹੀਂਤਾਂ {
                  stbi__skip(s, len);
                  ਰੋਕੋ;
               }
            } 
            ਜਦੋਂ ((len = stbi__get8(s)) != 0) {
               stbi__skip(s, len);
            }
            ਰੋਕੋ;
         }

         ਹਾਲਤ 0x3B: // gif stream termination code
            ਵਾਪਸ (stbi_uc *) s; // using '1' causes warning on some compilers

         ਮੂਲ:
            ਵਾਪਸ stbi__errpuc("unknown code", "Corrupt GIF");
      }
   }
}

ਸਥਿਰ ਖਾਲੀ *stbi_ਲੱਦੋ_gif_main(stbi__context *s, ਪੂਰਨ **delays, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *z, ਪੂਰਨ *comp, ਪੂਰਨ req_comp)
{
   ਜੇ (stbi__gif_test(s)) {
      ਪੂਰਨ layers = 0; 
      stbi_uc *u = 0;
      stbi_uc *ਬਾਹਰਲਾ = 0;
      stbi_uc *two_ਪਿੱਠ = 0; 
      stbi__gif g;
      ਪੂਰਨ stride; 
      memset(&g, 0, ਆਕਾਰ(g));
      ਜੇ (delays) {
         *delays = 0; 
      }

      ਕਰੋ {
         u = stbi_gif_ਲੱਦੋ_ਅਗਲਾ(s, &g, comp, req_comp, two_ਪਿੱਠ);
         ਜੇ (u == (stbi_uc *) s) u = 0;  // end of animated gif marker

         ਜੇ (u) {
            *x = g.w;
            *y = g.h;
            ++layers; 
            stride = g.w * g.h * 4; 
         
            ਜੇ (ਬਾਹਰਲਾ) {
               ਬਾਹਰਲਾ = (stbi_uc*) STBI_REALLOC( ਬਾਹਰਲਾ, layers * stride ); 
               ਜੇ (delays) {
                  *delays = (ਪੂਰਨ*) STBI_REALLOC( *delays, ਆਕਾਰ(ਪੂਰਨ) * layers ); 
               }
            } ਨਹੀਂਤਾਂ {
               ਬਾਹਰਲਾ = (stbi_uc*)stbi__malloc( layers * stride ); 
               ਜੇ (delays) {
                  *delays = (ਪੂਰਨ*) stbi__malloc( layers * ਆਕਾਰ(ਪੂਰਨ) ); 
               }
            }
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ਬਾਹਰਲਾ + ((layers - 1) * stride), u, stride ); 
            ਜੇ (layers >= 2) {
               two_ਪਿੱਠ = ਬਾਹਰਲਾ - 2 * stride; 
            }

            ਜੇ (delays) {
               (*delays)[layers - 1U] = g.delay; 
            }
         }
      } ਜਦੋਂ (u != 0); 

      // free temp buffer; 
      STBI_FREE(g.ਬਾਹਰਲਾ); 
      STBI_FREE(g.history); 
      STBI_FREE(g.background); 

      // do the final conversion after loading everything; 
      ਜੇ (req_comp && req_comp != 4)
         ਬਾਹਰਲਾ = stbi__convert_format(ਬਾਹਰਲਾ, 4, req_comp, layers * g.w, g.h);

      *z = layers; 
      ਵਾਪਸ ਬਾਹਰਲਾ;
   } ਨਹੀਂਤਾਂ {
      ਵਾਪਸ stbi__errpuc("not GIF", "Image was not as a gif type."); 
   }
}

ਸਥਿਰ ਖਾਲੀ *stbi_gif_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   stbi_uc *u = 0;
   stbi__gif g;
   memset(&g, 0, ਆਕਾਰ(g));

   u = stbi_gif_ਲੱਦੋ_ਅਗਲਾ(s, &g, comp, req_comp, 0);
   ਜੇ (u == (stbi_uc *) s) u = 0;  // end of animated gif marker
   ਜੇ (u) {
      *x = g.w;
      *y = g.h;

      // moved conversion to after successful load so that the same
      // can be done for multiple frames. 
      ਜੇ (req_comp && req_comp != 4)
         u = stbi__convert_format(u, 4, req_comp, g.w, g.h);
   }

   // free buffers needed for multiple frame loading; 
   STBI_FREE(g.history);
   STBI_FREE(g.background); 

   ਵਾਪਸ u;
}

ਸਥਿਰ ਪੂਰਨ stbi__gif_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਵਾਪਸ stbi__gif_info_raw(s,x,y,comp);
}
#ਜੇ_ਅੰਤ

// *************************************************************************************************
// Radiance RGBE HDR loader
// originally by Nicolas Schulz
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
ਸਥਿਰ ਪੂਰਨ stbi__hdr_test_core(stbi__context *s, ਅਚਲ ਅੱਖਰ *signature)
{
   ਪੂਰਨ i;
   ਲਈ (i=0; signature[i]; ++i)
      ਜੇ (stbi__get8(s) != signature[i])
          ਵਾਪਸ 0;
   stbi__rewind(s);
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__hdr_test(stbi__context* s)
{
   ਪੂਰਨ r = stbi__hdr_test_core(s, "#?RADIANCE\n");
   stbi__rewind(s);
   ਜੇ(!r) {
       r = stbi__hdr_test_core(s, "#?RGBE\n");
       stbi__rewind(s);
   }
   ਵਾਪਸ r;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STBI__HDR_BUFLEN  1024
ਸਥਿਰ ਅੱਖਰ *stbi__hdr_gettoken(stbi__context *z, ਅੱਖਰ *ਬਫਰ)
{
   ਪੂਰਨ len=0;
   ਅੱਖਰ c = '\0';

   c = (ਅੱਖਰ) stbi__get8(z);

   ਜਦੋਂ (!stbi__at_eof(z) && c != '\n') {
      ਬਫਰ[len++] = c;
      ਜੇ (len == STBI__HDR_BUFLEN-1) {
         // flush to end of line
         ਜਦੋਂ (!stbi__at_eof(z) && stbi__get8(z) != '\n')
            ;
         ਰੋਕੋ;
      }
      c = (ਅੱਖਰ) stbi__get8(z);
   }

   ਬਫਰ[len] = 0;
   ਵਾਪਸ ਬਫਰ;
}

ਸਥਿਰ ਖਾਲੀ stbi__hdr_convert(ਦਸ਼ਮਲਵ *output, stbi_uc *ਨਿਵੇਸ਼, ਪੂਰਨ req_comp)
{
   ਜੇ ( ਨਿਵੇਸ਼[3] != 0 ) {
      ਦਸ਼ਮਲਵ f1;
      // Exponent
      f1 = (ਦਸ਼ਮਲਵ) ldexp(1.0f, ਨਿਵੇਸ਼[3] - (ਪੂਰਨ)(128 + 8));
      ਜੇ (req_comp <= 2)
         output[0] = (ਨਿਵੇਸ਼[0] + ਨਿਵੇਸ਼[1] + ਨਿਵੇਸ਼[2]) * f1 / 3;
      ਨਹੀਂਤਾਂ {
         output[0] = ਨਿਵੇਸ਼[0] * f1;
         output[1] = ਨਿਵੇਸ਼[1] * f1;
         output[2] = ਨਿਵੇਸ਼[2] * f1;
      }
      ਜੇ (req_comp == 2) output[1] = 1;
      ਜੇ (req_comp == 4) output[3] = 1;
   } ਨਹੀਂਤਾਂ {
      ਚੋਣ (req_comp) {
         ਹਾਲਤ 4: output[3] = 1; /* fallthrough */
         ਹਾਲਤ 3: output[0] = output[1] = output[2] = 0;
                 ਰੋਕੋ;
         ਹਾਲਤ 2: output[1] = 1; /* fallthrough */
         ਹਾਲਤ 1: output[0] = 0;
                 ਰੋਕੋ;
      }
   }
}

ਸਥਿਰ ਦਸ਼ਮਲਵ *stbi_hdr_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   ਅੱਖਰ ਬਫਰ[STBI__HDR_BUFLEN];
   ਅੱਖਰ *token;
   ਪੂਰਨ valid = 0;
   ਪੂਰਨ ਚੌੜਾਈ, ਉਚਾਈ;
   stbi_uc *scanline;
   ਦਸ਼ਮਲਵ *hdr_ਅੰਕੜੇ;
   ਪੂਰਨ len;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਗਿਣਤੀ_2, ਮੁੱਲ;
   ਪੂਰਨ i, j, k, c1,c2, z;
   ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖ_Token;
   STBI_NOTUSED(ri);

   // Check identifier
   ਸਿਰਲੇਖ_Token = stbi__hdr_gettoken(s,ਬਫਰ);
   ਜੇ (strcmp(ਸਿਰਲੇਖ_Token, "#?RADIANCE") != 0 && strcmp(ਸਿਰਲੇਖ_Token, "#?RGBE") != 0)
      ਵਾਪਸ stbi__errpf("not HDR", "Corrupt HDR image");

   // Parse header
   ਲਈ(;;) {
      token = stbi__hdr_gettoken(s,ਬਫਰ);
      ਜੇ (token[0] == 0) ਰੋਕੋ;
      ਜੇ (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   ਜੇ (!valid)    ਵਾਪਸ stbi__errpf("unsupported format", "Unsupported HDR format");

   // Parse width and height
   // can't use sscanf() if we're not using stdio!
   token = stbi__hdr_gettoken(s,ਬਫਰ);
   ਜੇ (ਲੜੀ_ਤੁਲਨਾ(token, "-Y ", 3))  ਵਾਪਸ stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   ਉਚਾਈ = (ਪੂਰਨ) strtol(token, &token, 10);
   ਜਦੋਂ (*token == ' ') ++token;
   ਜੇ (ਲੜੀ_ਤੁਲਨਾ(token, "+X ", 3))  ਵਾਪਸ stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   ਚੌੜਾਈ = (ਪੂਰਨ) strtol(token, ਨਿਰਾ, 10);

   *x = ਚੌੜਾਈ;
   *y = ਉਚਾਈ;

   ਜੇ (comp) *comp = 3;
   ਜੇ (req_comp == 0) req_comp = 3;

   ਜੇ (!stbi__mad4sizes_valid(ਚੌੜਾਈ, ਉਚਾਈ, req_comp, ਆਕਾਰ(ਦਸ਼ਮਲਵ), 0))
      ਵਾਪਸ stbi__errpf("too large", "HDR image is too large");

   // Read data
   hdr_ਅੰਕੜੇ = (ਦਸ਼ਮਲਵ *) stbi__malloc_mad4(ਚੌੜਾਈ, ਉਚਾਈ, req_comp, ਆਕਾਰ(ਦਸ਼ਮਲਵ), 0);
   ਜੇ (!hdr_ਅੰਕੜੇ)
      ਵਾਪਸ stbi__errpf("outofmem", "Out of memory");

   // Load image data
   // image data is stored as some number of sca
   ਜੇ ( ਚੌੜਾਈ < 8 || ਚੌੜਾਈ >= 32768) {
      // Read flat data
      ਲਈ (j=0; j < ਉਚਾਈ; ++j) {
         ਲਈ (i=0; i < ਚੌੜਾਈ; ++i) {
            stbi_uc rgbe[4];
           main_decode_loop:
            stbi__getn(s, rgbe, 4);
            stbi__hdr_convert(hdr_ਅੰਕੜੇ + j * ਚੌੜਾਈ * req_comp + i * req_comp, rgbe, req_comp);
         }
      }
   } ਨਹੀਂਤਾਂ {
      // Read RLE-encoded data
      scanline = ਨਿਰਾ;

      ਲਈ (j = 0; j < ਉਚਾਈ; ++j) {
         c1 = stbi__get8(s);
         c2 = stbi__get8(s);
         len = stbi__get8(s);
         ਜੇ (c1 != 2 || c2 != 2 || (len & 0x80)) {
            // not run-length encoded, so we have to actually use THIS data as a decoded
            // pixel (note this can't be a valid pixel--one of RGB must be >= 128)
            stbi_uc rgbe[4];
            rgbe[0] = (stbi_uc) c1;
            rgbe[1] = (stbi_uc) c2;
            rgbe[2] = (stbi_uc) len;
            rgbe[3] = (stbi_uc) stbi__get8(s);
            stbi__hdr_convert(hdr_ਅੰਕੜੇ, rgbe, req_comp);
            i = 1;
            j = 0;
            STBI_FREE(scanline);
            ਜਾਓ main_decode_loop; // yes, this makes no sense
         }
         len <<= 8;
         len |= stbi__get8(s);
         ਜੇ (len != ਚੌੜਾਈ) { STBI_FREE(hdr_ਅੰਕੜੇ); STBI_FREE(scanline); ਵਾਪਸ stbi__errpf("invalid decoded scanline length", "corrupt HDR"); }
         ਜੇ (scanline == ਨਿਰਾ) {
            scanline = (stbi_uc *) stbi__malloc_mad2(ਚੌੜਾਈ, 4, 0);
            ਜੇ (!scanline) {
               STBI_FREE(hdr_ਅੰਕੜੇ);
               ਵਾਪਸ stbi__errpf("outofmem", "Out of memory");
            }
         }

         ਲਈ (k = 0; k < 4; ++k) {
            ਪੂਰਨ nleft;
            i = 0;
            ਜਦੋਂ ((nleft = ਚੌੜਾਈ - i) > 0) {
               ਗਿਣਤੀ_2 = stbi__get8(s);
               ਜੇ (ਗਿਣਤੀ_2 > 128) {
                  // Run
                  ਮੁੱਲ = stbi__get8(s);
                  ਗਿਣਤੀ_2 -= 128;
                  ਜੇ (ਗਿਣਤੀ_2 > nleft) { STBI_FREE(hdr_ਅੰਕੜੇ); STBI_FREE(scanline); ਵਾਪਸ stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  ਲਈ (z = 0; z < ਗਿਣਤੀ_2; ++z)
                     scanline[i++ * 4 + k] = ਮੁੱਲ;
               } ਨਹੀਂਤਾਂ {
                  // Dump
                  ਜੇ (ਗਿਣਤੀ_2 > nleft) { STBI_FREE(hdr_ਅੰਕੜੇ); STBI_FREE(scanline); ਵਾਪਸ stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  ਲਈ (z = 0; z < ਗਿਣਤੀ_2; ++z)
                     scanline[i++ * 4 + k] = stbi__get8(s);
               }
            }
         }
         ਲਈ (i=0; i < ਚੌੜਾਈ; ++i)
            stbi__hdr_convert(hdr_ਅੰਕੜੇ+(j*ਚੌੜਾਈ + i)*req_comp, scanline + i*4, req_comp);
      }
      ਜੇ (scanline)
         STBI_FREE(scanline);
   }

   ਵਾਪਸ hdr_ਅੰਕੜੇ;
}

ਸਥਿਰ ਪੂਰਨ stbi__hdr_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਅੱਖਰ ਬਫਰ[STBI__HDR_BUFLEN];
   ਅੱਖਰ *token;
   ਪੂਰਨ valid = 0;
   ਪੂਰਨ dummy;

   ਜੇ (!x) x = &dummy;
   ਜੇ (!y) y = &dummy;
   ਜੇ (!comp) comp = &dummy;

   ਜੇ (stbi__hdr_test(s) == 0) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }

   ਲਈ(;;) {
      token = stbi__hdr_gettoken(s,ਬਫਰ);
      ਜੇ (token[0] == 0) ਰੋਕੋ;
      ਜੇ (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   ਜੇ (!valid) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   token = stbi__hdr_gettoken(s,ਬਫਰ);
   ਜੇ (ਲੜੀ_ਤੁਲਨਾ(token, "-Y ", 3)) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   token += 3;
   *y = (ਪੂਰਨ) strtol(token, &token, 10);
   ਜਦੋਂ (*token == ' ') ++token;
   ਜੇ (ਲੜੀ_ਤੁਲਨਾ(token, "+X ", 3)) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   token += 3;
   *x = (ਪੂਰਨ) strtol(token, ਨਿਰਾ, 10);
   *comp = 3;
   ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ // STBI_NO_HDR

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_BMP
ਸਥਿਰ ਪੂਰਨ stbi__bmp_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਖਾਲੀ *p;
   stbi_bmp_ਅੰਕੜੇ info;

   info.all_a = 255;
   p = stbi_bmp_parse_ਸਿਰਲੇਖ(s, &info);
   stbi__rewind( s );
   ਜੇ (p == ਨਿਰਾ)
      ਵਾਪਸ 0;
   ਜੇ (x) *x = s->img_x;
   ਜੇ (y) *y = s->img_y;
   ਜੇ (comp) *comp = info.ma ? 4 : 3;
   ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PSD
ਸਥਿਰ ਪੂਰਨ stbi__psd_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਪੂਰਨ channel_ਗਿਣਤੀ, dummy, ਡੂੰਘਾਈ;
   ਜੇ (!x) x = &dummy;
   ਜੇ (!y) y = &dummy;
   ਜੇ (!comp) comp = &dummy;
   ਜੇ (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   ਜੇ (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   stbi__skip(s, 6);
   channel_ਗਿਣਤੀ = stbi__get16be(s);
   ਜੇ (channel_ਗਿਣਤੀ < 0 || channel_ਗਿਣਤੀ > 16) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   *y = stbi__get32be(s);
   *x = stbi__get32be(s);
   ਡੂੰਘਾਈ = stbi__get16be(s);
   ਜੇ (ਡੂੰਘਾਈ != 8 && ਡੂੰਘਾਈ != 16) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   ਜੇ (stbi__get16be(s) != 3) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   *comp = 4;
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ stbi__psd_is16(stbi__context *s)
{
   ਪੂਰਨ channel_ਗਿਣਤੀ, ਡੂੰਘਾਈ;
   ਜੇ (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   ਜੇ (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   stbi__skip(s, 6);
   channel_ਗਿਣਤੀ = stbi__get16be(s);
   ਜੇ (channel_ਗਿਣਤੀ < 0 || channel_ਗਿਣਤੀ > 16) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   (ਖਾਲੀ) stbi__get32be(s);
   (ਖਾਲੀ) stbi__get32be(s);
   ਡੂੰਘਾਈ = stbi__get16be(s);
   ਜੇ (ਡੂੰਘਾਈ != 16) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PIC
ਸਥਿਰ ਪੂਰਨ stbi__pic_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਪੂਰਨ act_comp=0,ਅੰਕ_packets=0,chained,dummy;
   stbi__pic_packet packets[10];

   ਜੇ (!x) x = &dummy;
   ਜੇ (!y) y = &dummy;
   ਜੇ (!comp) comp = &dummy;

   ਜੇ (!stbi__pic_is4(s,"\x53\x80\xF6\x34")) {
      stbi__rewind(s);
      ਵਾਪਸ 0;
   }

   stbi__skip(s, 88);

   *x = stbi__get16be(s);
   *y = stbi__get16be(s);
   ਜੇ (stbi__at_eof(s)) {
      stbi__rewind( s);
      ਵਾਪਸ 0;
   }
   ਜੇ ( (*x) != 0 && (1 << 28) / (*x) < (*y)) {
      stbi__rewind( s );
      ਵਾਪਸ 0;
   }

   stbi__skip(s, 8);

   ਕਰੋ {
      stbi__pic_packet *packet;

      ਜੇ (ਅੰਕ_packets==ਆਕਾਰ(packets)/ਆਕਾਰ(packets[0]))
         ਵਾਪਸ 0;

      packet = &packets[ਅੰਕ_packets++];
      chained = stbi__get8(s);
      packet->ਮਾਪ    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);
      act_comp |= packet->channel;

      ਜੇ (stbi__at_eof(s)) {
          stbi__rewind( s );
          ਵਾਪਸ 0;
      }
      ਜੇ (packet->ਮਾਪ != 8) {
          stbi__rewind( s );
          ਵਾਪਸ 0;
      }
   } ਜਦੋਂ (chained);

   *comp = (act_comp & 0x10 ? 4 : 3);

   ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNM

ਸਥਿਰ ਪੂਰਨ      stbi__pnm_test(stbi__context *s)
{
   ਅੱਖਰ p, t;
   p = (ਅੱਖਰ) stbi__get8(s);
   t = (ਅੱਖਰ) stbi__get8(s);
   ਜੇ (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind( s );
       ਵਾਪਸ 0;
   }
   ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ *stbi_pnm_ਲੱਦੋ(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp, ਪੂਰਨ req_comp, stbi_ਨਤੀਜਾ_info *ri)
{
   stbi_uc *ਬਾਹਰਲਾ;
   STBI_NOTUSED(ri);

   ਜੇ (!stbi__pnm_info(s, (ਪੂਰਨ *)&s->img_x, (ਪੂਰਨ *)&s->img_y, (ਪੂਰਨ *)&s->img_n))
      ਵਾਪਸ 0;

   *x = s->img_x;
   *y = s->img_y;
   ਜੇ (comp) *comp = s->img_n;

   ਜੇ (!stbi__mad3sizes_valid(s->img_n, s->img_x, s->img_y, 0))
      ਵਾਪਸ stbi__errpuc("too large", "PNM too large");

   ਬਾਹਰਲਾ = (stbi_uc *) stbi__malloc_mad3(s->img_n, s->img_x, s->img_y, 0);
   ਜੇ (!ਬਾਹਰਲਾ) ਵਾਪਸ stbi__errpuc("outofmem", "Out of memory");
   stbi__getn(s, ਬਾਹਰਲਾ, s->img_n * s->img_x * s->img_y);

   ਜੇ (req_comp && req_comp != s->img_n) {
      ਬਾਹਰਲਾ = stbi__convert_format(ਬਾਹਰਲਾ, s->img_n, req_comp, s->img_x, s->img_y);
      ਜੇ (ਬਾਹਰਲਾ == ਨਿਰਾ) ਵਾਪਸ ਬਾਹਰਲਾ; // stbi__convert_format frees input on failure
   }
   ਵਾਪਸ ਬਾਹਰਲਾ;
}

ਸਥਿਰ ਪੂਰਨ      stbi__pnm_isspace(ਅੱਖਰ c)
{
   ਵਾਪਸ c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

ਸਥਿਰ ਖਾਲੀ     stbi__pnm_skip_whitespace(stbi__context *s, ਅੱਖਰ *c)
{
   ਲਈ (;;) {
      ਜਦੋਂ (!stbi__at_eof(s) && stbi__pnm_isspace(*c))
         *c = (ਅੱਖਰ) stbi__get8(s);

      ਜੇ (stbi__at_eof(s) || *c != '#')
         ਰੋਕੋ;

      ਜਦੋਂ (!stbi__at_eof(s) && *c != '\n' && *c != '\r' )
         *c = (ਅੱਖਰ) stbi__get8(s);
   }
}

ਸਥਿਰ ਪੂਰਨ      stbi__pnm_isdigit(ਅੱਖਰ c)
{
   ਵਾਪਸ c >= '0' && c <= '9';
}

ਸਥਿਰ ਪੂਰਨ      stbi__pnm_getinteger(stbi__context *s, ਅੱਖਰ *c)
{
   ਪੂਰਨ ਮੁੱਲ = 0;

   ਜਦੋਂ (!stbi__at_eof(s) && stbi__pnm_isdigit(*c)) {
      ਮੁੱਲ = ਮੁੱਲ*10 + (*c - '0');
      *c = (ਅੱਖਰ) stbi__get8(s);
   }

   ਵਾਪਸ ਮੁੱਲ;
}

ਸਥਿਰ ਪੂਰਨ      stbi__pnm_info(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਪੂਰਨ maxv, dummy;
   ਅੱਖਰ c, p, t;

   ਜੇ (!x) x = &dummy;
   ਜੇ (!y) y = &dummy;
   ਜੇ (!comp) comp = &dummy;

   stbi__rewind(s);

   // Get identifier
   p = (ਅੱਖਰ) stbi__get8(s);
   t = (ਅੱਖਰ) stbi__get8(s);
   ਜੇ (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind(s);
       ਵਾਪਸ 0;
   }

   *comp = (t == '6') ? 3 : 1;  // '5' is 1-component .pgm; '6' is 3-component .ppm

   c = (ਅੱਖਰ) stbi__get8(s);
   stbi__pnm_skip_whitespace(s, &c);

   *x = stbi__pnm_getinteger(s, &c); // read width
   stbi__pnm_skip_whitespace(s, &c);

   *y = stbi__pnm_getinteger(s, &c); // read height
   stbi__pnm_skip_whitespace(s, &c);

   maxv = stbi__pnm_getinteger(s, &c);  // read max value

   ਜੇ (maxv > 255)
      ਵਾਪਸ stbi__err("max value > 255", "PPM image not 8-bit");
   ਨਹੀਂਤਾਂ
      ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ stbi__info_main(stbi__context *s, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_JPEG
   ਜੇ (stbi__jpeg_info(s, x, y, comp)) ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNG
   ਜੇ (stbi__png_info(s, x, y, comp))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_GIF
   ਜੇ (stbi__gif_info(s, x, y, comp))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_BMP
   ਜੇ (stbi__bmp_info(s, x, y, comp))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PSD
   ਜੇ (stbi__psd_info(s, x, y, comp))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PIC
   ਜੇ (stbi__pic_info(s, x, y, comp))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNM
   ਜੇ (stbi__pnm_info(s, x, y, comp))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_HDR
   ਜੇ (stbi__hdr_info(s, x, y, comp))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   // test tga last because it's a crappy test!
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_TGA
   ਜੇ (stbi__tga_info(s, x, y, comp))
       ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ
   ਵਾਪਸ stbi__err("unknown image type", "Image not of any known type, or corrupt");
}

ਸਥਿਰ ਪੂਰਨ stbi__is_16_main(stbi__context *s)
{
   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PNG
   ਜੇ (stbi__png_is16(s))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_PSD
   ਜੇ (stbi__psd_is16(s))  ਵਾਪਸ 1;
   #ਜੇ_ਅੰਤ

   ਵਾਪਸ 0;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STBI_NO_STDIO
STBIDEF ਪੂਰਨ stbi_info(ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
    FILE *f = stbi__fopen(filename, "rb");
    ਪੂਰਨ ਨਤੀਜਾ;
    ਜੇ (!f) ਵਾਪਸ stbi__err("can't fopen", "Unable to open file");
    ਨਤੀਜਾ = stbi_info_from_file(f, x, y, comp);
    fclose(f);
    ਵਾਪਸ ਨਤੀਜਾ;
}

STBIDEF ਪੂਰਨ stbi_info_from_file(FILE *f, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   ਪੂਰਨ r;
   stbi__context s;
   ਲੰਮਾ pos = ftell(f);
   stbi_ਸ਼ੁਰੂ_file(&s, f);
   r = stbi__info_main(&s,x,y,comp);
   fseek(f,pos,SEEK_SET);
   ਵਾਪਸ r;
}

STBIDEF ਪੂਰਨ stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ(ਅੱਖਰ ਅਚਲ *filename)
{
    FILE *f = stbi__fopen(filename, "rb");
    ਪੂਰਨ ਨਤੀਜਾ;
    ਜੇ (!f) ਵਾਪਸ stbi__err("can't fopen", "Unable to open file");
    ਨਤੀਜਾ = stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ_from_file(f);
    fclose(f);
    ਵਾਪਸ ਨਤੀਜਾ;
}

STBIDEF ਪੂਰਨ stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ_from_file(FILE *f)
{
   ਪੂਰਨ r;
   stbi__context s;
   ਲੰਮਾ pos = ftell(f);
   stbi_ਸ਼ੁਰੂ_file(&s, f);
   r = stbi__is_16_main(&s);
   fseek(f,pos,SEEK_SET);
   ਵਾਪਸ r;
}
#ਜੇ_ਅੰਤ // !STBI_NO_STDIO

STBIDEF ਪੂਰਨ stbi_info_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_mem(&s,ਬਫਰ,len);
   ਵਾਪਸ stbi__info_main(&s,x,y,comp);
}

STBIDEF ਪੂਰਨ stbi_info_from_callbacks(stbi_io_callbacks ਅਚਲ *c, ਖਾਲੀ *user, ਪੂਰਨ *x, ਪੂਰਨ *y, ਪੂਰਨ *comp)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_callbacks(&s, (stbi_io_callbacks *) c, user);
   ਵਾਪਸ stbi__info_main(&s,x,y,comp);
}

STBIDEF ਪੂਰਨ stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ_from_ਸਮ੍ਰਿਤੀ(stbi_uc ਅਚਲ *ਬਫਰ, ਪੂਰਨ len)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_mem(&s,ਬਫਰ,len);
   ਵਾਪਸ stbi__is_16_main(&s);
}

STBIDEF ਪੂਰਨ stbi_is_16_ਦੋਆਧਾਰੀ_ਅੰਕ_from_callbacks(stbi_io_callbacks ਅਚਲ *c, ਖਾਲੀ *user)
{
   stbi__context s;
   stbi_ਸ਼ੁਰੂ_callbacks(&s, (stbi_io_callbacks *) c, user);
   ਵਾਪਸ stbi__is_16_main(&s);
}

#ਜੇ_ਅੰਤ // STB_IMAGE_IMPLEMENTATION

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
