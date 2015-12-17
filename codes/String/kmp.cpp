void kmp(){
    la=strlen(a);
    lb=strlen(b);
    next[0]=-1;
    int j=-1;
    for(int i = 1 ; i < la ; i++){
        while(j>=0 && a[i]!=a[j+1])j=next[j];
        if(a[i]==a[j+1])j++;
        next[i]=j;
    }
    j=-1;
    for(int i = 0 ; i < lb ; i++){
        while(j>=0 && a[j+1]!=b[i])j=next[j];
        if(a[j+1]==b[i])j++;
        if(j==la-1){
            //find answer
        }
    }
}
