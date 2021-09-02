#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

void zigzag(int A[][SIZE])
{
	int value = 1;
	for (int i = 0; i < SIZE; i++)
	{
		if (i % 2 == 0)
			for (int j = 0; j < SIZE; j++)
				A[i][j] = value++;
		else
			for (int j = SIZE - 1; j >= 0; j--)
				A[i][j] = value++;
	}
}

//나선형 채우기
void spiral(int A[][SIZE], int n)
{
	int left = 0, right = SIZE - 1;
	int top = 0, bottom = n - 1;
	int value = 1, i;
	while (left <= right && top <= bottom)
	{
		for (i = left; i <= right; i++)
			A[top][i] = value++;
		top++;
		if (top <= bottom)
		{
			for (i = top; i <= bottom; i++)
				A[i][right] = value++;
			right--;
		}
		if (left <= right)
		{
			for (i = right; i >= left; i--)
				A[bottom][i] = value++;
			bottom--;
		}
		if (top <= bottom)
		{
			for (i = bottom; i >= top; i--)
				A[i][left] = value++;
			left++;
		}
	}
}

void printArray(int A[][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			printf("%2d ", A[i][j]);
		printf("\n");
	}
}

//나선형 용
void printArray2(int A[][SIZE])
{
	for (int i = 0; i < SIZE-1; i++)
	{
		for (int j = 0; j < SIZE; j++)
			printf("%2d ", A[i][j]);
		printf("\n");
	}
}


void main()
{
	int A[SIZE - 1][SIZE] = { 0 };
	//zigzag(A);
	spiral(A, SIZE - 1);
	printArray2(A);
}
