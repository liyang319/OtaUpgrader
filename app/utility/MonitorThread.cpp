#include "MonitorThread.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include "Base.h"
#include "Logger.h"
#include "Utility.h"
#include "HttpUtility.h"
#include "DataDef.h"
#include "OTAUtility.h"

#define DEFAULT_OTA_CONFIG_PATH "./ota_config.ini"
#define OTA_CONFIG_URL "url"
#define OTA_CONFIG_VERSION "version"
#define OTA_CONFIG_MD5 "md5"

using namespace std;

MonitorThread::MonitorThread() : m_running(false), msgIndex(0) {}

MonitorThread::~MonitorThread()
{
    stop();
}

void MonitorThread::start()
{
    if (!m_running)
    {
        m_running = true;
        m_thread = std::thread(&MonitorThread::threadFunction, this);
    }
}

void MonitorThread::stop()
{
    if (m_running)
    {
        m_running = false;
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
}

void MonitorThread::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void MonitorThread::threadFunction()
{
    IPC::getInstance().clear_memory();
    // ipc.clear_message_queue();
    while (m_running)
    {
        // Upload Status data here
        // COUT << "-----MonitorThread------" << std::endl;
        std::string message = "";
        // message = IPC::getInstance().recv_message_with_timeout(10);
        message = IPC::getInstance().recv_message();
        // COUT << "-----msg------" << msgIndex++ << " ---- " << message << std::endl;
        size_t ota_found = message.find("OTA");
        if (ota_found != std::string::npos)
        {
            DoOTA();
            IPC::getInstance().clear_memory();
            Utility::deleteDirectory(DEFAULT_OTA_SAVE_PATH);
        }

        size_t restart_found = message.find("RESTART");
        if (restart_found != std::string::npos)
        {
            OTAUtility::RestartApp();
            IPC::getInstance().clear_memory();
        }

        sleep(2);
    }
}

bool MonitorThread::DoOTA()
{
    COUT << "-------DO OTA-----" << endl;
    Utility::createDirIfNotExist(DEFAULT_OTA_SAVE_PATH);
    Utility::createDirIfNotExist(DEFAULT_OTA_BACKUP_PATH);
    string fileUrl = "";
    Utility::readMyConfig(DEFAULT_OTA_CONFIG_PATH, OTA_CONFIG_URL, fileUrl);
    std::string outputFile = DEFAULT_OTA_SAVE_PATH + Utility::getFilenameFromUrl(fileUrl);
    COUT << "----fileUrl---" << fileUrl << "----outputFile----" << outputFile << endl;
    int downloadRes = HttpUtility::httpdownload(fileUrl, outputFile);
    if (downloadRes == CURLE_OK)
    {
        COUT << "File downloaded to: " << outputFile << std::endl;
        sleep(1);
        // return true;
        //  std::string otaMd5 = Utility::calculateMD5(outputFile);
        //  COUT << "Md5 checking" << md5 << std::endl;
        //  if (md5 != otaMd5)
        //  {
        //      COUT << "Invalid Md5" << std::endl;
        //      return 0;
        //  }
        //  else
        //  {
        //      COUT << "Md5 OK" << std::endl;
        //  }
        //  备份原有版本到ota_backup
        OTAUtility::OtaBackup();
        // 解压升级包，修改权限
        OTAUtility::OtaUnzipPkg(outputFile);
        // // 替换新文件
        OTAUtility::OtaReplace();
        Utility::CloseWatchDog(); // 升级关狗
        sleep(3);
        OTAUtility::StartApp();
    }
    return true;
}
