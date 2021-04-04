#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int n;

  const char *usage = "Usage: sleep n\n";
  const char *error = "Error: time must be an integer\n";

  if (argc != 2) {
	  fprintf(2, usage);
	  exit(1);
  }

  for (uint i = 0; i < strlen(argv[1]); i++) {
	if (argv[1][i] < '0' || argv[1][i] > '9') {
		fprintf(2, error);
		exit(1);
	}
  }

  n = atoi(argv[1]);
  sleep(n);
  exit(0);
}
