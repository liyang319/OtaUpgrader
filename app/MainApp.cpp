#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include "HttpUtility.h"
#include "Utility.h"

#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include <thread>
#include <chrono>

#include <openssl/md5.h>
#include <iomanip>
#include <sstream>

#include <dirent.h>
#include <sys/stat.h>
#include <ctime>

#include "Base.h"
#include "Version.h"
#include "MonitorThread.h"
#include "DataDef.h"
#include "OTAUtility.h"
#include "MqttThread.h"
#include "MsgWorkerThread.h"

// #define USE_OTA_CHECK

using namespace rapidjson;
// maeusing namespace std;

int i = 0;
bool bExitOtaCheck = false;
bool bExitManageLog = false;

int DoOTA(std::string json)
{
    // 解析JSON字符串
    rapidjson::Document document;
    document.Parse(json.c_str());

    // 检查解析是否成功
    if (!document.IsObject())
    {
        COUT << "response解析失败！" << std::endl;
        return 0;
    }

    // 从JSON中获取值
    std::string status = document["status"].GetString();
    std::string needUpdate = document["needUpdate"].GetString();
    std::string url = document["url"].GetString();
    std::string md5 = document["md5"].GetString();
    std::string newVer = document["newVer"].GetString();

    // 打印获取的值
    COUT << "状态: " << status << std::endl;
    COUT << "是否有新版本: " << needUpdate << std::endl;
    COUT << "URL: ======" << url << "========" << std::endl;
    COUT << "MD5: " << md5 << std::endl;
    COUT << "newVer: " << newVer << std::endl;
    if (needUpdate == "true")
    {
        COUT << "Has new version" << std::endl;
        Utility::deleteDirectory(DEFAULT_OTA_SAVE_PATH);
        Utility::deleteDirectory(DEFAULT_OTA_BACKUP_PATH);
        // 创建升级包保存位置ota_save
        Utility::createDirIfNotExist(DEFAULT_OTA_SAVE_PATH);
        // 创建原程序备份位置ota_backup
        Utility::createDirIfNotExist(DEFAULT_OTA_BACKUP_PATH);

        std::string outputFile = DEFAULT_OTA_SAVE_PATH + Utility::getFilenameFromUrl(url);
        int downloadRes = HttpUtility::httpdownload(url, outputFile);
        if (downloadRes == CURLE_OK)
        {
            COUT << "File downloaded to: " << outputFile << std::endl;
            sleep(1);
            std::string otaMd5 = Utility::calculateMD5(outputFile);
            COUT << "Md5 checking" << md5 << std::endl;
            if (md5 != otaMd5)
            {
                COUT << "Invalid Md5" << std::endl;
                return 0;
            }
            else
            {
                COUT << "Md5 OK" << std::endl;
            }
            // 备份原有版本到ota_backup
            OTAUtility::OtaBackup();
            // 解压升级包，修改权限
            OTAUtility::OtaUnzipPkg(outputFile);
            // // 替换新文件
            OTAUtility::OtaReplace();
            Utility::CloseWatchDog(); // 升级关狗
            sleep(3);
            OTAUtility::StartApp();
            // Utility::FeedWatchDog(); // 重新开狗
        }
    }
    else
    {
        COUT << "No new version" << std::endl;
    }
    return 1;
}

void OtaCheck()
{
    COUT << "Checking OTA" << i++ << std::endl;
    std::string strVer = Utility::removeTrailingNewline(Utility::getFileContent(DEFAULT_VERSION_PATH));
    std::string deviceSN = Utility::removeTrailingNewline(Utility::getFileContent(DEFAULT_SN_FILE_PATH));

    if (strVer.empty())
    {
        strVer = "none";
    }
    if (deviceSN.empty())
    {
        deviceSN = "none";
    }

    // std::string deviceSN = DEVICE_SN;

    std::map<std::string, std::string> mapParam = {
        {"cmd", "otacheck"},
        {"version", strVer},
        {"sn", deviceSN}};

    std::string strParam = HttpUtility::buildQueryString(mapParam);
    // COUT << "====check params====" << strParam << endl;
    std::string response;
    CURLcode getRes = HttpUtility::httpget(URL_CHECK_OTA, strParam, response, 30);
    if (getRes == CURLE_OK)
    {
        COUT << "Get request successful" << std::endl;
        COUT << "Response: " << response << std::endl;
        DoOTA(response);
    }
}

int DoLogOperation(std::string json, std::string deviceSN)
{
    // 解析JSON字符串
    rapidjson::Document document;
    document.Parse(json.c_str());
    std::string outputFile = DEFAULT_OTA_SAVE_PATH;

    // 检查解析是否成功
    if (!document.IsObject())
    {
        COUT << "log请求response解析失败！" << endl;
        return 0;
    }

    // 从JSON中获取值
    std::string status = document["status"].GetString();
    std::string logStatus = document["log"].GetString();
    std::string path = document["path"].GetString();
    std::string logDate = document["date"].GetString();

    // 打印获取的值
    COUT << "状态: " << status << endl;
    COUT << "log: " << logStatus << endl;
    COUT << "path: " << path << endl;
    COUT << "date: " << logDate << endl;

    if (logStatus == LOGVAL_NEED_UPLOAD)
    {
        // Do upload here
        std::string logFileName = DEFAULT_LOGFILE_PREFIX + logDate + ".log";
        std::string uploadFileName = std::string("wb_") + logFileName;
        Utility::copyFile(logFileName, uploadFileName);
        COUT << "-----logfile-----" << uploadFileName << endl;
        usleep(1000000);
        if (Utility::fileExists(uploadFileName))
        {
            HttpUtility::httpUploadFile(URL_UPLOAD_LOG, uploadFileName, uploadFileName, deviceSN);
        }
        else
        {
            COUT << "File not exist" << endl;
        }
        Utility::removeFile(uploadFileName);
    }
    return 0;
}

void LogCheck()
{
    COUT << "Checking if need to upload log" << i++ << std::endl;
    std::string strVer = Utility::removeTrailingNewline(Utility::getFileContent(DEFAULT_VERSION_PATH));
    std::string deviceSN = Utility::removeTrailingNewline(Utility::getFileContent(DEFAULT_SN_FILE_PATH));

    if (strVer.empty())
    {
        strVer = "none";
    }
    if (deviceSN.empty())
    {
        deviceSN = "none";
    }

    std::map<std::string, std::string>
        mapParam = {
            {"cmd", "logcheck"},
            {"version", strVer},
            {"sn", deviceSN}};
    std::string strParam = HttpUtility::buildQueryString(mapParam);
    std::string response;
    CURLcode getRes = HttpUtility::httpget(URL_CHECK_LOG, strParam, response, 30);
    if (getRes == CURLE_OK)
    {
        COUT << "Get request successful" << endl;
        COUT << "Response: " << response << endl;
        DoLogOperation(response, deviceSN);
    }
}

void cleanOldLogs(const std::string &path)
{
    time_t now = time(0);
    // struct tm *timeinfo = localtime(&now);
    // timeinfo->tm_mday -= 7; // subtract 7 days
    COUT << "cleanOldLogs" << endl;
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            std::string filename = entry->d_name;
            if (filename.find(DEFAULT_LOGFILE_PREFIX) == 0)
            {
                // std::cout << "----filename--" << filename << std::endl;
                std::string date_str = filename.substr(9, 10); // extract date from filename
                // std::cout << "----data--" << date_str << std::endl;
                struct tm log_time = {};
                strptime(date_str.c_str(), "%Y-%m-%d", &log_time);
                time_t log_timestamp = mktime(&log_time);

                // Check if the log file is older than one week
                if (difftime(now, log_timestamp) > 7 * 24 * 60 * 60)
                {
                    std::string full_path = path + "/" + filename;
                    remove(full_path.c_str());
                    std::cout << "Deleted log file: " << full_path << std::endl;
                }
            }
        }
        closedir(dir);
    }
    else
    {
        std::cerr << "Error opening directory" << std::endl;
    }
}

void ManageLogFunction()
{
    while (!bExitManageLog)
    {
        cleanOldLogs(DEFAULT_LOG_PATH);
        std::this_thread::sleep_for(std::chrono::hours(24));
    }
}

void OTACheckFunction()
{
    while (!bExitOtaCheck)
    {
        OtaCheck();
        std::this_thread::sleep_for(std::chrono::seconds(30));
        LogCheck();
        std::this_thread::sleep_for(std::chrono::seconds(30));
        // std::this_thread::sleep_for(std::chrono::hours(1));
    }
}

int main()
{
    COUT << "=========OTAUPDATER=============" << VERSION << endl;
    int index = 1;
    // MonitorThread monitorThread;
    MsgWorkerThread msgWorkerThread;
    MqttThread mqttThread;
    // OtaCheck();
    // LogCheck();
    // std::this_thread::sleep_for(std::chrono::seconds(20));
    // std::thread manageLogThread = std::thread(ManageLogFunction);
    // #ifdef USE_OTA_CHECK
    //     std::thread otaCheckThread = std::thread(OTACheckFunction);
    // #endif
    // monitorThread.start();
    mqttThread.start();

    // manageLogThread.join();
    // #ifdef USE_OTA_CHECK
    //     otaCheckThread.join();
    // #endif
    //     monitorThread.join();
    mqttThread.join();
}
