#pragma once
#include "../include/CardRegistry.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;
using namespace std;

namespace Piece
{
    vector<vec2> Square =
    {
        vec2(0, 0),
        vec2(0, 1),
        vec2(1, 1),
        vec2(1, 0)
    };
    vector<vec2> I =
    {
        vec2(0, 0),
        vec2(0, 1),
        vec2(0, 2),
        vec2(0,3)
    };
    vector<vec2> T =
    {
        vec2(1, 0),
        vec2(0, 1),
        vec2(1, 1),
        vec2(2,1)
    };
    vector<vec2> L =
    {
        vec2(0, 0),
        vec2(0, 1),
        vec2(0, 2),
        vec2(1, 2)
    };
    vector<vec2> J = 
    {
        vec2(1, 0),
        vec2(1, 1),
        vec2(1, 2),
        vec2(0, 2)
    };
    vector<vec2> Z =
    {
        vec2(0, 0),
        vec2(1, 0),
        vec2(1, 1),
        vec2(2, 1)
    };
    vector<vec2> S = 
    {
        vec2(0, 1),
        vec2(1, 1),
        vec2(1, 0),
        vec2(2, 0)
    };
};


void load(CardRegistry& registry)
{
	registry.registerCard(Card(
		//Id
		"I_classic",
		//Name
		"Classic I",
		//Rarity
		0,
		//Front texture path
		"res/I_classic.png",
		//Back texture path
		"res/defaultCard.png",
		//On use function
		[](OnUseCardContext ctx)
		{
			return ctx.args;
		},
		//Piece To Place
		Piece::I,
		//Point Update Function
		[](OnUpdateContext ctx)
		{
			return ctx.args;
		}
	));

	registry.registerCard(Card(
		//Id
		"square_classic",
		//Name
		"Classic Square",
		//Rarity
		0,
		//Front texture path
		"res/square_classic.png",
		//Back texture path
		"res/defaultCard.png",
		//On use function
		[](OnUseCardContext ctx)
		{
			return ctx.args;
		},
		//Piece To Place
		Piece::Square,
		//Point Update Function
		[](OnUpdateContext ctx)
		{
			return ctx.args;
		}
	));

	registry.registerCard(Card(
		//Id
		"L_classic",
		//Name
		"Classic L",
		//Rarity
		0,
		//Front texture path
		"res/L_classic.png",
		//Back texture path
		"res/defaultCard.png",
		//On use function
		[](OnUseCardContext ctx)
		{
			return ctx.args;
		},
		//Piece To Place
		Piece::L,
		//Point Update Function
		[](OnUpdateContext ctx)
		{
			return ctx.args;
		}
	));

	registry.registerCard(Card(
		//Id
		"J_classic",
		//Name
		"Classic J",
		//Rarity
		0,
		//Front texture path
		"res/J_classic.png",
		//Back texture path
		"res/defaultCard.png",
		//On use function
		[](OnUseCardContext ctx)
		{
			return ctx.args;
		},
		//Piece To Place
		Piece::J,
		//Point Update Function
		[](OnUpdateContext ctx)
		{
			return ctx.args;
		}
	));

	registry.registerCard(Card(
		//Id
		"S_classic",
		//Name
		"Classic S",
		//Rarity
		0,
		//Front texture path
		"res/S_classic.png",
		//Back texture path
		"res/defaultCard.png",
		//On use function
		[](OnUseCardContext ctx)
		{
			return ctx.args;
		},
		//Piece To Place
		Piece::S,
		//Point Update Function
		[](OnUpdateContext ctx)
		{
			return ctx.args;
		}
	));

	registry.registerCard(Card(
		//Id
		"Z_classic",
		//Name
		"Classic Z",
		//Rarity
		0,
		//Front texture path
		"res/Z_classic.png",
		//Back texture path
		"res/defaultCard.png",
		//On use function
		[](OnUseCardContext ctx)
		{
			return ctx.args;
		},
		//Piece To Place
		Piece::Z,
		//Point Update Function
		[](OnUpdateContext ctx)
		{
			return ctx.args;
		}
	));

	registry.registerCard(Card(
		//Id
		"T_classic",
		//Name
		"Classic T",
		//Rarity
		0,
		//Front texture path
		"res/T_classic.png",
		//Back texture path
		"res/defaultCard.png",
		//On use function
		[](OnUseCardContext ctx)
		{
			return ctx.args;
		},
		//Piece To Place
		Piece::T,
		//Point Update Function
		[](OnUpdateContext ctx)
		{
			return ctx.args;
		}
	));

	cout << "cards loaded" << endl;
}
