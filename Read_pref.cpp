#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<fstream>
#include<string>

using namespace std;

//declaring the semaphores wrt and mutex
// one normal variable read_count

sem_t mutex;
sem_t wrt;
int read_count;
string file_read_contents;

void * read (void* parameters)
{
        /*
        Algorithm:

        wait(mutex)
        readcount++
            if readcount == 1
                then wait(wrt)
        signal mutex

        READ on file

        wait(mutex)
        readcount--
            if readcount == 0
                then signal(wrt)
        signal (mutex)
    */

    sem_wait(&mutex);
    read_count++;
    if(read_count==1)
        sem_wait(&wrt);
    sem_post(&mutex);
    ifstream read_file;
    read_file.open("Resource.txt");
    if(read_file.is_open())
    {
        while(getline(read_file,file_read_contents))
        {
            cout<< file_read_contents <<endl;
        }
        read_file.close();
    }
    else
        cout<<"Problem opening the read file "<<endl;

    sem_wait(&mutex);
    read_count--;
    if(read_count==0)
        sem_post(&wrt);
    sem_post(&mutex);

}

void* write(void* parameters)
{
    /*
        Algorithm:

        wait(wrt)
        write on file
        signal(wrt)
    */


    sem_wait(&wrt);
    cout<<"Writing : \n";

    ofstream myfile;
    myfile.open("Resource.txt",ios :: out);

    if(myfile.is_open())
    {

        for(int i=0;i<=5;i++)
        {
            myfile<<"This is line number",i;
            myfile<<" line written by thread id: ",pthread_self();
            myfile<<endl;

        }

        myfile.close();

    }

    else
    {
        cout<<"Error while opening the file"<<endl;
    }

    sem_post(&wrt);
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    int reader_count=3;
    int writer_count=2;
    cout<<"Reader's preference first reader threads will be executed and then writer"<<endl;

    //Initializing semaphores with values
    sem_init(&mutex,0,1);
    sem_init(&wrt,0,1);
    pthread_t readers[reader_count];
    pthread_t writers[writer_count];

    //creating threads
    pthread_create(&readers[2], NULL, read, NULL);
    pthread_create(&readers[3], NULL, read, NULL);
    pthread_create(&writers[0], NULL, write, NULL);
    pthread_create(&readers[0], NULL, read, NULL);
    pthread_create(&writers[1], NULL, write, NULL);

    //joining threads
    pthread_join(readers[2], NULL);
    pthread_join(readers[3], NULL);
    pthread_join(writers[0], NULL);
    pthread_join(readers[0], NULL);
    pthread_join(writers[1], NULL);

    return 0;
}


