#include <ctime>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstdlib>

#include <SFML/Graphics.hpp>

typedef struct
{
    std::string hour;
    std::string mil_hour;
    std::string minute;
    std::string second;
    std::string day;
    std::string month;
    std::string year;
    std::string n_day;
    std::string n_month;
    std::string meridiem;
} Chrono;

const Chrono time_now()
{
    Chrono c;
    char buf[10];
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);

    strftime(buf,sizeof(buf),"%I",&tstruct);
    c.hour = buf;
    strftime(buf,sizeof(buf),"%H",&tstruct);
    c.mil_hour = buf;
    strftime(buf,sizeof(buf),"%M",&tstruct);
    c.minute = buf;
    strftime(buf,sizeof(buf),"%S",&tstruct);
    c.second = buf;
    strftime(buf,sizeof(buf),"%d",&tstruct);
    c.day = buf;
    c.day = std::to_string(std::stoi(c.day)); //to remove the zero before the number
    strftime(buf,sizeof(buf),"%m",&tstruct);
    c.month = buf;
    strftime(buf,sizeof(buf),"%Y",&tstruct);
    c.year = buf;
    strftime(buf,sizeof(buf),"%a",&tstruct);
    c.n_day = buf;
    strftime(buf,sizeof(buf),"%B",&tstruct);
    c.n_month = buf;
    strftime(buf,sizeof(buf),"%p",&tstruct);
    c.meridiem = buf;

    return c;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(400,200),"Good-looking Clock",sf::Style::None);
    sf::Color background(200,200,255);
    sf::Texture texSheet;
    sf::Sprite day;
    sf::Sprite midday;
    sf::Sprite afternoon;
    sf::Sprite night;
    sf::Text t_hour;
    sf::Text t_min;
    sf::Text t_mer;
    sf::Text t_dotw;
    sf::Text t_date;
    sf::Text colon;
    sf::Font digital7;
    sf::Font centgoth;
    sf::Clock fadeTimer;
    sf::Clock colonTimer;

    bool introDone=false;
    bool showColon=false;
    int opacity=0;

    if (!digital7.loadFromFile("resources/digital-7.ttf") ||
        !centgoth.loadFromFile("resources/centurygothic.ttf") ||
        !texSheet.loadFromFile("resources/time_phases.png")) window.close();

    window.setPosition(sf::Vector2i(10,10));

    day.setTexture(texSheet);
    day.setTextureRect(sf::IntRect(0,0,536,310));
    day.setColor(sf::Color(200,200,200,255));

    midday.setTexture(texSheet);
    midday.setTextureRect(sf::IntRect(536,0,1072,310));
    midday.setColor(sf::Color(200,200,200,255));

    afternoon.setTexture(texSheet);
    afternoon.setTextureRect(sf::IntRect(0,310,536,620));
    afternoon.setColor(sf::Color(200,200,200,255));

    night.setTexture(texSheet);
    night.setTextureRect(sf::IntRect(536,310,1072,620));
    night.setColor(sf::Color(200,200,200,255));

    t_hour.setFont(digital7);
    t_hour.setCharacterSize(100);
    t_hour.setPosition(57,18);
    t_hour.setFillColor(sf::Color(255,255,255,0));

    colon.setFont(digital7);
    colon.setCharacterSize(100);
    colon.setPosition(t_hour.getPosition().x+100,t_hour.getPosition().y);
    colon.setString(':');
    colon.setFillColor(sf::Color(255,255,255,0));

    t_min.setFont(digital7);
    t_min.setCharacterSize(100);
    t_min.setPosition(t_hour.getPosition().x+120,t_hour.getPosition().y);
    t_min.setFillColor(sf::Color(255,255,255,0));

    t_mer.setFont(digital7);
    t_mer.setCharacterSize(40);
    t_mer.setPosition(t_min.getPosition().x+100,t_min.getPosition().y+20);
    t_mer.setFillColor(sf::Color(255,255,255,0));

    t_dotw.setFont(centgoth);
    t_dotw.setCharacterSize(40);
    t_dotw.setPosition(t_mer.getPosition().x,t_mer.getPosition().y+40);
    t_dotw.setFillColor(sf::Color(255,255,255,0));

    t_date.setFont(centgoth);
    t_date.setCharacterSize(20);
    t_date.setPosition(t_hour.getPosition().x+40,t_hour.getPosition().y+110);
    t_date.setFillColor(sf::Color(255,255,255,0));

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        int oras = std::stoi(time_now().mil_hour);

        if (!introDone && fadeTimer.getElapsedTime().asMilliseconds()>5)
        {
            opacity++;
            t_hour.setFillColor(sf::Color(254,254,254,opacity));
            t_min.setFillColor(sf::Color(254,254,254,opacity));
            t_mer.setFillColor(sf::Color(254,254,254,opacity));
            t_dotw.setFillColor(sf::Color(254,254,254,opacity));
            t_date.setFillColor(sf::Color(254,254,254,opacity));
            colon.setFillColor(sf::Color(254,254,254,opacity));
            if (opacity >= 254) introDone=true;
            fadeTimer.restart();
        }

        t_hour.setString(time_now().hour);
        t_min.setString(time_now().minute);
        t_mer.setString(time_now().meridiem);
        t_dotw.setString(time_now().n_day);
        t_date.setString(time_now().n_month+" "+time_now().day+", "+time_now().year);

        if (colonTimer.getElapsedTime().asMilliseconds() > 500)
        {
            showColon = !showColon;
            colonTimer.restart();
        }

        window.clear(background);

        //draw objects
        if (oras >= 6 && oras < 11) window.draw(day);
        else if (oras >= 11 && oras < 16) window.draw(midday);
        else if (oras >= 16 && oras < 18) window.draw(afternoon);
        else window.draw(night);
        window.draw(t_hour);
        if (showColon) window.draw(colon);
        window.draw(t_min);
        window.draw(t_mer);
        window.draw(t_dotw);
        window.draw(t_date);

        window.display();
    }
}
