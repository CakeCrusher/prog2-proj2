#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "FileReader.cpp"

int part1() {
    Image layer1;
    Image pattern1;
    layer1.loadTGA("input/layer1.tga");
    pattern1.loadTGA("input/pattern1.tga");

    for (int y = 0; y < pattern1.height; y++) {
        for (int x = 0; x < pattern1.width; x++) {
            Pixel& patternPixel = pattern1.getPixel(x, y);
            const Pixel& layerPixel = layer1.getPixel(x, y);

            patternPixel.red = Image::multiply(patternPixel.red, layerPixel.red);
            patternPixel.green = Image::multiply(patternPixel.green, layerPixel.green);
            patternPixel.blue = Image::multiply(patternPixel.blue, layerPixel.blue);
        }
    }
    pattern1.saveTGA("output/part1.tga");
    return 0;
}
int part2() {
    Image layer2;
    Image car;
    layer2.loadTGA("input/layer2.tga");
    car.loadTGA("input/car.tga");
    for (int y = 0; y < layer2.height; y++) {
        for (int x = 0; x < layer2.width; x++) {
            Pixel& layerPixel = layer2.getPixel(x, y);
            const Pixel& carPixel = car.getPixel(x, y);

            layerPixel.red = Image::subtract(carPixel.red, layerPixel.red);
            layerPixel.green = Image::subtract(carPixel.green, layerPixel.green);
            layerPixel.blue = Image::subtract(carPixel.blue, layerPixel.blue);
        }
    }

    layer2.saveTGA("output/part2.tga");
    return 0;
}

int part3() {
    Image layer1;
    layer1.loadTGA("input/layer1.tga");
    Image pattern2;
    pattern2.loadTGA("input/pattern2.tga");
    Image text;
    text.loadTGA("input/text.tga");

    Image result(layer1.width, layer1.height);
    for (int y = 0; y < layer1.height; y++) {
        for (int x = 0; x < layer1.width; x++) {
            Pixel& resultPixel = result.getPixel(x, y);
            const Pixel& layerPixel = layer1.getPixel(x, y);
            const Pixel& patternPixel = pattern2.getPixel(x, y);

            resultPixel.red = Image::multiply(layerPixel.red, patternPixel.red);
            resultPixel.green = Image::multiply(layerPixel.green, patternPixel.green);
            resultPixel.blue = Image::multiply(layerPixel.blue, patternPixel.blue);
        }
    }
    for (int y = 0; y < result.height; y++) {
        for (int x = 0; x < result.width; x++) {
            Pixel& resultPixel = result.getPixel(x, y);
            const Pixel& textPixel = text.getPixel(x, y);
            resultPixel.red = Image::screen(resultPixel.red, textPixel.red);
            resultPixel.green = Image::screen(resultPixel.green, textPixel.green);
            resultPixel.blue = Image::screen(resultPixel.blue, textPixel.blue);
        }
    }
    result.saveTGA("output/part3.tga");

    return 0;
}

int part4() {
    Image layer2;
    layer2.loadTGA("input/layer2.tga");
    Image circles;
    circles.loadTGA("input/circles.tga");

    Image multiplyResult(layer2.width, layer2.height);
    for (int y = 0; y < layer2.height; y++) {
        for (int x = 0; x < layer2.width; x++) {
            Pixel& resultPixel = multiplyResult.getPixel(x, y);
            const Pixel& layerPixel = layer2.getPixel(x, y);
            const Pixel& circlePixel = circles.getPixel(x, y);

            resultPixel.red = Image::multiply(layerPixel.red, circlePixel.red);
            resultPixel.green = Image::multiply(layerPixel.green, circlePixel.green);
            resultPixel.blue = Image::multiply(layerPixel.blue, circlePixel.blue);
        }
    }

    Image pattern2;
    pattern2.loadTGA("input/pattern2.tga");

    for (int y = 0; y < multiplyResult.height; y++) {
        for (int x = 0; x < multiplyResult.width; x++) {
            Pixel& resultPixel = multiplyResult.getPixel(x, y);
            const Pixel& patternPixel = pattern2.getPixel(x, y);

            resultPixel.red = Image::subtract(resultPixel.red, patternPixel.red);
            resultPixel.green = Image::subtract(resultPixel.green, patternPixel.green);
            resultPixel.blue = Image::subtract(resultPixel.blue, patternPixel.blue);
        }
    }

    multiplyResult.saveTGA("output/part4.tga");

    return 0;
}

int part5() {
    Image layer1;
    layer1.loadTGA("input/layer1.tga");

    Image pattern1;
    pattern1.loadTGA("input/pattern1.tga");

    for (int y = 0; y < layer1.height; y++) {
        for (int x = 0; x < layer1.width; x++) {
            Pixel& layerPixel = layer1.getPixel(x, y);
            const Pixel& patternPixel = pattern1.getPixel(x, y);

            layerPixel.red = Image::overlay(layerPixel.red, patternPixel.red);
            layerPixel.green = Image::overlay(layerPixel.green, patternPixel.green);
            layerPixel.blue = Image::overlay(layerPixel.blue, patternPixel.blue);
        }
    }

    layer1.saveTGA("output/part5.tga");

    return 0;
}

int part6() {
    Image car;
    car.loadTGA("input/car.tga");

    for (int y = 0; y < car.height; y++) {
        for (int x = 0; x < car.width; x++) {
            Pixel& pixel = car.getPixel(x, y);
            int newGreen = pixel.green + 200;
            pixel.green = newGreen > 255 ? 255 : static_cast<unsigned char>(newGreen);
        }
    }
    car.saveTGA("output/part6.tga");
    return 0;
}

int part7() {
    Image car;
    car.loadTGA("input/car.tga");

    for (int y = 0; y < car.height; y++) {
        for (int x = 0; x < car.width; x++) {
            Pixel& pixel = car.getPixel(x, y);

            int newRed = pixel.red * 4;
            pixel.red = newRed > 255 ? 255 : static_cast<unsigned char>(newRed);
            pixel.blue = 0;
        }
    }

    car.saveTGA("output/part7.tga");

    return 0;
}

int part8() {
    Image car;
    car.loadTGA("input/car.tga");

    Image redImage(car.width, car.height);
    Image greenImage(car.width, car.height);
    Image blueImage(car.width, car.height);
    for (int y = 0; y < car.height; y++) {
        for (int x = 0; x < car.width; x++) {
            Pixel originalPixel = car.getPixel(x, y);
            redImage.getPixel(x, y) = { 0, 0, originalPixel.red };
            greenImage.getPixel(x, y) = { 0, originalPixel.green, 0 };
            blueImage.getPixel(x, y) = { originalPixel.blue, 0, 0 };
        }
    }
    redImage.saveTGA("output/part8_r.tga");
    greenImage.saveTGA("output/part8_g.tga");
    blueImage.saveTGA("output/part8_b.tga");

    return 0;
}

int part9() {
    Image redLayer;
    redLayer.loadTGA("input/layer_red.tga");
    Image greenLayer;
    greenLayer.loadTGA("input/layer_green.tga");

    Image blueLayer;
    blueLayer.loadTGA("input/layer_blue.tga");

    Image combinedImage(redLayer.width, redLayer.height);

    for (int y = 0; y < combinedImage.height; y++) {
        for (int x = 0; x < combinedImage.width; x++) {
            Pixel& combinedPixel = combinedImage.getPixel(x, y);
            combinedPixel.red = redLayer.getPixel(x, y).red;
            combinedPixel.green = greenLayer.getPixel(x, y).green;
            combinedPixel.blue = blueLayer.getPixel(x, y).blue;
        }
    }

    combinedImage.saveTGA("output/part9.tga");

    return 0;
}

int part10() {
    Image text;
    text.loadTGA("input/text2.tga");
    for (int y = 0; y < text.height / 2; y++) {
        for (int x = 0; x < text.width; x++) {
            std::swap(text.getPixel(x, y), text.getPixel(text.width - 1 - x, text.height - 1 - y));
        }
    }

    text.saveTGA("output/part10.tga");

    return 0;
}

int main() {
    part1();
    part2();
    part3();
    part4();
    part5();
    part6();
    part7();
    part8();
    part9();
    part10();
    return 0;
}
