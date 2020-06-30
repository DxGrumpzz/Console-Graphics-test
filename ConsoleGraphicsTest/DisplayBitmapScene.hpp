#pragma once

#include <fstream>

#include "IScene.hpp"
#include "ConsoleEngine.hpp"
#include "Colour.hpp"
#include "ColourTransformer.hpp"

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
        std::ifstream fontFile("Resources\\Fixedsys16x28.bmp",
                               // Read/open the file in binary mode (handles \n\r characters differently, and more)
                               std::ios::binary |
                               // Move reader head to the end of the file so we can get the size of the file
                               std::ios::ate);

        // Check if file loaded succesfully
        if (fontFile.good() == false)
        {
            throw std::exception("Unable to open file");
        };

        // Get file size
        std::streamsize bufferSize = fontFile.tellg();

        // Restore reader position back to beggining
        fontFile.seekg(0);

        // A buffer that will store the file data
        uint8_t* fileBytes = new uint8_t[bufferSize + 0] { 0 };

        // Read the file into the buffer
        fontFile.read(reinterpret_cast<char*>(fileBytes), bufferSize);
        fontFile.seekg(0);

        // A header for this bitmap
        const int headerSize = 14;
        uint8_t bitmapHeader[headerSize] { 0 };
        fontFile.read(reinterpret_cast<char*>(bitmapHeader), headerSize);

        // The bitmap's DIB header size
        unsigned int dibHeaderSize = Read4Bytes(&fileBytes[14]);

        // The bitmap's actuall DIB header
        uint8_t* dibHeader = new uint8_t[dibHeaderSize] { 0 };

        // Read DIB header data from file
        fontFile.read(reinterpret_cast<char*>(dibHeader), dibHeaderSize);

        // Get image width and height
        _bmpWidth = Read4Bytes(&dibHeader[4]);
        _bmpHeight = Read4Bytes(&dibHeader[8]);

        // An offset starting from the begging to where the pixel data starts
        unsigned int pixelDataOffset = Read4Bytes(&fileBytes[10]);


        _pixelDataCount = static_cast<size_t>(_bmpWidth) * static_cast<size_t>(_bmpHeight);

        // The pixels inside the bitmap
        _pixelData = new Colour[_pixelDataCount];
        memset(_pixelData, 0, _pixelDataCount * sizeof(Colour));

        // Calculate row padding
        int padding = (4 - _bmpWidth % 4) % 4;


        int pixelIndex = pixelDataOffset;

        // Read the pixel data
        // Starts from y = _bmpHeight because bitmaps are stored upside vertically 
        for (long long y = _bmpHeight - 1; y >= 0; y--)
        {
            for (long long x = 0; x < _bmpWidth; x++)
            {
                // Calculate position of current pixel
                long long position = x + _bmpWidth * y;

                // Read pixels and create a colour
                Colour colour;

                colour.Red = fileBytes[pixelIndex++];
                colour.Green = fileBytes[pixelIndex++];
                colour.Blue = fileBytes[pixelIndex++];

                // Inset the colour into pixels array
                _pixelData[position] = colour;
            };

            // After we finish reading a row move the pixelIndex by the row padding
            pixelIndex += padding;
        };


        delete[] dibHeader;
        dibHeader = nullptr;

        delete[] fileBytes;
        fileBytes = nullptr;
    };


    unsigned int Read4Bytes(const uint8_t* data)
    {
        return (data[0] |
                data[1] << 8 |
                data[2] << 8 * 2 |
                data[3] << 8 * 3);
    };

    unsigned short Read2Bytes(const uint8_t* data)
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

                _consoleEngine.SetConsolePixel(imageXPos + x, imageYPos + y, ColourTransformer::RGBToConsoleColour(colour), false);
            };
        };

    };
};