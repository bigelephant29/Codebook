struct edge{int to,cap,next;}G[N*N];
int head[N],Gsz,s,t;
int lb[N*N],ub[N*N],diff[N];
queue<int>bfs;
int d[N];
bool visited[N];
int af;
void push_edge(int from,int to,int cap){
    G[++Gsz]=(edge){to,cap,head[from]};
    head[from]=Gsz;
    G[++Gsz]=(edge){from,0,head[to]};
    head[to]=Gsz;
}
bool dfs(int i,int a){
    visited[i]=1;
    if(i==t){
        af=a;
        return 1;
    }
    for(int j=head[i];j;j=G[j].next)
        if(G[j].cap&&d[G[j].to]>d[i]&&!visited[G[j].to]){
            if(dfs(G[j].to,min(a,G[j].cap))){
                G[j].cap-=af;
                G[(j-1^1)+1].cap+=af;
                return 1;
            }
        }
    return 0;
}
int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=m;i++){
        int from,to;
        scanf("%d%d%d%d",&from,&to,lb+i,ub+i);
        diff[from]+=lb[i];
        diff[to]-=lb[i];
        push_edge(from,to,ub[i]-lb[i]);
    }
    s=0,t=n+1;
    for(int i=1;i<=n;i++){
        if(diff[i]>0)push_edge(i,t,diff[i]);
        else if(diff[i]<0)push_edge(s,i,-diff[i]);
    }
    while(1){
        memset(visited,0,sizeof(visited));
        d[s]=0;
        bfs.push(s);
        visited[s]=1;
        while(!bfs.empty()){
            int i=bfs.front();
            bfs.pop();
            for(int j=head[i];j;j=G[j].next)
                if(G[j].cap&&!visited[G[j].to]){
                    d[G[j].to]=d[i]+1;
                    bfs.push(G[j].to);
                    visited[G[j].to]=1;
                }
        }
        if(!visited[t])break;
        while(1){
            memset(visited,0,sizeof(visited));
            if(!dfs(s,INF))break;
        }
    }
    for(int i=head[s];i;i=G[i].next)if(G[i].cap){
        puts("NO");
        return 0;
    }
    puts("YES");
    for(int i=1;i<=m;i++)
        printf("%d\n",ub[i]-G[2*i-1].cap);
    return 0;
}
