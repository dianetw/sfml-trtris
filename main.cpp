#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <ctime>

int main() {
    srand((unsigned int)time(NULL));
    const int fieldWidth = 10;
    const int fieldHeight = 20;

    enum class Type { None, O, I, L, J, T, S, Z };
    Type field[fieldWidth][fieldHeight] = {};
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        std::cout << "background.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture yelloTexture;
    if (!yelloTexture.loadFromFile("yellow.png")) {
        std::cout << "yellow.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture lightBlueTexture;
    if (!lightBlueTexture.loadFromFile("light_blue.png")) {
        std::cout << "light_blue.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture blueTexture;
    if (!blueTexture.loadFromFile("blue.png")) {
        std::cout << "blue.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture greenTexture;
    if (!greenTexture.loadFromFile("green.png")) {
        std::cout << "green.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture orangeTexture;
    if (!orangeTexture.loadFromFile("orange.png")) {
        std::cout << "orange.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture purpleTexture;
    if (!purpleTexture.loadFromFile("purple.png")) {
        std::cout << "purple.png is not found." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture redTexture;
    if (!redTexture.loadFromFile("red.png")) {
        std::cout << "red.png is not found." << std::endl;
        return EXIT_FAILURE;
    }
    const int blockWidth = yelloTexture.getSize().x;
    const int blockHeight = yelloTexture.getSize().y;

    const int windowWidth = blockWidth * fieldWidth;
    const int windowHeight = blockHeight * fieldHeight;

    std::map<Type, sf::Sprite> sprites = {
        {Type::O, sf::Sprite(yelloTexture)},
        {Type::I, sf::Sprite(lightBlueTexture)},
        {Type::L, sf::Sprite(blueTexture)},
        {Type::J, sf::Sprite(greenTexture)},
        {Type::T, sf::Sprite(orangeTexture)},
        {Type::S, sf::Sprite(purpleTexture)},
        {Type::Z, sf::Sprite(redTexture)},
    };

    backgroundTexture.setRepeated(true);
    sf::Sprite backgroundSprite(
        backgroundTexture,
        sf::IntRect(0, 0, windowWidth, windowHeight));

    std::vector<sf::Vector2i> currentShape;
    sf::Sprite currentSprite;

    const sf::Vector2i orign(fieldWidth / 2, 0);
    sf::Vector2i pos(orign);  // current position

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "arial.ttf is not found." << std::endl;
        return EXIT_FAILURE;
    }
    unsigned int high_score = 0;
    std::string fileName("HighScore.txt");
    std::fstream highScoreFile;
    highScoreFile.open(fileName, std::fstream::in | std::fstream::out);
    if (!highScoreFile.is_open()) {
        std::cout << "HighScore.txt is not found, will create a new one." << std::endl;
        highScoreFile.open(fileName, std::fstream::out);
        highScoreFile << 0;
    }
    else {
        highScoreFile >> high_score;
    }
    //  Use array to register data
    std::map<Type, std::vector<std::vector<sf::Vector2i>>> shapes = {
        {
            Type::O,
            {
                // O: OO (0,-1) (1,-1)
                //    OO (0, 0) (1, 0)
                {
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                    sf::Vector2i(0,-1),
                    sf::Vector2i(1,-1),
                }
            },
        },
        {
            Type::I,
            {
                // I: OOOO (-1,0) (0, 0) (1, 0) (2, 0)  shape_Is[0]
                {
                    sf::Vector2i(-1, 0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                    sf::Vector2i(2, 0),
                },
                // I: O (0,-2)   shape_Is[1]
                //    O (0,-1)
                //    O (0, 0)
                //    O (0, 1)
                {
                    sf::Vector2i(0,-2),
                    sf::Vector2i(0,-1),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(0, 1),
                }
            }
        },
        {
            Type::L,
            {
                // L: OOO (0, 0) (1, 0) (2, 0)    shape_Ls[0]
                //    O   (0, 1)
                {
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                    sf::Vector2i(2, 0),
                    sf::Vector2i(0, 1),
                },
                // L: O   (0,-2)       shape_Ls[1]
                //    O   (0,-1)
                //    OO  (0, 0) (1, 0)
                {
                    sf::Vector2i(0,-2),
                    sf::Vector2i(0,-1),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                },
                // L:   O                (0,-1)   shape_Ls[2]
                //    OOO  (-2,0) (-1,0) (0, 0)
                {
                    sf::Vector2i(0,-1),
                    sf::Vector2i(-2,0),
                    sf::Vector2i(-1,0),
                    sf::Vector2i(0, 0),
                },
                // L: OO  (-1, 0) (0, 0)     shape_Ls[3]
                //     O          (0, 1)
                //     O          (0, 2)
                {
                    sf::Vector2i(-1, 0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(0, 1),
                    sf::Vector2i(0, 2),
                }
            }
        },
        {
            Type::J,
            {
                // J: O   (0,-1)                shape_Js[0]
                //    OOO (0, 0) (1, 0) (2, 0)
                {
                    sf::Vector2i(0,-1),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                    sf::Vector2i(2, 0),
                },
                // J:  O        (0,-2)     shape_Js[1]
                //     O        (0,-1)
                //    OO (-1,0) (0, 0)
                {
                    sf::Vector2i(0,-2),
                    sf::Vector2i(0,-1),
                    sf::Vector2i(-1,0),
                    sf::Vector2i(0, 0),
                },
                // J: OOO (-2,0) (-1,0) (0, 0)   shape_Js[2]
                //      O               (0, 1)
                {
                    sf::Vector2i(-2,0),
                    sf::Vector2i(-1,0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(0, 1),
                },
                // J: OO (0, 0) (1, 0)    shape_Js[3]
                //    O  (0, 1)
                //    O  (0, 2)
                {
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                    sf::Vector2i(0, 1),
                    sf::Vector2i(0, 2),
                }
            }
        },
        {
            Type::T,
            {
                // T:  O          (0,-1)
                //    OOO (-1, 0) (0, 0) (1, 0)  shape_Ts[0]
                {
                    sf::Vector2i(0,-1),
                    sf::Vector2i(-1, 0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                },
                // T:  O         (0,-1)    shape_Ts[1]
                //    OO (-1, 0) (0, 0)
                //     O         (0, 1)
                {
                    sf::Vector2i(0,-1),
                    sf::Vector2i(-1,0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(0, 1),
                },
                // T:  OOO (-1, 0) (0, 0) (1, 0)  shape_Ts[2]
                //      O          (0, 1)
                {
                    sf::Vector2i(-1, 0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                    sf::Vector2i(0, 1),
                },
                // T: O  (0,-1)           shape_Ts[3]
                //    OO (0, 0) (1, 0)
                //    O  (0, 1)
                {
                    sf::Vector2i(0,-1),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                    sf::Vector2i(0, 1),
                }
            }
        },
        {
            Type::S,
            {
                // S:  OO         (0,-1) (1,-1)    shape_Ss[0]
                //    OO  (-1, 0) (0, 0)
                {
                    sf::Vector2i(0,-1),
                    sf::Vector2i(1,-1),
                    sf::Vector2i(-1, 0),
                    sf::Vector2i(0, 0),
                },
                // S: O   (-1,-1)                 shape_Ss[1]
                //    OO  (-1, 0) (0, 0)
                //     O          (0, 1)
                {
                    sf::Vector2i(-1,-1),
                    sf::Vector2i(-1, 0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(0, 1),
                }
            }
        },
        {
            Type::Z,
            {
                // Z: OO   (-1,-1) (0,-1)         shape_Zs[0]
                //     OO          (0, 0) (1, 0)
                {
                    sf::Vector2i(-1,-1),
                    sf::Vector2i(0,-1),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(1, 0),
                },
                // Z:  O          (0,-1)           shape_Zs[1]
                //    OO  (-1, 0) (0, 0)
                //    O   (-1, 1)
                {
                    sf::Vector2i(0,-1),
                    sf::Vector2i(-1, 0),
                    sf::Vector2i(0, 0),
                    sf::Vector2i(-1, 1),
                }
            }
        }
    };
    Type currentType = Type(rand() % 7 + 1);
    int currentIndex = 0;
    enum class GameState { RUNNING, PAUSE, LOSE };
    GameState gameState = GameState::RUNNING;

    sf::Text text;
    sf::Text warnText;
    sf::Text timeText;
    sf::Text highscoreText;
    sf::Text hintText;
    text.setFont(font);
    warnText.setFont(font);
    timeText.setFont(font);
    highscoreText.setFont(font);
    hintText.setFont(font);

    sf::String title("Tetris Battle");
    sf::RenderWindow window(sf::VideoMode(windowWidth + (windowWidth / 2), windowHeight), title);
    ImGui::SFML::Init(window);
    ImGui::StyleColorsDark();

    sf::Clock clock, gameClock;
    time_t total = 57600;
    struct tm timeinfo;
    float onetime = 0.f, gametime = 0.f;
    char GameTime[10] = { 0 };
    unsigned int count = 0;
    bool isLost = false;

    while (window.isOpen()) {
        onetime = gameClock.getElapsedTime().asSeconds();
        gameClock.restart();
        gametime += onetime;
        if (gametime > 1) {
            total += 1;
            gametime = 0;
        }
        localtime_s(&timeinfo, &total);
        sprintf_s(GameTime, sizeof(GameTime), "%02u : %02u", timeinfo.tm_min, timeinfo.tm_sec);
        currentShape = shapes[currentType][0];
        currentSprite = sprites[currentType];
        enum class Action {
            Hold,
            MoveLeft,
            MoveRight,
            MoveDown,
            Rotate,
        };
        Action action = Action::Hold;
        sf::Event evt;
        if (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
            }
            if (evt.type == sf::Event::KeyPressed) {
                switch (evt.key.code) {
                case sf::Keyboard::Left:
                    action = Action::MoveLeft;
                    break;
                case sf::Keyboard::Right:
                    action = Action::MoveRight;
                    break;
                case sf::Keyboard::Down:
                    action = Action::MoveDown;
                    break;
                case sf::Keyboard::Up:
                    action = Action::Rotate;
                    break;
                case sf::Keyboard::P:
                case sf::Keyboard::Pause:
                    if (gameState == GameState::RUNNING) {
                        gameState = GameState::PAUSE;
                    }
                    else if (gameState == GameState::PAUSE) {
                        gameState = GameState::RUNNING;
                    }
                    break;
                }
            }
        }

        if (clock.getElapsedTime().asSeconds() >= 0.3f) {
            action = Action::MoveDown;
            clock.restart();
        }

        // check next position
        sf::Vector2i nextPos(pos);
        int nextIndex = currentIndex;
        if (action == Action::Rotate) {
            // 0 => 1 => 0 => 1 ...
            nextIndex = (nextIndex + 1) % shapes[currentType].size();
        }
        std::vector<sf::Vector2i> nextShape = shapes[currentType][nextIndex];
        // check the block position this round
        switch (action) {
        case Action::Hold:
            break;
        case Action::MoveLeft:
            // move left
            nextPos.x--;
            break;
        case Action::MoveRight:
            // move right
            nextPos.x++;
            break;
        case Action::MoveDown:
            nextPos.y++;
            break;
        }

        int countEmpty = 0;
        for (const sf::Vector2i d : nextShape) {
            sf::Vector2i np = nextPos + d;
            if (np.x >= 0 && np.x < fieldWidth &&
                np.y < fieldHeight &&
                (np.y < 0 || field[np.x][np.y] == Type::None)) {
                countEmpty++;
            }
        }

        // Check next position
        if (countEmpty == 4) {
            pos = nextPos;
            currentIndex = nextIndex;
            currentShape = nextShape;
        } else {  // Make blocks stackable at the bottom
            if (action == Action::MoveDown) {
                for (const sf::Vector2i& d : nextShape) {
                    sf::Vector2i np = pos + d;
                    if (np.x >= 0 && np.x < fieldWidth &&
                        np.y >= 0 && np.y < fieldHeight) {
                        field[np.x][np.y] = currentType;  // register
                    }
                }
                
                for (int y = 0; y < fieldHeight; y++) {
                    bool isFull = true;  // check the line if full
                    for (int x = 0; x < fieldWidth; x++) {
                        if (field[x][y] == Type::None) {
                            isFull = false;
                        }
                    }
                    if (isFull) {
                        for (int ty = y; ty > 0; ty--) {
                            for (int x = 0; x < fieldWidth; x++) {
                                field[x][ty] = field[x][ty - 1];
                            }
                        }
                        for (int x = 0; x < fieldWidth; x++) {
                            field[x][0] = Type::None;
                            count++;
                        }
                    }
                }
                pos = orign;  // back to the start
                currentType = Type(rand() % 7 + 1);
                currentIndex = 0;
            }
        }

        for (int x = 0; x < fieldWidth; x++) {
            if (field[x][0] == Type::None) continue;
            isLost = true;
        }

        unsigned int score = count / 10;
        switch (gameState) {
        case GameState::RUNNING:
            warnText.setString("");
            highscoreText.setString("High Score: " + std::to_string(high_score));
            highscoreText.setCharacterSize(24);
            highscoreText.setFillColor(sf::Color(255, 255, 224));
            highscoreText.setPosition(windowWidth + 10.f, 70.f);
            text.setString("Score: " + std::to_string(score));
            text.setCharacterSize(24);
            text.setFillColor(sf::Color(255, 255, 0));
            text.setPosition(windowWidth + 10.f, 120.f);
            timeText.setString("Time: " + std::string(GameTime));
            timeText.setCharacterSize(24);
            timeText.setFillColor(sf::Color(255, 255, 255));
            timeText.setPosition(windowWidth + 10.f, 20.f);
            break;
        case GameState::PAUSE:
            warnText.setString("");
            gametime = 0;
            clock.restart();
            break;
        case GameState::LOSE:
            isLost = true;
            warnText.setString("Game Over!");
            warnText.setCharacterSize(30);
            warnText.setStyle(sf::Text::Bold);
            warnText.setFillColor(sf::Color(255, 0, 0));
            warnText.setPosition(20.f, 20.f);
            break;
        }

        if (isLost) {
            if (score >= high_score) {
                high_score = score;
                highScoreFile.open(fileName, std::fstream::in | std::fstream::out);
                highScoreFile << high_score;
                highScoreFile.close();
            }
            gameState = GameState::LOSE;
            if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Escape) {  // quit game
                window.close();
            }
        }

        window.clear();
        // draw background
        window.draw(backgroundSprite);
        // draw the background field
        for (int x = 0; x < fieldWidth; x++) {
            for (int y = 0; y < fieldHeight; y++) {
                if (field[x][y] == Type::None) continue;

                sf::Sprite& drawSprite = sprites[field[x][y]];

                drawSprite.setPosition(
                    float(x* blockWidth),
                    float(y* blockHeight));
                window.draw(drawSprite);
            }
        }
        // draw control object
        // O: OO (0,-1) (1,-1)
        //    OO (0, 0)=>pos (1, 0)
        for (const sf::Vector2i& d : currentShape) {
            sf::Vector2i np = pos + d;
            currentSprite.setPosition(
                float(np.x * blockWidth),
                float(np.y * blockHeight));
            window.draw(currentSprite);
        }
        hintText.setString("Up key: Rotate block.\nLeft key: Move left.\nRight key: Move right.\nDown key: block \naccelerates drop.\nP key: Pause game.\nEsc: Quit (game over).\n");
        hintText.setCharacterSize(18);
        hintText.setFillColor(sf::Color(173, 216, 230));
        hintText.setPosition(windowWidth + 10.f, windowHeight - hintText.getGlobalBounds().height);
        window.draw(text);
        window.draw(highscoreText);
        window.draw(timeText);
        window.draw(warnText);
        window.draw(hintText);
        window.display();
    }
    return EXIT_SUCCESS;
}