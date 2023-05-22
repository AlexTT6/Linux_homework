#include "manager.hpp"
#include <iostream>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define MUTEX 0
#define DB 1

void Manager::init(const char *file_path) {
  int token = ftok(file_path, 7);
  if (token == -1) {
    throw std::invalid_argument{"ftok"};
  }
  int shm_id = shmget(token, 33 * sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
  if (shm_id == -1) {
    throw std::logic_error{"shmget"};
  }

  int *rc_address = static_cast<int *>(shmat(shm_id, nullptr, 0));
  if (rc_address == reinterpret_cast<int *>(-1)) {
    throw std::logic_error{"shmat"};
  }
  *rc_address = 0;

  int sem_id = semget(token, 2, IPC_CREAT | IPC_EXCL | 0600);
  if (sem_id == -1) {
    throw std::logic_error{"semget"};
  }

  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
  };

  union semun arg;
  arg.val = 1;

  if (semctl(sem_id, MUTEX, SETVAL, arg) == -1) {
    throw std::logic_error{"semctl"};
  }

  arg.val = 0;

  if (semctl(sem_id, DB, SETVAL, arg) == -1) {
    throw std::logic_error{"semctl"};
  }
}

void Manager::destroy(const char *file_path) {
  int token = ftok(file_path, 7);
  if (token == -1) {
    throw std::invalid_argument{"ftok"};
  }

  int shm_id = shmget(token, 33 * sizeof(int), 0600);
  if (shm_id == -1) {
    throw std::logic_error{"shmget"};
  }

  if (shmctl(shm_id, IPC_RMID, nullptr) == -1) {
    throw std::logic_error{"shmctl"};
  }

  int sem_id = semget(token, 2, 0600);
  if (sem_id == -1) {
    throw std::logic_error{"semget"};
  }

  if (semctl(sem_id, 0, IPC_RMID) == -1) {
    throw std::logic_error{"semctl"};
  }
}

Manager::Manager(const char *file_path) {
  int token = ftok(file_path, 7);
  if (token == -1) {
    throw std::invalid_argument{"ftok"};
  }

  _shm_id = shmget(token, 33 * sizeof(int), 0600);
  if (_shm_id == -1) {
    throw std::logic_error{"shmget"};
  }

  _rc_address = static_cast<int *>(shmat(_shm_id, nullptr, 0));
  if (_rc_address == reinterpret_cast<int *>(-1)) {
    throw std::logic_error{"shmat"};
  }
  _a_address = _rc_address + 1;

  _sem_id = semget(token, 2, 0600);
  if (_sem_id == -1) {
    throw std::logic_error{"semget"};
  }
}

Manager::~Manager() { shmdt(_rc_address); }

void Manager::reader() {
  int b[32] = {};
  struct sembuf mutex_down = {MUTEX, -1, 0};
  struct sembuf mutex_up = {MUTEX, 1, 0};
  struct sembuf db_down = {DB, -1, 0};
  struct sembuf db_up = {DB, 1, 0};

  semop(_sem_id, &mutex_down, 1);

  *_rc_address = *_rc_address + 1;

  if (*_rc_address == 1) {
    semop(_sem_id, &db_down, 1);
  }

  semop(_sem_id, &mutex_up, 1);

  for (int i = 0; i < 32; i++) {
    b[i] = _a_address[i];
  }

  semop(_sem_id, &mutex_down, 1);
  *_rc_address = *_rc_address - 1;
  if (*_rc_address == 0) {
    semop(_sem_id, &db_up, 1);
  }
  semop(_sem_id, &mutex_up, 1);

  for (int i = 0; i < 32; ++i) {
    std::cout << b[i] << " ";
  }
  std::cout << std::endl;
}

void Manager::writer(int value) {
  struct sembuf db_down = {DB, -1, 0};
  struct sembuf db_up = {DB, 1, 0};

  semop(_sem_id, &db_down, 1);

  for (int i = 0; i < 32; ++i) {
    _a_address[i] = value;
  }

  semop(_sem_id, &db_up, 1);
}
