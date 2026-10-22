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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_INCLUDE_STB_IMAGE_H
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_INCLUDE_STB_IMAGE_H

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


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ_ಅಂತ್ಯ // STBI_NO_STDIO

#ವ್ಯಾಖ್ಯಾನಿಸು STBI_ಆವೃತ್ತಿ 1

ಎಣಿಕೆ
{
   STBI_default = 0, // only used for desired_channels

   STBI_grey       = 1,
   STBI_grey_alpha = 2,
   STBI_rgb        = 3,
   STBI_rgb_alpha  = 4
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ stbi_uc;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ stbi_us;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STB_IMAGE_STATIC
#ವ್ಯಾಖ್ಯಾನಿಸು STBIDEF ಸ್ಥಿರ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು STBIDEF ಬಾಹ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

//////////////////////////////////////////////////////////////////////////////
//
// PRIMARY API - works on images of any type
//

//
// load image by filename, open file, or memory buffer
//

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   ಪೂರ್ಣಾಂಕ      (*ಪಠ)  (ಶೂನ್ಯ *user,ಅಕ್ಷರ *ದತ್ತಾಂಶ,ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);   // fill 'data' with 'size' bytes.  return number of bytes actually read
   ಶೂನ್ಯ     (*skip)  (ಶೂನ್ಯ *user,ಪೂರ್ಣಾಂಕ n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
   ಪೂರ್ಣಾಂಕ      (*eof)   (ಶೂನ್ಯ *user);                       // returns nonzero if we are at end of file/data
} stbi_io_callbacks;

////////////////////////////////////
//
// 8-bits-per-channel interface
//

STBIDEF stbi_uc *stbi_ತುಂಬು_from_ಸ್ಮರಣೆ   (stbi_uc           ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len   , ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
STBIDEF stbi_uc *stbi_ತುಂಬು_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk  , ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_GIF
STBIDEF stbi_uc *stbi_ತುಂಬು_gif_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ **delays, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *z, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp);
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
STBIDEF stbi_uc *stbi_ತುಂಬು            (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
STBIDEF stbi_uc *stbi_ತುಂಬು_from_file  (FILE *f, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
// for stbi_load_from_file, file pointer is left pointing immediately after image
#ಆದರೆ_ಅಂತ್ಯ

////////////////////////////////////
//
// 16-bits-per-channel interface
//

STBIDEF stbi_us *stbi_ತುಂಬು_16_from_ಸ್ಮರಣೆ   (stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
STBIDEF stbi_us *stbi_ತುಂಬು_16_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
STBIDEF stbi_us *stbi_ತುಂಬು_16          (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
STBIDEF stbi_us *stbi_ತುಂಬು_from_file_16(FILE *f, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
#ಆದರೆ_ಅಂತ್ಯ

////////////////////////////////////
//
// float-per-channel interface
//
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_ರೇಖೀಯ
   STBIDEF ದಶಮಾಂಶ *stbi_loadf_from_ಸ್ಮರಣೆ     (stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
   STBIDEF ದಶಮಾಂಶ *stbi_loadf_from_callbacks  (stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y,  ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
   STBIDEF ದಶಮಾಂಶ *stbi_loadf            (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
   STBIDEF ದಶಮಾಂಶ *stbi_loadf_from_file  (FILE *f, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels);
   #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
   STBIDEF ಶೂನ್ಯ   stbi_hdr_ಗಮ್ಯ_ldr_gamma(ದಶಮಾಂಶ gamma);
   STBIDEF ಶೂನ್ಯ   stbi_hdr_ಗಮ್ಯ_ldr_scale(ದಶಮಾಂಶ scale);
#ಆದರೆ_ಅಂತ್ಯ // STBI_NO_HDR

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_ರೇಖೀಯ
   STBIDEF ಶೂನ್ಯ   stbi_ldr_ಗಮ್ಯ_hdr_gamma(ದಶಮಾಂಶ gamma);
   STBIDEF ಶೂನ್ಯ   stbi_ldr_ಗಮ್ಯ_hdr_scale(ದಶಮಾಂಶ scale);
#ಆದರೆ_ಅಂತ್ಯ // STBI_NO_LINEAR

// stbi_is_hdr is always defined, but always returns false if STBI_NO_HDR
STBIDEF ಪೂರ್ಣಾಂಕ    stbi_is_hdr_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user);
STBIDEF ಪೂರ್ಣಾಂಕ    stbi_is_hdr_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_hdr          (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename);
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_hdr_from_file(FILE *f);
#ಆದರೆ_ಅಂತ್ಯ // STBI_NO_STDIO


// get a VERY brief reason for failure
// NOT THREADSAFE
STBIDEF ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stbi_failure_reason  (ಶೂನ್ಯ);

// free the loaded image -- this is just free()
STBIDEF ಶೂನ್ಯ     stbi_image_free      (ಶೂನ್ಯ *retval_from_stbi_ತುಂಬು);

// get image dimensions & components without fully decoding
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_info_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_info_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_16_ಬಿಟ್_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len);
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_16_ಬಿಟ್_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_info               (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename,     ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_info_from_file     (FILE *f,                  ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_16_ಬಿಟ್          (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename);
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_16_ಬಿಟ್_from_file(FILE *f);
#ಆದರೆ_ಅಂತ್ಯ



// for image formats that explicitly notate that they have premultiplied alpha,
// we just return the colors as stored in the file. set this flag to force
// unpremultiplication. results are undefined if the unpremultiply overflow.
STBIDEF ಶೂನ್ಯ stbi_ಗಣ_unpremultiply_on_ತುಂಬು(ಪೂರ್ಣಾಂಕ flag_true_if_should_unpremultiply);

// indicate whether we should process iphone images back to canonical format,
// or just pass them through "as-is"
STBIDEF ಶೂನ್ಯ stbi_convert_iphone_png_ಗಮ್ಯ_rgb(ಪೂರ್ಣಾಂಕ flag_true_if_should_convert);

// flip the image vertically, so the first pixel in the output array is the bottom left
STBIDEF ಶೂನ್ಯ stbi_ಗಣ_flip_vertically_on_ತುಂಬು(ಪೂರ್ಣಾಂಕ flag_true_if_should_flip);

// ZLIB client - used by PNG, available for other purposes

STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_malloc_guesssize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ *outlen);
STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_malloc_guesssize_headerflag(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ *outlen, ಪೂರ್ಣಾಂಕ parse_ಶೀರ್ಷಿಕೆ);
STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_malloc(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *outlen);
STBIDEF ಪೂರ್ಣಾಂಕ   stbi_zlib_decode_ಬಫರ್(ಅಕ್ಷರ *obuffer, ಪೂರ್ಣಾಂಕ olen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ibuffer, ಪೂರ್ಣಾಂಕ ilen);

STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_noheader_malloc(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *outlen);
STBIDEF ಪೂರ್ಣಾಂಕ   stbi_zlib_decode_noheader_ಬಫರ್(ಅಕ್ಷರ *obuffer, ಪೂರ್ಣಾಂಕ olen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ibuffer, ಪೂರ್ಣಾಂಕ ilen);


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

//
//
////   end header file   /////////////////////////////////////////////////////
#ಆದರೆ_ಅಂತ್ಯ // STBI_INCLUDE_STB_IMAGE_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STB_IMAGE_IMPLEMENTATION

#ಆದರೆ defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_JPEG
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_JPEG
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_PNG
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_PNG
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_BMP
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_BMP
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_PSD
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_PSD
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_TGA
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_TGA
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_GIF
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_GIF
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_HDR
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_HDR
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_PIC
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_PIC
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ONLY_PNM
   #ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_PNM
   #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(STBI_NO_PNG) && !defined(STBI_SUPPORT_ZLIB) && !defined(STBI_NO_ZLIB)
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_ZLIB
#ಆದರೆ_ಅಂತ್ಯ


#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <stddef.h> // ptrdiff_t on osx
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಆದರೆ !defined(STBI_NO_ರೇಖೀಯ) || !defined(STBI_NO_HDR)
#ಅನ್ತರ್ಭಾವ <math.h>  // ldexp, pow
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_ASSERT
#ಅನ್ತರ್ಭಾವ <assert.h>
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_ASSERT(x) ಪ್ರತಿಜ್ಞಾ(x)
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _MSC_VER
   #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi_inline ಸಾಲೊಳಗೆ
   #ಇಲ್ಲದಿದ್ದರೆ
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi_inline
   #ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi_inline __forceinline
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ stbi__uint16;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ   ಚಿಹ್ನೆಯುಳ್ಳ ಗಿಡ್ಡ stbi__int16;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ   stbi__uint32;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ   ಚಿಹ್ನೆಯುಳ್ಳ ಪೂರ್ಣಾಂಕ   stbi__int32;
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <stdint.h>
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint16_t stbi__uint16;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int16_t  stbi__int16;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint32_t stbi__uint32;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int32_t  stbi__int32;
#ಆದರೆ_ಅಂತ್ಯ

// should produce compiler error if size is wrong
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ validate_uint32[ಗಾತ್ರ(stbi__uint32)==4 ? 1 : -1];

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_NOTUSED(v)  (ಶೂನ್ಯ)(v)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_NOTUSED(v)  (ಶೂನ್ಯ)ಗಾತ್ರ(v)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_HAS_LROTL
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_HAS_LROTL
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi_lrot(x,y)  _lrotl(x,y)
#ಇಲ್ಲದಿದ್ದರೆ
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi_lrot(x,y)  (((x) << (y)) | ((x) >> (32 - (y))))
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(STBI_MALLOC) && defined(STBI_FREE) && (defined(STBI_REALLOC) || defined(STBI_REALLOC_SIZED))
// ok
#ಅಥವಾಆದರೆ !defined(STBI_MALLOC) && !defined(STBI_FREE) && !defined(STBI_REALLOC) && !defined(STBI_REALLOC_SIZED)
// ok
#ಇಲ್ಲದಿದ್ದರೆ
#ದೋಷ "Must define all or none of STBI_MALLOC, STBI_FREE, and STBI_REALLOC (or STBI_REALLOC_SIZED)."
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_MALLOC
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_MALLOC(sz)           ಸ್ಮರಣೆ_ಕೊಡು(sz)
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_REALLOC(p,newsz)     realloc(p,newsz)
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_FREE(p)              ಬಿಡುಗಡೆ(p)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_REALLOC_SIZED
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_REALLOC_SIZED(p,oldsz,newsz) STBI_REALLOC(p,newsz)
#ಆದರೆ_ಅಂತ್ಯ

// x86/x64 detection
#ಆದರೆ defined(__x86_64__) || defined(_M_X64)
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_X64_ಗುರಿ
#ಅಥವಾಆದರೆ defined(__i386) || defined(_M_IX86)
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_X86_ಗುರಿ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__GNUC__) && defined(STBI_X86_ಗುರಿ) && !defined(__SSE2__) && !defined(STBI_NO_SIMD)
// gcc doesn't support sse2 intrinsics unless you compile with -msse2,
// which in turn means it gets to use SSE2 everywhere. This is unfortunate,
// but previous attempts to provide the SSE2 functions with runtime
// detection caused numerous issues. The way architecture extensions are
// exposed in GCC/Clang is, sadly, not really suited for one-file libs.
// New behavior: if compiled with -msse2, we use SSE2 without any
// detection; if not, we don't use it at all.
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_SIMD
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__MINGW32__) && defined(STBI_X86_ಗುರಿ) && !defined(STBI_MINGW_ENABLE_SSE2) && !defined(STBI_NO_SIMD)
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
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_NO_SIMD
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !defined(STBI_NO_SIMD) && (defined(STBI_X86_ಗುರಿ) || defined(STBI_X64_ಗುರಿ))
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_SSE2
#ಅನ್ತರ್ಭಾವ <emmintrin.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER

#ಆದರೆ _MSC_VER >= 1400  // not VC6
#ಅನ್ತರ್ಭಾವ <intrin.h> // __cpuid
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__cpuid3(ಶೂನ್ಯ)
{
   ಪೂರ್ಣಾಂಕ info[4];
   __cpuid(info,1);
   ಹಿಂತಿರುಗಿ info[3];
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__cpuid3(ಶೂನ್ಯ)
{
   ಪೂರ್ಣಾಂಕ res;
   __asm {
      mov  eax,1
      cpuid
      mov  res,edx
   }
   ಹಿಂತಿರುಗಿ res;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು STBI_SIMD_ALIGN(type, name) __declspec(align(16)) type name

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_sse2_ಲಭ್ಯ(ಶೂನ್ಯ)
{
   ಪೂರ್ಣಾಂಕ info3 = stbi__cpuid3();
   ಹಿಂತಿರುಗಿ ((info3 >> 26) & 1) != 0;
}
#ಇಲ್ಲದಿದ್ದರೆ // assume GCC-style if not VC++
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_sse2_ಲಭ್ಯ(ಶೂನ್ಯ)
{
   // If we're even attempting to compile this on GCC/Clang, that means
   // -msse2 is on, which means the compiler is allowed to use SSE2
   // instructions at will, and so are we.
   ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

// ARM NEON
#ಆದರೆ defined(STBI_NO_SIMD) && defined(STBI_NEON)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STBI_NEON
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_NEON
#ಅನ್ತರ್ಭಾವ <arm_neon.h>
// assume GCC or Clang on ARM targets
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_SIMD_ALIGN
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_SIMD_ALIGN(type, name) type name
#ಆದರೆ_ಅಂತ್ಯ

///////////////////////////////////////////////
//
//  stbi__context struct and start_xxx functions

// stbi__context structure is our basic context used by all images, so it
// contains all the IO context, plus some basic image information
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi__uint32 img_x, img_y;
   ಪೂರ್ಣಾಂಕ img_n, img_ಹೊರಹೋಗು_n;

   stbi_io_callbacks io;
   ಶೂನ್ಯ *io_user_ದತ್ತಾಂಶ;

   ಪೂರ್ಣಾಂಕ read_from_callbacks;
   ಪೂರ್ಣಾಂಕ buflen;
   stbi_uc ಬಫರ್_ಆರಂಭ[128];

   stbi_uc *img_ಬಫರ್, *img_ಬಫರ್_ಕೊನೆ;
   stbi_uc *img_ಬಫರ್_original, *img_ಬಫರ್_original_ಕೊನೆ;
} stbi__context;


ಸ್ಥಿರ ಶೂನ್ಯ stbi_refill_ಬಫರ್(stbi__context *s);

// initialize a memory-decode context
ಸ್ಥಿರ ಶೂನ್ಯ stbi_ಆರಂಭ_mem(stbi__context *s, stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len)
{
   s->io.ಪಠ = ಖಾಲಿ;
   s->read_from_callbacks = 0;
   s->img_ಬಫರ್ = s->img_ಬಫರ್_original = (stbi_uc *) ಬಫರ್;
   s->img_ಬಫರ್_ಕೊನೆ = s->img_ಬಫರ್_original_ಕೊನೆ = (stbi_uc *) ಬಫರ್+len;
}

// initialize a callback-based context
ಸ್ಥಿರ ಶೂನ್ಯ stbi_ಆರಂಭ_callbacks(stbi__context *s, stbi_io_callbacks *c, ಶೂನ್ಯ *user)
{
   s->io = *c;
   s->io_user_ದತ್ತಾಂಶ = user;
   s->buflen = ಗಾತ್ರ(s->ಬಫರ್_ಆರಂಭ);
   s->read_from_callbacks = 1;
   s->img_ಬಫರ್_original = s->ಬಫರ್_ಆರಂಭ;
   stbi_refill_ಬಫರ್(s);
   s->img_ಬಫರ್_original_ಕೊನೆ = s->img_ಬಫರ್_ಕೊನೆ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__stdio_read(ಶೂನ್ಯ *user, ಅಕ್ಷರ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
   ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) fread(ದತ್ತಾಂಶ,1,ಗಾತ್ರ_2,(FILE*) user);
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__stdio_skip(ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ n)
{
   fseek((FILE*) user, n, SEEK_CUR);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__stdio_eof(ಶೂನ್ಯ *user)
{
   ಹಿಂತಿರುಗಿ feof((FILE*) user);
}

ಸ್ಥಿರ stbi_io_callbacks stbi__stdio_callbacks =
{
   stbi__stdio_read,
   stbi__stdio_skip,
   stbi__stdio_eof,
};

ಸ್ಥಿರ ಶೂನ್ಯ stbi_ಆರಂಭ_file(stbi__context *s, FILE *f)
{
   stbi_ಆರಂಭ_callbacks(s, &stbi__stdio_callbacks, (ಶೂನ್ಯ *) f);
}

//static void stop_file(stbi__context *s) { }

#ಆದರೆ_ಅಂತ್ಯ // !STBI_NO_STDIO

ಸ್ಥಿರ ಶೂನ್ಯ stbi__rewind(stbi__context *s)
{
   // conceptually rewind SHOULD rewind to the beginning of the stream,
   // but we just rewind to the beginning of the initial buffer, because
   // we only use it after doing 'test', which only ever looks at at most 92 bytes
   s->img_ಬಫರ್ = s->img_ಬಫರ್_original;
   s->img_ಬಫರ್_ಕೊನೆ = s->img_ಬಫರ್_original_ಕೊನೆ;
}

ಎಣಿಕೆ
{
   STBI_ಕ್ರಮ_RGB,
   STBI_ಕ್ರಮ_BGR
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel;
   ಪೂರ್ಣಾಂಕ ಅಂಕೆ_channels;
   ಪೂರ್ಣಾಂಕ channel_ಕ್ರಮ;
} stbi_ಫಲಿತಾಂಶ_info;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_JPEG
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__jpeg_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_jpeg_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__jpeg_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNG
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__png_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_png_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__png_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__png_is16(stbi__context *s);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_BMP
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__bmp_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_bmp_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__bmp_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_TGA
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__tga_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_tga_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__tga_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PSD
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__psd_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_psd_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri, ಪೂರ್ಣಾಂಕ bpc);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__psd_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__psd_is16(stbi__context *s);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__hdr_test(stbi__context *s);
ಸ್ಥಿರ ದಶಮಾಂಶ   *stbi_hdr_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__hdr_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PIC
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pic_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_pic_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pic_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_GIF
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__gif_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_gif_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_ತುಂಬು_gif_main(stbi__context *s, ಪೂರ್ಣಾಂಕ **delays, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *z, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__gif_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNM
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pnm_test(stbi__context *s);
ಸ್ಥಿರ ಶೂನ್ಯ    *stbi_pnm_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pnm_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp);
#ಆದರೆ_ಅಂತ್ಯ

// this is not threadsafe
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stbi__g_failure_reason;

STBIDEF ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stbi_failure_reason(ಶೂನ್ಯ)
{
   ಹಿಂತಿರುಗಿ stbi__g_failure_reason;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__err(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
   stbi__g_failure_reason = str;
   ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi__malloc(size_t ಗಾತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ STBI_MALLOC(ಗಾತ್ರ_2);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__addsizes_valid(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
   ಆದರೆ (b < 0) ಹಿಂತಿರುಗಿ 0;
   // now 0 <= b <= INT_MAX, hence also
   // 0 <= INT_MAX - b <= INTMAX.
   // And "a + b <= INT_MAX" (which might overflow) is the
   // same as a <= INT_MAX - b (no overflow)
   ಹಿಂತಿರುಗಿ a <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ - b;
}

// returns 1 if the product is valid, 0 on overflow.
// negative factors are considered invalid.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__mul2sizes_valid(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
   ಆದರೆ (a < 0 || b < 0) ಹಿಂತಿರುಗಿ 0;
   ಆದರೆ (b == 0) ಹಿಂತಿರುಗಿ 1; // mul-by-0 is always safe
   // portable way to check for no overflows in a*b
   ಹಿಂತಿರುಗಿ a <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ/b;
}

// returns 1 if "a*b + add" has no negative terms/factors and doesn't overflow
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__mad2sizes_valid(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು)
{
   ಹಿಂತಿರುಗಿ stbi__mul2sizes_valid(a, b) && stbi__addsizes_valid(a*b, ಕೂಡಿಸು);
}

// returns 1 if "a*b*c + add" has no negative terms/factors and doesn't overflow
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__mad3sizes_valid(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು)
{
   ಹಿಂತಿರುಗಿ stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__addsizes_valid(a*b*c, ಕೂಡಿಸು);
}

// returns 1 if "a*b*c*d + add" has no negative terms/factors and doesn't overflow
#ಆದರೆ !defined(STBI_NO_ರೇಖೀಯ) || !defined(STBI_NO_HDR)
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__mad4sizes_valid(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ d, ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು)
{
   ಹಿಂತಿರುಗಿ stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__mul2sizes_valid(a*b*c, d) && stbi__addsizes_valid(a*b*c*d, ಕೂಡಿಸು);
}
#ಆದರೆ_ಅಂತ್ಯ

// mallocs with size overflow checking
ಸ್ಥಿರ ಶೂನ್ಯ *stbi__malloc_mad2(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು)
{
   ಆದರೆ (!stbi__mad2sizes_valid(a, b, ಕೂಡಿಸು)) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   ಹಿಂತಿರುಗಿ stbi__malloc(a*b + ಕೂಡಿಸು);
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi__malloc_mad3(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು)
{
   ಆದರೆ (!stbi__mad3sizes_valid(a, b, c, ಕೂಡಿಸು)) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   ಹಿಂತಿರುಗಿ stbi__malloc(a*b*c + ಕೂಡಿಸು);
}

#ಆದರೆ !defined(STBI_NO_ರೇಖೀಯ) || !defined(STBI_NO_HDR)
ಸ್ಥಿರ ಶೂನ್ಯ *stbi__malloc_mad4(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ d, ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು)
{
   ಆದರೆ (!stbi__mad4sizes_valid(a, b, c, d, ಕೂಡಿಸು)) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   ಹಿಂತಿರುಗಿ stbi__malloc(a*b*c*d + ಕೂಡಿಸು);
}
#ಆದರೆ_ಅಂತ್ಯ

// stbi__err - error
// stbi__errpf - error returning pointer to float
// stbi__errpuc - error returning pointer to unsigned char

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_NO_FAILURE_ಅಕ್ಷರಸರಣಿಗಳು
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi__err(x,y)  0
#ಅಥವಾಆದರೆ defined(STBI_FAILURE_USERMSG)
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi__err(x,y)  stbi__err(y)
#ಇಲ್ಲದಿದ್ದರೆ
   #ವ್ಯಾಖ್ಯಾನಿಸು stbi__err(x,y)  stbi__err(x)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು stbi__errpf(x,y)   ((ದಶಮಾಂಶ *)(size_t) (stbi__err(x,y)?ಖಾಲಿ:ಖಾಲಿ))
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__errpuc(x,y)  ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)(size_t) (stbi__err(x,y)?ಖಾಲಿ:ಖಾಲಿ))

STBIDEF ಶೂನ್ಯ stbi_image_free(ಶೂನ್ಯ *retval_from_stbi_ತುಂಬು)
{
   STBI_FREE(retval_from_stbi_ತುಂಬು);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_ರೇಖೀಯ
ಸ್ಥಿರ ದಶಮಾಂಶ   *stbi_ldr_ಗಮ್ಯ_hdr(stbi_uc *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ comp);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
ಸ್ಥಿರ stbi_uc *stbi_hdr_ಗಮ್ಯ_ldr(ದಶಮಾಂಶ   *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ comp);
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_vertically_flip_on_ತುಂಬು = 0;

STBIDEF ಶೂನ್ಯ stbi_ಗಣ_flip_vertically_on_ತುಂಬು(ಪೂರ್ಣಾಂಕ flag_true_if_should_flip)
{
    stbi_vertically_flip_on_ತುಂಬು = flag_true_if_should_flip;
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_ತುಂಬು_main(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri, ಪೂರ್ಣಾಂಕ bpc)
{
   memset(ri, 0, ಗಾತ್ರ(*ri)); // make sure it's initialized if we add new fields
   ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel = 8; // default is 8 so most paths don't have to be changed
   ri->channel_ಕ್ರಮ = STBI_ಕ್ರಮ_RGB; // all current input & output are this, but this is here so we can add BGR order
   ri->ಅಂಕೆ_channels = 0;

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_JPEG
   ಆದರೆ (stbi__jpeg_test(s)) ಹಿಂತಿರುಗಿ stbi_jpeg_ತುಂಬು(s,x,y,comp,req_comp, ri);
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNG
   ಆದರೆ (stbi__png_test(s))  ಹಿಂತಿರುಗಿ stbi_png_ತುಂಬು(s,x,y,comp,req_comp, ri);
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_BMP
   ಆದರೆ (stbi__bmp_test(s))  ಹಿಂತಿರುಗಿ stbi_bmp_ತುಂಬು(s,x,y,comp,req_comp, ri);
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_GIF
   ಆದರೆ (stbi__gif_test(s))  ಹಿಂತಿರುಗಿ stbi_gif_ತುಂಬು(s,x,y,comp,req_comp, ri);
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PSD
   ಆದರೆ (stbi__psd_test(s))  ಹಿಂತಿರುಗಿ stbi_psd_ತುಂಬು(s,x,y,comp,req_comp, ri, bpc);
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PIC
   ಆದರೆ (stbi__pic_test(s))  ಹಿಂತಿರುಗಿ stbi_pic_ತುಂಬು(s,x,y,comp,req_comp, ri);
   #ಆದರೆ_ಅಂತ್ಯ
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNM
   ಆದರೆ (stbi__pnm_test(s))  ಹಿಂತಿರುಗಿ stbi_pnm_ತುಂಬು(s,x,y,comp,req_comp, ri);
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
   ಆದರೆ (stbi__hdr_test(s)) {
      ದಶಮಾಂಶ *hdr = stbi_hdr_ತುಂಬು(s, x,y,comp,req_comp, ri);
      ಹಿಂತಿರುಗಿ stbi_hdr_ಗಮ್ಯ_ldr(hdr, *x, *y, req_comp ? req_comp : *comp);
   }
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_TGA
   // test tga last because it's a crappy test!
   ಆದರೆ (stbi__tga_test(s))
      ಹಿಂತಿರುಗಿ stbi_tga_ತುಂಬು(s,x,y,comp,req_comp, ri);
   #ಆದರೆ_ಅಂತ್ಯ

   ಹಿಂತಿರುಗಿ stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
}

ಸ್ಥಿರ stbi_uc *stbi_convert_16_ಗಮ್ಯ_8(stbi__uint16 *orig, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ channels)
{
   ಪೂರ್ಣಾಂಕ i;
   ಪೂರ್ಣಾಂಕ img_len = w * h * channels;
   stbi_uc *reduced;

   reduced = (stbi_uc *) stbi__malloc(img_len);
   ಆದರೆ (reduced == ಖಾಲಿ) ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");

   ಗಾಗಿ (i = 0; i < img_len; ++i)
      reduced[i] = (stbi_uc)((orig[i] >> 8) & 0xFF); // top half of each byte is sufficient approx of 16->8 bit scaling

   STBI_FREE(orig);
   ಹಿಂತಿರುಗಿ reduced;
}

ಸ್ಥಿರ stbi__uint16 *stbi_convert_8_ಗಮ್ಯ_16(stbi_uc *orig, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ channels)
{
   ಪೂರ್ಣಾಂಕ i;
   ಪೂರ್ಣಾಂಕ img_len = w * h * channels;
   stbi__uint16 *enlarged;

   enlarged = (stbi__uint16 *) stbi__malloc(img_len*2);
   ಆದರೆ (enlarged == ಖಾಲಿ) ಹಿಂತಿರುಗಿ (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");

   ಗಾಗಿ (i = 0; i < img_len; ++i)
      enlarged[i] = (stbi__uint16)((orig[i] << 8) + orig[i]); // replicate to high and low byte, maps 0->0, 255->0xffff

   STBI_FREE(orig);
   ಹಿಂತಿರುಗಿ enlarged;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__vertical_flip(ಶೂನ್ಯ *image, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ ಬೈಟುಗಳು_ಪ್ರತಿ_pixel)
{
   ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು;
   size_t ಬೈಟುಗಳು_ಪ್ರತಿ_ಅಡ್ಡಸಾಲು = (size_t)w * ಬೈಟುಗಳು_ಪ್ರತಿ_pixel;
   stbi_uc ತಾತ್ಕಾಲಿಕ[2048];
   stbi_uc *ಬೈಟುಗಳು = (stbi_uc *)image;

   ಗಾಗಿ (ಅಡ್ಡಸಾಲು = 0; ಅಡ್ಡಸಾಲು < (h>>1); ಅಡ್ಡಸಾಲು++) {
      stbi_uc *row0 = ಬೈಟುಗಳು + ಅಡ್ಡಸಾಲು*ಬೈಟುಗಳು_ಪ್ರತಿ_ಅಡ್ಡಸಾಲು;
      stbi_uc *row1 = ಬೈಟುಗಳು + (h - ಅಡ್ಡಸಾಲು - 1)*ಬೈಟುಗಳು_ಪ್ರತಿ_ಅಡ್ಡಸಾಲು;
      // swap row0 with row1
      size_t ಬೈಟುಗಳು_ಎಡ = ಬೈಟುಗಳು_ಪ್ರತಿ_ಅಡ್ಡಸಾಲು;
      ಆಗ (ಬೈಟುಗಳು_ಎಡ) {
         size_t ಬೈಟುಗಳು_copy = (ಬೈಟುಗಳು_ಎಡ < ಗಾತ್ರ(ತಾತ್ಕಾಲಿಕ)) ? ಬೈಟುಗಳು_ಎಡ : ಗಾತ್ರ(ತಾತ್ಕಾಲಿಕ);
         ಸ್ಮರಣೆ_ನಕಲು(ತಾತ್ಕಾಲಿಕ, row0, ಬೈಟುಗಳು_copy);
         ಸ್ಮರಣೆ_ನಕಲು(row0, row1, ಬೈಟುಗಳು_copy);
         ಸ್ಮರಣೆ_ನಕಲು(row1, ತಾತ್ಕಾಲಿಕ, ಬೈಟುಗಳು_copy);
         row0 += ಬೈಟುಗಳು_copy;
         row1 += ಬೈಟುಗಳು_copy;
         ಬೈಟುಗಳು_ಎಡ -= ಬೈಟುಗಳು_copy;
      }
   }
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__vertical_flip_slices(ಶೂನ್ಯ *image, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ z, ಪೂರ್ಣಾಂಕ ಬೈಟುಗಳು_ಪ್ರತಿ_pixel)
{
   ಪೂರ್ಣಾಂಕ ತುಂಡು;
   ಪೂರ್ಣಾಂಕ ತುಂಡು_ಗಾತ್ರ = w * h * ಬೈಟುಗಳು_ಪ್ರತಿ_pixel;

   stbi_uc *ಬೈಟುಗಳು = (stbi_uc *)image;
   ಗಾಗಿ (ತುಂಡು = 0; ತುಂಡು < z; ++ತುಂಡು) {
      stbi__vertical_flip(ಬೈಟುಗಳು, w, h, ಬೈಟುಗಳು_ಪ್ರತಿ_pixel); 
      ಬೈಟುಗಳು += ತುಂಡು_ಗಾತ್ರ; 
   }
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *stbi_ತುಂಬು_and_postprocess_8bit(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi_ಫಲಿತಾಂಶ_info ri;
   ಶೂನ್ಯ *ಫಲಿತಾಂಶ = stbi_ತುಂಬು_main(s, x, y, comp, req_comp, &ri, 8);

   ಆದರೆ (ಫಲಿತಾಂಶ == ಖಾಲಿ)
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

   ಆದರೆ (ri.ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel != 8) {
      STBI_ASSERT(ri.ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel == 16);
      ಫಲಿತಾಂಶ = stbi_convert_16_ಗಮ್ಯ_8((stbi__uint16 *) ಫಲಿತಾಂಶ, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel = 8;
   }

   // @TODO: move stbi__convert_format to here

   ಆದರೆ (stbi_vertically_flip_on_ತುಂಬು) {
      ಪೂರ್ಣಾಂಕ channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ಫಲಿತಾಂಶ, *x, *y, channels * ಗಾತ್ರ(stbi_uc));
   }

   ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ stbi__uint16 *stbi_ತುಂಬು_and_postprocess_16bit(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi_ಫಲಿತಾಂಶ_info ri;
   ಶೂನ್ಯ *ಫಲಿತಾಂಶ = stbi_ತುಂಬು_main(s, x, y, comp, req_comp, &ri, 16);

   ಆದರೆ (ಫಲಿತಾಂಶ == ಖಾಲಿ)
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

   ಆದರೆ (ri.ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel != 16) {
      STBI_ASSERT(ri.ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel == 8);
      ಫಲಿತಾಂಶ = stbi_convert_8_ಗಮ್ಯ_16((stbi_uc *) ಫಲಿತಾಂಶ, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel = 16;
   }

   // @TODO: move stbi__convert_format16 to here
   // @TODO: special case RGB-to-Y (and RGBA-to-YA) for 8-bit-to-16-bit case to keep more precision

   ಆದರೆ (stbi_vertically_flip_on_ತುಂಬು) {
      ಪೂರ್ಣಾಂಕ channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ಫಲಿತಾಂಶ, *x, *y, channels * ಗಾತ್ರ(stbi__uint16));
   }

   ಹಿಂತಿರುಗಿ (stbi__uint16 *) ಫಲಿತಾಂಶ;
}

#ಆದರೆ !defined(STBI_NO_HDR) || !defined(STBI_NO_ರೇಖೀಯ)
ಸ್ಥಿರ ಶೂನ್ಯ stbi__float_postprocess(ದಶಮಾಂಶ *ಫಲಿತಾಂಶ, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   ಆದರೆ (stbi_vertically_flip_on_ತುಂಬು && ಫಲಿತಾಂಶ != ಖಾಲಿ) {
      ಪೂರ್ಣಾಂಕ channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ಫಲಿತಾಂಶ, *x, *y, channels * ಗಾತ್ರ(ದಶಮಾಂಶ));
   }
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO

ಸ್ಥಿರ FILE *stbi__fopen(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಹುಲಕ)
{
   FILE *f;
#ಆದರೆ defined(_MSC_VER) && _MSC_VER >= 1400
   ಆದರೆ (0 != fopen_s(&f, filename, ಬಹುಲಕ))
      f=0;
#ಇಲ್ಲದಿದ್ದರೆ
   f = fopen(filename, ಬಹುಲಕ);
#ಆದರೆ_ಅಂತ್ಯ
   ಹಿಂತಿರುಗಿ f;
}


STBIDEF stbi_uc *stbi_ತುಂಬು(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಫಲಿತಾಂಶ;
   ಆದರೆ (!f) ಹಿಂತಿರುಗಿ stbi__errpuc("can't fopen", "Unable to open file");
   ಫಲಿತಾಂಶ = stbi_ತುಂಬು_from_file(f,x,y,comp,req_comp);
   fclose(f);
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

STBIDEF stbi_uc *stbi_ತುಂಬು_from_file(FILE *f, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಫಲಿತಾಂಶ;
   stbi__context s;
   stbi_ಆರಂಭ_file(&s,f);
   ಫಲಿತಾಂಶ = stbi_ತುಂಬು_and_postprocess_8bit(&s,x,y,comp,req_comp);
   ಆದರೆ (ಫಲಿತಾಂಶ) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (ಪೂರ್ಣಾಂಕ) (s.img_ಬಫರ್_ಕೊನೆ - s.img_ಬಫರ್), SEEK_CUR);
   }
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

STBIDEF stbi__uint16 *stbi_ತುಂಬು_from_file_16(FILE *f, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi__uint16 *ಫಲಿತಾಂಶ;
   stbi__context s;
   stbi_ಆರಂಭ_file(&s,f);
   ಫಲಿತಾಂಶ = stbi_ತುಂಬು_and_postprocess_16bit(&s,x,y,comp,req_comp);
   ಆದರೆ (ಫಲಿತಾಂಶ) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (ಪೂರ್ಣಾಂಕ) (s.img_ಬಫರ್_ಕೊನೆ - s.img_ಬಫರ್), SEEK_CUR);
   }
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

STBIDEF stbi_us *stbi_ತುಂಬು_16(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   stbi__uint16 *ಫಲಿತಾಂಶ;
   ಆದರೆ (!f) ಹಿಂತಿರುಗಿ (stbi_us *) stbi__errpuc("can't fopen", "Unable to open file");
   ಫಲಿತಾಂಶ = stbi_ತುಂಬು_from_file_16(f,x,y,comp,req_comp);
   fclose(f);
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}


#ಆದರೆ_ಅಂತ್ಯ //!STBI_NO_STDIO

STBIDEF stbi_us *stbi_ತುಂಬು_16_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels)
{
   stbi__context s;
   stbi_ಆರಂಭ_mem(&s,ಬಫರ್,len);
   ಹಿಂತಿರುಗಿ stbi_ತುಂಬು_and_postprocess_16bit(&s,x,y,channels_ಒಳ_file,desired_channels);
}

STBIDEF stbi_us *stbi_ತುಂಬು_16_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *channels_ಒಳ_file, ಪೂರ್ಣಾಂಕ desired_channels)
{
   stbi__context s;
   stbi_ಆರಂಭ_callbacks(&s, (stbi_io_callbacks *)clbk, user);
   ಹಿಂತಿರುಗಿ stbi_ತುಂಬು_and_postprocess_16bit(&s,x,y,channels_ಒಳ_file,desired_channels);
}

STBIDEF stbi_uc *stbi_ತುಂಬು_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi__context s;
   stbi_ಆರಂಭ_mem(&s,ಬಫರ್,len);
   ಹಿಂತಿರುಗಿ stbi_ತುಂಬು_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

STBIDEF stbi_uc *stbi_ತುಂಬು_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi__context s;
   stbi_ಆರಂಭ_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   ಹಿಂತಿರುಗಿ stbi_ತುಂಬು_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_GIF
STBIDEF stbi_uc *stbi_ತುಂಬು_gif_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ **delays, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *z, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಫಲಿತಾಂಶ;
   stbi__context s; 
   stbi_ಆರಂಭ_mem(&s,ಬಫರ್,len); 
   
   ಫಲಿತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*) stbi_ತುಂಬು_gif_main(&s, delays, x, y, z, comp, req_comp);
   ಆದರೆ (stbi_vertically_flip_on_ತುಂಬು) {
      stbi__vertical_flip_slices( ಫಲಿತಾಂಶ, *x, *y, *z, *comp ); 
   }

   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ; 
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_ರೇಖೀಯ
ಸ್ಥಿರ ದಶಮಾಂಶ *stbi__loadf_main(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ;
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
   ಆದರೆ (stbi__hdr_test(s)) {
      stbi_ಫಲಿತಾಂಶ_info ri;
      ದಶಮಾಂಶ *hdr_ದತ್ತಾಂಶ = stbi_hdr_ತುಂಬು(s,x,y,comp,req_comp, &ri);
      ಆದರೆ (hdr_ದತ್ತಾಂಶ)
         stbi__float_postprocess(hdr_ದತ್ತಾಂಶ,x,y,comp,req_comp);
      ಹಿಂತಿರುಗಿ hdr_ದತ್ತಾಂಶ;
   }
   #ಆದರೆ_ಅಂತ್ಯ
   ದತ್ತಾಂಶ = stbi_ತುಂಬು_and_postprocess_8bit(s, x, y, comp, req_comp);
   ಆದರೆ (ದತ್ತಾಂಶ)
      ಹಿಂತಿರುಗಿ stbi_ldr_ಗಮ್ಯ_hdr(ದತ್ತಾಂಶ, *x, *y, req_comp ? req_comp : *comp);
   ಹಿಂತಿರುಗಿ stbi__errpf("unknown image type", "Image not of any known type, or corrupt");
}

STBIDEF ದಶಮಾಂಶ *stbi_loadf_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi__context s;
   stbi_ಆರಂಭ_mem(&s,ಬಫರ್,len);
   ಹಿಂತಿರುಗಿ stbi__loadf_main(&s,x,y,comp,req_comp);
}

STBIDEF ದಶಮಾಂಶ *stbi_loadf_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi__context s;
   stbi_ಆರಂಭ_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   ಹಿಂತಿರುಗಿ stbi__loadf_main(&s,x,y,comp,req_comp);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
STBIDEF ದಶಮಾಂಶ *stbi_loadf(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   ದಶಮಾಂಶ *ಫಲಿತಾಂಶ;
   FILE *f = stbi__fopen(filename, "rb");
   ಆದರೆ (!f) ಹಿಂತಿರುಗಿ stbi__errpf("can't fopen", "Unable to open file");
   ಫಲಿತಾಂಶ = stbi_loadf_from_file(f,x,y,comp,req_comp);
   fclose(f);
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

STBIDEF ದಶಮಾಂಶ *stbi_loadf_from_file(FILE *f, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi__context s;
   stbi_ಆರಂಭ_file(&s,f);
   ಹಿಂತಿರುಗಿ stbi__loadf_main(&s,x,y,comp,req_comp);
}
#ಆದರೆ_ಅಂತ್ಯ // !STBI_NO_STDIO

#ಆದರೆ_ಅಂತ್ಯ // !STBI_NO_LINEAR

// these is-hdr-or-not is defined independent of whether STBI_NO_LINEAR is
// defined, for API simplicity; if STBI_NO_LINEAR is defined, it always
// reports false!

STBIDEF ಪೂರ್ಣಾಂಕ stbi_is_hdr_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len)
{
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
   stbi__context s;
   stbi_ಆರಂಭ_mem(&s,ಬಫರ್,len);
   ಹಿಂತಿರುಗಿ stbi__hdr_test(&s);
   #ಇಲ್ಲದಿದ್ದರೆ
   STBI_NOTUSED(ಬಫರ್);
   STBI_NOTUSED(len);
   ಹಿಂತಿರುಗಿ 0;
   #ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_hdr          (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
   FILE *f = stbi__fopen(filename, "rb");
   ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ=0;
   ಆದರೆ (f) {
      ಫಲಿತಾಂಶ = stbi_is_hdr_from_file(f);
      fclose(f);
   }
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_is_hdr_from_file(FILE *f)
{
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
   ದೀರ್ಘ pos = ftell(f);
   ಪೂರ್ಣಾಂಕ res;
   stbi__context s;
   stbi_ಆರಂಭ_file(&s,f);
   res = stbi__hdr_test(&s);
   fseek(f, pos, SEEK_SET);
   ಹಿಂತಿರುಗಿ res;
   #ಇಲ್ಲದಿದ್ದರೆ
   STBI_NOTUSED(f);
   ಹಿಂತಿರುಗಿ 0;
   #ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ // !STBI_NO_STDIO

STBIDEF ಪೂರ್ಣಾಂಕ      stbi_is_hdr_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *clbk, ಶೂನ್ಯ *user)
{
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
   stbi__context s;
   stbi_ಆರಂಭ_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   ಹಿಂತಿರುಗಿ stbi__hdr_test(&s);
   #ಇಲ್ಲದಿದ್ದರೆ
   STBI_NOTUSED(clbk);
   STBI_NOTUSED(user);
   ಹಿಂತಿರುಗಿ 0;
   #ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_ರೇಖೀಯ
ಸ್ಥಿರ ದಶಮಾಂಶ stbi__l2h_gamma=2.2f, stbi__l2h_scale=1.0f;

STBIDEF ಶೂನ್ಯ   stbi_ldr_ಗಮ್ಯ_hdr_gamma(ದಶಮಾಂಶ gamma) { stbi__l2h_gamma = gamma; }
STBIDEF ಶೂನ್ಯ   stbi_ldr_ಗಮ್ಯ_hdr_scale(ದಶಮಾಂಶ scale) { stbi__l2h_scale = scale; }
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ದಶಮಾಂಶ stbi__h2l_gamma_i=1.0f/2.2f, stbi__h2l_scale_i=1.0f;

STBIDEF ಶೂನ್ಯ   stbi_hdr_ಗಮ್ಯ_ldr_gamma(ದಶಮಾಂಶ gamma) { stbi__h2l_gamma_i = 1/gamma; }
STBIDEF ಶೂನ್ಯ   stbi_hdr_ಗಮ್ಯ_ldr_scale(ದಶಮಾಂಶ scale) { stbi__h2l_scale_i = 1/scale; }


//////////////////////////////////////////////////////////////////////////////
//
// Common code used by all image loaders
//

ಎಣಿಕೆ
{
   STBI_ಪರಿಶೀಲನೆ_ತುಂಬು=0,
   STBI_ಪರಿಶೀಲನೆ_type,
   STBI_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ
};

ಸ್ಥಿರ ಶೂನ್ಯ stbi_refill_ಬಫರ್(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ n = (s->io.ಪಠ)(s->io_user_ದತ್ತಾಂಶ,(ಅಕ್ಷರ*)s->ಬಫರ್_ಆರಂಭ,s->buflen);
   ಆದರೆ (n == 0) {
      // at end of file, treat same as if from memory, but need to handle case
      // where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file
      s->read_from_callbacks = 0;
      s->img_ಬಫರ್ = s->ಬಫರ್_ಆರಂಭ;
      s->img_ಬಫರ್_ಕೊನೆ = s->ಬಫರ್_ಆರಂಭ+1;
      *s->img_ಬಫರ್ = 0;
   } ಇಲ್ಲದಿದ್ದರೆ {
      s->img_ಬಫರ್ = s->ಬಫರ್_ಆರಂಭ;
      s->img_ಬಫರ್_ಕೊನೆ = s->ಬಫರ್_ಆರಂಭ + n;
   }
}

stbi_inline ಸ್ಥಿರ stbi_uc stbi__get8(stbi__context *s)
{
   ಆದರೆ (s->img_ಬಫರ್ < s->img_ಬಫರ್_ಕೊನೆ)
      ಹಿಂತಿರುಗಿ *s->img_ಬಫರ್++;
   ಆದರೆ (s->read_from_callbacks) {
      stbi_refill_ಬಫರ್(s);
      ಹಿಂತಿರುಗಿ *s->img_ಬಫರ್++;
   }
   ಹಿಂತಿರುಗಿ 0;
}

stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__at_eof(stbi__context *s)
{
   ಆದರೆ (s->io.ಪಠ) {
      ಆದರೆ (!(s->io.eof)(s->io_user_ದತ್ತಾಂಶ)) ಹಿಂತಿರುಗಿ 0;
      // if feof() is true, check if buffer = end
      // special case: we've only got the special 0 character at the end
      ಆದರೆ (s->read_from_callbacks == 0) ಹಿಂತಿರುಗಿ 1;
   }

   ಹಿಂತಿರುಗಿ s->img_ಬಫರ್ >= s->img_ಬಫರ್_ಕೊನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__skip(stbi__context *s, ಪೂರ್ಣಾಂಕ n)
{
   ಆದರೆ (n < 0) {
      s->img_ಬಫರ್ = s->img_ಬಫರ್_ಕೊನೆ;
      ಹಿಂತಿರುಗಿ;
   }
   ಆದರೆ (s->io.ಪಠ) {
      ಪೂರ್ಣಾಂಕ blen = (ಪೂರ್ಣಾಂಕ) (s->img_ಬಫರ್_ಕೊನೆ - s->img_ಬಫರ್);
      ಆದರೆ (blen < n) {
         s->img_ಬಫರ್ = s->img_ಬಫರ್_ಕೊನೆ;
         (s->io.skip)(s->io_user_ದತ್ತಾಂಶ, n - blen);
         ಹಿಂತಿರುಗಿ;
      }
   }
   s->img_ಬಫರ್ += n;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__getn(stbi__context *s, stbi_uc *ಬಫರ್, ಪೂರ್ಣಾಂಕ n)
{
   ಆದರೆ (s->io.ಪಠ) {
      ಪೂರ್ಣಾಂಕ blen = (ಪೂರ್ಣಾಂಕ) (s->img_ಬಫರ್_ಕೊನೆ - s->img_ಬಫರ್);
      ಆದರೆ (blen < n) {
         ಪೂರ್ಣಾಂಕ res, ಎಣಿಕೆ_2;

         ಸ್ಮರಣೆ_ನಕಲು(ಬಫರ್, s->img_ಬಫರ್, blen);

         ಎಣಿಕೆ_2 = (s->io.ಪಠ)(s->io_user_ದತ್ತಾಂಶ, (ಅಕ್ಷರ*) ಬಫರ್ + blen, n - blen);
         res = (ಎಣಿಕೆ_2 == (n-blen));
         s->img_ಬಫರ್ = s->img_ಬಫರ್_ಕೊನೆ;
         ಹಿಂತಿರುಗಿ res;
      }
   }

   ಆದರೆ (s->img_ಬಫರ್+n <= s->img_ಬಫರ್_ಕೊನೆ) {
      ಸ್ಮರಣೆ_ನಕಲು(ಬಫರ್, s->img_ಬಫರ್, n);
      s->img_ಬಫರ್ += n;
      ಹಿಂತಿರುಗಿ 1;
   } ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__get16be(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ z = stbi__get8(s);
   ಹಿಂತಿರುಗಿ (z << 8) + stbi__get8(s);
}

ಸ್ಥಿರ stbi__uint32 stbi__get32be(stbi__context *s)
{
   stbi__uint32 z = stbi__get16be(s);
   ಹಿಂತಿರುಗಿ (z << 16) + stbi__get16be(s);
}

#ಆದರೆ defined(STBI_NO_BMP) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF)
// nothing
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__get16le(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ z = stbi__get8(s);
   ಹಿಂತಿರುಗಿ z + (stbi__get8(s) << 8);
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_BMP
ಸ್ಥಿರ stbi__uint32 stbi__get32le(stbi__context *s)
{
   stbi__uint32 z = stbi__get16le(s);
   ಹಿಂತಿರುಗಿ z + (stbi__get16le(s) << 16);
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು STBI__BYTECAST(x)  ((stbi_uc) ((x) & 255))  // truncate int to byte without warnings


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

ಸ್ಥಿರ stbi_uc stbi__compute_y(ಪೂರ್ಣಾಂಕ r, ಪೂರ್ಣಾಂಕ g, ಪೂರ್ಣಾಂಕ b)
{
   ಹಿಂತಿರುಗಿ (stbi_uc) (((r*77) + (g*150) +  (29*b)) >> 8);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *stbi__convert_format(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ img_n, ಪೂರ್ಣಾಂಕ req_comp, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ x, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ y)
{
   ಪೂರ್ಣಾಂಕ i,j;
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *good;

   ಆದರೆ (req_comp == img_n) ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) stbi__malloc_mad3(req_comp, x, y, 0);
   ಆದರೆ (good == ಖಾಲಿ) {
      STBI_FREE(ದತ್ತಾಂಶ);
      ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");
   }

   ಗಾಗಿ (j=0; j < (ಪೂರ್ಣಾಂಕ) y; ++j) {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *src  = ದತ್ತಾಂಶ + j * x * img_n   ;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *dest = good + j * x * req_comp;

      #ವ್ಯಾಖ್ಯಾನಿಸು STBI__COMBO(a,b)  ((a)*8+(b))
      #ವ್ಯಾಖ್ಯಾನಿಸು STBI__CASE(a,b)   ಸ್ಥಿತಿ STBI__COMBO(a,b): ಗಾಗಿ(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      ಆಯ್ಕೆ (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=255;                                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } ನಿಲ್ಲಿಸು;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=255;                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(2,1) { dest[0]=src[0];                                                  } ನಿಲ್ಲಿಸು;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } ನಿಲ್ಲಿಸು;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                  } ನಿಲ್ಲಿಸು;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255;        } ನಿಲ್ಲಿಸು;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } ನಿಲ್ಲಿಸು;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = 255;    } ನಿಲ್ಲಿಸು;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } ನಿಲ್ಲಿಸು;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = src[3]; } ನಿಲ್ಲಿಸು;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                    } ನಿಲ್ಲಿಸು;
         ಪೂರ್ವನಿಯೋಜಿತ: STBI_ASSERT(0);
      }
      #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STBI__CASE
   }

   STBI_FREE(ದತ್ತಾಂಶ);
   ಹಿಂತಿರುಗಿ good;
}

ಸ್ಥಿರ stbi__uint16 stbi__compute_y_16(ಪೂರ್ಣಾಂಕ r, ಪೂರ್ಣಾಂಕ g, ಪೂರ್ಣಾಂಕ b)
{
   ಹಿಂತಿರುಗಿ (stbi__uint16) (((r*77) + (g*150) +  (29*b)) >> 8);
}

ಸ್ಥಿರ stbi__uint16 *stbi__convert_format16(stbi__uint16 *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ img_n, ಪೂರ್ಣಾಂಕ req_comp, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ x, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ y)
{
   ಪೂರ್ಣಾಂಕ i,j;
   stbi__uint16 *good;

   ಆದರೆ (req_comp == img_n) ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (stbi__uint16 *) stbi__malloc(req_comp * x * y * 2);
   ಆದರೆ (good == ಖಾಲಿ) {
      STBI_FREE(ದತ್ತಾಂಶ);
      ಹಿಂತಿರುಗಿ (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");
   }

   ಗಾಗಿ (j=0; j < (ಪೂರ್ಣಾಂಕ) y; ++j) {
      stbi__uint16 *src  = ದತ್ತಾಂಶ + j * x * img_n   ;
      stbi__uint16 *dest = good + j * x * req_comp;

      #ವ್ಯಾಖ್ಯಾನಿಸು STBI__COMBO(a,b)  ((a)*8+(b))
      #ವ್ಯಾಖ್ಯಾನಿಸು STBI__CASE(a,b)   ಸ್ಥಿತಿ STBI__COMBO(a,b): ಗಾಗಿ(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      ಆಯ್ಕೆ (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=0xffff;                                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=0xffff;                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(2,1) { dest[0]=src[0];                                                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                     } ನಿಲ್ಲಿಸು;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=0xffff;        } ನಿಲ್ಲಿಸು;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } ನಿಲ್ಲಿಸು;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = 0xffff; } ನಿಲ್ಲಿಸು;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } ನಿಲ್ಲಿಸು;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = src[3]; } ನಿಲ್ಲಿಸು;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                       } ನಿಲ್ಲಿಸು;
         ಪೂರ್ವನಿಯೋಜಿತ: STBI_ASSERT(0);
      }
      #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STBI__CASE
   }

   STBI_FREE(ದತ್ತಾಂಶ);
   ಹಿಂತಿರುಗಿ good;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_ರೇಖೀಯ
ಸ್ಥಿರ ದಶಮಾಂಶ   *stbi_ldr_ಗಮ್ಯ_hdr(stbi_uc *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ comp)
{
   ಪೂರ್ಣಾಂಕ i,k,n;
   ದಶಮಾಂಶ *output;
   ಆದರೆ (!ದತ್ತಾಂಶ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   output = (ದಶಮಾಂಶ *) stbi__malloc_mad4(x, y, comp, ಗಾತ್ರ(ದಶಮಾಂಶ), 0);
   ಆದರೆ (output == ಖಾಲಿ) { STBI_FREE(ದತ್ತಾಂಶ); ಹಿಂತಿರುಗಿ stbi__errpf("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   ಆದರೆ (comp & 1) n = comp; ಇಲ್ಲದಿದ್ದರೆ n = comp-1;
   ಗಾಗಿ (i=0; i < x*y; ++i) {
      ಗಾಗಿ (k=0; k < n; ++k) {
         output[i*comp + k] = (ದಶಮಾಂಶ) (ಘಾತ(ದತ್ತಾಂಶ[i*comp+k]/255.0f, stbi__l2h_gamma) * stbi__l2h_scale);
      }
      ಆದರೆ (k < comp) output[i*comp + k] = ದತ್ತಾಂಶ[i*comp+k]/255.0f;
   }
   STBI_FREE(ದತ್ತಾಂಶ);
   ಹಿಂತಿರುಗಿ output;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__float2int(x)   ((ಪೂರ್ಣಾಂಕ) (x))
ಸ್ಥಿರ stbi_uc *stbi_hdr_ಗಮ್ಯ_ldr(ದಶಮಾಂಶ   *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ comp)
{
   ಪೂರ್ಣಾಂಕ i,k,n;
   stbi_uc *output;
   ಆದರೆ (!ದತ್ತಾಂಶ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   output = (stbi_uc *) stbi__malloc_mad3(x, y, comp, 0);
   ಆದರೆ (output == ಖಾಲಿ) { STBI_FREE(ದತ್ತಾಂಶ); ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   ಆದರೆ (comp & 1) n = comp; ಇಲ್ಲದಿದ್ದರೆ n = comp-1;
   ಗಾಗಿ (i=0; i < x*y; ++i) {
      ಗಾಗಿ (k=0; k < n; ++k) {
         ದಶಮಾಂಶ z = (ದಶಮಾಂಶ) ಘಾತ(ದತ್ತಾಂಶ[i*comp+k]*stbi__h2l_scale_i, stbi__h2l_gamma_i) * 255 + 0.5f;
         ಆದರೆ (z < 0) z = 0;
         ಆದರೆ (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
      ಆದರೆ (k < comp) {
         ದಶಮಾಂಶ z = ದತ್ತಾಂಶ[i*comp+k] * 255 + 0.5f;
         ಆದರೆ (z < 0) z = 0;
         ಆದರೆ (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
   }
   STBI_FREE(ದತ್ತಾಂಶ);
   ಹಿಂತಿರುಗಿ output;
}
#ಆದರೆ_ಅಂತ್ಯ

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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_JPEG

// huffman decoding acceleration
#ವ್ಯಾಖ್ಯಾನಿಸು FAST_ಬಿಟ್ಟುಗಳು   9  // larger handles more cases; smaller stomps less cache

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi_uc  fast[1 << FAST_ಬಿಟ್ಟುಗಳು];
   // weirdly, repacking this into AoS is a 10% speed loss, instead of a win
   stbi__uint16 ಸಂಕೇತ_2[256];
   stbi_uc  ಮೌಲ್ಯಗಳು[256];
   stbi_uc  ಗಾತ್ರ_2[257];
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ maxcode[18];
   ಪೂರ್ಣಾಂಕ    delta[17];   // old 'firstsymbol' - old 'firstcode'
} stbi__huffman;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi__context *s;
   stbi__huffman huff_dc[4];
   stbi__huffman huff_ac[4];
   stbi__uint16 dequant[4][64];
   stbi__int16 fast_ac[4][1 << FAST_ಬಿಟ್ಟುಗಳು];

// sizes for components, interleaved MCUs
   ಪೂರ್ಣಾಂಕ img_h_max, img_v_max;
   ಪೂರ್ಣಾಂಕ img_mcu_x, img_mcu_y;
   ಪೂರ್ಣಾಂಕ img_mcu_w, img_mcu_h;

// definition of jpeg image component
   ರಚನೆ
   {
      ಪೂರ್ಣಾಂಕ id;
      ಪೂರ್ಣಾಂಕ h,v;
      ಪೂರ್ಣಾಂಕ tq;
      ಪೂರ್ಣಾಂಕ hd,ha;
      ಪೂರ್ಣಾಂಕ dc_pred;

      ಪೂರ್ಣಾಂಕ x,y,w2,h2;
      stbi_uc *ದತ್ತಾಂಶ;
      ಶೂನ್ಯ *raw_ದತ್ತಾಂಶ, *raw_coeff;
      stbi_uc *linebuf;
      ಗಿಡ್ಡ   *coeff;   // progressive only
      ಪೂರ್ಣಾಂಕ      coeff_w, coeff_h; // number of 8x8 coefficient blocks
   } img_comp[4];

   stbi__uint32   ಸಂಕೇತ_ಬಫರ್; // jpeg entropy-coded buffer
   ಪೂರ್ಣಾಂಕ            ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು;   // number of valid bits
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  marker;      // marker seen while filling entropy buffer
   ಪೂರ್ಣಾಂಕ            nomore;      // flag if we saw a marker so must stop

   ಪೂರ್ಣಾಂಕ            progressive;
   ಪೂರ್ಣಾಂಕ            spec_ಆರಂಭ;
   ಪೂರ್ಣಾಂಕ            spec_ಕೊನೆ;
   ಪೂರ್ಣಾಂಕ            succ_ಮೇಲಿನ;
   ಪೂರ್ಣಾಂಕ            succ_ಕೆಳಗಿನ;
   ಪೂರ್ಣಾಂಕ            eob_ಸತತ;
   ಪೂರ್ಣಾಂಕ            jfif;
   ಪೂರ್ಣಾಂಕ            app14_color_transform; // Adobe APP14 tag
   ಪೂರ್ಣಾಂಕ            rgb;

   ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ_n, ಕ್ರಮ[4];
   ಪೂರ್ಣಾಂಕ restart_interval, todo;

// kernels
   ಶೂನ್ಯ (*idct_ಖಂಡ_kernel)(stbi_uc *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_stride, ಗಿಡ್ಡ ದತ್ತಾಂಶ[64]);
   ಶೂನ್ಯ (*YCb_Cr_ಗಮ್ಯ_RGB_kernel)(stbi_uc *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ stbi_uc *y, ಸ್ಥಿರಾಂಕ stbi_uc *pcb, ಸ್ಥಿರಾಂಕ stbi_uc *pcr, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ);
   stbi_uc *(*resample_ಅಡ್ಡಸಾಲು_hv_2_kernel)(stbi_uc *ಹೊರಹೋಗು, stbi_uc *ಒಳ_near, stbi_uc *ಒಳ_far, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs);
} stbi__jpeg;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__build_huffman(stbi__huffman *h, ಪೂರ್ಣಾಂಕ *ಎಣಿಕೆ_2)
{
   ಪೂರ್ಣಾಂಕ i,j,k=0;
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;
   // build size list for each symbol (from JPEG spec)
   ಗಾಗಿ (i=0; i < 16; ++i)
      ಗಾಗಿ (j=0; j < ಎಣಿಕೆ_2[i]; ++j)
         h->ಗಾತ್ರ_2[k++] = (stbi_uc) (i+1);
   h->ಗಾತ್ರ_2[k] = 0;

   // compute actual symbols (from jpeg spec)
   ಸಂಕೇತ_2 = 0;
   k = 0;
   ಗಾಗಿ(j=1; j <= 16; ++j) {
      // compute delta to add to code to compute symbol id
      h->delta[j] = k - ಸಂಕೇತ_2;
      ಆದರೆ (h->ಗಾತ್ರ_2[k] == j) {
         ಆಗ (h->ಗಾತ್ರ_2[k] == j)
            h->ಸಂಕೇತ_2[k++] = (stbi__uint16) (ಸಂಕೇತ_2++);
         ಆದರೆ (ಸಂಕೇತ_2-1 >= (1u << j)) ಹಿಂತಿರುಗಿ stbi__err("bad code lengths","Corrupt JPEG");
      }
      // compute largest code + 1 for this size, preshifted as needed later
      h->maxcode[j] = ಸಂಕೇತ_2 << (16-j);
      ಸಂಕೇತ_2 <<= 1;
   }
   h->maxcode[j] = 0xffffffff;

   // build non-spec acceleration table; 255 is flag for not-accelerated
   memset(h->fast, 255, 1 << FAST_ಬಿಟ್ಟುಗಳು);
   ಗಾಗಿ (i=0; i < k; ++i) {
      ಪೂರ್ಣಾಂಕ s = h->ಗಾತ್ರ_2[i];
      ಆದರೆ (s <= FAST_ಬಿಟ್ಟುಗಳು) {
         ಪೂರ್ಣಾಂಕ c = h->ಸಂಕೇತ_2[i] << (FAST_ಬಿಟ್ಟುಗಳು-s);
         ಪೂರ್ಣಾಂಕ m = 1 << (FAST_ಬಿಟ್ಟುಗಳು-s);
         ಗಾಗಿ (j=0; j < m; ++j) {
            h->fast[c+j] = (stbi_uc) i;
         }
      }
   }
   ಹಿಂತಿರುಗಿ 1;
}

// build a table that decodes both magnitude and value of small ACs in
// one go.
ಸ್ಥಿರ ಶೂನ್ಯ stbi__build_fast_ac(stbi__int16 *fast_ac, stbi__huffman *h)
{
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; i < (1 << FAST_ಬಿಟ್ಟುಗಳು); ++i) {
      stbi_uc fast = h->fast[i];
      fast_ac[i] = 0;
      ಆದರೆ (fast < 255) {
         ಪೂರ್ಣಾಂಕ rs = h->ಮೌಲ್ಯಗಳು[fast];
         ಪೂರ್ಣಾಂಕ ಸತತ = (rs >> 4) & 15;
         ಪೂರ್ಣಾಂಕ magbits = rs & 15;
         ಪೂರ್ಣಾಂಕ len = h->ಗಾತ್ರ_2[fast];

         ಆದರೆ (magbits && len + magbits <= FAST_ಬಿಟ್ಟುಗಳು) {
            // magnitude code followed by receive_extend code
            ಪೂರ್ಣಾಂಕ k = ((i << len) & ((1 << FAST_ಬಿಟ್ಟುಗಳು) - 1)) >> (FAST_ಬಿಟ್ಟುಗಳು - magbits);
            ಪೂರ್ಣಾಂಕ m = 1 << (magbits - 1);
            ಆದರೆ (k < m) k += (~0U << magbits) + 1;
            // if the result is small enough, we can fit it in fast_ac table
            ಆದರೆ (k >= -128 && k <= 127)
               fast_ac[i] = (stbi__int16) ((k * 256) + (ಸತತ * 16) + (len + magbits));
         }
      }
   }
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi_grow_ಬಫರ್_unsafe(stbi__jpeg *j)
{
   ಮಾಡು {
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ b = j->nomore ? 0 : stbi__get8(j->s);
      ಆದರೆ (b == 0xff) {
         ಪೂರ್ಣಾಂಕ c = stbi__get8(j->s);
         ಆಗ (c == 0xff) c = stbi__get8(j->s); // consume fill bytes
         ಆದರೆ (c != 0) {
            j->marker = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) c;
            j->nomore = 1;
            ಹಿಂತಿರುಗಿ;
         }
      }
      j->ಸಂಕೇತ_ಬಫರ್ |= b << (24 - j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು);
      j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು += 8;
   } ಆಗ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು <= 24);
}

// (1 << n) - 1
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ stbi__uint32 stbi__bmask[17]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};

// decode a jpeg huffman value from the bitstream
stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__jpeg_huff_decode(stbi__jpeg *j, stbi__huffman *h)
{
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ತಾತ್ಕಾಲಿಕ;
   ಪೂರ್ಣಾಂಕ c,k;

   ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 16) stbi_grow_ಬಫರ್_unsafe(j);

   // look at the top FAST_BITS and determine what symbol ID it is,
   // if the code is <= FAST_BITS
   c = (j->ಸಂಕೇತ_ಬಫರ್ >> (32 - FAST_ಬಿಟ್ಟುಗಳು)) & ((1 << FAST_ಬಿಟ್ಟುಗಳು)-1);
   k = h->fast[c];
   ಆದರೆ (k < 255) {
      ಪೂರ್ಣಾಂಕ s = h->ಗಾತ್ರ_2[k];
      ಆದರೆ (s > j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು)
         ಹಿಂತಿರುಗಿ -1;
      j->ಸಂಕೇತ_ಬಫರ್ <<= s;
      j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು -= s;
      ಹಿಂತಿರುಗಿ h->ಮೌಲ್ಯಗಳು[k];
   }

   // naive test is to shift the code_buffer down so k bits are
   // valid, then test against maxcode. To speed this up, we've
   // preshifted maxcode left so that it has (16-k) 0s at the
   // end; in other words, regardless of the number of bits, it
   // wants to be compared against something shifted to have 16;
   // that way we don't need to shift inside the loop.
   ತಾತ್ಕಾಲಿಕ = j->ಸಂಕೇತ_ಬಫರ್ >> 16;
   ಗಾಗಿ (k=FAST_ಬಿಟ್ಟುಗಳು+1 ; ; ++k)
      ಆದರೆ (ತಾತ್ಕಾಲಿಕ < h->maxcode[k])
         ನಿಲ್ಲಿಸು;
   ಆದರೆ (k == 17) {
      // error! code not found
      j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು -= 16;
      ಹಿಂತಿರುಗಿ -1;
   }

   ಆದರೆ (k > j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು)
      ಹಿಂತಿರುಗಿ -1;

   // convert the huffman code to the symbol id
   c = ((j->ಸಂಕೇತ_ಬಫರ್ >> (32 - k)) & stbi__bmask[k]) + h->delta[k];
   STBI_ASSERT((((j->ಸಂಕೇತ_ಬಫರ್) >> (32 - h->ಗಾತ್ರ_2[c])) & stbi__bmask[h->ಗಾತ್ರ_2[c]]) == h->ಸಂಕೇತ_2[c]);

   // convert the id to a symbol
   j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು -= k;
   j->ಸಂಕೇತ_ಬಫರ್ <<= k;
   ಹಿಂತಿರುಗಿ h->ಮೌಲ್ಯಗಳು[c];
}

// bias[n] = (-1<<n) + 1
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stbi__jbias[16] = {0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767};

// combined JPEG 'receive' and JPEG 'extend', since baseline
// always extends everything it receives.
stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__extend_receive(stbi__jpeg *j, ಪೂರ್ಣಾಂಕ n)
{
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ k;
   ಪೂರ್ಣಾಂಕ sgn;
   ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < n) stbi_grow_ಬಫರ್_unsafe(j);

   sgn = (stbi__int32)j->ಸಂಕೇತ_ಬಫರ್ >> 31; // sign bit is always in MSB
   k = stbi_lrot(j->ಸಂಕೇತ_ಬಫರ್, n);
   STBI_ASSERT(n >= 0 && n < (ಪೂರ್ಣಾಂಕ) (ಗಾತ್ರ(stbi__bmask)/ಗಾತ್ರ(*stbi__bmask)));
   j->ಸಂಕೇತ_ಬಫರ್ = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು -= n;
   ಹಿಂತಿರುಗಿ k + (stbi__jbias[n] & ~sgn);
}

// get some unsigned bits
stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_jpeg_get_ಬಿಟ್ಟುಗಳು(stbi__jpeg *j, ಪೂರ್ಣಾಂಕ n)
{
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ k;
   ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < n) stbi_grow_ಬಫರ್_unsafe(j);
   k = stbi_lrot(j->ಸಂಕೇತ_ಬಫರ್, n);
   j->ಸಂಕೇತ_ಬಫರ್ = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು -= n;
   ಹಿಂತಿರುಗಿ k;
}

stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_jpeg_get_ಬಿಟ್(stbi__jpeg *j)
{
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ k;
   ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 1) stbi_grow_ಬಫರ್_unsafe(j);
   k = j->ಸಂಕೇತ_ಬಫರ್;
   j->ಸಂಕೇತ_ಬಫರ್ <<= 1;
   --j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು;
   ಹಿಂತಿರುಗಿ k & 0x80000000;
}

// given a value that's at position X in the zigzag stream,
// where does it appear in the 8x8 matrix coded as row-major?
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ stbi_uc stbi__jpeg_dezigzag[64+15] =
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_jpeg_decode_ಖಂಡ(stbi__jpeg *j, ಗಿಡ್ಡ ದತ್ತಾಂಶ[64], stbi__huffman *hdc, stbi__huffman *hac, stbi__int16 *fac, ಪೂರ್ಣಾಂಕ b, stbi__uint16 *dequant)
{
   ಪೂರ್ಣಾಂಕ diff,dc,k;
   ಪೂರ್ಣಾಂಕ t;

   ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 16) stbi_grow_ಬಫರ್_unsafe(j);
   t = stbi__jpeg_huff_decode(j, hdc);
   ಆದರೆ (t < 0) ಹಿಂತಿರುಗಿ stbi__err("bad huffman code","Corrupt JPEG");

   // 0 all the ac values now so we can do it 32-bits at a time
   memset(ದತ್ತಾಂಶ,0,64*ಗಾತ್ರ(ದತ್ತಾಂಶ[0]));

   diff = t ? stbi__extend_receive(j, t) : 0;
   dc = j->img_comp[b].dc_pred + diff;
   j->img_comp[b].dc_pred = dc;
   ದತ್ತಾಂಶ[0] = (ಗಿಡ್ಡ) (dc * dequant[0]);

   // decode AC components, see JPEG spec
   k = 1;
   ಮಾಡು {
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ zig;
      ಪೂರ್ಣಾಂಕ c,r,s;
      ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 16) stbi_grow_ಬಫರ್_unsafe(j);
      c = (j->ಸಂಕೇತ_ಬಫರ್ >> (32 - FAST_ಬಿಟ್ಟುಗಳು)) & ((1 << FAST_ಬಿಟ್ಟುಗಳು)-1);
      r = fac[c];
      ಆದರೆ (r) { // fast-AC path
         k += (r >> 4) & 15; //ಸತತ
         s = r & 15; // combined length
         j->ಸಂಕೇತ_ಬಫರ್ <<= s;
         j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು -= s;
         // decode into unzigzag'd location
         zig = stbi__jpeg_dezigzag[k++];
         ದತ್ತಾಂಶ[zig] = (ಗಿಡ್ಡ) ((r >> 8) * dequant[zig]);
      } ಇಲ್ಲದಿದ್ದರೆ {
         ಪೂರ್ಣಾಂಕ rs = stbi__jpeg_huff_decode(j, hac);
         ಆದರೆ (rs < 0) ಹಿಂತಿರುಗಿ stbi__err("bad huffman code","Corrupt JPEG");
         s = rs & 15;
         r = rs >> 4;
         ಆದರೆ (s == 0) {
            ಆದರೆ (rs != 0xf0) ನಿಲ್ಲಿಸು; // end block
            k += 16;
         } ಇಲ್ಲದಿದ್ದರೆ {
            k += r;
            // decode into unzigzag'd location
            zig = stbi__jpeg_dezigzag[k++];
            ದತ್ತಾಂಶ[zig] = (ಗಿಡ್ಡ) (stbi__extend_receive(j,s) * dequant[zig]);
         }
      }
   } ಆಗ (k < 64);
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_jpeg_decode_ಖಂಡ_prog_dc(stbi__jpeg *j, ಗಿಡ್ಡ ದತ್ತಾಂಶ[64], stbi__huffman *hdc, ಪೂರ್ಣಾಂಕ b)
{
   ಪೂರ್ಣಾಂಕ diff,dc;
   ಪೂರ್ಣಾಂಕ t;
   ಆದರೆ (j->spec_ಕೊನೆ != 0) ಹಿಂತಿರುಗಿ stbi__err("can't merge dc and ac", "Corrupt JPEG");

   ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 16) stbi_grow_ಬಫರ್_unsafe(j);

   ಆದರೆ (j->succ_ಮೇಲಿನ == 0) {
      // first scan for DC coefficient, must be first
      memset(ದತ್ತಾಂಶ,0,64*ಗಾತ್ರ(ದತ್ತಾಂಶ[0])); // 0 all the ac values now
      t = stbi__jpeg_huff_decode(j, hdc);
      diff = t ? stbi__extend_receive(j, t) : 0;

      dc = j->img_comp[b].dc_pred + diff;
      j->img_comp[b].dc_pred = dc;
      ದತ್ತಾಂಶ[0] = (ಗಿಡ್ಡ) (dc << j->succ_ಕೆಳಗಿನ);
   } ಇಲ್ಲದಿದ್ದರೆ {
      // refinement scan for DC coefficient
      ಆದರೆ (stbi_jpeg_get_ಬಿಟ್(j))
         ದತ್ತಾಂಶ[0] += (ಗಿಡ್ಡ) (1 << j->succ_ಕೆಳಗಿನ);
   }
   ಹಿಂತಿರುಗಿ 1;
}

// @OPTIMIZE: store non-zigzagged during the decode passes,
// and only de-zigzag when dequantizing
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_jpeg_decode_ಖಂಡ_prog_ac(stbi__jpeg *j, ಗಿಡ್ಡ ದತ್ತಾಂಶ[64], stbi__huffman *hac, stbi__int16 *fac)
{
   ಪೂರ್ಣಾಂಕ k;
   ಆದರೆ (j->spec_ಆರಂಭ == 0) ಹಿಂತಿರುಗಿ stbi__err("can't merge dc and ac", "Corrupt JPEG");

   ಆದರೆ (j->succ_ಮೇಲಿನ == 0) {
      ಪೂರ್ಣಾಂಕ ಸರಿಸು = j->succ_ಕೆಳಗಿನ;

      ಆದರೆ (j->eob_ಸತತ) {
         --j->eob_ಸತತ;
         ಹಿಂತಿರುಗಿ 1;
      }

      k = j->spec_ಆರಂಭ;
      ಮಾಡು {
         ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ zig;
         ಪೂರ್ಣಾಂಕ c,r,s;
         ಆದರೆ (j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 16) stbi_grow_ಬಫರ್_unsafe(j);
         c = (j->ಸಂಕೇತ_ಬಫರ್ >> (32 - FAST_ಬಿಟ್ಟುಗಳು)) & ((1 << FAST_ಬಿಟ್ಟುಗಳು)-1);
         r = fac[c];
         ಆದರೆ (r) { // fast-AC path
            k += (r >> 4) & 15; //ಸತತ
            s = r & 15; // combined length
            j->ಸಂಕೇತ_ಬಫರ್ <<= s;
            j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು -= s;
            zig = stbi__jpeg_dezigzag[k++];
            ದತ್ತಾಂಶ[zig] = (ಗಿಡ್ಡ) ((r >> 8) << ಸರಿಸು);
         } ಇಲ್ಲದಿದ್ದರೆ {
            ಪೂರ್ಣಾಂಕ rs = stbi__jpeg_huff_decode(j, hac);
            ಆದರೆ (rs < 0) ಹಿಂತಿರುಗಿ stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            ಆದರೆ (s == 0) {
               ಆದರೆ (r < 15) {
                  j->eob_ಸತತ = (1 << r);
                  ಆದರೆ (r)
                     j->eob_ಸತತ += stbi_jpeg_get_ಬಿಟ್ಟುಗಳು(j, r);
                  --j->eob_ಸತತ;
                  ನಿಲ್ಲಿಸು;
               }
               k += 16;
            } ಇಲ್ಲದಿದ್ದರೆ {
               k += r;
               zig = stbi__jpeg_dezigzag[k++];
               ದತ್ತಾಂಶ[zig] = (ಗಿಡ್ಡ) (stbi__extend_receive(j,s) << ಸರಿಸು);
            }
         }
      } ಆಗ (k <= j->spec_ಕೊನೆ);
   } ಇಲ್ಲದಿದ್ದರೆ {
      // refinement scan for these AC coefficients

      ಗಿಡ್ಡ ಬಿಟ್ = (ಗಿಡ್ಡ) (1 << j->succ_ಕೆಳಗಿನ);

      ಆದರೆ (j->eob_ಸತತ) {
         --j->eob_ಸತತ;
         ಗಾಗಿ (k = j->spec_ಆರಂಭ; k <= j->spec_ಕೊನೆ; ++k) {
            ಗಿಡ್ಡ *p = &ದತ್ತಾಂಶ[stbi__jpeg_dezigzag[k]];
            ಆದರೆ (*p != 0)
               ಆದರೆ (stbi_jpeg_get_ಬಿಟ್(j))
                  ಆದರೆ ((*p & ಬಿಟ್)==0) {
                     ಆದರೆ (*p > 0)
                        *p += ಬಿಟ್;
                     ಇಲ್ಲದಿದ್ದರೆ
                        *p -= ಬಿಟ್;
                  }
         }
      } ಇಲ್ಲದಿದ್ದರೆ {
         k = j->spec_ಆರಂಭ;
         ಮಾಡು {
            ಪೂರ್ಣಾಂಕ r,s;
            ಪೂರ್ಣಾಂಕ rs = stbi__jpeg_huff_decode(j, hac); // @OPTIMIZE see if we can use the fast path here, advance-by-r is so slow, eh
            ಆದರೆ (rs < 0) ಹಿಂತಿರುಗಿ stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            ಆದರೆ (s == 0) {
               ಆದರೆ (r < 15) {
                  j->eob_ಸತತ = (1 << r) - 1;
                  ಆದರೆ (r)
                     j->eob_ಸತತ += stbi_jpeg_get_ಬಿಟ್ಟುಗಳು(j, r);
                  r = 64; // force end of block
               } ಇಲ್ಲದಿದ್ದರೆ {
                  // r=15 s=0 should write 16 0s, so we just do
                  // a run of 15 0s and then write s (which is 0),
                  // so we don't have to do anything special here
               }
            } ಇಲ್ಲದಿದ್ದರೆ {
               ಆದರೆ (s != 1) ಹಿಂತಿರುಗಿ stbi__err("bad huffman code", "Corrupt JPEG");
               // sign bit
               ಆದರೆ (stbi_jpeg_get_ಬಿಟ್(j))
                  s = ಬಿಟ್;
               ಇಲ್ಲದಿದ್ದರೆ
                  s = -ಬಿಟ್;
            }

            // advance by r
            ಆಗ (k <= j->spec_ಕೊನೆ) {
               ಗಿಡ್ಡ *p = &ದತ್ತಾಂಶ[stbi__jpeg_dezigzag[k++]];
               ಆದರೆ (*p != 0) {
                  ಆದರೆ (stbi_jpeg_get_ಬಿಟ್(j))
                     ಆದರೆ ((*p & ಬಿಟ್)==0) {
                        ಆದರೆ (*p > 0)
                           *p += ಬಿಟ್;
                        ಇಲ್ಲದಿದ್ದರೆ
                           *p -= ಬಿಟ್;
                     }
               } ಇಲ್ಲದಿದ್ದರೆ {
                  ಆದರೆ (r == 0) {
                     *p = (ಗಿಡ್ಡ) s;
                     ನಿಲ್ಲಿಸು;
                  }
                  --r;
               }
            }
         } ಆಗ (k <= j->spec_ಕೊನೆ);
      }
   }
   ಹಿಂತಿರುಗಿ 1;
}

// take a -128..127 value and stbi__clamp it and convert to 0..255
stbi_inline ಸ್ಥಿರ stbi_uc stbi__clamp(ಪೂರ್ಣಾಂಕ x)
{
   // trick to use a single test to catch both cases
   ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) x > 255) {
      ಆದರೆ (x < 0) ಹಿಂತಿರುಗಿ 0;
      ಆದರೆ (x > 255) ಹಿಂತಿರುಗಿ 255;
   }
   ಹಿಂತಿರುಗಿ (stbi_uc) x;
}

#ವ್ಯಾಖ್ಯಾನಿಸು stbi__f2f(x)  ((ಪೂರ್ಣಾಂಕ) (((x) * 4096 + 0.5)))
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__fsh(x)  ((x) * 4096)

// derived from jidctint -- DCT_ISLOW
#ವ್ಯಾಖ್ಯಾನಿಸು STBI__IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7) \
   ಪೂರ್ಣಾಂಕ t0,t1,t2,t3,p1,p2,p3,p4,p5,x0,x1,x2,x3; \
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

ಸ್ಥಿರ ಶೂನ್ಯ stbi_idct_ಖಂಡ(stbi_uc *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_stride, ಗಿಡ್ಡ ದತ್ತಾಂಶ[64])
{
   ಪೂರ್ಣಾಂಕ i,val[64],*v=val;
   stbi_uc *o;
   ಗಿಡ್ಡ *d = ದತ್ತಾಂಶ;

   // columns
   ಗಾಗಿ (i=0; i < 8; ++i,++d, ++v) {
      // if all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing
      ಆದರೆ (d[ 8]==0 && d[16]==0 && d[24]==0 && d[32]==0
           && d[40]==0 && d[48]==0 && d[56]==0) {
         //    no shortcut                 0     seconds
         //    (1|2|3|4|5|6|7)==0          0     seconds
         //    all separate               -0.047 seconds
         //    1 && 2|3 && 4|5 && 6|7:    -0.047 seconds
         ಪೂರ್ಣಾಂಕ dcterm = d[0]*4;
         v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
      } ಇಲ್ಲದಿದ್ದರೆ {
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

   ಗಾಗಿ (i=0, v=val, o=ಹೊರಹೋಗು; i < 8; ++i,v+=8,o+=ಹೊರಹೋಗು_stride) {
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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_SSE2
// sse2 integer IDCT. not the fastest possible implementation but it
// produces bit-identical results to the generic C version so it's
// fully "transparent".
ಸ್ಥಿರ ಶೂನ್ಯ stbi__idct_simd(stbi_uc *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_stride, ಗಿಡ್ಡ ದತ್ತಾಂಶ[64])
{
   // This is constructed to match our regular (generic) integer IDCT exactly.
   __m128i row0, row1, row2, row3, row4, row5, row6, row7;
   __m128i tmp;

   // dot product constant: even elems=x, odd elems=y
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_const(x,y)  _mm_setr_epi16((x),(y),(x),(y),(x),(y),(x),(y))

   // out(0) = c0[even]*x + c0[odd]*y   (c0, x, y 16-bit, out 32-bit)
   // out(1) = c1[even]*x + c1[odd]*y
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_rot(out0,out1, x,y,c0,c1) \
      __m128i c0##lo = _mm_unpacklo_epi16((x),(y)); \
      __m128i c0##hi = _mm_unpackhi_epi16((x),(y)); \
      __m128i out0##_l = _mm_madd_epi16(c0##lo, c0); \
      __m128i out0##_h = _mm_madd_epi16(c0##hi, c0); \
      __m128i out1##_l = _mm_madd_epi16(c0##lo, c1); \
      __m128i out1##_h = _mm_madd_epi16(c0##hi, c1)

   // out = in << 12  (in 16-bit, out 32-bit)
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_widen(ಹೊರಹೋಗು, ಒಳ) \
      __m128i ಹೊರಹೋಗು##_l = _mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), (ಒಳ)), 4); \
      __m128i ಹೊರಹೋಗು##_h = _mm_srai_epi32(_mm_unpackhi_epi16(_mm_setzero_si128(), (ಒಳ)), 4)

   // wide add
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_wadd(ಹೊರಹೋಗು, a, b) \
      __m128i ಹೊರಹೋಗು##_l = mm_ಕೂಡಿಸು_epi32(a##_l, b##_l); \
      __m128i ಹೊರಹೋಗು##_h = mm_ಕೂಡಿಸು_epi32(a##_h, b##_h)

   // wide sub
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_wsub(ಹೊರಹೋಗು, a, b) \
      __m128i ಹೊರಹೋಗು##_l = _mm_sub_epi32(a##_l, b##_l); \
      __m128i ಹೊರಹೋಗು##_h = _mm_sub_epi32(a##_h, b##_h)

   // butterfly a/b, add bias, then shift by "s" and pack
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_bfly32o(out0, out1, a,b,bias,s) \
      { \
         __m128i abiased_l = mm_ಕೂಡಿಸು_epi32(a##_l, bias); \
         __m128i abiased_h = mm_ಕೂಡಿಸು_epi32(a##_h, bias); \
         dct_wadd(ಮೊತ್ತ, abiased, b); \
         dct_wsub(dif, abiased, b); \
         out0 = _mm_packs_epi32(_mm_srai_epi32(ಮೊತ್ತ_l, s), _mm_srai_epi32(ಮೊತ್ತ_h, s)); \
         out1 = _mm_packs_epi32(_mm_srai_epi32(dif_l, s), _mm_srai_epi32(dif_h, s)); \
      }

   // 8-bit interleave step (for transposes)
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_interleave8(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi8(a, b); \
      b = _mm_unpackhi_epi8(tmp, b)

   // 16-bit interleave step (for transposes)
   #ವ್ಯಾಖ್ಯಾನಿಸು dct_interleave16(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi16(a, b); \
      b = _mm_unpackhi_epi16(tmp, b)

   #ವ್ಯಾಖ್ಯಾನಿಸು dct_pass(bias,ಸರಿಸು) \
      { \
         /* even part */ \
         dct_rot(t2e,t3e, row2,row6, rot0_0,rot0_1); \
         __m128i sum04 = mm_ಕೂಡಿಸು_epi16(row0, row4); \
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
         __m128i sum17 = mm_ಕೂಡಿಸು_epi16(row1, row7); \
         __m128i sum35 = mm_ಕೂಡಿಸು_epi16(row3, row5); \
         dct_rot(y4o,y5o, sum17,sum35, rot1_0,rot1_1); \
         dct_wadd(x4, y0o, y4o); \
         dct_wadd(x5, y1o, y5o); \
         dct_wadd(x6, y2o, y5o); \
         dct_wadd(x7, y3o, y4o); \
         dct_bfly32o(row0,row7, x0,x7,bias,ಸರಿಸು); \
         dct_bfly32o(row1,row6, x1,x6,bias,ಸರಿಸು); \
         dct_bfly32o(row2,row5, x2,x5,bias,ಸರಿಸು); \
         dct_bfly32o(row3,row4, x3,x4,bias,ಸರಿಸು); \
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

   //ತುಂಬು
   row0 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 0*8));
   row1 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 1*8));
   row2 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 2*8));
   row3 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 3*8));
   row4 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 4*8));
   row5 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 5*8));
   row6 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 6*8));
   row7 = mm_ತುಂಬು_si128((ಸ್ಥಿರಾಂಕ __m128i *) (ದತ್ತಾಂಶ + 7*8));

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
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, p0); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, _mm_shuffle_epi32(p0, 0x4e)); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, p2); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, _mm_shuffle_epi32(p2, 0x4e)); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, p1); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, _mm_shuffle_epi32(p1, 0x4e)); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, p3); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      _mm_storel_epi64((__m128i *) ಹೊರಹೋಗು, _mm_shuffle_epi32(p3, 0x4e));
   }

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_const
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_rot
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_widen
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_wadd
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_wsub
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_bfly32o
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_interleave8
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_interleave16
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_pass
}

#ಆದರೆ_ಅಂತ್ಯ // STBI_SSE2

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_NEON

// NEON integer IDCT. should produce bit-identical
// results to the generic C version.
ಸ್ಥಿರ ಶೂನ್ಯ stbi__idct_simd(stbi_uc *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_stride, ಗಿಡ್ಡ ದತ್ತಾಂಶ[64])
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

#ವ್ಯಾಖ್ಯಾನಿಸು dct_long_mul(ಹೊರಹೋಗು, inq, coeff) \
   int32x4_t ಹೊರಹೋಗು##_l = vmull_s16(vget_ಕೆಳಗಿನ_s16(inq), coeff); \
   int32x4_t ಹೊರಹೋಗು##_h = vmull_s16(vget_ಮೇಲಿನ_s16(inq), coeff)

#ವ್ಯಾಖ್ಯಾನಿಸು dct_long_mac(ಹೊರಹೋಗು, acc, inq, coeff) \
   int32x4_t ಹೊರಹೋಗು##_l = vmlal_s16(acc##_l, vget_ಕೆಳಗಿನ_s16(inq), coeff); \
   int32x4_t ಹೊರಹೋಗು##_h = vmlal_s16(acc##_h, vget_ಮೇಲಿನ_s16(inq), coeff)

#ವ್ಯಾಖ್ಯಾನಿಸು dct_widen(ಹೊರಹೋಗು, inq) \
   int32x4_t ಹೊರಹೋಗು##_l = vshll_n_s16(vget_ಕೆಳಗಿನ_s16(inq), 12); \
   int32x4_t ಹೊರಹೋಗು##_h = vshll_n_s16(vget_ಮೇಲಿನ_s16(inq), 12)

// wide add
#ವ್ಯಾಖ್ಯಾನಿಸು dct_wadd(ಹೊರಹೋಗು, a, b) \
   int32x4_t ಹೊರಹೋಗು##_l = vaddq_s32(a##_l, b##_l); \
   int32x4_t ಹೊರಹೋಗು##_h = vaddq_s32(a##_h, b##_h)

// wide sub
#ವ್ಯಾಖ್ಯಾನಿಸು dct_wsub(ಹೊರಹೋಗು, a, b) \
   int32x4_t ಹೊರಹೋಗು##_l = vsubq_s32(a##_l, b##_l); \
   int32x4_t ಹೊರಹೋಗು##_h = vsubq_s32(a##_h, b##_h)

// butterfly a/b, then shift using "shiftop" by "s" and pack
#ವ್ಯಾಖ್ಯಾನಿಸು dct_bfly32o(out0,out1, a,b,shiftop,s) \
   { \
      dct_wadd(ಮೊತ್ತ, a, b); \
      dct_wsub(dif, a, b); \
      out0 = vcombine_s16(shiftop(ಮೊತ್ತ_l, s), shiftop(ಮೊತ್ತ_h, s)); \
      out1 = vcombine_s16(shiftop(dif_l, s), shiftop(dif_h, s)); \
   }

#ವ್ಯಾಖ್ಯಾನಿಸು dct_pass(shiftop, ಸರಿಸು) \
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
      dct_bfly32o(row0,row7, x0,x7,shiftop,ಸರಿಸು); \
      dct_bfly32o(row1,row6, x1,x6,shiftop,ಸರಿಸು); \
      dct_bfly32o(row2,row5, x2,x5,shiftop,ಸರಿಸು); \
      dct_bfly32o(row3,row4, x3,x4,shiftop,ಸರಿಸು); \
   }

   //ತುಂಬು
   row0 = vld1q_s16(ದತ್ತಾಂಶ + 0*8);
   row1 = vld1q_s16(ದತ್ತಾಂಶ + 1*8);
   row2 = vld1q_s16(ದತ್ತಾಂಶ + 2*8);
   row3 = vld1q_s16(ದತ್ತಾಂಶ + 3*8);
   row4 = vld1q_s16(ದತ್ತಾಂಶ + 4*8);
   row5 = vld1q_s16(ದತ್ತಾಂಶ + 5*8);
   row6 = vld1q_s16(ದತ್ತಾಂಶ + 6*8);
   row7 = vld1q_s16(ದತ್ತಾಂಶ + 7*8);

   // add DC bias
   row0 = vaddq_s16(row0, vsetq_lane_s16(1024, vdupq_n_s16(0), 0));

   // column pass
   dct_pass(vrshrn_n_s32, 10);

   // 16bit 8x8 transpose
   {
// these three map to a single VTRN.16, VTRN.32, and VSWP, respectively.
// whether compilers actually get this is another story, sadly.
#ವ್ಯಾಖ್ಯಾನಿಸು dct_trn16(x, y) { int16x8x2_t t = vtrnq_s16(x, y); x = t.val[0]; y = t.val[1]; }
#ವ್ಯಾಖ್ಯಾನಿಸು dct_trn32(x, y) { int32x4x2_t t = vtrnq_s32(vreinterpretq_s32_s16(x), vreinterpretq_s32_s16(y)); x = vreinterpretq_s16_s32(t.val[0]); y = vreinterpretq_s16_s32(t.val[1]); }
#ವ್ಯಾಖ್ಯಾನಿಸು dct_trn64(x, y) { int16x8_t x0 = x; int16x8_t y0 = y; x = vcombine_s16(vget_ಕೆಳಗಿನ_s16(x0), vget_ಕೆಳಗಿನ_s16(y0)); y = vcombine_s16(vget_ಮೇಲಿನ_s16(x0), vget_ಮೇಲಿನ_s16(y0)); }

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

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_trn16
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_trn32
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_trn64
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
#ವ್ಯಾಖ್ಯಾನಿಸು dct_trn8_8(x, y) { uint8x8x2_t t = vtrn_u8(x, y); x = t.val[0]; y = t.val[1]; }
#ವ್ಯಾಖ್ಯಾನಿಸು dct_trn8_16(x, y) { uint16x4x2_t t = vtrn_u16(vreinterpret_u16_u8(x), vreinterpret_u16_u8(y)); x = vreinterpret_u8_u16(t.val[0]); y = vreinterpret_u8_u16(t.val[1]); }
#ವ್ಯಾಖ್ಯಾನಿಸು dct_trn8_32(x, y) { uint32x2x2_t t = vtrn_u32(vreinterpret_u32_u8(x), vreinterpret_u32_u8(y)); x = vreinterpret_u8_u32(t.val[0]); y = vreinterpret_u8_u32(t.val[1]); }

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
      vst1_u8(ಹೊರಹೋಗು, p0); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      vst1_u8(ಹೊರಹೋಗು, p1); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      vst1_u8(ಹೊರಹೋಗು, p2); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      vst1_u8(ಹೊರಹೋಗು, p3); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      vst1_u8(ಹೊರಹೋಗು, p4); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      vst1_u8(ಹೊರಹೋಗು, p5); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      vst1_u8(ಹೊರಹೋಗು, p6); ಹೊರಹೋಗು += ಹೊರಹೋಗು_stride;
      vst1_u8(ಹೊರಹೋಗು, p7);

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_trn8_8
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_trn8_16
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_trn8_32
   }

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_long_mul
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_long_mac
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_widen
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_wadd
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_wsub
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_bfly32o
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು dct_pass
}

#ಆದರೆ_ಅಂತ್ಯ // STBI_NEON

#ವ್ಯಾಖ್ಯಾನಿಸು STBI__MARKER_none  0xff
// if there's a pending marker from the entropy stream, return that
// otherwise, fetch from the stream and get a marker. if there's no
// marker, return 0xff, which is never a valid marker value
ಸ್ಥಿರ stbi_uc stbi__get_marker(stbi__jpeg *j)
{
   stbi_uc x;
   ಆದರೆ (j->marker != STBI__MARKER_none) { x = j->marker; j->marker = STBI__MARKER_none; ಹಿಂತಿರುಗಿ x; }
   x = stbi__get8(j->s);
   ಆದರೆ (x != 0xff) ಹಿಂತಿರುಗಿ STBI__MARKER_none;
   ಆಗ (x == 0xff)
      x = stbi__get8(j->s); // consume repeated 0xff fill bytes
   ಹಿಂತಿರುಗಿ x;
}

// in each scan, we'll have scan_n components, and the order
// of the components is specified by order[]
#ವ್ಯಾಖ್ಯಾನಿಸು STBI__RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)

// after a restart interval, stbi__jpeg_reset the entropy decoder and
// the dc prediction
ಸ್ಥಿರ ಶೂನ್ಯ stbi__jpeg_reset(stbi__jpeg *j)
{
   j->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು = 0;
   j->ಸಂಕೇತ_ಬಫರ್ = 0;
   j->nomore = 0;
   j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = j->img_comp[3].dc_pred = 0;
   j->marker = STBI__MARKER_none;
   j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
   j->eob_ಸತತ = 0;
   // no more than 1<<31 MCUs if no restart_interal? that's plenty safe,
   // since we don't even allow 1<<30 pixels
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_parse_entropy_coded_ದತ್ತಾಂಶ(stbi__jpeg *z)
{
   stbi__jpeg_reset(z);
   ಆದರೆ (!z->progressive) {
      ಆದರೆ (z->ಪರಿಶೀಲನೆ_n == 1) {
         ಪೂರ್ಣಾಂಕ i,j;
         STBI_SIMD_ALIGN(ಗಿಡ್ಡ, ದತ್ತಾಂಶ[64]);
         ಪೂರ್ಣಾಂಕ n = z->ಕ್ರಮ[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         ಪೂರ್ಣಾಂಕ w = (z->img_comp[n].x+7) >> 3;
         ಪೂರ್ಣಾಂಕ h = (z->img_comp[n].y+7) >> 3;
         ಗಾಗಿ (j=0; j < h; ++j) {
            ಗಾಗಿ (i=0; i < w; ++i) {
               ಪೂರ್ಣಾಂಕ ha = z->img_comp[n].ha;
               ಆದರೆ (!stbi_jpeg_decode_ಖಂಡ(z, ದತ್ತಾಂಶ, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) ಹಿಂತಿರುಗಿ 0;
               z->idct_ಖಂಡ_kernel(z->img_comp[n].ದತ್ತಾಂಶ+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, ದತ್ತಾಂಶ);
               // every data block is an MCU, so countdown the restart interval
               ಆದರೆ (--z->todo <= 0) {
                  ಆದರೆ (z->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 24) stbi_grow_ಬಫರ್_unsafe(z);
                  // if it's NOT a restart, then just bail, so we get corrupt data
                  // rather than no data
                  ಆದರೆ (!STBI__RESTART(z->marker)) ಹಿಂತಿರುಗಿ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ಹಿಂತಿರುಗಿ 1;
      } ಇಲ್ಲದಿದ್ದರೆ { // interleaved
         ಪೂರ್ಣಾಂಕ i,j,k,x,y;
         STBI_SIMD_ALIGN(ಗಿಡ್ಡ, ದತ್ತಾಂಶ[64]);
         ಗಾಗಿ (j=0; j < z->img_mcu_y; ++j) {
            ಗಾಗಿ (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               ಗಾಗಿ (k=0; k < z->ಪರಿಶೀಲನೆ_n; ++k) {
                  ಪೂರ್ಣಾಂಕ n = z->ಕ್ರಮ[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  ಗಾಗಿ (y=0; y < z->img_comp[n].v; ++y) {
                     ಗಾಗಿ (x=0; x < z->img_comp[n].h; ++x) {
                        ಪೂರ್ಣಾಂಕ x2 = (i*z->img_comp[n].h + x)*8;
                        ಪೂರ್ಣಾಂಕ y2 = (j*z->img_comp[n].v + y)*8;
                        ಪೂರ್ಣಾಂಕ ha = z->img_comp[n].ha;
                        ಆದರೆ (!stbi_jpeg_decode_ಖಂಡ(z, ದತ್ತಾಂಶ, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) ಹಿಂತಿರುಗಿ 0;
                        z->idct_ಖಂಡ_kernel(z->img_comp[n].ದತ್ತಾಂಶ+z->img_comp[n].w2*y2+x2, z->img_comp[n].w2, ದತ್ತಾಂಶ);
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               ಆದರೆ (--z->todo <= 0) {
                  ಆದರೆ (z->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 24) stbi_grow_ಬಫರ್_unsafe(z);
                  ಆದರೆ (!STBI__RESTART(z->marker)) ಹಿಂತಿರುಗಿ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ಹಿಂತಿರುಗಿ 1;
      }
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಆದರೆ (z->ಪರಿಶೀಲನೆ_n == 1) {
         ಪೂರ್ಣಾಂಕ i,j;
         ಪೂರ್ಣಾಂಕ n = z->ಕ್ರಮ[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         ಪೂರ್ಣಾಂಕ w = (z->img_comp[n].x+7) >> 3;
         ಪೂರ್ಣಾಂಕ h = (z->img_comp[n].y+7) >> 3;
         ಗಾಗಿ (j=0; j < h; ++j) {
            ಗಾಗಿ (i=0; i < w; ++i) {
               ಗಿಡ್ಡ *ದತ್ತಾಂಶ = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               ಆದರೆ (z->spec_ಆರಂಭ == 0) {
                  ಆದರೆ (!stbi_jpeg_decode_ಖಂಡ_prog_dc(z, ದತ್ತಾಂಶ, &z->huff_dc[z->img_comp[n].hd], n))
                     ಹಿಂತಿರುಗಿ 0;
               } ಇಲ್ಲದಿದ್ದರೆ {
                  ಪೂರ್ಣಾಂಕ ha = z->img_comp[n].ha;
                  ಆದರೆ (!stbi_jpeg_decode_ಖಂಡ_prog_ac(z, ದತ್ತಾಂಶ, &z->huff_ac[ha], z->fast_ac[ha]))
                     ಹಿಂತಿರುಗಿ 0;
               }
               // every data block is an MCU, so countdown the restart interval
               ಆದರೆ (--z->todo <= 0) {
                  ಆದರೆ (z->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 24) stbi_grow_ಬಫರ್_unsafe(z);
                  ಆದರೆ (!STBI__RESTART(z->marker)) ಹಿಂತಿರುಗಿ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ಹಿಂತಿರುಗಿ 1;
      } ಇಲ್ಲದಿದ್ದರೆ { // interleaved
         ಪೂರ್ಣಾಂಕ i,j,k,x,y;
         ಗಾಗಿ (j=0; j < z->img_mcu_y; ++j) {
            ಗಾಗಿ (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               ಗಾಗಿ (k=0; k < z->ಪರಿಶೀಲನೆ_n; ++k) {
                  ಪೂರ್ಣಾಂಕ n = z->ಕ್ರಮ[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  ಗಾಗಿ (y=0; y < z->img_comp[n].v; ++y) {
                     ಗಾಗಿ (x=0; x < z->img_comp[n].h; ++x) {
                        ಪೂರ್ಣಾಂಕ x2 = (i*z->img_comp[n].h + x);
                        ಪೂರ್ಣಾಂಕ y2 = (j*z->img_comp[n].v + y);
                        ಗಿಡ್ಡ *ದತ್ತಾಂಶ = z->img_comp[n].coeff + 64 * (x2 + y2 * z->img_comp[n].coeff_w);
                        ಆದರೆ (!stbi_jpeg_decode_ಖಂಡ_prog_dc(z, ದತ್ತಾಂಶ, &z->huff_dc[z->img_comp[n].hd], n))
                           ಹಿಂತಿರುಗಿ 0;
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               ಆದರೆ (--z->todo <= 0) {
                  ಆದರೆ (z->ಸಂಕೇತ_ಬಿಟ್ಟುಗಳು < 24) stbi_grow_ಬಫರ್_unsafe(z);
                  ಆದರೆ (!STBI__RESTART(z->marker)) ಹಿಂತಿರುಗಿ 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         ಹಿಂತಿರುಗಿ 1;
      }
   }
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__jpeg_dequantize(ಗಿಡ್ಡ *ದತ್ತಾಂಶ, stbi__uint16 *dequant)
{
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; i < 64; ++i)
      ದತ್ತಾಂಶ[i] *= dequant[i];
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi_jpeg_ಮುಕ್ತಾಯ(stbi__jpeg *z)
{
   ಆದರೆ (z->progressive) {
      // dequantize and idct the data
      ಪೂರ್ಣಾಂಕ i,j,n;
      ಗಾಗಿ (n=0; n < z->s->img_n; ++n) {
         ಪೂರ್ಣಾಂಕ w = (z->img_comp[n].x+7) >> 3;
         ಪೂರ್ಣಾಂಕ h = (z->img_comp[n].y+7) >> 3;
         ಗಾಗಿ (j=0; j < h; ++j) {
            ಗಾಗಿ (i=0; i < w; ++i) {
               ಗಿಡ್ಡ *ದತ್ತಾಂಶ = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               stbi__jpeg_dequantize(ದತ್ತಾಂಶ, z->dequant[z->img_comp[n].tq]);
               z->idct_ಖಂಡ_kernel(z->img_comp[n].ದತ್ತಾಂಶ+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, ದತ್ತಾಂಶ);
            }
         }
      }
   }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__process_marker(stbi__jpeg *z, ಪೂರ್ಣಾಂಕ m)
{
   ಪೂರ್ಣಾಂಕ L;
   ಆಯ್ಕೆ (m) {
      ಸ್ಥಿತಿ STBI__MARKER_none: // no marker found
         ಹಿಂತಿರುಗಿ stbi__err("expected marker","Corrupt JPEG");

      ಸ್ಥಿತಿ 0xDD: // DRI - specify restart interval
         ಆದರೆ (stbi__get16be(z->s) != 4) ಹಿಂತಿರುಗಿ stbi__err("bad DRI len","Corrupt JPEG");
         z->restart_interval = stbi__get16be(z->s);
         ಹಿಂತಿರುಗಿ 1;

      ಸ್ಥಿತಿ 0xDB: // DQT - define quantization table
         L = stbi__get16be(z->s)-2;
         ಆಗ (L > 0) {
            ಪೂರ್ಣಾಂಕ q = stbi__get8(z->s);
            ಪೂರ್ಣಾಂಕ p = q >> 4, sixteen = (p != 0);
            ಪೂರ್ಣಾಂಕ t = q & 15,i;
            ಆದರೆ (p != 0 && p != 1) ಹಿಂತಿರುಗಿ stbi__err("bad DQT type","Corrupt JPEG");
            ಆದರೆ (t > 3) ಹಿಂತಿರುಗಿ stbi__err("bad DQT table","Corrupt JPEG");

            ಗಾಗಿ (i=0; i < 64; ++i)
               z->dequant[t][stbi__jpeg_dezigzag[i]] = (stbi__uint16)(sixteen ? stbi__get16be(z->s) : stbi__get8(z->s));
            L -= (sixteen ? 129 : 65);
         }
         ಹಿಂತಿರುಗಿ L==0;

      ಸ್ಥಿತಿ 0xC4: // DHT - define huffman table
         L = stbi__get16be(z->s)-2;
         ಆಗ (L > 0) {
            stbi_uc *v;
            ಪೂರ್ಣಾಂಕ sizes[16],i,n=0;
            ಪೂರ್ಣಾಂಕ q = stbi__get8(z->s);
            ಪೂರ್ಣಾಂಕ tc = q >> 4;
            ಪೂರ್ಣಾಂಕ th = q & 15;
            ಆದರೆ (tc > 1 || th > 3) ಹಿಂತಿರುಗಿ stbi__err("bad DHT header","Corrupt JPEG");
            ಗಾಗಿ (i=0; i < 16; ++i) {
               sizes[i] = stbi__get8(z->s);
               n += sizes[i];
            }
            L -= 17;
            ಆದರೆ (tc == 0) {
               ಆದರೆ (!stbi__build_huffman(z->huff_dc+th, sizes)) ಹಿಂತಿರುಗಿ 0;
               v = z->huff_dc[th].ಮೌಲ್ಯಗಳು;
            } ಇಲ್ಲದಿದ್ದರೆ {
               ಆದರೆ (!stbi__build_huffman(z->huff_ac+th, sizes)) ಹಿಂತಿರುಗಿ 0;
               v = z->huff_ac[th].ಮೌಲ್ಯಗಳು;
            }
            ಗಾಗಿ (i=0; i < n; ++i)
               v[i] = stbi__get8(z->s);
            ಆದರೆ (tc != 0)
               stbi__build_fast_ac(z->fast_ac[th], z->huff_ac + th);
            L -= n;
         }
         ಹಿಂತಿರುಗಿ L==0;
   }

   // check for comment block or APP blocks
   ಆದರೆ ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
      L = stbi__get16be(z->s);
      ಆದರೆ (L < 2) {
         ಆದರೆ (m == 0xFE)
            ಹಿಂತಿರುಗಿ stbi__err("bad COM len","Corrupt JPEG");
         ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ stbi__err("bad APP len","Corrupt JPEG");
      }
      L -= 2;

      ಆದರೆ (m == 0xE0 && L >= 5) { // JFIF APP0 segment
         ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಟ್ಯಾಗ್[5] = {'J','F','I','F','\0'};
         ಪೂರ್ಣಾಂಕ ok = 1;
         ಪೂರ್ಣಾಂಕ i;
         ಗಾಗಿ (i=0; i < 5; ++i)
            ಆದರೆ (stbi__get8(z->s) != ಟ್ಯಾಗ್[i])
               ok = 0;
         L -= 5;
         ಆದರೆ (ok)
            z->jfif = 1;
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (m == 0xEE && L >= 12) { // Adobe APP14 segment
         ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಟ್ಯಾಗ್[6] = {'A','d','o','b','e','\0'};
         ಪೂರ್ಣಾಂಕ ok = 1;
         ಪೂರ್ಣಾಂಕ i;
         ಗಾಗಿ (i=0; i < 6; ++i)
            ಆದರೆ (stbi__get8(z->s) != ಟ್ಯಾಗ್[i])
               ok = 0;
         L -= 6;
         ಆದರೆ (ok) {
            stbi__get8(z->s); //ಆವೃತ್ತಿ
            stbi__get16be(z->s); // flags0
            stbi__get16be(z->s); // flags1
            z->app14_color_transform = stbi__get8(z->s); // color transform
            L -= 6;
         }
      }

      stbi__skip(z->s, L);
      ಹಿಂತಿರುಗಿ 1;
   }

   ಹಿಂತಿರುಗಿ stbi__err("unknown marker","Corrupt JPEG");
}

// after we see SOS
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_process_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ(stbi__jpeg *z)
{
   ಪೂರ್ಣಾಂಕ i;
   ಪೂರ್ಣಾಂಕ Ls = stbi__get16be(z->s);
   z->ಪರಿಶೀಲನೆ_n = stbi__get8(z->s);
   ಆದರೆ (z->ಪರಿಶೀಲನೆ_n < 1 || z->ಪರಿಶೀಲನೆ_n > 4 || z->ಪರಿಶೀಲನೆ_n > (ಪೂರ್ಣಾಂಕ) z->s->img_n) ಹಿಂತಿರುಗಿ stbi__err("bad SOS component count","Corrupt JPEG");
   ಆದರೆ (Ls != 6+2*z->ಪರಿಶೀಲನೆ_n) ಹಿಂತಿರುಗಿ stbi__err("bad SOS len","Corrupt JPEG");
   ಗಾಗಿ (i=0; i < z->ಪರಿಶೀಲನೆ_n; ++i) {
      ಪೂರ್ಣಾಂಕ id = stbi__get8(z->s), which;
      ಪೂರ್ಣಾಂಕ q = stbi__get8(z->s);
      ಗಾಗಿ (which = 0; which < z->s->img_n; ++which)
         ಆದರೆ (z->img_comp[which].id == id)
            ನಿಲ್ಲಿಸು;
      ಆದರೆ (which == z->s->img_n) ಹಿಂತಿರುಗಿ 0; // no match
      z->img_comp[which].hd = q >> 4;   ಆದರೆ (z->img_comp[which].hd > 3) ಹಿಂತಿರುಗಿ stbi__err("bad DC huff","Corrupt JPEG");
      z->img_comp[which].ha = q & 15;   ಆದರೆ (z->img_comp[which].ha > 3) ಹಿಂತಿರುಗಿ stbi__err("bad AC huff","Corrupt JPEG");
      z->ಕ್ರಮ[i] = which;
   }

   {
      ಪೂರ್ಣಾಂಕ aa;
      z->spec_ಆರಂಭ = stbi__get8(z->s);
      z->spec_ಕೊನೆ   = stbi__get8(z->s); // should be 63, but might be 0
      aa = stbi__get8(z->s);
      z->succ_ಮೇಲಿನ = (aa >> 4);
      z->succ_ಕೆಳಗಿನ  = (aa & 15);
      ಆದರೆ (z->progressive) {
         ಆದರೆ (z->spec_ಆರಂಭ > 63 || z->spec_ಕೊನೆ > 63  || z->spec_ಆರಂಭ > z->spec_ಕೊನೆ || z->succ_ಮೇಲಿನ > 13 || z->succ_ಕೆಳಗಿನ > 13)
            ಹಿಂತಿರುಗಿ stbi__err("bad SOS", "Corrupt JPEG");
      } ಇಲ್ಲದಿದ್ದರೆ {
         ಆದರೆ (z->spec_ಆರಂಭ != 0) ಹಿಂತಿರುಗಿ stbi__err("bad SOS","Corrupt JPEG");
         ಆದರೆ (z->succ_ಮೇಲಿನ != 0 || z->succ_ಕೆಳಗಿನ != 0) ಹಿಂತಿರುಗಿ stbi__err("bad SOS","Corrupt JPEG");
         z->spec_ಕೊನೆ = 63;
      }
   }

   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__free_jpeg_components(stbi__jpeg *z, ಪೂರ್ಣಾಂಕ ncomp, ಪೂರ್ಣಾಂಕ why)
{
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; i < ncomp; ++i) {
      ಆದರೆ (z->img_comp[i].raw_ದತ್ತಾಂಶ) {
         STBI_FREE(z->img_comp[i].raw_ದತ್ತಾಂಶ);
         z->img_comp[i].raw_ದತ್ತಾಂಶ = ಖಾಲಿ;
         z->img_comp[i].ದತ್ತಾಂಶ = ಖಾಲಿ;
      }
      ಆದರೆ (z->img_comp[i].raw_coeff) {
         STBI_FREE(z->img_comp[i].raw_coeff);
         z->img_comp[i].raw_coeff = 0;
         z->img_comp[i].coeff = 0;
      }
      ಆದರೆ (z->img_comp[i].linebuf) {
         STBI_FREE(z->img_comp[i].linebuf);
         z->img_comp[i].linebuf = ಖಾಲಿ;
      }
   }
   ಹಿಂತಿರುಗಿ why;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_process_ಚೌಕಟ್ಟು_ಶೀರ್ಷಿಕೆ(stbi__jpeg *z, ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ)
{
   stbi__context *s = z->s;
   ಪೂರ್ಣಾಂಕ Lf,p,i,q, h_max=1,v_max=1,c;
   Lf = stbi__get16be(s);         ಆದರೆ (Lf < 11) ಹಿಂತಿರುಗಿ stbi__err("bad SOF len","Corrupt JPEG"); // JPEG
   p  = stbi__get8(s);            ಆದರೆ (p != 8) ಹಿಂತಿರುಗಿ stbi__err("only 8-bit","JPEG format not supported: 8-bit only"); // JPEG baseline
   s->img_y = stbi__get16be(s);   ಆದರೆ (s->img_y == 0) ಹಿಂತಿರುಗಿ stbi__err("no header height", "JPEG format not supported: delayed height"); // Legal, but we don't handle it--but neither does IJG
   s->img_x = stbi__get16be(s);   ಆದರೆ (s->img_x == 0) ಹಿಂತಿರುಗಿ stbi__err("0 width","Corrupt JPEG"); // JPEG requires
   c = stbi__get8(s);
   ಆದರೆ (c != 3 && c != 1 && c != 4) ಹಿಂತಿರುಗಿ stbi__err("bad component count","Corrupt JPEG");
   s->img_n = c;
   ಗಾಗಿ (i=0; i < c; ++i) {
      z->img_comp[i].ದತ್ತಾಂಶ = ಖಾಲಿ;
      z->img_comp[i].linebuf = ಖಾಲಿ;
   }

   ಆದರೆ (Lf != 8+3*s->img_n) ಹಿಂತಿರುಗಿ stbi__err("bad SOF len","Corrupt JPEG");

   z->rgb = 0;
   ಗಾಗಿ (i=0; i < s->img_n; ++i) {
      ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ rgb[3] = { 'R', 'G', 'B' };
      z->img_comp[i].id = stbi__get8(s);
      ಆದರೆ (s->img_n == 3 && z->img_comp[i].id == rgb[i])
         ++z->rgb;
      q = stbi__get8(s);
      z->img_comp[i].h = (q >> 4);  ಆದರೆ (!z->img_comp[i].h || z->img_comp[i].h > 4) ಹಿಂತಿರುಗಿ stbi__err("bad H","Corrupt JPEG");
      z->img_comp[i].v = q & 15;    ಆದರೆ (!z->img_comp[i].v || z->img_comp[i].v > 4) ಹಿಂತಿರುಗಿ stbi__err("bad V","Corrupt JPEG");
      z->img_comp[i].tq = stbi__get8(s);  ಆದರೆ (z->img_comp[i].tq > 3) ಹಿಂತಿರುಗಿ stbi__err("bad TQ","Corrupt JPEG");
   }

   ಆದರೆ (ಪರಿಶೀಲನೆ != STBI_ಪರಿಶೀಲನೆ_ತುಂಬು) ಹಿಂತಿರುಗಿ 1;

   ಆದರೆ (!stbi__mad3sizes_valid(s->img_x, s->img_y, s->img_n, 0)) ಹಿಂತಿರುಗಿ stbi__err("too large", "Image too large to decode");

   ಗಾಗಿ (i=0; i < s->img_n; ++i) {
      ಆದರೆ (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
      ಆದರೆ (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
   }

   // compute interleaved mcu info
   z->img_h_max = h_max;
   z->img_v_max = v_max;
   z->img_mcu_w = h_max * 8;
   z->img_mcu_h = v_max * 8;
   // these sizes can't be more than 17 bits
   z->img_mcu_x = (s->img_x + z->img_mcu_w-1) / z->img_mcu_w;
   z->img_mcu_y = (s->img_y + z->img_mcu_h-1) / z->img_mcu_h;

   ಗಾಗಿ (i=0; i < s->img_n; ++i) {
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
      z->img_comp[i].linebuf = ಖಾಲಿ;
      z->img_comp[i].raw_ದತ್ತಾಂಶ = stbi__malloc_mad2(z->img_comp[i].w2, z->img_comp[i].h2, 15);
      ಆದರೆ (z->img_comp[i].raw_ದತ್ತಾಂಶ == ಖಾಲಿ)
         ಹಿಂತಿರುಗಿ stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
      // align blocks for idct using mmx/sse
      z->img_comp[i].ದತ್ತಾಂಶ = (stbi_uc*) (((size_t) z->img_comp[i].raw_ದತ್ತಾಂಶ + 15) & ~15);
      ಆದರೆ (z->progressive) {
         // w2, h2 are multiples of 8 (see above)
         z->img_comp[i].coeff_w = z->img_comp[i].w2 / 8;
         z->img_comp[i].coeff_h = z->img_comp[i].h2 / 8;
         z->img_comp[i].raw_coeff = stbi__malloc_mad3(z->img_comp[i].w2, z->img_comp[i].h2, ಗಾತ್ರ(ಗಿಡ್ಡ), 15);
         ಆದರೆ (z->img_comp[i].raw_coeff == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
         z->img_comp[i].coeff = (ಗಿಡ್ಡ*) (((size_t) z->img_comp[i].raw_coeff + 15) & ~15);
      }
   }

   ಹಿಂತಿರುಗಿ 1;
}

// use comparisons since in some cases we handle more than one case (e.g. SOF)
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__DNL(x)         ((x) == 0xdc)
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__SOI(x)         ((x) == 0xd8)
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__EOI(x)         ((x) == 0xd9)
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__SOF(x)         ((x) == 0xc0 || (x) == 0xc1 || (x) == 0xc2)
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__SOS(x)         ((x) == 0xda)

#ವ್ಯಾಖ್ಯಾನಿಸು stbi__SOF_progressive(x)   ((x) == 0xc2)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_decode_jpeg_ಶೀರ್ಷಿಕೆ(stbi__jpeg *z, ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ)
{
   ಪೂರ್ಣಾಂಕ m;
   z->jfif = 0;
   z->app14_color_transform = -1; // valid values are 0,1,2
   z->marker = STBI__MARKER_none; // initialize cached marker to empty
   m = stbi__get_marker(z);
   ಆದರೆ (!stbi__SOI(m)) ಹಿಂತಿರುಗಿ stbi__err("no SOI","Corrupt JPEG");
   ಆದರೆ (ಪರಿಶೀಲನೆ == STBI_ಪರಿಶೀಲನೆ_type) ಹಿಂತಿರುಗಿ 1;
   m = stbi__get_marker(z);
   ಆಗ (!stbi__SOF(m)) {
      ಆದರೆ (!stbi__process_marker(z,m)) ಹಿಂತಿರುಗಿ 0;
      m = stbi__get_marker(z);
      ಆಗ (m == STBI__MARKER_none) {
         // some files have extra padding after their blocks, so ok, we'll scan
         ಆದರೆ (stbi__at_eof(z->s)) ಹಿಂತಿರುಗಿ stbi__err("no SOF", "Corrupt JPEG");
         m = stbi__get_marker(z);
      }
   }
   z->progressive = stbi__SOF_progressive(m);
   ಆದರೆ (!stbi_process_ಚೌಕಟ್ಟು_ಶೀರ್ಷಿಕೆ(z, ಪರಿಶೀಲನೆ)) ಹಿಂತಿರುಗಿ 0;
   ಹಿಂತಿರುಗಿ 1;
}

// decode image to YCbCr format
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__decode_jpeg_image(stbi__jpeg *j)
{
   ಪೂರ್ಣಾಂಕ m;
   ಗಾಗಿ (m = 0; m < 4; m++) {
      j->img_comp[m].raw_ದತ್ತಾಂಶ = ಖಾಲಿ;
      j->img_comp[m].raw_coeff = ಖಾಲಿ;
   }
   j->restart_interval = 0;
   ಆದರೆ (!stbi_decode_jpeg_ಶೀರ್ಷಿಕೆ(j, STBI_ಪರಿಶೀಲನೆ_ತುಂಬು)) ಹಿಂತಿರುಗಿ 0;
   m = stbi__get_marker(j);
   ಆಗ (!stbi__EOI(m)) {
      ಆದರೆ (stbi__SOS(m)) {
         ಆದರೆ (!stbi_process_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ(j)) ಹಿಂತಿರುಗಿ 0;
         ಆದರೆ (!stbi_parse_entropy_coded_ದತ್ತಾಂಶ(j)) ಹಿಂತಿರುಗಿ 0;
         ಆದರೆ (j->marker == STBI__MARKER_none ) {
            // handle 0s at the end of image data from IP Kamera 9060
            ಆಗ (!stbi__at_eof(j->s)) {
               ಪೂರ್ಣಾಂಕ x = stbi__get8(j->s);
               ಆದರೆ (x == 255) {
                  j->marker = stbi__get8(j->s);
                  ನಿಲ್ಲಿಸು;
               }
            }
            // if we reach eof without hitting a marker, stbi__get_marker() below will fail and we'll eventually return 0
         }
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stbi__DNL(m)) {
         ಪೂರ್ಣಾಂಕ Ld = stbi__get16be(j->s);
         stbi__uint32 NL = stbi__get16be(j->s);
         ಆದರೆ (Ld != 4) ಹಿಂತಿರುಗಿ stbi__err("bad DNL len", "Corrupt JPEG");
         ಆದರೆ (NL != j->s->img_y) ಹಿಂತಿರುಗಿ stbi__err("bad DNL height", "Corrupt JPEG");
      } ಇಲ್ಲದಿದ್ದರೆ {
         ಆದರೆ (!stbi__process_marker(j, m)) ಹಿಂತಿರುಗಿ 0;
      }
      m = stbi__get_marker(j);
   }
   ಆದರೆ (j->progressive)
      stbi_jpeg_ಮುಕ್ತಾಯ(j);
   ಹಿಂತಿರುಗಿ 1;
}

// static jfif-centered resampling (across block boundaries)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ stbi_uc *(*resample_ಅಡ್ಡಸಾಲು_func)(stbi_uc *ಹೊರಹೋಗು, stbi_uc *in0, stbi_uc *in1,
                                    ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs);

#ವ್ಯಾಖ್ಯಾನಿಸು stbi__div4(x) ((stbi_uc) ((x) >> 2))

ಸ್ಥಿರ stbi_uc *resample_ಅಡ್ಡಸಾಲು_1(stbi_uc *ಹೊರಹೋಗು, stbi_uc *ಒಳ_near, stbi_uc *ಒಳ_far, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs)
{
   STBI_NOTUSED(ಹೊರಹೋಗು);
   STBI_NOTUSED(ಒಳ_far);
   STBI_NOTUSED(w);
   STBI_NOTUSED(hs);
   ಹಿಂತಿರುಗಿ ಒಳ_near;
}

ಸ್ಥಿರ stbi_uc* stbi_resample_ಅಡ್ಡಸಾಲು_v_2(stbi_uc *ಹೊರಹೋಗು, stbi_uc *ಒಳ_near, stbi_uc *ಒಳ_far, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs)
{
   // need to generate two samples vertically for every one in input
   ಪೂರ್ಣಾಂಕ i;
   STBI_NOTUSED(hs);
   ಗಾಗಿ (i=0; i < w; ++i)
      ಹೊರಹೋಗು[i] = stbi__div4(3*ಒಳ_near[i] + ಒಳ_far[i] + 2);
   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

ಸ್ಥಿರ stbi_uc*  stbi_resample_ಅಡ್ಡಸಾಲು_h_2(stbi_uc *ಹೊರಹೋಗು, stbi_uc *ಒಳ_near, stbi_uc *ಒಳ_far, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs)
{
   // need to generate two samples horizontally for every one in input
   ಪೂರ್ಣಾಂಕ i;
   stbi_uc *ಒಳಹರಿವು = ಒಳ_near;

   ಆದರೆ (w == 1) {
      // if only one sample, can't do any interpolation
      ಹೊರಹೋಗು[0] = ಹೊರಹೋಗು[1] = ಒಳಹರಿವು[0];
      ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
   }

   ಹೊರಹೋಗು[0] = ಒಳಹರಿವು[0];
   ಹೊರಹೋಗು[1] = stbi__div4(ಒಳಹರಿವು[0]*3 + ಒಳಹರಿವು[1] + 2);
   ಗಾಗಿ (i=1; i < w-1; ++i) {
      ಪೂರ್ಣಾಂಕ n = 3*ಒಳಹರಿವು[i]+2;
      ಹೊರಹೋಗು[i*2+0] = stbi__div4(n+ಒಳಹರಿವು[i-1]);
      ಹೊರಹೋಗು[i*2+1] = stbi__div4(n+ಒಳಹರಿವು[i+1]);
   }
   ಹೊರಹೋಗು[i*2+0] = stbi__div4(ಒಳಹರಿವು[w-2]*3 + ಒಳಹರಿವು[w-1] + 2);
   ಹೊರಹೋಗು[i*2+1] = ಒಳಹರಿವು[w-1];

   STBI_NOTUSED(ಒಳ_far);
   STBI_NOTUSED(hs);

   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

#ವ್ಯಾಖ್ಯಾನಿಸು stbi__div16(x) ((stbi_uc) ((x) >> 4))

ಸ್ಥಿರ stbi_uc *stbi_resample_ಅಡ್ಡಸಾಲು_hv_2(stbi_uc *ಹೊರಹೋಗು, stbi_uc *ಒಳ_near, stbi_uc *ಒಳ_far, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs)
{
   // need to generate 2x2 samples for every one in input
   ಪೂರ್ಣಾಂಕ i,t0,t1;
   ಆದರೆ (w == 1) {
      ಹೊರಹೋಗು[0] = ಹೊರಹೋಗು[1] = stbi__div4(3*ಒಳ_near[0] + ಒಳ_far[0] + 2);
      ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
   }

   t1 = 3*ಒಳ_near[0] + ಒಳ_far[0];
   ಹೊರಹೋಗು[0] = stbi__div4(t1+2);
   ಗಾಗಿ (i=1; i < w; ++i) {
      t0 = t1;
      t1 = 3*ಒಳ_near[i]+ಒಳ_far[i];
      ಹೊರಹೋಗು[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      ಹೊರಹೋಗು[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   ಹೊರಹೋಗು[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

#ಆದರೆ defined(STBI_SSE2) || defined(STBI_NEON)
ಸ್ಥಿರ stbi_uc *stbi_resample_ಅಡ್ಡಸಾಲು_hv_2_simd(stbi_uc *ಹೊರಹೋಗು, stbi_uc *ಒಳ_near, stbi_uc *ಒಳ_far, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs)
{
   // need to generate 2x2 samples for every one in input
   ಪೂರ್ಣಾಂಕ i=0,t0,t1;

   ಆದರೆ (w == 1) {
      ಹೊರಹೋಗು[0] = ಹೊರಹೋಗು[1] = stbi__div4(3*ಒಳ_near[0] + ಒಳ_far[0] + 2);
      ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
   }

   t1 = 3*ಒಳ_near[0] + ಒಳ_far[0];
   // process groups of 8 pixels for as long as we can.
   // note we can't handle the last pixel in a row in this loop
   // because we need to handle the filter boundary conditions.
   ಗಾಗಿ (; i < ((w-1) & ~7); i += 8) {
#ಆದರೆ defined(STBI_SSE2)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      __m128i zero  = _mm_setzero_si128();
      __m128i farb  = _mm_loadl_epi64((__m128i *) (ಒಳ_far + i));
      __m128i nearb = _mm_loadl_epi64((__m128i *) (ಒಳ_near + i));
      __m128i farw  = _mm_unpacklo_epi8(farb, zero);
      __m128i nearw = _mm_unpacklo_epi8(nearb, zero);
      __m128i diff  = _mm_sub_epi16(farw, nearw);
      __m128i nears = _mm_slli_epi16(nearw, 2);
      __m128i curr  = mm_ಕೂಡಿಸು_epi16(nears, diff); // current row

      // horizontal filter works the same based on shifted vers of current
      // row. "prev" is current row shifted right by 1 pixel; we need to
      // insert the previous pixel value (from t1).
      // "next" is current row shifted left by 1 pixel, with first pixel
      // of next block of 8 pixels added in.
      __m128i prv0 = _mm_slli_si128(curr, 2);
      __m128i nxt0 = _mm_srli_si128(curr, 2);
      __m128i prev = mm_ಸೇರಿಸು_epi16(prv0, t1, 0);
      __m128i ಮುಂದಿನ = mm_ಸೇರಿಸು_epi16(nxt0, 3*ಒಳ_near[i+8] + ಒಳ_far[i+8], 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      __m128i bias  = _mm_set1_epi16(8);
      __m128i curs = _mm_slli_epi16(curr, 2);
      __m128i prvd = _mm_sub_epi16(prev, curr);
      __m128i nxtd = _mm_sub_epi16(ಮುಂದಿನ, curr);
      __m128i curb = mm_ಕೂಡಿಸು_epi16(curs, bias);
      __m128i even = mm_ಕೂಡಿಸು_epi16(prvd, curb);
      __m128i odd  = mm_ಕೂಡಿಸು_epi16(nxtd, curb);

      // interleave even and odd pixels, then undo scaling.
      __m128i int0 = _mm_unpacklo_epi16(even, odd);
      __m128i int1 = _mm_unpackhi_epi16(even, odd);
      __m128i de0  = _mm_srli_epi16(int0, 4);
      __m128i de1  = _mm_srli_epi16(int1, 4);

      // pack and write output
      __m128i outv = _mm_packus_epi16(de0, de1);
      _mm_storeu_si128((__m128i *) (ಹೊರಹೋಗು + i*2), outv);
#ಅಥವಾಆದರೆ defined(STBI_NEON)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      uint8x8_t farb  = vld1_u8(ಒಳ_far + i);
      uint8x8_t nearb = vld1_u8(ಒಳ_near + i);
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
      int16x8_t ಮುಂದಿನ = vsetq_lane_s16(3*ಒಳ_near[i+8] + ಒಳ_far[i+8], nxt0, 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      int16x8_t curs = vshlq_n_s16(curr, 2);
      int16x8_t prvd = vsubq_s16(prev, curr);
      int16x8_t nxtd = vsubq_s16(ಮುಂದಿನ, curr);
      int16x8_t even = vaddq_s16(curs, prvd);
      int16x8_t odd  = vaddq_s16(curs, nxtd);

      // undo scaling and round, then store with even/odd phases interleaved
      uint8x8x2_t o;
      o.val[0] = vqrshrun_n_s16(even, 4);
      o.val[1] = vqrshrun_n_s16(odd,  4);
      vst2_u8(ಹೊರಹೋಗು + i*2, o);
#ಆದರೆ_ಅಂತ್ಯ

      // "previous" value for next iter
      t1 = 3*ಒಳ_near[i+7] + ಒಳ_far[i+7];
   }

   t0 = t1;
   t1 = 3*ಒಳ_near[i] + ಒಳ_far[i];
   ಹೊರಹೋಗು[i*2] = stbi__div16(3*t1 + t0 + 8);

   ಗಾಗಿ (++i; i < w; ++i) {
      t0 = t1;
      t1 = 3*ಒಳ_near[i]+ಒಳ_far[i];
      ಹೊರಹೋಗು[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      ಹೊರಹೋಗು[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   ಹೊರಹೋಗು[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ stbi_uc *stbi_resample_ಅಡ್ಡಸಾಲು_generic(stbi_uc *ಹೊರಹೋಗು, stbi_uc *ಒಳ_near, stbi_uc *ಒಳ_far, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ hs)
{
   // resample with nearest-neighbor
   ಪೂರ್ಣಾಂಕ i,j;
   STBI_NOTUSED(ಒಳ_far);
   ಗಾಗಿ (i=0; i < w; ++i)
      ಗಾಗಿ (j=0; j < hs; ++j)
         ಹೊರಹೋಗು[i*hs+j] = ಒಳ_near[i];
   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

// this is a reduced-precision calculation of YCbCr-to-RGB introduced
// to make sure the code produces the same results in both SIMD and scalar
#ವ್ಯಾಖ್ಯಾನಿಸು stbi__float2fixed(x)  (((ಪೂರ್ಣಾಂಕ) ((x) * 4096.0f + 0.5f)) << 8)
ಸ್ಥಿರ ಶೂನ್ಯ stbi_YCb_Cr_ಗಮ್ಯ_RGB_ಅಡ್ಡಸಾಲು(stbi_uc *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ stbi_uc *y, ಸ್ಥಿರಾಂಕ stbi_uc *pcb, ಸ್ಥಿರಾಂಕ stbi_uc *pcr, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ)
{
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; i < ಎಣಿಕೆ_2; ++i) {
      ಪೂರ್ಣಾಂಕ y_fixed = (y[i] << 20) + (1<<19); // rounding
      ಪೂರ್ಣಾಂಕ r,g,b;
      ಪೂರ್ಣಾಂಕ cr = pcr[i] - 128;
      ಪೂರ್ಣಾಂಕ cb = pcb[i] - 128;
      r = y_fixed +  cr* stbi__float2fixed(1.40200f);
      g = y_fixed + (cr*-stbi__float2fixed(0.71414f)) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                     +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) r > 255) { ಆದರೆ (r < 0) r = 0; ಇಲ್ಲದಿದ್ದರೆ r = 255; }
      ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) g > 255) { ಆದರೆ (g < 0) g = 0; ಇಲ್ಲದಿದ್ದರೆ g = 255; }
      ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) b > 255) { ಆದರೆ (b < 0) b = 0; ಇಲ್ಲದಿದ್ದರೆ b = 255; }
      ಹೊರಹೋಗು[0] = (stbi_uc)r;
      ಹೊರಹೋಗು[1] = (stbi_uc)g;
      ಹೊರಹೋಗು[2] = (stbi_uc)b;
      ಹೊರಹೋಗು[3] = 255;
      ಹೊರಹೋಗು += ಹೆಜ್ಜೆ;
   }
}

#ಆದರೆ defined(STBI_SSE2) || defined(STBI_NEON)
ಸ್ಥಿರ ಶೂನ್ಯ stbi_YCb_Cr_ಗಮ್ಯ_RGB_simd(stbi_uc *ಹೊರಹೋಗು, stbi_uc ಸ್ಥಿರಾಂಕ *y, stbi_uc ಸ್ಥಿರಾಂಕ *pcb, stbi_uc ಸ್ಥಿರಾಂಕ *pcr, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ)
{
   ಪೂರ್ಣಾಂಕ i = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_SSE2
   // step == 3 is pretty ugly on the final interleave, and i'm not convinced
   // it's useful in practice (you wouldn't use it for textures, for example).
   // so just accelerate step == 4 case.
   ಆದರೆ (ಹೆಜ್ಜೆ == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      __m128i signflip  = _mm_set1_epi8(-0x80);
      __m128i cr_const0 = _mm_set1_epi16(   (ಗಿಡ್ಡ) ( 1.40200f*4096.0f+0.5f));
      __m128i cr_const1 = _mm_set1_epi16( - (ಗಿಡ್ಡ) ( 0.71414f*4096.0f+0.5f));
      __m128i cb_const0 = _mm_set1_epi16( - (ಗಿಡ್ಡ) ( 0.34414f*4096.0f+0.5f));
      __m128i cb_const1 = _mm_set1_epi16(   (ಗಿಡ್ಡ) ( 1.77200f*4096.0f+0.5f));
      __m128i y_bias = _mm_set1_epi8((ಅಕ್ಷರ) (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) 128);
      __m128i xw = _mm_set1_epi16(255); // alpha channel

      ಗಾಗಿ (; i+7 < ಎಣಿಕೆ_2; i += 8) {
         //ತುಂಬು
         __m128i y_ಬೈಟುಗಳು = _mm_loadl_epi64((__m128i *) (y+i));
         __m128i cr_ಬೈಟುಗಳು = _mm_loadl_epi64((__m128i *) (pcr+i));
         __m128i cb_ಬೈಟುಗಳು = _mm_loadl_epi64((__m128i *) (pcb+i));
         __m128i cr_biased = _mm_xor_si128(cr_ಬೈಟುಗಳು, signflip); // -128
         __m128i cb_biased = _mm_xor_si128(cb_ಬೈಟುಗಳು, signflip); // -128

         // unpack to short (and left-shift cr, cb by 8)
         __m128i yw  = _mm_unpacklo_epi8(y_bias, y_ಬೈಟುಗಳು);
         __m128i crw = _mm_unpacklo_epi8(_mm_setzero_si128(), cr_biased);
         __m128i cbw = _mm_unpacklo_epi8(_mm_setzero_si128(), cb_biased);

         // color transform
         __m128i yws = _mm_srli_epi16(yw, 4);
         __m128i cr0 = _mm_mulhi_epi16(cr_const0, crw);
         __m128i cb0 = _mm_mulhi_epi16(cb_const0, cbw);
         __m128i cb1 = _mm_mulhi_epi16(cbw, cb_const1);
         __m128i cr1 = _mm_mulhi_epi16(crw, cr_const1);
         __m128i rws = mm_ಕೂಡಿಸು_epi16(cr0, yws);
         __m128i gwt = mm_ಕೂಡಿಸು_epi16(cb0, yws);
         __m128i bws = mm_ಕೂಡಿಸು_epi16(yws, cb1);
         __m128i gws = mm_ಕೂಡಿಸು_epi16(gwt, cr1);

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
         _mm_storeu_si128((__m128i *) (ಹೊರಹೋಗು + 0), o0);
         _mm_storeu_si128((__m128i *) (ಹೊರಹೋಗು + 16), o1);
         ಹೊರಹೋಗು += 32;
      }
   }
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_NEON
   // in this version, step=3 support would be easy to add. but is there demand?
   ಆದರೆ (ಹೆಜ್ಜೆ == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      uint8x8_t signflip = vdup_n_u8(0x80);
      int16x8_t cr_const0 = vdupq_n_s16(   (ಗಿಡ್ಡ) ( 1.40200f*4096.0f+0.5f));
      int16x8_t cr_const1 = vdupq_n_s16( - (ಗಿಡ್ಡ) ( 0.71414f*4096.0f+0.5f));
      int16x8_t cb_const0 = vdupq_n_s16( - (ಗಿಡ್ಡ) ( 0.34414f*4096.0f+0.5f));
      int16x8_t cb_const1 = vdupq_n_s16(   (ಗಿಡ್ಡ) ( 1.77200f*4096.0f+0.5f));

      ಗಾಗಿ (; i+7 < ಎಣಿಕೆ_2; i += 8) {
         //ತುಂಬು
         uint8x8_t y_ಬೈಟುಗಳು  = vld1_u8(y + i);
         uint8x8_t cr_ಬೈಟುಗಳು = vld1_u8(pcr + i);
         uint8x8_t cb_ಬೈಟುಗಳು = vld1_u8(pcb + i);
         int8x8_t cr_biased = vreinterpret_s8_u8(vsub_u8(cr_ಬೈಟುಗಳು, signflip));
         int8x8_t cb_biased = vreinterpret_s8_u8(vsub_u8(cb_ಬೈಟುಗಳು, signflip));

         // expand to s16
         int16x8_t yws = vreinterpretq_s16_u16(vshll_n_u8(y_ಬೈಟುಗಳು, 4));
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
         vst4_u8(ಹೊರಹೋಗು, o);
         ಹೊರಹೋಗು += 8*4;
      }
   }
#ಆದರೆ_ಅಂತ್ಯ

   ಗಾಗಿ (; i < ಎಣಿಕೆ_2; ++i) {
      ಪೂರ್ಣಾಂಕ y_fixed = (y[i] << 20) + (1<<19); // rounding
      ಪೂರ್ಣಾಂಕ r,g,b;
      ಪೂರ್ಣಾಂಕ cr = pcr[i] - 128;
      ಪೂರ್ಣಾಂಕ cb = pcb[i] - 128;
      r = y_fixed + cr* stbi__float2fixed(1.40200f);
      g = y_fixed + cr*-stbi__float2fixed(0.71414f) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                   +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) r > 255) { ಆದರೆ (r < 0) r = 0; ಇಲ್ಲದಿದ್ದರೆ r = 255; }
      ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) g > 255) { ಆದರೆ (g < 0) g = 0; ಇಲ್ಲದಿದ್ದರೆ g = 255; }
      ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) b > 255) { ಆದರೆ (b < 0) b = 0; ಇಲ್ಲದಿದ್ದರೆ b = 255; }
      ಹೊರಹೋಗು[0] = (stbi_uc)r;
      ಹೊರಹೋಗು[1] = (stbi_uc)g;
      ಹೊರಹೋಗು[2] = (stbi_uc)b;
      ಹೊರಹೋಗು[3] = 255;
      ಹೊರಹೋಗು += ಹೆಜ್ಜೆ;
   }
}
#ಆದರೆ_ಅಂತ್ಯ

// set up the kernels
ಸ್ಥಿರ ಶೂನ್ಯ stbi_ಸಿದ್ಧತೆ_jpeg(stbi__jpeg *j)
{
   j->idct_ಖಂಡ_kernel = stbi_idct_ಖಂಡ;
   j->YCb_Cr_ಗಮ್ಯ_RGB_kernel = stbi_YCb_Cr_ಗಮ್ಯ_RGB_ಅಡ್ಡಸಾಲು;
   j->resample_ಅಡ್ಡಸಾಲು_hv_2_kernel = stbi_resample_ಅಡ್ಡಸಾಲು_hv_2;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_SSE2
   ಆದರೆ (stbi_sse2_ಲಭ್ಯ()) {
      j->idct_ಖಂಡ_kernel = stbi__idct_simd;
      j->YCb_Cr_ಗಮ್ಯ_RGB_kernel = stbi_YCb_Cr_ಗಮ್ಯ_RGB_simd;
      j->resample_ಅಡ್ಡಸಾಲು_hv_2_kernel = stbi_resample_ಅಡ್ಡಸಾಲು_hv_2_simd;
   }
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STBI_NEON
   j->idct_ಖಂಡ_kernel = stbi__idct_simd;
   j->YCb_Cr_ಗಮ್ಯ_RGB_kernel = stbi_YCb_Cr_ಗಮ್ಯ_RGB_simd;
   j->resample_ಅಡ್ಡಸಾಲು_hv_2_kernel = stbi_resample_ಅಡ್ಡಸಾಲು_hv_2_simd;
#ಆದರೆ_ಅಂತ್ಯ
}

// clean up the temporary component buffers
ಸ್ಥಿರ ಶೂನ್ಯ stbi__cleanup_jpeg(stbi__jpeg *j)
{
   stbi__free_jpeg_components(j, j->s->img_n, 0);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   resample_ಅಡ್ಡಸಾಲು_func resample;
   stbi_uc *line0,*line1;
   ಪೂರ್ಣಾಂಕ hs,vs;   // expansion factor in each axis
   ಪೂರ್ಣಾಂಕ w_lores; // horizontal pixels pre-expansion
   ಪೂರ್ಣಾಂಕ ystep;   // how far through vertical expansion we are
   ಪೂರ್ಣಾಂಕ ypos;    // which pre-expansion row we're on
} stbi__resample;

// fast 0..255 * 0..255 => 0..255 rounded multiplication
ಸ್ಥಿರ stbi_uc stbi__blinn_8x8(stbi_uc x, stbi_uc y)
{
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ t = x*y + 128;
   ಹಿಂತಿರುಗಿ (stbi_uc) ((t + (t >>8)) >> 8);
}

ಸ್ಥಿರ stbi_uc *ತುಂಬು_jpeg_image(stbi__jpeg *z, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_x, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   ಪೂರ್ಣಾಂಕ n, decode_n, is_rgb;
   z->s->img_n = 0; // make stbi__cleanup_jpeg safe

   // validate req_comp
   ಆದರೆ (req_comp < 0 || req_comp > 4) ಹಿಂತಿರುಗಿ stbi__errpuc("bad req_comp", "Internal error");

   // load a jpeg image from whichever source, but leave in YCbCr format
   ಆದರೆ (!stbi__decode_jpeg_image(z)) { stbi__cleanup_jpeg(z); ಹಿಂತಿರುಗಿ ಖಾಲಿ; }

   // determine actual number of components to generate
   n = req_comp ? req_comp : z->s->img_n >= 3 ? 3 : 1;

   is_rgb = z->s->img_n == 3 && (z->rgb == 3 || (z->app14_color_transform == 0 && !z->jfif));

   ಆದರೆ (z->s->img_n == 3 && n < 3 && !is_rgb)
      decode_n = 1;
   ಇಲ್ಲದಿದ್ದರೆ
      decode_n = z->s->img_n;

   // resample and color-convert
   {
      ಪೂರ್ಣಾಂಕ k;
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i,j;
      stbi_uc *output;
      stbi_uc *coutput[4];

      stbi__resample res_comp[4];

      ಗಾಗಿ (k=0; k < decode_n; ++k) {
         stbi__resample *r = &res_comp[k];

         // allocate line buffer big enough for upsampling off the edges
         // with upsample factor of 4
         z->img_comp[k].linebuf = (stbi_uc *) stbi__malloc(z->s->img_x + 3);
         ಆದರೆ (!z->img_comp[k].linebuf) { stbi__cleanup_jpeg(z); ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory"); }

         r->hs      = z->img_h_max / z->img_comp[k].h;
         r->vs      = z->img_v_max / z->img_comp[k].v;
         r->ystep   = r->vs >> 1;
         r->w_lores = (z->s->img_x + r->hs-1) / r->hs;
         r->ypos    = 0;
         r->line0   = r->line1 = z->img_comp[k].ದತ್ತಾಂಶ;

         ಆದರೆ      (r->hs == 1 && r->vs == 1) r->resample = resample_ಅಡ್ಡಸಾಲು_1;
         ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (r->hs == 1 && r->vs == 2) r->resample = stbi_resample_ಅಡ್ಡಸಾಲು_v_2;
         ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (r->hs == 2 && r->vs == 1) r->resample = stbi_resample_ಅಡ್ಡಸಾಲು_h_2;
         ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (r->hs == 2 && r->vs == 2) r->resample = z->resample_ಅಡ್ಡಸಾಲು_hv_2_kernel;
         ಇಲ್ಲದಿದ್ದರೆ                               r->resample = stbi_resample_ಅಡ್ಡಸಾಲು_generic;
      }

      // can't error after this so, this is safe
      output = (stbi_uc *) stbi__malloc_mad3(n, z->s->img_x, z->s->img_y, 1);
      ಆದರೆ (!output) { stbi__cleanup_jpeg(z); ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory"); }

      // now go ahead and resample
      ಗಾಗಿ (j=0; j < z->s->img_y; ++j) {
         stbi_uc *ಹೊರಹೋಗು = output + n * z->s->img_x * j;
         ಗಾಗಿ (k=0; k < decode_n; ++k) {
            stbi__resample *r = &res_comp[k];
            ಪೂರ್ಣಾಂಕ y_bot = r->ystep >= (r->vs >> 1);
            coutput[k] = r->resample(z->img_comp[k].linebuf,
                                     y_bot ? r->line1 : r->line0,
                                     y_bot ? r->line0 : r->line1,
                                     r->w_lores, r->hs);
            ಆದರೆ (++r->ystep >= r->vs) {
               r->ystep = 0;
               r->line0 = r->line1;
               ಆದರೆ (++r->ypos < z->img_comp[k].y)
                  r->line1 += z->img_comp[k].w2;
            }
         }
         ಆದರೆ (n >= 3) {
            stbi_uc *y = coutput[0];
            ಆದರೆ (z->s->img_n == 3) {
               ಆದರೆ (is_rgb) {
                  ಗಾಗಿ (i=0; i < z->s->img_x; ++i) {
                     ಹೊರಹೋಗು[0] = y[i];
                     ಹೊರಹೋಗು[1] = coutput[1][i];
                     ಹೊರಹೋಗು[2] = coutput[2][i];
                     ಹೊರಹೋಗು[3] = 255;
                     ಹೊರಹೋಗು += n;
                  }
               } ಇಲ್ಲದಿದ್ದರೆ {
                  z->YCb_Cr_ಗಮ್ಯ_RGB_kernel(ಹೊರಹೋಗು, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (z->s->img_n == 4) {
               ಆದರೆ (z->app14_color_transform == 0) { // CMYK
                  ಗಾಗಿ (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     ಹೊರಹೋಗು[0] = stbi__blinn_8x8(coutput[0][i], m);
                     ಹೊರಹೋಗು[1] = stbi__blinn_8x8(coutput[1][i], m);
                     ಹೊರಹೋಗು[2] = stbi__blinn_8x8(coutput[2][i], m);
                     ಹೊರಹೋಗು[3] = 255;
                     ಹೊರಹೋಗು += n;
                  }
               } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (z->app14_color_transform == 2) { // YCCK
                  z->YCb_Cr_ಗಮ್ಯ_RGB_kernel(ಹೊರಹೋಗು, y, coutput[1], coutput[2], z->s->img_x, n);
                  ಗಾಗಿ (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     ಹೊರಹೋಗು[0] = stbi__blinn_8x8(255 - ಹೊರಹೋಗು[0], m);
                     ಹೊರಹೋಗು[1] = stbi__blinn_8x8(255 - ಹೊರಹೋಗು[1], m);
                     ಹೊರಹೋಗು[2] = stbi__blinn_8x8(255 - ಹೊರಹೋಗು[2], m);
                     ಹೊರಹೋಗು += n;
                  }
               } ಇಲ್ಲದಿದ್ದರೆ { // YCbCr + alpha?  Ignore the fourth channel for now
                  z->YCb_Cr_ಗಮ್ಯ_RGB_kernel(ಹೊರಹೋಗು, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } ಇಲ್ಲದಿದ್ದರೆ
               ಗಾಗಿ (i=0; i < z->s->img_x; ++i) {
                  ಹೊರಹೋಗು[0] = ಹೊರಹೋಗು[1] = ಹೊರಹೋಗು[2] = y[i];
                  ಹೊರಹೋಗು[3] = 255; // not used if n==3
                  ಹೊರಹೋಗು += n;
               }
         } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (is_rgb) {
               ಆದರೆ (n == 1)
                  ಗಾಗಿ (i=0; i < z->s->img_x; ++i)
                     *ಹೊರಹೋಗು++ = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
               ಇಲ್ಲದಿದ್ದರೆ {
                  ಗಾಗಿ (i=0; i < z->s->img_x; ++i, ಹೊರಹೋಗು += 2) {
                     ಹೊರಹೋಗು[0] = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
                     ಹೊರಹೋಗು[1] = 255;
                  }
               }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (z->s->img_n == 4 && z->app14_color_transform == 0) {
               ಗಾಗಿ (i=0; i < z->s->img_x; ++i) {
                  stbi_uc m = coutput[3][i];
                  stbi_uc r = stbi__blinn_8x8(coutput[0][i], m);
                  stbi_uc g = stbi__blinn_8x8(coutput[1][i], m);
                  stbi_uc b = stbi__blinn_8x8(coutput[2][i], m);
                  ಹೊರಹೋಗು[0] = stbi__compute_y(r, g, b);
                  ಹೊರಹೋಗು[1] = 255;
                  ಹೊರಹೋಗು += n;
               }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (z->s->img_n == 4 && z->app14_color_transform == 2) {
               ಗಾಗಿ (i=0; i < z->s->img_x; ++i) {
                  ಹೊರಹೋಗು[0] = stbi__blinn_8x8(255 - coutput[0][i], coutput[3][i]);
                  ಹೊರಹೋಗು[1] = 255;
                  ಹೊರಹೋಗು += n;
               }
            } ಇಲ್ಲದಿದ್ದರೆ {
               stbi_uc *y = coutput[0];
               ಆದರೆ (n == 1)
                  ಗಾಗಿ (i=0; i < z->s->img_x; ++i) ಹೊರಹೋಗು[i] = y[i];
               ಇಲ್ಲದಿದ್ದರೆ
                  ಗಾಗಿ (i=0; i < z->s->img_x; ++i) *ಹೊರಹೋಗು++ = y[i], *ಹೊರಹೋಗು++ = 255;
            }
         }
      }
      stbi__cleanup_jpeg(z);
      *ಹೊರಹೋಗು_x = z->s->img_x;
      *ಹೊರಹೋಗು_y = z->s->img_y;
      ಆದರೆ (comp) *comp = z->s->img_n >= 3 ? 3 : 1; // report original components, not output
      ಹಿಂತಿರುಗಿ output;
   }
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_jpeg_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಫಲಿತಾಂಶ;
   stbi__jpeg* j = (stbi__jpeg*) stbi__malloc(ಗಾತ್ರ(stbi__jpeg));
   STBI_NOTUSED(ri);
   j->s = s;
   stbi_ಸಿದ್ಧತೆ_jpeg(j);
   ಫಲಿತಾಂಶ = ತುಂಬು_jpeg_image(j, x,y,comp,req_comp);
   STBI_FREE(j);
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__jpeg_test(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ r;
   stbi__jpeg* j = (stbi__jpeg*)stbi__malloc(ಗಾತ್ರ(stbi__jpeg));
   j->s = s;
   stbi_ಸಿದ್ಧತೆ_jpeg(j);
   r = stbi_decode_jpeg_ಶೀರ್ಷಿಕೆ(j, STBI_ಪರಿಶೀಲನೆ_type);
   stbi__rewind(s);
   STBI_FREE(j);
   ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__jpeg_info_raw(stbi__jpeg *j, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಆದರೆ (!stbi_decode_jpeg_ಶೀರ್ಷಿಕೆ(j, STBI_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ)) {
      stbi__rewind( j->s );
      ಹಿಂತಿರುಗಿ 0;
   }
   ಆದರೆ (x) *x = j->s->img_x;
   ಆದರೆ (y) *y = j->s->img_y;
   ಆದರೆ (comp) *comp = j->s->img_n >= 3 ? 3 : 1;
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__jpeg_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
   stbi__jpeg* j = (stbi__jpeg*) (stbi__malloc(ಗಾತ್ರ(stbi__jpeg)));
   j->s = s;
   ಫಲಿತಾಂಶ = stbi__jpeg_info_raw(j, x, y, comp);
   STBI_FREE(j);
   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
#ಆದರೆ_ಅಂತ್ಯ

// public domain zlib decode    v0.2  Sean Barrett 2006-11-18
//    simple implementation
//      - all input must be provided in an upfront buffer
//      - all output is written to a single output buffer (can malloc/realloc)
//    performance
//      - fast huffman

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_ZLIB

// fast-way is faster to check than jpeg huffman, but slow way is slower
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_ZFAST_ಬಿಟ್ಟುಗಳು  9 // accelerate all cases in default tables
#ವ್ಯಾಖ್ಯಾನಿಸು STBI_ZFAST_ಮುಖವಾಡ  ((1 << STBI_ZFAST_ಬಿಟ್ಟುಗಳು) - 1)

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi__uint16 fast[1 << STBI_ZFAST_ಬಿಟ್ಟುಗಳು];
   stbi__uint16 firstcode[16];
   ಪೂರ್ಣಾಂಕ maxcode[17];
   stbi__uint16 firstsymbol[16];
   stbi_uc  ಗಾತ್ರ_2[288];
   stbi__uint16 ಮೌಲ್ಯ[288];
} stbi__zhuffman;

stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__bitreverse16(ಪೂರ್ಣಾಂಕ n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  ಹಿಂತಿರುಗಿ n;
}

stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_ಬಿಟ್_ಹಿಮ್ಮುಖ(ಪೂರ್ಣಾಂಕ v, ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು)
{
   STBI_ASSERT(ಬಿಟ್ಟುಗಳು <= 16);
   // to bit reverse n bits, reverse 16 and shift
   // e.g. 11 bits, bit reverse and shift away 5
   ಹಿಂತಿರುಗಿ stbi__bitreverse16(v) >> (16-ಬಿಟ್ಟುಗಳು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__zbuild_huffman(stbi__zhuffman *z, ಸ್ಥಿರಾಂಕ stbi_uc *sizelist, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
   ಪೂರ್ಣಾಂಕ i,k=0;
   ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2, ಮುಂದಿನ_ಸಂಕೇತ[16], sizes[17];

   // DEFLATE spec for generating codes
   memset(sizes, 0, ಗಾತ್ರ(sizes));
   memset(z->fast, 0, ಗಾತ್ರ(z->fast));
   ಗಾಗಿ (i=0; i < ಅಂಕೆ; ++i)
      ++sizes[sizelist[i]];
   sizes[0] = 0;
   ಗಾಗಿ (i=1; i < 16; ++i)
      ಆದರೆ (sizes[i] > (1 << i))
         ಹಿಂತಿರುಗಿ stbi__err("bad sizes", "Corrupt PNG");
   ಸಂಕೇತ_2 = 0;
   ಗಾಗಿ (i=1; i < 16; ++i) {
      ಮುಂದಿನ_ಸಂಕೇತ[i] = ಸಂಕೇತ_2;
      z->firstcode[i] = (stbi__uint16) ಸಂಕೇತ_2;
      z->firstsymbol[i] = (stbi__uint16) k;
      ಸಂಕೇತ_2 = (ಸಂಕೇತ_2 + sizes[i]);
      ಆದರೆ (sizes[i])
         ಆದರೆ (ಸಂಕೇತ_2-1 >= (1 << i)) ಹಿಂತಿರುಗಿ stbi__err("bad codelengths","Corrupt PNG");
      z->maxcode[i] = ಸಂಕೇತ_2 << (16-i); // preshift for inner loop
      ಸಂಕೇತ_2 <<= 1;
      k += sizes[i];
   }
   z->maxcode[16] = 0x10000; // sentinel
   ಗಾಗಿ (i=0; i < ಅಂಕೆ; ++i) {
      ಪೂರ್ಣಾಂಕ s = sizelist[i];
      ಆದರೆ (s) {
         ಪೂರ್ಣಾಂಕ c = ಮುಂದಿನ_ಸಂಕೇತ[s] - z->firstcode[s] + z->firstsymbol[s];
         stbi__uint16 fastv = (stbi__uint16) ((s << 9) | i);
         z->ಗಾತ್ರ_2 [c] = (stbi_uc     ) s;
         z->ಮೌಲ್ಯ[c] = (stbi__uint16) i;
         ಆದರೆ (s <= STBI_ZFAST_ಬಿಟ್ಟುಗಳು) {
            ಪೂರ್ಣಾಂಕ j = stbi_ಬಿಟ್_ಹಿಮ್ಮುಖ(ಮುಂದಿನ_ಸಂಕೇತ[s],s);
            ಆಗ (j < (1 << STBI_ZFAST_ಬಿಟ್ಟುಗಳು)) {
               z->fast[j] = fastv;
               j += (1 << s);
            }
         }
         ++ಮುಂದಿನ_ಸಂಕೇತ[s];
      }
   }
   ಹಿಂತಿರುಗಿ 1;
}

// zlib-from-memory implementation for PNG reading
//    because PNG allows splitting the zlib stream arbitrarily,
//    and it's annoying structurally to have PNG call ZLIB call PNG,
//    we require PNG read all the IDATs and combine them into a single
//    memory buffer

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi_uc *zbuffer, *zbuffer_ಕೊನೆ;
   ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಬಿಟ್ಟುಗಳು;
   stbi__uint32 ಸಂಕೇತ_ಬಫರ್;

   ಅಕ್ಷರ *zout;
   ಅಕ್ಷರ *zout_ಆರಂಭ;
   ಅಕ್ಷರ *zout_ಕೊನೆ;
   ಪೂರ್ಣಾಂಕ   z_expandable;

   stbi__zhuffman z_ಉದ್ದಳತೆ, z_ದೂರ;
} stbi__zbuf;

stbi_inline ಸ್ಥಿರ stbi_uc stbi__zget8(stbi__zbuf *z)
{
   ಆದರೆ (z->zbuffer >= z->zbuffer_ಕೊನೆ) ಹಿಂತಿರುಗಿ 0;
   ಹಿಂತಿರುಗಿ *z->zbuffer++;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi_fill_ಬಿಟ್ಟುಗಳು(stbi__zbuf *z)
{
   ಮಾಡು {
      STBI_ASSERT(z->ಸಂಕೇತ_ಬಫರ್ < (1U << z->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು));
      z->ಸಂಕೇತ_ಬಫರ್ |= (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) stbi__zget8(z) << z->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು;
      z->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು += 8;
   } ಆಗ (z->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು <= 24);
}

stbi_inline ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ stbi__zreceive(stbi__zbuf *z, ಪೂರ್ಣಾಂಕ n)
{
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ k;
   ಆದರೆ (z->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು < n) stbi_fill_ಬಿಟ್ಟುಗಳು(z);
   k = z->ಸಂಕೇತ_ಬಫರ್ & ((1 << n) - 1);
   z->ಸಂಕೇತ_ಬಫರ್ >>= n;
   z->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು -= n;
   ಹಿಂತಿರುಗಿ k;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__zhuffman_decode_slowpath(stbi__zbuf *a, stbi__zhuffman *z)
{
   ಪೂರ್ಣಾಂಕ b,s,k;
   // not resolved by fast table, so compute it the slow way
   // use jpeg approach, which requires MSbits at top
   k = stbi_ಬಿಟ್_ಹಿಮ್ಮುಖ(a->ಸಂಕೇತ_ಬಫರ್, 16);
   ಗಾಗಿ (s=STBI_ZFAST_ಬಿಟ್ಟುಗಳು+1; ; ++s)
      ಆದರೆ (k < z->maxcode[s])
         ನಿಲ್ಲಿಸು;
   ಆದರೆ (s == 16) ಹಿಂತಿರುಗಿ -1; // invalid code!
   // code size is s, so:
   b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
   STBI_ASSERT(z->ಗಾತ್ರ_2[b] == s);
   a->ಸಂಕೇತ_ಬಫರ್ >>= s;
   a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು -= s;
   ಹಿಂತಿರುಗಿ z->ಮೌಲ್ಯ[b];
}

stbi_inline ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__zhuffman_decode(stbi__zbuf *a, stbi__zhuffman *z)
{
   ಪೂರ್ಣಾಂಕ b,s;
   ಆದರೆ (a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು < 16) stbi_fill_ಬಿಟ್ಟುಗಳು(a);
   b = z->fast[a->ಸಂಕೇತ_ಬಫರ್ & STBI_ZFAST_ಮುಖವಾಡ];
   ಆದರೆ (b) {
      s = b >> 9;
      a->ಸಂಕೇತ_ಬಫರ್ >>= s;
      a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು -= s;
      ಹಿಂತಿರುಗಿ b & 511;
   }
   ಹಿಂತಿರುಗಿ stbi__zhuffman_decode_slowpath(a, z);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__zexpand(stbi__zbuf *z, ಅಕ್ಷರ *zout, ಪೂರ್ಣಾಂಕ n)  // need to make room for n bytes
{
   ಅಕ್ಷರ *q;
   ಪೂರ್ಣಾಂಕ cur, ಎಲ್ಲೆ, old_ಎಲ್ಲೆ;
   z->zout = zout;
   ಆದರೆ (!z->z_expandable) ಹಿಂತಿರುಗಿ stbi__err("output buffer limit","Corrupt PNG");
   cur   = (ಪೂರ್ಣಾಂಕ) (z->zout     - z->zout_ಆರಂಭ);
   ಎಲ್ಲೆ = old_ಎಲ್ಲೆ = (ಪೂರ್ಣಾಂಕ) (z->zout_ಕೊನೆ - z->zout_ಆರಂಭ);
   ಆಗ (cur + n > ಎಲ್ಲೆ)
      ಎಲ್ಲೆ *= 2;
   q = (ಅಕ್ಷರ *) STBI_REALLOC_SIZED(z->zout_ಆರಂಭ, old_ಎಲ್ಲೆ, ಎಲ್ಲೆ);
   STBI_NOTUSED(old_ಎಲ್ಲೆ);
   ಆದರೆ (q == ಖಾಲಿ) ಹಿಂತಿರುಗಿ stbi__err("outofmem", "Out of memory");
   z->zout_ಆರಂಭ = q;
   z->zout       = q + cur;
   z->zout_ಕೊನೆ   = q + ಎಲ್ಲೆ;
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stbi_zlength_ತಳ[31] = {
   3,4,5,6,7,8,9,10,11,13,
   15,17,19,23,27,31,35,43,51,59,
   67,83,99,115,131,163,195,227,258,0,0 };

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stbi__zlength_extra[31]=
{ 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stbi_zdist_ತಳ[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stbi__zdist_extra[32] =
{ 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_parse_huffman_ಖಂಡ(stbi__zbuf *a)
{
   ಅಕ್ಷರ *zout = a->zout;
   ಗಾಗಿ(;;) {
      ಪೂರ್ಣಾಂಕ z = stbi__zhuffman_decode(a, &a->z_ಉದ್ದಳತೆ);
      ಆದರೆ (z < 256) {
         ಆದರೆ (z < 0) ಹಿಂತಿರುಗಿ stbi__err("bad huffman code","Corrupt PNG"); // error in huffman codes
         ಆದರೆ (zout >= a->zout_ಕೊನೆ) {
            ಆದರೆ (!stbi__zexpand(a, zout, 1)) ಹಿಂತಿರುಗಿ 0;
            zout = a->zout;
         }
         *zout++ = (ಅಕ್ಷರ) z;
      } ಇಲ್ಲದಿದ್ದರೆ {
         stbi_uc *p;
         ಪೂರ್ಣಾಂಕ len,dist;
         ಆದರೆ (z == 256) {
            a->zout = zout;
            ಹಿಂತಿರುಗಿ 1;
         }
         z -= 257;
         len = stbi_zlength_ತಳ[z];
         ಆದರೆ (stbi__zlength_extra[z]) len += stbi__zreceive(a, stbi__zlength_extra[z]);
         z = stbi__zhuffman_decode(a, &a->z_ದೂರ);
         ಆದರೆ (z < 0) ಹಿಂತಿರುಗಿ stbi__err("bad huffman code","Corrupt PNG");
         dist = stbi_zdist_ತಳ[z];
         ಆದರೆ (stbi__zdist_extra[z]) dist += stbi__zreceive(a, stbi__zdist_extra[z]);
         ಆದರೆ (zout - a->zout_ಆರಂಭ < dist) ಹಿಂತಿರುಗಿ stbi__err("bad dist","Corrupt PNG");
         ಆದರೆ (zout + len > a->zout_ಕೊನೆ) {
            ಆದರೆ (!stbi__zexpand(a, zout, len)) ಹಿಂತಿರುಗಿ 0;
            zout = a->zout;
         }
         p = (stbi_uc *) (zout - dist);
         ಆದರೆ (dist == 1) { // run of one byte; common in images.
            stbi_uc v = *p;
            ಆದರೆ (len) { ಮಾಡು *zout++ = v; ಆಗ (--len); }
         } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (len) { ಮಾಡು *zout++ = *p++; ಆಗ (--len); }
         }
      }
   }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__compute_huffman_codes(stbi__zbuf *a)
{
   ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ stbi_uc ಉದ್ದಳತೆ_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
   stbi__zhuffman z_codelength;
   stbi_uc lencodes[286+32+137];//padding for maximum single op
   stbi_uc codelength_sizes[19];
   ಪೂರ್ಣಾಂಕ i,n;

   ಪೂರ್ಣಾಂಕ hlit  = stbi__zreceive(a,5) + 257;
   ಪೂರ್ಣಾಂಕ hdist = stbi__zreceive(a,5) + 1;
   ಪೂರ್ಣಾಂಕ hclen = stbi__zreceive(a,4) + 4;
   ಪೂರ್ಣಾಂಕ ntot  = hlit + hdist;

   memset(codelength_sizes, 0, ಗಾತ್ರ(codelength_sizes));
   ಗಾಗಿ (i=0; i < hclen; ++i) {
      ಪೂರ್ಣಾಂಕ s = stbi__zreceive(a,3);
      codelength_sizes[ಉದ್ದಳತೆ_dezigzag[i]] = (stbi_uc) s;
   }
   ಆದರೆ (!stbi__zbuild_huffman(&z_codelength, codelength_sizes, 19)) ಹಿಂತಿರುಗಿ 0;

   n = 0;
   ಆಗ (n < ntot) {
      ಪೂರ್ಣಾಂಕ c = stbi__zhuffman_decode(a, &z_codelength);
      ಆದರೆ (c < 0 || c >= 19) ಹಿಂತಿರುಗಿ stbi__err("bad codelengths", "Corrupt PNG");
      ಆದರೆ (c < 16)
         lencodes[n++] = (stbi_uc) c;
      ಇಲ್ಲದಿದ್ದರೆ {
         stbi_uc fill = 0;
         ಆದರೆ (c == 16) {
            c = stbi__zreceive(a,2)+3;
            ಆದರೆ (n == 0) ಹಿಂತಿರುಗಿ stbi__err("bad codelengths", "Corrupt PNG");
            fill = lencodes[n-1];
         } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == 17)
            c = stbi__zreceive(a,3)+3;
         ಇಲ್ಲದಿದ್ದರೆ {
            STBI_ASSERT(c == 18);
            c = stbi__zreceive(a,7)+11;
         }
         ಆದರೆ (ntot - n < c) ಹಿಂತಿರುಗಿ stbi__err("bad codelengths", "Corrupt PNG");
         memset(lencodes+n, fill, c);
         n += c;
      }
   }
   ಆದರೆ (n != ntot) ಹಿಂತಿರುಗಿ stbi__err("bad codelengths","Corrupt PNG");
   ಆದರೆ (!stbi__zbuild_huffman(&a->z_ಉದ್ದಳತೆ, lencodes, hlit)) ಹಿಂತಿರುಗಿ 0;
   ಆದರೆ (!stbi__zbuild_huffman(&a->z_ದೂರ, lencodes+hlit, hdist)) ಹಿಂತಿರುಗಿ 0;
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_parse_uncompressed_ಖಂಡ(stbi__zbuf *a)
{
   stbi_uc ಶೀರ್ಷಿಕೆ[4];
   ಪೂರ್ಣಾಂಕ len,nlen,k;
   ಆದರೆ (a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು & 7)
      stbi__zreceive(a, a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು & 7); // discard
   // drain the bit-packed data into header
   k = 0;
   ಆಗ (a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು > 0) {
      ಶೀರ್ಷಿಕೆ[k++] = (stbi_uc) (a->ಸಂಕೇತ_ಬಫರ್ & 255); // suppress MSVC run-time check
      a->ಸಂಕೇತ_ಬಫರ್ >>= 8;
      a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು -= 8;
   }
   STBI_ASSERT(a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು == 0);
   // now fill header the normal way
   ಆಗ (k < 4)
      ಶೀರ್ಷಿಕೆ[k++] = stbi__zget8(a);
   len  = ಶೀರ್ಷಿಕೆ[1] * 256 + ಶೀರ್ಷಿಕೆ[0];
   nlen = ಶೀರ್ಷಿಕೆ[3] * 256 + ಶೀರ್ಷಿಕೆ[2];
   ಆದರೆ (nlen != (len ^ 0xffff)) ಹಿಂತಿರುಗಿ stbi__err("zlib corrupt","Corrupt PNG");
   ಆದರೆ (a->zbuffer + len > a->zbuffer_ಕೊನೆ) ಹಿಂತಿರುಗಿ stbi__err("read past buffer","Corrupt PNG");
   ಆದರೆ (a->zout + len > a->zout_ಕೊನೆ)
      ಆದರೆ (!stbi__zexpand(a, a->zout, len)) ಹಿಂತಿರುಗಿ 0;
   ಸ್ಮರಣೆ_ನಕಲು(a->zout, a->zbuffer, len);
   a->zbuffer += len;
   a->zout += len;
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_parse_zlib_ಶೀರ್ಷಿಕೆ(stbi__zbuf *a)
{
   ಪೂರ್ಣಾಂಕ cmf   = stbi__zget8(a);
   ಪೂರ್ಣಾಂಕ cm    = cmf & 15;
   /* int cinfo = cmf >> 4; */
   ಪೂರ್ಣಾಂಕ flg   = stbi__zget8(a);
   ಆದರೆ ((cmf*256+flg) % 31 != 0) ಹಿಂತಿರುಗಿ stbi__err("bad zlib header","Corrupt PNG"); // zlib spec
   ಆದರೆ (flg & 32) ಹಿಂತಿರುಗಿ stbi__err("no preset dict","Corrupt PNG"); // preset dictionary not allowed in png
   ಆದರೆ (cm != 8) ಹಿಂತಿರುಗಿ stbi__err("bad compression","Corrupt PNG"); // DEFLATE required for png
   // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ stbi_uc stbi_zdefault_ಉದ್ದಳತೆ[288] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ stbi_uc stbi_zdefault_ದೂರ[32] =
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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__parse_zlib(stbi__zbuf *a, ಪೂರ್ಣಾಂಕ parse_ಶೀರ್ಷಿಕೆ)
{
   ಪೂರ್ಣಾಂಕ final, type;
   ಆದರೆ (parse_ಶೀರ್ಷಿಕೆ)
      ಆದರೆ (!stbi_parse_zlib_ಶೀರ್ಷಿಕೆ(a)) ಹಿಂತಿರುಗಿ 0;
   a->ಅಂಕೆ_ಬಿಟ್ಟುಗಳು = 0;
   a->ಸಂಕೇತ_ಬಫರ್ = 0;
   ಮಾಡು {
      final = stbi__zreceive(a,1);
      type = stbi__zreceive(a,2);
      ಆದರೆ (type == 0) {
         ಆದರೆ (!stbi_parse_uncompressed_ಖಂಡ(a)) ಹಿಂತಿರುಗಿ 0;
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == 3) {
         ಹಿಂತಿರುಗಿ 0;
      } ಇಲ್ಲದಿದ್ದರೆ {
         ಆದರೆ (type == 1) {
            // use fixed code lengths
            ಆದರೆ (!stbi__zbuild_huffman(&a->z_ಉದ್ದಳತೆ  , stbi_zdefault_ಉದ್ದಳತೆ  , 288)) ಹಿಂತಿರುಗಿ 0;
            ಆದರೆ (!stbi__zbuild_huffman(&a->z_ದೂರ, stbi_zdefault_ದೂರ,  32)) ಹಿಂತಿರುಗಿ 0;
         } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (!stbi__compute_huffman_codes(a)) ಹಿಂತಿರುಗಿ 0;
         }
         ಆದರೆ (!stbi_parse_huffman_ಖಂಡ(a)) ಹಿಂತಿರುಗಿ 0;
      }
   } ಆಗ (!final);
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__do_zlib(stbi__zbuf *a, ಅಕ್ಷರ *obuf, ಪೂರ್ಣಾಂಕ olen, ಪೂರ್ಣಾಂಕ ಘಾತಾಂಕ, ಪೂರ್ಣಾಂಕ parse_ಶೀರ್ಷಿಕೆ)
{
   a->zout_ಆರಂಭ = obuf;
   a->zout       = obuf;
   a->zout_ಕೊನೆ   = obuf + olen;
   a->z_expandable = ಘಾತಾಂಕ;

   ಹಿಂತಿರುಗಿ stbi__parse_zlib(a, parse_ಶೀರ್ಷಿಕೆ);
}

STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_malloc_guesssize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ *outlen)
{
   stbi__zbuf a;
   ಅಕ್ಷರ *p = (ಅಕ್ಷರ *) stbi__malloc(initial_ಗಾತ್ರ);
   ಆದರೆ (p == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   a.zbuffer = (stbi_uc *) ಬಫರ್;
   a.zbuffer_ಕೊನೆ = (stbi_uc *) ಬಫರ್ + len;
   ಆದರೆ (stbi__do_zlib(&a, p, initial_ಗಾತ್ರ, 1, 1)) {
      ಆದರೆ (outlen) *outlen = (ಪೂರ್ಣಾಂಕ) (a.zout - a.zout_ಆರಂಭ);
      ಹಿಂತಿರುಗಿ a.zout_ಆರಂಭ;
   } ಇಲ್ಲದಿದ್ದರೆ {
      STBI_FREE(a.zout_ಆರಂಭ);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   }
}

STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_malloc(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *outlen)
{
   ಹಿಂತಿರುಗಿ stbi_zlib_decode_malloc_guesssize(ಬಫರ್, len, 16384, outlen);
}

STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_malloc_guesssize_headerflag(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ *outlen, ಪೂರ್ಣಾಂಕ parse_ಶೀರ್ಷಿಕೆ)
{
   stbi__zbuf a;
   ಅಕ್ಷರ *p = (ಅಕ್ಷರ *) stbi__malloc(initial_ಗಾತ್ರ);
   ಆದರೆ (p == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   a.zbuffer = (stbi_uc *) ಬಫರ್;
   a.zbuffer_ಕೊನೆ = (stbi_uc *) ಬಫರ್ + len;
   ಆದರೆ (stbi__do_zlib(&a, p, initial_ಗಾತ್ರ, 1, parse_ಶೀರ್ಷಿಕೆ)) {
      ಆದರೆ (outlen) *outlen = (ಪೂರ್ಣಾಂಕ) (a.zout - a.zout_ಆರಂಭ);
      ಹಿಂತಿರುಗಿ a.zout_ಆರಂಭ;
   } ಇಲ್ಲದಿದ್ದರೆ {
      STBI_FREE(a.zout_ಆರಂಭ);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   }
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_zlib_decode_ಬಫರ್(ಅಕ್ಷರ *obuffer, ಪೂರ್ಣಾಂಕ olen, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ibuffer, ಪೂರ್ಣಾಂಕ ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_ಕೊನೆ = (stbi_uc *) ibuffer + ilen;
   ಆದರೆ (stbi__do_zlib(&a, obuffer, olen, 0, 1))
      ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) (a.zout - a.zout_ಆರಂಭ);
   ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ -1;
}

STBIDEF ಅಕ್ಷರ *stbi_zlib_decode_noheader_malloc(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *outlen)
{
   stbi__zbuf a;
   ಅಕ್ಷರ *p = (ಅಕ್ಷರ *) stbi__malloc(16384);
   ಆದರೆ (p == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   a.zbuffer = (stbi_uc *) ಬಫರ್;
   a.zbuffer_ಕೊನೆ = (stbi_uc *) ಬಫರ್+len;
   ಆದರೆ (stbi__do_zlib(&a, p, 16384, 1, 0)) {
      ಆದರೆ (outlen) *outlen = (ಪೂರ್ಣಾಂಕ) (a.zout - a.zout_ಆರಂಭ);
      ಹಿಂತಿರುಗಿ a.zout_ಆರಂಭ;
   } ಇಲ್ಲದಿದ್ದರೆ {
      STBI_FREE(a.zout_ಆರಂಭ);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   }
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_zlib_decode_noheader_ಬಫರ್(ಅಕ್ಷರ *obuffer, ಪೂರ್ಣಾಂಕ olen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ibuffer, ಪೂರ್ಣಾಂಕ ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_ಕೊನೆ = (stbi_uc *) ibuffer + ilen;
   ಆದರೆ (stbi__do_zlib(&a, obuffer, olen, 0, 0))
      ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) (a.zout - a.zout_ಆರಂಭ);
   ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ -1;
}
#ಆದರೆ_ಅಂತ್ಯ

// public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
//    simple implementation
//      - only 8-bit samples
//      - no CRC checking
//      - allocates lots of intermediate memory
//        - avoids problem of streaming data between subsystems
//        - avoids explicit window management
//    performance
//      - uses stb_zlib, a PD zlib implementation with fast huffman decoding

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNG
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi__uint32 ಉದ್ದಳತೆ;
   stbi__uint32 type;
} stbi__pngchunk;

ಸ್ಥಿರ stbi__pngchunk stbi_get_chunk_ಶೀರ್ಷಿಕೆ(stbi__context *s)
{
   stbi__pngchunk c;
   c.ಉದ್ದಳತೆ = stbi__get32be(s);
   c.type   = stbi__get32be(s);
   ಹಿಂತಿರುಗಿ c;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_check_png_ಶೀರ್ಷಿಕೆ(stbi__context *s)
{
   ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ stbi_uc png_sig[8] = { 137,80,78,71,13,10,26,10 };
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; i < 8; ++i)
      ಆದರೆ (stbi__get8(s) != png_sig[i]) ಹಿಂತಿರುಗಿ stbi__err("bad png sig","Not a PNG");
   ಹಿಂತಿರುಗಿ 1;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi__context *s;
   stbi_uc *idata, *expanded, *ಹೊರಹೋಗು;
   ಪೂರ್ಣಾಂಕ ಆಳ;
} stbi__png;


ಎಣಿಕೆ {
   STBI__F_none=0,
   STBI__F_sub=1,
   STBI_F_ಮೇಲೆ=2,
   STBI__F_avg=3,
   STBI__F_paeth=4,
   // synthetic filters used for first scanline to avoid needing a dummy row of 0s
   STBI_F_avg_ಮೊದಲ,
   STBI_F_paeth_ಮೊದಲ
};

ಸ್ಥಿರ stbi_uc ಮೊದಲ_ಅಡ್ಡಸಾಲು_filter[5] =
{
   STBI__F_none,
   STBI__F_sub,
   STBI__F_none,
   STBI_F_avg_ಮೊದಲ,
   STBI_F_paeth_ಮೊದಲ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__paeth(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ c)
{
   ಪೂರ್ಣಾಂಕ p = a + b - c;
   ಪೂರ್ಣಾಂಕ pa = ನಿರಪೇಕ್ಷ(p-a);
   ಪೂರ್ಣಾಂಕ pb = ನಿರಪೇಕ್ಷ(p-b);
   ಪೂರ್ಣಾಂಕ pc = ನಿರಪೇಕ್ಷ(p-c);
   ಆದರೆ (pa <= pb && pa <= pc) ಹಿಂತಿರುಗಿ a;
   ಆದರೆ (pb <= pc) ಹಿಂತಿರುಗಿ b;
   ಹಿಂತಿರುಗಿ c;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ stbi_uc stbi_ಆಳ_scale_ಕೋಷ್ಟಕ[9] = { 0, 0xff, 0x55, 0, 0x11, 0,0,0, 0x01 };

// create the png data from post-deflated data
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__create_png_image_raw(stbi__png *a, stbi_uc *raw, stbi__uint32 raw_len, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_n, stbi__uint32 x, stbi__uint32 y, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ color)
{
   ಪೂರ್ಣಾಂಕ ಬೈಟುಗಳು = (ಆಳ == 16? 2 : 1);
   stbi__context *s = a->s;
   stbi__uint32 i,j,stride = x*ಹೊರಹೋಗು_n*ಬೈಟುಗಳು;
   stbi__uint32 img_len, img_ಅಗಲ_ಬೈಟುಗಳು;
   ಪೂರ್ಣಾಂಕ k;
   ಪೂರ್ಣಾಂಕ img_n = s->img_n; // copy it into a local for later

   ಪೂರ್ಣಾಂಕ output_ಬೈಟುಗಳು = ಹೊರಹೋಗು_n*ಬೈಟುಗಳು;
   ಪೂರ್ಣಾಂಕ filter_ಬೈಟುಗಳು = img_n*ಬೈಟುಗಳು;
   ಪೂರ್ಣಾಂಕ ಅಗಲ = x;

   STBI_ASSERT(ಹೊರಹೋಗು_n == s->img_n || ಹೊರಹೋಗು_n == s->img_n+1);
   a->ಹೊರಹೋಗು = (stbi_uc *) stbi__malloc_mad3(x, y, output_ಬೈಟುಗಳು, 0); // extra bytes to write off the end into
   ಆದರೆ (!a->ಹೊರಹೋಗು) ಹಿಂತಿರುಗಿ stbi__err("outofmem", "Out of memory");

   ಆದರೆ (!stbi__mad3sizes_valid(img_n, x, ಆಳ, 7)) ಹಿಂತಿರುಗಿ stbi__err("too large", "Corrupt PNG");
   img_ಅಗಲ_ಬೈಟುಗಳು = (((img_n * x * ಆಳ) + 7) >> 3);
   img_len = (img_ಅಗಲ_ಬೈಟುಗಳು + 1) * y;

   // we used to check for exact match between raw_len and img_len on non-interlaced PNGs,
   // but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),
   // so just check for raw_len < img_len always.
   ಆದರೆ (raw_len < img_len) ಹಿಂತಿರುಗಿ stbi__err("not enough pixels","Corrupt PNG");

   ಗಾಗಿ (j=0; j < y; ++j) {
      stbi_uc *cur = a->ಹೊರಹೋಗು + stride*j;
      stbi_uc *ಪೂರ್ವ;
      ಪೂರ್ಣಾಂಕ filter = *raw++;

      ಆದರೆ (filter > 4)
         ಹಿಂತಿರುಗಿ stbi__err("invalid filter","Corrupt PNG");

      ಆದರೆ (ಆಳ < 8) {
         STBI_ASSERT(img_ಅಗಲ_ಬೈಟುಗಳು <= x);
         cur += x*ಹೊರಹೋಗು_n - img_ಅಗಲ_ಬೈಟುಗಳು; // store output to the rightmost img_len bytes, so we can decode in place
         filter_ಬೈಟುಗಳು = 1;
         ಅಗಲ = img_ಅಗಲ_ಬೈಟುಗಳು;
      }
      ಪೂರ್ವ = cur - stride; // bugfix: need to compute this after 'cur +=' computation above

      // if first row, use special filter that doesn't sample previous row
      ಆದರೆ (j == 0) filter = ಮೊದಲ_ಅಡ್ಡಸಾಲು_filter[filter];

      // handle first byte explicitly
      ಗಾಗಿ (k=0; k < filter_ಬೈಟುಗಳು; ++k) {
         ಆಯ್ಕೆ (filter) {
            ಸ್ಥಿತಿ STBI__F_none       : cur[k] = raw[k]; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ STBI__F_sub        : cur[k] = raw[k]; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ STBI_F_ಮೇಲೆ         : cur[k] = STBI__BYTECAST(raw[k] + ಪೂರ್ವ[k]); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ STBI__F_avg        : cur[k] = STBI__BYTECAST(raw[k] + (ಪೂರ್ವ[k]>>1)); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ STBI__F_paeth      : cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(0,ಪೂರ್ವ[k],0)); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ STBI_F_avg_ಮೊದಲ  : cur[k] = raw[k]; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ STBI_F_paeth_ಮೊದಲ: cur[k] = raw[k]; ನಿಲ್ಲಿಸು;
         }
      }

      ಆದರೆ (ಆಳ == 8) {
         ಆದರೆ (img_n != ಹೊರಹೋಗು_n)
            cur[img_n] = 255; // first pixel
         raw += img_n;
         cur += ಹೊರಹೋಗು_n;
         ಪೂರ್ವ += ಹೊರಹೋಗು_n;
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == 16) {
         ಆದರೆ (img_n != ಹೊರಹೋಗು_n) {
            cur[filter_ಬೈಟುಗಳು]   = 255; // first pixel top byte
            cur[filter_ಬೈಟುಗಳು+1] = 255; // first pixel bottom byte
         }
         raw += filter_ಬೈಟುಗಳು;
         cur += output_ಬೈಟುಗಳು;
         ಪೂರ್ವ += output_ಬೈಟುಗಳು;
      } ಇಲ್ಲದಿದ್ದರೆ {
         raw += 1;
         cur += 1;
         ಪೂರ್ವ += 1;
      }

      // this is a little gross, so that we don't switch per-pixel or per-component
      ಆದರೆ (ಆಳ < 8 || img_n == ಹೊರಹೋಗು_n) {
         ಪೂರ್ಣಾಂಕ nk = (ಅಗಲ - 1)*filter_ಬೈಟುಗಳು;
         #ವ್ಯಾಖ್ಯಾನಿಸು STBI__CASE(f) \
             ಸ್ಥಿತಿ f:     \
                ಗಾಗಿ (k=0; k < nk; ++k)
         ಆಯ್ಕೆ (filter) {
            // "none" filter turns into a memcpy here; make that explicit.
            ಸ್ಥಿತಿ STBI__F_none:         ಸ್ಮರಣೆ_ನಕಲು(cur, raw, nk); ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k-filter_ಬೈಟುಗಳು]); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI_F_ಮೇಲೆ)           { cur[k] = STBI__BYTECAST(raw[k] + ಪೂರ್ವ[k]); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((ಪೂರ್ವ[k] + cur[k-filter_ಬೈಟುಗಳು])>>1)); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_ಬೈಟುಗಳು],ಪೂರ್ವ[k],ಪೂರ್ವ[k-filter_ಬೈಟುಗಳು])); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI_F_avg_ಮೊದಲ)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k-filter_ಬೈಟುಗಳು] >> 1)); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI_F_paeth_ಮೊದಲ)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_ಬೈಟುಗಳು],0,0)); } ನಿಲ್ಲಿಸು;
         }
         #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STBI__CASE
         raw += nk;
      } ಇಲ್ಲದಿದ್ದರೆ {
         STBI_ASSERT(img_n+1 == ಹೊರಹೋಗು_n);
         #ವ್ಯಾಖ್ಯಾನಿಸು STBI__CASE(f) \
             ಸ್ಥಿತಿ f:     \
                ಗಾಗಿ (i=x-1; i >= 1; --i, cur[filter_ಬೈಟುಗಳು]=255,raw+=filter_ಬೈಟುಗಳು,cur+=output_ಬೈಟುಗಳು,ಪೂರ್ವ+=output_ಬೈಟುಗಳು) \
                   ಗಾಗಿ (k=0; k < filter_ಬೈಟುಗಳು; ++k)
         ಆಯ್ಕೆ (filter) {
            STBI__CASE(STBI__F_none)         { cur[k] = raw[k]; } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k- output_ಬೈಟುಗಳು]); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI_F_ಮೇಲೆ)           { cur[k] = STBI__BYTECAST(raw[k] + ಪೂರ್ವ[k]); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((ಪೂರ್ವ[k] + cur[k- output_ಬೈಟುಗಳು])>>1)); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_ಬೈಟುಗಳು],ಪೂರ್ವ[k],ಪೂರ್ವ[k- output_ಬೈಟುಗಳು])); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI_F_avg_ಮೊದಲ)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k- output_ಬೈಟುಗಳು] >> 1)); } ನಿಲ್ಲಿಸು;
            STBI__CASE(STBI_F_paeth_ಮೊದಲ)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_ಬೈಟುಗಳು],0,0)); } ನಿಲ್ಲಿಸು;
         }
         #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STBI__CASE

         // the loop above sets the high byte of the pixels' alpha, but for
         // 16 bit png files we also need the low byte set. we'll do that here.
         ಆದರೆ (ಆಳ == 16) {
            cur = a->ಹೊರಹೋಗು + stride*j; // start at the beginning of the row again
            ಗಾಗಿ (i=0; i < x; ++i,cur+=output_ಬೈಟುಗಳು) {
               cur[filter_ಬೈಟುಗಳು+1] = 255;
            }
         }
      }
   }

   // we make a separate pass to expand bits to pixels; for performance,
   // this could run two scanlines behind the above code, so it won't
   // intefere with filtering but will still be in the cache.
   ಆದರೆ (ಆಳ < 8) {
      ಗಾಗಿ (j=0; j < y; ++j) {
         stbi_uc *cur = a->ಹೊರಹೋಗು + stride*j;
         stbi_uc *ಒಳ  = a->ಹೊರಹೋಗು + stride*j + x*ಹೊರಹೋಗು_n - img_ಅಗಲ_ಬೈಟುಗಳು;
         // unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit
         // png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop
         stbi_uc scale = (color == 0) ? stbi_ಆಳ_scale_ಕೋಷ್ಟಕ[ಆಳ] : 1; // scale grayscale values to 0..255 range

         // note that the final byte might overshoot and write more data than desired.
         // we can allocate enough data that this never writes out of memory, but it
         // could also overwrite the next scanline. can it overwrite non-empty data
         // on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.
         // so we need to explicitly clamp the final ones

         ಆದರೆ (ಆಳ == 4) {
            ಗಾಗಿ (k=x*img_n; k >= 2; k-=2, ++ಒಳ) {
               *cur++ = scale * ((*ಒಳ >> 4)       );
               *cur++ = scale * ((*ಒಳ     ) & 0x0f);
            }
            ಆದರೆ (k > 0) *cur++ = scale * ((*ಒಳ >> 4)       );
         } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == 2) {
            ಗಾಗಿ (k=x*img_n; k >= 4; k-=4, ++ಒಳ) {
               *cur++ = scale * ((*ಒಳ >> 6)       );
               *cur++ = scale * ((*ಒಳ >> 4) & 0x03);
               *cur++ = scale * ((*ಒಳ >> 2) & 0x03);
               *cur++ = scale * ((*ಒಳ     ) & 0x03);
            }
            ಆದರೆ (k > 0) *cur++ = scale * ((*ಒಳ >> 6)       );
            ಆದರೆ (k > 1) *cur++ = scale * ((*ಒಳ >> 4) & 0x03);
            ಆದರೆ (k > 2) *cur++ = scale * ((*ಒಳ >> 2) & 0x03);
         } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == 1) {
            ಗಾಗಿ (k=x*img_n; k >= 8; k-=8, ++ಒಳ) {
               *cur++ = scale * ((*ಒಳ >> 7)       );
               *cur++ = scale * ((*ಒಳ >> 6) & 0x01);
               *cur++ = scale * ((*ಒಳ >> 5) & 0x01);
               *cur++ = scale * ((*ಒಳ >> 4) & 0x01);
               *cur++ = scale * ((*ಒಳ >> 3) & 0x01);
               *cur++ = scale * ((*ಒಳ >> 2) & 0x01);
               *cur++ = scale * ((*ಒಳ >> 1) & 0x01);
               *cur++ = scale * ((*ಒಳ     ) & 0x01);
            }
            ಆದರೆ (k > 0) *cur++ = scale * ((*ಒಳ >> 7)       );
            ಆದರೆ (k > 1) *cur++ = scale * ((*ಒಳ >> 6) & 0x01);
            ಆದರೆ (k > 2) *cur++ = scale * ((*ಒಳ >> 5) & 0x01);
            ಆದರೆ (k > 3) *cur++ = scale * ((*ಒಳ >> 4) & 0x01);
            ಆದರೆ (k > 4) *cur++ = scale * ((*ಒಳ >> 3) & 0x01);
            ಆದರೆ (k > 5) *cur++ = scale * ((*ಒಳ >> 2) & 0x01);
            ಆದರೆ (k > 6) *cur++ = scale * ((*ಒಳ >> 1) & 0x01);
         }
         ಆದರೆ (img_n != ಹೊರಹೋಗು_n) {
            ಪೂರ್ಣಾಂಕ q;
            // insert alpha = 255
            cur = a->ಹೊರಹೋಗು + stride*j;
            ಆದರೆ (img_n == 1) {
               ಗಾಗಿ (q=x-1; q >= 0; --q) {
                  cur[q*2+1] = 255;
                  cur[q*2+0] = cur[q];
               }
            } ಇಲ್ಲದಿದ್ದರೆ {
               STBI_ASSERT(img_n == 3);
               ಗಾಗಿ (q=x-1; q >= 0; --q) {
                  cur[q*4+3] = 255;
                  cur[q*4+2] = cur[q*3+2];
                  cur[q*4+1] = cur[q*3+1];
                  cur[q*4+0] = cur[q*3+0];
               }
            }
         }
      }
   } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == 16) {
      // force the image data from big-endian to platform-native.
      // this is done in a separate pass due to the decoding relying
      // on the data being untouched, but could probably be done
      // per-line during decode if care is taken.
      stbi_uc *cur = a->ಹೊರಹೋಗು;
      stbi__uint16 *cur16 = (stbi__uint16*)cur;

      ಗಾಗಿ(i=0; i < x*y*ಹೊರಹೋಗು_n; ++i,cur16++,cur+=2) {
         *cur16 = (cur[0] << 8) | cur[1];
      }
   }

   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__create_png_image(stbi__png *a, stbi_uc *image_ದತ್ತಾಂಶ_2, stbi__uint32 image_ದತ್ತಾಂಶ_len, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_n, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ color, ಪೂರ್ಣಾಂಕ interlaced)
{
   ಪೂರ್ಣಾಂಕ ಬೈಟುಗಳು = (ಆಳ == 16 ? 2 : 1);
   ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಬೈಟುಗಳು = ಹೊರಹೋಗು_n * ಬೈಟುಗಳು;
   stbi_uc *final;
   ಪೂರ್ಣಾಂಕ p;
   ಆದರೆ (!interlaced)
      ಹಿಂತಿರುಗಿ stbi__create_png_image_raw(a, image_ದತ್ತಾಂಶ_2, image_ದತ್ತಾಂಶ_len, ಹೊರಹೋಗು_n, a->s->img_x, a->s->img_y, ಆಳ, color);

   // de-interlacing
   final = (stbi_uc *) stbi__malloc_mad3(a->s->img_x, a->s->img_y, ಹೊರಹೋಗು_ಬೈಟುಗಳು, 0);
   ಗಾಗಿ (p=0; p < 7; ++p) {
      ಪೂರ್ಣಾಂಕ xorig[] = { 0,4,0,2,0,1,0 };
      ಪೂರ್ಣಾಂಕ yorig[] = { 0,0,4,0,2,0,1 };
      ಪೂರ್ಣಾಂಕ xspc[]  = { 8,8,4,4,2,2,1 };
      ಪೂರ್ಣಾಂಕ yspc[]  = { 8,8,8,4,4,2,2 };
      ಪೂರ್ಣಾಂಕ i,j,x,y;
      // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
      x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
      y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
      ಆದರೆ (x && y) {
         stbi__uint32 img_len = ((((a->s->img_n * x * ಆಳ) + 7) >> 3) + 1) * y;
         ಆದರೆ (!stbi__create_png_image_raw(a, image_ದತ್ತಾಂಶ_2, image_ದತ್ತಾಂಶ_len, ಹೊರಹೋಗು_n, x, y, ಆಳ, color)) {
            STBI_FREE(final);
            ಹಿಂತಿರುಗಿ 0;
         }
         ಗಾಗಿ (j=0; j < y; ++j) {
            ಗಾಗಿ (i=0; i < x; ++i) {
               ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_y = j*yspc[p]+yorig[p];
               ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_x = i*xspc[p]+xorig[p];
               ಸ್ಮರಣೆ_ನಕಲು(final + ಹೊರಹೋಗು_y*a->s->img_x*ಹೊರಹೋಗು_ಬೈಟುಗಳು + ಹೊರಹೋಗು_x*ಹೊರಹೋಗು_ಬೈಟುಗಳು,
                      a->ಹೊರಹೋಗು + (j*x+i)*ಹೊರಹೋಗು_ಬೈಟುಗಳು, ಹೊರಹೋಗು_ಬೈಟುಗಳು);
            }
         }
         STBI_FREE(a->ಹೊರಹೋಗು);
         image_ದತ್ತಾಂಶ_2 += img_len;
         image_ದತ್ತಾಂಶ_len -= img_len;
      }
   }
   a->ಹೊರಹೋಗು = final;

   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__compute_transparency(stbi__png *z, stbi_uc tc[3], ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ಎಣಿಕೆ = s->img_x * s->img_y;
   stbi_uc *p = z->ಹೊರಹೋಗು;

   // compute color-based transparency, assuming we've
   // already got 255 as the alpha value in the output
   STBI_ASSERT(ಹೊರಹೋಗು_n == 2 || ಹೊರಹೋಗು_n == 4);

   ಆದರೆ (ಹೊರಹೋಗು_n == 2) {
      ಗಾಗಿ (i=0; i < pixel_ಎಣಿಕೆ; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 255);
         p += 2;
      }
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಗಾಗಿ (i=0; i < pixel_ಎಣಿಕೆ; ++i) {
         ಆದರೆ (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__compute_transparency16(stbi__png *z, stbi__uint16 tc[3], ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ಎಣಿಕೆ = s->img_x * s->img_y;
   stbi__uint16 *p = (stbi__uint16*) z->ಹೊರಹೋಗು;

   // compute color-based transparency, assuming we've
   // already got 65535 as the alpha value in the output
   STBI_ASSERT(ಹೊರಹೋಗು_n == 2 || ಹೊರಹೋಗು_n == 4);

   ಆದರೆ (ಹೊರಹೋಗು_n == 2) {
      ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 65535);
         p += 2;
      }
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ; ++i) {
         ಆದರೆ (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__expand_png_palette(stbi__png *a, stbi_uc *palette, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ pal_img_n)
{
   stbi__uint32 i, pixel_ಎಣಿಕೆ = a->s->img_x * a->s->img_y;
   stbi_uc *p, *ತಾತ್ಕಾಲಿಕ_ಹೊರಹೋಗು, *orig = a->ಹೊರಹೋಗು;

   p = (stbi_uc *) stbi__malloc_mad2(pixel_ಎಣಿಕೆ, pal_img_n, 0);
   ಆದರೆ (p == ಖಾಲಿ) ಹಿಂತಿರುಗಿ stbi__err("outofmem", "Out of memory");

   // between here and free(out) below, exitting would leak
   ತಾತ್ಕಾಲಿಕ_ಹೊರಹೋಗು = p;

   ಆದರೆ (pal_img_n == 3) {
      ಗಾಗಿ (i=0; i < pixel_ಎಣಿಕೆ; ++i) {
         ಪೂರ್ಣಾಂಕ n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p += 3;
      }
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಗಾಗಿ (i=0; i < pixel_ಎಣಿಕೆ; ++i) {
         ಪೂರ್ಣಾಂಕ n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p[3] = palette[n+3];
         p += 4;
      }
   }
   STBI_FREE(a->ಹೊರಹೋಗು);
   a->ಹೊರಹೋಗು = ತಾತ್ಕಾಲಿಕ_ಹೊರಹೋಗು;

   STBI_NOTUSED(len);

   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_unpremultiply_on_ತುಂಬು = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__de_iphone_flag = 0;

STBIDEF ಶೂನ್ಯ stbi_ಗಣ_unpremultiply_on_ತುಂಬು(ಪೂರ್ಣಾಂಕ flag_true_if_should_unpremultiply)
{
   stbi_unpremultiply_on_ತುಂಬು = flag_true_if_should_unpremultiply;
}

STBIDEF ಶೂನ್ಯ stbi_convert_iphone_png_ಗಮ್ಯ_rgb(ಪೂರ್ಣಾಂಕ flag_true_if_should_convert)
{
   stbi__de_iphone_flag = flag_true_if_should_convert;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__de_iphone(stbi__png *z)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ಎಣಿಕೆ = s->img_x * s->img_y;
   stbi_uc *p = z->ಹೊರಹೋಗು;

   ಆದರೆ (s->img_ಹೊರಹೋಗು_n == 3) {  // convert bgr to rgb
      ಗಾಗಿ (i=0; i < pixel_ಎಣಿಕೆ; ++i) {
         stbi_uc t = p[0];
         p[0] = p[2];
         p[2] = t;
         p += 3;
      }
   } ಇಲ್ಲದಿದ್ದರೆ {
      STBI_ASSERT(s->img_ಹೊರಹೋಗು_n == 4);
      ಆದರೆ (stbi_unpremultiply_on_ತುಂಬು) {
         // convert bgr to rgb and unpremultiply
         ಗಾಗಿ (i=0; i < pixel_ಎಣಿಕೆ; ++i) {
            stbi_uc a = p[3];
            stbi_uc t = p[0];
            ಆದರೆ (a) {
               stbi_uc half = a / 2;
               p[0] = (p[2] * 255 + half) / a;
               p[1] = (p[1] * 255 + half) / a;
               p[2] = ( t   * 255 + half) / a;
            } ಇಲ್ಲದಿದ್ದರೆ {
               p[0] = p[2];
               p[2] = t;
            }
            p += 4;
         }
      } ಇಲ್ಲದಿದ್ದರೆ {
         // convert bgr to rgb
         ಗಾಗಿ (i=0; i < pixel_ಎಣಿಕೆ; ++i) {
            stbi_uc t = p[0];
            p[0] = p[2];
            p[2] = t;
            p += 4;
         }
      }
   }
}

#ವ್ಯಾಖ್ಯಾನಿಸು STBI__PNG_TYPE(a,b,c,d)  (((ಚಿಹ್ನೆರಹಿತ) (a) << 24) + ((ಚಿಹ್ನೆರಹಿತ) (b) << 16) + ((ಚಿಹ್ನೆರಹಿತ) (c) << 8) + (ಚಿಹ್ನೆರಹಿತ) (d))

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__parse_png_file(stbi__png *z, ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ, ಪೂರ್ಣಾಂಕ req_comp)
{
   stbi_uc palette[1024], pal_img_n=0;
   stbi_uc has_trans=0, tc[3];
   stbi__uint16 tc16[3];
   stbi__uint32 ioff=0, idata_ಎಲ್ಲೆ=0, i, pal_len=0;
   ಪೂರ್ಣಾಂಕ ಮೊದಲ=1,k,interlace=0, color=0, is_iphone=0;
   stbi__context *s = z->s;

   z->expanded = ಖಾಲಿ;
   z->idata = ಖಾಲಿ;
   z->ಹೊರಹೋಗು = ಖಾಲಿ;

   ಆದರೆ (!stbi_check_png_ಶೀರ್ಷಿಕೆ(s)) ಹಿಂತಿರುಗಿ 0;

   ಆದರೆ (ಪರಿಶೀಲನೆ == STBI_ಪರಿಶೀಲನೆ_type) ಹಿಂತಿರುಗಿ 1;

   ಗಾಗಿ (;;) {
      stbi__pngchunk c = stbi_get_chunk_ಶೀರ್ಷಿಕೆ(s);
      ಆಯ್ಕೆ (c.type) {
         ಸ್ಥಿತಿ STBI__PNG_TYPE('C','g','B','I'):
            is_iphone = 1;
            stbi__skip(s, c.ಉದ್ದಳತೆ);
            ನಿಲ್ಲಿಸು;
         ಸ್ಥಿತಿ STBI__PNG_TYPE('I','H','D','R'): {
            ಪೂರ್ಣಾಂಕ comp,filter;
            ಆದರೆ (!ಮೊದಲ) ಹಿಂತಿರುಗಿ stbi__err("multiple IHDR","Corrupt PNG");
            ಮೊದಲ = 0;
            ಆದರೆ (c.ಉದ್ದಳತೆ != 13) ಹಿಂತಿರುಗಿ stbi__err("bad IHDR len","Corrupt PNG");
            s->img_x = stbi__get32be(s); ಆದರೆ (s->img_x > (1 << 24)) ಹಿಂತಿರುಗಿ stbi__err("too large","Very large image (corrupt?)");
            s->img_y = stbi__get32be(s); ಆದರೆ (s->img_y > (1 << 24)) ಹಿಂತಿರುಗಿ stbi__err("too large","Very large image (corrupt?)");
            z->ಆಳ = stbi__get8(s);  ಆದರೆ (z->ಆಳ != 1 && z->ಆಳ != 2 && z->ಆಳ != 4 && z->ಆಳ != 8 && z->ಆಳ != 16)  ಹಿಂತಿರುಗಿ stbi__err("1/2/4/8/16-bit only","PNG not supported: 1/2/4/8/16-bit only");
            color = stbi__get8(s);  ಆದರೆ (color > 6)         ಹಿಂತಿರುಗಿ stbi__err("bad ctype","Corrupt PNG");
            ಆದರೆ (color == 3 && z->ಆಳ == 16)                  ಹಿಂತಿರುಗಿ stbi__err("bad ctype","Corrupt PNG");
            ಆದರೆ (color == 3) pal_img_n = 3; ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (color & 1) ಹಿಂತಿರುಗಿ stbi__err("bad ctype","Corrupt PNG");
            comp  = stbi__get8(s);  ಆದರೆ (comp) ಹಿಂತಿರುಗಿ stbi__err("bad comp method","Corrupt PNG");
            filter= stbi__get8(s);  ಆದರೆ (filter) ಹಿಂತಿರುಗಿ stbi__err("bad filter method","Corrupt PNG");
            interlace = stbi__get8(s); ಆದರೆ (interlace>1) ಹಿಂತಿರುಗಿ stbi__err("bad interlace method","Corrupt PNG");
            ಆದರೆ (!s->img_x || !s->img_y) ಹಿಂತಿರುಗಿ stbi__err("0-pixel image","Corrupt PNG");
            ಆದರೆ (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               ಆದರೆ ((1 << 30) / s->img_x / s->img_n < s->img_y) ಹಿಂತಿರುಗಿ stbi__err("too large", "Image too large to decode");
               ಆದರೆ (ಪರಿಶೀಲನೆ == STBI_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ) ಹಿಂತಿರುಗಿ 1;
            } ಇಲ್ಲದಿದ್ದರೆ {
               // if paletted, then pal_n is our final components, and
               // img_n is # components to decompress/filter.
               s->img_n = 1;
               ಆದರೆ ((1 << 30) / s->img_x / 4 < s->img_y) ಹಿಂತಿರುಗಿ stbi__err("too large","Corrupt PNG");
               // if SCAN_header, have to scan to see if we have a tRNS
            }
            ನಿಲ್ಲಿಸು;
         }

         ಸ್ಥಿತಿ STBI__PNG_TYPE('P','L','T','E'):  {
            ಆದರೆ (ಮೊದಲ) ಹಿಂತಿರುಗಿ stbi__err("first not IHDR", "Corrupt PNG");
            ಆದರೆ (c.ಉದ್ದಳತೆ > 256*3) ಹಿಂತಿರುಗಿ stbi__err("invalid PLTE","Corrupt PNG");
            pal_len = c.ಉದ್ದಳತೆ / 3;
            ಆದರೆ (pal_len * 3 != c.ಉದ್ದಳತೆ) ಹಿಂತಿರುಗಿ stbi__err("invalid PLTE","Corrupt PNG");
            ಗಾಗಿ (i=0; i < pal_len; ++i) {
               palette[i*4+0] = stbi__get8(s);
               palette[i*4+1] = stbi__get8(s);
               palette[i*4+2] = stbi__get8(s);
               palette[i*4+3] = 255;
            }
            ನಿಲ್ಲಿಸು;
         }

         ಸ್ಥಿತಿ STBI__PNG_TYPE('t','R','N','S'): {
            ಆದರೆ (ಮೊದಲ) ಹಿಂತಿರುಗಿ stbi__err("first not IHDR", "Corrupt PNG");
            ಆದರೆ (z->idata) ಹಿಂತಿರುಗಿ stbi__err("tRNS after IDAT","Corrupt PNG");
            ಆದರೆ (pal_img_n) {
               ಆದರೆ (ಪರಿಶೀಲನೆ == STBI_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ) { s->img_n = 4; ಹಿಂತಿರುಗಿ 1; }
               ಆದರೆ (pal_len == 0) ಹಿಂತಿರುಗಿ stbi__err("tRNS before PLTE","Corrupt PNG");
               ಆದರೆ (c.ಉದ್ದಳತೆ > pal_len) ಹಿಂತಿರುಗಿ stbi__err("bad tRNS len","Corrupt PNG");
               pal_img_n = 4;
               ಗಾಗಿ (i=0; i < c.ಉದ್ದಳತೆ; ++i)
                  palette[i*4+3] = stbi__get8(s);
            } ಇಲ್ಲದಿದ್ದರೆ {
               ಆದರೆ (!(s->img_n & 1)) ಹಿಂತಿರುಗಿ stbi__err("tRNS with alpha","Corrupt PNG");
               ಆದರೆ (c.ಉದ್ದಳತೆ != (stbi__uint32) s->img_n*2) ಹಿಂತಿರುಗಿ stbi__err("bad tRNS len","Corrupt PNG");
               has_trans = 1;
               ಆದರೆ (z->ಆಳ == 16) {
                  ಗಾಗಿ (k = 0; k < s->img_n; ++k) tc16[k] = (stbi__uint16)stbi__get16be(s); // copy the values as-is
               } ಇಲ್ಲದಿದ್ದರೆ {
                  ಗಾಗಿ (k = 0; k < s->img_n; ++k) tc[k] = (stbi_uc)(stbi__get16be(s) & 255) * stbi_ಆಳ_scale_ಕೋಷ್ಟಕ[z->ಆಳ]; // non 8-bit images will be larger
               }
            }
            ನಿಲ್ಲಿಸು;
         }

         ಸ್ಥಿತಿ STBI__PNG_TYPE('I','D','A','T'): {
            ಆದರೆ (ಮೊದಲ) ಹಿಂತಿರುಗಿ stbi__err("first not IHDR", "Corrupt PNG");
            ಆದರೆ (pal_img_n && !pal_len) ಹಿಂತಿರುಗಿ stbi__err("no PLTE","Corrupt PNG");
            ಆದರೆ (ಪರಿಶೀಲನೆ == STBI_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ) { s->img_n = pal_img_n; ಹಿಂತಿರುಗಿ 1; }
            ಆದರೆ ((ಪೂರ್ಣಾಂಕ)(ioff + c.ಉದ್ದಳತೆ) < (ಪೂರ್ಣಾಂಕ)ioff) ಹಿಂತಿರುಗಿ 0;
            ಆದರೆ (ioff + c.ಉದ್ದಳತೆ > idata_ಎಲ್ಲೆ) {
               stbi__uint32 idata_ಎಲ್ಲೆ_old = idata_ಎಲ್ಲೆ;
               stbi_uc *p;
               ಆದರೆ (idata_ಎಲ್ಲೆ == 0) idata_ಎಲ್ಲೆ = c.ಉದ್ದಳತೆ > 4096 ? c.ಉದ್ದಳತೆ : 4096;
               ಆಗ (ioff + c.ಉದ್ದಳತೆ > idata_ಎಲ್ಲೆ)
                  idata_ಎಲ್ಲೆ *= 2;
               STBI_NOTUSED(idata_ಎಲ್ಲೆ_old);
               p = (stbi_uc *) STBI_REALLOC_SIZED(z->idata, idata_ಎಲ್ಲೆ_old, idata_ಎಲ್ಲೆ); ಆದರೆ (p == ಖಾಲಿ) ಹಿಂತಿರುಗಿ stbi__err("outofmem", "Out of memory");
               z->idata = p;
            }
            ಆದರೆ (!stbi__getn(s, z->idata+ioff,c.ಉದ್ದಳತೆ)) ಹಿಂತಿರುಗಿ stbi__err("outofdata","Corrupt PNG");
            ioff += c.ಉದ್ದಳತೆ;
            ನಿಲ್ಲಿಸು;
         }

         ಸ್ಥಿತಿ STBI__PNG_TYPE('I','E','N','D'): {
            stbi__uint32 raw_len, bpl;
            ಆದರೆ (ಮೊದಲ) ಹಿಂತಿರುಗಿ stbi__err("first not IHDR", "Corrupt PNG");
            ಆದರೆ (ಪರಿಶೀಲನೆ != STBI_ಪರಿಶೀಲನೆ_ತುಂಬು) ಹಿಂತಿರುಗಿ 1;
            ಆದರೆ (z->idata == ಖಾಲಿ) ಹಿಂತಿರುಗಿ stbi__err("no IDAT","Corrupt PNG");
            // initial guess for decoded data size to avoid unnecessary reallocs
            bpl = (s->img_x * z->ಆಳ + 7) / 8; // bytes per line, per component
            raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
            z->expanded = (stbi_uc *) stbi_zlib_decode_malloc_guesssize_headerflag((ಅಕ್ಷರ *) z->idata, ioff, raw_len, (ಪೂರ್ಣಾಂಕ *) &raw_len, !is_iphone);
            ಆದರೆ (z->expanded == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0; // zlib should set error
            STBI_FREE(z->idata); z->idata = ಖಾಲಿ;
            ಆದರೆ ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_ಹೊರಹೋಗು_n = s->img_n+1;
            ಇಲ್ಲದಿದ್ದರೆ
               s->img_ಹೊರಹೋಗು_n = s->img_n;
            ಆದರೆ (!stbi__create_png_image(z, z->expanded, raw_len, s->img_ಹೊರಹೋಗು_n, z->ಆಳ, color, interlace)) ಹಿಂತಿರುಗಿ 0;
            ಆದರೆ (has_trans) {
               ಆದರೆ (z->ಆಳ == 16) {
                  ಆದರೆ (!stbi__compute_transparency16(z, tc16, s->img_ಹೊರಹೋಗು_n)) ಹಿಂತಿರುಗಿ 0;
               } ಇಲ್ಲದಿದ್ದರೆ {
                  ಆದರೆ (!stbi__compute_transparency(z, tc, s->img_ಹೊರಹೋಗು_n)) ಹಿಂತಿರುಗಿ 0;
               }
            }
            ಆದರೆ (is_iphone && stbi__de_iphone_flag && s->img_ಹೊರಹೋಗು_n > 2)
               stbi__de_iphone(z);
            ಆದರೆ (pal_img_n) {
               // pal_img_n == 3 or 4
               s->img_n = pal_img_n; // record the actual colors we had
               s->img_ಹೊರಹೋಗು_n = pal_img_n;
               ಆದರೆ (req_comp >= 3) s->img_ಹೊರಹೋಗು_n = req_comp;
               ಆದರೆ (!stbi__expand_png_palette(z, palette, pal_len, s->img_ಹೊರಹೋಗು_n))
                  ಹಿಂತಿರುಗಿ 0;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_trans) {
               // non-paletted image with tRNS -> source image has (constant) alpha
               ++s->img_n;
            }
            STBI_FREE(z->expanded); z->expanded = ಖಾಲಿ;
            ಹಿಂತಿರುಗಿ 1;
         }

         ಪೂರ್ವನಿಯೋಜಿತ:
            // if critical, fail
            ಆದರೆ (ಮೊದಲ) ಹಿಂತಿರುಗಿ stbi__err("first not IHDR", "Corrupt PNG");
            ಆದರೆ ((c.type & (1 << 29)) == 0) {
               #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_FAILURE_ಅಕ್ಷರಸರಣಿಗಳು
               // not threadsafe
               ಸ್ಥಿರ ಅಕ್ಷರ invalid_chunk[] = "XXXX PNG chunk not known";
               invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
               invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
               invalid_chunk[2] = STBI__BYTECAST(c.type >>  8);
               invalid_chunk[3] = STBI__BYTECAST(c.type >>  0);
               #ಆದರೆ_ಅಂತ್ಯ
               ಹಿಂತಿರುಗಿ stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
            }
            stbi__skip(s, c.ಉದ್ದಳತೆ);
            ನಿಲ್ಲಿಸು;
      }
      // end of PNG chunk, read and skip CRC
      stbi__get32be(s);
   }
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi__do_png(stbi__png *p, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *n, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   ಶೂನ್ಯ *ಫಲಿತಾಂಶ=ಖಾಲಿ;
   ಆದರೆ (req_comp < 0 || req_comp > 4) ಹಿಂತಿರುಗಿ stbi__errpuc("bad req_comp", "Internal error");
   ಆದರೆ (stbi__parse_png_file(p, STBI_ಪರಿಶೀಲನೆ_ತುಂಬು, req_comp)) {
      ಆದರೆ (p->ಆಳ < 8)
         ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel = 8;
      ಇಲ್ಲದಿದ್ದರೆ
         ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel = p->ಆಳ;
      ಫಲಿತಾಂಶ = p->ಹೊರಹೋಗು;
      p->ಹೊರಹೋಗು = ಖಾಲಿ;
      ಆದರೆ (req_comp && req_comp != p->s->img_ಹೊರಹೋಗು_n) {
         ಆದರೆ (ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel == 8)
            ಫಲಿತಾಂಶ = stbi__convert_format((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) ಫಲಿತಾಂಶ, p->s->img_ಹೊರಹೋಗು_n, req_comp, p->s->img_x, p->s->img_y);
         ಇಲ್ಲದಿದ್ದರೆ
            ಫಲಿತಾಂಶ = stbi__convert_format16((stbi__uint16 *) ಫಲಿತಾಂಶ, p->s->img_ಹೊರಹೋಗು_n, req_comp, p->s->img_x, p->s->img_y);
         p->s->img_ಹೊರಹೋಗು_n = req_comp;
         ಆದರೆ (ಫಲಿತಾಂಶ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
      }
      *x = p->s->img_x;
      *y = p->s->img_y;
      ಆದರೆ (n) *n = p->s->img_n;
   }
   STBI_FREE(p->ಹೊರಹೋಗು);      p->ಹೊರಹೋಗು      = ಖಾಲಿ;
   STBI_FREE(p->expanded); p->expanded = ಖಾಲಿ;
   STBI_FREE(p->idata);    p->idata    = ಖಾಲಿ;

   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_png_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   stbi__png p;
   p.s = s;
   ಹಿಂತಿರುಗಿ stbi__do_png(&p, x,y,comp,req_comp, ri);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__png_test(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ r;
   r = stbi_check_png_ಶೀರ್ಷಿಕೆ(s);
   stbi__rewind(s);
   ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__png_info_raw(stbi__png *p, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಆದರೆ (!stbi__parse_png_file(p, STBI_ಪರಿಶೀಲನೆ_ಶೀರ್ಷಿಕೆ, 0)) {
      stbi__rewind( p->s );
      ಹಿಂತಿರುಗಿ 0;
   }
   ಆದರೆ (x) *x = p->s->img_x;
   ಆದರೆ (y) *y = p->s->img_y;
   ಆದರೆ (comp) *comp = p->s->img_n;
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__png_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   stbi__png p;
   p.s = s;
   ಹಿಂತಿರುಗಿ stbi__png_info_raw(&p, x, y, comp);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__png_is16(stbi__context *s)
{
   stbi__png p;
   p.s = s;
   ಆದರೆ (!stbi__png_info_raw(&p, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ))
	   ಹಿಂತಿರುಗಿ 0;
   ಆದರೆ (p.ಆಳ != 16) {
      stbi__rewind(p.s);
      ಹಿಂತಿರುಗಿ 0;
   }
   ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ

// Microsoft/Windows BMP image

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_BMP
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__bmp_test_raw(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ r;
   ಪೂರ್ಣಾಂಕ sz;
   ಆದರೆ (stbi__get8(s) != 'B') ಹಿಂತಿರುಗಿ 0;
   ಆದರೆ (stbi__get8(s) != 'M') ಹಿಂತಿರುಗಿ 0;
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   stbi__get32le(s); // discard data offset
   sz = stbi__get32le(s);
   r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
   ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__bmp_test(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ r = stbi__bmp_test_raw(s);
   stbi__rewind(s);
   ಹಿಂತಿರುಗಿ r;
}


// returns 0..31 for the highest set bit
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_ಮೇಲಿನ_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ z)
{
   ಪೂರ್ಣಾಂಕ n=0;
   ಆದರೆ (z == 0) ಹಿಂತಿರುಗಿ -1;
   ಆದರೆ (z >= 0x10000) n += 16, z >>= 16;
   ಆದರೆ (z >= 0x00100) n +=  8, z >>=  8;
   ಆದರೆ (z >= 0x00010) n +=  4, z >>=  4;
   ಆದರೆ (z >= 0x00004) n +=  2, z >>=  2;
   ಆದರೆ (z >= 0x00002) n +=  1, z >>=  1;
   ಹಿಂತಿರುಗಿ n;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__bitcount(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ a)
{
   a = (a & 0x55555555) + ((a >>  1) & 0x55555555); // max 2
   a = (a & 0x33333333) + ((a >>  2) & 0x33333333); // max 4
   a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
   a = (a + (a >> 8)); // max 16 per 8 bits
   a = (a + (a >> 16)); // max 32 per 8 bits
   ಹಿಂತಿರುಗಿ a & 0xff;
}

// extract an arbitrarily-aligned N-bit value (N=bits)
// from v, and then make it 8-bits long and fractionally
// extend it to full full range.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__shiftsigned(ಪೂರ್ಣಾಂಕ v, ಪೂರ್ಣಾಂಕ ಸರಿಸು, ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು)
{
   ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mul_ಕೋಷ್ಟಕ[9] = {
      0,
      0xff/*0b11111111*/, 0x55/*0b01010101*/, 0x49/*0b01001001*/, 0x11/*0b00010001*/,
      0x21/*0b00100001*/, 0x41/*0b01000001*/, 0x81/*0b10000001*/, 0x01/*0b00000001*/,
   };
   ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸರಿಸು_ಕೋಷ್ಟಕ[9] = {
      0, 0,0,1,0,2,4,6,0,
   };
   ಆದರೆ (ಸರಿಸು < 0)
      v <<= -ಸರಿಸು;
   ಇಲ್ಲದಿದ್ದರೆ
      v >>= ಸರಿಸು;
   STBI_ASSERT(v >= 0 && v < 256);
   v >>= (8-ಬಿಟ್ಟುಗಳು);
   STBI_ASSERT(ಬಿಟ್ಟುಗಳು >= 0 && ಬಿಟ್ಟುಗಳು <= 8);
   ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) ((ಚಿಹ್ನೆರಹಿತ) v * mul_ಕೋಷ್ಟಕ[ಬಿಟ್ಟುಗಳು]) >> ಸರಿಸು_ಕೋಷ್ಟಕ[ಬಿಟ್ಟುಗಳು];
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   ಪೂರ್ಣಾಂಕ bpp, ಸ್ಥಾನಾಂತರ, hsz;
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mr,mg,mb,ma, all_a;
} stbi_bmp_ದತ್ತಾಂಶ;

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_bmp_parse_ಶೀರ್ಷಿಕೆ(stbi__context *s, stbi_bmp_ದತ್ತಾಂಶ *info)
{
   ಪೂರ್ಣಾಂಕ hsz;
   ಆದರೆ (stbi__get8(s) != 'B' || stbi__get8(s) != 'M') ಹಿಂತಿರುಗಿ stbi__errpuc("not BMP", "Corrupt BMP");
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   info->ಸ್ಥಾನಾಂತರ = stbi__get32le(s);
   info->hsz = hsz = stbi__get32le(s);
   info->mr = info->mg = info->mb = info->ma = 0;

   ಆದರೆ (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) ಹಿಂತಿರುಗಿ stbi__errpuc("unknown BMP", "BMP type not supported: unknown");
   ಆದರೆ (hsz == 12) {
      s->img_x = stbi__get16le(s);
      s->img_y = stbi__get16le(s);
   } ಇಲ್ಲದಿದ್ದರೆ {
      s->img_x = stbi__get32le(s);
      s->img_y = stbi__get32le(s);
   }
   ಆದರೆ (stbi__get16le(s) != 1) ಹಿಂತಿರುಗಿ stbi__errpuc("bad BMP", "bad BMP");
   info->bpp = stbi__get16le(s);
   ಆದರೆ (hsz != 12) {
      ಪೂರ್ಣಾಂಕ compress = stbi__get32le(s);
      ಆದರೆ (compress == 1 || compress == 2) ಹಿಂತಿರುಗಿ stbi__errpuc("BMP RLE", "BMP type not supported: RLE");
      stbi__get32le(s); // discard sizeof
      stbi__get32le(s); // discard hres
      stbi__get32le(s); // discard vres
      stbi__get32le(s); // discard colorsused
      stbi__get32le(s); // discard max important
      ಆದರೆ (hsz == 40 || hsz == 56) {
         ಆದರೆ (hsz == 56) {
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
         }
         ಆದರೆ (info->bpp == 16 || info->bpp == 32) {
            ಆದರೆ (compress == 0) {
               ಆದರೆ (info->bpp == 32) {
                  info->mr = 0xffu << 16;
                  info->mg = 0xffu <<  8;
                  info->mb = 0xffu <<  0;
                  info->ma = 0xffu << 24;
                  info->all_a = 0; // if all_a is 0 at end, then we loaded alpha channel but it was all 0
               } ಇಲ್ಲದಿದ್ದರೆ {
                  info->mr = 31u << 10;
                  info->mg = 31u <<  5;
                  info->mb = 31u <<  0;
               }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (compress == 3) {
               info->mr = stbi__get32le(s);
               info->mg = stbi__get32le(s);
               info->mb = stbi__get32le(s);
               // not documented, but generated by photoshop and handled by mspaint
               ಆದರೆ (info->mr == info->mg && info->mg == info->mb) {
                  // ?!?!?
                  ಹಿಂತಿರುಗಿ stbi__errpuc("bad BMP", "bad BMP");
               }
            } ಇಲ್ಲದಿದ್ದರೆ
               ಹಿಂತಿರುಗಿ stbi__errpuc("bad BMP", "bad BMP");
         }
      } ಇಲ್ಲದಿದ್ದರೆ {
         ಪೂರ್ಣಾಂಕ i;
         ಆದರೆ (hsz != 108 && hsz != 124)
            ಹಿಂತಿರುಗಿ stbi__errpuc("bad BMP", "bad BMP");
         info->mr = stbi__get32le(s);
         info->mg = stbi__get32le(s);
         info->mb = stbi__get32le(s);
         info->ma = stbi__get32le(s);
         stbi__get32le(s); // discard color space
         ಗಾಗಿ (i=0; i < 12; ++i)
            stbi__get32le(s); // discard color space parameters
         ಆದರೆ (hsz == 124) {
            stbi__get32le(s); // discard rendering intent
            stbi__get32le(s); // discard offset of profile data
            stbi__get32le(s); // discard size of profile data
            stbi__get32le(s); // discard reserved
         }
      }
   }
   ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *) 1;
}


ಸ್ಥಿರ ಶೂನ್ಯ *stbi_bmp_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   stbi_uc *ಹೊರಹೋಗು;
   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mr=0,mg=0,mb=0,ma=0, all_a;
   stbi_uc pal[256][4];
   ಪೂರ್ಣಾಂಕ psize=0,i,j,ಅಗಲ;
   ಪೂರ್ಣಾಂಕ flip_vertically, pad, ಗುರಿ;
   stbi_bmp_ದತ್ತಾಂಶ info;
   STBI_NOTUSED(ri);

   info.all_a = 255;
   ಆದರೆ (stbi_bmp_parse_ಶೀರ್ಷಿಕೆ(s, &info) == ಖಾಲಿ)
      ಹಿಂತಿರುಗಿ ಖಾಲಿ; // error code already set

   flip_vertically = ((ಪೂರ್ಣಾಂಕ) s->img_y) > 0;
   s->img_y = ನಿರಪೇಕ್ಷ((ಪೂರ್ಣಾಂಕ) s->img_y);

   mr = info.mr;
   mg = info.mg;
   mb = info.mb;
   ma = info.ma;
   all_a = info.all_a;

   ಆದರೆ (info.hsz == 12) {
      ಆದರೆ (info.bpp < 24)
         psize = (info.ಸ್ಥಾನಾಂತರ - 14 - 24) / 3;
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಆದರೆ (info.bpp < 16)
         psize = (info.ಸ್ಥಾನಾಂತರ - 14 - info.hsz) >> 2;
   }

   s->img_n = ma ? 4 : 3;
   ಆದರೆ (req_comp && req_comp >= 3) // we can directly decode 3 or 4
      ಗುರಿ = req_comp;
   ಇಲ್ಲದಿದ್ದರೆ
      ಗುರಿ = s->img_n; // if they want monochrome, we'll post-convert

   // sanity-check size
   ಆದರೆ (!stbi__mad3sizes_valid(ಗುರಿ, s->img_x, s->img_y, 0))
      ಹಿಂತಿರುಗಿ stbi__errpuc("too large", "Corrupt BMP");

   ಹೊರಹೋಗು = (stbi_uc *) stbi__malloc_mad3(ಗುರಿ, s->img_x, s->img_y, 0);
   ಆದರೆ (!ಹೊರಹೋಗು) ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");
   ಆದರೆ (info.bpp < 16) {
      ಪೂರ್ಣಾಂಕ z=0;
      ಆದರೆ (psize == 0 || psize > 256) { STBI_FREE(ಹೊರಹೋಗು); ಹಿಂತಿರುಗಿ stbi__errpuc("invalid", "Corrupt BMP"); }
      ಗಾಗಿ (i=0; i < psize; ++i) {
         pal[i][2] = stbi__get8(s);
         pal[i][1] = stbi__get8(s);
         pal[i][0] = stbi__get8(s);
         ಆದರೆ (info.hsz != 12) stbi__get8(s);
         pal[i][3] = 255;
      }
      stbi__skip(s, info.ಸ್ಥಾನಾಂತರ - 14 - info.hsz - psize * (info.hsz == 12 ? 3 : 4));
      ಆದರೆ (info.bpp == 1) ಅಗಲ = (s->img_x + 7) >> 3;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info.bpp == 4) ಅಗಲ = (s->img_x + 1) >> 1;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info.bpp == 8) ಅಗಲ = s->img_x;
      ಇಲ್ಲದಿದ್ದರೆ { STBI_FREE(ಹೊರಹೋಗು); ಹಿಂತಿರುಗಿ stbi__errpuc("bad bpp", "Corrupt BMP"); }
      pad = (-ಅಗಲ)&3;
      ಆದರೆ (info.bpp == 1) {
         ಗಾಗಿ (j=0; j < (ಪೂರ್ಣಾಂಕ) s->img_y; ++j) {
            ಪೂರ್ಣಾಂಕ ಬಿಟ್_ಸ್ಥಾನಾಂತರ = 7, v = stbi__get8(s);
            ಗಾಗಿ (i=0; i < (ಪೂರ್ಣಾಂಕ) s->img_x; ++i) {
               ಪೂರ್ಣಾಂಕ color = (v>>ಬಿಟ್_ಸ್ಥಾನಾಂತರ)&0x1;
               ಹೊರಹೋಗು[z++] = pal[color][0];
               ಹೊರಹೋಗು[z++] = pal[color][1];
               ಹೊರಹೋಗು[z++] = pal[color][2];
               ಆದರೆ((--ಬಿಟ್_ಸ್ಥಾನಾಂತರ) < 0) {
                  ಬಿಟ್_ಸ್ಥಾನಾಂತರ = 7;
                  v = stbi__get8(s);
               }
            }
            stbi__skip(s, pad);
         }
      } ಇಲ್ಲದಿದ್ದರೆ {
         ಗಾಗಿ (j=0; j < (ಪೂರ್ಣಾಂಕ) s->img_y; ++j) {
            ಗಾಗಿ (i=0; i < (ಪೂರ್ಣಾಂಕ) s->img_x; i += 2) {
               ಪೂರ್ಣಾಂಕ v=stbi__get8(s),v2=0;
               ಆದರೆ (info.bpp == 4) {
                  v2 = v & 15;
                  v >>= 4;
               }
               ಹೊರಹೋಗು[z++] = pal[v][0];
               ಹೊರಹೋಗು[z++] = pal[v][1];
               ಹೊರಹೋಗು[z++] = pal[v][2];
               ಆದರೆ (ಗುರಿ == 4) ಹೊರಹೋಗು[z++] = 255;
               ಆದರೆ (i+1 == (ಪೂರ್ಣಾಂಕ) s->img_x) ನಿಲ್ಲಿಸು;
               v = (info.bpp == 8) ? stbi__get8(s) : v2;
               ಹೊರಹೋಗು[z++] = pal[v][0];
               ಹೊರಹೋಗು[z++] = pal[v][1];
               ಹೊರಹೋಗು[z++] = pal[v][2];
               ಆದರೆ (ಗುರಿ == 4) ಹೊರಹೋಗು[z++] = 255;
            }
            stbi__skip(s, pad);
         }
      }
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಪೂರ್ಣಾಂಕ rshift=0,gshift=0,bshift=0,ashift=0,rcount=0,gcount=0,bcount=0,acount=0;
      ಪೂರ್ಣಾಂಕ z = 0;
      ಪೂರ್ಣಾಂಕ easy=0;
      stbi__skip(s, info.ಸ್ಥಾನಾಂತರ - 14 - info.hsz);
      ಆದರೆ (info.bpp == 24) ಅಗಲ = 3 * s->img_x;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info.bpp == 16) ಅಗಲ = 2*s->img_x;
      ಇಲ್ಲದಿದ್ದರೆ /* bpp = 32 and pad = 0 */ ಅಗಲ=0;
      pad = (-ಅಗಲ) & 3;
      ಆದರೆ (info.bpp == 24) {
         easy = 1;
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info.bpp == 32) {
         ಆದರೆ (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
            easy = 2;
      }
      ಆದರೆ (!easy) {
         ಆದರೆ (!mr || !mg || !mb) { STBI_FREE(ಹೊರಹೋಗು); ಹಿಂತಿರುಗಿ stbi__errpuc("bad masks", "Corrupt BMP"); }
         // right shift amt to put high bit in position #7
         rshift = stbi_ಮೇಲಿನ_ಬಿಟ್(mr)-7; rcount = stbi__bitcount(mr);
         gshift = stbi_ಮೇಲಿನ_ಬಿಟ್(mg)-7; gcount = stbi__bitcount(mg);
         bshift = stbi_ಮೇಲಿನ_ಬಿಟ್(mb)-7; bcount = stbi__bitcount(mb);
         ashift = stbi_ಮೇಲಿನ_ಬಿಟ್(ma)-7; acount = stbi__bitcount(ma);
      }
      ಗಾಗಿ (j=0; j < (ಪೂರ್ಣಾಂಕ) s->img_y; ++j) {
         ಆದರೆ (easy) {
            ಗಾಗಿ (i=0; i < (ಪೂರ್ಣಾಂಕ) s->img_x; ++i) {
               ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ a;
               ಹೊರಹೋಗು[z+2] = stbi__get8(s);
               ಹೊರಹೋಗು[z+1] = stbi__get8(s);
               ಹೊರಹೋಗು[z+0] = stbi__get8(s);
               z += 3;
               a = (easy == 2 ? stbi__get8(s) : 255);
               all_a |= a;
               ಆದರೆ (ಗುರಿ == 4) ಹೊರಹೋಗು[z++] = a;
            }
         } ಇಲ್ಲದಿದ್ದರೆ {
            ಪೂರ್ಣಾಂಕ bpp = info.bpp;
            ಗಾಗಿ (i=0; i < (ಪೂರ್ಣಾಂಕ) s->img_x; ++i) {
               stbi__uint32 v = (bpp == 16 ? (stbi__uint32) stbi__get16le(s) : stbi__get32le(s));
               ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ a;
               ಹೊರಹೋಗು[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
               ಹೊರಹೋಗು[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
               ಹೊರಹೋಗು[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));
               a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
               all_a |= a;
               ಆದರೆ (ಗುರಿ == 4) ಹೊರಹೋಗು[z++] = STBI__BYTECAST(a);
            }
         }
         stbi__skip(s, pad);
      }
   }

   // if alpha channel is all 0s, replace with all 255s
   ಆದರೆ (ಗುರಿ == 4 && all_a == 0)
      ಗಾಗಿ (i=4*s->img_x*s->img_y-1; i >= 0; i -= 4)
         ಹೊರಹೋಗು[i] = 255;

   ಆದರೆ (flip_vertically) {
      stbi_uc t;
      ಗಾಗಿ (j=0; j < (ಪೂರ್ಣಾಂಕ) s->img_y>>1; ++j) {
         stbi_uc *p1 = ಹೊರಹೋಗು +      j     *s->img_x*ಗುರಿ;
         stbi_uc *p2 = ಹೊರಹೋಗು + (s->img_y-1-j)*s->img_x*ಗುರಿ;
         ಗಾಗಿ (i=0; i < (ಪೂರ್ಣಾಂಕ) s->img_x*ಗುರಿ; ++i) {
            t = p1[i], p1[i] = p2[i], p2[i] = t;
         }
      }
   }

   ಆದರೆ (req_comp && req_comp != ಗುರಿ) {
      ಹೊರಹೋಗು = stbi__convert_format(ಹೊರಹೋಗು, ಗುರಿ, req_comp, s->img_x, s->img_y);
      ಆದರೆ (ಹೊರಹೋಗು == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು; // stbi__convert_format frees input on failure
   }

   *x = s->img_x;
   *y = s->img_y;
   ಆದರೆ (comp) *comp = s->img_n;
   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}
#ಆದರೆ_ಅಂತ್ಯ

// Targa Truevision - TGA
// by Jonathan Dummer
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_TGA
// returns STBI_rgb or whatever, 0 on error
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__tga_get_comp(ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel, ಪೂರ್ಣಾಂಕ is_grey, ಪೂರ್ಣಾಂಕ* is_rgb16)
{
   // only RGB or RGBA (incl. 16bit) or grey allowed
   ಆದರೆ (is_rgb16) *is_rgb16 = 0;
   ಆಯ್ಕೆ(ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel) {
      ಸ್ಥಿತಿ 8:  ಹಿಂತಿರುಗಿ STBI_grey;
      ಸ್ಥಿತಿ 16: ಆದರೆ(is_grey) ಹಿಂತಿರುಗಿ STBI_grey_alpha;
               // fallthrough
      ಸ್ಥಿತಿ 15: ಆದರೆ(is_rgb16) *is_rgb16 = 1;
               ಹಿಂತಿರುಗಿ STBI_rgb;
      ಸ್ಥಿತಿ 24: // fallthrough
      ಸ್ಥಿತಿ 32: ಹಿಂತಿರುಗಿ ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel/8;
      ಪೂರ್ವನಿಯೋಜಿತ: ಹಿಂತಿರುಗಿ 0;
   }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__tga_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
    ಪೂರ್ಣಾಂಕ tga_w, tga_h, tga_comp, tga_image_type, tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel, tga_colormap_bpp;
    ಪೂರ್ಣಾಂಕ sz, tga_colormap_type;
    stbi__get8(s);                   // discard Offset
    tga_colormap_type = stbi__get8(s); // colormap type
    ಆದರೆ( tga_colormap_type > 1 ) {
        stbi__rewind(s);
        ಹಿಂತಿರುಗಿ 0;      // only RGB or indexed allowed
    }
    tga_image_type = stbi__get8(s); // image type
    ಆದರೆ ( tga_colormap_type == 1 ) { // colormapped (paletted) image
        ಆದರೆ (tga_image_type != 1 && tga_image_type != 9) {
            stbi__rewind(s);
            ಹಿಂತಿರುಗಿ 0;
        }
        stbi__skip(s,4);       // skip index of first colormap entry and number of entries
        sz = stbi__get8(s);    //   check bits per palette color entry
        ಆದರೆ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) {
            stbi__rewind(s);
            ಹಿಂತಿರುಗಿ 0;
        }
        stbi__skip(s,4);       // skip image x and y origin
        tga_colormap_bpp = sz;
    } ಇಲ್ಲದಿದ್ದರೆ { // "normal" image w/o colormap - only RGB or grey allowed, +/- RLE
        ಆದರೆ ( (tga_image_type != 2) && (tga_image_type != 3) && (tga_image_type != 10) && (tga_image_type != 11) ) {
            stbi__rewind(s);
            ಹಿಂತಿರುಗಿ 0; // only RGB or grey allowed, +/- RLE
        }
        stbi__skip(s,9); // skip colormap specification and image x/y origin
        tga_colormap_bpp = 0;
    }
    tga_w = stbi__get16le(s);
    ಆದರೆ( tga_w < 1 ) {
        stbi__rewind(s);
        ಹಿಂತಿರುಗಿ 0;   // test width
    }
    tga_h = stbi__get16le(s);
    ಆದರೆ( tga_h < 1 ) {
        stbi__rewind(s);
        ಹಿಂತಿರುಗಿ 0;   // test height
    }
    tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel = stbi__get8(s); // bits per pixel
    stbi__get8(s); // ignore alpha bits
    ಆದರೆ (tga_colormap_bpp != 0) {
        ಆದರೆ((tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel != 8) && (tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel != 16)) {
            // when using a colormap, tga_bits_per_pixel is the size of the indexes
            // I don't think anything but 8 or 16bit indexes makes sense
            stbi__rewind(s);
            ಹಿಂತಿರುಗಿ 0;
        }
        tga_comp = stbi__tga_get_comp(tga_colormap_bpp, 0, ಖಾಲಿ);
    } ಇಲ್ಲದಿದ್ದರೆ {
        tga_comp = stbi__tga_get_comp(tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel, (tga_image_type == 3) || (tga_image_type == 11), ಖಾಲಿ);
    }
    ಆದರೆ(!tga_comp) {
      stbi__rewind(s);
      ಹಿಂತಿರುಗಿ 0;
    }
    ಆದರೆ (x) *x = tga_w;
    ಆದರೆ (y) *y = tga_h;
    ಆದರೆ (comp) *comp = tga_comp;
    ಹಿಂತಿರುಗಿ 1;                   // seems to have passed everything
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__tga_test(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ res = 0;
   ಪೂರ್ಣಾಂಕ sz, tga_color_type;
   stbi__get8(s);      //   discard Offset
   tga_color_type = stbi__get8(s);   //   color type
   ಆದರೆ ( tga_color_type > 1 ) ಹೋಗು error_ಕೊನೆ;   //   only RGB or indexed allowed
   sz = stbi__get8(s);   //   image type
   ಆದರೆ ( tga_color_type == 1 ) { // colormapped (paletted) image
      ಆದರೆ (sz != 1 && sz != 9) ಹೋಗು error_ಕೊನೆ; // colortype 1 demands image type 1 or 9
      stbi__skip(s,4);       // skip index of first colormap entry and number of entries
      sz = stbi__get8(s);    //   check bits per palette color entry
      ಆದರೆ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) ಹೋಗು error_ಕೊನೆ;
      stbi__skip(s,4);       // skip image x and y origin
   } ಇಲ್ಲದಿದ್ದರೆ { // "normal" image w/o colormap
      ಆದರೆ ( (sz != 2) && (sz != 3) && (sz != 10) && (sz != 11) ) ಹೋಗು error_ಕೊನೆ; // only RGB or grey allowed, +/- RLE
      stbi__skip(s,9); // skip colormap specification and image x/y origin
   }
   ಆದರೆ ( stbi__get16le(s) < 1 ) ಹೋಗು error_ಕೊನೆ;      //   test width
   ಆದರೆ ( stbi__get16le(s) < 1 ) ಹೋಗು error_ಕೊನೆ;      //   test height
   sz = stbi__get8(s);   //   bits per pixel
   ಆದರೆ ( (tga_color_type == 1) && (sz != 8) && (sz != 16) ) ಹೋಗು error_ಕೊನೆ; // for colormapped images, bpp is size of an index
   ಆದರೆ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) ಹೋಗು error_ಕೊನೆ;

   res = 1; // if we got this far, everything's good and we can return 1 instead of 0

error_ಕೊನೆ:
   stbi__rewind(s);
   ಹಿಂತಿರುಗಿ res;
}

// read 16bit value and convert to 24bit RGB
ಸ್ಥಿರ ಶೂನ್ಯ stbi__tga_read_rgb16(stbi__context *s, stbi_uc* ಹೊರಹೋಗು)
{
   stbi__uint16 px = (stbi__uint16)stbi__get16le(s);
   stbi__uint16 five_ಬಿಟ್_ಮುಖವಾಡ = 31;
   // we have 3 channels with 5bits each
   ಪೂರ್ಣಾಂಕ r = (px >> 10) & five_ಬಿಟ್_ಮುಖವಾಡ;
   ಪೂರ್ಣಾಂಕ g = (px >> 5) & five_ಬಿಟ್_ಮುಖವಾಡ;
   ಪೂರ್ಣಾಂಕ b = px & five_ಬಿಟ್_ಮುಖವಾಡ;
   // Note that this saves the data in RGB(A) order, so it doesn't need to be swapped later
   ಹೊರಹೋಗು[0] = (stbi_uc)((r * 255)/31);
   ಹೊರಹೋಗು[1] = (stbi_uc)((g * 255)/31);
   ಹೊರಹೋಗು[2] = (stbi_uc)((b * 255)/31);

   // some people claim that the most significant bit might be used for alpha
   // (possibly if an alpha-bit is set in the "image descriptor byte")
   // but that only made 16bit test images completely translucent..
   // so let's treat all 15 and 16bit TGAs as RGB with no alpha.
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_tga_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   //   read in the TGA header stuff
   ಪೂರ್ಣಾಂಕ tga_ಸ್ಥಾನಾಂತರ = stbi__get8(s);
   ಪೂರ್ಣಾಂಕ tga_ಸೂಚ್ಯಂಕಿತ = stbi__get8(s);
   ಪೂರ್ಣಾಂಕ tga_image_type = stbi__get8(s);
   ಪೂರ್ಣಾಂಕ tga_is_RLE = 0;
   ಪೂರ್ಣಾಂಕ tga_palette_ಆರಂಭ = stbi__get16le(s);
   ಪೂರ್ಣಾಂಕ tga_palette_len = stbi__get16le(s);
   ಪೂರ್ಣಾಂಕ tga_palette_ಬಿಟ್ಟುಗಳು = stbi__get8(s);
   ಪೂರ್ಣಾಂಕ tga_x_origin = stbi__get16le(s);
   ಪೂರ್ಣಾಂಕ tga_y_origin = stbi__get16le(s);
   ಪೂರ್ಣಾಂಕ tga_ಅಗಲ = stbi__get16le(s);
   ಪೂರ್ಣಾಂಕ tga_ಎತ್ತರ = stbi__get16le(s);
   ಪೂರ್ಣಾಂಕ tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel = stbi__get8(s);
   ಪೂರ್ಣಾಂಕ tga_comp, tga_rgb16=0;
   ಪೂರ್ಣಾಂಕ tga_inverted = stbi__get8(s);
   // int tga_alpha_bits = tga_inverted & 15; // the 4 lowest bits - unused (useless?)
   //   image data
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *tga_ದತ್ತಾಂಶ;
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *tga_palette = ಖಾಲಿ;
   ಪೂರ್ಣಾಂಕ i, j;
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ raw_ದತ್ತಾಂಶ[4] = {0};
   ಪೂರ್ಣಾಂಕ RLE_ಎಣಿಕೆ = 0;
   ಪೂರ್ಣಾಂಕ RLE_repeating = 0;
   ಪೂರ್ಣಾಂಕ read_ಮುಂದಿನ_pixel = 1;
   STBI_NOTUSED(ri);

   //   do a tiny bit of precessing
   ಆದರೆ ( tga_image_type >= 8 )
   {
      tga_image_type -= 8;
      tga_is_RLE = 1;
   }
   tga_inverted = 1 - ((tga_inverted >> 5) & 1);

   //   If I'm paletted, then I'll use the number of bits from the palette
   ಆದರೆ ( tga_ಸೂಚ್ಯಂಕಿತ ) tga_comp = stbi__tga_get_comp(tga_palette_ಬಿಟ್ಟುಗಳು, 0, &tga_rgb16);
   ಇಲ್ಲದಿದ್ದರೆ tga_comp = stbi__tga_get_comp(tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel, (tga_image_type == 3), &tga_rgb16);

   ಆದರೆ(!tga_comp) // shouldn't really happen, stbi__tga_test() should have ensured basic consistency
      ಹಿಂತಿರುಗಿ stbi__errpuc("bad format", "Can't find out TGA pixelformat");

   //   tga info
   *x = tga_ಅಗಲ;
   *y = tga_ಎತ್ತರ;
   ಆದರೆ (comp) *comp = tga_comp;

   ಆದರೆ (!stbi__mad3sizes_valid(tga_ಅಗಲ, tga_ಎತ್ತರ, tga_comp, 0))
      ಹಿಂತಿರುಗಿ stbi__errpuc("too large", "Corrupt TGA");

   tga_ದತ್ತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)stbi__malloc_mad3(tga_ಅಗಲ, tga_ಎತ್ತರ, tga_comp, 0);
   ಆದರೆ (!tga_ದತ್ತಾಂಶ) ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");

   // skip to the data's starting position (offset usually = 0)
   stbi__skip(s, tga_ಸ್ಥಾನಾಂತರ );

   ಆದರೆ ( !tga_ಸೂಚ್ಯಂಕಿತ && !tga_is_RLE && !tga_rgb16 ) {
      ಗಾಗಿ (i=0; i < tga_ಎತ್ತರ; ++i) {
         ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = tga_inverted ? tga_ಎತ್ತರ -i - 1 : i;
         stbi_uc *tga_ಅಡ್ಡಸಾಲು = tga_ದತ್ತಾಂಶ + ಅಡ್ಡಸಾಲು*tga_ಅಗಲ*tga_comp;
         stbi__getn(s, tga_ಅಡ್ಡಸಾಲು, tga_ಅಗಲ * tga_comp);
      }
   } ಇಲ್ಲದಿದ್ದರೆ  {
      //   do I need to load a palette?
      ಆದರೆ ( tga_ಸೂಚ್ಯಂಕಿತ)
      {
         //   any data to skip? (offset usually = 0)
         stbi__skip(s, tga_palette_ಆರಂಭ );
         //   load the palette
         tga_palette = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)stbi__malloc_mad2(tga_palette_len, tga_comp, 0);
         ಆದರೆ (!tga_palette) {
            STBI_FREE(tga_ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");
         }
         ಆದರೆ (tga_rgb16) {
            stbi_uc *pal_ನಮೂದು = tga_palette;
            STBI_ASSERT(tga_comp == STBI_rgb);
            ಗಾಗಿ (i=0; i < tga_palette_len; ++i) {
               stbi__tga_read_rgb16(s, pal_ನಮೂದು);
               pal_ನಮೂದು += tga_comp;
            }
         } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!stbi__getn(s, tga_palette, tga_palette_len * tga_comp)) {
               STBI_FREE(tga_ದತ್ತಾಂಶ);
               STBI_FREE(tga_palette);
               ಹಿಂತಿರುಗಿ stbi__errpuc("bad palette", "Corrupt TGA");
         }
      }
      //   load the data
      ಗಾಗಿ (i=0; i < tga_ಅಗಲ * tga_ಎತ್ತರ; ++i)
      {
         //   if I'm in RLE mode, do I need to get a RLE stbi__pngchunk?
         ಆದರೆ ( tga_is_RLE )
         {
            ಆದರೆ ( RLE_ಎಣಿಕೆ == 0 )
            {
               //   yep, get the next byte as a RLE command
               ಪೂರ್ಣಾಂಕ RLE_cmd = stbi__get8(s);
               RLE_ಎಣಿಕೆ = 1 + (RLE_cmd & 127);
               RLE_repeating = RLE_cmd >> 7;
               read_ಮುಂದಿನ_pixel = 1;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !RLE_repeating )
            {
               read_ಮುಂದಿನ_pixel = 1;
            }
         } ಇಲ್ಲದಿದ್ದರೆ
         {
            read_ಮುಂದಿನ_pixel = 1;
         }
         //   OK, if I need to read a pixel, do it now
         ಆದರೆ ( read_ಮುಂದಿನ_pixel )
         {
            //   load however much data we did have
            ಆದರೆ ( tga_ಸೂಚ್ಯಂಕಿತ )
            {
               // read in index, then perform the lookup
               ಪೂರ್ಣಾಂಕ pal_idx = (tga_ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_pixel == 8) ? stbi__get8(s) : stbi__get16le(s);
               ಆದರೆ ( pal_idx >= tga_palette_len ) {
                  // invalid index
                  pal_idx = 0;
               }
               pal_idx *= tga_comp;
               ಗಾಗಿ (j = 0; j < tga_comp; ++j) {
                  raw_ದತ್ತಾಂಶ[j] = tga_palette[pal_idx+j];
               }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(tga_rgb16) {
               STBI_ASSERT(tga_comp == STBI_rgb);
               stbi__tga_read_rgb16(s, raw_ದತ್ತಾಂಶ);
            } ಇಲ್ಲದಿದ್ದರೆ {
               //   read in the data raw
               ಗಾಗಿ (j = 0; j < tga_comp; ++j) {
                  raw_ದತ್ತಾಂಶ[j] = stbi__get8(s);
               }
            }
            //   clear the reading flag for the next pixel
            read_ಮುಂದಿನ_pixel = 0;
         } // end of reading a pixel

         // copy data
         ಗಾಗಿ (j = 0; j < tga_comp; ++j)
           tga_ದತ್ತಾಂಶ[i*tga_comp+j] = raw_ದತ್ತಾಂಶ[j];

         //   in case we're in RLE mode, keep counting down
         --RLE_ಎಣಿಕೆ;
      }
      //   do I need to invert the image?
      ಆದರೆ ( tga_inverted )
      {
         ಗಾಗಿ (j = 0; j*2 < tga_ಎತ್ತರ; ++j)
         {
            ಪೂರ್ಣಾಂಕ index1 = j * tga_ಅಗಲ * tga_comp;
            ಪೂರ್ಣಾಂಕ index2 = (tga_ಎತ್ತರ - 1 - j) * tga_ಅಗಲ * tga_comp;
            ಗಾಗಿ (i = tga_ಅಗಲ * tga_comp; i > 0; --i)
            {
               ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ತಾತ್ಕಾಲಿಕ = tga_ದತ್ತಾಂಶ[index1];
               tga_ದತ್ತಾಂಶ[index1] = tga_ದತ್ತಾಂಶ[index2];
               tga_ದತ್ತಾಂಶ[index2] = ತಾತ್ಕಾಲಿಕ;
               ++index1;
               ++index2;
            }
         }
      }
      //   clear my palette, if I had one
      ಆದರೆ ( tga_palette != ಖಾಲಿ )
      {
         STBI_FREE( tga_palette );
      }
   }

   // swap RGB - if the source data was RGB16, it already is in the right order
   ಆದರೆ (tga_comp >= 3 && !tga_rgb16)
   {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* tga_pixel = tga_ದತ್ತಾಂಶ;
      ಗಾಗಿ (i=0; i < tga_ಅಗಲ * tga_ಎತ್ತರ; ++i)
      {
         ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ತಾತ್ಕಾಲಿಕ = tga_pixel[0];
         tga_pixel[0] = tga_pixel[2];
         tga_pixel[2] = ತಾತ್ಕಾಲಿಕ;
         tga_pixel += tga_comp;
      }
   }

   // convert to target component count
   ಆದರೆ (req_comp && req_comp != tga_comp)
      tga_ದತ್ತಾಂಶ = stbi__convert_format(tga_ದತ್ತಾಂಶ, tga_comp, req_comp, tga_ಅಗಲ, tga_ಎತ್ತರ);

   //   the things I do to get rid of an error message, and yet keep
   //   Microsoft's C compilers happy... [8^(
   tga_palette_ಆರಂಭ = tga_palette_len = tga_palette_ಬಿಟ್ಟುಗಳು =
         tga_x_origin = tga_y_origin = 0;
   //   OK, done
   ಹಿಂತಿರುಗಿ tga_ದತ್ತಾಂಶ;
}
#ಆದರೆ_ಅಂತ್ಯ

// *************************************************************************************************
// Photoshop PSD loader -- PD by Thatcher Ulrich, integration by Nicolas Schulz, tweaked by STB

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PSD
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__psd_test(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ r = (stbi__get32be(s) == 0x38425053);
   stbi__rewind(s);
   ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__psd_decode_rle(stbi__context *s, stbi_uc *p, ಪೂರ್ಣಾಂಕ pixel_ಎಣಿಕೆ_2)
{
   ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, nleft, len;

   ಎಣಿಕೆ_2 = 0;
   ಆಗ ((nleft = pixel_ಎಣಿಕೆ_2 - ಎಣಿಕೆ_2) > 0) {
      len = stbi__get8(s);
      ಆದರೆ (len == 128) {
         // No-op.
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len < 128) {
         // Copy next len+1 bytes literally.
         len++;
         ಆದರೆ (len > nleft) ಹಿಂತಿರುಗಿ 0; // corrupt data
         ಎಣಿಕೆ_2 += len;
         ಆಗ (len) {
            *p = stbi__get8(s);
            p += 4;
            len--;
         }
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len > 128) {
         stbi_uc   val;
         // Next -len+1 bytes in the dest are replicated from next source byte.
         // (Interpret len as a negative 8-bit int.)
         len = 257 - len;
         ಆದರೆ (len > nleft) ಹಿಂತಿರುಗಿ 0; // corrupt data
         val = stbi__get8(s);
         ಎಣಿಕೆ_2 += len;
         ಆಗ (len) {
            *p = val;
            p += 4;
            len--;
         }
      }
   }

   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_psd_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri, ಪೂರ್ಣಾಂಕ bpc)
{
   ಪೂರ್ಣಾಂಕ pixel_ಎಣಿಕೆ_2;
   ಪೂರ್ಣಾಂಕ channel_ಎಣಿಕೆ, compression;
   ಪೂರ್ಣಾಂಕ channel, i;
   ಪೂರ್ಣಾಂಕ bitdepth;
   ಪೂರ್ಣಾಂಕ w,h;
   stbi_uc *ಹೊರಹೋಗು;
   STBI_NOTUSED(ri);

   // Check identifier
   ಆದರೆ (stbi__get32be(s) != 0x38425053)   // "8BPS"
      ಹಿಂತಿರುಗಿ stbi__errpuc("not PSD", "Corrupt PSD image");

   // Check file type version.
   ಆದರೆ (stbi__get16be(s) != 1)
      ಹಿಂತಿರುಗಿ stbi__errpuc("wrong version", "Unsupported version of PSD image");

   // Skip 6 reserved bytes.
   stbi__skip(s, 6 );

   // Read the number of channels (R, G, B, A, etc).
   channel_ಎಣಿಕೆ = stbi__get16be(s);
   ಆದರೆ (channel_ಎಣಿಕೆ < 0 || channel_ಎಣಿಕೆ > 16)
      ಹಿಂತಿರುಗಿ stbi__errpuc("wrong channel count", "Unsupported number of channels in PSD image");

   // Read the rows and columns of the image.
   h = stbi__get32be(s);
   w = stbi__get32be(s);

   // Make sure the depth is 8 bits.
   bitdepth = stbi__get16be(s);
   ಆದರೆ (bitdepth != 8 && bitdepth != 16)
      ಹಿಂತಿರುಗಿ stbi__errpuc("unsupported bit depth", "PSD bit depth is not 8 or 16 bit");

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
   ಆದರೆ (stbi__get16be(s) != 3)
      ಹಿಂತಿರುಗಿ stbi__errpuc("wrong color format", "PSD is not in RGB color format");

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
   ಆದರೆ (compression > 1)
      ಹಿಂತಿರುಗಿ stbi__errpuc("bad compression", "PSD has an unknown compression format");

   // Check size
   ಆದರೆ (!stbi__mad3sizes_valid(4, w, h, 0))
      ಹಿಂತಿರುಗಿ stbi__errpuc("too large", "Corrupt PSD");

   // Create the destination image.

   ಆದರೆ (!compression && bitdepth == 16 && bpc == 16) {
      ಹೊರಹೋಗು = (stbi_uc *) stbi__malloc_mad3(8, w, h, 0);
      ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel = 16;
   } ಇಲ್ಲದಿದ್ದರೆ
      ಹೊರಹೋಗು = (stbi_uc *) stbi__malloc(4 * w*h);

   ಆದರೆ (!ಹೊರಹೋಗು) ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");
   pixel_ಎಣಿಕೆ_2 = w*h;

   // Initialize the data to zero.
   //memset( out, 0, pixelCount * 4 );

   // Finally, the image data.
   ಆದರೆ (compression) {
      // RLE as used by .PSD and .TIFF
      // Loop until you get the number of unpacked bytes you are expecting:
      //     Read the next source byte into n.
      //     If n is between 0 and 127 inclusive, copy the next n+1 bytes literally.
      //     Else if n is between -127 and -1 inclusive, copy the next byte -n+1 times.
      //     Else if n is 128, noop.
      // Endloop

      // The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
      // which we're going to just skip.
      stbi__skip(s, h * channel_ಎಣಿಕೆ * 2 );

      // Read the RLE data by channel.
      ಗಾಗಿ (channel = 0; channel < 4; channel++) {
         stbi_uc *p;

         p = ಹೊರಹೋಗು+channel;
         ಆದರೆ (channel >= channel_ಎಣಿಕೆ) {
            // Fill this channel with default data.
            ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ_2; i++, p += 4)
               *p = (channel == 3 ? 255 : 0);
         } ಇಲ್ಲದಿದ್ದರೆ {
            // Read the RLE data.
            ಆದರೆ (!stbi__psd_decode_rle(s, p, pixel_ಎಣಿಕೆ_2)) {
               STBI_FREE(ಹೊರಹೋಗು);
               ಹಿಂತಿರುಗಿ stbi__errpuc("corrupt", "bad RLE data");
            }
         }
      }

   } ಇಲ್ಲದಿದ್ದರೆ {
      // We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
      // where each channel consists of an 8-bit (or 16-bit) value for each pixel in the image.

      // Read the data by channel.
      ಗಾಗಿ (channel = 0; channel < 4; channel++) {
         ಆದರೆ (channel >= channel_ಎಣಿಕೆ) {
            // Fill this channel with default data.
            ಆದರೆ (bitdepth == 16 && bpc == 16) {
               stbi__uint16 *q = ((stbi__uint16 *) ಹೊರಹೋಗು) + channel;
               stbi__uint16 val = channel == 3 ? 65535 : 0;
               ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ_2; i++, q += 4)
                  *q = val;
            } ಇಲ್ಲದಿದ್ದರೆ {
               stbi_uc *p = ಹೊರಹೋಗು+channel;
               stbi_uc val = channel == 3 ? 255 : 0;
               ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ_2; i++, p += 4)
                  *p = val;
            }
         } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel == 16) {    // output bpc
               stbi__uint16 *q = ((stbi__uint16 *) ಹೊರಹೋಗು) + channel;
               ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ_2; i++, q += 4)
                  *q = (stbi__uint16) stbi__get16be(s);
            } ಇಲ್ಲದಿದ್ದರೆ {
               stbi_uc *p = ಹೊರಹೋಗು+channel;
               ಆದರೆ (bitdepth == 16) {  // input bpc
                  ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ_2; i++, p += 4)
                     *p = (stbi_uc) (stbi__get16be(s) >> 8);
               } ಇಲ್ಲದಿದ್ದರೆ {
                  ಗಾಗಿ (i = 0; i < pixel_ಎಣಿಕೆ_2; i++, p += 4)
                     *p = stbi__get8(s);
               }
            }
         }
      }
   }

   // remove weird white matte from PSD
   ಆದರೆ (channel_ಎಣಿಕೆ >= 4) {
      ಆದರೆ (ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel == 16) {
         ಗಾಗಿ (i=0; i < w*h; ++i) {
            stbi__uint16 *pixel = (stbi__uint16 *) ಹೊರಹೋಗು + 4*i;
            ಆದರೆ (pixel[3] != 0 && pixel[3] != 65535) {
               ದಶಮಾಂಶ a = pixel[3] / 65535.0f;
               ದಶಮಾಂಶ ra = 1.0f / a;
               ದಶಮಾಂಶ inv_a = 65535.0f * (1 - ra);
               pixel[0] = (stbi__uint16) (pixel[0]*ra + inv_a);
               pixel[1] = (stbi__uint16) (pixel[1]*ra + inv_a);
               pixel[2] = (stbi__uint16) (pixel[2]*ra + inv_a);
            }
         }
      } ಇಲ್ಲದಿದ್ದರೆ {
         ಗಾಗಿ (i=0; i < w*h; ++i) {
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *pixel = ಹೊರಹೋಗು + 4*i;
            ಆದರೆ (pixel[3] != 0 && pixel[3] != 255) {
               ದಶಮಾಂಶ a = pixel[3] / 255.0f;
               ದಶಮಾಂಶ ra = 1.0f / a;
               ದಶಮಾಂಶ inv_a = 255.0f * (1 - ra);
               pixel[0] = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) (pixel[0]*ra + inv_a);
               pixel[1] = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) (pixel[1]*ra + inv_a);
               pixel[2] = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) (pixel[2]*ra + inv_a);
            }
         }
      }
   }

   // convert to desired output format
   ಆದರೆ (req_comp && req_comp != 4) {
      ಆದರೆ (ri->ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_channel == 16)
         ಹೊರಹೋಗು = (stbi_uc *) stbi__convert_format16((stbi__uint16 *) ಹೊರಹೋಗು, 4, req_comp, w, h);
      ಇಲ್ಲದಿದ್ದರೆ
         ಹೊರಹೋಗು = stbi__convert_format(ಹೊರಹೋಗು, 4, req_comp, w, h);
      ಆದರೆ (ಹೊರಹೋಗು == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು; // stbi__convert_format frees input on failure
   }

   ಆದರೆ (comp) *comp = 4;
   *y = h;
   *x = w;

   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}
#ಆದರೆ_ಅಂತ್ಯ

// *************************************************************************************************
// Softimage PIC loader
// by Tom Seddon
//
// See http://softimage.wiki.softimage.com/index.php/INFO:_PIC_file_format
// See http://ozviz.wasp.uwa.edu.au/~pbourke/dataformats/softimagepic/

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PIC
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__pic_is4(stbi__context *s,ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; i<4; ++i)
      ಆದರೆ (stbi__get8(s) != (stbi_uc)str[i])
         ಹಿಂತಿರುಗಿ 0;

   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__pic_test_core(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ i;

   ಆದರೆ (!stbi__pic_is4(s,"\x53\x80\xF6\x34"))
      ಹಿಂತಿರುಗಿ 0;

   ಗಾಗಿ(i=0;i<84;++i)
      stbi__get8(s);

   ಆದರೆ (!stbi__pic_is4(s,"PICT"))
      ಹಿಂತಿರುಗಿ 0;

   ಹಿಂತಿರುಗಿ 1;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi_uc ಗಾತ್ರ_2,type,channel;
} stbi__pic_packet;

ಸ್ಥಿರ stbi_uc *stbi__readval(stbi__context *s, ಪೂರ್ಣಾಂಕ channel, stbi_uc *dest)
{
   ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ=0x80, i;

   ಗಾಗಿ (i=0; i<4; ++i, ಮುಖವಾಡ>>=1) {
      ಆದರೆ (channel & ಮುಖವಾಡ) {
         ಆದರೆ (stbi__at_eof(s)) ಹಿಂತಿರುಗಿ stbi__errpuc("bad file","PIC file too short");
         dest[i]=stbi__get8(s);
      }
   }

   ಹಿಂತಿರುಗಿ dest;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__copyval(ಪೂರ್ಣಾಂಕ channel,stbi_uc *dest,ಸ್ಥಿರಾಂಕ stbi_uc *src)
{
   ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ=0x80,i;

   ಗಾಗಿ (i=0;i<4; ++i, ಮುಖವಾಡ>>=1)
      ಆದರೆ (channel&ಮುಖವಾಡ)
         dest[i]=src[i];
}

ಸ್ಥಿರ stbi_uc *stbi_pic_ತುಂಬು_core(stbi__context *s,ಪೂರ್ಣಾಂಕ ಅಗಲ,ಪೂರ್ಣಾಂಕ ಎತ್ತರ,ಪೂರ್ಣಾಂಕ *comp, stbi_uc *ಫಲಿತಾಂಶ)
{
   ಪೂರ್ಣಾಂಕ act_comp=0,ಅಂಕೆ_packets=0,y,chained;
   stbi__pic_packet packets[10];

   // this will (should...) cater for even some bizarre stuff like having data
    // for the same channel in multiple packets.
   ಮಾಡು {
      stbi__pic_packet *packet;

      ಆದರೆ (ಅಂಕೆ_packets==ಗಾತ್ರ(packets)/ಗಾತ್ರ(packets[0]))
         ಹಿಂತಿರುಗಿ stbi__errpuc("bad format","too many packets");

      packet = &packets[ಅಂಕೆ_packets++];

      chained = stbi__get8(s);
      packet->ಗಾತ್ರ_2    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);

      act_comp |= packet->channel;

      ಆದರೆ (stbi__at_eof(s))          ಹಿಂತಿರುಗಿ stbi__errpuc("bad file","file too short (reading packets)");
      ಆದರೆ (packet->ಗಾತ್ರ_2 != 8)  ಹಿಂತಿರುಗಿ stbi__errpuc("bad format","packet isn't 8bpp");
   } ಆಗ (chained);

   *comp = (act_comp & 0x10 ? 4 : 3); // has alpha channel?

   ಗಾಗಿ(y=0; y<ಎತ್ತರ; ++y) {
      ಪೂರ್ಣಾಂಕ packet_idx;

      ಗಾಗಿ(packet_idx=0; packet_idx < ಅಂಕೆ_packets; ++packet_idx) {
         stbi__pic_packet *packet = &packets[packet_idx];
         stbi_uc *dest = ಫಲಿತಾಂಶ+y*ಅಗಲ*4;

         ಆಯ್ಕೆ (packet->type) {
            ಪೂರ್ವನಿಯೋಜಿತ:
               ಹಿಂತಿರುಗಿ stbi__errpuc("bad format","packet has bad compression type");

            ಸ್ಥಿತಿ 0: {//uncompressed
               ಪೂರ್ಣಾಂಕ x;

               ಗಾಗಿ(x=0;x<ಅಗಲ;++x, dest+=4)
                  ಆದರೆ (!stbi__readval(s,packet->channel,dest))
                     ಹಿಂತಿರುಗಿ 0;
               ನಿಲ್ಲಿಸು;
            }

            ಸ್ಥಿತಿ 1://Pure RLE
               {
                  ಪೂರ್ಣಾಂಕ ಎಡ=ಅಗಲ, i;

                  ಆಗ (ಎಡ>0) {
                     stbi_uc ಎಣಿಕೆ_2,ಮೌಲ್ಯ[4];

                     ಎಣಿಕೆ_2=stbi__get8(s);
                     ಆದರೆ (stbi__at_eof(s))   ಹಿಂತಿರುಗಿ stbi__errpuc("bad file","file too short (pure read count)");

                     ಆದರೆ (ಎಣಿಕೆ_2 > ಎಡ)
                        ಎಣಿಕೆ_2 = (stbi_uc) ಎಡ;

                     ಆದರೆ (!stbi__readval(s,packet->channel,ಮೌಲ್ಯ))  ಹಿಂತಿರುಗಿ 0;

                     ಗಾಗಿ(i=0; i<ಎಣಿಕೆ_2; ++i,dest+=4)
                        stbi__copyval(packet->channel,dest,ಮೌಲ್ಯ);
                     ಎಡ -= ಎಣಿಕೆ_2;
                  }
               }
               ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 2: {//Mixed RLE
               ಪೂರ್ಣಾಂಕ ಎಡ=ಅಗಲ;
               ಆಗ (ಎಡ>0) {
                  ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = stbi__get8(s), i;
                  ಆದರೆ (stbi__at_eof(s))  ಹಿಂತಿರುಗಿ stbi__errpuc("bad file","file too short (mixed read count)");

                  ಆದರೆ (ಎಣಿಕೆ_2 >= 128) { // Repeated
                     stbi_uc ಮೌಲ್ಯ[4];

                     ಆದರೆ (ಎಣಿಕೆ_2==128)
                        ಎಣಿಕೆ_2 = stbi__get16be(s);
                     ಇಲ್ಲದಿದ್ದರೆ
                        ಎಣಿಕೆ_2 -= 127;
                     ಆದರೆ (ಎಣಿಕೆ_2 > ಎಡ)
                        ಹಿಂತಿರುಗಿ stbi__errpuc("bad file","scanline overrun");

                     ಆದರೆ (!stbi__readval(s,packet->channel,ಮೌಲ್ಯ))
                        ಹಿಂತಿರುಗಿ 0;

                     ಗಾಗಿ(i=0;i<ಎಣಿಕೆ_2;++i, dest += 4)
                        stbi__copyval(packet->channel,dest,ಮೌಲ್ಯ);
                  } ಇಲ್ಲದಿದ್ದರೆ { // Raw
                     ++ಎಣಿಕೆ_2;
                     ಆದರೆ (ಎಣಿಕೆ_2>ಎಡ) ಹಿಂತಿರುಗಿ stbi__errpuc("bad file","scanline overrun");

                     ಗಾಗಿ(i=0;i<ಎಣಿಕೆ_2;++i, dest+=4)
                        ಆದರೆ (!stbi__readval(s,packet->channel,dest))
                           ಹಿಂತಿರುಗಿ 0;
                  }
                  ಎಡ-=ಎಣಿಕೆ_2;
               }
               ನಿಲ್ಲಿಸು;
            }
         }
      }
   }

   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_pic_ತುಂಬು(stbi__context *s,ಪೂರ್ಣಾಂಕ *px,ಪೂರ್ಣಾಂಕ *py,ಪೂರ್ಣಾಂಕ *comp,ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   stbi_uc *ಫಲಿತಾಂಶ;
   ಪೂರ್ಣಾಂಕ i, x,y, ಆಂತರಿಕ_comp;
   STBI_NOTUSED(ri);

   ಆದರೆ (!comp) comp = &ಆಂತರಿಕ_comp;

   ಗಾಗಿ (i=0; i<92; ++i)
      stbi__get8(s);

   x = stbi__get16be(s);
   y = stbi__get16be(s);
   ಆದರೆ (stbi__at_eof(s))  ಹಿಂತಿರುಗಿ stbi__errpuc("bad file","file too short (pic header)");
   ಆದರೆ (!stbi__mad3sizes_valid(x, y, 4, 0)) ಹಿಂತಿರುಗಿ stbi__errpuc("too large", "PIC image too large to decode");

   stbi__get32be(s); //skip `ratio'
   stbi__get16be(s); //skip `fields'
   stbi__get16be(s); //skip `pad'

   // intermediate buffer is RGBA
   ಫಲಿತಾಂಶ = (stbi_uc *) stbi__malloc_mad3(x, y, 4, 0);
   memset(ಫಲಿತಾಂಶ, 0xff, x*y*4);

   ಆದರೆ (!stbi_pic_ತುಂಬು_core(s,x,y,comp, ಫಲಿತಾಂಶ)) {
      STBI_FREE(ಫಲಿತಾಂಶ);
      ಫಲಿತಾಂಶ=0;
   }
   *px = x;
   *py = y;
   ಆದರೆ (req_comp == 0) req_comp = *comp;
   ಫಲಿತಾಂಶ=stbi__convert_format(ಫಲಿತಾಂಶ,4,req_comp,x,y);

   ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__pic_test(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ r = stbi__pic_test_core(s);
   stbi__rewind(s);
   ಹಿಂತಿರುಗಿ r;
}
#ಆದರೆ_ಅಂತ್ಯ

// *************************************************************************************************
// GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_GIF
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   stbi__int16 ಪೂರ್ವಪ್ರತ್ಯಯ;
   stbi_uc ಮೊದಲ;
   stbi_uc suffix;
} stbi__gif_lzw;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
   ಪೂರ್ಣಾಂಕ w,h;
   stbi_uc *ಹೊರಹೋಗು;                 // output buffer (always 4 components)
   stbi_uc *background;          // The current "background" as far as a gif is concerned
   stbi_uc *history; 
   ಪೂರ್ಣಾಂಕ flags, bgindex, ಅನುಪಾತ, transparent, eflags;
   stbi_uc  pal[256][4];
   stbi_uc lpal[256][4];
   stbi__gif_lzw codes[8192];
   stbi_uc *color_ಕೋಷ್ಟಕ;
   ಪೂರ್ಣಾಂಕ parse, ಹೆಜ್ಜೆ;
   ಪೂರ್ಣಾಂಕ lflags;
   ಪೂರ್ಣಾಂಕ ಆರಂಭ_x, ಆರಂಭ_y;
   ಪೂರ್ಣಾಂಕ max_x, max_y;
   ಪೂರ್ಣಾಂಕ cur_x, cur_y;
   ಪೂರ್ಣಾಂಕ ಸಾಲು_ಗಾತ್ರ;
   ಪೂರ್ಣಾಂಕ delay;
} stbi__gif;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__gif_test_raw(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ sz;
   ಆದರೆ (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8') ಹಿಂತಿರುಗಿ 0;
   sz = stbi__get8(s);
   ಆದರೆ (sz != '9' && sz != '7') ಹಿಂತಿರುಗಿ 0;
   ಆದರೆ (stbi__get8(s) != 'a') ಹಿಂತಿರುಗಿ 0;
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__gif_test(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ r = stbi__gif_test_raw(s);
   stbi__rewind(s);
   ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__gif_parse_colortable(stbi__context *s, stbi_uc pal[256][4], ಪೂರ್ಣಾಂಕ ಅಂಕೆ_entries, ಪೂರ್ಣಾಂಕ transp)
{
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; i < ಅಂಕೆ_entries; ++i) {
      pal[i][2] = stbi__get8(s);
      pal[i][1] = stbi__get8(s);
      pal[i][0] = stbi__get8(s);
      pal[i][3] = transp == i ? 0 : 255;
   }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi_gif_ಶೀರ್ಷಿಕೆ(stbi__context *s, stbi__gif *g, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ is_info)
{
   stbi_uc ಆವೃತ್ತಿ;
   ಆದರೆ (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8')
      ಹಿಂತಿರುಗಿ stbi__err("not GIF", "Corrupt GIF");

   ಆವೃತ್ತಿ = stbi__get8(s);
   ಆದರೆ (ಆವೃತ್ತಿ != '7' && ಆವೃತ್ತಿ != '9')    ಹಿಂತಿರುಗಿ stbi__err("not GIF", "Corrupt GIF");
   ಆದರೆ (stbi__get8(s) != 'a')                ಹಿಂತಿರುಗಿ stbi__err("not GIF", "Corrupt GIF");

   stbi__g_failure_reason = "";
   g->w = stbi__get16le(s);
   g->h = stbi__get16le(s);
   g->flags = stbi__get8(s);
   g->bgindex = stbi__get8(s);
   g->ಅನುಪಾತ = stbi__get8(s);
   g->transparent = -1;

   ಆದರೆ (comp != 0) *comp = 4;  // can't actually tell whether it's 3 or 4 until we parse the comments

   ಆದರೆ (is_info) ಹಿಂತಿರುಗಿ 1;

   ಆದರೆ (g->flags & 0x80)
      stbi__gif_parse_colortable(s,g->pal, 2 << (g->flags & 7), -1);

   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__gif_info_raw(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   stbi__gif* g = (stbi__gif*) stbi__malloc(ಗಾತ್ರ(stbi__gif));
   ಆದರೆ (!stbi_gif_ಶೀರ್ಷಿಕೆ(s, g, comp, 1)) {
      STBI_FREE(g);
      stbi__rewind( s );
      ಹಿಂತಿರುಗಿ 0;
   }
   ಆದರೆ (x) *x = g->w;
   ಆದರೆ (y) *y = g->h;
   STBI_FREE(g);
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi_ಹೊರಹೋಗು_gif_ಸಂಕೇತ(stbi__gif *g, stbi__uint16 ಸಂಕೇತ_2)
{
   stbi_uc *p, *c;
   ಪೂರ್ಣಾಂಕ idx; 

   // recurse to decode the prefixes, since the linked-list is backwards,
   // and working backwards through an interleaved image would be nasty
   ಆದರೆ (g->codes[ಸಂಕೇತ_2].ಪೂರ್ವಪ್ರತ್ಯಯ >= 0)
      stbi_ಹೊರಹೋಗು_gif_ಸಂಕೇತ(g, g->codes[ಸಂಕೇತ_2].ಪೂರ್ವಪ್ರತ್ಯಯ);

   ಆದರೆ (g->cur_y >= g->max_y) ಹಿಂತಿರುಗಿ;

   idx = g->cur_x + g->cur_y; 
   p = &g->ಹೊರಹೋಗು[idx];
   g->history[idx / 4] = 1;  

   c = &g->color_ಕೋಷ್ಟಕ[g->codes[ಸಂಕೇತ_2].suffix * 4];
   ಆದರೆ (c[3] > 128) { // don't render transparent pixels; 
      p[0] = c[2];
      p[1] = c[1];
      p[2] = c[0];
      p[3] = c[3];
   }
   g->cur_x += 4;

   ಆದರೆ (g->cur_x >= g->max_x) {
      g->cur_x = g->ಆರಂಭ_x;
      g->cur_y += g->ಹೆಜ್ಜೆ;

      ಆಗ (g->cur_y >= g->max_y && g->parse > 0) {
         g->ಹೆಜ್ಜೆ = (1 << g->parse) * g->ಸಾಲು_ಗಾತ್ರ;
         g->cur_y = g->ಆರಂಭ_y + (g->ಹೆಜ್ಜೆ >> 1);
         --g->parse;
      }
   }
}

ಸ್ಥಿರ stbi_uc *stbi__process_gif_raster(stbi__context *s, stbi__gif *g)
{
   stbi_uc lzw_cs;
   stbi__int32 len, init_ಸಂಕೇತ;
   stbi__uint32 ಮೊದಲ;
   stbi__int32 codesize, codemask, avail, oldcode, ಬಿಟ್ಟುಗಳು, valid_ಬಿಟ್ಟುಗಳು, clear;
   stbi__gif_lzw *p;

   lzw_cs = stbi__get8(s);
   ಆದರೆ (lzw_cs > 12) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
   clear = 1 << lzw_cs;
   ಮೊದಲ = 1;
   codesize = lzw_cs + 1;
   codemask = (1 << codesize) - 1;
   ಬಿಟ್ಟುಗಳು = 0;
   valid_ಬಿಟ್ಟುಗಳು = 0;
   ಗಾಗಿ (init_ಸಂಕೇತ = 0; init_ಸಂಕೇತ < clear; init_ಸಂಕೇತ++) {
      g->codes[init_ಸಂಕೇತ].ಪೂರ್ವಪ್ರತ್ಯಯ = -1;
      g->codes[init_ಸಂಕೇತ].ಮೊದಲ = (stbi_uc) init_ಸಂಕೇತ;
      g->codes[init_ಸಂಕೇತ].suffix = (stbi_uc) init_ಸಂಕೇತ;
   }

   // support no starting clear code
   avail = clear+2;
   oldcode = -1;

   len = 0;
   ಗಾಗಿ(;;) {
      ಆದರೆ (valid_ಬಿಟ್ಟುಗಳು < codesize) {
         ಆದರೆ (len == 0) {
            len = stbi__get8(s); // start new block
            ಆದರೆ (len == 0)
               ಹಿಂತಿರುಗಿ g->ಹೊರಹೋಗು;
         }
         --len;
         ಬಿಟ್ಟುಗಳು |= (stbi__int32) stbi__get8(s) << valid_ಬಿಟ್ಟುಗಳು;
         valid_ಬಿಟ್ಟುಗಳು += 8;
      } ಇಲ್ಲದಿದ್ದರೆ {
         stbi__int32 ಸಂಕೇತ_2 = ಬಿಟ್ಟುಗಳು & codemask;
         ಬಿಟ್ಟುಗಳು >>= codesize;
         valid_ಬಿಟ್ಟುಗಳು -= codesize;
         // @OPTIMIZE: is there some way we can accelerate the non-clear path?
         ಆದರೆ (ಸಂಕೇತ_2 == clear) {  // clear code
            codesize = lzw_cs + 1;
            codemask = (1 << codesize) - 1;
            avail = clear + 2;
            oldcode = -1;
            ಮೊದಲ = 0;
         } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಂಕೇತ_2 == clear + 1) { // end of stream code
            stbi__skip(s, len);
            ಆಗ ((len = stbi__get8(s)) > 0)
               stbi__skip(s,len);
            ಹಿಂತಿರುಗಿ g->ಹೊರಹೋಗು;
         } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಂಕೇತ_2 <= avail) {
            ಆದರೆ (ಮೊದಲ) {
               ಹಿಂತಿರುಗಿ stbi__errpuc("no clear code", "Corrupt GIF");
            }

            ಆದರೆ (oldcode >= 0) {
               p = &g->codes[avail++];
               ಆದರೆ (avail > 8192) {
                  ಹಿಂತಿರುಗಿ stbi__errpuc("too many codes", "Corrupt GIF");
               }

               p->ಪೂರ್ವಪ್ರತ್ಯಯ = (stbi__int16) oldcode;
               p->ಮೊದಲ = g->codes[oldcode].ಮೊದಲ;
               p->suffix = (ಸಂಕೇತ_2 == avail) ? p->ಮೊದಲ : g->codes[ಸಂಕೇತ_2].ಮೊದಲ;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಂಕೇತ_2 == avail)
               ಹಿಂತಿರುಗಿ stbi__errpuc("illegal code in raster", "Corrupt GIF");

            stbi_ಹೊರಹೋಗು_gif_ಸಂಕೇತ(g, (stbi__uint16) ಸಂಕೇತ_2);

            ಆದರೆ ((avail & codemask) == 0 && avail <= 0x0FFF) {
               codesize++;
               codemask = (1 << codesize) - 1;
            }

            oldcode = ಸಂಕೇತ_2;
         } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ stbi__errpuc("illegal code in raster", "Corrupt GIF");
         }
      }
   }
}

// this function is designed to support animated gifs, although stb_image doesn't support it
// two back is the image from two frames ago, used for a very specific disposal format
ಸ್ಥಿರ stbi_uc *stbi_gif_ತುಂಬು_ಮುಂದಿನ(stbi__context *s, stbi__gif *g, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_uc *two_ಹಿಂಭಾಗ)
{
   ಪೂರ್ಣಾಂಕ dispose; 
   ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಚೌಕಟ್ಟು; 
   ಪೂರ್ಣಾಂಕ pi; 
   ಪೂರ್ಣಾಂಕ pcount; 

   // on first frame, any non-written pixels get the background colour (non-transparent)
   ಮೊದಲ_ಚೌಕಟ್ಟು = 0; 
   ಆದರೆ (g->ಹೊರಹೋಗು == 0) {
      ಆದರೆ (!stbi_gif_ಶೀರ್ಷಿಕೆ(s, g, comp,0))     ಹಿಂತಿರುಗಿ 0; // stbi__g_failure_reason set by stbi__gif_header
      g->ಹೊರಹೋಗು = (stbi_uc *) stbi__malloc(4 * g->w * g->h);
      g->background = (stbi_uc *) stbi__malloc(4 * g->w * g->h); 
      g->history = (stbi_uc *) stbi__malloc(g->w * g->h); 
      ಆದರೆ (g->ಹೊರಹೋಗು == 0)                      ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");

      // image is treated as "tranparent" at the start - ie, nothing overwrites the current background; 
      // background colour is only used for pixels that are not rendered first frame, after that "background"
      // color refers to teh color that was there the previous frame. 
      memset( g->ಹೊರಹೋಗು, 0x00, 4 * g->w * g->h ); 
      memset( g->background, 0x00, 4 * g->w * g->h ); // state of the background (starts transparent)
      memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame
      ಮೊದಲ_ಚೌಕಟ್ಟು = 1; 
   } ಇಲ್ಲದಿದ್ದರೆ {
      // second frame - how do we dispoase of the previous one?
      dispose = (g->eflags & 0x1C) >> 2; 
      pcount = g->w * g->h; 

      ಆದರೆ ((dispose == 3) && (two_ಹಿಂಭಾಗ == 0)) {
         dispose = 2; // if I don't have an image to revert back to, default to the old background
      }

      ಆದರೆ (dispose == 3) { // use previous graphic
         ಗಾಗಿ (pi = 0; pi < pcount; ++pi) {
            ಆದರೆ (g->history[pi]) {
               ಸ್ಮರಣೆ_ನಕಲು( &g->ಹೊರಹೋಗು[pi * 4], &two_ಹಿಂಭಾಗ[pi * 4], 4 ); 
            }
         }
      } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dispose == 2) { 
         // restore what was changed last frame to background before that frame; 
         ಗಾಗಿ (pi = 0; pi < pcount; ++pi) {
            ಆದರೆ (g->history[pi]) {
               ಸ್ಮರಣೆ_ನಕಲು( &g->ಹೊರಹೋಗು[pi * 4], &g->background[pi * 4], 4 ); 
            }
         }
      } ಇಲ್ಲದಿದ್ದರೆ {
         // This is a non-disposal case eithe way, so just 
         // leave the pixels as is, and they will become the new background
         // 1: do not dispose
         // 0:  not specified.
      }

      // background is what out is after the undoing of the previou frame; 
      ಸ್ಮರಣೆ_ನಕಲು( g->background, g->ಹೊರಹೋಗು, 4 * g->w * g->h ); 
   }

   // clear my history; 
   memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame

   ಗಾಗಿ (;;) {
      ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = stbi__get8(s); 
      ಆಯ್ಕೆ (ಟ್ಯಾಗ್) {
         ಸ್ಥಿತಿ 0x2C: /* Image Descriptor */
         {
            stbi__int32 x, y, w, h;
            stbi_uc *o;

            x = stbi__get16le(s);
            y = stbi__get16le(s);
            w = stbi__get16le(s);
            h = stbi__get16le(s);
            ಆದರೆ (((x + w) > (g->w)) || ((y + h) > (g->h)))
               ಹಿಂತಿರುಗಿ stbi__errpuc("bad Image Descriptor", "Corrupt GIF");

            g->ಸಾಲು_ಗಾತ್ರ = g->w * 4;
            g->ಆರಂಭ_x = x * 4;
            g->ಆರಂಭ_y = y * g->ಸಾಲು_ಗಾತ್ರ;
            g->max_x   = g->ಆರಂಭ_x + w * 4;
            g->max_y   = g->ಆರಂಭ_y + h * g->ಸಾಲು_ಗಾತ್ರ;
            g->cur_x   = g->ಆರಂಭ_x;
            g->cur_y   = g->ಆರಂಭ_y;

            g->lflags = stbi__get8(s);

            ಆದರೆ (g->lflags & 0x40) {
               g->ಹೆಜ್ಜೆ = 8 * g->ಸಾಲು_ಗಾತ್ರ; // first interlaced spacing
               g->parse = 3;
            } ಇಲ್ಲದಿದ್ದರೆ {
               g->ಹೆಜ್ಜೆ = g->ಸಾಲು_ಗಾತ್ರ;
               g->parse = 0;
            }

            ಆದರೆ (g->lflags & 0x80) {
               stbi__gif_parse_colortable(s,g->lpal, 2 << (g->lflags & 7), g->eflags & 0x01 ? g->transparent : -1);
               g->color_ಕೋಷ್ಟಕ = (stbi_uc *) g->lpal;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (g->flags & 0x80) {
               g->color_ಕೋಷ್ಟಕ = (stbi_uc *) g->pal;
            } ಇಲ್ಲದಿದ್ದರೆ
               ಹಿಂತಿರುಗಿ stbi__errpuc("missing color table", "Corrupt GIF");            
            
            o = stbi__process_gif_raster(s, g);
            ಆದರೆ (o == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

            // if this was the first frame, 
            pcount = g->w * g->h; 
            ಆದರೆ (ಮೊದಲ_ಚೌಕಟ್ಟು && (g->bgindex > 0)) {
               // if first frame, any pixel not drawn to gets the background color
               ಗಾಗಿ (pi = 0; pi < pcount; ++pi) {
                  ಆದರೆ (g->history[pi] == 0) {
                     g->pal[g->bgindex][3] = 255; // just in case it was made transparent, undo that; It will be reset next frame if need be; 
                     ಸ್ಮರಣೆ_ನಕಲು( &g->ಹೊರಹೋಗು[pi * 4], &g->pal[g->bgindex], 4 ); 
                  }
               }
            }

            ಹಿಂತಿರುಗಿ o;
         }

         ಸ್ಥಿತಿ 0x21: // Comment Extension.
         {
            ಪೂರ್ಣಾಂಕ len;
            ಪೂರ್ಣಾಂಕ ext = stbi__get8(s); 
            ಆದರೆ (ext == 0xF9) { // Graphic Control Extension.
               len = stbi__get8(s);
               ಆದರೆ (len == 4) {
                  g->eflags = stbi__get8(s);
                  g->delay = 10 * stbi__get16le(s); // delay - 1/100th of a second, saving as 1/1000ths.

                  // unset old transparent
                  ಆದರೆ (g->transparent >= 0) {
                     g->pal[g->transparent][3] = 255; 
                  } 
                  ಆದರೆ (g->eflags & 0x01) {
                     g->transparent = stbi__get8(s);
                     ಆದರೆ (g->transparent >= 0) {
                        g->pal[g->transparent][3] = 0; 
                     }
                  } ಇಲ್ಲದಿದ್ದರೆ {
                     // don't need transparent
                     stbi__skip(s, 1); 
                     g->transparent = -1; 
                  }
               } ಇಲ್ಲದಿದ್ದರೆ {
                  stbi__skip(s, len);
                  ನಿಲ್ಲಿಸು;
               }
            } 
            ಆಗ ((len = stbi__get8(s)) != 0) {
               stbi__skip(s, len);
            }
            ನಿಲ್ಲಿಸು;
         }

         ಸ್ಥಿತಿ 0x3B: // gif stream termination code
            ಹಿಂತಿರುಗಿ (stbi_uc *) s; // using '1' causes warning on some compilers

         ಪೂರ್ವನಿಯೋಜಿತ:
            ಹಿಂತಿರುಗಿ stbi__errpuc("unknown code", "Corrupt GIF");
      }
   }
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_ತುಂಬು_gif_main(stbi__context *s, ಪೂರ್ಣಾಂಕ **delays, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *z, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp)
{
   ಆದರೆ (stbi__gif_test(s)) {
      ಪೂರ್ಣಾಂಕ layers = 0; 
      stbi_uc *u = 0;
      stbi_uc *ಹೊರಹೋಗು = 0;
      stbi_uc *two_ಹಿಂಭಾಗ = 0; 
      stbi__gif g;
      ಪೂರ್ಣಾಂಕ stride; 
      memset(&g, 0, ಗಾತ್ರ(g));
      ಆದರೆ (delays) {
         *delays = 0; 
      }

      ಮಾಡು {
         u = stbi_gif_ತುಂಬು_ಮುಂದಿನ(s, &g, comp, req_comp, two_ಹಿಂಭಾಗ);
         ಆದರೆ (u == (stbi_uc *) s) u = 0;  // end of animated gif marker

         ಆದರೆ (u) {
            *x = g.w;
            *y = g.h;
            ++layers; 
            stride = g.w * g.h * 4; 
         
            ಆದರೆ (ಹೊರಹೋಗು) {
               ಹೊರಹೋಗು = (stbi_uc*) STBI_REALLOC( ಹೊರಹೋಗು, layers * stride ); 
               ಆದರೆ (delays) {
                  *delays = (ಪೂರ್ಣಾಂಕ*) STBI_REALLOC( *delays, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) * layers ); 
               }
            } ಇಲ್ಲದಿದ್ದರೆ {
               ಹೊರಹೋಗು = (stbi_uc*)stbi__malloc( layers * stride ); 
               ಆದರೆ (delays) {
                  *delays = (ಪೂರ್ಣಾಂಕ*) stbi__malloc( layers * ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) ); 
               }
            }
            ಸ್ಮರಣೆ_ನಕಲು( ಹೊರಹೋಗು + ((layers - 1) * stride), u, stride ); 
            ಆದರೆ (layers >= 2) {
               two_ಹಿಂಭಾಗ = ಹೊರಹೋಗು - 2 * stride; 
            }

            ಆದರೆ (delays) {
               (*delays)[layers - 1U] = g.delay; 
            }
         }
      } ಆಗ (u != 0); 

      // free temp buffer; 
      STBI_FREE(g.ಹೊರಹೋಗು); 
      STBI_FREE(g.history); 
      STBI_FREE(g.background); 

      // do the final conversion after loading everything; 
      ಆದರೆ (req_comp && req_comp != 4)
         ಹೊರಹೋಗು = stbi__convert_format(ಹೊರಹೋಗು, 4, req_comp, layers * g.w, g.h);

      *z = layers; 
      ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಹಿಂತಿರುಗಿ stbi__errpuc("not GIF", "Image was not as a gif type."); 
   }
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_gif_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   stbi_uc *u = 0;
   stbi__gif g;
   memset(&g, 0, ಗಾತ್ರ(g));

   u = stbi_gif_ತುಂಬು_ಮುಂದಿನ(s, &g, comp, req_comp, 0);
   ಆದರೆ (u == (stbi_uc *) s) u = 0;  // end of animated gif marker
   ಆದರೆ (u) {
      *x = g.w;
      *y = g.h;

      // moved conversion to after successful load so that the same
      // can be done for multiple frames. 
      ಆದರೆ (req_comp && req_comp != 4)
         u = stbi__convert_format(u, 4, req_comp, g.w, g.h);
   }

   // free buffers needed for multiple frame loading; 
   STBI_FREE(g.history);
   STBI_FREE(g.background); 

   ಹಿಂತಿರುಗಿ u;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__gif_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಹಿಂತಿರುಗಿ stbi__gif_info_raw(s,x,y,comp);
}
#ಆದರೆ_ಅಂತ್ಯ

// *************************************************************************************************
// Radiance RGBE HDR loader
// originally by Nicolas Schulz
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__hdr_test_core(stbi__context *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signature)
{
   ಪೂರ್ಣಾಂಕ i;
   ಗಾಗಿ (i=0; signature[i]; ++i)
      ಆದರೆ (stbi__get8(s) != signature[i])
          ಹಿಂತಿರುಗಿ 0;
   stbi__rewind(s);
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__hdr_test(stbi__context* s)
{
   ಪೂರ್ಣಾಂಕ r = stbi__hdr_test_core(s, "#?RADIANCE\n");
   stbi__rewind(s);
   ಆದರೆ(!r) {
       r = stbi__hdr_test_core(s, "#?RGBE\n");
       stbi__rewind(s);
   }
   ಹಿಂತಿರುಗಿ r;
}

#ವ್ಯಾಖ್ಯಾನಿಸು STBI__HDR_BUFLEN  1024
ಸ್ಥಿರ ಅಕ್ಷರ *stbi__hdr_gettoken(stbi__context *z, ಅಕ್ಷರ *ಬಫರ್)
{
   ಪೂರ್ಣಾಂಕ len=0;
   ಅಕ್ಷರ c = '\0';

   c = (ಅಕ್ಷರ) stbi__get8(z);

   ಆಗ (!stbi__at_eof(z) && c != '\n') {
      ಬಫರ್[len++] = c;
      ಆದರೆ (len == STBI__HDR_BUFLEN-1) {
         // flush to end of line
         ಆಗ (!stbi__at_eof(z) && stbi__get8(z) != '\n')
            ;
         ನಿಲ್ಲಿಸು;
      }
      c = (ಅಕ್ಷರ) stbi__get8(z);
   }

   ಬಫರ್[len] = 0;
   ಹಿಂತಿರುಗಿ ಬಫರ್;
}

ಸ್ಥಿರ ಶೂನ್ಯ stbi__hdr_convert(ದಶಮಾಂಶ *output, stbi_uc *ಒಳಹರಿವು, ಪೂರ್ಣಾಂಕ req_comp)
{
   ಆದರೆ ( ಒಳಹರಿವು[3] != 0 ) {
      ದಶಮಾಂಶ f1;
      // Exponent
      f1 = (ದಶಮಾಂಶ) ldexp(1.0f, ಒಳಹರಿವು[3] - (ಪೂರ್ಣಾಂಕ)(128 + 8));
      ಆದರೆ (req_comp <= 2)
         output[0] = (ಒಳಹರಿವು[0] + ಒಳಹರಿವು[1] + ಒಳಹರಿವು[2]) * f1 / 3;
      ಇಲ್ಲದಿದ್ದರೆ {
         output[0] = ಒಳಹರಿವು[0] * f1;
         output[1] = ಒಳಹರಿವು[1] * f1;
         output[2] = ಒಳಹರಿವು[2] * f1;
      }
      ಆದರೆ (req_comp == 2) output[1] = 1;
      ಆದರೆ (req_comp == 4) output[3] = 1;
   } ಇಲ್ಲದಿದ್ದರೆ {
      ಆಯ್ಕೆ (req_comp) {
         ಸ್ಥಿತಿ 4: output[3] = 1; /* fallthrough */
         ಸ್ಥಿತಿ 3: output[0] = output[1] = output[2] = 0;
                 ನಿಲ್ಲಿಸು;
         ಸ್ಥಿತಿ 2: output[1] = 1; /* fallthrough */
         ಸ್ಥಿತಿ 1: output[0] = 0;
                 ನಿಲ್ಲಿಸು;
      }
   }
}

ಸ್ಥಿರ ದಶಮಾಂಶ *stbi_hdr_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   ಅಕ್ಷರ ಬಫರ್[STBI__HDR_BUFLEN];
   ಅಕ್ಷರ *token;
   ಪೂರ್ಣಾಂಕ valid = 0;
   ಪೂರ್ಣಾಂಕ ಅಗಲ, ಎತ್ತರ;
   stbi_uc *scanline;
   ದಶಮಾಂಶ *hdr_ದತ್ತಾಂಶ;
   ಪೂರ್ಣಾಂಕ len;
   ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಎಣಿಕೆ_2, ಮೌಲ್ಯ;
   ಪೂರ್ಣಾಂಕ i, j, k, c1,c2, z;
   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ_Token;
   STBI_NOTUSED(ri);

   // Check identifier
   ಶೀರ್ಷಿಕೆ_Token = stbi__hdr_gettoken(s,ಬಫರ್);
   ಆದರೆ (strcmp(ಶೀರ್ಷಿಕೆ_Token, "#?RADIANCE") != 0 && strcmp(ಶೀರ್ಷಿಕೆ_Token, "#?RGBE") != 0)
      ಹಿಂತಿರುಗಿ stbi__errpf("not HDR", "Corrupt HDR image");

   // Parse header
   ಗಾಗಿ(;;) {
      token = stbi__hdr_gettoken(s,ಬಫರ್);
      ಆದರೆ (token[0] == 0) ನಿಲ್ಲಿಸು;
      ಆದರೆ (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   ಆದರೆ (!valid)    ಹಿಂತಿರುಗಿ stbi__errpf("unsupported format", "Unsupported HDR format");

   // Parse width and height
   // can't use sscanf() if we're not using stdio!
   token = stbi__hdr_gettoken(s,ಬಫರ್);
   ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(token, "-Y ", 3))  ಹಿಂತಿರುಗಿ stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   ಎತ್ತರ = (ಪೂರ್ಣಾಂಕ) strtol(token, &token, 10);
   ಆಗ (*token == ' ') ++token;
   ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(token, "+X ", 3))  ಹಿಂತಿರುಗಿ stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   ಅಗಲ = (ಪೂರ್ಣಾಂಕ) strtol(token, ಖಾಲಿ, 10);

   *x = ಅಗಲ;
   *y = ಎತ್ತರ;

   ಆದರೆ (comp) *comp = 3;
   ಆದರೆ (req_comp == 0) req_comp = 3;

   ಆದರೆ (!stbi__mad4sizes_valid(ಅಗಲ, ಎತ್ತರ, req_comp, ಗಾತ್ರ(ದಶಮಾಂಶ), 0))
      ಹಿಂತಿರುಗಿ stbi__errpf("too large", "HDR image is too large");

   // Read data
   hdr_ದತ್ತಾಂಶ = (ದಶಮಾಂಶ *) stbi__malloc_mad4(ಅಗಲ, ಎತ್ತರ, req_comp, ಗಾತ್ರ(ದಶಮಾಂಶ), 0);
   ಆದರೆ (!hdr_ದತ್ತಾಂಶ)
      ಹಿಂತಿರುಗಿ stbi__errpf("outofmem", "Out of memory");

   // Load image data
   // image data is stored as some number of sca
   ಆದರೆ ( ಅಗಲ < 8 || ಅಗಲ >= 32768) {
      // Read flat data
      ಗಾಗಿ (j=0; j < ಎತ್ತರ; ++j) {
         ಗಾಗಿ (i=0; i < ಅಗಲ; ++i) {
            stbi_uc rgbe[4];
           main_decode_loop:
            stbi__getn(s, rgbe, 4);
            stbi__hdr_convert(hdr_ದತ್ತಾಂಶ + j * ಅಗಲ * req_comp + i * req_comp, rgbe, req_comp);
         }
      }
   } ಇಲ್ಲದಿದ್ದರೆ {
      // Read RLE-encoded data
      scanline = ಖಾಲಿ;

      ಗಾಗಿ (j = 0; j < ಎತ್ತರ; ++j) {
         c1 = stbi__get8(s);
         c2 = stbi__get8(s);
         len = stbi__get8(s);
         ಆದರೆ (c1 != 2 || c2 != 2 || (len & 0x80)) {
            // not run-length encoded, so we have to actually use THIS data as a decoded
            // pixel (note this can't be a valid pixel--one of RGB must be >= 128)
            stbi_uc rgbe[4];
            rgbe[0] = (stbi_uc) c1;
            rgbe[1] = (stbi_uc) c2;
            rgbe[2] = (stbi_uc) len;
            rgbe[3] = (stbi_uc) stbi__get8(s);
            stbi__hdr_convert(hdr_ದತ್ತಾಂಶ, rgbe, req_comp);
            i = 1;
            j = 0;
            STBI_FREE(scanline);
            ಹೋಗು main_decode_loop; // yes, this makes no sense
         }
         len <<= 8;
         len |= stbi__get8(s);
         ಆದರೆ (len != ಅಗಲ) { STBI_FREE(hdr_ದತ್ತಾಂಶ); STBI_FREE(scanline); ಹಿಂತಿರುಗಿ stbi__errpf("invalid decoded scanline length", "corrupt HDR"); }
         ಆದರೆ (scanline == ಖಾಲಿ) {
            scanline = (stbi_uc *) stbi__malloc_mad2(ಅಗಲ, 4, 0);
            ಆದರೆ (!scanline) {
               STBI_FREE(hdr_ದತ್ತಾಂಶ);
               ಹಿಂತಿರುಗಿ stbi__errpf("outofmem", "Out of memory");
            }
         }

         ಗಾಗಿ (k = 0; k < 4; ++k) {
            ಪೂರ್ಣಾಂಕ nleft;
            i = 0;
            ಆಗ ((nleft = ಅಗಲ - i) > 0) {
               ಎಣಿಕೆ_2 = stbi__get8(s);
               ಆದರೆ (ಎಣಿಕೆ_2 > 128) {
                  // Run
                  ಮೌಲ್ಯ = stbi__get8(s);
                  ಎಣಿಕೆ_2 -= 128;
                  ಆದರೆ (ಎಣಿಕೆ_2 > nleft) { STBI_FREE(hdr_ದತ್ತಾಂಶ); STBI_FREE(scanline); ಹಿಂತಿರುಗಿ stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  ಗಾಗಿ (z = 0; z < ಎಣಿಕೆ_2; ++z)
                     scanline[i++ * 4 + k] = ಮೌಲ್ಯ;
               } ಇಲ್ಲದಿದ್ದರೆ {
                  // Dump
                  ಆದರೆ (ಎಣಿಕೆ_2 > nleft) { STBI_FREE(hdr_ದತ್ತಾಂಶ); STBI_FREE(scanline); ಹಿಂತಿರುಗಿ stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  ಗಾಗಿ (z = 0; z < ಎಣಿಕೆ_2; ++z)
                     scanline[i++ * 4 + k] = stbi__get8(s);
               }
            }
         }
         ಗಾಗಿ (i=0; i < ಅಗಲ; ++i)
            stbi__hdr_convert(hdr_ದತ್ತಾಂಶ+(j*ಅಗಲ + i)*req_comp, scanline + i*4, req_comp);
      }
      ಆದರೆ (scanline)
         STBI_FREE(scanline);
   }

   ಹಿಂತಿರುಗಿ hdr_ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__hdr_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಅಕ್ಷರ ಬಫರ್[STBI__HDR_BUFLEN];
   ಅಕ್ಷರ *token;
   ಪೂರ್ಣಾಂಕ valid = 0;
   ಪೂರ್ಣಾಂಕ dummy;

   ಆದರೆ (!x) x = &dummy;
   ಆದರೆ (!y) y = &dummy;
   ಆದರೆ (!comp) comp = &dummy;

   ಆದರೆ (stbi__hdr_test(s) == 0) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }

   ಗಾಗಿ(;;) {
      token = stbi__hdr_gettoken(s,ಬಫರ್);
      ಆದರೆ (token[0] == 0) ನಿಲ್ಲಿಸು;
      ಆದರೆ (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   ಆದರೆ (!valid) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   token = stbi__hdr_gettoken(s,ಬಫರ್);
   ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(token, "-Y ", 3)) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   token += 3;
   *y = (ಪೂರ್ಣಾಂಕ) strtol(token, &token, 10);
   ಆಗ (*token == ' ') ++token;
   ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(token, "+X ", 3)) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   token += 3;
   *x = (ಪೂರ್ಣಾಂಕ) strtol(token, ಖಾಲಿ, 10);
   *comp = 3;
   ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ // STBI_NO_HDR

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_BMP
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__bmp_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಶೂನ್ಯ *p;
   stbi_bmp_ದತ್ತಾಂಶ info;

   info.all_a = 255;
   p = stbi_bmp_parse_ಶೀರ್ಷಿಕೆ(s, &info);
   stbi__rewind( s );
   ಆದರೆ (p == ಖಾಲಿ)
      ಹಿಂತಿರುಗಿ 0;
   ಆದರೆ (x) *x = s->img_x;
   ಆದರೆ (y) *y = s->img_y;
   ಆದರೆ (comp) *comp = info.ma ? 4 : 3;
   ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PSD
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__psd_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಪೂರ್ಣಾಂಕ channel_ಎಣಿಕೆ, dummy, ಆಳ;
   ಆದರೆ (!x) x = &dummy;
   ಆದರೆ (!y) y = &dummy;
   ಆದರೆ (!comp) comp = &dummy;
   ಆದರೆ (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   ಆದರೆ (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   stbi__skip(s, 6);
   channel_ಎಣಿಕೆ = stbi__get16be(s);
   ಆದರೆ (channel_ಎಣಿಕೆ < 0 || channel_ಎಣಿಕೆ > 16) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   *y = stbi__get32be(s);
   *x = stbi__get32be(s);
   ಆಳ = stbi__get16be(s);
   ಆದರೆ (ಆಳ != 8 && ಆಳ != 16) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   ಆದರೆ (stbi__get16be(s) != 3) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   *comp = 4;
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__psd_is16(stbi__context *s)
{
   ಪೂರ್ಣಾಂಕ channel_ಎಣಿಕೆ, ಆಳ;
   ಆದರೆ (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   ಆದರೆ (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   stbi__skip(s, 6);
   channel_ಎಣಿಕೆ = stbi__get16be(s);
   ಆದರೆ (channel_ಎಣಿಕೆ < 0 || channel_ಎಣಿಕೆ > 16) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   (ಶೂನ್ಯ) stbi__get32be(s);
   (ಶೂನ್ಯ) stbi__get32be(s);
   ಆಳ = stbi__get16be(s);
   ಆದರೆ (ಆಳ != 16) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PIC
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__pic_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಪೂರ್ಣಾಂಕ act_comp=0,ಅಂಕೆ_packets=0,chained,dummy;
   stbi__pic_packet packets[10];

   ಆದರೆ (!x) x = &dummy;
   ಆದರೆ (!y) y = &dummy;
   ಆದರೆ (!comp) comp = &dummy;

   ಆದರೆ (!stbi__pic_is4(s,"\x53\x80\xF6\x34")) {
      stbi__rewind(s);
      ಹಿಂತಿರುಗಿ 0;
   }

   stbi__skip(s, 88);

   *x = stbi__get16be(s);
   *y = stbi__get16be(s);
   ಆದರೆ (stbi__at_eof(s)) {
      stbi__rewind( s);
      ಹಿಂತಿರುಗಿ 0;
   }
   ಆದರೆ ( (*x) != 0 && (1 << 28) / (*x) < (*y)) {
      stbi__rewind( s );
      ಹಿಂತಿರುಗಿ 0;
   }

   stbi__skip(s, 8);

   ಮಾಡು {
      stbi__pic_packet *packet;

      ಆದರೆ (ಅಂಕೆ_packets==ಗಾತ್ರ(packets)/ಗಾತ್ರ(packets[0]))
         ಹಿಂತಿರುಗಿ 0;

      packet = &packets[ಅಂಕೆ_packets++];
      chained = stbi__get8(s);
      packet->ಗಾತ್ರ_2    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);
      act_comp |= packet->channel;

      ಆದರೆ (stbi__at_eof(s)) {
          stbi__rewind( s );
          ಹಿಂತಿರುಗಿ 0;
      }
      ಆದರೆ (packet->ಗಾತ್ರ_2 != 8) {
          stbi__rewind( s );
          ಹಿಂತಿರುಗಿ 0;
      }
   } ಆಗ (chained);

   *comp = (act_comp & 0x10 ? 4 : 3);

   ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ

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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNM

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pnm_test(stbi__context *s)
{
   ಅಕ್ಷರ p, t;
   p = (ಅಕ್ಷರ) stbi__get8(s);
   t = (ಅಕ್ಷರ) stbi__get8(s);
   ಆದರೆ (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind( s );
       ಹಿಂತಿರುಗಿ 0;
   }
   ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ *stbi_pnm_ತುಂಬು(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp, ಪೂರ್ಣಾಂಕ req_comp, stbi_ಫಲಿತಾಂಶ_info *ri)
{
   stbi_uc *ಹೊರಹೋಗು;
   STBI_NOTUSED(ri);

   ಆದರೆ (!stbi__pnm_info(s, (ಪೂರ್ಣಾಂಕ *)&s->img_x, (ಪೂರ್ಣಾಂಕ *)&s->img_y, (ಪೂರ್ಣಾಂಕ *)&s->img_n))
      ಹಿಂತಿರುಗಿ 0;

   *x = s->img_x;
   *y = s->img_y;
   ಆದರೆ (comp) *comp = s->img_n;

   ಆದರೆ (!stbi__mad3sizes_valid(s->img_n, s->img_x, s->img_y, 0))
      ಹಿಂತಿರುಗಿ stbi__errpuc("too large", "PNM too large");

   ಹೊರಹೋಗು = (stbi_uc *) stbi__malloc_mad3(s->img_n, s->img_x, s->img_y, 0);
   ಆದರೆ (!ಹೊರಹೋಗು) ಹಿಂತಿರುಗಿ stbi__errpuc("outofmem", "Out of memory");
   stbi__getn(s, ಹೊರಹೋಗು, s->img_n * s->img_x * s->img_y);

   ಆದರೆ (req_comp && req_comp != s->img_n) {
      ಹೊರಹೋಗು = stbi__convert_format(ಹೊರಹೋಗು, s->img_n, req_comp, s->img_x, s->img_y);
      ಆದರೆ (ಹೊರಹೋಗು == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು; // stbi__convert_format frees input on failure
   }
   ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pnm_isspace(ಅಕ್ಷರ c)
{
   ಹಿಂತಿರುಗಿ c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

ಸ್ಥಿರ ಶೂನ್ಯ     stbi__pnm_skip_whitespace(stbi__context *s, ಅಕ್ಷರ *c)
{
   ಗಾಗಿ (;;) {
      ಆಗ (!stbi__at_eof(s) && stbi__pnm_isspace(*c))
         *c = (ಅಕ್ಷರ) stbi__get8(s);

      ಆದರೆ (stbi__at_eof(s) || *c != '#')
         ನಿಲ್ಲಿಸು;

      ಆಗ (!stbi__at_eof(s) && *c != '\n' && *c != '\r' )
         *c = (ಅಕ್ಷರ) stbi__get8(s);
   }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pnm_isdigit(ಅಕ್ಷರ c)
{
   ಹಿಂತಿರುಗಿ c >= '0' && c <= '9';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pnm_getinteger(stbi__context *s, ಅಕ್ಷರ *c)
{
   ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = 0;

   ಆಗ (!stbi__at_eof(s) && stbi__pnm_isdigit(*c)) {
      ಮೌಲ್ಯ = ಮೌಲ್ಯ*10 + (*c - '0');
      *c = (ಅಕ್ಷರ) stbi__get8(s);
   }

   ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      stbi__pnm_info(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಪೂರ್ಣಾಂಕ maxv, dummy;
   ಅಕ್ಷರ c, p, t;

   ಆದರೆ (!x) x = &dummy;
   ಆದರೆ (!y) y = &dummy;
   ಆದರೆ (!comp) comp = &dummy;

   stbi__rewind(s);

   // Get identifier
   p = (ಅಕ್ಷರ) stbi__get8(s);
   t = (ಅಕ್ಷರ) stbi__get8(s);
   ಆದರೆ (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind(s);
       ಹಿಂತಿರುಗಿ 0;
   }

   *comp = (t == '6') ? 3 : 1;  // '5' is 1-component .pgm; '6' is 3-component .ppm

   c = (ಅಕ್ಷರ) stbi__get8(s);
   stbi__pnm_skip_whitespace(s, &c);

   *x = stbi__pnm_getinteger(s, &c); // read width
   stbi__pnm_skip_whitespace(s, &c);

   *y = stbi__pnm_getinteger(s, &c); // read height
   stbi__pnm_skip_whitespace(s, &c);

   maxv = stbi__pnm_getinteger(s, &c);  // read max value

   ಆದರೆ (maxv > 255)
      ಹಿಂತಿರುಗಿ stbi__err("max value > 255", "PPM image not 8-bit");
   ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__info_main(stbi__context *s, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_JPEG
   ಆದರೆ (stbi__jpeg_info(s, x, y, comp)) ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNG
   ಆದರೆ (stbi__png_info(s, x, y, comp))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_GIF
   ಆದರೆ (stbi__gif_info(s, x, y, comp))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_BMP
   ಆದರೆ (stbi__bmp_info(s, x, y, comp))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PSD
   ಆದರೆ (stbi__psd_info(s, x, y, comp))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PIC
   ಆದರೆ (stbi__pic_info(s, x, y, comp))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNM
   ಆದರೆ (stbi__pnm_info(s, x, y, comp))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_HDR
   ಆದರೆ (stbi__hdr_info(s, x, y, comp))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   // test tga last because it's a crappy test!
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_TGA
   ಆದರೆ (stbi__tga_info(s, x, y, comp))
       ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ
   ಹಿಂತಿರುಗಿ stbi__err("unknown image type", "Image not of any known type, or corrupt");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stbi__is_16_main(stbi__context *s)
{
   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PNG
   ಆದರೆ (stbi__png_is16(s))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_PSD
   ಆದರೆ (stbi__psd_is16(s))  ಹಿಂತಿರುಗಿ 1;
   #ಆದರೆ_ಅಂತ್ಯ

   ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STBI_NO_STDIO
STBIDEF ಪೂರ್ಣಾಂಕ stbi_info(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
    FILE *f = stbi__fopen(filename, "rb");
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
    ಆದರೆ (!f) ಹಿಂತಿರುಗಿ stbi__err("can't fopen", "Unable to open file");
    ಫಲಿತಾಂಶ = stbi_info_from_file(f, x, y, comp);
    fclose(f);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_info_from_file(FILE *f, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   ಪೂರ್ಣಾಂಕ r;
   stbi__context s;
   ದೀರ್ಘ pos = ftell(f);
   stbi_ಆರಂಭ_file(&s, f);
   r = stbi__info_main(&s,x,y,comp);
   fseek(f,pos,SEEK_SET);
   ಹಿಂತಿರುಗಿ r;
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_is_16_ಬಿಟ್(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
    FILE *f = stbi__fopen(filename, "rb");
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
    ಆದರೆ (!f) ಹಿಂತಿರುಗಿ stbi__err("can't fopen", "Unable to open file");
    ಫಲಿತಾಂಶ = stbi_is_16_ಬಿಟ್_from_file(f);
    fclose(f);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_is_16_ಬಿಟ್_from_file(FILE *f)
{
   ಪೂರ್ಣಾಂಕ r;
   stbi__context s;
   ದೀರ್ಘ pos = ftell(f);
   stbi_ಆರಂಭ_file(&s, f);
   r = stbi__is_16_main(&s);
   fseek(f,pos,SEEK_SET);
   ಹಿಂತಿರುಗಿ r;
}
#ಆದರೆ_ಅಂತ್ಯ // !STBI_NO_STDIO

STBIDEF ಪೂರ್ಣಾಂಕ stbi_info_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   stbi__context s;
   stbi_ಆರಂಭ_mem(&s,ಬಫರ್,len);
   ಹಿಂತಿರುಗಿ stbi__info_main(&s,x,y,comp);
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_info_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *c, ಶೂನ್ಯ *user, ಪೂರ್ಣಾಂಕ *x, ಪೂರ್ಣಾಂಕ *y, ಪೂರ್ಣಾಂಕ *comp)
{
   stbi__context s;
   stbi_ಆರಂಭ_callbacks(&s, (stbi_io_callbacks *) c, user);
   ಹಿಂತಿರುಗಿ stbi__info_main(&s,x,y,comp);
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_is_16_ಬಿಟ್_from_ಸ್ಮರಣೆ(stbi_uc ಸ್ಥಿರಾಂಕ *ಬಫರ್, ಪೂರ್ಣಾಂಕ len)
{
   stbi__context s;
   stbi_ಆರಂಭ_mem(&s,ಬಫರ್,len);
   ಹಿಂತಿರುಗಿ stbi__is_16_main(&s);
}

STBIDEF ಪೂರ್ಣಾಂಕ stbi_is_16_ಬಿಟ್_from_callbacks(stbi_io_callbacks ಸ್ಥಿರಾಂಕ *c, ಶೂನ್ಯ *user)
{
   stbi__context s;
   stbi_ಆರಂಭ_callbacks(&s, (stbi_io_callbacks *) c, user);
   ಹಿಂತಿರುಗಿ stbi__is_16_main(&s);
}

#ಆದರೆ_ಅಂತ್ಯ // STB_IMAGE_IMPLEMENTATION

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
