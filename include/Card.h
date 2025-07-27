#ifndef CARD_H
#define CARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>
#define PI 3.141592654

using namespace std;
using namespace sf;

using vec2 = Vector2f;
using vec2u = Vector2u;

class Card
{
    public:
        Card();
        Card(vec2 anchorPos, vec2 size, string frontTexturePath, string backTexturePath);
        void flip(float durationSeconds);
        void setSize(vec2 size);
        void setFlipped(bool state, float durationSeconds);
        void setTextures(string frontTexturePath, string backTexturePath);
        void setRotation(float angle);
        void setPos(vec2 pos);
        void setAnchorPos(vec2 pos);
        bool getCollision(vec2 point);
        void setTargetRotation(float angle);
        bool getCollision(RectangleShape rect);
        void setCardTranslationDuration(float duration);
        void update(float dt);
        vec2 getPos();
        void draw(RenderWindow& window);
    private:
        bool flipped;
        vec2 targetScale;
        float speed = 1.0;
        float animationDuration;
        float targetRotation = 0.f;
        vec2 anchorPos;
        RectangleShape sprite;
        Texture frontTexture;
        Texture backTexture;
};

#endif // CARD_H
