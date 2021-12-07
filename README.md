# cprog
## 사용할 언어는 c언어
***
_성적을 관리하고 csv 파일로 출력하는 프로그램_
***
```c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <stdlib.h> 
 
#define MAX_NAME_LEN  16    
 
typedef struct StudentData  
{
    int num; 
    char name[MAX_NAME_LEN];  
    int kor, eng, math, total; \
} S_DATA;
 

char *GetNextString(char *ap_src_str, char a_delimiter, char *ap_buffer)
{
    
    while (*ap_src_str && *ap_src_str != a_delimiter) *ap_buffer++ = *ap_src_str++;
 
    if (*(ap_buffer - 1) == '\n') *(ap_buffer - 1) = 0;
    else *ap_buffer = 0;
 

    if (*ap_src_str == a_delimiter) ap_src_str++;
 

    return ap_src_str;
}
 
int GetDataCount(FILE *ap_file) 
{
  
    int old_pos = ftell(ap_file), count = 0;
    char temp_string[128];
    while (NULL != fgets(temp_string, 128, ap_file)) count++;
    fseek(ap_file, old_pos, SEEK_SET);
    return count ;
}
 
int ReadData(const char *ap_file_name, S_DATA **ap_data, int *ap_data_count)
{
    FILE *p_file = NULL;  
    if (0 != fopen(ap_file_name, "r")) {

        char one_line_string[128], str[32], *p_pos;

        if (NULL != fgets(one_line_string, 128, p_file)) {

            *ap_data_count = GetDataCount(p_file);
            if (*ap_data_count > 0) {  
   
                S_DATA *p_data = (S_DATA*)malloc(sizeof(S_DATA)*(*ap_data_count));

                *ap_data = p_data;
  
                while (NULL != fgets(one_line_string, 128, p_file)) {
                    p_pos = GetNextString(one_line_string, ',', str); 
                    p_data->num = atoi(str);  
                    p_pos = GetNextString(p_pos, ',', p_data->name); 
                    p_pos = GetNextString(p_pos, ',', str); 
                    p_data->kor = atoi(str);  
                    p_pos = GetNextString(p_pos, ',', str); 
                    p_data->eng = atoi(str);  
                    p_pos = GetNextString(p_pos, ',', str); 
                    p_data->math = atoi(str); 
 
               
                    p_data->total = p_data->kor + p_data->eng + p_data->math;
                    p_data++; 
                }
            }
        }
        fclose(p_file);  
        return 1;  
    }
    return 0;  
}
 

void ShowData(S_DATA *ap_data, int a_count)
{
    printf("-------------------------------------------------------------\n");
    printf("    학번         이름   국어  영어  수학     총점     평균\n");
    printf("-------------------------------------------------------------\n");
 
    int sum = 0; 
    for( int i = 0; i < a_count; i++, ap_data++){
       
        printf("  %08u %10s   %3d   %3d   %3d,      %3d    %.2f\n", 
            ap_data->num, ap_data->name, ap_data->kor, ap_data->eng, ap_data->math, 
            ap_data->total, ap_data->total/3.0);
        sum += ap_data->total; 
    }
    printf("-------------------------------------------------------------\n");

    if (a_count > 0) {
        printf("    총 %d 명, 전체 평균 %.2f\n", a_count, sum /(a_count*3.0));
        printf("-------------------------------------------------------------\n");
    }
}

void ModifyData(S_DATA *ap_data, int a_count)
{
    int num, select;
    printf("정보를 수정할 학생의 학번을 입력하세요 : ");
    scanf("%d", &num);  
 
    for ( int i = 0; i < a_count; i++, ap_data++) {
        if (ap_data->num == num) {  
            printf("수정할 항목을 선택하세요.\n");
            printf("1.이름,  2.국어,  3.영어, 4.수학\n선택: ");
            scanf("%d", &select); 
            switch (select) {
                case 1:
                    printf("변경할 이름을 입력하세요 : ");
                    scanf("%s", ap_data->name); 
                    break;
                case 2:
                    printf("변경할 국어 성적을 입력하세요 : ");
                    scanf("%u", &ap_data->kor); 
                    break;
                case 3:
                    printf("변경할 영어 성적을 입력하세요 : ");
                    scanf("%u", &ap_data->eng); 
                    break;
                case 4:
                    printf("변경할 수학 성적을 입력하세요 : ");
                    scanf("%u", &ap_data->math); 
                    break;
            }
            if (select >= 2 && select <= 4) {  
                ap_data->total = ap_data->kor + ap_data->eng + ap_data->math;
            }
            return;
        }
    }
    printf("[오류] 입력한 학번의 학생이 없습니다.\n");
}
 
void SaveData(const char *ap_file_name, S_DATA *ap_data, int a_count)
{
    FILE *p_file = NULL; 
    if (0 != fopen(ap_file_name, "w")) {
       
        fprintf(p_file, "학번,이름,국어,영어,수학\n");
   
        for (unsigned int i = 0; i < a_count; i++, ap_data++) {
            fprintf(p_file, "%d,%s,%u,%u,%u\n", ap_data->num, ap_data->name, 
                            ap_data->kor, ap_data->eng, ap_data->math);
        }
        printf("%s 파일에 데이터를 저장했습니다!\n", ap_file_name);
        fclose(p_file); 
    }
}
void menu()
{
    S_DATA *p_data = NULL; 
    int data_count = 0, select = 0;  
 
   
    if (ReadData("data.csv", &p_data, &data_count)) {
   
        while (select != 4) {
            printf("\n\n==========[  메뉴  ]==========\n"); 
            printf("1.성적 보기\n");
            printf("2.정보 수정\n");
            printf("3.정보 저장\n");
            printf("4.프로그램 종료\n");
 
            printf("선택 : ");
          
            if (1 == scanf("%u", &select)) {
                printf("\n\n");
             
                if (select == 1) ShowData(p_data, data_count);  
                else if (select == 2) ModifyData(p_data, data_count);
                else if (select == 3) SaveData("data.csv", p_data, data_count);          
            } else {
               
                printf("\n잘못된 값을 입력했습니다. 다시 입력하세요.\n\n");
                rewind(stdin);
            }
        }
       
        if (p_data != NULL) free(p_data);
    }
	else {
        printf("data.csv 파일을 열수 없습니다. 파일 경로를 확인하세요!!\n");
    }

} 
int main()
{
	menu(); 
    return 0;
}
```
 ***
 **CLASS를 활용하였기 때문에 어려움이 많았으나 그래도 제대로 작동하는 것을 보니 뿌듯함이 있었다.**
 *어려웠던점은 Segmentation fault: 11 오류가 나서 몇시간 고생해서 결국 맥북의 문제였다고 한다.*
###~~하지만 난이도는 쉽기 때문에 누구나 어렵지 않은 코드이다.~~
**아래는 예시 **


<img width="536" alt="스크린샷 2021-12-07 오전 11 29 25" src="https://user-images.githubusercontent.com/92897127/144967749-1e4376d8-b7b0-48d3-9f7e-01c2bde9d36e.png">
<img width="532" alt="스크린샷 2021-12-07 오전 11 29 08" src="https://user-images.githubusercontent.com/92897127/144967870-71c1dcb2-ad92-4ba6-bd10-a1fed4f1fbc7.png">
