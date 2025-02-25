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
#ifndef ROBOT_FILE
    #include "robot.cpp"
    #define ROBOT_FILE
#endif
#pragma optimize(2)
//---------------------------------------------------------------------boat----------------------------------------------------------------------
inline int GetID(int x,int y,int dir){
    return (x*N+y<<2)+dir;
}
inline array<int,3> GetState(int s){
    int dir=s&3;
    s>>=2;
    int x=s/N;
    int y=s-x*N;
    return {x,y,dir};
}
int GetWeight(int x,int y,int dir){
    int curx=x,cury=y;
    for(int i=0;i<3;i++){
        if(IsMainSea(curx,cury)) return 2;
        curx+=DX[dir];
        cury+=DY[dir];
    }
    int rd=right_dir[dir];
    curx=x+DX[rd],cury=y+DY[rd];
    for(int i=0;i<3;i++){
        if(IsMainSea(curx,cury)) return 2;
        curx+=DX[dir];
        cury+=DY[dir];
    }
    return 1;
}
array<int,3> Boat::Go(int _dir){
    Coor coor{x,y};
    if(dir==_dir){
        coor+=DXY[dir];
    }else if(_dir==left_dir[dir]){
        coor+=DXY[dir];
        coor+=DXY[right_dir[dir]];
    }else if(_dir==right_dir[dir]){
        coor+=DXY[dir];
        coor+=DXY[dir];
    }else{
        return {INF,INF,INF};
    }
    return {coor.x,coor.y,_dir};
}
void Boat::EraseBoat(){
    int curx=x,cury=y;
    for(int i=0;i<3;i++){
        boat_grid[curx][cury]=INF;
        curx+=DX[dir];
        cury+=DY[dir];
    }
    int rd=right_dir[dir];
    curx=x+DX[rd],cury=y+DY[rd];
    for(int i=0;i<3;i++){
        boat_grid[curx][cury]=INF;
        curx+=DX[dir];
        cury+=DY[dir];
    }
}
void Boat::FillBoat(int boat_id){
    int curx=x,cury=y;
    for(int i=0;i<3;i++){
        boat_grid[curx][cury]=boat_id;
        curx+=DX[dir];
        cury+=DY[dir];
    }
    int rd=right_dir[dir];
    curx=x+DX[rd],cury=y+DY[rd];
    for(int i=0;i<3;i++){
        boat_grid[curx][cury]=boat_id;
        curx+=DX[dir];
        cury+=DY[dir];
    }
}

bool LegalBoat(int x,int y,int dir){
    int curx=x,cury=y;
    for(int i=0;i<3;i++){
        if(!InGrid(curx,cury)) return false;
        if(!IsSea(curx,cury)) return false;
        if(!IsMainSea(curx,cury)&&boat_grid[curx][cury]!=INF) return false;
        curx+=DX[dir];
        cury+=DY[dir];
    }
    int rd=right_dir[dir];
    curx=x+DX[rd],cury=y+DY[rd];
    for(int i=0;i<3;i++){
        if(!InGrid(curx,cury)) return false;
        if(!IsSea(curx,cury)) return false;
        if(!IsMainSea(curx,cury)&&boat_grid[curx][cury]!=INF) return false;
        curx+=DX[dir];
        cury+=DY[dir];
    }
    return true;
}
array<int,3>BackWard(int x,int y,int dir){
    int _dir=dir^1;
    int _x=x+DX[_dir],_y=y+DY[_dir];
    return {_x,_y,_dir};
}
array<int,3>BackLeft(int x,int y,int dir){
    auto coor=Coor{x,y}+DXY[dir]+DXY[left_dir[dir]];
    return {coor.x,coor.y,right_dir[dir]};
}
array<int,3>BackRight(int x,int y,int dir){
    int _dir=left_dir[dir];
    auto coor=Coor{x,y}+DXY[_dir^1]+DXY[_dir^1];
    return {coor.x,coor.y,_dir};
}
void Boat::Lboat(int idx,int bx,int by){
    if(LegalBoat(bx,by,RIGHT)){
        printf("lboat %d %d\n",bx,by);
        x=bx;y=by;dir=RIGHT;
        match_berth=INF;
        match_del=INF;
        money-=boat_price;
        FillBoat(idx);
    }
}
void Boat::Dept(int boat_idx){
    printf("dept %d\n",boat_idx);
    int&mb=match_berth;
    berth[mb].match=INF;
    mb=INF;
}
void Boat::Berth(int boat_idx){
    printf("berth %d\n",boat_idx);
    EraseBoat();
}
bool Boat::Rot(int idx,int d){
    int ex,ey,ed;
    if(d==0){
        ex=x+DX[dir]+DX[dir];
        ey=y+DY[dir]+DY[dir];
        ed=right_dir[dir];
    }else{
        int rd=right_dir[dir];
        ex=x+DX[dir]+DX[rd];
        ey=y+DY[dir]+DY[rd];
        ed=left_dir[dir];
    }
    EraseBoat();
    if(LegalBoat(ex,ey,ed)){
        printf("rot %d %d\n",idx,d);
        x=ex;y=ey;dir=ed;
        FillBoat(idx);
        return true;
    }
    FillBoat(idx);
    return false;
}
bool Boat::Ship(int idx){
    int ex,ey,ed;
    ex=x+DX[dir];
    ey=y+DY[dir];
    EraseBoat();
    if(LegalBoat(ex,ey,dir)){
        printf("ship %d\n",idx);
        x=ex;y=ey;
        FillBoat(idx);
        return true;
    }else{
        FillBoat(idx);
    }
    return false;
}
void Boat::DealBoat(int boat_id){
    if(status==1){
        FillBoat(boat_id);
        return;
    }
    if(status==2){
        int&mb=match_berth;
        if(berth[mb].goods_num==0||capacity<=goods_num){
            Boat::Dept(boat_id);
            return;
        }else if(frame_id+del_boat_dist[GetID(x,y,dir)]+10>15000){
            Boat::Dept(boat_id);
            return;
        }else{
            int d=min({berth[mb].goods_num,berth[mb].speed,capacity-goods_num});
            berth[mb].goods_num-=d;
            goods_num+=d;
            all_boat+=d;
            return;
        }
    }
    int s=GetID(x,y,dir);
    if(match_berth==INF&&match_del==INF){
        int maxv=INF,cs=INF;
        int n=berth_num;
        for(int i=0;i<n;i++){
            if(berth[i].match!=INF) continue;
            if(frame_id+del_boat_dist[s]+berth_boat_dist[cs][s]+10>15000) continue;
            int temp=1000000*min(capacity-goods_num,berth[i].goods_num)/(berth_boat_dist[i][s]*berth_boat_dist[i][s]+2);
            if(temp>maxv){
                maxv=temp;
                cs=i;
            }
        }
        int temp=1000000*goods_num/(del_boat_dist[s]*del_boat_dist[s]+2);
        if(temp>maxv){
            match_del=1;
        }else{
            match_berth=cs;
            berth[cs].match=boat_id;
        }
    }
    int has_act=0;
    if(match_berth!=INF){
        int d=berth_boat_nxt[match_berth][s];
        if(d==UP){
            has_act=Boat::Ship(boat_id);
        }else if(d==RIGHT){
            has_act=Boat::Rot(boat_id,0);
        }else if(d==LEFT){
            has_act=Boat::Rot(boat_id,1);
        }else if(d==INF){
            Boat::Berth(boat_id);
            has_act=1;
        }
    }else if(match_del!=INF){
        int d=del_boat_nxt[s];
        if(d==UP){
            has_act=Boat::Ship(boat_id);
        }else if(d==RIGHT){
            has_act=Boat::Rot(boat_id,0);
        }else if(d==LEFT){
            has_act=Boat::Rot(boat_id,1);
        }else if(d==INF){
            match_del=INF;
            has_act=1;
        }
    }else{
        return;
    }
    vector<int>p{0,1,2};
    random_shuffle(p.begin(),p.end());
    for(int _i=0;_i<3;_i++){
        if(has_act) break;
        int i=p[_i];
        if(i==0){
            has_act=Boat::Ship(boat_id);
        }else{
            has_act=Boat::Rot(boat_id,i-1);
        }
    }
}