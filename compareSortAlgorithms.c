#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
    int i, j, temp;

    for (i = 1; i < n; i++)
    {
        if (arr[i] > arr[(i - 1) / 2])
        {
            int j = i;

            while (arr[j] > arr[(j - 1) / 2])
            {
                temp=arr[j];
                arr[j]=arr[(j-1)/2];
                arr[(j-1)/2]=temp;
                j = (j - 1) / 2;
            }
        }
    }

    for (i = n - 1; i > 0; i--)
    {
        temp=arr[0];
        arr[0]=arr[i];
        arr[i]=temp;

        j=0;

        int index;

        do
        {
            index = (2 * j + 1);

            if (arr[index] < arr[index + 1] && index < (i - 1))
                index++;

            if (arr[j] < arr[index] && index < i)
            {
                int tem1=arr[j];
                arr[j]=arr[index];
                arr[index]=tem1;
            }

            j = index;

        } while (index < i);
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	int i, j, k;

	if(l<r)
	{
		int m=(r+l)/2;

		mergeSort(pData, l, m);
		mergeSort(pData, m+1, r);

		int n1=m-l+1;
		int n2=r-m;

		size_t size1=Size(n1);
		size_t size2=Size(n2);


		int *L=Alloc(size1);
		int *R=Alloc(size2);

		for(i=0;i<n1;i++)
		{
			L[i]=pData[l+i];
		}

		for(j=0;j<n2;j++)
		{
			R[i]=pData[m+1+j];
		}

		i=0;
		j=0;
		k=l;

		while(i<n1 && j<n2)
		{
			if(L[i]<=R[j])
			{
				pData[k]=L[i];
				i++;
			}
			else
			{
				pData[k]=R[j];
				j++;
			}
			k++;
		}

		while(i<n1)
		{
			pData[k]=L[i];
			i++;
			k++;
		}

		while(j<n2)
		{
			pData[k]=R[j];
			j++;
			k++;
		}

		DeAlloc(L);
		DeAlloc(R);
	}
}


// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, j, item;

	pData=Alloc(Size(n));

	for(i=1;i<n;i++)
    {
        item=pData[i];

        for(j=i-1;j>=0;j--)
        {
            if(pData[i]>item)
                pData[j+1]=pData[j];
            else
                break;
        }
        pData[j+1]=item;
    }
    DeAlloc(pData);
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int i, j, temp;

	pData=Alloc(Size(n));

	for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(pData[j]>pData[j+1])
            {
                temp=pData[j];
                pData[j]=pData[j+1];
                pData[j+1]=temp;
            }
        }
    }
    DeAlloc(pData);
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, min_index, temp;

	pData=Alloc(Size(n));

	for(i=0;i<n-1;i++)
    {
        min_index=i;

        for(j=i+1;j<n;j++)
            if(pData[j]<pData[min_index])
                min_index=j;

        temp=pData[i];
        pData[i]=pData[min_index];
        pData[min_index]=temp;

    }
    DeAlloc(pData);
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");

	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");

		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}

}
