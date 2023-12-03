#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <random>
#include <cmath>
#include <stdexcept>

extern "C" class PrimeProbabilityTest {
public:
    PrimeProbabilityTest(int value, double minProbability)
        : value_(value), minProbability_(minProbability) {}

    virtual ~PrimeProbabilityTest() {}


    bool isPrime() {
        if (value_ <= 1) return false;
        if (value_ <= 3) return true;

        if (customTest()) {
            return millerRabinTest();
        }

        return false;
    }

protected:
    virtual bool customTest() {
        return false;
    }

    int value_;

    bool millerRabinTest() {
        if (value_ <= 1 || value_ == 4) return false;
        if (value_ <= 3) return true;

        int k = 0;
        int d = value_ - 1;
        while (d % 2 == 0) {
            k++;
            d /= 2;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(2, value_ - 2);

        for (int i = 0; i < static_cast<int>(std::log2(value_)); i++) {
            int a = distribution(gen);
            int x = std::pow(a, d);
            x %= value_;

            if (x == 1 || x == value_ - 1) continue;

            for (int r = 0; r < k - 1; r++) {
                x = std::pow(x, 2);
                x %= value_;
                if (x == value_ - 1) break;
            }

            if (x != value_ - 1) return false;
        }

        return true;
    }

private:
    double minProbability_;
};

extern "C" class FermatTest final : public PrimeProbabilityTest {
public:
    FermatTest(int value, double minProbability)
        : PrimeProbabilityTest(value, minProbability) {}

protected:
    bool customTest() override {
        if (value_ == 2) return true;
        if (value_ % 2 == 0) return false;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(2, value_ - 2);

        for (int i = 0; i < 100; i++) {
            int a = distribution(gen);
            int x = static_cast<int>(std::pow(a, value_ - 1)) % value_;
            if (x != 1) return false;
        }

        return true;
    }
};

extern "C" class SolovayStrassenTest final : public PrimeProbabilityTest {
public:
    SolovayStrassenTest(int value, double minProbability)
        : PrimeProbabilityTest(value, minProbability) {}

protected:
    bool customTest() override {
        if (value_ == 2) return true;
        if (value_ % 2 == 0) return false;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(2, value_ - 2);
        for (int i = 0; i < 100; i++) {
            int a = distribution(gen);
            int x = static_cast<int>(std::pow(a, (value_ - 1) / 2)) % value_;
            int jacobiSymbol = jacobi(a, value_);

            if (jacobiSymbol == 0 || x != jacobiSymbol) return false;
        }
        return true;
    }

    int jacobi(int a, int n) {
        if (n <= 0 || n % 2 == 0) {
            throw std::invalid_argument("Invalid value of n");
        }

        int result = 1;
        if (a < 0) {
            a = -a;
            if (n % 4 == 3) {
                result = -result;
            }
        }

        while (a != 0) {
            while (a % 2 == 0) {
                a /= 2;
                int r = n % 8;
                if (r == 3 || r == 5) {
                    result = -result;
                }
            }

            std::swap(a, n);
            if (a % 4 == 3 && n % 4 == 3) {
                result = -result;
            }
            a %= n;
        }

        if (n == 1) {
            return result;
        }
        else {
            return 0;
        }
    }
};

unsigned long long p, q, N, e, d, D, P, SN, C;
int Dp, Dq;

extern "C" unsigned long long fastv(unsigned long long n, unsigned long long p, unsigned long long q, unsigned long long N) {
    if (n == 0)
        return 2;
    if (n == 1)
        return p;
    if (n % 2 == 0) {
        unsigned long long nn = n / 2;
        return pow((fastv(nn, p, q, N) % N), 2) - 2 * pow(q, nn);
    }
    else {
        unsigned long long nn = (n + 1) / 2, nm = (n - 1) / 2;
        return (fastv(nn, p, q, N) % N) * (fastv(nm, p, q, N) % N) - pow(q, nn) * p;
    }
};

extern "C" class LUC {
public:
    LUC(int bitLength, unsigned long long P) {
        
    }

    virtual ~LUC() {}

    unsigned long long Enc(unsigned long long key, unsigned long long N) {
        unsigned long long C = fastv(key, P, 1, N) % N;
        return C;
    }


private:


};

extern "C" unsigned long long FastPow(unsigned long long a, unsigned long long n, unsigned long long p) {

    if (n == 0) {
        return 1;

    }
    else if (n % 2 == 0) {

        return FastPow((a * a) % p, n / 2, p);

    }
    else {
        unsigned long long square = (a * a) % p;
        return (FastPow(square, n / 2, p) * a) % p;
    }
}

extern "C" unsigned long long gcd(unsigned long long e, unsigned long long t) {
    while (e > 0)
    {
        unsigned long long tmp;

        tmp = e;
        e = t % e;
        t = tmp;
    }

    return t;
}

extern "C" int legendre(int a, int p) {
    if (p != 2) {
        if (a % p == 0) {
            return 0;
        }
        else if ((gcd(a, p) == 1) && ((FastPow(a, (p - 1) / 2, p) == 1))) {
            return 1;
        }
        else {
            return -1;
        }
    }
    return -1;
}

extern "C" int Euclid(int a, int m, int& x, int& y) {
    if (m == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;
    int gcd = Euclid(m, a % m, x1, y1);

    x = y1;
    y = x1 - (a / m) * y1;

    return gcd;
}

extern "C" int modInverse(int a, int m) {
    int x, y;
    int gcd = Euclid(a, m, x, y);

    if (gcd != 1) {
        return -1;
    }
    else {
        return (x % m + m) % m;
    }
}

extern "C" unsigned long long lcm(unsigned long long a, unsigned long long b)
{
    return (a / gcd(a, b)) * b;
}

extern "C" unsigned long long generateRandomNumber(int bitLength) {
    unsigned long long number = 0;
    for (int i = 0; i < bitLength; ++i) {
        number <<= 1;
        number |= (rand() % 2);
    }
    return number;
}


extern "C" bool isPrime(unsigned long long number) {
    if (number <= 1) {
        return false;
    }
    if (number <= 3) {
        return true;
    }
    if (number % 2 == 0 || number % 3 == 0) {
        return false;
    }

    for (unsigned long long i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

extern "C" unsigned long long generatePrime(int bitLength) {
    if (bitLength <= 0) {
        throw "bitLength = 0!";
        return 0;
    }
    while (1) {
        unsigned long long candidate = generateRandomNumber(bitLength);
        if (isPrime(candidate)) {
            return candidate;
        }
    }
}

extern "C" unsigned long long calce(unsigned long long t) {
    unsigned long long e;

    for (e = generateRandomNumber(8); e < t; ++e)
    {
        if (gcd(e, t) == 1)
        {
            return e;
        }
    }

    return -1;
}

extern "C" void Encryption(const char* filename, const char* directory, bool cipher) {
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Directory: " << directory << std::endl;
    std::string f = filename, tmp;
    std::cout << "f = " << f << std::endl;
    std::string filename2 = f.substr(f.find_last_of('/') + 1, f.length() - f.find_last_of('/'));
    std::cout << "Filename2 = " << filename2 << std::endl;
    std::ifstream file(filename, std::ios::binary);
    std::getline(file, tmp);
    unsigned long long number = std::stoull(tmp, nullptr, 0);


    if (cipher) {
        srand(static_cast<unsigned>(time(nullptr)));
        p = generatePrime(12);
        q = generatePrime(12);
        N = p * q;
        e = calce((p - 1) * (q - 1) * (p + 1) * (q + 1));
        D = number * number - 4;
        Dp = legendre(D, p);
        Dq = legendre(D, q);
        SN = lcm(p - Dp, q - Dq);
        for (unsigned long long i = 1; i <= pow(10, 9); i++) {
            if (((e * i) % SN) == 1) {
                d = i;
                break;
            }
        }
        C = fastv(e, number, 1, N) % N;
        std::string name = "/encrypted_";
        std::ofstream file2(directory + name + filename2);
        file2 << C;
        file2.close();
    }
    else {
        P = fastv(d, number, 1, N) % N;
        std::string name = "/decrypted_";
        std::ofstream file2(directory + name + filename2);
        file2 << P;
        file2.close();
    }

    file.close();
}