#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

void manualAppend(const char* filePath) {
    int fd1 = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    int fd2 = open(filePath, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd2 == -1) {
        std::cerr << "Error opening file for second descriptor: " << strerror(errno) << std::endl;
        close(fd1);
        exit(EXIT_FAILURE);
    }

    if (dup2(fd1, STDOUT_FILENO) == -1) {
        std::cerr << "Error redirecting stdout to fd1: " << strerror(errno) << std::endl;
        close(fd1);
        close(fd2);
        exit(EXIT_FAILURE);
    }

    std::cout << "first line" << std::endl;

    if (dup2(fd2, STDOUT_FILENO) == -1) {
        std::cerr << "Error redirecting stdout to fd2: " << strerror(errno) << std::endl;
        close(fd1);
        close(fd2);
        exit(EXIT_FAILURE);
    }

    std::cout << "second line" << std::endl;

    close(fd1);
    close(fd2);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./manual-append <file-path>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* filePath = argv[1];
    manualAppend(filePath);

}


