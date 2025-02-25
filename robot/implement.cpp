#ifndef STD_FILE
    #include<bits/stdc++.h>
    using namespace std;
    #define STD_FILE
#endif 
#ifndef DEFINE_CPP_FILE
    #include "define.cpp"
    #define DEFINE_CPP_FILE
#endif
//--------------------------------------------------------------------------implement--------------------------------------------------------------
inline bool InGrid(int x,int y){
    return x>=0&&x<N&&y>=0&&y<N;
}
inline bool IsLand(int x,int y){
    if(!InGrid(x,y)) return false;
    char ch=grid[x][y];
    return ch=='.'||ch=='>'||ch=='R'||ch=='B'||ch=='C'||ch=='c';
}
inline bool IsMainLand(int x,int y){
    if(!InGrid(x,y)) return false;
    char ch=grid[x][y];
    return ch=='>'||ch=='R'||ch=='B'||ch=='c';
}
inline bool IsSea(int x,int y){
    if(!InGrid(x,y)) return false;
    char ch=grid[x][y];
    return ch=='*'||ch=='~'||ch=='S'||ch=='B'||ch=='K'||ch=='C'||ch=='c'||ch=='T';
}
inline bool IsMainSea(int x,int y){
    if(!InGrid(x,y)) return false;
    char ch=grid[x][y];
    return ch=='~'||ch=='S'||ch=='B'||ch=='K'||ch=='c'||ch=='T';
}
//------------------------------------Coor-------------------------------------
Coor& Coor::operator+=(const Coor&rhs){
    x+=rhs.x;
    y+=rhs.y;
    return *this;
}
Coor& Coor::operator-=(const Coor&rhs){
    x-=rhs.x;
    y-=rhs.y;
    return *this;
}
Coor operator +(const Coor&lhs,const Coor&rhs){
    Coor res(lhs);
    res+=rhs;
    return res;
}
Coor operator -(const Coor&lhs,const Coor&rhs){
    Coor res(lhs);
    res-=rhs;
    return res;
}

istream& operator>>(istream&is,Berth&rhs){
    is>>rhs.x>>rhs.y>>rhs.speed;
    return is; 
}
//Berth
void Berth::Bfs(int berth_id){
    queue<Coor>q;
    q.push({x,y});
    berth_grid[x][y]=berth_id;
    while(!q.empty()){
        auto [sx,sy]=q.front();q.pop();
        for(int dir=0;dir<4;dir++){
            int ex=sx+DX[dir];
            int ey=sy+DY[dir];
            if(!InGrid(ex,ey)) continue;
            if(berth_grid[ex][ey]!=INF) continue;
            if(grid[ex][ey]!='B') continue;
            berth_grid[ex][ey]=berth_id;
            q.push({ex,ey});
        }
    }
}
//-------------------------Robot-----------------------
bool occur(int x,const vector<int>&v){
    for(auto y:v){
        if(x==y) return true;
    }
    return false;
}