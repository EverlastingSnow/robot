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
#ifndef BOAT_FILE
    #include "boat.cpp"
    #define BOAT_FILE
#endif
#ifndef INIT_FILE
    #include "init.cpp"
    #define INIT_FILE
#endif
#pragma optimize(2)

//------------------------------------------------------------------input--------------------------------------------------------------
void Input(){
    scanf("%d",&money);
    int K;
    scanf("%d",&K);
    while(K--){
        int x,y,v;
        scanf("%d%d%d",&x,&y,&v);
        goods_grid[x][y]=v;
        goods_time[x][y]=frame_id;
        if(v>0){all_goods++;}
    }
    scanf("%d",&robot_num);
    for(int i=0;i<robot_num;i++){
        int id;scanf("%d",&id);
        scanf("%d%d%d",&robot[id].carry,&robot[id].x,&robot[id].y);
    }
    scanf("%d",&boat_num);
    for(int i=0;i<boat_num;i++){
        int id;scanf("%d",&id);
        Boat&b=boat[id];
        int last=b.goods_num;
        scanf("%d",&b.goods_num);
        if(b.goods_num==0) all_del+=last;
        scanf("%d%d",&b.x,&b.y);
        scanf("%d%d",&b.dir,&b.status);
    }
    char ok[20];
    scanf("%s",ok);
}
int goal_robot_num0=16;
int goal_robot_num1=0;
int robot_num0,robot_num1;
//------------------------------------------------------------------------output-------------------------------------------------------------
void Output(){
    //robot
    int flag=0;
    if(money>=robot_price[0]&&robot_num0<goal_robot_num0){
        int n=robot_purchase.size();
        int min_dist=oo;
        int bidx=robot[robot_num].bidx[0];
        for(int ridx=0;ridx<n;ridx++){
            auto [rx,ry]=robot_purchase[ridx];
            min_dist=min(min_dist,abs(rx-berth[bidx].x)+abs(ry-berth[bidx].y));
        }
        for(int ridx=0;ridx<n;ridx++)
        {
            auto [rx,ry]=robot_purchase[ridx];
            if(min_dist==abs(rx-berth[bidx].x)+abs(ry-berth[bidx].y)){
                robot[robot_num].Lbot(robot_num,robot_purchase[ridx].x,robot_purchase[ridx].y,0);
                robot_num0++;
                break;
            }
        }
    }
    else if(money>=robot_price[1]&&robot_num1<goal_robot_num1){
        int n=robot_purchase.size();
        int min_dist=oo;
        int bidx=robot[robot_num].bidx[0];
        for(int ridx=0;ridx<n;ridx++){
            auto [rx,ry]=robot_purchase[ridx];
            min_dist=min(min_dist,abs(rx-berth[bidx].x)+abs(ry-berth[bidx].y));
        }
        for(int ridx=0;ridx<n;ridx++)
        {
            auto [rx,ry]=robot_purchase[ridx];
            if(min_dist==abs(rx-berth[bidx].x)+abs(ry-berth[bidx].y)){
                robot[robot_num].Lbot(robot_num,robot_purchase[ridx].x,robot_purchase[ridx].y,1);
                robot_num1++;
                break;
            }
        }
    }
    memset(robot_grid,INF,sizeof(robot_grid));
    for(int i=0;i<robot_num;i++){
        int x=robot[i].x,y=robot[i].y;
        robot_grid[x][y]=i;
    }
    for(int i=0;i<robot_num;i++){
        robot[i].DealRobot(i,flag);
    }
    //boat
    memset(boat_grid,INF,sizeof(boat_grid));
    if(money>=boat_price&&(boat_num<1||robot_num>=goal_robot_num&&boat_num<goal_boat_num)){
        int n=boat_purchase.size();
        int idx=rand()%n;
        boat[idx].Lboat(boat_num,boat_purchase[idx].x,boat_purchase[idx].y);
    }
    for(int i=0;i<boat_num;i++){
        boat[i].FillBoat(i);
    }
    for(int i=0;i<boat_num;i++){
        boat[i].DealBoat(i);
    }
    printf("OK\n");
    fflush(stdout);
}
/*
map1 14 1
map2 14 2


4.8 111479 118986 131395 361,860
//100000
*/
int main(){
    srand(0);
    Init();
    while(scanf("%d",&frame_id)!=EOF){
        Input();
        Output();
    }
    freopen("out.txt","w",stdout);
    /*for(int row=0;row<N;row++){
        for(int col=0;col<N;col++){
            printf("%5d",berth_robot_dist[row][col]);
        }
        printf("\n");
    }*/
    printf("all_goal=%d,all_get=%d,all_pull=%d,all_boat=%d,all_del=%d\n",all_goal,all_get,all_pull,all_boat,all_del);
    printf("all_berth_del_dist=%d\n",all_berth_del_dist);
    printf("all_goods=%d,all_land=%d,all_land_berth_dist=%d\n",all_goods,all_land,all_land_berth_dist);
    for(int i=0;i<berth_num;i++){
        printf("berth[%d].S=%d,dS=%d\n",i,berth[i].S,berth[i].dS);
    }
    printf("all_land=%d,all_land_berth_dist=%d\n",all_land,all_land_berth_dist);
    for(int i=0;i<robot_num;i++){
        printf("robot[%d].bidx:",i);
        for(auto p:robot[i].bidx){
            cout<<p<<' ';
        }
        cout<<'\n';
    }
    for(int i=0;i<robot_num;i++){
        printf("robot[%d].cap=%d\n",i,robot[i].cap);
    }
    for(int i=0;i<berth_num;i++){
        for(int j=0;j<berth_num;j++){
            printf("%d ",isnear[i][j]);
        }
        printf("\n");
    }
    return 0;
}
/*
map1 14 1
map2 14 2

14个机器人
4.8 111479 118986 131395 361,860
    120277 113351 131468 365
15个机器人
    121368 117094 132338 371
    120580
4.10 分块 
            平方    非平方
mp1 13 1 122583     125251
mp2 14 2 117235     120752
mp3 16 1 136924     135491

4.11
mp1 12 1 124621
mp2 16 2 121983 
mp3 16 1 135491

all_berth_del_dist
map1    628
map2    1200
map3    360

        all_goods   all_land    all_land/all_goods  all_land_berth_dist all_dist/all_land
map1    2465        20698       8.39                1108029             53.53
map2    2485        32275       12.98               2124629             65.82
map3    2487        31574       12.69               1903659             60.29

4.11 修改了Move函数：增加MainLand判断
增加机器人在找不到领地goods的情况下找其他区域goods的选择
修改boat寻路为平方,不去最后会错过交货点的berth
试图修改robot最后的货物选择
增加goal_robot_num计算式
robot不会选择过去之后会消失的goods
boat::temp参数
robot_num
boat_num
robot::v参数（搜索货物）
flag
init分块


mp1
mp2
*/