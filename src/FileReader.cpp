#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

class Image {
public:
    int width, height;
    std::vector<Pixel> pixels;

    Image(int width, int height) {
        this->width = width;
        this->height = height;
        this->pixels.resize(width * height);
    }
    Image() {
        width = 0;
        height = 0;
    }
    ~Image() {}

    bool loadTGA(const std::string& filename) {
        std::ifstream binaryIn(filename, std::ios::binary);
        if (!binaryIn) {
            std::cerr << "Could not open file for reading: " << filename << std::endl;
            return false;
        }

        unsigned char header[18];
        binaryIn.read((char*)header, 18);

        width = header[12] | header[13] << 8;
        height = header[14] | header[15] << 8;
        unsigned char bpp = header[16];

        if (bpp != 24 && bpp != 32) {
            std::cerr << "Unsupported BPP: " << static_cast<int>(bpp) << std::endl;
            return false;
        }

        pixels.resize(width * height);

        for (int i = 0; i < width * height; ++i) {
            binaryIn.read((char*)&pixels[i].blue, 1);
            binaryIn.read((char*)&pixels[i].green, 1);
            binaryIn.read((char*)&pixels[i].red, 1);
        }

        binaryIn.close();
        return true;
    }
    bool saveTGA(const std::string& filename) {
        std::ofstream binaryOut(filename, std::ios::binary);
        if (!binaryOut) {
            std::cerr << "Could not open file for writing: " << filename << std::endl;
            return false;
        }

        unsigned char header[18] = {};
        header[2] = 2;
        header[12] = width & 0xFF;
        header[13] = (width >> 8) & 0xFF;
        header[14] = height & 0xFF;
        header[15] = (height >> 8) & 0xFF;
        header[16] = 24;  // bist per pixl

        binaryOut.write((char*)header, 18);

        for (const Pixel& pixel : pixels) {
            binaryOut.write((char*)&pixel.blue, 1);
            binaryOut.write((char*)&pixel.green, 1);
            binaryOut.write((char*)&pixel.red, 1);
        }

        binaryOut.close();
        return true;
    }

    Pixel& getPixel(int x, int y) {
        return pixels[y * width + x];
    }

    const Pixel& getPixel(int x, int y) const {
        return pixels[y * width + x];
    }

    void setPixel(int x, int y, const Pixel& pixel) {
        pixels[y * width + x] = pixel;
    }
    // use static for consistent class
    static unsigned char multiply(unsigned char p1, unsigned char p2) {
        float np1 = p1 / 255.0f;
        float np2 = p2 / 255.0f;
        return static_cast<unsigned char>(std::round((np1 * np2) * 255));
    }

    static unsigned char screen(unsigned char p1, unsigned char p2) {
        float np1 = p1 / 255.0f;
        float np2 = p2 / 255.0f;
        float result = 1 - (1 - np1) * (1 - np2);
        return static_cast<unsigned char>(std::round(result * 255));
    }

    static unsigned char subtract(unsigned char p1, unsigned char p2) {
        int result = static_cast<int>(p1) - p2;
        return result < 0 ? 0 : static_cast<unsigned char>(result);
    }


    static unsigned char addition(unsigned char p1, unsigned char p2) {
        int result = p1 + p2;
        return result > 255 ? 255 : static_cast<unsigned char>(result);
    }

    static unsigned char overlay(unsigned char p1, unsigned char p2) {
        float np1 = p1 / 255.0f;
        float np2 = p2 / 255.0f;
        float result;

        if (np2 <= 0.5f) {
            result = 2 * np1 * np2;
        }
        else {
            result = 1 - 2 * (1 - np1) * (1 - np2);
        }

        return static_cast<unsigned char>(std::round(result * 255));
    }
};