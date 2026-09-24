#ifndef MSGDISPATCHER_H
#define MSGDISPATCHER_H

#include "rapidjson/document.h"
#include <string>
#include "DataDef.h"

class MsgDispatcher
{
public:
    MsgDispatcher(std::string &json_data);

    void dispatchMsg();

private:
    void processOtaCmd();
    void processRestartCmd();

    rapidjson::Document m_document;
};

#endif // MSGDISPATCHER_H
