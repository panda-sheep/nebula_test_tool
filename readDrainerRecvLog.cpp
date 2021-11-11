#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


// use to read recv log file in nebula wal
int main(int argc, char *argv[]) {
    if (argc != 2) {
       std::cout << "usage: xxx recvLogFile" << std::endl;
       return 1;
    }

    auto recvLogFile = argv[1];

    int32_t fd = open(recvLogFile, O_RDONLY);
    if (fd < 0) {
        std::cout << "open file " << recvLogFile << " failed." << std::endl;
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
    int64_t lastRecvLogId;
    size = pread(fd, reinterpret_cast<char*>(&lastRecvLogId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last recv logId failed from part " << fromPartID  << std::endl;
        close(fd);
        return -1;
    }


    offset += sizeof(int64_t);
    int64_t lastRecvLogTerm;
    size = pread(fd, reinterpret_cast<char*>(&lastRecvLogTerm), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last recv log term failed from part "<< fromPartID  << std::endl;
        close(fd);
        return -1;
    }
    offset += sizeof(int64_t);

    close(fd);

    std::cout << "from part Id " << fromPartID << std::endl;
    std::cout << "last recv log id " << lastRecvLogId << std::endl;
    std::cout << "last recv log term " << lastRecvLogTerm << std::endl;

    return 0;
}
