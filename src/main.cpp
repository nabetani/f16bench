#include <iostream>
#include <chrono>
#include <array>

using stdclock = std::chrono::high_resolution_clock;

template <typename f_>
struct calc
{
    using f = f_;
    static constexpr size_t S = (1 << 8);
    using vals = std::array<f, S>;
    using o_vals = std::array<vals, S>;

    o_vals o_{};
    calc(int s)
    {
        for (auto &v : o_)
        {
            for (auto &e : v)
            {
                e = s-8;
                s = (s + 5) % 7;
            }
        }
    }

    void progress()
    {
        for (size_t i = 0; i < S; ++i)
        {
            auto &v0 = o_[i];
            auto &v1 = o_[(i + 3) % S];
            auto &v2 = o_[(i*2 + 5) % S];
            for (size_t j = 0; j < S; ++j)
            {
                v0[j] += v1[j]*v2[j];
            }
            constexpr f lim = 8;
            for( auto & e : v0 ){
                if (lim<e){
                    e = -lim;
                } else if ( e < -lim ){
                    e = lim;
                }
            }
        }
    }

    f get()
    {
        for (size_t i = 0; i < (1 << 8); ++i)
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
    if (r != 1234 && !output)
    {
        return;
    }
    auto d = (t1 - t0).count() * 1e-6;
    std::cout                                //
        << (f{1}/2*2==1 ? "float" : "int__") << sizeof(f) << "  " //
        << "r=" << (double)r << " tick=" << d << std::endl;
}

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 2; ++i)
    {
        run<_Float16>(argc, i != 1);
        run<float>(argc, i != 1);
        run<double>(argc, i != 1);
        run<std::int16_t>(argc, i != 1);
        run<std::int32_t>(argc, i != 1);
        run<std::int64_t>(argc, i != 1);
    }
}
