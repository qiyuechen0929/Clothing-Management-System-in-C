/* 服装销售系统 —— 零警告完整版 main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX       200
#define NAME_LEN  30
const char* g_file = "cloth.txt";

typedef struct {
    char  brand[NAME_LEN];
    char  category[NAME_LEN];
    float price;
    int   sales;
    float cost;
    int   stockNum;
    int   valid;
} Cloth;

Cloth g_db[MAX] = {0};
int   g_cart[MAX] = {0};
int   g_cartCnt = 0;

/* ====== 工具：落盘 & 读档 ====== */
void saveAll(void) {
    FILE* fp = fopen(g_file, "w");
    if (!fp) { perror("saveAll"); return; }
    for (int i = 0; i < MAX; i++)
        if (g_db[i].valid == 1)
            fprintf(fp, "%s %s %.1f %d %.1f %d\n",
                    g_db[i].brand, g_db[i].category,
                    g_db[i].price, g_db[i].sales,
                    g_db[i].cost, g_db[i].stockNum);
    fclose(fp);
}

void loadAll(void) {
    FILE* fp = fopen(g_file, "r");
    if (!fp) return;
    int idx = 0;
    while (fscanf(fp, "%s%s%f%d%f%d",
                  g_db[idx].brand,
                  g_db[idx].category,
                  &g_db[idx].price,
                  &g_db[idx].sales,
                  &g_db[idx].cost,
                  &g_db[idx].stockNum) == 6) {
        g_db[idx].valid = (g_db[idx].stockNum > 0) ? 1 : 0;
        idx++;
    }
    fclose(fp);
}

/* ====== 2. 浏览 ====== */
void listCloth(void) {
    int empty = 1;
    for (int i = 0; i < MAX; i++)
        if (g_db[i].valid == 1) { empty = 0; break; }
    if (empty) {
        puts("\n>>> 您还未录入数据！");
        return;
    }
    puts("\n序号  品牌        品类        售价   销量   成本   库存   毛利率");
    for (int i = 0; i < MAX; i++) {
        if (g_db[i].valid == 0) continue;
        Cloth* c = &g_db[i];
        float rate = (c->price - c->cost) / c->price * 100;
        printf("%-4d  %-10s %-10s %6.1f %6d %6.1f %6d %5.1f%%\n",
               i, c->brand, c->category, c->price, c->sales, c->cost,
               c->stockNum, rate);
    }
}

/* ====== 1. 录入（多条） ====== */
void addCloth(void) {
    puts("请逐行输入（品牌 品类 售价 销量 成本 库存），直接回车结束：");
    getchar();
    char line[256];
    int added = 0;
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\r') break;
        int pos = -1;
        for (int i = 0; i < MAX; i++)
            if (g_db[i].valid == 0) { pos = i; break; }
        if (pos == -1) { puts("数据库已满！中断录入"); break; }
        Cloth* c = &g_db[pos];
        if (sscanf(line, "%s%s%f%d%f%d", c->brand, c->category,
                   &c->price, &c->sales, &c->cost, &c->stockNum) != 6) {
            puts("格式错误，跳过此行");
            continue;
        }
        c->valid = (c->stockNum > 0) ? 1 : 0;
        added++;
    }
    if (added == 0) { puts("没有新增数据"); return; }
    saveAll();
    printf("-> 成功录入 %d 条，已保存！\n", added);
}

/* ====== 3. 删除 ====== */
void delCloth(void) {
    printf("输入要删除的序号（空格隔开）或输入“全部删除”清空库：\n");
    getchar();
    char line[256];
    if (!fgets(line, sizeof(line), stdin)) return;
    if (strncmp(line, "全部删除", 6) == 0) {
        for (int i = 0; i < MAX; i++) g_db[i].valid = 0;
        saveAll();
        puts("-> 已清空全部数据！");
        return;
    }
    int idx, ok = 0, bad = 0;
    char* p = line;
    while (sscanf(p, "%d", &idx) == 1) {
        while (isspace(*p)) p++;
        while (isdigit(*p)) p++;
        if (idx < 0 || idx >= MAX || g_db[idx].valid == 0) { bad++; continue; }
        g_db[idx].valid = 0; ok++;
    }
    if (ok == 0) { puts("没有可删除的序号！"); return; }
    saveAll();
    printf("-> 成功删除 %d 条，序号保持不变！\n", ok);
    if (bad) puts("部分序号非法或已删，已跳过。");
}

/* ====== 4. 修改（无空格冒号） ====== */
void editCloth(void) {
    int idx;
    printf("输入要修改的序号：");
    if (scanf("%d", &idx) != 1) return;
    if (idx < 0 || idx >= MAX || g_db[idx].valid == 0) {
        puts("序号非法或已被删！");
        return;
    }
    getchar();
    Cloth* c = &g_db[idx];
    puts("请逐条输入要修改的字段（格式：字段名:值），直接回车结束：");
    puts("示例(英文冒号)：售价:499 品牌:ADIDAS 库存:10");
    char line[256];
    int changed = 0;
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\r') break;

        char field[NAME_LEN], newVal[NAME_LEN];
        /* 关键：冒号前后无空格 */
        if (sscanf(line, "%[^:]:%s", field, newVal) != 2) {
            puts("格式不对，正确示例：售价:499");
            continue;
        }
        char* pf = field;
        while (isspace(*pf)) pf++;
        char* pe = field + strlen(field) - 1;
        while (pe > pf && isspace(*pe)) *pe-- = '\0';

        if (strcasecmp(pf, "品牌") == 0) {
            strcpy(c->brand, newVal); changed++;
        }
        else if (strcasecmp(pf, "品类") == 0) {
            strcpy(c->category, newVal); changed++;
        }
        else if (strcasecmp(pf, "售价") == 0) {
            float v = atof(newVal);
            if (v <= 0) { puts("售价必须>0"); continue; }
            c->price = v; changed++;
        }
        else if (strcasecmp(pf, "销量") == 0) {
            int v = atoi(newVal);
            if (v < 0) { puts("销量不能负数"); continue; }
            c->sales = v; changed++;
        }
        else if (strcasecmp(pf, "成本") == 0) {
            float v = atof(newVal);
            if (v < 0) { puts("成本不能负数"); continue; }
            c->cost = v; changed++;
        }
        else if (strcasecmp(pf, "库存") == 0) {
            int v = atoi(newVal);
            if (v < 0) { puts("库存不能负数"); continue; }
            c->stockNum = v; changed++;
            if (c->stockNum == 0) c->valid = 0;
        }
        else {
            printf("不识别的字段：%s\n", pf);
        }
    }
    if (changed == 0) { puts("没有字段被修改"); return; }
    saveAll();
    printf("-> 已修改 %d 处，已保存！\n", changed);
}

/* ========== 5. 加购（重复=数量） ========== */
void addCart(void) {
    printf("输入要加入购物车的序号（重复=数量，如 0 0 2 5）：\n");
    getchar();
    char line[256];
    if (!fgets(line, sizeof(line), stdin)) return;

    int idx, ok = 0, bad = 0;
    char* p = line;
    while (sscanf(p, "%d", &idx) == 1) {
        while (isspace(*p)) p++;
        while (isdigit(*p)) p++;
        if (idx < 0 || idx >= MAX || g_db[idx].valid == 0) { bad++; continue; }
        g_cart[g_cartCnt++] = idx;
        ok++;
    }
    if (ok == 0) { puts("没有成功加入任何商品！"); return; }
    printf("-> 成功加入 %d 件到购物车！\n", ok);
    if (bad) puts("部分序号非法或已删，已跳过。");
}

/* ========== 6. 查看购物车 ========== */
void showCart(void) {
    if (g_cartCnt == 0) { puts("购物车为空！"); return; }
    int qty[MAX] = {0};
    for (int i = 0; i < g_cartCnt; i++) qty[g_cart[i]]++;
    float total = 0;
    puts("\n购物车：");
    for (int i = 0; i < MAX; i++) {
        if (qty[i] == 0) continue;
        Cloth* c = &g_db[i];
        float sub = c->price * qty[i];
        printf("序号%-3d 数量%-3d  %-10s %-10s 单价%.1f 小计%.1f\n",
               i, qty[i], c->brand, c->category, c->price, sub);
        total += sub;
    }
    printf("总计：%.2f 元\n", total);
}

/* ========== 7. 结账 ========== */
void checkout(void) {
    if (g_cartCnt == 0) { puts("购物车为空！"); return; }
    showCart();
    printf("确认付款？（1-是 0-否）：");
    int ok; scanf("%d", &ok);
    if (!ok) { puts("已取消。"); return; }
    int qty[MAX] = {0};
    for (int i = 0; i < g_cartCnt; i++) qty[g_cart[i]]++;
    for (int i = 0; i < MAX; i++)
        if (qty[i] > 0 && g_db[i].stockNum < qty[i]) {
            printf("序号 %d 库存不足（需要 %d，仅有 %d），结账失败！\n",
                   i, qty[i], g_db[i].stockNum);
            return;
        }
    for (int i = 0; i < MAX; i++) {
        if (qty[i] == 0) continue;
        g_db[i].sales += qty[i];
        g_db[i].stockNum -= qty[i];
        if (g_db[i].stockNum <= 0) g_db[i].valid = 0;
    }
    g_cartCnt = 0;
    saveAll();
    puts("-> 付款成功！销量已更新，库存已扣减。");
}

/* ========== 8. 一键直接付款 ========== */
void directPay(void) {
    printf("您要选择什么商品（重复=数量，如 0 0 2 5）：\n");
    getchar();
    char line[256];
    if (!fgets(line, sizeof(line), stdin)) return;

    int idx, cnt = 0, bad = 0;
    int tempQty[MAX] = {0};
    char* p = line;
    while (sscanf(p, "%d", &idx) == 1) {
        while (isspace(*p)) p++;
        while (isdigit(*p)) p++;
        if (idx < 0 || idx >= MAX || g_db[idx].valid == 0) { bad++; continue; }
        tempQty[idx]++;
        cnt++;
    }
    if (cnt == 0) { puts("没有有效商品！"); return; }

    /* 库存预检 */
    for (int i = 0; i < MAX; i++)
        if (tempQty[i] > 0 && g_db[i].stockNum < tempQty[i]) {
            printf("序号 %d 库存不足（需要 %d，仅有 %d），付款失败！\n",
                   i, tempQty[i], g_db[i].stockNum);
            return;
        }

    /* 显示清单 */
    float total = 0;
    puts("\n直接付款清单：");
    for (int i = 0; i < MAX; i++) {
        if (tempQty[i] == 0) continue;
        Cloth* c = &g_db[i];
        float sub = c->price * tempQty[i];
        printf("序号%-3d 数量%-3d  %-10s %-10s 单价%.1f 小计%.1f\n",
               i, tempQty[i], c->brand, c->category, c->price, sub);
        total += sub;
    }
    printf("总计：%.2f 元\n", total);

    printf("确认直接付款？（1-是 0-否）：");
    int ok; scanf("%d", &ok);
    if (!ok) { puts("已取消。"); return; }

    /* 统一扣减 */
    for (int i = 0; i < MAX; i++) {
        if (tempQty[i] == 0) continue;
        g_db[i].sales += tempQty[i];
        g_db[i].stockNum -= tempQty[i];
        if (g_db[i].stockNum <= 0) g_db[i].valid = 0;
    }
    saveAll();
    puts("-> 直接付款成功！销量已更新，库存已扣减。");
}

/* ========== 9. 价格区间毛利率 ========== */
void priceClass(void) {
    float profit[3] = {0}, revenue[3] = {0};
    for (int i = 0; i < MAX; i++) {
        if (g_db[i].valid == 0) continue;
        Cloth* c = &g_db[i];
        int idx = c->price < 100 ? 0 : (c->price <= 300 ? 1 : 2);
        profit[idx]  += (c->price - c->cost) * c->sales;
        revenue[idx] += c->price * c->sales;
    }
    puts("\n价格区间毛利率：");
    const char* tag[3] = {"<100", "100-300", ">300"};
    for (int i = 0; i < 3; i++)
        if (revenue[i])
            printf("  %-7s 毛利率 %.1f%%\n", tag[i], profit[i] / revenue[i] * 100);
}

/* ========== 10. 因素分析 ========== */
void factorInfluence(void) {
    puts("\n因素分析（平均销量）：");
    /* 品牌 */
    char   brand[MAX][NAME_LEN];
    int    bSales[MAX] = {0}, bNum[MAX] = {0}, bCnt = 0;
    for (int i = 0; i < MAX; i++) {
        if (g_db[i].valid == 0) continue;
        Cloth* c = &g_db[i];
        int idx = -1;
        for (int k = 0; k < bCnt; k++)
            if (strcmp(brand[k], c->brand) == 0) { idx = k; break; }
        if (idx == -1) { idx = bCnt; strcpy(brand[bCnt++], c->brand); }
        bSales[idx] += c->sales; bNum[idx]++;
    }
    puts("  品牌：");
    for (int i = 0; i < bCnt; i++)
        printf("    %-10s 平均销量 %.1f\n", brand[i], (float)bSales[i] / bNum[i]);

    /* 品类 */
    char   cat[MAX][NAME_LEN];
    int    cSales[MAX] = {0}, cNum[MAX] = {0}, cCnt = 0;
    for (int i = 0; i < MAX; i++) {
        if (g_db[i].valid == 0) continue;
        Cloth* c = &g_db[i];
        int idx = -1;
        for (int k = 0; k < cCnt; k++)
            if (strcmp(cat[k], c->category) == 0) { idx = k; break; }
        if (idx == -1) { idx = cCnt; strcpy(cat[cCnt++], c->category); }
        cSales[idx] += c->sales; cNum[idx]++;
    }
    puts("  品类：");
    for (int i = 0; i < cCnt; i++)
        printf("    %-10s 平均销量 %.1f\n", cat[i], (float)cSales[i] / cNum[i]);

    /* 价格区间 */
    int pSales[3] = {0}, pNum[3] = {0};
    for (int i = 0; i < MAX; i++) {
        if (g_db[i].valid == 0) continue;
        Cloth* c = &g_db[i];
        int idx = c->price < 100 ? 0 : (c->price <= 300 ? 1 : 2);
        pSales[idx] += c->sales; pNum[idx]++;
    }
    const char* pTag[3] = {"<100", "100-300", ">300"};
    puts("  价格区间：");
    for (int i = 0; i < 3; i++)
        if (pNum[i])
            printf("    %-7s 平均销量 %.1f\n", pTag[i], (float)pSales[i] / pNum[i]);
}

/* ========== 11. 品牌营收+最赚品类+占比 ========== */
void brandRevenue(void) {
    char   brand[MAX][NAME_LEN];
    float  profit[MAX] = {0};
    float  maxCatProfit[MAX] = {0};
    char   maxCatName[MAX][NAME_LEN];
    int bCnt = 0;

    /* 统计总利润 & 最赚品类 */
    for (int i = 0; i < MAX; i++) {
        if (g_db[i].valid == 0) continue;
        Cloth* c = &g_db[i];
        int idx = -1;
        for (int k = 0; k < bCnt; k++)
            if (strcmp(brand[k], c->brand) == 0) { idx = k; break; }
        if (idx == -1) { idx = bCnt; strcpy(brand[bCnt++], c->brand); }

        float pro = (c->price - c->cost) * c->sales;
        profit[idx] += pro;
        if (pro > maxCatProfit[idx]) {
            maxCatProfit[idx] = pro;
            strcpy(maxCatName[idx], c->category);
        }
    }

    puts("\n品牌营收分析（总利润 & 最赚品类 & 占比）：");
    float total = 0;
    for (int i = 0; i < bCnt; i++) total += profit[i];
    for (int i = 0; i < bCnt; i++) {
        float pct = total > 0 ? profit[i] / total * 100 : 0;
        printf("  %-10s 总利润=%10.2f  最赚品类：%-10s  占比 %5.1f%%\n",
               brand[i], profit[i], maxCatName[i], pct);
    }
}

/* ========== 菜单 & main ========== */
void menu(void) {
    puts("\n========== 服装销售系统 ==========");
    puts("1. 录入服装（可一次录多条，空行结束）");
    puts("2. 浏览服装");
    puts("3. 删除服装（输“全部删除”秒清库）");
    puts("4. 修改服装（字段名:值，无空格）");
    puts("5. 加入购物车（重复=数量，如 0 0 2 5）");
    puts("6. 查看购物车");
    puts("7. 结账");
    puts("8. 一键直接付款（如 0 0 2 5）");
    puts("9. 价格区间毛利率");
    puts("10. 因素分析（品牌/品类/价格区间）");
    puts("11. 品牌营收+最赚品类+占比");
    puts("0. 退出");
    printf("请选择：");
}

int main(void) {
    loadAll();
    int opt;
    while (1) {
        menu();
        if (scanf("%d", &opt) != 1) break;
        if (opt == 1) addCloth();
        else if (opt == 2) listCloth();
        else if (opt == 3) delCloth();
        else if (opt == 4) editCloth();
        else if (opt == 5) addCart();
        else if (opt == 6) showCart();
        else if (opt == 7) checkout();
        else if (opt == 8) directPay();
        else if (opt == 9) priceClass();
        else if (opt == 10) factorInfluence();
        else if (opt == 11) brandRevenue();
        else if (opt == 0) break;
        else puts("输入无效！");
    }
    saveAll();
    puts("已退出，数据已保存。");
    return 0;
}
