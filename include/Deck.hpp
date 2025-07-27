#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Card.h"
#define PI 3.141592654

using namespace std;
using namespace sf;

struct Deck
{
    void addCard(vec2 anchorPos, vec2 size, string frontTexturePath, string backTexturePath)
    {
        cards.emplace_back(Card(anchorPos, size, frontTexturePath, backTexturePath));
    }

    void drawCards(RenderWindow& window)
    {
        for(auto& c : cards)
            c.draw(window);
    }

    void updateCards(float dt, vec2 mousepos)
    {
        for(auto& c : cards)
            c.update(dt);
        if(heldCardIndex > -1)
        {
            Card& card = cards[heldCardIndex];
            vec2 oldPos = card.getPos();
            card.setPos(mousepos + heldCardOffset);
            vec2 diff = oldPos - (mousepos + heldCardOffset);
            card.setRotation(-diff.x);
        }
    }

    void update(optional<Event> e, vec2 mousepos)
    {
        if(e->is<Event::MouseButtonPressed>())
        {
            if(e->getIf<Event::MouseButtonPressed>()->button == Mouse::Button::Left)
            {
                holding = true;
                int index = getCardOnCur(mousepos);
                if(index > -1)
                {
                    heldCardIndex = index;
                    heldCardOffset = cards[index].getPos() - mousepos;
                }
            }
        }
        else if(e->is<Event::MouseButtonReleased>())
        {
            if(e->getIf<Event::MouseButtonReleased>()->button == Mouse::Button::Left)
            {
                holding = false;
                heldCardIndex = -1;
            }
        }

        //for(auto& c : cards) c.setTargetRotation(fmod(mousepos.x, 180)-90);

    }

    void giveHand(int cardCount, vec2u winSize, float spacing)
    {
        float angle = 90;
        float ratio = angle / cardCount;
        vec2 center = vec2(winSize.x / 2.f, winSize.y);
        float sides = (180 - angle) / 2.f;
        for(int i = 0; i < cardCount; i++)
        {

            float angleAsDeg = ratio * i + sides;
            float angleAsRad = (float)angleAsDeg * PI/180.f;
            cards[cards.size()-1-i].setAnchorPos(vec2(center.x - cos(angleAsRad) * spacing, center.y - sin(angleAsRad)* spacing));
            cards[cards.size()-1-i].setTargetRotation(angleAsDeg - 90);
            //this_thread::sleep_for(300ms);
        }
    }

    int getCardOnCur(vec2 mousepos)
    {
        for(int i = cards.size() - 1; i >= 0; i--)
            if(cards[i].getCollision(mousepos))
                return i;
        return -1;
    }
    void setCardsTranslationDuration(float duration)
    {
        for(auto& c : cards)c.setCardTranslationDuration(duration);
    }
    vector<Card> cards;
    bool holding;
    int heldCardIndex;
    vec2 heldCardOffset;

};
