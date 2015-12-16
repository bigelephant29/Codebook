int t[20][MAX],sum[20][MAX];
int arr[MAX];
void build(int n, int l, int r){
    int mid = (l+r)>>1;
    int lm=0, ls=l, rs=mid+1;
    for(int i = mid; i >= l ; i--){
        if(arr[i]==arr[mid])
            lm++;
        else
            break;
    }
    for(int i = l ; i <= r ; i++){
        if(i==l)
            sum[n][i] = 0;
        else
            sum[n][i] = sum[n][i-1];
        if(t[n][i]==arr[mid]){
            if(lm){
                lm--;
                sum[n][i]++;
                t[n+1][ls++] = t[n][i];
            }else
                t[n+1][rs++] = t[n][i];
        }else if(t[n][i]<arr[mid]){
            sum[n][i]++;
            t[n+1][ls++] = t[n][i];
        }else
            t[n+1][rs++] = t[n][i];
    }
    if(l==r)
        return;
    build(n+1, l, mid);
    build(n+1, mid+1, r);
}
int query(int n, int l, int r, int ql, int qr, int k){
    int mid = (l+r)>>1;
    int s, ss;
    if(l==r)
        return t[n][l];
    if(ql==l)
        s = 0, ss = sum[n][qr];
    else
        s = sum[n][ql-1], ss = sum[n][qr]-s;
    if(k<=ss)
        return query(n+1, l, mid, l+s, l+sum[n][qr]-1, k);
    else
        return query(n+1, mid+1, r,
                mid+1-l+ql-s, mid+1-l+qr-sum[n][qr], k-ss);
}
void init(){
    input(arr);
    for(int i = 1 ; i <= n ; i++)
        t[0][i] = arr[i];
    sort(arr+1, arr+n+1);
    build(0, 1, n);
}
