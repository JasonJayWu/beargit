#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include "beargit.h"
#include "util.h"


/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - Here are some of the helper functions from util.h:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the project spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE* findex = fopen(".beargit/.index", "w");
  fclose(findex);

  FILE* fbranches = fopen(".beargit/.branches", "w");
  fprintf(fbranches, "%s\n", "master");
  fclose(fbranches);

  write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");
  write_string_to_file(".beargit/.current_branch", "master");

  return 0;
}



/* beargit add <filename>
 *
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR:  File <filename> has already been added.
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char* filename) {
  FILE* findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR:  File %s has already been added.\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}

/* beargit status
 *
 * See "Step 1" in the project spec.
 *
 */

int beargit_status() {
  /* COMPLETE THE REST */
  FILE *findex = fopen(".beargit/.index", "r");
  fprintf(stdout,"%s","Tracked files:");
  fprintf(stdout,"\n");
  fprintf(stdout,"\n");
  char line[FILENAME_SIZE];
  int counter;
  counter = 0;
  while(fgets(line, sizeof(line), findex)) {
        fprintf(stdout, "%s", line);
        counter += 1;
      }

      fprintf(stdout,"\nThere are %d files total.\n", counter);
      fclose(findex);
  /*
  for each of the file names in the directory, print out each name
  */
  return 0;
}

/* beargit rm <filename>
 *
 * See "Step 2" in the project spec.
 *
 */

int beargit_rm(const char* filename) {
  /*
  strcomp to try to find the filename, then remove it (using a delete file kind of func?)
  */
  /* COMPLETE THE REST */
  FILE* findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  int counter = 0;
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      counter += 1;
      continue;
      }
    else {
      fprintf(fnewindex, "%s\n", line);
        }
    }
    if (counter == 0) {
   fprintf(stderr, "ERROR:  File %s not tracked.\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
  return 0;
    }
    else {
    fclose(findex);
    fclose(fnewindex);
    fs_mv(".beargit/.newindex", ".beargit/.index");
   }

  return 0;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the project spec.
 *
 */

const char* go_bears = "THIS IS BEAR TERRITORY!";

int is_commit_msg_ok(const char* msg) {
  /* COMPLETE THE REST */
  int m = 0;
  int g = 0;
  while (msg[m] != '\0') {
    if (go_bears[g] == msg[m]) {
      m += 1;
      g += 1;
    }
    else if (go_bears[g] != msg[m] && go_bears[0] == msg[m]) {
      g = 0;
    }
    else {
      g = 0;
      m += 1;
    }
    if (go_bears[g] == '\0') {
      return 1;
    }
  }
    return 0;
}

/* Use next_commit_id to fill in the rest of the commit ID.
 *
 * Hints:
 * You will need a destination string buffer to hold your next_commit_id, before you copy it back to commit_id
 * You will need to use a function we have provided for you.
 */

void next_commit_id(char* commit_id) {
     /* COMPLETE THE REST */
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);
  //read_string_from_file(".beargit/.prev", prev_commit_id, COMMIT_ID_SIZE);
  char new_id[BRANCHNAME_SIZE + COMMIT_ID_SIZE]; //May need to add 1 
  strcpy(new_id, commit_id);
  strcat(new_id, current_branch);
  cryptohash(new_id, commit_id);
}

int beargit_commit(const char* msg) {
  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR:  Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
  next_commit_id(commit_id);
  /* COMPLETE THE REST */
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  //return error if not at head of branch
  if (current_branch[0] == '\0') {
    fprintf(stderr, "ERROR: Need to be on HEAD of a branch to commit.\n");
    return 1;
  }

  //Construct .beargit/commit_id
  char new_direct[9 + COMMIT_ID_SIZE + 1] = ".beargit/";
  strcat(new_direct, commit_id);
  //Construct .beargit/commit_id/.index
  char new_direct_index[9 + COMMIT_ID_SIZE + 6 + 1] = ".beargit/";
  strcat(new_direct_index, commit_id);
  strcat(new_direct_index, "/.index");
  //Construct .beargit/.commit_id/.prev
  char new_direct_prev[9 + COMMIT_ID_SIZE + 5 + 1] = ".beargit/";
  strcat(new_direct_prev, commit_id);
  strcat(new_direct_prev, "/.prev");
  //Generate new directory .beargit/commit_id
  fs_mkdir(new_direct);
  fs_cp(".beargit/.index", new_direct_index);
  fs_cp(".beargit/.prev", new_direct_prev);
  /*
  char prev_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", prev_commit_id, COMMIT_ID_SIZE);
  char tracked_files_loc[9 + COMMIT_ID_SIZE + 1] = ".beargit/";
  strcat(tracked_files_loc, prev_commit_id);*/
  //fs_cp(tracked_files_loc, new_direct);
  
  //Add tracked files into new directory .beargit/commit_id
  FILE *findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    char new_direct_loc[9 + COMMIT_ID_SIZE + 1 + sizeof(line) + 1] = ".beargit/";
    strcat(new_direct_loc, commit_id);
    strcat(new_direct_loc, "/");
    strcat(new_direct_loc, line);
    fs_cp(line, new_direct_loc);
    }
  fclose(findex);

  //Store commit message into new_id
  //Construct .beragit/commit_id/.msg
  char msg_loc[14 + COMMIT_ID_SIZE + 1] = ".beargit/";
  strcat(msg_loc, commit_id);
  strcat(msg_loc, "/.msg");
  write_string_to_file(msg_loc, msg);

  //Write new ID into .prev
  write_string_to_file(".beargit/.prev", commit_id);
  return 0;
}

/* beargit log
 *
 * See "Step 4" in the project spec.
 *
 */

int beargit_log(int limit) {
  /* COMPLETE THE REST */
  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);

  //Check if no commmits to beargit repo
  if( strcmp("0000000000000000000000000000000000000000", commit_id) == 0) {
    fprintf(stderr, "%s\n", "ERROR:  There are no commits.");
    return 1;
  }

  //next_commit_id(commit_id); //Is this right?
  int counter = 0;
  while (counter < limit) {
    //Print commit ID
    fprintf(stdout, "commit %s\n", commit_id);

    //Print message
    char msg_loc[14 + COMMIT_ID_SIZE + 1] = ".beargit/";
    strcat(msg_loc, commit_id);
    strcat(msg_loc, "/.msg");
    char msg[MSG_SIZE];
    read_string_from_file(msg_loc, msg, MSG_SIZE);
    strtok(msg, "\n");
    fprintf(stdout, "    %s\n\n", msg);
    counter += 1;

    //Get previous commit ID
    char prev_commit_direct[15 + COMMIT_ID_SIZE + 1] = ".beargit/";
    strcat(prev_commit_direct, commit_id);
    strcat(prev_commit_direct, "/.prev");
    read_string_from_file(prev_commit_direct, commit_id, COMMIT_ID_SIZE);

    //Check if at original ID 
    if( strcmp("0000000000000000000000000000000000000000", commit_id) == 0) {
      break;
    }
  }
  return 0;
}


// This helper function returns the branch number for a specific branch, or
// returns -1 if the branch does not exist.
int get_branch_number(const char* branch_name) {
  FILE* fbranches = fopen(".beargit/.branches", "r");

  int branch_index = -1;
  int counter = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), fbranches)) {
    strtok(line, "\n");
    if (strcmp(line, branch_name) == 0) {
      branch_index = counter;
    }
    counter++;
  }

  fclose(fbranches);

  return branch_index;
}

/* beargit branch
 *
 * See "Step 5" in the project spec.
 *
 */

int beargit_branch() {
  /* COMPLETE THE REST */
  FILE* fbranches = fopen(".beargit/.branches", "r");
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), fbranches)) {
    strtok(line, "\n");
    if (strcmp(current_branch, line) == 0) {
      fprintf(stdout, "*  %s\n", line);
    }
    else {
      fprintf(stdout, "   %s\n", line);
    }
  }

  fclose(fbranches);

  return 0;
}

/* beargit checkout
 *
 * See "Step 6" in the project spec.
 *
 */

int checkout_commit(const char* commit_id) {
  /* COMPLETE THE REST */
  //SPECIAL CASE: If new commit is 000 commit, empty index
  if (strcmp("0000000000000000000000000000000000000000", commit_id) == 0) {
    FILE *index = fopen(".beargit/.index", "r");
    char line[FILENAME_SIZE];
    //char file[9 + FILENAME_SIZE + 1] = ".beargit/";
    while (fgets(line, FILENAME_SIZE, index)) {
      strtok(line, "\n");
      //strcat(file, line);
      fs_rm(line);
    }
    fclose(index);

    //Clear current index.txt
    //write_string_to_file(".beargit/.index", "");
    remove(".beargit/.index");
    FILE* fx = fopen(".beargit/.index", "w");
    fclose(fx);

    write_string_to_file(".beargit/.prev", commit_id);
    return 0;
  }

  //Delete all files in current directory listed in index
  FILE *index = fopen(".beargit/.index", "r");
  char filename[FILENAME_SIZE];
  //char this_commit_id[COMMIT_ID_SIZE];
  //read_string_from_file(".beargit/.prev", this_commit_id, COMMIT_ID_SIZE);
  while (fgets(filename, FILENAME_SIZE, index)) {
    strtok(filename, "\n");
    /*char fileloc[9 + COMMIT_ID_SIZE + 1 + FILENAME_SIZE + 1] = ".beargit/";
    strcat(fileloc, this_commit_id);
    strcat(fileloc, "/");
    strcat(fileloc, filename);*/
    fs_rm(filename);
  }
  fclose(index);

  //Clear current index.txt
  //write_string_to_file(".beargit/.index", "");
  //remove(".beargit/.index");
  //FILE* fx = fopen(".beargit/.index", "w");
  //fclose(fx);


  //Copy index.txt of commit_id to current index.txt
  char commit_id_index[9 + COMMIT_ID_SIZE + 7 + 1] = ".beargit/";
  strcat(commit_id_index, commit_id);
  strcat(commit_id_index, "/.index");
  /*FILE *findex = fopen(commit_id_index, "r");
  char line[FILENAME_SIZE];
  while(fgets(line, FILENAME_SIZE, findex)) {
    strtok(line, "\n");
    beargit_add(line);
    }
  fclose(findex);*/
  fs_cp(commit_id_index, ".beargit/.index");

  //Copy all commit_id's tracked files into current directory
  FILE *finx = fopen(".beargit/.index", "r");
  char trck_file[FILENAME_SIZE];
  char curr_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", curr_commit_id, COMMIT_ID_SIZE);
  while(fgets(trck_file, sizeof(trck_file), finx)) {
    strtok(trck_file, "\n");
    char new_direct_loc[9 + COMMIT_ID_SIZE + 1 + sizeof(trck_file) + 1] = ".beargit/";
    //char current_direct[9 + COMMIT_ID_SIZE + 1 + sizeof(trck_file) + 1] = ".beargit/";
    strcat(new_direct_loc, commit_id);
    strcat(new_direct_loc, "/");
    strcat(new_direct_loc, trck_file);
    fs_cp(new_direct_loc, trck_file);
    }
  fclose(finx);
  
  //Write ID of commit being checked out into .prev
  write_string_to_file(".beargit/.prev", commit_id);

  return 0;
}

int is_it_a_commit_id(const char* commit_id) {
  /* COMPLETE THE REST */
  char direct[9 + COMMIT_ID_SIZE + 1] = ".beargit/";
  strcat(direct, commit_id);
  return fs_check_dir_exists(direct);
}

int beargit_checkout(const char* arg, int new_branch) {
  // Get the current branch
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  // If not detached, update the current branch by storing the current HEAD into that branch's file...
  if (strlen(current_branch)) { //Bug may be here
    char current_branch_file[BRANCHNAME_SIZE+50];
    sprintf(current_branch_file, ".beargit/.branch_%s", current_branch);
    fs_cp(".beargit/.prev", current_branch_file);
  }

  // Check whether the argument is a commit ID. If yes, we just stay in detached mode
  // without actually having to change into any other branch.
  if (is_it_a_commit_id(arg)) {
    char commit_dir[FILENAME_SIZE] = ".beargit/";
    strcat(commit_dir, arg);
    /*if (!fs_check_dir_exists(commit_dir)) {
      fprintf(stderr, "ERROR:  Commit %s does not exist.\n", arg);
      return 1;
    }*/

    // Set the current branch to none (i.e., detached).
    write_string_to_file(".beargit/.current_branch", "");

    return checkout_commit(arg);
  }

  // Just a better name, since we now know the argument is a branch name.
  const char* branch_name = arg;

  // Read branches file (giving us the HEAD commit id for that branch).
  int branch_exists = (get_branch_number(branch_name) >= 0);

  // Check for errors.
  if (!(!branch_exists || !new_branch)) { //Delete extra "!"?
    fprintf(stderr, "ERROR:  A branch named %s already exists.\n", branch_name);
    return 1;
  } else if (!branch_exists && !new_branch) {
    fprintf(stderr, "ERROR: No branch or commit %s exists\n", branch_name);
    return 1;
  }

  // File for the branch we are changing into.
  char branch_file[BRANCHNAME_SIZE] = ".beargit/.branch_";
  strcat(branch_file, branch_name);

  // Update the branch file if new branch is created (now it can't go wrong anymore)
  if (new_branch) {
    FILE* fbranches = fopen(".beargit/.branches", "a");
    fprintf(fbranches, "%s\n", branch_name);
    fclose(fbranches);
    fs_cp(".beargit/.prev", branch_file);
  }

  write_string_to_file(".beargit/.current_branch", branch_name);

  // Read the head commit ID of this branch.
  char branch_head_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(branch_file, branch_head_commit_id, COMMIT_ID_SIZE);

  // Check out the actual commit.
  return checkout_commit(branch_head_commit_id);
}

/* beargit reset
 *
 * See "Step 7" in the project spec.
 *
 */

int beargit_reset(const char* commit_id, const char* filename) {
  if (!is_it_a_commit_id(commit_id)) {
      fprintf(stderr, "ERROR:  Commit %s does not exist.\n", commit_id);
      return 1;
  }

  // Check if the file is in the commit directory
  /* COMPLETE THIS PART */
    // Check if file is in index of commit
    int is_in_commit_index = 0;
    // Construct .beargit/commit_id/.index
    char commit_index[9 + COMMIT_ID_SIZE + 7 + 1] = ".beargit/";
    strcat(commit_index, commit_id);
    strcat(commit_index, "/.index");
    FILE* index = fopen(commit_index, "r");
    char file[FILENAME_SIZE];
    while (fgets(file, FILENAME_SIZE, index)) {
      strtok(file, "\n");
      if (strcmp(filename, file) == 0) {
        is_in_commit_index = 1; 
      }
    }
    fclose(index);

    //If file not in commit's index, return error 
    if (is_in_commit_index == 0) {
      fprintf(stderr, "ERROR:  %s is not in the index of commit %s.", filename, commit_id);
      return 1;
    }
    //File must be in commit's index
    //Check if file is in current index
    int is_in_index = 0;
    FILE* findex = fopen(".beargit/.index", "r");
    char line[FILENAME_SIZE];
    while (fgets(line, FILENAME_SIZE, findex)) {
      strtok(line, "\n");
      if (strcmp(filename, line) == 0) {
        is_in_index = 1;
      }
    }
    fclose(findex);

    //Construct .beargit/commit_id/filename
    char file_loc[9 + COMMIT_ID_SIZE + 1 + FILENAME_SIZE + 1] = ".beargit/";
    strcat(file_loc, commit_id);
    strcat(file_loc, "/");
    strcat(file_loc, filename);
    // File is in current index, just replace the one in working directory
    if (is_in_index == 1) {
      fs_cp(file_loc, filename);
    } else {
    //If file not in working directory index, add to current directory and .index file
      fs_cp(file_loc, filename);
      beargit_add(filename);
    }

  // Copy the file to the current working directory
  /* COMPLETE THIS PART */

  // Add the file if it wasn't already there
  /* COMPLETE THIS PART */

  return 0;
}

/* beargit merge
 *
 * See "Step 8" in the project spec.
 *
 */

int beargit_merge(const char* arg) {
  // Get the commit_id or throw an error
  char commit_id[COMMIT_ID_SIZE];
  if (!is_it_a_commit_id(arg)) {
      if (get_branch_number(arg) == -1) {
            fprintf(stderr, "ERROR:  No branch or commit %s exists.\n", arg);
            return 1;
      }
      char branch_file[FILENAME_SIZE];
      snprintf(branch_file, FILENAME_SIZE, ".beargit/.branch_%s", arg);
      read_string_from_file(branch_file, commit_id, COMMIT_ID_SIZE);
  } else {
      snprintf(commit_id, COMMIT_ID_SIZE, "%s", arg);
  }

  // Iterate through each line of the commit_id index and determine how you
  // should copy the index file over
   /* COMPLETE THE REST */

  //Construct ".beargit/commit_id/.index"
  char commit_id_index[9 + COMMIT_ID_SIZE + 7 + 1] = ".beargit/";
  strcat(commit_id_index, commit_id);
  strcat(commit_id_index, "/.index");

  //Open commit_id/index.txt
  FILE *findex = fopen(commit_id_index, "r");
  char trck_file[FILENAME_SIZE];
  //Exists marks whether tracked files exists in current directory
  int exists = 0;

  while(fgets(trck_file, FILENAME_SIZE, findex)) {
    exists = 0;
    strtok(trck_file, "\n");
    FILE *index = fopen(".beargit/.index", "r");
    char curr_ind_file[FILENAME_SIZE];

    //Check if file also tracked in current index, then exists = 1
    while(fgets(curr_ind_file, FILENAME_SIZE, index)) {
      strtok(curr_ind_file, "\n");
      if (strcmp(curr_ind_file, trck_file) == 0) {
        exists = 1;
      }
    }
    fclose(index);

    //Construct ".beargit/commit_id/filename" to file_loc
    char file_loc[9 + COMMIT_ID_SIZE + 1 + FILENAME_SIZE + 1] = ".beargit/";
    strcat(file_loc, commit_id);
    strcat(file_loc, "/");
    strcat(file_loc, trck_file);

    //If file also tracked in current directory
    if (exists == 1) {
      //Construct filename.commit for conflicted copy
      char trck_file_commit[FILENAME_SIZE + 1 + COMMIT_ID_SIZE + 1];
      strcpy(trck_file_commit, trck_file);
      strcat(trck_file_commit, ".");
      strcat(trck_file_commit, commit_id);
      fs_cp(file_loc, trck_file_commit);
      fprintf(stdout, "%s conflicted copy created\n", trck_file);
    }
    else {
      fs_cp(file_loc, trck_file);
      beargit_add(trck_file);
      fprintf(stdout, "%s added\n", trck_file);
    }
  }
  fclose(findex);
  return 0;
}
