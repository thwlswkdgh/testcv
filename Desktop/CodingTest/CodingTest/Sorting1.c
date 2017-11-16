//선택정렬 -> 가장 큰 것을 찾는 방법

#include <stdio.h>
int main(){
	FILE * rfp = fopen("input.txt", "r");
	FILE * wfp = fopen("output.txt", "w");
	int a[10], b[10];
	char *c[10];
	int i, j;
	int big;
	//store 'a' array
	if (rfp != NULL){
		for (i = 0; i < 10; i++){
			fscanf(rfp, "%d ", &a[i]);
		}
		//copy 'a' array to 'b' array
		for (i = 0; i < 10; i++){
			b[i] = a[i];
		}
		fclose(rfp);
	}
	//arrange 'a' array
	for (j = 9; j >= 0; j--){
		for (i = j; i >= 0; i--){
			if (a[j] <= a[i]){
				big = a[i];
				a[i] = a[j];
				a[j] = big;
			}
		}
	}

	for (i = 9; i >= 0; i--){
		for (j = 0; j < 10; j++){
			if (b[j] == a[i]){
				//printf("b는 무엇인가:%d ", b[j]);
				//printf("i:%d / j:%d ", i, j);
				/*switch (i){
				case 1: c[j] = "D"; 
				case 2: c[j] = "D"; 
				case 3: c[j] = "C"; 
				case 4: c[j] = "C";
				case 5: c[j] = "B";
				case 6: c[j] = "B";
				case 7: c[j] = "A";
				case 8: c[j] = "A";
				case 9: c[j] = "A"; 
				}*/
				if (i == 9) c[j] = "A+";
				else if (i == 8) c[j] = "A+";
				else if (i == 7) c[j] = "A";
				else if (i == 6) c[j] = "B+";
				else if (i == 5) c[j] = "B";
				else if (i == 4) c[j] = "C+";
				else if (i == 3) c[j] = "C";
				else if (i == 2) c[j] = "D+";
				else if (i == 1) c[j] = "D";
				else if (i == 0) c[j] = "F";

			}
		}
	}
	
	
	for (i = 0; i < 10; i++){
		printf("%s ", c[i]);
		fprintf(wfp, "%s ", c[i]);
	}


}