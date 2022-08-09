#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define max 52
using namespace std;
using namespace sf;
const int spacea=max,spaceb=max,n=30;
int dir[8][2]={{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
int starta=1,startb=1,enda=max-2,endb=max-2,rate=200;
RectangleShape re,line;
RenderWindow window(VideoMode(spacea*n, spaceb*n), "My window");
struct point
{
    int id,prev_a,prev_b,trigger;
};
point a[max][max];
void print_map()
{
    window.clear(Color::White);
    int i,k;
        for(i=0;i<max;i++)
        {
            for(k=0;k<max;k++)
            {
                if(a[i][k].id==0)
                {
                    re.setFillColor(Color::Black);
                }
                else if(a[i][k].id==1)
                {
                    re.setFillColor(Color::White);
                }
                else if(a[i][k].id==2)
                {
                    if(a[i][k].trigger<0)
                    {
                        a[i][k].trigger++;
                        re.setFillColor(Color::Cyan);
                    }
                    else
                    {
                        re.setFillColor(Color::Yellow);
                    }
                }
                else if(a[i][k].id==3)
                {
                    re.setFillColor(Color::Blue);
                }
                else if(a[i][k].id==4)
                {
                    re.setFillColor(Color::Red);
                }
                else if(a[i][k].id==5)
                {
                    re.setFillColor(Color::Green);
                }
                else
                {
                    re.setFillColor(Color::Magenta);
                }
                line.setPosition(Vector2f(n*i,n*k));
                re.setPosition(Vector2f(n*i,n*k));
                window.draw(re);
                window.draw(line);
                line.rotate(90);
                window.draw(line);
            }
        }
        window.display();
}
bool slide(int posa,int posb,int posa2,int posb2)
{
    if(a[posa][posb2].id==0&&a[posa2][posb].id==0)return true;
    else return false;
}


//bfs
struct record_point
{
  int a,b;
};
vector<record_point> v;
void back_path(int posa,int posb)
{
    if(posa==starta&&posb==startb)
    {
        a[posa][posb].id=5;
        print_map();
        return;
    }
    usleep(rate*10);
    print_map();
    a[posa][posb].id=3;//最短路徑
    back_path(a[posa][posb].prev_a,a[posa][posb].prev_b);
}
void bfs(int posa,int posb)
{
    int i,posa2,posb2;
    
    if(v.empty()&&!(posa==starta&&posb==startb))
    {
        cout<<"no solution"<<endl;
        return;
    }
    
    for(i=0;i<8;i++)
    {
        if(i%2==0)print_map();
        
        if(slide(posa,posb,posa+dir[i][0],posb+dir[i][1])&&i>3)continue;
        
        if(a[posa+dir[i][0]][posb+dir[i][1]].id==1||a[posa+dir[i][0]][posb+dir[i][1]].id==4)
        {
            if(posa+dir[i][0]==enda&&posb+dir[i][1]==endb)
            {
                back_path(posa,posb);
                return;
            }
            
            a[posa+dir[i][0]][posb+dir[i][1]].id=2;
            a[posa+dir[i][0]][posb+dir[i][1]].prev_a=posa;
            a[posa+dir[i][0]][posb+dir[i][1]].prev_b=posb;
            record_point temp;
            temp.a=posa+dir[i][0];
            temp.b=posb+dir[i][1];
            v.push_back(temp);
        }
    }
    posa2=v.begin()->a;
    posb2=v.begin()->b;
    v.erase(v.begin());
    bfs(posa2,posb2);
    
}





struct point_a_star
{
    int g,h,f,a,b;
};
point_a_star node[max][max];
struct cmp
{
    bool operator()(point_a_star a,point_a_star b)
    {
        if(a.f==b.f)
        {
            return a.h>b.h;
        }
        else
        {
            return a.f>b.f;
        }
    }
};
priority_queue<point_a_star,vector<point_a_star>,cmp> open_list;
int distance_h(int a1,int b1,int a2,int b2)
{
    return int(10*(abs(a1-a2)+abs(b1-b2)));
}
int distance_g(int a1,int b1,int a2,int b2)
{
    return int(10*sqrt(pow(a1-a2,2)+pow(b1-b2,2)));
}
void find_path(int posa,int posb)
{
    if(posa==starta&&posb==startb)
    {
        a[posa][posb].id=5;
        print_map();
        return;
    }
    
    print_map();
    a[posa][posb].id=3;
    find_path(a[posa][posb].prev_a,a[posa][posb].prev_b);
}
void a_star(int posa,int posb)
{
    int i,tempa,tempb;
    a[posa][posb].id=-1;
    
    if(open_list.empty()&&!(posa==starta&&posb==startb))return;
    
    for(i=0;i<8;i++)
    {
        tempa=posa+dir[i][0];
        tempb=posb+dir[i][1];
        
        if(slide(posa,posb,tempa,tempb)&&i>3)continue;
        
        print_map();
        usleep(500);
        if(a[tempa][tempb].id==2)
        {
            if(node[tempa][tempb].g>node[posa][posb].g+distance_g(tempa, tempb, posa, posb))
            {
                node[tempa][tempb].g=node[posa][posb].g+distance_g(tempa, tempb, posa, posb);
                a[tempa][tempb].prev_b=posb;
                a[tempa][tempb].prev_a=posa;
            }
        }
        else if(a[tempa][tempb].id==1)
        {
            a[tempa][tempb].id=2;
            a[tempa][tempb].prev_a=posa;
            a[tempa][tempb].prev_b=posb;
            node[tempa][tempb].g=distance_g(tempa,tempb,posa, posb)+node[posa][posb].g;
            node[tempa][tempb].h=distance_h(tempa, tempb, enda, endb);
            node[tempa][tempb].f=node[tempa][tempb].g+node[tempa][tempb].h;
            node[tempa][tempb].a=tempa;
            node[tempa][tempb].b=tempb;
            open_list.push(node[tempa][tempb]);
        }
        
        if(tempa==enda&&tempb==endb)
        {
            find_path(posa,posb);
            return;
        }
    }
    int temptop=open_list.top().a,temptop2=open_list.top().b;
    open_list.pop();
    a_star(temptop,temptop2);
}






void sfml(char cmd)
{
    int pa,pb;
    bool isstart=false,isend=false,edit=true;
    Vector2i pos;
    re.setSize(Vector2f(n,n));
    line.setSize(Vector2f((max-2)*n,2));
    line.setFillColor(Color::Black);
    while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if(!isstart)
        {
            if(Mouse::isButtonPressed(Mouse::Right))
            {
                pos=Mouse::getPosition(window);
                pa=pos.x/n;
                pb=pos.y/n;
                a[pa][pb].id=5;
                starta=pa;
                startb=pb;
                isstart=true;
            }
        }
        if(isstart&&!isend)
        {
            if(Mouse::isButtonPressed(Mouse::Right))
            {
                pos=Mouse::getPosition(window);
                if(starta!=pos.x/n||startb!=pos.y/n)
                {
                    pa=pos.x/n;
                    pb=pos.y/n;
                    a[pa][pb].id=4;
                    enda=pa;
                    endb=pb;
                    isend=true;
                }
            }
        }
        if(edit)
        {
            
            if(Mouse::isButtonPressed(sf::Mouse::Left))
            {
                pos=Mouse::getPosition(window);
                a[int(pos.x/n)][int(pos.y/n)].id=0;
            }
            else if(Keyboard::isKeyPressed(Keyboard::Space))
            {
                if(cmd=='a')a_star(starta,startb);
                else bfs(starta, startb);
                edit=false;
            }
        }
        print_map();
    }
}
void initialize()
{
    int i,k;
    for(i=0;i<max;i++)
    {
        for(k=0;k<max;k++)
        {
            a[i][k].id=1;
            a[i][k].trigger=-10;
            node[i][k].f=0;
            node[i][k].g=0;
            node[i][k].h=0;
        }
    }
    for(i=0;i<max;i++)
    {
        a[0][i].id=0;
        a[i][0].id=0;
        a[max-1][i].id=0;
        a[i][max-1].id=0;
    }
}



int main()
{
    char cmd;
    cin>>cmd;
    initialize();
    sfml(cmd);
return 0;
}
