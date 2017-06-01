#ifndef MANAGE_H_INCLUDED
#define MANAGE_H_INCLUDED

#define Super_Manager_Code 666  //��������Ա����
#define NAME 11  //���ֻ����ǳ������
#define N 12
#define LEN sizeof(struct student)
#define LEN1 sizeof(struct teacher)
typedef struct teacher{//��ʦ��Ϣ
        char name[NAME];
        char nickname[NAME];
        char sex[4];
        long code;
        int student_member;
        int sweet_num;
        struct student *head;
        struct teacher *next;
}teacher, *ptr_teacher;

typedef struct student{//ѧ����Ϣ
        char name[NAME];
        char sex[4];
        long code;
        int glamour;
        int niubility;
        int changeable;
        struct student *next;
}student, *ptr_student;

//ö�ٶ���
typedef enum Status
{
	Success, Error
}Status;

/*�����ļ���������һ������ͬʱ�ڸ��ɹ������Ľ�㴦����һ������ѧԱ������
����󷵻ص�ʦͷ�ڵ㣬ʧ�ܷ���NULL*/
Status create(ptr_teacher *head_teacher);

//ȫ����Ϣ
void infomation(ptr_teacher head);

//�˵���
void menu(int flag, ptr_teacher *head);

//����ʦ����ѧԱ����
void input(ptr_teacher *head);

//���Һ���
int find(ptr_teacher head);

void delete(ptr_teacher *head);

void modify(ptr_teacher head);

void change(ptr_teacher head); 
#endif // MANAGE_H_INCLUDED
int teacher_num = 0, condition;//��ʦ����, condition�����жϷ��ص�ֵ��ѧԱ���ǵ�ʦ��1Ϊ��ʦ��2ΪѧԱ
void save(ptr_teacher* head){
        FILE *fp;
        if((fp = fopen("information.dat", "w")) == NULL){
                printf("\n\n\t\t\t�ļ��򿪴���\n");
                return ;
        }
        ptr_teacher cur = *head, re_teacher = *head;
        ptr_student re_student, cur1;
        int i, j;//��ʱ����
        for(i= 0; i < teacher_num; i++){//��δ��������ʱ��
                if(fwrite(cur, sizeof(teacher)-sizeof(ptr_teacher) - sizeof(ptr_student), 1, fp) != 1)
                        printf("\n\n\t\t\t����ʧ��!(��ʦ)\n");
                if(cur->student_member > 0){//����ʦ��ѧԱ����Ϊ���ʱ�����ļ��ﱣ��ѧԱ
                        cur1 = cur->head;//����ʦ��ѧ��ͷָ�븳ֵ��cur1
                        for(j=0; j < cur->student_member; j++){
                                if(fwrite(cur1, sizeof(student)-sizeof(ptr_student), 1, fp) != 1)
                                        printf("\n\n\t\t����ʧ��!(ѧԱ)\n");
                                re_student = cur1;//���������������
                                cur1 = cur1->next;
                                free(re_student);
                        }
                }
                re_teacher = cur;
                cur = cur->next;
                free(re_teacher);
        }
        *head = NULL;
        fclose(fp);//�ر��ļ�
}
Status create(ptr_teacher *head_teacher){/*�����ļ���������һ������ͬʱ�ڸ��ɹ������Ľ�㴦����һ������ѧԱ������
����󷵻ص�ʦͷ�ڵ㣬ʧ�ܷ���NULL*/
        FILE *fp;
        if((fp = fopen("information.dat", "r")) == NULL){
                printf("\n\n\t\t\t�ļ��򿪴���\n");
                return Error;
        }
        teacher temp;
        ptr_teacher cur, p; //cur����ǰ���ٵĿռ��ָ�룬 p������һ��
        *head_teacher = NULL;
        while(fread(&temp, sizeof(teacher)-2*sizeof(ptr_teacher), 1, fp) == 1){//�����ݶ�ȡ��temp��
                cur=(ptr_teacher)malloc(sizeof(teacher));//���ٵ�ʦ�ռ�
                if(teacher_num == 0)//����һ����ʦʱ���ѵ�ǰ�ռ丳ֵ��ͷָ��
                        *head_teacher = cur;
                else
                        p->next = cur;
                *cur = temp;
                p = cur;
                /*ѧԱ�ĵ���*/
                if(cur->student_member > 0){//����ʦ��ѧԱ����Ϊ���ʱ�����ļ���������ѧԱ
                        ptr_student cur1, p1;//cur1����ǰ���ٵĿռ��ָ�룬 p1������һ��
                        int i;
                        for(i=0; i < cur->student_member; i++){
                                cur1=(ptr_student)malloc(sizeof(student));
                                if(i == 0)
                                        cur->head = cur1;//��ѧԱ��ͷָ�븳����ʦ��ѧԱָ����
                                else
                                        p1->next = cur1;
                                if((fread(cur1, sizeof(student)-sizeof(ptr_student), 1, fp)) != 1){
                                        printf("\n\n\t\t\t�ļ���ȡ����(ѧԱ)��\n");
                                        return Error;
                                }
                                p1 = cur1;
                        }
                        p1->next = NULL;
                }
                else
                        cur->head = NULL;//ѧԱΪ��,��ʦ��ѧԱͷָ��ͷſ�ָ��

                teacher_num ++;///ѭ����������ʦ������һ
        }
        if(teacher_num > 0)//����Ƿ����˵�ʦ
                p->next = NULL;

        fclose(fp);//�ر��ļ�

        return Success;
}

int login(){//��½����
        void gotoxy(HANDLE hOut, int x, int y){//��λ���
                COORD pos;
                pos.X = x;             //������
                pos.Y = y;            //������
                SetConsoleCursorPosition(hOut, pos);
        }
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//������ʾ���������
        int secret_code, i;

        printf("\n\n\t\t\t ______________________________________\n");
        printf("\t\t\t|                                      |\n");
        printf("\t\t\t|                                      |\n");
        printf("\t\t\t|  ���������Ա���룬�������ѧԱϵͳ  |\n");
        printf("\t\t\t|            ���룺___                 |\n");
        printf("\t\t\t|______________________________________|\n");
        gotoxy(hOut,43,6);
        scanf("%d", &secret_code);
        fflush(stdin);

        printf("\n\t\t\tLoding��");//���붯��
        for(i=0; i<11; i++){
                Sleep(50);
                printf("�� ");
        }
        printf("\n\t\t\t������ɣ�������һ��ϵͳ");
        Sleep(1000);
        system("cls");//����

        if(secret_code == Super_Manager_Code)//�ж��ǲ��ǹ���Ա
                return 1;
        else
                return 0;
}
void menu(int flag, ptr_teacher *head){//�˵�
        int choice;
        if(flag){
                do{
                        printf("\n\n\t\t\t ______________________________________________\n");
                        printf("\t\t\t|                 ��������Աģʽ               |\n");
                        printf("\t\t\t|                                              |\n");
                        printf("\t\t\t|   1.��ʦ/ѧԱ��Ϣ      2.¼�뵼ʦ/ѧԱ��Ϣ   |\n");
                        printf("\t\t\t|                                              |\n");
                        printf("\t\t\t|   3.ɾ����ʦ/ѧԱ��Ϣ  4.�޸ĵ�ʦ/ѧԱ��Ϣ   |\n");
                        printf("\t\t\t|                                              |\n");
                        printf("\t\t\t|   5.���ҵ�ʦ/ѧԱ��Ϣ  6.����ѧԱ            |\n");
                        printf("\t\t\t|                                              |\n");
                        printf("\t\t\t|   7.�˳�ϵͳ                                 |\n");
                        printf("\t\t\t|______________________________________________|\n");
                        printf("\t\t\t��ѡ���ܣ�����1-6����һ�����֣�:");
                        scanf("%d", &choice);//���������
                        fflush(stdin);//��ջ�������
                        if(choice >= 0 && choice<=7){
                                system("cls");//�Ѳ˵����
                                switch(choice){
                                        case 1:
                                                infomation(*head);
                                                break;
                                        case 2:
                                                input(head);
                                                break;
                                        case 3:
                                                delete(head);
                                                break;
                                        case 4:
                                                modify(*head);
                                                break;
                                        case 5:
                                                find(*head);
                                                break;
                                        case 6:
												change(*head);
										 		break;
                                        default:;
                                }
                        }
                        else{
                                printf("\t\t\t���������룡Please input again!");
                                Sleep(500);
                        }
                        system("cls");
                }while(choice != 7);
        }
        else{
                do{
                        printf("\n\n\t\t\t ________________________________________\n");
                        printf("\t\t\t|                ѧԱģʽ                |\n");
                        printf("\t\t\t|                                        |\n");
                        printf("\t\t\t|  1.ѧԱ/��ʦ��Ϣ   2.����ѧԱ/��ʦ��Ϣ |\n");
                        printf("\t\t\t|                                        |\n");
                        printf("\t\t\t|               3.�˳�ϵͳ               |\n");
                        printf("\t\t\t|________________________________________|\n");
                        printf("\t\t\t��ѡ���ܣ�����1-3����һ�����֣�:");

                        scanf("%d", &choice);//���������
                        fflush(stdin);//��ջ�������
                        if(choice >= 0 && choice<=3){
                                system("cls");//�Ѳ˵����
                                switch(choice){
                                        case 1:
                                                infomation(*head);
                                                break;
                                        case 2:
                                                find(*head);
                                        default:;
                                }
                        }
                        else{
                                printf("\t\t\t���������룡Please input again!");
                                Sleep(500);
                        }
                        system("cls");
                }while(choice != 3);
        }
}
void infomation(ptr_teacher head){//ȫ����Ϣ
        printf("\n\n\t\t\t��ʦ����ѧԱ��Ϣ\n\n");
        if(teacher_num == 0)//�ж����޵�ʦ��Ϣ
                printf("\t\t\t�޵�ʦ��Ϣ��\n");
        else{
                printf("\t\t\t��������������������\n");
                ptr_teacher cur = head;
                while(cur != NULL){
                        printf("\t\t\t������%14s\n\t\t\t�ǳƣ�%14s\n\t\t\t�Ա�%14s\n\t\t\t��ʦ��ţ�%10ld\n\t\t\tѧԱ������%10d\n\t\t\t�ǹ�����%10d\n",
                        cur->name, cur->nickname, cur->sex, cur->code, cur->student_member,cur->sweet_num);
                        printf("\t\t\t************\n");
                        printf("\t\t\tѧԱ��Ϣ\n");
                        if(cur->student_member > 0){
                                ptr_student cur1 = cur->head;
                                while(cur1){
                                        printf("\t\t\t������%14s\n\t\t\t�Ա�%14s\n\t\t\tѧԱ��ţ�%10ld\n\t\t\t����ֵ��%12d\n\t\t\tװ��ֵ��%12d\n\t\t\t�ܷ񽻻���%10d\n",
                                        cur1->name, cur1->sex, cur1->code, cur1->glamour, cur1->niubility,cur1->changeable);
                                        cur1 = cur1->next;
                                        if(cur1)
                                                putchar('\n');//�����һ����Ϊ�գ���ӡ����
                                }
                        }
                        else
                                printf("\t\t\t�˵�ʦ��ѧԱ\n");
                        printf("\t\t\t��������������������\n");
                        cur = cur->next;
                }
        }
        printf("\t\t\t�����������");
        getch();
}
void input(ptr_teacher *head){//�����Ϣ
        int choice, flag=0;
        void gotoxy(HANDLE hOut, int x, int y){//��λ���
                COORD pos;
                pos.X = x;             //������
                pos.Y = y;            //������
                SetConsoleCursorPosition(hOut, pos);
        }
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//������ʾ���������
        printf("\n\n\t\t\t1.��ӵ�ʦ 2.���ѧԱ\n\n");
        printf("\t\t\t��ѡ��(1-2): ");
        scanf("%d", &choice);
        fflush(stdin);
        system("cls");
        if(choice == 1){
                ptr_teacher cur, p, head_teacher = *head, *head_save = head;//cur����ǰ���ٵĿռ��ָ�룬 p������һ��
                while(head_teacher != NULL){//�ص��������β��
                        head_save = &(head_teacher->next);
                        head_teacher = head_teacher->next;
                }
                while(1){
                        cur=(ptr_teacher)malloc(sizeof(teacher));//���ٵ�ʦ�ռ�
                        if(flag == 0){//����һ����ʦʱ���ѵ�ǰ�ռ丳ֵ��ͷָ��
                                *head_save = cur;
                                flag++;
                        }
                        else
                                p->next = cur;
                        printf("\n\n\t\t\t��ǰ��ʦ����%d\n", teacher_num);
                        printf("\t\t\t�밴������Ŀ��������\n");
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t  ����  ��\n\t\t\t  �ǳ�  ��\n\t\t\t  �Ա�  ��\n\t\t\t��ʦ��ţ�\n\t\t\t�ǹ���:\n");
                        printf("\t\t\t______________________\n");
                        gotoxy(hOut, 38, 5); gets(cur->name);
                        gotoxy(hOut, 38, 6); gets(cur->nickname);
                        gotoxy(hOut, 38, 7); gets(cur->sex);
                        gotoxy(hOut, 38, 8); scanf("%ld", &cur->code);
						gotoxy(hOut, 38, 9); scanf("%d", &cur->sweet_num); fflush(stdin);
                        teacher_num++;
                        cur->student_member = 0;

                        printf("\n\t\t\t�����Ƿ�Ϊ�����ѧԱ��(y/n)");
                        choice = getchar();
                        system("cls");fflush(stdin);
                        if(choice == 'Y' || choice == 'y'){
                                ptr_student cur1, p1;
                                while(1){
                                        cur1 = (ptr_student)malloc(sizeof(student));//����ѧԱ�ռ�
                                        if(cur->student_member == 0)
                                                cur->head = cur1;//��ʦ��ѧԱ��
                                        else
                                                p1->next = cur1;
                                        p1 = cur1;
                                        printf("\n\n\t\t\t��ǰѧԱ����%d\n", cur->student_member);
                                        printf("\t\t\t�밴������Ŀ��������\n");
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  ����  ��\n\t\t\t  �Ա�  ��\n\t\t\tѧԱ��ţ�\n\t\t\t ����ֵ ��\n\t\t\t װ��ֵ ��\n\t\t\t �ܷ񽻻���");
                                        printf("\t\t\t______________________\n");
                                        gotoxy(hOut, 38, 5); gets(cur1->name);
                                        gotoxy(hOut, 38, 6); gets(cur1->sex);
                                        gotoxy(hOut, 38, 7); scanf("%ld", &cur1->code);
                                        gotoxy(hOut, 38, 8); scanf("%d", &cur1->glamour);
                                        gotoxy(hOut, 38, 9); scanf("%d", &cur1->niubility);
										gotoxy(hOut, 38, 10); scanf("%d", &cur1->changeable); fflush(stdin);

                                        cur->student_member++;//ѧԱ��������һ
                                        printf("\n\t\t\t�����Ƿ񻹼�����ӣ�(y/n):");
                                        choice = getchar();
                                        system("cls");fflush(stdin);//�����建��
                                        if(choice =='N'||choice =='n')
                                                break;//�˳������������
                                }
                                p1->next = NULL;
                        }
                        else
                                cur->head =NULL;

                        p = cur;//�ƶ�βָ�뵽��һλ
                        printf("\n\n\t\t\t�����Ƿ񻹼�����ӵ�ʦ��(y/n):");
                        choice = getchar();
                        system("cls");fflush(stdin);//�����建��
                        if(choice =='N'||choice =='n')
                        break;//�˳������������
                }
                p->next = NULL;
        }
        else if(choice == 2){//ͨ��ѡ��ķ�ʽ
                int i, mark;
                printf("\n\n\t\t\t���Ȳ���Ҫ���ѧԱ�ĵ�ʦ,�������������һ��");
                getch();
                mark = find(*head);
                system("cls");
                if(mark != -1){
                        ptr_teacher cur = *head;
                        ptr_student cur1, p1, *head_save, head_student;
                        for(i=0; i<mark; i++)//���������뵼ʦ��ͷ�ڵ�λ�á�
                                cur = cur->next;
                        head_save = &(cur->head);
                        head_student = cur->head;
                        while(head_student){//�ص��������β��
                                head_save = &(head_student->next);
                                head_student = head_student->next;
                        }
                        while(1){
                                        cur1 = (ptr_student)malloc(sizeof(student));//����ѧԱ�ռ�
                                        if(flag == 0){//����һ����ʦʱ���ѵ�ǰ�ռ丳ֵ��ͷָ��
                                                *head_save = cur1;
                                                flag++;
                                        }
                                        else
                                                p1->next = cur1;
                                        p1 = cur1;
                                        printf("\n\n\t\t\t��ǰ�˵�ʦѧԱ����%d\n", cur->student_member);
                                        printf("\t\t\t�밴������Ŀ��������\n");
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  ����  ��\n\t\t\t  �Ա�  ��\n\t\t\tѧԱ��ţ�\n\t\t\t ����ֵ ��\n\t\t\t װ��ֵ ��\n\t\t\t �ܷ񽻻� ��");
                                        printf("\t\t\t______________________\n");fflush(stdin);
                                        gotoxy(hOut, 38, 4); gets(cur1->name);
                                        gotoxy(hOut, 38, 5); gets(cur1->sex);
                                        gotoxy(hOut, 38, 6); scanf("%ld", &cur1->code);
                                        gotoxy(hOut, 38, 7); scanf("%d", &cur1->glamour);
                                        gotoxy(hOut, 38, 8); scanf("%d", &cur1->niubility);
										gotoxy(hOut, 38, 9); scanf("%d", &cur1->changeable); fflush(stdin);

                                        cur->student_member++;//ѧԱ��������һ
                                        printf("\n\t\t\t�����Ƿ񻹼�����ӣ�(y/n):");
                                        choice = getchar();
                                        system("cls");fflush(stdin);//�����建��
                                        if(choice =='N'||choice =='n')
                                                break;//�˳������������
                                }
                        p1->next = NULL;
                }//����else�����find���Ҳ������᷵�ز��Ҳ�����
        }
}
int find(ptr_teacher head){//������Ϣ
        int choice, i, flag=1, mark=-1;//choice ���ڸ�ֵ����ѡ��ı�����i ��ʱ������ flag �ж���û���ҵ��ˣ� mark ����ҵ��˵����������շ���
        ptr_teacher cur = head;
        printf("\n\n\t\t\t1.���ҵ�ʦ 2.����ѧԱ\n\n");
        printf("\t\t\t��ѡ��(1-2): ");
        scanf("%d", &choice);
        if(choice == 1){
                condition = 1;
                printf("\t\t\t______________________\n");
                printf("\t\t\t1.��������  2.��Ų���\n");
                printf("\t\t\t��ѡ��(1-2): ");
                scanf("%d", &choice);
                fflush(stdin);
                if(choice == 1){
                        char temp[NAME];
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t���������֣�");
                        gets(temp);
                        for(i=0; i<teacher_num && flag == 1; i++){
                                if(strcmp(cur->name, temp)==0){
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  ����  ��%12s\n\t\t\t  �ǳ�  ��%12s\n\t\t\t  �Ա�  ��%12s\n\t\t\t��ʦ��ţ�%12ld\n\t\t\tѧԱ������%12d\n\t\t\t�ǹ�������%12d\n",
                                        cur->name, cur->nickname, cur->sex, cur->code, cur->student_member,cur->sweet_num);
                                        printf("\t\t\t______________________\n");
                                        printf("\n\t\t\t�����������");
                                        getch();
                                        flag = 0, mark = i;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t�޴���!\n\t\t\t�����������");
                                getch();
                        }
                }
                else if(choice == 2){
                        long temp_num;
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t�����뵼ʦ��ţ�");
                        scanf("%ld", &temp_num);
                        for(i=0; i<teacher_num && flag == 1; i++){
                                if(temp_num == cur->code){
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  ����  ��%12s\n\t\t\t  �ǳ�  ��%12s\n\t\t\t  �Ա�  ��%12s\n\t\t\t��ʦ��ţ�%12ld\n\t\t\tѧԱ������%12d\n\t\t\t�ǹ�������%12d\n",
                                        cur->name, cur->nickname, cur->sex, cur->code, cur->student_member,cur->sweet_num);
                                        printf("\t\t\t______________________\n");
                                        printf("\n\t\t\t�����������");
                                        getch();
                                        flag = 0, mark = i;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t�޴���!\n\t\t\t�����������");
                                getch();
                        }
                }
                else{
                        printf("\n\n\t\t\t�������\n\t\t\t�����������");
                        getch();
                }
        }
        else if(choice == 2){
                ptr_student cur1;//���ڱ�ʾѧԱ�ڴ��
                condition = 2;
                printf("\t\t\t______________________\n");
                printf("\t\t\t1.��������  2.��Ų���\n");
                printf("\t\t\t��ѡ��(1-2): ");
                scanf("%d", &choice);
                fflush(stdin);
                if(choice == 1){
                        char temp[NAME];//�����м����
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t���������֣�");
                        gets(temp);
                        while(cur!=NULL){
                                cur1 = cur->head;
                                for(i=0; i<cur->student_member && flag == 1; i++){
                                        if(strcmp(cur1->name, temp)==0){
                                                printf("\t\t\t______________________\n");
                                                printf("\t\t\t  ����  ��%12s\n\t\t\t  �Ա�  ��%12s\n\t\t\tѧԱ��ţ�%12ld\n\t\t\t ����ֵ ��%12d\n\t\t\t װ��ֵ ��%12d\n\t\t\t �ܷ񽻻� ��%10d\n",
                                                cur1->name, cur1->sex, cur1->code, cur1->glamour, cur1->niubility,cur1->changeable);
                                                printf("\t\t\t______________________\n");
                                                printf("\n\t\t\t�����������");
                                                getch();
                                                flag = 0, mark = i;
                                        }
                                        cur1 = cur1->next;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t�޴���!\n\t\t\t�����������");
                                getch();
                        }
                }
                else if(choice == 2){
                        long temp_num;
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t������ѧԱ��ţ�");
                        scanf("%ld", &temp_num);
                        while(cur!=NULL){
                                cur1 = cur->head;
                                for(i=0; i<cur->student_member && flag == 1; i++){
                                        if(temp_num == cur1->code){
                                                printf("\t\t\t______________________\n");
                                                printf("\t\t\t  ����  ��%12s\n\t\t\t  �Ա�  ��%12s\n\t\t\tѧԱ��ţ�%12ld\n\t\t\t ����ֵ ��%12d\n\t\t\t װ��ֵ ��%12d\n\t\t\t �ܷ񽻻���%10d\n",
                                                cur1->name, cur1->sex, cur1->code, cur1->glamour, cur1->niubility,cur1->changeable);
                                                printf("\t\t\t______________________\n");
                                                printf("\n\t\t\t�����������");
                                                getch();
                                                flag = 0, mark = i;
                                        }
                                        cur1 = cur1->next;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t�޴���!\n\t\t\t�����������");
                                getch();
                        }
                }
                else{
                        printf("\n\n\t\t\t�������\n\t\t\t�����������");
                        getch();
                }
        }
        else{
                printf("\n\n\t\t\t�������\n\t\t\t\t�����������");
                getch();
        }
        return mark;
}
void modify(ptr_teacher head){//�޸���Ϣ
        int choice, mark, i;
        void gotoxy(HANDLE hOut, int x, int y){//��λ���
                COORD pos;
                pos.X = x;             //������
                pos.Y = y;            //������
                SetConsoleCursorPosition(hOut, pos);
        }
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//������ʾ���������
        printf("\n\n\t\t\t���Ȳ���Ҫ�޸ĵ�ʦ,�ٲ���Ҫ�޸ĵ�ѧԱ���������������");
        getch();
        system("cls");
        mark = find(head);
        system("cls");
        if(mark!=-1){
                ptr_teacher cur = head;
                for(i=0; i<mark; i++)
                        cur=cur->next;
                printf("\n\n\t\t\t�밴������Ŀ��������\n");
                printf("\t\t\t______________________\n");
                printf("\t\t\t������\n\t\t\t�ǳƣ�\n\t\t\t�Ա�\n\t\t\t��ʦ��ţ�\n");
                printf("\t\t\t______________________");fflush(stdin);
                gotoxy(hOut, 38, 4); gets(cur->name);
                gotoxy(hOut, 38, 5); gets(cur->nickname);
                gotoxy(hOut, 38, 6); gets(cur->sex);
                gotoxy(hOut, 38, 7); scanf("%ld", &cur->code);fflush(stdin);
                printf("\t\t\t______________________\n");
                fflush(stdin);
                if(cur->student_member > 0){
                        printf("\t\t\t�����Ƿ���Ҫ�޸Ĵ˵�ʦ��ѧԱ����y/n����");
                        choice = getchar();fflush(stdin);system("cls");
                        if(choice == 'y'|| choice == 'Y'){
                                printf("\n\n\t\t\t�����Ҫ�޸ĵ�ѧԱ�������������");
                                getch();
                                system("cls");
                                mark = find(head);
                                system("cls");
                                if(mark!=-1){
                                        ptr_student cur1 = cur->head;
                                        for(i=0; i<mark; i++)
                                                cur1 = cur1->next;
                                        printf("\n\n\t\t\t�밴������Ŀ��������\n");
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  ����  ��\n\t\t\t  �Ա�  ��\n\t\t\tѧԱ��ţ�\n\t\t\t  ����ֵ��\n\t\t\t  װ��ֵ��\n\t\t\t  �ܷ񽻻���\n");
                                        printf("\t\t\t______________________\n");
                                        fflush(stdin);
                                        gotoxy(hOut, 38, 4); gets(cur1->name);
                                        gotoxy(hOut, 38, 5); gets(cur1->sex);
                                        gotoxy(hOut, 38, 6); scanf("%ld", &cur1->code);
                                        gotoxy(hOut, 38, 7); scanf("%d", &cur1->glamour);
                                        gotoxy(hOut, 38, 8); scanf("%d", &cur1->niubility);
										gotoxy(hOut, 38, 9); scanf("%d", &cur1->changeable); fflush(stdin);
                                        printf("\t\t\t______________________\n");
                                        printf("\n\t\t\t�����������");
                                        getch();
                                }
                        }
                }
                else{
                        printf("\n\t\t\t�����������");
                                getch();
                }
        }//������else��_find���������Ҳ����᷵�ز��Ҳ�����
}
void delete(ptr_teacher *head){//ɾ����Ϣ
        char choice;
        int i, mark;
        printf("\n\n\t\t���Ȳ���Ҫɾ���ĵ�ʦ,�����Ҫɾ����ѧԱ�ĵ�ʦ�������������");
        getch();
        system("cls");
        mark = find(*head);
        if(mark!=-1){
                printf("\n\t\t\t______________________\n");
                printf("\t\t\t��������ɾ����ʦ������ѧԱ����1/2����");
                fflush(stdin);
                choice = getchar();
                if(choice == '1'){
                        ptr_teacher cur = *head, *precur=head;
                        ptr_student cur1, restudent;
                        for(i=0; i<mark; i++){
                                precur = &(cur->next);
                                cur = cur->next;
                        }
                        cur1 = cur->head;
                        while(cur1){//�ͷ�ѧԱ�ռ�
                                restudent = cur1;
                                cur1 = cur1->next;
                                free(restudent);
                        }
                        *precur = cur->next;
                        free(cur);
                        cur = NULL;
                        teacher_num--;
                        printf("\t\t\tɾ���ɹ��������������");
                        getch();
                }
                else if(choice == '2'){
                        ptr_teacher cur = *head;
                        ptr_student cur1,  *precur1;
                        for(i=0; i<mark; i++)//��λ��ʦλ��
                                cur = cur->next;

                        precur1 = &(cur->head);
                        cur1 = cur->head;
                        printf("\n\n\t\t\t�����Ҫɾ����ѧԱ�������������");
                        getch();
                        system("cls");
                        mark = find(*head);
                        for(i=0; i<mark; i++){
                                precur1 = &(cur1->next);
                                cur1 = cur1->next;
                        }
                        *precur1 = cur1->next;
                        free(cur1);
                        cur1 = NULL;
                        cur->student_member--;
                        printf("\n\t\t\tɾ���ɹ��������������");
                        getch();
                }
                else{
                        printf("\t\t\tɾ��ʧ�ܣ������������");
                        getch();
                }
        }//������else��_find���������Ҳ����᷵�ز��Ҳ�����
}

                                                    
ptr_teacher find_daoshi(ptr_teacher head,long num)//���ҵ�ʦ
{
    while(head)
    {
        if(head->code==num)
            return head;
        head=head->next;
    }
    return NULL;
}

ptr_student find_xueyuan(ptr_student head,long num)//����ָ����ʦ�µ�ѧԱ
{
    while(head)
    {
        if(head->code==num)
            return head;
        head=head->next;
    }
    return NULL;
}

ptr_student change_in(ptr_student head,student c)//����ʽ����ѧԱ
{
    ptr_student p0,p1,p2;
    p2=p1=head;
    p0=(ptr_student)malloc(LEN);
    *p0=c;
    while(p1 && (p0->code < p1->code))
    {
        p2=p1;
        p1=p1->next;
    }
    if(p1==head)
        head=p0;
    else p2->next=p0;
    p0->next=p1;
    p1=p2=head;
    return head;
}

ptr_student change_del(ptr_student head,long code)//����ʱɾ��ѧԱ
{
    ptr_student p1,p2;
    while(head)
    {
        p1=head;
        while(p1 && code!=p1->code)//-----------
        {
            p2=p1;
            p1=p1->next;
        }
        if(p1)
        {
            if(p1==head) head=p1->next;
            else p2->next=p1->next;
            free(p1);
            return head;
        }
        else
            printf("%ld not been found!\n",code);
    }
    //return head;
}

void change(ptr_teacher head)
{
    long dnum1,dnum2,xnum1,xnum2;
    ptr_student xh=NULL;
    ptr_teacher dh=NULL;
    ptr_student xh2=NULL;
    ptr_teacher dh2=NULL;
    int i;
	ptr_teacher cur = head;
	ptr_student cur1;
    printf ("\t\t\t�ɹ�������ѧԱ���£�\n");
    while(cur!=NULL){
               cur1 = cur->head;
                   for(i=0; i<cur->student_member ; i++){
                         if(1 == cur1->changeable){
                           printf("\t\t\t______________________\n");
                           printf("\t\t\t  ����  ��%12s\n\t\t\t  �Ա�  ��%12s\n\t\t\tѧԱ��ţ�%12ld\n\t\t\t ����ֵ ��%12d\n\t\t\t װ��ֵ ��%12d\n\t\t\t �ܷ񽻻���%10d\n",
                           cur1->name, cur1->sex, cur1->code, cur1->glamour, cur1->niubility,cur1->changeable);
                           printf("\t\t\t______________________\n");                          
                       }
                           cur1 = cur1->next;
                }
                           cur = cur->next;
                        }
                        printf("\n\t\t\t�����������\n");
                           getch();
                           
    printf("\t\t���뽻���ĵ�ʦ�ź������½�����ѧԱ��ţ�\n");
    scanf("%ld %ld",&dnum1,&xnum1);
    dh=find_daoshi(head,dnum1);//�����Ƿ���ڵ�ʦ
    if(!dh)
    {
        printf("%ld�ŵ�ʦ�����ڣ�\n",dnum1);
        return ;
    }
    xh=find_xueyuan(dh->head,xnum1);//�õ�ʦѧԱ�Ƿ����
    if(!xh)
    {
        printf("%ld�ŵ�ʦ��%ld��ѧԱ�����ڣ�\n",dnum1,xnum1);
        return;
    }
    printf("�����뽻���ĵ�ʦ�ź������½�����ѧԱ��\n");
    scanf("%ld %ld",&dnum2,&xnum2);
    if(dnum1==dnum2)
    {
        printf("��ʦ����ͬ\n");
        return;
    }
    dh2=find_daoshi(head,dnum2);//�����Ƿ���ڵ�ʦ
    if(!dh2)
    {
        printf("%ld�ŵ�ʦ�����ڣ�\n",dnum2);
        return ;
    }
    xh2=find_xueyuan(dh2->head,xnum2);//�õ�ʦѧԱ�Ƿ����
    if(!xh2)
    {
        printf("%ld�ŵ�ʦ��%ld��ѧԱ�����ڣ�\n",dnum2,xnum2);
        return;
    }
    if(xh->changeable==0)
    {
        printf("%ld�ŵ�ʦ��%ld��ѧԱ���ܽ�����\n",dnum1,xnum1);
        return;
    }
    if(xh2->changeable==0)
    {
        printf("%ld�ŵ�ʦ������㽻��������������һֻDOG��\n",dnum2);
        return;
    }
    if(dh->sweet_num<3)
    {
        printf("%ld�ŵ�ʦ:����������������\n",dnum2);
        return;
    }
    dh->sweet_num-=3;
    dh2->sweet_num+=3;//�ǹ��������
    //��ʼ����
    dh->head=change_in(dh->head,*xh2);//�ֱ����
    dh2->head=change_in(dh2->head,*xh);
    dh->head=change_del(dh->head,xh->code);//�ٷֱ�ɾ��
    dh2->head=change_del(dh2->head,xh2->code);
    printf("������죡\n");//�ɹ�
    printf("\t\t\t�����������");
                        getch();
}


