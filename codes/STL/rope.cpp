#include<ext/rope>
using namespace __gnu_cxx;
crope r;
int main(){
    r = crope();   
    r.size();       
    r.length();     
    r.begin();     
    r.end();    
    r.front(), r.back();
    r.push_front('A');
    r.push_back('B');
    r.pop_front();
    r.pop_back();
    r.insert(0, "ABC");
    r.erase(0, 0);   
    r.append("abc"); 
    r.replace(0, 0, crope());
    r.replace(0, 0, "ABC", 2);
    r.substr(0, 0);   
    return 0;
}

