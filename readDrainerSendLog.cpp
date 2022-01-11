#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief use to read send log file in drainer nebula wal
 * Only output lastSendLogId
 *
 * @param argc
 * @param argv
 * @return int
 */
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

    int64_t lastSendLogId;
    auto size = pread(fd, reinterpret_cast<char*>(&lastSendLogId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last send logId failed from part " << std::endl;
        close(fd);
        return -1;
    }

    close(fd);

    std::cout << "last send log id " << lastSendLogId << std::endl;

    return 0;
}
