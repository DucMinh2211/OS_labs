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

#define BUFFER_SIZE \
  256  // preprocessor declare, maximum characters to read for a line

typedef struct {
  double sum;
  int count;
} MovieRating;

void get_average_ratings(const char* file_name, int shm_id) {
  FILE* fp;
  char line[BUFFER_SIZE];
  int user_id, movie_id, rating, timestamp;
  MovieRating* movie_ratings;

  movie_ratings = (MovieRating*)shmat(shm_id, NULL, 0);
  if (movie_ratings == (void*)-1) {
    perror("shmat failed");
    exit(EXIT_FAILURE);
  }

  fp = fopen(file_name, "r");
  if (fp == NULL) {
    perror("read file failed");
    shmdt(movie_ratings);
    exit(EXIT_FAILURE);
  }

  while (fgets(line, BUFFER_SIZE, fp) != NULL) {
    if (sscanf(line, "%d\t%d\t%d\t%d", &user_id, &movie_id, &rating,
               &timestamp) == 4) {
      movie_ratings->sum += rating;
      movie_ratings->count++;
    }

    shmdt(movie_ratings);
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char* argv[]) { return 0; }
