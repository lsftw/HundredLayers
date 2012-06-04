#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <string>
#include <vector>

// Stores all the information about items, retrieve the information with an ItemId
typedef unsigned int ItemId;

// If you ever add, remove, or rearrange this enum, make sure you make changes to the type parsing in initItems()
enum ItemType {NONE, PASSIVE, ACTIVE, HYBRID, INVOKABLE}; // NONE can be in either exploration/combat slot

/// TODO where should item scripting go?
/// TODO keep list of all items, add in the no items for 0 & warn for bad queries

// built-in limitation of 10,000 different items, though it should work fine if you increase/decrease
// ^- but save/load compatibility probably would break between files w/ different max number of items
// Hybrid Item Id-ing: digit combination, fill gap between 1st & 2nd item w/ zeroes
// Item # 85 + Item # 10000 = 8510000
// Item # 597 + Item # 4 = 59700004
// hybridid = item1id * pow(10, digits(ID_MAX)) + item2id
// item1id = hybridid / pow(10, digits(ID_MAX))
// item2id = hybridid % pow(10, digits(ID_MAX))

class Item
{
    private:
    ItemId id; // unique identifier of items
    std::string name;
    std::string imageName;
    ItemType type;
    // only 1 limited flag can be true, but both can be false
    bool limitedUse; // limited uses before vanishing, for exploration/active/hybrid
    bool limitedCombat; // limited uses per combat, for active/hybrid
    unsigned int charges; // only matters if invokable / limited use per combat
    //
    std::vector<std::string> description;
    std::vector<std::string> scripting; // after loading, this shouldn't be modified
    //
    static std::vector<Item> itemsById;
    // static std::map<std::string, Item> itemsByName;

    public:
    static const ItemId ID_NOITEM = 0; // represents an empty item slot, recommended to be lowest value
    static const ItemId ID_MAX = 10000; // affects hybrid item id calculation

    Item();

    // V- make this into a constant expression when that C++ update comes out
    static unsigned int digitOffsetCoefficient();
    // Hybrid Item ID Calculation Methods
    static ItemId calcHybridId(ItemId item1, ItemId item2);
    static ItemId extractId1(ItemId hybrid);
    static ItemId extractId2(ItemId hybrid);

    // Generates image name by lowercasing and replacing spaces with underscores
    static std::string getImageName(const std::string name);

    // Load all item information
    static void initItems();
};

#endif
