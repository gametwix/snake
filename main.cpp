#include <iostream>
#include "my_input.hpp"
#include <unistd.h>
#include <vector>

enum Direction{STOP = 0,LEFT,RIGHT,UP,DOWN};

class Screen{
private:
    int Height = 30;
    int Weight = 30;
    char Wall = '#';
    char Space = ' ';

public:

    Screen(int _Height, int _Weight, char _Wall, char _Space): 
        Height(_Height), 
        Weight(_Weight),
        Wall(_Wall),
        Space(_Space) {}
    
    Screen(){}

    void update(){
        std::system("clear");
        for(int i = 0; i < Height; ++i){
            for(int j = 0; j < Weight; ++j){
                if(i == 0 || j == 0 || i == Height - 1 || j == Weight - 1){
                    std::cout << Wall;
                }
                else{
                    std::cout << Space;
                }
            }
            std::cout << std::endl;
        }
        static int count = 0;
        std::cout << count << std::endl;
        ++count;
    }

};

class Snake{
private:
    class Elem{
    int x;
    int y;

    Elem(int _x,int _y): x(_x),y(_y){}
    };

    class Head:Elem{
        Direction direction = UP;
    };    

public:
    Head head;
    std::vector<Elem> Tail;
};

int main(){
    Screen my_screen;
    while (true){
        my_screen.update();
    }
    return 0;
}