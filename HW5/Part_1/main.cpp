#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "EventManager.h"   

using namespace sf;

void movePlatformWrapper(GameThread *gt, FloatingPlatform *platform, int movementSpeed)
{
    gt->movePlatform(platform, movementSpeed);
};

void inputWrapper(GameThread *gt, InputSubsystem *inputs, int movementSpeed, bool hasFocus)
{
    gt->checkInputs(hasFocus, inputs, movementSpeed);
};

int main()
{
    int origin = 0;
    GameObject game = GameObject();

    //Create game window
    sf::RenderWindow gameWindow;
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_WIDTH = 800;
    int TICSIZE = 4;
    int playerSpeed = 1;

    gameWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Game", sf::Style::Close);
    gameWindow.setPosition(sf::Vector2i(50, 50));
    gameWindow.setFramerateLimit(120);

    //Create player character
    sf::Texture texture;
    if (!texture.loadFromFile("image.png")) {
        return 1;
    }
    texture.setSmooth(true);
    sf::Vector2f startPosition = sf::Vector2f(100.f, 515.f);
    game.newPlayer(50, &texture, sf::Color(255, 255, 255), startPosition);    

    //Create Timelines
    int mainTimelineIDX = game.newTimeline(NULL, TICSIZE);
    int64_t last_time = game.getTimelines()->at(mainTimelineIDX)->getTime();
    int platTimelineIDX = game.newTimeline(game.getTimelines()->at(mainTimelineIDX), TICSIZE);

    //Create Event manager
    EventManager eManager(game.getTimelines()->at(mainTimelineIDX));

    //Create static platforms and death zones in between them
    int staticPlat1 = game.newStaticPlatform(sf::Vector2f(300, 75), sf::Color(100, 50, 0), 0, 525);
    int dz1Start = game.getStaticPlatforms()->at(staticPlat1)->getLocalBounds().width;
    int deathZone1IDX = game.newDeathZone(sf::Vector2f(200, 50), dz1Start, 600, game.getTimelines()->at(mainTimelineIDX), &eManager);

    int plat2Start = dz1Start + game.getDeathZones()->at(deathZone1IDX)->getLocalBounds().width;
    int staticPlat2 = game.newStaticPlatform(sf::Vector2f(400, 50), sf::Color(50, 200, 75), plat2Start, 550);
    int dz2Start = dz1Start + game.getStaticPlatforms()->at(staticPlat2)->getLocalBounds().width;
    int deathZone2IDX = game.newDeathZone(sf::Vector2f(600, 50), dz2Start, 600, game.getTimelines()->at(mainTimelineIDX), &eManager);
    
    int plat3Start = dz2Start + game.getDeathZones()->at(deathZone2IDX)->getLocalBounds().width;
    int staticPlat3 = game.newStaticPlatform(sf::Vector2f(700, 150), sf::Color(110, 102, 80), plat3Start, 450);

    //Set end location for side-scrolling
    int end = plat3Start +  game.getStaticPlatforms()->back()->getLocalBounds().width - 10;

    //Create floating platforms
    int floatingPlat = game.newFloatingPlatform(sf::Vector2f(150, 25), sf::Color(102, 0, 204), 600, 550, 300, true, false, game.getTimelines()->at(mainTimelineIDX));
    int floatingPlat2 = game.newFloatingPlatform(sf::Vector2f(200, 25), sf::Color(204, 0, 102), 1050, 300, 300, false, true, game.getTimelines()->at(mainTimelineIDX));

    //Create spwan point and window side boundaries
    int spawnIDX = game.newSpawnPoint(sf::Vector2f(0, 0), startPosition.x, startPosition.y);
    int sideBoundary1IDX = game.newSideBoundary(sf::Vector2f(50, 600), WINDOW_WIDTH - (WINDOW_WIDTH / 3), 0);
    int sideBoundary2IDX = game.newSideBoundary(sf::Vector2f(50, 600), 150, 0);

    bool hasFocus = true;

    //Subscribe all event types in Event Manager
    eManager.subscribe("PlayerMovement", game.getPlayer());
    eManager.subscribe("PlayerLeft", game.getPlayer());
    eManager.subscribe("PlayerRight", game.getPlayer());
    eManager.subscribe("PlayerJump", game.getPlayer());

    for (int i = 0; i < game.getDeathZones()->size(); i++) {
        eManager.subscribe("PlayerDeath", game.getDeathZones()->at(i));
    }

    for (int i = 0; i < game.getSpawnPoints()->size(); i++) {
        eManager.subscribe("PlayerSpawn", game.getSpawnPoints()->at(i));
    }

    for (int i = 0; i < game.getSideBoundaries()->size(); i++) {
        eManager.subscribe("Scroll", game.getSideBoundaries()->at(i));
    }

    for (int i = 0; i < game.getStaticPlatforms()->size(); i++) {
        eManager.subscribe("StaticPlatCollision", game.getStaticPlatforms()->at(i));
    }

    for (int i = 0; i < game.getFloatingPlatforms()->size(); i++) {
        eManager.subscribe("FloatingPlatCollision", game.getFloatingPlatforms()->at(i));
    }

    //Create input subsystem
    game.newInputSubsystem(game.getPlayer(), game.getTimelines()->at(mainTimelineIDX), &eManager);

    //Game Loop
    while(gameWindow.isOpen())
    {
        int64_t current_time = game.getTimelines()->at(mainTimelineIDX)->getTime();
        int64_t frame_delta;
        if (game.getTimelines()->at(mainTimelineIDX)->isPaused()) {
            frame_delta = 0;
        } else {
            frame_delta = current_time - last_time;
        }
        if (frame_delta < 0  || frame_delta > 100){
            frame_delta = 0;
        }
        last_time = current_time;

        sf::Event event;
        while(gameWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                break;
            } else if (event.type == sf::Event::GainedFocus) {
                hasFocus = true;
            } else if (event.type == sf::Event::LostFocus) {
                hasFocus = false;
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    if (game.getTimelines()->at(mainTimelineIDX)->isPaused()) {
                        game.getTimelines()->at(mainTimelineIDX)->unpause();
                    } else if (!game.getTimelines()->at(mainTimelineIDX)->isPaused()) {
                        game.getTimelines()->at(mainTimelineIDX)->pause();
                    } 
                }
            }
        }


        //Check if closing
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) ) {
            gameWindow.close();
            break;
        }

        int sideBoundarySpeed = 1;
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
            sideBoundarySpeed = 2;
        }
        if (hasFocus && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
            sideBoundarySpeed = 1;
        }
    
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
            playerSpeed = 2;
        }
        if (hasFocus && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ) {
            playerSpeed = 1;
        }
    
    
        float movementSpeed = frame_delta;
    
    
        //Check for user input
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            // GameEvent *e = new GameEvent("PlayerMovement", game.getTimelines()->at(mainTimelineIDX), 0);
            GameEvent *e = new GameEvent("PlayerLeft", game.getTimelines()->at(mainTimelineIDX), 0);

            variant *param = new variant;
            param->m_Type = param->TYPE_GAMEITEM;
            param->m_asGameItem = game.getPlayer();
            e->add_parameter("Character", param);

            variant *param2 = new variant;
            param2->m_Type = param2->TYPE_FLOAT;
            param2->m_asFloat = movementSpeed * playerSpeed;
            e->add_parameter("Left", param2);

            eManager.raise(e);
        }

        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            // GameEvent *e = new GameEvent("PlayerMovement", game.getTimelines()->at(mainTimelineIDX), 0);
            GameEvent *e = new GameEvent("PlayerRight", game.getTimelines()->at(mainTimelineIDX), 0);

            variant *param = new variant;
            param->m_Type = param->TYPE_GAMEITEM;
            param->m_asGameItem = game.getPlayer();
            e->add_parameter("Character", param);

            variant *param2 = new variant;
            param2->m_Type = param2->TYPE_FLOAT;
            param2->m_asFloat = movementSpeed * playerSpeed;
            e->add_parameter("Right", param2);

            eManager.raise(e);
        }
        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            // GameEvent *e = new GameEvent("PlayerMovement", game.getTimelines()->at(mainTimelineIDX), 0);
            GameEvent *e = new GameEvent("PlayerJump", game.getTimelines()->at(mainTimelineIDX), 0);

            variant *param = new variant;
            param->m_Type = param->TYPE_GAMEITEM;
            param->m_asGameItem = game.getPlayer();
            e->add_parameter("Character", param);
        
            variant *param2 = new variant;
            param2->m_Type = param2->TYPE_FLOAT;
            param2->m_asFloat = 120;
            e->add_parameter("Jump", param2);

            eManager.raise(e);
        }

        if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
                game.getTimelines()->at(mainTimelineIDX)->changeTic(TICSIZE * 2);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
                game.getTimelines()->at(mainTimelineIDX)->changeTic(TICSIZE);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
                game.getTimelines()->at(mainTimelineIDX)->changeTic(TICSIZE / 2);
            }
        }

        std::mutex m2;
        std::condition_variable cv2;

        //Other thread handles input, current thread will move the platforms
        for (int i = 0; i < game.getFloatingPlatforms()->size(); i++) {
            game.getFloatingPlatforms()->at(i)->move(&m2, &cv2, frame_delta);
        }

        //Join thread back together
        // first.join();         

        //Check left side of screen
        if (game.getPlayer()->getPosition().x < 0) {
            game.getPlayer()->setPosition(2, game.getPlayer()->getPosition().y);
        }

        //Check right side of screen
        if (game.getPlayer()->getPosition().x + game.getPlayer()->getGlobalBounds().width > WINDOW_WIDTH) {
            game.getPlayer()->setPosition(WINDOW_WIDTH - game.getPlayer()->getGlobalBounds().width, game.getPlayer()->getPosition().y);
        }

        //Apply gravity if needed
        game.getPlayer()->gravity(frame_delta);

        //Check for collision with platforms
        for (int i = 0; i < game.getStaticPlatforms()->size(); i++) {
            float playerHeight = game.getPlayer()->getGlobalBounds().height;
            int playerY = game.getPlayer()->getPosition().y;
            int playerX = game.getPlayer()->getPosition().x;
            
            if (game.getPlayer()->getGlobalBounds().intersects(game.getStaticPlatforms()->at(i)->getGlobalBounds())) {
                int staticY = game.getStaticPlatforms()->at(i)->getPosition().y;
                if (playerY + playerHeight >= staticY) {
                    GameEvent *e = new GameEvent("StaticPlatCollision", game.getTimelines()->at(mainTimelineIDX), 1);
                
                    variant *param = new variant;
                    param->m_Type = param->TYPE_GAMEITEM;
                    param->m_asGameItem = game.getPlayer();
                    e->add_parameter("Character", param);
                
                    variant *param2 = new variant;
                    param2->m_Type = param2->TYPE_GAMEITEM;
                    param2->m_asGameItem = game.getStaticPlatforms()->at(i);
                    e->add_parameter("Platform", param2);

                    eManager.raise(e);
                }
            }
        }

        //Check for collision with floating platforms
        for (int i = 0; i < game.getFloatingPlatforms()->size(); i++) {
            if (game.getPlayer()->getGlobalBounds().intersects(game.getFloatingPlatforms()->at(i)->getGlobalBounds())) {
                GameEvent *e = new GameEvent("FloatingPlatCollision", game.getTimelines()->at(mainTimelineIDX), 1);

                variant *param = new variant;
                param->m_Type = param->TYPE_GAMEITEM;
                param->m_asGameItem = game.getPlayer();
                e->add_parameter("Character", param);

                variant *param2 = new variant;
                param2->m_Type = param2->TYPE_GAMEITEM;
                param2->m_asGameItem = game.getFloatingPlatforms()->at(i);
                e->add_parameter("Platform", param2);

                variant *param3 = new variant;
                param3->m_Type = param3->TYPE_FLOAT;
                param3->m_asFloat = frame_delta;
                e->add_parameter("Speed", param3);

                eManager.raise(e);

            }
        }

        //Player walking right, shift objects
        if (origin < end - WINDOW_WIDTH && game.getPlayer()->getPosition().x > game.getSideBoundaries()->at(sideBoundary1IDX)->getPosition().x) {
            game.getPlayer()->setPosition(sf::Vector2f(game.getSideBoundaries()->at(sideBoundary1IDX)->getPosition().x, game.getPlayer()->getPosition().y));
            origin += frame_delta * sideBoundarySpeed;
            for (int i = 0; i < game.getStaticPlatforms()->size(); i++) {
                game.getStaticPlatforms()->at(i)->shiftRight(frame_delta * sideBoundarySpeed);
            }
            for (int i = 0; i < game.getFloatingPlatforms()->size(); i++) {
                game.getFloatingPlatforms()->at(i)->shiftRight(frame_delta * sideBoundarySpeed);
            }
            for (int i = 0; i < game.getDeathZones()->size(); i++) {
                game.getDeathZones()->at(i)->shiftRight(frame_delta * sideBoundarySpeed);
            }
        }
        //Player walking left, shift objects
        if (origin > 0 && game.getPlayer()->getPosition().x < game.getSideBoundaries()->at(sideBoundary2IDX)->getPosition().x) {
            game.getPlayer()->setPosition(sf::Vector2f(game.getSideBoundaries()->at(sideBoundary2IDX)->getPosition().x, game.getPlayer()->getPosition().y));
            origin -= frame_delta * sideBoundarySpeed;
            for (int i = 0; i < game.getFloatingPlatforms()->size(); i++) {
                game.getFloatingPlatforms()->at(i)->shiftLeft(frame_delta * sideBoundarySpeed);
            }
            for (int i = 0; i < game.getStaticPlatforms()->size(); i++) {
                game.getStaticPlatforms()->at(i)->shiftLeft(frame_delta * sideBoundarySpeed);
            }
            for (int i = 0; i < game.getDeathZones()->size(); i++) {
                game.getDeathZones()->at(i)->shiftLeft(frame_delta * sideBoundarySpeed);
            }
        }

        //Check for player death zone collision
        bool playerDeath = false;
        for (int i = 0; i < game.getDeathZones()->size(); i++) {
            if (game.getDeathZones()->at(i)->getGlobalBounds().intersects(game.getPlayer()->getGlobalBounds())) {                
                GameEvent *e = new GameEvent("PlayerDeath", game.getTimelines()->at(mainTimelineIDX), 0);

                variant *param = new variant;
                param->m_Type = param->TYPE_GAMEITEM;
                param->m_asGameItem = game.getPlayer();
                e->add_parameter("Character", param);

                variant *param2 = new variant;
                param2->m_Type = param2->TYPE_GAMEITEM;
                param2->m_asGameItem = game.getSpawnPoints()->at(spawnIDX);
                e->add_parameter("SpawnPoint", param2);

                variant *param3 = new variant;
                param3->m_Type = param3->TYPE_GAMEITEM;
                param3->m_asGameItem = game.getDeathZones()->at(i);
                e->add_parameter("Deathzone", param3);

                eManager.raise(e);
                playerDeath = true;                
            }
        }

        //If player died, reset objects to origin
        if (playerDeath) {
            while (origin > 0) {
                origin -= frame_delta;
                for (int i = 0; i < game.getFloatingPlatforms()->size(); i++) {
                    game.getFloatingPlatforms()->at(i)->shiftLeft(frame_delta);
                }
                for (int i = 0; i < game.getStaticPlatforms()->size(); i++) {
                    game.getStaticPlatforms()->at(i)->shiftLeft(frame_delta);
                }
                for (int i = 0; i < game.getDeathZones()->size(); i++) {
                    game.getDeathZones()->at(i)->shiftLeft(frame_delta);
                }
            }
        }

        //Handle all events in the order of their priority

        while (eManager.priorityQueue->size() > 0 && eManager.priorityQueue->top().first >= game.getTimelines()->at(mainTimelineIDX)->getTime()) {
            // std::cout << "Game Time: " << game.getTimelines()->at(mainTimelineIDX)->getTime() << " Event Time: " << eManager.priorityQueue->top().first << "\n";
            GameEvent *e = eManager.priorityQueue->top().second;
            eManager.handle(e);
            eManager.priorityQueue->pop();
        }

        //Clear the game window and render objects
        gameWindow.clear(sf::Color(117, 206, 239));
        for (int i = 0; i < game.getStaticPlatforms()->size(); i++) {
            gameWindow.draw(*game.getStaticPlatforms()->at(i)); 
        }
        for (int i = 0; i < game.getFloatingPlatforms()->size(); i++) {
            gameWindow.draw(*game.getFloatingPlatforms()->at(i));
        }
        for (int i = 0; i < game.getCharacters()->size(); i++) {
            gameWindow.draw(*game.getCharacters()->at(i));
        }
        gameWindow.draw(*game.getPlayer());      
        gameWindow.display();
    }   

    return 0;
}