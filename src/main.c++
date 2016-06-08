/*!*****************************************************************************
* \brief Jamper Game
* \author Vitaliy Zinchenko <zinc.vitaliy@gmail.com>
* \date 14.04.2016
* \copyright LGPLv3.
*******************************************************************************/

#include <SFML/Graphics.hpp>
#include <vector>

//------------------------------------------------------------------------------
int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 800),
							"BlueJump",
							sf::Style::Close | sf::Style::Titlebar
							);

    sf::Texture menLeft;
    menLeft.loadFromFile("Images/men_left.png");
    menLeft.setSmooth(true);

    sf::Texture menRight;
    menRight.loadFromFile("Images/men_right.png");
    menRight.setSmooth(true);

    // Character setup
    sf::Sprite character;
    character.setTexture(menRight);
    character.setOrigin(44, 99);
    character.setPosition(320, 600);

    float characterRun = .0f;
    float characterGravity = .0f;

    bool isPressA = false;
    bool isPressD = false;

    // Platforms setup
    sf::RectangleShape plat(sf::Vector2f(100, 30));
    plat.setFillColor(sf::Color(253, 166, 165));
    plat.setPosition(270, 700);

    std::vector<sf::RectangleShape> platforms;
    platforms.push_back(plat);

    for (auto i = 1; i < 6; ++i) {
        plat.setPosition(rand() % 540, 700 - (i * 150));
        platforms.push_back(plat);
    }

    // Spring
    sf::RectangleShape spring(sf::Vector2f(30, 20));
    spring.setFillColor(sf::Color(127, 127, 127));
    spring.setPosition(platforms[1].getPosition().x + 20,
                        platforms[1].getPosition().y - 20);


    // Score
    sf::Font font;
    font.loadFromFile("Fonts/Anonymous Pro.ttf");

    sf::Text textScore;
    textScore.setFont(font);
    textScore.setString("Score: 0");
    textScore.setPosition(10, 10);
    textScore.setCharacterSize(36);
    textScore.setColor(sf::Color::White);
    int score = 0;

    // Game over
    sf::Text gameOver;
    gameOver.setFont(font);
    gameOver.setString("GAME OVER");
    gameOver.setPosition(175, 350);
    gameOver.setCharacterSize(64);
    gameOver.setColor(sf::Color::White);

    // Timer
    sf::Clock clock;

    bool isRun = true;


    // Start events
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    character.setTexture(menLeft);
                    isPressA = true;
                }

                if (event.key.code == sf::Keyboard::D) {
                    character.setTexture(menRight);
                    isPressD = true;
                }

                if (event.key.code == sf::Keyboard::Return) {
                    score = 0;
                    // Platforms setup
                    plat.setPosition(270, 700);

                    platforms.clear();
                    platforms.push_back(plat);

                    for (auto i = 1; i < 6; ++i) {
                        plat.setPosition(rand() % 540, 700 - (i * 150));
                        platforms.push_back(plat);
                    }

                    // Spring
                    spring.setPosition(platforms[1].getPosition().x + 20,
                                        platforms[1].getPosition().y - 20);

                    // Character
                    character.setPosition(320, 600);

                    characterRun = .0f;
                    characterGravity = .0f;
                    textScore.setString("Score: 0");

                    isRun = true;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::A) {
                    isPressA = false;
                }

                if (event.key.code == sf::Keyboard::D) {
                    isPressD = false;
                }
            }


        }

        if (isRun) {
	        // Logic
	        if (clock.getElapsedTime().asMilliseconds() > 5) {
	            character.move(characterRun, 0);

	            if (character.getPosition().y > 300 || characterGravity > 0) {
	                character.move(0, characterGravity);
	            }
	            else {
	                for (auto& platform : platforms) {
	                    platform.move(0, -characterGravity);
	                }
	                spring.move(0, -characterGravity);
	                score += 1;
	                textScore.setString("Score: " + std::to_string(score));
	            }



	            if (isPressA) characterRun -= 0.1f;
	            if (isPressD) characterRun += 0.1f;

	            if (characterRun > 0.0001f) { characterRun -= 0.01f; }
	            else if (characterRun < -0.0001f) { characterRun += 0.01f; }
	            else { characterRun = 0.0f; }

	            characterGravity += 0.1f;

	            sf::Vector2f charLeftPos = character.getPosition();
	            charLeftPos.x -= 44;
	            sf::Vector2f charRightPos = character.getPosition();
	            charRightPos.x += 44;

	            // Platform jump
	            for (auto& platform : platforms) {
	                if (platform.getGlobalBounds().contains(charLeftPos)) {
	                    if (characterGravity > 0) characterGravity = -9;
	                    break;
	                }
	                else if (platform.getGlobalBounds().contains(charRightPos)) {
	                    if (characterGravity > 0) characterGravity = -9;
	                    break;
	                }
	            }

	            // Spring jump
	            if (spring.getGlobalBounds().contains(character.getPosition())) {
	                if (characterGravity > 0) characterGravity = -20;
	            }

	            // Update platforms
	            for (auto& platform : platforms) {
	                if (platform.getPosition().y > 800) {
	                    platform.setPosition(rand() % 540, platform.getPosition().y);
	                    platform.move(0, -1100);
	                }
	            }

	            if (spring.getPosition().y > 800) {
	                spring.setPosition(platforms[1].getPosition().x + 20,
	                                    platforms[1].getPosition().y - 20);
	            }

	            // Left
	            if (character.getPosition().x < -50) { character.move(690, 0); }
	            else if (character.getPosition().x > 690) { character.move(-690, 0); }

	            // Down
	            if (character.getPosition().y > 850) { isRun = false; }

	            clock.restart();
	        }
        }

        // Drawing
        window.clear(sf::Color(104, 184, 193));

        for (auto& platform : platforms) {
            window.draw(platform);
        }

        window.draw(spring);
        window.draw(character);
        window.draw(textScore);
        if (!isRun) window.draw(gameOver);

        window.display();
    }

    return 0;
}
