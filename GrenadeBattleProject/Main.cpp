#include <SFML/Graphics.hpp>
#include"VectorHelper.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle!");




    ///
    ///
    ///pips set up
    ///
    ///
    

    std::vector  <sf::Sprite> pips;
    sf::Texture pipTexture;
    pipTexture.loadFromFile("Assets/Graphics/pip.png");

    sf::Vector2f pipgravity(0,740.0f);
    sf::Vector2f firingPosition(300.0f, 300.0f);
    sf::Vector2f firingDirection(1.0f,0); 
    float firingSpeed = 750.0f;

    const int NUM_PIPS = 10;
    
    for (int i = 0;i < NUM_PIPS; ++i)
    {
        sf::Sprite newPip;
        newPip.setTexture(pipTexture);
        pips.push_back(newPip);
    }


    ///
    ///
    ///grenade set up
    ///
    /// 
   

    sf::Texture grenadeTexture;
    grenadeTexture.loadFromFile("Assets/Graphics/grenade.png");
    sf::Sprite grenadeSprite;
    grenadeSprite.setTexture(grenadeTexture);
    sf::Vector2f grenadeVelocity(0.0f, 0.0f);




    ///
    ///
    /// Crate set up 
    /// 
    /// 
    

    sf::Texture crateTexture;
    crateTexture.loadFromFile("Assets/Graphics/Block.png");
    const int NUM_CRATE = 30;
    std::vector<sf::Sprite> crateSprite;
    for (int i = 0;i < NUM_CRATE;++i)
    {
        crateSprite.push_back(sf::Sprite());
        crateSprite[i].setTexture(crateTexture);
        crateSprite[i].setPosition(i * 28, 500);
    }



    ///
    ///
    /// Easing function set up
    /// 
    /// 

    sf::Texture easingTexture;
    easingTexture.loadFromFile("Assets/Graphics/gun.png");
    sf::Sprite easingSprite;
    easingSprite.setTexture(easingTexture);
    sf::Vector2f easingPosition= sf::Vector2f(500.0f,700.0f);
    easingSprite.setPosition(easingPosition);


    sf::Vector2f begin = easingPosition; 
    sf::Vector2f end = easingPosition;
    sf::Vector2f change = end - begin;
    sf::Vector2f direction = sf::Vector2f(0, 0);
    float duration = 1.0f;
    float speed = 100.0f;
    float time = 0.0f;




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
    sf::Vector2f player1Position = sf::Vector2f(100.0f, 400.0f);
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

    sf::Vector2f deltaVelocity;
    sf::Vector2f deltaPosition;
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


        //update


        //add previous acceleration
        sf::Vector2f previousAcceleration = playerAcceleration;


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



        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            playerVelocity.y = -jumpSpeed;
        }













        //add previous velocity
        sf::Vector2f previousVelocity = playerVelocity;

        playerPositionPrev = player1Position;

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




    

     

        
     
      
        const int EXPLICIT_EULER=1;
        const int IMPLICIT_EULER = 2;
        const int SEMI_IMPLICIT_EULER = 3;
        const int VELOCITY_VERLET = 4;
        int physicsMode = 4;

   

        switch (physicsMode)
        {
            case EXPLICIT_EULER:
            //using explicet euler by using the previous fraims acceleration, velocity and position 

              deltaVelocity = previousAcceleration * deltaTime;

               playerVelocity = playerVelocity + deltaVelocity;

               //Change in velocity due to drag 
               playerVelocity.x = playerVelocity.x - playerVelocity.x * drag;


               deltaPosition = previousVelocity * deltaTime;
               player1Position = player1Position + deltaPosition;

                break;

            case IMPLICIT_EULER:

               deltaVelocity = playerAcceleration * deltaTime;

                playerVelocity = playerVelocity + deltaVelocity;

                //Change in velocity due to drag 
                playerVelocity.x = playerVelocity.x - playerVelocity.x * drag;


               deltaPosition = playerVelocity * deltaTime;
               player1Position = player1Position + deltaPosition;

                break;

            case SEMI_IMPLICIT_EULER:
            
                deltaVelocity = previousAcceleration * deltaTime;

                playerVelocity = playerVelocity + deltaVelocity;

                //Change in velocity due to drag 
                playerVelocity.x = playerVelocity.x - playerVelocity.x * drag;


                deltaPosition = playerVelocity * deltaTime;
                player1Position = player1Position + deltaPosition;
                break;


            case VELOCITY_VERLET:


                sf::Vector2f firstHalfVel = previousVelocity + previousAcceleration * (deltaTime * 0.5f);
                firstHalfVel.x = firstHalfVel.x - firstHalfVel.x * drag;

                player1Position += firstHalfVel * deltaTime;
                playerVelocity = firstHalfVel + playerAcceleration * (deltaTime * 0.5f);
                   

                playerVelocity = playerVelocity + deltaVelocity;

                   


                deltaPosition = playerVelocity * deltaTime;

               break;
        }
       



        ///
        ///
        /// pips update stuff
        /// 
        /// 

        sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        firingDirection = mousePosition - player1Position;
        
        //Normalising firingDirection to size 1 (unit vector
        float mag = VectorMagnitude(firingDirection);
        firingDirection.x /= mag;
        firingDirection.y /= mag;

        sf::Vector2f firingVelocity = firingDirection * firingSpeed;

        float pipTime = 0;
        for (int i = 0;i < pips.size();++i)
        {
            pipTime += 0.1;
            sf::Vector2f pipPostion; 
            pipPostion = player1Position + firingVelocity * pipTime + 0.5f *pipgravity* pipTime * pipTime;
            pips[i].setPosition(pipPostion);
        }




        ///
        ///
        /// Update grenade sttuff
        /// 
        /// 

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            grenadeSprite.setPosition(player1Position);
            grenadeVelocity = firingVelocity;
        }


        grenadeVelocity += pipgravity * frameTime.asSeconds();
        grenadeSprite.setPosition(grenadeSprite.getPosition() + grenadeVelocity * frameTime.asSeconds());


       ///
       ///
       /// Crate stuff :D
       /// 
       ///  

        //Check collision with the crates for rocochet
        for (int i = 0; i < crateSprite.size();++i)
        {
            sf::FloatRect crateRect = crateSprite[i].getGlobalBounds();
            sf::FloatRect grenadeRect = grenadeSprite.getGlobalBounds();
            sf::FloatRect playerRect = player1Sprite.getGlobalBounds();

            if (crateRect.intersects(grenadeRect))
            {
                //Ricochet :D

                // Find the side of collision using our collision depth 
                sf::Vector2f depth = CollisionDepth(grenadeRect, crateRect);
                sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));
                sf::Vector2f normal;

                if (absDepth.x < absDepth.y) //collided in the x direction 
                {
                    //Move out of the collision, fist 
                    sf::Vector2f grenadePos = grenadeSprite.getPosition();
                    grenadePos.x += depth.x;
                    grenadeSprite.setPosition(grenadePos);

                    // Are we colliding from the left or right?
                    if (depth.x > 0)//colliding form tthe left 
                    {
                        // Set the normal vector to point left 
                        normal = sf::Vector2f(-1, 0);
                    }
                    else //colliding from the right 
                    {
                        // Set the normal vector to point right 
                        normal = sf::Vector2f(1, 0);
                    }
                }
                else //colliding in th y direction
                {
                    //Move out of the collision fist of all 
                    sf::Vector2f grenadePos = grenadeSprite.getPosition();
                    grenadePos.y += depth.y;
                    grenadeSprite.setPosition(grenadePos);

                    //Are we colliding form top or botttom?

                    if (depth.y > 0)//colliding from top
                    {
                        normal = sf::Vector2f(0, -1);

                    }
                    else //colliding form bottom
                    {
                        normal = sf::Vector2f(0, 1);
                    }
                }

                //Apply the reflection equation 
                //R = I -2N (I *N)
                //R = outgoing velocity 
                //I = incoming velocity
                //N = normal vector

                grenadeVelocity = grenadeVelocity - 2.0f * normal * (VectorDot(grenadeVelocity, normal));

            }

            if (crateRect.intersects(playerRect))
            {
                // Find the side of collision using our collision depth 
                sf::Vector2f depth = CollisionDepth(playerRect, crateRect);
                sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));
                sf::Vector2f normal;

                if (absDepth.x < absDepth.y) //collided in the x direction 
                {
                    //Move out of the collision, fist 
                    sf::Vector2f playerPos = player1Sprite.getPosition();
                    playerPos.x += depth.x;
                    player1Sprite.setPosition(playerPos);


                    // Are we colliding from the left or right?
                    if (depth.x > 0)//colliding form tthe left 
                    {
                        // Set the normal vector to point left 
                        normal = sf::Vector2f(-1, 0);
                    }
                    else //colliding from the right 
                    {
                        // Set the normal vector to point right 
                        normal = sf::Vector2f(1, 0);
                    }
                }
                else //colliding in th y direction
                {
                    //Move out of the collision fist of all 
                    sf::Vector2f playerPos = player1Sprite.getPosition();
                    playerPos.y += depth.y;
                    player1Sprite.setPosition(playerPos);

                    //Are we colliding form top or botttom?

                    if (depth.y > 0)//colliding from top
                    {
                        normal = sf::Vector2f(0, -1);

                    }
                    else //colliding form bottom
                    {
                        normal = sf::Vector2f(0, 1);
                    }
                }


                playerVelocity.y = 0;
            }

        }




        ///
        ///
        /// Easing function stuff
        /// 
        /// 


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            sf::Vector2f targetLocation = sf::Vector2f(1000, 700);

            begin = easingPosition;
            end = targetLocation;
            change = end - begin;
            direction = VectorNormalize(change);

            time = 0;

        }

        easingPosition += speed * direction * deltaTime;

        if (time < duration)
        {
            sf::Vector2f k1 = change / (duration * duration);
            sf::Vector2f k2 = sf::Vector2f(0, 0);
            sf::Vector2f k3 = begin;

            easingPosition = (change / (duration * duration)) * (time * time) + begin;
            time += deltaTime;
        }
        else
        {
            easingPosition = end;
        }

        easingSprite.setPosition(easingPosition);
                

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
       


        
        //Draw grenade :D
        window.draw(grenadeSprite);



        //pip draw stuff
        for (int i = 0; i < pips.size();++i)
        {
            window.draw(pips[i]);
        }



        //CRATE draw stuff
        for (int i = 0; i < NUM_CRATE;++i)
        {
            window.draw(crateSprite[i]);
        }


        //easing function 
        window.draw(easingSprite);

        window.display();
    }

    return 0;
}