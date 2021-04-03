#include <iostream>
#include "my_input.hpp"
#include <unistd.h>
#include <vector>
#include <time.h>

enum Direction{NONE = -1, STOP = 0,LEFT,RIGHT,UP,DOWN};

class Fruit{
    public:
    int x;
    int y;
    Fruit(int _x,int _y): x(_x),y(_y){}

    bool isFruit(int _x,int _y){
        
        return (x == _x && y == _y);
    }
};

bool isFruit(std::vector<Fruit> fruits,int _x,int _y){
    for(auto& elem:fruits){
        if(elem.x == _x && elem.y == _y)
            return true;
    }
    return false;
}

class Snake{
private:
    class Elem{
    public:
        int x;
        int y;
        Elem(): x(0),y(0){}
    };

    class Head: public Elem{
    public:
        Direction direction = UP;
        Head(): direction(UP){}
    };

    void toHead(){
        if(eatfruit){
            score++;
            size++;
            Elem add_Tail;
            tail.push_back(add_Tail);
            eatfruit = false;
        }
        for(int i = size-1;i > 0;--i){
            tail[i].x = tail[i-1].x;
            tail[i].y = tail[i-1].y;
        }
        tail[0].x = head.x;
        tail[0].y = head.y;
    }    

public:
    Head head;
    std::vector<Elem> tail;
    int size;
    bool eatfruit = false;
    int score = 0;

    Snake(int _x,int _y,int _size = 3){
        head.x = _x;
        head.y = _y;
        size = _size;
        tail.resize(_size);
        for(int i = 0; i < _size;++i){
            tail[i].x = _x;
            tail[i].y = _y - i - 1;
        }
    }

    bool isHead(int _x, int _y){
        return _x == head.x && _y == head.y;
    }

    bool isTail(int _x, int _y){
        for(auto& elem:tail){
            if(elem.x == _x && elem.y == _y)
                return true;
        }

        return false;
    }

    void step(){
        toHead();
        switch (head.direction){
        case UP:
            head.y++;
            break;
        case DOWN:
            head.y--;
            break;
        case LEFT:
            head.x--;
            break;
        case RIGHT:
            head.x++;
            break;
        }
    }

    bool collision(int _Height,int _Weight,std::vector<Fruit>& fruits){
        if(isFruit(fruits,head.x,head.y)){ 
            eatfruit = true;
            std::vector<Fruit>::iterator it = fruits.begin();
            for(;it!=fruits.end();++it){
                if(it->x == head.x && it->y == head.y)
                    break;
            }
            fruits.erase(it);
        }
        return (isTail(head.x,head.y) || head.x == 0 || head.x == _Weight-1|| head.y == 0 || head.y == _Height-1);
    }

};

class Screen{
private:
    int Height = 30;
    int Weight = 30;
    char Wall = '#';
    char Space = ' ';
    char Head = '@';
    char Tail = '*';
    char chFruit = '%';
public:

    Screen(int _Height = 30, int _Weight = 30, char _Wall = '#', char _Space = ' '): 
        Height(_Height), 
        Weight(_Weight),
        Wall(_Wall),
        Space(_Space) {}
    
    Screen(){}

    void update(Snake &snake,std::vector<Fruit>& fruits){
        std::system("clear");
        for(int i = 0; i < Height; ++i){
            for(int j = 0; j < Weight; ++j){
                if(i == 0 || j == 0 || i == Height - 1 || j == Weight - 1){
                    std::cout << Wall;
                } else if(snake.isHead(j,i)){
                    std::cout << Head;
                } else if(snake.isTail(j,i)){
                    std::cout << Tail;
                } else if(isFruit(fruits,j,i)){
                    std::cout << chFruit;
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

Direction Intput(){
    if(kbhit()){
        char ch;
        ch = getch();
        if(ch == 'w' || ch == 'W'){
            return DOWN;
        }else if(ch == 'd' || ch == 'D'){
            return RIGHT;
        }else if(ch == 'a' || ch == 'A'){
            return LEFT;
        }else if(ch == 's' || ch == 'S'){
            return UP;
        }else if(ch == 'q' || ch == 'Q'){
            return STOP;
        }
    }
    return NONE;
}



void addFruit(std::vector<Fruit>& fruits, Snake& snake,int Height,int Weight){
    if(fruits.size() > 5) return;
    int x = (rand() % (Height-2)) + 1;
    int y = (rand() % (Weight-2)) + 1;
    while(snake.isTail(x,y) || snake.isHead(x,y) || isFruit(fruits,x,y)){
        int x = (rand() % (Height-2)) + 1;
        int y = (rand() % (Weight-2)) + 1;
    }
    Fruit new_fruit(x,y);
    fruits.push_back(new_fruit);
}


int main(){
    srand (time(NULL));
    const int Height = 30;
    const int Weight = 30;
    Screen my_screen(Height,Weight);
    std::vector<Fruit> fruits;
    Snake snake(Height/2,Weight/2,3);
    int count = 0;
    while (true){
        my_screen.update(snake,fruits);
        snake.step();
        if(snake.collision(Height,Weight,fruits)){
            break;
        }
        Direction dir = Intput();
        if(dir != NONE){
            if(dir == STOP)
                break;
            if(((snake.head.direction == UP || snake.head.direction == DOWN) && dir != UP && dir != DOWN) ||
                ((snake.head.direction == RIGHT || snake.head.direction == LEFT) && dir != LEFT && dir != RIGHT))
                snake.head.direction = dir;
        }
        if(count % 10 == 0)
            addFruit(fruits,snake,Height,Weight);
        count++;
        usleep(250000);
    }
    std::system("clear");
    std::cout << "Your score: " << snake.score << std::endl; 
    return 0;
}