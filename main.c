#include <stdio.h>
#include "system.h"
#include "menu.h"
#include "util.h"

int main() {
	// ��ʼ��ϵͳ
    SystemContext* systemContext = initializeSystem();
    if (!systemContext) {
        printf("��ʼ��ʧ��\n");
        return 1;
    } 
    
    while (1) {
		// �û���¼��ע��
	    while (currentUser == NULL) {
	        displayLoginOrRegisterMenu();
	        int choice = getIntChoice();
	        switch (choice) {
	            case 1: // ��¼
	                processUserLogin(systemContext->ht);
	                break;
	            case 2: // ע��
	                registerMenu(systemContext->ht);
	                break;
	            case 3: // �˳�ϵͳ
	                cleanupSystem(systemContext);
	                printf("�˳�ϵͳ����ӭ�´�ʹ��\n");
	                return 0;
	            default:
	                printf("��Ч��ѡ�����������롣\n");
	        }
		}
		// ��ʾ���˵��������û�����
	    while (currentUser != NULL) { // ���û�û��ѡ���˳�ʱ��ѭ���˵� 
	        displayMenu(currentUser); // ���ݽ�ɫ���ɲ˵�
	        int choice = getIntChoice();
	        handleUserInput(systemContext, choice);
	    }
	}
	
    cleanupSystem(systemContext);
    printf("�˳�ϵͳ����ӭ�´�ʹ��\n");
    return 0;
}
