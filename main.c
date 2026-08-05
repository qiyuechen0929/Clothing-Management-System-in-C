#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 定义服装信息的数据结构
typedef struct {
    char brand[20];     // 品牌
    char category[20];  // 品类
    float price;        // 价格
    int sales;          // 销售量
    float cost;         // 成本
} Clothing;

// 定义购物车项结构
typedef struct {
    Clothing* item;     // 指向服装的指针
    int quantity;       // 购买数量
} CartItem;

// 定义购物车结构
typedef struct {
    CartItem* items;    // 购物车中的商品
    int count;          // 购物车中的商品数量
    int capacity;       // 购物车容量
} ShoppingCart;

// 函数声明
void saveClothingToFile(Clothing* clothes, int count);
int loadClothingFromFile(Clothing** clothes);
void addClothing(Clothing** clothes, int* count, int* capacity);
void deleteClothing(Clothing* clothes, int* count);
void searchClothing(Clothing* clothes, int count);
void modifyClothing(Clothing* clothes, int count);
void displayAllClothing(Clothing* clothes, int count);
ShoppingCart* createShoppingCart(int capacity);
void addToCart(ShoppingCart* cart, Clothing* item, int quantity);
void removeFromCart(ShoppingCart* cart, int index);
void displayCart(ShoppingCart* cart);
float calculateTotal(ShoppingCart* cart);
void checkout(ShoppingCart* cart, Clothing* clothes, int count);
void analyzeBrandRevenue(Clothing* clothes, int count);
void kmeansClustering(Clothing* clothes, int count);
void analyzeFactors(Clothing* clothes, int count);

int main() {
    Clothing* clothes = NULL;
    int count = 0;
    int capacity = 0;
    ShoppingCart* cart = createShoppingCart(100);
    int choice;
    
    // 加载服装信息
    count = loadClothingFromFile(&clothes);
    
    if (count == 0) {
        printf("未找到服装信息文件，将创建新文件...\n");
        // 如果文件不存在，创建一些初始数据
        capacity = 50;
        clothes = (Clothing*)malloc(capacity * sizeof(Clothing));
        
        // 添加初始数据（示例）
        strcpy(clothes[0].brand, "NIKE");
        strcpy(clothes[0].category, "短袖");
        clothes[0].price = 198.0;
        clothes[0].sales = 2000;
        clothes[0].cost = 50.0;
        
        strcpy(clothes[1].brand, "NIKE");
        strcpy(clothes[1].category, "长裤");
        clothes[1].price = 298.0;
        clothes[1].sales = 1500;
        clothes[1].cost = 80.0;
        
        strcpy(clothes[2].brand, "NIKE");
        strcpy(clothes[2].category, "运动鞋");
        clothes[2].price = 498.0;
        clothes[2].sales = 3000;
        clothes[2].cost = 150.0;
        
        count = 3;
        saveClothingToFile(clothes, count);
    }
    
    while (1) {
        printf("\n===== 服装管理系统 =====\n");
        printf("1. 显示所有服装信息\n");
        printf("2. 添加服装信息\n");
        printf("3. 删除服装信息\n");
        printf("4. 查找服装信息\n");
        printf("5. 修改服装信息\n");
        printf("6. 购物车管理\n");
        printf("7. 品牌收入分析\n");
        printf("8. K-means聚类分析\n");
        printf("9. 因素影响分析\n");
        printf("0. 退出系统\n");
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                displayAllClothing(clothes, count);
                break;
            case 2:
                addClothing(&clothes, &count, &capacity);
                break;
            case 3:
                deleteClothing(clothes, &count);
                break;
            case 4:
                searchClothing(clothes, count);
                break;
            case 5:
                modifyClothing(clothes, count);
                break;
            case 6:
                // 购物车管理子菜单
                int cartChoice;
                while (1) {
                    printf("\n===== 购物车管理 =====\n");
                    printf("1. 查看购物车\n");
                    printf("2. 添加商品到购物车\n");
                    printf("3. 从购物车删除商品\n");
                    printf("4. 结算\n");
                    printf("5. 返回主菜单\n");
                    printf("请选择操作: ");
                    scanf("%d", &cartChoice);
                    
                    if (cartChoice == 5) break;
                    
                    switch (cartChoice) {
                        case 1:
                            displayCart(cart);
                            break;
                        case 2:
                            // 显示所有服装供选择
                            displayAllClothing(clothes, count);
                            printf("请输入要添加的服装编号: ");
                            int index;
                            scanf("%d", &index);
                            if (index >= 0 && index < count) {
                                printf("请输入购买数量: ");
                                int quantity;
                                scanf("%d", &quantity);
                                addToCart(cart, &clothes[index], quantity);
                            } else {
                                printf("无效的服装编号!\n");
                            }
                            break;
                        case 3:
                            displayCart(cart);
                            printf("请输入要删除的购物车项编号: ");
                            int cartIndex;
                            scanf("%d", &cartIndex);
                            if (cartIndex >= 0 && cartIndex < cart->count) {
                                removeFromCart(cart, cartIndex);
                            } else {
                                printf("无效的购物车项编号!\n");
                            }
                            break;
                        case 4:
                            checkout(cart, clothes, count);
                            break;
                        default:
                            printf("无效的选择!\n");
                    }
                }
                break;
            case 7:
                analyzeBrandRevenue(clothes, count);
                break;
            case 8:
                kmeansClustering(clothes, count);
                break;
            case 9:
                analyzeFactors(clothes, count);
                break;
            case 0:
                // 保存数据并退出
                saveClothingToFile(clothes, count);
                free(clothes);
                free(cart->items);
                free(cart);
                printf("系统已退出，数据已保存!\n");
                return 0;
            default:
                printf("无效的选择!\n");
        }
    }
    
    return 0;
}

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
    printf("数据已保存到文件!\n");
}

// 从文件加载服装信息
int loadClothingFromFile(Clothing** clothes) {
    FILE* file = fopen("clothing.dat", "rb");
    if (file == NULL) {
        return 0;
    }
    
    int count;
    fread(&count, sizeof(int), 1, file);
    
    *clothes = (Clothing*)malloc(count * sizeof(Clothing));
    fread(*clothes, sizeof(Clothing), count, file);
    
    fclose(file);
    return count;
}

// 添加服装信息
void addClothing(Clothing** clothes, int* count, int* capacity) {
    if (*count >= *capacity) {
        *capacity += 10;
        *clothes = (Clothing*)realloc(*clothes, *capacity * sizeof(Clothing));
    }
    
    printf("请输入服装信息:\n");
    printf("品牌: ");
    scanf("%s", (*clothes)[*count].brand);
    printf("品类: ");
    scanf("%s", (*clothes)[*count].category);
    printf("价格: ");
    scanf("%f", &(*clothes)[*count].price);
    printf("销售量: ");
    scanf("%d", &(*clothes)[*count].sales);
    printf("成本: ");
    scanf("%f", &(*clothes)[*count].cost);
    
    (*count)++;
    printf("服装信息已添加!\n");
}

// 删除服装信息
void deleteClothing(Clothing* clothes, int* count) {
    if (*count == 0) {
        printf("没有服装信息可删除!\n");
        return;
    }
    
    displayAllClothing(clothes, *count);
    printf("请输入要删除的服装编号: ");
    int index;
    scanf("%d", &index);
    
    if (index >= 0 && index < *count) {
        // 移动后面的元素覆盖要删除的元素
        for (int i = index; i < *count - 1; i++) {
            clothes[i] = clothes[i + 1];
        }
        (*count)--;
        printf("服装信息已删除!\n");
    } else {
        printf("无效的服装编号!\n");
    }
}

// 查找服装信息
void searchClothing(Clothing* clothes, int count) {
    if (count == 0) {
        printf("没有服装信息可查找!\n");
        return;
    }
    
    int choice;
    printf("按什么条件查找?\n");
    printf("1. 品牌\n2. 品类\n3. 价格范围\n请选择: ");
    scanf("%d", &choice);
    
    int found = 0;
    
    switch (choice) {
        case 1: {
            char brand[20];
            printf("请输入品牌名称: ");
            scanf("%s", brand);
            
            for (int i = 0; i < count; i++) {
                if (strcmp(clothes[i].brand, brand) == 0) {
                    printf("%d. %s-%s-%.2f-%d-%.2f\n", 
                           i, clothes[i].brand, clothes[i].category, 
                           clothes[i].price, clothes[i].sales, clothes[i].cost);
                    found = 1;
                }
            }
            break;
        }
        case 2: {
            char category[20];
            printf("请输入品类名称: ");
            scanf("%s", category);
            
            for (int i = 0; i < count; i++) {
                if (strcmp(clothes[i].category, category) == 0) {
                    printf("%d. %s-%s-%.2f-%d-%.2f\n", 
                           i, clothes[i].brand, clothes[i].category, 
                           clothes[i].price, clothes[i].sales, clothes[i].cost);
                    found = 1;
                }
            }
            break;
        }
        case 3: {
            float min, max;
            printf("请输入价格范围 (min max): ");
            scanf("%f %f", &min, &max);
            
            for (int i = 0; i < count; i++) {
                if (clothes[i].price >= min && clothes[i].price <= max) {
                    printf("%d. %s-%s-%.2f-%d-%.2f\n", 
                           i, clothes[i].brand, clothes[i].category, 
                           clothes[i].price, clothes[i].sales, clothes[i].cost);
                    found = 1;
                }
            }
            break;
        }
        default:
            printf("无效的选择!\n");
            return;
    }
    
    if (!found) {
        printf("未找到匹配的服装信息!\n");
    }
}

// 修改服装信息
void modifyClothing(Clothing* clothes, int count) {
    if (count == 0) {
        printf("没有服装信息可修改!\n");
        return;
    }
    
    displayAllClothing(clothes, count);
    printf("请输入要修改的服装编号: ");
    int index;
    scanf("%d", &index);
    
    if (index >= 0 && index < count) {
        printf("当前服装信息: %s-%s-%.2f-%d-%.2f\n", 
               clothes[index].brand, clothes[index].category, 
               clothes[index].price, clothes[index].sales, clothes[index].cost);
        
        printf("请输入新的服装信息:\n");
        printf("品牌: ");
        scanf("%s", clothes[index].brand);
        printf("品类: ");
        scanf("%s", clothes[index].category);
        printf("价格: ");
        scanf("%f", &clothes[index].price);
        printf("销售量: ");
        scanf("%d", &clothes[index].sales);
        printf("成本: ");
        scanf("%f", &clothes[index].cost);
        
        printf("服装信息已修改!\n");
    } else {
        printf("无效的服装编号!\n");
    }
}

// 显示所有服装信息
void displayAllClothing(Clothing* clothes, int count) {
    if (count == 0) {
        printf("没有服装信息可显示!\n");
        return;
    }
    
    printf("\n所有服装信息:\n");
    printf("编号 品牌-品类-价格-销售量-成本\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s-%s-%.2f-%d-%.2f\n", 
               i, clothes[i].brand, clothes[i].category, 
               clothes[i].price, clothes[i].sales, clothes[i].cost);
    }
}

// 创建购物车
ShoppingCart* createShoppingCart(int capacity) {
    ShoppingCart* cart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
    cart->items = (CartItem*)malloc(capacity * sizeof(CartItem));
    cart->count = 0;
    cart->capacity = capacity;
    return cart;
}

// 添加商品到购物车
void addToCart(ShoppingCart* cart, Clothing* item, int quantity) {
    if (cart->count >= cart->capacity) {
        cart->capacity += 10;
        cart->items = (CartItem*)realloc(cart->items, cart->capacity * sizeof(CartItem));
    }
    
    // 检查购物车中是否已存在该商品
    for (int i = 0; i < cart->count; i++) {
        if (cart->items[i].item == item) {
            cart->items[i].quantity += quantity;
            printf("商品数量已更新!\n");
            return;
        }
    }
    
    cart->items[cart->count].item = item;
    cart->items[cart->count].quantity = quantity;
    cart->count++;
    printf("商品已添加到购物车!\n");
}

// 从购物车删除商品
void removeFromCart(ShoppingCart* cart, int index) {
    if (index < 0 || index >= cart->count) {
        printf("无效的购物车项编号!\n");
        return;
    }
    
    for (int i = index; i < cart->count - 1; i++) {
        cart->items[i] = cart->items[i + 1];
    }
    
    cart->count--;
    printf("商品已从购物车删除!\n");
}

// 显示购物车内容
void displayCart(ShoppingCart* cart) {
    if (cart->count == 0) {
        printf("购物车为空!\n");
        return;
    }
    
    printf("\n购物车内容:\n");
    printf("编号 品牌-品类-价格-数量-小计\n");
    for (int i = 0; i < cart->count; i++) {
        float subtotal = cart->items[i].item->price * cart->items[i].quantity;
        printf("%d. %s-%s-%.2f-%d-%.2f\n", 
               i, cart->items[i].item->brand, cart->items[i].item->category, 
               cart->items[i].item->price, cart->items[i].quantity, subtotal);
    }
    printf("总计: %.2f\n", calculateTotal(cart));
}

// 计算购物车总价
float calculateTotal(ShoppingCart* cart) {
    float total = 0;
    for (int i = 0; i < cart->count; i++) {
        total += cart->items[i].item->price * cart->items[i].quantity;
    }
    return total;
}

// 结算
void checkout(ShoppingCart* cart, Clothing* clothes, int count) {
    if (cart->count == 0) {
        printf("购物车为空，无法结算!\n");
        return;
    }
    
    float total = calculateTotal(cart);
    printf("\n结算信息:\n");
    printf("商品总数: %d\n", cart->count);
    printf("总金额: %.2f\n", total);
    
    // 更新销售量
    for (int i = 0; i < cart->count; i++) {
        cart->items[i].item->sales += cart->items[i].quantity;
    }
    
    // 清空购物车
    cart->count = 0;
    printf("结算完成! 购物车已清空!\n");
}

// 分析品牌收入
void analyzeBrandRevenue(Clothing* clothes, int count) {
    if (count == 0) {
        printf("没有服装信息可分析!\n");
        return;
    }
    
    // 创建品牌列表
    char brands[10][20];
    int brandCount = 0;
    
    for (int i = 0; i < count; i++) {
        int exists = 0;
        for (int j = 0; j < brandCount; j++) {
            if (strcmp(clothes[i].brand, brands[j]) == 0) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            strcpy(brands[brandCount], clothes[i].brand);
            brandCount++;
        }
    }
    
    // 分析每个品牌
    for (int i = 0; i < brandCount; i++) {
        printf("\n品牌: %s\n", brands[i]);
        
        // 创建品类列表
        char categories[10][20];
        int categoryCount = 0;
        
        for (int j = 0; j < count; j++) {
            if (strcmp(clothes[j].brand, brands[i]) == 0) {
                int exists = 0;
                for (int k = 0; k < categoryCount; k++) {
                    if (strcmp(clothes[j].category, categories[k]) == 0) {
                        exists = 1;
                        break;
                    }
                }
                if (!exists) {
                    strcpy(categories[categoryCount], clothes[j].category);
                    categoryCount++;
                }
            }
        }
        
        float totalRevenue = 0;
        float maxProfit = -1;
        char mostProfitable[20];
        
        // 计算每个品类的收入和利润
        for (int j = 0; j < categoryCount; j++) {
            float categoryRevenue = 0;
            float categoryProfit = 0;
            
            for (int k = 0; k < count; k++) {
                if (strcmp(clothes[k].brand, brands[i]) == 0 && 
                    strcmp(clothes[k].category, categories[j]) == 0) {
                    categoryRevenue += clothes[k].price * clothes[k].sales;
                    categoryProfit += (clothes[k].price - clothes[k].cost) * clothes[k].sales;
                }
            }
            
            totalRevenue += categoryRevenue;
            
            // 找出最赚钱的品类
            if (categoryProfit > maxProfit) {
                maxProfit = categoryProfit;
                strcpy(mostProfitable, categories[j]);
            }
        }
        
        // 输出各品类收入占比
        printf("各品类收入占比:\n");
        for (int j = 0; j < categoryCount; j++) {
            float categoryRevenue = 0;
            
            for (int k = 0; k < count; k++) {
                if (strcmp(clothes[k].brand, brands[i]) == 0 && 
                    strcmp(clothes[k].category, categories[j]) == 0) {
                    categoryRevenue += clothes[k].price * clothes[k].sales;
                }
            }
            
            float percentage = (categoryRevenue / totalRevenue) * 100;
            printf("%s: %.2f%%\n", categories[j], percentage);
        }
        
        printf("最赚钱的品类: %s (利润: %.2f)\n", mostProfitable, maxProfit);
    }
}

// K-means聚类分析
void kmeansClustering(Clothing* clothes, int count) {
    if (count < 3) {
        printf("服装数量不足，无法进行聚类分析!\n");
        return;
    }
    
    int k = 3;  // 分成3个类别
    
    // 初始化聚类中心
    float centroids[3][3];
    for (int i = 0; i < k; i++) {
        centroids[i][0] = clothes[i].price;
        centroids[i][1] = clothes[i].sales;
        centroids[i][2] = clothes[i].cost;
    }
    
    int* clusters = (int*)malloc(count * sizeof(int));
    int changed;
    
    // K-means算法迭代
    do {
        changed = 0;
        
        // 分配每个服装到最近的聚类中心
        for (int i = 0; i < count; i++) {
            float minDist = 1e9;
            int newCluster = 0;
            
            for (int j = 0; j < k; j++) {
                float dist = sqrt(
                    pow(clothes[i].price - centroids[j][0], 2) +
                    pow(clothes[i].sales - centroids[j][1], 2) +
                    pow(clothes[i].cost - centroids[j][2], 2)
                );
                
                if (dist < minDist) {
                    minDist = dist;
                    newCluster = j;
                }
            }
            
            if (clusters[i] != newCluster) {
                clusters[i] = newCluster;
                changed = 1;
            }
        }
        
        // 更新聚类中心
        float sum[3][3] = {0};
        int clusterSize[3] = {0};
        
        for (int i = 0; i < count; i++) {
            sum[clusters[i]][0] += clothes[i].price;
            sum[clusters[i]][1] += clothes[i].sales;
            sum[clusters[i]][2] += clothes[i].cost;
            clusterSize[clusters[i]]++;
        }
        
        for (int i = 0; i < k; i++) {
            if (clusterSize[i] > 0) {
                centroids[i][0] = sum[i][0] / clusterSize[i];
                centroids[i][1] = sum[i][1] / clusterSize[i];
                centroids[i][2] = sum[i][2] / clusterSize[i];
            }
        }
        
    } while (changed);
    
    // 计算每个类别的毛利率
    printf("\nK-means聚类分析结果:\n");
    for (int i = 0; i < k; i++) {
        int size = 0;
        float totalPrice = 0, totalCost = 0;
        
        for (int j = 0; j < count; j++) {
            if (clusters[j] == i) {
                size++;
                totalPrice += clothes[j].price;
                totalCost += clothes[j].cost;
            }
        }
        
        float avgGrossMargin = size > 0 ? ((totalPrice - totalCost) / totalPrice) * 100 : 0;
        printf("类别 %d: 服装数量=%d, 平均毛利率=%.2f%%\n", i + 1, size, avgGrossMargin);
    }
    
    free(clusters);
}

// 分析品牌、品类、价格对销量的影响
void analyzeFactors(Clothing* clothes, int count) {
    if (count == 0) {
        printf("没有服装信息可分析!\n");
        return;
    }
    
    printf("\n品牌、品类、价格对销量的影响分析:\n");
    
    // 分析品牌对销量的影响
    printf("\n1. 品牌对销量的影响:\n");
    char brands[10][20];
    int brandCount = 0;
    
    for (int i = 0; i < count; i++) {
        int exists = 0;
        for (int j = 0; j < brandCount; j++) {
            if (strcmp(clothes[i].brand, brands[j]) == 0) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            strcpy(brands[brandCount], clothes[i].brand);
            brandCount++;
        }
    }
    
    for (int i = 0; i < brandCount; i++) {
        int totalSales = 0;
        int itemCount = 0;
        
        for (int j = 0; j < count; j++) {
            if (strcmp(clothes[j].brand, brands[i]) == 0) {
                totalSales += clothes[j].sales;
                itemCount++;
            }
        }
        
        float avgSales = (float)totalSales / itemCount;
        printf("%s: 平均销量=%.2f\n", brands[i], avgSales);
    }
    
    // 分析品类对销量的影响
    printf("\n2. 品类对销量的影响:\n");
    char categories[10][20];
    int categoryCount = 0;
    
    for (int i = 0; i < count; i++) {
        int exists = 0;
        for (int j = 0; j < categoryCount; j++) {
            if (strcmp(clothes[i].category, categories[j]) == 0) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            strcpy(categories[categoryCount], clothes[i].category);
            categoryCount++;
        }
    }
    
    for (int i = 0; i < categoryCount; i++) {
        int totalSales = 0;
        int itemCount = 0;
        
        for (int j = 0; j < count; j++) {
            if (strcmp(clothes[j].category, categories[i]) == 0) {
                totalSales += clothes[j].sales;
                itemCount++;
            }
        }
        
        float avgSales = (float)totalSales / itemCount;
        printf("%s: 平均销量=%.2f\n", categories[i], avgSales);
    }
    
    // 分析价格对销量的影响
    printf("\n3. 价格对销量的影响:\n");
    
    // 按价格分组
    float priceRanges[5][2] = {{0, 100}, {100, 200}, {200, 300}, {300, 400}, {400, 500}};
    char rangeNames[5][20] = {"0-100", "100-200", "200-300", "300-400", "400-500"};
    
    for (int i = 0; i < 5; i++) {
        int totalSales = 0;
        int itemCount = 0;
        
        for (int j = 0; j < count; j++) {
            if (clothes[j].price >= priceRanges[i][0] && clothes[j].price < priceRanges[i][1]) {
                totalSales += clothes[j].sales;
                itemCount++;
            }
        }
        
        if (itemCount > 0) {
            float avgSales = (float)totalSales / itemCount;
            printf("价格范围 %s: 平均销量=%.2f\n", rangeNames[i], avgSales);
        }
    }
}