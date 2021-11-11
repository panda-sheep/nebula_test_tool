#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


// use to read send log file in nebula wal
int main(int argc, char *argv[]) {
    if (argc != 2) {
       std::cout << "usage: xxx sendLogFile" << std::endl;
       return 1;
    }

    auto sendLogFile = argv[1];

    int32_t fd = open(sendLogFile, O_RDONLY);
    if (fd < 0) {
        std::cout << "open file " << sendLogFile << " failed." << std::endl;
        return -1;
    }

    int offset = 0;

    int32_t fromPartID;
    auto size = pread(fd, reinterpret_cast<char*>(&fromPartID), sizeof(int32_t), offset);
    if (size != sizeof(int32_t)) {
        std::cout << "read from part id failed." << std::endl;
        close(fd);
        return -1;
    }

    offset += sizeof(int32_t);
    int64_t lastSendLogId;
    size = pread(fd, reinterpret_cast<char*>(&lastSendLogId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last send logId failed from part " << fromPartID  << std::endl;
        close(fd);
        return -1;
    }


    offset += sizeof(int64_t);
    int64_t lastSendLogTerm;
    size = pread(fd, reinterpret_cast<char*>(&lastSendLogTerm), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last send log term failed from part "<< fromPartID  << std::endl;
        close(fd);
        return -1;
    }
    offset += sizeof(int64_t);

    close(fd);

    std::cout << "from part Id " << fromPartID << std::endl;
    std::cout << "last send log id " << lastSendLogId << std::endl;
    std::cout << "last send log term " << lastSendLogTerm << std::endl;

    return 0;
}
