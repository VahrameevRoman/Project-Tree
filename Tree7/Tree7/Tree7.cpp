#include<fstream>
#include<string>
#include<iostream>
#include<windows.h>
using namespace std;

class Node //(key - Eng, translation - Rus)
{
public:
    string key;
    string translation;
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

Node* Insert(Node* p, string k, string translation) //вставка ключа k в дерево с корнем p
{
    if (p == nullptr)
    {
        p = new Node;
        p->key = k;
        p->translation = translation;
        p->left = nullptr;
        p->right = nullptr;
        p->height = 1;
        return p;
    }
    if (k < p->key) p->left = Insert(p->left, k, translation);
    else p->right = Insert(p->right, k, translation);
    return Balance(p);
}

void PrintSort(Node* p)
{
    if (p->left) PrintSort(p->left);
    cout << p->key << " - " << p->translation << "\n";
    if (p->right) PrintSort(p->right);
}

void GetTranslate(Node* p, string key)
{
    if (p->left) GetTranslate(p->left, key);
    {
        if (p->key == key)
            cout << p->key << " - " << p->translation << "\n";
    }
    if (p->right) GetTranslate(p->right, key);
}

Node* GetAdresNode(Node* root, string key)
{
    if (root->key == key) return root;
    else if ((root->key > key) && (root->left != nullptr)) return GetAdresNode(root->left, key);
    else if ((root->key < key) && (root->right != nullptr)) return GetAdresNode(root->right, key);
    else return nullptr;
}

Node* GetAdresParent(Node* root, Node* p)
{
    if (root->key == p->key) return nullptr;
    if ((root->key > p->key) && ((root->left)->key == p->key)) return root;
    else if ((root->key > p->key) && ((root->left)->key != p->key)) return GetAdresParent(root->left, p);

    if ((root->key < p->key) && ((root->right)->key == p->key)) return root;
    else if ((root->key < p->key) && ((root->right)->key != p->key)) return GetAdresParent(root->right, p);
}

void DeleteNode(Node* root, Node* p)
{
    Node* parentQ = GetAdresParent(root, p);
    Node* q = p;

    if (!parentQ) //удаляем корень
    {
        if ((p->left == nullptr) && (p->right == nullptr)) //без ветвей
        {
            delete p;
            root = nullptr;
            return;
        }
        else if ((p->left == nullptr)) //нет левой ветви
        {
            root = root->right;
            delete p;
            return;
        }
        else if ((p->right == nullptr)) //нет правой ветви
        {
            root = root->left;
            delete p;
            return;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if (q->left)// удаление через левую ветвь
    {   //находим самую большую вершину левой ветви
        parentQ = q;
        q = q->left;
        while (true)
        {
            if (q->right)
            {
                parentQ = q;
                q = q->right;
            }
            else
            {
                parentQ->right = nullptr;
                p->key = q->key;
                delete q;
                break;
            }
        }
    }
    else if (q->right) // удаление через правую ветвь
    {   //находим самую малую вершину правой ветви
        parentQ = q;
        q = q->right;
        while (true)
        {
            if (q->left)
            {
                parentQ = q;
                q = q->left;
            }
            else
            {
                parentQ->left = nullptr;
                p->key = q->key;
                delete q;
                break;
            }
        }
    }
    else //если нет потомков
    {
        if (parentQ->left == q) parentQ->left = nullptr;
        else if (parentQ->right == q) parentQ->right = nullptr;
        delete q;
    }
    cout << "Удаление прошло успешно\n";
}

void StartDelete(Node* root, string key)
{
    Node* p = GetAdresNode(root, key);
    if (p != nullptr)
    {
        DeleteNode(root, p);
    }
    else cout << "Слово отсутствует в словаре \n";
}

Node* ReadFile(Node* root)
{
    string line = "";
    string word = "";
    char ch;
    string key;
    string translation;
    ifstream in("input.txt");
    if (in.is_open())
    {
        while (getline(in, line))
        {
            for (int i = 0; i < line.length(); i++)
            {
                ch = line[i];
                if (ch != '-') word += ch;
                else
                {
                    key = word;
                    word = "";
                }
            }
            translation = word;
            word = "";
            root = Insert(root, key, translation);
        }
    }
    else cout << "ошибка чтения файла" << endl;
    in.close();

    cout << "Слова добавлены в словарь \n";
    return root;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Node* root = nullptr;
    int menu;
    string key;
    string translation;
    while (true)
    {
        cout << "Меню работы с деревом \n";
        cout << "1 - Добавить элемент \n";
        cout << "2 - Вывести отсортированные элементы \n";
        cout << "3 - Поиск перевода \n";
        cout << "4 - Получение словаря из файла \"input.txt\" \n";
        cout << "5 - Удаление слова \n";
        cout << "6 - Выход \n";
        cin >> menu;
        cout << "\n";
        switch (menu)
        {
        case 1:
            cout << "Введите слово на английском: \n";
            cin >> key;
            cout << "Введите слово на русском: \n";
            cin >> translation;
            if ((root == nullptr) || (GetAdresNode(root, key) == nullptr))
            {
                root = Insert(root, key, translation);
            }
            else
            {
                cout << "Добавление отклонено. Слово уже присутствует в словаре\n";
            }
            cout << "\n";
            break;
        case 2:
            if (root) PrintSort(root);
            else cout << "словарь пуст\n";
            cout << "\n";
            break;
        case 3:
            if (root)
            {
                cout << "Введите слово на английском: \n";
                cin >> key;
                GetTranslate(root, key);
            }
            else cout << "словарь пуст\n";
            cout << "\n";
            break;
        case 4:
            root = ReadFile(root);
            cout << "\n";
            break;
        case 5:
            if (root)
            {
                cout << "Введите слово на английском: \n";
                cin >> key;
                StartDelete(root, key);
                if (root != nullptr) Balance(root);
            }
            else cout << "словарь пуст\n";
            cout << "\n";
            break;
        case 6:
            return 0;
        default:
            cout << "Неверный номер действия \n";
            break;
        }
    }
}