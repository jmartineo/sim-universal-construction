#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>

#include <config.h>
#include <primitives.h>
#include <fastrand.h>
#include <threadtools.h>
#include <osciqueue.h>
#include <barrier.h>
#include <bench_args.h>

OsciQueueStruct queue_object CACHE_ALIGN;
int64_t d1 CACHE_ALIGN, d2;
SynchBarrier bar CACHE_ALIGN;
SynchBenchArgs bench_args CACHE_ALIGN;

inline static void *Execute(void *Arg) {
    OsciQueueThreadState *th_state;
    long i, rnum;
    volatile int j;
    long id = (long)Arg;

    synchFastRandomSetSeed(id);
    synchBarrierWait(&bar);
    if (id == 0) d1 = synchGetTimeMillis();

    th_state = synchGetAlignedMemory(CACHE_LINE_SIZE, sizeof(OsciQueueThreadState));
    OsciQueueThreadStateInit(&queue_object, th_state, id);
    for (i = 0; i < bench_args.runs; i++) {
        // perform an enqueue operation
        OsciQueueApplyEnqueue(&queue_object, th_state, (ArgVal)id, id);
        rnum = synchFastRandomRange(1, bench_args.max_work);
        for (j = 0; j < rnum; j++)
            ;
        // perform a dequeue operation
        OsciQueueApplyDequeue(&queue_object, th_state, id);
        rnum = synchFastRandomRange(1, bench_args.max_work);
        for (j = 0; j < rnum; j++)
            ;
    }
    synchBarrierWait(&bar);
    if (id == 0) d2 = synchGetTimeMillis();

    return NULL;
}

int main(int argc, char *argv[]) {
    synchParseArguments(&bench_args, argc, argv);

    OsciQueueInit(&queue_object, bench_args.nthreads, bench_args.fibers_per_thread);
    synchBarrierSet(&bar, bench_args.nthreads);
    synchStartThreadsN(bench_args.nthreads, Execute, bench_args.fibers_per_thread);
    synchJoinThreadsN(bench_args.nthreads);

    printf("time: %d (ms)\tthroughput: %.2f (millions ops/sec)\t", (int)(d2 - d1), 2 * bench_args.runs * bench_args.nthreads / (1000.0 * (d2 - d1)));
    synchPrintStats(bench_args.nthreads, bench_args.total_runs);

#ifdef DEBUG
    fprintf(stderr, "DEBUG: Enqueue: Object state: %ld\n", queue_object.enqueue_struct.counter);
    fprintf(stderr, "DEBUG: Enqueue: rounds: %d\n", queue_object.enqueue_struct.rounds);
    fprintf(stderr, "DEBUG: Dequeue: Object state: %ld\n", queue_object.dequeue_struct.counter);
    fprintf(stderr, "DEBUG: Dequeue: rounds: %d\n", queue_object.dequeue_struct.rounds);
    volatile Node *first = queue_object.first;
    long counter = 0;

    while (first->next != NULL) {
        first = first->next;
        counter++;
    }
    fprintf(stderr, "DEBUG: %ld nodes were left in the queue\n", counter);
#endif

    return 0;
}
