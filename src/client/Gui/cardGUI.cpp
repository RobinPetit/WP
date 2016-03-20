#include "client/Gui/CardGui.hpp"


using namespace std;
using namespace sf;

CardGUI::CardGUI()
{
    if (!_backTexture.loadFromFile("back.png"))
        {
            std::cout << "Error back loading" ;
        }// Standarde protocole SFML 2.0 error ( CF documentations )
    _frontTexture.setSmooth(true); // enable smooth borders

    if (!_frontTexture.loadFromFile("front.png"))
        {
            std::cout << "Error front loading" ;
        }
    _frontTexture.setSmooth(true);

    _backView.setTexture(_backTexture);
    _frontView.setTexture(_frontTexture);
}
