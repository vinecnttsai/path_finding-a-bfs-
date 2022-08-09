#include <iostream>
#include <vector>
#include <math.h>
#include <queue>
#define max 52
using namespace std;
int dir[8][2]={{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
int enda=max-2,endb=max-2,starta=1,startb=1;

struct point
{
  int id=1,prev_a,prev_b;
};
point a[max][max];
void print_map()
{
    int i,k;
    for(i=0;i<max;i++)
    {
        for(k=0;k<max;k++)
        {
            if(a[i][k].id==0)cout<<"H";//無法走的路徑
            else if(a[i][k].id==1)cout<<" ";//還沒探詢的路徑
            else if(a[i][k].id==2)cout<<"O";//探過的路徑or open list
            else if(a[i][k].id==3)cout<<"◼︎";//最短路徑
            else cout<<"C";//close list
        }
        cout<<endl;
    }
}
bool slide(int posa,int posb,int posa2,int posb2)
{
    if(a[posa][posb2].id==0&&a[posa2][posb].id==0)return true;
    else return false;
}


//a*
struct point_a_star
{
    int g=0,h=0,f=0,a,b;
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
    if(posa==starta&&posb==startb)return;
    
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
            a[tempa][tempb]={.id=2,.prev_a=posa,.prev_b=posb};
            node[tempa][tempb]=
            {
                .g=distance_g(tempa,tempb,posa, posb)+node[posa][posb].g,
                .h=distance_h(tempa, tempb, enda, endb),
                .f=node[tempa][tempb].g+node[tempa][tempb].h,
                .a=tempa,
                .b=tempb
            };
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




//bfs
struct record_point
{
  int a,b;
};
vector<record_point> v;
void back_path(int posa,int posb)
{
    if(posa==starta&&posb==startb)return;
    
    a[posa][posb].id=3;//最短路徑
    back_path(a[posa][posb].prev_a,a[posa][posb].prev_b);
}
void bfs(int posa,int posb)
{
    int i,posa2,posb2;
    if(posa==enda&&posb==endb)
    {
        back_path(enda,endb);
        return;
    }
    else if(v.empty()&&!(posa==starta&&posb==startb))return;
    
    for(i=0;i<8;i++)
    {
        if(slide(posa,posb,posa+dir[i][0],posb+dir[i][1])&&i>3)continue;
        
        if(a[posa+dir[i][0]][posb+dir[i][1]].id==1)
        {
            a[posa+dir[i][0]][posb+dir[i][1]]={.id=2,.prev_a=posa,.prev_b=posb};
            v.push_back({posa+dir[i][0],posb+dir[i][1]});
        }
    }
    posa2=v.begin()->a;
    posb2=v.begin()->b;
    v.erase(v.begin());
    bfs(posa2,posb2);
    
}




int main()
{
    int i;
    for(i=0;i<max;i++)
    {
        a[0][i].id=0;
        a[i][0].id=0;
        a[max-1][i].id=0;
        a[i][max-1].id=0;
    }
    a[1][1].id=3;
    a_star(starta, startb);
    print_map();
    
    return 0;
}
