#include <iostream>
#include <time.h>
#include <functional>

using namespace std;

struct Action
{
    Action(function<void()> action, clock_t startTime)
    :action(action), startTime(startTime){};

    void update()
    {
        if(startTime - clock() <= 0 && !done)
        {
            action();
            done = true;
        }
    }

    function<void()> action;
    clock_t startTime;
    bool done = false;
};
