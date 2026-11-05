
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    printf("\nMatrix %c. Select filling method:\n", name);
    printf("1 - Manual input\n");
    printf("2 - Random values\n");
    printf("3 - Mathematical formula\n");
    printf("Your choice: ");

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
                printf("Element [%d][%d]: ", i, j);

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

    printf("\nMatrix %c (%d x %d):\n", name, rows, cols);

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
            printf("%6d", matrix[i][j]);

        printf("\n");
    }
}

int main()
{
    int **A, **B;
    int n_a = 10;
    int n_b = 10;
    int m_b, method;

    srand((unsigned int)time(NULL));

    // Введення кількості стовпців матриці B.
    printf("Enter number of columns for B (1-20, except 10): ");

    if (scanf("%d", &m_b) != 1 || m_b < 1 || m_b > 20 || m_b == n_b)
    {
        printf("Invalid matrix size.\n");
        return 1;
    }

    // Створення двох динамічних матриць.
    A = memory_allocation(n_a, n_a);
    B = memory_allocation(n_b, m_b);

    if (A == NULL || B == NULL)
    {
        printf("Memory allocation error.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Заповнення матриці A.
    method = select_fill_method('A');

    if (method == 0 || !fill_matrix(A, n_a, n_a, method))
    {
        printf("Invalid input for matrix A.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Заповнення матриці B.
    method = select_fill_method('B');

    if (method == 0 || !fill_matrix(B, n_b, m_b, method))
    {
        printf("Invalid input for matrix B.\n");
        clearMemory(A, n_a);
        clearMemory(B, n_b);
        return 1;
    }

    // Виведення створених матриць.
    print_matrix(A, n_a, n_a, 'A');
    print_matrix(B, n_b, m_b, 'B');

    // Звільнення динамічної пам'яті.
    clearMemory(A, n_a);
    clearMemory(B, n_b);

    return 0;
}
