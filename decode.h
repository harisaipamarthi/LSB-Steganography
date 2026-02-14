#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file from stego Image
 * Info about input and output data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *output_fname;
    FILE *fptr_output;
    char extn_output_file[MAX_FILE_SUFFIX];
    long size_output_file;
    long size_output_file_extn;


    /*Magic String*/
    char magic[20];

} DecodeInfo;


/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image);

/* Extract Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion size */
Status decode_output_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_output_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_output_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_output_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/* Decode a size from LSB of image data array */
int decode_size_from_lsb(DecodeInfo *decInfo);

/* Close File pointers for i/p and o/p files */
Status close_decode_files(DecodeInfo *decInfo);

#endif
