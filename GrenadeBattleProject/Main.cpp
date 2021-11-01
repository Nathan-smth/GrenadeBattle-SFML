#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle!");




    //load texture 
    sf::Texture player1Texture;
    player1Texture.loadFromFile("Assets/Graphics/Player_1.png");

    sf::Texture player2Texture;
    player2Texture.loadFromFile("Assets/Graphics/Player_2.png");


    //setup sprite 
    sf::Sprite player1Sprite;
    player1Sprite.setTexture(player1Texture);
    player1Sprite.setScale(1.5f, 1.5f);
    
   

    sf::Sprite player2Sprite;
    player2Sprite.setTexture(player2Texture);
    player2Sprite.setScale(1.5f, 1.5f);


    sf::Vector2f spriteSize;
    spriteSize.x = player1Texture.getSize().x;
    spriteSize.y = player1Texture.getSize().y;

    //setup position 
    sf::Vector2f player1Position = sf::Vector2f(100.0f, 500.0f);
    player1Sprite.setPosition(player1Position);
    sf::Vector2f playerPositionPrev = player1Position;

    sf::Vector2f player2Position = sf::Vector2f(1500.0f, 500.0f);
    player2Sprite.setPosition(player2Position);


    // Bounding circles
    float playerCircleRadius = player1Texture.getSize().x / 2;
    sf::Vector2f playerCircleCentre = player1Position + 0.5f * spriteSize;
    sf::CircleShape playerCircle;
    playerCircle.setRadius(playerCircleRadius);
    playerCircle.setPosition(player1Position);
    playerCircle.setFillColor(sf::Color::Green);


    float enemyCircleRadius = player2Texture.getSize().x / 2;
    sf::Vector2f enemyCircleCentre = player2Position + 0.5f * spriteSize;
    sf::CircleShape enemyCircle;
    enemyCircle.setRadius(enemyCircleRadius);
    enemyCircle.setPosition(player2Position);
    enemyCircle.setFillColor(sf::Color::Green);

    //bounding boxes 

    sf::Vector2f player1BoundsOffset(3, 0);
    sf::Vector2f player1BoundsScale(0.75f, 1);

    sf::Vector2f player2BoundsOffset(-7, -7);
    sf::Vector2f player2BoundsScale(1.5f, 1.5f);

    // Visual part of bounding boxes
   // No need to set these up as they are updated each frame
    sf::RectangleShape playerAABBDisplay;
    sf::RectangleShape player2AABBDisplay;

    //setup velocity 
    sf::Vector2f playerVelocity = sf::Vector2f(0.0f, 0.0f);
    float playerSpeed = 1000.0f;
    float accelerationRate = 2000.0;
    float drag = 0.003f;
    float jumpSpeed = 100.0f;
    float gravity = 500.0f;
    sf::Vector2f playerAcceleration = sf::Vector2f(0.0f, 0.0f);
    playerAcceleration.y = gravity;


    //Gravity stuff 
   

    const float BOTGAP = 125;

    //set up game clock 
    sf::Clock gameClock;

    while (window.isOpen())
    {
        //handle the time stuff 
        sf::Time frameTime = gameClock.restart();
        float deltaTime = frameTime.asSeconds();





        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Is the event a key press event
            if (event.type == sf::Event::KeyPressed)
            {
                // Is the event specifically related to the escape key
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }


        //set up player movement
        playerAcceleration.x = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            //left key is pressed: move our character 
            playerAcceleration.x = -accelerationRate;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            //Right key is pressed: move our character 
            playerAcceleration.x = accelerationRate;
        }


        //update
        if (player1Position.y >= sf::VideoMode::getDesktopMode().height - player1Texture.getSize().y- BOTGAP)
        {
            playerAcceleration.y = 0;
            playerVelocity.y = 0;
            player1Position.y = sf::VideoMode::getDesktopMode().height - player1Texture.getSize().y- BOTGAP;
        }
        else
        {
            //doing gravity and stuff
            playerAcceleration.y = gravity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {         
            playerVelocity.y = -jumpSpeed;
        }
    

        sf::Vector2f deltaVelocity = playerAcceleration * deltaTime;

        playerVelocity = playerVelocity + deltaVelocity;

        //Change in velocity due to drag 
        playerVelocity.x = playerVelocity.x - playerVelocity.x * drag;

        playerPositionPrev = player1Position;
        sf::Vector2f deltaPosition = playerVelocity * deltaTime;

        
        player1Position = player1Position + deltaPosition;




        

       

        //update player position
        player1Sprite.setPosition(player1Position);
        // Update collision geometry
            playerCircleCentre = player1Position + 0.5f * spriteSize;

        // Get current position and size from sprites
        sf::FloatRect playerAABB = player1Sprite.getGlobalBounds();
        sf::FloatRect player2AABB = player2Sprite.getGlobalBounds();

        // Update this geometry using our custom offsets and scales
        playerAABB.left += player1BoundsOffset.x;
        playerAABB.top += player1BoundsOffset.y;
        playerAABB.width *= player1BoundsScale.x;
        playerAABB.height *= player1BoundsScale.y;

        player2AABB.left += player2BoundsOffset.x;
        player2AABB.top += player2BoundsOffset.y;
        player2AABB.width *= player2BoundsScale.x;
        player2AABB.height *= player2BoundsScale.y;

        // Update collision visuals
        playerCircle.setPosition(player1Position);

        playerAABBDisplay.setPosition(playerAABB.left, playerAABB.top);
        player2AABBDisplay.setPosition(player2AABB.left, player2AABB.top);

        playerAABBDisplay.setSize(sf::Vector2f(playerAABB.width, playerAABB.height));
        player2AABBDisplay.setSize(sf::Vector2f(player2AABB.width, player2AABB.height));


        bool colliding = playerAABB.intersects(player2AABB);



        if (colliding)
        {
            sf::Color fillcolor = sf::Color::Red;
            fillcolor.a = 125;

            playerCircle.setFillColor(fillcolor);
            enemyCircle.setFillColor(fillcolor);

            playerAABBDisplay.setFillColor(fillcolor);
            player2AABBDisplay.setFillColor(fillcolor);

            // Collision Depth
            sf::Vector2f AWidthHeight = sf::Vector2f(playerAABB.width, playerAABB.height);
            sf::Vector2f BWidthHeight = sf::Vector2f(player2AABB.width, player2AABB.height);

            sf::Vector2f ACentre = sf::Vector2f(playerAABB.left, playerAABB.top);
            ACentre += 0.5f * AWidthHeight;
            sf::Vector2f BCentre = sf::Vector2f(player2AABB.left, player2AABB.top);
            BCentre += 0.5f * BWidthHeight;

            sf::Vector2f distance = BCentre - ACentre;
            sf::Vector2f minDistance = 0.5f * AWidthHeight + 0.5f * BWidthHeight;
            if (distance.x < 0)
                minDistance.x = -minDistance.x;
            if (distance.y < 0)
                minDistance.y = -minDistance.y;

            sf::Vector2f depth = minDistance - distance;
            sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));

            if (absDepth.x < absDepth.y)
            {
                // Move along x direction
                player1Position.x -= depth.x;

                // Stop movement in x direction
                playerVelocity.x = 0;
            }
            else
            {
                // Move along y direction
                player1Position.y -= depth.y;

                // Stop movement in y direction
                playerVelocity.y = 0;
            }
            player1Sprite.setPosition(player1Position);
        }
        else
        {
            sf::Color fillcolor = sf::Color::Green;
            fillcolor.a = 125;

            playerCircle.setFillColor(fillcolor);
            enemyCircle.setFillColor(fillcolor);

            playerAABBDisplay.setFillColor(fillcolor);
            player2AABBDisplay.setFillColor(fillcolor);
        }

        window.clear();


        //draw
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.draw(playerAABBDisplay);
        window.draw(player2AABBDisplay);


        window.display();
    }

    return 0;
}