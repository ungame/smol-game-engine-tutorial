#include <smol/smol.h>
#include <smol/smol_assetmanager.h>
#include <smol/smol_platform.h>
#include <smol/smol_log.h>

namespace smol
{

    const int BITMAP_SIGNATURE = 0x4D42;
    const int BITMAP_COMPRESSION_BI_BITFIELDS = 3;

#pragma pack(push, 1)
    
    /*
     * Refs:
     *      https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
     * 
     *      https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
     */

    struct BitmapHeader {
        unsigned short type;
        unsigned int   bitmapSize;
        unsigned short reserved1;
        unsigned short reserved2;
        unsigned int   offBits;
        unsigned int   size;
        unsigned int   width;
        unsigned int   height;
        unsigned short planes;
        unsigned short bitCount;
        unsigned int   compression;
        unsigned int   sizeImage;
        unsigned int   xPelsPerMeter;
        unsigned int   yPelsPerMeter;
        unsigned int   clrUsed;
        unsigned int   clrImportant;
    };

#pragma pack(pop)

    Image* createProceduralImage()
    {
        // Create a procedural checker texture
        const int texWidth = 800;
        const int texHeight = texWidth;
        const int squareCount = 32;
        const int squareSize = texWidth / squareCount;
        const int sizeInBytes = texWidth * texHeight * 3;

        char* buffer = new char[sizeInBytes + sizeof(Image)];

        unsigned char* texData = (unsigned char*) buffer + sizeof(Image);
        unsigned char* pixel = texData;

        for(int i = 0; i < texWidth; i++) 
        {
            for(int j = 0; j < texHeight; j++)
            {
                int x = i / squareSize;
                int y = j / squareSize;
                int squareNumber = x * squareCount + y;

                unsigned char color;
                bool isOdd = (squareNumber & 1);
                if(x & 1)
                {
                    color = (isOdd) ? 0xAA : 0x55; 
                }
                else
                {
                    color = (isOdd) ? 0x55 : 0xAA;
                }

                *pixel++ = color;
                *pixel++ = color;
                *pixel++ = color;
            }
        }


        Image* image = (Image*) buffer;
        image->width = texWidth;
        image->height = texHeight;
        image->bitsPerPixel = 24;
        image->data = (char*) texData;

        return image;
    }

    Image* AssetManager::loadImageBitmap(const char* fileName)
    {
        const char* enginePath = Platform::getBinaryPath();
        size_t enginePathLen = strlen(enginePath);

        size_t fileNameLen = strlen(fileName);
        char* fullFileName = new char[enginePathLen + fileNameLen + 1];
        size_t totalStringLen = fileNameLen + enginePathLen;
        strncpy(fullFileName, enginePath, enginePathLen);
        strncpy(fullFileName + enginePathLen, fileName, fileNameLen);
        fullFileName[totalStringLen] = 0;

        debugLogInfo("Loading image %s", fullFileName);

        const size_t imageHeaderSize = sizeof(Image);

        char* buffer = Platform::loadFileToBuffer(fullFileName, nullptr, imageHeaderSize, imageHeaderSize);
        
        delete fullFileName;

        if (buffer == nullptr)
        {
            return createProceduralImage();
        }
        
        BitmapHeader* bitmap = (BitmapHeader*) (buffer + imageHeaderSize);
        Image* image = (Image*) buffer;

        if ( bitmap->type != BITMAP_SIGNATURE )
        {
           smol::Log::error("Arquivo de bitmap invalido");
           Platform::unloadFileBuffer(buffer);
           return nullptr; 
        }

        if ( bitmap->compression != BITMAP_COMPRESSION_BI_BITFIELDS )
        {
            smol::Log::error("Compress??o de bitmap n??o suportada");
            Platform::unloadFileBuffer(buffer);
            return nullptr; 
        }

        image->width = bitmap->width;
        image->height = bitmap->height;
        image->bitsPerPixel = bitmap->bitCount;
        image->data = sizeof(BitmapHeader) + (char*) bitmap;

        if ( bitmap->bitCount == 24 || bitmap->bitCount == 32 )
        {
            // get color masks
            unsigned int* maskPtr = (unsigned int*) (sizeof(BitmapHeader) + (char*) bitmap);
            unsigned int rMask = *maskPtr++;
            unsigned int gMask = *maskPtr++;
            unsigned int bMask = *maskPtr++; 
            unsigned int aMask = ~(rMask | gMask | bMask); 

            unsigned int rMaskShift = (rMask == 0xFF000000) ? 24 : (rMask == 0xFF0000) ? 16 : (rMask == 0xFF00) ? 8 : 0;
            unsigned int gMaskShift = (gMask == 0xFF000000) ? 24 : (gMask == 0xFF0000) ? 16 : (gMask == 0xFF00) ? 8 : 0;
            unsigned int bMaskShift = (bMask == 0xFF000000) ? 24 : (bMask == 0xFF0000) ? 16 : (bMask == 0xFF00) ? 8 : 0;
            unsigned int aMaskShift = (aMask == 0xFF000000) ? 24 : (aMask == 0xFF0000) ? 16 : (aMask == 0xFF00) ? 8 : 0;

            const int numPixels = image->width * image->height;
            const int bytesPerPixel = image->bitsPerPixel / 8;
            
            for(int i = 0; i < numPixels; ++i)
            {
                unsigned int* pixelPtr = (unsigned int*) (i * bytesPerPixel + image->data);
                unsigned int pixel = *pixelPtr;

                unsigned int r = (pixel & rMask) >> rMaskShift;
                unsigned int g = (pixel & gMask) >> gMaskShift;
                unsigned int b = (pixel & bMask) >> bMaskShift;
                unsigned int a = (pixel & aMask) >> aMaskShift;

                unsigned color = a << 24 | b << 16 | g << 8 | r;
                *pixelPtr = color;
            }

        }
        else if ( bitmap->bitCount != 16 )
        {
            smol::Log::error("Bitmap bit count n??o suportado");
            Platform::unloadFileBuffer(buffer);
            return nullptr;     
        }

        return (Image*) buffer;
    }

    void AssetManager::unloadImageBitmap(Image* image)
    {
        
    }
}