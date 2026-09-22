#ifndef UTILITY_H
#define UTILITY_H

#include <string>

class Utility
{
public:
    static std::string removeTrailingNewline(std::string str);
    static std::string getFileContent(std::string filename);
    // static std::string calculateMD5(const std::string &file_path);
    static int replaceFileWithCmd(std::string orginalFile, std::string newFile);
    static int replaceFile(const std::string orginalFile, const std::string newFile);
    static int runFile(std::string executablePath, bool bBackground);
    static int killApp(std::string processName);
    static int changeFileMode(std::string filename, std::string mode);
    static std::string calculateMD5(std::string filename);
    static void fillVersionFile(std::string filename, std::string content);
    static int unzipFile(std::string zipFileName, std::string outputDirectory);
    static std::string getFilenameFromUrl(std::string url);
    static void deleteFiles(std::string dir);
    static bool deleteDirectory(std::string dir);
    static bool createDirIfNotExist(std::string dirPath);
    static bool copyFileTo(std::string filePath, std::string dstPath);
    static bool fileExists(std::string fileName);
    static bool isExecutable(std::string fileName);
    static bool startApp(std::string executablePath, bool bBackground);
    static bool isFileEmpty(std::string filename);
    static void FeedWatchDog();
    static void CloseWatchDog();
    static bool copyFile(std::string &srcFile, std::string &destFile);
    static bool removeFile(std::string &filePath);
    static bool readMyConfig(std::string filePath, std::string key, std::string &value);
    static bool writeMyConfig(std::string filePath, std::string key, std::string value);
};

#endif // UTILITY_H
