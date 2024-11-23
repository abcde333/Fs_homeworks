#include <iostream>
#include <fstream>
#include <cstring> 

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Ошибка: требуется два аргумента (путь к исходному и целевому файлу).\n";
        return 1;
    }

    const char* sourceFilePath = argv[1];
    const char* destFilePath = argv[2];

    std::ifstream sourceFile(sourceFilePath, std::ios::binary);
    if (!sourceFile.is_open()) {
        std::cerr << "Ошибка при открытии исходного файла '" << sourceFilePath << "': " << strerror(errno) << std::endl;
        return 1;
    }

    std::ofstream destFile(destFilePath, std::ios::binary);
    if (!destFile.is_open()) {
        std::cerr << "Ошибка при открытии целевого файла '" << destFilePath << "': " << strerror(errno) << std::endl;
        return 1;
    }

    destFile << sourceFile.rdbuf();

    std::cout << "Файл успешно скопирован с '" << sourceFilePath << "' на '" << destFilePath << "'." << std::endl;

    sourceFile.close();
    destFile.close();

    return 0;
}



