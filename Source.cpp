#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<Windows.h>
#include <cctype>
#include <algorithm>
using namespace std;

template <typename T>
string toString(T val) {
	ostringstream oss;					//открытие строкового потока
	oss << val;							//запись года в поток
	return oss.str();					//вывод года типа строка
}
class Month {
public:
	Month(string str) {					//в скобках выполняется код при объявление класса
		for (int i = 0; i < 12; ++i) {
			day[i] = new string * [count_day[i]];
			year[i] = new int* [count_day[i]];
		}
		//loadFromFile(str);				//загрузка с файла
	}
	/*void delEvent(int num, string month_t) {		//удаление события (номер дня; месяц (название))
		if (validator(num + 1, month_t)) {				//проверка на валидность числа
			string month = setReg(month_t);				//функция установки спец регистра (первая большая, а остальные маленькие)
			int tmp = getMonthNum(month) - 1;			//определить номер месяца
			if (!day[tmp][num - 1].empty()) {			//если в этот день есть событие, то работаем
				for (int i = 0; i < numb; ++i) {		//поиск нужного дня в зависимости от очереди его добавления
					if (number[tmp][i] + 1 == num) {	//если это нужный день, то удаляем упоминания об событии
						day[tmp][num - 1] = { "" };
						number[tmp][i] = 0;
						if (count_events[tmp] > 1) for (i; i < count_events[tmp]; ++i) number[tmp][i] = number[tmp][i + 1];		//сдвигаем на удаленное событие
						break;
					}
				}
				--numb;
				++count_del;
				--count_events[tmp];
				if (count_events[tmp] == 0)
					num_month[getLastRec()] = -1;		//если это было последнее событие в месяце, то удаляем запись об этом месяце
			}
		}
	}*/
	void setEvent(int num_d, int time_e, string event, string month) {		//добавление события (номер дня; событие; месяц)
		if (validator(num_d, month)) {
			string month = setReg(month);					//функция установки спец регистра (первая большая, а остальные маленькие)
			int month_n = (getMonthNum(month) - 1);				//определить номер месяца
			getArray(month_n, num_d, time_e, event);
			/*/day[tmp][num] = event;
			getNumberArray(num, tmp);
			for (int i = 0; i < 12; ++i) {
				if (num_month[i] == tmp) {				//поиск прошлой записи такого месяца
					flag = true;
					break;
				}
			}
			if (!flag) {								//если такого месяца не было, то делаем новую запись
				++tmp_tmp;
				num_month[tmp_tmp] = tmp;
			}
			++numb;
			index[tmp][num] = numb;*/
		}
	}
void showMonths() {			//вывод месяцев
	int tmp = 0;			//счетчи кол-ва дней в недели, он не должен быть больше 7
	cout << "Выбран год -  " << num_year << "\n\n";
	int tmp2 = x;					//временная переменная отступов, каждый месяц отступ меняется, но чтоб основную не трогать
	for (int q = 0; q < 12; ++q) {
		if (tmp2 >= 7) tmp2 -= 7;				//если отступ вышел больше "7", то он как бы переходит в следующую неделю и нужно вычитать 7 
		else if (tmp2 < 0) tmp2 += 7;			//если меньше, то это как бы предыдущая, тут прибавляем 7
		cout << "\n" << month_name[q] << "\n";
		cout << "Пн\tВт\tСр\tЧт\tПт\tСб\tВс\n\n";

		for (int w = 0; w < tmp2; ++w) {		//вывод нужного ко-ва отступов (один отступ = один таб)
			cout << " \t";
			++tmp;								//табы тоже считаются как день
		}
		for (int i = 0; i < count_day[q]; ++i) {
			if (tmp == 7) {		//если 7 дней, то
				cout << "\n";	//делаем перевод строки и
				tmp = 0;		//обнуляем
			}
			++tmp;

			cout << year[q][i] << "\t";								//просто выводим номер дня
		}
		cout << "\n\n";													//после каждого месяца два перевода строки
		tmp2 = tmp;														//отступ у нового месяца будет = количеству дней в последней неделе у предыдущего
		tmp = 0;														//кол-во зануляем
	}
}


/*void backup() {										//сохранение
	string tmp = toString(num_year);
	tmp += ".txt";
	ofstream backupFile(tmp, ios_base::trunc);		//создаем файл с названием года
	for (int q = 0; q < 12; ++q) {
		if (num_month[q] != -1) {					//сохраняем только те дни, в которые добавляли события
			backupFile << "\n!" << month_name[num_month[q]] << endl;		//название месяца("!" означает что этот месяц впервые)
			for (int i = 0; i < count_events[num_month[q]]; ++i)
				if (i != 0)		//если на этот месяц больше одного события, то
					backupFile << endl << "?" << number[num_month[q]][i] + 1 << " " << day[num_month[q]][number[num_month[q]][i]];		//ставим "?" и сохраняем
				else
					backupFile << number[num_month[q]][i] + 1 << " " << day[num_month[q]][number[num_month[q]][i]];
		}
		else
			break;
	}
	backupFile.close();								//закрываем файл
}*/
void setYear(int year) {			//создание года и определение отступа
	bool flag = false;
	int near_vis_year;				//ближайший меньший високосный год
	num_year = year;
	while (!flag) {
		if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))) flag = true;		//пока это не високосный год
		else
			--year;																			//отнимать один год
	}
	near_vis_year = year;
	int tmp = searchOffestVisYear(near_vis_year, near_vis_year / 100);		//ввод в функцию для определения индекса года ближайший меньший високосный год и столетие 
	int raz = num_year - near_vis_year;										//разность между начальным годом и его високосным
	if (near_vis_year == num_year) {										//если високосный, то 
		x = (tmp + raz) - 2;												//отступ находится суммой разности и индекса, но нужно вычесть 2 (просто потому что)
		count_day[1] = 29;													//и в феврале 29 дней
	}
	else																	//иначе
		x = (tmp + raz) - 1;												//из отступа вычетаем только 1 (тоже просто потому что)
}
string setReg(string tmp) {								//установка регистра (первая большая, а остальные маленькие)
	tmp[0] = toupper(tmp[0]);							//первая буква заглавная
	for (int i = 1; i < tmp.size(); ++i) {
		tmp[i] = tolower(tmp[i]);						//остальные все маленькие
	}
	return tmp;											//вернуть строку
}
private:
	void getArray(int month_n, int day_n, int time, string event) {
		
	}
	/*void setMonth() {									//создание массива с днями
		for (int q = 0; q < 12; ++q)					//12 месяцев
			for (int i = 0; i < count_day[q]; ++i)		//устанавливается кол-во дней по умолчанию
				year[q][i] = i + 1;						//заполнение ячеек
	}*/
	/*void loadFromFile(string str) {										//загрузка из файла
		setMonth();														//заполнение массивов месяцев с датами
		ifstream inputFile(str.c_str());								//если файл с названием года есть, то читать
		if (inputFile.is_open()) {										//если файл открылся
			if (!inputFile.eof()) {										//если файл не дочитан до конца
				int month = -1;											//номер месяца
				int mon = -1;											//количество месяцев с событиями ;номер месяца для заполнения массива с месяцами в порядке добавления в них события (num_month)
				while (!inputFile.eof()) {								//читать файл, пока не дошел до конца
					char tmp2 = ' ';									//опознавательный знак; переменная для загрузки события ("!" - этот месяц впервые: "?" - этот месяц уже был и события записываются в него)
					int tmp = 0;										//номер дня
					string tmp_str = " ";								//название месяца
					inputFile >> tmp2;
					if (tmp2 == '!') {
						inputFile >> tmp_str;							//чтение месяца
						month = getMonthNum(tmp_str) - 1;				//определение номера месяца
						inputFile >> tmp;								//чтение номера дня
						inputFile.ignore();								//специальная команда для чистого чтения getline
						getline(inputFile, day[month][tmp - 1]);		//чтение события из файла и запись в массив событий [номер месяца] [номер дня]
						getNumberArray(tmp - 1, month);					//добавление нового номера дня в массив всех добавленных номеров дней с событиями
						++numb;											//увеличение кол-ва событий
						++mon;											//увеличение кол-ва месяцев, значит в файле пошел новый месяц
						index[month][tmp - 1] = numb;					//добавление номера события в массив, когла все месяцы выводятся, дни с событиями выделяются
																		//последовательным номером, обозначающий в какой последовательности добавляли это событие
						num_month[mon] = month;							//добавление нового месяца в массив с месяцами у которых есть события
																		//[последовательный номер, когда этот месяц впервые упомянулся]
					}
					else if (tmp2 == '?') {
						inputFile >> tmp;								//чтение номера дня
						getline(inputFile, day[month][tmp - 1]);		//чтение события из файла и запись в массив событий [номер месяца] [номер дня]
						getNumberArray(tmp - 1, month);					//добавление нового номера дня в массив всех добавленных номеров дней с событиями
						++numb;											//увеличение кол-ва событий
						index[month][tmp - 1] = numb;					//добавление номера события в массив, когла все месяцы выводятся, дни с событиями выделяются
																		//последовательным номером, обозначающий в какой последовательности добавляли это событие
					}
				}
				inputFile.close();										//закрытие читаемого файла
			}
		}
	}*/
	int searchOffestVisYear(int a, int year) {			//поиск индекса для високосного года
		int tmp = a % 100;				//последние две цифры для поиска по таблице
		int i, max, j;
		if (year % 2 != 0) {			//если это не 20 век, то
			i = 1;						//начинаем с первой строчки и
			j = 0;						//с первого столбца, но
			max = 5;					//до 4 строки

		}
		else {							//иначе
			i = 4;						//с 4 строки
			j = 3;						//со 4 столбца
			max = 8;					//до 8 строки

		}
		for (i; i < max; ++i) {						//циклы по 
			for (j; j < 8; ++j) {					//заданным параметрам
				if (tmp == offest[i][j]) {			//ищут нужное окончание
					return offest[0][j];			//когда нашли вернути индекс (0-ая строка)
				}
			}
			j = 0;
		}
	}

	bool validator(int num, string month) {			//проверка валидности числа
		int count = count_day[getMonthNum(month) - 1];		//кол-ва дней в месяце
		if ((num <= count) && (num > 0)) return true;
		else
			return false;
	}


	int getMonthNum(string s) {									//определение номера месяца
		if (s == "Январь") return 1;
		else
			if (s == "Февраль") return 2;
			else
				if (s == "Март") return 3;
				else
					if (s == "Апрель") return 4;
					else
						if (s == "Май") return 5;
						else
							if (s == "Июнь") return 6;
							else
								if (s == "Июль") return 7;
								else
									if (s == "Август") return 8;
									else
										if (s == "Сентябрь") return 9;
										else
											if (s == "Октябрь") return 10;
											else
												if (s == "Ноябрь") return 11;
												else
													if (s == "Декабрь") return 12;
													else
														return -1;
	}
	int offest[8][7] = { {6, 4, 2, 0, 5, 3, 1},
						 {4, 8, 12, 16, 20, 24 ,28},
						 {32, 36, 40, 44, 48, 52, 56},
						 {60, 64, 68, 72, 76, 80, 84},
						 {88, 92, 96, 0, 4, 8, 12 },
						 {16, 20, 24, 28, 32, 36, 40},
						 {44, 48, 52, 56, 60, 64, 68},
						 {68, 76, 80, 84, 88, 92, 96}
	}; 									//таблица с високосными годами и соответствующими индексами			
	string*** day = new string * *[12];					//массив с событиями [номер месяца] [номер дня]
	int*** year = new int** [12];					//массив с номерами дней [номер месяца] [номер дня]
	int x;								//число отступа
	int num_year;						//номер введенного года
	int count_day[12] = { 31, 28,31,30,31,30,31,31,30,31,30,31 };		//кол-во дней в месяцах по умолчанию [номер месяца]
	string month_name[12] = { "Январь", "Февраль","Март" ,"Апрель","Май" ,"Июнь" ,"Июль" ,"Август" ,"Сентябрь" ,"Октябрь","Ноябрь", "Декабрь" };	//массив названий месяцев [номер месяца]

};
int main() {

	SetConsoleCP(1251);							//локализация консоли
	SetConsoleOutputCP(1251);					//локализация консоли
	setlocale(LC_ALL, "rus");					//добавляю русский язык

	while (true) {								//большой цикл программы
		system("color 70");						//цвет консоли (белый фон и черный шрифт)
		system("cls");							//очистка консоли
		int year = 0;
		cout << "Введите год\n";
		cin >> year;
		string tmp = toString(year);			//преобразование года в строку для создания файла
		tmp += ".txt";							//прибавка к году ".txt" для последующего создания файла
		Month month(tmp);						//объявление класса с вводом выбранного года						
		month.setYear(year);					//создание года (определение отступа)
		while (true) {	/*						//малый цикл программы
			system("cls");						//очистка консоли
			month.showMonths();					//вывод месяцев
			string tmp_str, tmp_str2;			//ввод команд; ввод месяца
			int tmp_int;						//номер дня
			month.showEvents();					//вывод событий
			cout << "Чтобы редактировать события, введите => Редактировать\nЧтобы изменить год, введите => Смена\nЧтобы выйти, введите любой символ\n\n";
			cin >> tmp_str;
			tmp_str = month.setReg(tmp_str);		//функция установки спец регистра (первая большая, а остальные маленькие)
			if (tmp_str == "Редактировать") {
				cout << "\nЧтобы удалить событие, введите => Удалить\nЧтобы добавить или изменить событие, введите => Добавить\n\n";
				cin >> tmp_str;
				tmp_str = month.setReg(tmp_str);	//функция установки спец регистра (первая большая, а остальные маленькие)
				if (tmp_str == "Удалить") {
					cout << "\nВыберите месяц\n";
					cin >> tmp_str;
					cout << "\nВыберите день\n";
					cin >> tmp_int;
					month.delEvent(tmp_int, tmp_str);		//функция удаления события
				}
				else
					if (tmp_str == "Добавить") {
						cout << "\nВыберите месяц\n";
						cin >> tmp_str2;
						cout << "\nВыберите день\n";
						cin >> tmp_int;
						cout << "\nВведите событие\n";
						cin.ignore();
						getline(cin, tmp_str);
						tmp_str2 = month.setReg(tmp_str2);
						month.setEvent(tmp_int, tmp_str, tmp_str2);			//добавление события
					}
			}
			else
				if (tmp_str == "Смена") break;
				else
					return 0;
			month.backup();							//бэкап (сохранение в файл)*/
		}
	}
	return 0;
}