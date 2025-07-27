#include <iostream>
#include <SFML/Graphics.hpp>
#include "../../class/PointEngine/include/PointEngine.h"
#include <thread>
#include "Deck.hpp"
#include "../../class/UIutils.h"
#define pi 3.141592654

using namespace std;
using namespace sf;

using vec2 = Vector2f;
using vec2u = Vector2u;

RenderWindow window(VideoMode({1000, 1000}), "Tetris Weird");
PointEngine pe;
UIutils ui;

const int BORDER_SIZE = 100;
const int RADIUS = (window.getSize().x - 2 * BORDER_SIZE) / 20 - 1;
const int RECT_SIZE = (window.getSize().x - 2 * BORDER_SIZE) / 10;
const int VERTICAL = 8;
const Color DISPLAY_COLOR = Color::White;
const int COUNT_TO_LINE = 7;

sf::Clock deltaClock;
float dt = 0.016f;
int rotation = 0;
int currentPiece = 0;
int timer = 0;
//0.74


Deck deck;
Card testCard(vec2(300, 800), vec2(150, 200), "res/long-I.png", "res/defaultCard.png");


vector<vector<vec2>> pieces =
{
    //square
    {
        vec2(0, 0),
        vec2(0, 1),
        vec2(1, 1),
        vec2(1, 0)
    },
    //L
    {
        vec2(0, 0),
        vec2(0, 1),
        vec2(0, 2),
        vec2(0,3)
    },
    //T
    {
        vec2(1, 0),
        vec2(0, 1),
        vec2(1, 1),
        vec2(2,1)
    },
    //L
    {
        vec2(0, 0),
        vec2(0, 1),
        vec2(0, 2),
        vec2(1, 2)
    },
    //J
    {
        vec2(1, 0),
        vec2(1, 1),
        vec2(1, 2),
        vec2(0, 2)
    },
    //Z
    {
        vec2(0, 0),
        vec2(1, 0),
        vec2(1, 1),
        vec2(2, 1)
    },
    //S
    {
        vec2(0, 1),
        vec2(1, 1),
        vec2(1, 0),
        vec2(2, 0)
    }
};

vector<Color> colors;
vector<int> colorCount;

void spawnPiece(vec2 pos, vector<vec2> piece);
bool isInGrid(vec2 p, vec2 rp, vec2 rs);
void dismantleGrid();
void drawPiece(vec2 pos, vector<vec2> piece, Color col, RenderWindow& window);
float clamp(float val, float minv, float maxv);
void start();
void endTurn();

int main()
{
    testCard.setPos(vec2(300, 300));
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
                    spawnPiece(vec2(clamp(mousepos.x, 311, 710), 50), pieces[currentPiece]);
                    currentPiece = rand()%pieces.size();
                    //currentPiece = 2;
                }
            }
            if(e->is<Event::KeyPressed>())
            {
                Keyboard::Key key = e->getIf<Event::KeyPressed>() -> code;
                if(key == Keyboard::Key::R)
                    rotation = (rotation + 90)%360;
                else if(key == Keyboard::Key::A)
                    dismantleGrid();
                else if(key == Keyboard::Key::F)
                    testCard.flip(0.2f);
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
            if(line == COUNT_TO_LINE)
            {

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
        testCard.update(dt);
        deck.updateCards(dt, mousepos);
        pe.updatePointPos(dt, mousepos);
        pe.applyConstraints(2);
        pe.applyCollisions(2);
        window.clear(Color::Black);
        pe.display(window, DISPLAY_COLOR);

        drawPiece(vec2(clamp(mousepos.x, 311, 710), 50), pieces[currentPiece], Color(DISPLAY_COLOR.r, DISPLAY_COLOR.g, DISPLAY_COLOR.b, 100), window);
        deck.drawCards(window);
        ui.displayElements(window);
        window.display();
        dt = deltaClock.restart().asSeconds();

        if(clock() - timer>= 1000)
        {
            window.setTitle("Tetris Weird, FPS:" + to_string((int)floor(1.0f / dt)));
            timer = clock();
        }
    }
    return 0;
}


void spawnPiece(vec2 pos, vector<vec2> piece)
{
    int index1 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.x < b.x;}));
    int index2 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.y < b.y;}));
    float maxX = piece[index1].x;
    float maxY = piece[index2].y;

    int pointCount = pe.getPointCount();
    for(auto& p : piece)
    {
        float theta = rotation * pi / 180.0f;
        p -= vec2(maxX/2.0f, maxY/2.0f);
        p = vec2(p.x * sin(theta) - p.y * cos(theta), p.x * cos(theta) - p.y * sin(theta));
        pe.addPoint(pos + p * (float)RADIUS * 2.1f + vec2(RADIUS, RADIUS), false, true, RADIUS, 200.0f,
                    [](OnUpdateContext ctx)
                    {

                        if(ctx.args.size() == 0)
                        {
                            vector<any> args;
                            args.push_back(clock());
                            return args;
                        }
                        else
                        {
                            //if(clock() - any_cast<clock_t>(ctx.args[0]) >= 5000) pe.getPoint(ctx.index).setIsStatic(true);
                            return ctx.args;
                        }


                    });
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

void drawPiece(vec2 pos, vector<vec2> piece, Color col, RenderWindow& window)
{
    CircleShape circle(RADIUS);
    circle.setFillColor(col);
    float theta = rotation * pi / 180.0f;
    int index1 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.x < b.x;}));
    int index2 = distance(piece.begin(), max_element(piece.begin(), piece.end(), [](const auto &a, const auto &b) { return a.y < b.y;}));

    float maxX = piece[index1].x;
    float maxY = piece[index2].y;
    vec2 size;
    for(auto& p : piece)
    {
        p -= vec2(maxX/2.0f, maxY/2.0f);
        p = vec2(p.x * sin(theta) - p.y * cos(theta), p.x * cos(theta) - p.y * sin(theta));
        circle.setPosition(pos + p * 2.0f * (float)RADIUS);
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
    float ratio = 700/10.f;
    testCard.setCardTranslationDuration(0.5f);
    for(int i = 0; i < 9; i++)
    {
        pe.addRectangle(Rect<int>({200, i * ratio}, {ratio, ratio}), "res/grass_pillar_left.png");
        pe.addRectangle(Rect<int>({900 - ratio, i * ratio}, {ratio, ratio}), "res/grass_pillar_right.png");
        pe.addRectangle(Rect<int>({200+i * ratio, 700 - ratio}, {ratio, ratio}), i == 0 ? "res/no_grass.png" : "res/grass_bottom.png");
    }
    pe.addRectangle(Rect<int>({200 + 9 * ratio, 700 - ratio}, {ratio, ratio}), "res/no_grass.png");
    for(int i = 0; i < 10; i++)
    {
        deck.cards.emplace_back(vec2(300 + i, 800 + i), vec2(150, 200), "res/long-I.png", "res/defaultCard.png");
    }
    //pe.addPoint(vec2(256, 256), false, true, 30, 200, cluster);

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
    deck.giveHand(5, window.getSize(), 110.0f);
}

void endTurn()
{
    cout << "turn ended\n";
}
