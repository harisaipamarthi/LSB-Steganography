#include <stdio.h>
#include <string.h>
#include "types.h"
#include "encode.h"

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{

    if(argv[2] != NULL)
    {
        if(strstr(argv[2], ".bmp") != NULL)
        {
            printf(".bmp is present\n");
            encInfo -> src_image_fname = argv[2];
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
            encInfo -> secret_fname = argv[3];
        }
        else
        {
            printf(".txt is not present\n");
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

    if(argv[4] != NULL)
    {
        if(strstr(argv[4], ".bmp") != NULL)
        {
            printf("stego.bmp is present\n");
            encInfo -> stego_image_fname = argv[4];
        }
        else
        {
            encInfo -> stego_image_fname = "stego.bmp";
        }
    }
    else
    {
        encInfo -> stego_image_fname = "stego.bmp";
    }

    return e_success;
}


/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_encode_files(encInfo) == e_success)
    {
        printf("All the Files are opened successfully\n");
    }
    else
    {
        printf("Files are not opened\n");
        return e_failure;
    }

    if(check_capacity(encInfo) == e_success)
    {
        printf("Check Capacity is successful\n");
    }
    else
    {
        printf("Check Capacity is un-successful\n");
        return e_failure;
    }

    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("Header copied successfully\n");
    }
    else
    {
        printf("Header is not copied successfully\n");
        return e_failure;
    }

    if(encode_magic_string(encInfo) == e_success)
    {
        printf("Magic string encoded successfully\n");
    }
    else
    {
        printf("Magic string not encoded successfully\n");
        return e_failure;
    }

    if(encode_secret_file_extn_size(encInfo) == e_success)
    {
        printf("Secret File Extention Size encoded successfully\n");
    }
    else
    {
        printf("Secret File Extention Size not encoded successfully\n");
        return e_failure;
    }

    if(encode_secret_file_extn(encInfo) == e_success)
    {
        printf("Secret File Extention encoded successfully\n");
    }
    else
    {
        printf("Secret File Extention not encoded successfully\n");
        return e_failure;
    }

    if(encode_secret_file_size(encInfo) == e_success)
    {
        printf("Secret File Size encoded successfully\n");
    }
    else
    {
        printf("Secret File Size not encoded successfully\n");
        return e_failure;
    }

    if(encode_secret_file_data(encInfo) == e_success)
    {
        printf("Secret File Data encoded successfully\n");
    }
    else
    {
        printf("Secret File Data not encoded successfully\n");
        return e_failure;
    }

    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("Remaining Image Data copied successfully\n");
    }
    else
    {
        printf("Remaining Image Data is not copied successfully\n");
        return e_failure;
    }

    if(close_encode_files(encInfo) == e_success)
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
Status open_encode_files(EncodeInfo *encInfo)
{
    encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname, "r");
    if(encInfo -> fptr_src_image == NULL)
    {
        printf("Source File is not present\n");
        return e_failure;
    }

    encInfo -> fptr_secret = fopen(encInfo -> secret_fname, "r");
    if(encInfo -> fptr_secret == NULL)
    {
        printf("Secret File is not present\n");
        return e_failure;
    }

    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname, "w");

    return e_success;
}

/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    printf("Enter the Magic String : ");
    scanf(" %[^\n]", encInfo -> magic);

    if( encInfo -> image_capacity > ((strlen(encInfo -> magic) * 8) + 32 + 32 + 32 + (encInfo -> size_secret_file * 8)))
    {
        return e_success;
    }
    else
    {
        printf("Image Capacity failed\n");
        return e_failure;
    }
}

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;

    fseek(fptr_image, 18, SEEK_SET);

    fread(&width, 4, 1, fptr_image);
    //printf("Width %d\n", width);
    fread(&height, 4, 1, fptr_image);
    //printf("Height %d\n", height);

    return width * height * 3;
}

/* Get file size */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);

    return ftell(fptr);
}

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);

    char buffer[54];

    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);

    return e_success;
}

/* Store Magic String */
Status encode_magic_string(EncodeInfo *encInfo)
{
    if(encode_data_to_image(encInfo -> magic, strlen(encInfo -> magic), encInfo) == e_success)
    {
        printf("Magic String is encoded\n");
    }
    else
    {
        printf("Magic String is not encoded\n");
        return e_failure;
    }

    return e_success;
}

/* Encode secret file extension size */
Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    unsigned int file_extn_size = strlen(strchr(encInfo -> secret_fname, '.'));
    
    if (encode_size_to_lsb(file_extn_size, encInfo) == e_success)
    {
        printf("Secret File Extension Size is encoded\n");
    }
    else
    {
        printf("Secret File Extension Size is not encoded\n");
        return e_failure;
    }

    return e_success;
}

/* Encode secret file extenstion */
Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    char *file_extn = strchr(encInfo -> secret_fname, '.');

    if(encode_data_to_image(file_extn, strlen(file_extn), encInfo) == e_success)
    {
        printf("Secret File Extention is encoded\n");
    }
    else
    {
        printf("Secret File Extention is not encoded\n");
        return e_failure;
    }

    return e_success;
}

/* Encode secret file size */
Status encode_secret_file_size(EncodeInfo *encInfo)
{    
    if (encode_size_to_lsb(encInfo->size_secret_file, encInfo) == e_success)
    {
        printf("Secret File Size is encoded\n");
    }
    else
    {
        printf("Secret File Size is not encoded\n");
        return e_failure;
    }

    return e_success;
}

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo -> fptr_secret);
    
    char buffer[encInfo -> size_secret_file + 1];
    fread(buffer, encInfo -> size_secret_file, 1, encInfo -> fptr_secret);
    buffer[encInfo -> size_secret_file] = '\0';

    if(encode_data_to_image(buffer, encInfo -> size_secret_file, encInfo) == e_success)
    {
        printf("Secret File Data is encoded\n");
    }
    else
    {
        printf("Secret File Data is not encoded\n");
        return e_failure;
    }

    return e_success;
}

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo)
{
    for(int i = 0; i < size; i++)
    {
        fread(encInfo -> image_data, 8, 1, encInfo -> fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo -> image_data);
        fwrite(encInfo -> image_data, 8, 1, encInfo -> fptr_stego_image);
    }

    return e_success;
}

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> i) & 1 );
    }

    return e_success;
}

/* Encode a size into LSB of image data array */
Status encode_size_to_lsb(int size, EncodeInfo *encInfo)
{
    char buffer[32];

    fread(buffer, 32, 1, encInfo -> fptr_src_image);

    for(int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((size & (1 << i)) >> i);
    }

    fwrite(buffer , 32, 1, encInfo -> fptr_stego_image);

    return e_success;
}

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char ch;

    while(fread(&ch, 1, 1, fptr_src_image) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest_image);
    }

    return e_success;
}

/* Close File pointers for i/p and o/p files */
Status close_encode_files(EncodeInfo *encInfo)
{
    fclose(encInfo -> fptr_src_image);
    fclose(encInfo -> fptr_secret);
    fclose(encInfo -> fptr_stego_image);

    return e_success;
}
