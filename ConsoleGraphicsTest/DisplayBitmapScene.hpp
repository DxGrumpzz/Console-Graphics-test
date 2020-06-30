#pragma once

#include <fstream>

#include "IScene.hpp"
#include "ConsoleEngine.hpp"
#include "Colour.hpp"

class DisplayBitmapScene : public IScene
{
private:


private:
    ConsoleEngine& _consoleEngine;

    Colour* _pixelData = nullptr;
    size_t _pixelDataCount;

    unsigned int _bmpWidth = 0;
    unsigned int _bmpHeight = 0;


private:

public:

    DisplayBitmapScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine)
    {
        int glpyhWidth = 16;
        int glpyhHeight = 28;

        std::ifstream fontFile("Resources\\Fixedsys16x28.bmp",
                               // Read/open the file in binary mode (handles \n\r characters differently, and more)
                               std::ios::binary |
                               // Move reader head to the end of the file so we can get the size of the file
                               std::ios::ate);

        if (fontFile.good() == false)
        {
            throw std::exception("Unable to open file");
        };

        std::streamsize bufferSize = fontFile.tellg();
        fontFile.seekg(0);

        uint8_t* fileBytes = new uint8_t[bufferSize + 0] { 0 };

        fontFile.read(reinterpret_cast<char*>(fileBytes), bufferSize);


        uint8_t bitmapHeader[14] { 0 };
        memcpy_s(bitmapHeader, 14, fileBytes, 14);


        unsigned int dibHeaderSize = Read4Bytes(&fileBytes[14]);

        uint8_t* dibHeader = new uint8_t[dibHeaderSize] { 0 };
        memcpy_s(dibHeader, dibHeaderSize, &fileBytes[14], dibHeaderSize);

        _bmpWidth = Read4Bytes(&dibHeader[4]);
        _bmpHeight = Read4Bytes(&dibHeader[8]);

        unsigned int horizontalResolution = Read4Bytes(&fileBytes[38]);
        unsigned int verticalResolution = Read4Bytes(&fileBytes[42]);

        unsigned short bitsPerPixel = Read2Bytes(&fileBytes[28]);

        unsigned int numberOfColoursInColourPallete = Read2Bytes(&fileBytes[46]);

        unsigned int imageSize = Read4Bytes(&fileBytes[34]);


        unsigned int pixelDataOffset = Read4Bytes(&fileBytes[10]);


        _pixelDataCount = imageSize;// static_cast<size_t>(_bmpWidth)* static_cast<size_t>(_bmpWHeight);

        _pixelData = new Colour[_pixelDataCount * sizeof(Colour)] { 0 };


        uint8_t* pixelDataPointer = &fileBytes[pixelDataOffset];


        int padding = (4 - _bmpWidth % 4) % 4;

        fontFile.seekg(pixelDataOffset);

        for (long long y = _bmpHeight; y >= 0; y--)
        {
            for (long long x = 0; x < _bmpWidth; x++)
            {
                int position = x + _bmpWidth * y;

                Colour c;
                c.Red = fontFile.get();
                c.Green = fontFile.get();
                c.Blue = fontFile.get();

                _pixelData[position] = c;

            };

            fontFile.seekg(padding, std::ios::cur);
        };


        delete[] dibHeader;
        dibHeader = nullptr;

        delete[] fileBytes;
        fileBytes = nullptr;
    };


    unsigned int Read4Bytes(const uint8_t const* data)
    {
        return (data[0] |
                data[1] << 8 |
                data[2] << 8 * 2 |
                data[3] << 8 * 3);
    };

    unsigned short Read2Bytes(const uint8_t const* data)
    {
        return (data[0] |
                data[1] << 8);
    };


public:

    virtual void UpdateScene(float deltaTime) override
    {

    };

    virtual void DrawScene() override
    {
        int imageXPos = 5;
        int imageYPos = 5;

        for (size_t x = 0; x < _bmpWidth; x++)
        {
            for (size_t y = 0; y < _bmpHeight; y++)
            {
                size_t pixelDataIndexer = x + _bmpWidth * y;

                Colour& colour = _pixelData[pixelDataIndexer];

                if (colour.Red   == 255 &&
                    colour.Green == 255 &&
                    colour.Blue  == 255)
                {
                    _consoleEngine.SetConsolePixel(imageXPos + x, imageYPos + y, ConsoleEngine::ConsoleColour::WHITE, false);
                }
                else if (colour.Red   == 0 &&
                         colour.Green == 0 &&
                         colour.Blue  == 0)
                {
                    _consoleEngine.SetConsolePixel(imageXPos + x, imageYPos + y, ConsoleEngine::ConsoleColour::BLACK, false);
                }
                else
                {
                    int s = 0;
                };
            };
        };


        /*
        size_t pixelDataIndexer = 0;
        for (size_t y = 0; y < _bmpHeight - 1; y += 4)
        {
            for (size_t x = 0; x < _bmpWidth - 1; x += 4)
            {
                uint8_t red = _pixelData[pixelDataIndexer];
                uint8_t green = _pixelData[pixelDataIndexer] >> 8;
                uint8_t blue = _pixelData[pixelDataIndexer] >> 8 * 2;

                if (red == 255 &&
                    green == 255 &&
                    blue == 255)
                {
                    _consoleEngine.SetConsolePixel(imageXPos + x, imageYPos + y, ConsoleEngine::ConsoleColour::RED, false);
                }
                else if (red == 0 &&
                         green == 0 &&
                         blue == 0)
                {
                    _consoleEngine.SetConsolePixel(imageXPos + x, imageYPos + y, ConsoleEngine::ConsoleColour::GREEN, false);
                }
                else
                {
                    int s = 0;
                };

                pixelDataIndexer++;
            };
        };
        */


        //_consoleEngine.DrawConsoleText(0, 0, L"Bitmap scene");
    };



};