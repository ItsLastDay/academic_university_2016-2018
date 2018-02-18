#include <bits/stdc++.h>
using namespace std;

const int MAXN = 60;
const int SZ = 52;

int x_pos[MAXN];
int y_pos[MAXN];

using path_t = array<int, 52>;

int evaluate_path(path_t const& path)
{
    int ret = 0;
    for (int i = 1; i < SZ; i++)
    {
        ret += abs(x_pos[path[i]] - x_pos[path[i - 1]]);
        ret += abs(y_pos[path[i]] - y_pos[path[i - 1]]);
    }

    return ret;
}

ostream& operator<<(ostream& out, path_t const& path)
{
    out << "(";
    for (int i = 0; i < SZ; i++)
        out << path[i] + 1 << " )"[i == SZ - 1];
    return out;
}


path_t climb(path_t cur_path)
{
    int cur_val = evaluate_path(cur_path);
    int prev_val;

    do
    {
        prev_val = cur_val;

        pair<int, int> best_swap;
        int best_next_val = 1e5;
        for (int i = 0; i < SZ; i++)
            for (int j = i + 1; j < SZ; j++)
            {
                swap(cur_path[i], cur_path[j]);
                int tmp = evaluate_path(cur_path);
                if (tmp < best_next_val)
                {
                    best_next_val = tmp;
                    best_swap = make_pair(i, j);
                }
                swap(cur_path[i], cur_path[j]);
            }

        if (best_next_val == -1) throw 1;

        swap(cur_path[best_swap.first], cur_path[best_swap.second]);
        cur_val = evaluate_path(cur_path);
    } while (cur_val < prev_val);

    return cur_path;
}

void go_hill()
{
    random_device rd;
    mt19937 gen(rd());

    int total_best = 1e5;
    path_t total_path;

    for (int it = 0; it < 1e6; it++)
    {
        path_t initial_path;
        iota(initial_path.begin(), initial_path.end(), 0);
        shuffle(initial_path.begin(), initial_path.end(), gen);

        path_t best = climb(initial_path);
        int cur_val = evaluate_path(best);
        if (cur_val < total_best)
        {
            total_best = cur_val;
            total_path = best;
        }

        if (it % 1000 == 0)
        {
            cout << total_best << " " << total_path << endl;
        }
    }
}

path_t climb_annealing(path_t cur_path)
{
    double T = 1e6;
    
    vector<tuple<int, int, int>> delta_i_j;
    vector<double> probs;
    delta_i_j.resize(SZ * SZ);
    probs.resize(SZ * SZ);

    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<double> p(0, 1);

    do
    {
        int cur_val = evaluate_path(cur_path);
        //cout << T << " " << cur_val << endl;

        int idx = 0;
        for (int i = 0; i < SZ; i++)
            for (int j = i + 1; j < SZ; j++)
            {
                swap(cur_path[i], cur_path[j]);
                int tmp = evaluate_path(cur_path);
                delta_i_j[idx++] = make_tuple(cur_val - tmp, i, j);
                swap(cur_path[i], cur_path[j]);
            }

        double denom = 0.0;
        for (int i = 0; i < idx; i++)
        {
            denom += exp(get<0>(delta_i_j[i]) / T);
        }

        for (int i = 0; i < idx; i++)
        {
            double prob = exp(get<0>(delta_i_j[i]) / T) / denom;
            probs[i] = prob;
            //cout << probs[i] << " ";
        }
        //cout << endl;

        double now_prob = p(gen);
        for (int i = 0; i < idx; i++)
        {
            if (now_prob < probs[i])
            {
                //cout << "Picked prob " << setprecision(10) << probs[i] << endl;
                swap(cur_path[get<1>(delta_i_j[i])], cur_path[get<2>(delta_i_j[i])]);
                break;
            }
            now_prob -= probs[i];
        }


        T = 0.95 * T;
    } while (T > 1);

    return cur_path;
}

void go_annealing()
{
    random_device rd;
    mt19937 gen(rd());

    int total_best = 1e5;
    path_t total_path;

    for (int it = 0; it < 1e6; it++)
    {
        path_t initial_path;
        iota(initial_path.begin(), initial_path.end(), 0);
        shuffle(initial_path.begin(), initial_path.end(), gen);

        path_t best = climb_annealing(initial_path);
        int cur_val = evaluate_path(best);
        if (cur_val < total_best)
        {
            total_best = cur_val;
            total_path = best;
        }

        if (it % 1000 == 0)
        {
            cout << total_best << " " << total_path << endl;
        }
    }
}

struct GeneticPopulation
{
    GeneticPopulation(random_device &rd_, mt19937 &gen_)
        : rd(rd_)
        , gen(gen_)
    {
        sorter_ = [](path_t const& x, path_t const& y)
        {
            return evaluate_path(x) < evaluate_path(y);
        };
        population_index_generator = uniform_int_distribution<int>(0, 
                TOP_POPULATION_CUTTOF - 1);
        path_index_generator = uniform_int_distribution<int>(0, SZ - 1);

        for (int it = 0; it < POP_SIZE; it++)
        {
            path_t cur;
            iota(cur.begin(), cur.end(), 0);
            shuffle(cur.begin(), cur.end(), gen);
            cur_population_.push_back(cur);
        }
    }

    void crossover_and_mutate() 
    {
        for (int it = 0; it < NUM_CROSSOVERS; it++)
        {
            int idx1 = population_index_generator(gen);
            int idx2 = population_index_generator(gen);
            auto c1 = crossover(cur_population_[idx1], 
                        cur_population_[idx2]);
            auto c2 = crossover(cur_population_[idx2], 
                        cur_population_[idx1]);
            //cur_population_.push_back(c1);
            //cur_population_.push_back(c2);
            cur_population_.push_back(mutate(c1, 5));
            //cur_population_.push_back(mutate(c1, 7));
            cur_population_.push_back(mutate(c2, 5));
            //cur_population_.push_back(mutate(c2, 7));
        }
    }

    void select()
    {
        // Erase duplicates
        sort(cur_population_.begin(), cur_population_.end());
        cur_population_.resize(unique(cur_population_.begin(), cur_population_.end()) - cur_population_.begin());

        // Sort the rest according to path length
        sort(cur_population_.begin(), cur_population_.end(), sorter_);
        cur_population_.resize(POP_SIZE);
        /*
        for (int i = 0; i < POP_SIZE; i++)
            if (not is_good_path(cur_population_[i]))
                exit(1);
        */
    }

    void print_best()
    {
        cout << evaluate_path(cur_population_[0]) << " (";
        for (const auto &u: cur_population_[0])
            cout << u << " ";
        cout << ")" << endl;
    }

private:

    bool is_good_path(path_t const& x)
    {
        for (int i = 0; i < SZ; i++)
            if (count(x.begin(), x.end(), i) != 1)
                return false;
        return true;
    }

    path_t crossover(path_t const& x, path_t const& y)
    {
        path_t result;
        fill(result.begin(), result.end(), -1);

        int place_idx = 0;
        for (int i = 0; i < SZ / 2; i++)
        {
            result[place_idx++] = x[i];
        }

        for (int i = 0; i < SZ; i++)
        {
            if (find(result.begin(), result.end(), y[i]) == result.end())
            {
                result[place_idx++] = y[i];
            }
        }

        return result;
    }

    path_t mutate(path_t x, int num_mut)
    {
        for (int it = 0; it < num_mut; it++)
        {
            int idx1 = path_index_generator(gen);
            int idx2 = path_index_generator(gen);
            swap(x[idx1], x[idx2]);
        }
        return x;
    }

    uniform_int_distribution<int> population_index_generator, path_index_generator;
    random_device &rd;
    mt19937 &gen;
    const int TOP_POPULATION_CUTTOF = 75;
    const int POP_SIZE = 600;
    const int NUM_CROSSOVERS = 55;
    vector<path_t> cur_population_;
    function<bool(path_t const&, path_t const&)> sorter_;
};

void go_genetic()
{
    random_device rd;
    mt19937 gen(rd());

    auto population = GeneticPopulation(rd, gen);

    for (int it = 0; it < 100000000; it++)
    {
        population.crossover_and_mutate();
        population.select();
        if (it % 1000 == 0)
        {
            cout << "Iteration " << it << ":\n\t";
            population.print_best();
        }
    }

    cout << endl << "Total:" << endl;
    population.print_best(); 
}

int main()
{
    freopen("tsp.csv", "r", stdin);

    int num, x, y;
    char sep;
    while (cin >> num >> sep >> x >> sep >> y)
    {
        x_pos[num - 1] = x;
        y_pos[num - 1] = y;
    }

    //go_hill();
    //go_annealing();
    go_genetic();
    return 0;
}
