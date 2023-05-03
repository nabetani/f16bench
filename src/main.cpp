#include <iostream>
#include <chrono>
#include <array>

using stdclock = std::chrono::high_resolution_clock;

template <typename f_>
struct calc
{
    using f = f_;
    static constexpr size_t S = (1 << 4);
    using vals = std::array<f, S>;
    using o_vals = std::array<vals, S>;

    o_vals o_{};
    calc(int s)
    {
        for (auto &v : o_)
        {
            for (auto &e : v)
            {
                e = s;
                s = (s + 1) % 17;
            }
        }
    }

    void cap(vals &v)
    {
        constexpr f lim = 2;
        for (auto &e : v)
        {
            while (lim * 2 < e)
            {
                e -= lim;
            }
        }
    }

    void progress()
    {
        for (size_t i = 0; i < o_.size(); ++i)
        {
            auto &v0 = o_[i];
            auto &v1 = o_[(i + 3) % o_.size()];
            auto &v2 = o_[(i + 5) % o_.size()];
            cap(v0);
            cap(v1);
            cap(v2);
            for (size_t j = 0; j < v0.size(); ++j)
            {
                v0[j] += v1[j]*v2[j];
            }
        }
    }

    f get()
    {
        for (size_t i = 0; i < (1 << 10); ++i)
        {
            progress();
        }
        f sum = 0;
        for (auto &v : o_)
        {
            for (auto &e : v)
            {
                sum += e;
            }
        }
        return sum;
    }
};

template <typename f>
void run(int src, bool output)
{
    auto t0 = stdclock::now();
    calc<f> c{src};
    auto r = c.get();
    auto t1 = stdclock::now();
    if (r != 0 && !output)
    {
        return;
    }
    auto d = (t1 - t0).count() * 1e-6;
    std::cout                                //
        << "sizeof(f)=" << sizeof(f) << "  " //
        << "r=" << (double)r << ":" << d << std::endl;
}

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 2; ++i)
    {
        run<double>(argc, i != 1);
        run<float>(argc, i != 1);
        run<_Float16>(argc, i != 1);
    }
}
