#ifndef DISPLAYELEMENT_H_INCLUDED
#define DISPLAYELEMENT_H_INCLUDED

// future improvement: implement caching
// A class meant to simplify image display from file
class DisplayElement
{
    private:
    sf::Image image;
    sf::Sprite sprite;
    sf::Texture texture;
    //
    public:
    DisplayElement(const std::string& fileName, unsigned int x = 0, unsigned int y = 0);/*
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
    }*/
    //
    sf::Sprite getSprite();/*
    {
        return sprite;
    }*/
};

#endif
