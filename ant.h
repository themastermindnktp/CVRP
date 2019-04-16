struct ant
{
    int     length;
    int     rest;
    int*    path;
    int*    remain;
    double* p;

    int     n;
    int**   distance;

    void initialize(const cvrpProb& prob)
    {
        n = prob.n;
        distance = prob.distance;

        path    = (int*) calloc(n, sizeof(int));
        remain  = (int*) calloc(n, sizeof(int));
        p       = (double*) calloc(n, sizeof(double));

    }

    void release()
    {
        free(path);
        free(remain);
        free(p);

    }

    int next(const int& current, double* tau)
    {
        double sum = 0.0;

        for(int i = 0; i < rest; ++i)
        {
            p[i] = pow(tau[remain[i]], ALPHA) / pow(distance[current][remain[i]], BETA);
            sum += p[i];

        }

        double prob = ((double) rand() * rand() / RAND_MAX / RAND_MAX);

        double tmp;

        for(int i = 0; i < rest; ++i)
        {
            tmp = p[i] / sum;
            if (prob <= tmp) return i;
            prob -= tmp;
        }

        interrupt("Error while ants colonizing");

        return -1;

    }

    void colonize(double** t)
    {
        length = 0;

        path[0] = rand() % n;

        rest = 0;

        for(int i = 0; i < n; ++i)
            if (i != path[0]) remain[rest++] = i;

        for(int i = 0; i < rest; ++i)
            swap(remain[i], remain[rand() % rest]);

        int id;
        for(int i = 1; i < n; ++i)
        {
            id = next(path[i-1], t[path[i-1]]);
            path[i] = remain[id];
            length += distance[path[i-1]][path[i]];
            swap(remain[id], remain[--rest]);

        }

        length += distance[path[n-1]][path[0]];

    }

};
