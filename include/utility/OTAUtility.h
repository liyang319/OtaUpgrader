#ifndef OTAUTILITY_H
#define OTAUTILITY_H

#include <string>
#include "rapidjson/document.h"

class OTAUtility
{
public:
    static void OtaRecovery();
    static void OtaBackup();
    static void OtaReplace();
    static void OtaUnzipPkg(const std::string &pkgName);
    static void StartApp();
    static void RestartApp();
};

#endif // OTAUTILITY_H
