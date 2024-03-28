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


class ImageManipulator {
public:
    static Image multiply(Image& img1, Image& img2) {
        for (int y = 0; y < img1.height; y++) {
            for (int x = 0; x < img1.width; x++) {
                Pixel& pixel1 = img1.getPixel(x, y);
                const Pixel& pixel2 = img2.getPixel(x, y);
                pixel1.red = Image::multiply(pixel1.red, pixel2.red);
                pixel1.green = Image::multiply(pixel1.green, pixel2.green);
                pixel1.blue = Image::multiply(pixel1.blue, pixel2.blue);
            }
        }

        return img1;
    }

    static Image subtract(Image& img1, Image& img2) {
        for (int y = 0; y < img1.height; y++) {
            for (int x = 0; x < img1.width; x++) {
                Pixel& pixel1 = img1.getPixel(x, y);
                const Pixel& pixel2 = img2.getPixel(x, y);

                pixel1.red = Image::subtract(pixel1.red, pixel2.red);
                pixel1.green = Image::subtract(pixel1.green, pixel2.green);
                pixel1.blue = Image::subtract(pixel1.blue, pixel2.blue);
            }
        }

        return img1;
    }
    static Image overlay(Image& topImage, Image& bottomImage) {

        for (int y = 0; y < topImage.height; y++) {
            for (int x = 0; x < topImage.width; x++) {
                Pixel& topPixel = topImage.getPixel(x, y);
                const Pixel& bottomPixel = bottomImage.getPixel(x, y);

                // call overlay for all colors
                topPixel.red = Image::overlay(topPixel.red, bottomPixel.red);
                topPixel.green = Image::overlay(topPixel.green, bottomPixel.green);
                topPixel.blue = Image::overlay(topPixel.blue, bottomPixel.blue);
            }
        }

        return topImage;
    }

    static Image screen(Image& bottomImage, Image& topImage) {
        for (int y = 0; y < bottomImage.height; y++) {
            for (int x = 0; x < bottomImage.width; x++) {
                Pixel& bottomPixel = bottomImage.getPixel(x, y);
                const Pixel& topPixel = topImage.getPixel(x, y);
                // call screen for all colors
                bottomPixel.red = Image::screen(bottomPixel.red, topPixel.red);
                bottomPixel.green = Image::screen(bottomPixel.green, topPixel.green);
                bottomPixel.blue = Image::screen(bottomPixel.blue, topPixel.blue);
            }
        }

        return bottomImage;
    }
    static Image combine(Image& redImage, Image& greenImage, Image& blueImage) {
        Image combinedImage(redImage.width, redImage.height);

        for (int y = 0; y < combinedImage.height; y++) {
            for (int x = 0; x < combinedImage.width; x++) {
                Pixel& combinedPixel = combinedImage.getPixel(x, y);
                combinedPixel.red = redImage.getPixel(x, y).red;
                combinedPixel.green = greenImage.getPixel(x, y).green;
                combinedPixel.blue = blueImage.getPixel(x, y).blue;
            }
        }

        return combinedImage;
    }

    static Image flip(Image& image) {
        Image flippedImage(image.width, image.height);

        for (int y = 0; y < image.height; y++) {
            for (int x = 0; x < image.width; x++) {
                Pixel& originalPixel = image.getPixel(x, y);
                Pixel& flippedPixel = flippedImage.getPixel(image.width - 1 - x, image.height - 1 - y);
                flippedPixel = originalPixel;
            }
        }

        return flippedImage;
    }

    static Image onlygreen(Image& img) {
        Image greenOnlyImage = img;

        for (int y = 0; y < greenOnlyImage.height; y++) {
            for (int x = 0; x < greenOnlyImage.width; x++) {
                Pixel& pixel = greenOnlyImage.getPixel(x, y);
                pixel.red = 0;
                pixel.blue = 0;
            }
        }

        return greenOnlyImage;
    }

    static Image onlyblue(Image& img) {
        Image blueOnlyImage = img;// copy

        for (int y = 0; y < blueOnlyImage.height; y++) {
            for (int x = 0; x < blueOnlyImage.width; x++) {
                Pixel& pixel = blueOnlyImage.getPixel(x, y);
                pixel.red = 0;
                pixel.green = 0;
            }
        }

        return blueOnlyImage;
    }

    static Image addred(Image& img, int value) {
        Image resultImage = img;

        for (int y = 0; y < resultImage.height; y++) {
            for (int x = 0; x < resultImage.width; x++) {
                Pixel& pixel = resultImage.getPixel(x, y);
                pixel.red = std::min(255, std::max(0, pixel.red + value));
            }
        }

        return resultImage;
    }

    static Image addgreen(Image& img, int value) {
        Image resultImage = img;

        for (int y = 0; y < resultImage.height; y++) {
            for (int x = 0; x < resultImage.width; x++) {
                Pixel& pixel = resultImage.getPixel(x, y);
                pixel.green = std::min(255, std::max(0, pixel.green + value));
            }
        }

        return resultImage;
    }

    static Image addblue(Image& img, int value) {
        Image resultImage = img; //copy

        for (int y = 0; y < resultImage.height; y++) {
            for (int x = 0; x < resultImage.width; x++) {
                Pixel& pixel = resultImage.getPixel(x, y);
                pixel.blue = std::min(255, std::max(0, pixel.blue + value));
            }
        }

        return resultImage;
    }
    static Image scalered(Image& img, float scale) {
        Image resultImage = img; 

        for (int y = 0; y < resultImage.height; y++) {
            for (int x = 0; x < resultImage.width; x++) {
                Pixel& pixel = resultImage.getPixel(x, y);
                pixel.red = std::min(255, std::max(0, static_cast<int>(pixel.red * scale)));
            }
        }

        return resultImage;
    }

    static Image scalegreen(Image& img, float scale) {
        Image resultImage = img;

        for (int y = 0; y < resultImage.height; y++) {
            for (int x = 0; x < resultImage.width; x++) {
                Pixel& pixel = resultImage.getPixel(x, y);
                pixel.green = std::min(255, std::max(0, static_cast<int>(pixel.green * scale)));
            }
        }

        return resultImage;
    }

    static Image scaleblue(Image& img, float scale) {
        Image resultImage = img;

        for (int y = 0; y < resultImage.height; y++) {
            for (int x = 0; x < resultImage.width; x++) {
                Pixel& pixel = resultImage.getPixel(x, y);
                pixel.blue = std::min(255, std::max(0, static_cast<int>(pixel.blue * scale)));
            }
        }

        return resultImage;
    }
};