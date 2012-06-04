//#include "Hud.hpp"
#include "Base.h"

// Responsible for all the UI elements
// Data in here does not need to be saved/loaded
// Note: Currently positioning of UI elements assumes 400x600; doesn't scale


// GUI Size Constants
const int STATS_TOTAL_HEIGHT = 100 + 2; // stats box + divider
const int INVENTORY_Y = 350 - 3; // y position of inventory, -y position for top border of inventory
//
const int ITEM_SLOT_HEIGHT = 80 + 5; // slot + divider
const int INVENTORY_BORDER_WIDTH = 2; // leftmost / rightmost borders of inventory
const int ITEM_USABILITY_WIDTH = 15; // item slot usability bar width
const int ITEM_IMAGE_X = INVENTORY_BORDER_WIDTH + ITEM_USABILITY_WIDTH; // x position of item image

// Game GUI - floor, stats / inventory, activatable / descent, player / enemy
std::vector<DisplayElement> guiElements;
//

void initHud()
{
    // TODO make it based on playersession, currently this is just a test function
    // Game GUI - floor, stats / inventory, activatable / descent, player / enemy
    DisplayElement* guiFloor = new DisplayElement("res/floor/stone_floor.png", 0, STATS_TOTAL_HEIGHT);
    guiElements.push_back(*guiFloor);
    //
    DisplayElement* guiStats = new DisplayElement("res/gui/stats.png");
    guiElements.push_back(*guiStats);
    DisplayElement* guiInventory = new DisplayElement("res/gui/inventory.png", 0, INVENTORY_Y -3);
    guiElements.push_back(*guiInventory);
    //
    DisplayElement* guiItem1 = new DisplayElement("res/item/olive_branch.png", ITEM_IMAGE_X, INVENTORY_Y);
    guiElements.push_back(*guiItem1);
    DisplayElement* guiItem1_usability = new DisplayElement("res/gui/item-usable.png", INVENTORY_BORDER_WIDTH, INVENTORY_Y);
    guiElements.push_back(*guiItem1_usability);
    DisplayElement* guiItem2 = new DisplayElement("res/item/green_box.png", ITEM_IMAGE_X, INVENTORY_Y + ITEM_SLOT_HEIGHT*1);
    guiElements.push_back(*guiItem2);
    DisplayElement* guiItem2_usability = new DisplayElement("res/gui/item-unusable.png", INVENTORY_BORDER_WIDTH, INVENTORY_Y + ITEM_SLOT_HEIGHT*1);
    guiElements.push_back(*guiItem2_usability);
}

void drawUI(sf::RenderWindow* app)
{
    for (unsigned int i = 0; i < guiElements.size(); ++i) {
        app->draw(guiElements.at(i).getSprite());
    }
}
