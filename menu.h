#ifndef MENU_H
#define MENU_H

#include "usersManage.h"
#include "system.h"

// 显示登录注册菜单
void displayLoginOrRegisterMenu();
// 注册菜单 
void registerMenu(HashTable* ht);
// 登录
void processUserLogin(HashTable *ht); 
// 根据不同角色生成登录界面 
void displayMenu();
// 处理输入前往对应模块 
void handleUserInput(SystemContext *context, int choice);
// 快递信息管理 
void manageParcelInfo(Parcels* parcels); 
// 用户管理 
void manageUsers(HashTable* ht);
// 快递分拣与存储模块 
void manageSortingAndStorage(Parcels* parcels, Queue sortingQueue, TreeNode* root, LockersList lockers); 
// 快递配送模块 
void manageDelivery(Parcels *parcels, TreeNode* root, Graph graph, PriorityQueue* pq);
// 数据可视化模块 
void generateStatistics();

#endif
