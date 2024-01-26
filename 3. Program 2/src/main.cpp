#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "tests.cpp"
#include <math.h>

void test_vec2();

int main()
{
    // test_vec2();
    Game g("config/config");
    g.run();
}





void test_vec2()
{
    Vec2 v1(1, 2);
    Vec2 v2(1, 2);
    assertBoolEquality(v1 == v2, true, "=");

    Vec2 v3 = v1 + v2;
    assertBoolInEquality(v2 != v3, true, "!=");
    assertNumbersEquality(v3.x, v1.x + v2.x, "+");
    assertNumbersEquality(v3.y, v1.y + v2.y, "+");

    Vec2 v4 = v3 - v2;
    assertNumbersEquality(v4.x, v3.x - v2.x, "-");
    assertNumbersEquality(v4.y, v3.y - v2.y, "-");

    Vec2 v5(10, 20);
    Vec2 v6 = v4 * v5;
    assertNumbersEquality(v6.x, v4.x * v5.x, "*");
    assertNumbersEquality(v6.y, v4.y * v5.y, "*");

    Vec2 v7 = v6 / v2;
    assertNumbersEquality(v7.x, v6.x / v2.x, "/");
    assertNumbersEquality(v7.y, v6.y / v2.y, "/");

    float old_x = v7.x;
    float old_y = v7.y;
    v7 += v2;
    assertNumbersEquality(v7.x, old_x + v2.x, "+=");
    assertNumbersEquality(v7.y, old_y + v2.y, "+=");

    old_x = v6.x;
    old_y = v6.y;
    v6 -= v1;
    assertNumbersEquality(v6.x, old_x - v1.x, "-=");
    assertNumbersEquality(v6.y, old_y - v1.y, "-=");

    old_x = v3.x;
    old_y = v3.y;
    v3 *= 9;
    assertNumbersEquality(v3.x, old_x * 9, "*=");
    assertNumbersEquality(v3.y, old_y * 9, "*=");

    old_x = v4.x;
    old_y = v4.y;
    v4 /= 4;
    assertNumbersEquality(v4.x, old_x / 4, "/=");
    assertNumbersEquality(v4.y, old_y / 4, "/=");

    Vec2 v8(40, 30);
    old_x = v8.x;
    old_y = v8.y;
    v8.normalize();
    assertNumbersEquality(v8.x, (old_x / sqrt(old_x * old_x + old_y * old_y)), "Normalize");
    assertNumbersEquality(v8.y, (old_y / sqrt(old_x * old_x + old_y * old_y)), "Normalize");

    assertNumbersEquality(v7.length(),
                          sqrt(v7.x * v7.x + v7.y * v7.y), "Length");
    assertNumbersEquality(v4.dist(v5),
                          sqrt(((v4.x - v5.x) * (v4.x - v5.x)) + ((v4.y - v5.y) * (v4.y - v5.y))), "Distance");
}