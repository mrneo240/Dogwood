#include "Rendering\Colour.h"

ColourRGB ColourRGB::Black      = ColourRGB(0.0f, 0.0f, 0.0f);
ColourRGB ColourRGB::White      = ColourRGB(1.0f, 1.0f, 1.0f);
ColourRGB ColourRGB::Gray       = ColourRGB(0.5f, 0.5f, 0.5f);
ColourRGB ColourRGB::Red        = ColourRGB(1.0f, 0.0f, 0.0f);
ColourRGB ColourRGB::Green      = ColourRGB(0.0f, 1.0f, 0.0f);
ColourRGB ColourRGB::Blue       = ColourRGB(0.0f, 0.0f, 1.0f);
ColourRGB ColourRGB::Yellow     = ColourRGB(1.0f, 1.0f, 0.0f);
ColourRGB ColourRGB::Magenta    = ColourRGB(1.0f, 0.0f, 1.0f);
ColourRGB ColourRGB::Cyan       = ColourRGB(0.0f, 1.0f, 1.0f);

ColourRGB::ColourRGB()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

ColourRGB::ColourRGB(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

ColourRGB::ColourRGB(Vector3 &v)
{
    r = v[0];
    g = v[1];
    b = v[2];
}

float* ColourRGB::Start()
{
    return &r;
}

Vector3 ColourRGB::ToVector()
{
    return Vector3(r, g, b);
}