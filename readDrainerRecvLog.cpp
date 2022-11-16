#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief use to read recv log file in drainer nebula wal
 * output lastRecvLogId
 *        lastRecvLog
 *
 * @param argc
 * @param argv
 * @return int
 */
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

    int64_t lastRecvLogId;
    auto size = pread(fd, reinterpret_cast<char*>(&lastRecvLogId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last recv logId failed from part " << std::endl;
        close(fd);
        return -1;
    }
    offset += sizeof(int64_t);

    int64_t lastRecvLogTime;
    size = pread(fd, reinterpret_cast<char*>(&lastRecvLogTime), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last recv logTime failed from part " << std::endl;
        close(fd);
        return -1;
    }

    close(fd);

    std::cout << "last recv log id " << lastRecvLogId << std::endl;
    std::cout << "last recv log time " << lastRecvLogTime << std::endl;

    return 0;
}
