#include "simage_loader.h"
#include "core/sassert.h"
#include "core/smemory.h"
#include "utils/utils.h"

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

PACK(struct BitmapFileHeader {
         char headerField[2];
         u32 size;
         i16 reserved[2];
         u32 offset;
     });

PACK(struct BitmapInfoHeader {
         u32 size;
         i32 width;
         i32 height;
         u16 planes;
         u16 bitCount;
         u32 compression;
         u32 sizeImage;
         i32 xPelsPerMeter;
         i32 yPelsPerMeter;
         u32 clrUsed;
         u32 clrImportant;
         u32 redMask;
         u32 greenMask;
         u32 blueMask;
     });

static u8* SImageLoadBmp(const char* filePath, i32* width, i32* height, i32* nrChannels);

// TODO(Tony): Replace with compiler intrinsic
static bool8 BitScanForward(u32* index, u32 value)
{
    u32 i = 0;
    for (; i < 32; i++) {
        if (value & (1 << i)) {
            *index = i;
            break;
        }
    }

    return i < 33;
}

u8* SImageLoad(const char* filePath, i32* width, i32* height, i32* nrChannels)
{
    return SImageLoadBmp(filePath, width, height, nrChannels);
}

void SImageUnload(void* data)
{
    SFree(data);
}

static u8* SImageLoadBmp(const char* filePath, i32* width, i32* height, i32* nrChannels)
{
    u8* file = FileLoadBinary(filePath);
    if (!file) {
        return nullptr;
    }

    BitmapFileHeader fileHeader = *((BitmapFileHeader*) file);
    SASSERT(fileHeader.headerField[0] == 'B' && fileHeader.headerField[1] == 'M');

    BitmapInfoHeader infoHeader = *((BitmapInfoHeader*) (file + sizeof(BitmapFileHeader)));
    SASSERT(infoHeader.bitCount == 32);
    SASSERT(infoHeader.compression == 3);

    *width = infoHeader.width;
    *height = infoHeader.height;
    *nrChannels = infoHeader.bitCount / 8;

    u32 imageDataSize = infoHeader.width * infoHeader.height;
    u32 alphaMask = ~(infoHeader.redMask | infoHeader.blueMask | infoHeader.greenMask);
    u32 redShift = 0;
    u32 greenShift = 0;
    u32 blueShift = 0;
    u32 alphaShift = 0;

    bool8 rFound = BitScanForward(&redShift, infoHeader.redMask);
    bool8 gFound = BitScanForward(&greenShift, infoHeader.greenMask);
    bool8 bFound = BitScanForward(&blueShift, infoHeader.blueMask);
    bool8 aFound = BitScanForward(&alphaShift, alphaMask);
    SASSERT(rFound && gFound && bFound && aFound);

    u32* tmpImageData = (u32*) (file + fileHeader.offset);
    for (u32 i = 0; i < imageDataSize; i++) {
        u32 p = tmpImageData[i];
        tmpImageData[i] = (((p >> redShift) & 0xFF) << 0) |
                          (((p >> greenShift) & 0xFF) << 8) |
                          (((p >> blueShift) & 0xFF) << 16) |
                          (((p >> alphaShift) & 0xFF) << 24);
    }

    u32* imageData = (u32*) SMalloc(imageDataSize * sizeof(*imageData), MEMORY_TAG_TEXTURE);
    for (i32 r = 0; r < infoHeader.height; r++) {
        u32* src = &tmpImageData[r * infoHeader.width];
        u32* dst = &imageData[(infoHeader.height - r - 1) * infoHeader.width];
        SMemCopy(dst, src, 4 * infoHeader.width);
    }
    FileUnload(file);

    return (u8*) imageData;
}