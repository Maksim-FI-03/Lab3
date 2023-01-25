#include <iostream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>

class Polinom {
private:
    std::vector<bool> v;

    static Polinom generator;

    static void removeLeadingZeros(std::vector<bool>& v);

public:
    Polinom();

    explicit Polinom(const std::string& str);

    explicit Polinom(std::vector<bool> v);

    static void setGenerator(Polinom polinom);

    friend std::ostream& operator<<(std::ostream& stream, const Polinom& polinom);

    bool operator==(const Polinom& polinom) const;

    Polinom operator%(const Polinom& polinom) const;

    Polinom operator+(const Polinom& polinom) const;

    Polinom operator*(const Polinom& polinom) const;

    Polinom sqr() const;

    Polinom operator^(const std::vector<bool>& n) const;
};

std::vector<bool> bitVectorFromStr(const std::string& str);

void Polinom::removeLeadingZeros(std::vector<bool>& v) {
    auto it = std::find_if(v.begin(), v.end(), [](bool x) { return x; });
    v.erase(v.begin(), it);
}

Polinom::Polinom() : v() {}

Polinom::Polinom(const std::string& str) : v(str.size()) {
    for (int i = 0; i < str.size(); ++i) {
        v[i] = (str[i] != '0');
    }
    removeLeadingZeros(v);
}

Polinom::Polinom(std::vector<bool> v) : v(std::move(v)) {
    removeLeadingZeros(v);
}

Polinom Polinom::generator;

void Polinom::setGenerator(Polinom polinom) {
    generator = std::move(polinom);
}

std::ostream& operator<<(std::ostream& stream, const Polinom& polinom) {
    for (bool i : polinom.v) {
        stream << (i ? '1' : '0');
    }
    return stream;
}

bool Polinom::operator==(const Polinom& polinom) const {
    return v == polinom.v;
}

Polinom Polinom::operator%(const Polinom& polinom) const {
    std::vector<bool> R = v;
    long long t = 0;
    while ((R.size() - t) >= polinom.v.size()) {
        for (int i = 0; i < polinom.v.size(); ++i) {
            R[i + t] = (R[i + t] != polinom.v[i]);
        }
        t = distance(R.begin(), find_if(R.begin(), R.end(), [](bool x) { return x; }));
    }
    removeLeadingZeros(R);
    return Polinom(R);
}

Polinom Polinom::operator+(const Polinom& polinom) const {
    std::vector<bool> res_v(std::max(v.size(), polinom.v.size()));
    if (v.size() < polinom.v.size()) {
        for (int i = 0; i < polinom.v.size() - v.size(); ++i) {
            res_v[i] = polinom.v[i];
        }
        for (int i = 0; i < v.size(); ++i) {
            res_v[polinom.v.size() - v.size() + i] = (v[i] != polinom.v[polinom.v.size() - v.size() + i]);
        }
    }
    if (v.size() > polinom.v.size()) {
        for (int i = 0; i < v.size() - polinom.v.size(); ++i) {
            res_v[i] = v[i];
        }
        for (int i = 0; i < polinom.v.size(); ++i) {
            res_v[v.size() - polinom.v.size() + i] = (v[v.size() - polinom.v.size() + i] != polinom.v[i]);
        }
    }
    if (v.size() == polinom.v.size()) {
        for (int i = 0; i < v.size(); ++i) {
            res_v[i] = (v[i] != polinom.v[i]);
        }
    }
    removeLeadingZeros(res_v);
    return Polinom(res_v) % generator;
}

Polinom Polinom::operator*(const Polinom& polinom) const {
    std::vector<bool> res_v(v.size() + polinom.v.size() - 1, false);
    for (int i = 0; i < v.size(); ++i) {
        if (v[i]) {
            for (int j = 0; j < polinom.v.size(); ++j) {
                res_v[i + j] = (res_v[i + j] != polinom.v[j]);
            }
        }
    }
    removeLeadingZeros(res_v);
    return Polinom(res_v) % generator;
}

Polinom Polinom::sqr() const {
    if (!v.empty()) {
        std::vector<bool> res_v(v.size() * 2 - 1);
        for (int i = 0; i < v.size() - 1; ++i) {
            res_v[i * 2] = v[i];
            res_v[i * 2 + 1] = false;
        }
        res_v[res_v.size() - 1] = v[v.size() - 1];
        return Polinom(res_v) % generator;
    }
    else
        return Polinom{};
}

Polinom Polinom::operator^(const std::vector<bool>& n) const {
    Polinom res("1"), a = *this;
    std::vector<bool> rev = n;
    std::reverse(rev.begin(), rev.end());
    for (auto&& i : rev) {
        if (i) {
            res = res * a;
        }
        a = a.sqr();
    }
    return res;
}

std::vector<bool> bitVectorFromStr(const std::string& str) {
    std::vector<bool> v(str.length());
    for (int i = 0; i < str.size(); ++i) {
        v[i] = (str[i] != '0');
    }
    return v;
}

int main() {
    Polinom generator("100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000010011");
    Polinom::setGenerator(generator);
    Polinom a("01100010010001001111100100100101100000110011111111011000001110011111101010011100000011010111101010000001001000110010110100110011000110110110110101110110000010110100001010000011100010111010111011101010010001101010100011100001111110110"),
        b("00111101011110111011101110110001111101000001011011100011110100000110100101000101101100000011110011100011011001011111110110100000101110111000110111001100110111001000011111111110100000010111011011111100000110101001100001001010101000101");
    std::vector<bool> n = bitVectorFromStr("00010011010111010111100101011011010010101001110100101101010100110101100100001011010110101010010000011010110010011000100111000111010011110110000100100110001100000110110011100111010001001011110001111010011110001010110111010010110110001");
    Polinom sum("01011111001111110100001010010100011101110010100100111011111010011001001111011001101111010100011001100010010001101101000010010011101000001110000010111010110101111100010101111101000010101101100000010110010111000011000010101011010110011"),
        mul("01011011011101111000000011101100111010000111111101100010111000101000100110010101101100111011111101000110101001101001001011101101110001011001001011010001101100110100110010001101110101101111011011111110000010111011011011000001011011101"),
        sqr("11010110011001110001101110011101101000111111001111001101100011011110001001010101011110100010100001101111010010101101001111111100101010011000001011011110010110000010111111101000110010111000000001000010101100110100010011010101101100111"),
        pow("00010011110010010011011100000101000111110011000111100100100000011100101000101110110011111011011101101001111011011011000100011010100011001001000111110000101111111010111000100100000010101000000001000111001001011001011011011000100101010");
    std::cout << "Sum: " << ((a + b) == sum ? "ok" : "not ok") << std::endl;
    std::cout << "Mul: " << ((a * b) == mul ? "ok" : "not ok") << std::endl;
    std::cout << "Sqr: " << (a.sqr() == sqr ? "ok" : "not ok") << std::endl;
    std::cout << "Pow: " << ((a ^ n) == pow ? "ok" : "not ok") << std::endl;

    return 0;
}