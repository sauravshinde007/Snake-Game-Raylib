#include <raylib.h>
#include <iostream>
#include <deque> // Double Ended Queue for body of snake
#include <raymath.h>
using namespace std;



int cellSize = 30, cellCount = 25;
int width = cellSize*cellCount ;
int height = width;

double lastUpdatedTime = 0;
//utility function to reduce speed of any obj
bool eventTrigger(double interval){
    double currentTime = GetTime();
    if (currentTime - lastUpdatedTime >= interval)
    {
        lastUpdatedTime = currentTime;
        return true;
    }
    
    return false;
}

//will check whether a pos lies in dq
bool checkPos(Vector2 pos, deque<Vector2> dq){
    for (int i = 0; i < dq.size(); i++)
    {
        if(Vector2Equals(pos, dq[i])){
            return true;
        }
    }
    return false;
    
}

//Food
class Food{
    public:
        //position in cell counts
        Vector2 position;
        

        Food(deque<Vector2> snake_body){
            position = GenerateRandomPos(snake_body);
        }

        void Draw(){
            DrawRectangle(position.x*cellSize, position.y*cellSize, cellSize,cellSize,RED);
        }

        Vector2 GenerateRandomCell(){
            int x = GetRandomValue(0, cellCount-1);
            int y = GetRandomValue(0, cellCount-1);

            Vector2 finalPos = {x,y};
            return finalPos;
        }

        Vector2 GenerateRandomPos(deque<Vector2> snake_body){
            Vector2 pos = GenerateRandomCell();
            //check whether food pos and snake pos matches or not
            while(checkPos(pos,snake_body)){
                //generate new pos
                Vector2 pos = GenerateRandomCell();
            }

            return pos;
        }
};

//Snake
class Snake{
public:
    deque<Vector2> body = {Vector2{5,9},Vector2{6,9},Vector2{7,9} };
    //Intial direction
    Vector2 direction = {1,0};
    void Draw(){
        for(unsigned int i=0; i<body.size(); i++){
            float x = body[i].x*cellSize;
            float y = body[i].y*cellSize;
            Rectangle segment = Rectangle{x,y,(float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment,0.5,6,GREEN);
        }
        
    }

    void Update(){
        //logic we will remove the tail and add new head to body of snake to simulate moving snake
        body.pop_back();
        body.push_front(Vector2Add(body[0],direction));
    }
};


int main(){
    InitWindow(width,height,"Snake Game");
    SetTargetFPS(60);

    
    Snake snake = Snake();
    Food food = Food(snake.body);

    //Game Loop
    while (WindowShouldClose() == false)
    {
        
        if (eventTrigger(0.2))
        {
            snake.Update();
        }
        

        //snake Movement
        if(IsKeyPressed(KEY_LEFT) && snake.direction.x != 1){
            snake.direction = {-1,0};
        }
        if(IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1){
            snake.direction = {1,0};
        }
        if(IsKeyPressed(KEY_UP) && snake.direction.y != 1){
            snake.direction = {0,-1};
        }
        if(IsKeyPressed(KEY_DOWN) && snake.direction.y != -1){
            snake.direction = {0,1};
        }

        //if head of snake and food position matches then snake has eaten the food
        if(Vector2Equals(snake.body[0], food.position)){
            //respawn the food
            food.position = food.GenerateRandomPos(snake.body);
        }
        
        //Draw
        BeginDrawing();
        ClearBackground(BLACK);

        food.Draw();
        snake.Draw();

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}