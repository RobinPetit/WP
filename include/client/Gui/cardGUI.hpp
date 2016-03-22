#ifndef CARDGUI_HPP
#define CARDGUI_HPP


class CardGUI
{
private:
    // name,cost, and effects
    sf::Text _effects;
    sf::Text _name;
    sf::Text _cost;

    //loading characters font
    sf::Font _font;
    // Arbitrary shape
    int const LENGTH = 360;
    int const WIDTH  = 240;

    int const NAME_POSITION_X = 24;
    int const NAME_POSITION_Y = 36;

    int const EFFECTS_POSITION_X = 10;
    int const EFFECTS_POSITION_Y = 28;

    int const CHAR_SIZE = 20;

    /* Back view texture and sprite only, because the front
    one depends on if it's a spell or creature card*/
    sf::RectangleShape _backView;
    sf::Texture _backTexture;

    sf::RectangleShape _picture;
    sf::Texture _pictureTexture;


public:
    CardGUI(std::string effects, std::string name, std::string cost);

    /// Getters
    inline sf::Text   getEffects() {return _effects;}
    inline sf::Text   getName() {return _name;}
    inline sf::Text   getCost() {return _cost;}

    /// Setters
    void setCost(std::string cost) {_cost.setString(cost);}

    /// Others
    void drawBack();
    void virtual drawFront() = 0;
};



#endif //CARDGUI_HPP
