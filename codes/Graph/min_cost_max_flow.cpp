struct edge{
    int to, cap, cost, flow, rev;
    edge(int a, int b, int c, int d, int e){
        to = a, cap = b, cost = c, flow = d, rev = e;
    }
};
struct weighted_flow_network{
    int n, s, t, mc, mf;
    vector<vector<edge> > G;
    vector<int> d, pv, pe, a;
    vector<bool> inq;
    queue<int> q;
    void clear(){
        for(int i=0; i<G.size(); i++) G[i].clear();
        G.clear();
        d.clear(), pv.clear(), pe.clear(), a.clear();
        inq.clear();
    }
    weighted_flow_network(int n2 = 0){
        clear();
        n = n2;
        G.resize(n+1);
        for(int i=0; i<G.size(); i++) G[i].clear();
        d.resize(n+1), pv.resize(n+1), pe.resize(n+1);
        a.resize(n+1);
        inq.resize(n+1);
    }
    void add_edge(int u, int v, int cap, int cost){
        G[u].push_back(edge(v, cap, cost, 0, G[v].size()));
        G[v].push_back(edge(u, 0, -cost, 0, G[u].size()-1));
    }
    bool spfa(){
        fill(d.begin(), d.end(), INF);
        fill(inq.begin(), inq.end(), false);
        d[s] = 0, pv[s] = s, a[s] = INF;
        q.push(s), inq[s] = true;
        while(!q.empty()){
            int i = q.front(); q.pop();
            inq[i] = false;
            for(int j=0; j<G[i].size(); j++){
                int to = G[i][j].to, flow = G[i][j].flow;
                int cap = G[i][j].cap, cost = G[i][j].cost;
                if(flow == cap || d[to] <= d[i] + cost) 
                    continue;
                d[to] = d[i] + cost;
                pv[to] = i, pe[to] = j;
                a[to] = min(a[i], cap - flow);
                if(!inq[to]){
                    q.push(to); inq[to] = true;
                }
            }
        }
        if(d[t] == INF) return false;
        mc += a[t] * d[t], mf += a[t];
        for(int i=t; i!=s; i=pv[i]){
            G[pv[i]][pe[i]].flow += a[t];
            G[i][G[pv[i]][pe[i]].rev].flow -= a[t];
        }
        return true;
    }
    int min_cost_max_flow(int s2, int t2){
        s = s2, t = t2, mc = mf = 0;
        while(spfa());
        return mc;
    }
}mcmf;
