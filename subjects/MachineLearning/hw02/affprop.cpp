#include <bits/stdc++.h>
using namespace std;

#define SQR(x) ((x)*(x))

double INF = 1e15;

const int MAXN = 500;
double s[MAXN][MAXN];
double r[MAXN][MAXN];
double a[MAXN][MAXN];
double tmp_r[MAXN][MAXN];
double tmp_a[MAXN][MAXN];

struct affinity_propagation
{

    affinity_propagation(vector<double> const& x_coord, 
            vector<double> const& y_coord, int max_iter_,
            int consecutive_max_unchanged_)
        : n(x_coord.size())
        , max_iter(max_iter_)
        , consecutive_max_unchanged(consecutive_max_unchanged_)
    {

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) 
            {
                s[i][j] = metric_squared_error(make_pair(x_coord[i], y_coord[i]),
                        make_pair(x_coord[j], y_coord[j]));
                cout << i << " " << j << " " << s[i][j] << endl;
            }

        for (int i = 0; i < n; i++)
        {
            s[i][i] = -1.0;
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                r[i][j] = a[i][j] = 0;
            }
    }


    vector<int> get_clusters()
    {
        auto clusters = calc_clusters();

        int it = 0;
        int unchanged = 0;
        while (unchanged < consecutive_max_unchanged and it < max_iter)
        {
            ++it;
            cout << "ITERATION " << it << endl;

            update_values();

            auto new_clusters = calc_clusters();

            if (new_clusters == clusters)
            {
                unchanged++;
            } 
            else 
            {
                unchanged = 1;
            }
        }

        return clusters;
    }

private:

    void print_r()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << r[i][j] << " \n"[j==n-1];
            }
        }
        cout << endl;
    }

    void update_r()
    {
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < n; k++)
            {
                double mx_val = -INF;
                for (int m = 0; m < n; m++)
                {
                    if (m != k)
                    {
                        mx_val = max(mx_val, a[i][m] + s[i][m]);
                    }
                }

                r[i][k] = s[i][k] - mx_val;
            }
        }
    }


    void update_a()
    {
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < n; k++)
            {
                if (k == i)
                {
                    double sum_vals = 0;
                    for (int j = 0; j < n; j++)
                    {
                        if (j != i) 
                        {
                            sum_vals += max<double>(0, r[j][k]);
                        }
                    }
                    a[i][k] = sum_vals;
                }
                else
                {
                    double sum_vals = 0;
                    for (int j = 0; j < n; j++) 
                    {
                        if (j != k and j != i)
                        {
                            sum_vals += max<double>(0, r[j][k]);
                        }
                    }
                    a[i][k] = min<double>(0, r[k][k] + sum_vals);
                }
            }
        }
    }

    vector<int> calc_clusters()
    {
        map<int, vector<int>> point_to_cluster;

        for (int i = 0; i < n; i++)
        {
            int opt_k = i;
            for (int k = 0; k < n; k++)
            {
                if (false and i == 100) 
                {
                    cout << a[i][k] + r[i][k] << endl;
                }
                if (a[i][k] + r[i][k] > a[i][opt_k] + r[i][opt_k])
                {
                    opt_k = k;
                }
            }

            point_to_cluster[opt_k].push_back(i);
        }

        vector<int> clusters(n, 0);

        int cl_num = 0;
        for (auto const& kv: point_to_cluster)
        {
            if (kv.second.empty())
            {
                continue;
            }
            
            cl_num++;
            for (const auto &u: kv.second)
            {
                clusters[u] = cl_num;
            }
        }

        return  clusters; 
    }

    void update_values()
    {
        update_r();
        update_a();

        /*
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                if (abs(a[i][j] -tmp_a[i][j]) > 1e-5)
                    cout << i << " "<< j << endl;
                a[i][j] = tmp_a[i][j];
                if (abs(r[i][j] -tmp_r[i][j]) > 1e-5)
                    cout << i << " "<< j << endl;
                r[i][j] = tmp_r[i][j];
            }
            */
    }

    double metric_squared_error(pair<double, double> p1, pair<double, double> p2)
    {
        return -1 * (SQR(p1.first - p2.first) + SQR(p1.second - p2.second));
    }

    const int n;
    const int max_iter;
    const int consecutive_max_unchanged;
};


int main() 
{
    freopen("blobs.csv", "r", stdin);
    string l;
    cin >> l;
    
    vector<double> x_coord, y_coord;

    char c;
    double x, y;
    while (cin >> x >> c >> y) 
    {
        x_coord.push_back(x);
        y_coord.push_back(y);
    }

    auto clusterer = affinity_propagation(x_coord, y_coord, 5, 15);
    auto clusters = clusterer.get_clusters();

    for (auto const& cl: clusters)
        cout << cl << " ";
    return 0;
}
