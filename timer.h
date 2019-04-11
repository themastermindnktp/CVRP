struct timer
{
    time_t startTime;

    void start()
    {
        time(&startTime);
    }

    double duration()
    {
        return difftime(time(NULL), startTime);

    }

};
