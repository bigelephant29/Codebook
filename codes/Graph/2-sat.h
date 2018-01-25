#include<cstring>
#include<vector>
using namespace std;

#define V 110
class twosat{
private:
    int v, ts[2*V], scc[2*V], cnt, m, b[2*V], r[2*V];
    vector<int> g[2*V], grev[2*V], dag[2*V];
    bool vis[2*V];
    char loli[2*V];
    void dfs(vector<int> *g, int i){
        vis[i] = true;
        for(int j=0; j<(int)g[i].size(); j++){
            if(!vis[g[i][j]]){
                dfs(g, g[i][j]);
            }
        }
        ts[--cnt] = i;
    }
    void dfsrev(int i){
        vis[i] = true;
        scc[i] = m;
        for(int j=0; j<(int)grev[i].size(); j++){
            if(!vis[grev[i][j]]){
                dfsrev(grev[i][j]);
            }
        }
    }
    void clear(){
        for(int i=0; i<2*v; i++){
            g[i].clear();
            grev[i].clear();
            dag[i].clear();
        }
    }
public:
    void reset(const int &v=0){
        clear();
        this->v = v;
    }
    void add_cond(int i, bool bi, int j, bool bj){
        int a = 2*i+(int)bi, b = 2*j+(int)bj;
        g[a^1].push_back(b);
        g[b^1].push_back(a);
        grev[b].push_back(a^1);
        grev[a].push_back(b^1);
    }
    bool solve(bool *sol = NULL){
        memset(vis, false, sizeof(vis));
        cnt = 2*v;
        for(int i=0; i<2*v; i++){
            if(!vis[i]){
                dfs(g, i);
            }
        }
        memset(vis, false, sizeof(vis));
        m = 0;
        for(int i=0; i<2*v; i++){
            if(!vis[ts[i]]){
                dfsrev(ts[i]);
                m++;
            }
        }
        for(int i=0; i<v; i++){
            if(scc[2*i] == scc[2*i+1]){
                return false;
            }
        }
        if(sol){
            for(int i=0; i<2*v; i++) for(int j=0; j<(int)g[i].size(); j++){
                dag[scc[i]].push_back(scc[g[i][j]]);
            }
            memset(vis, 0, sizeof(vis));
            cnt = m;
            for(int i=0; i<m; i++){
                if(!vis[i]){
                    dfs(dag, i);
                }
            }
            for(int i=0; i<m; i++){
                r[ts[i]] = i;
            }
            memset(b, 0, sizeof(b));
            for(int i=0; i<2*v; i++){
                b[r[scc[i]]]++;
            }
            for(int i=1; i<m; i++){
                b[i] += b[i-1];
            }
            for(int i=0; i<2*v; i++){
                ts[--b[r[scc[i]]]] = i;
            }
            memset(loli, 0, sizeof(loli));
            for(int i=2*v-1; i>=0; i--){
                if(!loli[scc[ts[i]]]){
                    loli[scc[ts[i]]] = 1;
                    loli[scc[ts[i]^1]] = 2;
                }
            }
            for(int i=0; i<v; i++){
                sol[i] = (loli[scc[2*i]] == 2);
            }
        }
        return true;
    }
};
#undef V
