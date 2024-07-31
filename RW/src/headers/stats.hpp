#include<vector>
#include<map>
#include<string>
#include<numeric>
#include<cmath>


double mean(std::vector<int> v) {
    return double(std::accumulate(v.begin(), v.end(), 0)) / double((v.size()));
}


double sd(std::vector<int> v)
{
    double mn = mean(v);

    double var = 0;
    for (int i = 0; i < v.size(); i++) {
        var = var + (v[i] - mn) * (v[i] - mn);
    }
    return std::sqrt(var / double(v.size()));

}