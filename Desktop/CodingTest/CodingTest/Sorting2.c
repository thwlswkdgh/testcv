//삽입정렬 -> M개짜리 배열에 하나의 원소를 추가하여 M+1개짜리 정렬된 배열을 만드는 과정 반복

#include <stdio.h>
int main(){
	FILE * rfp = fopen("input.txt", "r");
	FILE * wfp = fopen("output.txt", "w");
	int a[10]; int check;
	int i,j,temp;
	if (rfp != NULL){
		for (i = 0; i < 10; i++){
			fscanf(rfp, "%d ", &a[i]);
		}
		fscanf(rfp, "%d", &check);
	}
	for (i = 1; i < 10; i++){
		for (j = i-1; j >= 0; j--){
			if (a[j] > a[j+1]){
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}
	}
	for (i = 0; i < 10; i++){
		if (check == a[i])
			check = i + 1;
	}
	for (i = 0; i < 10; i++)
		fprintf(wfp, "%d ", a[i]);
	fprintf(wfp, "\n%d", check);



}