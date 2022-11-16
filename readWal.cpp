#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief use to read first log in nebula wal and and returns the total number of records.
 * The format of the output is as follows:
 * first log Id 1
 * first log term 0
 * first log msglen 45
 * first log cluster 0
 * total wal log nums 6
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
       std::cout << "usage: xxx walfile" << std::endl;
       return 1;
    }

    auto walFile = argv[1];

    struct stat st;
    if (lstat(walFile, &st)) {
        std::cout << "open file " << walFile << " failed." << std::endl;
        return -1;
    }

    auto fileSize = st.st_size;

    int32_t fd = open(walFile, O_RDONLY);
    if (fd < 0) {
        std::cout << "open file " << walFile << " failed." << std::endl;
        return -1;
    }

    int offset = 0;

    bool fistlog = true;
    int count = 0;

    int noheartbeatCount = 0;

    while (offset < fileSize) {
        int64_t firstLogID;
        auto size = pread(fd, reinterpret_cast<char*>(&firstLogID), sizeof(int64_t), offset);
        if (size != sizeof(int64_t)) {
            std::cout << "read first logId failed." << std::endl;
            close(fd);
            return -1;
        }
        offset += sizeof(int64_t);

        int64_t firstLogTerm;
        size = pread(fd, reinterpret_cast<char*>(&firstLogTerm), sizeof(int64_t), offset);
        if (size != sizeof(int64_t)) {
            std::cout << "read first logTerm failed." << std::endl;
            close(fd);
            return -1;
        }
        offset += sizeof(int64_t);

        int32_t head;
        size = pread(fd, reinterpret_cast<char*>(&head), sizeof(int32_t), offset);
        if (size != sizeof(int32_t)) {
            std::cout << "read first log head failed." << std::endl;
            close(fd);
            return -1;
        }
        offset += sizeof(int32_t);

        int64_t  ClusterID;
        size = pread(fd, reinterpret_cast<char*>(&ClusterID), sizeof(int64_t), offset);
        if (size != sizeof(int64_t)) {
            std::cout << "read first log clusterId failed." << std::endl;
            close(fd);
            return -1;
        }
        offset += sizeof(int64_t);

        // logMsg
        offset += head;
        if (head != 0) {
            noheartbeatCount++;
        }

        int32_t foot;
        size = pread(fd, reinterpret_cast<char*>(&foot), sizeof(int32_t), offset);
        if (size != sizeof(int32_t)) {
            std::cout << "read first log foot failed." << std::endl;
            close(fd);
            return -1;
        }
        offset += sizeof(int32_t);

        if (fistlog) {
            std::cout << "first log Id " << firstLogID << std::endl;
            std::cout << "first log term " << firstLogTerm << std::endl;
            std::cout << "first log msglen " << head << std::endl;
            std::cout << "first log cluster " << ClusterID << std::endl;
            fistlog = false;
        }
        count++;
    }
    close(fd);

    std::cout << "total wal log nums " << count << std::endl;
    std::cout << "total wal log nums(noheartbeat) " << noheartbeatCount << std::endl;

    return 0;
}
