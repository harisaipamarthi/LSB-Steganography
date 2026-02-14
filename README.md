📦 PROJECT TITLE
Image Steganography using LSB in C


 DESCRIPTION
This project implements Image Steganography using the Least Significant Bit (LSB) technique in C programming. It allows users to hide secret text data inside BMP images and extract it later without affecting the visible quality of the image. The system supports both encoding and decoding through command-line arguments with secure data handling.


 OBJECTIVE
To securely embed and retrieve confidential information within an image using bitwise operations and file handling concepts.

 TECHNOLOGIES USED
• C Programming
• Bitwise Operations
• File Handling
• BMP Image Processing
• Command Line Arguments


 FEATURES
• Encode secret text into image
• Decode hidden data from image
• Magic string verification
• File size embedding & extraction
• Secure data reconstruction
• Error handling support


 PROJECT MODULES
• Encoding Module
• Decoding Module
• File Validation
• Magic String Verification
• Data Extraction


 COMPILATION
gcc *.c -o lsb_steg


 EXECUTION – ENCODE
./lsb_steg -e input.bmp secret.txt output.bmp


 EXECUTION – DECODE
./lsb_steg -d output.bmp decoded.txt


 APPLICATIONS
• Secure Communication
• Data Hiding
• Digital Watermarking
• Cybersecurity Learning


 DEVELOPED BY
Harisai Pamarthi
Embedded Systems Trainee
