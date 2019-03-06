#ifndef QST_HH
#define QST_HH

#include<cstddef>
#include<algorithm>
#include<exception>
#include<functional>
#include<utility>
#include<vector>

template<typename T, typename Less=std::less<T>> class Qst{
private:
    static constexpr std::size_t buffer_size(std::size_t n) noexcept{
        std::size_t ceillog2n = 0;
        for(std::size_t t=n; t; t>>=1) ++ceillog2n;
        if((n&(~n+1)) == n) --ceillog2n;
        return n*(ceillog2n+3);
    }
    Less lt;
    std::vector<std::size_t> si, ra, val, sl, pos;
    std::size_t size = 0;
    void build(std::size_t i, std::size_t l, std::size_t r) noexcept{
        if(i == 1){
            pos[1] = 0;
            std::copy(ra.begin(), ra.end(), val.begin()+1);
            size = ra.size()+1;
        }
        if(l == r) return;
        std::size_t m = l + (r-l)/2;
        pos[2*i] = size; size += m-l+2;
        pos[2*i+1] = size; size += r-m+1;
        sl[pos[i]] = 0;
        for(std::size_t j=1, k1=0, k2=0; j<=r-l+1; ++j){
            if(val[pos[i]+j] <= m){
                sl[pos[i]+j] = sl[pos[i]+j-1]+1;
                val[pos[2*i]+(++k1)] = val[pos[i]+j];
            }else{
                sl[pos[i]+j] = sl[pos[i]+j-1];
                val[pos[2*i+1]+(++k2)] = val[pos[i]+j];
            }
        }
        build(2*i, l, m);
        build(2*i+1, m+1, r);
    }
    std::size_t query_ra(std::size_t l, std::size_t r, std::size_t k, std::size_t i) const noexcept{
        if(l == r) return val[pos[i]+l];
        if(k <= sl[pos[i]+r]-sl[pos[i]+l-1]){
            return query_ra(sl[pos[i]+l-1]+1, sl[pos[i]+r], k, 2*i);
        }else{
            return query_ra(l-sl[pos[i]+l-1], r-sl[pos[i]+r], k-sl[pos[i]+r]+sl[pos[i]+l-1], 2*i+1);
        }
    }
public:
    Qst() = default;
    template<typename ...A> Qst(T const *a, std::size_t n, A &&...args): lt(std::forward<A>(args)...), si(n), ra(n), val(buffer_size(n)), sl(buffer_size(n)), pos(n<<2){
        if(!n) return;
        for(std::size_t i=0; i<n; ++i) si[i] = i;
        std::sort(si.begin(), si.end(), [this, a](std::size_t const &i, std::size_t const &j) -> bool{
            return lt(a[i], a[j]);
        });
        for(std::size_t i=0; i<n; ++i) ra[si[i]] = i;
        build(1, 0, n-1);
    }
    template<typename ...A> Qst(std::vector<T> const &a, A &&...args): Qst(a.data(), a.size(), std::forward<A>(args)...){}
    std::size_t query(std::size_t l, std::size_t r, std::size_t k) const{
        if(l>r || r>=ra.size() || k>r-l+1){
            throw std::runtime_error("Exception from Qst::query: Bad Interval\n");
        }
        return si[query_ra(l+1, r+1, k, 1)];
    }
};

#endif
