#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "FileReader.cpp"

bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int multiply() {
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
int substract() {
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

int multiply_then_screen() {
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

int multiply_then_subtract() {
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

int overlay() {
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

int addGreen() {
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

int multiplyRed() {
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

int separateIntoRgb() {
    Image car;
    car.loadTGA("input/car.tga");
    Image redImage(car.width, car.height);
    Image greenImage(car.width, car.height);
    Image blueImage(car.width, car.height);
    for (int y = 0; y < car.height; y++) {
        for (int x = 0; x < car.width; x++) {
            Pixel originalPixel = car.getPixel(x, y);

            redImage.getPixel(x, y) = { originalPixel.red, originalPixel.red, originalPixel.red };
            greenImage.getPixel(x, y) = { originalPixel.green, originalPixel.green, originalPixel.green };
            blueImage.getPixel(x, y) = { originalPixel.blue, originalPixel.blue, originalPixel.blue };
        }
    }

    redImage.saveTGA("output/part8_r.tga");
    greenImage.saveTGA("output/part8_g.tga");
    blueImage.saveTGA("output/part8_b.tga");

    return 0;
}

int combine3Files() {
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

int rotate180() {
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

int main(int argc, char* argv[]) {
    if (argc == 1 || (argc == 2 && std::strcmp(argv[1], "--help") == 0)) {
        std::cout << "Usage: " << argv[0] << " <output_image> <input_image> <operation>...\n";
        std::cout << "Operations:\n"
            << "  multiply <image>\n"
            << "  subtract <image>\n"
            << "  overlay <image>\n"
            << "  screen <image>\n"
            << "  flip\n"
            << "  combine <green_image> <blue_image>\n"
            << "  onlygreen\n"
            << "  onlyblue\n"
            << "  addred <value>\n"
            << "  addgreen <value>\n"
            << "  addblue <value>\n"
            << "  scalered <factor>\n"
            << "  scalegreen <factor>\n"
            << "  scaleblue <factor>\n";
        return 0;
    }

    if (!endsWith(argv[1], ".tga")) {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }
    std::string outputPath = argv[1];
    Image result;


    if (!result.loadTGA(argv[2])) {
        std::cerr << "Failed to load " << argv[2] << std::endl;
        return 1;
    }

    int i = 3;
    while (i < argc) {
        std::string operation = argv[i];

        if (operation == "multiply") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            Image img;
            if (!img.loadTGA(argv[i + 1])) {
                std::cerr << "Invalid file name." << std::endl;
                return 1;
            }
            std::cout << "Multiplying " << argv[1] << " and " << argv[i + 1] << "..." << std::endl;
            result = ImageManipulator::multiply(result, img);
            i += 2;
        }
        else if (operation == "subtract") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            Image img;
            if (!img.loadTGA(argv[i + 1])) {
                std::cerr << "Invalid file name." << std::endl;
                return 1;
            }
            std::cout << "Multiplying " << argv[1] << " and " << argv[i + 1] << "..." << std::endl;
            result = ImageManipulator::subtract(result, img);
            i += 2;
        }
        else if (operation == "overlay") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            Image img;
            if (!img.loadTGA(argv[i + 1])) {
                std::cerr << "Invalid file name." << std::endl;
                return 1;
            }
            result = ImageManipulator::overlay(result, img);
            i += 2;
        }
        else if (operation == "screen") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            Image img;
            if (!img.loadTGA(argv[i + 1])) {
                std::cerr << "Invalid file name." << std::endl;
                return 1;
            }
            result = ImageManipulator::screen(result, img);
            i += 2;
        }
        else if (operation == "flip") {
            result = ImageManipulator::flip(result);
            std::cerr << "Flipping output of the previous step." << std::endl;
            i++;
        }
        else if (operation == "combine") {
            if (i + 2 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            Image greenImage, blueImage;
            if (!greenImage.loadTGA(argv[i + 1])) {
                std::cerr << "Invalid file name." << std::endl;
                return 1;
            }
            if (!blueImage.loadTGA(argv[i + 2])) {
                std::cerr << "Invalid file name." << std::endl;
                return 1;
            }

            result = ImageManipulator::combine(result, greenImage, blueImage);
            i += 3;
        }
        else if (operation == "onlygreen") {
            std::cout << "Isolating the green channel..." << std::endl;
            result = ImageManipulator::onlygreen(result);
            i++;
        }
        else if (operation == "onlyblue") {
            std::cout << "Isolating the blue channel..." << std::endl;
            result = ImageManipulator::onlyblue(result);
            i++;
        }
        else if (operation == "addred") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            int value;
            try {
                value = std::stoi(argv[i + 1]);
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number.\n";
                return 1;
            }
            std::cout << "Adding +" << value << " to the red channel..." << std::endl;
            result = ImageManipulator::addred(result, value);
            i += 2;
        }
        else if (operation == "addgreen") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            int value;
            try {
                value = std::stoi(argv[i + 1]);
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number.\n";
                return 1;
            }
            std::cout << "Adding +" << value << " to the green channel..." << std::endl;
            result = ImageManipulator::addgreen(result, value);
            i += 2;
        }
        else if (operation == "addblue") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            int value;
            try {
                value = std::stoi(argv[i + 1]);
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number.\n";
                return 1;
            }
            std::cout << "Adding +" << value << " to the blue channel..." << std::endl;
            result = ImageManipulator::addblue(result, value);
            i += 2;
        }
        else if (operation == "scalered") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            int scale;
            try {
                scale = std::stoi(argv[i + 1]);
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number.\n";
                return 1;
            }
            std::cout << "Scaling the red channel by " << scale << "..." << std::endl;
            result = ImageManipulator::scalered(result, scale);
            i += 2;
        }
        else if (operation == "scalegreen") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            int scale;
            try {
                scale = std::stoi(argv[i + 1]);
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number.\n";
                return 1;
            }
            std::cout << "Scaling the green channel by " << scale << "..." << std::endl;
            result = ImageManipulator::scalegreen(result, scale);
            i += 2;
        }
        else if (operation == "scaleblue") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument.";
                return 1;
            }
            int scale;
            try {
                scale = std::stoi(argv[i + 1]);
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number.\n";
                return 1;
            }
            std::cout << "Scaling the blue channel by " << scale << "..." << std::endl;
            result = ImageManipulator::scaleblue(result, scale);
            i += 2;
        }
        else {
            std::cerr << "Invalid method name." << std::endl;
            return 1;
        }
    }

    if (!result.saveTGA(outputPath.c_str())) {
        std::cerr << "Failed to save the result image to " << outputPath << std::endl;
        return 1;
    }

    std::cout << "... and saving output to " << outputPath << "!" << std::endl;
    return 0;
}