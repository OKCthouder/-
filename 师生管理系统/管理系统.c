#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "����ϵͳ.h"
#define Super_Manager_Code 666  //��������Ա����
int main(void){
        ptr_teacher head_teacher;//��ʦͷָ��
        if(create(&head_teacher) == Error)
                return;
        menu(login(), &head_teacher);//����˵�
        save(&head_teacher);

        return 0;
}
