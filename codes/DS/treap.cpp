struct Node{
    Node *l, *r;
    int v, size, HV;
    inline int make_rand(){
        return ((int)rand()<<30)+((int)rand()<<15)+rand();
    }
    Node(int _v = 0){
        v = _v;
        size = 1;
        l = NULL, r = NULL;
        HV = make_rand();
    }
    inline int l_size(){
        if(l) return l->size;
        else return 0;
    }
    void resize(){
        int tmp = 1;
        if(r) tmp += r->size;
        size = tmp + l_size();
    }
};
pair<Node*, Node*> split(Node *T, int x){
    if(T==NULL)
        return make_pair((Node*)NULL, (Node*)NULL);
    if(T->l_size()+1<=x){
        pair<Node*, Node*> tmp=split(T->r, x-T->l_size()-1);
        T->r = tmp.first;
        T->resize();
        return make_pair(T, tmp.second);
    }else{
        pair<Node*, Node*> tmp = split(T->l, x);
        T->l = tmp.second;
        T->resize();
        return make_pair(tmp.first, T);
    }
}
Node* merge(Node *T1, Node *T2){
    if(!T1) return T2;
    if(!T2) return T1;
    if(T1->HV>T2->HV){
        T1->r = merge(T1->r, T2);
        T1->resize();
        return T1;
    }else{
        T2->l = merge(T1, T2->l);
        T2->resize();
        return T2;
    }
}
