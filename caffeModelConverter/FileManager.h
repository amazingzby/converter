#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include<string>
#include<fstream>
using std::string;

class FileManager
{
private:
    static FileManager* p;
    //FileManager *fileManager=FileManager::getInstance();
    FILE* fp=NULL;
    FileManager(){}
    FileManager(const FileManager&);
    FileManager& operator = (const FileManager&);
public:
    static FileManager* getInstance();
    void init(const char* blobFile);
    void write(const void* buffer, size_t size, size_t count);
    void close();
};

class ExceptionManage
{
public:
    ExceptionManage(const bool condition_,const char* message_):condition(condition_),message(message_)
    {

    }

private:
    const bool condition;
    const char* message;
};

#endif // FILEMANAGE_H
