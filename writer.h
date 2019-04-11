void writeSolution(cvrpProb& prob)
{
    problem.checkSolution();

    ofstream output(outputFileName.c_str());

    if (output.fail())
    {
        inform("Cannot saving full solution into target file. It will be write to file 'temporary'.");
        output.open("temporary", ostream::out);

    }

    if (output.fail())
        interrupt("Cannot saving full solution into file");

    int cnt = 1;

    cvrpSol sol = problem.solution;

    output << "Route #1: " << prob.depot + 1 << " ";

    for(int i = 1; i < prob.n; ++i)
    {
        output << sol.route[i] + 1 << " ";
        if (sol.endRoute[i])
        {
            output << prob.depot + 1 << "\n";
            if (i != prob.n - 1)
                output << "Route #" << ++cnt << " " << prob.depot + 1 << " ";

        }

    }

    cerr << "\nNumber of Route: " << cnt << "\n";

    output << "Cost: " << sol.length;

    output.close();

}
