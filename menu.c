#include <stdio.h>
#include "util.h"
#include "menu.h"
#include "usersManage.h"

void displayLoginOrRegisterMenu() {
    printf("\n-----------------------------\n");
    printf("| У԰��ݹ���ϵͳ         |\n");
    printf("-----------------------------\n");
    printf("| 1. ��¼                  |\n");
    printf("| 2. ע��                  |\n");
    printf("| 3. �˳�ϵͳ              |\n");
    printf("-----------------------------\n");
	printf("����������ѡ��: ");     
}

void registerMenu(HashTable* ht) {
    char username[50], password[50];
    Role role;
    printf("��ѡ���ɫ (1 - ѧ��, 2 - ���Ա, 3 - ����Ա): ");
    
	int roleChoice = getIntChoice();    
    if (roleChoice >= 1 && roleChoice <= 3) {
	    printf("�������û���: ");
	    scanf("%s", username);
	    printf("����������: ");
	    scanf("%s", password);
        role = (Role)(roleChoice - 1);
        registerUser(ht, username, password, role);
    } else {
        printf("��Ч��ѡ�����������롣\n");
    }
}

void processUserLogin(HashTable *ht) {
    char username[50], password[50];
    printf("�������û���: ");
    scanf("%s", username);
    printf("����������: ");
    scanf("%s", password);
    loginUser(ht, username, password);
    currentUser = getCurrentUser();
}

void displayMenu(User* currentUser) {
    printf("\n-----------------------------\n");
    printf("| У԰��ݹ���ϵͳ         |\n");
    printf("-----------------------------\n");

    if (currentUser == NULL) {
        printf("| ���ȵ�¼��ע��          |\n");
    } else {
        switch(currentUser->role) {
            case STUDENT: // ѧ������
                printf("| 1. ��ѯ�ҵĿ����Ϣ      |\n");
                printf("| 2. �޸ĸ�����Ϣ          |\n");
                printf("| 3. �˳���¼              |\n"); 
                break;
            case COURIER: // ���Ա����
                printf("| 1. ��������Ϣ          |\n");
                printf("| 2. �ּ���洢����        |\n");
                printf("| 3. ���͹���              |\n");
                printf("| 4. �޸ĸ�����Ϣ          |\n");
                printf("| 5. �˳���¼              |\n"); 
                break;
            case ADMIN: // ����Ա����
                printf("| 1. ��������Ϣ          |\n");
                printf("| 2. �ּ���洢����        |\n");
                printf("| 3. ���͹���              |\n");
                printf("| 4. �û�����              |\n");
                printf("| 5. ����ͳ�������        |\n");
                printf("| 6. �޸ĸ�����Ϣ          |\n");
                printf("| 7. �˳���¼              |\n"); 
                break;
        }
    }
    printf("-----------------------------\n");
}

void handleUserInput(SystemContext *context, int choice) {
    User* currentUser = getCurrentUser(); // ��ȡ��ǰ�û���Ϣ
    switch (currentUser->role) {
        case STUDENT:
            switch (choice) {
                case 1: // ��ѯ�û��Լ��Ŀ����Ϣ
                    queryParcelByReceiver(context->parcels, currentUser->username); 
                    break;
                case 2: // �޸ĸ�����Ϣ
                    updateUserInfo(context->ht, currentUser->username, NULL, currentUser->role); 
                    break;
                case 3: // �˳���¼
                    setCurrentUser(NULL); // �����ǰ�û�
                    printf("���ѳɹ��˳���¼��\n");
                    return; // ���ص���ѭ���⣬����¼����
                default:
                    printf("��Ч��ѡ�����������롣\n");
            }
            break;

        case COURIER:
            switch (choice) {
                case 1: // ��������Ϣ
                    manageParcelInfo(context->parcels);
                    break;
                case 2: // �ּ���洢����
					manageSortingAndStorage(context->parcels, context->sortingQueue, context->root, context->lockers); 
                    break;
                case 3: // ���͹���
                    
					if (isPriorityQueueEmpty(&context->pq)) {
                    	printf("pq is emtpy in courier menu up\n");
					} else {
						printf("pq is not empty in courier menu up\n");
					}
                                    
                
                    manageDelivery(context->parcels, context->graph, &context->pq);
                    
                    if (isPriorityQueueEmpty(&context->pq)) {
                    	printf("pq is emtpy in courier menu down\n");
					} else {
						printf("pq is not empty in courier menu down\n");
					}
                    
                    break;
                case 4: // �޸ĸ�����Ϣ
                    updateUserInfo(context->ht, currentUser->username, NULL, currentUser->role); 
                    break;
                case 5: // �˳���¼
                    setCurrentUser(NULL); // �����ǰ�û�
                    printf("���ѳɹ��˳���¼��\n");
                    return; // ���ص���ѭ���⣬����¼����
                default:
                    printf("��Ч��ѡ�����������롣\n");
            }
            break;

        case ADMIN:
            switch (choice) {
                case 1: // ��������Ϣ
                    manageParcelInfo(context->parcels);
                    break;
                case 2: // �ּ���洢����
					manageSortingAndStorage(context->parcels, context->sortingQueue, context->root, context->lockers); 
                    break;
                case 3: // ���͹���
//                    manageDelivery();
                    break;
                case 4: // �û�����
                    manageUsers(context->ht);
                    break;
                case 5: // ����ͳ�������
                    generateStatistics();
                    break;
                case 6: // �޸ĸ�����Ϣ
                     updateUserInfo(context->ht, currentUser->username, NULL, currentUser->role);
                    break;
                case 7: // �˳���¼
                    setCurrentUser(NULL); // �����ǰ�û�
                    printf("���ѳɹ��˳���¼��\n");
                    return; // ���ص���ѭ���⣬����¼����
                default:
                    printf("��Ч��ѡ�����������롣\n");
            }
            break;
    }
}

void manageParcelInfo(Parcels* parcels) {
    printf("\n-----------------------------\n");
    printf("| �����Ϣ����             |\n");
    printf("-----------------------------\n");
    printf("| 1. ¼������Ϣ          |\n");
    printf("| 2. �޸Ŀ����Ϣ          |\n");
    printf("| 3. ɾ�������Ϣ          |\n");
    printf("| 4. ��ѯ�����Ϣ          |\n");
    printf("| 5. ��ά������            |\n");
    printf("| 6. �����ļ�              |\n");
    printf("| 7. �����ļ�              |\n");
    printf("-----------------------------\n");
    int subChoice;
    printf("����������ѡ��: ");
    scanf("%d", &subChoice);
    // ������Ӧ�ĺ��������Ӳ˵�ѡ��
    switch (subChoice) {
        case 1:
            {
                int id;
                char sender[50], receiver[50], address[100];
                printf("��������ID: ");
                scanf("%d", &id);
                printf("������ļ���: ");
                scanf("%s", sender);
                printf("�������ռ���: ");
                scanf("%s", receiver);
                printf("�������ַ: ");
                scanf("%s", address);
                
                addParcelInfo(parcels, id, sender, receiver, address);
            }
            break;
        case 2:
            {
                int id;
                printf("������Ҫ�޸ĵĿ��ID: ");
                scanf("%d", &id);
                updateParcelInfo(parcels, id);
            }
            break;
        case 3:
            {
                int id;
                printf("������Ҫɾ���Ŀ��ID: ");
                scanf("%d", &id);
                deleteParcelInfo(parcels, id);
            }
            break;
        case 4:
            {
                int id;
                printf("������Ҫ��ѯ�Ŀ��ID: ");
                scanf("%d", &id);
                queryParcelInfo(parcels, id);
            }
            break;
        case 5:
            {
                printf("\n-----------------------------\n");
                printf("| ��ѡ�������ֶ�           |\n");
                printf("-----------------------------\n");
                printf("| 1. ��ID����              |\n");
                printf("| 2. ���ļ�������          |\n");
                printf("| 3. ���ռ�������          |\n");
                printf("| 4. ����ַ����            |\n");
                printf("| 5. ��״̬����            |\n");
                printf("-----------------------------\n");
                int Field;
                printf("��������Ҫ������ֶα��: ");
                scanf("%d", &Field);
                sortParcels(parcels, Field - 1); // ����������0��ʼ
            }
                break;
        case 6:
            batchImport(parcels); 
            break;
        case 7:
            batchExport(parcels); 
            break;
        default:
            printf("��Ч��ѡ�����������롣\n");
    }    
}
 
void manageUsers(HashTable* ht) {
    int subChoice;
    
    do {
        printf("\n-----------------------------\n");
        printf("| �û�����                 |\n");
        printf("-----------------------------\n");
        printf("| 1. ά���û���Ϣ          |\n"); // ������ 
        printf("| 2. ɾ���û�              |\n");
        printf("| 3. �г������û�          |\n");
        printf("| 4. ������һ��            |\n");
        printf("-----------------------------\n");
        printf("����������ѡ��: ");
        
        if (scanf("%d", &subChoice) != 1) {
            printf("���벻���Ϲ淶��������������\n");
            while (getchar() != '\n'); // ������뻺����
            continue;
        }

        switch (subChoice) {
            case 1: {
                char username[50], oldPassword[50], newPassword[50];
                char newUsername[50];
                Role newRole;
                printf("�����뵱ǰ�û���: ");
                scanf("%s", username);
                printf("�����������: ");
                scanf("%s", oldPassword);
                printf("������������ (�����򲻸���): ");
                scanf("%s", newPassword);
                printf("�������µ��û��� (�����򲻸���): ");
                scanf("%s", newUsername);
                printf("��ѡ���µĽ�ɫ (1 - ѧ��, 2 - ���Ա, 3 - ����Ա, �������ղ�����): ");
                int roleChoice;
                if (scanf("%d", &roleChoice) == 1 && roleChoice >= 1 && roleChoice <= 3) {
                    newRole = (Role)(roleChoice - 1);
                } else {
                    newRole = STUDENT; // Ĭ��ֵ�����û�ѡ�񲻸��ģ��򱣳�ԭ��
                }
                changePassword(ht, username, oldPassword, *newPassword ? newPassword : oldPassword);
                updateUserInfo(ht, username, *newUsername ? newUsername : NULL, newRole);
                break;
            }
            case 2: {
                char username[50];
                printf("������Ҫɾ�����û���: ");
                scanf("%s", username);
                User* user = findUser(ht, username);
                if (user != NULL) {
                    // Remove the user from the hash table
                    unsigned int index = hash(username);
                    User* current = ht->table[index];
                    User* prev = NULL;

                    while (current != NULL && strcmp(current->username, username) != 0) {
                        prev = current;
                        current = current->next;
                    }

                    if (current == NULL) {
                        printf("δ�ҵ����û���\n");
                    } else {
                        if (prev == NULL) {
                            ht->table[index] = current->next;
                        } else {
                            prev->next = current->next;
                        }
                        free(current);
                        printf("�û���ɾ����\n");
                    }
                } else {
                    printf("δ�ҵ����û���\n");
                }
                break;
            }
            case 3: {
                 for (int i = 0; i < TABLE_SIZE; ++i) {
                    User* current = ht->table[i];
                    while (current != NULL) {
                        printf("�û���: %s, ��ɫ: %s\n", current->username, roleToString(current->role));
                        current = current->next;
                    }
                }
                break;
            }
            case 4:
                printf("���ѳɹ��˳��û�����ģ�顣\n");
                return;
            default:
                printf("��Ч��ѡ�����������롣\n");
        }
    } while (subChoice != 6);
}

void manageSortingAndStorage(Parcels *parcels, Queue sortingQueue, TreeNode* root, LockersList lockers) {
    int subChoice;
    do {
        printf("\n-----------------------------\n");
        printf("| ��ݷּ���洢����       |\n");
        printf("-----------------------------\n");
        printf("| 1. �ּ𲢴洢���        |\n");
        printf("| 2. ������һ��            |\n");
        printf("-----------------------------\n");
        printf("����������ѡ��: ");
        scanf("%d", &subChoice);

        switch (subChoice) {
            case 1:
            	// ��״̬���� 
            	printf("��״̬"); 
            	sortParcels(parcels, 4);
                // �����д��ּ�Ŀ�ݼ�����У���ִ�зּ����
			    movePendingToQueue(parcels, &sortingQueue);
			    // �������Զ����е�Ԫ�أ������ּ�Ŀ�ݣ����յ�ַ���з��࣬���洢�����ṹ��
			    sortParcelsFromQueue(&sortingQueue, &root);               
                printf("����ѳɹ��ּ�\n");
                printf("��״̬"); 
            	sortParcels(parcels, 4);                
                break;
            case 2:
                printf("���ѳɹ��˳���ݷּ���洢����ģ��\n");
                return;
            default:
                printf("��Ч��ѡ�����������롣\n");
        }
    } while (subChoice != 5);
}

void manageDelivery(Parcels *parcels, Graph graph, PriorityQueue* pq) {
    printf("\n-----------------------------\n");
    printf("| ���͹���                 |\n");
    printf("-----------------------------\n");
    printf("| 1. ���Ϳ��              |\n");
    printf("| 2. �Ӽ���ݴ���          |\n");
    printf("| 3. ������һ��            |\n");
    printf("-----------------------------\n");
    int subChoice;
    printf("����������ѡ��: ");
    scanf("%d", &subChoice);

    switch (subChoice) {
        case 1: // ���� 
        	displayAdjacencyGraph(&graph); // չ��У԰�ṹ
			
			if (isPriorityQueueEmpty(pq)) {
				printf("pq is empty befroe diaoyong\n");
			} else {
				printf("empty before diaoyong\n");
			}
            deliverParcels(parcels, &graph, pq); // ���Ϳ�� 
            break;
        case 2: // ��ݼӼ����� 
            {
                int parcelId;
                printf("������Ҫ�Ӽ�����Ŀ��ID: ");
                scanf("%d", &parcelId);
                expediteParcels(parcelId, parcels, &graph,  pq); // ����id�Ӽ���� 
                if (isPriorityQueueEmpty(pq)) {
                	printf("pq is empty in menu\n");
				} else {
					printf("pq is not empty in menu\n");
				}
            }
            break;
        case 3:
            printf("������һ����\n");
            return; // ���߸�����Ҫ�˳�ѭ��
        default:
            printf("��Ч��ѡ�����������롣\n");
            break;
    }
}

void generateStatistics() {
    printf("\n-----------------------------\n");
    printf("| ����ͳ�������           |\n");
    printf("-----------------------------\n");
    printf("| 1. �������ͳ��          |\n");
    printf("| 2. �߷���ͳ��            |\n");
    printf("| 3. ���ɱ���              |\n");
    printf("| 4. ���ݿ��ӻ�            |\n");
    printf("| 5. ��Դ�����Ż�          |\n");
    printf("-----------------------------\n");
    int subChoice;
    printf("����������ѡ��: ");
    scanf("%d", &subChoice);
    // ������Ӧ�ĺ��������Ӳ˵�ѡ��
}
