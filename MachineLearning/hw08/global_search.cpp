#include <bits/stdc++.h>
using namespace std;


double func(double x, double y)
{
    return 20 * exp(-1.0 / 5 * (pow(x - 2.5, 2) + pow(y + 1.5, 2))) +
        3 * (1 + pow(sin(0.08 * M_PI * x * y), 2)) + 
        3 * log(2 + pow(10 * (x - 2.4), 4) + pow(10 * (y + 1.3), 4)) / 
            (1.0 + pow((x * x + y * y) / 100.0, 0.1));
}

void monte_carlo()
{
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<double> dist_x(-10, 10);
    uniform_real_distribution<double> dist_y(-10, 10);
    long long it = 0;
    
    double cur_max = func(0, 0);
    double cur_best_x=0, cur_best_y=0;
    while (true)
    {
        ++it;
        double x = dist_x(gen);
        double y = dist_y(gen); 
        double z = func(x, y);
        if (z > cur_max)
        {
            cur_max = z;
            cur_best_x = x;
            cur_best_y = y;
        }

        if (it % 50000000 == 0)
        {
            cout << cur_best_x << " " << cur_best_y << " " << cur_max << "\n";
            cerr << it << "\n";
            cerr << cur_best_x << " " << cur_best_y << " " << cur_max << "\n";

            cur_max = -10000;
        }
    }
}

void cross_entropy()
{
    const double EPS = 1e-8;

    random_device rd;
    mt19937 gen(rd());

    normal_distribution<double> dist_x(0, 20);
    normal_distribution<double> dist_y(0, 10);

    const int NUM_SAMPLES = 100000;
    const int NUM_PIVOT = 100;
    vector< tuple<double, double, double> > samples;
    samples.resize(NUM_SAMPLES);

    for (int it = 0; it < 1000; it++)
    {
        cout << "iteration " << it << endl;
        for (int i = 0; i < NUM_SAMPLES; i++)
        {
            double x = dist_x(gen);
            double y = dist_y(gen);
            samples[i] = make_tuple(func(x, y), x, y);
        }

        sort(samples.rbegin(), samples.rend());
        cout << dist_x.mean() << " " << dist_x.stddev() << endl;
        cout << dist_y.mean() << " " << dist_y.stddev() << endl;
        cout << "Best val: " << setprecision(20) << get<0>(samples[0]) << endl;\
        cout << endl;

        double mean_x = 0, mean_y = 0;
        for (int i = 0; i < NUM_PIVOT; i++)
        {
            mean_x += get<1>(samples[i]);
            mean_y += get<2>(samples[i]);
        }
        mean_x /= NUM_PIVOT;
        mean_y /= NUM_PIVOT;

        double dev_x = 0, dev_y = 0;
        for (int i = 0; i < NUM_PIVOT; i++)
        {
//            cout << "\t" << i << " " << get<0>(samples[i]) << " " <<
//               get<1>(samples[i]) << " " << get<2>(samples[i])  << endl;
            dev_x += pow(get<1>(samples[i]) - mean_x, 2);
            dev_y += pow(get<2>(samples[i]) - mean_y, 2);
        }
        dev_x /= NUM_PIVOT - 1;
        dev_y /= NUM_PIVOT - 1;
        dev_x = sqrt(dev_x);
        dev_y = sqrt(dev_y);

        dist_x = normal_distribution<double>(mean_x, dev_x);
        dist_y = normal_distribution<double>(mean_y, dev_y);

        if (dev_x < EPS and dev_y < EPS)
            break;
    }
}

int main()
{
//    monte_carlo();
    cross_entropy();
    return 0;
}
