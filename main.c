
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

/**
 * @brief Зчитує ціле число з перевіркою введення.
 * @param prompt Повідомлення для користувача.
 * @param value Вказівник для запису значення.
 * @return 1 у разі успіху, 0 у разі завершення вводу.
 */
int read_integer(const char *prompt, int *value)
{
    int result, ch, correct;

    while (1)
    {
        printf("%s", prompt);
        fflush(stdout);

        result = scanf("%d", value);

        if (result == EOF)
            return 0;

        correct = (result == 1);

        // Перевірка зайвих символів та очищення буфера.
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            if (ch != ' ' && ch != '\t' && ch != '\r')
                correct = 0;
        }

        if (correct)
            return 1;

        printf("Помилка! Введіть ціле число.\n");

        if (ch == EOF)
            return 0;
    }
}

// Введення цілого числа у заданому діапазоні.
int read_range(const char *prompt, int min, int max,
               int excluded, int *value)
{
    while (read_integer(prompt, value))
    {
        if (*value >= min && *value <= max && *value != excluded)
            return 1;

        printf("Недопустиме значення. Повторіть введення.\n");
    }

    return 0;
}

// Вибір способу заповнення матриці.
int select_fill_method(char name)
{
    int method;

    printf("\nМатриця %c. Оберіть спосіб заповнення:\n", name);
    printf("1 - Введення з клавіатури\n");
    printf("2 - Випадкові числа\n");
    printf("3 - Математична формула\n");

    if (!read_range("Ваш вибір: ", 1, 3, -1, &method))
        return 0;

    return method;
}

// Заповнення матриці вибраним способом.
int fill_matrix(int **matrix, int rows, int cols, int method)
{
    int i, j;
    char prompt[64];

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (method == 1)
            {
                snprintf(prompt, sizeof(prompt),
                         "Елемент [%d][%d]: ", i, j);

                if (!read_integer(prompt, &matrix[i][j]))
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

int main(void)
{
    int **A, **B, **T, **C;
    int n_a = 10;
    int n_b = 10;
    int m_b, method, choice, row;
    int max_a, min_a;

    // Налаштування кодування UTF-8 для консолі Windows.
    SetConsoleOutputCP(CP_UTF8);

    srand((unsigned int)time(NULL));

    // Введення кількості стовпців матриці B.
    if (!read_range(
        "Введіть кількість стовпців матриці B (1-20, крім 10): ",
        1, 20, 10, &m_b))
        return 1;

    // Створення динамічних матриць.
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
        printf("Не вдалося заповнити матрицю A.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Заповнення матриці B.
    method = select_fill_method('B');

    if (method == 0 || !fill_matrix(B, n_b, m_b, method))
    {
        printf("Не вдалося заповнити матрицю B.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Виведення початкових матриць.
    print_matrix(A, n_a, n_a, 'A');
    print_matrix(B, n_b, m_b, 'B');

    // Повторне виконання операцій через меню.
    do
    {
        printf("\n========== ГОЛОВНЕ МЕНЮ ==========\n");
        printf("1 - Вивести матриці A та B\n");
        printf("2 - Знайти максимум і мінімум\n");
        printf("3 - Транспонувати матрицю B\n");
        printf("4 - Помножити матриці A та B\n");
        printf("5 - Відсортувати вибраний рядок A\n");
        printf("6 - Відсортувати всю матрицю A\n");
        printf("7 - Обчислити суми рядків і стовпців\n");
        printf("0 - Завершити програму\n");

        if (!read_range("Ваш вибір: ", 0, 7, -1, &choice))
        {
            clearMemory(A, n_a);
            clearMemory(B, n_b);
            return 1;
        }

        switch (choice)
        {
            case 1:
                print_matrix(A, n_a, n_a, 'A');
                print_matrix(B, n_b, m_b, 'B');
                break;

            case 2:
                max_min(A, n_a, &max_a, &min_a);

                printf("\nМаксимум нижче головної діагоналі: %d\n",
                       max_a);
                printf("Мінімум нижче головної діагоналі: %d\n",
                       min_a);
                break;

            case 3:
                // Транспонування зі створенням нової матриці.
                T = transpose_b(B, n_b, m_b);

                if (T == NULL)
                {
                    printf("Помилка виділення пам'яті.\n");
                    break;
                }

                printf("\nРезультат транспонування матриці B:\n");
                print_matrix(T, m_b, n_b, 'T');
                clearMemory(T, m_b);
                break;

            case 4:
                // Обчислення матричного добутку A x B.
                C = mult_a_b(A, n_a, B, n_b, m_b);

                if (C == NULL)
                {
                    printf("Помилка множення матриць.\n");
                    break;
                }

                printf("\nРезультат множення матриць A та B:\n");
                print_matrix(C, n_a, m_b, 'C');
                clearMemory(C, n_a);
                break;

            case 5:
                if (!read_range(
                    "Введіть номер рядка для сортування (1-10): ",
                    1, n_a, -1, &row))
                {
                    clearMemory(A, n_a);
                    clearMemory(B, n_b);
                    return 1;
                }

                sort_row(A, n_a, row - 1);

                printf("\nМатриця A після сортування рядка %d:\n",
                       row);
                print_matrix(A, n_a, n_a, 'A');
                break;

            case 6:
                sort_matrix(A, n_a);

                printf("\nМатриця A після сортування всіх елементів:\n");
                print_matrix(A, n_a, n_a, 'A');
                break;

            case 7:
                print_sums(A, n_a, B, n_b, m_b);
                break;

            case 0:
                printf("\nРоботу програми завершено.\n");
                break;
        }

    } while (choice != 0);

    // Звільнення динамічної пам'яті.
    clearMemory(A, n_a);
    clearMemory(B, n_b);

    return 0;
}
