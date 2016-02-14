#include "core/Application.hpp"

int main()
{
    vesp::Application app;

    vesp::Application::Status status = app.initializeDependencies();
    if (status == vesp::Application::Status::Failure)
        return -1;

    status = app.initializeComponents();
    if (status == vesp::Application::Status::Failure)
        return -1;

    app.run();
    return 0;
}