#include "ComplexPlane.h"

int main() 
{
int pixelWidth = VideoMode::getDesktopMode().width / 2;
int pixelHeight = VideoMode::getDesktopMode().height / 2;
VideoMode vm(pixelWidth, pixelHeight);

RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    Font arial;
    if (!arial.loadFromFile("arial.ttf"))
    {
        cout << "Error loading font!" << endl;
        return 1;
    }

    Text ss;
    ss.setFont(arial);
    ss.setCharacterSize(14);
    ss.setFillColor(Color::White);

    ComplexPlane plane(pixelWidth, pixelHeight);

    while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
{
    if (event.type == Event::Closed)
    {
    window.close();
    }

            if (event.type == Event::MouseButtonPressed)
    {
    if (event.mouseButton.button == Mouse::Left)
    {
                    plane.zoomIn();
                    Vector2i mousePixel(event.mouseButton.x, event.mouseButton.y);
                    plane.setCenter(mousePixel);
                }
                if (event.mouseButton.button == Mouse::Right)
                {
                    plane.zoomOut();
                    Vector2i mousePixel(event.mouseButton.x, event.mouseButton.y);
                    plane.setCenter(mousePixel);
                }
            }

            if (event.type == Event::MouseMoved)
            {
                Vector2i mousePixel(event.mouseMove.x, event.mouseMove.y);
                plane.setMouseLocation(mousePixel);
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
    window.close();
    }
        }

        plane.updateRender();
        plane.loadText(ss);

        window.clear();
        window.draw(plane);
        window.draw(ss);
        window.display();

    }

    return 0;
}
