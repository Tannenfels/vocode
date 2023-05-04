#include <complex.h>

double PI;

void _fft(double _Complex buf[], double _Complex out[], int n, int step)
{
    if (step < n) {
        _fft(out, buf, n, step * 2);
        _fft(out + step, buf + step, n, step * 2);

        for (int i = 0; i < n; i += 2 * step) {
            double _Complex t = cexp(-(__extension__ 1.0iF) * PI * i / n) * out[i + step];
            buf[i / 2]     = out[i] + t;
            buf[(i + n)/2] = out[i] - t;
        }
    }
}

void fft(double _Complex buf[], int n)
{
    double _Complex out[n];
    for (int i = 0; i < n; i++) out[i] = buf[i];

    _fft(buf, out, n, 1);
}
