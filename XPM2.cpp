#include "XPM2.hpp"
#include <fstream>
#include <map>
#include <iostream>
#include <iomanip>
namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        std::ifstream myfile(file);
        
        std::cout << "\nFilepath: " << file << "\n";
        
        std::string line;

        std::getline(myfile, line);

        int width, height, num_colors;
        myfile >> width >> height >> num_colors;

        std::cout << "\n Width: " << width << " height: " << height << " num_colors: " << num_colors << "\n";

        Image* image = new Image(width, height);

        int c;
        myfile >> c;

        std::getline(myfile, line);


        // Read the colors section
        std::map<char, Color> colorMap;
        for (int i = 0; i < num_colors; i++) {
            char symbol;
            char c2;
            std::string hexcode;
            myfile >> symbol >> c2 >> hexcode;
            
            std::cout << "\n symbol: " << symbol << " hexcode: " << hexcode << "\n";

            int red = std::stoi(hexcode.substr(1, 2), 0, 16);
            int green = std::stoi(hexcode.substr(3, 2), 0, 16);
            int blue = std::stoi(hexcode.substr(5, 2), 0, 16);

            std::cout << "\n Red: " << red << " Green: " << green << " Blue: " << blue << "\n";

            Color color(red, green, blue);

            std::cout << "\n Red2: " << (int)color.red() << "\n";

            colorMap[symbol] = color;
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                char pixelChar;
                myfile >> pixelChar;
                std::cout << "\n char: " << pixelChar << "\n";
                auto it = colorMap.find(pixelChar);
                if (it != colorMap.end()) {
                    std::cout << "\n color red: " << it->second.red() << "\n";
                    Color& c = image->at(x, y);
                    c = it->second;
                }
            }
            std::getline(myfile, line);
        }

        return image;
    }

    struct ColorComparator {
        bool operator()(const Color& color1, const Color& color2) const {
            if (color1.red() != color2.red())
                return color1.red() < color2.red();
            if (color1.green() != color2.green())
                return color1.green() < color2.green();
            return color1.blue() < color2.blue();
        }
    };

    void saveToXPM2(const std::string& file, const Image* image) {
        std::ofstream outfile(file);

        int width = image->width();
        int height = image->height();

        // Create a map to store the color-to-symbol mapping
        std::map<Color, char, ColorComparator> colorToSymbolMap;

        std::cout << "\n Width: " << width << "\n";
        std::cout << "\n Height: " << height << "\n";

        // Determine unique colors in the image and assign symbols
        char currentSymbol = '*';
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& color = image->at(x, y);
                auto it = colorToSymbolMap.find(color);
                if (it == colorToSymbolMap.end()) {
                    // New color found, assign the next symbol
                    colorToSymbolMap[color] = currentSymbol;
                    currentSymbol++;
                }
            }
        }

        // Determine the number of colors and symbols
        int numColors = colorToSymbolMap.size();
        int numSymbols = currentSymbol - '*';

        // Write XPM2 header
        outfile << "! XPM2" << std::endl;
        outfile << width << " " << height << " " << numColors << " " << numSymbols << std::endl;

        // Write XPM2 values section
        for (const auto& entry : colorToSymbolMap) {
            const Color& color = entry.first;
            char symbol = entry.second;
            outfile << symbol << " c #" << std::hex 
                << std::setw(2) << std::setfill('0') << static_cast<int>(color.red())
                << std::setw(2) << static_cast<int>(color.green()) 
                << std::setw(2) << static_cast<int>(color.blue())
                << std::dec << std::endl;
        }

        // Write XPM2 pixel values section
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& color = image->at(x, y);
                char symbol = colorToSymbolMap[color];
                outfile << symbol;
            }
            outfile << std::endl;
        }

        outfile.close();
    }
}
