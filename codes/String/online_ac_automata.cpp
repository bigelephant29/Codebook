struct aca_node{
    bool word, trie_next[2];
    int next[2], fail, cnt;
};
struct ACA{
    int size;
    vector<aca_node> node;
    void init(int sz){
        size = 1;
        node.resize(sz);
        node[1] = (aca_node){false,false,false,0,0,0,0};
    }
    void trie_insert(const string &s){
        int i = 1;
        for(int is=0; is<s.size(); is++){
            int &next = node[i].next[s[is]-'0'];
            if(next == 0){
                next = ++size;
                node[i].trie_next[s[is]-'0'] = true;
                node[next] =
                    (aca_node){false,false,false,0,0,0,0};
            }
            i = next;
        }
        node[i].word = true;
    }
    void build(){
        queue<int> bfs;
        node[1].fail = 1;
        for(int i=0; i<=1; i++){
            if(node[1].trie_next[i]){
                node[node[1].next[i]].fail = 1;
                bfs.push(node[1].next[i]);
            }
            else node[1].next[i] = 1;
        }
        while(!bfs.empty()){
            int i = bfs.front(); bfs.pop();
            node[i].cnt = node[node[i].fail].cnt;
            if(node[i].word) node[i].cnt++;
            for(int j=0; j<=1; j++){
                if(node[i].trie_next[j]){
                    node[node[i].next[j]].fail = 
                        node[node[i].fail].next[j];
                    bfs.push(node[i].next[j]);
                }else{
                    node[i].next[j] = node[node[i].fail].next[j];
                }
            }
        }
    }
    bool in(const string &s){
        int i = 1;
        for(int is=0; is<s.size(); is++){
            if(node[i].trie_next[s[is]-'0'])
                i = node[i].next[s[is]-'0'];
            else return false;
        }
        return node[i].word;
    }
    ll query(const string &s){
        ll result = 0;
        for(int i=1, is=0; is<s.size(); is++){
            i = node[i].next[s[is]-'0'];
            result += node[i].cnt;
        }
        return result;
    }
    void clear(){
        node.clear();
    }
};
struct online_ACA{
    int size;
    vector<string> str;
    vector<ACA> bit;
    vector<bool> valid;
    void init(){
        size = 0;
        str.resize(1);
        bit.resize(1);
        bit[0].init(10); bit[0].build();
        valid.resize(1);
    }
    bool in(const string &s){
        for(int i=size; i>0; i-=i&-i){
            if(bit[i].in(s)) return true;
        }
        return false;
    }
    void insert(const string &s){
        valid.push_back(!in(s)); size++;
        str.push_back(s);
        ACA temp;;
        bit.push_back(temp);
        int sz = 0;
        for(int i=size-(size&-size)+1; i<=size; i++){
            if(valid[i]) sz += str[i].size();
        }
        bit.back().init(sz + 10);
        for(int i=size-(size&-size)+1; i<=size; i++){
            if(valid[i]) bit[size].trie_insert(str[i]);
        }
        bit[size].build();
    }
    ll query(const string &s){
        ll result = 0;
        for(int i=size; i>0; i-=i&-i)
            result += bit[i].query(s);
        return result;
    }
    void clear(){
        for(int i=0; i<str.size(); i++) str[i].clear();
        str.clear();
        for(int i=0; i<bit.size(); i++) bit[i].clear();
        bit.clear();
        valid.clear();
    }
};

