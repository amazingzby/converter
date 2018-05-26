#include "FileManager.h"
FileManager* FileManager::p=nullptr;
FileManager* FileManager::getInstance()
{
    if(p==nullptr)
        p=new FileManager();
    return p;
}

void FileManager::init(const char* blobFile)
{
   fp=fopen(blobFile,"wb+");
   printf("Open the file %s\n",blobFile);
}

void FileManager::write(const void* buffer, size_t size, size_t count)
{
    fwrite(buffer,size,count,fp);
}
void FileManager::close()
{
    fclose(fp);
    printf("The binary file has been closed!\n");
}
