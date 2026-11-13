
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Виділення пам'яті для двовимірного динамічного масиву.
int **memory_allocation(int rows, int cols)
{
    int i, j;
    int **matrix = (int **)malloc(rows * sizeof(int *));

    if (matrix == NULL)
        return NULL;

    for (i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));

        if (matrix[i] == NULL)
        {
            // Звільнення пам'яті у разі помилки виділення.
            for (j = 0; j < i; j++)
                free(matrix[j]);

            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

// Звільнення пам'яті, виділеної для матриці.
void clearMemory(int **matrix, int rows)
{
    int i;

    if (matrix == NULL)
        return;

    for (i = 0; i < rows; i++)
        free(matrix[i]);

    free(matrix);
}

// Вибір способу заповнення матриці.
int select_fill_method(char name)
{
    int method;

    printf("\nМатриця %c. Оберіть спосіб заповнення:\n", name);
    printf("1 - Введення з клавіатури\n");
    printf("2 - Випадкові числа\n");
    printf("3 - Математична формула\n");
    printf("Ваш вибір: ");

    if (scanf("%d", &method) != 1 || method < 1 || method > 3)
        return 0;

    return method;
}

// Заповнення матриці вибраним способом.
int fill_matrix(int **matrix, int rows, int cols, int method)
{
    int i, j;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (method == 1)
            {
                printf("Елемент [%d][%d]: ", i, j);

                if (scanf("%d", &matrix[i][j]) != 1)
                    return 0;
            }
            else if (method == 2)
            {
                // Генерація випадкових чисел від -10 до 10.
                matrix[i][j] = rand() % 21 - 10;
            }
            else
            {
                // Обчислення значення за індексами елемента.
                matrix[i][j] = i + j;
            }
        }
    }

    return 1;
}

// Виведення матриці у табличному вигляді.
void print_matrix(int **matrix, int rows, int cols, char name)
{
    int i, j;

    printf("\nМатриця %c (%d x %d):\n", name, rows, cols);

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
            printf("%6d", matrix[i][j]);

        printf("\n");
    }
}

/**
 * @brief Знаходить максимальний і мінімальний елементи нижче головної діагоналі.
 * @param A Квадратна матриця.
 * @param n Порядок матриці.
 * @param max_a Вказівник для запису максимального значення.
 * @param min_a Вказівник для запису мінімального значення.
 */
void max_min(int **A, int n, int *max_a, int *min_a)
{
    int i, j;

    *max_a = A[1][0];
    *min_a = A[1][0];

    for (i = 1; i < n; i++)
    {
        for (j = 0; j < i; j++)
        {
            if (A[i][j] > *max_a)
                *max_a = A[i][j];

            if (A[i][j] < *min_a)
                *min_a = A[i][j];
        }
    }
}

/**
 * @brief Транспонує прямокутну матрицю B.
 * @param B Початкова матриця розміром n_b x m_b.
 * @param n_b Кількість рядків матриці B.
 * @param m_b Кількість стовпців матриці B.
 * @return Нова матриця розміром m_b x n_b або NULL у разі помилки.
 */
int **transpose_b(int **B, int n_b, int m_b)
{
    int i, j;
    int **T = memory_allocation(m_b, n_b);

    if (T == NULL)
        return NULL;

    for (i = 0; i < n_b; i++)
    {
        for (j = 0; j < m_b; j++)
            T[j][i] = B[i][j];
    }

    return T;
}

/**
 * @brief Обчислює матричний добуток A x B.
 * @param A Квадратна матриця.
 * @param n_a Порядок матриці A.
 * @param B Прямокутна матриця.
 * @param n_b Кількість рядків матриці B.
 * @param m_b Кількість стовпців матриці B.
 * @return Матриця добутку C або NULL у разі помилки.
 */
int **mult_a_b(int **A, int n_a, int **B, int n_b, int m_b)
{
    int i, j, k;
    int **C;

    if (n_a != n_b)
        return NULL;

    C = memory_allocation(n_a, m_b);

    if (C == NULL)
        return NULL;

    for (i = 0; i < n_a; i++)
    {
        for (j = 0; j < m_b; j++)
        {
            C[i][j] = 0;

            // Обчислення суми попарних добутків.
            for (k = 0; k < n_a; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }

    return C;
}

// Сортування елементів вибраного рядка за зростанням.
void sort_row(int **A, int cols, int row)
{
    int i, j, temp;

    for (i = 0; i < cols - 1; i++)
    {
        for (j = i + 1; j < cols; j++)
        {
            if (A[row][i] > A[row][j])
            {
                temp = A[row][i];
                A[row][i] = A[row][j];
                A[row][j] = temp;
            }
        }
    }
}

/**
 * @brief Сортує всі елементи квадратної матриці за зростанням.
 * @param A Матриця, елементи якої потрібно впорядкувати.
 * @param n Порядок матриці.
 */
void sort_matrix(int **A, int n)
{
    int i, j, temp;
    int size = n * n;

    // Перетворення лінійних індексів на індекси рядків і стовпців.
    for (i = 0; i < size - 1; i++)
    {
        for (j = i + 1; j < size; j++)
        {
            if (A[i / n][i % n] > A[j / n][j % n])
            {
                temp = A[i / n][i % n];
                A[i / n][i % n] = A[j / n][j % n];
                A[j / n][j % n] = temp;
            }
        }
    }
}

// Обчислення та виведення сум рядків A і стовпців B.
void print_sums(int **A, int n_a, int **B, int n_b, int m_b)
{
    int i, j;
    long long sum;

    printf("\nСуми рядків матриці A:\n");

    for (i = 0; i < n_a; i++)
    {
        sum = 0;

        for (j = 0; j < n_a; j++)
            sum += A[i][j];

        printf("Рядок %d: %lld\n", i + 1, sum);
    }

    printf("\nСуми стовпців матриці B:\n");

    for (j = 0; j < m_b; j++)
    {
        sum = 0;

        for (i = 0; i < n_b; i++)
            sum += B[i][j];

        printf("Стовпець %d: %lld\n", j + 1, sum);
    }
}

int main()
{
    int **A, **B, **T, **C;
    int n_a = 10;
    int n_b = 10;
    int m_b, method;
    int max_a, min_a, row;

    // Налаштування кодування UTF-8 для консолі Windows.
    SetConsoleOutputCP(CP_UTF8);

    srand((unsigned int)time(NULL));

    // Введення кількості стовпців матриці B.
    printf("Введіть кількість стовпців матриці B (1-20, крім 10): ");

    if (scanf("%d", &m_b) != 1 || m_b < 1 || m_b > 20 || m_b == n_b)
    {
        printf("Некоректна розмірність матриці.\n");
        return 1;
    }

    // Створення двох динамічних матриць.
    A = memory_allocation(n_a, n_a);
    B = memory_allocation(n_b, m_b);

    if (A == NULL || B == NULL)
    {
        printf("Помилка виділення пам'яті.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Заповнення матриці A.
    method = select_fill_method('A');

    if (method == 0 || !fill_matrix(A, n_a, n_a, method))
    {
        printf("Некоректне введення для матриці A.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Заповнення матриці B.
    method = select_fill_method('B');

    if (method == 0 || !fill_matrix(B, n_b, m_b, method))
    {
        printf("Некоректне введення для матриці B.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Виведення початкових матриць.
    print_matrix(A, n_a, n_a, 'A');
    print_matrix(B, n_b, m_b, 'B');

    // Пошук максимального та мінімального елементів матриці A.
    max_min(A, n_a, &max_a, &min_a);

    printf("\nМаксимум нижче головної діагоналі: %d\n", max_a);
    printf("Мінімум нижче головної діагоналі: %d\n", min_a);

    // Транспонування матриці B.
    T = transpose_b(B, n_b, m_b);

    if (T == NULL)
    {
        printf("Помилка виділення пам'яті.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    printf("\nРезультат транспонування матриці B:\n");
    print_matrix(T, m_b, n_b, 'T');
    clearMemory(T, m_b);

    // Обчислення матричного добутку A x B.
    C = mult_a_b(A, n_a, B, n_b, m_b);

    if (C == NULL)
    {
        printf("Помилка множення матриць.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    printf("\nРезультат множення матриць A та B:\n");
    print_matrix(C, n_a, m_b, 'C');
    clearMemory(C, n_a);

    // Обчислення сум рядків і стовпців матриць.
    print_sums(A, n_a, B, n_b, m_b);

    // Сортування вибраного рядка матриці A.
    printf("\nВведіть номер рядка для сортування (1-%d): ", n_a);

    if (scanf("%d", &row) != 1 || row < 1 || row > n_a)
    {
        printf("Некоректний номер рядка.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    sort_row(A, n_a, row - 1);

    printf("\nМатриця A після сортування рядка %d:\n", row);
    print_matrix(A, n_a, n_a, 'A');

    // Сортування всіх елементів матриці A.
    sort_matrix(A, n_a);

    printf("\nМатриця A після сортування всіх елементів:\n");
    print_matrix(A, n_a, n_a, 'A');

    // Звільнення динамічної пам'яті.
    clearMemory(A, n_a);
    clearMemory(B, n_b);

    return 0;
}
