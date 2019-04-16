struct environment
{
    ant         a;
    double**    t;
    cvrpSol     sol;

    bool        gBestFlag;
    int         gBestLength;
    int*        gBestPath;

    int         iBestLength;
    int*        iBestPath;

    int         n;
    int         depot;
    int         capacity;
    int*        demand;
    int**       distance;

    int         tLength;
    int*        tPath;
    int*        tCap;

    int*        dp;
    int*        trace;
    bool*       mark;

    environment(const cvrpProb& prob)
    {
        n           = prob.n;
        depot       = prob.depot;
        capacity    = prob.capacity;
        demand      = prob.demand;
        distance    = prob.distance;

        a.initialize(problem);
        t = (double**) calloc(n, sizeof(double*));
        for(int i = 0; i < n; ++i)
            t[i] = (double*) calloc(n, sizeof(double));
        sol.initialize(n);

        gBestPath = (int*) calloc(n, sizeof(int));
        iBestPath = (int*) calloc(n, sizeof(int));

        tPath   = (int*) calloc(n, sizeof(int));
        tCap    = (int*) calloc(n, sizeof(int));

        dp      = (int*) calloc(n, sizeof(int));
        trace   = (int*) calloc(n, sizeof(int));
        mark    = (bool*) calloc(n, sizeof(int));

    }

    void release()
    {
        a.release();
        for(int i = 0; i < n; ++i)
            free(t[i]);
        free(t);
        sol.release();

        free(gBestPath);
        free(iBestPath);

        free(tPath);
        free(tCap);

        free(dp);
        free(trace);
        free(mark);

    }

    void initialize()
    {
        srand(time(NULL));

        sol.initialize(n);

        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                t[i][j] = TMAX;

        gBestFlag = rand() & 1;
        gBestLength = INT_MAX;

    }

    void updatePheromone()
    {
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                t[i][j] = t[i][j] * (1 - RHO) + RHO * TMIN;

        if (gBestFlag)
        {
            for(int i = 1; i < n; ++i)
                t[gBestPath[i-1]][gBestPath[i]] += RHO * (TMAX - TMIN);
            t[gBestPath[n-1]][gBestPath[0]] += RHO * (TMAX - TMIN);

        }
        else
        {
            for(int i = 1; i < n; ++i)
                t[iBestPath[i-1]][iBestPath[i]] += RHO * (TMAX - TMIN);
            t[iBestPath[n-1]][iBestPath[0]] += RHO * (TMAX - TMIN);

        }

    }

    void tspSwap(const int &i, const int &j)
    {
        if (i == 0)
            iBestLength -= distance[iBestPath[n-1]][iBestPath[i]];
        else
            iBestLength -= distance[iBestPath[i-1]][iBestPath[i]];
        iBestLength -= distance[iBestPath[i]][iBestPath[i+1]];

        iBestLength -= distance[iBestPath[j-1]][iBestPath[j]];
        if (j == n - 1)
            iBestLength -= distance[iBestPath[j]][iBestPath[0]];
        else
            iBestLength -= distance[iBestPath[j]][iBestPath[j+1]];

        swap(iBestPath[i], iBestPath[j]);

        if (i == 0)
            iBestLength += distance[iBestPath[n-1]][iBestPath[i]];
        else
            iBestLength += distance[iBestPath[i-1]][iBestPath[i]];
        iBestLength += distance[iBestPath[i]][iBestPath[i+1]];

        iBestLength += distance[iBestPath[j-1]][iBestPath[j]];
        if (j == n - 1)
            iBestLength += distance[iBestPath[j]][iBestPath[0]];
        else
            iBestLength += distance[iBestPath[j]][iBestPath[j+1]];

    }

    bool tsp2Opt()
    {
        bool opt = false;
        for(int i = 0; i < n; ++i)
            for(int j = i + 1; j < n; ++j)
            {
                int tmp = iBestLength;
                tspSwap(i, j);
                if (iBestLength < tmp) opt = true;
                if (iBestLength > tmp) tspSwap(i, j);

            }

        return opt;

    }

    void tspLocalSearch()
    {
        while (tsp2Opt()) ;

    }

    int cvrpRoute(const int &i)
    {
        return (mark[i-1] ? distance[depot][tPath[i]] : distance[tPath[i-1]][tPath[i]]) +
               (mark[i]   ? distance[tPath[i]][depot] : distance[tPath[i]][tPath[i+1]]);

    }

    void cvrpSwap(const int& i, const int& j)
    {
        tLength -= cvrpRoute(i) + cvrpRoute(j);

        swap(tPath[i], tPath[j]);

        tLength += cvrpRoute(i) + cvrpRoute(j);

    }

    bool cvrp2Opt()
    {
        int p = 0;
        int sumD = 0;

        for(int i = 1; i < n; ++i)
        {
            sumD += demand[tPath[i]];
            if (mark[i])
            {
                while (p < i) tCap[++p] = sumD;
                sumD = 0;

            }

        }

        for(int i = 1; i < n; ++i)
        {
            bool changeRoute = mark[i];
            for(int j = i + 1; j < n; ++j)
            {
                if ((!changeRoute) ||
                    max(tCap[i] + demand[tPath[j]] - demand[tPath[i]], tCap[j] + demand[tPath[i]] - demand[tPath[j]]) <= capacity)
                {
                    int tmp = tLength;
                    cvrpSwap(i, j);
                    if (tLength < tmp) return true;
                    if (tLength > tmp) cvrpSwap(i, j);

                }

                if (mark[j]) changeRoute = true;

            }

        }

        return false;

    }

    void cvrpLocalSearch()
    {
        while (cvrp2Opt()) ;

    }

    void randomMix()
    {
        for(int i = 0; i < n; ++i)
            if (rand() % (n / 2) == 0)
                swap(iBestPath[i], iBestPath[rand() % n]);

        iBestLength = 0;
        for(int i = 1; i < n; ++i)
            iBestLength += distance[iBestPath[i-1]][iBestPath[i]];
        iBestLength += distance[iBestPath[n-1]][iBestPath[0]];

    }

    bool dynamicProgramming(int* path)
    {
        int cnt = 0;

        for(int i = 0; i < n; ++i)
        {
            if (path[i] != depot) tPath[++cnt] = path[i];

            dp[i] = INT_MAX;
            mark[i] = false;

        }

        dp[0] = 0;

        for(int i = 0; i < n; ++i)
        {
            int sumD = 0;
            int sumL = -distance[tPath[i]][tPath[i+1]];

            for(int j = i + 1; j < n; ++j)
            {
                sumD += demand[tPath[j]];
                if (sumD > capacity) break;
                sumL += distance[tPath[j-1]][tPath[j]];

                if (dp[j] > dp[i] + distance[depot][tPath[i+1]] + sumL + distance[tPath[j]][depot])
                {
                    dp[j] = dp[i] + distance[depot][tPath[i+1]] + sumL + distance[tPath[j]][depot];
                    trace[j] = i;

                }

            }
        }

        tLength = dp[n-1];

        int p = n - 1;
        do
        {
            mark[p] = true;
            p = trace[p];

        }
        while (p);

        cvrpLocalSearch();

        if (sol.length > tLength)
        {
            sol.length = tLength;
            for(int i = 0; i < n; ++i)
            {
                sol.route[i] = tPath[i];
                sol.endRoute[i] = mark[i];

            }

            return true;

        }

        return false;

    }

    void process()
    {
        int cnt;
        bool change;

        for(int round = 1; round <= nRound; ++round)
        {
            iBestLength = INT_MAX;

            for(int i = 0; i < nAnt; ++i)
            {
                a.colonize(t);

                if (iBestLength >= a.length)
                {
                    if (iBestLength == a.length && rand() % 2) continue;

                    iBestLength = a.length;
                    for(int i = 0; i < n; ++i)
                        iBestPath[i] = a.path[i];

                }

            }

            change = dynamicProgramming(iBestPath);

            tspLocalSearch();

            change = dynamicProgramming(iBestPath);

            if (gBestLength > iBestLength)
            {
                change = true;
                gBestLength = iBestLength;
                for(int i = 0; i < n; ++i)
                    gBestPath[i] = iBestPath[i];

            }

            randomMix();

            if (change) cnt = 0;
            else cnt++;

            if (cnt == 20) break;

            if (round != nRound) updatePheromone();

        }

        cerr << "\tTSP: " << gBestLength <<
                "\tCVRP: " << sol.length <<
                "\t" << (gBestFlag ? "(G-Best)" : "(I-Best)") << "\n";

        problem.solution.update(sol, n);

    }

};
