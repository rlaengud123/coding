#include<iostream>
#include<cstdio> 
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<cstddef>
#include<cassert>
#include<cfloat>
#include<cmath>
#include<typeinfo>
#include<string>
#include<stdio.h>


using namespace std;

double run_time, start, stop;

typedef struct order_info {
	int id_no;
	int alternatives_num;//대안 프로세스의 갯수는 10개까지
	int process_num; //프로세스 과정 ABC이면 3
	char order_name[10];//ABC
	int quantity;
	struct order_info *link;
}ORDER_UNIT;

typedef struct site_info {
	int id;
	char name;
}SITE;

int load(ORDER_UNIT* orders[100]);// 반환값은 갯수로 
//void create_matrix(int no_tid, int site_no, int f_matrix[100][100], SITE site_info[100], TOURIST_UNIT tourist[100]);
char check_copy(SITE site_info[100], int site_no, char site_name[]);
int copy(int no_tid, ORDER_UNIT* orders[100], SITE site_info[100]);
int **create_matrix(int no_tid, ORDER_UNIT orders[100]);

int load(ORDER_UNIT* orders[100]) {
	char  filename[13];
	int i = 0;//주문 주소의 인덱스 값 
	int j = 0;//배열 가르키는 위치

	errno_t err;
	FILE *fp;

	err = fopen_s(&fp, "input.txt", "rt");  // 전체 path 경로를 지정, 아니면 실행파일이 있는 곳에 생성됨 
	if (err == 0)
		- puts("파일오픈 성공!\n");
	else {
		puts("파일 오픈 실패\n");
		return -1;
	}
	while (!feof(fp)) {
		char s[81];
		ORDER_UNIT* temp;

		orders[i] = (ORDER_UNIT *)malloc(sizeof(ORDER_UNIT));
		temp = orders[i];
		fgets(s, 80, fp);// 최대 80칸짜리 한줄 읽기
		//주문 갯수 최댓값은  99개, order id 저장 
		if (int(s[1]) == 32) {
			orders[i]->id_no = s[0] - '0'; j = 2;
		}
		else
		{
			orders[i]->id_no = (s[0] - '0') * 10 + s[1] - '0'; j = 3;
		}
		int num = 0;
		orders[i]->alternatives_num = 0;

		while (true) {
			if (int(s[j]) == 32) break;
			if (s[j] != '-') {
				orders[i]->order_name[num] = s[j];
				num++;
			}
			if (s[j] == ',') {
				orders[i]->order_name[num - 1] = '\0';
				orders[i]->alternatives_num++;
				orders[i]->link = (ORDER_UNIT *)malloc(sizeof(ORDER_UNIT));
				orders[i]->process_num = num - 1;
				orders[i] = orders[i]->link;
				temp->alternatives_num++;
				j++; num = 0;
			}
			j++;
		}
		orders[i]->order_name[num] = '\0';
		orders[i]->process_num = num;
		if (orders[i]->alternatives_num == 0) orders[i]->alternatives_num++;
		orders[i] = temp;
		int temp_j = j--;
		//printf("%d \n", temp_j);
		//최대 용량은 9999
		int fin_j = 0;
		while (true) {
			if (int(s[temp_j]) == 10) {
				fin_j = temp_j;
				//printf("%d\n", fin_j);
				break;
			}
			else temp_j++;
		}
		int nn = fin_j - j - 2;
		//printf("%d\n ", nn);
		//printf("%d\n ", j);
		//printf("%d\n ", int(s[j + 3]) - 48);
		j++;
		if (nn == 3) orders[i]->quantity = (int(s[j + 1]) - 48) * 100 + (int(s[j + 2]) - 48) * 10 + (int(s[j + 3]) - 48);
		else if (nn == 4) orders[i]->quantity = (int(s[j + 1]) - 48) * 1000 + (int(s[j + 2]) - 48) * 100 + (int(s[j + 3]) - 48) * 10 + (int(s[j + 4]) - 48);
		else if (nn == 2) orders[i]->quantity = (int(s[j + 1]) - 48) * 10 + (int(s[j + 2]) - 48) * 1;
		else if (nn == 1) orders[i]->quantity = (int(s[j + 1]) - 48);
		printf("%d\n ", orders[i]->quantity);
		i++;
		//printf("\n");
	}
	fclose(fp);
	i--;
	//printf("%d ", i);
	return i;
}

/* end of void load */
char check_copy(SITE site_info[100], int site_no, char site_name) {
	int  i, identical_counter;
	char identical;
	identical_counter = 0;
	for (i = 0, identical = 'n'; i < site_no; i++) {
		if (site_info[i].name != site_name) { // 서로 다른 경우
			identical_counter += 1;
		}
	}
	//printf("Non identical_counter(%d == %d) site_no,    name_site = %s in  check_copy \n ", identical_counter, site_no, site_name);
	if (identical_counter == site_no) {
		return identical = 'n';
	}
	else return identical = 'y';
	// 서로 다른 회수와 비교수 site_no가 같으면 삽입대상으로 한다



} // end of char check_copy

int copy(int no_tid, ORDER_UNIT* orders[100], SITE site_info[100]) {
	int  i, j, l, count;
	int site_no = 1; int index_no = 0;
	for (i = 0, index_no = 0; i < no_tid + 1; i++) {
		int n = orders[i]->alternatives_num;
		for (l = 0; l < n; l++) {
			ORDER_UNIT* temp = orders[i];
			int num = orders[i]->process_num;
			for (j = 0; j < num; j++) {
				char site_name = orders[i]->order_name[j];
				if (check_copy(site_info, site_no, site_name) == 'n') {
					site_info[index_no].name = site_name;
					site_no++;
					site_info[index_no].id = site_no - 1;
					index_no++;
				}
			} // End of for (j = 0; 
			orders[i] = orders[i]->link;
		}
		//printf(" %d \n", tourist[i].frequency);
	}

	return site_no - 1;
}
//예은이 것
double setResult(string a, string b, int a_n, int b_n) {
	int intersect = 0;
	int union2 = 0;
	for (int j = 0; j < a_n; j++) {
		for (int i = 0; i < b_n; i++)
		{
			if (a[j] == b[i]) { intersect++; }
		}
	}
	union2 = a_n + b_n - intersect;
	//printf("%d", intersect);
	//printf("%d ", union2);
	string str_a(a);
	string str_b(b);
	string temp1[10];
	string temp2[10];
	int index1 = 0;
	for (int p = 0; p < a_n - 1; p++) {
		for (int q = 0; q < 2; q++) {
			temp1[p] += a[index1];
			index1++;
		}
		index1--;
	}
	int index2 = 0;
	for (int r = 0; r < b_n - 1; r++) {
		for (int s = 0; s < 2; s++) {
			temp2[r] += b[index2];
			index2++;
		}
		index2--;
	}
	//cout << temp1[1]<< temp2[1];



	int alpha = 0;
	int beta = 0;
	for (int j = 0; j < a_n - 1; j++) {
		for (int i = 0; i < b_n - 1; i++)
		{
			if (temp1[j].compare(temp2[i]) == 0) { alpha++; }
		}
	}
	//cout << alpha;
	beta = (a_n - 1) + (b_n - 1) - 2 * alpha;
	//cout << beta << endl;
	double result;
	result = (double)(alpha + intersect) / (double)(beta + union2);
	return result;
}

char change1(int i) {
	int temp_row;
	char temp_row_char;
	temp_row = (i / 8) + 1;
	for (int i = 0; i < 27; i++) {
		if (temp_row == i) {
			temp_row_char = i + 64;
			i = 0;
			break;
		}
	}
	return temp_row_char;
}
char change2(int i) {
	int temp_col;
	char temp_col_char;
	temp_col = i % 8;
	for (int i = 0; i < 27; i++) {
		if (temp_col == i) {
			temp_col_char = i + 64;
			break;
		}
	}
	return temp_col_char;
}

int cor(int i) {
	int temp_row;
	int temp_col;
	temp_row = (i / 8) + 1;
	temp_col = i % 8;
	return printf("(%d, %d) \n", temp_row, temp_col);
}

int main() {
	ORDER_UNIT* f_orders[100];
	SITE site_info[100];
	int no_tid; int p = 0; int r; int site_no;
	int quan2;



	no_tid = load(f_orders);
	site_no = copy(no_tid, f_orders, site_info);
	no_tid = load(f_orders);



	for (p = 0; p < no_tid; p++) {
		int s = f_orders[p]->alternatives_num;
		ORDER_UNIT* temp = f_orders[p];
		for (r = 0; r < s; r++) {
			cout << f_orders[p]->order_name << endl;
			cout << f_orders[p]->process_num << endl;



			if (r == 0) {
				cout << f_orders[p]->quantity << endl;
				quan2 = f_orders[p]->quantity;
			}
			else
				cout << quan2 << endl;
			f_orders[p] = f_orders[p]->link;
		}
		f_orders[p] = temp;
		cout << f_orders[p]->id_no << " 대체공정 갯수" << f_orders[p]->alternatives_num << endl;
	}
	cout << site_no << endl;
	for (int i = 0; i < site_no; i++) {
		cout << site_info[i].name;
		cout << site_info[i].id;
	}

	int i1, i2;
	int j1;
	int i;
	int i5;
	char arr[8][8];
	int qun[8][8];
	char ask1 = 65;
	char ask2 = 65;
	char*prt = arr[8];
	int temp123 = 0;
	int temp_max = 0;
	int save_point1 = 0;
	int save_point2 = 0;
	int count1 = 0;
	int count2 = 0;
	int column = 1;
	int row = 0;
	int enter = 0;
	int col_save = 0;
	int row_save = 0;
	int row_num = 0;
	int col_count = 0;
	int j = 0;



	for (int i = 0; i < 8; i++) {
		for (ask1; ask1 < 73; ask1++) {
			for (ask2; ask2 < 73; ask2++) {
				cout << endl << "첫번째 글자" << ask1 << "두번째 글자" << ask2 << endl;
				for (int p = 0; p < no_tid + 1; p++) {
					int s = f_orders[p]->alternatives_num;
					ORDER_UNIT* temp = f_orders[p];
					quan2 = f_orders[p]->quantity / 10;
					for (int r = 0; r < s; r++) {
						prt = f_orders[p]->order_name;
						cout << "확인중" << prt << endl;
						for (int i = 0; i < 10; i++) {
							if (prt[i] == ask1) {
								if (prt[i + 1] == ask2) {
									cout << "요거다" << prt << endl;
									if (r == 0) {
										quan2 = f_orders[p]->quantity / 10;
										cout << "quantity 값은 " << quan2 << endl;
										temp123 += quan2;
									}
									else {
										cout << "quantity 값은 " << quan2 << endl;
										temp123 += quan2;
									}
								}
								f_orders[p] = f_orders[p]->link;
							}
						}
							
					}
					f_orders[p] = temp;
						
				}
				
				qun[i][j] = temp123;
				cout << "최종 quantity 값은 " << qun[i][j] << endl;
				j++;
				
				temp123 = 0;

			}
			ask2 = 65;

		}
	}


	cout << "여기서부터는 행렬입니다." << endl;
	printf("\t");
	for (int j = 97; j < 105; j++) {
		printf("%c \t", j);
	}
	printf("\n");
	row_num = 97;
	for (int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++){
			if ((column % 8) == 1) {
				printf("%c \t", row_num);
				row_num++;
			}
			printf("%d \t", qun[i][j]);
			if ((column % 8) == 0) {
				cout << endl;
			}
			column++;
		}
	}

	printf("%d %d \n", row_save, col_save);
	int row_char;
	int col_char;
	string alpa1;
	string alpa2;
	string temp_ans;
	string r_ans;
	string ans_arr[100];
	string ans_alpa1;
	string ans_alpa2;
	string ans_alpa;
	string ans_next1[100];
	string ans_next2[100];
	string ans_next[100];
	int ans_arr_num[100];
	int ans_count = 0;
	int ans_num = 0;
	double taw;
	double ans_temp;
	int ans_temp_quan1 = 0;
	int ans_temp_quan2 = 0;
	int ans_quan1 = 0;
	int ans_quan2 = 0;
	int ans_quan = 0;
	int temp_quan1 = 0;
	int temp_quan2 = 0;
	int next_count1 = 0;
	int next_count2 = 0;
	int next_count = 0;
	int next_quan1[100];
	int next_quan2[100];
	int next_quan[100];
	int pn[100];
	int pn1[100];
	int pn2[100];
	for (int i = 0; i < 27; i++) {
		if (row_save == i) {
			row_char = i + 64;
			printf("찾은 값은 %c", row_char);
			alpa1 = row_char;
			i = 0;
			break;
		}
	}
	for (int i = 0; i < 27; i++) {
		if (col_save == i) {
			col_char = i + 64;
			printf("%c 입니다.\n\n", col_char);
			alpa2 = col_char;
			break;
		}
	}/*
	printf("타우값을 지정해 주세요.");
	scanf_s("%lf", &taw);

	cout << endl << "BC앞에 오는 것 중에서 0보다 큰 값은" << endl;
	//예시) ㅁBC 에서 ㅁ 구하기, B열에 있는 것중 0보다 큰 것 찾기
	for (int i = 1; i < 9; i++) {
		if (qun[(i - 1) * 8 + row_save - 1] > 0) {
			if ((i - 1) * 8 + row_save - 1 != save_point) {
				temp_ans = change1((i - 1) * 8 + row_save);
				r_ans = temp_ans + alpa1 + alpa2;
				cout << "찾은 공정은 " << r_ans << endl;
				for (p = 0; p < no_tid; p++) {
					int s = f_orders[p]->alternatives_num;
					ORDER_UNIT* temp = f_orders[p];
					ans_temp_quan1 = f_orders[p]->quantity / 10;
					for (r = 0; r < s; r++) {
						std::string ans(r_ans);
						std::string f_ans(f_orders[p]->order_name);
						std::size_t found = f_ans.find(ans);
						if (found != std::string::npos) {
							if (found >= 0) {
								std::cout << f_orders[p]->order_name << endl;
								std::cout << f_orders[p]->process_num << endl;
								if (r == 0) {
									ans_temp_quan1 = f_orders[p]->quantity / 10;
									temp_quan1 += ans_temp_quan1;
								}
								else {
									temp_quan1 += ans_temp_quan1;
								}
								cout << ans_temp_quan1 << endl;
								ans_arr[ans_count] = f_orders[p]->order_name;
								ans_arr_num[ans_count] = f_orders[p]->process_num;
								if (ans_quan1 < temp_quan1) {
									ans_quan1 = temp_quan1;
									ans_alpa1 = r_ans;
									ans_next1[ans_count] = f_orders[p]->order_name;
									next_count1 = ans_count + 1;
									next_quan1[ans_count] = ans_temp_quan1;
									pn1[ans_count] = f_orders[p]->process_num;
								}
								ans_count++;
							}
						}
						f_orders[p] = f_orders[p]->link;
					}
					f_orders[p] = temp;
				}
				cout << "유효성 검사 시작" << endl;
				while (ans_num < ans_count) {
					for (int r = ans_num + 1; r < ans_count; r++) {
						std::cout << ans_arr[ans_num] << "와" << '\t' << ans_arr[r] << endl;
						ans_temp = setResult(ans_arr[ans_num], ans_arr[r], ans_arr_num[ans_num], ans_arr_num[r]);
						if (ans_temp >= taw) {
							std::cout << ans_temp << endl;
						}
						else {
							cout << "타우값보다 작습니다." << endl;
							std::cout << ans_temp << endl;
						}
						ans_temp = 0;
					}
					ans_num++;
				}
				cout << "quantity는 " << temp_quan1 << " 입니다." << endl;
				temp_quan1 = 0;
				temp_ans.erase();
				r_ans.erase();
				printf("%d번째 \n좌표는", (i - 1) * 8 + row_save);
				cor((i - 1) * 8 + row_save);
				printf("값은 %d\n\n", qun[(i - 1) * 8 + row_save - 1]);
				ans_count = 0;
				ans_num = 0;
			}
		}
	}
	cout << endl << "BC뒤에 오는 것 중에서 0보다 큰 값은" << endl;

	//예시) BCㅁ 에서 ㅁ 구하기, C행에 있는 것중 0보다 큰 것 찾기
	for (int i = 0; i < 8; i++) {
		if (qun[(col_save - 1) * 8 + i] > 0) {
			if ((col_save - 1) * 8 + i != save_point + 1) {
				temp_ans = change2((col_save - 1) * 8 + i + 1);
				r_ans = alpa1 + alpa2 + temp_ans;
				cout << "찾은 공정은 " << r_ans << endl;
				for (p = 0; p < no_tid; p++) {
					int s = f_orders[p]->alternatives_num;
					ORDER_UNIT* temp = f_orders[p];
					ans_temp_quan2 = f_orders[p]->quantity / 10;
					for (r = 0; r < s; r++) {
						std::string ans(r_ans);
						std::string f_ans(f_orders[p]->order_name);
						std::size_t found = f_ans.find(ans);
						if (found != std::string::npos) {
							if (found >= 0) {
								std::cout << f_orders[p]->order_name << endl;
								std::cout << f_orders[p]->process_num << endl;
								if (r == 0) {
									ans_temp_quan2 = f_orders[p]->quantity / 10;
									temp_quan2 += ans_temp_quan2;
								}
								else {
									temp_quan2 += ans_temp_quan2;
								}
								cout << ans_temp_quan2 << endl;
								ans_arr[ans_count] = f_orders[p]->order_name;
								ans_arr_num[ans_count] = f_orders[p]->process_num;
								if (ans_quan2 < temp_quan2) {
									ans_quan2 = temp_quan2;
									ans_alpa2 = r_ans;
									ans_next2[ans_count] = f_orders[p]->order_name;
									next_count2 = ans_count + 1;
									next_quan2[ans_count] = ans_temp_quan2;
									pn2[ans_count] = f_orders[p]->process_num;
								}
								ans_count++;
							}
						}
						f_orders[p] = f_orders[p]->link;
					}
					f_orders[p] = temp;
				}
				cout << "유효성 검사 시작" << endl;
				while (ans_num < ans_count) {
					for (int r = ans_num + 1; r < ans_count; r++) {
						std::cout << ans_arr[ans_num] << "와" << '\t' << ans_arr[r] << endl;
						ans_temp = setResult(ans_arr[ans_num], ans_arr[r], ans_arr_num[ans_num], ans_arr_num[r]);
						if (ans_temp >= taw) {
							std::cout << ans_temp << endl;
						}
						else {
							cout << "타우값보다 작습니다." << endl;
							std::cout << ans_temp << endl;
						}
						ans_temp = 0;
					}
					ans_num++;
				}
				cout << "quantity는 " << temp_quan2 << " 입니다." << endl;
				temp_quan2 = 0;
				temp_ans.erase();
				r_ans.erase();
				printf("%d번째 \n좌표는", (col_save - 1) * 8 + i + 1);
				cor((col_save - 1) * 8 + i + 1);
				printf("값은 %d\n\n", qun[(col_save - 1) * 8 + i]);
				ans_count = 0;
				ans_num = 0;
			}
		}
	}

	if (ans_quan1 > ans_quan2) {
		ans_quan = ans_quan1;
		ans_alpa = ans_alpa1;
		for (int i = 0; i < next_count1; i++) {
			ans_next[i] = ans_next1[i];
			next_quan[i] = next_quan1[i];
			pn[i] = pn1[i];
		}
		next_count = next_count1;
		printf("가장 큰 값은 %d 입니다.\n", ans_quan1);
		std::cout << "프로세스는 " << ans_alpa1 << " 입니다." << endl;
	}
	else {
		ans_quan = ans_quan2;
		ans_alpa = ans_alpa2;
		for (int i = 0; i < next_count2; i++) {
			ans_next[i] = ans_next2[i];
			next_quan[i] = next_quan2[i];
			pn[i] = pn2[i];
		}
		next_count = next_count2;
		printf("가장 큰 값은 %d 입니다.\n", ans_quan2);
		std::cout << "프로세스는 " << ans_alpa2 << " 입니다." << endl;
	}

	cout << "\n" << "프로세스를 포함한 것들은 다음과 같습니다." << endl;

	for (int i = 0; i < next_count; i++) {
		std::cout << ans_next[i] << endl;
	}
	cout << "이것을 분해하면 다음과 같습니다. " << endl;

	const char *next;
	int alphabet1_n = 0;
	int alphabet2_n = 0;
	int alphabet_count1 = 0;
	int alphabet_count2 = 0;
	int new_num = 0;



	string alphabet1, alphabet2, alphabet;
	for (int i = 0; i < next_count; i++) {
		next = ans_next[i].c_str();
		cout << next << " 분해," << " quantity는 " << next_quan[i] << " 입니다." << endl;
		for (int j = 0; j < pn[i] - 1; j++) {
			for (int l = j + 1; l < pn[i]; l++) {
				alphabet1 = next[j];
				alphabet2 = next[l];
				alphabet1_n = next[j];
				alphabet2_n = next[l];
				if (alphabet1 != alphabet2) {
					alphabet = alphabet1 + alphabet2;
					std::cout << alphabet << endl;
					for (int i = 65; i < 73; i++) {
						if (i == alphabet1_n) {
							break;
						}
						alphabet_count1++;
					}
					for (int i = 65; i < 73; i++) {

						if (i == alphabet2_n) {

							break;
						}
						alphabet_count2++;
					}
					new_num = alphabet_count1 * 8 + alphabet_count2;
					cout << "원래 값은 " << qun[new_num] << "입니다." << endl;
					if (qun[new_num] > 0) {
						qun[new_num] -= next_quan[i];
					}
					cout << "바뀐 값은 " << qun[new_num] << "입니다." << endl;
					alphabet_count1 = 0;
					alphabet_count2 = 0;
					break;
				}
			}
		}
	}


	cout << "여기서부터는 행렬입니다." << endl;
	printf("\t");
	for (int j = 97; j < 105; j++) {
		printf("%c \t", j);
	}
	printf("\n");

	row_num = 97;

	for (int i = 0; i < 64; i++) {
		if ((column % 8) == 1) {
			printf("%c \t", row_num);
			row_num++;
		}
		printf("%d \t", qun);
		if ((column % 8) == 0) {
			cout << endl;
		}
		column++;
	}

	*/
}

