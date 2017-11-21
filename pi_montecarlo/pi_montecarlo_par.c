#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#define PI 3.14159265

int main(int argc, char** argv)
{
    float e; 
    double start,end;
    float pi=0.0;
    MPI_Status status;
    int master =0;
    int request_tag = 0, chunk_tag = 1, pi_tag = 2;
    int total_done =0;
    const int chunk_size = 100000000;
    float chunk[chunk_size];

    srand((unsigned int)time(NULL));

    MPI_Init (&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int server_rank = world_size - 1;
    
    const int server_ranks[1] = {server_rank};
    // Get the group of processes in MPI_COMM_WORLD
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    // Construct a group containing all of the world_ranks except in server_ranks in world_group
    MPI_Group worker_group;
    MPI_Group_excl(world_group, 1, server_ranks, &worker_group);

    // Create a new communicator based on the worker group
    MPI_Comm worker_comm;
    MPI_Comm_create(MPI_COMM_WORLD, worker_group, &worker_comm);

    int worker_rank = -1, worker_size = -1;
    if (MPI_COMM_NULL != worker_comm) {
        MPI_Comm_rank(worker_comm, &worker_rank);
        MPI_Comm_size(worker_comm, &worker_size);
    }

    
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    if (world_rank == 0){
        sscanf(argv[1],"%f",&e);
    }

    MPI_Bcast(&e,1,MPI_REAL,0,MPI_COMM_WORLD);

    int done=0;
    if (world_rank == server_rank) {
        while(done !=(world_size-1)){
            // printf("[%d]: Server Done = %d\n", world_rank, done);
            int request;
            int flag = 0;

            while(!flag){
                // printf("Server %d check incoming message = %d\n",world_rank,flag);
                MPI_Iprobe(MPI_ANY_SOURCE, request_tag, MPI_COMM_WORLD, &flag, &status );
                
            }
            // printf("Server %d receive incoming message = %d\n",world_rank,flag);
            MPI_Recv(&request,1,MPI_INT,MPI_ANY_SOURCE,request_tag,MPI_COMM_WORLD,&status);
            // printf("Server %d receive request = %d from worker %d\n",world_rank,request,status.MPI_SOURCE);
            if(request){
                for (int i=0; i<chunk_size; i++){
                    chunk[i] = ((float)rand())/RAND_MAX;
                }
                MPI_Send(&chunk, chunk_size, MPI_REAL,status.MPI_SOURCE,chunk_tag,MPI_COMM_WORLD);
                // printf("Server %d send array %f to worker %d\n",world_rank,chunk[2],status.MPI_SOURCE);
            }
            else{
                done++;
                // printf("[%d] count done = %d\n",world_rank,done);
            }
        }
        // printf("==============Server Done==============\n");
    }
    else {
        while(!total_done){
            // printf("[%d]: Before Reduce, total_done is %d\n", world_rank, total_done);
            int inside_count = 0;
            int request = 1;
            // printf("[%d]\n",world_rank);
            MPI_Send(&request, 1, MPI_INT,server_rank,request_tag,MPI_COMM_WORLD);
            // printf("Worker %d send request= %d to server %d\n",world_rank,request,server_rank);

            MPI_Recv(&chunk,chunk_size,MPI_REAL,server_rank,chunk_tag,MPI_COMM_WORLD,&status);
            // printf("Woker %d receive chunk= %f from server %d \n",world_rank,chunk[2],server_rank);
            for (int i=0; i<chunk_size; i+=2) {
                float x = chunk[i];
                float y = chunk[i+1];
                float z = x*x + y*y;
                if (z<=1) inside_count++; 
            } /* for all the slave processes send results to the master */
            pi=(float)(4*inside_count)/(chunk_size/2); //4 quadrants of circle
            float temp_e = fabs((pi-PI)/PI);
            if(temp_e<= e) {
                MPI_Send(&pi, 1, MPI_REAL, 0, pi_tag, worker_comm);
                // printf("[%d] Worker %d satisfies!\n",world_rank,worker_rank);
                // printf("PI = %f and Temp_Error = %f and Error = %f\n",pi,temp_e,e);
                done = 1;  
            }
            MPI_Allreduce(&done, &total_done, 1, MPI_INT, MPI_SUM, worker_comm);
            // printf("[%d]: After Reduce, total_done is %d\n", world_rank, total_done);
            if (total_done){
                int request = 0;
                MPI_Send(&request, 1, MPI_INT,server_rank,request_tag,MPI_COMM_WORLD);
                // printf("Done: Worker %d send request= %d to server %d\n",world_rank,request,server_rank);
            }
        }
    }
    // printf("==============Worker [%d] Done==============\n",world_rank);
    
    if (world_rank == 0){
        int flag = 0;
        while(!flag){
            // printf("[%d] check incoming PI message = %d\n",world_rank,flag);
            MPI_Iprobe(MPI_ANY_SOURCE, pi_tag, worker_comm, &flag, &status );
        }
        // printf("[%d] receive PI message from [%d] - flag = %d\n",world_rank,status.MPI_SOURCE,flag);
        MPI_Recv(&pi,1,MPI_REAL,status.MPI_SOURCE,pi_tag,worker_comm,&status);
    }  

    MPI_Group_free(&worker_group);
    MPI_Group_free(&world_group);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    if (world_rank == 0){
        // printf("Time_Start = %f\n",start);
        // printf("Time_End = %f\n",end);
        printf("OUTPUT:\n");
        printf("PI = %f\n",pi);
        printf("Time = %f\n",end-start);
    }
    MPI_Finalize();
  
return 0;
}
