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

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

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


#ifndef STBI_NO_STDIO
#અન્તર્ભાવ <stdio.h>
#endif // STBI_NO_STDIO

#define STBI_VERSION 1

ગણના
{
   STBI_default = 0, // only used for desired_channels

   STBI_grey       = 1,
   STBI_grey_alpha = 2,
   STBI_rgb        = 3,
   STBI_rgb_alpha  = 4
};

પ્રકારવ્યાખ્યા unsigned અક્ષર stbi_uc;
પ્રકારવ્યાખ્યા unsigned short stbi_us;

#ifdef __cplusplus
બાહ્ય "C" {
#endif

#ifdef STB_IMAGE_STATIC
#define STBIDEF સ્થિર
#નહીંતર
#define STBIDEF બાહ્ય
#endif

//////////////////////////////////////////////////////////////////////////////
//
// PRIMARY API - works on images of any type
//

//
// load image by filename, open file, or memory buffer
//

પ્રકારવ્યાખ્યા સંરચના
{
   પૂર્ણાંક      (*પઠ)  (શૂન્ય *user,અક્ષર *data,પૂર્ણાંક size);   // fill 'data' with 'size' bytes.  return number of bytes actually read
   શૂન્ય     (*skip)  (શૂન્ય *user,પૂર્ણાંક n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
   પૂર્ણાંક      (*eof)   (શૂન્ય *user);                       // returns nonzero if we are at end of file/data
} stbi_io_callbacks;

////////////////////////////////////
//
// 8-bits-per-channel interface
//

STBIDEF stbi_uc *stbi_load_from_memory   (stbi_uc           અચલ *buffer, પૂર્ણાંક len   , પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
STBIDEF stbi_uc *stbi_load_from_callbacks(stbi_io_callbacks અચલ *clbk  , શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
#ifndef STBI_NO_GIF
STBIDEF stbi_uc *stbi_load_gif_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક **delays, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *z, પૂર્ણાંક *comp, પૂર્ણાંક req_comp);
#endif


#ifndef STBI_NO_STDIO
STBIDEF stbi_uc *stbi_load            (અક્ષર અચલ *filename, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
STBIDEF stbi_uc *stbi_load_from_file  (FILE *f, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
// for stbi_load_from_file, file pointer is left pointing immediately after image
#endif

////////////////////////////////////
//
// 16-bits-per-channel interface
//

STBIDEF stbi_us *stbi_load_16_from_memory   (stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
STBIDEF stbi_us *stbi_load_16_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);

#ifndef STBI_NO_STDIO
STBIDEF stbi_us *stbi_load_16          (અક્ષર અચલ *filename, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
STBIDEF stbi_us *stbi_load_from_file_16(FILE *f, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
#endif

////////////////////////////////////
//
// float-per-channel interface
//
#ifndef STBI_NO_LINEAR
   STBIDEF દશાંશ *stbi_loadf_from_memory     (stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
   STBIDEF દશાંશ *stbi_loadf_from_callbacks  (stbi_io_callbacks અચલ *clbk, શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y,  પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);

   #ifndef STBI_NO_STDIO
   STBIDEF દશાંશ *stbi_loadf            (અક્ષર અચલ *filename, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
   STBIDEF દશાંશ *stbi_loadf_from_file  (FILE *f, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels);
   #endif
#endif

#ifndef STBI_NO_HDR
   STBIDEF શૂન્ય   stbi_hdr_to_ldr_gamma(દશાંશ gamma);
   STBIDEF શૂન્ય   stbi_hdr_to_ldr_scale(દશાંશ scale);
#endif // STBI_NO_HDR

#ifndef STBI_NO_LINEAR
   STBIDEF શૂન્ય   stbi_ldr_to_hdr_gamma(દશાંશ gamma);
   STBIDEF શૂન્ય   stbi_ldr_to_hdr_scale(દશાંશ scale);
#endif // STBI_NO_LINEAR

// stbi_is_hdr is always defined, but always returns false if STBI_NO_HDR
STBIDEF પૂર્ણાંક    stbi_is_hdr_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user);
STBIDEF પૂર્ણાંક    stbi_is_hdr_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len);
#ifndef STBI_NO_STDIO
STBIDEF પૂર્ણાંક      stbi_is_hdr          (અક્ષર અચલ *filename);
STBIDEF પૂર્ણાંક      stbi_is_hdr_from_file(FILE *f);
#endif // STBI_NO_STDIO


// get a VERY brief reason for failure
// NOT THREADSAFE
STBIDEF અચલ અક્ષર *stbi_failure_reason  (શૂન્ય);

// free the loaded image -- this is just free()
STBIDEF શૂન્ય     stbi_image_free      (શૂન્ય *retval_from_stbi_load);

// get image dimensions & components without fully decoding
STBIDEF પૂર્ણાંક      stbi_info_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
STBIDEF પૂર્ણાંક      stbi_info_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
STBIDEF પૂર્ણાંક      stbi_is_16_bit_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len);
STBIDEF પૂર્ણાંક      stbi_is_16_bit_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user);

#ifndef STBI_NO_STDIO
STBIDEF પૂર્ણાંક      stbi_info               (અક્ષર અચલ *filename,     પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
STBIDEF પૂર્ણાંક      stbi_info_from_file     (FILE *f,                  પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
STBIDEF પૂર્ણાંક      stbi_is_16_bit          (અક્ષર અચલ *filename);
STBIDEF પૂર્ણાંક      stbi_is_16_bit_from_file(FILE *f);
#endif



// for image formats that explicitly notate that they have premultiplied alpha,
// we just return the colors as stored in the file. set this flag to force
// unpremultiplication. results are undefined if the unpremultiply overflow.
STBIDEF શૂન્ય stbi_set_unpremultiply_on_load(પૂર્ણાંક flag_true_if_should_unpremultiply);

// indicate whether we should process iphone images back to canonical format,
// or just pass them through "as-is"
STBIDEF શૂન્ય stbi_convert_iphone_png_to_rgb(પૂર્ણાંક flag_true_if_should_convert);

// flip the image vertically, so the first pixel in the output array is the bottom left
STBIDEF શૂન્ય stbi_set_flip_vertically_on_load(પૂર્ણાંક flag_true_if_should_flip);

// ZLIB client - used by PNG, available for other purposes

STBIDEF અક્ષર *stbi_zlib_decode_malloc_guesssize(અચલ અક્ષર *buffer, પૂર્ણાંક len, પૂર્ણાંક initial_size, પૂર્ણાંક *outlen);
STBIDEF અક્ષર *stbi_zlib_decode_malloc_guesssize_headerflag(અચલ અક્ષર *buffer, પૂર્ણાંક len, પૂર્ણાંક initial_size, પૂર્ણાંક *outlen, પૂર્ણાંક parse_header);
STBIDEF અક્ષર *stbi_zlib_decode_malloc(અચલ અક્ષર *buffer, પૂર્ણાંક len, પૂર્ણાંક *outlen);
STBIDEF પૂર્ણાંક   stbi_zlib_decode_buffer(અક્ષર *obuffer, પૂર્ણાંક olen, અચલ અક્ષર *ibuffer, પૂર્ણાંક ilen);

STBIDEF અક્ષર *stbi_zlib_decode_noheader_malloc(અચલ અક્ષર *buffer, પૂર્ણાંક len, પૂર્ણાંક *outlen);
STBIDEF પૂર્ણાંક   stbi_zlib_decode_noheader_buffer(અક્ષર *obuffer, પૂર્ણાંક olen, અચલ અક્ષર *ibuffer, પૂર્ણાંક ilen);


#ifdef __cplusplus
}
#endif

//
//
////   end header file   /////////////////////////////////////////////////////
#endif // STBI_INCLUDE_STB_IMAGE_H

#ifdef STB_IMAGE_IMPLEMENTATION

#જો defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
   #ifndef STBI_ONLY_JPEG
   #define STBI_NO_JPEG
   #endif
   #ifndef STBI_ONLY_PNG
   #define STBI_NO_PNG
   #endif
   #ifndef STBI_ONLY_BMP
   #define STBI_NO_BMP
   #endif
   #ifndef STBI_ONLY_PSD
   #define STBI_NO_PSD
   #endif
   #ifndef STBI_ONLY_TGA
   #define STBI_NO_TGA
   #endif
   #ifndef STBI_ONLY_GIF
   #define STBI_NO_GIF
   #endif
   #ifndef STBI_ONLY_HDR
   #define STBI_NO_HDR
   #endif
   #ifndef STBI_ONLY_PIC
   #define STBI_NO_PIC
   #endif
   #ifndef STBI_ONLY_PNM
   #define STBI_NO_PNM
   #endif
#endif

#જો defined(STBI_NO_PNG) && !defined(STBI_SUPPORT_ZLIB) && !defined(STBI_NO_ZLIB)
#define STBI_NO_ZLIB
#endif


#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <stddef.h> // ptrdiff_t on osx
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <limits.h>

#જો !defined(STBI_NO_LINEAR) || !defined(STBI_NO_HDR)
#અન્તર્ભાવ <math.h>  // ldexp, pow
#endif

#ifndef STBI_NO_STDIO
#અન્તર્ભાવ <stdio.h>
#endif

#ifndef STBI_ASSERT
#અન્તર્ભાવ <assert.h>
#define STBI_ASSERT(x) પ્રતિજ્ઞા(x)
#endif


#ifndef _MSC_VER
   #ifdef __cplusplus
   #define stbi_inline અંતઃરેખ
   #નહીંતર
   #define stbi_inline
   #endif
#નહીંતર
   #define stbi_inline __forceinline
#endif


#ifdef _MSC_VER
પ્રકારવ્યાખ્યા unsigned short stbi__uint16;
પ્રકારવ્યાખ્યા   signed short stbi__int16;
પ્રકારવ્યાખ્યા unsigned પૂર્ણાંક   stbi__uint32;
પ્રકારવ્યાખ્યા   signed પૂર્ણાંક   stbi__int32;
#નહીંતર
#અન્તર્ભાવ <stdint.h>
પ્રકારવ્યાખ્યા uint16_t stbi__uint16;
પ્રકારવ્યાખ્યા int16_t  stbi__int16;
પ્રકારવ્યાખ્યા uint32_t stbi__uint32;
પ્રકારવ્યાખ્યા int32_t  stbi__int32;
#endif

// should produce compiler error if size is wrong
પ્રકારવ્યાખ્યા unsigned અક્ષર validate_uint32[કદ(stbi__uint32)==4 ? 1 : -1];

#ifdef _MSC_VER
#define STBI_NOTUSED(v)  (શૂન્ય)(v)
#નહીંતર
#define STBI_NOTUSED(v)  (શૂન્ય)કદ(v)
#endif

#ifdef _MSC_VER
#define STBI_HAS_LROTL
#endif

#ifdef STBI_HAS_LROTL
   #define stbi_lrot(x,y)  _lrotl(x,y)
#નહીંતર
   #define stbi_lrot(x,y)  (((x) << (y)) | ((x) >> (32 - (y))))
#endif

#જો defined(STBI_MALLOC) && defined(STBI_FREE) && (defined(STBI_REALLOC) || defined(STBI_REALLOC_SIZED))
// ok
#અથવાજો !defined(STBI_MALLOC) && !defined(STBI_FREE) && !defined(STBI_REALLOC) && !defined(STBI_REALLOC_SIZED)
// ok
#નહીંતર
#error "Must define all or none of STBI_MALLOC, STBI_FREE, and STBI_REALLOC (or STBI_REALLOC_SIZED)."
#endif

#ifndef STBI_MALLOC
#define STBI_MALLOC(sz)           malloc(sz)
#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
#define STBI_FREE(p)              મુક્ત(p)
#endif

#ifndef STBI_REALLOC_SIZED
#define STBI_REALLOC_SIZED(p,oldsz,newsz) STBI_REALLOC(p,newsz)
#endif

// x86/x64 detection
#જો defined(__x86_64__) || defined(_M_X64)
#define STBI__X64_TARGET
#અથવાજો defined(__i386) || defined(_M_IX86)
#define STBI__X86_TARGET
#endif

#જો defined(__GNUC__) && defined(STBI__X86_TARGET) && !defined(__SSE2__) && !defined(STBI_NO_SIMD)
// gcc doesn't support sse2 intrinsics unless you compile with -msse2,
// which in turn means it gets to use SSE2 everywhere. This is unfortunate,
// but previous attempts to provide the SSE2 functions with runtime
// detection caused numerous issues. The way architecture extensions are
// exposed in GCC/Clang is, sadly, not really suited for one-file libs.
// New behavior: if compiled with -msse2, we use SSE2 without any
// detection; if not, we don't use it at all.
#define STBI_NO_SIMD
#endif

#જો defined(__MINGW32__) && defined(STBI__X86_TARGET) && !defined(STBI_MINGW_ENABLE_SSE2) && !defined(STBI_NO_SIMD)
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
#define STBI_NO_SIMD
#endif

#જો !defined(STBI_NO_SIMD) && (defined(STBI__X86_TARGET) || defined(STBI__X64_TARGET))
#define STBI_SSE2
#અન્તર્ભાવ <emmintrin.h>

#ifdef _MSC_VER

#જો _MSC_VER >= 1400  // not VC6
#અન્તર્ભાવ <intrin.h> // __cpuid
સ્થિર પૂર્ણાંક stbi__cpuid3(શૂન્ય)
{
   પૂર્ણાંક info[4];
   __cpuid(info,1);
   પાછા info[3];
}
#નહીંતર
સ્થિર પૂર્ણાંક stbi__cpuid3(શૂન્ય)
{
   પૂર્ણાંક res;
   __asm {
      mov  eax,1
      cpuid
      mov  res,edx
   }
   પાછા res;
}
#endif

#define STBI_SIMD_ALIGN(type, name) __declspec(align(16)) type name

સ્થિર પૂર્ણાંક stbi__sse2_available(શૂન્ય)
{
   પૂર્ણાંક info3 = stbi__cpuid3();
   પાછા ((info3 >> 26) & 1) != 0;
}
#નહીંતર // assume GCC-style if not VC++
#define STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))

સ્થિર પૂર્ણાંક stbi__sse2_available(શૂન્ય)
{
   // If we're even attempting to compile this on GCC/Clang, that means
   // -msse2 is on, which means the compiler is allowed to use SSE2
   // instructions at will, and so are we.
   પાછા 1;
}
#endif
#endif

// ARM NEON
#જો defined(STBI_NO_SIMD) && defined(STBI_NEON)
#undef STBI_NEON
#endif

#ifdef STBI_NEON
#અન્તર્ભાવ <arm_neon.h>
// assume GCC or Clang on ARM targets
#define STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))
#endif

#ifndef STBI_SIMD_ALIGN
#define STBI_SIMD_ALIGN(type, name) type name
#endif

///////////////////////////////////////////////
//
//  stbi__context struct and start_xxx functions

// stbi__context structure is our basic context used by all images, so it
// contains all the IO context, plus some basic image information
પ્રકારવ્યાખ્યા સંરચના
{
   stbi__uint32 img_x, img_y;
   પૂર્ણાંક img_n, img_out_n;

   stbi_io_callbacks io;
   શૂન્ય *io_user_data;

   પૂર્ણાંક read_from_callbacks;
   પૂર્ણાંક buflen;
   stbi_uc buffer_start[128];

   stbi_uc *img_buffer, *img_buffer_end;
   stbi_uc *img_buffer_original, *img_buffer_original_end;
} stbi__context;


સ્થિર શૂન્ય stbi__refill_buffer(stbi__context *s);

// initialize a memory-decode context
સ્થિર શૂન્ય stbi__start_mem(stbi__context *s, stbi_uc અચલ *buffer, પૂર્ણાંક len)
{
   s->io.પઠ = ખાલી;
   s->read_from_callbacks = 0;
   s->img_buffer = s->img_buffer_original = (stbi_uc *) buffer;
   s->img_buffer_end = s->img_buffer_original_end = (stbi_uc *) buffer+len;
}

// initialize a callback-based context
સ્થિર શૂન્ય stbi__start_callbacks(stbi__context *s, stbi_io_callbacks *c, શૂન્ય *user)
{
   s->io = *c;
   s->io_user_data = user;
   s->buflen = કદ(s->buffer_start);
   s->read_from_callbacks = 1;
   s->img_buffer_original = s->buffer_start;
   stbi__refill_buffer(s);
   s->img_buffer_original_end = s->img_buffer_end;
}

#ifndef STBI_NO_STDIO

સ્થિર પૂર્ણાંક stbi__stdio_read(શૂન્ય *user, અક્ષર *data, પૂર્ણાંક size)
{
   પાછા (પૂર્ણાંક) fread(data,1,size,(FILE*) user);
}

સ્થિર શૂન્ય stbi__stdio_skip(શૂન્ય *user, પૂર્ણાંક n)
{
   fseek((FILE*) user, n, SEEK_CUR);
}

સ્થિર પૂર્ણાંક stbi__stdio_eof(શૂન્ય *user)
{
   પાછા feof((FILE*) user);
}

સ્થિર stbi_io_callbacks stbi__stdio_callbacks =
{
   stbi__stdio_read,
   stbi__stdio_skip,
   stbi__stdio_eof,
};

સ્થિર શૂન્ય stbi__start_file(stbi__context *s, FILE *f)
{
   stbi__start_callbacks(s, &stbi__stdio_callbacks, (શૂન્ય *) f);
}

//static void stop_file(stbi__context *s) { }

#endif // !STBI_NO_STDIO

સ્થિર શૂન્ય stbi__rewind(stbi__context *s)
{
   // conceptually rewind SHOULD rewind to the beginning of the stream,
   // but we just rewind to the beginning of the initial buffer, because
   // we only use it after doing 'test', which only ever looks at at most 92 bytes
   s->img_buffer = s->img_buffer_original;
   s->img_buffer_end = s->img_buffer_original_end;
}

ગણના
{
   STBI_ORDER_RGB,
   STBI_ORDER_BGR
};

પ્રકારવ્યાખ્યા સંરચના
{
   પૂર્ણાંક bits_per_channel;
   પૂર્ણાંક અંક_channels;
   પૂર્ણાંક channel_order;
} stbi__result_info;

#ifndef STBI_NO_JPEG
સ્થિર પૂર્ણાંક      stbi__jpeg_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__jpeg_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર પૂર્ણાંક      stbi__jpeg_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
#endif

#ifndef STBI_NO_PNG
સ્થિર પૂર્ણાંક      stbi__png_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__png_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર પૂર્ણાંક      stbi__png_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
સ્થિર પૂર્ણાંક      stbi__png_is16(stbi__context *s);
#endif

#ifndef STBI_NO_BMP
સ્થિર પૂર્ણાંક      stbi__bmp_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__bmp_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર પૂર્ણાંક      stbi__bmp_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
#endif

#ifndef STBI_NO_TGA
સ્થિર પૂર્ણાંક      stbi__tga_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__tga_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર પૂર્ણાંક      stbi__tga_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
#endif

#ifndef STBI_NO_PSD
સ્થિર પૂર્ણાંક      stbi__psd_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__psd_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri, પૂર્ણાંક bpc);
સ્થિર પૂર્ણાંક      stbi__psd_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
સ્થિર પૂર્ણાંક      stbi__psd_is16(stbi__context *s);
#endif

#ifndef STBI_NO_HDR
સ્થિર પૂર્ણાંક      stbi__hdr_test(stbi__context *s);
સ્થિર દશાંશ   *stbi__hdr_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર પૂર્ણાંક      stbi__hdr_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
#endif

#ifndef STBI_NO_PIC
સ્થિર પૂર્ણાંક      stbi__pic_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__pic_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર પૂર્ણાંક      stbi__pic_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
#endif

#ifndef STBI_NO_GIF
સ્થિર પૂર્ણાંક      stbi__gif_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__gif_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર શૂન્ય    *stbi__load_gif_main(stbi__context *s, પૂર્ણાંક **delays, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *z, પૂર્ણાંક *comp, પૂર્ણાંક req_comp);
સ્થિર પૂર્ણાંક      stbi__gif_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
#endif

#ifndef STBI_NO_PNM
સ્થિર પૂર્ણાંક      stbi__pnm_test(stbi__context *s);
સ્થિર શૂન્ય    *stbi__pnm_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri);
સ્થિર પૂર્ણાંક      stbi__pnm_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp);
#endif

// this is not threadsafe
સ્થિર અચલ અક્ષર *stbi__g_failure_reason;

STBIDEF અચલ અક્ષર *stbi_failure_reason(શૂન્ય)
{
   પાછા stbi__g_failure_reason;
}

સ્થિર પૂર્ણાંક stbi__err(અચલ અક્ષર *str)
{
   stbi__g_failure_reason = str;
   પાછા 0;
}

સ્થિર શૂન્ય *stbi__malloc(size_t size)
{
    પાછા STBI_MALLOC(size);
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
સ્થિર પૂર્ણાંક stbi__addsizes_valid(પૂર્ણાંક a, પૂર્ણાંક b)
{
   જો (b < 0) પાછા 0;
   // now 0 <= b <= INT_MAX, hence also
   // 0 <= INT_MAX - b <= INTMAX.
   // And "a + b <= INT_MAX" (which might overflow) is the
   // same as a <= INT_MAX - b (no overflow)
   પાછા a <= INT_MAX - b;
}

// returns 1 if the product is valid, 0 on overflow.
// negative factors are considered invalid.
સ્થિર પૂર્ણાંક stbi__mul2sizes_valid(પૂર્ણાંક a, પૂર્ણાંક b)
{
   જો (a < 0 || b < 0) પાછા 0;
   જો (b == 0) પાછા 1; // mul-by-0 is always safe
   // portable way to check for no overflows in a*b
   પાછા a <= INT_MAX/b;
}

// returns 1 if "a*b + add" has no negative terms/factors and doesn't overflow
સ્થિર પૂર્ણાંક stbi__mad2sizes_valid(પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક add)
{
   પાછા stbi__mul2sizes_valid(a, b) && stbi__addsizes_valid(a*b, add);
}

// returns 1 if "a*b*c + add" has no negative terms/factors and doesn't overflow
સ્થિર પૂર્ણાંક stbi__mad3sizes_valid(પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક c, પૂર્ણાંક add)
{
   પાછા stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__addsizes_valid(a*b*c, add);
}

// returns 1 if "a*b*c*d + add" has no negative terms/factors and doesn't overflow
#જો !defined(STBI_NO_LINEAR) || !defined(STBI_NO_HDR)
સ્થિર પૂર્ણાંક stbi__mad4sizes_valid(પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક c, પૂર્ણાંક d, પૂર્ણાંક add)
{
   પાછા stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__mul2sizes_valid(a*b*c, d) && stbi__addsizes_valid(a*b*c*d, add);
}
#endif

// mallocs with size overflow checking
સ્થિર શૂન્ય *stbi__malloc_mad2(પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક add)
{
   જો (!stbi__mad2sizes_valid(a, b, add)) પાછા ખાલી;
   પાછા stbi__malloc(a*b + add);
}

સ્થિર શૂન્ય *stbi__malloc_mad3(પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક c, પૂર્ણાંક add)
{
   જો (!stbi__mad3sizes_valid(a, b, c, add)) પાછા ખાલી;
   પાછા stbi__malloc(a*b*c + add);
}

#જો !defined(STBI_NO_LINEAR) || !defined(STBI_NO_HDR)
સ્થિર શૂન્ય *stbi__malloc_mad4(પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક c, પૂર્ણાંક d, પૂર્ણાંક add)
{
   જો (!stbi__mad4sizes_valid(a, b, c, d, add)) પાછા ખાલી;
   પાછા stbi__malloc(a*b*c*d + add);
}
#endif

// stbi__err - error
// stbi__errpf - error returning pointer to float
// stbi__errpuc - error returning pointer to unsigned char

#ifdef STBI_NO_FAILURE_STRINGS
   #define stbi__err(x,y)  0
#અથવાજો defined(STBI_FAILURE_USERMSG)
   #define stbi__err(x,y)  stbi__err(y)
#નહીંતર
   #define stbi__err(x,y)  stbi__err(x)
#endif

#define stbi__errpf(x,y)   ((દશાંશ *)(size_t) (stbi__err(x,y)?ખાલી:ખાલી))
#define stbi__errpuc(x,y)  ((unsigned અક્ષર *)(size_t) (stbi__err(x,y)?ખાલી:ખાલી))

STBIDEF શૂન્ય stbi_image_free(શૂન્ય *retval_from_stbi_load)
{
   STBI_FREE(retval_from_stbi_load);
}

#ifndef STBI_NO_LINEAR
સ્થિર દશાંશ   *stbi__ldr_to_hdr(stbi_uc *data, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક comp);
#endif

#ifndef STBI_NO_HDR
સ્થિર stbi_uc *stbi__hdr_to_ldr(દશાંશ   *data, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક comp);
#endif

સ્થિર પૂર્ણાંક stbi__vertically_flip_on_load = 0;

STBIDEF શૂન્ય stbi_set_flip_vertically_on_load(પૂર્ણાંક flag_true_if_should_flip)
{
    stbi__vertically_flip_on_load = flag_true_if_should_flip;
}

સ્થિર શૂન્ય *stbi__load_main(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri, પૂર્ણાંક bpc)
{
   memset(ri, 0, કદ(*ri)); // make sure it's initialized if we add new fields
   ri->bits_per_channel = 8; // default is 8 so most paths don't have to be changed
   ri->channel_order = STBI_ORDER_RGB; // all current input & output are this, but this is here so we can add BGR order
   ri->અંક_channels = 0;

   #ifndef STBI_NO_JPEG
   જો (stbi__jpeg_test(s)) પાછા stbi__jpeg_load(s,x,y,comp,req_comp, ri);
   #endif
   #ifndef STBI_NO_PNG
   જો (stbi__png_test(s))  પાછા stbi__png_load(s,x,y,comp,req_comp, ri);
   #endif
   #ifndef STBI_NO_BMP
   જો (stbi__bmp_test(s))  પાછા stbi__bmp_load(s,x,y,comp,req_comp, ri);
   #endif
   #ifndef STBI_NO_GIF
   જો (stbi__gif_test(s))  પાછા stbi__gif_load(s,x,y,comp,req_comp, ri);
   #endif
   #ifndef STBI_NO_PSD
   જો (stbi__psd_test(s))  પાછા stbi__psd_load(s,x,y,comp,req_comp, ri, bpc);
   #endif
   #ifndef STBI_NO_PIC
   જો (stbi__pic_test(s))  પાછા stbi__pic_load(s,x,y,comp,req_comp, ri);
   #endif
   #ifndef STBI_NO_PNM
   જો (stbi__pnm_test(s))  પાછા stbi__pnm_load(s,x,y,comp,req_comp, ri);
   #endif

   #ifndef STBI_NO_HDR
   જો (stbi__hdr_test(s)) {
      દશાંશ *hdr = stbi__hdr_load(s, x,y,comp,req_comp, ri);
      પાછા stbi__hdr_to_ldr(hdr, *x, *y, req_comp ? req_comp : *comp);
   }
   #endif

   #ifndef STBI_NO_TGA
   // test tga last because it's a crappy test!
   જો (stbi__tga_test(s))
      પાછા stbi__tga_load(s,x,y,comp,req_comp, ri);
   #endif

   પાછા stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
}

સ્થિર stbi_uc *stbi__convert_16_to_8(stbi__uint16 *orig, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક channels)
{
   પૂર્ણાંક i;
   પૂર્ણાંક img_len = w * h * channels;
   stbi_uc *reduced;

   reduced = (stbi_uc *) stbi__malloc(img_len);
   જો (reduced == ખાલી) પાછા stbi__errpuc("outofmem", "Out of memory");

   માટે (i = 0; i < img_len; ++i)
      reduced[i] = (stbi_uc)((orig[i] >> 8) & 0xFF); // top half of each byte is sufficient approx of 16->8 bit scaling

   STBI_FREE(orig);
   પાછા reduced;
}

સ્થિર stbi__uint16 *stbi__convert_8_to_16(stbi_uc *orig, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક channels)
{
   પૂર્ણાંક i;
   પૂર્ણાંક img_len = w * h * channels;
   stbi__uint16 *enlarged;

   enlarged = (stbi__uint16 *) stbi__malloc(img_len*2);
   જો (enlarged == ખાલી) પાછા (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");

   માટે (i = 0; i < img_len; ++i)
      enlarged[i] = (stbi__uint16)((orig[i] << 8) + orig[i]); // replicate to high and low byte, maps 0->0, 255->0xffff

   STBI_FREE(orig);
   પાછા enlarged;
}

સ્થિર શૂન્ય stbi__vertical_flip(શૂન્ય *image, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક bytes_per_pixel)
{
   પૂર્ણાંક row;
   size_t bytes_per_row = (size_t)w * bytes_per_pixel;
   stbi_uc temp[2048];
   stbi_uc *bytes = (stbi_uc *)image;

   માટે (row = 0; row < (h>>1); row++) {
      stbi_uc *row0 = bytes + row*bytes_per_row;
      stbi_uc *row1 = bytes + (h - row - 1)*bytes_per_row;
      // swap row0 with row1
      size_t bytes_ડાબું = bytes_per_row;
      જ્યારે (bytes_ડાબું) {
         size_t bytes_copy = (bytes_ડાબું < કદ(temp)) ? bytes_ડાબું : કદ(temp);
         memcpy(temp, row0, bytes_copy);
         memcpy(row0, row1, bytes_copy);
         memcpy(row1, temp, bytes_copy);
         row0 += bytes_copy;
         row1 += bytes_copy;
         bytes_ડાબું -= bytes_copy;
      }
   }
}

સ્થિર શૂન્ય stbi__vertical_flip_slices(શૂન્ય *image, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક z, પૂર્ણાંક bytes_per_pixel)
{
   પૂર્ણાંક slice;
   પૂર્ણાંક slice_size = w * h * bytes_per_pixel;

   stbi_uc *bytes = (stbi_uc *)image;
   માટે (slice = 0; slice < z; ++slice) {
      stbi__vertical_flip(bytes, w, h, bytes_per_pixel); 
      bytes += slice_size; 
   }
}

સ્થિર unsigned અક્ષર *stbi__load_and_postprocess_8bit(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__result_info ri;
   શૂન્ય *result = stbi__load_main(s, x, y, comp, req_comp, &ri, 8);

   જો (result == ખાલી)
      પાછા ખાલી;

   જો (ri.bits_per_channel != 8) {
      STBI_ASSERT(ri.bits_per_channel == 16);
      result = stbi__convert_16_to_8((stbi__uint16 *) result, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.bits_per_channel = 8;
   }

   // @TODO: move stbi__convert_format to here

   જો (stbi__vertically_flip_on_load) {
      પૂર્ણાંક channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(result, *x, *y, channels * કદ(stbi_uc));
   }

   પાછા (unsigned અક્ષર *) result;
}

સ્થિર stbi__uint16 *stbi__load_and_postprocess_16bit(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__result_info ri;
   શૂન્ય *result = stbi__load_main(s, x, y, comp, req_comp, &ri, 16);

   જો (result == ખાલી)
      પાછા ખાલી;

   જો (ri.bits_per_channel != 16) {
      STBI_ASSERT(ri.bits_per_channel == 8);
      result = stbi__convert_8_to_16((stbi_uc *) result, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.bits_per_channel = 16;
   }

   // @TODO: move stbi__convert_format16 to here
   // @TODO: special case RGB-to-Y (and RGBA-to-YA) for 8-bit-to-16-bit case to keep more precision

   જો (stbi__vertically_flip_on_load) {
      પૂર્ણાંક channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(result, *x, *y, channels * કદ(stbi__uint16));
   }

   પાછા (stbi__uint16 *) result;
}

#જો !defined(STBI_NO_HDR) || !defined(STBI_NO_LINEAR)
સ્થિર શૂન્ય stbi__float_postprocess(દશાંશ *result, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   જો (stbi__vertically_flip_on_load && result != ખાલી) {
      પૂર્ણાંક channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(result, *x, *y, channels * કદ(દશાંશ));
   }
}
#endif

#ifndef STBI_NO_STDIO

સ્થિર FILE *stbi__fopen(અક્ષર અચલ *filename, અક્ષર અચલ *mode)
{
   FILE *f;
#જો defined(_MSC_VER) && _MSC_VER >= 1400
   જો (0 != fopen_s(&f, filename, mode))
      f=0;
#નહીંતર
   f = fopen(filename, mode);
#endif
   પાછા f;
}


STBIDEF stbi_uc *stbi_load(અક્ષર અચલ *filename, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   unsigned અક્ષર *result;
   જો (!f) પાછા stbi__errpuc("can't fopen", "Unable to open file");
   result = stbi_load_from_file(f,x,y,comp,req_comp);
   fclose(f);
   પાછા result;
}

STBIDEF stbi_uc *stbi_load_from_file(FILE *f, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   unsigned અક્ષર *result;
   stbi__context s;
   stbi__start_file(&s,f);
   result = stbi__load_and_postprocess_8bit(&s,x,y,comp,req_comp);
   જો (result) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (પૂર્ણાંક) (s.img_buffer_end - s.img_buffer), SEEK_CUR);
   }
   પાછા result;
}

STBIDEF stbi__uint16 *stbi_load_from_file_16(FILE *f, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__uint16 *result;
   stbi__context s;
   stbi__start_file(&s,f);
   result = stbi__load_and_postprocess_16bit(&s,x,y,comp,req_comp);
   જો (result) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (પૂર્ણાંક) (s.img_buffer_end - s.img_buffer), SEEK_CUR);
   }
   પાછા result;
}

STBIDEF stbi_us *stbi_load_16(અક્ષર અચલ *filename, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   stbi__uint16 *result;
   જો (!f) પાછા (stbi_us *) stbi__errpuc("can't fopen", "Unable to open file");
   result = stbi_load_from_file_16(f,x,y,comp,req_comp);
   fclose(f);
   પાછા result;
}


#endif //!STBI_NO_STDIO

STBIDEF stbi_us *stbi_load_16_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels)
{
   stbi__context s;
   stbi__start_mem(&s,buffer,len);
   પાછા stbi__load_and_postprocess_16bit(&s,x,y,channels_in_file,desired_channels);
}

STBIDEF stbi_us *stbi_load_16_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *channels_in_file, પૂર્ણાંક desired_channels)
{
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *)clbk, user);
   પાછા stbi__load_and_postprocess_16bit(&s,x,y,channels_in_file,desired_channels);
}

STBIDEF stbi_uc *stbi_load_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__context s;
   stbi__start_mem(&s,buffer,len);
   પાછા stbi__load_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

STBIDEF stbi_uc *stbi_load_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   પાછા stbi__load_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

#ifndef STBI_NO_GIF
STBIDEF stbi_uc *stbi_load_gif_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક **delays, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *z, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   unsigned અક્ષર *result;
   stbi__context s; 
   stbi__start_mem(&s,buffer,len); 
   
   result = (unsigned અક્ષર*) stbi__load_gif_main(&s, delays, x, y, z, comp, req_comp);
   જો (stbi__vertically_flip_on_load) {
      stbi__vertical_flip_slices( result, *x, *y, *z, *comp ); 
   }

   પાછા result; 
}
#endif

#ifndef STBI_NO_LINEAR
સ્થિર દશાંશ *stbi__loadf_main(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   unsigned અક્ષર *data;
   #ifndef STBI_NO_HDR
   જો (stbi__hdr_test(s)) {
      stbi__result_info ri;
      દશાંશ *hdr_data = stbi__hdr_load(s,x,y,comp,req_comp, &ri);
      જો (hdr_data)
         stbi__float_postprocess(hdr_data,x,y,comp,req_comp);
      પાછા hdr_data;
   }
   #endif
   data = stbi__load_and_postprocess_8bit(s, x, y, comp, req_comp);
   જો (data)
      પાછા stbi__ldr_to_hdr(data, *x, *y, req_comp ? req_comp : *comp);
   પાછા stbi__errpf("unknown image type", "Image not of any known type, or corrupt");
}

STBIDEF દશાંશ *stbi_loadf_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__context s;
   stbi__start_mem(&s,buffer,len);
   પાછા stbi__loadf_main(&s,x,y,comp,req_comp);
}

STBIDEF દશાંશ *stbi_loadf_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   પાછા stbi__loadf_main(&s,x,y,comp,req_comp);
}

#ifndef STBI_NO_STDIO
STBIDEF દશાંશ *stbi_loadf(અક્ષર અચલ *filename, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   દશાંશ *result;
   FILE *f = stbi__fopen(filename, "rb");
   જો (!f) પાછા stbi__errpf("can't fopen", "Unable to open file");
   result = stbi_loadf_from_file(f,x,y,comp,req_comp);
   fclose(f);
   પાછા result;
}

STBIDEF દશાંશ *stbi_loadf_from_file(FILE *f, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   stbi__context s;
   stbi__start_file(&s,f);
   પાછા stbi__loadf_main(&s,x,y,comp,req_comp);
}
#endif // !STBI_NO_STDIO

#endif // !STBI_NO_LINEAR

// these is-hdr-or-not is defined independent of whether STBI_NO_LINEAR is
// defined, for API simplicity; if STBI_NO_LINEAR is defined, it always
// reports false!

STBIDEF પૂર્ણાંક stbi_is_hdr_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len)
{
   #ifndef STBI_NO_HDR
   stbi__context s;
   stbi__start_mem(&s,buffer,len);
   પાછા stbi__hdr_test(&s);
   #નહીંતર
   STBI_NOTUSED(buffer);
   STBI_NOTUSED(len);
   પાછા 0;
   #endif
}

#ifndef STBI_NO_STDIO
STBIDEF પૂર્ણાંક      stbi_is_hdr          (અક્ષર અચલ *filename)
{
   FILE *f = stbi__fopen(filename, "rb");
   પૂર્ણાંક result=0;
   જો (f) {
      result = stbi_is_hdr_from_file(f);
      fclose(f);
   }
   પાછા result;
}

STBIDEF પૂર્ણાંક stbi_is_hdr_from_file(FILE *f)
{
   #ifndef STBI_NO_HDR
   long pos = ftell(f);
   પૂર્ણાંક res;
   stbi__context s;
   stbi__start_file(&s,f);
   res = stbi__hdr_test(&s);
   fseek(f, pos, SEEK_SET);
   પાછા res;
   #નહીંતર
   STBI_NOTUSED(f);
   પાછા 0;
   #endif
}
#endif // !STBI_NO_STDIO

STBIDEF પૂર્ણાંક      stbi_is_hdr_from_callbacks(stbi_io_callbacks અચલ *clbk, શૂન્ય *user)
{
   #ifndef STBI_NO_HDR
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   પાછા stbi__hdr_test(&s);
   #નહીંતર
   STBI_NOTUSED(clbk);
   STBI_NOTUSED(user);
   પાછા 0;
   #endif
}

#ifndef STBI_NO_LINEAR
સ્થિર દશાંશ stbi__l2h_gamma=2.2f, stbi__l2h_scale=1.0f;

STBIDEF શૂન્ય   stbi_ldr_to_hdr_gamma(દશાંશ gamma) { stbi__l2h_gamma = gamma; }
STBIDEF શૂન્ય   stbi_ldr_to_hdr_scale(દશાંશ scale) { stbi__l2h_scale = scale; }
#endif

સ્થિર દશાંશ stbi__h2l_gamma_i=1.0f/2.2f, stbi__h2l_scale_i=1.0f;

STBIDEF શૂન્ય   stbi_hdr_to_ldr_gamma(દશાંશ gamma) { stbi__h2l_gamma_i = 1/gamma; }
STBIDEF શૂન્ય   stbi_hdr_to_ldr_scale(દશાંશ scale) { stbi__h2l_scale_i = 1/scale; }


//////////////////////////////////////////////////////////////////////////////
//
// Common code used by all image loaders
//

ગણના
{
   STBI__SCAN_load=0,
   STBI__SCAN_type,
   STBI__SCAN_header
};

સ્થિર શૂન્ય stbi__refill_buffer(stbi__context *s)
{
   પૂર્ણાંક n = (s->io.પઠ)(s->io_user_data,(અક્ષર*)s->buffer_start,s->buflen);
   જો (n == 0) {
      // at end of file, treat same as if from memory, but need to handle case
      // where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file
      s->read_from_callbacks = 0;
      s->img_buffer = s->buffer_start;
      s->img_buffer_end = s->buffer_start+1;
      *s->img_buffer = 0;
   } નહીંતર {
      s->img_buffer = s->buffer_start;
      s->img_buffer_end = s->buffer_start + n;
   }
}

stbi_inline સ્થિર stbi_uc stbi__get8(stbi__context *s)
{
   જો (s->img_buffer < s->img_buffer_end)
      પાછા *s->img_buffer++;
   જો (s->read_from_callbacks) {
      stbi__refill_buffer(s);
      પાછા *s->img_buffer++;
   }
   પાછા 0;
}

stbi_inline સ્થિર પૂર્ણાંક stbi__at_eof(stbi__context *s)
{
   જો (s->io.પઠ) {
      જો (!(s->io.eof)(s->io_user_data)) પાછા 0;
      // if feof() is true, check if buffer = end
      // special case: we've only got the special 0 character at the end
      જો (s->read_from_callbacks == 0) પાછા 1;
   }

   પાછા s->img_buffer >= s->img_buffer_end;
}

સ્થિર શૂન્ય stbi__skip(stbi__context *s, પૂર્ણાંક n)
{
   જો (n < 0) {
      s->img_buffer = s->img_buffer_end;
      પાછા;
   }
   જો (s->io.પઠ) {
      પૂર્ણાંક blen = (પૂર્ણાંક) (s->img_buffer_end - s->img_buffer);
      જો (blen < n) {
         s->img_buffer = s->img_buffer_end;
         (s->io.skip)(s->io_user_data, n - blen);
         પાછા;
      }
   }
   s->img_buffer += n;
}

સ્થિર પૂર્ણાંક stbi__getn(stbi__context *s, stbi_uc *buffer, પૂર્ણાંક n)
{
   જો (s->io.પઠ) {
      પૂર્ણાંક blen = (પૂર્ણાંક) (s->img_buffer_end - s->img_buffer);
      જો (blen < n) {
         પૂર્ણાંક res, ગણતરી;

         memcpy(buffer, s->img_buffer, blen);

         ગણતરી = (s->io.પઠ)(s->io_user_data, (અક્ષર*) buffer + blen, n - blen);
         res = (ગણતરી == (n-blen));
         s->img_buffer = s->img_buffer_end;
         પાછા res;
      }
   }

   જો (s->img_buffer+n <= s->img_buffer_end) {
      memcpy(buffer, s->img_buffer, n);
      s->img_buffer += n;
      પાછા 1;
   } નહીંતર
      પાછા 0;
}

સ્થિર પૂર્ણાંક stbi__get16be(stbi__context *s)
{
   પૂર્ણાંક z = stbi__get8(s);
   પાછા (z << 8) + stbi__get8(s);
}

સ્થિર stbi__uint32 stbi__get32be(stbi__context *s)
{
   stbi__uint32 z = stbi__get16be(s);
   પાછા (z << 16) + stbi__get16be(s);
}

#જો defined(STBI_NO_BMP) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF)
// nothing
#નહીંતર
સ્થિર પૂર્ણાંક stbi__get16le(stbi__context *s)
{
   પૂર્ણાંક z = stbi__get8(s);
   પાછા z + (stbi__get8(s) << 8);
}
#endif

#ifndef STBI_NO_BMP
સ્થિર stbi__uint32 stbi__get32le(stbi__context *s)
{
   stbi__uint32 z = stbi__get16le(s);
   પાછા z + (stbi__get16le(s) << 16);
}
#endif

#define STBI__BYTECAST(x)  ((stbi_uc) ((x) & 255))  // truncate int to byte without warnings


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

સ્થિર stbi_uc stbi__compute_y(પૂર્ણાંક r, પૂર્ણાંક g, પૂર્ણાંક b)
{
   પાછા (stbi_uc) (((r*77) + (g*150) +  (29*b)) >> 8);
}

સ્થિર unsigned અક્ષર *stbi__convert_format(unsigned અક્ષર *data, પૂર્ણાંક img_n, પૂર્ણાંક req_comp, unsigned પૂર્ણાંક x, unsigned પૂર્ણાંક y)
{
   પૂર્ણાંક i,j;
   unsigned અક્ષર *good;

   જો (req_comp == img_n) પાછા data;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (unsigned અક્ષર *) stbi__malloc_mad3(req_comp, x, y, 0);
   જો (good == ખાલી) {
      STBI_FREE(data);
      પાછા stbi__errpuc("outofmem", "Out of memory");
   }

   માટે (j=0; j < (પૂર્ણાંક) y; ++j) {
      unsigned અક્ષર *src  = data + j * x * img_n   ;
      unsigned અક્ષર *dest = good + j * x * req_comp;

      #define STBI__COMBO(a,b)  ((a)*8+(b))
      #define STBI__CASE(a,b)   સ્થિતિ STBI__COMBO(a,b): માટે(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      પસંદગી (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=255;                                     } તોડો;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } તોડો;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=255;                     } તોડો;
         STBI__CASE(2,1) { dest[0]=src[0];                                                  } તોડો;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } તોડો;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                  } તોડો;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255;        } તોડો;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } તોડો;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = 255;    } તોડો;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } તોડો;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = src[3]; } તોડો;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                    } તોડો;
         મૂળભૂત: STBI_ASSERT(0);
      }
      #undef STBI__CASE
   }

   STBI_FREE(data);
   પાછા good;
}

સ્થિર stbi__uint16 stbi__compute_y_16(પૂર્ણાંક r, પૂર્ણાંક g, પૂર્ણાંક b)
{
   પાછા (stbi__uint16) (((r*77) + (g*150) +  (29*b)) >> 8);
}

સ્થિર stbi__uint16 *stbi__convert_format16(stbi__uint16 *data, પૂર્ણાંક img_n, પૂર્ણાંક req_comp, unsigned પૂર્ણાંક x, unsigned પૂર્ણાંક y)
{
   પૂર્ણાંક i,j;
   stbi__uint16 *good;

   જો (req_comp == img_n) પાછા data;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (stbi__uint16 *) stbi__malloc(req_comp * x * y * 2);
   જો (good == ખાલી) {
      STBI_FREE(data);
      પાછા (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");
   }

   માટે (j=0; j < (પૂર્ણાંક) y; ++j) {
      stbi__uint16 *src  = data + j * x * img_n   ;
      stbi__uint16 *dest = good + j * x * req_comp;

      #define STBI__COMBO(a,b)  ((a)*8+(b))
      #define STBI__CASE(a,b)   સ્થિતિ STBI__COMBO(a,b): માટે(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      પસંદગી (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=0xffff;                                     } તોડો;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } તોડો;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=0xffff;                     } તોડો;
         STBI__CASE(2,1) { dest[0]=src[0];                                                     } તોડો;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } તોડો;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                     } તોડો;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=0xffff;        } તોડો;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } તોડો;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = 0xffff; } તોડો;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } તોડો;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = src[3]; } તોડો;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                       } તોડો;
         મૂળભૂત: STBI_ASSERT(0);
      }
      #undef STBI__CASE
   }

   STBI_FREE(data);
   પાછા good;
}

#ifndef STBI_NO_LINEAR
સ્થિર દશાંશ   *stbi__ldr_to_hdr(stbi_uc *data, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક comp)
{
   પૂર્ણાંક i,k,n;
   દશાંશ *output;
   જો (!data) પાછા ખાલી;
   output = (દશાંશ *) stbi__malloc_mad4(x, y, comp, કદ(દશાંશ), 0);
   જો (output == ખાલી) { STBI_FREE(data); પાછા stbi__errpf("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   જો (comp & 1) n = comp; નહીંતર n = comp-1;
   માટે (i=0; i < x*y; ++i) {
      માટે (k=0; k < n; ++k) {
         output[i*comp + k] = (દશાંશ) (pow(data[i*comp+k]/255.0f, stbi__l2h_gamma) * stbi__l2h_scale);
      }
      જો (k < comp) output[i*comp + k] = data[i*comp+k]/255.0f;
   }
   STBI_FREE(data);
   પાછા output;
}
#endif

#ifndef STBI_NO_HDR
#define stbi__float2int(x)   ((પૂર્ણાંક) (x))
સ્થિર stbi_uc *stbi__hdr_to_ldr(દશાંશ   *data, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક comp)
{
   પૂર્ણાંક i,k,n;
   stbi_uc *output;
   જો (!data) પાછા ખાલી;
   output = (stbi_uc *) stbi__malloc_mad3(x, y, comp, 0);
   જો (output == ખાલી) { STBI_FREE(data); પાછા stbi__errpuc("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   જો (comp & 1) n = comp; નહીંતર n = comp-1;
   માટે (i=0; i < x*y; ++i) {
      માટે (k=0; k < n; ++k) {
         દશાંશ z = (દશાંશ) pow(data[i*comp+k]*stbi__h2l_scale_i, stbi__h2l_gamma_i) * 255 + 0.5f;
         જો (z < 0) z = 0;
         જો (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
      જો (k < comp) {
         દશાંશ z = data[i*comp+k] * 255 + 0.5f;
         જો (z < 0) z = 0;
         જો (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
   }
   STBI_FREE(data);
   પાછા output;
}
#endif

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

#ifndef STBI_NO_JPEG

// huffman decoding acceleration
#define FAST_BITS   9  // larger handles more cases; smaller stomps less cache

પ્રકારવ્યાખ્યા સંરચના
{
   stbi_uc  fast[1 << FAST_BITS];
   // weirdly, repacking this into AoS is a 10% speed loss, instead of a win
   stbi__uint16 code[256];
   stbi_uc  કિંમતો[256];
   stbi_uc  size[257];
   unsigned પૂર્ણાંક maxcode[18];
   પૂર્ણાંક    delta[17];   // old 'firstsymbol' - old 'firstcode'
} stbi__huffman;

પ્રકારવ્યાખ્યા સંરચના
{
   stbi__context *s;
   stbi__huffman huff_dc[4];
   stbi__huffman huff_ac[4];
   stbi__uint16 dequant[4][64];
   stbi__int16 fast_ac[4][1 << FAST_BITS];

// sizes for components, interleaved MCUs
   પૂર્ણાંક img_h_max, img_v_max;
   પૂર્ણાંક img_mcu_x, img_mcu_y;
   પૂર્ણાંક img_mcu_w, img_mcu_h;

// definition of jpeg image component
   સંરચના
   {
      પૂર્ણાંક id;
      પૂર્ણાંક h,v;
      પૂર્ણાંક tq;
      પૂર્ણાંક hd,ha;
      પૂર્ણાંક dc_pred;

      પૂર્ણાંક x,y,w2,h2;
      stbi_uc *data;
      શૂન્ય *raw_data, *raw_coeff;
      stbi_uc *linebuf;
      short   *coeff;   // progressive only
      પૂર્ણાંક      coeff_w, coeff_h; // number of 8x8 coefficient blocks
   } img_comp[4];

   stbi__uint32   code_buffer; // jpeg entropy-coded buffer
   પૂર્ણાંક            code_bits;   // number of valid bits
   unsigned અક્ષર  marker;      // marker seen while filling entropy buffer
   પૂર્ણાંક            nomore;      // flag if we saw a marker so must stop

   પૂર્ણાંક            progressive;
   પૂર્ણાંક            spec_start;
   પૂર્ણાંક            spec_end;
   પૂર્ણાંક            succ_ઉપલું;
   પૂર્ણાંક            succ_નીચલું;
   પૂર્ણાંક            eob_run;
   પૂર્ણાંક            jfif;
   પૂર્ણાંક            app14_color_transform; // Adobe APP14 tag
   પૂર્ણાંક            rgb;

   પૂર્ણાંક scan_n, order[4];
   પૂર્ણાંક restart_interval, todo;

// kernels
   શૂન્ય (*idct_block_kernel)(stbi_uc *out, પૂર્ણાંક out_stride, short data[64]);
   શૂન્ય (*YCbCr_to_RGB_kernel)(stbi_uc *out, અચલ stbi_uc *y, અચલ stbi_uc *pcb, અચલ stbi_uc *pcr, પૂર્ણાંક ગણતરી, પૂર્ણાંક step);
   stbi_uc *(*resample_row_hv_2_kernel)(stbi_uc *out, stbi_uc *in_near, stbi_uc *in_far, પૂર્ણાંક w, પૂર્ણાંક hs);
} stbi__jpeg;

સ્થિર પૂર્ણાંક stbi__build_huffman(stbi__huffman *h, પૂર્ણાંક *ગણતરી)
{
   પૂર્ણાંક i,j,k=0;
   unsigned પૂર્ણાંક code;
   // build size list for each symbol (from JPEG spec)
   માટે (i=0; i < 16; ++i)
      માટે (j=0; j < ગણતરી[i]; ++j)
         h->size[k++] = (stbi_uc) (i+1);
   h->size[k] = 0;

   // compute actual symbols (from jpeg spec)
   code = 0;
   k = 0;
   માટે(j=1; j <= 16; ++j) {
      // compute delta to add to code to compute symbol id
      h->delta[j] = k - code;
      જો (h->size[k] == j) {
         જ્યારે (h->size[k] == j)
            h->code[k++] = (stbi__uint16) (code++);
         જો (code-1 >= (1u << j)) પાછા stbi__err("bad code lengths","Corrupt JPEG");
      }
      // compute largest code + 1 for this size, preshifted as needed later
      h->maxcode[j] = code << (16-j);
      code <<= 1;
   }
   h->maxcode[j] = 0xffffffff;

   // build non-spec acceleration table; 255 is flag for not-accelerated
   memset(h->fast, 255, 1 << FAST_BITS);
   માટે (i=0; i < k; ++i) {
      પૂર્ણાંક s = h->size[i];
      જો (s <= FAST_BITS) {
         પૂર્ણાંક c = h->code[i] << (FAST_BITS-s);
         પૂર્ણાંક m = 1 << (FAST_BITS-s);
         માટે (j=0; j < m; ++j) {
            h->fast[c+j] = (stbi_uc) i;
         }
      }
   }
   પાછા 1;
}

// build a table that decodes both magnitude and value of small ACs in
// one go.
સ્થિર શૂન્ય stbi__build_fast_ac(stbi__int16 *fast_ac, stbi__huffman *h)
{
   પૂર્ણાંક i;
   માટે (i=0; i < (1 << FAST_BITS); ++i) {
      stbi_uc fast = h->fast[i];
      fast_ac[i] = 0;
      જો (fast < 255) {
         પૂર્ણાંક rs = h->કિંમતો[fast];
         પૂર્ણાંક run = (rs >> 4) & 15;
         પૂર્ણાંક magbits = rs & 15;
         પૂર્ણાંક len = h->size[fast];

         જો (magbits && len + magbits <= FAST_BITS) {
            // magnitude code followed by receive_extend code
            પૂર્ણાંક k = ((i << len) & ((1 << FAST_BITS) - 1)) >> (FAST_BITS - magbits);
            પૂર્ણાંક m = 1 << (magbits - 1);
            જો (k < m) k += (~0U << magbits) + 1;
            // if the result is small enough, we can fit it in fast_ac table
            જો (k >= -128 && k <= 127)
               fast_ac[i] = (stbi__int16) ((k * 256) + (run * 16) + (len + magbits));
         }
      }
   }
}

સ્થિર શૂન્ય stbi__grow_buffer_unsafe(stbi__jpeg *j)
{
   કરો {
      unsigned પૂર્ણાંક b = j->nomore ? 0 : stbi__get8(j->s);
      જો (b == 0xff) {
         પૂર્ણાંક c = stbi__get8(j->s);
         જ્યારે (c == 0xff) c = stbi__get8(j->s); // consume fill bytes
         જો (c != 0) {
            j->marker = (unsigned અક્ષર) c;
            j->nomore = 1;
            પાછા;
         }
      }
      j->code_buffer |= b << (24 - j->code_bits);
      j->code_bits += 8;
   } જ્યારે (j->code_bits <= 24);
}

// (1 << n) - 1
સ્થિર અચલ stbi__uint32 stbi__bmask[17]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};

// decode a jpeg huffman value from the bitstream
stbi_inline સ્થિર પૂર્ણાંક stbi__jpeg_huff_decode(stbi__jpeg *j, stbi__huffman *h)
{
   unsigned પૂર્ણાંક temp;
   પૂર્ણાંક c,k;

   જો (j->code_bits < 16) stbi__grow_buffer_unsafe(j);

   // look at the top FAST_BITS and determine what symbol ID it is,
   // if the code is <= FAST_BITS
   c = (j->code_buffer >> (32 - FAST_BITS)) & ((1 << FAST_BITS)-1);
   k = h->fast[c];
   જો (k < 255) {
      પૂર્ણાંક s = h->size[k];
      જો (s > j->code_bits)
         પાછા -1;
      j->code_buffer <<= s;
      j->code_bits -= s;
      પાછા h->કિંમતો[k];
   }

   // naive test is to shift the code_buffer down so k bits are
   // valid, then test against maxcode. To speed this up, we've
   // preshifted maxcode left so that it has (16-k) 0s at the
   // end; in other words, regardless of the number of bits, it
   // wants to be compared against something shifted to have 16;
   // that way we don't need to shift inside the loop.
   temp = j->code_buffer >> 16;
   માટે (k=FAST_BITS+1 ; ; ++k)
      જો (temp < h->maxcode[k])
         તોડો;
   જો (k == 17) {
      // error! code not found
      j->code_bits -= 16;
      પાછા -1;
   }

   જો (k > j->code_bits)
      પાછા -1;

   // convert the huffman code to the symbol id
   c = ((j->code_buffer >> (32 - k)) & stbi__bmask[k]) + h->delta[k];
   STBI_ASSERT((((j->code_buffer) >> (32 - h->size[c])) & stbi__bmask[h->size[c]]) == h->code[c]);

   // convert the id to a symbol
   j->code_bits -= k;
   j->code_buffer <<= k;
   પાછા h->કિંમતો[c];
}

// bias[n] = (-1<<n) + 1
સ્થિર અચલ પૂર્ણાંક stbi__jbias[16] = {0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767};

// combined JPEG 'receive' and JPEG 'extend', since baseline
// always extends everything it receives.
stbi_inline સ્થિર પૂર્ણાંક stbi__extend_receive(stbi__jpeg *j, પૂર્ણાંક n)
{
   unsigned પૂર્ણાંક k;
   પૂર્ણાંક sgn;
   જો (j->code_bits < n) stbi__grow_buffer_unsafe(j);

   sgn = (stbi__int32)j->code_buffer >> 31; // sign bit is always in MSB
   k = stbi_lrot(j->code_buffer, n);
   STBI_ASSERT(n >= 0 && n < (પૂર્ણાંક) (કદ(stbi__bmask)/કદ(*stbi__bmask)));
   j->code_buffer = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->code_bits -= n;
   પાછા k + (stbi__jbias[n] & ~sgn);
}

// get some unsigned bits
stbi_inline સ્થિર પૂર્ણાંક stbi__jpeg_get_bits(stbi__jpeg *j, પૂર્ણાંક n)
{
   unsigned પૂર્ણાંક k;
   જો (j->code_bits < n) stbi__grow_buffer_unsafe(j);
   k = stbi_lrot(j->code_buffer, n);
   j->code_buffer = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->code_bits -= n;
   પાછા k;
}

stbi_inline સ્થિર પૂર્ણાંક stbi__jpeg_get_bit(stbi__jpeg *j)
{
   unsigned પૂર્ણાંક k;
   જો (j->code_bits < 1) stbi__grow_buffer_unsafe(j);
   k = j->code_buffer;
   j->code_buffer <<= 1;
   --j->code_bits;
   પાછા k & 0x80000000;
}

// given a value that's at position X in the zigzag stream,
// where does it appear in the 8x8 matrix coded as row-major?
સ્થિર અચલ stbi_uc stbi__jpeg_dezigzag[64+15] =
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
સ્થિર પૂર્ણાંક stbi__jpeg_decode_block(stbi__jpeg *j, short data[64], stbi__huffman *hdc, stbi__huffman *hac, stbi__int16 *fac, પૂર્ણાંક b, stbi__uint16 *dequant)
{
   પૂર્ણાંક diff,dc,k;
   પૂર્ણાંક t;

   જો (j->code_bits < 16) stbi__grow_buffer_unsafe(j);
   t = stbi__jpeg_huff_decode(j, hdc);
   જો (t < 0) પાછા stbi__err("bad huffman code","Corrupt JPEG");

   // 0 all the ac values now so we can do it 32-bits at a time
   memset(data,0,64*કદ(data[0]));

   diff = t ? stbi__extend_receive(j, t) : 0;
   dc = j->img_comp[b].dc_pred + diff;
   j->img_comp[b].dc_pred = dc;
   data[0] = (short) (dc * dequant[0]);

   // decode AC components, see JPEG spec
   k = 1;
   કરો {
      unsigned પૂર્ણાંક zig;
      પૂર્ણાંક c,r,s;
      જો (j->code_bits < 16) stbi__grow_buffer_unsafe(j);
      c = (j->code_buffer >> (32 - FAST_BITS)) & ((1 << FAST_BITS)-1);
      r = fac[c];
      જો (r) { // fast-AC path
         k += (r >> 4) & 15; // run
         s = r & 15; // combined length
         j->code_buffer <<= s;
         j->code_bits -= s;
         // decode into unzigzag'd location
         zig = stbi__jpeg_dezigzag[k++];
         data[zig] = (short) ((r >> 8) * dequant[zig]);
      } નહીંતર {
         પૂર્ણાંક rs = stbi__jpeg_huff_decode(j, hac);
         જો (rs < 0) પાછા stbi__err("bad huffman code","Corrupt JPEG");
         s = rs & 15;
         r = rs >> 4;
         જો (s == 0) {
            જો (rs != 0xf0) તોડો; // end block
            k += 16;
         } નહીંતર {
            k += r;
            // decode into unzigzag'd location
            zig = stbi__jpeg_dezigzag[k++];
            data[zig] = (short) (stbi__extend_receive(j,s) * dequant[zig]);
         }
      }
   } જ્યારે (k < 64);
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__jpeg_decode_block_prog_dc(stbi__jpeg *j, short data[64], stbi__huffman *hdc, પૂર્ણાંક b)
{
   પૂર્ણાંક diff,dc;
   પૂર્ણાંક t;
   જો (j->spec_end != 0) પાછા stbi__err("can't merge dc and ac", "Corrupt JPEG");

   જો (j->code_bits < 16) stbi__grow_buffer_unsafe(j);

   જો (j->succ_ઉપલું == 0) {
      // first scan for DC coefficient, must be first
      memset(data,0,64*કદ(data[0])); // 0 all the ac values now
      t = stbi__jpeg_huff_decode(j, hdc);
      diff = t ? stbi__extend_receive(j, t) : 0;

      dc = j->img_comp[b].dc_pred + diff;
      j->img_comp[b].dc_pred = dc;
      data[0] = (short) (dc << j->succ_નીચલું);
   } નહીંતર {
      // refinement scan for DC coefficient
      જો (stbi__jpeg_get_bit(j))
         data[0] += (short) (1 << j->succ_નીચલું);
   }
   પાછા 1;
}

// @OPTIMIZE: store non-zigzagged during the decode passes,
// and only de-zigzag when dequantizing
સ્થિર પૂર્ણાંક stbi__jpeg_decode_block_prog_ac(stbi__jpeg *j, short data[64], stbi__huffman *hac, stbi__int16 *fac)
{
   પૂર્ણાંક k;
   જો (j->spec_start == 0) પાછા stbi__err("can't merge dc and ac", "Corrupt JPEG");

   જો (j->succ_ઉપલું == 0) {
      પૂર્ણાંક shift = j->succ_નીચલું;

      જો (j->eob_run) {
         --j->eob_run;
         પાછા 1;
      }

      k = j->spec_start;
      કરો {
         unsigned પૂર્ણાંક zig;
         પૂર્ણાંક c,r,s;
         જો (j->code_bits < 16) stbi__grow_buffer_unsafe(j);
         c = (j->code_buffer >> (32 - FAST_BITS)) & ((1 << FAST_BITS)-1);
         r = fac[c];
         જો (r) { // fast-AC path
            k += (r >> 4) & 15; // run
            s = r & 15; // combined length
            j->code_buffer <<= s;
            j->code_bits -= s;
            zig = stbi__jpeg_dezigzag[k++];
            data[zig] = (short) ((r >> 8) << shift);
         } નહીંતર {
            પૂર્ણાંક rs = stbi__jpeg_huff_decode(j, hac);
            જો (rs < 0) પાછા stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            જો (s == 0) {
               જો (r < 15) {
                  j->eob_run = (1 << r);
                  જો (r)
                     j->eob_run += stbi__jpeg_get_bits(j, r);
                  --j->eob_run;
                  તોડો;
               }
               k += 16;
            } નહીંતર {
               k += r;
               zig = stbi__jpeg_dezigzag[k++];
               data[zig] = (short) (stbi__extend_receive(j,s) << shift);
            }
         }
      } જ્યારે (k <= j->spec_end);
   } નહીંતર {
      // refinement scan for these AC coefficients

      short bit = (short) (1 << j->succ_નીચલું);

      જો (j->eob_run) {
         --j->eob_run;
         માટે (k = j->spec_start; k <= j->spec_end; ++k) {
            short *p = &data[stbi__jpeg_dezigzag[k]];
            જો (*p != 0)
               જો (stbi__jpeg_get_bit(j))
                  જો ((*p & bit)==0) {
                     જો (*p > 0)
                        *p += bit;
                     નહીંતર
                        *p -= bit;
                  }
         }
      } નહીંતર {
         k = j->spec_start;
         કરો {
            પૂર્ણાંક r,s;
            પૂર્ણાંક rs = stbi__jpeg_huff_decode(j, hac); // @OPTIMIZE see if we can use the fast path here, advance-by-r is so slow, eh
            જો (rs < 0) પાછા stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            જો (s == 0) {
               જો (r < 15) {
                  j->eob_run = (1 << r) - 1;
                  જો (r)
                     j->eob_run += stbi__jpeg_get_bits(j, r);
                  r = 64; // force end of block
               } નહીંતર {
                  // r=15 s=0 should write 16 0s, so we just do
                  // a run of 15 0s and then write s (which is 0),
                  // so we don't have to do anything special here
               }
            } નહીંતર {
               જો (s != 1) પાછા stbi__err("bad huffman code", "Corrupt JPEG");
               // sign bit
               જો (stbi__jpeg_get_bit(j))
                  s = bit;
               નહીંતર
                  s = -bit;
            }

            // advance by r
            જ્યારે (k <= j->spec_end) {
               short *p = &data[stbi__jpeg_dezigzag[k++]];
               જો (*p != 0) {
                  જો (stbi__jpeg_get_bit(j))
                     જો ((*p & bit)==0) {
                        જો (*p > 0)
                           *p += bit;
                        નહીંતર
                           *p -= bit;
                     }
               } નહીંતર {
                  જો (r == 0) {
                     *p = (short) s;
                     તોડો;
                  }
                  --r;
               }
            }
         } જ્યારે (k <= j->spec_end);
      }
   }
   પાછા 1;
}

// take a -128..127 value and stbi__clamp it and convert to 0..255
stbi_inline સ્થિર stbi_uc stbi__clamp(પૂર્ણાંક x)
{
   // trick to use a single test to catch both cases
   જો ((unsigned પૂર્ણાંક) x > 255) {
      જો (x < 0) પાછા 0;
      જો (x > 255) પાછા 255;
   }
   પાછા (stbi_uc) x;
}

#define stbi__f2f(x)  ((પૂર્ણાંક) (((x) * 4096 + 0.5)))
#define stbi__fsh(x)  ((x) * 4096)

// derived from jidctint -- DCT_ISLOW
#define STBI__IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7) \
   પૂર્ણાંક t0,t1,t2,t3,p1,p2,p3,p4,p5,x0,x1,x2,x3; \
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

સ્થિર શૂન્ય stbi__idct_block(stbi_uc *out, પૂર્ણાંક out_stride, short data[64])
{
   પૂર્ણાંક i,val[64],*v=val;
   stbi_uc *o;
   short *d = data;

   // columns
   માટે (i=0; i < 8; ++i,++d, ++v) {
      // if all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing
      જો (d[ 8]==0 && d[16]==0 && d[24]==0 && d[32]==0
           && d[40]==0 && d[48]==0 && d[56]==0) {
         //    no shortcut                 0     seconds
         //    (1|2|3|4|5|6|7)==0          0     seconds
         //    all separate               -0.047 seconds
         //    1 && 2|3 && 4|5 && 6|7:    -0.047 seconds
         પૂર્ણાંક dcterm = d[0]*4;
         v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
      } નહીંતર {
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

   માટે (i=0, v=val, o=out; i < 8; ++i,v+=8,o+=out_stride) {
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

#ifdef STBI_SSE2
// sse2 integer IDCT. not the fastest possible implementation but it
// produces bit-identical results to the generic C version so it's
// fully "transparent".
સ્થિર શૂન્ય stbi__idct_simd(stbi_uc *out, પૂર્ણાંક out_stride, short data[64])
{
   // This is constructed to match our regular (generic) integer IDCT exactly.
   __m128i row0, row1, row2, row3, row4, row5, row6, row7;
   __m128i tmp;

   // dot product constant: even elems=x, odd elems=y
   #define dct_const(x,y)  _mm_setr_epi16((x),(y),(x),(y),(x),(y),(x),(y))

   // out(0) = c0[even]*x + c0[odd]*y   (c0, x, y 16-bit, out 32-bit)
   // out(1) = c1[even]*x + c1[odd]*y
   #define dct_rot(out0,out1, x,y,c0,c1) \
      __m128i c0##lo = _mm_unpacklo_epi16((x),(y)); \
      __m128i c0##hi = _mm_unpackhi_epi16((x),(y)); \
      __m128i out0##_l = _mm_madd_epi16(c0##lo, c0); \
      __m128i out0##_h = _mm_madd_epi16(c0##hi, c0); \
      __m128i out1##_l = _mm_madd_epi16(c0##lo, c1); \
      __m128i out1##_h = _mm_madd_epi16(c0##hi, c1)

   // out = in << 12  (in 16-bit, out 32-bit)
   #define dct_widen(out, in) \
      __m128i out##_l = _mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), (in)), 4); \
      __m128i out##_h = _mm_srai_epi32(_mm_unpackhi_epi16(_mm_setzero_si128(), (in)), 4)

   // wide add
   #define dct_wadd(out, a, b) \
      __m128i out##_l = _mm_add_epi32(a##_l, b##_l); \
      __m128i out##_h = _mm_add_epi32(a##_h, b##_h)

   // wide sub
   #define dct_wsub(out, a, b) \
      __m128i out##_l = _mm_sub_epi32(a##_l, b##_l); \
      __m128i out##_h = _mm_sub_epi32(a##_h, b##_h)

   // butterfly a/b, add bias, then shift by "s" and pack
   #define dct_bfly32o(out0, out1, a,b,bias,s) \
      { \
         __m128i abiased_l = _mm_add_epi32(a##_l, bias); \
         __m128i abiased_h = _mm_add_epi32(a##_h, bias); \
         dct_wadd(sum, abiased, b); \
         dct_wsub(dif, abiased, b); \
         out0 = _mm_packs_epi32(_mm_srai_epi32(sum_l, s), _mm_srai_epi32(sum_h, s)); \
         out1 = _mm_packs_epi32(_mm_srai_epi32(dif_l, s), _mm_srai_epi32(dif_h, s)); \
      }

   // 8-bit interleave step (for transposes)
   #define dct_interleave8(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi8(a, b); \
      b = _mm_unpackhi_epi8(tmp, b)

   // 16-bit interleave step (for transposes)
   #define dct_interleave16(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi16(a, b); \
      b = _mm_unpackhi_epi16(tmp, b)

   #define dct_pass(bias,shift) \
      { \
         /* even part */ \
         dct_rot(t2e,t3e, row2,row6, rot0_0,rot0_1); \
         __m128i sum04 = _mm_add_epi16(row0, row4); \
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
         __m128i sum17 = _mm_add_epi16(row1, row7); \
         __m128i sum35 = _mm_add_epi16(row3, row5); \
         dct_rot(y4o,y5o, sum17,sum35, rot1_0,rot1_1); \
         dct_wadd(x4, y0o, y4o); \
         dct_wadd(x5, y1o, y5o); \
         dct_wadd(x6, y2o, y5o); \
         dct_wadd(x7, y3o, y4o); \
         dct_bfly32o(row0,row7, x0,x7,bias,shift); \
         dct_bfly32o(row1,row6, x1,x6,bias,shift); \
         dct_bfly32o(row2,row5, x2,x5,bias,shift); \
         dct_bfly32o(row3,row4, x3,x4,bias,shift); \
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

   // load
   row0 = _mm_load_si128((અચલ __m128i *) (data + 0*8));
   row1 = _mm_load_si128((અચલ __m128i *) (data + 1*8));
   row2 = _mm_load_si128((અચલ __m128i *) (data + 2*8));
   row3 = _mm_load_si128((અચલ __m128i *) (data + 3*8));
   row4 = _mm_load_si128((અચલ __m128i *) (data + 4*8));
   row5 = _mm_load_si128((અચલ __m128i *) (data + 5*8));
   row6 = _mm_load_si128((અચલ __m128i *) (data + 6*8));
   row7 = _mm_load_si128((અચલ __m128i *) (data + 7*8));

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
      _mm_storel_epi64((__m128i *) out, p0); out += out_stride;
      _mm_storel_epi64((__m128i *) out, _mm_shuffle_epi32(p0, 0x4e)); out += out_stride;
      _mm_storel_epi64((__m128i *) out, p2); out += out_stride;
      _mm_storel_epi64((__m128i *) out, _mm_shuffle_epi32(p2, 0x4e)); out += out_stride;
      _mm_storel_epi64((__m128i *) out, p1); out += out_stride;
      _mm_storel_epi64((__m128i *) out, _mm_shuffle_epi32(p1, 0x4e)); out += out_stride;
      _mm_storel_epi64((__m128i *) out, p3); out += out_stride;
      _mm_storel_epi64((__m128i *) out, _mm_shuffle_epi32(p3, 0x4e));
   }

#undef dct_const
#undef dct_rot
#undef dct_widen
#undef dct_wadd
#undef dct_wsub
#undef dct_bfly32o
#undef dct_interleave8
#undef dct_interleave16
#undef dct_pass
}

#endif // STBI_SSE2

#ifdef STBI_NEON

// NEON integer IDCT. should produce bit-identical
// results to the generic C version.
સ્થિર શૂન્ય stbi__idct_simd(stbi_uc *out, પૂર્ણાંક out_stride, short data[64])
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

#define dct_long_mul(out, inq, coeff) \
   int32x4_t out##_l = vmull_s16(vget_નીચલું_s16(inq), coeff); \
   int32x4_t out##_h = vmull_s16(vget_ઉપલું_s16(inq), coeff)

#define dct_long_mac(out, acc, inq, coeff) \
   int32x4_t out##_l = vmlal_s16(acc##_l, vget_નીચલું_s16(inq), coeff); \
   int32x4_t out##_h = vmlal_s16(acc##_h, vget_ઉપલું_s16(inq), coeff)

#define dct_widen(out, inq) \
   int32x4_t out##_l = vshll_n_s16(vget_નીચલું_s16(inq), 12); \
   int32x4_t out##_h = vshll_n_s16(vget_ઉપલું_s16(inq), 12)

// wide add
#define dct_wadd(out, a, b) \
   int32x4_t out##_l = vaddq_s32(a##_l, b##_l); \
   int32x4_t out##_h = vaddq_s32(a##_h, b##_h)

// wide sub
#define dct_wsub(out, a, b) \
   int32x4_t out##_l = vsubq_s32(a##_l, b##_l); \
   int32x4_t out##_h = vsubq_s32(a##_h, b##_h)

// butterfly a/b, then shift using "shiftop" by "s" and pack
#define dct_bfly32o(out0,out1, a,b,shiftop,s) \
   { \
      dct_wadd(sum, a, b); \
      dct_wsub(dif, a, b); \
      out0 = vcombine_s16(shiftop(sum_l, s), shiftop(sum_h, s)); \
      out1 = vcombine_s16(shiftop(dif_l, s), shiftop(dif_h, s)); \
   }

#define dct_pass(shiftop, shift) \
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
      dct_bfly32o(row0,row7, x0,x7,shiftop,shift); \
      dct_bfly32o(row1,row6, x1,x6,shiftop,shift); \
      dct_bfly32o(row2,row5, x2,x5,shiftop,shift); \
      dct_bfly32o(row3,row4, x3,x4,shiftop,shift); \
   }

   // load
   row0 = vld1q_s16(data + 0*8);
   row1 = vld1q_s16(data + 1*8);
   row2 = vld1q_s16(data + 2*8);
   row3 = vld1q_s16(data + 3*8);
   row4 = vld1q_s16(data + 4*8);
   row5 = vld1q_s16(data + 5*8);
   row6 = vld1q_s16(data + 6*8);
   row7 = vld1q_s16(data + 7*8);

   // add DC bias
   row0 = vaddq_s16(row0, vsetq_lane_s16(1024, vdupq_n_s16(0), 0));

   // column pass
   dct_pass(vrshrn_n_s32, 10);

   // 16bit 8x8 transpose
   {
// these three map to a single VTRN.16, VTRN.32, and VSWP, respectively.
// whether compilers actually get this is another story, sadly.
#define dct_trn16(x, y) { int16x8x2_t t = vtrnq_s16(x, y); x = t.val[0]; y = t.val[1]; }
#define dct_trn32(x, y) { int32x4x2_t t = vtrnq_s32(vreinterpretq_s32_s16(x), vreinterpretq_s32_s16(y)); x = vreinterpretq_s16_s32(t.val[0]); y = vreinterpretq_s16_s32(t.val[1]); }
#define dct_trn64(x, y) { int16x8_t x0 = x; int16x8_t y0 = y; x = vcombine_s16(vget_નીચલું_s16(x0), vget_નીચલું_s16(y0)); y = vcombine_s16(vget_ઉપલું_s16(x0), vget_ઉપલું_s16(y0)); }

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

#undef dct_trn16
#undef dct_trn32
#undef dct_trn64
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
#define dct_trn8_8(x, y) { uint8x8x2_t t = vtrn_u8(x, y); x = t.val[0]; y = t.val[1]; }
#define dct_trn8_16(x, y) { uint16x4x2_t t = vtrn_u16(vreinterpret_u16_u8(x), vreinterpret_u16_u8(y)); x = vreinterpret_u8_u16(t.val[0]); y = vreinterpret_u8_u16(t.val[1]); }
#define dct_trn8_32(x, y) { uint32x2x2_t t = vtrn_u32(vreinterpret_u32_u8(x), vreinterpret_u32_u8(y)); x = vreinterpret_u8_u32(t.val[0]); y = vreinterpret_u8_u32(t.val[1]); }

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
      vst1_u8(out, p0); out += out_stride;
      vst1_u8(out, p1); out += out_stride;
      vst1_u8(out, p2); out += out_stride;
      vst1_u8(out, p3); out += out_stride;
      vst1_u8(out, p4); out += out_stride;
      vst1_u8(out, p5); out += out_stride;
      vst1_u8(out, p6); out += out_stride;
      vst1_u8(out, p7);

#undef dct_trn8_8
#undef dct_trn8_16
#undef dct_trn8_32
   }

#undef dct_long_mul
#undef dct_long_mac
#undef dct_widen
#undef dct_wadd
#undef dct_wsub
#undef dct_bfly32o
#undef dct_pass
}

#endif // STBI_NEON

#define STBI__MARKER_none  0xff
// if there's a pending marker from the entropy stream, return that
// otherwise, fetch from the stream and get a marker. if there's no
// marker, return 0xff, which is never a valid marker value
સ્થિર stbi_uc stbi__get_marker(stbi__jpeg *j)
{
   stbi_uc x;
   જો (j->marker != STBI__MARKER_none) { x = j->marker; j->marker = STBI__MARKER_none; પાછા x; }
   x = stbi__get8(j->s);
   જો (x != 0xff) પાછા STBI__MARKER_none;
   જ્યારે (x == 0xff)
      x = stbi__get8(j->s); // consume repeated 0xff fill bytes
   પાછા x;
}

// in each scan, we'll have scan_n components, and the order
// of the components is specified by order[]
#define STBI__RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)

// after a restart interval, stbi__jpeg_reset the entropy decoder and
// the dc prediction
સ્થિર શૂન્ય stbi__jpeg_reset(stbi__jpeg *j)
{
   j->code_bits = 0;
   j->code_buffer = 0;
   j->nomore = 0;
   j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = j->img_comp[3].dc_pred = 0;
   j->marker = STBI__MARKER_none;
   j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
   j->eob_run = 0;
   // no more than 1<<31 MCUs if no restart_interal? that's plenty safe,
   // since we don't even allow 1<<30 pixels
}

સ્થિર પૂર્ણાંક stbi__parse_entropy_coded_data(stbi__jpeg *z)
{
   stbi__jpeg_reset(z);
   જો (!z->progressive) {
      જો (z->scan_n == 1) {
         પૂર્ણાંક i,j;
         STBI_SIMD_ALIGN(short, data[64]);
         પૂર્ણાંક n = z->order[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         પૂર્ણાંક w = (z->img_comp[n].x+7) >> 3;
         પૂર્ણાંક h = (z->img_comp[n].y+7) >> 3;
         માટે (j=0; j < h; ++j) {
            માટે (i=0; i < w; ++i) {
               પૂર્ણાંક ha = z->img_comp[n].ha;
               જો (!stbi__jpeg_decode_block(z, data, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) પાછા 0;
               z->idct_block_kernel(z->img_comp[n].data+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, data);
               // every data block is an MCU, so countdown the restart interval
               જો (--z->todo <= 0) {
                  જો (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                  // if it's NOT a restart, then just bail, so we get corrupt data
                  // rather than no data
                  જો (!STBI__RESTART(z->marker)) પાછા 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         પાછા 1;
      } નહીંતર { // interleaved
         પૂર્ણાંક i,j,k,x,y;
         STBI_SIMD_ALIGN(short, data[64]);
         માટે (j=0; j < z->img_mcu_y; ++j) {
            માટે (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               માટે (k=0; k < z->scan_n; ++k) {
                  પૂર્ણાંક n = z->order[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  માટે (y=0; y < z->img_comp[n].v; ++y) {
                     માટે (x=0; x < z->img_comp[n].h; ++x) {
                        પૂર્ણાંક x2 = (i*z->img_comp[n].h + x)*8;
                        પૂર્ણાંક y2 = (j*z->img_comp[n].v + y)*8;
                        પૂર્ણાંક ha = z->img_comp[n].ha;
                        જો (!stbi__jpeg_decode_block(z, data, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) પાછા 0;
                        z->idct_block_kernel(z->img_comp[n].data+z->img_comp[n].w2*y2+x2, z->img_comp[n].w2, data);
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               જો (--z->todo <= 0) {
                  જો (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                  જો (!STBI__RESTART(z->marker)) પાછા 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         પાછા 1;
      }
   } નહીંતર {
      જો (z->scan_n == 1) {
         પૂર્ણાંક i,j;
         પૂર્ણાંક n = z->order[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         પૂર્ણાંક w = (z->img_comp[n].x+7) >> 3;
         પૂર્ણાંક h = (z->img_comp[n].y+7) >> 3;
         માટે (j=0; j < h; ++j) {
            માટે (i=0; i < w; ++i) {
               short *data = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               જો (z->spec_start == 0) {
                  જો (!stbi__jpeg_decode_block_prog_dc(z, data, &z->huff_dc[z->img_comp[n].hd], n))
                     પાછા 0;
               } નહીંતર {
                  પૂર્ણાંક ha = z->img_comp[n].ha;
                  જો (!stbi__jpeg_decode_block_prog_ac(z, data, &z->huff_ac[ha], z->fast_ac[ha]))
                     પાછા 0;
               }
               // every data block is an MCU, so countdown the restart interval
               જો (--z->todo <= 0) {
                  જો (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                  જો (!STBI__RESTART(z->marker)) પાછા 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         પાછા 1;
      } નહીંતર { // interleaved
         પૂર્ણાંક i,j,k,x,y;
         માટે (j=0; j < z->img_mcu_y; ++j) {
            માટે (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               માટે (k=0; k < z->scan_n; ++k) {
                  પૂર્ણાંક n = z->order[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  માટે (y=0; y < z->img_comp[n].v; ++y) {
                     માટે (x=0; x < z->img_comp[n].h; ++x) {
                        પૂર્ણાંક x2 = (i*z->img_comp[n].h + x);
                        પૂર્ણાંક y2 = (j*z->img_comp[n].v + y);
                        short *data = z->img_comp[n].coeff + 64 * (x2 + y2 * z->img_comp[n].coeff_w);
                        જો (!stbi__jpeg_decode_block_prog_dc(z, data, &z->huff_dc[z->img_comp[n].hd], n))
                           પાછા 0;
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               જો (--z->todo <= 0) {
                  જો (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                  જો (!STBI__RESTART(z->marker)) પાછા 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         પાછા 1;
      }
   }
}

સ્થિર શૂન્ય stbi__jpeg_dequantize(short *data, stbi__uint16 *dequant)
{
   પૂર્ણાંક i;
   માટે (i=0; i < 64; ++i)
      data[i] *= dequant[i];
}

સ્થિર શૂન્ય stbi__jpeg_finish(stbi__jpeg *z)
{
   જો (z->progressive) {
      // dequantize and idct the data
      પૂર્ણાંક i,j,n;
      માટે (n=0; n < z->s->img_n; ++n) {
         પૂર્ણાંક w = (z->img_comp[n].x+7) >> 3;
         પૂર્ણાંક h = (z->img_comp[n].y+7) >> 3;
         માટે (j=0; j < h; ++j) {
            માટે (i=0; i < w; ++i) {
               short *data = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               stbi__jpeg_dequantize(data, z->dequant[z->img_comp[n].tq]);
               z->idct_block_kernel(z->img_comp[n].data+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, data);
            }
         }
      }
   }
}

સ્થિર પૂર્ણાંક stbi__process_marker(stbi__jpeg *z, પૂર્ણાંક m)
{
   પૂર્ણાંક L;
   પસંદગી (m) {
      સ્થિતિ STBI__MARKER_none: // no marker found
         પાછા stbi__err("expected marker","Corrupt JPEG");

      સ્થિતિ 0xDD: // DRI - specify restart interval
         જો (stbi__get16be(z->s) != 4) પાછા stbi__err("bad DRI len","Corrupt JPEG");
         z->restart_interval = stbi__get16be(z->s);
         પાછા 1;

      સ્થિતિ 0xDB: // DQT - define quantization table
         L = stbi__get16be(z->s)-2;
         જ્યારે (L > 0) {
            પૂર્ણાંક q = stbi__get8(z->s);
            પૂર્ણાંક p = q >> 4, sixteen = (p != 0);
            પૂર્ણાંક t = q & 15,i;
            જો (p != 0 && p != 1) પાછા stbi__err("bad DQT type","Corrupt JPEG");
            જો (t > 3) પાછા stbi__err("bad DQT table","Corrupt JPEG");

            માટે (i=0; i < 64; ++i)
               z->dequant[t][stbi__jpeg_dezigzag[i]] = (stbi__uint16)(sixteen ? stbi__get16be(z->s) : stbi__get8(z->s));
            L -= (sixteen ? 129 : 65);
         }
         પાછા L==0;

      સ્થિતિ 0xC4: // DHT - define huffman table
         L = stbi__get16be(z->s)-2;
         જ્યારે (L > 0) {
            stbi_uc *v;
            પૂર્ણાંક sizes[16],i,n=0;
            પૂર્ણાંક q = stbi__get8(z->s);
            પૂર્ણાંક tc = q >> 4;
            પૂર્ણાંક th = q & 15;
            જો (tc > 1 || th > 3) પાછા stbi__err("bad DHT header","Corrupt JPEG");
            માટે (i=0; i < 16; ++i) {
               sizes[i] = stbi__get8(z->s);
               n += sizes[i];
            }
            L -= 17;
            જો (tc == 0) {
               જો (!stbi__build_huffman(z->huff_dc+th, sizes)) પાછા 0;
               v = z->huff_dc[th].કિંમતો;
            } નહીંતર {
               જો (!stbi__build_huffman(z->huff_ac+th, sizes)) પાછા 0;
               v = z->huff_ac[th].કિંમતો;
            }
            માટે (i=0; i < n; ++i)
               v[i] = stbi__get8(z->s);
            જો (tc != 0)
               stbi__build_fast_ac(z->fast_ac[th], z->huff_ac + th);
            L -= n;
         }
         પાછા L==0;
   }

   // check for comment block or APP blocks
   જો ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
      L = stbi__get16be(z->s);
      જો (L < 2) {
         જો (m == 0xFE)
            પાછા stbi__err("bad COM len","Corrupt JPEG");
         નહીંતર
            પાછા stbi__err("bad APP len","Corrupt JPEG");
      }
      L -= 2;

      જો (m == 0xE0 && L >= 5) { // JFIF APP0 segment
         સ્થિર અચલ unsigned અક્ષર tag[5] = {'J','F','I','F','\0'};
         પૂર્ણાંક ok = 1;
         પૂર્ણાંક i;
         માટે (i=0; i < 5; ++i)
            જો (stbi__get8(z->s) != tag[i])
               ok = 0;
         L -= 5;
         જો (ok)
            z->jfif = 1;
      } નહીંતર જો (m == 0xEE && L >= 12) { // Adobe APP14 segment
         સ્થિર અચલ unsigned અક્ષર tag[6] = {'A','d','o','b','e','\0'};
         પૂર્ણાંક ok = 1;
         પૂર્ણાંક i;
         માટે (i=0; i < 6; ++i)
            જો (stbi__get8(z->s) != tag[i])
               ok = 0;
         L -= 6;
         જો (ok) {
            stbi__get8(z->s); // version
            stbi__get16be(z->s); // flags0
            stbi__get16be(z->s); // flags1
            z->app14_color_transform = stbi__get8(z->s); // color transform
            L -= 6;
         }
      }

      stbi__skip(z->s, L);
      પાછા 1;
   }

   પાછા stbi__err("unknown marker","Corrupt JPEG");
}

// after we see SOS
સ્થિર પૂર્ણાંક stbi__process_scan_header(stbi__jpeg *z)
{
   પૂર્ણાંક i;
   પૂર્ણાંક Ls = stbi__get16be(z->s);
   z->scan_n = stbi__get8(z->s);
   જો (z->scan_n < 1 || z->scan_n > 4 || z->scan_n > (પૂર્ણાંક) z->s->img_n) પાછા stbi__err("bad SOS component count","Corrupt JPEG");
   જો (Ls != 6+2*z->scan_n) પાછા stbi__err("bad SOS len","Corrupt JPEG");
   માટે (i=0; i < z->scan_n; ++i) {
      પૂર્ણાંક id = stbi__get8(z->s), which;
      પૂર્ણાંક q = stbi__get8(z->s);
      માટે (which = 0; which < z->s->img_n; ++which)
         જો (z->img_comp[which].id == id)
            તોડો;
      જો (which == z->s->img_n) પાછા 0; // no match
      z->img_comp[which].hd = q >> 4;   જો (z->img_comp[which].hd > 3) પાછા stbi__err("bad DC huff","Corrupt JPEG");
      z->img_comp[which].ha = q & 15;   જો (z->img_comp[which].ha > 3) પાછા stbi__err("bad AC huff","Corrupt JPEG");
      z->order[i] = which;
   }

   {
      પૂર્ણાંક aa;
      z->spec_start = stbi__get8(z->s);
      z->spec_end   = stbi__get8(z->s); // should be 63, but might be 0
      aa = stbi__get8(z->s);
      z->succ_ઉપલું = (aa >> 4);
      z->succ_નીચલું  = (aa & 15);
      જો (z->progressive) {
         જો (z->spec_start > 63 || z->spec_end > 63  || z->spec_start > z->spec_end || z->succ_ઉપલું > 13 || z->succ_નીચલું > 13)
            પાછા stbi__err("bad SOS", "Corrupt JPEG");
      } નહીંતર {
         જો (z->spec_start != 0) પાછા stbi__err("bad SOS","Corrupt JPEG");
         જો (z->succ_ઉપલું != 0 || z->succ_નીચલું != 0) પાછા stbi__err("bad SOS","Corrupt JPEG");
         z->spec_end = 63;
      }
   }

   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__free_jpeg_components(stbi__jpeg *z, પૂર્ણાંક ncomp, પૂર્ણાંક why)
{
   પૂર્ણાંક i;
   માટે (i=0; i < ncomp; ++i) {
      જો (z->img_comp[i].raw_data) {
         STBI_FREE(z->img_comp[i].raw_data);
         z->img_comp[i].raw_data = ખાલી;
         z->img_comp[i].data = ખાલી;
      }
      જો (z->img_comp[i].raw_coeff) {
         STBI_FREE(z->img_comp[i].raw_coeff);
         z->img_comp[i].raw_coeff = 0;
         z->img_comp[i].coeff = 0;
      }
      જો (z->img_comp[i].linebuf) {
         STBI_FREE(z->img_comp[i].linebuf);
         z->img_comp[i].linebuf = ખાલી;
      }
   }
   પાછા why;
}

સ્થિર પૂર્ણાંક stbi__process_frame_header(stbi__jpeg *z, પૂર્ણાંક scan)
{
   stbi__context *s = z->s;
   પૂર્ણાંક Lf,p,i,q, h_max=1,v_max=1,c;
   Lf = stbi__get16be(s);         જો (Lf < 11) પાછા stbi__err("bad SOF len","Corrupt JPEG"); // JPEG
   p  = stbi__get8(s);            જો (p != 8) પાછા stbi__err("only 8-bit","JPEG format not supported: 8-bit only"); // JPEG baseline
   s->img_y = stbi__get16be(s);   જો (s->img_y == 0) પાછા stbi__err("no header height", "JPEG format not supported: delayed height"); // Legal, but we don't handle it--but neither does IJG
   s->img_x = stbi__get16be(s);   જો (s->img_x == 0) પાછા stbi__err("0 width","Corrupt JPEG"); // JPEG requires
   c = stbi__get8(s);
   જો (c != 3 && c != 1 && c != 4) પાછા stbi__err("bad component count","Corrupt JPEG");
   s->img_n = c;
   માટે (i=0; i < c; ++i) {
      z->img_comp[i].data = ખાલી;
      z->img_comp[i].linebuf = ખાલી;
   }

   જો (Lf != 8+3*s->img_n) પાછા stbi__err("bad SOF len","Corrupt JPEG");

   z->rgb = 0;
   માટે (i=0; i < s->img_n; ++i) {
      સ્થિર અચલ unsigned અક્ષર rgb[3] = { 'R', 'G', 'B' };
      z->img_comp[i].id = stbi__get8(s);
      જો (s->img_n == 3 && z->img_comp[i].id == rgb[i])
         ++z->rgb;
      q = stbi__get8(s);
      z->img_comp[i].h = (q >> 4);  જો (!z->img_comp[i].h || z->img_comp[i].h > 4) પાછા stbi__err("bad H","Corrupt JPEG");
      z->img_comp[i].v = q & 15;    જો (!z->img_comp[i].v || z->img_comp[i].v > 4) પાછા stbi__err("bad V","Corrupt JPEG");
      z->img_comp[i].tq = stbi__get8(s);  જો (z->img_comp[i].tq > 3) પાછા stbi__err("bad TQ","Corrupt JPEG");
   }

   જો (scan != STBI__SCAN_load) પાછા 1;

   જો (!stbi__mad3sizes_valid(s->img_x, s->img_y, s->img_n, 0)) પાછા stbi__err("too large", "Image too large to decode");

   માટે (i=0; i < s->img_n; ++i) {
      જો (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
      જો (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
   }

   // compute interleaved mcu info
   z->img_h_max = h_max;
   z->img_v_max = v_max;
   z->img_mcu_w = h_max * 8;
   z->img_mcu_h = v_max * 8;
   // these sizes can't be more than 17 bits
   z->img_mcu_x = (s->img_x + z->img_mcu_w-1) / z->img_mcu_w;
   z->img_mcu_y = (s->img_y + z->img_mcu_h-1) / z->img_mcu_h;

   માટે (i=0; i < s->img_n; ++i) {
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
      z->img_comp[i].linebuf = ખાલી;
      z->img_comp[i].raw_data = stbi__malloc_mad2(z->img_comp[i].w2, z->img_comp[i].h2, 15);
      જો (z->img_comp[i].raw_data == ખાલી)
         પાછા stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
      // align blocks for idct using mmx/sse
      z->img_comp[i].data = (stbi_uc*) (((size_t) z->img_comp[i].raw_data + 15) & ~15);
      જો (z->progressive) {
         // w2, h2 are multiples of 8 (see above)
         z->img_comp[i].coeff_w = z->img_comp[i].w2 / 8;
         z->img_comp[i].coeff_h = z->img_comp[i].h2 / 8;
         z->img_comp[i].raw_coeff = stbi__malloc_mad3(z->img_comp[i].w2, z->img_comp[i].h2, કદ(short), 15);
         જો (z->img_comp[i].raw_coeff == ખાલી)
            પાછા stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
         z->img_comp[i].coeff = (short*) (((size_t) z->img_comp[i].raw_coeff + 15) & ~15);
      }
   }

   પાછા 1;
}

// use comparisons since in some cases we handle more than one case (e.g. SOF)
#define stbi__DNL(x)         ((x) == 0xdc)
#define stbi__SOI(x)         ((x) == 0xd8)
#define stbi__EOI(x)         ((x) == 0xd9)
#define stbi__SOF(x)         ((x) == 0xc0 || (x) == 0xc1 || (x) == 0xc2)
#define stbi__SOS(x)         ((x) == 0xda)

#define stbi__SOF_progressive(x)   ((x) == 0xc2)

સ્થિર પૂર્ણાંક stbi__decode_jpeg_header(stbi__jpeg *z, પૂર્ણાંક scan)
{
   પૂર્ણાંક m;
   z->jfif = 0;
   z->app14_color_transform = -1; // valid values are 0,1,2
   z->marker = STBI__MARKER_none; // initialize cached marker to empty
   m = stbi__get_marker(z);
   જો (!stbi__SOI(m)) પાછા stbi__err("no SOI","Corrupt JPEG");
   જો (scan == STBI__SCAN_type) પાછા 1;
   m = stbi__get_marker(z);
   જ્યારે (!stbi__SOF(m)) {
      જો (!stbi__process_marker(z,m)) પાછા 0;
      m = stbi__get_marker(z);
      જ્યારે (m == STBI__MARKER_none) {
         // some files have extra padding after their blocks, so ok, we'll scan
         જો (stbi__at_eof(z->s)) પાછા stbi__err("no SOF", "Corrupt JPEG");
         m = stbi__get_marker(z);
      }
   }
   z->progressive = stbi__SOF_progressive(m);
   જો (!stbi__process_frame_header(z, scan)) પાછા 0;
   પાછા 1;
}

// decode image to YCbCr format
સ્થિર પૂર્ણાંક stbi__decode_jpeg_image(stbi__jpeg *j)
{
   પૂર્ણાંક m;
   માટે (m = 0; m < 4; m++) {
      j->img_comp[m].raw_data = ખાલી;
      j->img_comp[m].raw_coeff = ખાલી;
   }
   j->restart_interval = 0;
   જો (!stbi__decode_jpeg_header(j, STBI__SCAN_load)) પાછા 0;
   m = stbi__get_marker(j);
   જ્યારે (!stbi__EOI(m)) {
      જો (stbi__SOS(m)) {
         જો (!stbi__process_scan_header(j)) પાછા 0;
         જો (!stbi__parse_entropy_coded_data(j)) પાછા 0;
         જો (j->marker == STBI__MARKER_none ) {
            // handle 0s at the end of image data from IP Kamera 9060
            જ્યારે (!stbi__at_eof(j->s)) {
               પૂર્ણાંક x = stbi__get8(j->s);
               જો (x == 255) {
                  j->marker = stbi__get8(j->s);
                  તોડો;
               }
            }
            // if we reach eof without hitting a marker, stbi__get_marker() below will fail and we'll eventually return 0
         }
      } નહીંતર જો (stbi__DNL(m)) {
         પૂર્ણાંક Ld = stbi__get16be(j->s);
         stbi__uint32 NL = stbi__get16be(j->s);
         જો (Ld != 4) પાછા stbi__err("bad DNL len", "Corrupt JPEG");
         જો (NL != j->s->img_y) પાછા stbi__err("bad DNL height", "Corrupt JPEG");
      } નહીંતર {
         જો (!stbi__process_marker(j, m)) પાછા 0;
      }
      m = stbi__get_marker(j);
   }
   જો (j->progressive)
      stbi__jpeg_finish(j);
   પાછા 1;
}

// static jfif-centered resampling (across block boundaries)

પ્રકારવ્યાખ્યા stbi_uc *(*resample_row_func)(stbi_uc *out, stbi_uc *in0, stbi_uc *in1,
                                    પૂર્ણાંક w, પૂર્ણાંક hs);

#define stbi__div4(x) ((stbi_uc) ((x) >> 2))

સ્થિર stbi_uc *resample_row_1(stbi_uc *out, stbi_uc *in_near, stbi_uc *in_far, પૂર્ણાંક w, પૂર્ણાંક hs)
{
   STBI_NOTUSED(out);
   STBI_NOTUSED(in_far);
   STBI_NOTUSED(w);
   STBI_NOTUSED(hs);
   પાછા in_near;
}

સ્થિર stbi_uc* stbi__resample_row_v_2(stbi_uc *out, stbi_uc *in_near, stbi_uc *in_far, પૂર્ણાંક w, પૂર્ણાંક hs)
{
   // need to generate two samples vertically for every one in input
   પૂર્ણાંક i;
   STBI_NOTUSED(hs);
   માટે (i=0; i < w; ++i)
      out[i] = stbi__div4(3*in_near[i] + in_far[i] + 2);
   પાછા out;
}

સ્થિર stbi_uc*  stbi__resample_row_h_2(stbi_uc *out, stbi_uc *in_near, stbi_uc *in_far, પૂર્ણાંક w, પૂર્ણાંક hs)
{
   // need to generate two samples horizontally for every one in input
   પૂર્ણાંક i;
   stbi_uc *આગત = in_near;

   જો (w == 1) {
      // if only one sample, can't do any interpolation
      out[0] = out[1] = આગત[0];
      પાછા out;
   }

   out[0] = આગત[0];
   out[1] = stbi__div4(આગત[0]*3 + આગત[1] + 2);
   માટે (i=1; i < w-1; ++i) {
      પૂર્ણાંક n = 3*આગત[i]+2;
      out[i*2+0] = stbi__div4(n+આગત[i-1]);
      out[i*2+1] = stbi__div4(n+આગત[i+1]);
   }
   out[i*2+0] = stbi__div4(આગત[w-2]*3 + આગત[w-1] + 2);
   out[i*2+1] = આગત[w-1];

   STBI_NOTUSED(in_far);
   STBI_NOTUSED(hs);

   પાછા out;
}

#define stbi__div16(x) ((stbi_uc) ((x) >> 4))

સ્થિર stbi_uc *stbi__resample_row_hv_2(stbi_uc *out, stbi_uc *in_near, stbi_uc *in_far, પૂર્ણાંક w, પૂર્ણાંક hs)
{
   // need to generate 2x2 samples for every one in input
   પૂર્ણાંક i,t0,t1;
   જો (w == 1) {
      out[0] = out[1] = stbi__div4(3*in_near[0] + in_far[0] + 2);
      પાછા out;
   }

   t1 = 3*in_near[0] + in_far[0];
   out[0] = stbi__div4(t1+2);
   માટે (i=1; i < w; ++i) {
      t0 = t1;
      t1 = 3*in_near[i]+in_far[i];
      out[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      out[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   out[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   પાછા out;
}

#જો defined(STBI_SSE2) || defined(STBI_NEON)
સ્થિર stbi_uc *stbi__resample_row_hv_2_simd(stbi_uc *out, stbi_uc *in_near, stbi_uc *in_far, પૂર્ણાંક w, પૂર્ણાંક hs)
{
   // need to generate 2x2 samples for every one in input
   પૂર્ણાંક i=0,t0,t1;

   જો (w == 1) {
      out[0] = out[1] = stbi__div4(3*in_near[0] + in_far[0] + 2);
      પાછા out;
   }

   t1 = 3*in_near[0] + in_far[0];
   // process groups of 8 pixels for as long as we can.
   // note we can't handle the last pixel in a row in this loop
   // because we need to handle the filter boundary conditions.
   માટે (; i < ((w-1) & ~7); i += 8) {
#જો defined(STBI_SSE2)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      __m128i zero  = _mm_setzero_si128();
      __m128i farb  = _mm_loadl_epi64((__m128i *) (in_far + i));
      __m128i nearb = _mm_loadl_epi64((__m128i *) (in_near + i));
      __m128i farw  = _mm_unpacklo_epi8(farb, zero);
      __m128i nearw = _mm_unpacklo_epi8(nearb, zero);
      __m128i diff  = _mm_sub_epi16(farw, nearw);
      __m128i nears = _mm_slli_epi16(nearw, 2);
      __m128i curr  = _mm_add_epi16(nears, diff); // current row

      // horizontal filter works the same based on shifted vers of current
      // row. "prev" is current row shifted right by 1 pixel; we need to
      // insert the previous pixel value (from t1).
      // "next" is current row shifted left by 1 pixel, with first pixel
      // of next block of 8 pixels added in.
      __m128i prv0 = _mm_slli_si128(curr, 2);
      __m128i nxt0 = _mm_srli_si128(curr, 2);
      __m128i prev = mm_ઉમેરો_epi16(prv0, t1, 0);
      __m128i next = mm_ઉમેરો_epi16(nxt0, 3*in_near[i+8] + in_far[i+8], 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      __m128i bias  = _mm_set1_epi16(8);
      __m128i curs = _mm_slli_epi16(curr, 2);
      __m128i prvd = _mm_sub_epi16(prev, curr);
      __m128i nxtd = _mm_sub_epi16(next, curr);
      __m128i curb = _mm_add_epi16(curs, bias);
      __m128i even = _mm_add_epi16(prvd, curb);
      __m128i odd  = _mm_add_epi16(nxtd, curb);

      // interleave even and odd pixels, then undo scaling.
      __m128i int0 = _mm_unpacklo_epi16(even, odd);
      __m128i int1 = _mm_unpackhi_epi16(even, odd);
      __m128i de0  = _mm_srli_epi16(int0, 4);
      __m128i de1  = _mm_srli_epi16(int1, 4);

      // pack and write output
      __m128i outv = _mm_packus_epi16(de0, de1);
      _mm_storeu_si128((__m128i *) (out + i*2), outv);
#અથવાજો defined(STBI_NEON)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      uint8x8_t farb  = vld1_u8(in_far + i);
      uint8x8_t nearb = vld1_u8(in_near + i);
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
      int16x8_t next = vsetq_lane_s16(3*in_near[i+8] + in_far[i+8], nxt0, 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      int16x8_t curs = vshlq_n_s16(curr, 2);
      int16x8_t prvd = vsubq_s16(prev, curr);
      int16x8_t nxtd = vsubq_s16(next, curr);
      int16x8_t even = vaddq_s16(curs, prvd);
      int16x8_t odd  = vaddq_s16(curs, nxtd);

      // undo scaling and round, then store with even/odd phases interleaved
      uint8x8x2_t o;
      o.val[0] = vqrshrun_n_s16(even, 4);
      o.val[1] = vqrshrun_n_s16(odd,  4);
      vst2_u8(out + i*2, o);
#endif

      // "previous" value for next iter
      t1 = 3*in_near[i+7] + in_far[i+7];
   }

   t0 = t1;
   t1 = 3*in_near[i] + in_far[i];
   out[i*2] = stbi__div16(3*t1 + t0 + 8);

   માટે (++i; i < w; ++i) {
      t0 = t1;
      t1 = 3*in_near[i]+in_far[i];
      out[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      out[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   out[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   પાછા out;
}
#endif

સ્થિર stbi_uc *stbi__resample_row_generic(stbi_uc *out, stbi_uc *in_near, stbi_uc *in_far, પૂર્ણાંક w, પૂર્ણાંક hs)
{
   // resample with nearest-neighbor
   પૂર્ણાંક i,j;
   STBI_NOTUSED(in_far);
   માટે (i=0; i < w; ++i)
      માટે (j=0; j < hs; ++j)
         out[i*hs+j] = in_near[i];
   પાછા out;
}

// this is a reduced-precision calculation of YCbCr-to-RGB introduced
// to make sure the code produces the same results in both SIMD and scalar
#define stbi__float2fixed(x)  (((પૂર્ણાંક) ((x) * 4096.0f + 0.5f)) << 8)
સ્થિર શૂન્ય stbi__YCbCr_to_RGB_row(stbi_uc *out, અચલ stbi_uc *y, અચલ stbi_uc *pcb, અચલ stbi_uc *pcr, પૂર્ણાંક ગણતરી, પૂર્ણાંક step)
{
   પૂર્ણાંક i;
   માટે (i=0; i < ગણતરી; ++i) {
      પૂર્ણાંક y_fixed = (y[i] << 20) + (1<<19); // rounding
      પૂર્ણાંક r,g,b;
      પૂર્ણાંક cr = pcr[i] - 128;
      પૂર્ણાંક cb = pcb[i] - 128;
      r = y_fixed +  cr* stbi__float2fixed(1.40200f);
      g = y_fixed + (cr*-stbi__float2fixed(0.71414f)) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                     +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      જો ((unsigned) r > 255) { જો (r < 0) r = 0; નહીંતર r = 255; }
      જો ((unsigned) g > 255) { જો (g < 0) g = 0; નહીંતર g = 255; }
      જો ((unsigned) b > 255) { જો (b < 0) b = 0; નહીંતર b = 255; }
      out[0] = (stbi_uc)r;
      out[1] = (stbi_uc)g;
      out[2] = (stbi_uc)b;
      out[3] = 255;
      out += step;
   }
}

#જો defined(STBI_SSE2) || defined(STBI_NEON)
સ્થિર શૂન્ય stbi__YCbCr_to_RGB_simd(stbi_uc *out, stbi_uc અચલ *y, stbi_uc અચલ *pcb, stbi_uc અચલ *pcr, પૂર્ણાંક ગણતરી, પૂર્ણાંક step)
{
   પૂર્ણાંક i = 0;

#ifdef STBI_SSE2
   // step == 3 is pretty ugly on the final interleave, and i'm not convinced
   // it's useful in practice (you wouldn't use it for textures, for example).
   // so just accelerate step == 4 case.
   જો (step == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      __m128i signflip  = _mm_set1_epi8(-0x80);
      __m128i cr_const0 = _mm_set1_epi16(   (short) ( 1.40200f*4096.0f+0.5f));
      __m128i cr_const1 = _mm_set1_epi16( - (short) ( 0.71414f*4096.0f+0.5f));
      __m128i cb_const0 = _mm_set1_epi16( - (short) ( 0.34414f*4096.0f+0.5f));
      __m128i cb_const1 = _mm_set1_epi16(   (short) ( 1.77200f*4096.0f+0.5f));
      __m128i y_bias = _mm_set1_epi8((અક્ષર) (unsigned અક્ષર) 128);
      __m128i xw = _mm_set1_epi16(255); // alpha channel

      માટે (; i+7 < ગણતરી; i += 8) {
         // load
         __m128i y_bytes = _mm_loadl_epi64((__m128i *) (y+i));
         __m128i cr_bytes = _mm_loadl_epi64((__m128i *) (pcr+i));
         __m128i cb_bytes = _mm_loadl_epi64((__m128i *) (pcb+i));
         __m128i cr_biased = _mm_xor_si128(cr_bytes, signflip); // -128
         __m128i cb_biased = _mm_xor_si128(cb_bytes, signflip); // -128

         // unpack to short (and left-shift cr, cb by 8)
         __m128i yw  = _mm_unpacklo_epi8(y_bias, y_bytes);
         __m128i crw = _mm_unpacklo_epi8(_mm_setzero_si128(), cr_biased);
         __m128i cbw = _mm_unpacklo_epi8(_mm_setzero_si128(), cb_biased);

         // color transform
         __m128i yws = _mm_srli_epi16(yw, 4);
         __m128i cr0 = _mm_mulhi_epi16(cr_const0, crw);
         __m128i cb0 = _mm_mulhi_epi16(cb_const0, cbw);
         __m128i cb1 = _mm_mulhi_epi16(cbw, cb_const1);
         __m128i cr1 = _mm_mulhi_epi16(crw, cr_const1);
         __m128i rws = _mm_add_epi16(cr0, yws);
         __m128i gwt = _mm_add_epi16(cb0, yws);
         __m128i bws = _mm_add_epi16(yws, cb1);
         __m128i gws = _mm_add_epi16(gwt, cr1);

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
         _mm_storeu_si128((__m128i *) (out + 0), o0);
         _mm_storeu_si128((__m128i *) (out + 16), o1);
         out += 32;
      }
   }
#endif

#ifdef STBI_NEON
   // in this version, step=3 support would be easy to add. but is there demand?
   જો (step == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      uint8x8_t signflip = vdup_n_u8(0x80);
      int16x8_t cr_const0 = vdupq_n_s16(   (short) ( 1.40200f*4096.0f+0.5f));
      int16x8_t cr_const1 = vdupq_n_s16( - (short) ( 0.71414f*4096.0f+0.5f));
      int16x8_t cb_const0 = vdupq_n_s16( - (short) ( 0.34414f*4096.0f+0.5f));
      int16x8_t cb_const1 = vdupq_n_s16(   (short) ( 1.77200f*4096.0f+0.5f));

      માટે (; i+7 < ગણતરી; i += 8) {
         // load
         uint8x8_t y_bytes  = vld1_u8(y + i);
         uint8x8_t cr_bytes = vld1_u8(pcr + i);
         uint8x8_t cb_bytes = vld1_u8(pcb + i);
         int8x8_t cr_biased = vreinterpret_s8_u8(vsub_u8(cr_bytes, signflip));
         int8x8_t cb_biased = vreinterpret_s8_u8(vsub_u8(cb_bytes, signflip));

         // expand to s16
         int16x8_t yws = vreinterpretq_s16_u16(vshll_n_u8(y_bytes, 4));
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
         vst4_u8(out, o);
         out += 8*4;
      }
   }
#endif

   માટે (; i < ગણતરી; ++i) {
      પૂર્ણાંક y_fixed = (y[i] << 20) + (1<<19); // rounding
      પૂર્ણાંક r,g,b;
      પૂર્ણાંક cr = pcr[i] - 128;
      પૂર્ણાંક cb = pcb[i] - 128;
      r = y_fixed + cr* stbi__float2fixed(1.40200f);
      g = y_fixed + cr*-stbi__float2fixed(0.71414f) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                   +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      જો ((unsigned) r > 255) { જો (r < 0) r = 0; નહીંતર r = 255; }
      જો ((unsigned) g > 255) { જો (g < 0) g = 0; નહીંતર g = 255; }
      જો ((unsigned) b > 255) { જો (b < 0) b = 0; નહીંતર b = 255; }
      out[0] = (stbi_uc)r;
      out[1] = (stbi_uc)g;
      out[2] = (stbi_uc)b;
      out[3] = 255;
      out += step;
   }
}
#endif

// set up the kernels
સ્થિર શૂન્ય stbi__setup_jpeg(stbi__jpeg *j)
{
   j->idct_block_kernel = stbi__idct_block;
   j->YCbCr_to_RGB_kernel = stbi__YCbCr_to_RGB_row;
   j->resample_row_hv_2_kernel = stbi__resample_row_hv_2;

#ifdef STBI_SSE2
   જો (stbi__sse2_available()) {
      j->idct_block_kernel = stbi__idct_simd;
      j->YCbCr_to_RGB_kernel = stbi__YCbCr_to_RGB_simd;
      j->resample_row_hv_2_kernel = stbi__resample_row_hv_2_simd;
   }
#endif

#ifdef STBI_NEON
   j->idct_block_kernel = stbi__idct_simd;
   j->YCbCr_to_RGB_kernel = stbi__YCbCr_to_RGB_simd;
   j->resample_row_hv_2_kernel = stbi__resample_row_hv_2_simd;
#endif
}

// clean up the temporary component buffers
સ્થિર શૂન્ય stbi__cleanup_jpeg(stbi__jpeg *j)
{
   stbi__free_jpeg_components(j, j->s->img_n, 0);
}

પ્રકારવ્યાખ્યા સંરચના
{
   resample_row_func resample;
   stbi_uc *line0,*line1;
   પૂર્ણાંક hs,vs;   // expansion factor in each axis
   પૂર્ણાંક w_lores; // horizontal pixels pre-expansion
   પૂર્ણાંક ystep;   // how far through vertical expansion we are
   પૂર્ણાંક ypos;    // which pre-expansion row we're on
} stbi__resample;

// fast 0..255 * 0..255 => 0..255 rounded multiplication
સ્થિર stbi_uc stbi__blinn_8x8(stbi_uc x, stbi_uc y)
{
   unsigned પૂર્ણાંક t = x*y + 128;
   પાછા (stbi_uc) ((t + (t >>8)) >> 8);
}

સ્થિર stbi_uc *load_jpeg_image(stbi__jpeg *z, પૂર્ણાંક *out_x, પૂર્ણાંક *out_y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   પૂર્ણાંક n, decode_n, is_rgb;
   z->s->img_n = 0; // make stbi__cleanup_jpeg safe

   // validate req_comp
   જો (req_comp < 0 || req_comp > 4) પાછા stbi__errpuc("bad req_comp", "Internal error");

   // load a jpeg image from whichever source, but leave in YCbCr format
   જો (!stbi__decode_jpeg_image(z)) { stbi__cleanup_jpeg(z); પાછા ખાલી; }

   // determine actual number of components to generate
   n = req_comp ? req_comp : z->s->img_n >= 3 ? 3 : 1;

   is_rgb = z->s->img_n == 3 && (z->rgb == 3 || (z->app14_color_transform == 0 && !z->jfif));

   જો (z->s->img_n == 3 && n < 3 && !is_rgb)
      decode_n = 1;
   નહીંતર
      decode_n = z->s->img_n;

   // resample and color-convert
   {
      પૂર્ણાંક k;
      unsigned પૂર્ણાંક i,j;
      stbi_uc *output;
      stbi_uc *coutput[4];

      stbi__resample res_comp[4];

      માટે (k=0; k < decode_n; ++k) {
         stbi__resample *r = &res_comp[k];

         // allocate line buffer big enough for upsampling off the edges
         // with upsample factor of 4
         z->img_comp[k].linebuf = (stbi_uc *) stbi__malloc(z->s->img_x + 3);
         જો (!z->img_comp[k].linebuf) { stbi__cleanup_jpeg(z); પાછા stbi__errpuc("outofmem", "Out of memory"); }

         r->hs      = z->img_h_max / z->img_comp[k].h;
         r->vs      = z->img_v_max / z->img_comp[k].v;
         r->ystep   = r->vs >> 1;
         r->w_lores = (z->s->img_x + r->hs-1) / r->hs;
         r->ypos    = 0;
         r->line0   = r->line1 = z->img_comp[k].data;

         જો      (r->hs == 1 && r->vs == 1) r->resample = resample_row_1;
         નહીંતર જો (r->hs == 1 && r->vs == 2) r->resample = stbi__resample_row_v_2;
         નહીંતર જો (r->hs == 2 && r->vs == 1) r->resample = stbi__resample_row_h_2;
         નહીંતર જો (r->hs == 2 && r->vs == 2) r->resample = z->resample_row_hv_2_kernel;
         નહીંતર                               r->resample = stbi__resample_row_generic;
      }

      // can't error after this so, this is safe
      output = (stbi_uc *) stbi__malloc_mad3(n, z->s->img_x, z->s->img_y, 1);
      જો (!output) { stbi__cleanup_jpeg(z); પાછા stbi__errpuc("outofmem", "Out of memory"); }

      // now go ahead and resample
      માટે (j=0; j < z->s->img_y; ++j) {
         stbi_uc *out = output + n * z->s->img_x * j;
         માટે (k=0; k < decode_n; ++k) {
            stbi__resample *r = &res_comp[k];
            પૂર્ણાંક y_bot = r->ystep >= (r->vs >> 1);
            coutput[k] = r->resample(z->img_comp[k].linebuf,
                                     y_bot ? r->line1 : r->line0,
                                     y_bot ? r->line0 : r->line1,
                                     r->w_lores, r->hs);
            જો (++r->ystep >= r->vs) {
               r->ystep = 0;
               r->line0 = r->line1;
               જો (++r->ypos < z->img_comp[k].y)
                  r->line1 += z->img_comp[k].w2;
            }
         }
         જો (n >= 3) {
            stbi_uc *y = coutput[0];
            જો (z->s->img_n == 3) {
               જો (is_rgb) {
                  માટે (i=0; i < z->s->img_x; ++i) {
                     out[0] = y[i];
                     out[1] = coutput[1][i];
                     out[2] = coutput[2][i];
                     out[3] = 255;
                     out += n;
                  }
               } નહીંતર {
                  z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } નહીંતર જો (z->s->img_n == 4) {
               જો (z->app14_color_transform == 0) { // CMYK
                  માટે (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     out[0] = stbi__blinn_8x8(coutput[0][i], m);
                     out[1] = stbi__blinn_8x8(coutput[1][i], m);
                     out[2] = stbi__blinn_8x8(coutput[2][i], m);
                     out[3] = 255;
                     out += n;
                  }
               } નહીંતર જો (z->app14_color_transform == 2) { // YCCK
                  z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
                  માટે (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     out[0] = stbi__blinn_8x8(255 - out[0], m);
                     out[1] = stbi__blinn_8x8(255 - out[1], m);
                     out[2] = stbi__blinn_8x8(255 - out[2], m);
                     out += n;
                  }
               } નહીંતર { // YCbCr + alpha?  Ignore the fourth channel for now
                  z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } નહીંતર
               માટે (i=0; i < z->s->img_x; ++i) {
                  out[0] = out[1] = out[2] = y[i];
                  out[3] = 255; // not used if n==3
                  out += n;
               }
         } નહીંતર {
            જો (is_rgb) {
               જો (n == 1)
                  માટે (i=0; i < z->s->img_x; ++i)
                     *out++ = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
               નહીંતર {
                  માટે (i=0; i < z->s->img_x; ++i, out += 2) {
                     out[0] = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
                     out[1] = 255;
                  }
               }
            } નહીંતર જો (z->s->img_n == 4 && z->app14_color_transform == 0) {
               માટે (i=0; i < z->s->img_x; ++i) {
                  stbi_uc m = coutput[3][i];
                  stbi_uc r = stbi__blinn_8x8(coutput[0][i], m);
                  stbi_uc g = stbi__blinn_8x8(coutput[1][i], m);
                  stbi_uc b = stbi__blinn_8x8(coutput[2][i], m);
                  out[0] = stbi__compute_y(r, g, b);
                  out[1] = 255;
                  out += n;
               }
            } નહીંતર જો (z->s->img_n == 4 && z->app14_color_transform == 2) {
               માટે (i=0; i < z->s->img_x; ++i) {
                  out[0] = stbi__blinn_8x8(255 - coutput[0][i], coutput[3][i]);
                  out[1] = 255;
                  out += n;
               }
            } નહીંતર {
               stbi_uc *y = coutput[0];
               જો (n == 1)
                  માટે (i=0; i < z->s->img_x; ++i) out[i] = y[i];
               નહીંતર
                  માટે (i=0; i < z->s->img_x; ++i) *out++ = y[i], *out++ = 255;
            }
         }
      }
      stbi__cleanup_jpeg(z);
      *out_x = z->s->img_x;
      *out_y = z->s->img_y;
      જો (comp) *comp = z->s->img_n >= 3 ? 3 : 1; // report original components, not output
      પાછા output;
   }
}

સ્થિર શૂન્ય *stbi__jpeg_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   unsigned અક્ષર* result;
   stbi__jpeg* j = (stbi__jpeg*) stbi__malloc(કદ(stbi__jpeg));
   STBI_NOTUSED(ri);
   j->s = s;
   stbi__setup_jpeg(j);
   result = load_jpeg_image(j, x,y,comp,req_comp);
   STBI_FREE(j);
   પાછા result;
}

સ્થિર પૂર્ણાંક stbi__jpeg_test(stbi__context *s)
{
   પૂર્ણાંક r;
   stbi__jpeg* j = (stbi__jpeg*)stbi__malloc(કદ(stbi__jpeg));
   j->s = s;
   stbi__setup_jpeg(j);
   r = stbi__decode_jpeg_header(j, STBI__SCAN_type);
   stbi__rewind(s);
   STBI_FREE(j);
   પાછા r;
}

સ્થિર પૂર્ણાંક stbi__jpeg_info_raw(stbi__jpeg *j, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   જો (!stbi__decode_jpeg_header(j, STBI__SCAN_header)) {
      stbi__rewind( j->s );
      પાછા 0;
   }
   જો (x) *x = j->s->img_x;
   જો (y) *y = j->s->img_y;
   જો (comp) *comp = j->s->img_n >= 3 ? 3 : 1;
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__jpeg_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   પૂર્ણાંક result;
   stbi__jpeg* j = (stbi__jpeg*) (stbi__malloc(કદ(stbi__jpeg)));
   j->s = s;
   result = stbi__jpeg_info_raw(j, x, y, comp);
   STBI_FREE(j);
   પાછા result;
}
#endif

// public domain zlib decode    v0.2  Sean Barrett 2006-11-18
//    simple implementation
//      - all input must be provided in an upfront buffer
//      - all output is written to a single output buffer (can malloc/realloc)
//    performance
//      - fast huffman

#ifndef STBI_NO_ZLIB

// fast-way is faster to check than jpeg huffman, but slow way is slower
#define STBI__ZFAST_BITS  9 // accelerate all cases in default tables
#define STBI__ZFAST_MASK  ((1 << STBI__ZFAST_BITS) - 1)

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
પ્રકારવ્યાખ્યા સંરચના
{
   stbi__uint16 fast[1 << STBI__ZFAST_BITS];
   stbi__uint16 firstcode[16];
   પૂર્ણાંક maxcode[17];
   stbi__uint16 firstsymbol[16];
   stbi_uc  size[288];
   stbi__uint16 કિંમત[288];
} stbi__zhuffman;

stbi_inline સ્થિર પૂર્ણાંક stbi__bitreverse16(પૂર્ણાંક n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  પાછા n;
}

stbi_inline સ્થિર પૂર્ણાંક stbi__bit_reverse(પૂર્ણાંક v, પૂર્ણાંક bits)
{
   STBI_ASSERT(bits <= 16);
   // to bit reverse n bits, reverse 16 and shift
   // e.g. 11 bits, bit reverse and shift away 5
   પાછા stbi__bitreverse16(v) >> (16-bits);
}

સ્થિર પૂર્ણાંક stbi__zbuild_huffman(stbi__zhuffman *z, અચલ stbi_uc *sizelist, પૂર્ણાંક અંક)
{
   પૂર્ણાંક i,k=0;
   પૂર્ણાંક code, next_code[16], sizes[17];

   // DEFLATE spec for generating codes
   memset(sizes, 0, કદ(sizes));
   memset(z->fast, 0, કદ(z->fast));
   માટે (i=0; i < અંક; ++i)
      ++sizes[sizelist[i]];
   sizes[0] = 0;
   માટે (i=1; i < 16; ++i)
      જો (sizes[i] > (1 << i))
         પાછા stbi__err("bad sizes", "Corrupt PNG");
   code = 0;
   માટે (i=1; i < 16; ++i) {
      next_code[i] = code;
      z->firstcode[i] = (stbi__uint16) code;
      z->firstsymbol[i] = (stbi__uint16) k;
      code = (code + sizes[i]);
      જો (sizes[i])
         જો (code-1 >= (1 << i)) પાછા stbi__err("bad codelengths","Corrupt PNG");
      z->maxcode[i] = code << (16-i); // preshift for inner loop
      code <<= 1;
      k += sizes[i];
   }
   z->maxcode[16] = 0x10000; // sentinel
   માટે (i=0; i < અંક; ++i) {
      પૂર્ણાંક s = sizelist[i];
      જો (s) {
         પૂર્ણાંક c = next_code[s] - z->firstcode[s] + z->firstsymbol[s];
         stbi__uint16 fastv = (stbi__uint16) ((s << 9) | i);
         z->size [c] = (stbi_uc     ) s;
         z->કિંમત[c] = (stbi__uint16) i;
         જો (s <= STBI__ZFAST_BITS) {
            પૂર્ણાંક j = stbi__bit_reverse(next_code[s],s);
            જ્યારે (j < (1 << STBI__ZFAST_BITS)) {
               z->fast[j] = fastv;
               j += (1 << s);
            }
         }
         ++next_code[s];
      }
   }
   પાછા 1;
}

// zlib-from-memory implementation for PNG reading
//    because PNG allows splitting the zlib stream arbitrarily,
//    and it's annoying structurally to have PNG call ZLIB call PNG,
//    we require PNG read all the IDATs and combine them into a single
//    memory buffer

પ્રકારવ્યાખ્યા સંરચના
{
   stbi_uc *zbuffer, *zbuffer_end;
   પૂર્ણાંક અંક_bits;
   stbi__uint32 code_buffer;

   અક્ષર *zout;
   અક્ષર *zout_start;
   અક્ષર *zout_end;
   પૂર્ણાંક   z_expandable;

   stbi__zhuffman z_length, z_distance;
} stbi__zbuf;

stbi_inline સ્થિર stbi_uc stbi__zget8(stbi__zbuf *z)
{
   જો (z->zbuffer >= z->zbuffer_end) પાછા 0;
   પાછા *z->zbuffer++;
}

સ્થિર શૂન્ય stbi__fill_bits(stbi__zbuf *z)
{
   કરો {
      STBI_ASSERT(z->code_buffer < (1U << z->અંક_bits));
      z->code_buffer |= (unsigned પૂર્ણાંક) stbi__zget8(z) << z->અંક_bits;
      z->અંક_bits += 8;
   } જ્યારે (z->અંક_bits <= 24);
}

stbi_inline સ્થિર unsigned પૂર્ણાંક stbi__zreceive(stbi__zbuf *z, પૂર્ણાંક n)
{
   unsigned પૂર્ણાંક k;
   જો (z->અંક_bits < n) stbi__fill_bits(z);
   k = z->code_buffer & ((1 << n) - 1);
   z->code_buffer >>= n;
   z->અંક_bits -= n;
   પાછા k;
}

સ્થિર પૂર્ણાંક stbi__zhuffman_decode_slowpath(stbi__zbuf *a, stbi__zhuffman *z)
{
   પૂર્ણાંક b,s,k;
   // not resolved by fast table, so compute it the slow way
   // use jpeg approach, which requires MSbits at top
   k = stbi__bit_reverse(a->code_buffer, 16);
   માટે (s=STBI__ZFAST_BITS+1; ; ++s)
      જો (k < z->maxcode[s])
         તોડો;
   જો (s == 16) પાછા -1; // invalid code!
   // code size is s, so:
   b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
   STBI_ASSERT(z->size[b] == s);
   a->code_buffer >>= s;
   a->અંક_bits -= s;
   પાછા z->કિંમત[b];
}

stbi_inline સ્થિર પૂર્ણાંક stbi__zhuffman_decode(stbi__zbuf *a, stbi__zhuffman *z)
{
   પૂર્ણાંક b,s;
   જો (a->અંક_bits < 16) stbi__fill_bits(a);
   b = z->fast[a->code_buffer & STBI__ZFAST_MASK];
   જો (b) {
      s = b >> 9;
      a->code_buffer >>= s;
      a->અંક_bits -= s;
      પાછા b & 511;
   }
   પાછા stbi__zhuffman_decode_slowpath(a, z);
}

સ્થિર પૂર્ણાંક stbi__zexpand(stbi__zbuf *z, અક્ષર *zout, પૂર્ણાંક n)  // need to make room for n bytes
{
   અક્ષર *q;
   પૂર્ણાંક cur, limit, old_limit;
   z->zout = zout;
   જો (!z->z_expandable) પાછા stbi__err("output buffer limit","Corrupt PNG");
   cur   = (પૂર્ણાંક) (z->zout     - z->zout_start);
   limit = old_limit = (પૂર્ણાંક) (z->zout_end - z->zout_start);
   જ્યારે (cur + n > limit)
      limit *= 2;
   q = (અક્ષર *) STBI_REALLOC_SIZED(z->zout_start, old_limit, limit);
   STBI_NOTUSED(old_limit);
   જો (q == ખાલી) પાછા stbi__err("outofmem", "Out of memory");
   z->zout_start = q;
   z->zout       = q + cur;
   z->zout_end   = q + limit;
   પાછા 1;
}

સ્થિર અચલ પૂર્ણાંક stbi__zlength_base[31] = {
   3,4,5,6,7,8,9,10,11,13,
   15,17,19,23,27,31,35,43,51,59,
   67,83,99,115,131,163,195,227,258,0,0 };

સ્થિર અચલ પૂર્ણાંક stbi__zlength_extra[31]=
{ 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };

સ્થિર અચલ પૂર્ણાંક stbi__zdist_base[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0};

સ્થિર અચલ પૂર્ણાંક stbi__zdist_extra[32] =
{ 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

સ્થિર પૂર્ણાંક stbi__parse_huffman_block(stbi__zbuf *a)
{
   અક્ષર *zout = a->zout;
   માટે(;;) {
      પૂર્ણાંક z = stbi__zhuffman_decode(a, &a->z_length);
      જો (z < 256) {
         જો (z < 0) પાછા stbi__err("bad huffman code","Corrupt PNG"); // error in huffman codes
         જો (zout >= a->zout_end) {
            જો (!stbi__zexpand(a, zout, 1)) પાછા 0;
            zout = a->zout;
         }
         *zout++ = (અક્ષર) z;
      } નહીંતર {
         stbi_uc *p;
         પૂર્ણાંક len,dist;
         જો (z == 256) {
            a->zout = zout;
            પાછા 1;
         }
         z -= 257;
         len = stbi__zlength_base[z];
         જો (stbi__zlength_extra[z]) len += stbi__zreceive(a, stbi__zlength_extra[z]);
         z = stbi__zhuffman_decode(a, &a->z_distance);
         જો (z < 0) પાછા stbi__err("bad huffman code","Corrupt PNG");
         dist = stbi__zdist_base[z];
         જો (stbi__zdist_extra[z]) dist += stbi__zreceive(a, stbi__zdist_extra[z]);
         જો (zout - a->zout_start < dist) પાછા stbi__err("bad dist","Corrupt PNG");
         જો (zout + len > a->zout_end) {
            જો (!stbi__zexpand(a, zout, len)) પાછા 0;
            zout = a->zout;
         }
         p = (stbi_uc *) (zout - dist);
         જો (dist == 1) { // run of one byte; common in images.
            stbi_uc v = *p;
            જો (len) { કરો *zout++ = v; જ્યારે (--len); }
         } નહીંતર {
            જો (len) { કરો *zout++ = *p++; જ્યારે (--len); }
         }
      }
   }
}

સ્થિર પૂર્ણાંક stbi__compute_huffman_codes(stbi__zbuf *a)
{
   સ્થિર અચલ stbi_uc length_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
   stbi__zhuffman z_codelength;
   stbi_uc lencodes[286+32+137];//padding for maximum single op
   stbi_uc codelength_sizes[19];
   પૂર્ણાંક i,n;

   પૂર્ણાંક hlit  = stbi__zreceive(a,5) + 257;
   પૂર્ણાંક hdist = stbi__zreceive(a,5) + 1;
   પૂર્ણાંક hclen = stbi__zreceive(a,4) + 4;
   પૂર્ણાંક ntot  = hlit + hdist;

   memset(codelength_sizes, 0, કદ(codelength_sizes));
   માટે (i=0; i < hclen; ++i) {
      પૂર્ણાંક s = stbi__zreceive(a,3);
      codelength_sizes[length_dezigzag[i]] = (stbi_uc) s;
   }
   જો (!stbi__zbuild_huffman(&z_codelength, codelength_sizes, 19)) પાછા 0;

   n = 0;
   જ્યારે (n < ntot) {
      પૂર્ણાંક c = stbi__zhuffman_decode(a, &z_codelength);
      જો (c < 0 || c >= 19) પાછા stbi__err("bad codelengths", "Corrupt PNG");
      જો (c < 16)
         lencodes[n++] = (stbi_uc) c;
      નહીંતર {
         stbi_uc fill = 0;
         જો (c == 16) {
            c = stbi__zreceive(a,2)+3;
            જો (n == 0) પાછા stbi__err("bad codelengths", "Corrupt PNG");
            fill = lencodes[n-1];
         } નહીંતર જો (c == 17)
            c = stbi__zreceive(a,3)+3;
         નહીંતર {
            STBI_ASSERT(c == 18);
            c = stbi__zreceive(a,7)+11;
         }
         જો (ntot - n < c) પાછા stbi__err("bad codelengths", "Corrupt PNG");
         memset(lencodes+n, fill, c);
         n += c;
      }
   }
   જો (n != ntot) પાછા stbi__err("bad codelengths","Corrupt PNG");
   જો (!stbi__zbuild_huffman(&a->z_length, lencodes, hlit)) પાછા 0;
   જો (!stbi__zbuild_huffman(&a->z_distance, lencodes+hlit, hdist)) પાછા 0;
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__parse_uncompressed_block(stbi__zbuf *a)
{
   stbi_uc header[4];
   પૂર્ણાંક len,nlen,k;
   જો (a->અંક_bits & 7)
      stbi__zreceive(a, a->અંક_bits & 7); // discard
   // drain the bit-packed data into header
   k = 0;
   જ્યારે (a->અંક_bits > 0) {
      header[k++] = (stbi_uc) (a->code_buffer & 255); // suppress MSVC run-time check
      a->code_buffer >>= 8;
      a->અંક_bits -= 8;
   }
   STBI_ASSERT(a->અંક_bits == 0);
   // now fill header the normal way
   જ્યારે (k < 4)
      header[k++] = stbi__zget8(a);
   len  = header[1] * 256 + header[0];
   nlen = header[3] * 256 + header[2];
   જો (nlen != (len ^ 0xffff)) પાછા stbi__err("zlib corrupt","Corrupt PNG");
   જો (a->zbuffer + len > a->zbuffer_end) પાછા stbi__err("read past buffer","Corrupt PNG");
   જો (a->zout + len > a->zout_end)
      જો (!stbi__zexpand(a, a->zout, len)) પાછા 0;
   memcpy(a->zout, a->zbuffer, len);
   a->zbuffer += len;
   a->zout += len;
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__parse_zlib_header(stbi__zbuf *a)
{
   પૂર્ણાંક cmf   = stbi__zget8(a);
   પૂર્ણાંક cm    = cmf & 15;
   /* int cinfo = cmf >> 4; */
   પૂર્ણાંક flg   = stbi__zget8(a);
   જો ((cmf*256+flg) % 31 != 0) પાછા stbi__err("bad zlib header","Corrupt PNG"); // zlib spec
   જો (flg & 32) પાછા stbi__err("no preset dict","Corrupt PNG"); // preset dictionary not allowed in png
   જો (cm != 8) પાછા stbi__err("bad compression","Corrupt PNG"); // DEFLATE required for png
   // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
   પાછા 1;
}

સ્થિર અચલ stbi_uc stbi__zdefault_length[288] =
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
સ્થિર અચલ stbi_uc stbi__zdefault_distance[32] =
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

સ્થિર પૂર્ણાંક stbi__parse_zlib(stbi__zbuf *a, પૂર્ણાંક parse_header)
{
   પૂર્ણાંક final, type;
   જો (parse_header)
      જો (!stbi__parse_zlib_header(a)) પાછા 0;
   a->અંક_bits = 0;
   a->code_buffer = 0;
   કરો {
      final = stbi__zreceive(a,1);
      type = stbi__zreceive(a,2);
      જો (type == 0) {
         જો (!stbi__parse_uncompressed_block(a)) પાછા 0;
      } નહીંતર જો (type == 3) {
         પાછા 0;
      } નહીંતર {
         જો (type == 1) {
            // use fixed code lengths
            જો (!stbi__zbuild_huffman(&a->z_length  , stbi__zdefault_length  , 288)) પાછા 0;
            જો (!stbi__zbuild_huffman(&a->z_distance, stbi__zdefault_distance,  32)) પાછા 0;
         } નહીંતર {
            જો (!stbi__compute_huffman_codes(a)) પાછા 0;
         }
         જો (!stbi__parse_huffman_block(a)) પાછા 0;
      }
   } જ્યારે (!final);
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__do_zlib(stbi__zbuf *a, અક્ષર *obuf, પૂર્ણાંક olen, પૂર્ણાંક exp, પૂર્ણાંક parse_header)
{
   a->zout_start = obuf;
   a->zout       = obuf;
   a->zout_end   = obuf + olen;
   a->z_expandable = exp;

   પાછા stbi__parse_zlib(a, parse_header);
}

STBIDEF અક્ષર *stbi_zlib_decode_malloc_guesssize(અચલ અક્ષર *buffer, પૂર્ણાંક len, પૂર્ણાંક initial_size, પૂર્ણાંક *outlen)
{
   stbi__zbuf a;
   અક્ષર *p = (અક્ષર *) stbi__malloc(initial_size);
   જો (p == ખાલી) પાછા ખાલી;
   a.zbuffer = (stbi_uc *) buffer;
   a.zbuffer_end = (stbi_uc *) buffer + len;
   જો (stbi__do_zlib(&a, p, initial_size, 1, 1)) {
      જો (outlen) *outlen = (પૂર્ણાંક) (a.zout - a.zout_start);
      પાછા a.zout_start;
   } નહીંતર {
      STBI_FREE(a.zout_start);
      પાછા ખાલી;
   }
}

STBIDEF અક્ષર *stbi_zlib_decode_malloc(અક્ષર અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *outlen)
{
   પાછા stbi_zlib_decode_malloc_guesssize(buffer, len, 16384, outlen);
}

STBIDEF અક્ષર *stbi_zlib_decode_malloc_guesssize_headerflag(અચલ અક્ષર *buffer, પૂર્ણાંક len, પૂર્ણાંક initial_size, પૂર્ણાંક *outlen, પૂર્ણાંક parse_header)
{
   stbi__zbuf a;
   અક્ષર *p = (અક્ષર *) stbi__malloc(initial_size);
   જો (p == ખાલી) પાછા ખાલી;
   a.zbuffer = (stbi_uc *) buffer;
   a.zbuffer_end = (stbi_uc *) buffer + len;
   જો (stbi__do_zlib(&a, p, initial_size, 1, parse_header)) {
      જો (outlen) *outlen = (પૂર્ણાંક) (a.zout - a.zout_start);
      પાછા a.zout_start;
   } નહીંતર {
      STBI_FREE(a.zout_start);
      પાછા ખાલી;
   }
}

STBIDEF પૂર્ણાંક stbi_zlib_decode_buffer(અક્ષર *obuffer, પૂર્ણાંક olen, અક્ષર અચલ *ibuffer, પૂર્ણાંક ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_end = (stbi_uc *) ibuffer + ilen;
   જો (stbi__do_zlib(&a, obuffer, olen, 0, 1))
      પાછા (પૂર્ણાંક) (a.zout - a.zout_start);
   નહીંતર
      પાછા -1;
}

STBIDEF અક્ષર *stbi_zlib_decode_noheader_malloc(અક્ષર અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *outlen)
{
   stbi__zbuf a;
   અક્ષર *p = (અક્ષર *) stbi__malloc(16384);
   જો (p == ખાલી) પાછા ખાલી;
   a.zbuffer = (stbi_uc *) buffer;
   a.zbuffer_end = (stbi_uc *) buffer+len;
   જો (stbi__do_zlib(&a, p, 16384, 1, 0)) {
      જો (outlen) *outlen = (પૂર્ણાંક) (a.zout - a.zout_start);
      પાછા a.zout_start;
   } નહીંતર {
      STBI_FREE(a.zout_start);
      પાછા ખાલી;
   }
}

STBIDEF પૂર્ણાંક stbi_zlib_decode_noheader_buffer(અક્ષર *obuffer, પૂર્ણાંક olen, અચલ અક્ષર *ibuffer, પૂર્ણાંક ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_end = (stbi_uc *) ibuffer + ilen;
   જો (stbi__do_zlib(&a, obuffer, olen, 0, 0))
      પાછા (પૂર્ણાંક) (a.zout - a.zout_start);
   નહીંતર
      પાછા -1;
}
#endif

// public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
//    simple implementation
//      - only 8-bit samples
//      - no CRC checking
//      - allocates lots of intermediate memory
//        - avoids problem of streaming data between subsystems
//        - avoids explicit window management
//    performance
//      - uses stb_zlib, a PD zlib implementation with fast huffman decoding

#ifndef STBI_NO_PNG
પ્રકારવ્યાખ્યા સંરચના
{
   stbi__uint32 length;
   stbi__uint32 type;
} stbi__pngchunk;

સ્થિર stbi__pngchunk stbi__get_chunk_header(stbi__context *s)
{
   stbi__pngchunk c;
   c.length = stbi__get32be(s);
   c.type   = stbi__get32be(s);
   પાછા c;
}

સ્થિર પૂર્ણાંક stbi__check_png_header(stbi__context *s)
{
   સ્થિર અચલ stbi_uc png_sig[8] = { 137,80,78,71,13,10,26,10 };
   પૂર્ણાંક i;
   માટે (i=0; i < 8; ++i)
      જો (stbi__get8(s) != png_sig[i]) પાછા stbi__err("bad png sig","Not a PNG");
   પાછા 1;
}

પ્રકારવ્યાખ્યા સંરચના
{
   stbi__context *s;
   stbi_uc *idata, *expanded, *out;
   પૂર્ણાંક depth;
} stbi__png;


ગણના {
   STBI__F_none=0,
   STBI__F_sub=1,
   STBI__F_up=2,
   STBI__F_avg=3,
   STBI__F_paeth=4,
   // synthetic filters used for first scanline to avoid needing a dummy row of 0s
   STBI__F_avg_first,
   STBI__F_paeth_first
};

સ્થિર stbi_uc first_row_filter[5] =
{
   STBI__F_none,
   STBI__F_sub,
   STBI__F_none,
   STBI__F_avg_first,
   STBI__F_paeth_first
};

સ્થિર પૂર્ણાંક stbi__paeth(પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક c)
{
   પૂર્ણાંક p = a + b - c;
   પૂર્ણાંક pa = abs(p-a);
   પૂર્ણાંક pb = abs(p-b);
   પૂર્ણાંક pc = abs(p-c);
   જો (pa <= pb && pa <= pc) પાછા a;
   જો (pb <= pc) પાછા b;
   પાછા c;
}

સ્થિર અચલ stbi_uc stbi__depth_scale_table[9] = { 0, 0xff, 0x55, 0, 0x11, 0,0,0, 0x01 };

// create the png data from post-deflated data
સ્થિર પૂર્ણાંક stbi__create_png_image_raw(stbi__png *a, stbi_uc *raw, stbi__uint32 raw_len, પૂર્ણાંક out_n, stbi__uint32 x, stbi__uint32 y, પૂર્ણાંક depth, પૂર્ણાંક color)
{
   પૂર્ણાંક bytes = (depth == 16? 2 : 1);
   stbi__context *s = a->s;
   stbi__uint32 i,j,stride = x*out_n*bytes;
   stbi__uint32 img_len, img_width_bytes;
   પૂર્ણાંક k;
   પૂર્ણાંક img_n = s->img_n; // copy it into a local for later

   પૂર્ણાંક output_bytes = out_n*bytes;
   પૂર્ણાંક filter_bytes = img_n*bytes;
   પૂર્ણાંક width = x;

   STBI_ASSERT(out_n == s->img_n || out_n == s->img_n+1);
   a->out = (stbi_uc *) stbi__malloc_mad3(x, y, output_bytes, 0); // extra bytes to write off the end into
   જો (!a->out) પાછા stbi__err("outofmem", "Out of memory");

   જો (!stbi__mad3sizes_valid(img_n, x, depth, 7)) પાછા stbi__err("too large", "Corrupt PNG");
   img_width_bytes = (((img_n * x * depth) + 7) >> 3);
   img_len = (img_width_bytes + 1) * y;

   // we used to check for exact match between raw_len and img_len on non-interlaced PNGs,
   // but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),
   // so just check for raw_len < img_len always.
   જો (raw_len < img_len) પાછા stbi__err("not enough pixels","Corrupt PNG");

   માટે (j=0; j < y; ++j) {
      stbi_uc *cur = a->out + stride*j;
      stbi_uc *prior;
      પૂર્ણાંક filter = *raw++;

      જો (filter > 4)
         પાછા stbi__err("invalid filter","Corrupt PNG");

      જો (depth < 8) {
         STBI_ASSERT(img_width_bytes <= x);
         cur += x*out_n - img_width_bytes; // store output to the rightmost img_len bytes, so we can decode in place
         filter_bytes = 1;
         width = img_width_bytes;
      }
      prior = cur - stride; // bugfix: need to compute this after 'cur +=' computation above

      // if first row, use special filter that doesn't sample previous row
      જો (j == 0) filter = first_row_filter[filter];

      // handle first byte explicitly
      માટે (k=0; k < filter_bytes; ++k) {
         પસંદગી (filter) {
            સ્થિતિ STBI__F_none       : cur[k] = raw[k]; તોડો;
            સ્થિતિ STBI__F_sub        : cur[k] = raw[k]; તોડો;
            સ્થિતિ STBI__F_up         : cur[k] = STBI__BYTECAST(raw[k] + prior[k]); તોડો;
            સ્થિતિ STBI__F_avg        : cur[k] = STBI__BYTECAST(raw[k] + (prior[k]>>1)); તોડો;
            સ્થિતિ STBI__F_paeth      : cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(0,prior[k],0)); તોડો;
            સ્થિતિ STBI__F_avg_first  : cur[k] = raw[k]; તોડો;
            સ્થિતિ STBI__F_paeth_first: cur[k] = raw[k]; તોડો;
         }
      }

      જો (depth == 8) {
         જો (img_n != out_n)
            cur[img_n] = 255; // first pixel
         raw += img_n;
         cur += out_n;
         prior += out_n;
      } નહીંતર જો (depth == 16) {
         જો (img_n != out_n) {
            cur[filter_bytes]   = 255; // first pixel top byte
            cur[filter_bytes+1] = 255; // first pixel bottom byte
         }
         raw += filter_bytes;
         cur += output_bytes;
         prior += output_bytes;
      } નહીંતર {
         raw += 1;
         cur += 1;
         prior += 1;
      }

      // this is a little gross, so that we don't switch per-pixel or per-component
      જો (depth < 8 || img_n == out_n) {
         પૂર્ણાંક nk = (width - 1)*filter_bytes;
         #define STBI__CASE(f) \
             સ્થિતિ f:     \
                માટે (k=0; k < nk; ++k)
         પસંદગી (filter) {
            // "none" filter turns into a memcpy here; make that explicit.
            સ્થિતિ STBI__F_none:         memcpy(cur, raw, nk); તોડો;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k-filter_bytes]); } તોડો;
            STBI__CASE(STBI__F_up)           { cur[k] = STBI__BYTECAST(raw[k] + prior[k]); } તોડો;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((prior[k] + cur[k-filter_bytes])>>1)); } તોડો;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_bytes],prior[k],prior[k-filter_bytes])); } તોડો;
            STBI__CASE(STBI__F_avg_first)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k-filter_bytes] >> 1)); } તોડો;
            STBI__CASE(STBI__F_paeth_first)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_bytes],0,0)); } તોડો;
         }
         #undef STBI__CASE
         raw += nk;
      } નહીંતર {
         STBI_ASSERT(img_n+1 == out_n);
         #define STBI__CASE(f) \
             સ્થિતિ f:     \
                માટે (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) \
                   માટે (k=0; k < filter_bytes; ++k)
         પસંદગી (filter) {
            STBI__CASE(STBI__F_none)         { cur[k] = raw[k]; } તોડો;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k- output_bytes]); } તોડો;
            STBI__CASE(STBI__F_up)           { cur[k] = STBI__BYTECAST(raw[k] + prior[k]); } તોડો;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((prior[k] + cur[k- output_bytes])>>1)); } તોડો;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_bytes],prior[k],prior[k- output_bytes])); } તોડો;
            STBI__CASE(STBI__F_avg_first)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k- output_bytes] >> 1)); } તોડો;
            STBI__CASE(STBI__F_paeth_first)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_bytes],0,0)); } તોડો;
         }
         #undef STBI__CASE

         // the loop above sets the high byte of the pixels' alpha, but for
         // 16 bit png files we also need the low byte set. we'll do that here.
         જો (depth == 16) {
            cur = a->out + stride*j; // start at the beginning of the row again
            માટે (i=0; i < x; ++i,cur+=output_bytes) {
               cur[filter_bytes+1] = 255;
            }
         }
      }
   }

   // we make a separate pass to expand bits to pixels; for performance,
   // this could run two scanlines behind the above code, so it won't
   // intefere with filtering but will still be in the cache.
   જો (depth < 8) {
      માટે (j=0; j < y; ++j) {
         stbi_uc *cur = a->out + stride*j;
         stbi_uc *in  = a->out + stride*j + x*out_n - img_width_bytes;
         // unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit
         // png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop
         stbi_uc scale = (color == 0) ? stbi__depth_scale_table[depth] : 1; // scale grayscale values to 0..255 range

         // note that the final byte might overshoot and write more data than desired.
         // we can allocate enough data that this never writes out of memory, but it
         // could also overwrite the next scanline. can it overwrite non-empty data
         // on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.
         // so we need to explicitly clamp the final ones

         જો (depth == 4) {
            માટે (k=x*img_n; k >= 2; k-=2, ++in) {
               *cur++ = scale * ((*in >> 4)       );
               *cur++ = scale * ((*in     ) & 0x0f);
            }
            જો (k > 0) *cur++ = scale * ((*in >> 4)       );
         } નહીંતર જો (depth == 2) {
            માટે (k=x*img_n; k >= 4; k-=4, ++in) {
               *cur++ = scale * ((*in >> 6)       );
               *cur++ = scale * ((*in >> 4) & 0x03);
               *cur++ = scale * ((*in >> 2) & 0x03);
               *cur++ = scale * ((*in     ) & 0x03);
            }
            જો (k > 0) *cur++ = scale * ((*in >> 6)       );
            જો (k > 1) *cur++ = scale * ((*in >> 4) & 0x03);
            જો (k > 2) *cur++ = scale * ((*in >> 2) & 0x03);
         } નહીંતર જો (depth == 1) {
            માટે (k=x*img_n; k >= 8; k-=8, ++in) {
               *cur++ = scale * ((*in >> 7)       );
               *cur++ = scale * ((*in >> 6) & 0x01);
               *cur++ = scale * ((*in >> 5) & 0x01);
               *cur++ = scale * ((*in >> 4) & 0x01);
               *cur++ = scale * ((*in >> 3) & 0x01);
               *cur++ = scale * ((*in >> 2) & 0x01);
               *cur++ = scale * ((*in >> 1) & 0x01);
               *cur++ = scale * ((*in     ) & 0x01);
            }
            જો (k > 0) *cur++ = scale * ((*in >> 7)       );
            જો (k > 1) *cur++ = scale * ((*in >> 6) & 0x01);
            જો (k > 2) *cur++ = scale * ((*in >> 5) & 0x01);
            જો (k > 3) *cur++ = scale * ((*in >> 4) & 0x01);
            જો (k > 4) *cur++ = scale * ((*in >> 3) & 0x01);
            જો (k > 5) *cur++ = scale * ((*in >> 2) & 0x01);
            જો (k > 6) *cur++ = scale * ((*in >> 1) & 0x01);
         }
         જો (img_n != out_n) {
            પૂર્ણાંક q;
            // insert alpha = 255
            cur = a->out + stride*j;
            જો (img_n == 1) {
               માટે (q=x-1; q >= 0; --q) {
                  cur[q*2+1] = 255;
                  cur[q*2+0] = cur[q];
               }
            } નહીંતર {
               STBI_ASSERT(img_n == 3);
               માટે (q=x-1; q >= 0; --q) {
                  cur[q*4+3] = 255;
                  cur[q*4+2] = cur[q*3+2];
                  cur[q*4+1] = cur[q*3+1];
                  cur[q*4+0] = cur[q*3+0];
               }
            }
         }
      }
   } નહીંતર જો (depth == 16) {
      // force the image data from big-endian to platform-native.
      // this is done in a separate pass due to the decoding relying
      // on the data being untouched, but could probably be done
      // per-line during decode if care is taken.
      stbi_uc *cur = a->out;
      stbi__uint16 *cur16 = (stbi__uint16*)cur;

      માટે(i=0; i < x*y*out_n; ++i,cur16++,cur+=2) {
         *cur16 = (cur[0] << 8) | cur[1];
      }
   }

   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__create_png_image(stbi__png *a, stbi_uc *image_data, stbi__uint32 image_data_len, પૂર્ણાંક out_n, પૂર્ણાંક depth, પૂર્ણાંક color, પૂર્ણાંક interlaced)
{
   પૂર્ણાંક bytes = (depth == 16 ? 2 : 1);
   પૂર્ણાંક out_bytes = out_n * bytes;
   stbi_uc *final;
   પૂર્ણાંક p;
   જો (!interlaced)
      પાછા stbi__create_png_image_raw(a, image_data, image_data_len, out_n, a->s->img_x, a->s->img_y, depth, color);

   // de-interlacing
   final = (stbi_uc *) stbi__malloc_mad3(a->s->img_x, a->s->img_y, out_bytes, 0);
   માટે (p=0; p < 7; ++p) {
      પૂર્ણાંક xorig[] = { 0,4,0,2,0,1,0 };
      પૂર્ણાંક yorig[] = { 0,0,4,0,2,0,1 };
      પૂર્ણાંક xspc[]  = { 8,8,4,4,2,2,1 };
      પૂર્ણાંક yspc[]  = { 8,8,8,4,4,2,2 };
      પૂર્ણાંક i,j,x,y;
      // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
      x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
      y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
      જો (x && y) {
         stbi__uint32 img_len = ((((a->s->img_n * x * depth) + 7) >> 3) + 1) * y;
         જો (!stbi__create_png_image_raw(a, image_data, image_data_len, out_n, x, y, depth, color)) {
            STBI_FREE(final);
            પાછા 0;
         }
         માટે (j=0; j < y; ++j) {
            માટે (i=0; i < x; ++i) {
               પૂર્ણાંક out_y = j*yspc[p]+yorig[p];
               પૂર્ણાંક out_x = i*xspc[p]+xorig[p];
               memcpy(final + out_y*a->s->img_x*out_bytes + out_x*out_bytes,
                      a->out + (j*x+i)*out_bytes, out_bytes);
            }
         }
         STBI_FREE(a->out);
         image_data += img_len;
         image_data_len -= img_len;
      }
   }
   a->out = final;

   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__compute_transparency(stbi__png *z, stbi_uc tc[3], પૂર્ણાંક out_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ગણતરી = s->img_x * s->img_y;
   stbi_uc *p = z->out;

   // compute color-based transparency, assuming we've
   // already got 255 as the alpha value in the output
   STBI_ASSERT(out_n == 2 || out_n == 4);

   જો (out_n == 2) {
      માટે (i=0; i < pixel_ગણતરી; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 255);
         p += 2;
      }
   } નહીંતર {
      માટે (i=0; i < pixel_ગણતરી; ++i) {
         જો (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__compute_transparency16(stbi__png *z, stbi__uint16 tc[3], પૂર્ણાંક out_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ગણતરી = s->img_x * s->img_y;
   stbi__uint16 *p = (stbi__uint16*) z->out;

   // compute color-based transparency, assuming we've
   // already got 65535 as the alpha value in the output
   STBI_ASSERT(out_n == 2 || out_n == 4);

   જો (out_n == 2) {
      માટે (i = 0; i < pixel_ગણતરી; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 65535);
         p += 2;
      }
   } નહીંતર {
      માટે (i = 0; i < pixel_ગણતરી; ++i) {
         જો (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__expand_png_palette(stbi__png *a, stbi_uc *palette, પૂર્ણાંક len, પૂર્ણાંક pal_img_n)
{
   stbi__uint32 i, pixel_ગણતરી = a->s->img_x * a->s->img_y;
   stbi_uc *p, *temp_out, *orig = a->out;

   p = (stbi_uc *) stbi__malloc_mad2(pixel_ગણતરી, pal_img_n, 0);
   જો (p == ખાલી) પાછા stbi__err("outofmem", "Out of memory");

   // between here and free(out) below, exitting would leak
   temp_out = p;

   જો (pal_img_n == 3) {
      માટે (i=0; i < pixel_ગણતરી; ++i) {
         પૂર્ણાંક n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p += 3;
      }
   } નહીંતર {
      માટે (i=0; i < pixel_ગણતરી; ++i) {
         પૂર્ણાંક n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p[3] = palette[n+3];
         p += 4;
      }
   }
   STBI_FREE(a->out);
   a->out = temp_out;

   STBI_NOTUSED(len);

   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__unpremultiply_on_load = 0;
સ્થિર પૂર્ણાંક stbi__de_iphone_flag = 0;

STBIDEF શૂન્ય stbi_set_unpremultiply_on_load(પૂર્ણાંક flag_true_if_should_unpremultiply)
{
   stbi__unpremultiply_on_load = flag_true_if_should_unpremultiply;
}

STBIDEF શૂન્ય stbi_convert_iphone_png_to_rgb(પૂર્ણાંક flag_true_if_should_convert)
{
   stbi__de_iphone_flag = flag_true_if_should_convert;
}

સ્થિર શૂન્ય stbi__de_iphone(stbi__png *z)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_ગણતરી = s->img_x * s->img_y;
   stbi_uc *p = z->out;

   જો (s->img_out_n == 3) {  // convert bgr to rgb
      માટે (i=0; i < pixel_ગણતરી; ++i) {
         stbi_uc t = p[0];
         p[0] = p[2];
         p[2] = t;
         p += 3;
      }
   } નહીંતર {
      STBI_ASSERT(s->img_out_n == 4);
      જો (stbi__unpremultiply_on_load) {
         // convert bgr to rgb and unpremultiply
         માટે (i=0; i < pixel_ગણતરી; ++i) {
            stbi_uc a = p[3];
            stbi_uc t = p[0];
            જો (a) {
               stbi_uc half = a / 2;
               p[0] = (p[2] * 255 + half) / a;
               p[1] = (p[1] * 255 + half) / a;
               p[2] = ( t   * 255 + half) / a;
            } નહીંતર {
               p[0] = p[2];
               p[2] = t;
            }
            p += 4;
         }
      } નહીંતર {
         // convert bgr to rgb
         માટે (i=0; i < pixel_ગણતરી; ++i) {
            stbi_uc t = p[0];
            p[0] = p[2];
            p[2] = t;
            p += 4;
         }
      }
   }
}

#define STBI__PNG_TYPE(a,b,c,d)  (((unsigned) (a) << 24) + ((unsigned) (b) << 16) + ((unsigned) (c) << 8) + (unsigned) (d))

સ્થિર પૂર્ણાંક stbi__parse_png_file(stbi__png *z, પૂર્ણાંક scan, પૂર્ણાંક req_comp)
{
   stbi_uc palette[1024], pal_img_n=0;
   stbi_uc has_trans=0, tc[3];
   stbi__uint16 tc16[3];
   stbi__uint32 ioff=0, idata_limit=0, i, pal_len=0;
   પૂર્ણાંક first=1,k,interlace=0, color=0, is_iphone=0;
   stbi__context *s = z->s;

   z->expanded = ખાલી;
   z->idata = ખાલી;
   z->out = ખાલી;

   જો (!stbi__check_png_header(s)) પાછા 0;

   જો (scan == STBI__SCAN_type) પાછા 1;

   માટે (;;) {
      stbi__pngchunk c = stbi__get_chunk_header(s);
      પસંદગી (c.type) {
         સ્થિતિ STBI__PNG_TYPE('C','g','B','I'):
            is_iphone = 1;
            stbi__skip(s, c.length);
            તોડો;
         સ્થિતિ STBI__PNG_TYPE('I','H','D','R'): {
            પૂર્ણાંક comp,filter;
            જો (!first) પાછા stbi__err("multiple IHDR","Corrupt PNG");
            first = 0;
            જો (c.length != 13) પાછા stbi__err("bad IHDR len","Corrupt PNG");
            s->img_x = stbi__get32be(s); જો (s->img_x > (1 << 24)) પાછા stbi__err("too large","Very large image (corrupt?)");
            s->img_y = stbi__get32be(s); જો (s->img_y > (1 << 24)) પાછા stbi__err("too large","Very large image (corrupt?)");
            z->depth = stbi__get8(s);  જો (z->depth != 1 && z->depth != 2 && z->depth != 4 && z->depth != 8 && z->depth != 16)  પાછા stbi__err("1/2/4/8/16-bit only","PNG not supported: 1/2/4/8/16-bit only");
            color = stbi__get8(s);  જો (color > 6)         પાછા stbi__err("bad ctype","Corrupt PNG");
            જો (color == 3 && z->depth == 16)                  પાછા stbi__err("bad ctype","Corrupt PNG");
            જો (color == 3) pal_img_n = 3; નહીંતર જો (color & 1) પાછા stbi__err("bad ctype","Corrupt PNG");
            comp  = stbi__get8(s);  જો (comp) પાછા stbi__err("bad comp method","Corrupt PNG");
            filter= stbi__get8(s);  જો (filter) પાછા stbi__err("bad filter method","Corrupt PNG");
            interlace = stbi__get8(s); જો (interlace>1) પાછા stbi__err("bad interlace method","Corrupt PNG");
            જો (!s->img_x || !s->img_y) પાછા stbi__err("0-pixel image","Corrupt PNG");
            જો (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               જો ((1 << 30) / s->img_x / s->img_n < s->img_y) પાછા stbi__err("too large", "Image too large to decode");
               જો (scan == STBI__SCAN_header) પાછા 1;
            } નહીંતર {
               // if paletted, then pal_n is our final components, and
               // img_n is # components to decompress/filter.
               s->img_n = 1;
               જો ((1 << 30) / s->img_x / 4 < s->img_y) પાછા stbi__err("too large","Corrupt PNG");
               // if SCAN_header, have to scan to see if we have a tRNS
            }
            તોડો;
         }

         સ્થિતિ STBI__PNG_TYPE('P','L','T','E'):  {
            જો (first) પાછા stbi__err("first not IHDR", "Corrupt PNG");
            જો (c.length > 256*3) પાછા stbi__err("invalid PLTE","Corrupt PNG");
            pal_len = c.length / 3;
            જો (pal_len * 3 != c.length) પાછા stbi__err("invalid PLTE","Corrupt PNG");
            માટે (i=0; i < pal_len; ++i) {
               palette[i*4+0] = stbi__get8(s);
               palette[i*4+1] = stbi__get8(s);
               palette[i*4+2] = stbi__get8(s);
               palette[i*4+3] = 255;
            }
            તોડો;
         }

         સ્થિતિ STBI__PNG_TYPE('t','R','N','S'): {
            જો (first) પાછા stbi__err("first not IHDR", "Corrupt PNG");
            જો (z->idata) પાછા stbi__err("tRNS after IDAT","Corrupt PNG");
            જો (pal_img_n) {
               જો (scan == STBI__SCAN_header) { s->img_n = 4; પાછા 1; }
               જો (pal_len == 0) પાછા stbi__err("tRNS before PLTE","Corrupt PNG");
               જો (c.length > pal_len) પાછા stbi__err("bad tRNS len","Corrupt PNG");
               pal_img_n = 4;
               માટે (i=0; i < c.length; ++i)
                  palette[i*4+3] = stbi__get8(s);
            } નહીંતર {
               જો (!(s->img_n & 1)) પાછા stbi__err("tRNS with alpha","Corrupt PNG");
               જો (c.length != (stbi__uint32) s->img_n*2) પાછા stbi__err("bad tRNS len","Corrupt PNG");
               has_trans = 1;
               જો (z->depth == 16) {
                  માટે (k = 0; k < s->img_n; ++k) tc16[k] = (stbi__uint16)stbi__get16be(s); // copy the values as-is
               } નહીંતર {
                  માટે (k = 0; k < s->img_n; ++k) tc[k] = (stbi_uc)(stbi__get16be(s) & 255) * stbi__depth_scale_table[z->depth]; // non 8-bit images will be larger
               }
            }
            તોડો;
         }

         સ્થિતિ STBI__PNG_TYPE('I','D','A','T'): {
            જો (first) પાછા stbi__err("first not IHDR", "Corrupt PNG");
            જો (pal_img_n && !pal_len) પાછા stbi__err("no PLTE","Corrupt PNG");
            જો (scan == STBI__SCAN_header) { s->img_n = pal_img_n; પાછા 1; }
            જો ((પૂર્ણાંક)(ioff + c.length) < (પૂર્ણાંક)ioff) પાછા 0;
            જો (ioff + c.length > idata_limit) {
               stbi__uint32 idata_limit_old = idata_limit;
               stbi_uc *p;
               જો (idata_limit == 0) idata_limit = c.length > 4096 ? c.length : 4096;
               જ્યારે (ioff + c.length > idata_limit)
                  idata_limit *= 2;
               STBI_NOTUSED(idata_limit_old);
               p = (stbi_uc *) STBI_REALLOC_SIZED(z->idata, idata_limit_old, idata_limit); જો (p == ખાલી) પાછા stbi__err("outofmem", "Out of memory");
               z->idata = p;
            }
            જો (!stbi__getn(s, z->idata+ioff,c.length)) પાછા stbi__err("outofdata","Corrupt PNG");
            ioff += c.length;
            તોડો;
         }

         સ્થિતિ STBI__PNG_TYPE('I','E','N','D'): {
            stbi__uint32 raw_len, bpl;
            જો (first) પાછા stbi__err("first not IHDR", "Corrupt PNG");
            જો (scan != STBI__SCAN_load) પાછા 1;
            જો (z->idata == ખાલી) પાછા stbi__err("no IDAT","Corrupt PNG");
            // initial guess for decoded data size to avoid unnecessary reallocs
            bpl = (s->img_x * z->depth + 7) / 8; // bytes per line, per component
            raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
            z->expanded = (stbi_uc *) stbi_zlib_decode_malloc_guesssize_headerflag((અક્ષર *) z->idata, ioff, raw_len, (પૂર્ણાંક *) &raw_len, !is_iphone);
            જો (z->expanded == ખાલી) પાછા 0; // zlib should set error
            STBI_FREE(z->idata); z->idata = ખાલી;
            જો ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_out_n = s->img_n+1;
            નહીંતર
               s->img_out_n = s->img_n;
            જો (!stbi__create_png_image(z, z->expanded, raw_len, s->img_out_n, z->depth, color, interlace)) પાછા 0;
            જો (has_trans) {
               જો (z->depth == 16) {
                  જો (!stbi__compute_transparency16(z, tc16, s->img_out_n)) પાછા 0;
               } નહીંતર {
                  જો (!stbi__compute_transparency(z, tc, s->img_out_n)) પાછા 0;
               }
            }
            જો (is_iphone && stbi__de_iphone_flag && s->img_out_n > 2)
               stbi__de_iphone(z);
            જો (pal_img_n) {
               // pal_img_n == 3 or 4
               s->img_n = pal_img_n; // record the actual colors we had
               s->img_out_n = pal_img_n;
               જો (req_comp >= 3) s->img_out_n = req_comp;
               જો (!stbi__expand_png_palette(z, palette, pal_len, s->img_out_n))
                  પાછા 0;
            } નહીંતર જો (has_trans) {
               // non-paletted image with tRNS -> source image has (constant) alpha
               ++s->img_n;
            }
            STBI_FREE(z->expanded); z->expanded = ખાલી;
            પાછા 1;
         }

         મૂળભૂત:
            // if critical, fail
            જો (first) પાછા stbi__err("first not IHDR", "Corrupt PNG");
            જો ((c.type & (1 << 29)) == 0) {
               #ifndef STBI_NO_FAILURE_STRINGS
               // not threadsafe
               સ્થિર અક્ષર invalid_chunk[] = "XXXX PNG chunk not known";
               invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
               invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
               invalid_chunk[2] = STBI__BYTECAST(c.type >>  8);
               invalid_chunk[3] = STBI__BYTECAST(c.type >>  0);
               #endif
               પાછા stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
            }
            stbi__skip(s, c.length);
            તોડો;
      }
      // end of PNG chunk, read and skip CRC
      stbi__get32be(s);
   }
}

સ્થિર શૂન્ય *stbi__do_png(stbi__png *p, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *n, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   શૂન્ય *result=ખાલી;
   જો (req_comp < 0 || req_comp > 4) પાછા stbi__errpuc("bad req_comp", "Internal error");
   જો (stbi__parse_png_file(p, STBI__SCAN_load, req_comp)) {
      જો (p->depth < 8)
         ri->bits_per_channel = 8;
      નહીંતર
         ri->bits_per_channel = p->depth;
      result = p->out;
      p->out = ખાલી;
      જો (req_comp && req_comp != p->s->img_out_n) {
         જો (ri->bits_per_channel == 8)
            result = stbi__convert_format((unsigned અક્ષર *) result, p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y);
         નહીંતર
            result = stbi__convert_format16((stbi__uint16 *) result, p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y);
         p->s->img_out_n = req_comp;
         જો (result == ખાલી) પાછા result;
      }
      *x = p->s->img_x;
      *y = p->s->img_y;
      જો (n) *n = p->s->img_n;
   }
   STBI_FREE(p->out);      p->out      = ખાલી;
   STBI_FREE(p->expanded); p->expanded = ખાલી;
   STBI_FREE(p->idata);    p->idata    = ખાલી;

   પાછા result;
}

સ્થિર શૂન્ય *stbi__png_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   stbi__png p;
   p.s = s;
   પાછા stbi__do_png(&p, x,y,comp,req_comp, ri);
}

સ્થિર પૂર્ણાંક stbi__png_test(stbi__context *s)
{
   પૂર્ણાંક r;
   r = stbi__check_png_header(s);
   stbi__rewind(s);
   પાછા r;
}

સ્થિર પૂર્ણાંક stbi__png_info_raw(stbi__png *p, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   જો (!stbi__parse_png_file(p, STBI__SCAN_header, 0)) {
      stbi__rewind( p->s );
      પાછા 0;
   }
   જો (x) *x = p->s->img_x;
   જો (y) *y = p->s->img_y;
   જો (comp) *comp = p->s->img_n;
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__png_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   stbi__png p;
   p.s = s;
   પાછા stbi__png_info_raw(&p, x, y, comp);
}

સ્થિર પૂર્ણાંક stbi__png_is16(stbi__context *s)
{
   stbi__png p;
   p.s = s;
   જો (!stbi__png_info_raw(&p, ખાલી, ખાલી, ખાલી))
	   પાછા 0;
   જો (p.depth != 16) {
      stbi__rewind(p.s);
      પાછા 0;
   }
   પાછા 1;
}
#endif

// Microsoft/Windows BMP image

#ifndef STBI_NO_BMP
સ્થિર પૂર્ણાંક stbi__bmp_test_raw(stbi__context *s)
{
   પૂર્ણાંક r;
   પૂર્ણાંક sz;
   જો (stbi__get8(s) != 'B') પાછા 0;
   જો (stbi__get8(s) != 'M') પાછા 0;
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   stbi__get32le(s); // discard data offset
   sz = stbi__get32le(s);
   r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
   પાછા r;
}

સ્થિર પૂર્ણાંક stbi__bmp_test(stbi__context *s)
{
   પૂર્ણાંક r = stbi__bmp_test_raw(s);
   stbi__rewind(s);
   પાછા r;
}


// returns 0..31 for the highest set bit
સ્થિર પૂર્ણાંક stbi_ઉપલું_bit(unsigned પૂર્ણાંક z)
{
   પૂર્ણાંક n=0;
   જો (z == 0) પાછા -1;
   જો (z >= 0x10000) n += 16, z >>= 16;
   જો (z >= 0x00100) n +=  8, z >>=  8;
   જો (z >= 0x00010) n +=  4, z >>=  4;
   જો (z >= 0x00004) n +=  2, z >>=  2;
   જો (z >= 0x00002) n +=  1, z >>=  1;
   પાછા n;
}

સ્થિર પૂર્ણાંક stbi__bitcount(unsigned પૂર્ણાંક a)
{
   a = (a & 0x55555555) + ((a >>  1) & 0x55555555); // max 2
   a = (a & 0x33333333) + ((a >>  2) & 0x33333333); // max 4
   a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
   a = (a + (a >> 8)); // max 16 per 8 bits
   a = (a + (a >> 16)); // max 32 per 8 bits
   પાછા a & 0xff;
}

// extract an arbitrarily-aligned N-bit value (N=bits)
// from v, and then make it 8-bits long and fractionally
// extend it to full full range.
સ્થિર પૂર્ણાંક stbi__shiftsigned(પૂર્ણાંક v, પૂર્ણાંક shift, પૂર્ણાંક bits)
{
   સ્થિર unsigned પૂર્ણાંક mul_table[9] = {
      0,
      0xff/*0b11111111*/, 0x55/*0b01010101*/, 0x49/*0b01001001*/, 0x11/*0b00010001*/,
      0x21/*0b00100001*/, 0x41/*0b01000001*/, 0x81/*0b10000001*/, 0x01/*0b00000001*/,
   };
   સ્થિર unsigned પૂર્ણાંક shift_table[9] = {
      0, 0,0,1,0,2,4,6,0,
   };
   જો (shift < 0)
      v <<= -shift;
   નહીંતર
      v >>= shift;
   STBI_ASSERT(v >= 0 && v < 256);
   v >>= (8-bits);
   STBI_ASSERT(bits >= 0 && bits <= 8);
   પાછા (પૂર્ણાંક) ((unsigned) v * mul_table[bits]) >> shift_table[bits];
}

પ્રકારવ્યાખ્યા સંરચના
{
   પૂર્ણાંક bpp, offset, hsz;
   unsigned પૂર્ણાંક mr,mg,mb,ma, all_a;
} stbi__bmp_data;

સ્થિર શૂન્ય *stbi__bmp_parse_header(stbi__context *s, stbi__bmp_data *info)
{
   પૂર્ણાંક hsz;
   જો (stbi__get8(s) != 'B' || stbi__get8(s) != 'M') પાછા stbi__errpuc("not BMP", "Corrupt BMP");
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   info->offset = stbi__get32le(s);
   info->hsz = hsz = stbi__get32le(s);
   info->mr = info->mg = info->mb = info->ma = 0;

   જો (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) પાછા stbi__errpuc("unknown BMP", "BMP type not supported: unknown");
   જો (hsz == 12) {
      s->img_x = stbi__get16le(s);
      s->img_y = stbi__get16le(s);
   } નહીંતર {
      s->img_x = stbi__get32le(s);
      s->img_y = stbi__get32le(s);
   }
   જો (stbi__get16le(s) != 1) પાછા stbi__errpuc("bad BMP", "bad BMP");
   info->bpp = stbi__get16le(s);
   જો (hsz != 12) {
      પૂર્ણાંક compress = stbi__get32le(s);
      જો (compress == 1 || compress == 2) પાછા stbi__errpuc("BMP RLE", "BMP type not supported: RLE");
      stbi__get32le(s); // discard sizeof
      stbi__get32le(s); // discard hres
      stbi__get32le(s); // discard vres
      stbi__get32le(s); // discard colorsused
      stbi__get32le(s); // discard max important
      જો (hsz == 40 || hsz == 56) {
         જો (hsz == 56) {
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
         }
         જો (info->bpp == 16 || info->bpp == 32) {
            જો (compress == 0) {
               જો (info->bpp == 32) {
                  info->mr = 0xffu << 16;
                  info->mg = 0xffu <<  8;
                  info->mb = 0xffu <<  0;
                  info->ma = 0xffu << 24;
                  info->all_a = 0; // if all_a is 0 at end, then we loaded alpha channel but it was all 0
               } નહીંતર {
                  info->mr = 31u << 10;
                  info->mg = 31u <<  5;
                  info->mb = 31u <<  0;
               }
            } નહીંતર જો (compress == 3) {
               info->mr = stbi__get32le(s);
               info->mg = stbi__get32le(s);
               info->mb = stbi__get32le(s);
               // not documented, but generated by photoshop and handled by mspaint
               જો (info->mr == info->mg && info->mg == info->mb) {
                  // ?!?!?
                  પાછા stbi__errpuc("bad BMP", "bad BMP");
               }
            } નહીંતર
               પાછા stbi__errpuc("bad BMP", "bad BMP");
         }
      } નહીંતર {
         પૂર્ણાંક i;
         જો (hsz != 108 && hsz != 124)
            પાછા stbi__errpuc("bad BMP", "bad BMP");
         info->mr = stbi__get32le(s);
         info->mg = stbi__get32le(s);
         info->mb = stbi__get32le(s);
         info->ma = stbi__get32le(s);
         stbi__get32le(s); // discard color space
         માટે (i=0; i < 12; ++i)
            stbi__get32le(s); // discard color space parameters
         જો (hsz == 124) {
            stbi__get32le(s); // discard rendering intent
            stbi__get32le(s); // discard offset of profile data
            stbi__get32le(s); // discard size of profile data
            stbi__get32le(s); // discard reserved
         }
      }
   }
   પાછા (શૂન્ય *) 1;
}


સ્થિર શૂન્ય *stbi__bmp_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   stbi_uc *out;
   unsigned પૂર્ણાંક mr=0,mg=0,mb=0,ma=0, all_a;
   stbi_uc pal[256][4];
   પૂર્ણાંક psize=0,i,j,width;
   પૂર્ણાંક flip_vertically, pad, target;
   stbi__bmp_data info;
   STBI_NOTUSED(ri);

   info.all_a = 255;
   જો (stbi__bmp_parse_header(s, &info) == ખાલી)
      પાછા ખાલી; // error code already set

   flip_vertically = ((પૂર્ણાંક) s->img_y) > 0;
   s->img_y = abs((પૂર્ણાંક) s->img_y);

   mr = info.mr;
   mg = info.mg;
   mb = info.mb;
   ma = info.ma;
   all_a = info.all_a;

   જો (info.hsz == 12) {
      જો (info.bpp < 24)
         psize = (info.offset - 14 - 24) / 3;
   } નહીંતર {
      જો (info.bpp < 16)
         psize = (info.offset - 14 - info.hsz) >> 2;
   }

   s->img_n = ma ? 4 : 3;
   જો (req_comp && req_comp >= 3) // we can directly decode 3 or 4
      target = req_comp;
   નહીંતર
      target = s->img_n; // if they want monochrome, we'll post-convert

   // sanity-check size
   જો (!stbi__mad3sizes_valid(target, s->img_x, s->img_y, 0))
      પાછા stbi__errpuc("too large", "Corrupt BMP");

   out = (stbi_uc *) stbi__malloc_mad3(target, s->img_x, s->img_y, 0);
   જો (!out) પાછા stbi__errpuc("outofmem", "Out of memory");
   જો (info.bpp < 16) {
      પૂર્ણાંક z=0;
      જો (psize == 0 || psize > 256) { STBI_FREE(out); પાછા stbi__errpuc("invalid", "Corrupt BMP"); }
      માટે (i=0; i < psize; ++i) {
         pal[i][2] = stbi__get8(s);
         pal[i][1] = stbi__get8(s);
         pal[i][0] = stbi__get8(s);
         જો (info.hsz != 12) stbi__get8(s);
         pal[i][3] = 255;
      }
      stbi__skip(s, info.offset - 14 - info.hsz - psize * (info.hsz == 12 ? 3 : 4));
      જો (info.bpp == 1) width = (s->img_x + 7) >> 3;
      નહીંતર જો (info.bpp == 4) width = (s->img_x + 1) >> 1;
      નહીંતર જો (info.bpp == 8) width = s->img_x;
      નહીંતર { STBI_FREE(out); પાછા stbi__errpuc("bad bpp", "Corrupt BMP"); }
      pad = (-width)&3;
      જો (info.bpp == 1) {
         માટે (j=0; j < (પૂર્ણાંક) s->img_y; ++j) {
            પૂર્ણાંક bit_offset = 7, v = stbi__get8(s);
            માટે (i=0; i < (પૂર્ણાંક) s->img_x; ++i) {
               પૂર્ણાંક color = (v>>bit_offset)&0x1;
               out[z++] = pal[color][0];
               out[z++] = pal[color][1];
               out[z++] = pal[color][2];
               જો((--bit_offset) < 0) {
                  bit_offset = 7;
                  v = stbi__get8(s);
               }
            }
            stbi__skip(s, pad);
         }
      } નહીંતર {
         માટે (j=0; j < (પૂર્ણાંક) s->img_y; ++j) {
            માટે (i=0; i < (પૂર્ણાંક) s->img_x; i += 2) {
               પૂર્ણાંક v=stbi__get8(s),v2=0;
               જો (info.bpp == 4) {
                  v2 = v & 15;
                  v >>= 4;
               }
               out[z++] = pal[v][0];
               out[z++] = pal[v][1];
               out[z++] = pal[v][2];
               જો (target == 4) out[z++] = 255;
               જો (i+1 == (પૂર્ણાંક) s->img_x) તોડો;
               v = (info.bpp == 8) ? stbi__get8(s) : v2;
               out[z++] = pal[v][0];
               out[z++] = pal[v][1];
               out[z++] = pal[v][2];
               જો (target == 4) out[z++] = 255;
            }
            stbi__skip(s, pad);
         }
      }
   } નહીંતર {
      પૂર્ણાંક rshift=0,gshift=0,bshift=0,ashift=0,rcount=0,gcount=0,bcount=0,acount=0;
      પૂર્ણાંક z = 0;
      પૂર્ણાંક easy=0;
      stbi__skip(s, info.offset - 14 - info.hsz);
      જો (info.bpp == 24) width = 3 * s->img_x;
      નહીંતર જો (info.bpp == 16) width = 2*s->img_x;
      નહીંતર /* bpp = 32 and pad = 0 */ width=0;
      pad = (-width) & 3;
      જો (info.bpp == 24) {
         easy = 1;
      } નહીંતર જો (info.bpp == 32) {
         જો (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
            easy = 2;
      }
      જો (!easy) {
         જો (!mr || !mg || !mb) { STBI_FREE(out); પાછા stbi__errpuc("bad masks", "Corrupt BMP"); }
         // right shift amt to put high bit in position #7
         rshift = stbi_ઉપલું_bit(mr)-7; rcount = stbi__bitcount(mr);
         gshift = stbi_ઉપલું_bit(mg)-7; gcount = stbi__bitcount(mg);
         bshift = stbi_ઉપલું_bit(mb)-7; bcount = stbi__bitcount(mb);
         ashift = stbi_ઉપલું_bit(ma)-7; acount = stbi__bitcount(ma);
      }
      માટે (j=0; j < (પૂર્ણાંક) s->img_y; ++j) {
         જો (easy) {
            માટે (i=0; i < (પૂર્ણાંક) s->img_x; ++i) {
               unsigned અક્ષર a;
               out[z+2] = stbi__get8(s);
               out[z+1] = stbi__get8(s);
               out[z+0] = stbi__get8(s);
               z += 3;
               a = (easy == 2 ? stbi__get8(s) : 255);
               all_a |= a;
               જો (target == 4) out[z++] = a;
            }
         } નહીંતર {
            પૂર્ણાંક bpp = info.bpp;
            માટે (i=0; i < (પૂર્ણાંક) s->img_x; ++i) {
               stbi__uint32 v = (bpp == 16 ? (stbi__uint32) stbi__get16le(s) : stbi__get32le(s));
               unsigned પૂર્ણાંક a;
               out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
               out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
               out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));
               a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
               all_a |= a;
               જો (target == 4) out[z++] = STBI__BYTECAST(a);
            }
         }
         stbi__skip(s, pad);
      }
   }

   // if alpha channel is all 0s, replace with all 255s
   જો (target == 4 && all_a == 0)
      માટે (i=4*s->img_x*s->img_y-1; i >= 0; i -= 4)
         out[i] = 255;

   જો (flip_vertically) {
      stbi_uc t;
      માટે (j=0; j < (પૂર્ણાંક) s->img_y>>1; ++j) {
         stbi_uc *p1 = out +      j     *s->img_x*target;
         stbi_uc *p2 = out + (s->img_y-1-j)*s->img_x*target;
         માટે (i=0; i < (પૂર્ણાંક) s->img_x*target; ++i) {
            t = p1[i], p1[i] = p2[i], p2[i] = t;
         }
      }
   }

   જો (req_comp && req_comp != target) {
      out = stbi__convert_format(out, target, req_comp, s->img_x, s->img_y);
      જો (out == ખાલી) પાછા out; // stbi__convert_format frees input on failure
   }

   *x = s->img_x;
   *y = s->img_y;
   જો (comp) *comp = s->img_n;
   પાછા out;
}
#endif

// Targa Truevision - TGA
// by Jonathan Dummer
#ifndef STBI_NO_TGA
// returns STBI_rgb or whatever, 0 on error
સ્થિર પૂર્ણાંક stbi__tga_get_comp(પૂર્ણાંક bits_per_pixel, પૂર્ણાંક is_grey, પૂર્ણાંક* is_rgb16)
{
   // only RGB or RGBA (incl. 16bit) or grey allowed
   જો (is_rgb16) *is_rgb16 = 0;
   પસંદગી(bits_per_pixel) {
      સ્થિતિ 8:  પાછા STBI_grey;
      સ્થિતિ 16: જો(is_grey) પાછા STBI_grey_alpha;
               // fallthrough
      સ્થિતિ 15: જો(is_rgb16) *is_rgb16 = 1;
               પાછા STBI_rgb;
      સ્થિતિ 24: // fallthrough
      સ્થિતિ 32: પાછા bits_per_pixel/8;
      મૂળભૂત: પાછા 0;
   }
}

સ્થિર પૂર્ણાંક stbi__tga_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
    પૂર્ણાંક tga_w, tga_h, tga_comp, tga_image_type, tga_bits_per_pixel, tga_colormap_bpp;
    પૂર્ણાંક sz, tga_colormap_type;
    stbi__get8(s);                   // discard Offset
    tga_colormap_type = stbi__get8(s); // colormap type
    જો( tga_colormap_type > 1 ) {
        stbi__rewind(s);
        પાછા 0;      // only RGB or indexed allowed
    }
    tga_image_type = stbi__get8(s); // image type
    જો ( tga_colormap_type == 1 ) { // colormapped (paletted) image
        જો (tga_image_type != 1 && tga_image_type != 9) {
            stbi__rewind(s);
            પાછા 0;
        }
        stbi__skip(s,4);       // skip index of first colormap entry and number of entries
        sz = stbi__get8(s);    //   check bits per palette color entry
        જો ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) {
            stbi__rewind(s);
            પાછા 0;
        }
        stbi__skip(s,4);       // skip image x and y origin
        tga_colormap_bpp = sz;
    } નહીંતર { // "normal" image w/o colormap - only RGB or grey allowed, +/- RLE
        જો ( (tga_image_type != 2) && (tga_image_type != 3) && (tga_image_type != 10) && (tga_image_type != 11) ) {
            stbi__rewind(s);
            પાછા 0; // only RGB or grey allowed, +/- RLE
        }
        stbi__skip(s,9); // skip colormap specification and image x/y origin
        tga_colormap_bpp = 0;
    }
    tga_w = stbi__get16le(s);
    જો( tga_w < 1 ) {
        stbi__rewind(s);
        પાછા 0;   // test width
    }
    tga_h = stbi__get16le(s);
    જો( tga_h < 1 ) {
        stbi__rewind(s);
        પાછા 0;   // test height
    }
    tga_bits_per_pixel = stbi__get8(s); // bits per pixel
    stbi__get8(s); // ignore alpha bits
    જો (tga_colormap_bpp != 0) {
        જો((tga_bits_per_pixel != 8) && (tga_bits_per_pixel != 16)) {
            // when using a colormap, tga_bits_per_pixel is the size of the indexes
            // I don't think anything but 8 or 16bit indexes makes sense
            stbi__rewind(s);
            પાછા 0;
        }
        tga_comp = stbi__tga_get_comp(tga_colormap_bpp, 0, ખાલી);
    } નહીંતર {
        tga_comp = stbi__tga_get_comp(tga_bits_per_pixel, (tga_image_type == 3) || (tga_image_type == 11), ખાલી);
    }
    જો(!tga_comp) {
      stbi__rewind(s);
      પાછા 0;
    }
    જો (x) *x = tga_w;
    જો (y) *y = tga_h;
    જો (comp) *comp = tga_comp;
    પાછા 1;                   // seems to have passed everything
}

સ્થિર પૂર્ણાંક stbi__tga_test(stbi__context *s)
{
   પૂર્ણાંક res = 0;
   પૂર્ણાંક sz, tga_color_type;
   stbi__get8(s);      //   discard Offset
   tga_color_type = stbi__get8(s);   //   color type
   જો ( tga_color_type > 1 ) જાઓ errorEnd;   //   only RGB or indexed allowed
   sz = stbi__get8(s);   //   image type
   જો ( tga_color_type == 1 ) { // colormapped (paletted) image
      જો (sz != 1 && sz != 9) જાઓ errorEnd; // colortype 1 demands image type 1 or 9
      stbi__skip(s,4);       // skip index of first colormap entry and number of entries
      sz = stbi__get8(s);    //   check bits per palette color entry
      જો ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) જાઓ errorEnd;
      stbi__skip(s,4);       // skip image x and y origin
   } નહીંતર { // "normal" image w/o colormap
      જો ( (sz != 2) && (sz != 3) && (sz != 10) && (sz != 11) ) જાઓ errorEnd; // only RGB or grey allowed, +/- RLE
      stbi__skip(s,9); // skip colormap specification and image x/y origin
   }
   જો ( stbi__get16le(s) < 1 ) જાઓ errorEnd;      //   test width
   જો ( stbi__get16le(s) < 1 ) જાઓ errorEnd;      //   test height
   sz = stbi__get8(s);   //   bits per pixel
   જો ( (tga_color_type == 1) && (sz != 8) && (sz != 16) ) જાઓ errorEnd; // for colormapped images, bpp is size of an index
   જો ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) જાઓ errorEnd;

   res = 1; // if we got this far, everything's good and we can return 1 instead of 0

errorEnd:
   stbi__rewind(s);
   પાછા res;
}

// read 16bit value and convert to 24bit RGB
સ્થિર શૂન્ય stbi__tga_read_rgb16(stbi__context *s, stbi_uc* out)
{
   stbi__uint16 px = (stbi__uint16)stbi__get16le(s);
   stbi__uint16 fiveBitMask = 31;
   // we have 3 channels with 5bits each
   પૂર્ણાંક r = (px >> 10) & fiveBitMask;
   પૂર્ણાંક g = (px >> 5) & fiveBitMask;
   પૂર્ણાંક b = px & fiveBitMask;
   // Note that this saves the data in RGB(A) order, so it doesn't need to be swapped later
   out[0] = (stbi_uc)((r * 255)/31);
   out[1] = (stbi_uc)((g * 255)/31);
   out[2] = (stbi_uc)((b * 255)/31);

   // some people claim that the most significant bit might be used for alpha
   // (possibly if an alpha-bit is set in the "image descriptor byte")
   // but that only made 16bit test images completely translucent..
   // so let's treat all 15 and 16bit TGAs as RGB with no alpha.
}

સ્થિર શૂન્ય *stbi__tga_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   //   read in the TGA header stuff
   પૂર્ણાંક tga_offset = stbi__get8(s);
   પૂર્ણાંક tga_indexed = stbi__get8(s);
   પૂર્ણાંક tga_image_type = stbi__get8(s);
   પૂર્ણાંક tga_is_RLE = 0;
   પૂર્ણાંક tga_palette_start = stbi__get16le(s);
   પૂર્ણાંક tga_palette_len = stbi__get16le(s);
   પૂર્ણાંક tga_palette_bits = stbi__get8(s);
   પૂર્ણાંક tga_x_origin = stbi__get16le(s);
   પૂર્ણાંક tga_y_origin = stbi__get16le(s);
   પૂર્ણાંક tga_width = stbi__get16le(s);
   પૂર્ણાંક tga_height = stbi__get16le(s);
   પૂર્ણાંક tga_bits_per_pixel = stbi__get8(s);
   પૂર્ણાંક tga_comp, tga_rgb16=0;
   પૂર્ણાંક tga_inverted = stbi__get8(s);
   // int tga_alpha_bits = tga_inverted & 15; // the 4 lowest bits - unused (useless?)
   //   image data
   unsigned અક્ષર *tga_data;
   unsigned અક્ષર *tga_palette = ખાલી;
   પૂર્ણાંક i, j;
   unsigned અક્ષર raw_data[4] = {0};
   પૂર્ણાંક RLE_ગણતરી = 0;
   પૂર્ણાંક RLE_repeating = 0;
   પૂર્ણાંક read_next_pixel = 1;
   STBI_NOTUSED(ri);

   //   do a tiny bit of precessing
   જો ( tga_image_type >= 8 )
   {
      tga_image_type -= 8;
      tga_is_RLE = 1;
   }
   tga_inverted = 1 - ((tga_inverted >> 5) & 1);

   //   If I'm paletted, then I'll use the number of bits from the palette
   જો ( tga_indexed ) tga_comp = stbi__tga_get_comp(tga_palette_bits, 0, &tga_rgb16);
   નહીંતર tga_comp = stbi__tga_get_comp(tga_bits_per_pixel, (tga_image_type == 3), &tga_rgb16);

   જો(!tga_comp) // shouldn't really happen, stbi__tga_test() should have ensured basic consistency
      પાછા stbi__errpuc("bad format", "Can't find out TGA pixelformat");

   //   tga info
   *x = tga_width;
   *y = tga_height;
   જો (comp) *comp = tga_comp;

   જો (!stbi__mad3sizes_valid(tga_width, tga_height, tga_comp, 0))
      પાછા stbi__errpuc("too large", "Corrupt TGA");

   tga_data = (unsigned અક્ષર*)stbi__malloc_mad3(tga_width, tga_height, tga_comp, 0);
   જો (!tga_data) પાછા stbi__errpuc("outofmem", "Out of memory");

   // skip to the data's starting position (offset usually = 0)
   stbi__skip(s, tga_offset );

   જો ( !tga_indexed && !tga_is_RLE && !tga_rgb16 ) {
      માટે (i=0; i < tga_height; ++i) {
         પૂર્ણાંક row = tga_inverted ? tga_height -i - 1 : i;
         stbi_uc *tga_row = tga_data + row*tga_width*tga_comp;
         stbi__getn(s, tga_row, tga_width * tga_comp);
      }
   } નહીંતર  {
      //   do I need to load a palette?
      જો ( tga_indexed)
      {
         //   any data to skip? (offset usually = 0)
         stbi__skip(s, tga_palette_start );
         //   load the palette
         tga_palette = (unsigned અક્ષર*)stbi__malloc_mad2(tga_palette_len, tga_comp, 0);
         જો (!tga_palette) {
            STBI_FREE(tga_data);
            પાછા stbi__errpuc("outofmem", "Out of memory");
         }
         જો (tga_rgb16) {
            stbi_uc *pal_entry = tga_palette;
            STBI_ASSERT(tga_comp == STBI_rgb);
            માટે (i=0; i < tga_palette_len; ++i) {
               stbi__tga_read_rgb16(s, pal_entry);
               pal_entry += tga_comp;
            }
         } નહીંતર જો (!stbi__getn(s, tga_palette, tga_palette_len * tga_comp)) {
               STBI_FREE(tga_data);
               STBI_FREE(tga_palette);
               પાછા stbi__errpuc("bad palette", "Corrupt TGA");
         }
      }
      //   load the data
      માટે (i=0; i < tga_width * tga_height; ++i)
      {
         //   if I'm in RLE mode, do I need to get a RLE stbi__pngchunk?
         જો ( tga_is_RLE )
         {
            જો ( RLE_ગણતરી == 0 )
            {
               //   yep, get the next byte as a RLE command
               પૂર્ણાંક RLE_cmd = stbi__get8(s);
               RLE_ગણતરી = 1 + (RLE_cmd & 127);
               RLE_repeating = RLE_cmd >> 7;
               read_next_pixel = 1;
            } નહીંતર જો ( !RLE_repeating )
            {
               read_next_pixel = 1;
            }
         } નહીંતર
         {
            read_next_pixel = 1;
         }
         //   OK, if I need to read a pixel, do it now
         જો ( read_next_pixel )
         {
            //   load however much data we did have
            જો ( tga_indexed )
            {
               // read in index, then perform the lookup
               પૂર્ણાંક pal_idx = (tga_bits_per_pixel == 8) ? stbi__get8(s) : stbi__get16le(s);
               જો ( pal_idx >= tga_palette_len ) {
                  // invalid index
                  pal_idx = 0;
               }
               pal_idx *= tga_comp;
               માટે (j = 0; j < tga_comp; ++j) {
                  raw_data[j] = tga_palette[pal_idx+j];
               }
            } નહીંતર જો(tga_rgb16) {
               STBI_ASSERT(tga_comp == STBI_rgb);
               stbi__tga_read_rgb16(s, raw_data);
            } નહીંતર {
               //   read in the data raw
               માટે (j = 0; j < tga_comp; ++j) {
                  raw_data[j] = stbi__get8(s);
               }
            }
            //   clear the reading flag for the next pixel
            read_next_pixel = 0;
         } // end of reading a pixel

         // copy data
         માટે (j = 0; j < tga_comp; ++j)
           tga_data[i*tga_comp+j] = raw_data[j];

         //   in case we're in RLE mode, keep counting down
         --RLE_ગણતરી;
      }
      //   do I need to invert the image?
      જો ( tga_inverted )
      {
         માટે (j = 0; j*2 < tga_height; ++j)
         {
            પૂર્ણાંક index1 = j * tga_width * tga_comp;
            પૂર્ણાંક index2 = (tga_height - 1 - j) * tga_width * tga_comp;
            માટે (i = tga_width * tga_comp; i > 0; --i)
            {
               unsigned અક્ષર temp = tga_data[index1];
               tga_data[index1] = tga_data[index2];
               tga_data[index2] = temp;
               ++index1;
               ++index2;
            }
         }
      }
      //   clear my palette, if I had one
      જો ( tga_palette != ખાલી )
      {
         STBI_FREE( tga_palette );
      }
   }

   // swap RGB - if the source data was RGB16, it already is in the right order
   જો (tga_comp >= 3 && !tga_rgb16)
   {
      unsigned અક્ષર* tga_pixel = tga_data;
      માટે (i=0; i < tga_width * tga_height; ++i)
      {
         unsigned અક્ષર temp = tga_pixel[0];
         tga_pixel[0] = tga_pixel[2];
         tga_pixel[2] = temp;
         tga_pixel += tga_comp;
      }
   }

   // convert to target component count
   જો (req_comp && req_comp != tga_comp)
      tga_data = stbi__convert_format(tga_data, tga_comp, req_comp, tga_width, tga_height);

   //   the things I do to get rid of an error message, and yet keep
   //   Microsoft's C compilers happy... [8^(
   tga_palette_start = tga_palette_len = tga_palette_bits =
         tga_x_origin = tga_y_origin = 0;
   //   OK, done
   પાછા tga_data;
}
#endif

// *************************************************************************************************
// Photoshop PSD loader -- PD by Thatcher Ulrich, integration by Nicolas Schulz, tweaked by STB

#ifndef STBI_NO_PSD
સ્થિર પૂર્ણાંક stbi__psd_test(stbi__context *s)
{
   પૂર્ણાંક r = (stbi__get32be(s) == 0x38425053);
   stbi__rewind(s);
   પાછા r;
}

સ્થિર પૂર્ણાંક stbi__psd_decode_rle(stbi__context *s, stbi_uc *p, પૂર્ણાંક pixel_ગણતરી_2)
{
   પૂર્ણાંક ગણતરી, nleft, len;

   ગણતરી = 0;
   જ્યારે ((nleft = pixel_ગણતરી_2 - ગણતરી) > 0) {
      len = stbi__get8(s);
      જો (len == 128) {
         // No-op.
      } નહીંતર જો (len < 128) {
         // Copy next len+1 bytes literally.
         len++;
         જો (len > nleft) પાછા 0; // corrupt data
         ગણતરી += len;
         જ્યારે (len) {
            *p = stbi__get8(s);
            p += 4;
            len--;
         }
      } નહીંતર જો (len > 128) {
         stbi_uc   val;
         // Next -len+1 bytes in the dest are replicated from next source byte.
         // (Interpret len as a negative 8-bit int.)
         len = 257 - len;
         જો (len > nleft) પાછા 0; // corrupt data
         val = stbi__get8(s);
         ગણતરી += len;
         જ્યારે (len) {
            *p = val;
            p += 4;
            len--;
         }
      }
   }

   પાછા 1;
}

સ્થિર શૂન્ય *stbi__psd_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri, પૂર્ણાંક bpc)
{
   પૂર્ણાંક pixel_ગણતરી_2;
   પૂર્ણાંક channel_ગણતરી, compression;
   પૂર્ણાંક channel, i;
   પૂર્ણાંક bitdepth;
   પૂર્ણાંક w,h;
   stbi_uc *out;
   STBI_NOTUSED(ri);

   // Check identifier
   જો (stbi__get32be(s) != 0x38425053)   // "8BPS"
      પાછા stbi__errpuc("not PSD", "Corrupt PSD image");

   // Check file type version.
   જો (stbi__get16be(s) != 1)
      પાછા stbi__errpuc("wrong version", "Unsupported version of PSD image");

   // Skip 6 reserved bytes.
   stbi__skip(s, 6 );

   // Read the number of channels (R, G, B, A, etc).
   channel_ગણતરી = stbi__get16be(s);
   જો (channel_ગણતરી < 0 || channel_ગણતરી > 16)
      પાછા stbi__errpuc("wrong channel count", "Unsupported number of channels in PSD image");

   // Read the rows and columns of the image.
   h = stbi__get32be(s);
   w = stbi__get32be(s);

   // Make sure the depth is 8 bits.
   bitdepth = stbi__get16be(s);
   જો (bitdepth != 8 && bitdepth != 16)
      પાછા stbi__errpuc("unsupported bit depth", "PSD bit depth is not 8 or 16 bit");

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
   જો (stbi__get16be(s) != 3)
      પાછા stbi__errpuc("wrong color format", "PSD is not in RGB color format");

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
   જો (compression > 1)
      પાછા stbi__errpuc("bad compression", "PSD has an unknown compression format");

   // Check size
   જો (!stbi__mad3sizes_valid(4, w, h, 0))
      પાછા stbi__errpuc("too large", "Corrupt PSD");

   // Create the destination image.

   જો (!compression && bitdepth == 16 && bpc == 16) {
      out = (stbi_uc *) stbi__malloc_mad3(8, w, h, 0);
      ri->bits_per_channel = 16;
   } નહીંતર
      out = (stbi_uc *) stbi__malloc(4 * w*h);

   જો (!out) પાછા stbi__errpuc("outofmem", "Out of memory");
   pixel_ગણતરી_2 = w*h;

   // Initialize the data to zero.
   //memset( out, 0, pixelCount * 4 );

   // Finally, the image data.
   જો (compression) {
      // RLE as used by .PSD and .TIFF
      // Loop until you get the number of unpacked bytes you are expecting:
      //     Read the next source byte into n.
      //     If n is between 0 and 127 inclusive, copy the next n+1 bytes literally.
      //     Else if n is between -127 and -1 inclusive, copy the next byte -n+1 times.
      //     Else if n is 128, noop.
      // Endloop

      // The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
      // which we're going to just skip.
      stbi__skip(s, h * channel_ગણતરી * 2 );

      // Read the RLE data by channel.
      માટે (channel = 0; channel < 4; channel++) {
         stbi_uc *p;

         p = out+channel;
         જો (channel >= channel_ગણતરી) {
            // Fill this channel with default data.
            માટે (i = 0; i < pixel_ગણતરી_2; i++, p += 4)
               *p = (channel == 3 ? 255 : 0);
         } નહીંતર {
            // Read the RLE data.
            જો (!stbi__psd_decode_rle(s, p, pixel_ગણતરી_2)) {
               STBI_FREE(out);
               પાછા stbi__errpuc("corrupt", "bad RLE data");
            }
         }
      }

   } નહીંતર {
      // We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
      // where each channel consists of an 8-bit (or 16-bit) value for each pixel in the image.

      // Read the data by channel.
      માટે (channel = 0; channel < 4; channel++) {
         જો (channel >= channel_ગણતરી) {
            // Fill this channel with default data.
            જો (bitdepth == 16 && bpc == 16) {
               stbi__uint16 *q = ((stbi__uint16 *) out) + channel;
               stbi__uint16 val = channel == 3 ? 65535 : 0;
               માટે (i = 0; i < pixel_ગણતરી_2; i++, q += 4)
                  *q = val;
            } નહીંતર {
               stbi_uc *p = out+channel;
               stbi_uc val = channel == 3 ? 255 : 0;
               માટે (i = 0; i < pixel_ગણતરી_2; i++, p += 4)
                  *p = val;
            }
         } નહીંતર {
            જો (ri->bits_per_channel == 16) {    // output bpc
               stbi__uint16 *q = ((stbi__uint16 *) out) + channel;
               માટે (i = 0; i < pixel_ગણતરી_2; i++, q += 4)
                  *q = (stbi__uint16) stbi__get16be(s);
            } નહીંતર {
               stbi_uc *p = out+channel;
               જો (bitdepth == 16) {  // input bpc
                  માટે (i = 0; i < pixel_ગણતરી_2; i++, p += 4)
                     *p = (stbi_uc) (stbi__get16be(s) >> 8);
               } નહીંતર {
                  માટે (i = 0; i < pixel_ગણતરી_2; i++, p += 4)
                     *p = stbi__get8(s);
               }
            }
         }
      }
   }

   // remove weird white matte from PSD
   જો (channel_ગણતરી >= 4) {
      જો (ri->bits_per_channel == 16) {
         માટે (i=0; i < w*h; ++i) {
            stbi__uint16 *pixel = (stbi__uint16 *) out + 4*i;
            જો (pixel[3] != 0 && pixel[3] != 65535) {
               દશાંશ a = pixel[3] / 65535.0f;
               દશાંશ ra = 1.0f / a;
               દશાંશ inv_a = 65535.0f * (1 - ra);
               pixel[0] = (stbi__uint16) (pixel[0]*ra + inv_a);
               pixel[1] = (stbi__uint16) (pixel[1]*ra + inv_a);
               pixel[2] = (stbi__uint16) (pixel[2]*ra + inv_a);
            }
         }
      } નહીંતર {
         માટે (i=0; i < w*h; ++i) {
            unsigned અક્ષર *pixel = out + 4*i;
            જો (pixel[3] != 0 && pixel[3] != 255) {
               દશાંશ a = pixel[3] / 255.0f;
               દશાંશ ra = 1.0f / a;
               દશાંશ inv_a = 255.0f * (1 - ra);
               pixel[0] = (unsigned અક્ષર) (pixel[0]*ra + inv_a);
               pixel[1] = (unsigned અક્ષર) (pixel[1]*ra + inv_a);
               pixel[2] = (unsigned અક્ષર) (pixel[2]*ra + inv_a);
            }
         }
      }
   }

   // convert to desired output format
   જો (req_comp && req_comp != 4) {
      જો (ri->bits_per_channel == 16)
         out = (stbi_uc *) stbi__convert_format16((stbi__uint16 *) out, 4, req_comp, w, h);
      નહીંતર
         out = stbi__convert_format(out, 4, req_comp, w, h);
      જો (out == ખાલી) પાછા out; // stbi__convert_format frees input on failure
   }

   જો (comp) *comp = 4;
   *y = h;
   *x = w;

   પાછા out;
}
#endif

// *************************************************************************************************
// Softimage PIC loader
// by Tom Seddon
//
// See http://softimage.wiki.softimage.com/index.php/INFO:_PIC_file_format
// See http://ozviz.wasp.uwa.edu.au/~pbourke/dataformats/softimagepic/

#ifndef STBI_NO_PIC
સ્થિર પૂર્ણાંક stbi__pic_is4(stbi__context *s,અચલ અક્ષર *str)
{
   પૂર્ણાંક i;
   માટે (i=0; i<4; ++i)
      જો (stbi__get8(s) != (stbi_uc)str[i])
         પાછા 0;

   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__pic_test_core(stbi__context *s)
{
   પૂર્ણાંક i;

   જો (!stbi__pic_is4(s,"\x53\x80\xF6\x34"))
      પાછા 0;

   માટે(i=0;i<84;++i)
      stbi__get8(s);

   જો (!stbi__pic_is4(s,"PICT"))
      પાછા 0;

   પાછા 1;
}

પ્રકારવ્યાખ્યા સંરચના
{
   stbi_uc size,type,channel;
} stbi__pic_packet;

સ્થિર stbi_uc *stbi__readval(stbi__context *s, પૂર્ણાંક channel, stbi_uc *dest)
{
   પૂર્ણાંક mask=0x80, i;

   માટે (i=0; i<4; ++i, mask>>=1) {
      જો (channel & mask) {
         જો (stbi__at_eof(s)) પાછા stbi__errpuc("bad file","PIC file too short");
         dest[i]=stbi__get8(s);
      }
   }

   પાછા dest;
}

સ્થિર શૂન્ય stbi__copyval(પૂર્ણાંક channel,stbi_uc *dest,અચલ stbi_uc *src)
{
   પૂર્ણાંક mask=0x80,i;

   માટે (i=0;i<4; ++i, mask>>=1)
      જો (channel&mask)
         dest[i]=src[i];
}

સ્થિર stbi_uc *stbi__pic_load_core(stbi__context *s,પૂર્ણાંક width,પૂર્ણાંક height,પૂર્ણાંક *comp, stbi_uc *result)
{
   પૂર્ણાંક act_comp=0,અંક_packets=0,y,chained;
   stbi__pic_packet packets[10];

   // this will (should...) cater for even some bizarre stuff like having data
    // for the same channel in multiple packets.
   કરો {
      stbi__pic_packet *packet;

      જો (અંક_packets==કદ(packets)/કદ(packets[0]))
         પાછા stbi__errpuc("bad format","too many packets");

      packet = &packets[અંક_packets++];

      chained = stbi__get8(s);
      packet->size    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);

      act_comp |= packet->channel;

      જો (stbi__at_eof(s))          પાછા stbi__errpuc("bad file","file too short (reading packets)");
      જો (packet->size != 8)  પાછા stbi__errpuc("bad format","packet isn't 8bpp");
   } જ્યારે (chained);

   *comp = (act_comp & 0x10 ? 4 : 3); // has alpha channel?

   માટે(y=0; y<height; ++y) {
      પૂર્ણાંક packet_idx;

      માટે(packet_idx=0; packet_idx < અંક_packets; ++packet_idx) {
         stbi__pic_packet *packet = &packets[packet_idx];
         stbi_uc *dest = result+y*width*4;

         પસંદગી (packet->type) {
            મૂળભૂત:
               પાછા stbi__errpuc("bad format","packet has bad compression type");

            સ્થિતિ 0: {//uncompressed
               પૂર્ણાંક x;

               માટે(x=0;x<width;++x, dest+=4)
                  જો (!stbi__readval(s,packet->channel,dest))
                     પાછા 0;
               તોડો;
            }

            સ્થિતિ 1://Pure RLE
               {
                  પૂર્ણાંક ડાબું=width, i;

                  જ્યારે (ડાબું>0) {
                     stbi_uc ગણતરી,કિંમત[4];

                     ગણતરી=stbi__get8(s);
                     જો (stbi__at_eof(s))   પાછા stbi__errpuc("bad file","file too short (pure read count)");

                     જો (ગણતરી > ડાબું)
                        ગણતરી = (stbi_uc) ડાબું;

                     જો (!stbi__readval(s,packet->channel,કિંમત))  પાછા 0;

                     માટે(i=0; i<ગણતરી; ++i,dest+=4)
                        stbi__copyval(packet->channel,dest,કિંમત);
                     ડાબું -= ગણતરી;
                  }
               }
               તોડો;

            સ્થિતિ 2: {//Mixed RLE
               પૂર્ણાંક ડાબું=width;
               જ્યારે (ડાબું>0) {
                  પૂર્ણાંક ગણતરી = stbi__get8(s), i;
                  જો (stbi__at_eof(s))  પાછા stbi__errpuc("bad file","file too short (mixed read count)");

                  જો (ગણતરી >= 128) { // Repeated
                     stbi_uc કિંમત[4];

                     જો (ગણતરી==128)
                        ગણતરી = stbi__get16be(s);
                     નહીંતર
                        ગણતરી -= 127;
                     જો (ગણતરી > ડાબું)
                        પાછા stbi__errpuc("bad file","scanline overrun");

                     જો (!stbi__readval(s,packet->channel,કિંમત))
                        પાછા 0;

                     માટે(i=0;i<ગણતરી;++i, dest += 4)
                        stbi__copyval(packet->channel,dest,કિંમત);
                  } નહીંતર { // Raw
                     ++ગણતરી;
                     જો (ગણતરી>ડાબું) પાછા stbi__errpuc("bad file","scanline overrun");

                     માટે(i=0;i<ગણતરી;++i, dest+=4)
                        જો (!stbi__readval(s,packet->channel,dest))
                           પાછા 0;
                  }
                  ડાબું-=ગણતરી;
               }
               તોડો;
            }
         }
      }
   }

   પાછા result;
}

સ્થિર શૂન્ય *stbi__pic_load(stbi__context *s,પૂર્ણાંક *px,પૂર્ણાંક *py,પૂર્ણાંક *comp,પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   stbi_uc *result;
   પૂર્ણાંક i, x,y, internal_comp;
   STBI_NOTUSED(ri);

   જો (!comp) comp = &internal_comp;

   માટે (i=0; i<92; ++i)
      stbi__get8(s);

   x = stbi__get16be(s);
   y = stbi__get16be(s);
   જો (stbi__at_eof(s))  પાછા stbi__errpuc("bad file","file too short (pic header)");
   જો (!stbi__mad3sizes_valid(x, y, 4, 0)) પાછા stbi__errpuc("too large", "PIC image too large to decode");

   stbi__get32be(s); //skip `ratio'
   stbi__get16be(s); //skip `fields'
   stbi__get16be(s); //skip `pad'

   // intermediate buffer is RGBA
   result = (stbi_uc *) stbi__malloc_mad3(x, y, 4, 0);
   memset(result, 0xff, x*y*4);

   જો (!stbi__pic_load_core(s,x,y,comp, result)) {
      STBI_FREE(result);
      result=0;
   }
   *px = x;
   *py = y;
   જો (req_comp == 0) req_comp = *comp;
   result=stbi__convert_format(result,4,req_comp,x,y);

   પાછા result;
}

સ્થિર પૂર્ણાંક stbi__pic_test(stbi__context *s)
{
   પૂર્ણાંક r = stbi__pic_test_core(s);
   stbi__rewind(s);
   પાછા r;
}
#endif

// *************************************************************************************************
// GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb

#ifndef STBI_NO_GIF
પ્રકારવ્યાખ્યા સંરચના
{
   stbi__int16 prefix;
   stbi_uc first;
   stbi_uc suffix;
} stbi__gif_lzw;

પ્રકારવ્યાખ્યા સંરચના
{
   પૂર્ણાંક w,h;
   stbi_uc *out;                 // output buffer (always 4 components)
   stbi_uc *background;          // The current "background" as far as a gif is concerned
   stbi_uc *history; 
   પૂર્ણાંક flags, bgindex, ratio, transparent, eflags;
   stbi_uc  pal[256][4];
   stbi_uc lpal[256][4];
   stbi__gif_lzw codes[8192];
   stbi_uc *color_table;
   પૂર્ણાંક parse, step;
   પૂર્ણાંક lflags;
   પૂર્ણાંક start_x, start_y;
   પૂર્ણાંક max_x, max_y;
   પૂર્ણાંક cur_x, cur_y;
   પૂર્ણાંક લીટી_size;
   પૂર્ણાંક delay;
} stbi__gif;

સ્થિર પૂર્ણાંક stbi__gif_test_raw(stbi__context *s)
{
   પૂર્ણાંક sz;
   જો (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8') પાછા 0;
   sz = stbi__get8(s);
   જો (sz != '9' && sz != '7') પાછા 0;
   જો (stbi__get8(s) != 'a') પાછા 0;
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__gif_test(stbi__context *s)
{
   પૂર્ણાંક r = stbi__gif_test_raw(s);
   stbi__rewind(s);
   પાછા r;
}

સ્થિર શૂન્ય stbi__gif_parse_colortable(stbi__context *s, stbi_uc pal[256][4], પૂર્ણાંક અંક_entries, પૂર્ણાંક transp)
{
   પૂર્ણાંક i;
   માટે (i=0; i < અંક_entries; ++i) {
      pal[i][2] = stbi__get8(s);
      pal[i][1] = stbi__get8(s);
      pal[i][0] = stbi__get8(s);
      pal[i][3] = transp == i ? 0 : 255;
   }
}

સ્થિર પૂર્ણાંક stbi__gif_header(stbi__context *s, stbi__gif *g, પૂર્ણાંક *comp, પૂર્ણાંક is_info)
{
   stbi_uc version;
   જો (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8')
      પાછા stbi__err("not GIF", "Corrupt GIF");

   version = stbi__get8(s);
   જો (version != '7' && version != '9')    પાછા stbi__err("not GIF", "Corrupt GIF");
   જો (stbi__get8(s) != 'a')                પાછા stbi__err("not GIF", "Corrupt GIF");

   stbi__g_failure_reason = "";
   g->w = stbi__get16le(s);
   g->h = stbi__get16le(s);
   g->flags = stbi__get8(s);
   g->bgindex = stbi__get8(s);
   g->ratio = stbi__get8(s);
   g->transparent = -1;

   જો (comp != 0) *comp = 4;  // can't actually tell whether it's 3 or 4 until we parse the comments

   જો (is_info) પાછા 1;

   જો (g->flags & 0x80)
      stbi__gif_parse_colortable(s,g->pal, 2 << (g->flags & 7), -1);

   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__gif_info_raw(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   stbi__gif* g = (stbi__gif*) stbi__malloc(કદ(stbi__gif));
   જો (!stbi__gif_header(s, g, comp, 1)) {
      STBI_FREE(g);
      stbi__rewind( s );
      પાછા 0;
   }
   જો (x) *x = g->w;
   જો (y) *y = g->h;
   STBI_FREE(g);
   પાછા 1;
}

સ્થિર શૂન્ય stbi__out_gif_code(stbi__gif *g, stbi__uint16 code)
{
   stbi_uc *p, *c;
   પૂર્ણાંક idx; 

   // recurse to decode the prefixes, since the linked-list is backwards,
   // and working backwards through an interleaved image would be nasty
   જો (g->codes[code].prefix >= 0)
      stbi__out_gif_code(g, g->codes[code].prefix);

   જો (g->cur_y >= g->max_y) પાછા;

   idx = g->cur_x + g->cur_y; 
   p = &g->out[idx];
   g->history[idx / 4] = 1;  

   c = &g->color_table[g->codes[code].suffix * 4];
   જો (c[3] > 128) { // don't render transparent pixels; 
      p[0] = c[2];
      p[1] = c[1];
      p[2] = c[0];
      p[3] = c[3];
   }
   g->cur_x += 4;

   જો (g->cur_x >= g->max_x) {
      g->cur_x = g->start_x;
      g->cur_y += g->step;

      જ્યારે (g->cur_y >= g->max_y && g->parse > 0) {
         g->step = (1 << g->parse) * g->લીટી_size;
         g->cur_y = g->start_y + (g->step >> 1);
         --g->parse;
      }
   }
}

સ્થિર stbi_uc *stbi__process_gif_raster(stbi__context *s, stbi__gif *g)
{
   stbi_uc lzw_cs;
   stbi__int32 len, init_code;
   stbi__uint32 first;
   stbi__int32 codesize, codemask, avail, oldcode, bits, valid_bits, clear;
   stbi__gif_lzw *p;

   lzw_cs = stbi__get8(s);
   જો (lzw_cs > 12) પાછા ખાલી;
   clear = 1 << lzw_cs;
   first = 1;
   codesize = lzw_cs + 1;
   codemask = (1 << codesize) - 1;
   bits = 0;
   valid_bits = 0;
   માટે (init_code = 0; init_code < clear; init_code++) {
      g->codes[init_code].prefix = -1;
      g->codes[init_code].first = (stbi_uc) init_code;
      g->codes[init_code].suffix = (stbi_uc) init_code;
   }

   // support no starting clear code
   avail = clear+2;
   oldcode = -1;

   len = 0;
   માટે(;;) {
      જો (valid_bits < codesize) {
         જો (len == 0) {
            len = stbi__get8(s); // start new block
            જો (len == 0)
               પાછા g->out;
         }
         --len;
         bits |= (stbi__int32) stbi__get8(s) << valid_bits;
         valid_bits += 8;
      } નહીંતર {
         stbi__int32 code = bits & codemask;
         bits >>= codesize;
         valid_bits -= codesize;
         // @OPTIMIZE: is there some way we can accelerate the non-clear path?
         જો (code == clear) {  // clear code
            codesize = lzw_cs + 1;
            codemask = (1 << codesize) - 1;
            avail = clear + 2;
            oldcode = -1;
            first = 0;
         } નહીંતર જો (code == clear + 1) { // end of stream code
            stbi__skip(s, len);
            જ્યારે ((len = stbi__get8(s)) > 0)
               stbi__skip(s,len);
            પાછા g->out;
         } નહીંતર જો (code <= avail) {
            જો (first) {
               પાછા stbi__errpuc("no clear code", "Corrupt GIF");
            }

            જો (oldcode >= 0) {
               p = &g->codes[avail++];
               જો (avail > 8192) {
                  પાછા stbi__errpuc("too many codes", "Corrupt GIF");
               }

               p->prefix = (stbi__int16) oldcode;
               p->first = g->codes[oldcode].first;
               p->suffix = (code == avail) ? p->first : g->codes[code].first;
            } નહીંતર જો (code == avail)
               પાછા stbi__errpuc("illegal code in raster", "Corrupt GIF");

            stbi__out_gif_code(g, (stbi__uint16) code);

            જો ((avail & codemask) == 0 && avail <= 0x0FFF) {
               codesize++;
               codemask = (1 << codesize) - 1;
            }

            oldcode = code;
         } નહીંતર {
            પાછા stbi__errpuc("illegal code in raster", "Corrupt GIF");
         }
      }
   }
}

// this function is designed to support animated gifs, although stb_image doesn't support it
// two back is the image from two frames ago, used for a very specific disposal format
સ્થિર stbi_uc *stbi__gif_load_next(stbi__context *s, stbi__gif *g, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi_uc *two_back)
{
   પૂર્ણાંક dispose; 
   પૂર્ણાંક first_frame; 
   પૂર્ણાંક pi; 
   પૂર્ણાંક pcount; 

   // on first frame, any non-written pixels get the background colour (non-transparent)
   first_frame = 0; 
   જો (g->out == 0) {
      જો (!stbi__gif_header(s, g, comp,0))     પાછા 0; // stbi__g_failure_reason set by stbi__gif_header
      g->out = (stbi_uc *) stbi__malloc(4 * g->w * g->h);
      g->background = (stbi_uc *) stbi__malloc(4 * g->w * g->h); 
      g->history = (stbi_uc *) stbi__malloc(g->w * g->h); 
      જો (g->out == 0)                      પાછા stbi__errpuc("outofmem", "Out of memory");

      // image is treated as "tranparent" at the start - ie, nothing overwrites the current background; 
      // background colour is only used for pixels that are not rendered first frame, after that "background"
      // color refers to teh color that was there the previous frame. 
      memset( g->out, 0x00, 4 * g->w * g->h ); 
      memset( g->background, 0x00, 4 * g->w * g->h ); // state of the background (starts transparent)
      memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame
      first_frame = 1; 
   } નહીંતર {
      // second frame - how do we dispoase of the previous one?
      dispose = (g->eflags & 0x1C) >> 2; 
      pcount = g->w * g->h; 

      જો ((dispose == 3) && (two_back == 0)) {
         dispose = 2; // if I don't have an image to revert back to, default to the old background
      }

      જો (dispose == 3) { // use previous graphic
         માટે (pi = 0; pi < pcount; ++pi) {
            જો (g->history[pi]) {
               memcpy( &g->out[pi * 4], &two_back[pi * 4], 4 ); 
            }
         }
      } નહીંતર જો (dispose == 2) { 
         // restore what was changed last frame to background before that frame; 
         માટે (pi = 0; pi < pcount; ++pi) {
            જો (g->history[pi]) {
               memcpy( &g->out[pi * 4], &g->background[pi * 4], 4 ); 
            }
         }
      } નહીંતર {
         // This is a non-disposal case eithe way, so just 
         // leave the pixels as is, and they will become the new background
         // 1: do not dispose
         // 0:  not specified.
      }

      // background is what out is after the undoing of the previou frame; 
      memcpy( g->background, g->out, 4 * g->w * g->h ); 
   }

   // clear my history; 
   memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame

   માટે (;;) {
      પૂર્ણાંક tag = stbi__get8(s); 
      પસંદગી (tag) {
         સ્થિતિ 0x2C: /* Image Descriptor */
         {
            stbi__int32 x, y, w, h;
            stbi_uc *o;

            x = stbi__get16le(s);
            y = stbi__get16le(s);
            w = stbi__get16le(s);
            h = stbi__get16le(s);
            જો (((x + w) > (g->w)) || ((y + h) > (g->h)))
               પાછા stbi__errpuc("bad Image Descriptor", "Corrupt GIF");

            g->લીટી_size = g->w * 4;
            g->start_x = x * 4;
            g->start_y = y * g->લીટી_size;
            g->max_x   = g->start_x + w * 4;
            g->max_y   = g->start_y + h * g->લીટી_size;
            g->cur_x   = g->start_x;
            g->cur_y   = g->start_y;

            g->lflags = stbi__get8(s);

            જો (g->lflags & 0x40) {
               g->step = 8 * g->લીટી_size; // first interlaced spacing
               g->parse = 3;
            } નહીંતર {
               g->step = g->લીટી_size;
               g->parse = 0;
            }

            જો (g->lflags & 0x80) {
               stbi__gif_parse_colortable(s,g->lpal, 2 << (g->lflags & 7), g->eflags & 0x01 ? g->transparent : -1);
               g->color_table = (stbi_uc *) g->lpal;
            } નહીંતર જો (g->flags & 0x80) {
               g->color_table = (stbi_uc *) g->pal;
            } નહીંતર
               પાછા stbi__errpuc("missing color table", "Corrupt GIF");            
            
            o = stbi__process_gif_raster(s, g);
            જો (o == ખાલી) પાછા ખાલી;

            // if this was the first frame, 
            pcount = g->w * g->h; 
            જો (first_frame && (g->bgindex > 0)) {
               // if first frame, any pixel not drawn to gets the background color
               માટે (pi = 0; pi < pcount; ++pi) {
                  જો (g->history[pi] == 0) {
                     g->pal[g->bgindex][3] = 255; // just in case it was made transparent, undo that; It will be reset next frame if need be; 
                     memcpy( &g->out[pi * 4], &g->pal[g->bgindex], 4 ); 
                  }
               }
            }

            પાછા o;
         }

         સ્થિતિ 0x21: // Comment Extension.
         {
            પૂર્ણાંક len;
            પૂર્ણાંક ext = stbi__get8(s); 
            જો (ext == 0xF9) { // Graphic Control Extension.
               len = stbi__get8(s);
               જો (len == 4) {
                  g->eflags = stbi__get8(s);
                  g->delay = 10 * stbi__get16le(s); // delay - 1/100th of a second, saving as 1/1000ths.

                  // unset old transparent
                  જો (g->transparent >= 0) {
                     g->pal[g->transparent][3] = 255; 
                  } 
                  જો (g->eflags & 0x01) {
                     g->transparent = stbi__get8(s);
                     જો (g->transparent >= 0) {
                        g->pal[g->transparent][3] = 0; 
                     }
                  } નહીંતર {
                     // don't need transparent
                     stbi__skip(s, 1); 
                     g->transparent = -1; 
                  }
               } નહીંતર {
                  stbi__skip(s, len);
                  તોડો;
               }
            } 
            જ્યારે ((len = stbi__get8(s)) != 0) {
               stbi__skip(s, len);
            }
            તોડો;
         }

         સ્થિતિ 0x3B: // gif stream termination code
            પાછા (stbi_uc *) s; // using '1' causes warning on some compilers

         મૂળભૂત:
            પાછા stbi__errpuc("unknown code", "Corrupt GIF");
      }
   }
}

સ્થિર શૂન્ય *stbi__load_gif_main(stbi__context *s, પૂર્ણાંક **delays, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *z, પૂર્ણાંક *comp, પૂર્ણાંક req_comp)
{
   જો (stbi__gif_test(s)) {
      પૂર્ણાંક layers = 0; 
      stbi_uc *u = 0;
      stbi_uc *out = 0;
      stbi_uc *two_back = 0; 
      stbi__gif g;
      પૂર્ણાંક stride; 
      memset(&g, 0, કદ(g));
      જો (delays) {
         *delays = 0; 
      }

      કરો {
         u = stbi__gif_load_next(s, &g, comp, req_comp, two_back);
         જો (u == (stbi_uc *) s) u = 0;  // end of animated gif marker

         જો (u) {
            *x = g.w;
            *y = g.h;
            ++layers; 
            stride = g.w * g.h * 4; 
         
            જો (out) {
               out = (stbi_uc*) STBI_REALLOC( out, layers * stride ); 
               જો (delays) {
                  *delays = (પૂર્ણાંક*) STBI_REALLOC( *delays, કદ(પૂર્ણાંક) * layers ); 
               }
            } નહીંતર {
               out = (stbi_uc*)stbi__malloc( layers * stride ); 
               જો (delays) {
                  *delays = (પૂર્ણાંક*) stbi__malloc( layers * કદ(પૂર્ણાંક) ); 
               }
            }
            memcpy( out + ((layers - 1) * stride), u, stride ); 
            જો (layers >= 2) {
               two_back = out - 2 * stride; 
            }

            જો (delays) {
               (*delays)[layers - 1U] = g.delay; 
            }
         }
      } જ્યારે (u != 0); 

      // free temp buffer; 
      STBI_FREE(g.out); 
      STBI_FREE(g.history); 
      STBI_FREE(g.background); 

      // do the final conversion after loading everything; 
      જો (req_comp && req_comp != 4)
         out = stbi__convert_format(out, 4, req_comp, layers * g.w, g.h);

      *z = layers; 
      પાછા out;
   } નહીંતર {
      પાછા stbi__errpuc("not GIF", "Image was not as a gif type."); 
   }
}

સ્થિર શૂન્ય *stbi__gif_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   stbi_uc *u = 0;
   stbi__gif g;
   memset(&g, 0, કદ(g));

   u = stbi__gif_load_next(s, &g, comp, req_comp, 0);
   જો (u == (stbi_uc *) s) u = 0;  // end of animated gif marker
   જો (u) {
      *x = g.w;
      *y = g.h;

      // moved conversion to after successful load so that the same
      // can be done for multiple frames. 
      જો (req_comp && req_comp != 4)
         u = stbi__convert_format(u, 4, req_comp, g.w, g.h);
   }

   // free buffers needed for multiple frame loading; 
   STBI_FREE(g.history);
   STBI_FREE(g.background); 

   પાછા u;
}

સ્થિર પૂર્ણાંક stbi__gif_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   પાછા stbi__gif_info_raw(s,x,y,comp);
}
#endif

// *************************************************************************************************
// Radiance RGBE HDR loader
// originally by Nicolas Schulz
#ifndef STBI_NO_HDR
સ્થિર પૂર્ણાંક stbi__hdr_test_core(stbi__context *s, અચલ અક્ષર *signature)
{
   પૂર્ણાંક i;
   માટે (i=0; signature[i]; ++i)
      જો (stbi__get8(s) != signature[i])
          પાછા 0;
   stbi__rewind(s);
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__hdr_test(stbi__context* s)
{
   પૂર્ણાંક r = stbi__hdr_test_core(s, "#?RADIANCE\n");
   stbi__rewind(s);
   જો(!r) {
       r = stbi__hdr_test_core(s, "#?RGBE\n");
       stbi__rewind(s);
   }
   પાછા r;
}

#define STBI__HDR_BUFLEN  1024
સ્થિર અક્ષર *stbi__hdr_gettoken(stbi__context *z, અક્ષર *buffer)
{
   પૂર્ણાંક len=0;
   અક્ષર c = '\0';

   c = (અક્ષર) stbi__get8(z);

   જ્યારે (!stbi__at_eof(z) && c != '\n') {
      buffer[len++] = c;
      જો (len == STBI__HDR_BUFLEN-1) {
         // flush to end of line
         જ્યારે (!stbi__at_eof(z) && stbi__get8(z) != '\n')
            ;
         તોડો;
      }
      c = (અક્ષર) stbi__get8(z);
   }

   buffer[len] = 0;
   પાછા buffer;
}

સ્થિર શૂન્ય stbi__hdr_convert(દશાંશ *output, stbi_uc *આગત, પૂર્ણાંક req_comp)
{
   જો ( આગત[3] != 0 ) {
      દશાંશ f1;
      // Exponent
      f1 = (દશાંશ) ldexp(1.0f, આગત[3] - (પૂર્ણાંક)(128 + 8));
      જો (req_comp <= 2)
         output[0] = (આગત[0] + આગત[1] + આગત[2]) * f1 / 3;
      નહીંતર {
         output[0] = આગત[0] * f1;
         output[1] = આગત[1] * f1;
         output[2] = આગત[2] * f1;
      }
      જો (req_comp == 2) output[1] = 1;
      જો (req_comp == 4) output[3] = 1;
   } નહીંતર {
      પસંદગી (req_comp) {
         સ્થિતિ 4: output[3] = 1; /* fallthrough */
         સ્થિતિ 3: output[0] = output[1] = output[2] = 0;
                 તોડો;
         સ્થિતિ 2: output[1] = 1; /* fallthrough */
         સ્થિતિ 1: output[0] = 0;
                 તોડો;
      }
   }
}

સ્થિર દશાંશ *stbi__hdr_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   અક્ષર buffer[STBI__HDR_BUFLEN];
   અક્ષર *token;
   પૂર્ણાંક valid = 0;
   પૂર્ણાંક width, height;
   stbi_uc *scanline;
   દશાંશ *hdr_data;
   પૂર્ણાંક len;
   unsigned અક્ષર ગણતરી, કિંમત;
   પૂર્ણાંક i, j, k, c1,c2, z;
   અચલ અક્ષર *headerToken;
   STBI_NOTUSED(ri);

   // Check identifier
   headerToken = stbi__hdr_gettoken(s,buffer);
   જો (strcmp(headerToken, "#?RADIANCE") != 0 && strcmp(headerToken, "#?RGBE") != 0)
      પાછા stbi__errpf("not HDR", "Corrupt HDR image");

   // Parse header
   માટે(;;) {
      token = stbi__hdr_gettoken(s,buffer);
      જો (token[0] == 0) તોડો;
      જો (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   જો (!valid)    પાછા stbi__errpf("unsupported format", "Unsupported HDR format");

   // Parse width and height
   // can't use sscanf() if we're not using stdio!
   token = stbi__hdr_gettoken(s,buffer);
   જો (strncmp(token, "-Y ", 3))  પાછા stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   height = (પૂર્ણાંક) strtol(token, &token, 10);
   જ્યારે (*token == ' ') ++token;
   જો (strncmp(token, "+X ", 3))  પાછા stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   width = (પૂર્ણાંક) strtol(token, ખાલી, 10);

   *x = width;
   *y = height;

   જો (comp) *comp = 3;
   જો (req_comp == 0) req_comp = 3;

   જો (!stbi__mad4sizes_valid(width, height, req_comp, કદ(દશાંશ), 0))
      પાછા stbi__errpf("too large", "HDR image is too large");

   // Read data
   hdr_data = (દશાંશ *) stbi__malloc_mad4(width, height, req_comp, કદ(દશાંશ), 0);
   જો (!hdr_data)
      પાછા stbi__errpf("outofmem", "Out of memory");

   // Load image data
   // image data is stored as some number of sca
   જો ( width < 8 || width >= 32768) {
      // Read flat data
      માટે (j=0; j < height; ++j) {
         માટે (i=0; i < width; ++i) {
            stbi_uc rgbe[4];
           main_decode_loop:
            stbi__getn(s, rgbe, 4);
            stbi__hdr_convert(hdr_data + j * width * req_comp + i * req_comp, rgbe, req_comp);
         }
      }
   } નહીંતર {
      // Read RLE-encoded data
      scanline = ખાલી;

      માટે (j = 0; j < height; ++j) {
         c1 = stbi__get8(s);
         c2 = stbi__get8(s);
         len = stbi__get8(s);
         જો (c1 != 2 || c2 != 2 || (len & 0x80)) {
            // not run-length encoded, so we have to actually use THIS data as a decoded
            // pixel (note this can't be a valid pixel--one of RGB must be >= 128)
            stbi_uc rgbe[4];
            rgbe[0] = (stbi_uc) c1;
            rgbe[1] = (stbi_uc) c2;
            rgbe[2] = (stbi_uc) len;
            rgbe[3] = (stbi_uc) stbi__get8(s);
            stbi__hdr_convert(hdr_data, rgbe, req_comp);
            i = 1;
            j = 0;
            STBI_FREE(scanline);
            જાઓ main_decode_loop; // yes, this makes no sense
         }
         len <<= 8;
         len |= stbi__get8(s);
         જો (len != width) { STBI_FREE(hdr_data); STBI_FREE(scanline); પાછા stbi__errpf("invalid decoded scanline length", "corrupt HDR"); }
         જો (scanline == ખાલી) {
            scanline = (stbi_uc *) stbi__malloc_mad2(width, 4, 0);
            જો (!scanline) {
               STBI_FREE(hdr_data);
               પાછા stbi__errpf("outofmem", "Out of memory");
            }
         }

         માટે (k = 0; k < 4; ++k) {
            પૂર્ણાંક nleft;
            i = 0;
            જ્યારે ((nleft = width - i) > 0) {
               ગણતરી = stbi__get8(s);
               જો (ગણતરી > 128) {
                  // Run
                  કિંમત = stbi__get8(s);
                  ગણતરી -= 128;
                  જો (ગણતરી > nleft) { STBI_FREE(hdr_data); STBI_FREE(scanline); પાછા stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  માટે (z = 0; z < ગણતરી; ++z)
                     scanline[i++ * 4 + k] = કિંમત;
               } નહીંતર {
                  // Dump
                  જો (ગણતરી > nleft) { STBI_FREE(hdr_data); STBI_FREE(scanline); પાછા stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  માટે (z = 0; z < ગણતરી; ++z)
                     scanline[i++ * 4 + k] = stbi__get8(s);
               }
            }
         }
         માટે (i=0; i < width; ++i)
            stbi__hdr_convert(hdr_data+(j*width + i)*req_comp, scanline + i*4, req_comp);
      }
      જો (scanline)
         STBI_FREE(scanline);
   }

   પાછા hdr_data;
}

સ્થિર પૂર્ણાંક stbi__hdr_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   અક્ષર buffer[STBI__HDR_BUFLEN];
   અક્ષર *token;
   પૂર્ણાંક valid = 0;
   પૂર્ણાંક dummy;

   જો (!x) x = &dummy;
   જો (!y) y = &dummy;
   જો (!comp) comp = &dummy;

   જો (stbi__hdr_test(s) == 0) {
       stbi__rewind( s );
       પાછા 0;
   }

   માટે(;;) {
      token = stbi__hdr_gettoken(s,buffer);
      જો (token[0] == 0) તોડો;
      જો (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   જો (!valid) {
       stbi__rewind( s );
       પાછા 0;
   }
   token = stbi__hdr_gettoken(s,buffer);
   જો (strncmp(token, "-Y ", 3)) {
       stbi__rewind( s );
       પાછા 0;
   }
   token += 3;
   *y = (પૂર્ણાંક) strtol(token, &token, 10);
   જ્યારે (*token == ' ') ++token;
   જો (strncmp(token, "+X ", 3)) {
       stbi__rewind( s );
       પાછા 0;
   }
   token += 3;
   *x = (પૂર્ણાંક) strtol(token, ખાલી, 10);
   *comp = 3;
   પાછા 1;
}
#endif // STBI_NO_HDR

#ifndef STBI_NO_BMP
સ્થિર પૂર્ણાંક stbi__bmp_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   શૂન્ય *p;
   stbi__bmp_data info;

   info.all_a = 255;
   p = stbi__bmp_parse_header(s, &info);
   stbi__rewind( s );
   જો (p == ખાલી)
      પાછા 0;
   જો (x) *x = s->img_x;
   જો (y) *y = s->img_y;
   જો (comp) *comp = info.ma ? 4 : 3;
   પાછા 1;
}
#endif

#ifndef STBI_NO_PSD
સ્થિર પૂર્ણાંક stbi__psd_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   પૂર્ણાંક channel_ગણતરી, dummy, depth;
   જો (!x) x = &dummy;
   જો (!y) y = &dummy;
   જો (!comp) comp = &dummy;
   જો (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       પાછા 0;
   }
   જો (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       પાછા 0;
   }
   stbi__skip(s, 6);
   channel_ગણતરી = stbi__get16be(s);
   જો (channel_ગણતરી < 0 || channel_ગણતરી > 16) {
       stbi__rewind( s );
       પાછા 0;
   }
   *y = stbi__get32be(s);
   *x = stbi__get32be(s);
   depth = stbi__get16be(s);
   જો (depth != 8 && depth != 16) {
       stbi__rewind( s );
       પાછા 0;
   }
   જો (stbi__get16be(s) != 3) {
       stbi__rewind( s );
       પાછા 0;
   }
   *comp = 4;
   પાછા 1;
}

સ્થિર પૂર્ણાંક stbi__psd_is16(stbi__context *s)
{
   પૂર્ણાંક channel_ગણતરી, depth;
   જો (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       પાછા 0;
   }
   જો (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       પાછા 0;
   }
   stbi__skip(s, 6);
   channel_ગણતરી = stbi__get16be(s);
   જો (channel_ગણતરી < 0 || channel_ગણતરી > 16) {
       stbi__rewind( s );
       પાછા 0;
   }
   (શૂન્ય) stbi__get32be(s);
   (શૂન્ય) stbi__get32be(s);
   depth = stbi__get16be(s);
   જો (depth != 16) {
       stbi__rewind( s );
       પાછા 0;
   }
   પાછા 1;
}
#endif

#ifndef STBI_NO_PIC
સ્થિર પૂર્ણાંક stbi__pic_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   પૂર્ણાંક act_comp=0,અંક_packets=0,chained,dummy;
   stbi__pic_packet packets[10];

   જો (!x) x = &dummy;
   જો (!y) y = &dummy;
   જો (!comp) comp = &dummy;

   જો (!stbi__pic_is4(s,"\x53\x80\xF6\x34")) {
      stbi__rewind(s);
      પાછા 0;
   }

   stbi__skip(s, 88);

   *x = stbi__get16be(s);
   *y = stbi__get16be(s);
   જો (stbi__at_eof(s)) {
      stbi__rewind( s);
      પાછા 0;
   }
   જો ( (*x) != 0 && (1 << 28) / (*x) < (*y)) {
      stbi__rewind( s );
      પાછા 0;
   }

   stbi__skip(s, 8);

   કરો {
      stbi__pic_packet *packet;

      જો (અંક_packets==કદ(packets)/કદ(packets[0]))
         પાછા 0;

      packet = &packets[અંક_packets++];
      chained = stbi__get8(s);
      packet->size    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);
      act_comp |= packet->channel;

      જો (stbi__at_eof(s)) {
          stbi__rewind( s );
          પાછા 0;
      }
      જો (packet->size != 8) {
          stbi__rewind( s );
          પાછા 0;
      }
   } જ્યારે (chained);

   *comp = (act_comp & 0x10 ? 4 : 3);

   પાછા 1;
}
#endif

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

#ifndef STBI_NO_PNM

સ્થિર પૂર્ણાંક      stbi__pnm_test(stbi__context *s)
{
   અક્ષર p, t;
   p = (અક્ષર) stbi__get8(s);
   t = (અક્ષર) stbi__get8(s);
   જો (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind( s );
       પાછા 0;
   }
   પાછા 1;
}

સ્થિર શૂન્ય *stbi__pnm_load(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp, પૂર્ણાંક req_comp, stbi__result_info *ri)
{
   stbi_uc *out;
   STBI_NOTUSED(ri);

   જો (!stbi__pnm_info(s, (પૂર્ણાંક *)&s->img_x, (પૂર્ણાંક *)&s->img_y, (પૂર્ણાંક *)&s->img_n))
      પાછા 0;

   *x = s->img_x;
   *y = s->img_y;
   જો (comp) *comp = s->img_n;

   જો (!stbi__mad3sizes_valid(s->img_n, s->img_x, s->img_y, 0))
      પાછા stbi__errpuc("too large", "PNM too large");

   out = (stbi_uc *) stbi__malloc_mad3(s->img_n, s->img_x, s->img_y, 0);
   જો (!out) પાછા stbi__errpuc("outofmem", "Out of memory");
   stbi__getn(s, out, s->img_n * s->img_x * s->img_y);

   જો (req_comp && req_comp != s->img_n) {
      out = stbi__convert_format(out, s->img_n, req_comp, s->img_x, s->img_y);
      જો (out == ખાલી) પાછા out; // stbi__convert_format frees input on failure
   }
   પાછા out;
}

સ્થિર પૂર્ણાંક      stbi__pnm_isspace(અક્ષર c)
{
   પાછા c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

સ્થિર શૂન્ય     stbi__pnm_skip_whitespace(stbi__context *s, અક્ષર *c)
{
   માટે (;;) {
      જ્યારે (!stbi__at_eof(s) && stbi__pnm_isspace(*c))
         *c = (અક્ષર) stbi__get8(s);

      જો (stbi__at_eof(s) || *c != '#')
         તોડો;

      જ્યારે (!stbi__at_eof(s) && *c != '\n' && *c != '\r' )
         *c = (અક્ષર) stbi__get8(s);
   }
}

સ્થિર પૂર્ણાંક      stbi__pnm_isdigit(અક્ષર c)
{
   પાછા c >= '0' && c <= '9';
}

સ્થિર પૂર્ણાંક      stbi__pnm_getinteger(stbi__context *s, અક્ષર *c)
{
   પૂર્ણાંક કિંમત = 0;

   જ્યારે (!stbi__at_eof(s) && stbi__pnm_isdigit(*c)) {
      કિંમત = કિંમત*10 + (*c - '0');
      *c = (અક્ષર) stbi__get8(s);
   }

   પાછા કિંમત;
}

સ્થિર પૂર્ણાંક      stbi__pnm_info(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   પૂર્ણાંક maxv, dummy;
   અક્ષર c, p, t;

   જો (!x) x = &dummy;
   જો (!y) y = &dummy;
   જો (!comp) comp = &dummy;

   stbi__rewind(s);

   // Get identifier
   p = (અક્ષર) stbi__get8(s);
   t = (અક્ષર) stbi__get8(s);
   જો (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind(s);
       પાછા 0;
   }

   *comp = (t == '6') ? 3 : 1;  // '5' is 1-component .pgm; '6' is 3-component .ppm

   c = (અક્ષર) stbi__get8(s);
   stbi__pnm_skip_whitespace(s, &c);

   *x = stbi__pnm_getinteger(s, &c); // read width
   stbi__pnm_skip_whitespace(s, &c);

   *y = stbi__pnm_getinteger(s, &c); // read height
   stbi__pnm_skip_whitespace(s, &c);

   maxv = stbi__pnm_getinteger(s, &c);  // read max value

   જો (maxv > 255)
      પાછા stbi__err("max value > 255", "PPM image not 8-bit");
   નહીંતર
      પાછા 1;
}
#endif

સ્થિર પૂર્ણાંક stbi__info_main(stbi__context *s, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   #ifndef STBI_NO_JPEG
   જો (stbi__jpeg_info(s, x, y, comp)) પાછા 1;
   #endif

   #ifndef STBI_NO_PNG
   જો (stbi__png_info(s, x, y, comp))  પાછા 1;
   #endif

   #ifndef STBI_NO_GIF
   જો (stbi__gif_info(s, x, y, comp))  પાછા 1;
   #endif

   #ifndef STBI_NO_BMP
   જો (stbi__bmp_info(s, x, y, comp))  પાછા 1;
   #endif

   #ifndef STBI_NO_PSD
   જો (stbi__psd_info(s, x, y, comp))  પાછા 1;
   #endif

   #ifndef STBI_NO_PIC
   જો (stbi__pic_info(s, x, y, comp))  પાછા 1;
   #endif

   #ifndef STBI_NO_PNM
   જો (stbi__pnm_info(s, x, y, comp))  પાછા 1;
   #endif

   #ifndef STBI_NO_HDR
   જો (stbi__hdr_info(s, x, y, comp))  પાછા 1;
   #endif

   // test tga last because it's a crappy test!
   #ifndef STBI_NO_TGA
   જો (stbi__tga_info(s, x, y, comp))
       પાછા 1;
   #endif
   પાછા stbi__err("unknown image type", "Image not of any known type, or corrupt");
}

સ્થિર પૂર્ણાંક stbi__is_16_main(stbi__context *s)
{
   #ifndef STBI_NO_PNG
   જો (stbi__png_is16(s))  પાછા 1;
   #endif

   #ifndef STBI_NO_PSD
   જો (stbi__psd_is16(s))  પાછા 1;
   #endif

   પાછા 0;
}

#ifndef STBI_NO_STDIO
STBIDEF પૂર્ણાંક stbi_info(અક્ષર અચલ *filename, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
    FILE *f = stbi__fopen(filename, "rb");
    પૂર્ણાંક result;
    જો (!f) પાછા stbi__err("can't fopen", "Unable to open file");
    result = stbi_info_from_file(f, x, y, comp);
    fclose(f);
    પાછા result;
}

STBIDEF પૂર્ણાંક stbi_info_from_file(FILE *f, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   પૂર્ણાંક r;
   stbi__context s;
   long pos = ftell(f);
   stbi__start_file(&s, f);
   r = stbi__info_main(&s,x,y,comp);
   fseek(f,pos,SEEK_SET);
   પાછા r;
}

STBIDEF પૂર્ણાંક stbi_is_16_bit(અક્ષર અચલ *filename)
{
    FILE *f = stbi__fopen(filename, "rb");
    પૂર્ણાંક result;
    જો (!f) પાછા stbi__err("can't fopen", "Unable to open file");
    result = stbi_is_16_bit_from_file(f);
    fclose(f);
    પાછા result;
}

STBIDEF પૂર્ણાંક stbi_is_16_bit_from_file(FILE *f)
{
   પૂર્ણાંક r;
   stbi__context s;
   long pos = ftell(f);
   stbi__start_file(&s, f);
   r = stbi__is_16_main(&s);
   fseek(f,pos,SEEK_SET);
   પાછા r;
}
#endif // !STBI_NO_STDIO

STBIDEF પૂર્ણાંક stbi_info_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   stbi__context s;
   stbi__start_mem(&s,buffer,len);
   પાછા stbi__info_main(&s,x,y,comp);
}

STBIDEF પૂર્ણાંક stbi_info_from_callbacks(stbi_io_callbacks અચલ *c, શૂન્ય *user, પૂર્ણાંક *x, પૂર્ણાંક *y, પૂર્ણાંક *comp)
{
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) c, user);
   પાછા stbi__info_main(&s,x,y,comp);
}

STBIDEF પૂર્ણાંક stbi_is_16_bit_from_memory(stbi_uc અચલ *buffer, પૂર્ણાંક len)
{
   stbi__context s;
   stbi__start_mem(&s,buffer,len);
   પાછા stbi__is_16_main(&s);
}

STBIDEF પૂર્ણાંક stbi_is_16_bit_from_callbacks(stbi_io_callbacks અચલ *c, શૂન્ય *user)
{
   stbi__context s;
   stbi__start_callbacks(&s, (stbi_io_callbacks *) c, user);
   પાછા stbi__is_16_main(&s);
}

#endif // STB_IMAGE_IMPLEMENTATION

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
