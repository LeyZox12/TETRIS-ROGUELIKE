#ifndef MODIFIER_H
#define MODIFIER_H

#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../../../class/PointEngine/include/PointEngine.h"

using namespace std;
using namespace sf;

typedef Vector2f vec2;

class Modifier
{
    public:
        Modifier();
        Modifier(int maxLevel, function<void(PointEngine& pe, vec2 mousepos)> onClick, function<void(PointEngine& pe)> onLine, bool isPassive);
        void callOnLine(PointEngine& pe);
        void callOnClick(PointEngine& pe, vec2 mouse);
        void setLevel(int level);
        int getLevel();
        int getMaxLevel();
        bool getIsPassive();
        void setIsPassive(bool isPassive);
    protected:

    private:
        function<void(PointEngine& pe, vec2 mousepos)> onClick;
        function<void(PointEngine& pe)> onLine;
        bool isPassive;
        int maxLevel;
        int level;
};

#endif // MODIFIER_H
