#include<iostream>
#include<string>
#include<fstream>
#include<Windows.h>
#include <cctype>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class Month {

public:

	Month(string str) {					//� ������� ����������� ��� ��� ���������� ������
		for (int i = 0; i < 12; ++i) {
			day[i] = new string * [count_day[i]];
			numb[i] = new int* [count_day[i]];

		}
		loadFromFile(str);				//�������� � �����
	}

	void delEvent(string month_t, int num_d, string time) {		//�������� ������� (����� ���; ����� (��������))
		convertTime(time);
		int tmp;
		if (identifyMonth(month_t)) tmp = stoi(month_t) - 1; else
			tmp = (getMonthNum(month_t) - 1);		//���������� ����� ������
		if ((validator_d(num_d, tmp)) && (validator_t(hour, minute)) && (tmp >= 0)) {				//�������� �� ���������� 
			int tmp2 = searchEvent(tmp, num_d, hour * 100 + minute);
			if (tmp2 >= 0) {											//���� � ���� ���� ���� �������, �� ��������
				day[tmp][num_d - 1][tmp2] = { "" };
				numb[tmp][num_d - 1][tmp2] = -1;
				if (count_e[tmp][num_d - 1][0] > 1) {
					for (tmp2; tmp2 < count_e[tmp][num_d - 1][0] - 1; ++tmp2) {
						numb[tmp][num_d - 1][tmp2] = numb[tmp][num_d - 1][tmp2 + 1];
						day[tmp][num_d - 1][tmp2] = day[tmp][num_d - 1][tmp2 + 1];
					}
				}
				--count_e[tmp][num_d - 1][0];
				num_month[tmp]--;
			}
		}
	}

	void setEvent(int num_d, string time_e, string event, string month) {		//���������� ������� (����� ���; �������; �����)
		convertTime(time_e);
		int month_n;
		if (identifyMonth(month)) month_n = stoi(month) - 1; else
			month_n = (getMonthNum(month) - 1);
		if ((validator_d(num_d, month_n)) && (validator_t(hour, minute)) && (month_n >= 0)) {
			getArray(month_n, num_d, time_e, event);
			num_month[month_n]++;
		}
	}

	void showMonths() {			//����� �������
		int tmp = 0;			//������ ���-�� ���� � ������, �� �� ������ ���� ������ 7		
		cout << "\t������ ��� -  " << num_year << "\n\n";
		int tmp2 = x;					//��������� ���������� ��������, ������ ����� ������ ��������, �� ���� �������� �� �������
		for (int q = 0; q < 12; ++q) {
			if (tmp2 >= 7) tmp2 -= 7;				//���� ������ ����� ������ "7", �� �� ��� �� ��������� � ��������� ������ � ����� �������� 7 
			else if (tmp2 < 0) tmp2 += 7;			//���� ������, �� ��� ��� �� ����������, ��� ���������� 7
			SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 2));
			cout << "\n\t\t\t      " << month_name[q] << "\n\n";
			SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 0));
			cout << "\t��\t��\t��\t��\t��\t";
			SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 4));
			cout << "��\t��\n\n";
			SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 0));
			for (int w = 0; w < tmp2; ++w) {		//����� ������� ��-�� �������� (���� ������ = ���� ���)
				cout << " \t";
				++tmp;								//���� ���� ��������� ��� ����
			}
			for (int i = 0; i < count_day[q]; ++i) {
				if (tmp == 7) {		//���� 7 ����, ��
					cout << "\n";	//������ ������� ������ �
					tmp = 0;		//��������
				}
				++tmp;
				if ((tmp == 6) || (tmp == 7)) SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 4));
				else SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 0));
				if ((count_e[q][i][0] == 0)&&(year[q][i]>0))
					cout << '\t' << year[q][i];								//������ ������� ����� ���
				else {
					SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 9));
					cout << '\t' << year[q][i] << '*';
				}
			}
			SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 0));
			cout << "\n\n";													//����� ������� ������ ��� �������� ������
			tmp2 = tmp;														//������ � ������ ������ ����� = ���������� ���� � ��������� ������ � �����������
			tmp = 0;														//���-�� ��������
			showEvents(q);
		}
	}

	void backup() {										//����������
		string tmp = to_string(num_year);
		tmp += ".txt";
		ofstream backupFile(tmp, ios_base::trunc);		//������� ���� � ��������� ����
		for (int q = 0; q < 12; ++q) {
			if (num_month[q] > 0) {					//��������� ������ �� ���, � ������� ��������� �������
				for (int i = 0; i < count_day[q]; ++i) {
					if (count_e[q][i][0] > 0) {
						for (int j = 0; j < count_e[q][i][0]; ++j) {
							backupFile << i + 1 << '.' << q + 1 << ';' << numb[q][i][j] / 100 << ':' << numb[q][i][j] % 100 << '-' << day[q][i][j] << "\n";
						}
					}
				}
			}
		}
		backupFile << '*';
		backupFile.close();								//��������� ����
	}

	void setYear(int year_i) {			//�������� ���� � ����������� �������
		bool flag = false;
		int near_vis_year;				//��������� ������� ���������� ���
		num_year = year_i;
		while (!flag) {
			if ((year_i % 4 == 0) && ((year_i % 100 != 0) || (year_i % 400 == 0))) flag = true;		//���� ��� �� ���������� ���
			else
				--year_i;																			//�������� ���� ���
		}
		near_vis_year = year_i;
		int tmp = searchOffestVisYear(near_vis_year, near_vis_year / 100);		//���� � ������� ��� ����������� ������� ���� ��������� ������� ���������� ��� � �������� 
		int raz = num_year - near_vis_year;										//�������� ����� ��������� ����� � ��� ����������
		if (near_vis_year == num_year) {										//���� ����������, �� 
			x = (tmp + raz) - 2;												//������ ��������� ������ �������� � �������, �� ����� ������� 2 (������ ������ ���)
			count_day[1] = 29;													//� � ������� 29 ����
			year[1][28] = 29;
		}
		else																	//�����
			x = (tmp + raz) - 1;												//�� ������� �������� ������ 1 (���� ������ ������ ���)		
	}

	string setReg(string tmp) {								//��������� �������� (������ �������, � ��������� ���������)
		if (tmp[0] == '�') tmp[0] = '�';
		tmp[0] = toupper(tmp[0]);							//������ ����� ���������

		for (int i = 1; i < tmp.size(); ++i) {
			tmp[i] = tolower(tmp[i]);						//��������� ��� ���������
		}
		return tmp;											//������� ������
	}
	
private:

	void showEvents(int month) {
		if (num_month[month] > 0) {
			cout << "\t ------------------------------------------------\n";
			SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 9));
			cout << "\t*";
			for (int i = 0; i < count_day[month]; ++i) {
				for (int j = 0; j < count_e[month][i][0]; ++j) {
					string tmp3 = { "" }, tmp4 = { "" }, tmp = { "" }, tmp2 = { "" };
					(month < 9) ? (tmp += "0") += to_string(month + 1) : tmp += to_string(month + 1);
					(i < 9) ? (tmp2 += "0") += to_string(i + 1) : tmp2 += to_string(i + 1);
					(numb[month][i][j] / 100 < 9) ? (tmp3 += "0") += to_string(numb[month][i][j] / 100) : tmp3 += to_string(numb[month][i][j] / 100);
					(numb[month][i][j] % 100 < 9) ? (tmp4 += "0") += to_string(numb[month][i][j] % 100) : tmp4 += to_string(numb[month][i][j] % 100);
					cout << "  " << tmp2 << '.' << tmp << " " << tmp3 << ":" << tmp4 << " --- " << day[month][i][j] << "\n\t ";
				}
			}
			SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 0));
			cout << "################################################\n";
		}
	}

	bool identifyMonth(string month) {
		if (stoi(month) > 0) return true;
		else false;
	}

	int searchEvent(int month, int day, int time) {
		for (int i = 0; i < count_e[month][day - 1][0]; ++i) {
			if (numb[month][day - 1][i] == time) return i;
		}
		return -1;
	}

	void getArray(int month_n, int day_n, string time, string event) {
		int* int_arr = new int[count_e[month_n][day_n - 1][0]];
		bool flag = false;
		int last = count_e[month_n][day_n - 1][0];
		string* str_arr = new string[count_e[month_n][day_n - 1][0]];
		int tmp = hour * 100 + minute;
		for (int i = 0; i < count_e[month_n][day_n - 1][0]; ++i) {
			int_arr[i] = numb[month_n][day_n - 1][i];
			str_arr[i] = day[month_n][day_n - 1][i];
		}
		++count_e[month_n][day_n - 1][0];
		numb[month_n][day_n - 1] = new int[count_e[month_n][day_n - 1][0]];
		day[month_n][day_n - 1] = new string[count_e[month_n][day_n - 1][0]];
		for (int i = 0, j = 0; i < count_e[month_n][day_n - 1][0]; ++i) {
			if (((tmp < int_arr[j]) || (count_e[month_n][day_n - 1][0] == 1) || (j == last)) && (flag == false)) {
				numb[month_n][day_n - 1][i] = tmp;
				day[month_n][day_n - 1][i] = event;
				flag = true;
			}
			else {
				numb[month_n][day_n - 1][i] = int_arr[j];
				day[month_n][day_n - 1][i] = str_arr[j];
				++j;
			}
		}
		delete [] int_arr;
	}	

	void loadFromFile(string str) {										
		ifstream inputFile(str.c_str());								//���� ���� � ��������� ���� ����, �� ������
		if (inputFile.is_open()) {										//���� ���� ��������																		
			while (true) {								
				string str = { "" };
				getline(inputFile, str);
				if (str == "*") break;
				else {
					string day_s = { "" }, month_s = { "" }, time = { "" };
					int day_i = 0, month = 0;
					string event = { "" };
					bool flag = true, flag2 = false, flag3 = false, flag4 = false, flag5 = false;
					for (int i = 0; i < str.size(); ++i) {
						if (str[i] == '.') {
							day_i = stoi(day_s);
							flag = false;
							flag2 = true;
							++i;
						}
						else if (str[i] == ';') {
							month = stoi(month_s);
							flag2 = false;
							flag3 = true;
							++i;
						}

						else if (str[i] == '-') {

							flag3 = false;
							flag4 = true;
							++i;
						}
						if (flag) day_s += str[i];
						if (flag2) month_s += str[i];
						if (flag3) time += str[i];
						if (flag4) {
							for (i; i < str.size(); ++i) {
								event += str[i];
							}
							break;
						}
					}
					setEvent(day_i, time, event, to_string(month));
				}
			}
			inputFile.close();										//�������� ��������� �����

		}
	}

	void convertTime(string time) {
		string str_h = { "" }, str_m = { "" };
		bool flag = true;
		for (int i = 0; i < time.size(); ++i) {
			if (time[i] == ':') {
				flag = false;
				++i;
			}
			if (flag) str_h += time[i];
			else str_m += time[i];
		}
		hour = stoi(str_h);
		minute = stoi(str_m);
	}

	int searchOffestVisYear(int a, int year) {			//����� ������� ��� ����������� ����
		int tmp = a % 100;				//��������� ��� ����� ��� ������ �� �������
		int i, max, j;
		if (year % 2 != 0) {			//���� ��� �� 20 ���, ��
			i = 1;						//�������� � ������ ������� �
			j = 0;						//� ������� �������, ��
			max = 5;					//�� 4 ������

		}
		else {							//�����
			i = 4;						//� 4 ������
			j = 3;						//�� 4 �������
			max = 8;					//�� 8 ������

		}
		for (i; i < max; ++i) {						//����� �� 
			for (j; j < 8; ++j) {					//�������� ����������
				if (tmp == offest[i][j]) {			//���� ������ ���������
					return offest[0][j];			//����� ����� ������� ������ (0-�� ������)
				}
			}
			j = 0;
		}
	}

	bool validator_d(int num, int month) {			//�������� ���������� �����
		int count = count_day[month];		//���-�� ���� � ������
		if ((num <= count) && (num > 0)) return true;
		else
			return false;
	}

	bool validator_t(int h, int m) {
		if ((hour >= 0) && (hour <= 23) && (minute >= 0) && (minute <= 59)) return true;
		else
			return false;
	}

	int getMonthNum(string s) {									//����������� ������ ������
		if (s == "������") return 1;
		else
			if (s == "�������") return 2;
			else
				if (s == "����") return 3;
				else
					if (s == "������") return 4;
					else
						if (s == "���") return 5;
						else
							if (s == "����") return 6;
							else
								if (s == "����") return 7;
								else
									if (s == "������") return 8;
									else
										if (s == "��������") return 9;
										else
											if (s == "�������") return 10;
											else
												if (s == "������") return 11;
												else
													if (s == "�������") return 12;
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
	}; 									//������� � ����������� ������ � ���������������� ���������			
	string*** day = new string * *[12];					//������ � ��������� [����� ������] [����� ���]
	int*** numb = new int** [12];					//������ � �������� ���� [����� ������] [����� ���]
	int year[12][31] = { {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31}, };
	int x;								//����� �������
	int num_year;						//����� ���������� ����
	int num_month[12] = { 0 };
	int count_e[12][31][1] = { 0 };
	int hour, minute;
	int count_day[12] = { 31, 28,31,30,31,30,31,31,30,31,30,31 };		//���-�� ���� � ������� �� ��������� [����� ������]
	string month_name[12] = { "������", "�������","����" ,"������","���" ,"����" ,"����" ,"������" ,"��������" ,"�������","������", "�������" };	//������ �������� ������� [����� ������]

};

int main() {

	SetConsoleCP(1251);							//����������� �������
	SetConsoleOutputCP(1251);					//����������� �������
	setlocale(LC_ALL, "rus");					//�������� ������� ����

	while (true) {								//������� ���� ���������
		system("color 70");						//���� ������� (����� ��� � ������ �����)
		system("cls");							//������� �������
		int year = 0;
		cout << "\t������� ���\n\t";
		cin >> year;
		string tmp = to_string(year);			//�������������� ���� � ������ ��� �������� �����
		tmp += ".txt";							//�������� � ���� ".txt" ��� ������������ �������� �����
		Month month(tmp);						//���������� ������ � ������ ���������� ����						
		month.setYear(year);					//�������� ���� (����������� �������)

		while (true) {							//����� ���� ���������
			system("cls");		//������� �������
			month.showMonths();					//����� �������
			string tmp_str, tmp_str2, time;			//���� ������; ���� ������
			int tmp_int;						//����� ���
			cout << "\t����� �������� �������, ������� -> �\n\t����� ������� �������, ������� -> �\n\t����� ������� ���, ������� -> �\n\t����� �����, ������� -> ����� ������\n\t";
			cin >> tmp_str;
			tmp_str = month.setReg(tmp_str);		//������� ��������� ���� �������� (������ �������, � ��������� ���������)
			if (tmp_str == "�") {
				cout << "\n\t������� �����\n\t";
				cin >> tmp_str2;
				cout << "\n\t������� ����\n\t";
				cin >> tmp_int;
				cout << "\n\t������� �����\n\t";
				cin >> time;
				cout << "\n\t������� �������\n\t";
				cin.ignore();
				getline(cin, tmp_str);
				tmp_str2 = month.setReg(tmp_str2);
				month.setEvent(tmp_int, time, tmp_str, tmp_str2);			//���������� �������
			}
			else if (tmp_str == "�") {
				cout << "\t\n������� �����\n\t";
				cin >> tmp_str2;
				cout << "\t\n������� ����\n\t";
				cin >> tmp_int;
				cout << "\t\n������� �����\n\t";
				cin >> time;
				tmp_str2 = month.setReg(tmp_str2);
				month.delEvent(tmp_str2, tmp_int, time);
			}
			else
				if (tmp_str == "�") break;
				else
					return 0;
			month.backup();							//����� (���������� � ����)
		}
	}	
	return 0;
}