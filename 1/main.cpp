#include <iostream>

class Foo {
public:
    operator int() const {
        return *this;
    }
};

int main() {
    Foo foo;
    int i = foo;
    return 0;
}
