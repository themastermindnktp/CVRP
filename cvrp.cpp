#include <bits/stdc++.h>

using namespace std;

#include "interface.h"
#include "timer.h"
#include "point.h"
#include "cvrp.h"
#include "reader.h"
#include "writer.h"
#include "ant.h"
#include "environment.h"

int main()
{
    timer totalTime;
    totalTime.start();

    readSetting();
    readData(problem);

    environment env(problem);

    for(int attempt = 1; attempt <= nAttempt; ++attempt)
    {
        timer attemptTime;
        attemptTime.start();

        showValue("Attempt #", attempt, ":\n");

        env.initialize();
        env.process();

        showValue("Time: ", attemptTime.duration(), "s");

        inform("__________________________________");

        if (totalTime.duration() > timeLimit) break;

    }

    env.release();

    showValue("Best CVRP Solution: ", problem.solution.length, "");

    writeSolution(problem);

}
