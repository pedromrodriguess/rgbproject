#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if(command == "invert"){
                invert();
                continue;
            }
            if(command == "to_gray_scale"){
                to_gray_scale();
                continue;
            }
            if(command == "replace"){
                replace();
                continue;
            }
            if(command == "fill"){
                fill();
                continue;
            }
            if(command == "h_mirror"){
                h_mirror();
                continue;
            }
            if(command == "v_mirror"){
                v_mirror();
                continue;
            }
            if(command == "add"){
                add();
                continue;
            }
            if(command == "crop"){
                crop();
                continue;
            }
            if(command == "rotate_left"){
                rotate_left();
                continue;
            }
            if(command == "rotate_right"){
                rotate_right();
                continue;
            }
            if(command == "xpm2_open"){
                xpm2_open();
                continue;
            }
            if(command == "xpm2_save"){
                xpm2_save();
                continue;
            }
        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::invert() {
        for (int x = 0; x < image->width(); x++) {
            for (int y = 0; y < image->height(); y++) {
                Color& c = image->at(x, y);
                c.red() = 255 - c.red();
                c.green() = 255 - c.green();
                c.blue() = 255 - c.blue();
            }
        }
    }

    void Script::to_gray_scale(){
        for (int x = 0; x < image->width(); x++) {
            for (int y = 0; y < image->height(); y++) {
                Color& c = image->at(x, y);
                int gray = (c.red() + c.green() + c.blue()) / 3;
                c.red() = gray;
                c.green() = gray;
                c.blue() = gray;
            }
        }  
    }

    void Script::replace() {
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        Color c1(r1, g1, b1);
        Color c2(r2, g2, b2);

        for (int x = 0; x < image->width(); x++) {
            for (int y = 0; y < image->height(); y++) {
                Color& c = image->at(x, y);
                if (c.red() == c1.red() && c.blue() == c1.blue() && c.green() == c1.green()) {
                    c = c2;
                }
            }
        }
    }

    void Script::fill() {
        int x, y, w, h, r, g, b;
        input >> x >> y >> w >> h >> r >> g >> b;
        for (int i = x; i < x + w; i++) {
            for (int j = y; j < y + h; j++) {
                Color& c = image->at(i, j);
                c.red() = r;
                c.green() = g;
                c.blue() = b;
            }
        }
    }

    void Script::h_mirror() {
        for (int x = 0; x < image->width() / 2; x++) {
            for (int y = 0; y < image->height(); y++) {
                Color c = image->at(x, y);
                image->at(x, y) = image->at(image->width() - 1 - x, y);
                image->at(image->width() - 1 - x, y) = c;
            }
        }
    }

    void Script::v_mirror() {
        for (int x = 0; x < image->width(); x++) {
            for (int y = 0; y < image->height() / 2; y++) {
                Color c = image->at(x, y);
                image->at(x, y) = image->at(x, image->height() - 1 - y);
                image->at(x, image->height() - 1 - y) = c;
            }
        }
    }

    void Script::add() {
        int r, g, b, x, y;
        string filename;
        input >> filename >> r >> g >> b >> x >> y;
        Color cNeutral(r, g, b);
        Image* img2 = loadFromPNG(filename);
        for (int i = 0; i < img2->width(); i++) {
            for (int j = 0; j < img2->height(); j++) {
                Color& c = img2->at(i, j);
                if (c.red() != cNeutral.red() || c.blue() != cNeutral.blue() || c.green() != cNeutral.green()) {
                    image->at(x + i, y + j) = c;
                }
            }
        }
        delete img2;
    }

    void Script::crop(){
        int x, y, w, h;
        input >> x >> y >> w >> h;
        
        Image* newImage = new Image(w, h);
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                Color& c = image->at(x + i, y + j);
                newImage->at(i, j) = c;
            }
        }
        delete image;
        image = newImage;
    }

    void Script::rotate_left() {
        Image* newImage = new Image(image->height(), image->width());

        for (int i = 0; i < image->width(); i++) {
            for (int j = 0; j < image->height(); j++) {
                newImage->at(j, image->width() - 1 - i) = image->at(i, j);
            }
        }

        delete image;
        image = newImage;
    }

    void Script::rotate_right() {
        Image* newImage = new Image(image->height(), image->width());

        for (int i = 0; i < image->width(); i++) {
            for (int j = 0; j < image->height(); j++) {
                newImage->at(image->height() - 1 - j, i) = image->at(i, j);
            }
        }

        delete image;
        image = newImage;
    }

    void Script::xpm2_open() {
        string filePath;
        input >> filePath;
        image = loadFromXPM2(filePath);
    }

    void Script::xpm2_save() {
        string filePath;
        input >> filePath;
        saveToXPM2(filePath, image);
    }
}
