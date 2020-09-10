#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

typedef struct
{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t  biPlanes;
    uint16_t  biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} DIB;

typedef struct
{
    uint16_t type;
    uint32_t bfSize;
    uint32_t reserved;
    uint32_t offset;
} HEADER;

typedef struct
{
    HEADER header;
    DIB dib;
} BMP;

void clipboard_to_file(std::string const& file_name)
{
    if (IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB) || IsClipboardFormatAvailable(CF_DIBV5))
    {
        if (OpenClipboard(NULL))
        {
            HANDLE hClipboard = GetClipboardData(CF_DIB);

            if (!hClipboard)
            {
                hClipboard = GetClipboardData(CF_DIBV5);
            }

            if (hClipboard != NULL && hClipboard != INVALID_HANDLE_VALUE)
            {
                void* dib = GlobalLock(hClipboard);

                if (dib)
                {
                    DIB* info = reinterpret_cast<DIB*>(dib);
                    BMP bmp = { 0 };
                    bmp.header.type = 0x4D42;
                    bmp.header.offset = 54;
                    bmp.header.bfSize = info->biSizeImage + bmp.header.offset;
                    bmp.dib = *info;

                    std::cout << "Type: " << std::hex << bmp.header.type << std::dec << "\n";
                    std::cout << "bfSize: " << bmp.header.bfSize << "\n";
                    std::cout << "Reserved: " << bmp.header.reserved << "\n";
                    std::cout << "Offset: " << bmp.header.offset << "\n";
                    std::cout << "biSize: " << bmp.dib.biSize << "\n";
                    std::cout << "Width: " << bmp.dib.biWidth << "\n";
                    std::cout << "Height: " << bmp.dib.biHeight << "\n";
                    std::cout << "Planes: " << bmp.dib.biPlanes << "\n";
                    std::cout << "Bits: " << bmp.dib.biBitCount << "\n";
                    std::cout << "Compression: " << bmp.dib.biCompression << "\n";
                    std::cout << "Size: " << bmp.dib.biSizeImage << "\n";
                    std::cout << "X-res: " << bmp.dib.biXPelsPerMeter << "\n";
                    std::cout << "Y-res: " << bmp.dib.biYPelsPerMeter << "\n";
                    std::cout << "ClrUsed: " << bmp.dib.biClrUsed << "\n";
                    std::cout << "ClrImportant: " << bmp.dib.biClrImportant << "\n";

                    std::ofstream file(file_name, std::ios::out | std::ios::binary);
                    if (file)
                    {
                        file.write(reinterpret_cast<char*>(&bmp.header.type), sizeof(bmp.header.type));
                        file.write(reinterpret_cast<char*>(&bmp.header.bfSize), sizeof(bmp.header.bfSize));
                        file.write(reinterpret_cast<char*>(&bmp.header.reserved), sizeof(bmp.header.reserved));
                        file.write(reinterpret_cast<char*>(&bmp.header.offset), sizeof(bmp.header.offset));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biSize), sizeof(bmp.dib.biSize));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biWidth), sizeof(bmp.dib.biWidth));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biHeight), sizeof(bmp.dib.biHeight));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biPlanes), sizeof(bmp.dib.biPlanes));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biBitCount), sizeof(bmp.dib.biBitCount));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biCompression), sizeof(bmp.dib.biCompression));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biSizeImage), sizeof(bmp.dib.biSizeImage));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biXPelsPerMeter), sizeof(bmp.dib.biXPelsPerMeter));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biYPelsPerMeter), sizeof(bmp.dib.biYPelsPerMeter));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biClrUsed), sizeof(bmp.dib.biClrUsed));
                        file.write(reinterpret_cast<char*>(&bmp.dib.biClrImportant), sizeof(bmp.dib.biClrImportant));
                        file.write(reinterpret_cast<char*>(info + 1), bmp.dib.biSizeImage);
                    }

                    GlobalUnlock(dib);
                }
            }

            CloseClipboard();
        }
    }
}