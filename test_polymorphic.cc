#include <iostream>
#include <memory>
#include <any>

using namespace std;

class Base {
    public:
        virtual string f1() = 0;
};

class Child1 : public Base {
    public:
        Child1(string ss): s{ss} {}
        virtual string f1() override {
            return s;
        }
    private:
        string s;
};

class Child2 : public Base {
    public:
        Child2(string ss): s{ss} {}
        virtual string f1() override {
            return s; 
        }
    private:
        string s;
};


int main(int argc, char *argv[]) {
    string s {"Call me with this"};
    unique_ptr<Base> base = make_unique<Child1>(s);
    cout << any_cast<string>("nil");
    cout << base->f1();
    return 0;
}