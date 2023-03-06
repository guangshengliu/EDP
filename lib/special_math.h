#ifndef SPECIAL_MATH_H
#define SPECIAL_MATH_H

#include <math.h>
#include <numbers>
#include <vector>

// Calculate the coefficients used by Spouge's approximation
static std::vector<double> CalculateCoefficients(int numCoeff)
{
    std::vector<double> c(numCoeff);
    double k1_factrl = 1.0;
    c[0] = sqrt(2.0 * 3.14159265);
    for(size_t k=1; k < numCoeff; k++)
    {
        c[k] = exp(numCoeff-k) * pow(numCoeff-k, k-0.5) / k1_factrl;
        k1_factrl *= -(double)k;
    }
    return c;
}

/*
 *  The Spouge approximation
 */
static double _Gamma(const std::vector<double>& coeffs, double x)
{
    const size_t numCoeff = coeffs.size();
    double accm = coeffs[0];
    for(size_t k=1; k < numCoeff; k++)
    {
        accm += coeffs[k] / ( x + k );
    }
    accm *= exp(-(x+numCoeff)) * pow(x+numCoeff, x+0.5);
    return accm/x;
}

//  Gamma function
static double Gamma(double x)
{
    const auto coeff10 = CalculateCoefficients(10);
    return _Gamma(coeff10, x);
}

static double gammln(const double x)
{
    return log(Gamma(x));
}

// Series expression P(a,x)
static void gser(double &gamser, const double a, const double x, double &gln)
{
    const int ITMAX = 100;
    const double EPS=std::numeric_limits<double>::epsilon();
    int n;
    double sum,del,ap;

    gln=gammln(a);
    if(x <= 0.0) {
        if(x < 0.0){
            gamser = 0.0;
            return ;
        }
        else{
            ap = a;
            del = sum = 1.0/a;
            for (int n = 0; n < ITMAX; ++n) {
                ++ap;
                del *= x/ap;
                sum += del;
                if(fabs(del) < fabs(sum)*EPS) {
                    gamser = sum * exp(-x + a*log(x) - gln);
                    return ;
                }
            }
            return ;
        }
    }
}

//
static void gcf(double &gammcf, const double a, const double x, double &gln)
{
    const int ITMAX = 100;
    const double EPS = std::numeric_limits<double>::epsilon();
    const double FPMIN = std::numeric_limits<double>::min()/EPS;
    // ITMAX is the maximum allowed number of iterations
    int i;
    double an,b,c,d,del,h;

    gln = gammln(a);
    b = x + 1.0 - a;
    c = 1.0 / FPMIN;
    d = 1.0 / b;
    h = d;
    for (i = 0; i < ITMAX; ++i) {
        an = -i*(i-a);
        b += 2.0;
        d = an * d +b;
        if(fabs(d) < FPMIN)
            d = FPMIN;
        c = b + an / c;
        if(fabs(c) < FPMIN)
            c = FPMIN;
        d= 1.0 / d;
        del = d * c;
        h *= del;
        if(fabs(del - 1.0) <= EPS)
            break;
    }
    if(i > ITMAX)
        return ;
    gammcf = exp(-x + a * log(x) - gln) * h;
}

// return Ingamma function P(a,x) value
static double gammp(const double a, const double x)
{
    double gamser = 0.0;
    double gammcf = 0.0;
    double gln = 0.0;

    if(x < 0.0 || a <= 0.0)
        return -1;
    if(x < a + 1.0){
        gser(gamser,a,x,gln);
        return gamser;
    }
    else{
        gcf(gammcf,a,x,gln);
        return 1.0 - gammcf;
    }
}

// return Ingamma function Q(a,x) value
static double gammq(const double a, const double x)
{
    double gamser = 0.0;
    double gammcf = 0.0;
    double gln = 0.0;

    if(x < 0.0 || a <= 0.0)
        return -1;
    if(x < a + 1.0){
        gser(gamser,a,x,gln);
        return 1.0 - gammcf;
    }
    else{
        gcf(gammcf,a,x,gln);
        return gammcf;
    }
}

//  Ingamma function
static double Ingamma(double a, double x)
{
    return gammp(a,x);
}

// Evaluation of inbeta function by modified *Lenta* method
static double betacf(double a,double b,double x)
{
    const int MAXIT = 100;
    const double EPS = std::numeric_limits<double>::epsilon();
    const double FPMIN = std::numeric_limits<double>::min()/EPS;
    int m,m2;
    double aa,c,d,del,h,qab,qam,qap;

    qab = a + b;
    qap = a + 1.0;
    qam = a - 1.0;
    c = 1.0;
    d = 1.0 - qab * x / qap;
    if(fabs(d) < FPMIN)
        d = FPMIN;
    d = 1.0 / d;
    h = d;
    for (m = 1; m < MAXIT; ++m) {
        m2 = 2 * m;
        aa = m * (b-m) * x/((qam + m2)*(a + m2));
        d = 1.0 + aa * d;
        if(fabs(d) < FPMIN)
            d = FPMIN;
        c = 1.0 + aa / c;
        if(fabs(c) < FPMIN)
            c = FPMIN;
        d = 1.0 / d;
        h *= d * c;
        aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
        d = 1.0 + aa * d;
        if(fabs(d) < FPMIN)
            d = FPMIN;
        c = 1.0 + aa / c;
        if(fabs(c) < FPMIN)
            c = FPMIN;
        d = 1.0 / d;
        del = d * c;
        h *= del;
        if(fabs(del - 1.0) <= EPS)
            break;
    }
    if(m > MAXIT)
        return -1;
    return h;
}

// inbeta function
static double inbeta(double a,double b,double x)
{
    double bt;
    if(x < 0.0 || x > 1.0)
        return -1;
    if(x == 0.0 || x == 1.0)
        bt = 0.0;
    else
        bt = exp(gammln(a+b) - gammln(a) - gammln(b) + a*log(x) + b*log(1.0-x));
    if(x < (a+1.0) / (a + b +2.0))
        return bt * betacf(a,b,x)/a;
    else
        return 1.0 - bt*betacf(b,a,1.0-x)/b;
}

// F distribution function
static double fff(double f_value,int n1,int n2)
{
    double y;
    if(f_value < 0)
        f_value = -f_value;
    y = inbeta(n2/2.0, n1/2.0, n2/(n2+n1*f_value));
    return y;
}

// F-Value
static double get_f_value(double f,int n1,int n2)
{
    double upper = 0.0;
    double MIN = 1.0;
    int integer_num;
    int integer_best = 0;
    for (integer_num = 0; integer_num < 100; ++integer_num) {
        if(fabs(fff(integer_num,n1,n2) - f) < MIN){
            integer_best = integer_num;
            MIN = fabs(fff(integer_num,n1,n2) - f);
        }
    }
    int decimal_num;
    int decimal_best = 0;
    // best num = integer_num or best num = integer_num - 1
    bool falg = false;
    for (decimal_num = 1; decimal_num < 100; ++decimal_num) {
        double search = integer_best + double(decimal_num)/100;
        if(fabs(fff(search,n1,n2) - f) < MIN){
            decimal_best = decimal_num;
            MIN = fabs(fff(search,n1,n2) - f);
            falg = false;
        }
        if(fabs(fff(search-1,n1,n2) - f) < MIN){
            decimal_best = decimal_num;
            MIN = fabs(fff(search-1,n1,n2) - f);
            falg = true;
        }
    }
    if(falg)
        return integer_best - 1 + double(decimal_best)/100;
    return integer_best + double(decimal_best)/100;
}

#endif // SPECIAL_MATH_H
