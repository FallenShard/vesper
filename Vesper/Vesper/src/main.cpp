#include "core/Application.hpp"

int main()
{
    Application app;

    Application::Status status = app.initializeDependencies();
    if (status == Application::Status::Failure)
        return -1;

    status = app.initializeComponents();
    if (status == Application::Status::Failure)
        return -1;

    app.run();
    return 0;
}