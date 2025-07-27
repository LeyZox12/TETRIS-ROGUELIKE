#include "Modifier.h"

Modifier::Modifier()
{

}

Modifier::Modifier(int maxLevel, function<void(PointEngine& pe, vec2 mousepos)> onClick, function<void(PointEngine& pe)> onLine, bool isPassive)
{
    this -> maxLevel = maxLevel;
    this -> onClick = onClick;
    this -> onLine = onLine;
    this -> isPassive = isPassive;
}

void Modifier::callOnLine(PointEngine& pe)
{
    onLine(pe);
}

void Modifier::callOnClick(PointEngine& pe, vec2 mousepos)
{
    onClick(pe, mousepos);
}

void Modifier::setLevel(int level)
{

}

int Modifier::getLevel()
{

}

int Modifier::getMaxLevel()
{

}

bool Modifier::getIsPassive()
{

}

void Modifier::setIsPassive(bool isPassive)
{

}
