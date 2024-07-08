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
    float maxDistance;
    sf::Vector2f getMid(){
        return(sf::Vector2f(this->shape.getPosition().x + this->shape.getRadius(), this->shape.getPosition().y + this->shape.getRadius()));
    }
    circle(){}
    circle(float diamiter, float weight, float maxDistance, float x, float y) : weight(weight), maxDistance(maxDistance){
        this->shape = sf::CircleShape(diamiter/2);
        this->shape.setPosition(x, y);
    }
};


void display(), processEvent(sf::Event *event), getFPS(), addBalls(int amount, float wMin, float wMax, float sMin, float sMax, float x, float y), mapRange(float *rangearr, float min, float max, int count);
float sqar(float in);
const int FPSRESOLUTION = 4, WIDTH = 1920, HEIGHT = 1080, dx = WIDTH/2, dy = HEIGHT/2;
const sf::Vector2f aim(0, 0);
const float force = 100;

int fps = -1;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "tentacle-thing", sf::Style::Fullscreen);

vector<circle> balls;
int main(){
    addBalls(200, 0.1, 3, 30, 100, WIDTH/2, HEIGHT/2);
    cout << "test: " << sqrtf32(sqar(3)*sqar(4)) << endl;
    while(window.isOpen()){
        getFPS();
        sf::Event event;
        processEvent(&event);

        for(int y = balls.size(); y > 0; y--){
            if(y == 0)
                continue;
            // float rot = -atan2f32(balls[y].getMid().x - aim.x, balls[y].getMid().y - aim.y)*180/M_PIf32; //goes to a point
            float rot = -atan2f32(balls[y].getMid().x - sf::Mouse::getPosition(window).x, balls[y].getMid().y - sf::Mouse::getPosition(window).y)*180/M_PIf32; //goes to mouse
            float xmv = sinf32((180 - rot) * (M_PIf32/180)) * force/(balls[y].weight*(float)fps);
            float ymv = cosf32((180 - rot) * (M_PIf32/180)) * force/(balls[y].weight*(float)fps);

            float curdist = sqrtf32(sqar(balls[y].getMid().x + xmv - balls[y-1].getMid().x) + sqar(balls[y].getMid().y + ymv - balls[y-1].getMid().y));
            if(curdist > balls[y].shape.getRadius()){
                // cout << "curdist: " << curdist << " max dist: " << balls[y].maxDistance << " index: " << y << " curmid: (" <<balls[y].getMid().x << ", " << balls[y].getMid().y << ") original mid: (" << balls[0].getMid().x << ", " << balls[0].getMid().y << ")"<< endl;
                continue;
            }

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

void addBalls(int amount, float wMin, float wMax, float sMin, float sMax, float x, float y){
    static float *weights, *sizes;
    weights = (float *)malloc(sizeof(float) * amount);
    sizes = (float *)malloc(sizeof(float) * amount);
    mapRange(weights, wMin, wMax, amount);
    mapRange(sizes, sMin, sMax, amount);
    float maxD;
    for(int i = 0; i < amount; i++){
        if(i == 0){
            balls.push_back(circle(sizes[i], weights[i], 0, WIDTH/2, HEIGHT/2));
            cout << "ball[" << 0 <<"] size: " << balls[i].shape.getRadius()/2 << " weight: " << balls[i].weight << " max distance: " << balls[i].maxDistance << endl;
            continue;
        }
        maxD = balls[i-1].maxDistance + sizes[i]/3; 
        balls.push_back(circle(sizes[i], weights[i], maxD, WIDTH/2, HEIGHT/2));
        cout << "ball[" << i <<"] size: " << balls[i].shape.getRadius()/2 << " weight: " << balls[i].weight << " max distance: " << balls[i].maxDistance << endl;
    } 
    free(weights);
    free(sizes);
}

void mapRange(float *rangearr, float min, float max, int count){
    for(int i = 0; i < count;  i++){
        rangearr[i] = ((max-min)/count)*(count-i)+min;
        cout << "rangearr["<<i<<"]: " << rangearr[i] << endl;
    }
    cout << "\\\\\\\\\n";
}
float sqar(float in){
    return in*in;
}