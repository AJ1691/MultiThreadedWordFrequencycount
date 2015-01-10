/*
 * project.c
 *
 *  Created on: 01-Nov-2014
 *      Author: Junglee
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

struct word_record{
char *str;
int count;
};

struct thread_data{
    FILE *fp;
    int start;
    int blockSize;
    char filepath[1024];
};

pthread_mutex_t critical_mutex;
int i=0;
struct word_record * Rec;
//*start;
//int data_index(struct word_record *Rec,char *word,int max);
//void sort(struct word_record *Rec);

void* count_frequency(void* data){
        int index=0,k=0;

      struct thread_data* td=data;

      char *buffer = malloc(td->blockSize);
      memset(buffer,'\0',sizeof(buffer));
       
      FILE *lfp=fopen(td->filepath,"r"); 
      fseek(lfp, td->start,SEEK_SET);
        

      for(k=0;k<td->blockSize;k++)
                *(buffer+k)=fgetc(lfp);
                   
      
      fclose(lfp);
      //critical section entry
      pthread_mutex_lock(&critical_mutex);

      (Rec+i)->str= strtok(buffer," ,.-?'!@#$%^&*");
      while ((Rec+i)->str != NULL)
      {
        index=data_index(Rec,(Rec+i)->str,i);    
       if(index >=0){
         (Rec+index)->count++;
          (Rec+i)->str= strtok (NULL," ,.-?'!@#$%^&*"); 
       }
       else{
       i++;
       (Rec+i)->str= strtok (NULL," ,.-?'!@#$%^&*");
       }
      }
      free(buffer);

      //critical section exit
      pthread_mutex_unlock(&critical_mutex);
}


int main(int argc, char **argv){

    int nthreads=5, x, id, blockSize,len;
    FILE *fp;
    pthread_t *threads;

    struct thread_data data[nthreads];
    char filepath[1024] = {'\0',};
    printf("enter file path \n");
    scanf("%s",filepath);
    fp=fopen(filepath,"r");
    printf("Enter the number of threads: ");
    scanf("%d",&nthreads);
    threads = malloc(nthreads*sizeof(pthread_t));

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    printf("len= %d\n",len);
    fclose(fp);
    Rec =(struct word_record *)calloc((len/1000),(sizeof(struct word_record)));
    //start=Rec;
    blockSize=(len)/nthreads;
    printf("size= %d\n",blockSize);

    for(id = 0; id < nthreads; id++){
        strcpy(data[id].filepath, filepath);
        data[id].fp=fp;   
        data[id].start = id*blockSize;
        data[id].blockSize=blockSize;
        }

    for(id = 0; id < nthreads; id++)
            pthread_create(&threads[id], NULL, &count_frequency,&data[id]);
                

    for(id = 0; id < nthreads; id++)
        pthread_join(threads[id],NULL);

    fclose(fp);

    //printf("%d\n",words);
    return 0;

    //sort(Rec);

    //printing out the data

    int k=0;
    for(k=0;k<=i;k++){
    printf("%s : %d", (Rec+k)->str , ((Rec+k)->count));
    }

}


//this function compares the string with the current table of strings and returns the index of the matching string
//or -1 if the string is new
int data_index(struct word_record *Rec,char *word,int max){
int j;
   for(j=0; j<max;i++){
       if(strcmp((Rec+j)->str,word)==0){
          return j;
       }
    }
    return -1;
}






void sort(struct word_record *Rec){

//index of the last entry
int max= i;
//swapping var
char *con;
int m, n;
//doing bubble sort
for( m=0;m<max;m++){
for( n=0;n<m;n++){

    if(strcmp((Rec+m)->str,(Rec+n)->str)>1){

       con=(Rec+n)->str;
       (Rec+n)->str=(Rec+m)->str;
       (Rec+m)->str=con;

      }
}

}

}



