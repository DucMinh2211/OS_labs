/*
        Firstly, downloading two text files from the url:
   https://drive.google.com/file/d/1fgJqOeWbJC4gh MKHkuxfIP6dh2F911-E These
   file contains the 100000 ratings of 943 users for 1682 movies in the
   following format: userID <tab> movieID <tab> rating <tab> timeStamp userID
   <tab> movieID <tab> rating <tab> timeStamp
        ...
        Secondly, you should write a program that spawns two child processes,
   and each of them will read a file and compute the average ratings of movies
   in the file. You implement the program by using shared memory method.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_MOVIES 1682
#define BUFFER_SIZE \
  256  // preprocessor declare, maximum characters to read for a line
#define file_1 "movie-100k_1.txt"
#define file_2 "movie-100k_2.txt"

typedef struct {
  double sum;
  int count;
} MovieRating;

void calculate_average_ratings(const char* file_name, int shm_id) {
  FILE* fp;
  char line[BUFFER_SIZE];
  int user_id, movie_id, rating, timestamp;
  MovieRating* movie_ratings;

  movie_ratings = (MovieRating*)shmat(shm_id, NULL, 0);
  if (movie_ratings == (void*)-1) {
    perror("shmat failed in child process!");
    exit(EXIT_FAILURE);
  }

  fp = fopen(file_name, "r");
  if (fp == NULL) {
    perror("fopen faild in child process!");
    shmdt(movie_ratings);
    exit(EXIT_FAILURE);
  }

  while (fgets(line, BUFFER_SIZE, fp) != NULL) {
    if (sscanf(line, "%d\t%d\t%d\t%d", &user_id, &movie_id, &rating,
               &timestamp) == 4) {
      movie_ratings[movie_id].sum += rating;
      movie_ratings[movie_id].count++;
    } else {
      perror("fgets failed in child process!");
      shmdt(movie_ratings);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fp);

  if (shmdt(movie_ratings) == -1) {
    perror("shmdt failed in child process!");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

void print_average_ratings(MovieRating* movie_ratings) {
  printf("Average Movie Ratings:\n");
  for (int i = 1; i < NUM_MOVIES; i++) {
    if (movie_ratings[i].count > 0) {
      double average = movie_ratings[i].sum / movie_ratings[i].count;
      printf("Movie ID %d: Average Rating = %.2f (%d ratings)\n", i, average,
             movie_ratings[i].count);
    }
  }
}

MovieRating* init_shm_movie_ratings(int shm_id) {
  MovieRating* movie_ratings = (MovieRating*)shmat(shm_id, NULL, 0);

  if (movie_ratings == (void*)-1) {
    perror("shmat failed at init movie ratings");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < NUM_MOVIES; i++) {
    movie_ratings[i].sum = 0;
    movie_ratings[i].count = 0;
  }
  return movie_ratings;
}

int main(int argc, char* argv[]) {
  int shm_id;
  key_t key = IPC_PRIVATE;  // private key for share mem
  size_t shm_size = NUM_MOVIES * sizeof(MovieRating);

  if ((shm_id = shmget(key, shm_size, IPC_CREAT | 0666)) < 0) {
    perror("shmget failed!");
    exit(EXIT_FAILURE);
  }

  MovieRating* movie_ratings = init_shm_movie_ratings(shm_id);

  pid_t pid_1, pid_2;

  if ((pid_1 = fork()) == -1) {
    perror("fork failed!");
    shmdt(movie_ratings);
    shmctl(shm_id, IPC_RMID, NULL);  // Clean up shared memory on error
    exit(EXIT_FAILURE);
  } else if (pid_1 == 0) {
    calculate_average_ratings(file_1, shm_id);
  }

  if ((pid_2 = fork()) == -1) {
    perror("fork failed");
    shmdt(movie_ratings);
    shmctl(shm_id, IPC_RMID, NULL);  // Clean up shared memory on error
    kill(pid_1);  // Kill the first child if it's still running
    exit(EXIT_FAILURE);
  } else if (pid_2 == 0) {
    calculate_average_ratings(file_2, shm_id);
  }

  // Wait for both child processes to complete
  waitpid(pid_1, NULL, 0);
  waitpid(pid_2, NULL, 0);

  print_average_ratings(movie_ratings);

  if (shmdt(movie_ratings) == -1) {
    perror("shmdt failed in parent process");
    exit(EXIT_FAILURE);
  }

  if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
    perror("shmctl failed");
    exit(EXIT_FAILURE);
  }

  return 0;
}
