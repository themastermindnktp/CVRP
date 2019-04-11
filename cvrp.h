struct cvrpSol
{
    int     length;
    int*    route;
    bool*   endRoute;

    void initialize(const int& n)
    {
        length = INT_MAX;
        route = (int*) calloc(n, sizeof(int));
        endRoute = (bool*) calloc(n, sizeof(bool));

    }

    void release()
    {
        free(route);
        free(endRoute);

    }

    void update(const cvrpSol& sol, const int& n)
    {
        if (length <= sol.length) return;
        length = sol.length;
        for(int i = 1; i < n; ++i)
        {
            route[i] = sol.route[i];
            endRoute[i] = sol.endRoute[i];

        }

    }

};

struct cvrpProb
{
    string  name;

    int     n;
    int     depot;
    int     capacity;

    point*  node;
    int*    demand;
    int**   distance;

    cvrpSol solution;

    void initialize()
    {
        node = (point*) calloc(n, sizeof(point));
        demand = (int*) calloc(n, sizeof(int));

        distance = (int**) calloc(n, sizeof(int*));
        for(int i = 0; i < n; ++i)
            distance[i] = (int*) calloc(n, sizeof(int));

        solution.initialize(n);

    }

    void release()
    {
        free(node);
        free(demand);

        for(int i = 0; i < n; ++i) free(distance[i]);
        free(distance);

        solution.release();

    }

    void calculateDistance()
    {
        for(int i = 0; i < n; ++i)
            for(int j = i + 1; j < n; ++j)
                distance[i][j] = distance[j][i] = _distance(node[i], node[j]);

    }

    void checkSolution()
    {
        bool* mark = (bool*) calloc(n, sizeof(bool));
        for(int i = 0; i < n; ++i) mark[i] = false;
        mark[depot] = true;

        int length = 0;
        int sumD = 0;

        for(int i = 1; i < n; ++i)
        {
            if (mark[solution.route[i]])
                inform("Wrong solution: Repeated node");
            mark[solution.route[i]] = true;

            sumD += demand[solution.route[i]];
            if (sumD > capacity)
                inform("Wrong solution: Extended capacity");

            if (solution.endRoute[i-1])
                length += distance[depot][solution.route[i]];
            else
                length += distance[solution.route[i-1]][solution.route[i]];

            if (solution.endRoute[i])
            {
                length += distance[solution.route[i]][depot];
                sumD = 0;

            }

            if (length > solution.length)
                inform("Wrong solution: Unmatched length");

        }

        if (length != solution.length)
            inform("Wrong solution: Unmatched length");
        inform("Solution has been checked");

        free(mark);

    }

};

const string settingFileName = "setting";

cvrpProb    problem;

string      inputFileName;
string      outputFileName;

int         nAnt;
int         nRound;
int         nAttempt;

double      RHO;
double      TMIN;
double      TMAX;
double      ALPHA;
double      BETA;

int         timeLimit;
