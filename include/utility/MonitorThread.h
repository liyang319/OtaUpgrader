#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <thread>
#include "IPC.h"

class MonitorThread
{
public:
    MonitorThread();

    ~MonitorThread();

    void start();

    void stop();

    void join();

private:
    std::thread m_thread;
    bool m_running;
    // IPC ipc;
    int msgIndex;
    void threadFunction();
    bool DoOTA();
};

#endif // MONITORTHREAD_H
