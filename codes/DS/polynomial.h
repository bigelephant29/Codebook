#include<cmath>
#include<cassert>
#include<complex>
#include<algorithm>
#include<vector>
#include<type_traits>
using namespace std;

#define FFT_LEN 131072
#ifndef M_PI
const double M_PI = acos(-1);
#endif
vector<complex<double>> fft(const vector<complex<double>> &x, const int &inv){
    static bool fft_ready = false;
    static complex<double> loli[FFT_LEN];
    int n = x.size();
    assert((n&-n)==n && n<=FFT_LEN && abs(inv)==1);
    if(!fft_ready){
        for(int k=0; k<FFT_LEN; k++){
            loli[k] = exp(complex<double>(0, 2*M_PI*k/FFT_LEN));
        }
        fft_ready = true;
    }
    vector<complex<double>> X = x;
    for(int i=1, j=0; i<n; i++){
        for(int k=n>>1; !((j^=k)&k); k>>=1);
        if(i < j){
            swap(X[i], X[j]);
        }
    }
    for(int i=2; i<=n; i*=2){
        int d = (inv==1)? FFT_LEN-(FFT_LEN/i): FFT_LEN/i;
        for(int j=0; j<n; j+=i){
            for(int k=0, a=0; k<i/2; k++, a=(a+d)%FFT_LEN){
                complex<double> s = X[j+k], t = loli[a] * X[j+k+i/2];
                X[j+k] = s + t;
                X[j+k+i/2] = s - t;
            }
        }
    }
    if(inv == -1){
        for(int i=0; i<(int)X.size(); i++){
            X[i] /= n;
        }
    }
    return X;
}

#define NTT_LEN FFT_LEN
static const long long nttp = (15<<27)+1;
vector<long long> ntt(const vector<long long> &x, const int &inv){
    static const long long pr = 31;
    static bool ntt_ready = false;
    static long long loli[NTT_LEN];
    int n = x.size();
    assert((n&-n)==n && n<=NTT_LEN && abs(inv)==1);
    for(int i=0; i<=n-1; i++) assert(0<=x[i] && x[i]<=nttp-1);
    if(!ntt_ready){
        loli[0] = loli[1] = 1;
        for(int i=1; i<=15; i++){
            loli[1] = pr*loli[1]%nttp;
        }
        for(int i=(1<<27)/NTT_LEN; i>1; i>>=1){
            loli[1] = loli[1]*loli[1]%nttp;
        }
        for(int i=2; i<NTT_LEN; i++){
            loli[i] = loli[1]*loli[i-1]%nttp;
        }
        ntt_ready = true;
    }
    vector<long long> X = x;
    for(int i=1, j=0; i<n; i++){
        for(int k=n>>1; !((j^=k)&k); k>>=1);
        if(i < j) swap(X[i], X[j]);
    }
    for(int i=2; i<=n; i*=2){
        int d = (inv==1)? NTT_LEN-(NTT_LEN/i): NTT_LEN/i;
        for(int j=0; j<n; j+=i){
            for(int k=0, a=0; k<i/2; k++, a=(a+d)%NTT_LEN){
                long long s = X[j+k], t = loli[a]*X[j+k+i/2]%nttp;
                X[j+k] = (s+t)%nttp;
                X[j+k+i/2] = (s-t+nttp)%nttp;
            }
        }
    }
    if(inv == -1){
        for(int i=0; i<n; i++){
            X[i] = X[i]*(nttp-(nttp-1)/n)%nttp;
        }
    }
    return X;
}

template<class Z> class polynomial{
    static_assert(is_integral<Z>::value, "Usage: polynomial<Z> for an integer type Z.\n");
private:
    vector<Z> a;
    Z m;
    polynomial<Z> osoi_kakezan(const polynomial<Z> &b) const{
        assert(m == b.m);
        int d1=deg(), d2=b.deg();
        if(d1<0 || d2<0){
            return polynomial<Z>(0, m);
        }
        polynomial<Z> result(d1+d2+1, m);
        for(int i=0; i<=d1; i++) for(int j=0; j<=d2; j++){
            if(m){
                result[i+j] = (result[i+j]+a[i]*b[j]%m)%m;
            }else{
                result[i+j] += a[i]*b[j];
            }
        }
        result.trunc();
        return result;
    }
    pair<polynomial<Z>, polynomial<Z>> osoi_warizan(const polynomial<Z> &b) const{
        assert(m == b.m);
        polynomial<Z> r=*this, g=b;
        r.trunc(); g.trunc();
        int d1=r.deg(), d2=g.deg();
        assert(d2>=0 && abs(g[d2])==1);
        if(d1 < d2){
            return make_pair(polynomial<Z>(0, m), r);
        }
        polynomial<Z> q(d1-d2+1, m);
        for(int i=d1-d2; i>=0; i--){
            q[i] = g[d2] * r[d2+i];
            for(int j=0; j<=d2; j++){
                if(m){
                    r[i+j] = (r[i+j]+(m-q[i])*g[j]%m)%m;
                }else{
                    r[i+j] -= q[i]*g[j];
                }
            }
        }
        r.trunc();
        return make_pair(q, r);
    }
public:
    polynomial(const size_t &n=0, const Z &m=0): m(m){
        assert(m>=0 && m!=1);
        a = vector<Z>(n);
    }
    polynomial(const vector<Z> &coef, const Z &m=0): m(m){
        assert(m>=0 && m!=1);
        a = coef;
    }
    void resize(const size_t &n){
        a.resize(n);
    }
    size_t size() const{
        return a.size();
    }
    Z& operator [](const int &i){
        assert(0<=i && i<(int)a.size());
        return a[i];
    }
    const Z& operator [](const int &i) const{
        assert(0<=i && i<(int)a.size());
        return a[i];
    }
    Z mod() const{
        return m;
    }
    int deg() const{
        int d = (int)size()-1;
        if(m){
            for(; d>=0&&a[d]%m==0; d--);
        }else{
            for(; d>=0&&!a[d]; d--);
        }
        return d;
    }
    void trunc(){
        if(m){
            for(int i=0; i<(int)size(); i++){
                a[i] = (a[i]%m+m)%m;
            }
        }
        while(!a.empty() && !a.back()){
            a.pop_back();
        }
    }
    polynomial<Z> operator +(const polynomial<Z> &b) const{
        assert(m == b.m);
        polynomial<Z> result = *this;
        if(size() < b.size()){
            result.resize(b.size());
        }
        for(int i=0; i<(int)b.size(); i++){
            result[i] += b[i];
        }
        result.trunc();
        return result;
    }
    polynomial<Z> operator +=(const polynomial<Z> &b){
        return (*this) = (*this) + b;
    }
    polynomial<Z> operator -() const{
        polynomial<Z> result = *this;
        result.trunc();
        for(int i=0; i<(int)result.size(); i++){
            if(m){
                result[i] = m-result[i];
            }else{
                result[i] = -result[i];
            }
        }
        result.trunc();
        return result;
    }
    polynomial<Z> operator -(const polynomial<Z> &b) const{
        return (*this) + (-b);
    }
    polynomial<Z> operator -=(const polynomial<Z> &b){
        return (*this) = (*this) - b;
    }
    polynomial<Z> operator *(const Z &c) const{
        polynomial<Z> result = *this;
        Z c2 = c;
        if(m) c2 %= m;
        for(int i=0; i<(int)result.size(); i++){
            result[i] *= c2;
        }
        result.trunc();
        return result;
    }
    polynomial<Z> operator *=(const Z &c){
        return (*this) = (*this) * c;
    }
    /*
    polynomial<Z> karatsuba(const polynomial<Z> &b) const{
        assert(m == b.m);
        int d1=deg(), d2=b.deg();
        if(d1<=64 || d2<=64){
            return osoi_kakezan(b);
        }
        int n = max(d1, d2)/2 + 1;
        polynomial<Z> A(n, m), B(n, m), C(n, m), D(n, m);
        for(int i=0; i<=n-1&&i<(int)size(); i++) B[i] = a[i];
        for(int i=0; i<=n-1&&n+i<(int)size(); i++) A[i] = a[n+i];
        for(int i=0; i<=n-1&&i<(int)b.size(); i++) D[i] = b[i];
        for(int i=0; i<=n-1&&n+i<(int)b.size(); i++) C[i] = b[n+i];
        A.trunc(); B.trunc(); C.trunc(); D.trunc();
        polynomial<Z> E=A.karatsuba(C), F=B.karatsuba(D), G=(A+B).karatsuba(C+D)-E-F;
        polynomial<Z> result(d1+d2+1, m);
        for(int i=0; i<(int)E.size(); i++) result[2*n+i] += E[i];
        for(int i=0; i<(int)G.size(); i++) result[n+i] += G[i];
        for(int i=0; i<(int)F.size(); i++) result[i] += F[i];
        result.trunc();
        return result;
    }
    */
    polynomial<Z> operator *(const polynomial<Z> &b) const{
        assert(m == b.m);
        int d1=deg(), d2=b.deg();
        if(d1<=16 || d2<=16){
            return osoi_kakezan(b);
        }
        int n = d1+d2+1;
        for(; (n&-n)!=n; n+=n&-n);
        vector<complex<double>> x(n), y(n);
        for(int i=0; i<=d1; i++) x[i] = a[i];
        for(int i=0; i<=d2; i++) y[i] = b[i];
        x = fft(x, 1); y = fft(y, 1);
        for(int i=0; i<=n-1; i++) x[i] *= y[i];
        x = fft(x, -1);
        vector<long long> xi(n), eta(n);
        for(int i=0; i<=d1; i++){
            xi[i] = (a[i]%nttp+nttp)%nttp;
        }
        for(int i=0; i<=d2; i++){
            eta[i] = (b[i]%nttp+nttp)%nttp;
        }
        xi = ntt(xi, 1); eta = ntt(eta, 1);
        for(int i=0; i<=n-1; i++) xi[i] = xi[i]*eta[i]%nttp;
        xi = ntt(xi, -1);
        polynomial<Z> result(n, m);
        for(int i=0; i<=n-1; i++){
            long long q = floor((x[i].real()-xi[i])/nttp+.5);
            if(!m){
                if(q < 0){
                    result[i] = -(-q-1)*nttp-(nttp-xi[i]);
                }else{
                    result[i] = (Z)q*nttp+xi[i];
                }
            }else{
                result[i] = ((q%m)*(nttp%m))%m+xi[i]%m;
            }
        }
        result.trunc();
        return result;
    }
    polynomial<Z> operator *=(const polynomial<Z> &b){
        return (*this) = (*this) * b;
    }
    polynomial<Z> operator /(const Z &c) const{
        polynomial<Z> result = (*this);
        for(int i=0; i<(int)result.size(); i++){
            result[i] /= c;
        }
        result.trunc();
        return result;
    }
    polynomial<Z> operator /=(const Z &c){
        return (*this) = (*this) / c;
    }
    polynomial<Z> operator %(const Z &c) const{
        polynomial<Z> result = (*this);
        for(int i=0; i<(int)result.size(); i++){
            result[i] = (result[i]%c+c)%c;
        }
        result.trunc();
        return result;
    }
    polynomial<Z> operator %=(const Z &c){
        return (*this) = (*this) % c;
    }
    polynomial<Z> rev() const{
        polynomial<Z> result = *this;
        result.trunc();
        reverse(result.a.begin(), result.a.end());
        result.trunc();
        return result;
    }
    polynomial<Z> invmodxn(const int &n) const{
        assert(n>0 && deg()>=0 && abs(a[0])==1);
        if(n == 1){
            polynomial<Z> b(1, m);
            b[0]=a[0]; return b;
        }
        int l = (n+1)/2;
        polynomial<Z> p = invmodxn(l);
        polynomial<Z> a0(l, m), a1(l, m);
        for(int i=0; i<=l-1&&i<(int)size(); i++){
            a0[i] = a[i];
        }
        a0.trunc();
        for(int i=0; i<=l-1&&l+i<(int)size(); i++){
            a1[i] = a[l+i];
        }
        a1.trunc();
        polynomial<Z> a0p = a0*p;
        polynomial<Z> b(l, m);
        for(int i=0; i<=l-1&&l+i<(int)a0p.size(); i++){
            b[i] = a0p[l+i];
        }
        b.trunc();
        polynomial<Z> q = a1*p+b;
        q.resize(l);
        q *= -p;
        polynomial<Z> result(n, m);
        for(int i=0; i<=l-1&&i<(int)p.size(); i++){
            result[i] = p[i];
        }
        for(int i=l; i<=n-1&&i-l<(int)q.size(); i++){
            result[i] = q[i-l];
        }
        result.trunc();
        return result;
    }
    polynomial<Z> operator /(const polynomial<Z> &b) const{
        assert(m == b.m);
        int d1 = deg(), d2 = b.deg();
        assert(d2 >= 0);
        if(d1 < d2){
            return polynomial<Z>(0, m);
        }
        if(d2 <= 32){
            return osoi_warizan(b).first;
        }
        polynomial<Z> rf=rev(), rg=b.rev(), rq=rf*rg.invmodxn(d1-d2+1);
        polynomial<Z> q(d1-d2+1, m);
        for(int i=d1-d2; i>=0&&d1-d2-i<(int)rq.size(); i--){
            q[i] = rq[d1-d2-i];
        }
        return q;
    }
    polynomial<Z> operator /=(const polynomial<Z> &b){
        return (*this) = (*this) / b;
    }
    polynomial<Z> operator %(const polynomial<Z> &b) const{
        assert(m == b.m);
        int d1 = deg(), d2 = b.deg();
        assert(d2 >= 0);
        if(d1 < d2){
            polynomial<Z> r=*this; r.trunc();
            return r;
        }
        if(d2 <= 32){
            return osoi_warizan(b).second;
        }
        return (*this)-b*((*this)/b);
    }
    polynomial<Z> operator %=(const polynomial<Z> &b){
        return (*this) = (*this) % b;
    }
    pair<polynomial<Z>, polynomial<Z>> division(const polynomial<Z> &b) const{
        assert(m == b.m);
        int d1 = deg(), d2 = b.deg();
        assert(d2 >= 0);
        if(d1 < d2){
            polynomial<Z> r=*this; r.trunc();
            return make_pair(polynomial<Z>(0, m), r);
        }
        if(d2 <= 32){
            return osoi_warizan(b);
        }
        polynomial<Z> q = (*this)/b, r = (*this)-b*q;
        return make_pair(q, r);
    }
};
template<class Z> polynomial<Z> operator *(const Z &c, const polynomial<Z> &f){
    return f*c;
}