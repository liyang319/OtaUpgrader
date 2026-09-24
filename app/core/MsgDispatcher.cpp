#include "MsgDispatcher.h"
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Base.h"
#include "AppData.h"
#include "Utility.h"
#include "Base.h"
#include "DeviceConfig.h"
#include <sstream>
#include "DataDef.h"
#include "HttpUtility.h"
#include "IPC.h"

std::string convertToTwoDigits(int num)
{
    if (num < 10)
    {
        return "0" + std::to_string(num);
    }
    else
    {
        return std::to_string(num);
    }
}

MsgDispatcher::MsgDispatcher(std::string &json_data)
{
    COUT << "[Received StatusCmd] :  " << json_data << endl;
    m_document.Parse(json_data.c_str());
    if (m_document.HasParseError())
    {
        COUT << "json error" << endl;
    }
}

void MsgDispatcher::dispatchMsg()
{
    if (m_document.HasParseError())
    {
        COUT << "Failed to parse JSON data" << endl;
        return;
    }
    string cmd = "";
    // string url = "";
    // string cmdType = "";
    if (m_document.HasMember("cmd"))
    {
        cmd = m_document["cmd"].GetString();

        if (cmd == MSG_CMD_OTA)
        {
            processOtaCmd();
        }
        else if (cmd == MSG_CMD_RESTART)
        {
            processRestartCmd();
        }
        else
        {
            COUT << "Unknown command: " << cmd << endl;
        }
    }
    else
    {
        COUT << "Command not found in JSON" << endl;
        return;
    }
}

void MsgDispatcher::processOtaCmd()
{
    COUT << "-----processOtaCmd-----" << endl;
    uint16_t filterType = 0;
    if (m_document.HasMember("value"))
    {
        filterType = static_cast<uint16_t>(m_document["value"].GetInt());
    }
    else
    {
        return;
    }
    COUT << "mqtt filterType: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(filterType) << std::endl;
}

void MsgDispatcher::processRestartCmd()
{
    COUT << "-----processRestartCmd-----" << endl;
    uint16_t emptyLoadValue = 0;
    if (m_document.HasMember("value"))
    {
        emptyLoadValue = static_cast<uint16_t>(m_document["value"].GetInt());
    }
    else
    {
        return;
    }
    COUT << "mqtt emptyLoadValue: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(emptyLoadValue) << std::endl;
}
