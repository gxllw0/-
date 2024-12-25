#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "menu.h"
#include "usersManage.h"

void displayLoginOrRegisterMenu() {
    printf("\n-----------------------------\n");
    printf("| 校园快递管理系统         |\n");
    printf("-----------------------------\n");
    printf("| 1. 登录                  |\n");
    printf("| 2. 注册                  |\n");
    printf("| 3. 退出系统              |\n");
    printf("-----------------------------\n");
	printf("请输入您的选择: ");     
}

void registerMenu(HashTable* ht) {
    char username[50], password[50];
    Role role;
    printf("请选择角色 (1 - 学生, 2 - 快递员, 3 - 管理员): ");
    
	int roleChoice = getIntChoice();    
    if (roleChoice >= 1 && roleChoice <= 3) {
	    printf("请输入用户名: ");
	    scanf("%s", username);
	    printf("请输入密码: ");
	    scanf("%s", password);
        role = (Role)(roleChoice - 1);
        registerUser(ht, username, password, role);
    } else {
        printf("无效的选择，请重新输入。\n");
    }
}

void processUserLogin(HashTable *ht) {
    char username[50], password[50];
    printf("请输入用户名: ");
    scanf("%s", username);
    printf("请输入密码: ");
    scanf("%s", password);
    loginUser(ht, username, password);
    currentUser = getCurrentUser();
}

void displayMenu(User* currentUser) {
    printf("\n-----------------------------\n");
    printf("| 校园快递管理系统         |\n");
    printf("-----------------------------\n");

    if (currentUser == NULL) {
        printf("| 请先登录或注册          |\n");
    } else {
        switch(currentUser->role) {
            case STUDENT: // 学生界面
                printf("| 1. 查询我的快递信息      |\n");
                printf("| 2. 修改个人信息          |\n");
                printf("| 3. 退出登录              |\n"); 
                break;
            case COURIER: // 快递员界面
                printf("| 1. 管理快递信息          |\n");
                printf("| 2. 分拣与存储管理        |\n");
                printf("| 3. 配送管理              |\n");
                printf("| 4. 修改个人信息          |\n");
                printf("| 5. 退出登录              |\n"); 
                break;
            case ADMIN: // 管理员界面
                printf("| 1. 管理快递信息          |\n");
                printf("| 2. 分拣与存储管理        |\n");
                printf("| 3. 配送管理              |\n");
                printf("| 4. 用户管理              |\n");
                printf("| 5. 数据统计与分析        |\n");
                printf("| 6. 修改个人信息          |\n");
                printf("| 7. 退出登录              |\n"); 
                break;
        }
    }
    printf("-----------------------------\n");
}

void handleUserInput(SystemContext *context, int choice) {
    User* currentUser = getCurrentUser(); // 获取当前用户信息
    switch (currentUser->role) {
        case STUDENT:
            switch (choice) {
                case 1: // 查询用户自己的快递信息
                    queryParcelByReceiver(context->parcels, currentUser->username); 
                    break;
                case 2: {// 修改个人信息
					char userName[50], oldPassword[50], newPassword[50];
					printf("请输入旧密码: ");
					scanf("%s", oldPassword);
					if (strcmp(currentUser->password, oldPassword) != 0)
					{
						printf("旧密码错误!\n");
						break;
					}
					printf("请输入新密码: ");
					scanf("%s", newPassword);
					changePassword(context->ht, currentUser->username, oldPassword, *newPassword ? newPassword : oldPassword);
					} 
	                break;
                case 3: // 退出登录
                    setCurrentUser(NULL); // 清除当前用户
                    printf("您已成功退出登录。\n");
                    return; // 返回到主循环外，即登录界面
                default:
                    printf("无效的选择，请重新输入。\n");
            	break;
			}
        case COURIER:
            switch (choice) {
                case 1: // 管理快递信息
                    manageParcelInfo(context->parcels);
                    break;
                case 2: // 分拣与存储管理
					manageSortingAndStorage(context->parcels, context->sortingQueue, context->root, context->lockers); 
			    
				
				if (isQueueEmpty(&context->sortingQueue)) {
			    	printf("queue is empty handle down\n");
				} else {
			    	printf("queue is not empty handle down\n");					
				}

                    break;
                case 3: // 配送管理
                    manageDelivery(context->parcels, context->root, context->graph, &context->pq);
                    break;
                case 4: {// 修改个人信息
					char userName[50], oldPassword[50], newPassword[50];
					printf("请输入旧密码: ");
					scanf("%s", oldPassword);
					if (strcmp(currentUser->password, oldPassword) != 0)
					{
						printf("旧密码错误!\n");
						break;
					}
					printf("请输入新密码: ");
					scanf("%s", newPassword);
					changePassword(context->ht, currentUser->username, oldPassword, *newPassword ? newPassword : oldPassword);
                    break;
				}
                case 5: // 退出登录
                    setCurrentUser(NULL); // 清除当前用户
                    printf("您已成功退出登录。\n");
                    return; // 返回到主循环外，即登录界面
                default:
                    printf("无效的选择，请重新输入。\n");
            }
            break;
        case ADMIN:
            switch (choice) {
                case 1: // 管理快递信息
                    manageParcelInfo(context->parcels);
                    break;
                case 2: // 分拣与存储管理
					manageSortingAndStorage(context->parcels, context->sortingQueue, context->root, context->lockers); 
                    break;
                case 3: // 配送管理
//                    manageDelivery();
                    break;
                case 4: // 用户管理
                    manageUsers(context->ht);
                    break;
                case 5: // 数据统计与分析
                    generateStatistics();
                    break;
                case 6: // 修改个人信息
                    {
  						char userName[50], oldPassword[50], newPassword[50];
  						printf("请输入旧密码: ");
  						scanf("%s", oldPassword);
  						if (strcmp(currentUser->password, oldPassword) != 0)
						{
   							printf("旧密码错误!\n");
   							break;
						}
  						printf("请输入新密码: ");
  						scanf("%s", newPassword);
  						changePassword(context->ht, currentUser->username, oldPassword, *newPassword ? newPassword : oldPassword);
					}
                    break;
                case 7: // 退出登录
                    setCurrentUser(NULL); // 清除当前用户
                    printf("您已成功退出登录。\n");
                    return; // 返回到主循环外，即登录界面
                default:
                    printf("无效的选择，请重新输入。\n");
            }
            break;
    }
}

void manageParcelInfo(Parcels* parcels) {
    printf("\n-----------------------------\n");
    printf("| 快递信息管理             |\n");
    printf("-----------------------------\n");
    printf("| 1. 录入快递信息          |\n");
    printf("| 2. 修改快递信息          |\n");
    printf("| 3. 删除快递信息          |\n");
    printf("| 4. 查询快递信息          |\n");
    printf("| 5. 多维度排序            |\n");
    printf("| 6. 导入文件              |\n");
    printf("| 7. 导出文件              |\n");
    printf("| 8. 返回上一级            |\n");
    printf("-----------------------------\n");
    int subChoice;
    printf("请输入您的选择: ");
    scanf("%d", &subChoice);
    // 调用相应的函数处理子菜单选项
    switch (subChoice) {
        case 1:
            {
                int id;
                char sender[50], receiver[50], address[100];
                printf("请输入快递ID: ");
                scanf("%d", &id);
                printf("请输入寄件人: ");
                scanf("%s", sender);
                printf("请输入收件人: ");
                scanf("%s", receiver);
                printf("请输入地址: ");
                scanf("%s", address);
                
                addParcelInfo(parcels, id, sender, receiver, address);
            }
            break;
        case 2:
            {
                int id;
                printf("请输入要修改的快递ID: ");
                scanf("%d", &id);
                updateParcelInfo(parcels, id);
            }
            break;
        case 3:
            {
                int id;
                printf("请输入要删除的快递ID: ");
                scanf("%d", &id);
                deleteParcelInfo(parcels, id);
            }
            break;
        case 4:
            {
                int id;
                printf("请输入要查询的快递ID: ");
                scanf("%d", &id);
                queryParcelInfo(parcels, id);
            }
            break;
        case 5:
            {
                printf("\n-----------------------------\n");
                printf("| 请选择排序字段           |\n");
                printf("-----------------------------\n");
                printf("| 1. 按ID排序              |\n");
                printf("| 2. 按寄件人排序          |\n");
                printf("| 3. 按收件人排序          |\n");
                printf("| 4. 按地址排序            |\n");
                printf("| 5. 按状态排序            |\n");
                printf("-----------------------------\n");
                int Field;
                printf("请输入您要排序的字段编号: ");
                scanf("%d", &Field);
                sortParcels(parcels, Field - 1); // 假设索引从0开始
            }
                break;
        case 6:
            batchImport(parcels); 
            break;
        case 7:
            batchExport(parcels); 
            break;
        case 8:
			break; 
        default:
            printf("无效的选择，请重新输入。\n");
    }    
}
 
void manageUsers(HashTable* ht) {
    int subChoice;
    
    do {
        printf("\n-----------------------------\n");
        printf("| 用户管理                 |\n");
        printf("-----------------------------\n");
        printf("| 1. 维护用户信息          |\n"); // 需重做 
        printf("| 2. 删除用户              |\n");
        printf("| 3. 列出所有用户          |\n");
        printf("| 4. 返回上一级            |\n");
        printf("-----------------------------\n");
        printf("请输入您的选择: ");
        
        if (scanf("%d", &subChoice) != 1) {
            printf("输入不符合规范，请输入整数。\n");
            while (getchar() != '\n'); // 清空输入缓冲区
            continue;
        }

        switch (subChoice) {
            case 1: 
				{
    				char username[50], newPassword[50];
    				Role newRole;
    				printf("请输入用户名: ");
    				scanf("%s", username);
	    			User* user = findUser(ht, username);
    				if(user == NULL)
					{
						printf("没有找到该用户！");
       					break;
	    			}
    				printf("请输入新密码: ");
    				scanf("%s", newPassword);
    				printf("请选择新的角色 (1 - 学生, 2 - 快递员, 3 - 管理员): ");
    				int roleChoice = getIntChoice();
    				if (roleChoice >= 1 && roleChoice <= 3) {
        				newRole = (Role)(roleChoice - 1);
    				} else {
    				    printf("输入无效！");
						break; // 默认值，若用户选择不更改，则保持原样
    				}
	    			changePassword(ht, username, user->password, *newPassword ? newPassword : user->password);
 		   			updateUserInfo(ht, username, newRole);
				}
				break;
            case 2: 
			{
                char username[50];
                printf("请输入要删除的用户名: ");
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
                        printf("未找到该用户。\n");
                    } else {
                        if (prev == NULL) {
                            ht->table[index] = current->next;
                        } else {
                            prev->next = current->next;
                        }
                        free(current);
                        printf("用户已删除。\n");
                    }
                } else {
                    printf("未找到该用户。\n");
                }
                break;
            }
            case 3: 
			{
                 for (int i = 0; i < TABLE_SIZE; ++i) {
                    User* current = ht->table[i];
                    while (current != NULL) {
                        printf("用户名: %s, 角色: %s, 密码: %s\n", current->username, roleToString(current->role),current->password);
                        current = current->next;
                    }
                }
                break;
            }
            case 4:
                printf("您已成功退出用户管理模块。\n");
                return;
            default:
                printf("无效的选择，请重新输入。\n");
        }
    } while (subChoice != 6);
}

void manageSortingAndStorage(Parcels *parcels, Queue sortingQueue, TreeNode* root, LockersList lockers) {
    int subChoice;
    do {
        printf("\n-----------------------------\n");
        printf("| 快递分拣与存储管理       |\n");
        printf("-----------------------------\n");
        printf("| 1. 分拣并存储快递        |\n");
        printf("| 2. 返回上一级            |\n");
        printf("-----------------------------\n");
        printf("请输入您的选择: ");
        scanf("%d", &subChoice);

        switch (subChoice) {
            case 1:
            	// 按状态排序 
            	printf("按状态"); 
            	sortParcels(parcels, 4);
                // 将所有待分拣的快递加入队列，并执行分拣操作
			    movePendingToQueue(parcels, &sortingQueue);
			    
			    if (isQueueEmpty(&sortingQueue)) {
			    	printf("queue is empty after sorting\n");
				} else {
			    	printf("queue is not empty after sorting\n");					
				}
			    
			    // 接下来对队列中的元素（即待分拣的快递）按照地址进行分类，并存储到树结构中
			    sortParcelsFromQueue(&sortingQueue, &root); 
					 
			    if (root == NULL) {
			    	printf("root is empty menu down.\n");
				} else {
			    	printf("root is not empty menu down.\n");		
				}		
			             
                printf("快递已成功分拣。\n");
                printf("按状态"); 
            	sortParcels(parcels, 4);                
                break;
            case 2:
                printf("您已成功退出快递分拣与存储管理模块\n");
                return;
            default:
                printf("无效的选择，请重新输入。\n");
        }
    } while (subChoice != 5);
}

void manageDelivery(Parcels *parcels, TreeNode* root, Graph graph, PriorityQueue* pq) {
    printf("\n-----------------------------\n");
    printf("| 配送管理                 |\n");
    printf("-----------------------------\n");
    printf("| 1. 配送快递              |\n");
    printf("| 2. 加急快递处理          |\n");
    printf("| 3. 返回上一级            |\n");
    printf("-----------------------------\n");
    int subChoice;
    printf("请输入您的选择: ");
    scanf("%d", &subChoice);

    switch (subChoice) {
        case 1: // 配送 
        	displayAdjacencyGraph(&graph); // 展现校园结构
            deliverParcels(parcels, &graph, pq); // 配送快递 
            break;
        case 2: // 快递加急处理 
            {
                int parcelId;
                printf("请输入要加急处理的快递ID: ");
                scanf("%d", &parcelId);
                expediteParcels(parcelId, parcels, &root, &graph,  pq); // 根据id加急快递 
                if (isPriorityQueueEmpty(pq)) {
                	printf("pq is empty in menu\n");
				} else {
					printf("pq is not empty in menu\n");
				}
            }
            break;
        case 3:
            printf("返回上一级。\n");
            return; // 或者根据需要退出循环
        default:
            printf("无效的选择，请重新输入。\n");
            break;
    }
}

void generateStatistics() {
    printf("\n-----------------------------\n");
    printf("| 数据统计与分析           |\n");
    printf("-----------------------------\n");
    printf("| 1. 快递流量统计          |\n");
    printf("| 2. 高峰期统计            |\n");
    printf("| 3. 生成报表              |\n");
    printf("| 4. 数据可视化            |\n");
    printf("| 5. 资源配置优化          |\n");
    printf("-----------------------------\n");
    int subChoice;
    printf("请输入您的选择: ");
    scanf("%d", &subChoice);
    // 调用相应的函数处理子菜单选项
}
