#include "Utility.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "openssl/md5.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "Base.h"
#include <cerrno>
#include <cstring>
#include <unordered_map>

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

std::string Utility::removeTrailingNewline(std::string str)
{
    std::string modifiedStr = str;
    if (!modifiedStr.empty() && modifiedStr.back() == '\n')
    {
        modifiedStr.erase(modifiedStr.length() - 1);
    }
    return modifiedStr;
}

std::string Utility::getFileContent(std::string fileName)
{
    std::ifstream file(fileName);
    std::string content = "";
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            content += line;
            if (!file.eof())
            {
                content += "\n";
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file: " << fileName << std::endl;
    }

    return content;
}

int Utility::replaceFileWithCmd(std::string orginalFile, std::string newFile)
{
    std::string command = "cp " + newFile + " " + orginalFile;
    int result = system(command.c_str()); // 执行命令行

    if (result == 0)
    {
        return 0; // 成功
    }
    else
    {
        return -1; // 失败
    }
}

bool Utility::isFileEmpty(std::string filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate); // 打开文件并移动文件指针到文件末尾
    if (file.is_open())
    {
        std::streampos size = file.tellg(); // 获取文件指针位置，即文件大小
        file.close();                       // 关闭文件流

        return size == 0;
    }
    else
    {
        std::cout << "无法打开文件" << std::endl;
        return false;
    }
}

int replaceFile(const std::string orginalFile, const std::string newFile)
{
    std::ifstream newFileInput(newFile);
    if (!newFileInput)
    {
        std::cerr << "Error: Unable to open new file" << std::endl;
        return -1;
    }

    std::ofstream orginalFileOutput(orginalFile);
    if (!orginalFileOutput)
    {
        std::cerr << "Error: Unable to open orginal file" << std::endl;
        return -1;
    }

    orginalFileOutput << newFileInput.rdbuf();

    return 0;
}

int Utility::runFile(std::string executablePath, bool bBackground)
{
    // std::string command = "./" + executablePath; // 构建启动可执行文件的命令行
    // int result = system(command.c_str());        // 执行命令行
    // return result;
    std::string command = executablePath; // 使用绝对路径启动可执行文件
    if (bBackground)
        command += " &";
    int result = system(command.c_str()); // 执行命令行
    return result;
}

int Utility::killApp(std::string processName)
{
    // std::string processName = "example_process";     // 要杀死的进程名字
    std::string command = "pkill -x " + processName; // 构造要执行的命令
    int result = system(command.c_str());            // 执行命令
    if (result == 0)
    {
        COUT << "进程 " << processName << " 已成功被杀死。" << std::endl;
    }
    else
    {
        std::cerr << "无法杀死进程 " << processName << "。" << std::endl;
    }
    return result;
}

int Utility::changeFileMode(std::string filename, std::string mode)
{
    std::string command = "chmod " + mode + " " + filename;
    COUT << "---cmd = " << command << std::endl;
    int result = system(command.c_str());
    if (result == 0)
    {
        COUT << "文件权限修改成功" << std::endl;
    }
    else
    {
        std::cerr << "文件权限修改失败" << std::endl;
    }
    return result;
}

std::string Utility::calculateMD5(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return "";
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);

    char buffer[1024];
    while (file)
    {
        file.read(buffer, sizeof(buffer));
        std::streamsize count = file.gcount();
        if (count > 0)
        {
            MD5_Update(&ctx, buffer, count);
        }
    }

    unsigned char md[MD5_DIGEST_LENGTH];
    MD5_Final(md, &ctx);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md[i]);
    }

    return ss.str();
}

void Utility::fillVersionFile(std::string filename, std::string content)
{
    // Open the file in out mode to clear the original content
    std::ofstream file(filename, std::ios::out | std::ios::trunc);

    if (file.is_open())
    {
        // Write the new content to the file
        file << content;

        // Close the file
        file.close();

        COUT << "Content written to " << filename << " successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error opening file " << filename << std::endl;
    }
}

int Utility::unzipFile(std::string zipFileName, std::string outputDirectory)
{
    std::string command = "unzip " + zipFileName + " -d " + outputDirectory;
    COUT << "-----unzipFile-----" << command << endl;
    int result = system(command.c_str());
    // if (result != 0)
    // {
    //     std::cerr << "Error: Failed to unzip file " << zipFileName << std::endl;
    // }
    return result;
}

std::string Utility::getFilenameFromUrl(std::string url)
{
    size_t found = url.find_last_of("/\\");
    if (found != std::string::npos)
    {
        return url.substr(found + 1);
    }
    else
    {
        return "";
    }
}

void Utility::deleteFiles(std::string dir)
{
    DIR *dp = opendir(dir.c_str());
    if (dp != NULL)
    {
        struct dirent *ep;
        while ((ep = readdir(dp)))
        {
            if (ep->d_type == DT_REG)
            { // regular file
                std::string filePath = dir + "/" + ep->d_name;
                if (remove(filePath.c_str()) != 0)
                {
                    std::cerr << "Failed to delete file: " << filePath << std::endl;
                }
            }
        }
        closedir(dp);
    }
    else
    {
        std::cerr << "Failed to open directory: " << dir << std::endl;
    }
}

bool Utility::deleteDirectory(std::string dir)
{
    DIR *dp = opendir(dir.c_str());
    if (dp != NULL)
    {
        struct dirent *ep;
        bool success = true;
        while ((ep = readdir(dp)))
        {
            if (ep->d_type == DT_DIR)
            { // directory
                if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0)
                {
                    std::string subDirPath = dir + "/" + ep->d_name;
                    if (!deleteDirectory(subDirPath))
                    {
                        success = false;
                    }
                }
            }
            else if (ep->d_type == DT_REG)
            { // regular file
                std::string filePath = dir + "/" + ep->d_name;
                if (remove(filePath.c_str()) != 0)
                {
                    std::cerr << "Failed to delete file: " << filePath << std::endl;
                    success = false;
                }
            }
        }
        closedir(dp);
        if (rmdir(dir.c_str()) != 0)
        {
            std::cerr << "Failed to delete directory: " << dir << std::endl;
            success = false;
        }
        return success;
    }
    else
    {
        std::cerr << "Failed to open directory: " << dir << std::endl;
        return false;
    }
}

bool Utility::createDirIfNotExist(std::string dirPath)
{
    struct stat st;
    if (stat(dirPath.c_str(), &st) != 0)
    {
        if (mkdir(dirPath.c_str(), 0777) == 0)
        {
            return true;
        }
        else
        {
            std::cerr << "Failed to create directory: " << dirPath << std::endl;
            return false;
        }
    }
    else
    {
        if (S_ISDIR(st.st_mode))
        {
            COUT << "Directory already exists: " << dirPath << std::endl;
            return true;
        }
        else
        {
            std::cerr << dirPath << " is not a directory" << std::endl;
            return false;
        }
    }
}

bool Utility::copyFileTo(std::string filePath, std::string dstPath)
{
    std::ifstream srcFile(filePath, std::ios::binary);
    if (!srcFile)
    {
        std::cerr << "Failed to open source file: " << filePath << std::endl;
        return false;
    }

    std::string dstFilePath = dstPath + "/" + filePath.substr(filePath.find_last_of('/') + 1);
    std::ofstream dstFile(dstFilePath, std::ios::binary);
    if (!dstFile)
    {
        srcFile.close();
        std::cerr << "Failed to create destination file: " << dstFilePath << std::endl;
        return false;
    }
    dstFile << srcFile.rdbuf();

    srcFile.close();
    dstFile.close();

    return true;
}

bool Utility::fileExists(std::string fileName)
{
    std::ifstream file(fileName);
    return file.good();
}

bool Utility::isExecutable(std::string fileName)
{
    return access(fileName.c_str(), X_OK) == 0;
}

bool Utility::startApp(std::string executablePath, bool bBackground)
{
    if (!fileExists(executablePath))
    {
        COUT << "File " << executablePath << " does not exist" << std::endl;
        return false;
    }
    if (!isExecutable(executablePath))
    {
        COUT << "File " << executablePath << " is not executable" << std::endl;
        return false;
    }
    std::string command = executablePath; // 使用绝对路径启动可执行文件
    if (bBackground)
        command += " &";
    int result = system(command.c_str());
    if (result == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Utility::FeedWatchDog()
{
    std::cout << "-----------FeedWatchDog----------" << endl;
    system("echo 1 > /dev/watchdog");
}

void Utility::CloseWatchDog()
{
    std::cout << "-----------CloseWatchDog----------" << endl;
    system("echo V > /dev/watchdog");
}

bool Utility::copyFile(std::string &srcFile, std::string &destFile)
{
    // 打开源文件
    std::ifstream src(srcFile, std::ios::binary);
    if (!src)
    {
        std::cerr << "Error opening source file: " << srcFile << " - " << std::strerror(errno) << std::endl;
        return false;
    }

    // 打开目标文件
    std::ofstream dest(destFile, std::ios::binary);
    if (!dest)
    {
        std::cerr << "Error opening destination file: " << destFile << " - " << std::strerror(errno) << std::endl;
        return false;
    }

    // 复制文件内容
    dest << src.rdbuf();

    // 检查是否复制成功
    if (src.bad() || dest.bad())
    {
        std::cerr << "Error during file copy." << std::endl;
        return false;
    }

    return true;
}

bool Utility::removeFile(std::string &filePath)
{
    struct stat buffer;
    // 检查文件是否存在
    if (stat(filePath.c_str(), &buffer) == 0)
    {
        // 尝试删除文件
        if (remove(filePath.c_str()) == 0)
        {
            return true; // 删除成功
        }
        else
        {
            std::cerr << "Error deleting file: " << strerror(errno) << std::endl;
            return false; // 删除失败
        }
    }
    else
    {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return false; // 文件不存在
    }
}

bool Utility::readMyConfig(std::string filePath, std::string key, std::string &value)
{
    std::ifstream configFile(filePath);
    if (!configFile.is_open())
    {
        return false; // 文件不存在或打开失败
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        std::string k, v;
        if (std::getline(iss, k, '=') && std::getline(iss, v))
        {
            if (k == key)
            {
                value = v;   // 直接赋值为字符串
                return true; // 找到键并成功读取值
            }
        }
    }

    return false; // 没有找到该键
}

bool Utility::writeMyConfig(std::string filePath, std::string key, std::string value)
{
    std::unordered_map<std::string, std::string> configMap;
    std::ifstream configFile(filePath);

    // 如果文件存在，读取现有的键值对
    if (configFile.is_open())
    {
        std::string line;
        while (std::getline(configFile, line))
        {
            std::istringstream iss(line);
            std::string k, v;
            if (std::getline(iss, k, '=') && std::getline(iss, v))
            {
                configMap[k] = v; // 读取键值对
            }
        }
        configFile.close();
    }

    // 修改或添加新的键值对
    configMap[key] = value;

    // 写回文件
    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        return false; // 打开文件失败
    }

    for (const auto &pair : configMap)
    {
        outFile << pair.first << "=" << pair.second << "\n";
    }

    outFile.close();
    return true; // 成功写入
}
