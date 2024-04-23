#include "Physics.h"
#include "Components.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{

    a->getComponent<CTransform>()->pos.x;
    a->getComponent<CAnimation>()->animation.getSize().x;

    float x1, y1, x2, y2, h1, h2, w1, w2, ox, oy, dx, dy;
    h1 = a->getComponent<CAnimation>()->animation.getSize().y;
    w1 = a->getComponent<CAnimation>()->animation.getSize().x;
    h2 = b->getComponent<CAnimation>()->animation.getSize().y;
    w2 = b->getComponent<CAnimation>()->animation.getSize().x;

    x1 = a->getComponent<CTransform>()->pos.x - (w1 / 2);
    y1 = a->getComponent<CTransform>()->pos.y - (h1 / 2);
    x2 = b->getComponent<CTransform>()->pos.x - (w2 / 2);
    y2 = b->getComponent<CTransform>()->pos.y - (h2 / 2);
    dx = abs(x1 - x2);
    dy = abs(y1 - y2);

    ox = (w1 / 2) + (w2 / 2) - dx;
    oy = (h1 / 2) + (h2 / 2) - dy;

    if (
        x2 < x1 + w1 &&
        x1 < x2 + w2 &&
        y1 < y2 + h2 &&
        y2 < y1 + h1)
    {
        std::cout << "collision between" << a->tag() << " and " << b->tag() << std::endl;
        return Vec2(ox, oy);
    }

    return Vec2(0, 0);
}
