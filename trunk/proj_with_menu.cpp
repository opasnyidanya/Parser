#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

struct user { //хранение данных файла в структуре
    string tag;
    string request;
};

void check (int &Answer, user &search, vector<user> &data) {
    switch(Answer) {
    case 1: { //если задали поиск по имени/ID
        for(int i = 0; i < data.size(); i++) {
            if(data[i].tag == search.tag && data[i].request == search.request) {
                for(int j = 0; j < data.size(); j++) {
                    cout << data[j].request << ends;
                }
                cout << endl;
                break;
            }
        }
        break;
    }
    case 2: { //если всех вывести
        for(int j = 0; j < data.size(); j++) {
            cout << data[j].request << ends;
        }
        cout << endl;
        break;
    }
    case 4: {//если записывать в файл
        ofstream file("data.txt", ios::app);
        for(int j = 0; j < data.size(); j++) {
            file << data[j].request << ends;
        }
        file << endl;
        file.close();
    }
    }
}

void parser_xml (int &Answer, user &search) {
    vector<user> write; //запись всех пользователей
    string buf; // буффер для строки файла
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
                        cout << endl << "Error! Tag " << tags[tags.size() - 1] << " is wrong!" << endl;
                        exit(0);
                    }

                    if(buf2 == tags[1]) {//если закрыли тег личности, то обнуляем переменную
                        check(Answer, search, write);
                        write.clear();
                    }
                }
            }

            else {//Если у нас нет тега
                user buffer;
                pos = buf.find("<"); //ищем открытие тега
                string buf2(buf, 0, pos);// создаем строку с тегом
                string buf_for_next(buf, pos, buf.size()); // создаем строку с остатком
                buf = buf_for_next; //присваиваем изначальной строке остаток
                buffer.tag = tags[tags.size() - 1];
                buffer.request = buf2;
                write.push_back(buffer);
            }
        }

        if(buf == "</base>")
            break;
    }

    base.close(); //закрываем файл
}

void Menu (int &Answer, user &find) {
    cout << "Choose what you want to find: ";
    cout << endl << "1 - by tag";
    cout << endl << "2 - show all";
    cout << endl << "3 - exit";
    cout << endl << "4 - write all data to file";
    for(;;) {//чтобы не вводили недействительные числа
        cout << endl;
        cin >> Answer;
        if (Answer > 0 && Answer < 5)
            break;
    }
    cin.ignore(); // игнорируем значение в потоке ввода, чтобы вводить строку
    system("cls");
    if (Answer == 1) {
        cout << "Enter the request: ";
        getline(cin, find.request);
        cout << endl << "Enter the tag, like <name>: ";
        getline(cin, find.tag);
    }
}

int main() {
    int Answer;
    user found; //Имя или ID

    ifstream base ("base.xml");
    if(!base.is_open()) {//если xml файл отсутствует
        cout << endl << "ERROR! base.xml wasn't created. Put this file near exe file of this program!" << endl; //выводим ошибку
        exit(0); //выходим
    }
    base.close();

    for(;;) {
        system("cls"); // очистка экрана
        Menu(Answer, found); // меню выбора имени или ID
        system("cls");
        if (Answer == 3)
            exit(0);
        else {
            if(Answer == 4) {
                ofstream file("data.txt");//открываем файл, чтобы data.txt был пустым
                file.close();
            }
            parser_xml(Answer, found);
            system("pause");
        }
    }
}
