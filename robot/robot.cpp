#ifndef STD_FILE
    #include<bits/stdc++.h>
    using namespace std;
    #define STD_FILE
#endif 
#ifndef DEFINE_CPP_FILE
    #include "define.cpp"
    #define DEFINE_CPP_FILE
#endif
#ifndef IMPLEMENT_FILE
    #include "implement.cpp"
    #define IMPLEMENT_FILE
#endif
#pragma optimize(2)
//----------------------------------------------------------------------Robot---------------------------------------------------------------------------
istream& operator>>(istream&is,Robot&rhs){
    is>>rhs.carry>>rhs.x>>rhs.y;
    return is;
}
void Robot::Lbot(int idx,int _x,int _y,int _tp){
    x=_x;y=_y;
    cap=_tp+1;
    carry=0;
    if(robot_grid[x][y]!=INF) return;
    printf("lbot %d %d %d\n",x,y,_tp);
    robot_grid[x][y]=idx;
    money-=robot_price[_tp];
}
void Robot::Pull(int robot_idx){
    printf("pull %d\n",robot_idx);
    all_pull++;
    int id=berth_grid[x][y];
    if(id!=INF){
        berth[id].goods_num++;
    }
    has_goal=false;
    carry--;
}
void Robot::Get(int robot_idx){
    printf("get %d\n",robot_idx);
    carry++;
    all_get++;
    has_goal=false;
}
void Robot::Move(int robot_idx,int dir){
    int ex=x+DX[dir],ey=y+DY[dir];
    if(IsLand(ex,ey)&&robot_grid[ex][ey]==INF){
        printf("move %d %d\n",robot_idx,dir);
        robot_grid[ex][ey]=robot_idx;
        robot_grid[x][y]=INF;
        x=ex;y=ey;
    }else{
        vector<int>p{0,1,2,3};
        random_shuffle(p.begin(),p.end());
        for(int _i=0;_i<4;_i++){
            int i=p[_i];
            ex=x+DX[i],ey=y+DY[i];
            if(IsLand(ex,ey)&&robot_grid[ex][ey]==INF){
                printf("move %d %d\n",robot_idx,i);
                robot_grid[ex][ey]=robot_idx;
                robot_grid[x][y]=INF;
                x=ex;y=ey;
                break;
            }
        }
    }
}
Coor Robot::SearchGoods(){
    Coor res{INF,INF};
    int maxv=INF;
    vector<vector<bool>>vis(N+10,vector<bool>(N+10));
    vector<vector<int>>dist(N+10,vector<int>(N+10));
    queue<Coor>q;
    q.push({x,y});
    vis[x][y]=true;
    while(!q.empty()){
        auto [sx,sy]=q.front();q.pop();
        if(goods_grid[sx][sy]>0&&1000-(frame_id-goods_time[sx][sy])-dist[sx][sy]>10){
            int ds=0;
            int bbidx=occupy[sx][sy];
            int xx=berth[bbidx].x,yy=berth[bbidx].y;
            for(int i=0;i<4;i++){
                int id=GetID(xx,yy,i);
                if(del_boat_dist[id]!=INF){
                    ds=del_boat_dist[id];
                    break;
                }
            }
            if(frame_id+dist[sx][sy]+berth_robot_dist[x][y]+ds+10<15000){
                int v;
                if(all_berth_del_dist==628){
                    v=1000000*goods_grid[sx][sy]/(2*dist[sx][sy]+1);
                    if(occur(occupy[sx][sy],bidx)) v+=1000000000;
                }else{
                    if(occur(occupy[sx][sy],bidx)){
                        v=1000000*goods_grid[sx][sy]/(2*dist[sx][sy]+1);
                    }else{
                        v=1000000*goods_grid[sx][sy]/(2*dist[sx][sy]*dist[sx][sy]+1);
                }
                }
                if(v>maxv){
                    maxv=v;
                    res={sx,sy};
                }
            }
        }
        for(int dir=0;dir<4;dir++){
            int ex=sx+DX[dir],ey=sy+DY[dir];
            if(!InGrid(ex,ey)) continue;
            if(vis[ex][ey]) continue;
            if(!IsLand(ex,ey)) continue;
            vis[ex][ey]=true;
            q.push({ex,ey});
            dist[ex][ey]=dist[sx][sy]+1;
        }
    }
    if(maxv>0){
        return res;
    }
    return {INF,INF};
}
void Robot::BfsFromGoods(int gx,int gy){
    vector<vector<bool>>vis(N+10,vector<bool>(N+10));
    queue<Coor>q;
    memset(nxt,INF,sizeof(nxt));
    q.push({gx,gy});
    vis[gx][gy]=true;
    while(!q.empty()){
        auto [sx,sy]=q.front();q.pop();
        for(int dir=0;dir<4;dir++){
            int ex=sx+DX[dir],ey=sy+DY[dir];
            if(!InGrid(ex,ey)) continue;
            if(vis[ex][ey]) continue;
            if(!IsLand(ex,ey)) continue;
            vis[ex][ey]=true;
            q.push({ex,ey});
            nxt[ex][ey]=dir^1;
        }
    }
    has_goal=true;
}
void Robot::DealRobot(int robot_idx,int&flag){
    if(berth_grid[x][y]!=INF&&carry){
        Pull(robot_idx);
        return;
    }
    if(carry>=cap){
        if(berth_robot_nxt[x][y]!=INF){
            Move(robot_idx,berth_robot_nxt[x][y]);
        }
        return;
    }
    if(!has_goal){
        if(flag>3) return;
        auto [gx,gy]=SearchGoods();
        flag++;
        if(gx!=INF){
            Robot::BfsFromGoods(gx,gy);
            flag++;
            goods_grid[gx][gy]=0;
            all_goal++;
        }else{
            return;
        }
    }
    if(nxt[x][y]!=INF){
        Move(robot_idx,nxt[x][y]);
    }
    if(nxt[x][y]==INF){
        Robot::Get(robot_idx);
    }
}