#ifndef UFDS_H
#define UFDS_H

#include <vector>
#include <set>

class UFDS
{
    public:
        UFDS(int n) : n(n)
        {
            vec.assign(n, 0);
            for(int i = 0; i < n; i++) vec[i] = i;

            comps = std::set<int>(vec.begin(), vec.end());
        }

        int Parent(int u)
        {
            if(vec[u] == u) return u;
            return vec[u] = Parent(vec[u]);
        }

        bool IsSameSet(int u, int v) { return Parent(u) == Parent(v); }

        void Union(int u, int v)
        {
            if(IsSameSet(u, v)) return;

            v = Parent(v);
            vec[v] = Parent(u);
            comps.erase(v);
        }

        const std::set<int>& Components() const
        {
            return comps;
        }

    private:
        int n;
        std::vector<int> vec;
        std::set<int> comps;
};

#endif
