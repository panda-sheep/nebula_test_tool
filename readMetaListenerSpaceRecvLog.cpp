#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief use to read meta listener space last_apply_log in nebula wal
 * Only output lastApplyLogId
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
       std::cout << "usage: xxx last_apply_log" << std::endl;
       return 1;
    }

    auto lastApplyLofFile = argv[1];

    int32_t fd = open(lastApplyLofFile, O_RDONLY);
    if (fd < 0) {
        std::cout << "open file " << lastApplyLofFile << " failed." << std::endl;
        return -1;
    }

    int offset = 0;

    int64_t lastApplyLogId;
    auto size = pread(fd, reinterpret_cast<char*>(&lastApplyLogId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read last apply logId failed " << std::endl;
        close(fd);
        return -1;
    }

    close(fd);

    std::cout << "last apply log id " << lastApplyLogId << std::endl;
    return 0;
}
