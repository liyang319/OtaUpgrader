#include "OTAUtility.h"
#include "Base.h"
#include "Utility.h"
#include "DataDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <iostream>

void OTAUtility::OtaRecovery()
{
    COUT << "OtaRecovery" << endl;
    Utility::startApp(std::string(DEFAULT_OTA_SAVE_PATH) + RESTORE_SCRIPT_NAME, false);
}

void OTAUtility::OtaBackup()
{
    COUT << "OtaBackup" << endl;
    Utility::copyFileTo(APP_NAME, DEFAULT_OTA_BACKUP_PATH);
    Utility::copyFileTo(CONFIG_NAME, DEFAULT_OTA_BACKUP_PATH);
    Utility::copyFileTo(UPDATER_NAME, DEFAULT_OTA_BACKUP_PATH);
}

void OTAUtility::OtaReplace()
{
    sleep(2);
    COUT << "Replace old version" << std::endl;
    Utility::startApp(std::string(DEFAULT_OTA_SAVE_PATH) + UPDATE_REPLACE_SCRIPT_NAME, false);
    if (Utility::isFileEmpty(DEFAULT_APP_PATH))
    {
        COUT << "Replace again!!! with backup version" << std::endl;
        Utility::replaceFileWithCmd(DEFAULT_APP_PATH, std::string(DEFAULT_OTA_BACKUP_PATH) + APP_NAME);
    }
    sleep(1);
}

void OTAUtility::OtaUnzipPkg(const std::string &pkgName)
{
    COUT << "Unzip Ota package" << endl;
    int status = Utility::unzipFile(pkgName, DEFAULT_OTA_SAVE_PATH);
    sleep(2);
    Utility::changeFileMode(std::string(DEFAULT_OTA_SAVE_PATH) + APP_NAME, DEFAULT_APP_RIGHTS);
    Utility::changeFileMode(std::string(DEFAULT_OTA_SAVE_PATH) + RESTORE_SCRIPT_NAME, DEFAULT_APP_RIGHTS);
    Utility::changeFileMode(std::string(DEFAULT_OTA_SAVE_PATH) + UPDATE_REPLACE_SCRIPT_NAME, DEFAULT_APP_RIGHTS);
    COUT << "Unzip Ota package status = " << status << endl;
}

void OTAUtility::StartApp()
{
    COUT << "==================StartApp==================" << endl;
    int tryTime = 0;
    bool bLaunched = false;
    while (tryTime++ < MAX_LAUNCH_TRY_TIME)
    {
        COUT << "==================StartApp==================" << tryTime << endl;
        bLaunched = Utility::startApp(DEFAULT_APP_PATH, true);
        if (bLaunched)
        {
            COUT << APP_NAME << " 启动成功" << endl;
            break;
        }
        else
        {
            COUT << APP_NAME << " 启动失败" << endl;
        }
        sleep(1);
    }
    // if (!bLaunched)
    // {
    //     OtaRecovery();
    // }
}

void OTAUtility::RestartApp()
{
    COUT << "==================RestartApp==================" << endl;
    int status = Utility::killApp(APP_NAME);
    if (status == 0)
    {
        StartApp();
    }
    else
    {
        COUT << "RestartApp failed" << endl;
    }
}