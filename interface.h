void inform(const string& message)
{
    cerr << "\n" << message << "\n";

}

void interrupt(const string& message)
{
    inform(message);
    exit(1);
}

void showValue(const string& prefix, const int& value, const string& suffix)
{
    cerr << "\n" << prefix << value << suffix << "\n";

}
