#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>

class leksema {
    int l_type; std::string name; int value;
public:
    leksema(int l_type1 = 0, std::string name1 = "", int value1 = 0) { l_type = l_type1; name = name1;  value = value1; };
    leksema(const leksema& a) { l_type = a.l_type; name = a.name;  value = a.value; }; // конструктор копирования
    ~leksema() {}; // деструктор
    leksema& operator = (const leksema& a) { l_type = a.l_type; name = a.name;  value = a.value; return *this;};
    int get_l_type() { return l_type; }
    std::string get_name() { return name; }
    int get_value() { return value; }
    void set_l_type(int a) { l_type = a; }
    void set_name(std::string a) { name = a; }
    void set_value(int a) { value = a; }
};

leksema Lexical_tokenizator(std::ifstream& program_file, int& str_number, int& str_position, int& error_flag) {
    char Ci;
    std::string name; 
    int value, index;
    std::string a = "+-*/()[];,"; std::string a1 = "><";
    std::vector<int> b = { 3,4,5,6,7,8,16,17,24,29 };
    std::vector<std::string> function_words_a { "if", "endif", "else", "endelse", "while", "endwihle", "read", "write", "int", "int1" };
    std::vector<int> function_words_b = { 18,19,20,21,22,23,25,26,27,28 };
    std::vector<std::string>::iterator is_fw;
    leksema res;
    S:
        Ci = program_file.peek();

        if (isalpha(Ci)) {
            name = Ci; // 1
            program_file.get();
            ++str_position;
            goto B;
        }
        if (isdigit(Ci)) {
            value = Ci - '0'; // 2
            program_file.get();
            ++str_position;
            goto C;
        }
        if ((index = a.find(Ci)) != std::string::npos) {
            res.set_l_type(b[index]); // 5
            program_file.get();
            ++str_position;
            goto Z;
        }
        if (Ci == '=') {
            program_file.get();
            ++str_position;
            goto A;
        }
        if (Ci == '!') {
            program_file.get();
            ++str_position;
            goto D;
        }
        if ((index = a1.find(Ci)) != std::string::npos) { // 1
            program_file.get();
            ++str_position;
            goto E;
        }
        if (isblank(Ci)) {
            program_file.get();
            ++str_position;
            goto S;
        }
        if (Ci == '\n') {
            program_file.get();
            str_position = 1; // 14
            ++str_number;
            goto S;
        }
        if (program_file.eof()) {
            res.set_l_type(0);
            program_file.get();
            ++str_position;
            goto Z;
        }
        error_flag = 1;
        ++str_position; // ?
        goto Z;
    B:
        Ci = program_file.peek();

        if (isalnum(Ci)) {
            name += Ci; // 4
            program_file.get();
            ++str_position;
            goto B;
        }
        is_fw = std::find(begin(function_words_a), end(function_words_a), name);
        if (is_fw == end(function_words_a)) { // не найдено
            res.set_l_type(2);
            res.set_name(name);
            goto Z;
        } 
        else { 
            res.set_l_type( function_words_b[is_fw - begin(function_words_a)] );
            goto Z;
        }
        /*
        switch (name) { // 8
            case function_words[0]: return 18; break;
            case function_words[1]: return 19; break;
            case function_words[4]: return 20; break;
            case function_words[5]: return 21; break;
            case function_words[2]: return 22; break;
            case function_words[3]: return 23; break;
            case function_words[6]: return 27; break;
            case function_words[7]: return 28; break;
            case function_words[8]: return 25; break;
            case function_words[9]: return 26; break;
            default:    res.set_l_type(2);
                        res.set_name(name);
                        goto Z;
        }
        */
        //-----------------//

    C:
        Ci = program_file.peek();

        if (isalpha(Ci)) {
            ++str_position; // ?
            error_flag = 2;
            goto Z;
        }
        if (isdigit(Ci)) {
            value = value * 10 + (Ci - '0'); // 3
            program_file.get();
            ++str_position;
            goto C;
        }
        res.set_l_type(1); res.set_value(value); // 9
        program_file.get();
        ++str_position;
        goto Z; 
         
    A:
        Ci = program_file.peek();

        if (Ci == '=') {
            res.set_l_type(10); // 10
            program_file.get();
            ++str_position;
            goto Z;
        }
        res.set_l_type(9); // 11
        goto Z;

    D:
        Ci = program_file.peek();

        if (Ci == '=') {
            res.set_l_type(11); // 12
            program_file.get();
            ++str_position;
            goto Z;
        }
        ++str_position; // ?
        error_flag = 3;
        goto Z;

    E:
        Ci = program_file.peek();

        if (Ci == '=') {
            res.set_l_type(14 + index); // 6
            program_file.get();
            ++str_position;
            goto Z;
        }
        res.set_l_type(12 + index); // 7
        goto Z;
    Z:
        return res;
};

int main()
{
    SetConsoleCP(1251); // устанавливаем кодировку для ввода\вывода на консоль
    SetConsoleOutputCP(1251);
    std::ifstream program_file("prog.txt");
    int index;
    bool end_flag = true; int error_flag = 0, str_number = 1, str_position = 1;
    leksema x;
    std::vector<std::string>leksema_list = { "конец программы", "константа", "переменная", "+", "-", "*", "/", "(", ")" , "=" , "==" , "!=" , ">" , "<" , ">=" , "<=" , "[" , "]" , "if" , "endif" , "else" , "endelse" , "while" , "endwhile" , ";" , "read" , "write" , "int" , "int1", "," };
    
    while (error_flag == 0 && end_flag) {
        x = Lexical_tokenizator(program_file, str_number, str_position, error_flag);
        if (error_flag == 0) {
            index = x.get_l_type();
            switch (index) {
            case 0: 
                std::cout << index << " : " << leksema_list[index] << std::endl;
                end_flag = false;
                break;
            case 1: 
                std::cout << index << " : " << leksema_list[index] << " : " << x.get_value() << std::endl;
                break;
            case 2: 
                std::cout << index << " : " << leksema_list[index] << " : " << x.get_name() << std::endl;
                break;
            default: 
                std::cout << index << " : " << leksema_list[index] << std::endl;
                break;
            }
        }
    }
    std::cout << "Строка: " << str_number << " Позиция: " << str_position << " Код ошибки: " << error_flag << "\nОписание: " << std::endl;
    program_file.close();
    return 0;
}



// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
