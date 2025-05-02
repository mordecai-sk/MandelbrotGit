#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = { pixelWidth, pixelHeight };
    m_aspectRatio = pixelHeight / pixelWidth;
    m_plane_center = { 0, 0 };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_state = CALCULATING;

    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    if (m_state == CALCULATING)
    {
        for (int i = 0; i < m_pixel_size.y; i++)
        {
            for (int j = 0; j < m_pixel_size.x; j++)
            {
                m_vArray[j + i * m_pixel_size.x].position = { (static_cast<float>(j), static_cast<float>(i)) };
                int c = countIterations(mapPixelToCoords({ j, i }));
                Uint8 r, g, b;
                iterationsToRGB(c, r, g, b);
                m_vArray[j + i * m_pixel_size.x].color = { r, g, b };
            }
        }
        m_state = DISPLAYING;
    }
}

void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_state = CALCULATING;
}

void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_state = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set" << endl;
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << endl;
    ss << "Left-click to Zoom in" << endl;
    ss << "Right-click to Zoom out" << endl;
    text.setString(ss.str());
}

int ComplexPlane::countIterations(Vector2f coord)
{
    complex<double> c(coord.x, coord.y);
    complex<double> z = c;
    int i = 0;
    while(abs(z) < 2.0 && i < MAX_ITER)
    {
        z = z * z + c;
        i++;
    }
    return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER)
    {
        r = 0; 
        g = 0; 
        b = 0; // Black for points inside the set
    }
    else if (count > 53)
    {
        r = 255; 
        g = 255; 
        b = 0; // Bright yellow
    }
    else if (count > 43)
    {
        r = 255;
        g = 165 + (count - 44) * 9; // Transition from orange to yellow
        b = 0;
    }
    else if (count > 32)
    {
        r = 255 - (43 - count) * 10; // Red to orange
        g = 69;
        b = 0;
    }
    else if (count > 21)
    {
        r = 75;
        g = 0;
        b = 130 + (count - 22) * 6; // Indigo to violet
    }
    else if (count > 11)
    {
        r = 0;
        g = 191 - (21 - count) * 15; // Teal to cyan
        b = 255;
    }
    else
    {
        r = 0;
        g = 0;
        b = 255 - count * 20; // Deep blue to lighter blue
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    Vector2f coord;
    coord.x = (static_cast<float>(mousePixel.x) / (m_pixel_size.x)) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0);
    coord.y = (static_cast<float>(mousePixel.y - m_pixel_size.y) / -(m_pixel_size.y)) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0);
    return coord;
}