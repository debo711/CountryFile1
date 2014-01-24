/* 
 * File:   Ch5Part1_main.c
 * Author: Obed Ornelas
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <sys/stat.h>
#include <errno.h>
#include "apue.h"
/*
 * 
 */
struct DirectoryStruct{
        char Code[4];    //field 1
        off_t Offset;    //field 2
    };
    
struct Record{
    char CountryCode[4];   //field 2
    char CountryName[41];   //field 3
    int Pop;             //field 8
    float LifeExp;      //field 9
    };

void bubbleSort(struct DirectoryStruct *myDirStructPtr, int array_size);
int main(int argc, char** argv) {
      
      
    int J;
    int numOfRecs = 239;
    char *result;
    struct Record *RecordPtr;
    struct DirectoryStruct *DStructPtr;
    
    FILE *ptrFile;
    FILE *recFilePtr;
    char buffer[MAXLINE];
    
    ptrFile = fopen("AllCountries.txt","r");
    recFilePtr = fopen("RandomCountryFile.bin","wb");
    
    if(!ptrFile)err_sys("AllCountries file was not opened");
    if (!recFilePtr)err_sys("Random Country file was not opened");
    
    RecordPtr = (struct Record*)calloc(numOfRecs, sizeof(struct Record));//allocate space for 239 records
    DStructPtr = calloc(numOfRecs,sizeof(struct DirectoryStruct));//allocate space for directory of 239 countries
    
    J = 0;
    char *endptr;
    while(fgets(buffer,MAXLINE,ptrFile)!= NULL)
    {
        printf("%s",buffer);
        result = strtok(buffer,",\n");
        int i;
        for(i = 1; i < 16;i++)
        {
            switch(i)
            {
                case 2:
                    strncpy(RecordPtr[J].CountryCode,result,4);
                    printf("%s\n",RecordPtr[J].CountryCode);
                    strncpy(DStructPtr[J].Code,result,4);
                    break;
                case 3:
                    strncpy(RecordPtr[J].CountryName,result,41);
                    printf("%s\n",RecordPtr[J].CountryName);
                    break;
                case 8:
                    RecordPtr[J].Pop = atoi(result);
                    printf("%d\n",RecordPtr[J].Pop);
                    break;
                case 9:
                    RecordPtr[J].LifeExp = strtof(result,&endptr);
                    printf("%.1f\n",RecordPtr[J].LifeExp);
                    break;
            }
            result = strtok(NULL,",\n");
        }
        DStructPtr[J].Offset = J * sizeof(struct Record); //calculate offset and store it as soon whole record is read
        J++;
    }
    
    if(fwrite(RecordPtr,sizeof(struct Record),numOfRecs,recFilePtr) != numOfRecs)//write array of structs to file
        err_sys("fwrite error for array of records");

    FILE *dirFilePtr;
    dirFilePtr = fopen("CountryDirectory.bin","wb");
    if (!dirFilePtr)err_sys("Country directory cant write");
    
    bubbleSort(DStructPtr,numOfRecs);//sort directory 
    if(fwrite(DStructPtr,sizeof(struct DirectoryStruct),numOfRecs,dirFilePtr) != numOfRecs) //write directory to file
        err_sys("fwrite error for Directory");
    
    printf("Files Written");
    
    free(RecordPtr);
    free(DStructPtr);
    fclose(dirFilePtr);
    fclose(recFilePtr);
    fclose(ptrFile);
    return (EXIT_SUCCESS);
}
void bubbleSort(struct DirectoryStruct *myDirStructPtr, int array_size)
{
    int i, j;
    struct DirectoryStruct temp;

    for (i = (array_size - 1); i > 0; i--)
    {
	for (j = 1; j <= i; j++)
	{
            if ( strncmp(((myDirStructPtr[j-1]).Code), ((myDirStructPtr[j]).Code), 3) > 0)
            {
				temp = myDirStructPtr[j-1];
				myDirStructPtr[j-1] = myDirStructPtr[j];
				myDirStructPtr[j] = temp;
            }
	}
    }
}

