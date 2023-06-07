#include "lulesh.h"
#include <unistd.h>
#include <time.h>


struct proc_info* pstruct_p;
struct PREDICTOR* predictor;

void try_extrawork(){
    while(can_do_extrawork() &&
         (MPI_Wtime()) - pstruct_p->start_time < pstruct_p->expected_superstep_duration){
        do_extrawork();
    }

    MPI_Barrier(MPI_COMM_WORLD);

    pstruct_p->start_time = MPI_Wtime();

}

void finish_extrawork(){

    stage=1;
    //while(can_do_extrawork()){
        //do_extrawork();
    //}
    MPI_Barrier(MPI_COMM_WORLD);
    timelord_stat();
    //cleanup_extrawork();
    free(pstruct_p);
}

void setup_timelord(){
    pstruct_p=(struct proc_info*)malloc(sizeof(struct proc_info));
    predictor=(struct PREDICTOR*)malloc(sizeof(struct PREDICTOR));
    MPI_Comm_rank(MPI_COMM_WORLD, &pstruct_p->rank);
    MPI_Comm_size(MPI_COMM_WORLD, &pstruct_p->size);
    setup_extrawork();
    pstruct_p->start_time = MPI_Wtime();
}

static double count_try, count_finish;
static unsigned int seed;  // Initial seed value
int stage;

const unsigned int a = 1103515245;  // Multiplier
const unsigned int c = 12345;  // Increment
const unsigned int m = 2147483648;  // Modulus


unsigned int lcg() {
    seed = (a * seed + c ) % m;
    return seed;

}

void setup_extrawork(){
    count_try =0;
    count_finish=0;
    stage=0;
    seed = 123;
}

void do_extrawork(){
    lcg();
    if(stage)
        count_finish+=1;
    else
        count_try+=1;
}

void timelord_stat(){
    std::cout<<"STATS: P"<<pstruct_p->rank<<" try_EW: "<<count_try<<" finish_EW: "<<count_finish<<std::endl;
}

void cleanup_extrawork(){
    free((double*)pstruct_p->scratch);
}

int can_do_extrawork(){
    return true;
}
