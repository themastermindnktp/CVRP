void readSetting()
{
    ifstream input(settingFileName.c_str());

    inform("Reading setting file");

    getline(input, inputFileName);
    getline(input, outputFileName);

    input >> nAnt >> nRound >> nAttempt;
    input >> RHO >> TMIN >> TMAX >> ALPHA >> BETA;
    input >> timeLimit;

    inform("Finish reading setting file");

    input.close();

}

void setEdgeWeightType(const string& type)
{
    if (!type.compare("EUC_2D"))
        _distance = euc_2d_distance;
    else
    if (!type.compare("CEIL_2D"))
        _distance = ceil_2d_distance;
    else
    if (!type.compare("GEO"))
        _distance = geo_distance;
    else
    if (!type.compare("ATT"))
        _distance = att_distance;
    else interrupt("Edge Weight Type " + type + "are not implemented in this program");

}

void readData(cvrpProb& prob)
{
    ifstream input(inputFileName.c_str());
    string buffer;

    bool readType               = false;
    bool readDimension          = false;
    bool readEdgeWeightType     = false;
    bool readCapacity           = false;
    bool readNodeCoordSection   = false;
    bool readDemand             = false;
    bool readDepotSection       = false;

    if (input.fail())
        interrupt("Cannot open file " + inputFileName);

    inform("Reading data file " + inputFileName);

    while (input >> buffer)
    {
        if (!buffer.compare("NAME"))
        {
            input >> buffer >> prob.name;

        }
        else
        if (!buffer.compare("NAME:"))
        {
            input >> buffer >> prob.name;

        }
        else
        if (!buffer.compare("COMMENT"))
        {
            getline(input, buffer);

        }
        else
        if (!buffer.compare("COMMENT:"))
        {
            getline(input, buffer);

        }
        else
        if (!buffer.compare("TYPE"))
        {
            input >> buffer >> buffer;
            if (buffer.compare("CVRP"))
                interrupt("Wrong problem type\n\
                          This program is for solving Capacitated Vehicle Routing Problem only");
            readType = true;

        }
        else
        if (!buffer.compare("TYPE:"))
        {
            input >> buffer;
            if (buffer.compare("CVRP"))
                interrupt("Wrong problem type\n\
                          This program is for solving Capacitated Vehicle Routing Problem only");
            readType = true;

        }
        else
        if (!buffer.compare("DIMENSION"))
        {
            input >> buffer >> prob.n;
            prob.initialize();
            readDimension = true;

        }
        else
        if (!buffer.compare("DIMENSION:"))
        {
            input >> prob.n;
            prob.initialize();
            readDimension = true;

        }
        else
        if (!buffer.compare("EDGE_WEIGHT_TYPE"))
        {
            input >> buffer >> buffer;
            setEdgeWeightType(buffer);
            readEdgeWeightType = true;

        }
        else
        if (!buffer.compare("EDGE_WEIGHT_TYPE:"))
        {
            input >> buffer;
            setEdgeWeightType(buffer);
            readEdgeWeightType = true;

        }
        else
        if (!buffer.compare("CAPACITY"))
        {
            input >> buffer >> prob.capacity;
            readCapacity = true;

        }
        else
        if (!buffer.compare("CAPACITY:"))
        {
            input >> prob.capacity;
            readCapacity = true;

        }
        else
        if (!buffer.compare("NODE_COORD_SECTION"))
        {
            if (!(readDimension && readEdgeWeightType && readCapacity))
                interrupt("Reading Node Coordinate Section: Missing needed information");

            int id;
            for(int i = 0; i < prob.n; ++i)
            {
                input >> id >> prob.node[i];
                if (id != i + 1)
                    interrupt("Error reading Node Coordinate Section");

            }

            prob.calculateDistance();

            readNodeCoordSection = true;

        }
        else
        if (!buffer.compare("DEMAND_SECTION"))
        {
            if (!(readType && readDimension))
                interrupt("Reading Demand Section: Missing needed information");

            int id;
            for(int i = 0; i < prob.n; ++i)
            {
                input >> id >> prob.demand[i];
                if (id != i + 1)
                    interrupt("Error reading Demand Section");
                if (prob.demand[i] > prob.capacity)
                    interrupt("The demand of one node is greater than the Capacity, this data has no solution");

            }

            readDemand = true;

        }
        else
        if (!buffer.compare("DEPOT_SECTION"))
        {
            if (!(readType && readDimension))
                interrupt("Reading Depot Section: Missing needed information");

            input >> prob.depot >> buffer;
            prob.depot--;
            if (buffer.compare("-1"))
                interrupt("This problem is for solving with single depot section only");

            readDepotSection = true;

        }
        else
        if (!buffer.compare("EOF"))
        {
            if (!(readType && readDimension && readEdgeWeightType && readCapacity &&
                  readNodeCoordSection && readDemand && readDepotSection))
                    interrupt("Missing needed information");
            break;
        }
        else
            interrupt("Unidentified information in data file");

    }

    inform("Finish reading data file");

    input.close();

}
