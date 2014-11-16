#include "profile.h"
#include "os.h"

typedef enum { PROF_START = 'B', PROF_END = 'E' } ProfileType;

typedef struct {
    uint64_t nanoTimeStamp;
    const char* ID;
    ProfileType type;
} ProfileSample;

static const int MAX_SAMPLES = 1 << 17;

static ProfileSample g_samples[MAX_SAMPLES];

static int g_sampleCnt = 0;

void profileBlockStart(const char* name) {
    ProfileSample* sample = &g_samples[g_sampleCnt % MAX_SAMPLES];
    sample->ID = name;
    sample->nanoTimeStamp = nanoTime();
    sample->type = PROF_START;
    g_sampleCnt++;
}

void profileBlockEnd(const char* name) {
    ProfileSample* sample = &g_samples[g_sampleCnt % MAX_SAMPLES];
    sample->ID = name;
    sample->nanoTimeStamp = nanoTime();
    sample->type = PROF_END;
    g_sampleCnt++;
}

void saveProfile(const char* filename) {
    FILE* fd = fopen(filename, "w");
    int i = 0;
    if(g_sampleCnt >= MAX_SAMPLES) {
        i = g_sampleCnt;
    }
    fprintf(fd, "{\"deviceName\":\"\",\n\"osName\":\"none\",\n\"osVersion\":\"none\",\n\"traceEvents\":[\n");

    do {
        ProfileSample* sample = &g_samples[i % MAX_SAMPLES];

        fprintf(fd, "{\"ph\":\"%c\",\"ts\":%f,\"pid\": 1,\"tid\":0,\"name\":\"%s\"},\n", sample->type,
                sample->nanoTimeStamp / 1000.0, sample->ID);

        i = (i + 1) % MAX_SAMPLES;
    } while(i % MAX_SAMPLES != g_sampleCnt % MAX_SAMPLES);

    fprintf(fd,
            "{\"cat\":\"__metadata\",\"pid\":1,\"tid\": "
            "0,\"ts\":0,\"ph\":\"M\",\"name\":\"thread_name\",\"args\":{\"name\":\"main\"}}]}");
}
/*
vi: ft=cpp
*/
