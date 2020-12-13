#include <iostream>
#include <iterator>
#include <algorithm>
#include <memory>
#include <cmath>
#include "vector.hpp"
#include "triangle.hpp"

void help()
{
    printf("Для выполнения операции введите соответствующую цифру:\n ");
    printf("\t 1 - Добавить фигуру\n ");
    printf("\t 2 - Удалить фигуру\n ");
    printf("\t 3 - Вывести все фигуры на экран\n ");
    printf("\t 4 - Вывести количество всех фигур, у которых площадь меньше, заданной, на экран\n ");
    printf("\t 0 - Выйти из программы\n ");
}

int main()
{

    int i = 1;
    char enter = -1;
    TP xl, yl, xr, yr;
    int c;
    TVector<Triangle<TP>, Alloc<Triangle<TP>, 10000>> vec;
    help();
    std::cin >> enter;
    while (isdigit(enter))
    {
        switch (enter - 48)
        {

        case 1:
        {
            cout << "Введите координаты двух вершин вашей фигуры в направлении против часовой стрелки." << endl;
            std::cin >> xl >> yl >> xr >> yr;
            /* Triangle<TP>* ptr =  allocmain.allocate();
            *ptr = ; */
            Triangle<TP> a(xl, yl, xr, yr);
            vec.insert(vec.size, a);
            //ptr = NULL;
            //allocmain.deallocate(ptr);
            cout << "Элемент вставлен." << endl;
            break;
        }
        case 2:
        {
            cout << "Введите номер удаляемой фигуры " << endl;
            int numb;
            std::cin >> numb;
            int err = (vec.Erase(numb - 1));
            if (err < 0)
            {
                switch (err)
                {
                    {
                    case -1:
                    {
                        printf("Вектор пуст, удалять - нечего.\n");
                        break;
                    } case -2:
                    {
                        printf("В векторе нет элемента под индексом %d\n", numb);
                        break;
                    }

                    default:
                        printf("Ошибка при удалении вектора.\n");
                        break;
                    }
                }
                break;
            }
            cout << "Фигура удалена\n Текущий размер хранилища :Size = " << vec.size << endl;
            break;
        }
        case 3:
        { //вывод всех фигур
            for_each(vec.begin(), vec.end(), [&](Triangle<TP> &&tr) {
                printf("Фигура №%d\n", i);
                i++;
                tr.Print();
            });
            i = 1;

            break;
        }
        case 4:
        {
            TP square = 0;
            printf("Введите площадь больше площадей только тех фигур , которые надо вывести\n");
            std::cin >> square;
            //вывод если прлощадь меньше заданной
            int r = std::count_if(vec.begin(), vec.end(), [&](Triangle<TP> &&tr) { return tr.Square() < square; });
            cout << r << endl;
            break;
        }
        case 0:
        {
            cout << "Выход из программы..." << endl;
            return 0;
        }
        default:
        {
            help();
            break;
        }
        }
        enter = 48;
        std::cin >> enter;
    }
    if(!(isdigit(enter))){
        cout << "Похоже не цифра введена, ну в другой раз не промахнётесь по клавиатуре ;-) \n\n"<< endl;
    }
    return 0;
}