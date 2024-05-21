#include "Physics.h"
#include "Components.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{

    float x1, y1, x2, y2, h1, h2, w1, w2, ox, oy, dx, dy;
    h1 = a->getComponent<CBoundingBox>()->size.y;
    w1 = a->getComponent<CBoundingBox>()->size.x;
    h2 = b->getComponent<CBoundingBox>()->size.y;
    w2 = b->getComponent<CBoundingBox>()->size.x;

    x1 = a->getComponent<CTransform>()->pos.x;
    y1 = a->getComponent<CTransform>()->pos.y;
    x2 = b->getComponent<CTransform>()->pos.x;
    y2 = b->getComponent<CTransform>()->pos.y;
    dx = abs(x1 - x2);
    dy = abs(y1 - y2);

    ox = (w1 / 2) + (w2 / 2) - dx;
    oy = (h1 / 2) + (h2 / 2) - dy;

    return Vec2(ox, oy);
}

Vec2 Physics::GetPrevOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> player)
{

    float x1, y1, x2, y2, h1, h2, w1, w2, ox, oy, dx, dy;
    h1 = a->getComponent<CBoundingBox>()->size.y;
    w1 = a->getComponent<CBoundingBox>()->size.x;
    h2 = player->getComponent<CBoundingBox>()->size.y;
    w2 = player->getComponent<CBoundingBox>()->size.x;

    x1 = a->getComponent<CTransform>()->pos.x;
    y1 = a->getComponent<CTransform>()->pos.y;
    x2 = player->getComponent<CTransform>()->prevPos.x;
    y2 = player->getComponent<CTransform>()->prevPos.y;
    dx = abs(x1 - x2);
    dy = abs(y1 - y2);

    ox = (w1 / 2) + (w2 / 2) - dx;
    oy = (h1 / 2) + (h2 / 2) - dy;

    return Vec2(ox, oy);
}
