#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name;
    unsigned int start;
    unsigned int end;
} Job;

int compareJobs(const void *a, const void *b) {
    Job *job1 = (Job *)a;
    Job *job2 = (Job *)b;

    if (job1->end != job2->end) {
        return job1->end - job2->end;
    }
    if (job1->start != job2->start) {
        return job1->start - job2->start;
    }
    return 1;
}

void scheduleJobs(Job jobs[], int numJobs, unsigned int timeslots) {
    qsort(jobs, numJobs, sizeof(Job), compareJobs);

    int currentEndTime = 0;

    for (unsigned int slot = 0; slot < timeslots; slot++) {
        for (int i = 0; i < numJobs; i++) {
            if (jobs[i].start > currentEndTime && jobs[i].end == slot) {
                printf("%c\n", jobs[i].name);
                currentEndTime = jobs[i].end;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    char buf[256];

    // Read the number of jobs to be scheduled
    if (!fscanf(fp, "%s\n", buf)) {
        perror("reading the number of jobs failed");
        exit(EXIT_FAILURE);
    }
    int jobcount = atoi(buf);

    // Next read the maximum number of timeslots
    if (!fscanf(fp, "%s\n", buf)) {
        perror("reading the number of timeslots failed");
        exit(EXIT_FAILURE);
    }
    unsigned int timeslots = atoi(buf);

    // Now read the rest of the file
    Job jobs[jobcount];
    for ( int line=0; line<jobcount; line++ ) {
        if (!fgets(buf, 256, fp)) {
            perror("reading a line for a job failed");
            exit(EXIT_FAILURE);
        }

        if (!sscanf(buf, "%c %d %d", &jobs[line].name, &jobs[line].start, &jobs[line].end)) {
            perror("parsing a line for a job failed");
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);

    scheduleJobs(jobs, jobcount, timeslots);

    exit(EXIT_SUCCESS);
}