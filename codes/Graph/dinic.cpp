struct edge{
    int to, cap, flow, rev;
    edge(){}
    edge(const int &a, const int &b, const int &c, const int &d)
        : to(a), cap(b), flow(c), rev(d) {}
};
struct flow_network{
    int n, s, t;
    vector<vector<edge> > G;
    vector<bool> vis;
    vector<int> lv, dfs_j;
    void clear(){
        for(int i=0; i<(int)G.size(); i++) G[i].clear();
        G.clear();
        vis.clear();
        lv.clear();
        dfs_j.clear();
    }
    void resize(const int &n2){
        clear();
        n = n2;
        G.resize(n+1);
        for(int i=0; i<(int)G.size(); i++) G[i].clear();
        vis.resize(n+1);
        lv.resize(n+1);
        dfs_j.resize(n+1);
    }
    void add_edge(const int &a, const int &b, const int &c){
        G[a].push_back(edge(b, c, 0, (int)G[b].size()));
        G[b].push_back(edge(a, 0, 0, (int)G[a].size()-1));
    }
    bool bfs(){
        queue<int> q;
        fill(vis.begin(), vis.end(), false);
        vis[s] = true; lv[s] = 0;
        q.push(s);
        while(!q.empty()){
            int i = q.front(); q.pop();
            for(int j=0; j<(int)G[i].size(); j++){
                if(G[i][j].flow < G[i][j].cap &&
                        !vis[G[i][j].to]){
                    vis[G[i][j].to] = true;
                    lv[G[i][j].to] = lv[i] + 1;
                    q.push(G[i][j].to);
                }
            }
        }
        return vis[t];
    }
    int dfs(int i, int a){
        if(i == t) return a;
        int df = 0;
        for(int &j=dfs_j[i]; j<(int)G[i].size(); j++){
            if(lv[G[i][j].to] == lv[i] + 1 &&
                    G[i][j].flow < G[i][j].cap){
                int f = dfs(G[i][j].to,
                        min(a, G[i][j].cap - G[i][j].flow));
                G[i][j].flow += f;
                G[G[i][j].to][G[i][j].rev].flow -= f;
                df += f; a -= f;
                if(a == 0) break;
            }
        }
        return df;
    }
    int max_flow(const int &s2, const int &t2){
        s = s2, t = t2;
        int result = 0;
        while(bfs()){
            fill(dfs_j.begin(), dfs_j.end(), 0);
            result += dfs(s, INF);
        }
        return result;
    }
}fn;
