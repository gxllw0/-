#include <stdio.h>
#include "system.h"
#include "menu.h"
#include "util.h"

int main() {
	// 初始化系统
    SystemContext* systemContext = initializeSystem();
    if (!systemContext) {
        printf("初始化失败\n");
        return 1;
    } 
    
    while (1) {
		// 用户登录或注册
	    while (currentUser == NULL) {
	        displayLoginOrRegisterMenu();
	        int choice = getIntChoice();
	        switch (choice) {
	            case 1: // 登录
	                processUserLogin(systemContext->ht);
	                break;
	            case 2: // 注册
	                registerMenu(systemContext->ht);
	                break;
	            case 3: // 退出系统
	                cleanupSystem(systemContext);
	                printf("退出系统，欢迎下次使用\n");
	                return 0;
	            default:
	                printf("无效的选择，请重新输入。\n");
	        }
		}
		// 显示主菜单并处理用户输入
	    while (currentUser != NULL) { // 当用户没有选择退出时，循环菜单 
	        displayMenu(currentUser); // 根据角色生成菜单
	        int choice = getIntChoice();
	        handleUserInput(systemContext, choice);
	    }
	}
	
    cleanupSystem(systemContext);
    printf("退出系统，欢迎下次使用\n");
    return 0;
}
