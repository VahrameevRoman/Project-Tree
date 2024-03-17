/*
АВЛ-дерево

Разработать программу для реализации структуры дерево:

    1)  Сформировать АВЛ-дерево, тип информационного поля int.

    2)  Распечатать полученное дерево отсортировав элементы по ключу.

    3)  Распечатать структуру дерева (указать для каждого узла его потомков, начиная с корня).

    4)  Обеспечить поиск элемента по ключу и вывод пути до него.

    5)  Найти максимальный и минимальный элементы дерева, количество листьев в дереве, высоту дерева.
*/

#include<iostream>
using namespace std;

class Node
{
public:
    int key;
    int height;
    Node* left;
    Node* right;
};

int GetHeight(Node* p)
{
    if (p) return p->height;
    else return 0;
}

int BFactor(Node* p)
{
    return GetHeight(p->right) - GetHeight(p->left);
}

void FixHeighter(Node* p)
{
    int hl = GetHeight(p->left);
    int hr = GetHeight(p->right);
    if (hl > hr) p->height = hl + 1;
    else p->height = hr + 1;
}

Node* RotateRight(Node* p) //правый поворот
{
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    FixHeighter(p);
    FixHeighter(q);
    return q;
}

Node* RotateLeft(Node* q) //левый поворот
{
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    FixHeighter(q);
    FixHeighter(p);
    return p;
}

Node* Balance(Node* p) // балансировка узла p
{
    FixHeighter(p);
    int bFactor = BFactor(p);
    if (bFactor == 2)
    {
        if (BFactor(p->right) < 0)
            p->right = RotateRight(p->right);
        return RotateLeft(p);
    }
    if (bFactor == -2)
    {
        if (BFactor(p->left) > 0)
            p->left = RotateLeft(p->right);
        return RotateRight(p);
    }
    return p; //балансировка не нужна
}

Node* Insert(Node* p, int k) //вставка ключа k в дерево с корнем p
{
    if (p == nullptr)
    {
        p = new Node;
        p->key = k;
        p->left = nullptr;
        p->right = nullptr;
        p->height = 1;
        return p;
    }
    if (k < p->key) p->left = Insert(p->left, k);
    else p->right = Insert(p->right, k);
    return Balance(p);
}

void Seek(Node* p, int k) //поиск ключа k в дерееве с корнем p
{
    cout << p->key << " ";
    if (k > p->key) Seek(p->right, k);
    else if (k < p->key) Seek(p->left, k);
}

//вывод дерева
void PrintSort(Node* p)
{
    if (p->left) PrintSort(p->left);
    cout << p->key;
    if (p->right) PrintSort(p->right);
}

void print(Node* p)
{
    if (p->left) cout << p->key << "->" << (p->left)->key << endl;
    if (p->right) cout << p->key << "->" << (p->right)->key << endl;
    if (p->left) print(p->left);
    if (p->right) print(p->right);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Node* root = nullptr;
    int menu;
    int key;
    while (true)
    {
        cout << "Меню работы с деревом \n";
        cout << "1 - Добавить элемент \n";
        cout << "2 - Вывести отсортированные элементы \n";
        cout << "3 - Вывести структуру дерева \n";
        cout << "4 - Вывест путь до узла \n";
        cout << "5 - Выход \n";
        cin >> menu;
        switch (menu)
        {
        case 1:
            cout << "Введите ключ: \n";
            cin >> key;
            root = Insert(root, key);
            break;
        case 2:
            PrintSort(root);
            break;
        case 3:
            print(root);
            break;
        case 4:
            cout << "Введите ключ узла \n";
            cin >> key;
            Seek(root, key);
            break;
        case 5:
            return 0;
        default:
            cout << "Неверный номер действия \n";
            break;
        }
    }
}