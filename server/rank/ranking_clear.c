#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main(){
    int fdrec, fdread;
    int record_num = 0;

    if((fdrec = open("record", O_CREAT | O_WRONLY | O_TRUNC, 0644))== -1){
        perror("record file open error");
        return -1;
    }
    if(write(fdrec, &record_num, sizeof(record_num)) == -1){
        perror("record file write error");
        return -1;
    }

    if((fdread = open("readable", O_CREAT | O_WRONLY | O_TRUNC, 0644))== -1){
        perror("record file open error");
        return -1;
    }
    
    close(fdread);
    close(fdrec);
    
    return 0;
}