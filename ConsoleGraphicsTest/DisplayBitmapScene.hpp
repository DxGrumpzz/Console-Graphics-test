#pragma once

#include <fstream>

#include "IScene.hpp"
#include "ConsoleEngine.hpp"

class DisplayBitmapScene : public IScene
{

private:
    ConsoleEngine& _consoleEngine;

    unsigned int* _pixelData;
    size_t _pixelDataCount;

    unsigned int _bmpWidth = 0;
    unsigned int _bmpWHeight = 0;

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


        unsigned short DIB_Header = Read2Bytes(&bitmapHeader[0]);

        // DIB header equals to BM
        if (DIB_Header == (0x42 | 0x4D))
        {
            int s = 0;
        };


        unsigned int dibHeaderSize = Read4Bytes(&fileBytes[14]);

        uint8_t* dibHeader = new uint8_t[dibHeaderSize] { 0 };
        memcpy_s(dibHeader, dibHeaderSize, &fileBytes[14], dibHeaderSize);

        _bmpWidth = Read4Bytes(&dibHeader[4]);
        _bmpWHeight = Read4Bytes(&dibHeader[8]);

        unsigned int horizontalResolution = Read4Bytes(&fileBytes[38]);
        unsigned int verticalResolution = Read4Bytes(&fileBytes[42]);

        unsigned short bitsPerPixel = Read2Bytes(&fileBytes[28]);

        unsigned int numberOfColoursInColourPallete = Read2Bytes(&fileBytes[46]);

        unsigned int imageSize = Read4Bytes(&fileBytes[34]);


        unsigned int pixelDataOffset = 14 + dibHeaderSize + numberOfColoursInColourPallete;


        _pixelDataCount = static_cast<size_t>(_bmpWidth) * static_cast<size_t>(_bmpWHeight);

        _pixelData = new unsigned int[_pixelDataCount]
        {
            0
        };


        uint8_t* pixelDataPointer = &fileBytes[pixelDataOffset];


        size_t pixelDataIndexer = 0;
        for (size_t y = 0; y < _bmpWHeight - 1; y += 4)
        {
            for (size_t x = 0; x < _bmpWidth - 1; x += 4)
            {
                uint8_t* p = &pixelDataPointer[x + _bmpWidth * y];

                unsigned int pp = Read4Bytes(p);

                _pixelData[pixelDataIndexer] = pp;

                int ss = 0;
                pixelDataIndexer++;
            };
        };



        //delete[] pixelData;
        //pixelData = nullptr;

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
        int imageXPos = 0;
        int imageYPos = 0;

        size_t pixelDataIndexer = 0;
        for (size_t y = 0; y < _bmpWHeight - 1; y += 4)
        {
            for (size_t x = 0; x < _bmpWidth - 1; x += 4)
            {
                uint8_t alpha = _pixelData[pixelDataIndexer];
                uint8_t red =   _pixelData[pixelDataIndexer] >> 8;
                uint8_t green = _pixelData[pixelDataIndexer] >> 8 * 2;
                uint8_t blue =  _pixelData[pixelDataIndexer] >> 8 * 3;

                if (red == 255 &&
                    green == 255 &&
                    blue == 255)
                {
                    _consoleEngine.SetConsolePixel(imageXPos + x, imageYPos + y, ConsoleEngine::ConsoleColour::WHITE, false);
                }
                else if (red == 0 &&
                         green == 0 &&
                         blue == 0)
                {
                    _consoleEngine.SetConsolePixel(imageXPos + x, imageYPos + y, ConsoleEngine::ConsoleColour::BLACK, false);
                }
                else
                {
                    int s = 0;
                };

                pixelDataIndexer++;
            };
        };


        //_consoleEngine.DrawConsoleText(0, 0, L"Bitmap scene");
    };



};