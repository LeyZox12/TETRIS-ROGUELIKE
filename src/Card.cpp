#include "../include/Card.h"

Card::Card()
{

}

Card::Card(vec2 anchorPos, vec2 size, string id, string frontTexturePath, string backTexturePath, function<vector<any>(OnUseCardContext ctx)> onUse, vector<vec2> pieceOnPlace, function<vector<any>(OnUpdateContext)> pointsFunc)
{
    this->anchorPos = anchorPos;
    if(!frontTexture.loadFromFile(frontTexturePath))
        cout << "couldn't load front texture\n";
    if(!backTexture.loadFromFile(backTexturePath))
        cout << "couldn't load front texture\n";
    sprite.setPosition(anchorPos);
    targetScale = vec2(1, 1);
    sprite.setSize(vec2(size.x, size.y));
    sprite.setOrigin({size.x / 2.0f, size.y / 2.0f});
	this->onUse = onUse;
	this->pieceOnPlace = pieceOnPlace;
	this->pointsFunc = pointsFunc;
	this->id = id;
}

Card::Card(string id, string name, int rarity, string frontTexturePath, string backTexturePath, function<vector<any>(OnUseCardContext ctx)> onUse, vector<vec2> pieceOnPlace, function<vector<any>(OnUpdateContext)> pointsFunc)
{
    if(!frontTexture.loadFromFile(frontTexturePath))
        cout << "couldn't load front texture\n";
    if(!backTexture.loadFromFile(backTexturePath))
        cout << "couldn't load front texture\n";
    sprite.setPosition(anchorPos);
	this->onUse = onUse;
	this->pieceOnPlace = pieceOnPlace;
	this->pointsFunc = pointsFunc;
	this->id = id;
	this->name = name;
	this->rarity = rarity;
	sprite.setScale({-1, 1});
}

void Card::flip(float durationSeconds)
{
    animationDuration = durationSeconds;
    targetScale.x = targetScale.x > 0 ? -1 : 1;
}

void Card::setSize(vec2 size)
{
    sprite.setSize(size);
}

void Card::setFlipped(bool state, float durationSeconds)
{
    if(flipped != state)
    {
        targetScale.x = -targetScale.x;
        animationDuration = durationSeconds;
    }
}

void Card::setCardTranslationDuration(float duration)
{
    this->speed = duration;
}

void Card::setPos(vec2 pos)
{
    sprite.setPosition(pos);
}

void Card::setAnchorPos(vec2 pos)
{
    this->anchorPos = pos;
}

vec2 rotateAround(vec2 point, vec2 center, float angle)
{
    point -= center;
    point = vec2(point.x * cos(angle) - point.y * sin(angle),
                 point.x * sin(angle) + point.y * cos(angle));
    return point + center;
}

bool Card::getCollision(vec2 point)
{
    vec2 p = sprite.getPosition();
    vec2 s = sprite.getSize();
    point = rotateAround(point, p, -sprite.getRotation().asRadians());
    return (abs(p.x - point.x) < s.x * 0.5f && abs(p.y - point.y) < s.y * 0.5f);
}

vec2 Card::getAnchorPos()
{
    return anchorPos;
}

void Card::setTextures(string frontTexturePath, string backTexturePath)
{
    if(!frontTexture.loadFromFile(frontTexturePath))
        cout << "error, couldn't load front texture\n";
    if(!backTexture.loadFromFile(backTexturePath))
        cout << "error, couldn't load back texture\n";
}

bool Card::getCollision(RectangleShape rect)
{
	vec2 p1 = rect.getPosition();
	vec2 s1 = rect.getSize();
	vec2 p2 = sprite.getPosition() - sprite.getOrigin();
	vec2 s2 = sprite.getSize();
	return p1.x + s1.x > p2.x && p1.x < p2.x && p1.y + s1.y > p2.y && p1.y < p2.y; 
}

void Card::setRotation(float angle)
{
    sprite.setRotation(degrees(angle));
}

void Card::setTargetRotation(float angle)
{
    targetRotation = angle;
}

vec2 Card::getPos()
{
    return sprite.getPosition();
}

void Card::update(float dt)
{
    sprite.setTexture(sprite.getScale().x > 0 ? &frontTexture : &backTexture);
    vec2 diff = vec2(anchorPos - sprite.getPosition());
    float mag = hypot(diff.x, diff.y);
    if(mag != 0.0f)
    {
        vec2 normalized = diff / mag;

        sprite.move(diff * dt *(1.0f/speed));
    }
    float fixedTargetRot = targetRotation >= 0 ? targetRotation + 90 : 90+targetRotation;
    float rot = sprite.getRotation().asDegrees();
    float fixedRot = rot > 180 ? rot - 270 : rot+90;
    //cout << fixedTargetRot << "," <<  fixedRot << ":" << fixedRot - targetRotation<< endl;
    sprite.rotate(degrees((fixedTargetRot - fixedRot) * dt * (1.f / speed)));
    if(animationDuration != 0.0f)
        sprite.setScale(sprite.getScale() + (targetScale - sprite.getScale()) * dt * (1.0f/animationDuration));
}

void Card::setAlpha(float alpha)
{
    sprite.setFillColor(Color(255, 255, 255, alpha));
}

void Card::draw(RenderWindow& window)
{
    window.draw(sprite);
}

vector<any> Card::callOnUse(OnUseCardContext ctx)
{
	return onUse(ctx);
}

bool Card::getFlipped()
{
	return targetScale.x < 0; 
}

string Card::getId()
{
	return id;
}

string Card::getName()
{
	return name;
} 

int Card::getRarity()
{
	return rarity;
}
