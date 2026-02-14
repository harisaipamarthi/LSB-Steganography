#include <stdio.h>
#include <string.h>
#include "types.h"
#include "decode.h"

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{

    if(argv[2] != NULL)
    {
        if(strstr(argv[2], ".bmp") != NULL)
        {
            printf(".bmp is present\n");
            decInfo -> stego_image_fname = argv[2];
        }
        else
        {
            printf(".bmp is not present\n");
            return e_failure;
        }
    }
    else
    {
        printf("Arguemnt incomplete!!\n");
        printf("Pass correct arguements\n");
        printf("./a.out -e beautiful.bmp secret.txt  --> for encoding\n");
        printf("./a.out -d stego.bmp  --> for decoding\n");
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        if(strstr(argv[3], ".txt") != NULL)
        {
            printf(".txt is present\n");
            decInfo -> output_fname = argv[3];
        }
        else
        {
            decInfo -> output_fname = "output.txt";
        }
    }
    else
    {
        decInfo -> output_fname = "output.txt";
    }

    return e_success;
}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_success)
    {
        printf("All the files are opened successfully\n");
    }
    else
    {
        printf("Files are not opened\n");
        return e_failure;
    }

    if(skip_bmp_header(decInfo -> fptr_stego_image) == e_success)
    {
        printf("Header skipped successfully\n");
    }
    else
    {
        printf("Header not skipped\n");
        return e_failure;
    }

    if(decode_magic_string(decInfo) == e_success)
    {
        printf("Magic string is decoded successfully\n");
    }
    else
    {
        printf("Magic string is not decoded\n");
        return e_failure;
    }

    if(decode_output_file_extn_size(decInfo) == e_success)
    {
        printf("Output File Extension Size is decoded successfully\n");
    }
    else
    {
        printf("Output File Extension Size is not decoded\n");
        return e_failure;
    }

    if(decode_output_file_extn(decInfo) == e_success)
    {
        printf("Output File Extension is decoded successfully\n");
    }
    else
    {
        printf("Output File Extension is not decoded\n");
        return e_failure;
    }

    if(decode_output_file_size(decInfo) == e_success)
    {
        printf("Output File Size is decoded successfully\n");
    }
    else
    {
        printf("Output File Size is not decoded\n");
        return e_failure;
    }

    if(decode_output_file_data(decInfo) == e_success)
    {
        printf("Output File Data is decoded successfully\n");
    }
    else
    {
        printf("Output File Data is not decoded\n");
        return e_failure;
    }

    if(close_decode_files(decInfo) == e_success)
    {
        printf("All the Files are closed successfully\n");
    }
    else
    {
        printf("Files are not closed\n");
        return e_failure;
    }


    return e_success;
}

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");
    if(decInfo -> fptr_stego_image == NULL)
    {
        printf("Stego File is not present\n");
        return e_failure;
    }

    decInfo -> fptr_output = fopen(decInfo -> output_fname, "w");

    return e_success;
}

/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image)
{
    rewind(fptr_stego_image);

    fseek(fptr_stego_image, 54, SEEK_SET);

    return e_success;
}

/* Extract Magic String */
Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("Enter the Magic String : ");
    scanf(" %[^\n]", decInfo -> magic);

    char str[strlen(decInfo -> magic) + 1];

    if(decode_data_from_image(str, strlen(decInfo -> magic), decInfo) == e_success)
    {
        printf("Magic String is decoded\n");
    }
    else
    {
        printf("Magic string is not decoded\n");
        return e_failure;
    }

    str[strlen(decInfo -> magic)] = '\0';

    if(strcmp(str, decInfo -> magic) != 0)
    {
        printf("Magic String does not match\n");
        return e_failure;
    }

    printf("Magic String matches\n");
    return e_success;
}

/* Decode secret file extenstion size */
Status decode_output_file_extn_size(DecodeInfo *decInfo)
{
    decInfo -> size_output_file_extn = decode_size_from_lsb(decInfo);

    return e_success;
}

/* Decode secret file extenstion */
Status decode_output_file_extn(DecodeInfo *decInfo)
{
    char output_file_extn[decInfo -> size_output_file_extn + 1];

    if(decode_data_from_image(output_file_extn, decInfo -> size_output_file_extn, decInfo) == e_success)
    {
        printf("Output File Extention is decoded\n");
    }
    else
    {
        printf("Output File Extention is not decoded\n");
        return e_failure;
    }

    output_file_extn[decInfo -> size_output_file_extn] = '\0';

    return e_success;
}

/* Decode secret file size */
Status decode_output_file_size(DecodeInfo *decInfo)
{
    decInfo -> size_output_file = decode_size_from_lsb(decInfo);

    return e_success;
}

/* Decode secret file data*/
Status decode_output_file_data(DecodeInfo *decInfo)
{
    char buffer[decInfo -> size_output_file + 1];

    if(decode_data_from_image(buffer, decInfo -> size_output_file, decInfo) == e_success)
    {
        printf("Output File Data is decoded\n");
    }
    else
    {
        printf("Output File Data is not decoded\n");
        return e_failure;
    }

    buffer[decInfo -> size_output_file] = '\0';
    
    if(fwrite(buffer , decInfo -> size_output_file, 1, decInfo -> fptr_output) != 1)
    {
        printf("Error in writing to output file\n");
        return e_failure;
    }

    return e_success;
}

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    for(int i = 0; i < size; i++)
    {
        fread(decInfo -> image_data, 8, 1, decInfo -> fptr_stego_image);
        data[i] = decode_byte_from_lsb(decInfo -> image_data);
    }

    return e_success;
}

/* Decode a byte from LSB of image data array */
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;

    for(int i = 0; i < 8; i++)
    {
        data = data | ((image_buffer[i] & 1) << i);
    }

    return data;
}

/* Decode a size from LSB of image data array */
int decode_size_from_lsb(DecodeInfo *decInfo)
{
    char buffer[32];
    int size = 0;

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    for (int i = 0; i < 32; i++)
    {
        size = size | ((buffer[i] & 1) << i);
    }

    return size;
}

/* Close File pointers for i/p and o/p files */
Status close_decode_files(DecodeInfo *decInfo)
{
    fclose(decInfo -> fptr_stego_image);
    fclose(decInfo -> fptr_output);

    return e_success;
}
