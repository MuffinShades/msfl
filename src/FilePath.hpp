#include <iostream>

/**
 * 
 * General utility functions around file paths
 * 
 */

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

#include "Date.hpp"
#include "msutil.hpp"

struct FileInfo {
    Date creationDate, modifiedDate;
    std::string name, type;
};

class FilePath_int {
    static std::string getFileType(std::string path);
    static std::string getFileName(std::string path);
    static FileInfo getFileInfo(std::string path);
};