🖼️ Image Steganography using LSB in C
📌 Description

This project implements Image Steganography using the Least Significant Bit (LSB) technique in C programming. It enables users to securely hide secret text data inside BMP image files and retrieve it later without affecting the visible quality of the image. The system supports both encoding and decoding operations through command-line arguments with proper validation.

🎯 Objective

To design and develop a secure data hiding system that embeds and extracts confidential information within an image using bitwise operations and file handling concepts.

🛠️ Technologies Used

C Programming

Bitwise Operations

File Handling

BMP Image Processing

Command Line Arguments

⚙️ Functionality
🔐 Encoding

Accepts BMP image and secret text file

Verifies image format

Embeds magic string

Stores secret file size

Hides data in LSB bits

Generates stego image

🔓 Decoding

Reads stego image

Verifies magic string

Extracts file size

Decodes hidden bits

Reconstructs secret file

📂 Project Modules

Encoding Module

Decoding Module

File Validation

Magic String Verification

Data Extraction

💻 Compilation
gcc *.c -o lsb_steg

▶️ Execution
Encode
./lsb_steg -e input.bmp secret.txt output.bmp

Decode
./lsb_steg -d output.bmp decoded.txt

📊 Applications

Secure Communication

Data Hiding

Digital Watermarking

Cybersecurity Projects

