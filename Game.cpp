#include "Base.h"

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 600;
const char * WINDOW_TITLE = "Hundred Layers";

const int FPS_CAP = 60;

// GUI functions

typedef void (*GuiFunction)(sf::RenderWindow*); // takes app, returns void

typedef std::map<sf::Keyboard::Key, GuiFunction> KeyFunctionMap;

void manualExit(sf::RenderWindow*);
void takeScreenshot(sf::RenderWindow*);

//

void runKeyPressed(sf::RenderWindow*, const KeyFunctionMap, sf::Keyboard::Key);

//

int main()
{
    // Initialization
    sf::RenderWindow* app = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), WINDOW_TITLE);
    Item::initItems();
    initHud();
    // Lookup Tables for Keyboard Actions
    KeyFunctionMap keyPressedAction;
    keyPressedAction[sf::Keyboard::Escape] = manualExit;
    keyPressedAction[sf::Keyboard::F12] = takeScreenshot;
    //app->UseVerticalSync(true); // VSYNC caps FPS at display fps (usually 60)
    app->setFramerateLimit(FPS_CAP);
    app->setMouseCursorVisible(true);
    // Main Loop
    sf::Clock clock;
    while (app->isOpen())
    {
        sf::Event event;
        while (app->pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                runKeyPressed(app, keyPressedAction, event.key.code);
            }
            //
            if (event.type == sf::Event::Resized)
            {
                //glViewport(0, 0, event.Size.Width, event.Size.Height);
                // doSomethingWithTheNewSize(event.size.width, event.size.height);
                /// TODO make code for resizing work, ex: mouse tracking
            }
            //
            if (event.type == sf::Event::Closed) manualExit(app);
        }

        //app->setActive();
        // Repaint
        app->clear(sf::Color(255, 255, 255));
        drawUI(app);
        app->display();
    }
    //
    return 0;
}

//

void manualExit(sf::RenderWindow* app)
{
    std::cout << "Manual exit" << std::endl;
    app->close();
}

void takeScreenshot(sf::RenderWindow* app)
{
    sf::Image screenshot = app->capture();
    std::string ssFileName = "screenshot.png";
    screenshot.saveToFile(ssFileName);
    std::cout << "Screenshot saved to \"" << ssFileName << "\"." << std::endl;
}

//

void runKeyPressed(sf::RenderWindow* app, const KeyFunctionMap kfm, sf::Keyboard::Key key)
{
    if (kfm.find(key) == kfm.end()) return; // no value found in map
    //
    GuiFunction function = kfm.find(key)->second;
    function(app);
}
