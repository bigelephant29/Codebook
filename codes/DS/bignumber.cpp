const int size = 10000;
const int BN_AZ= 1000;
struct BigNum
{
    int len,x[BN_AZ];
    void print(){
        printf("%d", x[len-1]);
        for(int i = len-2 ; i >= 0 ; i--)
            printf("%04d", x[i]);
    }
    void reset(){
        len = 1;
        memset(x, 0, sizeof(x));
    }
    void clear_trash(){
        while (len>1 && !x[len-1])
            len--;
    }
    void str_change(char s[]){
        reset();
        len = strlen(s) / 4;
        int remain = strlen(s)%4, j=0;
        if (remain) len++;
        else remain = 4;
        for(int i = 0 ; i < remain ; i++)
            x[len-1] = x[len-1]*10 + s[i]-'0';
        for(int i = len-2 ; i >= 0 ; i--){
            sscanf(s+remain+j, "%04d", &x[i]);
            j+=4;
        }
        clear_trash();
    }
    void int_change(long long num){
        reset();
        if (num==0) return;
        len = 0;
        while (num){
            x[len++] = num%size;
            num /= size;
        }
    }
    bool operator > (const BigNum &ag){
        if (len != ag.len) return len > ag.len;
        for(int i = len-1 ; i >= 0 ; i--)
            if (x[i] != ag.x[i])
                return x[i] > ag.x[i];
        return false;
    }
    bool operator < (const BigNum &ag){
        if (len != ag.len) return len < ag.len;
        for(int i = len-1 ; i >= 0 ; i--)
            if (x[i] != ag.x[i])
                return x[i] < ag.x[i];
        return false;
    }
    bool operator == (const BigNum &ag){
        if (len != ag.len) return false;
        for(int i = 0 ; i < len ; i++)
            if (x[i] != ag.x[i])
                return false;
        return true;
    }
    bool operator != (const BigNum &ag){
        return !(*this==ag);
    }
    bool operator >= (const BigNum &ag){
        return !(*this<ag);
    }
    bool operator <= (BigNum ag){
        return !(*this>ag);
    }
    BigNum (){ reset(); }
    BigNum (int num){ int_change(num); }
    BigNum (long long num){ int_change(num); }
    BigNum (char s[]){ str_change(s); }
    BigNum operator + (const BigNum &ag){
        BigNum ans;
        ans.reset();
        int l = std::max(ag.len,len);
        for(int i = 0 ; i < l ; i++){
            ans.x[i] += x[i] + ag.x[i];
            if (ans.x[i] >= size){
                ans.x[i+1]++;
                ans.x[i] -= size;
            }
        }
        if (ans.x[l]) l++;
        ans.len = l;
        ans.clear_trash();

        return ans;
    }
    BigNum operator - (const BigNum &ag){
        BigNum ans;
        ans.reset();
        int l = std::max(ag.len, len);
        for(int i = 0 ; i < l ; i++){
            ans.x[i] += x[i] - ag.x[i];
            if (ans.x[i] < 0){
                ans.x[i] += size;
                ans.x[i+1]--;
            }
        }
        ans.len = l;
        ans.clear_trash();
        return ans;
    }
    BigNum operator * (const BigNum &ag){
        BigNum ans;
        ans.reset();
        for(int i = 0 ; i < len ; i++){
            for(int j = 0 ; j < ag.len ; j++){
                int l = i+j;
                ans.x[l] += x[i] * ag.x[j];
                while (ans.x[l] >= size){
                    ans.x[l+1] += ans.x[l] / size;
                    ans.x[l++] %= size;
                }
            }
        }
        ans.len = len+ag.len+1;
        ans.clear_trash();
        return ans;
    }
    BigNum operator / (BigNum ag2){
        int l,r,m;
        BigNum ag1=*this, ans, tmp, minus;
        ans.len = ag1.len;
        for(int i = ag1.len-1 ; i >= 0 ; i--){
            l = 0;
            r = size;
            while (l < r){
                m = (l + r) / 2;
                tmp=ag2*BigNum(m);
                minus.reset();
                for(int j = 0 ; j < tmp.len ; j++)
                    minus.x[i+j] = tmp.x[j];
                minus.len = tmp.len + i;
                if (minus <= ag1)
                    l = m + 1;
                else
                    r = m;
            }
            tmp=ag2*BigNum(ans.x[i]=l-1);
            minus.reset();
            for(int j = 0 ; j < tmp.len ; j++)
                minus.x[i+j] = tmp.x[j];
            minus.len = tmp.len+i-1;
            ag1 = ag1-minus;
        }
        ans.clear_trash();
        return ans;
    }
    BigNum operator % (BigNum ag){
        return *this-*this/ag*ag;
    }
};
