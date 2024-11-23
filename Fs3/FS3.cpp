#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

void handleError(const std::string &message) {
    std::cerr << message << std::endl;
    exit(EXIT_FAILURE);
}

void overwriteFile(const std::string &filePath) {
    int fd = open(filePath.c_str(), O_RDWR);
    if (fd == -1) {
        handleError("Ошибка при открытии файла: " + std::string(strerror(errno)));
    }

    struct stat fileStats;
    if (fstat(fd, &fileStats) == -1) {
        close(fd);
        handleError("Ошибка при получении информации о файле: " + std::string(strerror(errno)));
    }

    off_t fileSize = fileStats.st_size;
    
    char nullByte = '\0';
    for (off_t i = 0; i < fileSize; ++i) {
        if (write(fd, &nullByte, 1) != 1) {
            close(fd);
            handleError("Ошибка при записи нулевого байта в файл: " + std::string(strerror(errno)));
        }
    }

    close(fd);
}

void deleteFile(const std::string &filePath) {
    if (unlink(filePath.c_str()) == -1) {
        handleError("Ошибка при удалении файла: " + std::string(strerror(errno)));
    }
    std::cout << "Файл " << filePath << " успешно удалён." << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        handleError("Использование: hacker-rm <путь к файлу>");
    }

    std::string filePath = argv[1];

    struct stat fileStats;
    if (stat(filePath.c_str(), &fileStats) == -1) {
        handleError("Файл не существует или не может быть доступен: " + std::string(strerror(errno)));
    }

    overwriteFile(filePath);
    deleteFile(filePath);

    return 0;
}




