#pragma once

#include <fstream>

#include "IScene.hpp"
#include "ConsoleEngine.hpp"
#include "Colour.hpp"
#include "ColourTransformer.hpp"


class DisplayBitmapScene : public IScene
{
private:

    class Sprite
    {

    private:
        ConsoleEngine& _consoleEngine;

    public:
        unsigned int Width = 0;
        unsigned int Height = 0;

        Colour* Pixels = nullptr;
        size_t PixelCount = 0;

    public:

        Sprite(ConsoleEngine& consoleEngine) :
            _consoleEngine(consoleEngine)
        {
        };


    public:

        void DrawSprite(int x, int y) const
        {
            for (size_t spriteX = 0; spriteX < Width; spriteX++)
            {
                for (size_t spriteY = 0; spriteY < Height; spriteY++)
                {
                    size_t pixelDataIndexer = spriteX + Width * spriteY;

                    Colour& colour = Pixels[pixelDataIndexer];

                    _consoleEngine.SetConsolePixel(spriteX + x, spriteY + y, ColourTransformer::RGBToConsoleColour(colour), false);
                };
            };
        };


        void DrawSprite(int x, int y, int xOffset, int yOffset, int width, int height) const
        {

            for (size_t spriteX = xOffset; spriteX < width; spriteX++)
            {
                for (size_t spriteY = yOffset; spriteY < height; spriteY++)
                {

                    size_t pixelDataIndexer = spriteX + Width * spriteY;

                    Colour& colour = Pixels[pixelDataIndexer];

                    _consoleEngine.SetConsolePixel(spriteX + x, spriteY + y, ColourTransformer::RGBToConsoleColour(colour), false);
                };
            };

        };


        void DrawSpriteChromaKey(int x, int y, int xOffset, int yOffset, int width, int height, const Colour& chromaColour) const
        {

            for (size_t spriteX = 0; spriteX < width - xOffset; spriteX++)
            {
                for (size_t spriteY = 0; spriteY < height - yOffset; spriteY++)
                {
                    size_t pixelDataIndexer = (spriteX + xOffset) + Width * (spriteY + yOffset);

                    Colour& colour = Pixels[pixelDataIndexer];

                    if (colour == chromaColour)
                        continue;

                    _consoleEngine.SetConsolePixel(spriteX + x, spriteY + y, ColourTransformer::RGBToConsoleColour(colour), false);
                };
            };

        };


        void DrawSpriteColourChromaKey(int x, int y, int xOffset, int yOffset, int width, int height, const Colour& spriteolour, const Colour& chromaColour) const
        {

            for (size_t spriteX = 0; spriteX < width - xOffset; spriteX++)
            {
                for (size_t spriteY = 0; spriteY < height - yOffset; spriteY++)
                {
                    size_t pixelDataIndexer = (spriteX + xOffset) + Width * (spriteY + yOffset);

                    Colour& colour = Pixels[pixelDataIndexer];

                    if (colour == chromaColour)
                        continue;

                    _consoleEngine.SetConsolePixel(spriteX + x, spriteY + y, ColourTransformer::RGBToConsoleColour(spriteolour), false);
                };
            };

        };


    };

private:
    ConsoleEngine& _consoleEngine;

    Sprite _sprite;

private:

public:

    DisplayBitmapScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),
        _sprite(consoleEngine)
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
        _sprite.Width = Read4Bytes(&dibHeader[4]);
        _sprite.Height = Read4Bytes(&dibHeader[8]);

        // An offset starting from the begging to where the pixel data starts
        unsigned int pixelDataOffset = Read4Bytes(&fileBytes[10]);


        _sprite.PixelCount = static_cast<size_t>(_sprite.Width) * static_cast<size_t>(_sprite.Height);

        // The pixels inside the bitmap
        _sprite.Pixels = new Colour[_sprite.PixelCount];
        memset(_sprite.Pixels, 0, _sprite.PixelCount * sizeof(Colour));

        // Calculate row padding
        int padding = (4 - _sprite.Width % 4) % 4;


        int pixelIndex = pixelDataOffset;

        // Read the pixel data
        // Starts from y = _bmpHeight because bitmaps are stored upside vertically 
        for (long long y = _sprite.Height - 1; y >= 0; y--)
        {
            for (long long x = 0; x < _sprite.Width; x++)
            {
                // Calculate position of current pixel
                long long position = x + _sprite.Width * y;

                // Read pixels and create a colour
                Colour colour;

                colour.Red = fileBytes[pixelIndex++];
                colour.Green = fileBytes[pixelIndex++];
                colour.Blue = fileBytes[pixelIndex++];

                // Inset the colour into pixels array
                _sprite.Pixels[position] = colour;
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


    void DrawTextFromSprite(int x, int y, std::wstring text, Sprite& sprite, ConsoleEngine::ConsoleColour textColor = ConsoleEngine::ConsoleColour::WHITE)
    {
        int glyphWidth = 16;
        int glyphHeight = 28;

        for (size_t a = 0; a < text.length(); a++)
        {
            const wchar_t& character2 = text[a];

            int glyphX = character2 % 32;
            int glyphY = (character2 / 32) - 1;

            int x1 = (glyphX * glyphWidth);
            int y1 = (glyphY * glyphHeight);

            int x2 = (x1 + glyphWidth);
            int y2 = y1 + glyphHeight;

            _sprite.DrawSpriteColourChromaKey(x + (a * glyphWidth), y, x1, y1, x2, y2, { 255, 0, 255 }, { 255, 255, 255 });
        }
    };


    virtual void DrawScene() override
    {
        int spriteXPos = 75;
        int spriteYPos = 0;

        DrawTextFromSprite(spriteXPos, spriteYPos, L"Text text", _sprite);
    };
};