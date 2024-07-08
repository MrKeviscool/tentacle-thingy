#include <iostream>
#include <chrono>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace std;

#define STEADY_CLOCK std::chrono::steady_clock

class circle{
    public:
    sf::CircleShape shape;
    float weight;
    sf::Vector2f getMid(){
        return(sf::Vector2f(this->shape.getPosition().x + this->shape.getRadius(), this->shape.getPosition().y + this->shape.getRadius()));
    }
    circle(){}
    circle(float diamiter, float weight, float x, float y) : weight(weight){
        this->shape = sf::CircleShape(diamiter/2);
        this->shape.setPosition(x, y);
    }
};


void display(), processEvent(sf::Event *event), getFPS(), addBalls(int amount, float weightDefault, float x, float y), mapRange();
const int FPSRESOLUTION = 4, WIDTH = 1920, HEIGHT = 1080, dx = WIDTH/2, dy = HEIGHT/2;
const sf::Vector2f aim(0, 0);
const float force = 100;

int fps = -1;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "psych balls", sf::Style::Fullscreen);

// circle balls[][5];
vector<circle> balls;
int main(){
    balls.push_back(circle(50, 1, dx, dy));
    balls.push_back(circle(50, 0.5, dx, dy+50));
    balls.push_back(circle(50, 0.25, dx, dy+100));
    while(window.isOpen()){
        getFPS();
        sf::Event event;
        processEvent(&event);

        for(int y = 0; y < balls.size(); y++){
            // float rot = -atan2f32(balls[y].getMid().x - aim.x, balls[y].getMid().y - aim.y)*180/M_PIf32; //goes to a point
            float rot = -atan2f32(balls[y].getMid().x - sf::Mouse::getPosition(window).x, balls[y].getMid().y - sf::Mouse::getPosition(window).y)*180/M_PIf32; //goes to mouse
            float xmv = sinf32((180 - rot) * (M_PIf32/180)) * force/(balls[y].weight*(float)fps);
            float ymv = cosf32((180 - rot) * (M_PIf32/180)) * force/(balls[y].weight*(float)fps);

            // cout << "index: " << y << " amount to move x: " << xmv << " y: " << ymv << endl;
            balls[y].shape.move(xmv, ymv);
        }

        display();
    }
    return 0;
}


void display(){
    window.clear();
    for(int y = 0; y < balls.size(); y++){
        window.draw(balls[y].shape);
    }
    window.display();
    
}

void processEvent(sf::Event *event){
    while(window.pollEvent(*event)){
        if(event->type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }
    }
}

void getFPS(){
    static STEADY_CLOCK::time_point fpstimer = STEADY_CLOCK::now();
    static int framesdone = 0;
    if(fps < 0){
        while(std::chrono::duration<float, std::milli>(STEADY_CLOCK::now() - fpstimer) <= std::chrono::milliseconds((1000/FPSRESOLUTION))){
            display();
            framesdone++;
        }
    }
    if(std::chrono::duration<float, std::milli>(STEADY_CLOCK::now() - fpstimer) >= std::chrono::milliseconds((1000/FPSRESOLUTION))){
        fps = framesdone * FPSRESOLUTION;
        framesdone = 0;
        fpstimer = STEADY_CLOCK::now();
        cout << "fps: " << fps << endl;
    }
    framesdone++;
}

void addBalls(int amount, float wMin, float wMax, float x, float y){
    // float weight = 
    for(int i = 0; i < amount; i++){
        float weight = ((wMax-wMin)/amount)*i+wMin;
        //give mapRange an empty float array;
        balls.push_back(circle(50, weight, WIDTH/2, HEIGHT/2));
    } 
}

void mapRange(){

}