#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "管理系统.h"
#define Super_Manager_Code 666  //超级管理员密码
int main(void){
        ptr_teacher head_teacher;//导师头指针
        if(create(&head_teacher) == Error)
                return;
        menu(login(), &head_teacher);//进入菜单
        save(&head_teacher);

        return 0;
}
