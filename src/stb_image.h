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

#నిర్వచితం_కాకపోతే STBI_INCLUDE_STB_IMAGE_H
#నిర్వచించు STBI_INCLUDE_STB_IMAGE_H

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


#నిర్వచితం_కాకపోతే STBI_NO_STDIO
#అన్తర్భావ <stdio.h>
#అయితే_ముగింపు // STBI_NO_STDIO

#నిర్వచించు STBI_సంస్కరణ 1

గణన
{
   STBI_default = 0, // only used for desired_channels

   STBI_grey       = 1,
   STBI_grey_alpha = 2,
   STBI_rgb        = 3,
   STBI_rgb_alpha  = 4
};

రకనిర్వచనం గుర్తులేని అక్షరం stbi_uc;
రకనిర్వచనం గుర్తులేని పొట్టి stbi_us;

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

#నిర్వచితమైతే STB_IMAGE_STATIC
#నిర్వచించు STBIDEF స్థిర
#లేకపోతే
#నిర్వచించు STBIDEF బాహ్య
#అయితే_ముగింపు

//////////////////////////////////////////////////////////////////////////////
//
// PRIMARY API - works on images of any type
//

//
// load image by filename, open file, or memory buffer
//

రకనిర్వచనం నిర్మాణం
{
   పూర్ణాంకం      (*పఠ)  (శూన్య *user,అక్షరం *దత్తాంశం,పూర్ణాంకం కొలత);   // fill 'data' with 'size' bytes.  return number of bytes actually read
   శూన్య     (*skip)  (శూన్య *user,పూర్ణాంకం n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
   పూర్ణాంకం      (*eof)   (శూన్య *user);                       // returns nonzero if we are at end of file/data
} stbi_io_callbacks;

////////////////////////////////////
//
// 8-bits-per-channel interface
//

STBIDEF stbi_uc *stbi_ఎక్కించు_from_జ్ఞాపకం   (stbi_uc           స్థిరరాశి *బఫరు, పూర్ణాంకం len   , పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
STBIDEF stbi_uc *stbi_ఎక్కించు_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk  , శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
#నిర్వచితం_కాకపోతే STBI_NO_GIF
STBIDEF stbi_uc *stbi_ఎక్కించు_gif_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం **delays, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *z, పూర్ణాంకం *comp, పూర్ణాంకం req_comp);
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే STBI_NO_STDIO
STBIDEF stbi_uc *stbi_ఎక్కించు            (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
STBIDEF stbi_uc *stbi_ఎక్కించు_from_file  (FILE *f, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
// for stbi_load_from_file, file pointer is left pointing immediately after image
#అయితే_ముగింపు

////////////////////////////////////
//
// 16-bits-per-channel interface
//

STBIDEF stbi_us *stbi_ఎక్కించు_16_from_జ్ఞాపకం   (stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
STBIDEF stbi_us *stbi_ఎక్కించు_16_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);

#నిర్వచితం_కాకపోతే STBI_NO_STDIO
STBIDEF stbi_us *stbi_ఎక్కించు_16          (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
STBIDEF stbi_us *stbi_ఎక్కించు_from_file_16(FILE *f, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
#అయితే_ముగింపు

////////////////////////////////////
//
// float-per-channel interface
//
#నిర్వచితం_కాకపోతే STBI_NO_రేఖీయ
   STBIDEF దశాంశం *stbi_loadf_from_జ్ఞాపకం     (stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
   STBIDEF దశాంశం *stbi_loadf_from_callbacks  (stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y,  పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);

   #నిర్వచితం_కాకపోతే STBI_NO_STDIO
   STBIDEF దశాంశం *stbi_loadf            (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
   STBIDEF దశాంశం *stbi_loadf_from_file  (FILE *f, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels);
   #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_HDR
   STBIDEF శూన్య   stbi_hdr_గమ్యం_ldr_gamma(దశాంశం gamma);
   STBIDEF శూన్య   stbi_hdr_గమ్యం_ldr_scale(దశాంశం scale);
#అయితే_ముగింపు // STBI_NO_HDR

#నిర్వచితం_కాకపోతే STBI_NO_రేఖీయ
   STBIDEF శూన్య   stbi_ldr_గమ్యం_hdr_gamma(దశాంశం gamma);
   STBIDEF శూన్య   stbi_ldr_గమ్యం_hdr_scale(దశాంశం scale);
#అయితే_ముగింపు // STBI_NO_LINEAR

// stbi_is_hdr is always defined, but always returns false if STBI_NO_HDR
STBIDEF పూర్ణాంకం    stbi_is_hdr_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user);
STBIDEF పూర్ణాంకం    stbi_is_hdr_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len);
#నిర్వచితం_కాకపోతే STBI_NO_STDIO
STBIDEF పూర్ణాంకం      stbi_is_hdr          (అక్షరం స్థిరరాశి *filename);
STBIDEF పూర్ణాంకం      stbi_is_hdr_from_file(FILE *f);
#అయితే_ముగింపు // STBI_NO_STDIO


// get a VERY brief reason for failure
// NOT THREADSAFE
STBIDEF స్థిరరాశి అక్షరం *stbi_failure_reason  (శూన్య);

// free the loaded image -- this is just free()
STBIDEF శూన్య     stbi_image_free      (శూన్య *retval_from_stbi_ఎక్కించు);

// get image dimensions & components without fully decoding
STBIDEF పూర్ణాంకం      stbi_info_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
STBIDEF పూర్ణాంకం      stbi_info_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
STBIDEF పూర్ణాంకం      stbi_is_16_బిట్టు_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len);
STBIDEF పూర్ణాంకం      stbi_is_16_బిట్టు_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user);

#నిర్వచితం_కాకపోతే STBI_NO_STDIO
STBIDEF పూర్ణాంకం      stbi_info               (అక్షరం స్థిరరాశి *filename,     పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
STBIDEF పూర్ణాంకం      stbi_info_from_file     (FILE *f,                  పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
STBIDEF పూర్ణాంకం      stbi_is_16_బిట్టు          (అక్షరం స్థిరరాశి *filename);
STBIDEF పూర్ణాంకం      stbi_is_16_బిట్టు_from_file(FILE *f);
#అయితే_ముగింపు



// for image formats that explicitly notate that they have premultiplied alpha,
// we just return the colors as stored in the file. set this flag to force
// unpremultiplication. results are undefined if the unpremultiply overflow.
STBIDEF శూన్య stbi_సమితి_unpremultiply_on_ఎక్కించు(పూర్ణాంకం flag_true_if_should_unpremultiply);

// indicate whether we should process iphone images back to canonical format,
// or just pass them through "as-is"
STBIDEF శూన్య stbi_convert_iphone_png_గమ్యం_rgb(పూర్ణాంకం flag_true_if_should_convert);

// flip the image vertically, so the first pixel in the output array is the bottom left
STBIDEF శూన్య stbi_సమితి_flip_vertically_on_ఎక్కించు(పూర్ణాంకం flag_true_if_should_flip);

// ZLIB client - used by PNG, available for other purposes

STBIDEF అక్షరం *stbi_zlib_decode_malloc_guesssize(స్థిరరాశి అక్షరం *బఫరు, పూర్ణాంకం len, పూర్ణాంకం initial_కొలత, పూర్ణాంకం *outlen);
STBIDEF అక్షరం *stbi_zlib_decode_malloc_guesssize_headerflag(స్థిరరాశి అక్షరం *బఫరు, పూర్ణాంకం len, పూర్ణాంకం initial_కొలత, పూర్ణాంకం *outlen, పూర్ణాంకం parse_శీర్షిక);
STBIDEF అక్షరం *stbi_zlib_decode_malloc(స్థిరరాశి అక్షరం *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *outlen);
STBIDEF పూర్ణాంకం   stbi_zlib_decode_బఫరు(అక్షరం *obuffer, పూర్ణాంకం olen, స్థిరరాశి అక్షరం *ibuffer, పూర్ణాంకం ilen);

STBIDEF అక్షరం *stbi_zlib_decode_noheader_malloc(స్థిరరాశి అక్షరం *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *outlen);
STBIDEF పూర్ణాంకం   stbi_zlib_decode_noheader_బఫరు(అక్షరం *obuffer, పూర్ణాంకం olen, స్థిరరాశి అక్షరం *ibuffer, పూర్ణాంకం ilen);


#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

//
//
////   end header file   /////////////////////////////////////////////////////
#అయితే_ముగింపు // STBI_INCLUDE_STB_IMAGE_H

#నిర్వచితమైతే STB_IMAGE_IMPLEMENTATION

#ఒకవేళ defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
   #నిర్వచితం_కాకపోతే STBI_ONLY_JPEG
   #నిర్వచించు STBI_NO_JPEG
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_PNG
   #నిర్వచించు STBI_NO_PNG
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_BMP
   #నిర్వచించు STBI_NO_BMP
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_PSD
   #నిర్వచించు STBI_NO_PSD
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_TGA
   #నిర్వచించు STBI_NO_TGA
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_GIF
   #నిర్వచించు STBI_NO_GIF
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_HDR
   #నిర్వచించు STBI_NO_HDR
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_PIC
   #నిర్వచించు STBI_NO_PIC
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_ONLY_PNM
   #నిర్వచించు STBI_NO_PNM
   #అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ defined(STBI_NO_PNG) && !defined(STBI_SUPPORT_ZLIB) && !defined(STBI_NO_ZLIB)
#నిర్వచించు STBI_NO_ZLIB
#అయితే_ముగింపు


#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <stddef.h> // ptrdiff_t on osx
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <limits.h>

#ఒకవేళ !defined(STBI_NO_రేఖీయ) || !defined(STBI_NO_HDR)
#అన్తర్భావ <math.h>  // ldexp, pow
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_STDIO
#అన్తర్భావ <stdio.h>
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_ASSERT
#అన్తర్భావ <assert.h>
#నిర్వచించు STBI_ASSERT(x) ప్రతిజ్ఞా(x)
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే _MSC_VER
   #నిర్వచితమైతే __cplusplus
   #నిర్వచించు stbi_inline అంతఃవరుస
   #లేకపోతే
   #నిర్వచించు stbi_inline
   #అయితే_ముగింపు
#లేకపోతే
   #నిర్వచించు stbi_inline __forceinline
#అయితే_ముగింపు


#నిర్వచితమైతే _MSC_VER
రకనిర్వచనం గుర్తులేని పొట్టి stbi__uint16;
రకనిర్వచనం   గుర్తుగల పొట్టి stbi__int16;
రకనిర్వచనం గుర్తులేని పూర్ణాంకం   stbi__uint32;
రకనిర్వచనం   గుర్తుగల పూర్ణాంకం   stbi__int32;
#లేకపోతే
#అన్తర్భావ <stdint.h>
రకనిర్వచనం uint16_t stbi__uint16;
రకనిర్వచనం int16_t  stbi__int16;
రకనిర్వచనం uint32_t stbi__uint32;
రకనిర్వచనం int32_t  stbi__int32;
#అయితే_ముగింపు

// should produce compiler error if size is wrong
రకనిర్వచనం గుర్తులేని అక్షరం validate_uint32[పరిమాణం(stbi__uint32)==4 ? 1 : -1];

#నిర్వచితమైతే _MSC_VER
#నిర్వచించు STBI_NOTUSED(v)  (శూన్య)(v)
#లేకపోతే
#నిర్వచించు STBI_NOTUSED(v)  (శూన్య)పరిమాణం(v)
#అయితే_ముగింపు

#నిర్వచితమైతే _MSC_VER
#నిర్వచించు STBI_HAS_LROTL
#అయితే_ముగింపు

#నిర్వచితమైతే STBI_HAS_LROTL
   #నిర్వచించు stbi_lrot(x,y)  _lrotl(x,y)
#లేకపోతే
   #నిర్వచించు stbi_lrot(x,y)  (((x) << (y)) | ((x) >> (32 - (y))))
#అయితే_ముగింపు

#ఒకవేళ defined(STBI_MALLOC) && defined(STBI_FREE) && (defined(STBI_REALLOC) || defined(STBI_REALLOC_SIZED))
// ok
#లేదావేళ !defined(STBI_MALLOC) && !defined(STBI_FREE) && !defined(STBI_REALLOC) && !defined(STBI_REALLOC_SIZED)
// ok
#లేకపోతే
#దోషం "Must define all or none of STBI_MALLOC, STBI_FREE, and STBI_REALLOC (or STBI_REALLOC_SIZED)."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_MALLOC
#నిర్వచించు STBI_MALLOC(sz)           జ్ఞాపకం_ఇవ్వు(sz)
#నిర్వచించు STBI_REALLOC(p,newsz)     realloc(p,newsz)
#నిర్వచించు STBI_FREE(p)              విడుదల(p)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_REALLOC_SIZED
#నిర్వచించు STBI_REALLOC_SIZED(p,oldsz,newsz) STBI_REALLOC(p,newsz)
#అయితే_ముగింపు

// x86/x64 detection
#ఒకవేళ defined(__x86_64__) || defined(_M_X64)
#నిర్వచించు STBI_X64_లక్ష్యం
#లేదావేళ defined(__i386) || defined(_M_IX86)
#నిర్వచించు STBI_X86_లక్ష్యం
#అయితే_ముగింపు

#ఒకవేళ defined(__GNUC__) && defined(STBI_X86_లక్ష్యం) && !defined(__SSE2__) && !defined(STBI_NO_SIMD)
// gcc doesn't support sse2 intrinsics unless you compile with -msse2,
// which in turn means it gets to use SSE2 everywhere. This is unfortunate,
// but previous attempts to provide the SSE2 functions with runtime
// detection caused numerous issues. The way architecture extensions are
// exposed in GCC/Clang is, sadly, not really suited for one-file libs.
// New behavior: if compiled with -msse2, we use SSE2 without any
// detection; if not, we don't use it at all.
#నిర్వచించు STBI_NO_SIMD
#అయితే_ముగింపు

#ఒకవేళ defined(__MINGW32__) && defined(STBI_X86_లక్ష్యం) && !defined(STBI_MINGW_ENABLE_SSE2) && !defined(STBI_NO_SIMD)
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
#నిర్వచించు STBI_NO_SIMD
#అయితే_ముగింపు

#ఒకవేళ !defined(STBI_NO_SIMD) && (defined(STBI_X86_లక్ష్యం) || defined(STBI_X64_లక్ష్యం))
#నిర్వచించు STBI_SSE2
#అన్తర్భావ <emmintrin.h>

#నిర్వచితమైతే _MSC_VER

#ఒకవేళ _MSC_VER >= 1400  // not VC6
#అన్తర్భావ <intrin.h> // __cpuid
స్థిర పూర్ణాంకం stbi__cpuid3(శూన్య)
{
   పూర్ణాంకం info[4];
   __cpuid(info,1);
   తిరిగి info[3];
}
#లేకపోతే
స్థిర పూర్ణాంకం stbi__cpuid3(శూన్య)
{
   పూర్ణాంకం res;
   __asm {
      mov  eax,1
      cpuid
      mov  res,edx
   }
   తిరిగి res;
}
#అయితే_ముగింపు

#నిర్వచించు STBI_SIMD_ALIGN(type, name) __declspec(align(16)) type name

స్థిర పూర్ణాంకం stbi_sse2_అందుబాటు(శూన్య)
{
   పూర్ణాంకం info3 = stbi__cpuid3();
   తిరిగి ((info3 >> 26) & 1) != 0;
}
#లేకపోతే // assume GCC-style if not VC++
#నిర్వచించు STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))

స్థిర పూర్ణాంకం stbi_sse2_అందుబాటు(శూన్య)
{
   // If we're even attempting to compile this on GCC/Clang, that means
   // -msse2 is on, which means the compiler is allowed to use SSE2
   // instructions at will, and so are we.
   తిరిగి 1;
}
#అయితే_ముగింపు
#అయితే_ముగింపు

// ARM NEON
#ఒకవేళ defined(STBI_NO_SIMD) && defined(STBI_NEON)
#నిర్వచనం_రద్దు STBI_NEON
#అయితే_ముగింపు

#నిర్వచితమైతే STBI_NEON
#అన్తర్భావ <arm_neon.h>
// assume GCC or Clang on ARM targets
#నిర్వచించు STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_SIMD_ALIGN
#నిర్వచించు STBI_SIMD_ALIGN(type, name) type name
#అయితే_ముగింపు

///////////////////////////////////////////////
//
//  stbi__context struct and start_xxx functions

// stbi__context structure is our basic context used by all images, so it
// contains all the IO context, plus some basic image information
రకనిర్వచనం నిర్మాణం
{
   stbi__uint32 img_x, img_y;
   పూర్ణాంకం img_n, img_బహిర్గతం_n;

   stbi_io_callbacks io;
   శూన్య *io_user_దత్తాంశం;

   పూర్ణాంకం read_from_callbacks;
   పూర్ణాంకం buflen;
   stbi_uc బఫరు_ప్రారంభం[128];

   stbi_uc *img_బఫరు, *img_బఫరు_చివర;
   stbi_uc *img_బఫరు_original, *img_బఫరు_original_చివర;
} stbi__context;


స్థిర శూన్య stbi_refill_బఫరు(stbi__context *s);

// initialize a memory-decode context
స్థిర శూన్య stbi_ప్రారంభం_mem(stbi__context *s, stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len)
{
   s->io.పఠ = ఖాళీ;
   s->read_from_callbacks = 0;
   s->img_బఫరు = s->img_బఫరు_original = (stbi_uc *) బఫరు;
   s->img_బఫరు_చివర = s->img_బఫరు_original_చివర = (stbi_uc *) బఫరు+len;
}

// initialize a callback-based context
స్థిర శూన్య stbi_ప్రారంభం_callbacks(stbi__context *s, stbi_io_callbacks *c, శూన్య *user)
{
   s->io = *c;
   s->io_user_దత్తాంశం = user;
   s->buflen = పరిమాణం(s->బఫరు_ప్రారంభం);
   s->read_from_callbacks = 1;
   s->img_బఫరు_original = s->బఫరు_ప్రారంభం;
   stbi_refill_బఫరు(s);
   s->img_బఫరు_original_చివర = s->img_బఫరు_చివర;
}

#నిర్వచితం_కాకపోతే STBI_NO_STDIO

స్థిర పూర్ణాంకం stbi__stdio_read(శూన్య *user, అక్షరం *దత్తాంశం, పూర్ణాంకం కొలత)
{
   తిరిగి (పూర్ణాంకం) fread(దత్తాంశం,1,కొలత,(FILE*) user);
}

స్థిర శూన్య stbi__stdio_skip(శూన్య *user, పూర్ణాంకం n)
{
   fseek((FILE*) user, n, SEEK_CUR);
}

స్థిర పూర్ణాంకం stbi__stdio_eof(శూన్య *user)
{
   తిరిగి feof((FILE*) user);
}

స్థిర stbi_io_callbacks stbi__stdio_callbacks =
{
   stbi__stdio_read,
   stbi__stdio_skip,
   stbi__stdio_eof,
};

స్థిర శూన్య stbi_ప్రారంభం_file(stbi__context *s, FILE *f)
{
   stbi_ప్రారంభం_callbacks(s, &stbi__stdio_callbacks, (శూన్య *) f);
}

//static void stop_file(stbi__context *s) { }

#అయితే_ముగింపు // !STBI_NO_STDIO

స్థిర శూన్య stbi__rewind(stbi__context *s)
{
   // conceptually rewind SHOULD rewind to the beginning of the stream,
   // but we just rewind to the beginning of the initial buffer, because
   // we only use it after doing 'test', which only ever looks at at most 92 bytes
   s->img_బఫరు = s->img_బఫరు_original;
   s->img_బఫరు_చివర = s->img_బఫరు_original_చివర;
}

గణన
{
   STBI_క్రమం_RGB,
   STBI_క్రమం_BGR
};

రకనిర్వచనం నిర్మాణం
{
   పూర్ణాంకం బిట్లు_చొప్పున_channel;
   పూర్ణాంకం అంకె_channels;
   పూర్ణాంకం channel_క్రమం;
} stbi_ఫలితం_info;

#నిర్వచితం_కాకపోతే STBI_NO_JPEG
స్థిర పూర్ణాంకం      stbi__jpeg_test(stbi__context *s);
స్థిర శూన్య    *stbi_jpeg_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర పూర్ణాంకం      stbi__jpeg_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_PNG
స్థిర పూర్ణాంకం      stbi__png_test(stbi__context *s);
స్థిర శూన్య    *stbi_png_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర పూర్ణాంకం      stbi__png_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
స్థిర పూర్ణాంకం      stbi__png_is16(stbi__context *s);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_BMP
స్థిర పూర్ణాంకం      stbi__bmp_test(stbi__context *s);
స్థిర శూన్య    *stbi_bmp_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర పూర్ణాంకం      stbi__bmp_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_TGA
స్థిర పూర్ణాంకం      stbi__tga_test(stbi__context *s);
స్థిర శూన్య    *stbi_tga_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర పూర్ణాంకం      stbi__tga_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_PSD
స్థిర పూర్ణాంకం      stbi__psd_test(stbi__context *s);
స్థిర శూన్య    *stbi_psd_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri, పూర్ణాంకం bpc);
స్థిర పూర్ణాంకం      stbi__psd_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
స్థిర పూర్ణాంకం      stbi__psd_is16(stbi__context *s);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_HDR
స్థిర పూర్ణాంకం      stbi__hdr_test(stbi__context *s);
స్థిర దశాంశం   *stbi_hdr_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర పూర్ణాంకం      stbi__hdr_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_PIC
స్థిర పూర్ణాంకం      stbi__pic_test(stbi__context *s);
స్థిర శూన్య    *stbi_pic_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర పూర్ణాంకం      stbi__pic_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_GIF
స్థిర పూర్ణాంకం      stbi__gif_test(stbi__context *s);
స్థిర శూన్య    *stbi_gif_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర శూన్య    *stbi_ఎక్కించు_gif_main(stbi__context *s, పూర్ణాంకం **delays, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *z, పూర్ణాంకం *comp, పూర్ణాంకం req_comp);
స్థిర పూర్ణాంకం      stbi__gif_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_PNM
స్థిర పూర్ణాంకం      stbi__pnm_test(stbi__context *s);
స్థిర శూన్య    *stbi_pnm_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri);
స్థిర పూర్ణాంకం      stbi__pnm_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp);
#అయితే_ముగింపు

// this is not threadsafe
స్థిర స్థిరరాశి అక్షరం *stbi__g_failure_reason;

STBIDEF స్థిరరాశి అక్షరం *stbi_failure_reason(శూన్య)
{
   తిరిగి stbi__g_failure_reason;
}

స్థిర పూర్ణాంకం stbi__err(స్థిరరాశి అక్షరం *str)
{
   stbi__g_failure_reason = str;
   తిరిగి 0;
}

స్థిర శూన్య *stbi__malloc(size_t కొలత)
{
    తిరిగి STBI_MALLOC(కొలత);
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
స్థిర పూర్ణాంకం stbi__addsizes_valid(పూర్ణాంకం a, పూర్ణాంకం b)
{
   ఒకవేళ (b < 0) తిరిగి 0;
   // now 0 <= b <= INT_MAX, hence also
   // 0 <= INT_MAX - b <= INTMAX.
   // And "a + b <= INT_MAX" (which might overflow) is the
   // same as a <= INT_MAX - b (no overflow)
   తిరిగి a <= గరిష్ఠ_పూర్ణాంకం - b;
}

// returns 1 if the product is valid, 0 on overflow.
// negative factors are considered invalid.
స్థిర పూర్ణాంకం stbi__mul2sizes_valid(పూర్ణాంకం a, పూర్ణాంకం b)
{
   ఒకవేళ (a < 0 || b < 0) తిరిగి 0;
   ఒకవేళ (b == 0) తిరిగి 1; // mul-by-0 is always safe
   // portable way to check for no overflows in a*b
   తిరిగి a <= గరిష్ఠ_పూర్ణాంకం/b;
}

// returns 1 if "a*b + add" has no negative terms/factors and doesn't overflow
స్థిర పూర్ణాంకం stbi__mad2sizes_valid(పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం కూడు)
{
   తిరిగి stbi__mul2sizes_valid(a, b) && stbi__addsizes_valid(a*b, కూడు);
}

// returns 1 if "a*b*c + add" has no negative terms/factors and doesn't overflow
స్థిర పూర్ణాంకం stbi__mad3sizes_valid(పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం c, పూర్ణాంకం కూడు)
{
   తిరిగి stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__addsizes_valid(a*b*c, కూడు);
}

// returns 1 if "a*b*c*d + add" has no negative terms/factors and doesn't overflow
#ఒకవేళ !defined(STBI_NO_రేఖీయ) || !defined(STBI_NO_HDR)
స్థిర పూర్ణాంకం stbi__mad4sizes_valid(పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం c, పూర్ణాంకం d, పూర్ణాంకం కూడు)
{
   తిరిగి stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a*b, c) &&
      stbi__mul2sizes_valid(a*b*c, d) && stbi__addsizes_valid(a*b*c*d, కూడు);
}
#అయితే_ముగింపు

// mallocs with size overflow checking
స్థిర శూన్య *stbi__malloc_mad2(పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం కూడు)
{
   ఒకవేళ (!stbi__mad2sizes_valid(a, b, కూడు)) తిరిగి ఖాళీ;
   తిరిగి stbi__malloc(a*b + కూడు);
}

స్థిర శూన్య *stbi__malloc_mad3(పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం c, పూర్ణాంకం కూడు)
{
   ఒకవేళ (!stbi__mad3sizes_valid(a, b, c, కూడు)) తిరిగి ఖాళీ;
   తిరిగి stbi__malloc(a*b*c + కూడు);
}

#ఒకవేళ !defined(STBI_NO_రేఖీయ) || !defined(STBI_NO_HDR)
స్థిర శూన్య *stbi__malloc_mad4(పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం c, పూర్ణాంకం d, పూర్ణాంకం కూడు)
{
   ఒకవేళ (!stbi__mad4sizes_valid(a, b, c, d, కూడు)) తిరిగి ఖాళీ;
   తిరిగి stbi__malloc(a*b*c*d + కూడు);
}
#అయితే_ముగింపు

// stbi__err - error
// stbi__errpf - error returning pointer to float
// stbi__errpuc - error returning pointer to unsigned char

#నిర్వచితమైతే STBI_NO_FAILURE_అక్షరశ్రేణులు
   #నిర్వచించు stbi__err(x,y)  0
#లేదావేళ defined(STBI_FAILURE_USERMSG)
   #నిర్వచించు stbi__err(x,y)  stbi__err(y)
#లేకపోతే
   #నిర్వచించు stbi__err(x,y)  stbi__err(x)
#అయితే_ముగింపు

#నిర్వచించు stbi__errpf(x,y)   ((దశాంశం *)(size_t) (stbi__err(x,y)?ఖాళీ:ఖాళీ))
#నిర్వచించు stbi__errpuc(x,y)  ((గుర్తులేని అక్షరం *)(size_t) (stbi__err(x,y)?ఖాళీ:ఖాళీ))

STBIDEF శూన్య stbi_image_free(శూన్య *retval_from_stbi_ఎక్కించు)
{
   STBI_FREE(retval_from_stbi_ఎక్కించు);
}

#నిర్వచితం_కాకపోతే STBI_NO_రేఖీయ
స్థిర దశాంశం   *stbi_ldr_గమ్యం_hdr(stbi_uc *దత్తాంశం, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం comp);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_HDR
స్థిర stbi_uc *stbi_hdr_గమ్యం_ldr(దశాంశం   *దత్తాంశం, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం comp);
#అయితే_ముగింపు

స్థిర పూర్ణాంకం stbi_vertically_flip_on_ఎక్కించు = 0;

STBIDEF శూన్య stbi_సమితి_flip_vertically_on_ఎక్కించు(పూర్ణాంకం flag_true_if_should_flip)
{
    stbi_vertically_flip_on_ఎక్కించు = flag_true_if_should_flip;
}

స్థిర శూన్య *stbi_ఎక్కించు_main(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri, పూర్ణాంకం bpc)
{
   memset(ri, 0, పరిమాణం(*ri)); // make sure it's initialized if we add new fields
   ri->బిట్లు_చొప్పున_channel = 8; // default is 8 so most paths don't have to be changed
   ri->channel_క్రమం = STBI_క్రమం_RGB; // all current input & output are this, but this is here so we can add BGR order
   ri->అంకె_channels = 0;

   #నిర్వచితం_కాకపోతే STBI_NO_JPEG
   ఒకవేళ (stbi__jpeg_test(s)) తిరిగి stbi_jpeg_ఎక్కించు(s,x,y,comp,req_comp, ri);
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_NO_PNG
   ఒకవేళ (stbi__png_test(s))  తిరిగి stbi_png_ఎక్కించు(s,x,y,comp,req_comp, ri);
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_NO_BMP
   ఒకవేళ (stbi__bmp_test(s))  తిరిగి stbi_bmp_ఎక్కించు(s,x,y,comp,req_comp, ri);
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_NO_GIF
   ఒకవేళ (stbi__gif_test(s))  తిరిగి stbi_gif_ఎక్కించు(s,x,y,comp,req_comp, ri);
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_NO_PSD
   ఒకవేళ (stbi__psd_test(s))  తిరిగి stbi_psd_ఎక్కించు(s,x,y,comp,req_comp, ri, bpc);
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_NO_PIC
   ఒకవేళ (stbi__pic_test(s))  తిరిగి stbi_pic_ఎక్కించు(s,x,y,comp,req_comp, ri);
   #అయితే_ముగింపు
   #నిర్వచితం_కాకపోతే STBI_NO_PNM
   ఒకవేళ (stbi__pnm_test(s))  తిరిగి stbi_pnm_ఎక్కించు(s,x,y,comp,req_comp, ri);
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_HDR
   ఒకవేళ (stbi__hdr_test(s)) {
      దశాంశం *hdr = stbi_hdr_ఎక్కించు(s, x,y,comp,req_comp, ri);
      తిరిగి stbi_hdr_గమ్యం_ldr(hdr, *x, *y, req_comp ? req_comp : *comp);
   }
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_TGA
   // test tga last because it's a crappy test!
   ఒకవేళ (stbi__tga_test(s))
      తిరిగి stbi_tga_ఎక్కించు(s,x,y,comp,req_comp, ri);
   #అయితే_ముగింపు

   తిరిగి stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
}

స్థిర stbi_uc *stbi_convert_16_గమ్యం_8(stbi__uint16 *orig, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం channels)
{
   పూర్ణాంకం i;
   పూర్ణాంకం img_len = w * h * channels;
   stbi_uc *reduced;

   reduced = (stbi_uc *) stbi__malloc(img_len);
   ఒకవేళ (reduced == ఖాళీ) తిరిగి stbi__errpuc("outofmem", "Out of memory");

   కొరకు (i = 0; i < img_len; ++i)
      reduced[i] = (stbi_uc)((orig[i] >> 8) & 0xFF); // top half of each byte is sufficient approx of 16->8 bit scaling

   STBI_FREE(orig);
   తిరిగి reduced;
}

స్థిర stbi__uint16 *stbi_convert_8_గమ్యం_16(stbi_uc *orig, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం channels)
{
   పూర్ణాంకం i;
   పూర్ణాంకం img_len = w * h * channels;
   stbi__uint16 *enlarged;

   enlarged = (stbi__uint16 *) stbi__malloc(img_len*2);
   ఒకవేళ (enlarged == ఖాళీ) తిరిగి (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");

   కొరకు (i = 0; i < img_len; ++i)
      enlarged[i] = (stbi__uint16)((orig[i] << 8) + orig[i]); // replicate to high and low byte, maps 0->0, 255->0xffff

   STBI_FREE(orig);
   తిరిగి enlarged;
}

స్థిర శూన్య stbi__vertical_flip(శూన్య *image, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం బైట్లు_చొప్పున_pixel)
{
   పూర్ణాంకం అడ్డువరుస;
   size_t బైట్లు_చొప్పున_అడ్డువరుస = (size_t)w * బైట్లు_చొప్పున_pixel;
   stbi_uc తాత్కాలిక[2048];
   stbi_uc *బైట్లు = (stbi_uc *)image;

   కొరకు (అడ్డువరుస = 0; అడ్డువరుస < (h>>1); అడ్డువరుస++) {
      stbi_uc *row0 = బైట్లు + అడ్డువరుస*బైట్లు_చొప్పున_అడ్డువరుస;
      stbi_uc *row1 = బైట్లు + (h - అడ్డువరుస - 1)*బైట్లు_చొప్పున_అడ్డువరుస;
      // swap row0 with row1
      size_t బైట్లు_ఎడమ = బైట్లు_చొప్పున_అడ్డువరుస;
      అయితే (బైట్లు_ఎడమ) {
         size_t బైట్లు_copy = (బైట్లు_ఎడమ < పరిమాణం(తాత్కాలిక)) ? బైట్లు_ఎడమ : పరిమాణం(తాత్కాలిక);
         జ్ఞాపకం_నకలు(తాత్కాలిక, row0, బైట్లు_copy);
         జ్ఞాపకం_నకలు(row0, row1, బైట్లు_copy);
         జ్ఞాపకం_నకలు(row1, తాత్కాలిక, బైట్లు_copy);
         row0 += బైట్లు_copy;
         row1 += బైట్లు_copy;
         బైట్లు_ఎడమ -= బైట్లు_copy;
      }
   }
}

స్థిర శూన్య stbi__vertical_flip_slices(శూన్య *image, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం z, పూర్ణాంకం బైట్లు_చొప్పున_pixel)
{
   పూర్ణాంకం ముక్క;
   పూర్ణాంకం ముక్క_కొలత = w * h * బైట్లు_చొప్పున_pixel;

   stbi_uc *బైట్లు = (stbi_uc *)image;
   కొరకు (ముక్క = 0; ముక్క < z; ++ముక్క) {
      stbi__vertical_flip(బైట్లు, w, h, బైట్లు_చొప్పున_pixel); 
      బైట్లు += ముక్క_కొలత; 
   }
}

స్థిర గుర్తులేని అక్షరం *stbi_ఎక్కించు_and_postprocess_8bit(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi_ఫలితం_info ri;
   శూన్య *ఫలితం = stbi_ఎక్కించు_main(s, x, y, comp, req_comp, &ri, 8);

   ఒకవేళ (ఫలితం == ఖాళీ)
      తిరిగి ఖాళీ;

   ఒకవేళ (ri.బిట్లు_చొప్పున_channel != 8) {
      STBI_ASSERT(ri.బిట్లు_చొప్పున_channel == 16);
      ఫలితం = stbi_convert_16_గమ్యం_8((stbi__uint16 *) ఫలితం, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.బిట్లు_చొప్పున_channel = 8;
   }

   // @TODO: move stbi__convert_format to here

   ఒకవేళ (stbi_vertically_flip_on_ఎక్కించు) {
      పూర్ణాంకం channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ఫలితం, *x, *y, channels * పరిమాణం(stbi_uc));
   }

   తిరిగి (గుర్తులేని అక్షరం *) ఫలితం;
}

స్థిర stbi__uint16 *stbi_ఎక్కించు_and_postprocess_16bit(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi_ఫలితం_info ri;
   శూన్య *ఫలితం = stbi_ఎక్కించు_main(s, x, y, comp, req_comp, &ri, 16);

   ఒకవేళ (ఫలితం == ఖాళీ)
      తిరిగి ఖాళీ;

   ఒకవేళ (ri.బిట్లు_చొప్పున_channel != 16) {
      STBI_ASSERT(ri.బిట్లు_చొప్పున_channel == 8);
      ఫలితం = stbi_convert_8_గమ్యం_16((stbi_uc *) ఫలితం, *x, *y, req_comp == 0 ? *comp : req_comp);
      ri.బిట్లు_చొప్పున_channel = 16;
   }

   // @TODO: move stbi__convert_format16 to here
   // @TODO: special case RGB-to-Y (and RGBA-to-YA) for 8-bit-to-16-bit case to keep more precision

   ఒకవేళ (stbi_vertically_flip_on_ఎక్కించు) {
      పూర్ణాంకం channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ఫలితం, *x, *y, channels * పరిమాణం(stbi__uint16));
   }

   తిరిగి (stbi__uint16 *) ఫలితం;
}

#ఒకవేళ !defined(STBI_NO_HDR) || !defined(STBI_NO_రేఖీయ)
స్థిర శూన్య stbi__float_postprocess(దశాంశం *ఫలితం, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   ఒకవేళ (stbi_vertically_flip_on_ఎక్కించు && ఫలితం != ఖాళీ) {
      పూర్ణాంకం channels = req_comp ? req_comp : *comp;
      stbi__vertical_flip(ఫలితం, *x, *y, channels * పరిమాణం(దశాంశం));
   }
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_STDIO

స్థిర FILE *stbi__fopen(అక్షరం స్థిరరాశి *filename, అక్షరం స్థిరరాశి *బహుళకం)
{
   FILE *f;
#ఒకవేళ defined(_MSC_VER) && _MSC_VER >= 1400
   ఒకవేళ (0 != fopen_s(&f, filename, బహుళకం))
      f=0;
#లేకపోతే
   f = fopen(filename, బహుళకం);
#అయితే_ముగింపు
   తిరిగి f;
}


STBIDEF stbi_uc *stbi_ఎక్కించు(అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   గుర్తులేని అక్షరం *ఫలితం;
   ఒకవేళ (!f) తిరిగి stbi__errpuc("can't fopen", "Unable to open file");
   ఫలితం = stbi_ఎక్కించు_from_file(f,x,y,comp,req_comp);
   fclose(f);
   తిరిగి ఫలితం;
}

STBIDEF stbi_uc *stbi_ఎక్కించు_from_file(FILE *f, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   గుర్తులేని అక్షరం *ఫలితం;
   stbi__context s;
   stbi_ప్రారంభం_file(&s,f);
   ఫలితం = stbi_ఎక్కించు_and_postprocess_8bit(&s,x,y,comp,req_comp);
   ఒకవేళ (ఫలితం) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (పూర్ణాంకం) (s.img_బఫరు_చివర - s.img_బఫరు), SEEK_CUR);
   }
   తిరిగి ఫలితం;
}

STBIDEF stbi__uint16 *stbi_ఎక్కించు_from_file_16(FILE *f, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi__uint16 *ఫలితం;
   stbi__context s;
   stbi_ప్రారంభం_file(&s,f);
   ఫలితం = stbi_ఎక్కించు_and_postprocess_16bit(&s,x,y,comp,req_comp);
   ఒకవేళ (ఫలితం) {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (పూర్ణాంకం) (s.img_బఫరు_చివర - s.img_బఫరు), SEEK_CUR);
   }
   తిరిగి ఫలితం;
}

STBIDEF stbi_us *stbi_ఎక్కించు_16(అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   FILE *f = stbi__fopen(filename, "rb");
   stbi__uint16 *ఫలితం;
   ఒకవేళ (!f) తిరిగి (stbi_us *) stbi__errpuc("can't fopen", "Unable to open file");
   ఫలితం = stbi_ఎక్కించు_from_file_16(f,x,y,comp,req_comp);
   fclose(f);
   తిరిగి ఫలితం;
}


#అయితే_ముగింపు //!STBI_NO_STDIO

STBIDEF stbi_us *stbi_ఎక్కించు_16_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels)
{
   stbi__context s;
   stbi_ప్రారంభం_mem(&s,బఫరు,len);
   తిరిగి stbi_ఎక్కించు_and_postprocess_16bit(&s,x,y,channels_లోపల_file,desired_channels);
}

STBIDEF stbi_us *stbi_ఎక్కించు_16_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *channels_లోపల_file, పూర్ణాంకం desired_channels)
{
   stbi__context s;
   stbi_ప్రారంభం_callbacks(&s, (stbi_io_callbacks *)clbk, user);
   తిరిగి stbi_ఎక్కించు_and_postprocess_16bit(&s,x,y,channels_లోపల_file,desired_channels);
}

STBIDEF stbi_uc *stbi_ఎక్కించు_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi__context s;
   stbi_ప్రారంభం_mem(&s,బఫరు,len);
   తిరిగి stbi_ఎక్కించు_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

STBIDEF stbi_uc *stbi_ఎక్కించు_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi__context s;
   stbi_ప్రారంభం_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   తిరిగి stbi_ఎక్కించు_and_postprocess_8bit(&s,x,y,comp,req_comp);
}

#నిర్వచితం_కాకపోతే STBI_NO_GIF
STBIDEF stbi_uc *stbi_ఎక్కించు_gif_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం **delays, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *z, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   గుర్తులేని అక్షరం *ఫలితం;
   stbi__context s; 
   stbi_ప్రారంభం_mem(&s,బఫరు,len); 
   
   ఫలితం = (గుర్తులేని అక్షరం*) stbi_ఎక్కించు_gif_main(&s, delays, x, y, z, comp, req_comp);
   ఒకవేళ (stbi_vertically_flip_on_ఎక్కించు) {
      stbi__vertical_flip_slices( ఫలితం, *x, *y, *z, *comp ); 
   }

   తిరిగి ఫలితం; 
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_రేఖీయ
స్థిర దశాంశం *stbi__loadf_main(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   గుర్తులేని అక్షరం *దత్తాంశం;
   #నిర్వచితం_కాకపోతే STBI_NO_HDR
   ఒకవేళ (stbi__hdr_test(s)) {
      stbi_ఫలితం_info ri;
      దశాంశం *hdr_దత్తాంశం = stbi_hdr_ఎక్కించు(s,x,y,comp,req_comp, &ri);
      ఒకవేళ (hdr_దత్తాంశం)
         stbi__float_postprocess(hdr_దత్తాంశం,x,y,comp,req_comp);
      తిరిగి hdr_దత్తాంశం;
   }
   #అయితే_ముగింపు
   దత్తాంశం = stbi_ఎక్కించు_and_postprocess_8bit(s, x, y, comp, req_comp);
   ఒకవేళ (దత్తాంశం)
      తిరిగి stbi_ldr_గమ్యం_hdr(దత్తాంశం, *x, *y, req_comp ? req_comp : *comp);
   తిరిగి stbi__errpf("unknown image type", "Image not of any known type, or corrupt");
}

STBIDEF దశాంశం *stbi_loadf_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi__context s;
   stbi_ప్రారంభం_mem(&s,బఫరు,len);
   తిరిగి stbi__loadf_main(&s,x,y,comp,req_comp);
}

STBIDEF దశాంశం *stbi_loadf_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi__context s;
   stbi_ప్రారంభం_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   తిరిగి stbi__loadf_main(&s,x,y,comp,req_comp);
}

#నిర్వచితం_కాకపోతే STBI_NO_STDIO
STBIDEF దశాంశం *stbi_loadf(అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   దశాంశం *ఫలితం;
   FILE *f = stbi__fopen(filename, "rb");
   ఒకవేళ (!f) తిరిగి stbi__errpf("can't fopen", "Unable to open file");
   ఫలితం = stbi_loadf_from_file(f,x,y,comp,req_comp);
   fclose(f);
   తిరిగి ఫలితం;
}

STBIDEF దశాంశం *stbi_loadf_from_file(FILE *f, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   stbi__context s;
   stbi_ప్రారంభం_file(&s,f);
   తిరిగి stbi__loadf_main(&s,x,y,comp,req_comp);
}
#అయితే_ముగింపు // !STBI_NO_STDIO

#అయితే_ముగింపు // !STBI_NO_LINEAR

// these is-hdr-or-not is defined independent of whether STBI_NO_LINEAR is
// defined, for API simplicity; if STBI_NO_LINEAR is defined, it always
// reports false!

STBIDEF పూర్ణాంకం stbi_is_hdr_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len)
{
   #నిర్వచితం_కాకపోతే STBI_NO_HDR
   stbi__context s;
   stbi_ప్రారంభం_mem(&s,బఫరు,len);
   తిరిగి stbi__hdr_test(&s);
   #లేకపోతే
   STBI_NOTUSED(బఫరు);
   STBI_NOTUSED(len);
   తిరిగి 0;
   #అయితే_ముగింపు
}

#నిర్వచితం_కాకపోతే STBI_NO_STDIO
STBIDEF పూర్ణాంకం      stbi_is_hdr          (అక్షరం స్థిరరాశి *filename)
{
   FILE *f = stbi__fopen(filename, "rb");
   పూర్ణాంకం ఫలితం=0;
   ఒకవేళ (f) {
      ఫలితం = stbi_is_hdr_from_file(f);
      fclose(f);
   }
   తిరిగి ఫలితం;
}

STBIDEF పూర్ణాంకం stbi_is_hdr_from_file(FILE *f)
{
   #నిర్వచితం_కాకపోతే STBI_NO_HDR
   దీర్ఘ pos = ftell(f);
   పూర్ణాంకం res;
   stbi__context s;
   stbi_ప్రారంభం_file(&s,f);
   res = stbi__hdr_test(&s);
   fseek(f, pos, SEEK_SET);
   తిరిగి res;
   #లేకపోతే
   STBI_NOTUSED(f);
   తిరిగి 0;
   #అయితే_ముగింపు
}
#అయితే_ముగింపు // !STBI_NO_STDIO

STBIDEF పూర్ణాంకం      stbi_is_hdr_from_callbacks(stbi_io_callbacks స్థిరరాశి *clbk, శూన్య *user)
{
   #నిర్వచితం_కాకపోతే STBI_NO_HDR
   stbi__context s;
   stbi_ప్రారంభం_callbacks(&s, (stbi_io_callbacks *) clbk, user);
   తిరిగి stbi__hdr_test(&s);
   #లేకపోతే
   STBI_NOTUSED(clbk);
   STBI_NOTUSED(user);
   తిరిగి 0;
   #అయితే_ముగింపు
}

#నిర్వచితం_కాకపోతే STBI_NO_రేఖీయ
స్థిర దశాంశం stbi__l2h_gamma=2.2f, stbi__l2h_scale=1.0f;

STBIDEF శూన్య   stbi_ldr_గమ్యం_hdr_gamma(దశాంశం gamma) { stbi__l2h_gamma = gamma; }
STBIDEF శూన్య   stbi_ldr_గమ్యం_hdr_scale(దశాంశం scale) { stbi__l2h_scale = scale; }
#అయితే_ముగింపు

స్థిర దశాంశం stbi__h2l_gamma_i=1.0f/2.2f, stbi__h2l_scale_i=1.0f;

STBIDEF శూన్య   stbi_hdr_గమ్యం_ldr_gamma(దశాంశం gamma) { stbi__h2l_gamma_i = 1/gamma; }
STBIDEF శూన్య   stbi_hdr_గమ్యం_ldr_scale(దశాంశం scale) { stbi__h2l_scale_i = 1/scale; }


//////////////////////////////////////////////////////////////////////////////
//
// Common code used by all image loaders
//

గణన
{
   STBI_పరిశీలన_ఎక్కించు=0,
   STBI_పరిశీలన_type,
   STBI_పరిశీలన_శీర్షిక
};

స్థిర శూన్య stbi_refill_బఫరు(stbi__context *s)
{
   పూర్ణాంకం n = (s->io.పఠ)(s->io_user_దత్తాంశం,(అక్షరం*)s->బఫరు_ప్రారంభం,s->buflen);
   ఒకవేళ (n == 0) {
      // at end of file, treat same as if from memory, but need to handle case
      // where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file
      s->read_from_callbacks = 0;
      s->img_బఫరు = s->బఫరు_ప్రారంభం;
      s->img_బఫరు_చివర = s->బఫరు_ప్రారంభం+1;
      *s->img_బఫరు = 0;
   } లేకపోతే {
      s->img_బఫరు = s->బఫరు_ప్రారంభం;
      s->img_బఫరు_చివర = s->బఫరు_ప్రారంభం + n;
   }
}

stbi_inline స్థిర stbi_uc stbi__get8(stbi__context *s)
{
   ఒకవేళ (s->img_బఫరు < s->img_బఫరు_చివర)
      తిరిగి *s->img_బఫరు++;
   ఒకవేళ (s->read_from_callbacks) {
      stbi_refill_బఫరు(s);
      తిరిగి *s->img_బఫరు++;
   }
   తిరిగి 0;
}

stbi_inline స్థిర పూర్ణాంకం stbi__at_eof(stbi__context *s)
{
   ఒకవేళ (s->io.పఠ) {
      ఒకవేళ (!(s->io.eof)(s->io_user_దత్తాంశం)) తిరిగి 0;
      // if feof() is true, check if buffer = end
      // special case: we've only got the special 0 character at the end
      ఒకవేళ (s->read_from_callbacks == 0) తిరిగి 1;
   }

   తిరిగి s->img_బఫరు >= s->img_బఫరు_చివర;
}

స్థిర శూన్య stbi__skip(stbi__context *s, పూర్ణాంకం n)
{
   ఒకవేళ (n < 0) {
      s->img_బఫరు = s->img_బఫరు_చివర;
      తిరిగి;
   }
   ఒకవేళ (s->io.పఠ) {
      పూర్ణాంకం blen = (పూర్ణాంకం) (s->img_బఫరు_చివర - s->img_బఫరు);
      ఒకవేళ (blen < n) {
         s->img_బఫరు = s->img_బఫరు_చివర;
         (s->io.skip)(s->io_user_దత్తాంశం, n - blen);
         తిరిగి;
      }
   }
   s->img_బఫరు += n;
}

స్థిర పూర్ణాంకం stbi__getn(stbi__context *s, stbi_uc *బఫరు, పూర్ణాంకం n)
{
   ఒకవేళ (s->io.పఠ) {
      పూర్ణాంకం blen = (పూర్ణాంకం) (s->img_బఫరు_చివర - s->img_బఫరు);
      ఒకవేళ (blen < n) {
         పూర్ణాంకం res, లెక్క;

         జ్ఞాపకం_నకలు(బఫరు, s->img_బఫరు, blen);

         లెక్క = (s->io.పఠ)(s->io_user_దత్తాంశం, (అక్షరం*) బఫరు + blen, n - blen);
         res = (లెక్క == (n-blen));
         s->img_బఫరు = s->img_బఫరు_చివర;
         తిరిగి res;
      }
   }

   ఒకవేళ (s->img_బఫరు+n <= s->img_బఫరు_చివర) {
      జ్ఞాపకం_నకలు(బఫరు, s->img_బఫరు, n);
      s->img_బఫరు += n;
      తిరిగి 1;
   } లేకపోతే
      తిరిగి 0;
}

స్థిర పూర్ణాంకం stbi__get16be(stbi__context *s)
{
   పూర్ణాంకం z = stbi__get8(s);
   తిరిగి (z << 8) + stbi__get8(s);
}

స్థిర stbi__uint32 stbi__get32be(stbi__context *s)
{
   stbi__uint32 z = stbi__get16be(s);
   తిరిగి (z << 16) + stbi__get16be(s);
}

#ఒకవేళ defined(STBI_NO_BMP) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF)
// nothing
#లేకపోతే
స్థిర పూర్ణాంకం stbi__get16le(stbi__context *s)
{
   పూర్ణాంకం z = stbi__get8(s);
   తిరిగి z + (stbi__get8(s) << 8);
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_BMP
స్థిర stbi__uint32 stbi__get32le(stbi__context *s)
{
   stbi__uint32 z = stbi__get16le(s);
   తిరిగి z + (stbi__get16le(s) << 16);
}
#అయితే_ముగింపు

#నిర్వచించు STBI__BYTECAST(x)  ((stbi_uc) ((x) & 255))  // truncate int to byte without warnings


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

స్థిర stbi_uc stbi__compute_y(పూర్ణాంకం r, పూర్ణాంకం g, పూర్ణాంకం b)
{
   తిరిగి (stbi_uc) (((r*77) + (g*150) +  (29*b)) >> 8);
}

స్థిర గుర్తులేని అక్షరం *stbi__convert_format(గుర్తులేని అక్షరం *దత్తాంశం, పూర్ణాంకం img_n, పూర్ణాంకం req_comp, గుర్తులేని పూర్ణాంకం x, గుర్తులేని పూర్ణాంకం y)
{
   పూర్ణాంకం i,j;
   గుర్తులేని అక్షరం *good;

   ఒకవేళ (req_comp == img_n) తిరిగి దత్తాంశం;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (గుర్తులేని అక్షరం *) stbi__malloc_mad3(req_comp, x, y, 0);
   ఒకవేళ (good == ఖాళీ) {
      STBI_FREE(దత్తాంశం);
      తిరిగి stbi__errpuc("outofmem", "Out of memory");
   }

   కొరకు (j=0; j < (పూర్ణాంకం) y; ++j) {
      గుర్తులేని అక్షరం *src  = దత్తాంశం + j * x * img_n   ;
      గుర్తులేని అక్షరం *dest = good + j * x * req_comp;

      #నిర్వచించు STBI__COMBO(a,b)  ((a)*8+(b))
      #నిర్వచించు STBI__CASE(a,b)   స్థితి STBI__COMBO(a,b): కొరకు(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      ఎంపిక (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=255;                                     } ఆపు;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } ఆపు;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=255;                     } ఆపు;
         STBI__CASE(2,1) { dest[0]=src[0];                                                  } ఆపు;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                  } ఆపు;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                  } ఆపు;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255;        } ఆపు;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } ఆపు;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = 255;    } ఆపు;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y(src[0],src[1],src[2]);                   } ఆపు;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y(src[0],src[1],src[2]), dest[1] = src[3]; } ఆపు;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                    } ఆపు;
         అప్రమేయ: STBI_ASSERT(0);
      }
      #నిర్వచనం_రద్దు STBI__CASE
   }

   STBI_FREE(దత్తాంశం);
   తిరిగి good;
}

స్థిర stbi__uint16 stbi__compute_y_16(పూర్ణాంకం r, పూర్ణాంకం g, పూర్ణాంకం b)
{
   తిరిగి (stbi__uint16) (((r*77) + (g*150) +  (29*b)) >> 8);
}

స్థిర stbi__uint16 *stbi__convert_format16(stbi__uint16 *దత్తాంశం, పూర్ణాంకం img_n, పూర్ణాంకం req_comp, గుర్తులేని పూర్ణాంకం x, గుర్తులేని పూర్ణాంకం y)
{
   పూర్ణాంకం i,j;
   stbi__uint16 *good;

   ఒకవేళ (req_comp == img_n) తిరిగి దత్తాంశం;
   STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (stbi__uint16 *) stbi__malloc(req_comp * x * y * 2);
   ఒకవేళ (good == ఖాళీ) {
      STBI_FREE(దత్తాంశం);
      తిరిగి (stbi__uint16 *) stbi__errpuc("outofmem", "Out of memory");
   }

   కొరకు (j=0; j < (పూర్ణాంకం) y; ++j) {
      stbi__uint16 *src  = దత్తాంశం + j * x * img_n   ;
      stbi__uint16 *dest = good + j * x * req_comp;

      #నిర్వచించు STBI__COMBO(a,b)  ((a)*8+(b))
      #నిర్వచించు STBI__CASE(a,b)   స్థితి STBI__COMBO(a,b): కొరకు(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      ఎంపిక (STBI__COMBO(img_n, req_comp)) {
         STBI__CASE(1,2) { dest[0]=src[0], dest[1]=0xffff;                                     } ఆపు;
         STBI__CASE(1,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } ఆపు;
         STBI__CASE(1,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=0xffff;                     } ఆపు;
         STBI__CASE(2,1) { dest[0]=src[0];                                                     } ఆపు;
         STBI__CASE(2,3) { dest[0]=dest[1]=dest[2]=src[0];                                     } ఆపు;
         STBI__CASE(2,4) { dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                     } ఆపు;
         STBI__CASE(3,4) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=0xffff;        } ఆపు;
         STBI__CASE(3,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } ఆపు;
         STBI__CASE(3,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = 0xffff; } ఆపు;
         STBI__CASE(4,1) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]);                   } ఆపు;
         STBI__CASE(4,2) { dest[0]=stbi__compute_y_16(src[0],src[1],src[2]), dest[1] = src[3]; } ఆపు;
         STBI__CASE(4,3) { dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                       } ఆపు;
         అప్రమేయ: STBI_ASSERT(0);
      }
      #నిర్వచనం_రద్దు STBI__CASE
   }

   STBI_FREE(దత్తాంశం);
   తిరిగి good;
}

#నిర్వచితం_కాకపోతే STBI_NO_రేఖీయ
స్థిర దశాంశం   *stbi_ldr_గమ్యం_hdr(stbi_uc *దత్తాంశం, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం comp)
{
   పూర్ణాంకం i,k,n;
   దశాంశం *output;
   ఒకవేళ (!దత్తాంశం) తిరిగి ఖాళీ;
   output = (దశాంశం *) stbi__malloc_mad4(x, y, comp, పరిమాణం(దశాంశం), 0);
   ఒకవేళ (output == ఖాళీ) { STBI_FREE(దత్తాంశం); తిరిగి stbi__errpf("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   ఒకవేళ (comp & 1) n = comp; లేకపోతే n = comp-1;
   కొరకు (i=0; i < x*y; ++i) {
      కొరకు (k=0; k < n; ++k) {
         output[i*comp + k] = (దశాంశం) (ఘాతం(దత్తాంశం[i*comp+k]/255.0f, stbi__l2h_gamma) * stbi__l2h_scale);
      }
      ఒకవేళ (k < comp) output[i*comp + k] = దత్తాంశం[i*comp+k]/255.0f;
   }
   STBI_FREE(దత్తాంశం);
   తిరిగి output;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_HDR
#నిర్వచించు stbi__float2int(x)   ((పూర్ణాంకం) (x))
స్థిర stbi_uc *stbi_hdr_గమ్యం_ldr(దశాంశం   *దత్తాంశం, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం comp)
{
   పూర్ణాంకం i,k,n;
   stbi_uc *output;
   ఒకవేళ (!దత్తాంశం) తిరిగి ఖాళీ;
   output = (stbi_uc *) stbi__malloc_mad3(x, y, comp, 0);
   ఒకవేళ (output == ఖాళీ) { STBI_FREE(దత్తాంశం); తిరిగి stbi__errpuc("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   ఒకవేళ (comp & 1) n = comp; లేకపోతే n = comp-1;
   కొరకు (i=0; i < x*y; ++i) {
      కొరకు (k=0; k < n; ++k) {
         దశాంశం z = (దశాంశం) ఘాతం(దత్తాంశం[i*comp+k]*stbi__h2l_scale_i, stbi__h2l_gamma_i) * 255 + 0.5f;
         ఒకవేళ (z < 0) z = 0;
         ఒకవేళ (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
      ఒకవేళ (k < comp) {
         దశాంశం z = దత్తాంశం[i*comp+k] * 255 + 0.5f;
         ఒకవేళ (z < 0) z = 0;
         ఒకవేళ (z > 255) z = 255;
         output[i*comp + k] = (stbi_uc) stbi__float2int(z);
      }
   }
   STBI_FREE(దత్తాంశం);
   తిరిగి output;
}
#అయితే_ముగింపు

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

#నిర్వచితం_కాకపోతే STBI_NO_JPEG

// huffman decoding acceleration
#నిర్వచించు FAST_బిట్లు   9  // larger handles more cases; smaller stomps less cache

రకనిర్వచనం నిర్మాణం
{
   stbi_uc  fast[1 << FAST_బిట్లు];
   // weirdly, repacking this into AoS is a 10% speed loss, instead of a win
   stbi__uint16 సంకేతం_2[256];
   stbi_uc  విలువలు[256];
   stbi_uc  కొలత[257];
   గుర్తులేని పూర్ణాంకం maxcode[18];
   పూర్ణాంకం    delta[17];   // old 'firstsymbol' - old 'firstcode'
} stbi__huffman;

రకనిర్వచనం నిర్మాణం
{
   stbi__context *s;
   stbi__huffman huff_dc[4];
   stbi__huffman huff_ac[4];
   stbi__uint16 dequant[4][64];
   stbi__int16 fast_ac[4][1 << FAST_బిట్లు];

// sizes for components, interleaved MCUs
   పూర్ణాంకం img_h_max, img_v_max;
   పూర్ణాంకం img_mcu_x, img_mcu_y;
   పూర్ణాంకం img_mcu_w, img_mcu_h;

// definition of jpeg image component
   నిర్మాణం
   {
      పూర్ణాంకం id;
      పూర్ణాంకం h,v;
      పూర్ణాంకం tq;
      పూర్ణాంకం hd,ha;
      పూర్ణాంకం dc_pred;

      పూర్ణాంకం x,y,w2,h2;
      stbi_uc *దత్తాంశం;
      శూన్య *raw_దత్తాంశం, *raw_coeff;
      stbi_uc *linebuf;
      పొట్టి   *coeff;   // progressive only
      పూర్ణాంకం      coeff_w, coeff_h; // number of 8x8 coefficient blocks
   } img_comp[4];

   stbi__uint32   సంకేతం_బఫరు; // jpeg entropy-coded buffer
   పూర్ణాంకం            సంకేతం_బిట్లు;   // number of valid bits
   గుర్తులేని అక్షరం  marker;      // marker seen while filling entropy buffer
   పూర్ణాంకం            nomore;      // flag if we saw a marker so must stop

   పూర్ణాంకం            progressive;
   పూర్ణాంకం            spec_ప్రారంభం;
   పూర్ణాంకం            spec_చివర;
   పూర్ణాంకం            succ_ఎగువ;
   పూర్ణాంకం            succ_దిగువ;
   పూర్ణాంకం            eob_నిరంతరం;
   పూర్ణాంకం            jfif;
   పూర్ణాంకం            app14_color_transform; // Adobe APP14 tag
   పూర్ణాంకం            rgb;

   పూర్ణాంకం పరిశీలన_n, క్రమం[4];
   పూర్ణాంకం restart_interval, todo;

// kernels
   శూన్య (*idct_ఖండం_kernel)(stbi_uc *బహిర్గతం, పూర్ణాంకం బహిర్గతం_stride, పొట్టి దత్తాంశం[64]);
   శూన్య (*YCb_Cr_గమ్యం_RGB_kernel)(stbi_uc *బహిర్గతం, స్థిరరాశి stbi_uc *y, స్థిరరాశి stbi_uc *pcb, స్థిరరాశి stbi_uc *pcr, పూర్ణాంకం లెక్క, పూర్ణాంకం అడుగు);
   stbi_uc *(*resample_అడ్డువరుస_hv_2_kernel)(stbi_uc *బహిర్గతం, stbi_uc *లోపల_near, stbi_uc *లోపల_far, పూర్ణాంకం w, పూర్ణాంకం hs);
} stbi__jpeg;

స్థిర పూర్ణాంకం stbi__build_huffman(stbi__huffman *h, పూర్ణాంకం *లెక్క)
{
   పూర్ణాంకం i,j,k=0;
   గుర్తులేని పూర్ణాంకం సంకేతం_2;
   // build size list for each symbol (from JPEG spec)
   కొరకు (i=0; i < 16; ++i)
      కొరకు (j=0; j < లెక్క[i]; ++j)
         h->కొలత[k++] = (stbi_uc) (i+1);
   h->కొలత[k] = 0;

   // compute actual symbols (from jpeg spec)
   సంకేతం_2 = 0;
   k = 0;
   కొరకు(j=1; j <= 16; ++j) {
      // compute delta to add to code to compute symbol id
      h->delta[j] = k - సంకేతం_2;
      ఒకవేళ (h->కొలత[k] == j) {
         అయితే (h->కొలత[k] == j)
            h->సంకేతం_2[k++] = (stbi__uint16) (సంకేతం_2++);
         ఒకవేళ (సంకేతం_2-1 >= (1u << j)) తిరిగి stbi__err("bad code lengths","Corrupt JPEG");
      }
      // compute largest code + 1 for this size, preshifted as needed later
      h->maxcode[j] = సంకేతం_2 << (16-j);
      సంకేతం_2 <<= 1;
   }
   h->maxcode[j] = 0xffffffff;

   // build non-spec acceleration table; 255 is flag for not-accelerated
   memset(h->fast, 255, 1 << FAST_బిట్లు);
   కొరకు (i=0; i < k; ++i) {
      పూర్ణాంకం s = h->కొలత[i];
      ఒకవేళ (s <= FAST_బిట్లు) {
         పూర్ణాంకం c = h->సంకేతం_2[i] << (FAST_బిట్లు-s);
         పూర్ణాంకం m = 1 << (FAST_బిట్లు-s);
         కొరకు (j=0; j < m; ++j) {
            h->fast[c+j] = (stbi_uc) i;
         }
      }
   }
   తిరిగి 1;
}

// build a table that decodes both magnitude and value of small ACs in
// one go.
స్థిర శూన్య stbi__build_fast_ac(stbi__int16 *fast_ac, stbi__huffman *h)
{
   పూర్ణాంకం i;
   కొరకు (i=0; i < (1 << FAST_బిట్లు); ++i) {
      stbi_uc fast = h->fast[i];
      fast_ac[i] = 0;
      ఒకవేళ (fast < 255) {
         పూర్ణాంకం rs = h->విలువలు[fast];
         పూర్ణాంకం నిరంతరం = (rs >> 4) & 15;
         పూర్ణాంకం magbits = rs & 15;
         పూర్ణాంకం len = h->కొలత[fast];

         ఒకవేళ (magbits && len + magbits <= FAST_బిట్లు) {
            // magnitude code followed by receive_extend code
            పూర్ణాంకం k = ((i << len) & ((1 << FAST_బిట్లు) - 1)) >> (FAST_బిట్లు - magbits);
            పూర్ణాంకం m = 1 << (magbits - 1);
            ఒకవేళ (k < m) k += (~0U << magbits) + 1;
            // if the result is small enough, we can fit it in fast_ac table
            ఒకవేళ (k >= -128 && k <= 127)
               fast_ac[i] = (stbi__int16) ((k * 256) + (నిరంతరం * 16) + (len + magbits));
         }
      }
   }
}

స్థిర శూన్య stbi_grow_బఫరు_unsafe(stbi__jpeg *j)
{
   చేయి {
      గుర్తులేని పూర్ణాంకం b = j->nomore ? 0 : stbi__get8(j->s);
      ఒకవేళ (b == 0xff) {
         పూర్ణాంకం c = stbi__get8(j->s);
         అయితే (c == 0xff) c = stbi__get8(j->s); // consume fill bytes
         ఒకవేళ (c != 0) {
            j->marker = (గుర్తులేని అక్షరం) c;
            j->nomore = 1;
            తిరిగి;
         }
      }
      j->సంకేతం_బఫరు |= b << (24 - j->సంకేతం_బిట్లు);
      j->సంకేతం_బిట్లు += 8;
   } అయితే (j->సంకేతం_బిట్లు <= 24);
}

// (1 << n) - 1
స్థిర స్థిరరాశి stbi__uint32 stbi__bmask[17]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};

// decode a jpeg huffman value from the bitstream
stbi_inline స్థిర పూర్ణాంకం stbi__jpeg_huff_decode(stbi__jpeg *j, stbi__huffman *h)
{
   గుర్తులేని పూర్ణాంకం తాత్కాలిక;
   పూర్ణాంకం c,k;

   ఒకవేళ (j->సంకేతం_బిట్లు < 16) stbi_grow_బఫరు_unsafe(j);

   // look at the top FAST_BITS and determine what symbol ID it is,
   // if the code is <= FAST_BITS
   c = (j->సంకేతం_బఫరు >> (32 - FAST_బిట్లు)) & ((1 << FAST_బిట్లు)-1);
   k = h->fast[c];
   ఒకవేళ (k < 255) {
      పూర్ణాంకం s = h->కొలత[k];
      ఒకవేళ (s > j->సంకేతం_బిట్లు)
         తిరిగి -1;
      j->సంకేతం_బఫరు <<= s;
      j->సంకేతం_బిట్లు -= s;
      తిరిగి h->విలువలు[k];
   }

   // naive test is to shift the code_buffer down so k bits are
   // valid, then test against maxcode. To speed this up, we've
   // preshifted maxcode left so that it has (16-k) 0s at the
   // end; in other words, regardless of the number of bits, it
   // wants to be compared against something shifted to have 16;
   // that way we don't need to shift inside the loop.
   తాత్కాలిక = j->సంకేతం_బఫరు >> 16;
   కొరకు (k=FAST_బిట్లు+1 ; ; ++k)
      ఒకవేళ (తాత్కాలిక < h->maxcode[k])
         ఆపు;
   ఒకవేళ (k == 17) {
      // error! code not found
      j->సంకేతం_బిట్లు -= 16;
      తిరిగి -1;
   }

   ఒకవేళ (k > j->సంకేతం_బిట్లు)
      తిరిగి -1;

   // convert the huffman code to the symbol id
   c = ((j->సంకేతం_బఫరు >> (32 - k)) & stbi__bmask[k]) + h->delta[k];
   STBI_ASSERT((((j->సంకేతం_బఫరు) >> (32 - h->కొలత[c])) & stbi__bmask[h->కొలత[c]]) == h->సంకేతం_2[c]);

   // convert the id to a symbol
   j->సంకేతం_బిట్లు -= k;
   j->సంకేతం_బఫరు <<= k;
   తిరిగి h->విలువలు[c];
}

// bias[n] = (-1<<n) + 1
స్థిర స్థిరరాశి పూర్ణాంకం stbi__jbias[16] = {0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767};

// combined JPEG 'receive' and JPEG 'extend', since baseline
// always extends everything it receives.
stbi_inline స్థిర పూర్ణాంకం stbi__extend_receive(stbi__jpeg *j, పూర్ణాంకం n)
{
   గుర్తులేని పూర్ణాంకం k;
   పూర్ణాంకం sgn;
   ఒకవేళ (j->సంకేతం_బిట్లు < n) stbi_grow_బఫరు_unsafe(j);

   sgn = (stbi__int32)j->సంకేతం_బఫరు >> 31; // sign bit is always in MSB
   k = stbi_lrot(j->సంకేతం_బఫరు, n);
   STBI_ASSERT(n >= 0 && n < (పూర్ణాంకం) (పరిమాణం(stbi__bmask)/పరిమాణం(*stbi__bmask)));
   j->సంకేతం_బఫరు = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->సంకేతం_బిట్లు -= n;
   తిరిగి k + (stbi__jbias[n] & ~sgn);
}

// get some unsigned bits
stbi_inline స్థిర పూర్ణాంకం stbi_jpeg_get_బిట్లు(stbi__jpeg *j, పూర్ణాంకం n)
{
   గుర్తులేని పూర్ణాంకం k;
   ఒకవేళ (j->సంకేతం_బిట్లు < n) stbi_grow_బఫరు_unsafe(j);
   k = stbi_lrot(j->సంకేతం_బఫరు, n);
   j->సంకేతం_బఫరు = k & ~stbi__bmask[n];
   k &= stbi__bmask[n];
   j->సంకేతం_బిట్లు -= n;
   తిరిగి k;
}

stbi_inline స్థిర పూర్ణాంకం stbi_jpeg_get_బిట్టు(stbi__jpeg *j)
{
   గుర్తులేని పూర్ణాంకం k;
   ఒకవేళ (j->సంకేతం_బిట్లు < 1) stbi_grow_బఫరు_unsafe(j);
   k = j->సంకేతం_బఫరు;
   j->సంకేతం_బఫరు <<= 1;
   --j->సంకేతం_బిట్లు;
   తిరిగి k & 0x80000000;
}

// given a value that's at position X in the zigzag stream,
// where does it appear in the 8x8 matrix coded as row-major?
స్థిర స్థిరరాశి stbi_uc stbi__jpeg_dezigzag[64+15] =
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
స్థిర పూర్ణాంకం stbi_jpeg_decode_ఖండం(stbi__jpeg *j, పొట్టి దత్తాంశం[64], stbi__huffman *hdc, stbi__huffman *hac, stbi__int16 *fac, పూర్ణాంకం b, stbi__uint16 *dequant)
{
   పూర్ణాంకం diff,dc,k;
   పూర్ణాంకం t;

   ఒకవేళ (j->సంకేతం_బిట్లు < 16) stbi_grow_బఫరు_unsafe(j);
   t = stbi__jpeg_huff_decode(j, hdc);
   ఒకవేళ (t < 0) తిరిగి stbi__err("bad huffman code","Corrupt JPEG");

   // 0 all the ac values now so we can do it 32-bits at a time
   memset(దత్తాంశం,0,64*పరిమాణం(దత్తాంశం[0]));

   diff = t ? stbi__extend_receive(j, t) : 0;
   dc = j->img_comp[b].dc_pred + diff;
   j->img_comp[b].dc_pred = dc;
   దత్తాంశం[0] = (పొట్టి) (dc * dequant[0]);

   // decode AC components, see JPEG spec
   k = 1;
   చేయి {
      గుర్తులేని పూర్ణాంకం zig;
      పూర్ణాంకం c,r,s;
      ఒకవేళ (j->సంకేతం_బిట్లు < 16) stbi_grow_బఫరు_unsafe(j);
      c = (j->సంకేతం_బఫరు >> (32 - FAST_బిట్లు)) & ((1 << FAST_బిట్లు)-1);
      r = fac[c];
      ఒకవేళ (r) { // fast-AC path
         k += (r >> 4) & 15; //నిరంతరం
         s = r & 15; // combined length
         j->సంకేతం_బఫరు <<= s;
         j->సంకేతం_బిట్లు -= s;
         // decode into unzigzag'd location
         zig = stbi__jpeg_dezigzag[k++];
         దత్తాంశం[zig] = (పొట్టి) ((r >> 8) * dequant[zig]);
      } లేకపోతే {
         పూర్ణాంకం rs = stbi__jpeg_huff_decode(j, hac);
         ఒకవేళ (rs < 0) తిరిగి stbi__err("bad huffman code","Corrupt JPEG");
         s = rs & 15;
         r = rs >> 4;
         ఒకవేళ (s == 0) {
            ఒకవేళ (rs != 0xf0) ఆపు; // end block
            k += 16;
         } లేకపోతే {
            k += r;
            // decode into unzigzag'd location
            zig = stbi__jpeg_dezigzag[k++];
            దత్తాంశం[zig] = (పొట్టి) (stbi__extend_receive(j,s) * dequant[zig]);
         }
      }
   } అయితే (k < 64);
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi_jpeg_decode_ఖండం_prog_dc(stbi__jpeg *j, పొట్టి దత్తాంశం[64], stbi__huffman *hdc, పూర్ణాంకం b)
{
   పూర్ణాంకం diff,dc;
   పూర్ణాంకం t;
   ఒకవేళ (j->spec_చివర != 0) తిరిగి stbi__err("can't merge dc and ac", "Corrupt JPEG");

   ఒకవేళ (j->సంకేతం_బిట్లు < 16) stbi_grow_బఫరు_unsafe(j);

   ఒకవేళ (j->succ_ఎగువ == 0) {
      // first scan for DC coefficient, must be first
      memset(దత్తాంశం,0,64*పరిమాణం(దత్తాంశం[0])); // 0 all the ac values now
      t = stbi__jpeg_huff_decode(j, hdc);
      diff = t ? stbi__extend_receive(j, t) : 0;

      dc = j->img_comp[b].dc_pred + diff;
      j->img_comp[b].dc_pred = dc;
      దత్తాంశం[0] = (పొట్టి) (dc << j->succ_దిగువ);
   } లేకపోతే {
      // refinement scan for DC coefficient
      ఒకవేళ (stbi_jpeg_get_బిట్టు(j))
         దత్తాంశం[0] += (పొట్టి) (1 << j->succ_దిగువ);
   }
   తిరిగి 1;
}

// @OPTIMIZE: store non-zigzagged during the decode passes,
// and only de-zigzag when dequantizing
స్థిర పూర్ణాంకం stbi_jpeg_decode_ఖండం_prog_ac(stbi__jpeg *j, పొట్టి దత్తాంశం[64], stbi__huffman *hac, stbi__int16 *fac)
{
   పూర్ణాంకం k;
   ఒకవేళ (j->spec_ప్రారంభం == 0) తిరిగి stbi__err("can't merge dc and ac", "Corrupt JPEG");

   ఒకవేళ (j->succ_ఎగువ == 0) {
      పూర్ణాంకం జరుపు = j->succ_దిగువ;

      ఒకవేళ (j->eob_నిరంతరం) {
         --j->eob_నిరంతరం;
         తిరిగి 1;
      }

      k = j->spec_ప్రారంభం;
      చేయి {
         గుర్తులేని పూర్ణాంకం zig;
         పూర్ణాంకం c,r,s;
         ఒకవేళ (j->సంకేతం_బిట్లు < 16) stbi_grow_బఫరు_unsafe(j);
         c = (j->సంకేతం_బఫరు >> (32 - FAST_బిట్లు)) & ((1 << FAST_బిట్లు)-1);
         r = fac[c];
         ఒకవేళ (r) { // fast-AC path
            k += (r >> 4) & 15; //నిరంతరం
            s = r & 15; // combined length
            j->సంకేతం_బఫరు <<= s;
            j->సంకేతం_బిట్లు -= s;
            zig = stbi__jpeg_dezigzag[k++];
            దత్తాంశం[zig] = (పొట్టి) ((r >> 8) << జరుపు);
         } లేకపోతే {
            పూర్ణాంకం rs = stbi__jpeg_huff_decode(j, hac);
            ఒకవేళ (rs < 0) తిరిగి stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            ఒకవేళ (s == 0) {
               ఒకవేళ (r < 15) {
                  j->eob_నిరంతరం = (1 << r);
                  ఒకవేళ (r)
                     j->eob_నిరంతరం += stbi_jpeg_get_బిట్లు(j, r);
                  --j->eob_నిరంతరం;
                  ఆపు;
               }
               k += 16;
            } లేకపోతే {
               k += r;
               zig = stbi__jpeg_dezigzag[k++];
               దత్తాంశం[zig] = (పొట్టి) (stbi__extend_receive(j,s) << జరుపు);
            }
         }
      } అయితే (k <= j->spec_చివర);
   } లేకపోతే {
      // refinement scan for these AC coefficients

      పొట్టి బిట్టు = (పొట్టి) (1 << j->succ_దిగువ);

      ఒకవేళ (j->eob_నిరంతరం) {
         --j->eob_నిరంతరం;
         కొరకు (k = j->spec_ప్రారంభం; k <= j->spec_చివర; ++k) {
            పొట్టి *p = &దత్తాంశం[stbi__jpeg_dezigzag[k]];
            ఒకవేళ (*p != 0)
               ఒకవేళ (stbi_jpeg_get_బిట్టు(j))
                  ఒకవేళ ((*p & బిట్టు)==0) {
                     ఒకవేళ (*p > 0)
                        *p += బిట్టు;
                     లేకపోతే
                        *p -= బిట్టు;
                  }
         }
      } లేకపోతే {
         k = j->spec_ప్రారంభం;
         చేయి {
            పూర్ణాంకం r,s;
            పూర్ణాంకం rs = stbi__jpeg_huff_decode(j, hac); // @OPTIMIZE see if we can use the fast path here, advance-by-r is so slow, eh
            ఒకవేళ (rs < 0) తిరిగి stbi__err("bad huffman code","Corrupt JPEG");
            s = rs & 15;
            r = rs >> 4;
            ఒకవేళ (s == 0) {
               ఒకవేళ (r < 15) {
                  j->eob_నిరంతరం = (1 << r) - 1;
                  ఒకవేళ (r)
                     j->eob_నిరంతరం += stbi_jpeg_get_బిట్లు(j, r);
                  r = 64; // force end of block
               } లేకపోతే {
                  // r=15 s=0 should write 16 0s, so we just do
                  // a run of 15 0s and then write s (which is 0),
                  // so we don't have to do anything special here
               }
            } లేకపోతే {
               ఒకవేళ (s != 1) తిరిగి stbi__err("bad huffman code", "Corrupt JPEG");
               // sign bit
               ఒకవేళ (stbi_jpeg_get_బిట్టు(j))
                  s = బిట్టు;
               లేకపోతే
                  s = -బిట్టు;
            }

            // advance by r
            అయితే (k <= j->spec_చివర) {
               పొట్టి *p = &దత్తాంశం[stbi__jpeg_dezigzag[k++]];
               ఒకవేళ (*p != 0) {
                  ఒకవేళ (stbi_jpeg_get_బిట్టు(j))
                     ఒకవేళ ((*p & బిట్టు)==0) {
                        ఒకవేళ (*p > 0)
                           *p += బిట్టు;
                        లేకపోతే
                           *p -= బిట్టు;
                     }
               } లేకపోతే {
                  ఒకవేళ (r == 0) {
                     *p = (పొట్టి) s;
                     ఆపు;
                  }
                  --r;
               }
            }
         } అయితే (k <= j->spec_చివర);
      }
   }
   తిరిగి 1;
}

// take a -128..127 value and stbi__clamp it and convert to 0..255
stbi_inline స్థిర stbi_uc stbi__clamp(పూర్ణాంకం x)
{
   // trick to use a single test to catch both cases
   ఒకవేళ ((గుర్తులేని పూర్ణాంకం) x > 255) {
      ఒకవేళ (x < 0) తిరిగి 0;
      ఒకవేళ (x > 255) తిరిగి 255;
   }
   తిరిగి (stbi_uc) x;
}

#నిర్వచించు stbi__f2f(x)  ((పూర్ణాంకం) (((x) * 4096 + 0.5)))
#నిర్వచించు stbi__fsh(x)  ((x) * 4096)

// derived from jidctint -- DCT_ISLOW
#నిర్వచించు STBI__IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7) \
   పూర్ణాంకం t0,t1,t2,t3,p1,p2,p3,p4,p5,x0,x1,x2,x3; \
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

స్థిర శూన్య stbi_idct_ఖండం(stbi_uc *బహిర్గతం, పూర్ణాంకం బహిర్గతం_stride, పొట్టి దత్తాంశం[64])
{
   పూర్ణాంకం i,val[64],*v=val;
   stbi_uc *o;
   పొట్టి *d = దత్తాంశం;

   // columns
   కొరకు (i=0; i < 8; ++i,++d, ++v) {
      // if all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing
      ఒకవేళ (d[ 8]==0 && d[16]==0 && d[24]==0 && d[32]==0
           && d[40]==0 && d[48]==0 && d[56]==0) {
         //    no shortcut                 0     seconds
         //    (1|2|3|4|5|6|7)==0          0     seconds
         //    all separate               -0.047 seconds
         //    1 && 2|3 && 4|5 && 6|7:    -0.047 seconds
         పూర్ణాంకం dcterm = d[0]*4;
         v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
      } లేకపోతే {
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

   కొరకు (i=0, v=val, o=బహిర్గతం; i < 8; ++i,v+=8,o+=బహిర్గతం_stride) {
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

#నిర్వచితమైతే STBI_SSE2
// sse2 integer IDCT. not the fastest possible implementation but it
// produces bit-identical results to the generic C version so it's
// fully "transparent".
స్థిర శూన్య stbi__idct_simd(stbi_uc *బహిర్గతం, పూర్ణాంకం బహిర్గతం_stride, పొట్టి దత్తాంశం[64])
{
   // This is constructed to match our regular (generic) integer IDCT exactly.
   __m128i row0, row1, row2, row3, row4, row5, row6, row7;
   __m128i tmp;

   // dot product constant: even elems=x, odd elems=y
   #నిర్వచించు dct_const(x,y)  _mm_setr_epi16((x),(y),(x),(y),(x),(y),(x),(y))

   // out(0) = c0[even]*x + c0[odd]*y   (c0, x, y 16-bit, out 32-bit)
   // out(1) = c1[even]*x + c1[odd]*y
   #నిర్వచించు dct_rot(out0,out1, x,y,c0,c1) \
      __m128i c0##lo = _mm_unpacklo_epi16((x),(y)); \
      __m128i c0##hi = _mm_unpackhi_epi16((x),(y)); \
      __m128i out0##_l = _mm_madd_epi16(c0##lo, c0); \
      __m128i out0##_h = _mm_madd_epi16(c0##hi, c0); \
      __m128i out1##_l = _mm_madd_epi16(c0##lo, c1); \
      __m128i out1##_h = _mm_madd_epi16(c0##hi, c1)

   // out = in << 12  (in 16-bit, out 32-bit)
   #నిర్వచించు dct_widen(బహిర్గతం, లోపల) \
      __m128i బహిర్గతం##_l = _mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), (లోపల)), 4); \
      __m128i బహిర్గతం##_h = _mm_srai_epi32(_mm_unpackhi_epi16(_mm_setzero_si128(), (లోపల)), 4)

   // wide add
   #నిర్వచించు dct_wadd(బహిర్గతం, a, b) \
      __m128i బహిర్గతం##_l = mm_కూడు_epi32(a##_l, b##_l); \
      __m128i బహిర్గతం##_h = mm_కూడు_epi32(a##_h, b##_h)

   // wide sub
   #నిర్వచించు dct_wsub(బహిర్గతం, a, b) \
      __m128i బహిర్గతం##_l = _mm_sub_epi32(a##_l, b##_l); \
      __m128i బహిర్గతం##_h = _mm_sub_epi32(a##_h, b##_h)

   // butterfly a/b, add bias, then shift by "s" and pack
   #నిర్వచించు dct_bfly32o(out0, out1, a,b,bias,s) \
      { \
         __m128i abiased_l = mm_కూడు_epi32(a##_l, bias); \
         __m128i abiased_h = mm_కూడు_epi32(a##_h, bias); \
         dct_wadd(కూడిక, abiased, b); \
         dct_wsub(dif, abiased, b); \
         out0 = _mm_packs_epi32(_mm_srai_epi32(కూడిక_l, s), _mm_srai_epi32(కూడిక_h, s)); \
         out1 = _mm_packs_epi32(_mm_srai_epi32(dif_l, s), _mm_srai_epi32(dif_h, s)); \
      }

   // 8-bit interleave step (for transposes)
   #నిర్వచించు dct_interleave8(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi8(a, b); \
      b = _mm_unpackhi_epi8(tmp, b)

   // 16-bit interleave step (for transposes)
   #నిర్వచించు dct_interleave16(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi16(a, b); \
      b = _mm_unpackhi_epi16(tmp, b)

   #నిర్వచించు dct_pass(bias,జరుపు) \
      { \
         /* even part */ \
         dct_rot(t2e,t3e, row2,row6, rot0_0,rot0_1); \
         __m128i sum04 = mm_కూడు_epi16(row0, row4); \
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
         __m128i sum17 = mm_కూడు_epi16(row1, row7); \
         __m128i sum35 = mm_కూడు_epi16(row3, row5); \
         dct_rot(y4o,y5o, sum17,sum35, rot1_0,rot1_1); \
         dct_wadd(x4, y0o, y4o); \
         dct_wadd(x5, y1o, y5o); \
         dct_wadd(x6, y2o, y5o); \
         dct_wadd(x7, y3o, y4o); \
         dct_bfly32o(row0,row7, x0,x7,bias,జరుపు); \
         dct_bfly32o(row1,row6, x1,x6,bias,జరుపు); \
         dct_bfly32o(row2,row5, x2,x5,bias,జరుపు); \
         dct_bfly32o(row3,row4, x3,x4,bias,జరుపు); \
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

   //ఎక్కించు
   row0 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 0*8));
   row1 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 1*8));
   row2 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 2*8));
   row3 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 3*8));
   row4 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 4*8));
   row5 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 5*8));
   row6 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 6*8));
   row7 = mm_ఎక్కించు_si128((స్థిరరాశి __m128i *) (దత్తాంశం + 7*8));

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
      _mm_storel_epi64((__m128i *) బహిర్గతం, p0); బహిర్గతం += బహిర్గతం_stride;
      _mm_storel_epi64((__m128i *) బహిర్గతం, _mm_shuffle_epi32(p0, 0x4e)); బహిర్గతం += బహిర్గతం_stride;
      _mm_storel_epi64((__m128i *) బహిర్గతం, p2); బహిర్గతం += బహిర్గతం_stride;
      _mm_storel_epi64((__m128i *) బహిర్గతం, _mm_shuffle_epi32(p2, 0x4e)); బహిర్గతం += బహిర్గతం_stride;
      _mm_storel_epi64((__m128i *) బహిర్గతం, p1); బహిర్గతం += బహిర్గతం_stride;
      _mm_storel_epi64((__m128i *) బహిర్గతం, _mm_shuffle_epi32(p1, 0x4e)); బహిర్గతం += బహిర్గతం_stride;
      _mm_storel_epi64((__m128i *) బహిర్గతం, p3); బహిర్గతం += బహిర్గతం_stride;
      _mm_storel_epi64((__m128i *) బహిర్గతం, _mm_shuffle_epi32(p3, 0x4e));
   }

#నిర్వచనం_రద్దు dct_const
#నిర్వచనం_రద్దు dct_rot
#నిర్వచనం_రద్దు dct_widen
#నిర్వచనం_రద్దు dct_wadd
#నిర్వచనం_రద్దు dct_wsub
#నిర్వచనం_రద్దు dct_bfly32o
#నిర్వచనం_రద్దు dct_interleave8
#నిర్వచనం_రద్దు dct_interleave16
#నిర్వచనం_రద్దు dct_pass
}

#అయితే_ముగింపు // STBI_SSE2

#నిర్వచితమైతే STBI_NEON

// NEON integer IDCT. should produce bit-identical
// results to the generic C version.
స్థిర శూన్య stbi__idct_simd(stbi_uc *బహిర్గతం, పూర్ణాంకం బహిర్గతం_stride, పొట్టి దత్తాంశం[64])
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

#నిర్వచించు dct_long_mul(బహిర్గతం, inq, coeff) \
   int32x4_t బహిర్గతం##_l = vmull_s16(vget_దిగువ_s16(inq), coeff); \
   int32x4_t బహిర్గతం##_h = vmull_s16(vget_ఎగువ_s16(inq), coeff)

#నిర్వచించు dct_long_mac(బహిర్గతం, acc, inq, coeff) \
   int32x4_t బహిర్గతం##_l = vmlal_s16(acc##_l, vget_దిగువ_s16(inq), coeff); \
   int32x4_t బహిర్గతం##_h = vmlal_s16(acc##_h, vget_ఎగువ_s16(inq), coeff)

#నిర్వచించు dct_widen(బహిర్గతం, inq) \
   int32x4_t బహిర్గతం##_l = vshll_n_s16(vget_దిగువ_s16(inq), 12); \
   int32x4_t బహిర్గతం##_h = vshll_n_s16(vget_ఎగువ_s16(inq), 12)

// wide add
#నిర్వచించు dct_wadd(బహిర్గతం, a, b) \
   int32x4_t బహిర్గతం##_l = vaddq_s32(a##_l, b##_l); \
   int32x4_t బహిర్గతం##_h = vaddq_s32(a##_h, b##_h)

// wide sub
#నిర్వచించు dct_wsub(బహిర్గతం, a, b) \
   int32x4_t బహిర్గతం##_l = vsubq_s32(a##_l, b##_l); \
   int32x4_t బహిర్గతం##_h = vsubq_s32(a##_h, b##_h)

// butterfly a/b, then shift using "shiftop" by "s" and pack
#నిర్వచించు dct_bfly32o(out0,out1, a,b,shiftop,s) \
   { \
      dct_wadd(కూడిక, a, b); \
      dct_wsub(dif, a, b); \
      out0 = vcombine_s16(shiftop(కూడిక_l, s), shiftop(కూడిక_h, s)); \
      out1 = vcombine_s16(shiftop(dif_l, s), shiftop(dif_h, s)); \
   }

#నిర్వచించు dct_pass(shiftop, జరుపు) \
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
      dct_bfly32o(row0,row7, x0,x7,shiftop,జరుపు); \
      dct_bfly32o(row1,row6, x1,x6,shiftop,జరుపు); \
      dct_bfly32o(row2,row5, x2,x5,shiftop,జరుపు); \
      dct_bfly32o(row3,row4, x3,x4,shiftop,జరుపు); \
   }

   //ఎక్కించు
   row0 = vld1q_s16(దత్తాంశం + 0*8);
   row1 = vld1q_s16(దత్తాంశం + 1*8);
   row2 = vld1q_s16(దత్తాంశం + 2*8);
   row3 = vld1q_s16(దత్తాంశం + 3*8);
   row4 = vld1q_s16(దత్తాంశం + 4*8);
   row5 = vld1q_s16(దత్తాంశం + 5*8);
   row6 = vld1q_s16(దత్తాంశం + 6*8);
   row7 = vld1q_s16(దత్తాంశం + 7*8);

   // add DC bias
   row0 = vaddq_s16(row0, vsetq_lane_s16(1024, vdupq_n_s16(0), 0));

   // column pass
   dct_pass(vrshrn_n_s32, 10);

   // 16bit 8x8 transpose
   {
// these three map to a single VTRN.16, VTRN.32, and VSWP, respectively.
// whether compilers actually get this is another story, sadly.
#నిర్వచించు dct_trn16(x, y) { int16x8x2_t t = vtrnq_s16(x, y); x = t.val[0]; y = t.val[1]; }
#నిర్వచించు dct_trn32(x, y) { int32x4x2_t t = vtrnq_s32(vreinterpretq_s32_s16(x), vreinterpretq_s32_s16(y)); x = vreinterpretq_s16_s32(t.val[0]); y = vreinterpretq_s16_s32(t.val[1]); }
#నిర్వచించు dct_trn64(x, y) { int16x8_t x0 = x; int16x8_t y0 = y; x = vcombine_s16(vget_దిగువ_s16(x0), vget_దిగువ_s16(y0)); y = vcombine_s16(vget_ఎగువ_s16(x0), vget_ఎగువ_s16(y0)); }

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

#నిర్వచనం_రద్దు dct_trn16
#నిర్వచనం_రద్దు dct_trn32
#నిర్వచనం_రద్దు dct_trn64
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
#నిర్వచించు dct_trn8_8(x, y) { uint8x8x2_t t = vtrn_u8(x, y); x = t.val[0]; y = t.val[1]; }
#నిర్వచించు dct_trn8_16(x, y) { uint16x4x2_t t = vtrn_u16(vreinterpret_u16_u8(x), vreinterpret_u16_u8(y)); x = vreinterpret_u8_u16(t.val[0]); y = vreinterpret_u8_u16(t.val[1]); }
#నిర్వచించు dct_trn8_32(x, y) { uint32x2x2_t t = vtrn_u32(vreinterpret_u32_u8(x), vreinterpret_u32_u8(y)); x = vreinterpret_u8_u32(t.val[0]); y = vreinterpret_u8_u32(t.val[1]); }

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
      vst1_u8(బహిర్గతం, p0); బహిర్గతం += బహిర్గతం_stride;
      vst1_u8(బహిర్గతం, p1); బహిర్గతం += బహిర్గతం_stride;
      vst1_u8(బహిర్గతం, p2); బహిర్గతం += బహిర్గతం_stride;
      vst1_u8(బహిర్గతం, p3); బహిర్గతం += బహిర్గతం_stride;
      vst1_u8(బహిర్గతం, p4); బహిర్గతం += బహిర్గతం_stride;
      vst1_u8(బహిర్గతం, p5); బహిర్గతం += బహిర్గతం_stride;
      vst1_u8(బహిర్గతం, p6); బహిర్గతం += బహిర్గతం_stride;
      vst1_u8(బహిర్గతం, p7);

#నిర్వచనం_రద్దు dct_trn8_8
#నిర్వచనం_రద్దు dct_trn8_16
#నిర్వచనం_రద్దు dct_trn8_32
   }

#నిర్వచనం_రద్దు dct_long_mul
#నిర్వచనం_రద్దు dct_long_mac
#నిర్వచనం_రద్దు dct_widen
#నిర్వచనం_రద్దు dct_wadd
#నిర్వచనం_రద్దు dct_wsub
#నిర్వచనం_రద్దు dct_bfly32o
#నిర్వచనం_రద్దు dct_pass
}

#అయితే_ముగింపు // STBI_NEON

#నిర్వచించు STBI__MARKER_none  0xff
// if there's a pending marker from the entropy stream, return that
// otherwise, fetch from the stream and get a marker. if there's no
// marker, return 0xff, which is never a valid marker value
స్థిర stbi_uc stbi__get_marker(stbi__jpeg *j)
{
   stbi_uc x;
   ఒకవేళ (j->marker != STBI__MARKER_none) { x = j->marker; j->marker = STBI__MARKER_none; తిరిగి x; }
   x = stbi__get8(j->s);
   ఒకవేళ (x != 0xff) తిరిగి STBI__MARKER_none;
   అయితే (x == 0xff)
      x = stbi__get8(j->s); // consume repeated 0xff fill bytes
   తిరిగి x;
}

// in each scan, we'll have scan_n components, and the order
// of the components is specified by order[]
#నిర్వచించు STBI__RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)

// after a restart interval, stbi__jpeg_reset the entropy decoder and
// the dc prediction
స్థిర శూన్య stbi__jpeg_reset(stbi__jpeg *j)
{
   j->సంకేతం_బిట్లు = 0;
   j->సంకేతం_బఫరు = 0;
   j->nomore = 0;
   j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = j->img_comp[3].dc_pred = 0;
   j->marker = STBI__MARKER_none;
   j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
   j->eob_నిరంతరం = 0;
   // no more than 1<<31 MCUs if no restart_interal? that's plenty safe,
   // since we don't even allow 1<<30 pixels
}

స్థిర పూర్ణాంకం stbi_parse_entropy_coded_దత్తాంశం(stbi__jpeg *z)
{
   stbi__jpeg_reset(z);
   ఒకవేళ (!z->progressive) {
      ఒకవేళ (z->పరిశీలన_n == 1) {
         పూర్ణాంకం i,j;
         STBI_SIMD_ALIGN(పొట్టి, దత్తాంశం[64]);
         పూర్ణాంకం n = z->క్రమం[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         పూర్ణాంకం w = (z->img_comp[n].x+7) >> 3;
         పూర్ణాంకం h = (z->img_comp[n].y+7) >> 3;
         కొరకు (j=0; j < h; ++j) {
            కొరకు (i=0; i < w; ++i) {
               పూర్ణాంకం ha = z->img_comp[n].ha;
               ఒకవేళ (!stbi_jpeg_decode_ఖండం(z, దత్తాంశం, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) తిరిగి 0;
               z->idct_ఖండం_kernel(z->img_comp[n].దత్తాంశం+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, దత్తాంశం);
               // every data block is an MCU, so countdown the restart interval
               ఒకవేళ (--z->todo <= 0) {
                  ఒకవేళ (z->సంకేతం_బిట్లు < 24) stbi_grow_బఫరు_unsafe(z);
                  // if it's NOT a restart, then just bail, so we get corrupt data
                  // rather than no data
                  ఒకవేళ (!STBI__RESTART(z->marker)) తిరిగి 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         తిరిగి 1;
      } లేకపోతే { // interleaved
         పూర్ణాంకం i,j,k,x,y;
         STBI_SIMD_ALIGN(పొట్టి, దత్తాంశం[64]);
         కొరకు (j=0; j < z->img_mcu_y; ++j) {
            కొరకు (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               కొరకు (k=0; k < z->పరిశీలన_n; ++k) {
                  పూర్ణాంకం n = z->క్రమం[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  కొరకు (y=0; y < z->img_comp[n].v; ++y) {
                     కొరకు (x=0; x < z->img_comp[n].h; ++x) {
                        పూర్ణాంకం x2 = (i*z->img_comp[n].h + x)*8;
                        పూర్ణాంకం y2 = (j*z->img_comp[n].v + y)*8;
                        పూర్ణాంకం ha = z->img_comp[n].ha;
                        ఒకవేళ (!stbi_jpeg_decode_ఖండం(z, దత్తాంశం, z->huff_dc+z->img_comp[n].hd, z->huff_ac+ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) తిరిగి 0;
                        z->idct_ఖండం_kernel(z->img_comp[n].దత్తాంశం+z->img_comp[n].w2*y2+x2, z->img_comp[n].w2, దత్తాంశం);
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               ఒకవేళ (--z->todo <= 0) {
                  ఒకవేళ (z->సంకేతం_బిట్లు < 24) stbi_grow_బఫరు_unsafe(z);
                  ఒకవేళ (!STBI__RESTART(z->marker)) తిరిగి 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         తిరిగి 1;
      }
   } లేకపోతే {
      ఒకవేళ (z->పరిశీలన_n == 1) {
         పూర్ణాంకం i,j;
         పూర్ణాంకం n = z->క్రమం[0];
         // non-interleaved data, we just need to process one block at a time,
         // in trivial scanline order
         // number of blocks to do just depends on how many actual "pixels" this
         // component has, independent of interleaved MCU blocking and such
         పూర్ణాంకం w = (z->img_comp[n].x+7) >> 3;
         పూర్ణాంకం h = (z->img_comp[n].y+7) >> 3;
         కొరకు (j=0; j < h; ++j) {
            కొరకు (i=0; i < w; ++i) {
               పొట్టి *దత్తాంశం = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               ఒకవేళ (z->spec_ప్రారంభం == 0) {
                  ఒకవేళ (!stbi_jpeg_decode_ఖండం_prog_dc(z, దత్తాంశం, &z->huff_dc[z->img_comp[n].hd], n))
                     తిరిగి 0;
               } లేకపోతే {
                  పూర్ణాంకం ha = z->img_comp[n].ha;
                  ఒకవేళ (!stbi_jpeg_decode_ఖండం_prog_ac(z, దత్తాంశం, &z->huff_ac[ha], z->fast_ac[ha]))
                     తిరిగి 0;
               }
               // every data block is an MCU, so countdown the restart interval
               ఒకవేళ (--z->todo <= 0) {
                  ఒకవేళ (z->సంకేతం_బిట్లు < 24) stbi_grow_బఫరు_unsafe(z);
                  ఒకవేళ (!STBI__RESTART(z->marker)) తిరిగి 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         తిరిగి 1;
      } లేకపోతే { // interleaved
         పూర్ణాంకం i,j,k,x,y;
         కొరకు (j=0; j < z->img_mcu_y; ++j) {
            కొరకు (i=0; i < z->img_mcu_x; ++i) {
               // scan an interleaved mcu... process scan_n components in order
               కొరకు (k=0; k < z->పరిశీలన_n; ++k) {
                  పూర్ణాంకం n = z->క్రమం[k];
                  // scan out an mcu's worth of this component; that's just determined
                  // by the basic H and V specified for the component
                  కొరకు (y=0; y < z->img_comp[n].v; ++y) {
                     కొరకు (x=0; x < z->img_comp[n].h; ++x) {
                        పూర్ణాంకం x2 = (i*z->img_comp[n].h + x);
                        పూర్ణాంకం y2 = (j*z->img_comp[n].v + y);
                        పొట్టి *దత్తాంశం = z->img_comp[n].coeff + 64 * (x2 + y2 * z->img_comp[n].coeff_w);
                        ఒకవేళ (!stbi_jpeg_decode_ఖండం_prog_dc(z, దత్తాంశం, &z->huff_dc[z->img_comp[n].hd], n))
                           తిరిగి 0;
                     }
                  }
               }
               // after all interleaved components, that's an interleaved MCU,
               // so now count down the restart interval
               ఒకవేళ (--z->todo <= 0) {
                  ఒకవేళ (z->సంకేతం_బిట్లు < 24) stbi_grow_బఫరు_unsafe(z);
                  ఒకవేళ (!STBI__RESTART(z->marker)) తిరిగి 1;
                  stbi__jpeg_reset(z);
               }
            }
         }
         తిరిగి 1;
      }
   }
}

స్థిర శూన్య stbi__jpeg_dequantize(పొట్టి *దత్తాంశం, stbi__uint16 *dequant)
{
   పూర్ణాంకం i;
   కొరకు (i=0; i < 64; ++i)
      దత్తాంశం[i] *= dequant[i];
}

స్థిర శూన్య stbi_jpeg_ముగింపు(stbi__jpeg *z)
{
   ఒకవేళ (z->progressive) {
      // dequantize and idct the data
      పూర్ణాంకం i,j,n;
      కొరకు (n=0; n < z->s->img_n; ++n) {
         పూర్ణాంకం w = (z->img_comp[n].x+7) >> 3;
         పూర్ణాంకం h = (z->img_comp[n].y+7) >> 3;
         కొరకు (j=0; j < h; ++j) {
            కొరకు (i=0; i < w; ++i) {
               పొట్టి *దత్తాంశం = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
               stbi__jpeg_dequantize(దత్తాంశం, z->dequant[z->img_comp[n].tq]);
               z->idct_ఖండం_kernel(z->img_comp[n].దత్తాంశం+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, దత్తాంశం);
            }
         }
      }
   }
}

స్థిర పూర్ణాంకం stbi__process_marker(stbi__jpeg *z, పూర్ణాంకం m)
{
   పూర్ణాంకం L;
   ఎంపిక (m) {
      స్థితి STBI__MARKER_none: // no marker found
         తిరిగి stbi__err("expected marker","Corrupt JPEG");

      స్థితి 0xDD: // DRI - specify restart interval
         ఒకవేళ (stbi__get16be(z->s) != 4) తిరిగి stbi__err("bad DRI len","Corrupt JPEG");
         z->restart_interval = stbi__get16be(z->s);
         తిరిగి 1;

      స్థితి 0xDB: // DQT - define quantization table
         L = stbi__get16be(z->s)-2;
         అయితే (L > 0) {
            పూర్ణాంకం q = stbi__get8(z->s);
            పూర్ణాంకం p = q >> 4, sixteen = (p != 0);
            పూర్ణాంకం t = q & 15,i;
            ఒకవేళ (p != 0 && p != 1) తిరిగి stbi__err("bad DQT type","Corrupt JPEG");
            ఒకవేళ (t > 3) తిరిగి stbi__err("bad DQT table","Corrupt JPEG");

            కొరకు (i=0; i < 64; ++i)
               z->dequant[t][stbi__jpeg_dezigzag[i]] = (stbi__uint16)(sixteen ? stbi__get16be(z->s) : stbi__get8(z->s));
            L -= (sixteen ? 129 : 65);
         }
         తిరిగి L==0;

      స్థితి 0xC4: // DHT - define huffman table
         L = stbi__get16be(z->s)-2;
         అయితే (L > 0) {
            stbi_uc *v;
            పూర్ణాంకం sizes[16],i,n=0;
            పూర్ణాంకం q = stbi__get8(z->s);
            పూర్ణాంకం tc = q >> 4;
            పూర్ణాంకం th = q & 15;
            ఒకవేళ (tc > 1 || th > 3) తిరిగి stbi__err("bad DHT header","Corrupt JPEG");
            కొరకు (i=0; i < 16; ++i) {
               sizes[i] = stbi__get8(z->s);
               n += sizes[i];
            }
            L -= 17;
            ఒకవేళ (tc == 0) {
               ఒకవేళ (!stbi__build_huffman(z->huff_dc+th, sizes)) తిరిగి 0;
               v = z->huff_dc[th].విలువలు;
            } లేకపోతే {
               ఒకవేళ (!stbi__build_huffman(z->huff_ac+th, sizes)) తిరిగి 0;
               v = z->huff_ac[th].విలువలు;
            }
            కొరకు (i=0; i < n; ++i)
               v[i] = stbi__get8(z->s);
            ఒకవేళ (tc != 0)
               stbi__build_fast_ac(z->fast_ac[th], z->huff_ac + th);
            L -= n;
         }
         తిరిగి L==0;
   }

   // check for comment block or APP blocks
   ఒకవేళ ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
      L = stbi__get16be(z->s);
      ఒకవేళ (L < 2) {
         ఒకవేళ (m == 0xFE)
            తిరిగి stbi__err("bad COM len","Corrupt JPEG");
         లేకపోతే
            తిరిగి stbi__err("bad APP len","Corrupt JPEG");
      }
      L -= 2;

      ఒకవేళ (m == 0xE0 && L >= 5) { // JFIF APP0 segment
         స్థిర స్థిరరాశి గుర్తులేని అక్షరం ట్యాగ్[5] = {'J','F','I','F','\0'};
         పూర్ణాంకం ok = 1;
         పూర్ణాంకం i;
         కొరకు (i=0; i < 5; ++i)
            ఒకవేళ (stbi__get8(z->s) != ట్యాగ్[i])
               ok = 0;
         L -= 5;
         ఒకవేళ (ok)
            z->jfif = 1;
      } లేకపోతే ఒకవేళ (m == 0xEE && L >= 12) { // Adobe APP14 segment
         స్థిర స్థిరరాశి గుర్తులేని అక్షరం ట్యాగ్[6] = {'A','d','o','b','e','\0'};
         పూర్ణాంకం ok = 1;
         పూర్ణాంకం i;
         కొరకు (i=0; i < 6; ++i)
            ఒకవేళ (stbi__get8(z->s) != ట్యాగ్[i])
               ok = 0;
         L -= 6;
         ఒకవేళ (ok) {
            stbi__get8(z->s); //సంస్కరణ
            stbi__get16be(z->s); // flags0
            stbi__get16be(z->s); // flags1
            z->app14_color_transform = stbi__get8(z->s); // color transform
            L -= 6;
         }
      }

      stbi__skip(z->s, L);
      తిరిగి 1;
   }

   తిరిగి stbi__err("unknown marker","Corrupt JPEG");
}

// after we see SOS
స్థిర పూర్ణాంకం stbi_process_పరిశీలన_శీర్షిక(stbi__jpeg *z)
{
   పూర్ణాంకం i;
   పూర్ణాంకం Ls = stbi__get16be(z->s);
   z->పరిశీలన_n = stbi__get8(z->s);
   ఒకవేళ (z->పరిశీలన_n < 1 || z->పరిశీలన_n > 4 || z->పరిశీలన_n > (పూర్ణాంకం) z->s->img_n) తిరిగి stbi__err("bad SOS component count","Corrupt JPEG");
   ఒకవేళ (Ls != 6+2*z->పరిశీలన_n) తిరిగి stbi__err("bad SOS len","Corrupt JPEG");
   కొరకు (i=0; i < z->పరిశీలన_n; ++i) {
      పూర్ణాంకం id = stbi__get8(z->s), which;
      పూర్ణాంకం q = stbi__get8(z->s);
      కొరకు (which = 0; which < z->s->img_n; ++which)
         ఒకవేళ (z->img_comp[which].id == id)
            ఆపు;
      ఒకవేళ (which == z->s->img_n) తిరిగి 0; // no match
      z->img_comp[which].hd = q >> 4;   ఒకవేళ (z->img_comp[which].hd > 3) తిరిగి stbi__err("bad DC huff","Corrupt JPEG");
      z->img_comp[which].ha = q & 15;   ఒకవేళ (z->img_comp[which].ha > 3) తిరిగి stbi__err("bad AC huff","Corrupt JPEG");
      z->క్రమం[i] = which;
   }

   {
      పూర్ణాంకం aa;
      z->spec_ప్రారంభం = stbi__get8(z->s);
      z->spec_చివర   = stbi__get8(z->s); // should be 63, but might be 0
      aa = stbi__get8(z->s);
      z->succ_ఎగువ = (aa >> 4);
      z->succ_దిగువ  = (aa & 15);
      ఒకవేళ (z->progressive) {
         ఒకవేళ (z->spec_ప్రారంభం > 63 || z->spec_చివర > 63  || z->spec_ప్రారంభం > z->spec_చివర || z->succ_ఎగువ > 13 || z->succ_దిగువ > 13)
            తిరిగి stbi__err("bad SOS", "Corrupt JPEG");
      } లేకపోతే {
         ఒకవేళ (z->spec_ప్రారంభం != 0) తిరిగి stbi__err("bad SOS","Corrupt JPEG");
         ఒకవేళ (z->succ_ఎగువ != 0 || z->succ_దిగువ != 0) తిరిగి stbi__err("bad SOS","Corrupt JPEG");
         z->spec_చివర = 63;
      }
   }

   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__free_jpeg_components(stbi__jpeg *z, పూర్ణాంకం ncomp, పూర్ణాంకం why)
{
   పూర్ణాంకం i;
   కొరకు (i=0; i < ncomp; ++i) {
      ఒకవేళ (z->img_comp[i].raw_దత్తాంశం) {
         STBI_FREE(z->img_comp[i].raw_దత్తాంశం);
         z->img_comp[i].raw_దత్తాంశం = ఖాళీ;
         z->img_comp[i].దత్తాంశం = ఖాళీ;
      }
      ఒకవేళ (z->img_comp[i].raw_coeff) {
         STBI_FREE(z->img_comp[i].raw_coeff);
         z->img_comp[i].raw_coeff = 0;
         z->img_comp[i].coeff = 0;
      }
      ఒకవేళ (z->img_comp[i].linebuf) {
         STBI_FREE(z->img_comp[i].linebuf);
         z->img_comp[i].linebuf = ఖాళీ;
      }
   }
   తిరిగి why;
}

స్థిర పూర్ణాంకం stbi_process_చట్రం_శీర్షిక(stbi__jpeg *z, పూర్ణాంకం పరిశీలన)
{
   stbi__context *s = z->s;
   పూర్ణాంకం Lf,p,i,q, h_max=1,v_max=1,c;
   Lf = stbi__get16be(s);         ఒకవేళ (Lf < 11) తిరిగి stbi__err("bad SOF len","Corrupt JPEG"); // JPEG
   p  = stbi__get8(s);            ఒకవేళ (p != 8) తిరిగి stbi__err("only 8-bit","JPEG format not supported: 8-bit only"); // JPEG baseline
   s->img_y = stbi__get16be(s);   ఒకవేళ (s->img_y == 0) తిరిగి stbi__err("no header height", "JPEG format not supported: delayed height"); // Legal, but we don't handle it--but neither does IJG
   s->img_x = stbi__get16be(s);   ఒకవేళ (s->img_x == 0) తిరిగి stbi__err("0 width","Corrupt JPEG"); // JPEG requires
   c = stbi__get8(s);
   ఒకవేళ (c != 3 && c != 1 && c != 4) తిరిగి stbi__err("bad component count","Corrupt JPEG");
   s->img_n = c;
   కొరకు (i=0; i < c; ++i) {
      z->img_comp[i].దత్తాంశం = ఖాళీ;
      z->img_comp[i].linebuf = ఖాళీ;
   }

   ఒకవేళ (Lf != 8+3*s->img_n) తిరిగి stbi__err("bad SOF len","Corrupt JPEG");

   z->rgb = 0;
   కొరకు (i=0; i < s->img_n; ++i) {
      స్థిర స్థిరరాశి గుర్తులేని అక్షరం rgb[3] = { 'R', 'G', 'B' };
      z->img_comp[i].id = stbi__get8(s);
      ఒకవేళ (s->img_n == 3 && z->img_comp[i].id == rgb[i])
         ++z->rgb;
      q = stbi__get8(s);
      z->img_comp[i].h = (q >> 4);  ఒకవేళ (!z->img_comp[i].h || z->img_comp[i].h > 4) తిరిగి stbi__err("bad H","Corrupt JPEG");
      z->img_comp[i].v = q & 15;    ఒకవేళ (!z->img_comp[i].v || z->img_comp[i].v > 4) తిరిగి stbi__err("bad V","Corrupt JPEG");
      z->img_comp[i].tq = stbi__get8(s);  ఒకవేళ (z->img_comp[i].tq > 3) తిరిగి stbi__err("bad TQ","Corrupt JPEG");
   }

   ఒకవేళ (పరిశీలన != STBI_పరిశీలన_ఎక్కించు) తిరిగి 1;

   ఒకవేళ (!stbi__mad3sizes_valid(s->img_x, s->img_y, s->img_n, 0)) తిరిగి stbi__err("too large", "Image too large to decode");

   కొరకు (i=0; i < s->img_n; ++i) {
      ఒకవేళ (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
      ఒకవేళ (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
   }

   // compute interleaved mcu info
   z->img_h_max = h_max;
   z->img_v_max = v_max;
   z->img_mcu_w = h_max * 8;
   z->img_mcu_h = v_max * 8;
   // these sizes can't be more than 17 bits
   z->img_mcu_x = (s->img_x + z->img_mcu_w-1) / z->img_mcu_w;
   z->img_mcu_y = (s->img_y + z->img_mcu_h-1) / z->img_mcu_h;

   కొరకు (i=0; i < s->img_n; ++i) {
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
      z->img_comp[i].linebuf = ఖాళీ;
      z->img_comp[i].raw_దత్తాంశం = stbi__malloc_mad2(z->img_comp[i].w2, z->img_comp[i].h2, 15);
      ఒకవేళ (z->img_comp[i].raw_దత్తాంశం == ఖాళీ)
         తిరిగి stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
      // align blocks for idct using mmx/sse
      z->img_comp[i].దత్తాంశం = (stbi_uc*) (((size_t) z->img_comp[i].raw_దత్తాంశం + 15) & ~15);
      ఒకవేళ (z->progressive) {
         // w2, h2 are multiples of 8 (see above)
         z->img_comp[i].coeff_w = z->img_comp[i].w2 / 8;
         z->img_comp[i].coeff_h = z->img_comp[i].h2 / 8;
         z->img_comp[i].raw_coeff = stbi__malloc_mad3(z->img_comp[i].w2, z->img_comp[i].h2, పరిమాణం(పొట్టి), 15);
         ఒకవేళ (z->img_comp[i].raw_coeff == ఖాళీ)
            తిరిగి stbi__free_jpeg_components(z, i+1, stbi__err("outofmem", "Out of memory"));
         z->img_comp[i].coeff = (పొట్టి*) (((size_t) z->img_comp[i].raw_coeff + 15) & ~15);
      }
   }

   తిరిగి 1;
}

// use comparisons since in some cases we handle more than one case (e.g. SOF)
#నిర్వచించు stbi__DNL(x)         ((x) == 0xdc)
#నిర్వచించు stbi__SOI(x)         ((x) == 0xd8)
#నిర్వచించు stbi__EOI(x)         ((x) == 0xd9)
#నిర్వచించు stbi__SOF(x)         ((x) == 0xc0 || (x) == 0xc1 || (x) == 0xc2)
#నిర్వచించు stbi__SOS(x)         ((x) == 0xda)

#నిర్వచించు stbi__SOF_progressive(x)   ((x) == 0xc2)

స్థిర పూర్ణాంకం stbi_decode_jpeg_శీర్షిక(stbi__jpeg *z, పూర్ణాంకం పరిశీలన)
{
   పూర్ణాంకం m;
   z->jfif = 0;
   z->app14_color_transform = -1; // valid values are 0,1,2
   z->marker = STBI__MARKER_none; // initialize cached marker to empty
   m = stbi__get_marker(z);
   ఒకవేళ (!stbi__SOI(m)) తిరిగి stbi__err("no SOI","Corrupt JPEG");
   ఒకవేళ (పరిశీలన == STBI_పరిశీలన_type) తిరిగి 1;
   m = stbi__get_marker(z);
   అయితే (!stbi__SOF(m)) {
      ఒకవేళ (!stbi__process_marker(z,m)) తిరిగి 0;
      m = stbi__get_marker(z);
      అయితే (m == STBI__MARKER_none) {
         // some files have extra padding after their blocks, so ok, we'll scan
         ఒకవేళ (stbi__at_eof(z->s)) తిరిగి stbi__err("no SOF", "Corrupt JPEG");
         m = stbi__get_marker(z);
      }
   }
   z->progressive = stbi__SOF_progressive(m);
   ఒకవేళ (!stbi_process_చట్రం_శీర్షిక(z, పరిశీలన)) తిరిగి 0;
   తిరిగి 1;
}

// decode image to YCbCr format
స్థిర పూర్ణాంకం stbi__decode_jpeg_image(stbi__jpeg *j)
{
   పూర్ణాంకం m;
   కొరకు (m = 0; m < 4; m++) {
      j->img_comp[m].raw_దత్తాంశం = ఖాళీ;
      j->img_comp[m].raw_coeff = ఖాళీ;
   }
   j->restart_interval = 0;
   ఒకవేళ (!stbi_decode_jpeg_శీర్షిక(j, STBI_పరిశీలన_ఎక్కించు)) తిరిగి 0;
   m = stbi__get_marker(j);
   అయితే (!stbi__EOI(m)) {
      ఒకవేళ (stbi__SOS(m)) {
         ఒకవేళ (!stbi_process_పరిశీలన_శీర్షిక(j)) తిరిగి 0;
         ఒకవేళ (!stbi_parse_entropy_coded_దత్తాంశం(j)) తిరిగి 0;
         ఒకవేళ (j->marker == STBI__MARKER_none ) {
            // handle 0s at the end of image data from IP Kamera 9060
            అయితే (!stbi__at_eof(j->s)) {
               పూర్ణాంకం x = stbi__get8(j->s);
               ఒకవేళ (x == 255) {
                  j->marker = stbi__get8(j->s);
                  ఆపు;
               }
            }
            // if we reach eof without hitting a marker, stbi__get_marker() below will fail and we'll eventually return 0
         }
      } లేకపోతే ఒకవేళ (stbi__DNL(m)) {
         పూర్ణాంకం Ld = stbi__get16be(j->s);
         stbi__uint32 NL = stbi__get16be(j->s);
         ఒకవేళ (Ld != 4) తిరిగి stbi__err("bad DNL len", "Corrupt JPEG");
         ఒకవేళ (NL != j->s->img_y) తిరిగి stbi__err("bad DNL height", "Corrupt JPEG");
      } లేకపోతే {
         ఒకవేళ (!stbi__process_marker(j, m)) తిరిగి 0;
      }
      m = stbi__get_marker(j);
   }
   ఒకవేళ (j->progressive)
      stbi_jpeg_ముగింపు(j);
   తిరిగి 1;
}

// static jfif-centered resampling (across block boundaries)

రకనిర్వచనం stbi_uc *(*resample_అడ్డువరుస_func)(stbi_uc *బహిర్గతం, stbi_uc *in0, stbi_uc *in1,
                                    పూర్ణాంకం w, పూర్ణాంకం hs);

#నిర్వచించు stbi__div4(x) ((stbi_uc) ((x) >> 2))

స్థిర stbi_uc *resample_అడ్డువరుస_1(stbi_uc *బహిర్గతం, stbi_uc *లోపల_near, stbi_uc *లోపల_far, పూర్ణాంకం w, పూర్ణాంకం hs)
{
   STBI_NOTUSED(బహిర్గతం);
   STBI_NOTUSED(లోపల_far);
   STBI_NOTUSED(w);
   STBI_NOTUSED(hs);
   తిరిగి లోపల_near;
}

స్థిర stbi_uc* stbi_resample_అడ్డువరుస_v_2(stbi_uc *బహిర్గతం, stbi_uc *లోపల_near, stbi_uc *లోపల_far, పూర్ణాంకం w, పూర్ణాంకం hs)
{
   // need to generate two samples vertically for every one in input
   పూర్ణాంకం i;
   STBI_NOTUSED(hs);
   కొరకు (i=0; i < w; ++i)
      బహిర్గతం[i] = stbi__div4(3*లోపల_near[i] + లోపల_far[i] + 2);
   తిరిగి బహిర్గతం;
}

స్థిర stbi_uc*  stbi_resample_అడ్డువరుస_h_2(stbi_uc *బహిర్గతం, stbi_uc *లోపల_near, stbi_uc *లోపల_far, పూర్ణాంకం w, పూర్ణాంకం hs)
{
   // need to generate two samples horizontally for every one in input
   పూర్ణాంకం i;
   stbi_uc *ఆదానం = లోపల_near;

   ఒకవేళ (w == 1) {
      // if only one sample, can't do any interpolation
      బహిర్గతం[0] = బహిర్గతం[1] = ఆదానం[0];
      తిరిగి బహిర్గతం;
   }

   బహిర్గతం[0] = ఆదానం[0];
   బహిర్గతం[1] = stbi__div4(ఆదానం[0]*3 + ఆదానం[1] + 2);
   కొరకు (i=1; i < w-1; ++i) {
      పూర్ణాంకం n = 3*ఆదానం[i]+2;
      బహిర్గతం[i*2+0] = stbi__div4(n+ఆదానం[i-1]);
      బహిర్గతం[i*2+1] = stbi__div4(n+ఆదానం[i+1]);
   }
   బహిర్గతం[i*2+0] = stbi__div4(ఆదానం[w-2]*3 + ఆదానం[w-1] + 2);
   బహిర్గతం[i*2+1] = ఆదానం[w-1];

   STBI_NOTUSED(లోపల_far);
   STBI_NOTUSED(hs);

   తిరిగి బహిర్గతం;
}

#నిర్వచించు stbi__div16(x) ((stbi_uc) ((x) >> 4))

స్థిర stbi_uc *stbi_resample_అడ్డువరుస_hv_2(stbi_uc *బహిర్గతం, stbi_uc *లోపల_near, stbi_uc *లోపల_far, పూర్ణాంకం w, పూర్ణాంకం hs)
{
   // need to generate 2x2 samples for every one in input
   పూర్ణాంకం i,t0,t1;
   ఒకవేళ (w == 1) {
      బహిర్గతం[0] = బహిర్గతం[1] = stbi__div4(3*లోపల_near[0] + లోపల_far[0] + 2);
      తిరిగి బహిర్గతం;
   }

   t1 = 3*లోపల_near[0] + లోపల_far[0];
   బహిర్గతం[0] = stbi__div4(t1+2);
   కొరకు (i=1; i < w; ++i) {
      t0 = t1;
      t1 = 3*లోపల_near[i]+లోపల_far[i];
      బహిర్గతం[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      బహిర్గతం[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   బహిర్గతం[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   తిరిగి బహిర్గతం;
}

#ఒకవేళ defined(STBI_SSE2) || defined(STBI_NEON)
స్థిర stbi_uc *stbi_resample_అడ్డువరుస_hv_2_simd(stbi_uc *బహిర్గతం, stbi_uc *లోపల_near, stbi_uc *లోపల_far, పూర్ణాంకం w, పూర్ణాంకం hs)
{
   // need to generate 2x2 samples for every one in input
   పూర్ణాంకం i=0,t0,t1;

   ఒకవేళ (w == 1) {
      బహిర్గతం[0] = బహిర్గతం[1] = stbi__div4(3*లోపల_near[0] + లోపల_far[0] + 2);
      తిరిగి బహిర్గతం;
   }

   t1 = 3*లోపల_near[0] + లోపల_far[0];
   // process groups of 8 pixels for as long as we can.
   // note we can't handle the last pixel in a row in this loop
   // because we need to handle the filter boundary conditions.
   కొరకు (; i < ((w-1) & ~7); i += 8) {
#ఒకవేళ defined(STBI_SSE2)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      __m128i zero  = _mm_setzero_si128();
      __m128i farb  = _mm_loadl_epi64((__m128i *) (లోపల_far + i));
      __m128i nearb = _mm_loadl_epi64((__m128i *) (లోపల_near + i));
      __m128i farw  = _mm_unpacklo_epi8(farb, zero);
      __m128i nearw = _mm_unpacklo_epi8(nearb, zero);
      __m128i diff  = _mm_sub_epi16(farw, nearw);
      __m128i nears = _mm_slli_epi16(nearw, 2);
      __m128i curr  = mm_కూడు_epi16(nears, diff); // current row

      // horizontal filter works the same based on shifted vers of current
      // row. "prev" is current row shifted right by 1 pixel; we need to
      // insert the previous pixel value (from t1).
      // "next" is current row shifted left by 1 pixel, with first pixel
      // of next block of 8 pixels added in.
      __m128i prv0 = _mm_slli_si128(curr, 2);
      __m128i nxt0 = _mm_srli_si128(curr, 2);
      __m128i prev = mm_చొప్పించు_epi16(prv0, t1, 0);
      __m128i తదుపరి = mm_చొప్పించు_epi16(nxt0, 3*లోపల_near[i+8] + లోపల_far[i+8], 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      __m128i bias  = _mm_set1_epi16(8);
      __m128i curs = _mm_slli_epi16(curr, 2);
      __m128i prvd = _mm_sub_epi16(prev, curr);
      __m128i nxtd = _mm_sub_epi16(తదుపరి, curr);
      __m128i curb = mm_కూడు_epi16(curs, bias);
      __m128i even = mm_కూడు_epi16(prvd, curb);
      __m128i odd  = mm_కూడు_epi16(nxtd, curb);

      // interleave even and odd pixels, then undo scaling.
      __m128i int0 = _mm_unpacklo_epi16(even, odd);
      __m128i int1 = _mm_unpackhi_epi16(even, odd);
      __m128i de0  = _mm_srli_epi16(int0, 4);
      __m128i de1  = _mm_srli_epi16(int1, 4);

      // pack and write output
      __m128i outv = _mm_packus_epi16(de0, de1);
      _mm_storeu_si128((__m128i *) (బహిర్గతం + i*2), outv);
#లేదావేళ defined(STBI_NEON)
      // load and perform the vertical filtering pass
      // this uses 3*x + y = 4*x + (y - x)
      uint8x8_t farb  = vld1_u8(లోపల_far + i);
      uint8x8_t nearb = vld1_u8(లోపల_near + i);
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
      int16x8_t తదుపరి = vsetq_lane_s16(3*లోపల_near[i+8] + లోపల_far[i+8], nxt0, 7);

      // horizontal filter, polyphase implementation since it's convenient:
      // even pixels = 3*cur + prev = cur*4 + (prev - cur)
      // odd  pixels = 3*cur + next = cur*4 + (next - cur)
      // note the shared term.
      int16x8_t curs = vshlq_n_s16(curr, 2);
      int16x8_t prvd = vsubq_s16(prev, curr);
      int16x8_t nxtd = vsubq_s16(తదుపరి, curr);
      int16x8_t even = vaddq_s16(curs, prvd);
      int16x8_t odd  = vaddq_s16(curs, nxtd);

      // undo scaling and round, then store with even/odd phases interleaved
      uint8x8x2_t o;
      o.val[0] = vqrshrun_n_s16(even, 4);
      o.val[1] = vqrshrun_n_s16(odd,  4);
      vst2_u8(బహిర్గతం + i*2, o);
#అయితే_ముగింపు

      // "previous" value for next iter
      t1 = 3*లోపల_near[i+7] + లోపల_far[i+7];
   }

   t0 = t1;
   t1 = 3*లోపల_near[i] + లోపల_far[i];
   బహిర్గతం[i*2] = stbi__div16(3*t1 + t0 + 8);

   కొరకు (++i; i < w; ++i) {
      t0 = t1;
      t1 = 3*లోపల_near[i]+లోపల_far[i];
      బహిర్గతం[i*2-1] = stbi__div16(3*t0 + t1 + 8);
      బహిర్గతం[i*2  ] = stbi__div16(3*t1 + t0 + 8);
   }
   బహిర్గతం[w*2-1] = stbi__div4(t1+2);

   STBI_NOTUSED(hs);

   తిరిగి బహిర్గతం;
}
#అయితే_ముగింపు

స్థిర stbi_uc *stbi_resample_అడ్డువరుస_generic(stbi_uc *బహిర్గతం, stbi_uc *లోపల_near, stbi_uc *లోపల_far, పూర్ణాంకం w, పూర్ణాంకం hs)
{
   // resample with nearest-neighbor
   పూర్ణాంకం i,j;
   STBI_NOTUSED(లోపల_far);
   కొరకు (i=0; i < w; ++i)
      కొరకు (j=0; j < hs; ++j)
         బహిర్గతం[i*hs+j] = లోపల_near[i];
   తిరిగి బహిర్గతం;
}

// this is a reduced-precision calculation of YCbCr-to-RGB introduced
// to make sure the code produces the same results in both SIMD and scalar
#నిర్వచించు stbi__float2fixed(x)  (((పూర్ణాంకం) ((x) * 4096.0f + 0.5f)) << 8)
స్థిర శూన్య stbi_YCb_Cr_గమ్యం_RGB_అడ్డువరుస(stbi_uc *బహిర్గతం, స్థిరరాశి stbi_uc *y, స్థిరరాశి stbi_uc *pcb, స్థిరరాశి stbi_uc *pcr, పూర్ణాంకం లెక్క, పూర్ణాంకం అడుగు)
{
   పూర్ణాంకం i;
   కొరకు (i=0; i < లెక్క; ++i) {
      పూర్ణాంకం y_fixed = (y[i] << 20) + (1<<19); // rounding
      పూర్ణాంకం r,g,b;
      పూర్ణాంకం cr = pcr[i] - 128;
      పూర్ణాంకం cb = pcb[i] - 128;
      r = y_fixed +  cr* stbi__float2fixed(1.40200f);
      g = y_fixed + (cr*-stbi__float2fixed(0.71414f)) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                     +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      ఒకవేళ ((గుర్తులేని) r > 255) { ఒకవేళ (r < 0) r = 0; లేకపోతే r = 255; }
      ఒకవేళ ((గుర్తులేని) g > 255) { ఒకవేళ (g < 0) g = 0; లేకపోతే g = 255; }
      ఒకవేళ ((గుర్తులేని) b > 255) { ఒకవేళ (b < 0) b = 0; లేకపోతే b = 255; }
      బహిర్గతం[0] = (stbi_uc)r;
      బహిర్గతం[1] = (stbi_uc)g;
      బహిర్గతం[2] = (stbi_uc)b;
      బహిర్గతం[3] = 255;
      బహిర్గతం += అడుగు;
   }
}

#ఒకవేళ defined(STBI_SSE2) || defined(STBI_NEON)
స్థిర శూన్య stbi_YCb_Cr_గమ్యం_RGB_simd(stbi_uc *బహిర్గతం, stbi_uc స్థిరరాశి *y, stbi_uc స్థిరరాశి *pcb, stbi_uc స్థిరరాశి *pcr, పూర్ణాంకం లెక్క, పూర్ణాంకం అడుగు)
{
   పూర్ణాంకం i = 0;

#నిర్వచితమైతే STBI_SSE2
   // step == 3 is pretty ugly on the final interleave, and i'm not convinced
   // it's useful in practice (you wouldn't use it for textures, for example).
   // so just accelerate step == 4 case.
   ఒకవేళ (అడుగు == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      __m128i signflip  = _mm_set1_epi8(-0x80);
      __m128i cr_const0 = _mm_set1_epi16(   (పొట్టి) ( 1.40200f*4096.0f+0.5f));
      __m128i cr_const1 = _mm_set1_epi16( - (పొట్టి) ( 0.71414f*4096.0f+0.5f));
      __m128i cb_const0 = _mm_set1_epi16( - (పొట్టి) ( 0.34414f*4096.0f+0.5f));
      __m128i cb_const1 = _mm_set1_epi16(   (పొట్టి) ( 1.77200f*4096.0f+0.5f));
      __m128i y_bias = _mm_set1_epi8((అక్షరం) (గుర్తులేని అక్షరం) 128);
      __m128i xw = _mm_set1_epi16(255); // alpha channel

      కొరకు (; i+7 < లెక్క; i += 8) {
         //ఎక్కించు
         __m128i y_బైట్లు = _mm_loadl_epi64((__m128i *) (y+i));
         __m128i cr_బైట్లు = _mm_loadl_epi64((__m128i *) (pcr+i));
         __m128i cb_బైట్లు = _mm_loadl_epi64((__m128i *) (pcb+i));
         __m128i cr_biased = _mm_xor_si128(cr_బైట్లు, signflip); // -128
         __m128i cb_biased = _mm_xor_si128(cb_బైట్లు, signflip); // -128

         // unpack to short (and left-shift cr, cb by 8)
         __m128i yw  = _mm_unpacklo_epi8(y_bias, y_బైట్లు);
         __m128i crw = _mm_unpacklo_epi8(_mm_setzero_si128(), cr_biased);
         __m128i cbw = _mm_unpacklo_epi8(_mm_setzero_si128(), cb_biased);

         // color transform
         __m128i yws = _mm_srli_epi16(yw, 4);
         __m128i cr0 = _mm_mulhi_epi16(cr_const0, crw);
         __m128i cb0 = _mm_mulhi_epi16(cb_const0, cbw);
         __m128i cb1 = _mm_mulhi_epi16(cbw, cb_const1);
         __m128i cr1 = _mm_mulhi_epi16(crw, cr_const1);
         __m128i rws = mm_కూడు_epi16(cr0, yws);
         __m128i gwt = mm_కూడు_epi16(cb0, yws);
         __m128i bws = mm_కూడు_epi16(yws, cb1);
         __m128i gws = mm_కూడు_epi16(gwt, cr1);

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
         _mm_storeu_si128((__m128i *) (బహిర్గతం + 0), o0);
         _mm_storeu_si128((__m128i *) (బహిర్గతం + 16), o1);
         బహిర్గతం += 32;
      }
   }
#అయితే_ముగింపు

#నిర్వచితమైతే STBI_NEON
   // in this version, step=3 support would be easy to add. but is there demand?
   ఒకవేళ (అడుగు == 4) {
      // this is a fairly straightforward implementation and not super-optimized.
      uint8x8_t signflip = vdup_n_u8(0x80);
      int16x8_t cr_const0 = vdupq_n_s16(   (పొట్టి) ( 1.40200f*4096.0f+0.5f));
      int16x8_t cr_const1 = vdupq_n_s16( - (పొట్టి) ( 0.71414f*4096.0f+0.5f));
      int16x8_t cb_const0 = vdupq_n_s16( - (పొట్టి) ( 0.34414f*4096.0f+0.5f));
      int16x8_t cb_const1 = vdupq_n_s16(   (పొట్టి) ( 1.77200f*4096.0f+0.5f));

      కొరకు (; i+7 < లెక్క; i += 8) {
         //ఎక్కించు
         uint8x8_t y_బైట్లు  = vld1_u8(y + i);
         uint8x8_t cr_బైట్లు = vld1_u8(pcr + i);
         uint8x8_t cb_బైట్లు = vld1_u8(pcb + i);
         int8x8_t cr_biased = vreinterpret_s8_u8(vsub_u8(cr_బైట్లు, signflip));
         int8x8_t cb_biased = vreinterpret_s8_u8(vsub_u8(cb_బైట్లు, signflip));

         // expand to s16
         int16x8_t yws = vreinterpretq_s16_u16(vshll_n_u8(y_బైట్లు, 4));
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
         vst4_u8(బహిర్గతం, o);
         బహిర్గతం += 8*4;
      }
   }
#అయితే_ముగింపు

   కొరకు (; i < లెక్క; ++i) {
      పూర్ణాంకం y_fixed = (y[i] << 20) + (1<<19); // rounding
      పూర్ణాంకం r,g,b;
      పూర్ణాంకం cr = pcr[i] - 128;
      పూర్ణాంకం cb = pcb[i] - 128;
      r = y_fixed + cr* stbi__float2fixed(1.40200f);
      g = y_fixed + cr*-stbi__float2fixed(0.71414f) + ((cb*-stbi__float2fixed(0.34414f)) & 0xffff0000);
      b = y_fixed                                   +   cb* stbi__float2fixed(1.77200f);
      r >>= 20;
      g >>= 20;
      b >>= 20;
      ఒకవేళ ((గుర్తులేని) r > 255) { ఒకవేళ (r < 0) r = 0; లేకపోతే r = 255; }
      ఒకవేళ ((గుర్తులేని) g > 255) { ఒకవేళ (g < 0) g = 0; లేకపోతే g = 255; }
      ఒకవేళ ((గుర్తులేని) b > 255) { ఒకవేళ (b < 0) b = 0; లేకపోతే b = 255; }
      బహిర్గతం[0] = (stbi_uc)r;
      బహిర్గతం[1] = (stbi_uc)g;
      బహిర్గతం[2] = (stbi_uc)b;
      బహిర్గతం[3] = 255;
      బహిర్గతం += అడుగు;
   }
}
#అయితే_ముగింపు

// set up the kernels
స్థిర శూన్య stbi_అమరిక_jpeg(stbi__jpeg *j)
{
   j->idct_ఖండం_kernel = stbi_idct_ఖండం;
   j->YCb_Cr_గమ్యం_RGB_kernel = stbi_YCb_Cr_గమ్యం_RGB_అడ్డువరుస;
   j->resample_అడ్డువరుస_hv_2_kernel = stbi_resample_అడ్డువరుస_hv_2;

#నిర్వచితమైతే STBI_SSE2
   ఒకవేళ (stbi_sse2_అందుబాటు()) {
      j->idct_ఖండం_kernel = stbi__idct_simd;
      j->YCb_Cr_గమ్యం_RGB_kernel = stbi_YCb_Cr_గమ్యం_RGB_simd;
      j->resample_అడ్డువరుస_hv_2_kernel = stbi_resample_అడ్డువరుస_hv_2_simd;
   }
#అయితే_ముగింపు

#నిర్వచితమైతే STBI_NEON
   j->idct_ఖండం_kernel = stbi__idct_simd;
   j->YCb_Cr_గమ్యం_RGB_kernel = stbi_YCb_Cr_గమ్యం_RGB_simd;
   j->resample_అడ్డువరుస_hv_2_kernel = stbi_resample_అడ్డువరుస_hv_2_simd;
#అయితే_ముగింపు
}

// clean up the temporary component buffers
స్థిర శూన్య stbi__cleanup_jpeg(stbi__jpeg *j)
{
   stbi__free_jpeg_components(j, j->s->img_n, 0);
}

రకనిర్వచనం నిర్మాణం
{
   resample_అడ్డువరుస_func resample;
   stbi_uc *line0,*line1;
   పూర్ణాంకం hs,vs;   // expansion factor in each axis
   పూర్ణాంకం w_lores; // horizontal pixels pre-expansion
   పూర్ణాంకం ystep;   // how far through vertical expansion we are
   పూర్ణాంకం ypos;    // which pre-expansion row we're on
} stbi__resample;

// fast 0..255 * 0..255 => 0..255 rounded multiplication
స్థిర stbi_uc stbi__blinn_8x8(stbi_uc x, stbi_uc y)
{
   గుర్తులేని పూర్ణాంకం t = x*y + 128;
   తిరిగి (stbi_uc) ((t + (t >>8)) >> 8);
}

స్థిర stbi_uc *ఎక్కించు_jpeg_image(stbi__jpeg *z, పూర్ణాంకం *బహిర్గతం_x, పూర్ణాంకం *బహిర్గతం_y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   పూర్ణాంకం n, decode_n, is_rgb;
   z->s->img_n = 0; // make stbi__cleanup_jpeg safe

   // validate req_comp
   ఒకవేళ (req_comp < 0 || req_comp > 4) తిరిగి stbi__errpuc("bad req_comp", "Internal error");

   // load a jpeg image from whichever source, but leave in YCbCr format
   ఒకవేళ (!stbi__decode_jpeg_image(z)) { stbi__cleanup_jpeg(z); తిరిగి ఖాళీ; }

   // determine actual number of components to generate
   n = req_comp ? req_comp : z->s->img_n >= 3 ? 3 : 1;

   is_rgb = z->s->img_n == 3 && (z->rgb == 3 || (z->app14_color_transform == 0 && !z->jfif));

   ఒకవేళ (z->s->img_n == 3 && n < 3 && !is_rgb)
      decode_n = 1;
   లేకపోతే
      decode_n = z->s->img_n;

   // resample and color-convert
   {
      పూర్ణాంకం k;
      గుర్తులేని పూర్ణాంకం i,j;
      stbi_uc *output;
      stbi_uc *coutput[4];

      stbi__resample res_comp[4];

      కొరకు (k=0; k < decode_n; ++k) {
         stbi__resample *r = &res_comp[k];

         // allocate line buffer big enough for upsampling off the edges
         // with upsample factor of 4
         z->img_comp[k].linebuf = (stbi_uc *) stbi__malloc(z->s->img_x + 3);
         ఒకవేళ (!z->img_comp[k].linebuf) { stbi__cleanup_jpeg(z); తిరిగి stbi__errpuc("outofmem", "Out of memory"); }

         r->hs      = z->img_h_max / z->img_comp[k].h;
         r->vs      = z->img_v_max / z->img_comp[k].v;
         r->ystep   = r->vs >> 1;
         r->w_lores = (z->s->img_x + r->hs-1) / r->hs;
         r->ypos    = 0;
         r->line0   = r->line1 = z->img_comp[k].దత్తాంశం;

         ఒకవేళ      (r->hs == 1 && r->vs == 1) r->resample = resample_అడ్డువరుస_1;
         లేకపోతే ఒకవేళ (r->hs == 1 && r->vs == 2) r->resample = stbi_resample_అడ్డువరుస_v_2;
         లేకపోతే ఒకవేళ (r->hs == 2 && r->vs == 1) r->resample = stbi_resample_అడ్డువరుస_h_2;
         లేకపోతే ఒకవేళ (r->hs == 2 && r->vs == 2) r->resample = z->resample_అడ్డువరుస_hv_2_kernel;
         లేకపోతే                               r->resample = stbi_resample_అడ్డువరుస_generic;
      }

      // can't error after this so, this is safe
      output = (stbi_uc *) stbi__malloc_mad3(n, z->s->img_x, z->s->img_y, 1);
      ఒకవేళ (!output) { stbi__cleanup_jpeg(z); తిరిగి stbi__errpuc("outofmem", "Out of memory"); }

      // now go ahead and resample
      కొరకు (j=0; j < z->s->img_y; ++j) {
         stbi_uc *బహిర్గతం = output + n * z->s->img_x * j;
         కొరకు (k=0; k < decode_n; ++k) {
            stbi__resample *r = &res_comp[k];
            పూర్ణాంకం y_bot = r->ystep >= (r->vs >> 1);
            coutput[k] = r->resample(z->img_comp[k].linebuf,
                                     y_bot ? r->line1 : r->line0,
                                     y_bot ? r->line0 : r->line1,
                                     r->w_lores, r->hs);
            ఒకవేళ (++r->ystep >= r->vs) {
               r->ystep = 0;
               r->line0 = r->line1;
               ఒకవేళ (++r->ypos < z->img_comp[k].y)
                  r->line1 += z->img_comp[k].w2;
            }
         }
         ఒకవేళ (n >= 3) {
            stbi_uc *y = coutput[0];
            ఒకవేళ (z->s->img_n == 3) {
               ఒకవేళ (is_rgb) {
                  కొరకు (i=0; i < z->s->img_x; ++i) {
                     బహిర్గతం[0] = y[i];
                     బహిర్గతం[1] = coutput[1][i];
                     బహిర్గతం[2] = coutput[2][i];
                     బహిర్గతం[3] = 255;
                     బహిర్గతం += n;
                  }
               } లేకపోతే {
                  z->YCb_Cr_గమ్యం_RGB_kernel(బహిర్గతం, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } లేకపోతే ఒకవేళ (z->s->img_n == 4) {
               ఒకవేళ (z->app14_color_transform == 0) { // CMYK
                  కొరకు (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     బహిర్గతం[0] = stbi__blinn_8x8(coutput[0][i], m);
                     బహిర్గతం[1] = stbi__blinn_8x8(coutput[1][i], m);
                     బహిర్గతం[2] = stbi__blinn_8x8(coutput[2][i], m);
                     బహిర్గతం[3] = 255;
                     బహిర్గతం += n;
                  }
               } లేకపోతే ఒకవేళ (z->app14_color_transform == 2) { // YCCK
                  z->YCb_Cr_గమ్యం_RGB_kernel(బహిర్గతం, y, coutput[1], coutput[2], z->s->img_x, n);
                  కొరకు (i=0; i < z->s->img_x; ++i) {
                     stbi_uc m = coutput[3][i];
                     బహిర్గతం[0] = stbi__blinn_8x8(255 - బహిర్గతం[0], m);
                     బహిర్గతం[1] = stbi__blinn_8x8(255 - బహిర్గతం[1], m);
                     బహిర్గతం[2] = stbi__blinn_8x8(255 - బహిర్గతం[2], m);
                     బహిర్గతం += n;
                  }
               } లేకపోతే { // YCbCr + alpha?  Ignore the fourth channel for now
                  z->YCb_Cr_గమ్యం_RGB_kernel(బహిర్గతం, y, coutput[1], coutput[2], z->s->img_x, n);
               }
            } లేకపోతే
               కొరకు (i=0; i < z->s->img_x; ++i) {
                  బహిర్గతం[0] = బహిర్గతం[1] = బహిర్గతం[2] = y[i];
                  బహిర్గతం[3] = 255; // not used if n==3
                  బహిర్గతం += n;
               }
         } లేకపోతే {
            ఒకవేళ (is_rgb) {
               ఒకవేళ (n == 1)
                  కొరకు (i=0; i < z->s->img_x; ++i)
                     *బహిర్గతం++ = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
               లేకపోతే {
                  కొరకు (i=0; i < z->s->img_x; ++i, బహిర్గతం += 2) {
                     బహిర్గతం[0] = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
                     బహిర్గతం[1] = 255;
                  }
               }
            } లేకపోతే ఒకవేళ (z->s->img_n == 4 && z->app14_color_transform == 0) {
               కొరకు (i=0; i < z->s->img_x; ++i) {
                  stbi_uc m = coutput[3][i];
                  stbi_uc r = stbi__blinn_8x8(coutput[0][i], m);
                  stbi_uc g = stbi__blinn_8x8(coutput[1][i], m);
                  stbi_uc b = stbi__blinn_8x8(coutput[2][i], m);
                  బహిర్గతం[0] = stbi__compute_y(r, g, b);
                  బహిర్గతం[1] = 255;
                  బహిర్గతం += n;
               }
            } లేకపోతే ఒకవేళ (z->s->img_n == 4 && z->app14_color_transform == 2) {
               కొరకు (i=0; i < z->s->img_x; ++i) {
                  బహిర్గతం[0] = stbi__blinn_8x8(255 - coutput[0][i], coutput[3][i]);
                  బహిర్గతం[1] = 255;
                  బహిర్గతం += n;
               }
            } లేకపోతే {
               stbi_uc *y = coutput[0];
               ఒకవేళ (n == 1)
                  కొరకు (i=0; i < z->s->img_x; ++i) బహిర్గతం[i] = y[i];
               లేకపోతే
                  కొరకు (i=0; i < z->s->img_x; ++i) *బహిర్గతం++ = y[i], *బహిర్గతం++ = 255;
            }
         }
      }
      stbi__cleanup_jpeg(z);
      *బహిర్గతం_x = z->s->img_x;
      *బహిర్గతం_y = z->s->img_y;
      ఒకవేళ (comp) *comp = z->s->img_n >= 3 ? 3 : 1; // report original components, not output
      తిరిగి output;
   }
}

స్థిర శూన్య *stbi_jpeg_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   గుర్తులేని అక్షరం* ఫలితం;
   stbi__jpeg* j = (stbi__jpeg*) stbi__malloc(పరిమాణం(stbi__jpeg));
   STBI_NOTUSED(ri);
   j->s = s;
   stbi_అమరిక_jpeg(j);
   ఫలితం = ఎక్కించు_jpeg_image(j, x,y,comp,req_comp);
   STBI_FREE(j);
   తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం stbi__jpeg_test(stbi__context *s)
{
   పూర్ణాంకం r;
   stbi__jpeg* j = (stbi__jpeg*)stbi__malloc(పరిమాణం(stbi__jpeg));
   j->s = s;
   stbi_అమరిక_jpeg(j);
   r = stbi_decode_jpeg_శీర్షిక(j, STBI_పరిశీలన_type);
   stbi__rewind(s);
   STBI_FREE(j);
   తిరిగి r;
}

స్థిర పూర్ణాంకం stbi__jpeg_info_raw(stbi__jpeg *j, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   ఒకవేళ (!stbi_decode_jpeg_శీర్షిక(j, STBI_పరిశీలన_శీర్షిక)) {
      stbi__rewind( j->s );
      తిరిగి 0;
   }
   ఒకవేళ (x) *x = j->s->img_x;
   ఒకవేళ (y) *y = j->s->img_y;
   ఒకవేళ (comp) *comp = j->s->img_n >= 3 ? 3 : 1;
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__jpeg_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   పూర్ణాంకం ఫలితం;
   stbi__jpeg* j = (stbi__jpeg*) (stbi__malloc(పరిమాణం(stbi__jpeg)));
   j->s = s;
   ఫలితం = stbi__jpeg_info_raw(j, x, y, comp);
   STBI_FREE(j);
   తిరిగి ఫలితం;
}
#అయితే_ముగింపు

// public domain zlib decode    v0.2  Sean Barrett 2006-11-18
//    simple implementation
//      - all input must be provided in an upfront buffer
//      - all output is written to a single output buffer (can malloc/realloc)
//    performance
//      - fast huffman

#నిర్వచితం_కాకపోతే STBI_NO_ZLIB

// fast-way is faster to check than jpeg huffman, but slow way is slower
#నిర్వచించు STBI_ZFAST_బిట్లు  9 // accelerate all cases in default tables
#నిర్వచించు STBI_ZFAST_ముసుగు  ((1 << STBI_ZFAST_బిట్లు) - 1)

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
రకనిర్వచనం నిర్మాణం
{
   stbi__uint16 fast[1 << STBI_ZFAST_బిట్లు];
   stbi__uint16 firstcode[16];
   పూర్ణాంకం maxcode[17];
   stbi__uint16 firstsymbol[16];
   stbi_uc  కొలత[288];
   stbi__uint16 విలువ[288];
} stbi__zhuffman;

stbi_inline స్థిర పూర్ణాంకం stbi__bitreverse16(పూర్ణాంకం n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  తిరిగి n;
}

stbi_inline స్థిర పూర్ణాంకం stbi_బిట్టు_తిరగేయి(పూర్ణాంకం v, పూర్ణాంకం బిట్లు)
{
   STBI_ASSERT(బిట్లు <= 16);
   // to bit reverse n bits, reverse 16 and shift
   // e.g. 11 bits, bit reverse and shift away 5
   తిరిగి stbi__bitreverse16(v) >> (16-బిట్లు);
}

స్థిర పూర్ణాంకం stbi__zbuild_huffman(stbi__zhuffman *z, స్థిరరాశి stbi_uc *sizelist, పూర్ణాంకం అంకె)
{
   పూర్ణాంకం i,k=0;
   పూర్ణాంకం సంకేతం_2, తదుపరి_సంకేతం[16], sizes[17];

   // DEFLATE spec for generating codes
   memset(sizes, 0, పరిమాణం(sizes));
   memset(z->fast, 0, పరిమాణం(z->fast));
   కొరకు (i=0; i < అంకె; ++i)
      ++sizes[sizelist[i]];
   sizes[0] = 0;
   కొరకు (i=1; i < 16; ++i)
      ఒకవేళ (sizes[i] > (1 << i))
         తిరిగి stbi__err("bad sizes", "Corrupt PNG");
   సంకేతం_2 = 0;
   కొరకు (i=1; i < 16; ++i) {
      తదుపరి_సంకేతం[i] = సంకేతం_2;
      z->firstcode[i] = (stbi__uint16) సంకేతం_2;
      z->firstsymbol[i] = (stbi__uint16) k;
      సంకేతం_2 = (సంకేతం_2 + sizes[i]);
      ఒకవేళ (sizes[i])
         ఒకవేళ (సంకేతం_2-1 >= (1 << i)) తిరిగి stbi__err("bad codelengths","Corrupt PNG");
      z->maxcode[i] = సంకేతం_2 << (16-i); // preshift for inner loop
      సంకేతం_2 <<= 1;
      k += sizes[i];
   }
   z->maxcode[16] = 0x10000; // sentinel
   కొరకు (i=0; i < అంకె; ++i) {
      పూర్ణాంకం s = sizelist[i];
      ఒకవేళ (s) {
         పూర్ణాంకం c = తదుపరి_సంకేతం[s] - z->firstcode[s] + z->firstsymbol[s];
         stbi__uint16 fastv = (stbi__uint16) ((s << 9) | i);
         z->కొలత [c] = (stbi_uc     ) s;
         z->విలువ[c] = (stbi__uint16) i;
         ఒకవేళ (s <= STBI_ZFAST_బిట్లు) {
            పూర్ణాంకం j = stbi_బిట్టు_తిరగేయి(తదుపరి_సంకేతం[s],s);
            అయితే (j < (1 << STBI_ZFAST_బిట్లు)) {
               z->fast[j] = fastv;
               j += (1 << s);
            }
         }
         ++తదుపరి_సంకేతం[s];
      }
   }
   తిరిగి 1;
}

// zlib-from-memory implementation for PNG reading
//    because PNG allows splitting the zlib stream arbitrarily,
//    and it's annoying structurally to have PNG call ZLIB call PNG,
//    we require PNG read all the IDATs and combine them into a single
//    memory buffer

రకనిర్వచనం నిర్మాణం
{
   stbi_uc *zbuffer, *zbuffer_చివర;
   పూర్ణాంకం అంకె_బిట్లు;
   stbi__uint32 సంకేతం_బఫరు;

   అక్షరం *zout;
   అక్షరం *zout_ప్రారంభం;
   అక్షరం *zout_చివర;
   పూర్ణాంకం   z_expandable;

   stbi__zhuffman z_నిడివి, z_దూరం;
} stbi__zbuf;

stbi_inline స్థిర stbi_uc stbi__zget8(stbi__zbuf *z)
{
   ఒకవేళ (z->zbuffer >= z->zbuffer_చివర) తిరిగి 0;
   తిరిగి *z->zbuffer++;
}

స్థిర శూన్య stbi_fill_బిట్లు(stbi__zbuf *z)
{
   చేయి {
      STBI_ASSERT(z->సంకేతం_బఫరు < (1U << z->అంకె_బిట్లు));
      z->సంకేతం_బఫరు |= (గుర్తులేని పూర్ణాంకం) stbi__zget8(z) << z->అంకె_బిట్లు;
      z->అంకె_బిట్లు += 8;
   } అయితే (z->అంకె_బిట్లు <= 24);
}

stbi_inline స్థిర గుర్తులేని పూర్ణాంకం stbi__zreceive(stbi__zbuf *z, పూర్ణాంకం n)
{
   గుర్తులేని పూర్ణాంకం k;
   ఒకవేళ (z->అంకె_బిట్లు < n) stbi_fill_బిట్లు(z);
   k = z->సంకేతం_బఫరు & ((1 << n) - 1);
   z->సంకేతం_బఫరు >>= n;
   z->అంకె_బిట్లు -= n;
   తిరిగి k;
}

స్థిర పూర్ణాంకం stbi__zhuffman_decode_slowpath(stbi__zbuf *a, stbi__zhuffman *z)
{
   పూర్ణాంకం b,s,k;
   // not resolved by fast table, so compute it the slow way
   // use jpeg approach, which requires MSbits at top
   k = stbi_బిట్టు_తిరగేయి(a->సంకేతం_బఫరు, 16);
   కొరకు (s=STBI_ZFAST_బిట్లు+1; ; ++s)
      ఒకవేళ (k < z->maxcode[s])
         ఆపు;
   ఒకవేళ (s == 16) తిరిగి -1; // invalid code!
   // code size is s, so:
   b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
   STBI_ASSERT(z->కొలత[b] == s);
   a->సంకేతం_బఫరు >>= s;
   a->అంకె_బిట్లు -= s;
   తిరిగి z->విలువ[b];
}

stbi_inline స్థిర పూర్ణాంకం stbi__zhuffman_decode(stbi__zbuf *a, stbi__zhuffman *z)
{
   పూర్ణాంకం b,s;
   ఒకవేళ (a->అంకె_బిట్లు < 16) stbi_fill_బిట్లు(a);
   b = z->fast[a->సంకేతం_బఫరు & STBI_ZFAST_ముసుగు];
   ఒకవేళ (b) {
      s = b >> 9;
      a->సంకేతం_బఫరు >>= s;
      a->అంకె_బిట్లు -= s;
      తిరిగి b & 511;
   }
   తిరిగి stbi__zhuffman_decode_slowpath(a, z);
}

స్థిర పూర్ణాంకం stbi__zexpand(stbi__zbuf *z, అక్షరం *zout, పూర్ణాంకం n)  // need to make room for n bytes
{
   అక్షరం *q;
   పూర్ణాంకం cur, హద్దు, old_హద్దు;
   z->zout = zout;
   ఒకవేళ (!z->z_expandable) తిరిగి stbi__err("output buffer limit","Corrupt PNG");
   cur   = (పూర్ణాంకం) (z->zout     - z->zout_ప్రారంభం);
   హద్దు = old_హద్దు = (పూర్ణాంకం) (z->zout_చివర - z->zout_ప్రారంభం);
   అయితే (cur + n > హద్దు)
      హద్దు *= 2;
   q = (అక్షరం *) STBI_REALLOC_SIZED(z->zout_ప్రారంభం, old_హద్దు, హద్దు);
   STBI_NOTUSED(old_హద్దు);
   ఒకవేళ (q == ఖాళీ) తిరిగి stbi__err("outofmem", "Out of memory");
   z->zout_ప్రారంభం = q;
   z->zout       = q + cur;
   z->zout_చివర   = q + హద్దు;
   తిరిగి 1;
}

స్థిర స్థిరరాశి పూర్ణాంకం stbi_zlength_పునాది[31] = {
   3,4,5,6,7,8,9,10,11,13,
   15,17,19,23,27,31,35,43,51,59,
   67,83,99,115,131,163,195,227,258,0,0 };

స్థిర స్థిరరాశి పూర్ణాంకం stbi__zlength_extra[31]=
{ 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };

స్థిర స్థిరరాశి పూర్ణాంకం stbi_zdist_పునాది[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0};

స్థిర స్థిరరాశి పూర్ణాంకం stbi__zdist_extra[32] =
{ 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

స్థిర పూర్ణాంకం stbi_parse_huffman_ఖండం(stbi__zbuf *a)
{
   అక్షరం *zout = a->zout;
   కొరకు(;;) {
      పూర్ణాంకం z = stbi__zhuffman_decode(a, &a->z_నిడివి);
      ఒకవేళ (z < 256) {
         ఒకవేళ (z < 0) తిరిగి stbi__err("bad huffman code","Corrupt PNG"); // error in huffman codes
         ఒకవేళ (zout >= a->zout_చివర) {
            ఒకవేళ (!stbi__zexpand(a, zout, 1)) తిరిగి 0;
            zout = a->zout;
         }
         *zout++ = (అక్షరం) z;
      } లేకపోతే {
         stbi_uc *p;
         పూర్ణాంకం len,dist;
         ఒకవేళ (z == 256) {
            a->zout = zout;
            తిరిగి 1;
         }
         z -= 257;
         len = stbi_zlength_పునాది[z];
         ఒకవేళ (stbi__zlength_extra[z]) len += stbi__zreceive(a, stbi__zlength_extra[z]);
         z = stbi__zhuffman_decode(a, &a->z_దూరం);
         ఒకవేళ (z < 0) తిరిగి stbi__err("bad huffman code","Corrupt PNG");
         dist = stbi_zdist_పునాది[z];
         ఒకవేళ (stbi__zdist_extra[z]) dist += stbi__zreceive(a, stbi__zdist_extra[z]);
         ఒకవేళ (zout - a->zout_ప్రారంభం < dist) తిరిగి stbi__err("bad dist","Corrupt PNG");
         ఒకవేళ (zout + len > a->zout_చివర) {
            ఒకవేళ (!stbi__zexpand(a, zout, len)) తిరిగి 0;
            zout = a->zout;
         }
         p = (stbi_uc *) (zout - dist);
         ఒకవేళ (dist == 1) { // run of one byte; common in images.
            stbi_uc v = *p;
            ఒకవేళ (len) { చేయి *zout++ = v; అయితే (--len); }
         } లేకపోతే {
            ఒకవేళ (len) { చేయి *zout++ = *p++; అయితే (--len); }
         }
      }
   }
}

స్థిర పూర్ణాంకం stbi__compute_huffman_codes(stbi__zbuf *a)
{
   స్థిర స్థిరరాశి stbi_uc నిడివి_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
   stbi__zhuffman z_codelength;
   stbi_uc lencodes[286+32+137];//padding for maximum single op
   stbi_uc codelength_sizes[19];
   పూర్ణాంకం i,n;

   పూర్ణాంకం hlit  = stbi__zreceive(a,5) + 257;
   పూర్ణాంకం hdist = stbi__zreceive(a,5) + 1;
   పూర్ణాంకం hclen = stbi__zreceive(a,4) + 4;
   పూర్ణాంకం ntot  = hlit + hdist;

   memset(codelength_sizes, 0, పరిమాణం(codelength_sizes));
   కొరకు (i=0; i < hclen; ++i) {
      పూర్ణాంకం s = stbi__zreceive(a,3);
      codelength_sizes[నిడివి_dezigzag[i]] = (stbi_uc) s;
   }
   ఒకవేళ (!stbi__zbuild_huffman(&z_codelength, codelength_sizes, 19)) తిరిగి 0;

   n = 0;
   అయితే (n < ntot) {
      పూర్ణాంకం c = stbi__zhuffman_decode(a, &z_codelength);
      ఒకవేళ (c < 0 || c >= 19) తిరిగి stbi__err("bad codelengths", "Corrupt PNG");
      ఒకవేళ (c < 16)
         lencodes[n++] = (stbi_uc) c;
      లేకపోతే {
         stbi_uc fill = 0;
         ఒకవేళ (c == 16) {
            c = stbi__zreceive(a,2)+3;
            ఒకవేళ (n == 0) తిరిగి stbi__err("bad codelengths", "Corrupt PNG");
            fill = lencodes[n-1];
         } లేకపోతే ఒకవేళ (c == 17)
            c = stbi__zreceive(a,3)+3;
         లేకపోతే {
            STBI_ASSERT(c == 18);
            c = stbi__zreceive(a,7)+11;
         }
         ఒకవేళ (ntot - n < c) తిరిగి stbi__err("bad codelengths", "Corrupt PNG");
         memset(lencodes+n, fill, c);
         n += c;
      }
   }
   ఒకవేళ (n != ntot) తిరిగి stbi__err("bad codelengths","Corrupt PNG");
   ఒకవేళ (!stbi__zbuild_huffman(&a->z_నిడివి, lencodes, hlit)) తిరిగి 0;
   ఒకవేళ (!stbi__zbuild_huffman(&a->z_దూరం, lencodes+hlit, hdist)) తిరిగి 0;
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi_parse_uncompressed_ఖండం(stbi__zbuf *a)
{
   stbi_uc శీర్షిక[4];
   పూర్ణాంకం len,nlen,k;
   ఒకవేళ (a->అంకె_బిట్లు & 7)
      stbi__zreceive(a, a->అంకె_బిట్లు & 7); // discard
   // drain the bit-packed data into header
   k = 0;
   అయితే (a->అంకె_బిట్లు > 0) {
      శీర్షిక[k++] = (stbi_uc) (a->సంకేతం_బఫరు & 255); // suppress MSVC run-time check
      a->సంకేతం_బఫరు >>= 8;
      a->అంకె_బిట్లు -= 8;
   }
   STBI_ASSERT(a->అంకె_బిట్లు == 0);
   // now fill header the normal way
   అయితే (k < 4)
      శీర్షిక[k++] = stbi__zget8(a);
   len  = శీర్షిక[1] * 256 + శీర్షిక[0];
   nlen = శీర్షిక[3] * 256 + శీర్షిక[2];
   ఒకవేళ (nlen != (len ^ 0xffff)) తిరిగి stbi__err("zlib corrupt","Corrupt PNG");
   ఒకవేళ (a->zbuffer + len > a->zbuffer_చివర) తిరిగి stbi__err("read past buffer","Corrupt PNG");
   ఒకవేళ (a->zout + len > a->zout_చివర)
      ఒకవేళ (!stbi__zexpand(a, a->zout, len)) తిరిగి 0;
   జ్ఞాపకం_నకలు(a->zout, a->zbuffer, len);
   a->zbuffer += len;
   a->zout += len;
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi_parse_zlib_శీర్షిక(stbi__zbuf *a)
{
   పూర్ణాంకం cmf   = stbi__zget8(a);
   పూర్ణాంకం cm    = cmf & 15;
   /* int cinfo = cmf >> 4; */
   పూర్ణాంకం flg   = stbi__zget8(a);
   ఒకవేళ ((cmf*256+flg) % 31 != 0) తిరిగి stbi__err("bad zlib header","Corrupt PNG"); // zlib spec
   ఒకవేళ (flg & 32) తిరిగి stbi__err("no preset dict","Corrupt PNG"); // preset dictionary not allowed in png
   ఒకవేళ (cm != 8) తిరిగి stbi__err("bad compression","Corrupt PNG"); // DEFLATE required for png
   // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
   తిరిగి 1;
}

స్థిర స్థిరరాశి stbi_uc stbi_zdefault_నిడివి[288] =
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
స్థిర స్థిరరాశి stbi_uc stbi_zdefault_దూరం[32] =
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

స్థిర పూర్ణాంకం stbi__parse_zlib(stbi__zbuf *a, పూర్ణాంకం parse_శీర్షిక)
{
   పూర్ణాంకం final, type;
   ఒకవేళ (parse_శీర్షిక)
      ఒకవేళ (!stbi_parse_zlib_శీర్షిక(a)) తిరిగి 0;
   a->అంకె_బిట్లు = 0;
   a->సంకేతం_బఫరు = 0;
   చేయి {
      final = stbi__zreceive(a,1);
      type = stbi__zreceive(a,2);
      ఒకవేళ (type == 0) {
         ఒకవేళ (!stbi_parse_uncompressed_ఖండం(a)) తిరిగి 0;
      } లేకపోతే ఒకవేళ (type == 3) {
         తిరిగి 0;
      } లేకపోతే {
         ఒకవేళ (type == 1) {
            // use fixed code lengths
            ఒకవేళ (!stbi__zbuild_huffman(&a->z_నిడివి  , stbi_zdefault_నిడివి  , 288)) తిరిగి 0;
            ఒకవేళ (!stbi__zbuild_huffman(&a->z_దూరం, stbi_zdefault_దూరం,  32)) తిరిగి 0;
         } లేకపోతే {
            ఒకవేళ (!stbi__compute_huffman_codes(a)) తిరిగి 0;
         }
         ఒకవేళ (!stbi_parse_huffman_ఖండం(a)) తిరిగి 0;
      }
   } అయితే (!final);
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__do_zlib(stbi__zbuf *a, అక్షరం *obuf, పూర్ణాంకం olen, పూర్ణాంకం ఘాతాంకం, పూర్ణాంకం parse_శీర్షిక)
{
   a->zout_ప్రారంభం = obuf;
   a->zout       = obuf;
   a->zout_చివర   = obuf + olen;
   a->z_expandable = ఘాతాంకం;

   తిరిగి stbi__parse_zlib(a, parse_శీర్షిక);
}

STBIDEF అక్షరం *stbi_zlib_decode_malloc_guesssize(స్థిరరాశి అక్షరం *బఫరు, పూర్ణాంకం len, పూర్ణాంకం initial_కొలత, పూర్ణాంకం *outlen)
{
   stbi__zbuf a;
   అక్షరం *p = (అక్షరం *) stbi__malloc(initial_కొలత);
   ఒకవేళ (p == ఖాళీ) తిరిగి ఖాళీ;
   a.zbuffer = (stbi_uc *) బఫరు;
   a.zbuffer_చివర = (stbi_uc *) బఫరు + len;
   ఒకవేళ (stbi__do_zlib(&a, p, initial_కొలత, 1, 1)) {
      ఒకవేళ (outlen) *outlen = (పూర్ణాంకం) (a.zout - a.zout_ప్రారంభం);
      తిరిగి a.zout_ప్రారంభం;
   } లేకపోతే {
      STBI_FREE(a.zout_ప్రారంభం);
      తిరిగి ఖాళీ;
   }
}

STBIDEF అక్షరం *stbi_zlib_decode_malloc(అక్షరం స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *outlen)
{
   తిరిగి stbi_zlib_decode_malloc_guesssize(బఫరు, len, 16384, outlen);
}

STBIDEF అక్షరం *stbi_zlib_decode_malloc_guesssize_headerflag(స్థిరరాశి అక్షరం *బఫరు, పూర్ణాంకం len, పూర్ణాంకం initial_కొలత, పూర్ణాంకం *outlen, పూర్ణాంకం parse_శీర్షిక)
{
   stbi__zbuf a;
   అక్షరం *p = (అక్షరం *) stbi__malloc(initial_కొలత);
   ఒకవేళ (p == ఖాళీ) తిరిగి ఖాళీ;
   a.zbuffer = (stbi_uc *) బఫరు;
   a.zbuffer_చివర = (stbi_uc *) బఫరు + len;
   ఒకవేళ (stbi__do_zlib(&a, p, initial_కొలత, 1, parse_శీర్షిక)) {
      ఒకవేళ (outlen) *outlen = (పూర్ణాంకం) (a.zout - a.zout_ప్రారంభం);
      తిరిగి a.zout_ప్రారంభం;
   } లేకపోతే {
      STBI_FREE(a.zout_ప్రారంభం);
      తిరిగి ఖాళీ;
   }
}

STBIDEF పూర్ణాంకం stbi_zlib_decode_బఫరు(అక్షరం *obuffer, పూర్ణాంకం olen, అక్షరం స్థిరరాశి *ibuffer, పూర్ణాంకం ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_చివర = (stbi_uc *) ibuffer + ilen;
   ఒకవేళ (stbi__do_zlib(&a, obuffer, olen, 0, 1))
      తిరిగి (పూర్ణాంకం) (a.zout - a.zout_ప్రారంభం);
   లేకపోతే
      తిరిగి -1;
}

STBIDEF అక్షరం *stbi_zlib_decode_noheader_malloc(అక్షరం స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *outlen)
{
   stbi__zbuf a;
   అక్షరం *p = (అక్షరం *) stbi__malloc(16384);
   ఒకవేళ (p == ఖాళీ) తిరిగి ఖాళీ;
   a.zbuffer = (stbi_uc *) బఫరు;
   a.zbuffer_చివర = (stbi_uc *) బఫరు+len;
   ఒకవేళ (stbi__do_zlib(&a, p, 16384, 1, 0)) {
      ఒకవేళ (outlen) *outlen = (పూర్ణాంకం) (a.zout - a.zout_ప్రారంభం);
      తిరిగి a.zout_ప్రారంభం;
   } లేకపోతే {
      STBI_FREE(a.zout_ప్రారంభం);
      తిరిగి ఖాళీ;
   }
}

STBIDEF పూర్ణాంకం stbi_zlib_decode_noheader_బఫరు(అక్షరం *obuffer, పూర్ణాంకం olen, స్థిరరాశి అక్షరం *ibuffer, పూర్ణాంకం ilen)
{
   stbi__zbuf a;
   a.zbuffer = (stbi_uc *) ibuffer;
   a.zbuffer_చివర = (stbi_uc *) ibuffer + ilen;
   ఒకవేళ (stbi__do_zlib(&a, obuffer, olen, 0, 0))
      తిరిగి (పూర్ణాంకం) (a.zout - a.zout_ప్రారంభం);
   లేకపోతే
      తిరిగి -1;
}
#అయితే_ముగింపు

// public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
//    simple implementation
//      - only 8-bit samples
//      - no CRC checking
//      - allocates lots of intermediate memory
//        - avoids problem of streaming data between subsystems
//        - avoids explicit window management
//    performance
//      - uses stb_zlib, a PD zlib implementation with fast huffman decoding

#నిర్వచితం_కాకపోతే STBI_NO_PNG
రకనిర్వచనం నిర్మాణం
{
   stbi__uint32 నిడివి;
   stbi__uint32 type;
} stbi__pngchunk;

స్థిర stbi__pngchunk stbi_get_chunk_శీర్షిక(stbi__context *s)
{
   stbi__pngchunk c;
   c.నిడివి = stbi__get32be(s);
   c.type   = stbi__get32be(s);
   తిరిగి c;
}

స్థిర పూర్ణాంకం stbi_check_png_శీర్షిక(stbi__context *s)
{
   స్థిర స్థిరరాశి stbi_uc png_sig[8] = { 137,80,78,71,13,10,26,10 };
   పూర్ణాంకం i;
   కొరకు (i=0; i < 8; ++i)
      ఒకవేళ (stbi__get8(s) != png_sig[i]) తిరిగి stbi__err("bad png sig","Not a PNG");
   తిరిగి 1;
}

రకనిర్వచనం నిర్మాణం
{
   stbi__context *s;
   stbi_uc *idata, *expanded, *బహిర్గతం;
   పూర్ణాంకం లోతు;
} stbi__png;


గణన {
   STBI__F_none=0,
   STBI__F_sub=1,
   STBI_F_పైకి=2,
   STBI__F_avg=3,
   STBI__F_paeth=4,
   // synthetic filters used for first scanline to avoid needing a dummy row of 0s
   STBI_F_avg_మొదటి,
   STBI_F_paeth_మొదటి
};

స్థిర stbi_uc మొదటి_అడ్డువరుస_filter[5] =
{
   STBI__F_none,
   STBI__F_sub,
   STBI__F_none,
   STBI_F_avg_మొదటి,
   STBI_F_paeth_మొదటి
};

స్థిర పూర్ణాంకం stbi__paeth(పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం c)
{
   పూర్ణాంకం p = a + b - c;
   పూర్ణాంకం pa = నిరపేక్ష(p-a);
   పూర్ణాంకం pb = నిరపేక్ష(p-b);
   పూర్ణాంకం pc = నిరపేక్ష(p-c);
   ఒకవేళ (pa <= pb && pa <= pc) తిరిగి a;
   ఒకవేళ (pb <= pc) తిరిగి b;
   తిరిగి c;
}

స్థిర స్థిరరాశి stbi_uc stbi_లోతు_scale_పట్టిక[9] = { 0, 0xff, 0x55, 0, 0x11, 0,0,0, 0x01 };

// create the png data from post-deflated data
స్థిర పూర్ణాంకం stbi__create_png_image_raw(stbi__png *a, stbi_uc *raw, stbi__uint32 raw_len, పూర్ణాంకం బహిర్గతం_n, stbi__uint32 x, stbi__uint32 y, పూర్ణాంకం లోతు, పూర్ణాంకం color)
{
   పూర్ణాంకం బైట్లు = (లోతు == 16? 2 : 1);
   stbi__context *s = a->s;
   stbi__uint32 i,j,stride = x*బహిర్గతం_n*బైట్లు;
   stbi__uint32 img_len, img_వెడల్పు_బైట్లు;
   పూర్ణాంకం k;
   పూర్ణాంకం img_n = s->img_n; // copy it into a local for later

   పూర్ణాంకం output_బైట్లు = బహిర్గతం_n*బైట్లు;
   పూర్ణాంకం filter_బైట్లు = img_n*బైట్లు;
   పూర్ణాంకం వెడల్పు = x;

   STBI_ASSERT(బహిర్గతం_n == s->img_n || బహిర్గతం_n == s->img_n+1);
   a->బహిర్గతం = (stbi_uc *) stbi__malloc_mad3(x, y, output_బైట్లు, 0); // extra bytes to write off the end into
   ఒకవేళ (!a->బహిర్గతం) తిరిగి stbi__err("outofmem", "Out of memory");

   ఒకవేళ (!stbi__mad3sizes_valid(img_n, x, లోతు, 7)) తిరిగి stbi__err("too large", "Corrupt PNG");
   img_వెడల్పు_బైట్లు = (((img_n * x * లోతు) + 7) >> 3);
   img_len = (img_వెడల్పు_బైట్లు + 1) * y;

   // we used to check for exact match between raw_len and img_len on non-interlaced PNGs,
   // but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),
   // so just check for raw_len < img_len always.
   ఒకవేళ (raw_len < img_len) తిరిగి stbi__err("not enough pixels","Corrupt PNG");

   కొరకు (j=0; j < y; ++j) {
      stbi_uc *cur = a->బహిర్గతం + stride*j;
      stbi_uc *పూర్వ;
      పూర్ణాంకం filter = *raw++;

      ఒకవేళ (filter > 4)
         తిరిగి stbi__err("invalid filter","Corrupt PNG");

      ఒకవేళ (లోతు < 8) {
         STBI_ASSERT(img_వెడల్పు_బైట్లు <= x);
         cur += x*బహిర్గతం_n - img_వెడల్పు_బైట్లు; // store output to the rightmost img_len bytes, so we can decode in place
         filter_బైట్లు = 1;
         వెడల్పు = img_వెడల్పు_బైట్లు;
      }
      పూర్వ = cur - stride; // bugfix: need to compute this after 'cur +=' computation above

      // if first row, use special filter that doesn't sample previous row
      ఒకవేళ (j == 0) filter = మొదటి_అడ్డువరుస_filter[filter];

      // handle first byte explicitly
      కొరకు (k=0; k < filter_బైట్లు; ++k) {
         ఎంపిక (filter) {
            స్థితి STBI__F_none       : cur[k] = raw[k]; ఆపు;
            స్థితి STBI__F_sub        : cur[k] = raw[k]; ఆపు;
            స్థితి STBI_F_పైకి         : cur[k] = STBI__BYTECAST(raw[k] + పూర్వ[k]); ఆపు;
            స్థితి STBI__F_avg        : cur[k] = STBI__BYTECAST(raw[k] + (పూర్వ[k]>>1)); ఆపు;
            స్థితి STBI__F_paeth      : cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(0,పూర్వ[k],0)); ఆపు;
            స్థితి STBI_F_avg_మొదటి  : cur[k] = raw[k]; ఆపు;
            స్థితి STBI_F_paeth_మొదటి: cur[k] = raw[k]; ఆపు;
         }
      }

      ఒకవేళ (లోతు == 8) {
         ఒకవేళ (img_n != బహిర్గతం_n)
            cur[img_n] = 255; // first pixel
         raw += img_n;
         cur += బహిర్గతం_n;
         పూర్వ += బహిర్గతం_n;
      } లేకపోతే ఒకవేళ (లోతు == 16) {
         ఒకవేళ (img_n != బహిర్గతం_n) {
            cur[filter_బైట్లు]   = 255; // first pixel top byte
            cur[filter_బైట్లు+1] = 255; // first pixel bottom byte
         }
         raw += filter_బైట్లు;
         cur += output_బైట్లు;
         పూర్వ += output_బైట్లు;
      } లేకపోతే {
         raw += 1;
         cur += 1;
         పూర్వ += 1;
      }

      // this is a little gross, so that we don't switch per-pixel or per-component
      ఒకవేళ (లోతు < 8 || img_n == బహిర్గతం_n) {
         పూర్ణాంకం nk = (వెడల్పు - 1)*filter_బైట్లు;
         #నిర్వచించు STBI__CASE(f) \
             స్థితి f:     \
                కొరకు (k=0; k < nk; ++k)
         ఎంపిక (filter) {
            // "none" filter turns into a memcpy here; make that explicit.
            స్థితి STBI__F_none:         జ్ఞాపకం_నకలు(cur, raw, nk); ఆపు;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k-filter_బైట్లు]); } ఆపు;
            STBI__CASE(STBI_F_పైకి)           { cur[k] = STBI__BYTECAST(raw[k] + పూర్వ[k]); } ఆపు;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((పూర్వ[k] + cur[k-filter_బైట్లు])>>1)); } ఆపు;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_బైట్లు],పూర్వ[k],పూర్వ[k-filter_బైట్లు])); } ఆపు;
            STBI__CASE(STBI_F_avg_మొదటి)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k-filter_బైట్లు] >> 1)); } ఆపు;
            STBI__CASE(STBI_F_paeth_మొదటి)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k-filter_బైట్లు],0,0)); } ఆపు;
         }
         #నిర్వచనం_రద్దు STBI__CASE
         raw += nk;
      } లేకపోతే {
         STBI_ASSERT(img_n+1 == బహిర్గతం_n);
         #నిర్వచించు STBI__CASE(f) \
             స్థితి f:     \
                కొరకు (i=x-1; i >= 1; --i, cur[filter_బైట్లు]=255,raw+=filter_బైట్లు,cur+=output_బైట్లు,పూర్వ+=output_బైట్లు) \
                   కొరకు (k=0; k < filter_బైట్లు; ++k)
         ఎంపిక (filter) {
            STBI__CASE(STBI__F_none)         { cur[k] = raw[k]; } ఆపు;
            STBI__CASE(STBI__F_sub)          { cur[k] = STBI__BYTECAST(raw[k] + cur[k- output_బైట్లు]); } ఆపు;
            STBI__CASE(STBI_F_పైకి)           { cur[k] = STBI__BYTECAST(raw[k] + పూర్వ[k]); } ఆపు;
            STBI__CASE(STBI__F_avg)          { cur[k] = STBI__BYTECAST(raw[k] + ((పూర్వ[k] + cur[k- output_బైట్లు])>>1)); } ఆపు;
            STBI__CASE(STBI__F_paeth)        { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_బైట్లు],పూర్వ[k],పూర్వ[k- output_బైట్లు])); } ఆపు;
            STBI__CASE(STBI_F_avg_మొదటి)    { cur[k] = STBI__BYTECAST(raw[k] + (cur[k- output_బైట్లు] >> 1)); } ఆపు;
            STBI__CASE(STBI_F_paeth_మొదటి)  { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k- output_బైట్లు],0,0)); } ఆపు;
         }
         #నిర్వచనం_రద్దు STBI__CASE

         // the loop above sets the high byte of the pixels' alpha, but for
         // 16 bit png files we also need the low byte set. we'll do that here.
         ఒకవేళ (లోతు == 16) {
            cur = a->బహిర్గతం + stride*j; // start at the beginning of the row again
            కొరకు (i=0; i < x; ++i,cur+=output_బైట్లు) {
               cur[filter_బైట్లు+1] = 255;
            }
         }
      }
   }

   // we make a separate pass to expand bits to pixels; for performance,
   // this could run two scanlines behind the above code, so it won't
   // intefere with filtering but will still be in the cache.
   ఒకవేళ (లోతు < 8) {
      కొరకు (j=0; j < y; ++j) {
         stbi_uc *cur = a->బహిర్గతం + stride*j;
         stbi_uc *లోపల  = a->బహిర్గతం + stride*j + x*బహిర్గతం_n - img_వెడల్పు_బైట్లు;
         // unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit
         // png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop
         stbi_uc scale = (color == 0) ? stbi_లోతు_scale_పట్టిక[లోతు] : 1; // scale grayscale values to 0..255 range

         // note that the final byte might overshoot and write more data than desired.
         // we can allocate enough data that this never writes out of memory, but it
         // could also overwrite the next scanline. can it overwrite non-empty data
         // on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.
         // so we need to explicitly clamp the final ones

         ఒకవేళ (లోతు == 4) {
            కొరకు (k=x*img_n; k >= 2; k-=2, ++లోపల) {
               *cur++ = scale * ((*లోపల >> 4)       );
               *cur++ = scale * ((*లోపల     ) & 0x0f);
            }
            ఒకవేళ (k > 0) *cur++ = scale * ((*లోపల >> 4)       );
         } లేకపోతే ఒకవేళ (లోతు == 2) {
            కొరకు (k=x*img_n; k >= 4; k-=4, ++లోపల) {
               *cur++ = scale * ((*లోపల >> 6)       );
               *cur++ = scale * ((*లోపల >> 4) & 0x03);
               *cur++ = scale * ((*లోపల >> 2) & 0x03);
               *cur++ = scale * ((*లోపల     ) & 0x03);
            }
            ఒకవేళ (k > 0) *cur++ = scale * ((*లోపల >> 6)       );
            ఒకవేళ (k > 1) *cur++ = scale * ((*లోపల >> 4) & 0x03);
            ఒకవేళ (k > 2) *cur++ = scale * ((*లోపల >> 2) & 0x03);
         } లేకపోతే ఒకవేళ (లోతు == 1) {
            కొరకు (k=x*img_n; k >= 8; k-=8, ++లోపల) {
               *cur++ = scale * ((*లోపల >> 7)       );
               *cur++ = scale * ((*లోపల >> 6) & 0x01);
               *cur++ = scale * ((*లోపల >> 5) & 0x01);
               *cur++ = scale * ((*లోపల >> 4) & 0x01);
               *cur++ = scale * ((*లోపల >> 3) & 0x01);
               *cur++ = scale * ((*లోపల >> 2) & 0x01);
               *cur++ = scale * ((*లోపల >> 1) & 0x01);
               *cur++ = scale * ((*లోపల     ) & 0x01);
            }
            ఒకవేళ (k > 0) *cur++ = scale * ((*లోపల >> 7)       );
            ఒకవేళ (k > 1) *cur++ = scale * ((*లోపల >> 6) & 0x01);
            ఒకవేళ (k > 2) *cur++ = scale * ((*లోపల >> 5) & 0x01);
            ఒకవేళ (k > 3) *cur++ = scale * ((*లోపల >> 4) & 0x01);
            ఒకవేళ (k > 4) *cur++ = scale * ((*లోపల >> 3) & 0x01);
            ఒకవేళ (k > 5) *cur++ = scale * ((*లోపల >> 2) & 0x01);
            ఒకవేళ (k > 6) *cur++ = scale * ((*లోపల >> 1) & 0x01);
         }
         ఒకవేళ (img_n != బహిర్గతం_n) {
            పూర్ణాంకం q;
            // insert alpha = 255
            cur = a->బహిర్గతం + stride*j;
            ఒకవేళ (img_n == 1) {
               కొరకు (q=x-1; q >= 0; --q) {
                  cur[q*2+1] = 255;
                  cur[q*2+0] = cur[q];
               }
            } లేకపోతే {
               STBI_ASSERT(img_n == 3);
               కొరకు (q=x-1; q >= 0; --q) {
                  cur[q*4+3] = 255;
                  cur[q*4+2] = cur[q*3+2];
                  cur[q*4+1] = cur[q*3+1];
                  cur[q*4+0] = cur[q*3+0];
               }
            }
         }
      }
   } లేకపోతే ఒకవేళ (లోతు == 16) {
      // force the image data from big-endian to platform-native.
      // this is done in a separate pass due to the decoding relying
      // on the data being untouched, but could probably be done
      // per-line during decode if care is taken.
      stbi_uc *cur = a->బహిర్గతం;
      stbi__uint16 *cur16 = (stbi__uint16*)cur;

      కొరకు(i=0; i < x*y*బహిర్గతం_n; ++i,cur16++,cur+=2) {
         *cur16 = (cur[0] << 8) | cur[1];
      }
   }

   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__create_png_image(stbi__png *a, stbi_uc *image_దత్తాంశం_2, stbi__uint32 image_దత్తాంశం_len, పూర్ణాంకం బహిర్గతం_n, పూర్ణాంకం లోతు, పూర్ణాంకం color, పూర్ణాంకం interlaced)
{
   పూర్ణాంకం బైట్లు = (లోతు == 16 ? 2 : 1);
   పూర్ణాంకం బహిర్గతం_బైట్లు = బహిర్గతం_n * బైట్లు;
   stbi_uc *final;
   పూర్ణాంకం p;
   ఒకవేళ (!interlaced)
      తిరిగి stbi__create_png_image_raw(a, image_దత్తాంశం_2, image_దత్తాంశం_len, బహిర్గతం_n, a->s->img_x, a->s->img_y, లోతు, color);

   // de-interlacing
   final = (stbi_uc *) stbi__malloc_mad3(a->s->img_x, a->s->img_y, బహిర్గతం_బైట్లు, 0);
   కొరకు (p=0; p < 7; ++p) {
      పూర్ణాంకం xorig[] = { 0,4,0,2,0,1,0 };
      పూర్ణాంకం yorig[] = { 0,0,4,0,2,0,1 };
      పూర్ణాంకం xspc[]  = { 8,8,4,4,2,2,1 };
      పూర్ణాంకం yspc[]  = { 8,8,8,4,4,2,2 };
      పూర్ణాంకం i,j,x,y;
      // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
      x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
      y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
      ఒకవేళ (x && y) {
         stbi__uint32 img_len = ((((a->s->img_n * x * లోతు) + 7) >> 3) + 1) * y;
         ఒకవేళ (!stbi__create_png_image_raw(a, image_దత్తాంశం_2, image_దత్తాంశం_len, బహిర్గతం_n, x, y, లోతు, color)) {
            STBI_FREE(final);
            తిరిగి 0;
         }
         కొరకు (j=0; j < y; ++j) {
            కొరకు (i=0; i < x; ++i) {
               పూర్ణాంకం బహిర్గతం_y = j*yspc[p]+yorig[p];
               పూర్ణాంకం బహిర్గతం_x = i*xspc[p]+xorig[p];
               జ్ఞాపకం_నకలు(final + బహిర్గతం_y*a->s->img_x*బహిర్గతం_బైట్లు + బహిర్గతం_x*బహిర్గతం_బైట్లు,
                      a->బహిర్గతం + (j*x+i)*బహిర్గతం_బైట్లు, బహిర్గతం_బైట్లు);
            }
         }
         STBI_FREE(a->బహిర్గతం);
         image_దత్తాంశం_2 += img_len;
         image_దత్తాంశం_len -= img_len;
      }
   }
   a->బహిర్గతం = final;

   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__compute_transparency(stbi__png *z, stbi_uc tc[3], పూర్ణాంకం బహిర్గతం_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_లెక్క = s->img_x * s->img_y;
   stbi_uc *p = z->బహిర్గతం;

   // compute color-based transparency, assuming we've
   // already got 255 as the alpha value in the output
   STBI_ASSERT(బహిర్గతం_n == 2 || బహిర్గతం_n == 4);

   ఒకవేళ (బహిర్గతం_n == 2) {
      కొరకు (i=0; i < pixel_లెక్క; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 255);
         p += 2;
      }
   } లేకపోతే {
      కొరకు (i=0; i < pixel_లెక్క; ++i) {
         ఒకవేళ (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__compute_transparency16(stbi__png *z, stbi__uint16 tc[3], పూర్ణాంకం బహిర్గతం_n)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_లెక్క = s->img_x * s->img_y;
   stbi__uint16 *p = (stbi__uint16*) z->బహిర్గతం;

   // compute color-based transparency, assuming we've
   // already got 65535 as the alpha value in the output
   STBI_ASSERT(బహిర్గతం_n == 2 || బహిర్గతం_n == 4);

   ఒకవేళ (బహిర్గతం_n == 2) {
      కొరకు (i = 0; i < pixel_లెక్క; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 65535);
         p += 2;
      }
   } లేకపోతే {
      కొరకు (i = 0; i < pixel_లెక్క; ++i) {
         ఒకవేళ (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__expand_png_palette(stbi__png *a, stbi_uc *palette, పూర్ణాంకం len, పూర్ణాంకం pal_img_n)
{
   stbi__uint32 i, pixel_లెక్క = a->s->img_x * a->s->img_y;
   stbi_uc *p, *తాత్కాలిక_బహిర్గతం, *orig = a->బహిర్గతం;

   p = (stbi_uc *) stbi__malloc_mad2(pixel_లెక్క, pal_img_n, 0);
   ఒకవేళ (p == ఖాళీ) తిరిగి stbi__err("outofmem", "Out of memory");

   // between here and free(out) below, exitting would leak
   తాత్కాలిక_బహిర్గతం = p;

   ఒకవేళ (pal_img_n == 3) {
      కొరకు (i=0; i < pixel_లెక్క; ++i) {
         పూర్ణాంకం n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p += 3;
      }
   } లేకపోతే {
      కొరకు (i=0; i < pixel_లెక్క; ++i) {
         పూర్ణాంకం n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p[3] = palette[n+3];
         p += 4;
      }
   }
   STBI_FREE(a->బహిర్గతం);
   a->బహిర్గతం = తాత్కాలిక_బహిర్గతం;

   STBI_NOTUSED(len);

   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi_unpremultiply_on_ఎక్కించు = 0;
స్థిర పూర్ణాంకం stbi__de_iphone_flag = 0;

STBIDEF శూన్య stbi_సమితి_unpremultiply_on_ఎక్కించు(పూర్ణాంకం flag_true_if_should_unpremultiply)
{
   stbi_unpremultiply_on_ఎక్కించు = flag_true_if_should_unpremultiply;
}

STBIDEF శూన్య stbi_convert_iphone_png_గమ్యం_rgb(పూర్ణాంకం flag_true_if_should_convert)
{
   stbi__de_iphone_flag = flag_true_if_should_convert;
}

స్థిర శూన్య stbi__de_iphone(stbi__png *z)
{
   stbi__context *s = z->s;
   stbi__uint32 i, pixel_లెక్క = s->img_x * s->img_y;
   stbi_uc *p = z->బహిర్గతం;

   ఒకవేళ (s->img_బహిర్గతం_n == 3) {  // convert bgr to rgb
      కొరకు (i=0; i < pixel_లెక్క; ++i) {
         stbi_uc t = p[0];
         p[0] = p[2];
         p[2] = t;
         p += 3;
      }
   } లేకపోతే {
      STBI_ASSERT(s->img_బహిర్గతం_n == 4);
      ఒకవేళ (stbi_unpremultiply_on_ఎక్కించు) {
         // convert bgr to rgb and unpremultiply
         కొరకు (i=0; i < pixel_లెక్క; ++i) {
            stbi_uc a = p[3];
            stbi_uc t = p[0];
            ఒకవేళ (a) {
               stbi_uc half = a / 2;
               p[0] = (p[2] * 255 + half) / a;
               p[1] = (p[1] * 255 + half) / a;
               p[2] = ( t   * 255 + half) / a;
            } లేకపోతే {
               p[0] = p[2];
               p[2] = t;
            }
            p += 4;
         }
      } లేకపోతే {
         // convert bgr to rgb
         కొరకు (i=0; i < pixel_లెక్క; ++i) {
            stbi_uc t = p[0];
            p[0] = p[2];
            p[2] = t;
            p += 4;
         }
      }
   }
}

#నిర్వచించు STBI__PNG_TYPE(a,b,c,d)  (((గుర్తులేని) (a) << 24) + ((గుర్తులేని) (b) << 16) + ((గుర్తులేని) (c) << 8) + (గుర్తులేని) (d))

స్థిర పూర్ణాంకం stbi__parse_png_file(stbi__png *z, పూర్ణాంకం పరిశీలన, పూర్ణాంకం req_comp)
{
   stbi_uc palette[1024], pal_img_n=0;
   stbi_uc has_trans=0, tc[3];
   stbi__uint16 tc16[3];
   stbi__uint32 ioff=0, idata_హద్దు=0, i, pal_len=0;
   పూర్ణాంకం మొదటి=1,k,interlace=0, color=0, is_iphone=0;
   stbi__context *s = z->s;

   z->expanded = ఖాళీ;
   z->idata = ఖాళీ;
   z->బహిర్గతం = ఖాళీ;

   ఒకవేళ (!stbi_check_png_శీర్షిక(s)) తిరిగి 0;

   ఒకవేళ (పరిశీలన == STBI_పరిశీలన_type) తిరిగి 1;

   కొరకు (;;) {
      stbi__pngchunk c = stbi_get_chunk_శీర్షిక(s);
      ఎంపిక (c.type) {
         స్థితి STBI__PNG_TYPE('C','g','B','I'):
            is_iphone = 1;
            stbi__skip(s, c.నిడివి);
            ఆపు;
         స్థితి STBI__PNG_TYPE('I','H','D','R'): {
            పూర్ణాంకం comp,filter;
            ఒకవేళ (!మొదటి) తిరిగి stbi__err("multiple IHDR","Corrupt PNG");
            మొదటి = 0;
            ఒకవేళ (c.నిడివి != 13) తిరిగి stbi__err("bad IHDR len","Corrupt PNG");
            s->img_x = stbi__get32be(s); ఒకవేళ (s->img_x > (1 << 24)) తిరిగి stbi__err("too large","Very large image (corrupt?)");
            s->img_y = stbi__get32be(s); ఒకవేళ (s->img_y > (1 << 24)) తిరిగి stbi__err("too large","Very large image (corrupt?)");
            z->లోతు = stbi__get8(s);  ఒకవేళ (z->లోతు != 1 && z->లోతు != 2 && z->లోతు != 4 && z->లోతు != 8 && z->లోతు != 16)  తిరిగి stbi__err("1/2/4/8/16-bit only","PNG not supported: 1/2/4/8/16-bit only");
            color = stbi__get8(s);  ఒకవేళ (color > 6)         తిరిగి stbi__err("bad ctype","Corrupt PNG");
            ఒకవేళ (color == 3 && z->లోతు == 16)                  తిరిగి stbi__err("bad ctype","Corrupt PNG");
            ఒకవేళ (color == 3) pal_img_n = 3; లేకపోతే ఒకవేళ (color & 1) తిరిగి stbi__err("bad ctype","Corrupt PNG");
            comp  = stbi__get8(s);  ఒకవేళ (comp) తిరిగి stbi__err("bad comp method","Corrupt PNG");
            filter= stbi__get8(s);  ఒకవేళ (filter) తిరిగి stbi__err("bad filter method","Corrupt PNG");
            interlace = stbi__get8(s); ఒకవేళ (interlace>1) తిరిగి stbi__err("bad interlace method","Corrupt PNG");
            ఒకవేళ (!s->img_x || !s->img_y) తిరిగి stbi__err("0-pixel image","Corrupt PNG");
            ఒకవేళ (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               ఒకవేళ ((1 << 30) / s->img_x / s->img_n < s->img_y) తిరిగి stbi__err("too large", "Image too large to decode");
               ఒకవేళ (పరిశీలన == STBI_పరిశీలన_శీర్షిక) తిరిగి 1;
            } లేకపోతే {
               // if paletted, then pal_n is our final components, and
               // img_n is # components to decompress/filter.
               s->img_n = 1;
               ఒకవేళ ((1 << 30) / s->img_x / 4 < s->img_y) తిరిగి stbi__err("too large","Corrupt PNG");
               // if SCAN_header, have to scan to see if we have a tRNS
            }
            ఆపు;
         }

         స్థితి STBI__PNG_TYPE('P','L','T','E'):  {
            ఒకవేళ (మొదటి) తిరిగి stbi__err("first not IHDR", "Corrupt PNG");
            ఒకవేళ (c.నిడివి > 256*3) తిరిగి stbi__err("invalid PLTE","Corrupt PNG");
            pal_len = c.నిడివి / 3;
            ఒకవేళ (pal_len * 3 != c.నిడివి) తిరిగి stbi__err("invalid PLTE","Corrupt PNG");
            కొరకు (i=0; i < pal_len; ++i) {
               palette[i*4+0] = stbi__get8(s);
               palette[i*4+1] = stbi__get8(s);
               palette[i*4+2] = stbi__get8(s);
               palette[i*4+3] = 255;
            }
            ఆపు;
         }

         స్థితి STBI__PNG_TYPE('t','R','N','S'): {
            ఒకవేళ (మొదటి) తిరిగి stbi__err("first not IHDR", "Corrupt PNG");
            ఒకవేళ (z->idata) తిరిగి stbi__err("tRNS after IDAT","Corrupt PNG");
            ఒకవేళ (pal_img_n) {
               ఒకవేళ (పరిశీలన == STBI_పరిశీలన_శీర్షిక) { s->img_n = 4; తిరిగి 1; }
               ఒకవేళ (pal_len == 0) తిరిగి stbi__err("tRNS before PLTE","Corrupt PNG");
               ఒకవేళ (c.నిడివి > pal_len) తిరిగి stbi__err("bad tRNS len","Corrupt PNG");
               pal_img_n = 4;
               కొరకు (i=0; i < c.నిడివి; ++i)
                  palette[i*4+3] = stbi__get8(s);
            } లేకపోతే {
               ఒకవేళ (!(s->img_n & 1)) తిరిగి stbi__err("tRNS with alpha","Corrupt PNG");
               ఒకవేళ (c.నిడివి != (stbi__uint32) s->img_n*2) తిరిగి stbi__err("bad tRNS len","Corrupt PNG");
               has_trans = 1;
               ఒకవేళ (z->లోతు == 16) {
                  కొరకు (k = 0; k < s->img_n; ++k) tc16[k] = (stbi__uint16)stbi__get16be(s); // copy the values as-is
               } లేకపోతే {
                  కొరకు (k = 0; k < s->img_n; ++k) tc[k] = (stbi_uc)(stbi__get16be(s) & 255) * stbi_లోతు_scale_పట్టిక[z->లోతు]; // non 8-bit images will be larger
               }
            }
            ఆపు;
         }

         స్థితి STBI__PNG_TYPE('I','D','A','T'): {
            ఒకవేళ (మొదటి) తిరిగి stbi__err("first not IHDR", "Corrupt PNG");
            ఒకవేళ (pal_img_n && !pal_len) తిరిగి stbi__err("no PLTE","Corrupt PNG");
            ఒకవేళ (పరిశీలన == STBI_పరిశీలన_శీర్షిక) { s->img_n = pal_img_n; తిరిగి 1; }
            ఒకవేళ ((పూర్ణాంకం)(ioff + c.నిడివి) < (పూర్ణాంకం)ioff) తిరిగి 0;
            ఒకవేళ (ioff + c.నిడివి > idata_హద్దు) {
               stbi__uint32 idata_హద్దు_old = idata_హద్దు;
               stbi_uc *p;
               ఒకవేళ (idata_హద్దు == 0) idata_హద్దు = c.నిడివి > 4096 ? c.నిడివి : 4096;
               అయితే (ioff + c.నిడివి > idata_హద్దు)
                  idata_హద్దు *= 2;
               STBI_NOTUSED(idata_హద్దు_old);
               p = (stbi_uc *) STBI_REALLOC_SIZED(z->idata, idata_హద్దు_old, idata_హద్దు); ఒకవేళ (p == ఖాళీ) తిరిగి stbi__err("outofmem", "Out of memory");
               z->idata = p;
            }
            ఒకవేళ (!stbi__getn(s, z->idata+ioff,c.నిడివి)) తిరిగి stbi__err("outofdata","Corrupt PNG");
            ioff += c.నిడివి;
            ఆపు;
         }

         స్థితి STBI__PNG_TYPE('I','E','N','D'): {
            stbi__uint32 raw_len, bpl;
            ఒకవేళ (మొదటి) తిరిగి stbi__err("first not IHDR", "Corrupt PNG");
            ఒకవేళ (పరిశీలన != STBI_పరిశీలన_ఎక్కించు) తిరిగి 1;
            ఒకవేళ (z->idata == ఖాళీ) తిరిగి stbi__err("no IDAT","Corrupt PNG");
            // initial guess for decoded data size to avoid unnecessary reallocs
            bpl = (s->img_x * z->లోతు + 7) / 8; // bytes per line, per component
            raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
            z->expanded = (stbi_uc *) stbi_zlib_decode_malloc_guesssize_headerflag((అక్షరం *) z->idata, ioff, raw_len, (పూర్ణాంకం *) &raw_len, !is_iphone);
            ఒకవేళ (z->expanded == ఖాళీ) తిరిగి 0; // zlib should set error
            STBI_FREE(z->idata); z->idata = ఖాళీ;
            ఒకవేళ ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_బహిర్గతం_n = s->img_n+1;
            లేకపోతే
               s->img_బహిర్గతం_n = s->img_n;
            ఒకవేళ (!stbi__create_png_image(z, z->expanded, raw_len, s->img_బహిర్గతం_n, z->లోతు, color, interlace)) తిరిగి 0;
            ఒకవేళ (has_trans) {
               ఒకవేళ (z->లోతు == 16) {
                  ఒకవేళ (!stbi__compute_transparency16(z, tc16, s->img_బహిర్గతం_n)) తిరిగి 0;
               } లేకపోతే {
                  ఒకవేళ (!stbi__compute_transparency(z, tc, s->img_బహిర్గతం_n)) తిరిగి 0;
               }
            }
            ఒకవేళ (is_iphone && stbi__de_iphone_flag && s->img_బహిర్గతం_n > 2)
               stbi__de_iphone(z);
            ఒకవేళ (pal_img_n) {
               // pal_img_n == 3 or 4
               s->img_n = pal_img_n; // record the actual colors we had
               s->img_బహిర్గతం_n = pal_img_n;
               ఒకవేళ (req_comp >= 3) s->img_బహిర్గతం_n = req_comp;
               ఒకవేళ (!stbi__expand_png_palette(z, palette, pal_len, s->img_బహిర్గతం_n))
                  తిరిగి 0;
            } లేకపోతే ఒకవేళ (has_trans) {
               // non-paletted image with tRNS -> source image has (constant) alpha
               ++s->img_n;
            }
            STBI_FREE(z->expanded); z->expanded = ఖాళీ;
            తిరిగి 1;
         }

         అప్రమేయ:
            // if critical, fail
            ఒకవేళ (మొదటి) తిరిగి stbi__err("first not IHDR", "Corrupt PNG");
            ఒకవేళ ((c.type & (1 << 29)) == 0) {
               #నిర్వచితం_కాకపోతే STBI_NO_FAILURE_అక్షరశ్రేణులు
               // not threadsafe
               స్థిర అక్షరం invalid_chunk[] = "XXXX PNG chunk not known";
               invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
               invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
               invalid_chunk[2] = STBI__BYTECAST(c.type >>  8);
               invalid_chunk[3] = STBI__BYTECAST(c.type >>  0);
               #అయితే_ముగింపు
               తిరిగి stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
            }
            stbi__skip(s, c.నిడివి);
            ఆపు;
      }
      // end of PNG chunk, read and skip CRC
      stbi__get32be(s);
   }
}

స్థిర శూన్య *stbi__do_png(stbi__png *p, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *n, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   శూన్య *ఫలితం=ఖాళీ;
   ఒకవేళ (req_comp < 0 || req_comp > 4) తిరిగి stbi__errpuc("bad req_comp", "Internal error");
   ఒకవేళ (stbi__parse_png_file(p, STBI_పరిశీలన_ఎక్కించు, req_comp)) {
      ఒకవేళ (p->లోతు < 8)
         ri->బిట్లు_చొప్పున_channel = 8;
      లేకపోతే
         ri->బిట్లు_చొప్పున_channel = p->లోతు;
      ఫలితం = p->బహిర్గతం;
      p->బహిర్గతం = ఖాళీ;
      ఒకవేళ (req_comp && req_comp != p->s->img_బహిర్గతం_n) {
         ఒకవేళ (ri->బిట్లు_చొప్పున_channel == 8)
            ఫలితం = stbi__convert_format((గుర్తులేని అక్షరం *) ఫలితం, p->s->img_బహిర్గతం_n, req_comp, p->s->img_x, p->s->img_y);
         లేకపోతే
            ఫలితం = stbi__convert_format16((stbi__uint16 *) ఫలితం, p->s->img_బహిర్గతం_n, req_comp, p->s->img_x, p->s->img_y);
         p->s->img_బహిర్గతం_n = req_comp;
         ఒకవేళ (ఫలితం == ఖాళీ) తిరిగి ఫలితం;
      }
      *x = p->s->img_x;
      *y = p->s->img_y;
      ఒకవేళ (n) *n = p->s->img_n;
   }
   STBI_FREE(p->బహిర్గతం);      p->బహిర్గతం      = ఖాళీ;
   STBI_FREE(p->expanded); p->expanded = ఖాళీ;
   STBI_FREE(p->idata);    p->idata    = ఖాళీ;

   తిరిగి ఫలితం;
}

స్థిర శూన్య *stbi_png_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   stbi__png p;
   p.s = s;
   తిరిగి stbi__do_png(&p, x,y,comp,req_comp, ri);
}

స్థిర పూర్ణాంకం stbi__png_test(stbi__context *s)
{
   పూర్ణాంకం r;
   r = stbi_check_png_శీర్షిక(s);
   stbi__rewind(s);
   తిరిగి r;
}

స్థిర పూర్ణాంకం stbi__png_info_raw(stbi__png *p, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   ఒకవేళ (!stbi__parse_png_file(p, STBI_పరిశీలన_శీర్షిక, 0)) {
      stbi__rewind( p->s );
      తిరిగి 0;
   }
   ఒకవేళ (x) *x = p->s->img_x;
   ఒకవేళ (y) *y = p->s->img_y;
   ఒకవేళ (comp) *comp = p->s->img_n;
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__png_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   stbi__png p;
   p.s = s;
   తిరిగి stbi__png_info_raw(&p, x, y, comp);
}

స్థిర పూర్ణాంకం stbi__png_is16(stbi__context *s)
{
   stbi__png p;
   p.s = s;
   ఒకవేళ (!stbi__png_info_raw(&p, ఖాళీ, ఖాళీ, ఖాళీ))
	   తిరిగి 0;
   ఒకవేళ (p.లోతు != 16) {
      stbi__rewind(p.s);
      తిరిగి 0;
   }
   తిరిగి 1;
}
#అయితే_ముగింపు

// Microsoft/Windows BMP image

#నిర్వచితం_కాకపోతే STBI_NO_BMP
స్థిర పూర్ణాంకం stbi__bmp_test_raw(stbi__context *s)
{
   పూర్ణాంకం r;
   పూర్ణాంకం sz;
   ఒకవేళ (stbi__get8(s) != 'B') తిరిగి 0;
   ఒకవేళ (stbi__get8(s) != 'M') తిరిగి 0;
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   stbi__get32le(s); // discard data offset
   sz = stbi__get32le(s);
   r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
   తిరిగి r;
}

స్థిర పూర్ణాంకం stbi__bmp_test(stbi__context *s)
{
   పూర్ణాంకం r = stbi__bmp_test_raw(s);
   stbi__rewind(s);
   తిరిగి r;
}


// returns 0..31 for the highest set bit
స్థిర పూర్ణాంకం stbi_ఎగువ_బిట్టు(గుర్తులేని పూర్ణాంకం z)
{
   పూర్ణాంకం n=0;
   ఒకవేళ (z == 0) తిరిగి -1;
   ఒకవేళ (z >= 0x10000) n += 16, z >>= 16;
   ఒకవేళ (z >= 0x00100) n +=  8, z >>=  8;
   ఒకవేళ (z >= 0x00010) n +=  4, z >>=  4;
   ఒకవేళ (z >= 0x00004) n +=  2, z >>=  2;
   ఒకవేళ (z >= 0x00002) n +=  1, z >>=  1;
   తిరిగి n;
}

స్థిర పూర్ణాంకం stbi__bitcount(గుర్తులేని పూర్ణాంకం a)
{
   a = (a & 0x55555555) + ((a >>  1) & 0x55555555); // max 2
   a = (a & 0x33333333) + ((a >>  2) & 0x33333333); // max 4
   a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
   a = (a + (a >> 8)); // max 16 per 8 bits
   a = (a + (a >> 16)); // max 32 per 8 bits
   తిరిగి a & 0xff;
}

// extract an arbitrarily-aligned N-bit value (N=bits)
// from v, and then make it 8-bits long and fractionally
// extend it to full full range.
స్థిర పూర్ణాంకం stbi__shiftsigned(పూర్ణాంకం v, పూర్ణాంకం జరుపు, పూర్ణాంకం బిట్లు)
{
   స్థిర గుర్తులేని పూర్ణాంకం mul_పట్టిక[9] = {
      0,
      0xff/*0b11111111*/, 0x55/*0b01010101*/, 0x49/*0b01001001*/, 0x11/*0b00010001*/,
      0x21/*0b00100001*/, 0x41/*0b01000001*/, 0x81/*0b10000001*/, 0x01/*0b00000001*/,
   };
   స్థిర గుర్తులేని పూర్ణాంకం జరుపు_పట్టిక[9] = {
      0, 0,0,1,0,2,4,6,0,
   };
   ఒకవేళ (జరుపు < 0)
      v <<= -జరుపు;
   లేకపోతే
      v >>= జరుపు;
   STBI_ASSERT(v >= 0 && v < 256);
   v >>= (8-బిట్లు);
   STBI_ASSERT(బిట్లు >= 0 && బిట్లు <= 8);
   తిరిగి (పూర్ణాంకం) ((గుర్తులేని) v * mul_పట్టిక[బిట్లు]) >> జరుపు_పట్టిక[బిట్లు];
}

రకనిర్వచనం నిర్మాణం
{
   పూర్ణాంకం bpp, స్థానభ్రంశం, hsz;
   గుర్తులేని పూర్ణాంకం mr,mg,mb,ma, all_a;
} stbi_bmp_దత్తాంశం;

స్థిర శూన్య *stbi_bmp_parse_శీర్షిక(stbi__context *s, stbi_bmp_దత్తాంశం *info)
{
   పూర్ణాంకం hsz;
   ఒకవేళ (stbi__get8(s) != 'B' || stbi__get8(s) != 'M') తిరిగి stbi__errpuc("not BMP", "Corrupt BMP");
   stbi__get32le(s); // discard filesize
   stbi__get16le(s); // discard reserved
   stbi__get16le(s); // discard reserved
   info->స్థానభ్రంశం = stbi__get32le(s);
   info->hsz = hsz = stbi__get32le(s);
   info->mr = info->mg = info->mb = info->ma = 0;

   ఒకవేళ (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) తిరిగి stbi__errpuc("unknown BMP", "BMP type not supported: unknown");
   ఒకవేళ (hsz == 12) {
      s->img_x = stbi__get16le(s);
      s->img_y = stbi__get16le(s);
   } లేకపోతే {
      s->img_x = stbi__get32le(s);
      s->img_y = stbi__get32le(s);
   }
   ఒకవేళ (stbi__get16le(s) != 1) తిరిగి stbi__errpuc("bad BMP", "bad BMP");
   info->bpp = stbi__get16le(s);
   ఒకవేళ (hsz != 12) {
      పూర్ణాంకం compress = stbi__get32le(s);
      ఒకవేళ (compress == 1 || compress == 2) తిరిగి stbi__errpuc("BMP RLE", "BMP type not supported: RLE");
      stbi__get32le(s); // discard sizeof
      stbi__get32le(s); // discard hres
      stbi__get32le(s); // discard vres
      stbi__get32le(s); // discard colorsused
      stbi__get32le(s); // discard max important
      ఒకవేళ (hsz == 40 || hsz == 56) {
         ఒకవేళ (hsz == 56) {
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
            stbi__get32le(s);
         }
         ఒకవేళ (info->bpp == 16 || info->bpp == 32) {
            ఒకవేళ (compress == 0) {
               ఒకవేళ (info->bpp == 32) {
                  info->mr = 0xffu << 16;
                  info->mg = 0xffu <<  8;
                  info->mb = 0xffu <<  0;
                  info->ma = 0xffu << 24;
                  info->all_a = 0; // if all_a is 0 at end, then we loaded alpha channel but it was all 0
               } లేకపోతే {
                  info->mr = 31u << 10;
                  info->mg = 31u <<  5;
                  info->mb = 31u <<  0;
               }
            } లేకపోతే ఒకవేళ (compress == 3) {
               info->mr = stbi__get32le(s);
               info->mg = stbi__get32le(s);
               info->mb = stbi__get32le(s);
               // not documented, but generated by photoshop and handled by mspaint
               ఒకవేళ (info->mr == info->mg && info->mg == info->mb) {
                  // ?!?!?
                  తిరిగి stbi__errpuc("bad BMP", "bad BMP");
               }
            } లేకపోతే
               తిరిగి stbi__errpuc("bad BMP", "bad BMP");
         }
      } లేకపోతే {
         పూర్ణాంకం i;
         ఒకవేళ (hsz != 108 && hsz != 124)
            తిరిగి stbi__errpuc("bad BMP", "bad BMP");
         info->mr = stbi__get32le(s);
         info->mg = stbi__get32le(s);
         info->mb = stbi__get32le(s);
         info->ma = stbi__get32le(s);
         stbi__get32le(s); // discard color space
         కొరకు (i=0; i < 12; ++i)
            stbi__get32le(s); // discard color space parameters
         ఒకవేళ (hsz == 124) {
            stbi__get32le(s); // discard rendering intent
            stbi__get32le(s); // discard offset of profile data
            stbi__get32le(s); // discard size of profile data
            stbi__get32le(s); // discard reserved
         }
      }
   }
   తిరిగి (శూన్య *) 1;
}


స్థిర శూన్య *stbi_bmp_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   stbi_uc *బహిర్గతం;
   గుర్తులేని పూర్ణాంకం mr=0,mg=0,mb=0,ma=0, all_a;
   stbi_uc pal[256][4];
   పూర్ణాంకం psize=0,i,j,వెడల్పు;
   పూర్ణాంకం flip_vertically, pad, లక్ష్యం;
   stbi_bmp_దత్తాంశం info;
   STBI_NOTUSED(ri);

   info.all_a = 255;
   ఒకవేళ (stbi_bmp_parse_శీర్షిక(s, &info) == ఖాళీ)
      తిరిగి ఖాళీ; // error code already set

   flip_vertically = ((పూర్ణాంకం) s->img_y) > 0;
   s->img_y = నిరపేక్ష((పూర్ణాంకం) s->img_y);

   mr = info.mr;
   mg = info.mg;
   mb = info.mb;
   ma = info.ma;
   all_a = info.all_a;

   ఒకవేళ (info.hsz == 12) {
      ఒకవేళ (info.bpp < 24)
         psize = (info.స్థానభ్రంశం - 14 - 24) / 3;
   } లేకపోతే {
      ఒకవేళ (info.bpp < 16)
         psize = (info.స్థానభ్రంశం - 14 - info.hsz) >> 2;
   }

   s->img_n = ma ? 4 : 3;
   ఒకవేళ (req_comp && req_comp >= 3) // we can directly decode 3 or 4
      లక్ష్యం = req_comp;
   లేకపోతే
      లక్ష్యం = s->img_n; // if they want monochrome, we'll post-convert

   // sanity-check size
   ఒకవేళ (!stbi__mad3sizes_valid(లక్ష్యం, s->img_x, s->img_y, 0))
      తిరిగి stbi__errpuc("too large", "Corrupt BMP");

   బహిర్గతం = (stbi_uc *) stbi__malloc_mad3(లక్ష్యం, s->img_x, s->img_y, 0);
   ఒకవేళ (!బహిర్గతం) తిరిగి stbi__errpuc("outofmem", "Out of memory");
   ఒకవేళ (info.bpp < 16) {
      పూర్ణాంకం z=0;
      ఒకవేళ (psize == 0 || psize > 256) { STBI_FREE(బహిర్గతం); తిరిగి stbi__errpuc("invalid", "Corrupt BMP"); }
      కొరకు (i=0; i < psize; ++i) {
         pal[i][2] = stbi__get8(s);
         pal[i][1] = stbi__get8(s);
         pal[i][0] = stbi__get8(s);
         ఒకవేళ (info.hsz != 12) stbi__get8(s);
         pal[i][3] = 255;
      }
      stbi__skip(s, info.స్థానభ్రంశం - 14 - info.hsz - psize * (info.hsz == 12 ? 3 : 4));
      ఒకవేళ (info.bpp == 1) వెడల్పు = (s->img_x + 7) >> 3;
      లేకపోతే ఒకవేళ (info.bpp == 4) వెడల్పు = (s->img_x + 1) >> 1;
      లేకపోతే ఒకవేళ (info.bpp == 8) వెడల్పు = s->img_x;
      లేకపోతే { STBI_FREE(బహిర్గతం); తిరిగి stbi__errpuc("bad bpp", "Corrupt BMP"); }
      pad = (-వెడల్పు)&3;
      ఒకవేళ (info.bpp == 1) {
         కొరకు (j=0; j < (పూర్ణాంకం) s->img_y; ++j) {
            పూర్ణాంకం బిట్టు_స్థానభ్రంశం = 7, v = stbi__get8(s);
            కొరకు (i=0; i < (పూర్ణాంకం) s->img_x; ++i) {
               పూర్ణాంకం color = (v>>బిట్టు_స్థానభ్రంశం)&0x1;
               బహిర్గతం[z++] = pal[color][0];
               బహిర్గతం[z++] = pal[color][1];
               బహిర్గతం[z++] = pal[color][2];
               ఒకవేళ((--బిట్టు_స్థానభ్రంశం) < 0) {
                  బిట్టు_స్థానభ్రంశం = 7;
                  v = stbi__get8(s);
               }
            }
            stbi__skip(s, pad);
         }
      } లేకపోతే {
         కొరకు (j=0; j < (పూర్ణాంకం) s->img_y; ++j) {
            కొరకు (i=0; i < (పూర్ణాంకం) s->img_x; i += 2) {
               పూర్ణాంకం v=stbi__get8(s),v2=0;
               ఒకవేళ (info.bpp == 4) {
                  v2 = v & 15;
                  v >>= 4;
               }
               బహిర్గతం[z++] = pal[v][0];
               బహిర్గతం[z++] = pal[v][1];
               బహిర్గతం[z++] = pal[v][2];
               ఒకవేళ (లక్ష్యం == 4) బహిర్గతం[z++] = 255;
               ఒకవేళ (i+1 == (పూర్ణాంకం) s->img_x) ఆపు;
               v = (info.bpp == 8) ? stbi__get8(s) : v2;
               బహిర్గతం[z++] = pal[v][0];
               బహిర్గతం[z++] = pal[v][1];
               బహిర్గతం[z++] = pal[v][2];
               ఒకవేళ (లక్ష్యం == 4) బహిర్గతం[z++] = 255;
            }
            stbi__skip(s, pad);
         }
      }
   } లేకపోతే {
      పూర్ణాంకం rshift=0,gshift=0,bshift=0,ashift=0,rcount=0,gcount=0,bcount=0,acount=0;
      పూర్ణాంకం z = 0;
      పూర్ణాంకం easy=0;
      stbi__skip(s, info.స్థానభ్రంశం - 14 - info.hsz);
      ఒకవేళ (info.bpp == 24) వెడల్పు = 3 * s->img_x;
      లేకపోతే ఒకవేళ (info.bpp == 16) వెడల్పు = 2*s->img_x;
      లేకపోతే /* bpp = 32 and pad = 0 */ వెడల్పు=0;
      pad = (-వెడల్పు) & 3;
      ఒకవేళ (info.bpp == 24) {
         easy = 1;
      } లేకపోతే ఒకవేళ (info.bpp == 32) {
         ఒకవేళ (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
            easy = 2;
      }
      ఒకవేళ (!easy) {
         ఒకవేళ (!mr || !mg || !mb) { STBI_FREE(బహిర్గతం); తిరిగి stbi__errpuc("bad masks", "Corrupt BMP"); }
         // right shift amt to put high bit in position #7
         rshift = stbi_ఎగువ_బిట్టు(mr)-7; rcount = stbi__bitcount(mr);
         gshift = stbi_ఎగువ_బిట్టు(mg)-7; gcount = stbi__bitcount(mg);
         bshift = stbi_ఎగువ_బిట్టు(mb)-7; bcount = stbi__bitcount(mb);
         ashift = stbi_ఎగువ_బిట్టు(ma)-7; acount = stbi__bitcount(ma);
      }
      కొరకు (j=0; j < (పూర్ణాంకం) s->img_y; ++j) {
         ఒకవేళ (easy) {
            కొరకు (i=0; i < (పూర్ణాంకం) s->img_x; ++i) {
               గుర్తులేని అక్షరం a;
               బహిర్గతం[z+2] = stbi__get8(s);
               బహిర్గతం[z+1] = stbi__get8(s);
               బహిర్గతం[z+0] = stbi__get8(s);
               z += 3;
               a = (easy == 2 ? stbi__get8(s) : 255);
               all_a |= a;
               ఒకవేళ (లక్ష్యం == 4) బహిర్గతం[z++] = a;
            }
         } లేకపోతే {
            పూర్ణాంకం bpp = info.bpp;
            కొరకు (i=0; i < (పూర్ణాంకం) s->img_x; ++i) {
               stbi__uint32 v = (bpp == 16 ? (stbi__uint32) stbi__get16le(s) : stbi__get32le(s));
               గుర్తులేని పూర్ణాంకం a;
               బహిర్గతం[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
               బహిర్గతం[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
               బహిర్గతం[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));
               a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
               all_a |= a;
               ఒకవేళ (లక్ష్యం == 4) బహిర్గతం[z++] = STBI__BYTECAST(a);
            }
         }
         stbi__skip(s, pad);
      }
   }

   // if alpha channel is all 0s, replace with all 255s
   ఒకవేళ (లక్ష్యం == 4 && all_a == 0)
      కొరకు (i=4*s->img_x*s->img_y-1; i >= 0; i -= 4)
         బహిర్గతం[i] = 255;

   ఒకవేళ (flip_vertically) {
      stbi_uc t;
      కొరకు (j=0; j < (పూర్ణాంకం) s->img_y>>1; ++j) {
         stbi_uc *p1 = బహిర్గతం +      j     *s->img_x*లక్ష్యం;
         stbi_uc *p2 = బహిర్గతం + (s->img_y-1-j)*s->img_x*లక్ష్యం;
         కొరకు (i=0; i < (పూర్ణాంకం) s->img_x*లక్ష్యం; ++i) {
            t = p1[i], p1[i] = p2[i], p2[i] = t;
         }
      }
   }

   ఒకవేళ (req_comp && req_comp != లక్ష్యం) {
      బహిర్గతం = stbi__convert_format(బహిర్గతం, లక్ష్యం, req_comp, s->img_x, s->img_y);
      ఒకవేళ (బహిర్గతం == ఖాళీ) తిరిగి బహిర్గతం; // stbi__convert_format frees input on failure
   }

   *x = s->img_x;
   *y = s->img_y;
   ఒకవేళ (comp) *comp = s->img_n;
   తిరిగి బహిర్గతం;
}
#అయితే_ముగింపు

// Targa Truevision - TGA
// by Jonathan Dummer
#నిర్వచితం_కాకపోతే STBI_NO_TGA
// returns STBI_rgb or whatever, 0 on error
స్థిర పూర్ణాంకం stbi__tga_get_comp(పూర్ణాంకం బిట్లు_చొప్పున_pixel, పూర్ణాంకం is_grey, పూర్ణాంకం* is_rgb16)
{
   // only RGB or RGBA (incl. 16bit) or grey allowed
   ఒకవేళ (is_rgb16) *is_rgb16 = 0;
   ఎంపిక(బిట్లు_చొప్పున_pixel) {
      స్థితి 8:  తిరిగి STBI_grey;
      స్థితి 16: ఒకవేళ(is_grey) తిరిగి STBI_grey_alpha;
               // fallthrough
      స్థితి 15: ఒకవేళ(is_rgb16) *is_rgb16 = 1;
               తిరిగి STBI_rgb;
      స్థితి 24: // fallthrough
      స్థితి 32: తిరిగి బిట్లు_చొప్పున_pixel/8;
      అప్రమేయ: తిరిగి 0;
   }
}

స్థిర పూర్ణాంకం stbi__tga_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
    పూర్ణాంకం tga_w, tga_h, tga_comp, tga_image_type, tga_బిట్లు_చొప్పున_pixel, tga_colormap_bpp;
    పూర్ణాంకం sz, tga_colormap_type;
    stbi__get8(s);                   // discard Offset
    tga_colormap_type = stbi__get8(s); // colormap type
    ఒకవేళ( tga_colormap_type > 1 ) {
        stbi__rewind(s);
        తిరిగి 0;      // only RGB or indexed allowed
    }
    tga_image_type = stbi__get8(s); // image type
    ఒకవేళ ( tga_colormap_type == 1 ) { // colormapped (paletted) image
        ఒకవేళ (tga_image_type != 1 && tga_image_type != 9) {
            stbi__rewind(s);
            తిరిగి 0;
        }
        stbi__skip(s,4);       // skip index of first colormap entry and number of entries
        sz = stbi__get8(s);    //   check bits per palette color entry
        ఒకవేళ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) {
            stbi__rewind(s);
            తిరిగి 0;
        }
        stbi__skip(s,4);       // skip image x and y origin
        tga_colormap_bpp = sz;
    } లేకపోతే { // "normal" image w/o colormap - only RGB or grey allowed, +/- RLE
        ఒకవేళ ( (tga_image_type != 2) && (tga_image_type != 3) && (tga_image_type != 10) && (tga_image_type != 11) ) {
            stbi__rewind(s);
            తిరిగి 0; // only RGB or grey allowed, +/- RLE
        }
        stbi__skip(s,9); // skip colormap specification and image x/y origin
        tga_colormap_bpp = 0;
    }
    tga_w = stbi__get16le(s);
    ఒకవేళ( tga_w < 1 ) {
        stbi__rewind(s);
        తిరిగి 0;   // test width
    }
    tga_h = stbi__get16le(s);
    ఒకవేళ( tga_h < 1 ) {
        stbi__rewind(s);
        తిరిగి 0;   // test height
    }
    tga_బిట్లు_చొప్పున_pixel = stbi__get8(s); // bits per pixel
    stbi__get8(s); // ignore alpha bits
    ఒకవేళ (tga_colormap_bpp != 0) {
        ఒకవేళ((tga_బిట్లు_చొప్పున_pixel != 8) && (tga_బిట్లు_చొప్పున_pixel != 16)) {
            // when using a colormap, tga_bits_per_pixel is the size of the indexes
            // I don't think anything but 8 or 16bit indexes makes sense
            stbi__rewind(s);
            తిరిగి 0;
        }
        tga_comp = stbi__tga_get_comp(tga_colormap_bpp, 0, ఖాళీ);
    } లేకపోతే {
        tga_comp = stbi__tga_get_comp(tga_బిట్లు_చొప్పున_pixel, (tga_image_type == 3) || (tga_image_type == 11), ఖాళీ);
    }
    ఒకవేళ(!tga_comp) {
      stbi__rewind(s);
      తిరిగి 0;
    }
    ఒకవేళ (x) *x = tga_w;
    ఒకవేళ (y) *y = tga_h;
    ఒకవేళ (comp) *comp = tga_comp;
    తిరిగి 1;                   // seems to have passed everything
}

స్థిర పూర్ణాంకం stbi__tga_test(stbi__context *s)
{
   పూర్ణాంకం res = 0;
   పూర్ణాంకం sz, tga_color_type;
   stbi__get8(s);      //   discard Offset
   tga_color_type = stbi__get8(s);   //   color type
   ఒకవేళ ( tga_color_type > 1 ) వెళ్ళు error_చివర;   //   only RGB or indexed allowed
   sz = stbi__get8(s);   //   image type
   ఒకవేళ ( tga_color_type == 1 ) { // colormapped (paletted) image
      ఒకవేళ (sz != 1 && sz != 9) వెళ్ళు error_చివర; // colortype 1 demands image type 1 or 9
      stbi__skip(s,4);       // skip index of first colormap entry and number of entries
      sz = stbi__get8(s);    //   check bits per palette color entry
      ఒకవేళ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) వెళ్ళు error_చివర;
      stbi__skip(s,4);       // skip image x and y origin
   } లేకపోతే { // "normal" image w/o colormap
      ఒకవేళ ( (sz != 2) && (sz != 3) && (sz != 10) && (sz != 11) ) వెళ్ళు error_చివర; // only RGB or grey allowed, +/- RLE
      stbi__skip(s,9); // skip colormap specification and image x/y origin
   }
   ఒకవేళ ( stbi__get16le(s) < 1 ) వెళ్ళు error_చివర;      //   test width
   ఒకవేళ ( stbi__get16le(s) < 1 ) వెళ్ళు error_చివర;      //   test height
   sz = stbi__get8(s);   //   bits per pixel
   ఒకవేళ ( (tga_color_type == 1) && (sz != 8) && (sz != 16) ) వెళ్ళు error_చివర; // for colormapped images, bpp is size of an index
   ఒకవేళ ( (sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32) ) వెళ్ళు error_చివర;

   res = 1; // if we got this far, everything's good and we can return 1 instead of 0

error_చివర:
   stbi__rewind(s);
   తిరిగి res;
}

// read 16bit value and convert to 24bit RGB
స్థిర శూన్య stbi__tga_read_rgb16(stbi__context *s, stbi_uc* బహిర్గతం)
{
   stbi__uint16 px = (stbi__uint16)stbi__get16le(s);
   stbi__uint16 five_బిట్టు_ముసుగు = 31;
   // we have 3 channels with 5bits each
   పూర్ణాంకం r = (px >> 10) & five_బిట్టు_ముసుగు;
   పూర్ణాంకం g = (px >> 5) & five_బిట్టు_ముసుగు;
   పూర్ణాంకం b = px & five_బిట్టు_ముసుగు;
   // Note that this saves the data in RGB(A) order, so it doesn't need to be swapped later
   బహిర్గతం[0] = (stbi_uc)((r * 255)/31);
   బహిర్గతం[1] = (stbi_uc)((g * 255)/31);
   బహిర్గతం[2] = (stbi_uc)((b * 255)/31);

   // some people claim that the most significant bit might be used for alpha
   // (possibly if an alpha-bit is set in the "image descriptor byte")
   // but that only made 16bit test images completely translucent..
   // so let's treat all 15 and 16bit TGAs as RGB with no alpha.
}

స్థిర శూన్య *stbi_tga_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   //   read in the TGA header stuff
   పూర్ణాంకం tga_స్థానభ్రంశం = stbi__get8(s);
   పూర్ణాంకం tga_సూచీకృత = stbi__get8(s);
   పూర్ణాంకం tga_image_type = stbi__get8(s);
   పూర్ణాంకం tga_is_RLE = 0;
   పూర్ణాంకం tga_palette_ప్రారంభం = stbi__get16le(s);
   పూర్ణాంకం tga_palette_len = stbi__get16le(s);
   పూర్ణాంకం tga_palette_బిట్లు = stbi__get8(s);
   పూర్ణాంకం tga_x_origin = stbi__get16le(s);
   పూర్ణాంకం tga_y_origin = stbi__get16le(s);
   పూర్ణాంకం tga_వెడల్పు = stbi__get16le(s);
   పూర్ణాంకం tga_ఎత్తు = stbi__get16le(s);
   పూర్ణాంకం tga_బిట్లు_చొప్పున_pixel = stbi__get8(s);
   పూర్ణాంకం tga_comp, tga_rgb16=0;
   పూర్ణాంకం tga_inverted = stbi__get8(s);
   // int tga_alpha_bits = tga_inverted & 15; // the 4 lowest bits - unused (useless?)
   //   image data
   గుర్తులేని అక్షరం *tga_దత్తాంశం;
   గుర్తులేని అక్షరం *tga_palette = ఖాళీ;
   పూర్ణాంకం i, j;
   గుర్తులేని అక్షరం raw_దత్తాంశం[4] = {0};
   పూర్ణాంకం RLE_లెక్క = 0;
   పూర్ణాంకం RLE_repeating = 0;
   పూర్ణాంకం read_తదుపరి_pixel = 1;
   STBI_NOTUSED(ri);

   //   do a tiny bit of precessing
   ఒకవేళ ( tga_image_type >= 8 )
   {
      tga_image_type -= 8;
      tga_is_RLE = 1;
   }
   tga_inverted = 1 - ((tga_inverted >> 5) & 1);

   //   If I'm paletted, then I'll use the number of bits from the palette
   ఒకవేళ ( tga_సూచీకృత ) tga_comp = stbi__tga_get_comp(tga_palette_బిట్లు, 0, &tga_rgb16);
   లేకపోతే tga_comp = stbi__tga_get_comp(tga_బిట్లు_చొప్పున_pixel, (tga_image_type == 3), &tga_rgb16);

   ఒకవేళ(!tga_comp) // shouldn't really happen, stbi__tga_test() should have ensured basic consistency
      తిరిగి stbi__errpuc("bad format", "Can't find out TGA pixelformat");

   //   tga info
   *x = tga_వెడల్పు;
   *y = tga_ఎత్తు;
   ఒకవేళ (comp) *comp = tga_comp;

   ఒకవేళ (!stbi__mad3sizes_valid(tga_వెడల్పు, tga_ఎత్తు, tga_comp, 0))
      తిరిగి stbi__errpuc("too large", "Corrupt TGA");

   tga_దత్తాంశం = (గుర్తులేని అక్షరం*)stbi__malloc_mad3(tga_వెడల్పు, tga_ఎత్తు, tga_comp, 0);
   ఒకవేళ (!tga_దత్తాంశం) తిరిగి stbi__errpuc("outofmem", "Out of memory");

   // skip to the data's starting position (offset usually = 0)
   stbi__skip(s, tga_స్థానభ్రంశం );

   ఒకవేళ ( !tga_సూచీకృత && !tga_is_RLE && !tga_rgb16 ) {
      కొరకు (i=0; i < tga_ఎత్తు; ++i) {
         పూర్ణాంకం అడ్డువరుస = tga_inverted ? tga_ఎత్తు -i - 1 : i;
         stbi_uc *tga_అడ్డువరుస = tga_దత్తాంశం + అడ్డువరుస*tga_వెడల్పు*tga_comp;
         stbi__getn(s, tga_అడ్డువరుస, tga_వెడల్పు * tga_comp);
      }
   } లేకపోతే  {
      //   do I need to load a palette?
      ఒకవేళ ( tga_సూచీకృత)
      {
         //   any data to skip? (offset usually = 0)
         stbi__skip(s, tga_palette_ప్రారంభం );
         //   load the palette
         tga_palette = (గుర్తులేని అక్షరం*)stbi__malloc_mad2(tga_palette_len, tga_comp, 0);
         ఒకవేళ (!tga_palette) {
            STBI_FREE(tga_దత్తాంశం);
            తిరిగి stbi__errpuc("outofmem", "Out of memory");
         }
         ఒకవేళ (tga_rgb16) {
            stbi_uc *pal_నమోదు = tga_palette;
            STBI_ASSERT(tga_comp == STBI_rgb);
            కొరకు (i=0; i < tga_palette_len; ++i) {
               stbi__tga_read_rgb16(s, pal_నమోదు);
               pal_నమోదు += tga_comp;
            }
         } లేకపోతే ఒకవేళ (!stbi__getn(s, tga_palette, tga_palette_len * tga_comp)) {
               STBI_FREE(tga_దత్తాంశం);
               STBI_FREE(tga_palette);
               తిరిగి stbi__errpuc("bad palette", "Corrupt TGA");
         }
      }
      //   load the data
      కొరకు (i=0; i < tga_వెడల్పు * tga_ఎత్తు; ++i)
      {
         //   if I'm in RLE mode, do I need to get a RLE stbi__pngchunk?
         ఒకవేళ ( tga_is_RLE )
         {
            ఒకవేళ ( RLE_లెక్క == 0 )
            {
               //   yep, get the next byte as a RLE command
               పూర్ణాంకం RLE_cmd = stbi__get8(s);
               RLE_లెక్క = 1 + (RLE_cmd & 127);
               RLE_repeating = RLE_cmd >> 7;
               read_తదుపరి_pixel = 1;
            } లేకపోతే ఒకవేళ ( !RLE_repeating )
            {
               read_తదుపరి_pixel = 1;
            }
         } లేకపోతే
         {
            read_తదుపరి_pixel = 1;
         }
         //   OK, if I need to read a pixel, do it now
         ఒకవేళ ( read_తదుపరి_pixel )
         {
            //   load however much data we did have
            ఒకవేళ ( tga_సూచీకృత )
            {
               // read in index, then perform the lookup
               పూర్ణాంకం pal_idx = (tga_బిట్లు_చొప్పున_pixel == 8) ? stbi__get8(s) : stbi__get16le(s);
               ఒకవేళ ( pal_idx >= tga_palette_len ) {
                  // invalid index
                  pal_idx = 0;
               }
               pal_idx *= tga_comp;
               కొరకు (j = 0; j < tga_comp; ++j) {
                  raw_దత్తాంశం[j] = tga_palette[pal_idx+j];
               }
            } లేకపోతే ఒకవేళ(tga_rgb16) {
               STBI_ASSERT(tga_comp == STBI_rgb);
               stbi__tga_read_rgb16(s, raw_దత్తాంశం);
            } లేకపోతే {
               //   read in the data raw
               కొరకు (j = 0; j < tga_comp; ++j) {
                  raw_దత్తాంశం[j] = stbi__get8(s);
               }
            }
            //   clear the reading flag for the next pixel
            read_తదుపరి_pixel = 0;
         } // end of reading a pixel

         // copy data
         కొరకు (j = 0; j < tga_comp; ++j)
           tga_దత్తాంశం[i*tga_comp+j] = raw_దత్తాంశం[j];

         //   in case we're in RLE mode, keep counting down
         --RLE_లెక్క;
      }
      //   do I need to invert the image?
      ఒకవేళ ( tga_inverted )
      {
         కొరకు (j = 0; j*2 < tga_ఎత్తు; ++j)
         {
            పూర్ణాంకం index1 = j * tga_వెడల్పు * tga_comp;
            పూర్ణాంకం index2 = (tga_ఎత్తు - 1 - j) * tga_వెడల్పు * tga_comp;
            కొరకు (i = tga_వెడల్పు * tga_comp; i > 0; --i)
            {
               గుర్తులేని అక్షరం తాత్కాలిక = tga_దత్తాంశం[index1];
               tga_దత్తాంశం[index1] = tga_దత్తాంశం[index2];
               tga_దత్తాంశం[index2] = తాత్కాలిక;
               ++index1;
               ++index2;
            }
         }
      }
      //   clear my palette, if I had one
      ఒకవేళ ( tga_palette != ఖాళీ )
      {
         STBI_FREE( tga_palette );
      }
   }

   // swap RGB - if the source data was RGB16, it already is in the right order
   ఒకవేళ (tga_comp >= 3 && !tga_rgb16)
   {
      గుర్తులేని అక్షరం* tga_pixel = tga_దత్తాంశం;
      కొరకు (i=0; i < tga_వెడల్పు * tga_ఎత్తు; ++i)
      {
         గుర్తులేని అక్షరం తాత్కాలిక = tga_pixel[0];
         tga_pixel[0] = tga_pixel[2];
         tga_pixel[2] = తాత్కాలిక;
         tga_pixel += tga_comp;
      }
   }

   // convert to target component count
   ఒకవేళ (req_comp && req_comp != tga_comp)
      tga_దత్తాంశం = stbi__convert_format(tga_దత్తాంశం, tga_comp, req_comp, tga_వెడల్పు, tga_ఎత్తు);

   //   the things I do to get rid of an error message, and yet keep
   //   Microsoft's C compilers happy... [8^(
   tga_palette_ప్రారంభం = tga_palette_len = tga_palette_బిట్లు =
         tga_x_origin = tga_y_origin = 0;
   //   OK, done
   తిరిగి tga_దత్తాంశం;
}
#అయితే_ముగింపు

// *************************************************************************************************
// Photoshop PSD loader -- PD by Thatcher Ulrich, integration by Nicolas Schulz, tweaked by STB

#నిర్వచితం_కాకపోతే STBI_NO_PSD
స్థిర పూర్ణాంకం stbi__psd_test(stbi__context *s)
{
   పూర్ణాంకం r = (stbi__get32be(s) == 0x38425053);
   stbi__rewind(s);
   తిరిగి r;
}

స్థిర పూర్ణాంకం stbi__psd_decode_rle(stbi__context *s, stbi_uc *p, పూర్ణాంకం pixel_లెక్క_2)
{
   పూర్ణాంకం లెక్క, nleft, len;

   లెక్క = 0;
   అయితే ((nleft = pixel_లెక్క_2 - లెక్క) > 0) {
      len = stbi__get8(s);
      ఒకవేళ (len == 128) {
         // No-op.
      } లేకపోతే ఒకవేళ (len < 128) {
         // Copy next len+1 bytes literally.
         len++;
         ఒకవేళ (len > nleft) తిరిగి 0; // corrupt data
         లెక్క += len;
         అయితే (len) {
            *p = stbi__get8(s);
            p += 4;
            len--;
         }
      } లేకపోతే ఒకవేళ (len > 128) {
         stbi_uc   val;
         // Next -len+1 bytes in the dest are replicated from next source byte.
         // (Interpret len as a negative 8-bit int.)
         len = 257 - len;
         ఒకవేళ (len > nleft) తిరిగి 0; // corrupt data
         val = stbi__get8(s);
         లెక్క += len;
         అయితే (len) {
            *p = val;
            p += 4;
            len--;
         }
      }
   }

   తిరిగి 1;
}

స్థిర శూన్య *stbi_psd_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri, పూర్ణాంకం bpc)
{
   పూర్ణాంకం pixel_లెక్క_2;
   పూర్ణాంకం channel_లెక్క, compression;
   పూర్ణాంకం channel, i;
   పూర్ణాంకం bitdepth;
   పూర్ణాంకం w,h;
   stbi_uc *బహిర్గతం;
   STBI_NOTUSED(ri);

   // Check identifier
   ఒకవేళ (stbi__get32be(s) != 0x38425053)   // "8BPS"
      తిరిగి stbi__errpuc("not PSD", "Corrupt PSD image");

   // Check file type version.
   ఒకవేళ (stbi__get16be(s) != 1)
      తిరిగి stbi__errpuc("wrong version", "Unsupported version of PSD image");

   // Skip 6 reserved bytes.
   stbi__skip(s, 6 );

   // Read the number of channels (R, G, B, A, etc).
   channel_లెక్క = stbi__get16be(s);
   ఒకవేళ (channel_లెక్క < 0 || channel_లెక్క > 16)
      తిరిగి stbi__errpuc("wrong channel count", "Unsupported number of channels in PSD image");

   // Read the rows and columns of the image.
   h = stbi__get32be(s);
   w = stbi__get32be(s);

   // Make sure the depth is 8 bits.
   bitdepth = stbi__get16be(s);
   ఒకవేళ (bitdepth != 8 && bitdepth != 16)
      తిరిగి stbi__errpuc("unsupported bit depth", "PSD bit depth is not 8 or 16 bit");

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
   ఒకవేళ (stbi__get16be(s) != 3)
      తిరిగి stbi__errpuc("wrong color format", "PSD is not in RGB color format");

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
   ఒకవేళ (compression > 1)
      తిరిగి stbi__errpuc("bad compression", "PSD has an unknown compression format");

   // Check size
   ఒకవేళ (!stbi__mad3sizes_valid(4, w, h, 0))
      తిరిగి stbi__errpuc("too large", "Corrupt PSD");

   // Create the destination image.

   ఒకవేళ (!compression && bitdepth == 16 && bpc == 16) {
      బహిర్గతం = (stbi_uc *) stbi__malloc_mad3(8, w, h, 0);
      ri->బిట్లు_చొప్పున_channel = 16;
   } లేకపోతే
      బహిర్గతం = (stbi_uc *) stbi__malloc(4 * w*h);

   ఒకవేళ (!బహిర్గతం) తిరిగి stbi__errpuc("outofmem", "Out of memory");
   pixel_లెక్క_2 = w*h;

   // Initialize the data to zero.
   //memset( out, 0, pixelCount * 4 );

   // Finally, the image data.
   ఒకవేళ (compression) {
      // RLE as used by .PSD and .TIFF
      // Loop until you get the number of unpacked bytes you are expecting:
      //     Read the next source byte into n.
      //     If n is between 0 and 127 inclusive, copy the next n+1 bytes literally.
      //     Else if n is between -127 and -1 inclusive, copy the next byte -n+1 times.
      //     Else if n is 128, noop.
      // Endloop

      // The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
      // which we're going to just skip.
      stbi__skip(s, h * channel_లెక్క * 2 );

      // Read the RLE data by channel.
      కొరకు (channel = 0; channel < 4; channel++) {
         stbi_uc *p;

         p = బహిర్గతం+channel;
         ఒకవేళ (channel >= channel_లెక్క) {
            // Fill this channel with default data.
            కొరకు (i = 0; i < pixel_లెక్క_2; i++, p += 4)
               *p = (channel == 3 ? 255 : 0);
         } లేకపోతే {
            // Read the RLE data.
            ఒకవేళ (!stbi__psd_decode_rle(s, p, pixel_లెక్క_2)) {
               STBI_FREE(బహిర్గతం);
               తిరిగి stbi__errpuc("corrupt", "bad RLE data");
            }
         }
      }

   } లేకపోతే {
      // We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
      // where each channel consists of an 8-bit (or 16-bit) value for each pixel in the image.

      // Read the data by channel.
      కొరకు (channel = 0; channel < 4; channel++) {
         ఒకవేళ (channel >= channel_లెక్క) {
            // Fill this channel with default data.
            ఒకవేళ (bitdepth == 16 && bpc == 16) {
               stbi__uint16 *q = ((stbi__uint16 *) బహిర్గతం) + channel;
               stbi__uint16 val = channel == 3 ? 65535 : 0;
               కొరకు (i = 0; i < pixel_లెక్క_2; i++, q += 4)
                  *q = val;
            } లేకపోతే {
               stbi_uc *p = బహిర్గతం+channel;
               stbi_uc val = channel == 3 ? 255 : 0;
               కొరకు (i = 0; i < pixel_లెక్క_2; i++, p += 4)
                  *p = val;
            }
         } లేకపోతే {
            ఒకవేళ (ri->బిట్లు_చొప్పున_channel == 16) {    // output bpc
               stbi__uint16 *q = ((stbi__uint16 *) బహిర్గతం) + channel;
               కొరకు (i = 0; i < pixel_లెక్క_2; i++, q += 4)
                  *q = (stbi__uint16) stbi__get16be(s);
            } లేకపోతే {
               stbi_uc *p = బహిర్గతం+channel;
               ఒకవేళ (bitdepth == 16) {  // input bpc
                  కొరకు (i = 0; i < pixel_లెక్క_2; i++, p += 4)
                     *p = (stbi_uc) (stbi__get16be(s) >> 8);
               } లేకపోతే {
                  కొరకు (i = 0; i < pixel_లెక్క_2; i++, p += 4)
                     *p = stbi__get8(s);
               }
            }
         }
      }
   }

   // remove weird white matte from PSD
   ఒకవేళ (channel_లెక్క >= 4) {
      ఒకవేళ (ri->బిట్లు_చొప్పున_channel == 16) {
         కొరకు (i=0; i < w*h; ++i) {
            stbi__uint16 *pixel = (stbi__uint16 *) బహిర్గతం + 4*i;
            ఒకవేళ (pixel[3] != 0 && pixel[3] != 65535) {
               దశాంశం a = pixel[3] / 65535.0f;
               దశాంశం ra = 1.0f / a;
               దశాంశం inv_a = 65535.0f * (1 - ra);
               pixel[0] = (stbi__uint16) (pixel[0]*ra + inv_a);
               pixel[1] = (stbi__uint16) (pixel[1]*ra + inv_a);
               pixel[2] = (stbi__uint16) (pixel[2]*ra + inv_a);
            }
         }
      } లేకపోతే {
         కొరకు (i=0; i < w*h; ++i) {
            గుర్తులేని అక్షరం *pixel = బహిర్గతం + 4*i;
            ఒకవేళ (pixel[3] != 0 && pixel[3] != 255) {
               దశాంశం a = pixel[3] / 255.0f;
               దశాంశం ra = 1.0f / a;
               దశాంశం inv_a = 255.0f * (1 - ra);
               pixel[0] = (గుర్తులేని అక్షరం) (pixel[0]*ra + inv_a);
               pixel[1] = (గుర్తులేని అక్షరం) (pixel[1]*ra + inv_a);
               pixel[2] = (గుర్తులేని అక్షరం) (pixel[2]*ra + inv_a);
            }
         }
      }
   }

   // convert to desired output format
   ఒకవేళ (req_comp && req_comp != 4) {
      ఒకవేళ (ri->బిట్లు_చొప్పున_channel == 16)
         బహిర్గతం = (stbi_uc *) stbi__convert_format16((stbi__uint16 *) బహిర్గతం, 4, req_comp, w, h);
      లేకపోతే
         బహిర్గతం = stbi__convert_format(బహిర్గతం, 4, req_comp, w, h);
      ఒకవేళ (బహిర్గతం == ఖాళీ) తిరిగి బహిర్గతం; // stbi__convert_format frees input on failure
   }

   ఒకవేళ (comp) *comp = 4;
   *y = h;
   *x = w;

   తిరిగి బహిర్గతం;
}
#అయితే_ముగింపు

// *************************************************************************************************
// Softimage PIC loader
// by Tom Seddon
//
// See http://softimage.wiki.softimage.com/index.php/INFO:_PIC_file_format
// See http://ozviz.wasp.uwa.edu.au/~pbourke/dataformats/softimagepic/

#నిర్వచితం_కాకపోతే STBI_NO_PIC
స్థిర పూర్ణాంకం stbi__pic_is4(stbi__context *s,స్థిరరాశి అక్షరం *str)
{
   పూర్ణాంకం i;
   కొరకు (i=0; i<4; ++i)
      ఒకవేళ (stbi__get8(s) != (stbi_uc)str[i])
         తిరిగి 0;

   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__pic_test_core(stbi__context *s)
{
   పూర్ణాంకం i;

   ఒకవేళ (!stbi__pic_is4(s,"\x53\x80\xF6\x34"))
      తిరిగి 0;

   కొరకు(i=0;i<84;++i)
      stbi__get8(s);

   ఒకవేళ (!stbi__pic_is4(s,"PICT"))
      తిరిగి 0;

   తిరిగి 1;
}

రకనిర్వచనం నిర్మాణం
{
   stbi_uc కొలత,type,channel;
} stbi__pic_packet;

స్థిర stbi_uc *stbi__readval(stbi__context *s, పూర్ణాంకం channel, stbi_uc *dest)
{
   పూర్ణాంకం ముసుగు=0x80, i;

   కొరకు (i=0; i<4; ++i, ముసుగు>>=1) {
      ఒకవేళ (channel & ముసుగు) {
         ఒకవేళ (stbi__at_eof(s)) తిరిగి stbi__errpuc("bad file","PIC file too short");
         dest[i]=stbi__get8(s);
      }
   }

   తిరిగి dest;
}

స్థిర శూన్య stbi__copyval(పూర్ణాంకం channel,stbi_uc *dest,స్థిరరాశి stbi_uc *src)
{
   పూర్ణాంకం ముసుగు=0x80,i;

   కొరకు (i=0;i<4; ++i, ముసుగు>>=1)
      ఒకవేళ (channel&ముసుగు)
         dest[i]=src[i];
}

స్థిర stbi_uc *stbi_pic_ఎక్కించు_core(stbi__context *s,పూర్ణాంకం వెడల్పు,పూర్ణాంకం ఎత్తు,పూర్ణాంకం *comp, stbi_uc *ఫలితం)
{
   పూర్ణాంకం act_comp=0,అంకె_packets=0,y,chained;
   stbi__pic_packet packets[10];

   // this will (should...) cater for even some bizarre stuff like having data
    // for the same channel in multiple packets.
   చేయి {
      stbi__pic_packet *packet;

      ఒకవేళ (అంకె_packets==పరిమాణం(packets)/పరిమాణం(packets[0]))
         తిరిగి stbi__errpuc("bad format","too many packets");

      packet = &packets[అంకె_packets++];

      chained = stbi__get8(s);
      packet->కొలత    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);

      act_comp |= packet->channel;

      ఒకవేళ (stbi__at_eof(s))          తిరిగి stbi__errpuc("bad file","file too short (reading packets)");
      ఒకవేళ (packet->కొలత != 8)  తిరిగి stbi__errpuc("bad format","packet isn't 8bpp");
   } అయితే (chained);

   *comp = (act_comp & 0x10 ? 4 : 3); // has alpha channel?

   కొరకు(y=0; y<ఎత్తు; ++y) {
      పూర్ణాంకం packet_idx;

      కొరకు(packet_idx=0; packet_idx < అంకె_packets; ++packet_idx) {
         stbi__pic_packet *packet = &packets[packet_idx];
         stbi_uc *dest = ఫలితం+y*వెడల్పు*4;

         ఎంపిక (packet->type) {
            అప్రమేయ:
               తిరిగి stbi__errpuc("bad format","packet has bad compression type");

            స్థితి 0: {//uncompressed
               పూర్ణాంకం x;

               కొరకు(x=0;x<వెడల్పు;++x, dest+=4)
                  ఒకవేళ (!stbi__readval(s,packet->channel,dest))
                     తిరిగి 0;
               ఆపు;
            }

            స్థితి 1://Pure RLE
               {
                  పూర్ణాంకం ఎడమ=వెడల్పు, i;

                  అయితే (ఎడమ>0) {
                     stbi_uc లెక్క,విలువ[4];

                     లెక్క=stbi__get8(s);
                     ఒకవేళ (stbi__at_eof(s))   తిరిగి stbi__errpuc("bad file","file too short (pure read count)");

                     ఒకవేళ (లెక్క > ఎడమ)
                        లెక్క = (stbi_uc) ఎడమ;

                     ఒకవేళ (!stbi__readval(s,packet->channel,విలువ))  తిరిగి 0;

                     కొరకు(i=0; i<లెక్క; ++i,dest+=4)
                        stbi__copyval(packet->channel,dest,విలువ);
                     ఎడమ -= లెక్క;
                  }
               }
               ఆపు;

            స్థితి 2: {//Mixed RLE
               పూర్ణాంకం ఎడమ=వెడల్పు;
               అయితే (ఎడమ>0) {
                  పూర్ణాంకం లెక్క = stbi__get8(s), i;
                  ఒకవేళ (stbi__at_eof(s))  తిరిగి stbi__errpuc("bad file","file too short (mixed read count)");

                  ఒకవేళ (లెక్క >= 128) { // Repeated
                     stbi_uc విలువ[4];

                     ఒకవేళ (లెక్క==128)
                        లెక్క = stbi__get16be(s);
                     లేకపోతే
                        లెక్క -= 127;
                     ఒకవేళ (లెక్క > ఎడమ)
                        తిరిగి stbi__errpuc("bad file","scanline overrun");

                     ఒకవేళ (!stbi__readval(s,packet->channel,విలువ))
                        తిరిగి 0;

                     కొరకు(i=0;i<లెక్క;++i, dest += 4)
                        stbi__copyval(packet->channel,dest,విలువ);
                  } లేకపోతే { // Raw
                     ++లెక్క;
                     ఒకవేళ (లెక్క>ఎడమ) తిరిగి stbi__errpuc("bad file","scanline overrun");

                     కొరకు(i=0;i<లెక్క;++i, dest+=4)
                        ఒకవేళ (!stbi__readval(s,packet->channel,dest))
                           తిరిగి 0;
                  }
                  ఎడమ-=లెక్క;
               }
               ఆపు;
            }
         }
      }
   }

   తిరిగి ఫలితం;
}

స్థిర శూన్య *stbi_pic_ఎక్కించు(stbi__context *s,పూర్ణాంకం *px,పూర్ణాంకం *py,పూర్ణాంకం *comp,పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   stbi_uc *ఫలితం;
   పూర్ణాంకం i, x,y, అంతర్గత_comp;
   STBI_NOTUSED(ri);

   ఒకవేళ (!comp) comp = &అంతర్గత_comp;

   కొరకు (i=0; i<92; ++i)
      stbi__get8(s);

   x = stbi__get16be(s);
   y = stbi__get16be(s);
   ఒకవేళ (stbi__at_eof(s))  తిరిగి stbi__errpuc("bad file","file too short (pic header)");
   ఒకవేళ (!stbi__mad3sizes_valid(x, y, 4, 0)) తిరిగి stbi__errpuc("too large", "PIC image too large to decode");

   stbi__get32be(s); //skip `ratio'
   stbi__get16be(s); //skip `fields'
   stbi__get16be(s); //skip `pad'

   // intermediate buffer is RGBA
   ఫలితం = (stbi_uc *) stbi__malloc_mad3(x, y, 4, 0);
   memset(ఫలితం, 0xff, x*y*4);

   ఒకవేళ (!stbi_pic_ఎక్కించు_core(s,x,y,comp, ఫలితం)) {
      STBI_FREE(ఫలితం);
      ఫలితం=0;
   }
   *px = x;
   *py = y;
   ఒకవేళ (req_comp == 0) req_comp = *comp;
   ఫలితం=stbi__convert_format(ఫలితం,4,req_comp,x,y);

   తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం stbi__pic_test(stbi__context *s)
{
   పూర్ణాంకం r = stbi__pic_test_core(s);
   stbi__rewind(s);
   తిరిగి r;
}
#అయితే_ముగింపు

// *************************************************************************************************
// GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb

#నిర్వచితం_కాకపోతే STBI_NO_GIF
రకనిర్వచనం నిర్మాణం
{
   stbi__int16 ఉపసర్గ;
   stbi_uc మొదటి;
   stbi_uc suffix;
} stbi__gif_lzw;

రకనిర్వచనం నిర్మాణం
{
   పూర్ణాంకం w,h;
   stbi_uc *బహిర్గతం;                 // output buffer (always 4 components)
   stbi_uc *background;          // The current "background" as far as a gif is concerned
   stbi_uc *history; 
   పూర్ణాంకం flags, bgindex, నిష్పత్తి, transparent, eflags;
   stbi_uc  pal[256][4];
   stbi_uc lpal[256][4];
   stbi__gif_lzw codes[8192];
   stbi_uc *color_పట్టిక;
   పూర్ణాంకం parse, అడుగు;
   పూర్ణాంకం lflags;
   పూర్ణాంకం ప్రారంభం_x, ప్రారంభం_y;
   పూర్ణాంకం max_x, max_y;
   పూర్ణాంకం cur_x, cur_y;
   పూర్ణాంకం పంక్తి_కొలత;
   పూర్ణాంకం delay;
} stbi__gif;

స్థిర పూర్ణాంకం stbi__gif_test_raw(stbi__context *s)
{
   పూర్ణాంకం sz;
   ఒకవేళ (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8') తిరిగి 0;
   sz = stbi__get8(s);
   ఒకవేళ (sz != '9' && sz != '7') తిరిగి 0;
   ఒకవేళ (stbi__get8(s) != 'a') తిరిగి 0;
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__gif_test(stbi__context *s)
{
   పూర్ణాంకం r = stbi__gif_test_raw(s);
   stbi__rewind(s);
   తిరిగి r;
}

స్థిర శూన్య stbi__gif_parse_colortable(stbi__context *s, stbi_uc pal[256][4], పూర్ణాంకం అంకె_entries, పూర్ణాంకం transp)
{
   పూర్ణాంకం i;
   కొరకు (i=0; i < అంకె_entries; ++i) {
      pal[i][2] = stbi__get8(s);
      pal[i][1] = stbi__get8(s);
      pal[i][0] = stbi__get8(s);
      pal[i][3] = transp == i ? 0 : 255;
   }
}

స్థిర పూర్ణాంకం stbi_gif_శీర్షిక(stbi__context *s, stbi__gif *g, పూర్ణాంకం *comp, పూర్ణాంకం is_info)
{
   stbi_uc సంస్కరణ;
   ఒకవేళ (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8')
      తిరిగి stbi__err("not GIF", "Corrupt GIF");

   సంస్కరణ = stbi__get8(s);
   ఒకవేళ (సంస్కరణ != '7' && సంస్కరణ != '9')    తిరిగి stbi__err("not GIF", "Corrupt GIF");
   ఒకవేళ (stbi__get8(s) != 'a')                తిరిగి stbi__err("not GIF", "Corrupt GIF");

   stbi__g_failure_reason = "";
   g->w = stbi__get16le(s);
   g->h = stbi__get16le(s);
   g->flags = stbi__get8(s);
   g->bgindex = stbi__get8(s);
   g->నిష్పత్తి = stbi__get8(s);
   g->transparent = -1;

   ఒకవేళ (comp != 0) *comp = 4;  // can't actually tell whether it's 3 or 4 until we parse the comments

   ఒకవేళ (is_info) తిరిగి 1;

   ఒకవేళ (g->flags & 0x80)
      stbi__gif_parse_colortable(s,g->pal, 2 << (g->flags & 7), -1);

   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__gif_info_raw(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   stbi__gif* g = (stbi__gif*) stbi__malloc(పరిమాణం(stbi__gif));
   ఒకవేళ (!stbi_gif_శీర్షిక(s, g, comp, 1)) {
      STBI_FREE(g);
      stbi__rewind( s );
      తిరిగి 0;
   }
   ఒకవేళ (x) *x = g->w;
   ఒకవేళ (y) *y = g->h;
   STBI_FREE(g);
   తిరిగి 1;
}

స్థిర శూన్య stbi_బహిర్గతం_gif_సంకేతం(stbi__gif *g, stbi__uint16 సంకేతం_2)
{
   stbi_uc *p, *c;
   పూర్ణాంకం idx; 

   // recurse to decode the prefixes, since the linked-list is backwards,
   // and working backwards through an interleaved image would be nasty
   ఒకవేళ (g->codes[సంకేతం_2].ఉపసర్గ >= 0)
      stbi_బహిర్గతం_gif_సంకేతం(g, g->codes[సంకేతం_2].ఉపసర్గ);

   ఒకవేళ (g->cur_y >= g->max_y) తిరిగి;

   idx = g->cur_x + g->cur_y; 
   p = &g->బహిర్గతం[idx];
   g->history[idx / 4] = 1;  

   c = &g->color_పట్టిక[g->codes[సంకేతం_2].suffix * 4];
   ఒకవేళ (c[3] > 128) { // don't render transparent pixels; 
      p[0] = c[2];
      p[1] = c[1];
      p[2] = c[0];
      p[3] = c[3];
   }
   g->cur_x += 4;

   ఒకవేళ (g->cur_x >= g->max_x) {
      g->cur_x = g->ప్రారంభం_x;
      g->cur_y += g->అడుగు;

      అయితే (g->cur_y >= g->max_y && g->parse > 0) {
         g->అడుగు = (1 << g->parse) * g->పంక్తి_కొలత;
         g->cur_y = g->ప్రారంభం_y + (g->అడుగు >> 1);
         --g->parse;
      }
   }
}

స్థిర stbi_uc *stbi__process_gif_raster(stbi__context *s, stbi__gif *g)
{
   stbi_uc lzw_cs;
   stbi__int32 len, init_సంకేతం;
   stbi__uint32 మొదటి;
   stbi__int32 codesize, codemask, avail, oldcode, బిట్లు, valid_బిట్లు, clear;
   stbi__gif_lzw *p;

   lzw_cs = stbi__get8(s);
   ఒకవేళ (lzw_cs > 12) తిరిగి ఖాళీ;
   clear = 1 << lzw_cs;
   మొదటి = 1;
   codesize = lzw_cs + 1;
   codemask = (1 << codesize) - 1;
   బిట్లు = 0;
   valid_బిట్లు = 0;
   కొరకు (init_సంకేతం = 0; init_సంకేతం < clear; init_సంకేతం++) {
      g->codes[init_సంకేతం].ఉపసర్గ = -1;
      g->codes[init_సంకేతం].మొదటి = (stbi_uc) init_సంకేతం;
      g->codes[init_సంకేతం].suffix = (stbi_uc) init_సంకేతం;
   }

   // support no starting clear code
   avail = clear+2;
   oldcode = -1;

   len = 0;
   కొరకు(;;) {
      ఒకవేళ (valid_బిట్లు < codesize) {
         ఒకవేళ (len == 0) {
            len = stbi__get8(s); // start new block
            ఒకవేళ (len == 0)
               తిరిగి g->బహిర్గతం;
         }
         --len;
         బిట్లు |= (stbi__int32) stbi__get8(s) << valid_బిట్లు;
         valid_బిట్లు += 8;
      } లేకపోతే {
         stbi__int32 సంకేతం_2 = బిట్లు & codemask;
         బిట్లు >>= codesize;
         valid_బిట్లు -= codesize;
         // @OPTIMIZE: is there some way we can accelerate the non-clear path?
         ఒకవేళ (సంకేతం_2 == clear) {  // clear code
            codesize = lzw_cs + 1;
            codemask = (1 << codesize) - 1;
            avail = clear + 2;
            oldcode = -1;
            మొదటి = 0;
         } లేకపోతే ఒకవేళ (సంకేతం_2 == clear + 1) { // end of stream code
            stbi__skip(s, len);
            అయితే ((len = stbi__get8(s)) > 0)
               stbi__skip(s,len);
            తిరిగి g->బహిర్గతం;
         } లేకపోతే ఒకవేళ (సంకేతం_2 <= avail) {
            ఒకవేళ (మొదటి) {
               తిరిగి stbi__errpuc("no clear code", "Corrupt GIF");
            }

            ఒకవేళ (oldcode >= 0) {
               p = &g->codes[avail++];
               ఒకవేళ (avail > 8192) {
                  తిరిగి stbi__errpuc("too many codes", "Corrupt GIF");
               }

               p->ఉపసర్గ = (stbi__int16) oldcode;
               p->మొదటి = g->codes[oldcode].మొదటి;
               p->suffix = (సంకేతం_2 == avail) ? p->మొదటి : g->codes[సంకేతం_2].మొదటి;
            } లేకపోతే ఒకవేళ (సంకేతం_2 == avail)
               తిరిగి stbi__errpuc("illegal code in raster", "Corrupt GIF");

            stbi_బహిర్గతం_gif_సంకేతం(g, (stbi__uint16) సంకేతం_2);

            ఒకవేళ ((avail & codemask) == 0 && avail <= 0x0FFF) {
               codesize++;
               codemask = (1 << codesize) - 1;
            }

            oldcode = సంకేతం_2;
         } లేకపోతే {
            తిరిగి stbi__errpuc("illegal code in raster", "Corrupt GIF");
         }
      }
   }
}

// this function is designed to support animated gifs, although stb_image doesn't support it
// two back is the image from two frames ago, used for a very specific disposal format
స్థిర stbi_uc *stbi_gif_ఎక్కించు_తదుపరి(stbi__context *s, stbi__gif *g, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_uc *two_వెనుక)
{
   పూర్ణాంకం dispose; 
   పూర్ణాంకం మొదటి_చట్రం; 
   పూర్ణాంకం pi; 
   పూర్ణాంకం pcount; 

   // on first frame, any non-written pixels get the background colour (non-transparent)
   మొదటి_చట్రం = 0; 
   ఒకవేళ (g->బహిర్గతం == 0) {
      ఒకవేళ (!stbi_gif_శీర్షిక(s, g, comp,0))     తిరిగి 0; // stbi__g_failure_reason set by stbi__gif_header
      g->బహిర్గతం = (stbi_uc *) stbi__malloc(4 * g->w * g->h);
      g->background = (stbi_uc *) stbi__malloc(4 * g->w * g->h); 
      g->history = (stbi_uc *) stbi__malloc(g->w * g->h); 
      ఒకవేళ (g->బహిర్గతం == 0)                      తిరిగి stbi__errpuc("outofmem", "Out of memory");

      // image is treated as "tranparent" at the start - ie, nothing overwrites the current background; 
      // background colour is only used for pixels that are not rendered first frame, after that "background"
      // color refers to teh color that was there the previous frame. 
      memset( g->బహిర్గతం, 0x00, 4 * g->w * g->h ); 
      memset( g->background, 0x00, 4 * g->w * g->h ); // state of the background (starts transparent)
      memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame
      మొదటి_చట్రం = 1; 
   } లేకపోతే {
      // second frame - how do we dispoase of the previous one?
      dispose = (g->eflags & 0x1C) >> 2; 
      pcount = g->w * g->h; 

      ఒకవేళ ((dispose == 3) && (two_వెనుక == 0)) {
         dispose = 2; // if I don't have an image to revert back to, default to the old background
      }

      ఒకవేళ (dispose == 3) { // use previous graphic
         కొరకు (pi = 0; pi < pcount; ++pi) {
            ఒకవేళ (g->history[pi]) {
               జ్ఞాపకం_నకలు( &g->బహిర్గతం[pi * 4], &two_వెనుక[pi * 4], 4 ); 
            }
         }
      } లేకపోతే ఒకవేళ (dispose == 2) { 
         // restore what was changed last frame to background before that frame; 
         కొరకు (pi = 0; pi < pcount; ++pi) {
            ఒకవేళ (g->history[pi]) {
               జ్ఞాపకం_నకలు( &g->బహిర్గతం[pi * 4], &g->background[pi * 4], 4 ); 
            }
         }
      } లేకపోతే {
         // This is a non-disposal case eithe way, so just 
         // leave the pixels as is, and they will become the new background
         // 1: do not dispose
         // 0:  not specified.
      }

      // background is what out is after the undoing of the previou frame; 
      జ్ఞాపకం_నకలు( g->background, g->బహిర్గతం, 4 * g->w * g->h ); 
   }

   // clear my history; 
   memset( g->history, 0x00, g->w * g->h );        // pixels that were affected previous frame

   కొరకు (;;) {
      పూర్ణాంకం ట్యాగ్ = stbi__get8(s); 
      ఎంపిక (ట్యాగ్) {
         స్థితి 0x2C: /* Image Descriptor */
         {
            stbi__int32 x, y, w, h;
            stbi_uc *o;

            x = stbi__get16le(s);
            y = stbi__get16le(s);
            w = stbi__get16le(s);
            h = stbi__get16le(s);
            ఒకవేళ (((x + w) > (g->w)) || ((y + h) > (g->h)))
               తిరిగి stbi__errpuc("bad Image Descriptor", "Corrupt GIF");

            g->పంక్తి_కొలత = g->w * 4;
            g->ప్రారంభం_x = x * 4;
            g->ప్రారంభం_y = y * g->పంక్తి_కొలత;
            g->max_x   = g->ప్రారంభం_x + w * 4;
            g->max_y   = g->ప్రారంభం_y + h * g->పంక్తి_కొలత;
            g->cur_x   = g->ప్రారంభం_x;
            g->cur_y   = g->ప్రారంభం_y;

            g->lflags = stbi__get8(s);

            ఒకవేళ (g->lflags & 0x40) {
               g->అడుగు = 8 * g->పంక్తి_కొలత; // first interlaced spacing
               g->parse = 3;
            } లేకపోతే {
               g->అడుగు = g->పంక్తి_కొలత;
               g->parse = 0;
            }

            ఒకవేళ (g->lflags & 0x80) {
               stbi__gif_parse_colortable(s,g->lpal, 2 << (g->lflags & 7), g->eflags & 0x01 ? g->transparent : -1);
               g->color_పట్టిక = (stbi_uc *) g->lpal;
            } లేకపోతే ఒకవేళ (g->flags & 0x80) {
               g->color_పట్టిక = (stbi_uc *) g->pal;
            } లేకపోతే
               తిరిగి stbi__errpuc("missing color table", "Corrupt GIF");            
            
            o = stbi__process_gif_raster(s, g);
            ఒకవేళ (o == ఖాళీ) తిరిగి ఖాళీ;

            // if this was the first frame, 
            pcount = g->w * g->h; 
            ఒకవేళ (మొదటి_చట్రం && (g->bgindex > 0)) {
               // if first frame, any pixel not drawn to gets the background color
               కొరకు (pi = 0; pi < pcount; ++pi) {
                  ఒకవేళ (g->history[pi] == 0) {
                     g->pal[g->bgindex][3] = 255; // just in case it was made transparent, undo that; It will be reset next frame if need be; 
                     జ్ఞాపకం_నకలు( &g->బహిర్గతం[pi * 4], &g->pal[g->bgindex], 4 ); 
                  }
               }
            }

            తిరిగి o;
         }

         స్థితి 0x21: // Comment Extension.
         {
            పూర్ణాంకం len;
            పూర్ణాంకం ext = stbi__get8(s); 
            ఒకవేళ (ext == 0xF9) { // Graphic Control Extension.
               len = stbi__get8(s);
               ఒకవేళ (len == 4) {
                  g->eflags = stbi__get8(s);
                  g->delay = 10 * stbi__get16le(s); // delay - 1/100th of a second, saving as 1/1000ths.

                  // unset old transparent
                  ఒకవేళ (g->transparent >= 0) {
                     g->pal[g->transparent][3] = 255; 
                  } 
                  ఒకవేళ (g->eflags & 0x01) {
                     g->transparent = stbi__get8(s);
                     ఒకవేళ (g->transparent >= 0) {
                        g->pal[g->transparent][3] = 0; 
                     }
                  } లేకపోతే {
                     // don't need transparent
                     stbi__skip(s, 1); 
                     g->transparent = -1; 
                  }
               } లేకపోతే {
                  stbi__skip(s, len);
                  ఆపు;
               }
            } 
            అయితే ((len = stbi__get8(s)) != 0) {
               stbi__skip(s, len);
            }
            ఆపు;
         }

         స్థితి 0x3B: // gif stream termination code
            తిరిగి (stbi_uc *) s; // using '1' causes warning on some compilers

         అప్రమేయ:
            తిరిగి stbi__errpuc("unknown code", "Corrupt GIF");
      }
   }
}

స్థిర శూన్య *stbi_ఎక్కించు_gif_main(stbi__context *s, పూర్ణాంకం **delays, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *z, పూర్ణాంకం *comp, పూర్ణాంకం req_comp)
{
   ఒకవేళ (stbi__gif_test(s)) {
      పూర్ణాంకం layers = 0; 
      stbi_uc *u = 0;
      stbi_uc *బహిర్గతం = 0;
      stbi_uc *two_వెనుక = 0; 
      stbi__gif g;
      పూర్ణాంకం stride; 
      memset(&g, 0, పరిమాణం(g));
      ఒకవేళ (delays) {
         *delays = 0; 
      }

      చేయి {
         u = stbi_gif_ఎక్కించు_తదుపరి(s, &g, comp, req_comp, two_వెనుక);
         ఒకవేళ (u == (stbi_uc *) s) u = 0;  // end of animated gif marker

         ఒకవేళ (u) {
            *x = g.w;
            *y = g.h;
            ++layers; 
            stride = g.w * g.h * 4; 
         
            ఒకవేళ (బహిర్గతం) {
               బహిర్గతం = (stbi_uc*) STBI_REALLOC( బహిర్గతం, layers * stride ); 
               ఒకవేళ (delays) {
                  *delays = (పూర్ణాంకం*) STBI_REALLOC( *delays, పరిమాణం(పూర్ణాంకం) * layers ); 
               }
            } లేకపోతే {
               బహిర్గతం = (stbi_uc*)stbi__malloc( layers * stride ); 
               ఒకవేళ (delays) {
                  *delays = (పూర్ణాంకం*) stbi__malloc( layers * పరిమాణం(పూర్ణాంకం) ); 
               }
            }
            జ్ఞాపకం_నకలు( బహిర్గతం + ((layers - 1) * stride), u, stride ); 
            ఒకవేళ (layers >= 2) {
               two_వెనుక = బహిర్గతం - 2 * stride; 
            }

            ఒకవేళ (delays) {
               (*delays)[layers - 1U] = g.delay; 
            }
         }
      } అయితే (u != 0); 

      // free temp buffer; 
      STBI_FREE(g.బహిర్గతం); 
      STBI_FREE(g.history); 
      STBI_FREE(g.background); 

      // do the final conversion after loading everything; 
      ఒకవేళ (req_comp && req_comp != 4)
         బహిర్గతం = stbi__convert_format(బహిర్గతం, 4, req_comp, layers * g.w, g.h);

      *z = layers; 
      తిరిగి బహిర్గతం;
   } లేకపోతే {
      తిరిగి stbi__errpuc("not GIF", "Image was not as a gif type."); 
   }
}

స్థిర శూన్య *stbi_gif_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   stbi_uc *u = 0;
   stbi__gif g;
   memset(&g, 0, పరిమాణం(g));

   u = stbi_gif_ఎక్కించు_తదుపరి(s, &g, comp, req_comp, 0);
   ఒకవేళ (u == (stbi_uc *) s) u = 0;  // end of animated gif marker
   ఒకవేళ (u) {
      *x = g.w;
      *y = g.h;

      // moved conversion to after successful load so that the same
      // can be done for multiple frames. 
      ఒకవేళ (req_comp && req_comp != 4)
         u = stbi__convert_format(u, 4, req_comp, g.w, g.h);
   }

   // free buffers needed for multiple frame loading; 
   STBI_FREE(g.history);
   STBI_FREE(g.background); 

   తిరిగి u;
}

స్థిర పూర్ణాంకం stbi__gif_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   తిరిగి stbi__gif_info_raw(s,x,y,comp);
}
#అయితే_ముగింపు

// *************************************************************************************************
// Radiance RGBE HDR loader
// originally by Nicolas Schulz
#నిర్వచితం_కాకపోతే STBI_NO_HDR
స్థిర పూర్ణాంకం stbi__hdr_test_core(stbi__context *s, స్థిరరాశి అక్షరం *signature)
{
   పూర్ణాంకం i;
   కొరకు (i=0; signature[i]; ++i)
      ఒకవేళ (stbi__get8(s) != signature[i])
          తిరిగి 0;
   stbi__rewind(s);
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__hdr_test(stbi__context* s)
{
   పూర్ణాంకం r = stbi__hdr_test_core(s, "#?RADIANCE\n");
   stbi__rewind(s);
   ఒకవేళ(!r) {
       r = stbi__hdr_test_core(s, "#?RGBE\n");
       stbi__rewind(s);
   }
   తిరిగి r;
}

#నిర్వచించు STBI__HDR_BUFLEN  1024
స్థిర అక్షరం *stbi__hdr_gettoken(stbi__context *z, అక్షరం *బఫరు)
{
   పూర్ణాంకం len=0;
   అక్షరం c = '\0';

   c = (అక్షరం) stbi__get8(z);

   అయితే (!stbi__at_eof(z) && c != '\n') {
      బఫరు[len++] = c;
      ఒకవేళ (len == STBI__HDR_BUFLEN-1) {
         // flush to end of line
         అయితే (!stbi__at_eof(z) && stbi__get8(z) != '\n')
            ;
         ఆపు;
      }
      c = (అక్షరం) stbi__get8(z);
   }

   బఫరు[len] = 0;
   తిరిగి బఫరు;
}

స్థిర శూన్య stbi__hdr_convert(దశాంశం *output, stbi_uc *ఆదానం, పూర్ణాంకం req_comp)
{
   ఒకవేళ ( ఆదానం[3] != 0 ) {
      దశాంశం f1;
      // Exponent
      f1 = (దశాంశం) ldexp(1.0f, ఆదానం[3] - (పూర్ణాంకం)(128 + 8));
      ఒకవేళ (req_comp <= 2)
         output[0] = (ఆదానం[0] + ఆదానం[1] + ఆదానం[2]) * f1 / 3;
      లేకపోతే {
         output[0] = ఆదానం[0] * f1;
         output[1] = ఆదానం[1] * f1;
         output[2] = ఆదానం[2] * f1;
      }
      ఒకవేళ (req_comp == 2) output[1] = 1;
      ఒకవేళ (req_comp == 4) output[3] = 1;
   } లేకపోతే {
      ఎంపిక (req_comp) {
         స్థితి 4: output[3] = 1; /* fallthrough */
         స్థితి 3: output[0] = output[1] = output[2] = 0;
                 ఆపు;
         స్థితి 2: output[1] = 1; /* fallthrough */
         స్థితి 1: output[0] = 0;
                 ఆపు;
      }
   }
}

స్థిర దశాంశం *stbi_hdr_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   అక్షరం బఫరు[STBI__HDR_BUFLEN];
   అక్షరం *token;
   పూర్ణాంకం valid = 0;
   పూర్ణాంకం వెడల్పు, ఎత్తు;
   stbi_uc *scanline;
   దశాంశం *hdr_దత్తాంశం;
   పూర్ణాంకం len;
   గుర్తులేని అక్షరం లెక్క, విలువ;
   పూర్ణాంకం i, j, k, c1,c2, z;
   స్థిరరాశి అక్షరం *శీర్షిక_Token;
   STBI_NOTUSED(ri);

   // Check identifier
   శీర్షిక_Token = stbi__hdr_gettoken(s,బఫరు);
   ఒకవేళ (strcmp(శీర్షిక_Token, "#?RADIANCE") != 0 && strcmp(శీర్షిక_Token, "#?RGBE") != 0)
      తిరిగి stbi__errpf("not HDR", "Corrupt HDR image");

   // Parse header
   కొరకు(;;) {
      token = stbi__hdr_gettoken(s,బఫరు);
      ఒకవేళ (token[0] == 0) ఆపు;
      ఒకవేళ (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   ఒకవేళ (!valid)    తిరిగి stbi__errpf("unsupported format", "Unsupported HDR format");

   // Parse width and height
   // can't use sscanf() if we're not using stdio!
   token = stbi__hdr_gettoken(s,బఫరు);
   ఒకవేళ (అక్షరశ్రేణి_పోలిక(token, "-Y ", 3))  తిరిగి stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   ఎత్తు = (పూర్ణాంకం) strtol(token, &token, 10);
   అయితే (*token == ' ') ++token;
   ఒకవేళ (అక్షరశ్రేణి_పోలిక(token, "+X ", 3))  తిరిగి stbi__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   వెడల్పు = (పూర్ణాంకం) strtol(token, ఖాళీ, 10);

   *x = వెడల్పు;
   *y = ఎత్తు;

   ఒకవేళ (comp) *comp = 3;
   ఒకవేళ (req_comp == 0) req_comp = 3;

   ఒకవేళ (!stbi__mad4sizes_valid(వెడల్పు, ఎత్తు, req_comp, పరిమాణం(దశాంశం), 0))
      తిరిగి stbi__errpf("too large", "HDR image is too large");

   // Read data
   hdr_దత్తాంశం = (దశాంశం *) stbi__malloc_mad4(వెడల్పు, ఎత్తు, req_comp, పరిమాణం(దశాంశం), 0);
   ఒకవేళ (!hdr_దత్తాంశం)
      తిరిగి stbi__errpf("outofmem", "Out of memory");

   // Load image data
   // image data is stored as some number of sca
   ఒకవేళ ( వెడల్పు < 8 || వెడల్పు >= 32768) {
      // Read flat data
      కొరకు (j=0; j < ఎత్తు; ++j) {
         కొరకు (i=0; i < వెడల్పు; ++i) {
            stbi_uc rgbe[4];
           main_decode_loop:
            stbi__getn(s, rgbe, 4);
            stbi__hdr_convert(hdr_దత్తాంశం + j * వెడల్పు * req_comp + i * req_comp, rgbe, req_comp);
         }
      }
   } లేకపోతే {
      // Read RLE-encoded data
      scanline = ఖాళీ;

      కొరకు (j = 0; j < ఎత్తు; ++j) {
         c1 = stbi__get8(s);
         c2 = stbi__get8(s);
         len = stbi__get8(s);
         ఒకవేళ (c1 != 2 || c2 != 2 || (len & 0x80)) {
            // not run-length encoded, so we have to actually use THIS data as a decoded
            // pixel (note this can't be a valid pixel--one of RGB must be >= 128)
            stbi_uc rgbe[4];
            rgbe[0] = (stbi_uc) c1;
            rgbe[1] = (stbi_uc) c2;
            rgbe[2] = (stbi_uc) len;
            rgbe[3] = (stbi_uc) stbi__get8(s);
            stbi__hdr_convert(hdr_దత్తాంశం, rgbe, req_comp);
            i = 1;
            j = 0;
            STBI_FREE(scanline);
            వెళ్ళు main_decode_loop; // yes, this makes no sense
         }
         len <<= 8;
         len |= stbi__get8(s);
         ఒకవేళ (len != వెడల్పు) { STBI_FREE(hdr_దత్తాంశం); STBI_FREE(scanline); తిరిగి stbi__errpf("invalid decoded scanline length", "corrupt HDR"); }
         ఒకవేళ (scanline == ఖాళీ) {
            scanline = (stbi_uc *) stbi__malloc_mad2(వెడల్పు, 4, 0);
            ఒకవేళ (!scanline) {
               STBI_FREE(hdr_దత్తాంశం);
               తిరిగి stbi__errpf("outofmem", "Out of memory");
            }
         }

         కొరకు (k = 0; k < 4; ++k) {
            పూర్ణాంకం nleft;
            i = 0;
            అయితే ((nleft = వెడల్పు - i) > 0) {
               లెక్క = stbi__get8(s);
               ఒకవేళ (లెక్క > 128) {
                  // Run
                  విలువ = stbi__get8(s);
                  లెక్క -= 128;
                  ఒకవేళ (లెక్క > nleft) { STBI_FREE(hdr_దత్తాంశం); STBI_FREE(scanline); తిరిగి stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  కొరకు (z = 0; z < లెక్క; ++z)
                     scanline[i++ * 4 + k] = విలువ;
               } లేకపోతే {
                  // Dump
                  ఒకవేళ (లెక్క > nleft) { STBI_FREE(hdr_దత్తాంశం); STBI_FREE(scanline); తిరిగి stbi__errpf("corrupt", "bad RLE data in HDR"); }
                  కొరకు (z = 0; z < లెక్క; ++z)
                     scanline[i++ * 4 + k] = stbi__get8(s);
               }
            }
         }
         కొరకు (i=0; i < వెడల్పు; ++i)
            stbi__hdr_convert(hdr_దత్తాంశం+(j*వెడల్పు + i)*req_comp, scanline + i*4, req_comp);
      }
      ఒకవేళ (scanline)
         STBI_FREE(scanline);
   }

   తిరిగి hdr_దత్తాంశం;
}

స్థిర పూర్ణాంకం stbi__hdr_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   అక్షరం బఫరు[STBI__HDR_BUFLEN];
   అక్షరం *token;
   పూర్ణాంకం valid = 0;
   పూర్ణాంకం dummy;

   ఒకవేళ (!x) x = &dummy;
   ఒకవేళ (!y) y = &dummy;
   ఒకవేళ (!comp) comp = &dummy;

   ఒకవేళ (stbi__hdr_test(s) == 0) {
       stbi__rewind( s );
       తిరిగి 0;
   }

   కొరకు(;;) {
      token = stbi__hdr_gettoken(s,బఫరు);
      ఒకవేళ (token[0] == 0) ఆపు;
      ఒకవేళ (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   ఒకవేళ (!valid) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   token = stbi__hdr_gettoken(s,బఫరు);
   ఒకవేళ (అక్షరశ్రేణి_పోలిక(token, "-Y ", 3)) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   token += 3;
   *y = (పూర్ణాంకం) strtol(token, &token, 10);
   అయితే (*token == ' ') ++token;
   ఒకవేళ (అక్షరశ్రేణి_పోలిక(token, "+X ", 3)) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   token += 3;
   *x = (పూర్ణాంకం) strtol(token, ఖాళీ, 10);
   *comp = 3;
   తిరిగి 1;
}
#అయితే_ముగింపు // STBI_NO_HDR

#నిర్వచితం_కాకపోతే STBI_NO_BMP
స్థిర పూర్ణాంకం stbi__bmp_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   శూన్య *p;
   stbi_bmp_దత్తాంశం info;

   info.all_a = 255;
   p = stbi_bmp_parse_శీర్షిక(s, &info);
   stbi__rewind( s );
   ఒకవేళ (p == ఖాళీ)
      తిరిగి 0;
   ఒకవేళ (x) *x = s->img_x;
   ఒకవేళ (y) *y = s->img_y;
   ఒకవేళ (comp) *comp = info.ma ? 4 : 3;
   తిరిగి 1;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_PSD
స్థిర పూర్ణాంకం stbi__psd_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   పూర్ణాంకం channel_లెక్క, dummy, లోతు;
   ఒకవేళ (!x) x = &dummy;
   ఒకవేళ (!y) y = &dummy;
   ఒకవేళ (!comp) comp = &dummy;
   ఒకవేళ (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   ఒకవేళ (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   stbi__skip(s, 6);
   channel_లెక్క = stbi__get16be(s);
   ఒకవేళ (channel_లెక్క < 0 || channel_లెక్క > 16) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   *y = stbi__get32be(s);
   *x = stbi__get32be(s);
   లోతు = stbi__get16be(s);
   ఒకవేళ (లోతు != 8 && లోతు != 16) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   ఒకవేళ (stbi__get16be(s) != 3) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   *comp = 4;
   తిరిగి 1;
}

స్థిర పూర్ణాంకం stbi__psd_is16(stbi__context *s)
{
   పూర్ణాంకం channel_లెక్క, లోతు;
   ఒకవేళ (stbi__get32be(s) != 0x38425053) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   ఒకవేళ (stbi__get16be(s) != 1) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   stbi__skip(s, 6);
   channel_లెక్క = stbi__get16be(s);
   ఒకవేళ (channel_లెక్క < 0 || channel_లెక్క > 16) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   (శూన్య) stbi__get32be(s);
   (శూన్య) stbi__get32be(s);
   లోతు = stbi__get16be(s);
   ఒకవేళ (లోతు != 16) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   తిరిగి 1;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే STBI_NO_PIC
స్థిర పూర్ణాంకం stbi__pic_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   పూర్ణాంకం act_comp=0,అంకె_packets=0,chained,dummy;
   stbi__pic_packet packets[10];

   ఒకవేళ (!x) x = &dummy;
   ఒకవేళ (!y) y = &dummy;
   ఒకవేళ (!comp) comp = &dummy;

   ఒకవేళ (!stbi__pic_is4(s,"\x53\x80\xF6\x34")) {
      stbi__rewind(s);
      తిరిగి 0;
   }

   stbi__skip(s, 88);

   *x = stbi__get16be(s);
   *y = stbi__get16be(s);
   ఒకవేళ (stbi__at_eof(s)) {
      stbi__rewind( s);
      తిరిగి 0;
   }
   ఒకవేళ ( (*x) != 0 && (1 << 28) / (*x) < (*y)) {
      stbi__rewind( s );
      తిరిగి 0;
   }

   stbi__skip(s, 8);

   చేయి {
      stbi__pic_packet *packet;

      ఒకవేళ (అంకె_packets==పరిమాణం(packets)/పరిమాణం(packets[0]))
         తిరిగి 0;

      packet = &packets[అంకె_packets++];
      chained = stbi__get8(s);
      packet->కొలత    = stbi__get8(s);
      packet->type    = stbi__get8(s);
      packet->channel = stbi__get8(s);
      act_comp |= packet->channel;

      ఒకవేళ (stbi__at_eof(s)) {
          stbi__rewind( s );
          తిరిగి 0;
      }
      ఒకవేళ (packet->కొలత != 8) {
          stbi__rewind( s );
          తిరిగి 0;
      }
   } అయితే (chained);

   *comp = (act_comp & 0x10 ? 4 : 3);

   తిరిగి 1;
}
#అయితే_ముగింపు

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

#నిర్వచితం_కాకపోతే STBI_NO_PNM

స్థిర పూర్ణాంకం      stbi__pnm_test(stbi__context *s)
{
   అక్షరం p, t;
   p = (అక్షరం) stbi__get8(s);
   t = (అక్షరం) stbi__get8(s);
   ఒకవేళ (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind( s );
       తిరిగి 0;
   }
   తిరిగి 1;
}

స్థిర శూన్య *stbi_pnm_ఎక్కించు(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp, పూర్ణాంకం req_comp, stbi_ఫలితం_info *ri)
{
   stbi_uc *బహిర్గతం;
   STBI_NOTUSED(ri);

   ఒకవేళ (!stbi__pnm_info(s, (పూర్ణాంకం *)&s->img_x, (పూర్ణాంకం *)&s->img_y, (పూర్ణాంకం *)&s->img_n))
      తిరిగి 0;

   *x = s->img_x;
   *y = s->img_y;
   ఒకవేళ (comp) *comp = s->img_n;

   ఒకవేళ (!stbi__mad3sizes_valid(s->img_n, s->img_x, s->img_y, 0))
      తిరిగి stbi__errpuc("too large", "PNM too large");

   బహిర్గతం = (stbi_uc *) stbi__malloc_mad3(s->img_n, s->img_x, s->img_y, 0);
   ఒకవేళ (!బహిర్గతం) తిరిగి stbi__errpuc("outofmem", "Out of memory");
   stbi__getn(s, బహిర్గతం, s->img_n * s->img_x * s->img_y);

   ఒకవేళ (req_comp && req_comp != s->img_n) {
      బహిర్గతం = stbi__convert_format(బహిర్గతం, s->img_n, req_comp, s->img_x, s->img_y);
      ఒకవేళ (బహిర్గతం == ఖాళీ) తిరిగి బహిర్గతం; // stbi__convert_format frees input on failure
   }
   తిరిగి బహిర్గతం;
}

స్థిర పూర్ణాంకం      stbi__pnm_isspace(అక్షరం c)
{
   తిరిగి c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

స్థిర శూన్య     stbi__pnm_skip_whitespace(stbi__context *s, అక్షరం *c)
{
   కొరకు (;;) {
      అయితే (!stbi__at_eof(s) && stbi__pnm_isspace(*c))
         *c = (అక్షరం) stbi__get8(s);

      ఒకవేళ (stbi__at_eof(s) || *c != '#')
         ఆపు;

      అయితే (!stbi__at_eof(s) && *c != '\n' && *c != '\r' )
         *c = (అక్షరం) stbi__get8(s);
   }
}

స్థిర పూర్ణాంకం      stbi__pnm_isdigit(అక్షరం c)
{
   తిరిగి c >= '0' && c <= '9';
}

స్థిర పూర్ణాంకం      stbi__pnm_getinteger(stbi__context *s, అక్షరం *c)
{
   పూర్ణాంకం విలువ = 0;

   అయితే (!stbi__at_eof(s) && stbi__pnm_isdigit(*c)) {
      విలువ = విలువ*10 + (*c - '0');
      *c = (అక్షరం) stbi__get8(s);
   }

   తిరిగి విలువ;
}

స్థిర పూర్ణాంకం      stbi__pnm_info(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   పూర్ణాంకం maxv, dummy;
   అక్షరం c, p, t;

   ఒకవేళ (!x) x = &dummy;
   ఒకవేళ (!y) y = &dummy;
   ఒకవేళ (!comp) comp = &dummy;

   stbi__rewind(s);

   // Get identifier
   p = (అక్షరం) stbi__get8(s);
   t = (అక్షరం) stbi__get8(s);
   ఒకవేళ (p != 'P' || (t != '5' && t != '6')) {
       stbi__rewind(s);
       తిరిగి 0;
   }

   *comp = (t == '6') ? 3 : 1;  // '5' is 1-component .pgm; '6' is 3-component .ppm

   c = (అక్షరం) stbi__get8(s);
   stbi__pnm_skip_whitespace(s, &c);

   *x = stbi__pnm_getinteger(s, &c); // read width
   stbi__pnm_skip_whitespace(s, &c);

   *y = stbi__pnm_getinteger(s, &c); // read height
   stbi__pnm_skip_whitespace(s, &c);

   maxv = stbi__pnm_getinteger(s, &c);  // read max value

   ఒకవేళ (maxv > 255)
      తిరిగి stbi__err("max value > 255", "PPM image not 8-bit");
   లేకపోతే
      తిరిగి 1;
}
#అయితే_ముగింపు

స్థిర పూర్ణాంకం stbi__info_main(stbi__context *s, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   #నిర్వచితం_కాకపోతే STBI_NO_JPEG
   ఒకవేళ (stbi__jpeg_info(s, x, y, comp)) తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_PNG
   ఒకవేళ (stbi__png_info(s, x, y, comp))  తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_GIF
   ఒకవేళ (stbi__gif_info(s, x, y, comp))  తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_BMP
   ఒకవేళ (stbi__bmp_info(s, x, y, comp))  తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_PSD
   ఒకవేళ (stbi__psd_info(s, x, y, comp))  తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_PIC
   ఒకవేళ (stbi__pic_info(s, x, y, comp))  తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_PNM
   ఒకవేళ (stbi__pnm_info(s, x, y, comp))  తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_HDR
   ఒకవేళ (stbi__hdr_info(s, x, y, comp))  తిరిగి 1;
   #అయితే_ముగింపు

   // test tga last because it's a crappy test!
   #నిర్వచితం_కాకపోతే STBI_NO_TGA
   ఒకవేళ (stbi__tga_info(s, x, y, comp))
       తిరిగి 1;
   #అయితే_ముగింపు
   తిరిగి stbi__err("unknown image type", "Image not of any known type, or corrupt");
}

స్థిర పూర్ణాంకం stbi__is_16_main(stbi__context *s)
{
   #నిర్వచితం_కాకపోతే STBI_NO_PNG
   ఒకవేళ (stbi__png_is16(s))  తిరిగి 1;
   #అయితే_ముగింపు

   #నిర్వచితం_కాకపోతే STBI_NO_PSD
   ఒకవేళ (stbi__psd_is16(s))  తిరిగి 1;
   #అయితే_ముగింపు

   తిరిగి 0;
}

#నిర్వచితం_కాకపోతే STBI_NO_STDIO
STBIDEF పూర్ణాంకం stbi_info(అక్షరం స్థిరరాశి *filename, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
    FILE *f = stbi__fopen(filename, "rb");
    పూర్ణాంకం ఫలితం;
    ఒకవేళ (!f) తిరిగి stbi__err("can't fopen", "Unable to open file");
    ఫలితం = stbi_info_from_file(f, x, y, comp);
    fclose(f);
    తిరిగి ఫలితం;
}

STBIDEF పూర్ణాంకం stbi_info_from_file(FILE *f, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   పూర్ణాంకం r;
   stbi__context s;
   దీర్ఘ pos = ftell(f);
   stbi_ప్రారంభం_file(&s, f);
   r = stbi__info_main(&s,x,y,comp);
   fseek(f,pos,SEEK_SET);
   తిరిగి r;
}

STBIDEF పూర్ణాంకం stbi_is_16_బిట్టు(అక్షరం స్థిరరాశి *filename)
{
    FILE *f = stbi__fopen(filename, "rb");
    పూర్ణాంకం ఫలితం;
    ఒకవేళ (!f) తిరిగి stbi__err("can't fopen", "Unable to open file");
    ఫలితం = stbi_is_16_బిట్టు_from_file(f);
    fclose(f);
    తిరిగి ఫలితం;
}

STBIDEF పూర్ణాంకం stbi_is_16_బిట్టు_from_file(FILE *f)
{
   పూర్ణాంకం r;
   stbi__context s;
   దీర్ఘ pos = ftell(f);
   stbi_ప్రారంభం_file(&s, f);
   r = stbi__is_16_main(&s);
   fseek(f,pos,SEEK_SET);
   తిరిగి r;
}
#అయితే_ముగింపు // !STBI_NO_STDIO

STBIDEF పూర్ణాంకం stbi_info_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   stbi__context s;
   stbi_ప్రారంభం_mem(&s,బఫరు,len);
   తిరిగి stbi__info_main(&s,x,y,comp);
}

STBIDEF పూర్ణాంకం stbi_info_from_callbacks(stbi_io_callbacks స్థిరరాశి *c, శూన్య *user, పూర్ణాంకం *x, పూర్ణాంకం *y, పూర్ణాంకం *comp)
{
   stbi__context s;
   stbi_ప్రారంభం_callbacks(&s, (stbi_io_callbacks *) c, user);
   తిరిగి stbi__info_main(&s,x,y,comp);
}

STBIDEF పూర్ణాంకం stbi_is_16_బిట్టు_from_జ్ఞాపకం(stbi_uc స్థిరరాశి *బఫరు, పూర్ణాంకం len)
{
   stbi__context s;
   stbi_ప్రారంభం_mem(&s,బఫరు,len);
   తిరిగి stbi__is_16_main(&s);
}

STBIDEF పూర్ణాంకం stbi_is_16_బిట్టు_from_callbacks(stbi_io_callbacks స్థిరరాశి *c, శూన్య *user)
{
   stbi__context s;
   stbi_ప్రారంభం_callbacks(&s, (stbi_io_callbacks *) c, user);
   తిరిగి stbi__is_16_main(&s);
}

#అయితే_ముగింపు // STB_IMAGE_IMPLEMENTATION

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
