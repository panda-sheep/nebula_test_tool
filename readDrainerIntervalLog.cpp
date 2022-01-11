#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief use to read recv log file in drainer nebula wal
 * Only output lastRecvLogId
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
       std::cout << "usage: xxx inteervalLogFile" << std::endl;
       return 1;
    }

    auto intervalLogFile = argv[1];

    int32_t fd = open(intervalLogFile, O_RDONLY);
    if (fd < 0) {
        std::cout << "open file " << intervalLogFile << " failed." << std::endl;
        return -1;
    }

    int offset = 0;

    int64_t inteervalLogId;
    auto size = pread(fd, reinterpret_cast<char*>(&inteervalLogId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read inter logId failed from part " << std::endl;
        close(fd);
        return -1;
    }

    close(fd);

    std::cout << "last interval log id " << inteervalLogId  << std::endl;

    return 0;
}
