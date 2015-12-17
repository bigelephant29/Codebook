int len, val[MAX<<1];
char rec[MAX];
char arr[MAX<<1];
int match(int l, int r){
    int t = 0;
    while( l-t >= 0 && r+t < len
           && arr[l-t] == arr[r+t] ) t++;
    return t;
}
void manacher(){
    memset(val, 0, sizeof(val));
    len = strlen(rec);
    arr[0] = '.';
    for(int i = 0 ; i < len ; i++){
        arr[(i<<1)+1] = rec[i];
        arr[(i<<1)+2] = '.';
    }
    len = (len<<1)+1;
    val[0] = 1;
    int l = 0, r = 0;
    for(int i = 1 ; i < len ; i++){
        int pst = l - (i-l);
        int dis = r-i+1;
        if(i>r){
            val[i] = match(i, i);
            l = i;
            r = i+val[i]-1;
        }else if(val[pst]==dis){
            val[i] = dis + match(i-dis, i+dis);
            l = i;
            r = i+val[i]-1;
        }else{
            val[i] = min(val[pst], dis);
        }
    }
}

