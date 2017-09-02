#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << (x) << endl;
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector 

    -std=c++14 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

struct my_complex
{
    my_complex() = default;

    my_complex(double x, double y)
        : imag_(y)
        , real_(x)
    {
    }

    my_complex& operator/=(double x)
    {
        real_ /= x;
        imag_ /= x;
        return *this;
    }

    my_complex operator/=(my_complex const& other)
    {
        double x = real_ * other.real_ + imag_ * other.imag_;
        double y = other.real_ * other.real_ + other.imag_ * other.imag_;
        double z = imag_ * other.real_ - real_ * other.imag_;
        real_ = x / y;
        imag_ = z / y;
        return *this;
    }

    my_complex operator/(my_complex const& other) const
    {
        my_complex ret = *this;
        ret /= other;
        return ret;
    }

    my_complex& operator*=(my_complex const& other)
    {
        double x = real_ * other.real_ - imag_ * other.imag_;
        double y = real_ * other.imag_ + imag_ * other.real_;
        real_ = x;
        imag_ = y;
        return *this;
    }

    my_complex operator*(my_complex const& other) const
    {
        my_complex ret = *this;
        ret *= other;
        return ret;
    }

    my_complex& operator+=(my_complex const& other)
    {
        imag_ += other.imag_;
        real_ += other.real_;
        return *this;
    }

    my_complex operator+(my_complex const& other) const
    {
        my_complex ret = *this;
        ret += other;
        return ret;
    }

    my_complex& operator-=(my_complex const& other)
    {
        imag_ -= other.imag_;
        real_ -= other.real_;
        return *this;
    }

    my_complex operator-(my_complex const& other) const
    {
        my_complex ret = *this;
        ret -= other;
        return ret;
    }

    my_complex(double x)
        : real_(x)
    {
    }

    double real() const
    {
        return real_;
    }
private:
    double imag_, real_;
};

using comp = complex<double>;

vector<comp> fft(vector<comp> a, bool invert)
{
    int n = a.size();
    reverse(a.begin(), a.end());
    
    // Copy-paste from e-maxx, because my implementation is too slow.
	for (int i=1, j=0; i<n; ++i) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}
 
	for (int len=2; len<=n; len<<=1) {
		double ang = 2*M_PI/len * (invert ? -1 : 1);
		comp wlen (cos(ang), sin(ang));
		for (int i=0; i<n; i+=len) {
			comp w (1);
			for (int j=0; j<len/2; ++j) {
				comp u = a[i+j],  v = a[i+j+len/2] * w;
				a[i+j] = u + v;
				a[i+j+len/2] = u - v;
				w *= wlen;
			}
		}
	}

    reverse(a.begin(), a.end());
    for (int i = 0; i < n; i++)
    {
        a[i] = conj(a[i]);
    }
    return a;
}

vector<comp> fft_my(vector<comp> a, comp w)
{
    // Highest coef. first.
    int n = a.size();
    if (n == 1)
        return a;
    if (n == 4)
    {
        vector<comp> res(n);
        comp base_w = w;
        for (int i = 0; i < n; i++)
        {
            auto cur_w = comp(1, 0);
            for (int j = n - 1; j >= 0; j--)
            {
                res[i] += a[j] * cur_w;
                cur_w *= w;
            }
            w *= base_w;
        }
        return res;
    }

    vector<comp> odd(n / 2), even(n / 2);
    for (int i = 0; i < n; i++)
    {
        if (not (i & 1))
            odd[i / 2] = a[i];
        else
            even[i / 2] = a[i];
    }

    auto res_odd = fft_my(move(odd), w * w);
    auto res_even = fft_my(move(even), w * w);

    comp w_cur = w;
    for (int i = 0; i < n / 2; i++)
    {
        a[i] = res_even[i] + w_cur * res_odd[i];
        a[i + n / 2] = res_even[i] - w_cur * res_odd[i]; 
        w_cur *= w;
    }

    return a;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    string s, t;
    cin >> s >> t;

    vector<int> sum_match(n);

    string alph = "ACGT";
    vector<comp> a(2 * n, 0), b(2 * n, 0);
    for (char c: alph)
    {
        comp w = { polar<double>(1, M_PI / n).real(),
                    polar<double>(1, M_PI / n).imag() };
        a.assign(2 * n, 0);
        b.assign(2 * n, 0);
        for (int i = 0; i < n; i++)
        {
            if (s[i] != c)
                continue;
            a[2 * n - i - 1] = 1;
        }

        for (int i = 0; i < n; i++)
        {
            if (t[i] != c)
                continue;
            b[i] = b[i + n] = 1;
        }

        auto res_a = fft(move(a), false);
        auto res_b = fft(move(b), false);
        vector<comp> res_total(2 * n, 0);
        for (int i = 0; i < 2 * n; i++)
        {
            res_total[i] = res_a[i] * res_b[i];
        }
#ifdef DEBUG
        cout << endl;
        DBG(c);
#endif
        auto inv = fft_my(move(res_total), pow(w, -1));
        for (int i = 0; i < 2 * n; i++)
        {
            inv[i] /= 2 * n;
        }

#ifdef DEBUG
        for (int i = 0; i < 2 * n; i++)
        {
            auto val_i = static_cast<int>(round(inv[i].real()));
            cout << i << " "<< val_i << endl;
        }
#endif
        for (int i = 0; i < n; i++)
        {
            auto val_i = static_cast<int>(round(inv[i + n].real()));
            sum_match[i] += val_i;

#ifdef DEBUG
            auto tt = t + t;
            int check = 0;
            for (int j = 0; j < n; j++)
            {
                if (s[j] == tt[i + j] and s[j] == c)
                    check++;
            }
            cout << s << endl;
            cout << tt.substr(i, n) << endl;
            DBG(val_i);
            DBG(check);
            assert(val_i == check);
#endif
        }
    }

    int best_val = sum_match[0];
    int best_ans = 0;
    for (int i = 1; i < n; i++)
    {
        //cout << sum_match[i] << " ";
        if (sum_match[i] > best_val)
        {
            best_val = sum_match[i];
            best_ans = n - i;
        }
    }

    cout << best_val << " " << best_ans << endl;
    return 0;
}
