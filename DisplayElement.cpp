//#include "DisplayElement.hpp"
#include "Base.h"

// future improvement: implement caching
// A class meant to simplify image display from file

DisplayElement::DisplayElement(const std::string& fileName, unsigned int x, unsigned int y)
{
    if (!image.loadFromFile(fileName))
    {
        //error("Can't load image from file \"" + fileName + "\"!");
        error() << "Can't load image from file \"" << fileName << "\"!";
        exit(-1);
    }
    if (!texture.loadFromImage(image))
    {
        //error("Can't load image \"" + fileName + "\" into a texture!");
        error() << "Can't load image \"" << fileName << "\" into a texture!";
        exit(-1);
    }
    sprite.setTexture(texture);
    //
    sprite.setPosition(x, y);
}
//
sf::Sprite DisplayElement::getSprite()
{
    return sprite;
}
