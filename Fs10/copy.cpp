#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

void printUsage() {
    cerr << "Usage: ./copy <source-file> <destination-file>" << endl;
}

int main(int argc, char* argv[]) {
    // Проверка наличия двух аргументов (путь к исходному и целевому файлу)
    if (argc != 3) {
        printUsage();
        return 1;
    }

    const char* sourceFile = argv[1];
    const char* destFile = argv[2];

    // Открытие исходного файла для чтения
    int srcFd = open(sourceFile, O_RDONLY);
    if (srcFd == -1) {
        cerr << "Error opening source file '" << sourceFile << "': " << strerror(errno) << endl;
        return 1;
    }

    // Открытие целевого файла для записи (создание нового или перезапись)
    int destFd = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) {
        cerr << "Error opening destination file '" << destFile << "': " << strerror(errno) << endl;
        close(srcFd);
        return 1;
    }

    off_t totalCopied = 0;
    off_t totalData = 0;
    off_t totalHoles = 0;
    const size_t bufferSize = 4096;
    char buffer[bufferSize];

    struct stat st;

    // Получаем информацию о размере исходного файла
    if (fstat(srcFd, &st) == -1) {
        cerr << "Error getting source file info: " << strerror(errno) << endl;
        close(srcFd);
        close(destFd);
        return 1;
    }

    off_t fileSize = st.st_size;
    off_t currentOffset = 0;

    // Обработка данных и дыр
    while (currentOffset < fileSize) {
        // Ищем следующий участок с данными
        off_t dataOffset = lseek(srcFd, currentOffset, SEEK_DATA);
        if (dataOffset == -1) {
            if (errno == ENXIO) {
                // Нет больше данных, заканчиваем обработку
                totalHoles += fileSize - currentOffset;
                break;
            } else {
                cerr << "Error seeking data: " << strerror(errno) << endl;
                close(srcFd);
                close(destFd);
                return 1;
            }
        }

        // Учёт дыр между текущей позицией и началом данных
        if (dataOffset > currentOffset) {
            totalHoles += dataOffset - currentOffset;
        }

        // Ищем следующий участок с дырой
        off_t holeOffset = lseek(srcFd, dataOffset, SEEK_HOLE);
        if (holeOffset == -1) {
            cerr << "Error seeking hole: " << strerror(errno) << endl;
            close(srcFd);
            close(destFd);
            return 1;
        }

        // Копируем данные
        off_t dataSize = holeOffset - dataOffset;
        off_t bytesCopied = 0;

        while (bytesCopied < dataSize) {
            size_t bytesToRead = min(bufferSize, static_cast<size_t>(dataSize - bytesCopied));
            ssize_t bytesRead = pread(srcFd, buffer, bytesToRead, dataOffset + bytesCopied);
            if (bytesRead == -1) {
                cerr << "Error reading from source file: " << strerror(errno) << endl;
                close(srcFd);
                close(destFd);
                return 1;
            }

            ssize_t bytesWritten = write(destFd, buffer, bytesRead);
            if (bytesWritten == -1) {
                cerr << "Error writing to destination file: " << strerror(errno) << endl;
                close(srcFd);
                close(destFd);
                return 1;
            }

            bytesCopied += bytesRead;
        }

        totalCopied += bytesCopied;
        totalData += bytesCopied;

        // Обновляем текущую позицию
        currentOffset = holeOffset;
    }

    // Выводим результат
    cout << "Successfully copied " << totalCopied << " bytes (data: " << totalData
         << ", holes: " << totalHoles << ")." << endl;

    // Закрытие файлов
    close(srcFd);
    close(destFd);

    return 0;
}
