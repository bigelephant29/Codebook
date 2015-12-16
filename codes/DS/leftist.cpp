struct Node{
    Node *l, *r;
    int dis, val, size;
    Node(){}
    Node(int _val){
        l = NULL, r = NULL;
        dis = 0, val = _val, size = 1;
    }
    ~Node(){
        if(l) delete l;
        if(r) delete r;
    }
}*root[MAX];
Node* merge(Node *x, Node *y){
    if(x == NULL) return y;
    if(y == NULL) return x;
    if(y->val > x->val) swap(x, y);
    x->size += y->size;
    x->r = merge(x->r, y);
    if(x->l==NULL&&x->r) swap(x->l, x->r);
    else if(x->r && x->l->dis < x->r->dis)
        swap(x->l, x->r);
    if(x->r == NULL) x->dis = 0;
    else x->dis = x->r->dis + 1;
    return x;
}
