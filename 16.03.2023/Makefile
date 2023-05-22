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
      std::cout << "Resources initialized successfully.\n";
    } catch (const std::exception &e) {
      std::cerr << "Failed to initialize resources: " << e.what() << std::endl;
    }
  } else if (strcmp(command, "destroy") == 0) {
    try {
      Manager::destroy(path);
      std::cout << "Resources destroyed successfully.\n";
    } catch (const std::exception &e) {
      std::cerr << "Failed to destroy resources: " << e.what() << std::endl;
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
        std::cerr << "Failed to fork process.\n";
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
        std::cerr << "Failed to fork process.\n";
        return 1;
      }
    }

    for (int i = 0; i < N + M; i++) {
      wait(nullptr);
    }
  } else {
    std::cerr << "Unknown command: " << command << std::endl;
    return 1;
  }

  return 0;
}
