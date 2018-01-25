#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 100010
int a[N], si[N], ra[N];
int cmp(const void *p, const void *q){
    int i = *(int*)p, j = *(int*)q;
    return (a[i]>a[j])-(a[i]<a[j]);
}

// QSTL = (ceil(log2(N))+3)*N = 20*N = 2000200
#define QSTL 2000200
int *val[4*N], *sl[4*N], _val[QSTL], _sl[QSTL], qstl;
void qst_build(int i, int l, int r){
    if(i == 1){
        val[1]=_val, sl[1]=_sl;
        memcpy(val[1]+1, ra+1, r*sizeof(int));
        qstl = r+1;
    }
    if(l == r) return;
    int m = (l+r)/2;
    val[2*i]=_val+qstl; sl[2*i]=_sl+qstl; qstl+=m-l+2;
    val[2*i+1]=_val+qstl; sl[2*i+1]=_sl+qstl; qstl+=r-m+1;
    sl[i][0] = 0;
    for(int j=1, k1=0, k2=0; j<=r-l+1; j++){
        if(val[i][j] <= m){
            sl[i][j] = sl[i][j-1]+1;
            val[2*i][++k1] = val[i][j];
        }else{
            sl[i][j] = sl[i][j-1];
            val[2*i+1][++k2] = val[i][j];
        }
    }
    qst_build(2*i, l, m);
    qst_build(2*i+1, m+1, r);
}
int qst_query(int L, int R, int K, int i){
    if(L == R) return val[i][L];
    if(K <= sl[i][R]-sl[i][L-1]){
        return qst_query(sl[i][L-1]+1, sl[i][R], K, 2*i);
    }else{
        return qst_query(L-sl[i][L-1], R-sl[i][R], K-sl[i][R]+sl[i][L-1], 2*i+1);
    }
}

int main(){
    int n, m;
    while(~scanf("%d%d", &n, &m)){
        for(int i=1; i<=n; i++) scanf("%d", a+i);
        for(int i=1; i<=n; i++) si[i] = i;
        qsort(si+1, n, sizeof(int), cmp);
        for(int i=1; i<=n; i++) ra[si[i]] = i;
        qst_build(1, 1, n);
        while(m--){
            int l, r, k;
            scanf("%d%d%d", &l, &r, &k);
            // find the k-th minimum of a[l...r]
            printf("%d\n", a[si[qst_query(l, r, k, 1)]]);
        }
    }
    return 0;
}
