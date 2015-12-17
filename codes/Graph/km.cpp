int weight[30][30];
int lx[30], ly[30], delta;
bool vx[30], vy[30];
int linky[30];
bool find(int now)
{
    vx[now] = true;
    for(int i=0; i<n; ++i)
    {
        if(vy[i]) continue;
        int t = lx[now] + ly[i] - weight[now][i];
        if(t==0)
        {
            vy[i] = true;
            if(linky[i]==-1||find(linky[i]))
            {
                linky[i] = now;
                return true;
            }
        }
        else if(!vy[i]) delta = min(delta, t);
    }
    return false;
}
int km()
{
    memset(linky, -1, sizeof(linky));
    memset(ly, 0, sizeof(ly));
    for(int i=0; i<n; ++i) lx[i] = -INF;
    for(int i=0; i<n; ++i)
        for(int j=0; j<n; ++j)
            lx[i] = max(lx[i], weight[i][j]);
    for(int i=0; i<n; ++i)
        while(1)
        {
            memset(vx, 0, sizeof(vx));
            memset(vy, 0, sizeof(vy));
            delta = INF;
            if(find(i)) break;
            for(int i=0; i<n; ++i)
            {
                if(vx[i]) lx[i] -= delta;
                if(vy[i]) ly[i] += delta;
            }
        }
    int ans = 0;
    for(int i=0; i<n; ++i) ans += lx[i] + ly[i];
    return ans;
}
