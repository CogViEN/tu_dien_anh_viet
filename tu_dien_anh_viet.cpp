#include<iostream>
using namespace std;
#include<string>
#include<fstream>
#include"Do_Hoa.h"
#include<vector>
#define M 26
// khai báo cấu trúc một từ anh viêt
struct Word
{
	string english; // từ tiếng anh
	string Vietnamese; // nghĩa tiếng việt
	string type; // từ loại
};

// khai báo một node từ điển
struct NODE_dic
{
	Word word;
	NODE_dic *next;
};

//=============== Nguyên Mẫu hàm ================
istream& operator >>(istream& is, Word &w);
void XoaHetBangTienDoan();
void disPlayBucket(NODE_dic *dic, string s, int &max_tu, vector<Word> &ds_tien_doan);
void Xu_Li_Thanh_Sang(int x_chu, int y_chu, string nd);
void box(int x, int y, int w, int h, int t_color, int b_color, string nd);
Word tra_ve_tu_dang_o_thanh_sang_i(vector<Word> ds_tien_doan, int vt_sang);
void Xoa_Het_Thanh_Sang();
int kt_tu_can_tim_co_ton_tai(NODE_dic *dic, string tu_can_tim);
//====================

// khởi tạo danh sách  các danh sách liên kết đơn
void init(NODE_dic *heads[])
{
	for (int i = 0; i < M; i++)
		heads[i] = NULL;
}

int hashFunc(string w_english)
{
	int value = (int)tolower(w_english[0]);
	/*for (int i = 0; i < w_english.length(); i++)
		value += (int)tolower(w_english[i]) * (i+1);*/
	return value % M;
}
NODE_dic *createNode_dict(Word w)
{
	NODE_dic *p = new NODE_dic;
	p->next = NULL;
	p->word = w;
	return p;
}
void addWord(NODE_dic *heads[],Word w)
{
	int index = hashFunc(w.english);
	NODE_dic *p = heads[index];
	NODE_dic *temp = createNode_dict(w); // node được tạo ra để thêm vào
	// danh sách rỗng
	if (p == NULL)
	{
		heads[index] = temp;
		return;
	}
	// trường hợp đặc biệt danh sách trong ô đó chỉ có 1 phần tử mà phần cần thêm vào bằng phần tử đầu tiên trong danh sách cần thêm vào
	if (p != NULL && p->next == NULL)
	{
		if (_stricmp(p->word.english.c_str(), w.english.c_str()) == 0)
		{
			if (_stricmp(p->word.type.c_str(), w.type.c_str()) == 0)
				return;
		}
	}
	while (p->next != NULL)
	{
		// nếu là trùng từ tiếng anh mà trùng cả loại từ thì kết thúc hàm
		if (_stricmp(p->word.english.c_str(), w.english.c_str()) == 0)
		{
			if (_stricmp(p->word.type.c_str(), w.type.c_str()) == 0)
				return;
		}
		p = p->next;
	}
	p->next = temp; // cho node cuối danh sách liên kết với node vừa được tạo ra
}
void disPlay(NODE_dic *heads[])
{
	for (int i = 0; i < M; i++)
	{
		if (heads[i] != NULL)
		{
			cout << "\n\t BUCKET " << i << "   \n";
			NODE_dic *p = heads[i];
			while (p != NULL)
			{
				cout << p->word.english << " (" << p->word.type << ") : " << p->word.Vietnamese << endl;
				p = p->next;
			}
			cout << "==============" <<  endl;
		}
	}
}
istream& operator >>(istream& is, Word &w)
{
	cout << "Nhap tu tieng anh: "; getline(is, w.english);
	cout << "Nhap nghia tieng viet: "; getline(is, w.Vietnamese);
	cout << "Nhap tu loai: "; getline(is, w.type);
	return is;
}
NODE_dic* search(NODE_dic *heads[], string tu_tieng_anh)
{
	int index = hashFunc(tu_tieng_anh);
	if (heads[index] == NULL)
	{
		return NULL;
	}
	NODE_dic *p = heads[index];
	while (p != NULL && _stricmp(tu_tieng_anh.c_str(), p->word.english.c_str()) != 0)
	{
		p = p->next;
	}
	return p;
}
void update_Word(NODE_dic *heads[], string tu_tieng_anh)
{
	int index = hashFunc(tu_tieng_anh);
	NODE_dic *p = heads[index];
	while (p != NULL)
	{
		if (_stricmp(p->word.english.c_str(), tu_tieng_anh.c_str()) == 0)
		{
			cout << "Nhap lai tu tieng anh: "; getline(cin, p->word.english);
			cout << "Nhap lai tu nghia tieng viet: "; getline(cin, p->word.Vietnamese);
			cout << "Nhap lai tu loai: "; getline(cin, p->word.type);
			return;
		}
		p = p->next;
	}
	cout << "\nKhong co tu " << tu_tieng_anh << " trong tu dien";
}
void deleteWord(NODE_dic *heads[], string tu_tieng_anh)
{
	int index = hashFunc(tu_tieng_anh);
	NODE_dic *prev = NULL;
	if (heads[index] == NULL)
	{
		cout << "\nKhong ton tai tu " << tu_tieng_anh << " trong tu dien";
		return;
	}
	// neu phan tu dau chinh la phan tu can xoa
	while (heads[index] != NULL && _stricmp(heads[index]->word.english.c_str(), tu_tieng_anh.c_str()) == 0)
	{
		NODE_dic *temp = heads[index];
		heads[index] = heads[index]->next;
		delete temp;
	}
	NODE_dic *p = heads[index];
	while (p != NULL)
	{
		if (_stricmp(p->word.english.c_str(), tu_tieng_anh.c_str()) == 0)
		{
			NODE_dic *temp = p;
			prev->next = p->next;
			p = prev;
			delete temp;
		}
		prev = p;
		p = p->next;
	}
}
// load file
void loadFile(NODE_dic *heads[])
{
	ifstream FileIn;
	FileIn.open("tieng_anh.txt");
	while (!FileIn.eof())
	{
		Word w;
		getline(FileIn, w.english, ',');
		getline(FileIn, w.Vietnamese, ',');
		getline(FileIn, w.type);
		addWord(heads, w);
	}
	FileIn.close();
}
void menu(NODE_dic *heads[])
{
	// load file
	loadFile(heads);
	while (66)
	{
		system("cls");
		cout << "\n\n\t\t========= TU DIEN =======\n";
		cout << "\n\t1. Them tu vao tu dien";
		cout << "\n\t2. Xuat tac ca cac tu";
		cout << "\n\t3. Tim kiem mot tu trong tu dien";
		cout << "\n\t4. Cap nhat mot tu trong tu dien";
		cout << "\n\t5. Xoa mot tu trong tu dien";
		cout << "\n\t0. Thoat";
		cout << "\n\n\t\t======================\n";
		int lc;
		cout << "\n\tNhap lua chon: ";
		cin >> lc;
		if (lc == 0)
		{
			break;
		}
		else if (lc == 1)
		{
			int n;
			cout << "Nhap so luong tu can them: ";
			cin >> n;
			cin.ignore();
			for (int i = 0; i < n; i++)
			{
				cout << "- Nhap tu thu " << i + 1 << endl;
				Word w;
				cin >> w;
				addWord(heads, w);
			}
		}
		else if (lc == 2)
		{
			disPlay(heads);
			system("pause");
		}
		else if (lc == 3)
		{
			cin.ignore();
			string tu_tieng_anh;
			cout << "\nNhap tu tieng anh can tim kiem: ";
			getline(cin, tu_tieng_anh);
			if (search(heads, tu_tieng_anh) == NULL)
			{
				cout << "\nKhong tim thay";
			}
			else
			{
				cout << search(heads, tu_tieng_anh)->word.english << " (" << search(heads, tu_tieng_anh)->word.type << ") : " << search(heads, tu_tieng_anh)->word.Vietnamese;
			}
			system("pause");
		}
		else if (lc == 5)
		{
			cin.ignore();
			string tu_tieng_anh;
			cout << "Nhap tu muon cap xoa: ";
			getline(cin, tu_tieng_anh);
			deleteWord(heads, tu_tieng_anh);
			system("pause");
		}
		else if (lc == 4)
		{
			cin.ignore();
			string tu_tieng_anh;
			cout << "Nhap tu muon cap nhat: ";
			getline(cin, tu_tieng_anh);
			update_Word(heads, tu_tieng_anh);
			system("pause");
		}
	}
}
// hàm so sánh chuỗi của tôi ( so sánh chuỗi theo chuỗi nhỏ nhất)
// sa == sad
// sa != se
// lấy từng kí tự chuỗi nhỏ so sánh với từng kí tự chuỗi lớn 
int my_compare_str(string x, string y)
{
	int main_lengh = x.length() > y.length() ? y.length() : x.length();
	for (int i = 0; i < main_lengh; i++)
	{
		if (x.at(i) != y.at(i))
		{
			return -1;
		}
	}
	return 0;
}
void disPlayBucket(NODE_dic *dic, string s,int &max_tu,vector<Word> &ds_tien_doan)
{
	max_tu = 0;
	if (s.length() == 1)
	{
		XoaHetBangTienDoan();
		int i = 10;
		for (NODE_dic *k = dic; k != NULL; k = k->next)
		{
			gotoXY(21, i++);
			cout << k->word.english;
			max_tu++;
			ds_tien_doan.push_back(k->word);
		}
	}
	else if (s.length() >= 1)
	{
		int i = 10;
		XoaHetBangTienDoan();
		for (NODE_dic *k = dic; k != NULL; k = k->next)
		{
			if (my_compare_str(s, k->word.english) == 0)
			{
				gotoXY(21, i++);
				cout << k->word.english;
				max_tu++;
				ds_tien_doan.push_back(k->word);
			}
		}
	}
}
void XoaHetBangTienDoan()
{
	for (int i = 10; i <= 25; i++)
	{
		gotoXY(21, i);
		cout << "                                                          ";
	}
}
void test(NODE_dic *heads[])
{
	Nocursortype();
	int max_tu = 0;
	int x_chu_tien_doan = 21; // toa do chu tien doan dau tien
	int y_chu_tien_doan = 10; // toa do chu tien doan dau tien
	int vt_thanh_sang = 1;
	string nd_tien_doan;
	vector<Word> ds_tien_doan;
	// load file
	loadFile(heads);
	string s;
	char key_press;
	// mở file tiêu đề
	while (66)
	{
		int ycu = y_chu_tien_doan;
		key_press = _getch();
		if (key_press != 8 && key_press != -32 && key_press != 13)
		{
			s.push_back(key_press);
			// xoá bảng tiên đoán cập nhật lại bảng tiên đoán mới
			ds_tien_doan.clear();
			y_chu_tien_doan = 10;
			// xoa het thanh sang
			Xoa_Het_Thanh_Sang();
			vt_thanh_sang = 1;
		}
		if (key_press == -32)
		{
			key_press = _getch();
			if (key_press == 72) // di len
			{
				if (y_chu_tien_doan > 10)
				{
					y_chu_tien_doan--;
					vt_thanh_sang--;
					Xoa_Het_Thanh_Sang();
				}
			}
			else if (key_press == 80) // di xuong
			{
				if (vt_thanh_sang < max_tu)
				{
					vt_thanh_sang++;
					y_chu_tien_doan++;
					Xoa_Het_Thanh_Sang();
				}
			}
		}
		if (key_press == 8)
		{
			if (!s.empty())
			{
				s.pop_back();
				gotoXY(20, 9);
				cout << "                       ";
				// xoá bảng tiên đoán cập nhật lại bảng tiên đoán mới
				ds_tien_doan.clear();
				y_chu_tien_doan = 10;
				// xoa het thanh sang
				Xoa_Het_Thanh_Sang();
				vt_thanh_sang = 1;
			}
			if (s.empty())
			{
				XoaHetBangTienDoan();
				// xoá bảng tiên đoán cập nhật lại bảng tiên đoán mới
				ds_tien_doan.clear();
				y_chu_tien_doan = 10;
				// xoa het thanh sang
				Xoa_Het_Thanh_Sang();
				vt_thanh_sang = 1;
			}
		}
		if (key_press == 13)
		{
			if (s == "tuyet nang")
			{
				while (true)
				{	
					PlaySound(TEXT("tuyetnang.wav"), NULL, SND_ASYNC);
					char c;
					c = _getch();
					if (c == 27)
					{
						break;
					}
				}
				PlaySound("NULL", NULL, SND_ASYNC);
			}
			if (!ds_tien_doan.empty() && s != "tuyet nang")
			{
				system("cls");
				while (66)
				{
					Word temp;
					temp = tra_ve_tu_dang_o_thanh_sang_i(ds_tien_doan, vt_thanh_sang);
					gotoXY(25, 15);
					SetColor(13);
					cout << temp.english << " (" << temp.type << ") : " << temp.Vietnamese;
					gotoXY(25, 10);
					SetColor(10);
					cout << "Nhan esc de thoat khoi che do xem";
					gotoXY(25, 12);
					cout << "Nhan x de nghe tu vung";
					char c;
					c = _getch();
					if (c == 'x')
					{
						if (temp.english == "active")
						{
							PlaySound("active.wav", NULL, SND_SYNC);
						}
						if (temp.english == "funny")
						{
							PlaySound("funny.wav", NULL, SND_SYNC);
						}
					}	
					Sleep(200);
					if (c == 27)
					{
						break;
					}
				}
			}
		}
		box(20, 7, 30, 2, 7, 0, s);
		int index = hashFunc(s);
		NODE_dic *check = heads[index];
		// nếu mà danh sách không rỗng thì mới thực hiện công việc này
		if (check != NULL)
		{
			if (kt_tu_can_tim_co_ton_tai(check, s) == 1)
			{
				disPlayBucket(heads[index], s, max_tu, ds_tien_doan);
				if (s.length() > 0)
				{
					nd_tien_doan = tra_ve_tu_dang_o_thanh_sang_i(ds_tien_doan, vt_thanh_sang).english;
				}
				if (s.length() >= 1)
				{
					Xu_Li_Thanh_Sang(x_chu_tien_doan, y_chu_tien_doan, nd_tien_doan);
				}
			}
		}
	}
	
}
int kt_tu_can_tim_co_ton_tai(NODE_dic *dic, string tu_can_tim)
{
	for (NODE_dic *k = dic; k != NULL; k = k->next)
	{
		if (my_compare_str(tu_can_tim, k->word.english) == 0)
		{
			return 1;
		}
	}
	return -1;
}
Word tra_ve_tu_dang_o_thanh_sang_i(vector<Word> ds_tien_doan, int vt_sang)
{
	for (int i = 0; i < ds_tien_doan.size(); i++)
	{
		if (vt_sang - 1 == i)
		{
			return ds_tien_doan[i];
		}
	}
}
void Xoa_Het_Thanh_Sang()
{
	for (int i = 10; i <= 25; i++)
	{
		HANDLE console_color;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 0);
		gotoXY(0, i);
		cout << "                                                                                                                                                    ";
	}
}
void box(int x, int y, int w, int h, int t_color, int b_color, string nd)
{
	textcolor(b_color);
	for (int iy = y + 1; iy <= y + h - 1; iy++)
	{
		for (int ix = x + 1; ix <= x + w - 1; ix++)
		{
			gotoXY(ix, iy); cout << " ";
		}
	}
	SetColor(7);
	gotoXY(x + 1, y + 1);
	cout << nd;
	//============= ve vien =============
	textcolor(1);
	SetColor(t_color);//update
	if (h <= 1 || w <= 1)return;
	for (int ix = x; ix <= x + w; ix++)
	{
		gotoXY(ix, y);
		cout << char(196);
		gotoXY(ix, y + h);
		cout << char(196);
	}
	for (int iy = y; iy <= y + h; iy++)
	{
		gotoXY(x, iy);
		cout << char(179);
		gotoXY(x + w, iy);
		cout << char(179);
	}
	gotoXY(x, y); cout << char(218);
	gotoXY(x + w, y); cout << char(191);
	gotoXY(x, y + h); cout << char(192);
	gotoXY(x + w, y + h); cout << char(217);

}
void Xu_Li_Thanh_Sang(int x_chu, int y_chu, string nd)
{
	int x_background = 0;
	int y_background = y_chu;
	// Color of the console
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 70);
	gotoXY(x_background, y_background);
	cout << "                                                                                                                                                    ";
	gotoXY(x_chu, y_chu);
	SetColor(11);
	cout << nd;
	SetConsoleTextAttribute(console_color, 0);
}
int main()
{
	box(20, 7, 30, 2, 7, 0, "tra tu dien Anh_Viet");
	// xử lí trong
	NODE_dic *heads[M];
	init(heads);
	////menu(heads);
	test(heads);
	////Xu_Li_Thanh_Sang();
}