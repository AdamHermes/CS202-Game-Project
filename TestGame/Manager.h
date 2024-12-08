#include <string>
#ifndef MANAGER_H
#define MANAGER_H
using namespace std;
class Manager {
public:
    virtual void notify(const std::string& event, float value = 0) = 0;
    virtual ~Manager() = default;
};
#endif
