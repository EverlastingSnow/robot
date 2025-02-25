#ifndef STD_FILE
    #include<bits/stdc++.h>
    using namespace std;
    #define STD_FILE
#endif 

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3
#define N 200
#define MAXN 25
//^1代表相反方向

typedef pair<int,int>pii;


const int INF=-1;
const int oo=2e9;
const int left_dir[]={2,3,1,0};
const int right_dir[]={3,2,0,1};
const int DX[]={0,0,-1,1};
const int DY[]={1,-1,0,0};
const int robot_price[]={2000,5000},boat_price=8000;


char grid[N+10][N+10];
int occupy[N+10][N+10];
int goods_grid[N+10][N+10],robot_grid[N+10][N+10],boat_grid[N+10][N+10],berth_grid[N+10][N+10];
int berth_robot_nxt[N+10][N+10],berth_robot_dist[N+10][N+10];
int del_boat_nxt[N*N+10<<2],del_boat_dist[N*N+10<<2];
int berth_boat_nxt[MAXN+5][N*N+10<<2],berth_boat_dist[MAXN+5][N*N+10<<2];
int isnear[MAXN+5][MAXN+5];
int goods_time[N+5][N+5];
int capacity;
int robot_num,berth_num,boat_num;
int frame_id,money;

int all_get,all_goal,all_pull,all_boat,all_del;
int all_berth_del_dist;
int all_land_berth_dist;
int all_goods,all_land;

int goal_boat_num;
int goal_robot_num;

struct Coor{
    int x,y;
    Coor(int _x=-1,int _y=-1):x(_x),y(_y){}
    Coor& operator+=(const Coor&rhs);
    friend Coor operator +(const Coor&lhs,const Coor&rhs);
    Coor& operator-=(const Coor&rhs);
    friend Coor operator -(const Coor&lhs,const Coor&rhs);
};
const Coor DXY[]={{0,1},{0,-1},{-1,0},{1,0}};
vector<Coor>robot_purchase;
vector<Coor>boat_purchase;
vector<Coor>delivery;

struct Robot{
    int carry;
    int x,y;
    int nxt[N+10][N+10],has_goal;
    int cap;
    vector<int>bidx;
    void Lbot(int idx,int x,int y,int tp);
    void Move(int id,int dir);
    void Get(int id);
    void Pull(int id);
    friend istream& operator>>(istream&is,Robot&rhs);
    void DealRobot(int id,int&flag);
    Coor SearchGoods();
    void BfsFromGoods(int gx,int gy);
}robot[MAXN+5];

struct Berth{
    int x,y;
    int speed;
    int match;
    int goods_num;
    int S,dS;//occupyS
    friend istream&operator>>(istream&is,Berth&rhs);
    void Bfs(int berth_id);
}berth[MAXN+5];

struct Boat{
    int goods_num,goods_val;
    int x,y;
    int dir;
    int status;
    int match_berth,match_del;
    void EraseBoat();
    void FillBoat(int boat_id);
    array<int,3> Go(int _dir);
    void Lboat(int idx,int x,int y);
    void Dept(int id);
    void Berth(int id);
    bool Rot(int id,int d);
    bool Ship(int id);
    void DealBoat(int id);
}boat[MAXN+5];


bool InGrid(int x,int y);
bool LegalBoat(int x,int y,int dir);
bool IsLand(int x,int y);
bool IsSea(int x,int y);
bool IsMainLand(int x,int y);
bool IsMainSea(int x,int y);

//--------------------boat-----------------
int GetID(int x,int y,int dir);
int GetWeight(int x,int y,int dir);
array<int,3>GetState(int s);
array<int,3>Backward(int x,int y,int dir);
array<int,3>BackLeft(int x,int y,int dir);
array<int,3>BackRight(int x,int y,int dir);
//--------------Robot----------------------
bool occur(int x,const vector<int>&v);
//--------------------Init------------------
void Init();
void InitBerthRobotPath(int berth_id);
void InitDeliveryBoatPath();
//--------------------Input-----------------
void Input();
//--------------------Output----------------
void Output();
void DealRobot();
