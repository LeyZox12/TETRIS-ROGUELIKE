#include <iostream>
#include <SFML/Graphics.hpp>
#include "../../class/PointEngine/include/PointEngine.h"
#include <thread>
#include <future>
#include "Deck.hpp"
#include "CardRegistry.hpp"
#include "OnUseCardContext.hpp"
#include "../../class/UIutils.h"
#include "include/cards.hpp"
#define pi 3.141592654

using namespace std;
using namespace sf;

using vec2 = Vector2f;
using vec2u = Vector2u;

RenderWindow window(VideoMode({1000, 1000}), "Tetris Weird");
PointEngine pe;
UIutils ui;
vector<future<void>> pool;

CardRegistry cardRegistry;

const int BORDER_SIZE = 100;
const int RADIUS = (window.getSize().x - 2 * BORDER_SIZE) / 20 - 1;
const int RECT_SIZE = (window.getSize().x - 2 * BORDER_SIZE) / 10;
const int VERTICAL = 8;
const Color DISPLAY_COLOR = Color::White;
const int COUNT_TO_LINE = 7;
const int ANGLE_OFFSET_ON_ROT = 90;

sf::Clock deltaClock;
float dt = 0.016f;
int rotation = 0;
int currentPiece = 0;
int timer = 0;
float cardAngle = 110;
float cardSpacing = 300;
vec2 center = vec2(window.getSize().x/2.f, window.getSize().y + window.getSize().y * 0.1);
RectangleShape placingZone({560, 200});
Texture placingZoneTexture;

Deck deck;
vector<Color> colors;
vector<int> colorCount;

void spawnPiece(vec2 pos, vector<vec2> piece, function<vector<any>(OnUpdateContext ctx)> pointsFunc);
bool isInGrid(vec2 p, vec2 rp, vec2 rs);
void dismantleGrid();
void drawPiece(vec2 pos, vector<vec2> piece, Color col, RenderWindow& window);
float clamp(float val, float minv, float maxv);
void start();
void endTurn();
void updateThreadPool();

int main()
{
    start();
    while(window.isOpen())
    {
        vec2 mousepos = vec2(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
        //screen to grid pos
        //int m = floorpos ((mousepos.x-BORDER_SIZE*1.25) / ((float)RADIUS * 2)) * RADIUS * 2 + BORDER_SIZE;
        while(const optional<Event> e = window.pollEvent())
        {
            ui.updateElements(e, window);
            if(e->is<Event::Closed>() ||
               e->is<Event::KeyPressed>() &&
               e->getIf<Event::KeyPressed>() -> code == Keyboard::Key::Escape) window.close();
            if(e->is<Event::MouseButtonPressed>())
            {
                if(e->getIf<Event::MouseButtonPressed>() -> button == Mouse::Button::Right)
                    pe.removePoint(pe.getPointIndexAtPos(mousepos));
                else if(mousepos.y < 700)
                {
                    
                }
            }
            if(e->is<Event::KeyPressed>())
            {
                Keyboard::Key key = e->getIf<Event::KeyPressed>() -> code;
                if(key == Keyboard::Key::R)
                    rotation = (rotation + ANGLE_OFFSET_ON_ROT)%360;
                else if(key == Keyboard::Key::A)
                    dismantleGrid();
            }
            deck.update(e, mousepos);
        }
        colorCount.clear();
        for(int p = 0; p < pe.getPointCount(); p++)
        {
            Point& point = pe.getPoint(p);
            int index = point.getPos().y / RECT_SIZE;
            point.setColor(colors[index]);
            colorCount.push_back(index);
        }
        for(int c = 0; c < VERTICAL; c++)
        {
            int line = count(colorCount.begin(), colorCount.end(), c);
            if(line == COUNT_TO_LINE) {
                vector<int> indexArr;
                for(int p = 0; p < pe.getPointCount(); p++)
                {
                    if(pe.getPoint(p).getColor() == colors[c])
                    {
                        indexArr.push_back(p);
                    }
                }
                //sort(indexArr.begin(), indexArr.end(), [](auto &i1, auto &i2){return pe.getPoint(i1).getPos().x < pe.getPoint(i2).getPos().x;});
                sort(indexArr.begin(), indexArr.end(), greater<int>());
                VertexArray line(PrimitiveType::LineStrip, 10);
                for(int i = 0; i < COUNT_TO_LINE; i++)
                {
                    line[i].position = pe.getPoint(indexArr[i]).getPos();
                    line[i].color = colors[c];
                }
                for(auto& index : indexArr)
                    pe.removePoint(index);
            }
        }
        deck.updateCards(dt, mousepos);
        pe.updatePointPos(dt, mousepos);
        pe.applyConstraints(2);
        pe.applyCollisions(2);
        window.clear(Color::Black);
        pe.display(window, DISPLAY_COLOR);

        updateThreadPool();
        deck.drawCards(window);
        ui.displayElements(window);

		int indexToDestroy = -1;
		int i = 0;
		for(int i = 0; i < deck.cards.size(); i++)
		{
			Card& card = deck.cards[i];
			if(card.getPos().y < 200 && card.getPos().x > 200)
			{
				card.setAlpha(100);
        		drawPiece(vec2(clamp(card.getPos().x, 311, 710), 50), card.pieceOnPlace, Color(DISPLAY_COLOR.r, DISPLAY_COLOR.g, DISPLAY_COLOR.b, 100), window);
				if(!deck.holding) indexToDestroy = i;
			}
			else card.setAlpha(255);
		}
		if(indexToDestroy >= 0 && !deck.cards[indexToDestroy].getFlipped())
		{
			Card& card = deck.cards[indexToDestroy];
			card.args = card.callOnUse(OnUseCardContext(pe, card.args, window, mousepos));
			spawnPiece(vec2(clamp(card.getPos().x, 311, 710), 50), card.pieceOnPlace, card.pointsFunc);
			deck.removeCard(indexToDestroy, cardSpacing, cardAngle, center);
		}

		window.draw(placingZone);
        window.display();
        dt = deltaClock.restart().asSeconds();
        if(clock() - timer>= 1000)
        {
            //for(auto& c : deck.cards) cout <<
              //  "pos(sprite):" << c.getPos().x << ";" << c.getPos().y << "\npos(anchor)" << c.getAnchorPos().x << ";" << c.getAnchorPos().y << "\nSize:" << c.sprite.getSize().x << ";" << c.sprite.getSize().y << "scale:" << c.sprite.getScale().x << ";" << c.sprite.getScale().y<< endl;
            window.setTitle("Tetris Weird, FPS:" + to_string((int)floor(1.0f / dt)));
            timer = clock();
        }
    }
    return 0;
}


void spawnPiece(vec2 spawnPos, vector<vec2> piece, function<vector<any>(OnUpdateContext ctx)> pointsFunc)
{
    float theta = rotation * pi / 180.0f;
    int index1 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.x < b.x;}));
    int index2 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.y < b.y;}));
    float maxX = piece[index1].x;
    float maxY = piece[index2].y;
    int pointCount = pe.getPointCount();
	for(auto& p : piece)
    {
        float theta = rotation * pi / 180.0f;
		vec2 pos = spawnPos + p * (float)RADIUS * 2.f;
		vec2 size = vec2(maxX * (float)RADIUS * 2.f, maxY * (float)RADIUS * 2.f);
		vec2 center = spawnPos + size * 0.5f;
        pos -= center;
        pos = vec2(pos.x * cos(theta) - pos.y * sin(theta), pos.x * sin(theta) + pos.y * cos(theta));
        pos += center; 
		pe.addPoint(pos, false, true, RADIUS, 200.0f, pointsFunc);
	}
   
    for(int i = 0; i < piece.size(); i++)
    {
        for(int j = 0; j < piece.size(); j++)
        {
            if(i != j)
                pe.addConstraint(pointCount+ i, pointCount+ j, 2, 0, false);
        }
    }
}

bool isInGrid(vec2 p, vec2 rp, vec2 rs)
{
    return  p.x > rp.x &&
            p.y > rp.y &&
            p.x < rp.x + rs.x &&
            p.y < rp.y + rs.y;
}

void dismantleGrid()
{
    for(int i = 0; i < pe.getPointCount(); i++)
        pe.removeConstraints(i);
}

void drawPiece(vec2 drawPos, vector<vec2> piece, Color col, RenderWindow& window)
{
    CircleShape circle(RADIUS);
    circle.setFillColor(col);
    float theta = rotation * pi / 180.0f;
    int index1 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.x < b.x;}));
    int index2 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.y < b.y;}));
    float maxX = piece[index1].x;
    float maxY = piece[index2].y;
    int pointCount = pe.getPointCount();
	for(auto& p : piece)
    {
        float theta = rotation * pi / 180.0f;
		vec2 pos = drawPos + p * (float)RADIUS * 2.f;
		vec2 size = vec2(maxX * (float)RADIUS * 2.f, maxY * (float)RADIUS * 2.f);
		vec2 center = drawPos + size * 0.5f;
        pos -= center;
        pos = vec2(pos.x * cos(theta) - pos.y * sin(theta), pos.x * sin(theta) + pos.y * cos(theta));
        pos += center; 
        circle.setPosition(pos - vec2(RADIUS, RADIUS));
        window.draw(circle);
	}
}


float clamp(float val, float minv, float maxv)
{
    if(val > maxv) return maxv;
    if(val < minv) return minv;
    return val;
}

void start()
{
    window.setFramerateLimit(240);
    window.setVerticalSyncEnabled(false);
	load(cardRegistry);
    float ratio = 700/10.f;
    for(int i = 0; i < 9; i++)
    {
        pe.addRectangle(Rect<int>({200, i * ratio}, {ratio, ratio}), "res/grass_pillar_left.png");
        pe.addRectangle(Rect<int>({900 - ratio, i * ratio}, {ratio, ratio}), "res/grass_pillar_right.png");
        pe.addRectangle(Rect<int>({200+i * ratio, 700 - ratio}, {ratio, ratio}), i == 0 ? "res/no_grass.png" : "res/grass_bottom.png");
    }
    pe.addRectangle(Rect<int>({200 + 9 * ratio, 700 - ratio}, {ratio, ratio}), "res/no_grass.png");
	vector<string> ids = {"square_classic", "I_classic", "L_classic", "J_classic", "Z_classic", "S_classic", "T_classic"};
    for(int i = 0; i < ids.size(); i++)
    {
		//deck.cards.push_back(cardRegistry.getRandomCard(0));
		deck.cards.push_back(cardRegistry.getById(ids[i]));
		deck.cards[deck.cards.size()-1].setAnchorPos(vec2(200 - i, 800 - i));
		deck.cards[deck.cards.size()-1].sprite.setSize(vec2(150, 200));
        deck.cards[deck.cards.size()-1].sprite.setOrigin({75, 100});
    }
	placingZoneTexture.loadFromFile("res/placeZone.png");
	placingZone.setTexture(&placingZoneTexture);
	placingZone.setPosition({270, 0});
	placingZone.setFillColor(Color(255, 255, 255, 120));
    deck.setCardsTranslationDuration(0.2f);
    colors.push_back(Color(116, 0, 184));
    colors.push_back(Color(105, 48, 195));
    colors.push_back(Color(94, 96, 206));
    colors.push_back(Color(83, 144, 217));
    colors.push_back(Color(78, 168, 222));
    colors.push_back(Color(72, 191, 227));
    colors.push_back(Color(86, 207, 225));
    colors.push_back(Color(100, 223, 223));
    colors.push_back(Color(114, 239, 221));
    colors.push_back(Color(128, 255, 219));
    srand(time(nullptr));
    ui.font = Font("res/font.ttf");
    ui.addButton(vec2(window.getSize().x, window.getSize().y)- vec2(200, 100), vec2(100, 50), endTurn, "EndTurn");
    pool.emplace_back(std::async([&deck, &window](){deck.giveHand(7, cardSpacing, cardAngle, center);}));
}

void endTurn()
{
    cout << "turn ended\n";
}

void updateThreadPool()
{
    vector<int> toDelete;
    for(int i = 0; i < pool.size(); i++)
    {
        if(pool[i].wait_for(0ms) == future_status::ready)
            toDelete.push_back(i);
    }
    sort(toDelete.begin(), toDelete.end(), greater<int>());
    for(auto& i : toDelete) pool.erase(pool.begin() + i);
}
