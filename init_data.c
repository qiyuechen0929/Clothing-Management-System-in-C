#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 定义服装信息的数据结构
typedef struct {
    char brand[20];     // 品牌
    char category[20];  // 品类
    float price;        // 价格
    int sales;          // 销售量
    float cost;         // 成本
} Clothing;

// 保存服装信息到文件
void saveClothingToFile(Clothing* clothes, int count) {
    FILE* file = fopen("clothing.dat", "wb");
    if (file == NULL) {
        printf("无法打开文件!\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(clothes, sizeof(Clothing), count, file);
    
    fclose(file);
    printf("初始数据已保存到文件!\n");
}

int main() {
    srand(time(NULL));  // 初始化随机数生成器
    
    const int count = 30;
    Clothing clothes[count];
    
    // 定义品牌和品类
    char brands[3][20] = {"NIKE", "ADIDAS", "PUMA"};
    char categories[3][20] = {"短袖", "长裤", "运动鞋"};
    
    // 为每个品牌和品类生成10个服装数据
    int index = 0;
    for (int i = 0; i < 3; i++) {  // 3个品牌
        for (int j = 0; j < 3; j++) {  // 3个品类
            for (int k = 0; k < 10; k++) {  // 每个品牌每个品类10个款式
                strcpy(clothes[index].brand, brands[i]);
                strcpy(clothes[index].category, categories[j]);
                
                // 根据品类设置合理的价格和成本范围
                float basePrice, baseCost;
                if (strcmp(categories[j], "短袖") == 0) {
                    basePrice = 100.0 + rand() % 200;
                    baseCost = basePrice * 0.25 + (rand() % 10);
                } else if (strcmp(categories[j], "长裤") == 0) {
                    basePrice = 200.0 + rand() % 250;
                    baseCost = basePrice * 0.30 + (rand() % 15);
                } else if (strcmp(categories[j], "运动鞋") == 0) {
                    basePrice = 300.0 + rand() % 300;
                    baseCost = basePrice * 0.35 + (rand() % 20);
                }
                
                clothes[index].price = basePrice;
                clothes[index].cost = baseCost;
                
                // 销售量在100-5000之间
                clothes[index].sales = 100 + rand() % 4901;
                
                index++;
            }
        }
    }
    
    // 保存到文件
    saveClothingToFile(clothes, count);
    
    // 显示生成的数据
    printf("生成的服装数据:\n");
    printf("编号 品牌-品类-价格-销售量-成本\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s-%s-%.2f-%d-%.2f\n", 
               i, clothes[i].brand, clothes[i].category, 
               clothes[i].price, clothes[i].sales, clothes[i].cost);
    }
    
    return 0;
}