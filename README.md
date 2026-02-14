🖼️ Image Steganography using LSB -(C)
📌 Overview

This project implements Image Steganography using the Least Significant Bit (LSB) technique in C.

It allows users to securely hide secret text data inside a BMP image and later extract it without affecting the visible image quality.

The application works via command-line arguments and supports both Encoding (-e) and Decoding (-d) operations.

 Features

Hide secret text inside BMP image

Extract hidden text from stego image

Magic string verification

File size embedding & extraction

Custom output file support

Default file creation option

Error handling for invalid inputs

🛠️ Technologies Used

C Programming

Bitwise Operations

File Handling

BMP Image Processing

Command Line Interface

📂 Project Structure
C-Project-1/
│
├── 1-References        → Study materials
├── 2-OutputImages     → Stego output images
├── 3-Design           → Flowcharts & diagrams
├── 4-SkeletonCode     → Source code
└── README.md

⚙️ Working Principle
🔐 Encoding Process

Read input BMP image

Read secret text file

Embed magic string

Store secret file size

Hide data in LSB bits

Generate stego image

🔓 Decoding Process

Read stego image

Verify magic string

Extract file size

Decode LSB bits

Reconstruct secret file

💻 Compilation
gcc *.c -o lsb_steg

▶️ Execution
Encode
./lsb_steg -e input.bmp secret.txt output.bmp

Decode
./lsb_steg -d output.bmp decoded.txt

📊 Applications

Secure communication

Data hiding

Digital watermarking

Cybersecurity learning



⭐ Future Enhancements

Support PNG/JPEG images

Password protection

GUI interface

Audio/Video steganography
