#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Card.h"
#include <thread>
#define PI 3.141592654

using namespace std;
using namespace sf;

struct Deck
{
    //void addCard(vec2 anchorPos, vec2 size, string frontTexturePath, string backTexturePath, function<vector<any>(OnUseCardContext ctx)> onUse)
    //{
      //  cards.emplace_back(Card(anchorPos, size, frontTexturePath, backTexturePath, onUse));
    //}

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
				
    }

    void giveHand(int cardCount, float spacing, float angle, vec2 center)
    {
        this_thread::sleep_for(400ms);
        float ratio = angle / cardCount;
        float sides = (180 - angle) / 2.f;
        for(int i = 0; i < cardCount; i++)
        {
            cards[i].setAnchorPos(vec2(center.x, center.y - 50));
			hand.push_back(i);
            this_thread::sleep_for(100ms);
            cards[i].flip(0.2f);
            this_thread::sleep_for(200ms);
        }

        for(float i = 0.5; i < cardCount; i++)
        {
            this_thread::sleep_for(100ms);
            float angleAsDeg = ratio * i + sides;
            float angleAsRad = (float)angleAsDeg * PI/180.f;
            {
                cards[i].setAnchorPos(vec2(center.x - cos(angleAsRad) * spacing, center.y - sin(angleAsRad)* spacing));
                cards[i].setTargetRotation(angleAsDeg - 90);
            }

        }

        this_thread::sleep_for(1000ms);


    }
	
	void removeCard(int index, float spacing, float angle, vec2 center)
	{
		if(index >= 0 && index < cards.size())
		{
			cards.erase(cards.begin() + index);
			if(count(hand.begin(), hand.end(), index) == 1)hand.erase(find(hand.begin(), hand.end(), index));
			for(auto& handIndex : hand) if(handIndex > index) handIndex--;
		}
        float ratio = angle / hand.size();
        float sides = (180 - angle) / 2.f;
		float i = 0.5f;
        for(auto& index : hand)
        {
            float angleAsDeg = ratio * i + sides;
            float angleAsRad = (float)angleAsDeg * PI/180.f;
            {
                cards[index].setAnchorPos(vec2(center.x - cos(angleAsRad) * spacing, center.y - sin(angleAsRad)* spacing));
                cards[index].setTargetRotation(angleAsDeg - 90);
            }
			i++;
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
    bool holding = false;
	vector<int> hand;
    int heldCardIndex = -1;
    vec2 heldCardOffset;

};
