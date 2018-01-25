#include<cassert>
#include<cstring>
using namespace std;

template<class T> void sais(T *s, int n, int m, int *sa, int *b=0, int *p=0, bool *t=0){
#define SAIS_N 4000010
#define SAIS_M 128
    static int b0[SAIS_N+SAIS_M], p0[2*SAIS_N];
    static bool t0[2*SAIS_N];
    if(!b){
        b=b0, p=p0, t=t0;
        for(int i=0; i<=n-1; i++) assert(1<=s[i] && s[i]<=m-1);
        assert(s[n] == 0);
    }
    int *b2=b+m, *s2=p+n/2, *r=p+n, n2=0, m2=0;
    t[n-1] = 0;
    for(int i=n-2; i>=0; i--) t[i] = (s[i]==s[i+1])? t[i+1]: (s[i]<s[i+1]);
    for(int i=0; i<=n-1; i++) r[i] = (i>=1&&t[i]&&!t[i-1])? (p[n2]=i, n2++): (-1);
    memset(b, 0, m*sizeof(int));
    for(int i=0; i<=n-1; i++) b[(int)s[i]]++;
    for(int i=1; i<=m-1; i++) b[i] += b[i-1];
#define is(q) do{\
    memset(sa, 0, n*sizeof(int));\
    memcpy(b2, b, m*sizeof(int));\
    for(int i=n2-1; i>=0; i--) sa[--b2[(int)s[q[i]]]] = q[i];\
    b2[0]=0; memcpy(b2+1, b, (m-1)*sizeof(int));\
    sa[b2[(int)s[n-1]]++] = n-1;\
    for(int i=0; i<=n-1; i++) if(sa[i]&&!t[sa[i]-1]) sa[b2[(int)s[sa[i]-1]]++] = sa[i]-1;\
    memcpy(b2, b, m*sizeof(int));\
    for(int i=n-1; i>=0; i--) if(sa[i]&&t[sa[i]-1]) sa[--b2[(int)s[sa[i]-1]]] = sa[i]-1;\
}while(0)
    is(p);
    for(int i=0, j=-1, k=-1; i<=n-1; i++) if((j=r[sa[i]])>=0){
        int l = (k<0) || memcmp(s+p[j], s+p[k], ((j==n2-1)? (n-p[j]+1): (p[j+1]-p[j]+1))*sizeof(T));
        s2[k=j] = (m2+=l);
    }
    s2[n2] = 0;
    if(m2<n2) sais(s2, n2, m2+1, sa, b2, r, t+n);
    else for(int i=0; i<=n2-1; i++) sa[s2[i]-1] = i;
    for(int i=0; i<=n2-1; i++) s2[i] = p[sa[i]];
    is(s2);
#undef is
#undef SAIS_N
#undef SAIS_M
}
