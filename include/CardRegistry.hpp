#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include "Card.h"

using namespace std;
using namespace sf;

typedef Vector2f vec2;

struct CardRegistry
{
	CardRegistry()
	{}
	
	Card getById(string id)
	{
		for(int i = 1; i < cards.size(); i++)
			if(cards[i].getId() == id) return cards[i];
		cout << "card of id " << id << " doesn't exist\n";
		return cards[0];
	}
	
	Card getRandomCard(int seed)
	{
		//srand(seed);
		int rarity = rand()%100 + 1;
		for(int i = 0; i < percentByRarity.size()-1; i++)
		{
			if(rarity > percentByRarity[i])
			{
				vector<Card> cardsOfThisRarity;
				for(int c = 0; c < cards.size(); c++)
					if(cards[c].getRarity() == i)
						cardsOfThisRarity.push_back(cards[c]);
				if(cardsOfThisRarity.size() > 0)
					return cardsOfThisRarity[rand()%cardsOfThisRarity.size()];	
			}
		}
		return cards[0];
	}	
	
	void registerCard(Card card)
	{
		cards.emplace_back(card);
	}
	
	vector<int> percentByRarity = {60, 35, 10, 5};
	vector<Card> cards = {Card("error", "error", -1, "res/error.png", "res/error.png", [](OnUseCardContext ctx){ return ctx.args;}, {vec2(0, 0)}, [](OnUpdateContext ctx){return ctx.args;})};

};
