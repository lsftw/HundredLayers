//#include "Items.hpp"
//#include <fstream>
//#include "Utility.hpp"
#include "Base.h"

std::vector<Item> Item::itemsById;

const char* ITEM_SCHEMA_FILE = "data/items.txt";
const char* IMAGE_EXTENSION = ".png";
const char* BLANK_IMAGE_STR = "blank"; // signals not to draw any image

Item::Item()
{
    limitedUse = false;
    limitedCombat = false;
    charges = 1;
}

//
// Calculations / Constants
//

unsigned int Item::digitOffsetCoefficient()
{
    // get # digits in ID_MAX, yes this doesn't work for edge case ID_MAX = 0, doesn't matter
    unsigned int cur = ID_MAX;
    unsigned int digits = 0;
    while (cur > 0)
    {
        cur %= 10;
        ++digits;
    }
    // calculate coefficient, 10^(digits)
    unsigned int coefficient = 1;
    while (digits--)
    {
        coefficient *= 10;
    }
    return coefficient;
}
//
ItemId Item::calcHybridId(ItemId item1, ItemId item2)
{
    return item1 * digitOffsetCoefficient() + item2;
}
ItemId Item::extractId1(ItemId hybrid)
{
    return hybrid / digitOffsetCoefficient();
}
ItemId Item::extractId2(ItemId hybrid)
{
    return hybrid % digitOffsetCoefficient();
}

//
// Item Schema Loading
//

// Non-Private Helper for Item::initItems();
std::string Item::getImageName(const std::string name)
{
    std::string imageName = name;
    //
    unsigned int length = imageName.length();
    for (unsigned int i = 0; i < length; ++i)
    {
        if (imageName[i] == ' ')
        {
            imageName[i] = '_';
        }
        else if (imageName[i] >= 'A' && imageName[i] <= 'Z') {
            imageName[i] = imageName[i] - 'A' + 'a';
        }
    }
    return imageName.append(IMAGE_EXTENSION);
}

// Public Helper
std::string toLowercase(const std::string str)
{
    std::string lowercased = str;
    //
    unsigned int length = lowercased.length();
    for (unsigned int i = 0; i < length; ++i)
    {
        if (lowercased[i] >= 'A' && lowercased[i] <= 'Z') {
            lowercased[i] = lowercased[i] - 'A' + 'a';
        }
    }
    return lowercased;
}

// Private Helper for Item::initItems();
// tokenIndex = 0 means get first token, =1 means second, etc.
//
// Edge Case Warning:
// This *might* fail for a string of length (string::npos -1) with the desired token beginning at the (string::npos -1)th token
//  due to position offset
std::string parseToken(const std::string str, const char delimiter, const unsigned int tokenIndex)
{
    unsigned int tokenCount = -1;
    unsigned int position = 0;
    while (position != std::string::npos && ++tokenCount < tokenIndex)
    {
        position = str.find(delimiter, position) +1; // offset to skip over character
    }
    if (position != std::string::npos && tokenCount == tokenIndex)
    {
        unsigned int endPos = str.find(delimiter, position);
        return str.substr(position, endPos);
    }
    return NULL;
}

// Parses through the string for a positive integer, stopping upon reaching a non-numeric character
//
// Doesn't check for overflow
bool extractInt(const std::string str, unsigned int& val, unsigned int& pos)
{
    unsigned int length = str.length();
    bool numberFound = false;
    val = 0;
    for (pos = 0; pos < length && (str[pos] >= '0' && str[pos] <= '9'); ++pos)
    {
        val *= 10;
        val += str[pos] - '0';
        numberFound = true;
    }
    return numberFound;
}

void Item::initItems()
{
    std::ifstream itemSchema(ITEM_SCHEMA_FILE);
    std::string curLine;
    //
    std::string token; // temp used to parse curLine w/o modifying curLine
    ItemType type = NONE; // initial value to pacify compiler warning
    unsigned int val; // temp used to store parsed item charge count
    unsigned int pos; // temp used to check if limited use or limited per combat
    //
    ItemId lastId;
    Item* curItem = new Item;
    bool itemNamed = false;
    bool itemTyped = false;
    bool itemScripted = false;
    bool itemDefined = false; // all item fields filled?
    unsigned int lineNumber = 0;
    //
    if (itemSchema.is_open())
    {
        // Add Item #0: No Item / Empty Item Slot
        curItem->id = ID_NOITEM;
        curItem->name = "";
        curItem->type = NONE;
        curItem->imageName = BLANK_IMAGE_STR;
        itemsById.push_back(*curItem);
        //
        lastId = curItem->id;
        curItem = new Item;
        while (itemSchema.good())
        {
            ++lineNumber;
            getline(itemSchema, curLine);
            if (cleanLine(curLine))
            { // update item data
                if (!itemNamed)
                {
                    curItem->id = ++lastId;
                    curItem->name = curLine;
                    curItem->imageName = getImageName(curLine);
                    // TODO preload/load image?
                    itemNamed = true;
                }
                else if (!itemTyped) {
                    token = toLowercase(parseToken(curLine, ' ', 0));
                    if (token == "none")
                    {
                        type = NONE;
                    }
                    else if (token == "passive") {
                        type = PASSIVE;
                    }
                    else if (token == "active") {
                        type = ACTIVE;
                    }
                    else if (token == "hybrid") { // shouldn't happen normally
                        type = HYBRID;
                    }
                    else if (token == "invokable") {
                        type = INVOKABLE;
                    }
                    else {
                        warn() << "Item definition has unparseable type \""
                            << token << "\" on line " << lineNumber
                            << ", treating as no type." << std::endl;
                        type = NONE;
                    }
                    curItem->type = type; // TODO will there be a scope error?
                    // Get limited uses, if any
                    token = toLowercase(parseToken(curLine, ' ', 1));
                    if (extractInt(token, val, pos))
                    {
                        curItem->charges = val;
                        token = token.substr(pos);
                        if (token == "/combat")
                        {
                            curItem->limitedCombat = true;
                        }
                        else {
                            curItem->limitedUse = true;
                        }
                    }
                    itemTyped = true;
                    // TODO set limitedUses / charges with second token
                }
                else {
                    if (curLine.length() >= 2 && curLine[0] == '\"' && curLine[curLine.length() - 1] == '\"')
                    { // grab description from inside quotation marks
                        curItem->description.push_back(curLine.substr(1, curLine.length() - 2));
                    }
                    else {
                        // TODO if line doesn't start with, but still contains "", must generate description
                        curItem->scripting.push_back(curLine);
                        itemScripted = true;
                        // TODO generate description?
                    }
                }
            }
            else { // save item data, prepare for new item
                itemDefined = itemNamed && itemTyped && itemScripted;
                //
                if (itemDefined)
                {
                    itemsById.push_back(*curItem);
                    curItem = new Item;
                    itemNamed = false;
                    itemTyped = false;
                    itemScripted = false;
                    itemDefined = false;
                }
                else if (itemNamed) {
                    warn() << "Item definition incomplete by line " << lineNumber
                        << ", missing " << (itemTyped ? "scripting" : "type")
                        << ", ignoring incomplete item." << std::endl;
                }
            }
            std::cout << curLine << std::endl;
        }
    }
    else {
        //error("Cannot load item schema from file \"" + ITEM_SCHEMA_FILE + "\". Maybe the data folder is missing?");
        error() << "Cannot load item schema from file \"" << ITEM_SCHEMA_FILE << "\". Maybe the data folder is missing?" << std::endl;
        exit(-1);
    }
    itemSchema.close();
    // TODO check and warn for duplicate item names?
}
