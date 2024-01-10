/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"


// Game-related State data
SpriteRenderer  *Renderer;
GameObject* Player;

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

BallObject* Ball;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("C:/cz/BO/BreakOutGame/BreakOutGame/src/shaders/sprite.vs", "C:/cz/BO/BreakOutGame/BreakOutGame/src/shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls

    // Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    Shader myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    
    // load textures
    ResourceManager::LoadTexture("C:/cz/BO/BreakOutGame/BreakOutGame/src/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("C:/cz/BO/BreakOutGame/BreakOutGame/src/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("C:/cz/BO/BreakOutGame/BreakOutGame/src/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("C:/cz/BO/BreakOutGame/BreakOutGame/src/textures/block.png", false, "block");
    ResourceManager::LoadTexture("C:/cz/BO/BreakOutGame/BreakOutGame/src/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("C:/cz/BO/BreakOutGame/BreakOutGame/src/textures/paddle.png", true, "paddle");

    // load levels
    GameLevel one; one.Load("C:/cz/BO/BreakOutGame/BreakOutGame/src/levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("C:/cz/BO/BreakOutGame/BreakOutGame/src/levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("C:/cz/BO/BreakOutGame/BreakOutGame/src/levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("C:/cz/BO/BreakOutGame/BreakOutGame/src/levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
        -BALL_RADIUS * 2.0f);

    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
        ResourceManager::GetTexture("face"));

}

void Game::Update(float dt)
{
    Ball->Move(dt, this->Width);
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
    }
}

void Game::Render()
{
    Texture2D backgroundText = ResourceManager::GetTexture("background");

    if (this->State == GAME_ACTIVE)
    {
        // draw background
        Renderer->DrawSprite(backgroundText,
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
        );
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        Ball->Draw(*Renderer);
    }
}