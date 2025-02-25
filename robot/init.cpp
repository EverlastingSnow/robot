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
#pragma optimize(2)

//---------------------------------------------------------------------init---------------------------------------------------------------------
void InitBerthRobotPath(){
    //---------------预处理泊位-机器人路径----------------
    memset(berth_robot_dist,INF,sizeof(berth_robot_dist));
    memset(berth_robot_nxt,INF,sizeof(berth_robot_nxt));
    queue<Coor>q;
    vector<vector<bool>>vis(N+10,vector<bool>(N+10));
    for(int row=0;row<N;row++)
    for(int col=0;col<N;col++){
        if(grid[row][col]=='B'){
            q.push({row,col});
            vis[row][col]=true;
            berth_robot_dist[row][col]=0;
            occupy[row][col]=berth_grid[row][col];
        }
    }
    while(!q.empty()){
        auto [sx,sy]=q.front();q.pop();
        int sd=berth_robot_dist[sx][sy];
        int oc=occupy[sx][sy];
        for(int dir=0;dir<4;dir++){
            int x=sx+DX[dir],y=sy+DY[dir];
            if(!InGrid(x,y)) continue;
            if(vis[x][y]) {
                int occ=occupy[x][y];
                isnear[oc][occ]=1;
                isnear[occ][oc]=1;
                continue;
            }
            if(!IsLand(x,y)) continue;
            vis[x][y]=true;
            berth_robot_dist[x][y]=sd+1;
            berth_robot_nxt[x][y]=dir^1;
            occupy[x][y]=oc;
            q.push({x,y});
        }
    }
}
void InitDeliveryBoatPath(){
    //--------------预处理交货点-船路径-----------------------
    memset(del_boat_dist,INF,sizeof(del_boat_dist));
    memset(del_boat_nxt,INF,sizeof(del_boat_nxt));
    typedef array<int,3> aaa;
    priority_queue<aaa,vector<aaa>,greater<aaa>>pq;
    vector<bool>vis(N*N+10<<2);
    for(int row=0;row<N;row++)
    for(int col=0;col<N;col++)
        if(grid[row][col]=='T'){
            for(int dir=0;dir<4;dir++)
                if(LegalBoat(row,col,dir)){
                    int id=GetID(row,col,dir);
                    pq.push({0,id,-1});
                }
        }
    while(!pq.empty()){
        auto [dist,es,dir]=pq.top();pq.pop();
        auto [ex,ey,ed]=GetState(es);
        if(vis[es]) continue;
        vis[es]=true;
        del_boat_dist[es]=dist;
        del_boat_nxt[es]=dir;
        //处理前进
        int sx,sy,sd,ss,w;
        sd=ed;
        sx=ex+DX[sd^1];
        sy=ey+DY[sd^1];
        ss=GetID(sx,sy,sd);
        w=GetWeight(sx,sy,sd)+dist;
        if(LegalBoat(sx,sy,sd)&&!vis[ss]){
            pq.push({w,ss,UP});
        }
        //处理右转/顺时针
        sd=left_dir[ed];
        sx=ex+DX[sd^1]+DX[sd^1];
        sy=ey+DY[sd^1]+DY[sd^1];
        ss=GetID(sx,sy,sd);
        w=GetWeight(sx,sy,sd)+dist;
        if(LegalBoat(sx,sy,sd)&&!vis[ss]){
            pq.push({w,ss,RIGHT});
        }
        //处理左转/逆时针
        sd=right_dir[ed];
        sx=ex+DX[sd^1]+DX[ed];
        sy=ey+DY[sd^1]+DY[ed];
        ss=GetID(sx,sy,sd);
        w=GetWeight(sx,sy,sd)+dist;
        if(LegalBoat(sx,sy,sd)&&!vis[ss]){
            pq.push({w,ss,LEFT});
        }
    }
}
void InitBerthBoatPath(int berth_id){
    //--------------预处理泊位-船路径-----------------------
    memset(berth_boat_dist[berth_id],INF,sizeof(berth_boat_dist[berth_id]));
    memset(berth_boat_nxt[berth_id],INF,sizeof(berth_boat_nxt[berth_id]));
    typedef array<int,3> aaa;
    priority_queue<aaa,vector<aaa>,greater<aaa>>pq;
    vector<bool>vis(N*N+10<<2);
    for(int row=0;row<N;row++)
    for(int col=0;col<N;col++)
    if(berth_grid[row][col]==berth_id){
        for(int dir=0;dir<4;dir++)
        if(LegalBoat(row,col,dir)){
            int s=GetID(row,col,dir);
            pq.push({0,s,-1});
        }
    }
    while(!pq.empty()){
        auto [dist,es,dir]=pq.top();pq.pop();
        auto [ex,ey,ed]=GetState(es);
        if(vis[es]) continue;
        vis[es]=true;
        berth_boat_dist[berth_id][es]=dist;
        berth_boat_nxt[berth_id][es]=dir;
        //处理前进
        int sx,sy,sd,ss,w;
        sd=ed;
        sx=ex+DX[sd^1];
        sy=ey+DY[sd^1];
        ss=GetID(sx,sy,sd);
        w=GetWeight(sx,sy,sd)+dist;
        if(LegalBoat(sx,sy,sd)&&!vis[ss]){
            pq.push({w,ss,UP});
        }
        //处理右转/顺时针
        sd=left_dir[ed];
        sx=ex+DX[sd^1]+DX[sd^1];
        sy=ey+DY[sd^1]+DY[sd^1];
        ss=GetID(sx,sy,sd);
        w=GetWeight(sx,sy,sd)+dist;
        if(LegalBoat(sx,sy,sd)&&!vis[ss]){
            pq.push({w,ss,RIGHT});
        }
        //处理左转/逆时针
        sd=right_dir[ed];
        sx=ex+DX[sd^1]+DX[ed];
        sy=ey+DY[sd^1]+DY[ed];
        ss=GetID(sx,sy,sd);
        w=GetWeight(sx,sy,sd)+dist;
        if(LegalBoat(sx,sy,sd)&&!vis[ss]){
            pq.push({w,ss,LEFT});
        }
    }
}
void Init(){
    memset(robot_grid,INF,sizeof(robot_grid));
    memset(occupy,INF,sizeof(occupy));
    memset(boat_grid,INF,sizeof(boat_grid));
    memset(goods_grid,0,sizeof(goods_grid));
    memset(berth_grid,INF,sizeof(berth_grid));
    for(int i=0;i<N;i++){
        scanf("%s",grid[i]);
    }
    scanf("%d",&berth_num);
    for(int i=0;i<berth_num;i++){
        int bid;cin>>bid;
        scanf("%d%d%d",&berth[bid].x,&berth[bid].y,&berth[bid].speed);
        berth[bid].match=INF;
        berth[bid].Bfs(bid);
    }
    cin>>capacity;
    for(int row=0;row<N;row++)
    for(int col=0;col<N;col++){
        if(grid[row][col]=='T'){
            delivery.push_back({row,col});
        }else if(grid[row][col]=='R'){
            robot_purchase.push_back({row,col});
        }else if(grid[row][col]=='S'){
            boat_purchase.push_back({row,col});
        }
    }
    InitBerthRobotPath();
    InitDeliveryBoatPath();
    for(int id=0;id<berth_num;id++){
        InitBerthBoatPath(id);
    }
    for(int i=0;i<berth_num;i++){
        int x=berth[i].x,y=berth[i].y;
        for(int dir=0;dir<4;dir++){
            if(LegalBoat(x,y,dir)){
                int id=GetID(x,y,dir);
                all_berth_del_dist+=del_boat_dist[id];
                break;
            }
        }
    }
    for(int row=0;row<N;row++)
    for(int col=0;col<N;col++)
    if(grid[row][col]=='.'){
        all_land++;
        all_land_berth_dist+=berth_robot_dist[row][col];
    }
    auto chu=[](int x,int y){
        return (x+y/2)/y;
    };
    goal_robot_num=chu(all_land+16000,3000);
    for(int row=0;row<N;row++)
    for(int col=0;col<N;col++)
    {
        int oc=occupy[row][col];
        if(oc!=INF){
            berth[oc].S++;
            berth[oc].dS+=berth_robot_dist[row][col];
        }
    }
    /*========这边是给机器人确定归属的berth的==================
    需要的参数berth_num,berth[].S,occupy(用来判断物理相邻，表示某一格陆地归属于哪个berth，非陆地为INF)
    robot[].idx 是一个向量，表示归属berth的集合
    S用于合并，dS用于分配
    */
    struct Block{
        vector<int>id;
        int S,dS;
        void merge(const Block&blk){
            copy(blk.id.begin(),blk.id.end(),back_inserter(id));
            S+=blk.S;
            dS+=blk.dS;
        }
        bool operator<(const Block&rhs)const{
            return S<rhs.S;
        }
    };
    auto isneighbor=[&](const Block&lhs,const Block&rhs){
        for(auto x:lhs.id)
        for(auto y:rhs.id){
            if(isnear[x][y]) return true;
        }
        return false;
    };
    vector<Block>v;
    for(int i=0;i<berth_num;i++){
        Block blk;
        blk.id.push_back(i);
        blk.S=berth[i].S;
        blk.dS=berth[i].dS;
        v.push_back(blk);
    }
    while(v.size()>1){
        sort(v.begin(),v.end());
        if(v.front().dS>180000) break;
        int sz=v.size();
        for(int i=1;i<sz;i++){
            if(isneighbor(v[0],v[i])){
                v[0].merge(v[i]);
                v.erase(v.begin()+i);
                break;
            }
        }
    }
    vector<int>cnt(v.size()),rnk(v.size());
    iota(rnk.begin(),rnk.end(),0);
    auto f=[&](int x){
            if(cnt[x]==0) return oo;
            return v[x].dS/cnt[x];
    };
    for(int i=0;i<MAXN;i++){
        sort(rnk.begin(),rnk.begin()+v.size(),[&](int lhs,int rhs){
            int x=f(lhs),y=f(rhs);
            if(x!=y){
                return x>y;
            }
            return berth[lhs].dS>berth[rhs].dS;
        });
        for(auto p:v[rnk[0]].id){
            robot[i].bidx.push_back(p);
        }
        cnt[rnk[0]]++;
    }
    //==========================================
    goal_boat_num=all_berth_del_dist/700+1;
    string ok;
    cin>>ok;
    cout<<"OK";
    fflush(stdout);
}
