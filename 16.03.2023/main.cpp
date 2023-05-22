#include "manager.hpp"
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <command> <path> [arguments]\n";
    return 1;
  }

  const char *command = argv[1];
  const char *path = argv[2];

  if (strcmp(command, "init") == 0) {
    try {
      Manager::init(path);
      std::cout << "Иницализация рессурсов прошла ВЕЛИКОЛЕПНО\n";
    } catch (const std::exception &e) {
      std::cerr << "Fail " << e.what() << std::endl;
    }
  } else if (strcmp(command, "destroy") == 0) {
    try {
      Manager::destroy(path);
      std::cout << "Рессурсы уничтожены\n";
    } catch (const std::exception &e) {
      std::cerr << "Fail " << e.what() << std::endl;
    }
  } else if (strcmp(command, "rw") == 0) {
    if (argc < 5) {
      std::cerr << "Usage: " << argv[0] << " rw <N> <M> <path>\n";
      return 1;
    }

    int N = std::stoi(argv[3]);
    int M = std::stoi(argv[4]);

    for (int i = 0; i < N; i++) {
      pid_t pid = fork();
      if (pid == 0) {
        Manager manager(path);
        manager.reader();
        return 0;
      } else if (pid < 0) {
        std::cerr << "Error.\n";
        return 1;
      }
    }

    for (int i = 0; i < M; i++) {
      pid_t pid = fork();
      if (pid == 0) {
        Manager manager(path);
        manager.writer(i);
        return 0;
      } else if (pid < 0) {
        std::cerr << "Ошибка при форке.\n";
        return 1;
      }
    }

    for (int i = 0; i < N + M; i++) {
      wait(nullptr);
    }
  } else {
    std::cerr << "Неизвестная команда " << command << std::endl;
    return 1;
  }

  return 0;
}
