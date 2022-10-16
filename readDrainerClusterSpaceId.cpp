#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief use to read drainer space cluster space id file
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
       std::cout << "usage: xxx cluster_space_id" << std::endl;
       return 1;
    }

    auto clusterSpaceIdFile = argv[1];

    int32_t fd = open(clusterSpaceIdFile, O_RDONLY);
    if (fd < 0) {
        std::cout << "open file " << clusterSpaceIdFile << " failed." << std::endl;
        return -1;
    }

    int offset = 0;

    int64_t masterClusterId;
    int32_t fromSpaceId;
    int32_t fromPartNum;
    int64_t slaveClusterId;


    auto size = pread(fd, reinterpret_cast<char*>(&masterClusterId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read space cluster space id failed " << std::endl;
        close(fd);
        return -1;
    }

    size = pread(fd, reinterpret_cast<char*>(&fromSpaceId), sizeof(int32_t), offset);
    if (size != sizeof(int32_t)) {
        std::cout << "read space cluster space id failed " << std::endl;
        close(fd);
        return -1;
    }

    size = pread(fd, reinterpret_cast<char*>(&fromPartNum), sizeof(int32_t), offset);
    if (size != sizeof(int32_t)) {
        std::cout << "read space cluster space id failed " << std::endl;
        close(fd);
        return -1;
    }

    size = pread(fd, reinterpret_cast<char*>(&slaveClusterId), sizeof(int64_t), offset);
    if (size != sizeof(int64_t)) {
        std::cout << "read space cluster space id failed " << std::endl;
        close(fd);
        return -1;
    }

    close(fd);

    std::cout << "masterClusterId " << masterClusterId << std::endl;
    std::cout << "fromSpaceId    " << fromSpaceId << std::endl;
    std::cout << "fromPartNum    " << fromPartNum << std::endl;
    std::cout << "slaveClusterId " << slaveClusterId << std::endl;
    return 0;
}
