#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

struct user { //хранение данных файла в структуре
    string name;
    string id;
};

void check (int &Answer, string &search, user &data) {
    switch(Answer) {
    case 1: { //если задали поиск по имени
        if(search == data.name) {// если запрос == имени - выводим
            cout << "person name=" << data.name;
            cout << " ID=" << data.id << endl ;
        }
        break;
    }
    case 2: { // если по id
        if(search == data.id) {// если запрос == id - выводим
            cout << "person name=" << data.name;
            cout << " ID=" << data.id << endl ;
        }
        break;
    }
    case 3: { //если всех вывести
        cout << "person name=" << data.name;
        cout << " ID=" << data.id << endl ;
        break;
    }
    }
}

void parser_xml (int &Answer, string &search) {
    user write; //запись всех пользователей
    string buf; // буффер для строки файла
    int choice = 0; //1 - запись имени, 2 - ID
    int pos = 0; //позиция символов в строке
    vector<string> tags; //вектор тегов

    ifstream base("base.xml"); //открываем файл
    while(!base.eof()) { //до конца файла читаем
        getline(base, buf); //пишем строку из файла в буффер

        while(!buf.empty()) {//до тех пор, пока строка не будет пустой
            if (buf[0] == '<') {// если первый элемент - открытие тега
                pos = buf.find(">") + 1; //ищем позицию закрывания тега
                string buf2(buf, 0, pos);// создаем строку с тегом
                string buf_for_next(buf, pos, buf.size()); // создаем строку с остатком
                buf = buf_for_next; //присваиваем изначальной строке остаток


                if(buf2[1] != '/') { // если тег открытия - записываем в вектор
                    tags.push_back(buf2);
                }

                if(buf2[1] == '/') {//если тег закрытия - удаляем из вектора
                    buf2.erase(1, 1);
                    if(tags[tags.size() - 1] == buf2) //если последний открытый тег равен закрывающему - удаляем из вектора
                        tags.erase(tags.end());
                    else { //не закрыли - ошибка и выход из программы
                        cout << endl << "Error! tag " << tags[tags.size() - 1] << " didn't close!" << endl;
                        exit(0);
                    }

                    if(buf2 == "<person>") {//если закрыли тег личности, то обнуляем переменную
                        choice = 0;
                        check(Answer, search, write);
                    }
                }

                if(buf2 == "<name>") //если открыли тег имени - меняем переменную на 1
                    choice = 1;
                if(buf2 == "<ID>") //если тег ID - 2
                    choice = 2;
            }

            else {//Если у нас нет тега
                pos = buf.find("<"); //ищем открытие тега
                string buf2(buf, 0, pos);// создаем строку с тегом
                string buf_for_next(buf, pos, buf.size()); // создаем строку с остатком
                buf = buf_for_next; //присваиваем изначальной строке остаток
                if(choice == 1) //если в переменной 1, то записываем имя
                    write.name = buf2;
                if(choice == 2) //если 2, то ID
                    write.id = buf2;
            }
        }

        if(buf == "</base>")
            break;
    }

    for(int i = 0; i < tags.size(); i++) {
        cout << endl << tags[i];
    }

    base.close(); //закрываем файл
}

void Menu (int &Answer, string &find) {
    cout << "Choose what you want to find: ";
    cout << endl << "1 - by the name";
    cout << endl << "2 - by ID";
    cout << endl << "3 - show all";
    cout << endl << "4 - exit";
    for(;;) {//чтобы не вводили недействительные числа
        cout << endl;
        cin >> Answer;
        if (Answer > 0 && Answer < 5)
            break;
    }
    cin.ignore(); // игнорируем значение в потоке ввода, чтобы вводить строку
    system("cls");
    if (Answer == 1) {
        cout << "Enter the name: ";
        getline(cin, find);
    }
    if (Answer == 2) {
        cout << "Enter ID: ";
        getline(cin, find);
    }
}

int main() {
    int Answer;
    string found; //Имя или ID

    for(;;) {
        system("cls"); // очистка экрана
        Menu(Answer, found); // меню выбора имени или ID
        system("cls");
        if (Answer == 4)
            exit(0);
        else {
            parser_xml(Answer, found);
            system("pause");
        }
    }
}
