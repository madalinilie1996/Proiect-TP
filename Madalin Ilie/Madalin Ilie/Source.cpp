#include <stdio.h>
#include <time.h>
#include <Windows.h>

#define IDB_BITMAP1			2501
#define IDB_BITMAP2			2600
#define IDC_BUTTON1			2502
#define IDC_BUTTON2			2503
#define IDC_BUTTON3			2504
#define IDC_BUTTON4			2505
#define IDC_BUTTON5			2506
#define IDC_BUTTON6			2507
#define IDC_BUTTON7			2508
#define IDC_ABOUT			2509
#define IDC_INSERT1			2510
#define IDC_INSERT2			2510
#define IDC_INSERT3			2510


#define maxn 100005
#define n 2000
#define true 1
#define false 0

int iint_nr1[5], iint_nr2[5], iint_nr3[5];
HWND tab1, tab2, tab3;
int nr_graf1 = 0, nr_graf2 = 0, nr_graf3 = 0;
int okk1 = 0, okk2 = 0, okk3 = 0;

HINSTANCE _hInstace;

int a[maxn]; //Vector de numere

// Vector de gap-uri folosite
int gap[8] = { 701, 301, 132, 57, 23, 10, 4, 1 };

int main_closed = false;

void create_vector() {

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		a[i] = rand() % 10000;
	}

}

// Sortare pe vector ------------------------------------------------
void shell_vector()
{
	int aux;
	for (int k = 0; k<8; k++)
	{
		for (int i = gap[k]; i<n; i++)
		{
			int j;
			aux = a[i];
			for (j = i; j >= gap[k] && a[j - gap[k]]>aux; j -= gap[k])
				a[j] = a[j - gap[k]];

			a[j] = aux;
		}
	}
}



// Sortare pe lista ---------------------------------------------------
struct node
{
	int inf;
	struct node* next;
} *start = NULL;

struct node* create_node()
{
	struct node *q;
	q = (struct node*)malloc(sizeof(struct node));
	q->inf = 0;
	q->next = NULL;

	return q;
}

void insert_node(int val)
{
	if (start == NULL)
	{
		start = create_node();
		start->inf = val;
		return;
	}

	struct node *p, *q;
	for (p = start; p->next != NULL; p = p->next);

	q = create_node();
	q->inf = val;

	p->next = q;
}

struct node* get_node(int pos)
{
	struct node *p = start;
	for (int i = 0; i<pos; i++) p = p->next;

	return p;
}

void shell_list()
{
	for (int i = 0; i<n; i++) insert_node(a[i]);

	int aux;
	for (int k = 0; k<8; k++)
	{
		for (int i = gap[k]; i<n; i++)
		{
			int j;
			aux = get_node(i)->inf;

			for (j = i; j >= gap[k] && get_node(j - gap[k])->inf>aux; j -= gap[k])
				get_node(i)->inf = get_node(j - gap[k])->inf;

			get_node(j)->inf = aux;
		}
	}
}

// ---------------------------------------------------------------------------

// Sortare pe lista dublu inlantuita -----------------------------------------
struct dnode
{
	int inf;
	struct dnode *prev, *next;
};
struct dnode *dstart = NULL;

struct dnode* create_dnode()
{
	struct dnode *q;
	q = (struct dnode*)malloc(sizeof(struct dnode));
	q->inf = 0;
	q->prev = NULL;
	q->next = NULL;

	return q;
}

void insert_dnode(int val)
{
	if (dstart == NULL)
	{
		dstart = create_dnode();
		dstart->inf = val;
		return;
	}

	struct dnode *p, *q;
	for (p = dstart; p->next != NULL; p = p->next);

	q = create_dnode();
	q->inf = val;

	p->next = q;
	q->prev = p;
}

struct dnode* get_dnode(int pos)
{
	struct dnode *p = dstart;
	for (int i = 0; i<pos; i++) p = p->next;

	return p;
}

struct dnode* get_prev(struct dnode *cnt, int pos)
{
	struct dnode *p = cnt;
	for (int i = 0; i<pos; i++) p = p->prev;

	return p;
}

void shell_dlist()
{
	for (int i = 0; i<n; i++) insert_dnode(a[i]);

	int aux;
	for (int k = 0; k<8; k++)
	{
		for (int i = gap[k]; i<n; i++)
		{
			int j;

			struct dnode *inode = get_dnode(i), *jnode, *new_jnode;
			aux = inode->inf;

			jnode = inode;
			for (j = i; j >= gap[k]; j -= gap[k])
			{
				new_jnode = get_prev(jnode, gap[k]);
				if (new_jnode->inf <= aux) break;

				jnode->inf = new_jnode->inf;
				jnode = new_jnode;
			}

			jnode->inf = aux;
		}
	}
}

// -----------------------------------------------------------

WNDCLASSW wc, ws1, ws2, ws3, ws4, ws5, ws6, ws7;
HINSTANCE	ghAppInst = 0;

#include <windows.h>

LRESULT CALLBACK Main(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLS(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LSLD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLSLD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Proc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Proc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Proc3(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

clock_t v_start, v_end;
clock_t ls_start, ls_end;
clock_t ld_start, ld_end;
int v_elapse, ls_elapse, ld_elapse;

//Functia de intrare in program
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	MSG  msg;
	HWND hwnd;		//handle catre fereasra principala
	_hInstace = hInstance;

	create_vector();

	v_start = clock();
	shell_vector();
	v_end = clock();
	v_elapse = v_end - v_start;

	ls_start = clock();
	shell_list();
	ls_end = clock();
	ls_elapse = ls_end - ls_start;

	ld_start = clock();
	shell_dlist();
	ld_end = clock();
	ld_elapse = ld_end - ld_start;

	//parent
	//initializam fereastra 
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"Parent";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = Main;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//inregitram fereastra
	RegisterClassW(&wc);
	//creem fereastra
	hwnd = CreateWindowW(L"Parent", L"Shell Sort",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 1000, 600, NULL, NULL, hInstance, NULL);

	//child 1

	ws1.style = CS_HREDRAW | CS_VREDRAW;
	ws1.cbClsExtra = 0;
	ws1.cbWndExtra = 0;
	ws1.lpszClassName = L"Child1";
	ws1.hInstance = hInstance;
	ws1.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	ws1.lpszMenuName = NULL;
	ws1.lpfnWndProc = VLS;
	ws1.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws1.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&ws1);

	//child 2

	ws2.style = CS_HREDRAW | CS_VREDRAW;
	ws2.cbClsExtra = 0;
	ws2.cbWndExtra = 0;
	ws2.lpszClassName = L"Child2";
	ws2.hInstance = hInstance;
	ws2.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	ws2.lpszMenuName = NULL;
	ws2.lpfnWndProc = VLD;
	ws2.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws2.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&ws2);

	//child 3

	ws3.style = CS_HREDRAW | CS_VREDRAW;
	ws3.cbClsExtra = 0;
	ws3.cbWndExtra = 0;
	ws3.lpszClassName = L"Child3";
	ws3.hInstance = hInstance;
	ws3.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	ws3.lpszMenuName = NULL;
	ws3.lpfnWndProc = LSLD;
	ws3.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws3.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&ws3);

	//child 4

	ws4.style = CS_HREDRAW | CS_VREDRAW;
	ws4.cbClsExtra = 0;
	ws4.cbWndExtra = 0;
	ws4.lpszClassName = L"Child4";
	ws4.hInstance = hInstance;
	ws4.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	ws4.lpszMenuName = NULL;
	ws4.lpfnWndProc = VLSLD;
	ws4.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws4.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&ws4);

	//child 5

	ws5.style = CS_HREDRAW | CS_VREDRAW;
	ws5.cbClsExtra = 0;
	ws5.cbWndExtra = 0;
	ws5.lpszClassName = L"Child5";
	ws5.hInstance = hInstance;
	ws5.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	ws5.lpszMenuName = NULL;
	ws5.lpfnWndProc = Proc1;
	ws5.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws5.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&ws5);

	//child 6

	ws6.style = CS_HREDRAW | CS_VREDRAW;
	ws6.cbClsExtra = 0;
	ws6.cbWndExtra = 0;
	ws6.lpszClassName = L"Child6";
	ws6.hInstance = hInstance;
	ws6.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	ws6.lpszMenuName = NULL;
	ws6.lpfnWndProc = Proc2;
	ws6.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws6.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&ws6);

	//child 7

	ws7.style = CS_HREDRAW | CS_VREDRAW;
	ws7.cbClsExtra = 0;
	ws7.cbWndExtra = 0;
	ws7.lpszClassName = L"Child7";
	ws7.hInstance = hInstance;
	ws7.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	ws7.lpszMenuName = NULL;
	ws7.lpfnWndProc = Proc3;
	ws7.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws7.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&ws7);
	//message loopul
	while (main_closed == false){

		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void DrawMenu(HWND hwnd) {

	PAINTSTRUCT ps;

	DWORD color;
	HFONT hFont, holdFont;
	//incepem sa picam in fereastra principala
	HDC hdc = BeginPaint(hwnd, &ps);

	HBITMAP bmpExercising;
	HDC MemDCExercising;
	//Afisam imagine de fundal
	bmpExercising = LoadBitmap(_hInstace, MAKEINTRESOURCE(IDB_BITMAP1));
	MemDCExercising = CreateCompatibleDC(hdc);
	SelectObject(MemDCExercising, bmpExercising);
	BitBlt(hdc, 11, 15, 960, 521, MemDCExercising, 0, 0, SRCCOPY);

	DeleteDC(MemDCExercising);
	DeleteObject(bmpExercising);

	hFont = CreateFontW(60, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = (HFONT)SelectObject(hdc, hFont);


	static wchar_t *title = L"Shell Sort";
	//Creem butoanle
	CreateWindowW(L"BUTTON", L"About", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 100, 130, 36, hwnd, (HMENU)IDC_ABOUT, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"BUTTON", L"V-LS", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 150, 130, 36, hwnd, (HMENU)IDC_BUTTON1, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"BUTTON", L"V-LD", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 200, 130, 36, hwnd, (HMENU)IDC_BUTTON2, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"BUTTON", L"LS-LD", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 250, 130, 36, hwnd, (HMENU)IDC_BUTTON3, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"BUTTON", L"V-LS-LD", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 300, 130, 36, hwnd, (HMENU)IDC_BUTTON4, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"BUTTON", L"Vector", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 350, 130, 36, hwnd, (HMENU)IDC_BUTTON5, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"BUTTON", L"Liste Simplu", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 400, 130, 36, hwnd, (HMENU)IDC_BUTTON6, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"BUTTON", L"Liste Dublu", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 450, 130, 36, hwnd, (HMENU)IDC_BUTTON7, GetModuleHandle(NULL), NULL);
	color = GetSysColor(COLOR_BTNFACE);
	SetBkColor(hdc, color);

	TextOutW(hdc, 380, 0, title, lstrlenW(title));
	//terminam cu pictatul
	EndPaint(hwnd, &ps);
}

//Procedura pentru fereastra principala(CE face fereastra)
LRESULT CALLBACK Main(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		DrawMenu(hwnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_ABOUT:
			MessageBox(hwnd, "Creat de Ilie Madalin!", "About", MB_ICONEXCLAMATION);
			break;
		case IDC_BUTTON1:
			CreateWindowW(ws1.lpszClassName, L"VLS",
				WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				0, 0, 1120, 700, NULL, NULL, ghAppInst, NULL);
			break;
		case IDC_BUTTON2:
			CreateWindowW(ws2.lpszClassName, L"VLD",
				WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				0, 0, 1120, 700, NULL, NULL, ghAppInst, NULL);
			break;
		case IDC_BUTTON3:
			CreateWindowW(ws3.lpszClassName, L"LSLD",
				WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				0, 0, 1120, 700, NULL, NULL, ghAppInst, NULL);
			break;
		case IDC_BUTTON4:
			CreateWindowW(ws4.lpszClassName, L"VLSLD",
				WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				0, 0, 1120, 700, NULL, NULL, ghAppInst, NULL);
			break;
		case IDC_BUTTON5:
			CreateWindowW(ws5.lpszClassName, L"Proc1",
				WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				0, 0, 1120, 700, NULL, NULL, ghAppInst, NULL);
			break;
		case IDC_BUTTON6:
			CreateWindowW(ws6.lpszClassName, L"Proc2",
				WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				0, 0, 1120, 700, NULL, NULL, ghAppInst, NULL);
			break;
		case IDC_BUTTON7:
			CreateWindowW(ws7.lpszClassName, L"Proc3",
				WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				0, 0, 1120, 700, NULL, NULL, ghAppInst, NULL);
			break;
		}
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		main_closed = true;
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Draw_coordinates(HWND hwnd, int child) {

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;

	HBITMAP bmpExercising;
	HDC MemDCExercising;
	//Incarcam backgoud 2
	bmpExercising = LoadBitmap(_hInstace, MAKEINTRESOURCE(IDB_BITMAP2));
	MemDCExercising = CreateCompatibleDC(hdc);
	SelectObject(MemDCExercising, bmpExercising);
	BitBlt(hdc, 0, 0, 1120, 700, MemDCExercising, 0, 0, SRCCOPY);

	DeleteDC(MemDCExercising);
	DeleteObject(bmpExercising);
	//electam sa avem o linie de culoare rosie
	qLineColor = RGB(255, 0, 0);
	hLinePen = CreatePen(PS_SOLID, 8, qLineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	//vertical
	MoveToEx(hdc, 100, 600, NULL);
	LineTo(hdc, 100, 100);

	//horizontal
	MoveToEx(hdc, 100, 600, NULL);
	LineTo(hdc, 900, 600);

	//arrows
	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 120, 120);

	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 80, 120);

	MoveToEx(hdc, 900, 600, NULL);
	LineTo(hdc, 880, 580);

	MoveToEx(hdc, 900, 600, NULL);
	LineTo(hdc, 880, 620);

	DWORD color;
	HFONT hFont, holdFont;
	hFont = CreateFontW(50, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = (HFONT)SelectObject(hdc, hFont);
	color = GetSysColor(COLOR_BTNFACE);
	SetBkColor(hdc, color);

	static wchar_t *time = L"Time";
	static wchar_t *data = L"Data";
	//Creez hasira pentru grafic
	HBRUSH      hasura = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 255));
	SelectObject(hdc, hasura);

	TextOutW(hdc, 60, 45, time, lstrlenW(time));
	TextOutW(hdc, 910, 570, data, lstrlenW(data));

	static wchar_t *vector = L"Vector";
	static wchar_t *ls = L"Liste simplu";
	static wchar_t *ld = L"Liste dublu";

	//histograms

	switch (child) {
	case 1:
		//Afsam grafic pentru fereastr V-LS
		if (v_elapse == 0) Rectangle(hdc, 300, 600, 350, 580);
		else Rectangle(hdc, 300, 600, 350, 580 - v_elapse);
		Rectangle(hdc, 540, 600, 590, 600 - ls_elapse);

		TextOutW(hdc, 255, 603, vector, lstrlenW(vector));
		TextOutW(hdc, 450, 603, ls, lstrlenW(ls));

		break;
	case 2:
		//Afsam grafic pentru fereastr V-LD
		if (v_elapse == 0) Rectangle(hdc, 400, 600, 450, 580);
		else Rectangle(hdc, 500, 600, 550, 580 - v_elapse);
		Rectangle(hdc, 700, 600, 750, 600 - ld_elapse);

		TextOutW(hdc, 360, 603, vector, lstrlenW(vector));
		TextOutW(hdc, 620, 603, ld, lstrlenW(ld));

		break;
	case 3:
		//Afsam grafic pentru fereastr LD-LS
		Rectangle(hdc, 400, 600, 450, 600 - ls_elapse);
		Rectangle(hdc, 700, 600, 750, 600 - ld_elapse);

		TextOutW(hdc, 320, 603, ls, lstrlenW(ls));
		TextOutW(hdc, 620, 603, ld, lstrlenW(ld));

		break;
	case 4:
		//Afsam grafic pentru fereastr V-LS-LD
		if (v_elapse == 0) Rectangle(hdc, 180, 600, 230, 590);
		else Rectangle(hdc, 180, 600, 230, 590 - v_elapse);
		Rectangle(hdc, 440, 600, 490, 600 - ls_elapse);
		Rectangle(hdc, 740, 600, 790, 600 - ld_elapse);

		TextOutW(hdc, 150, 603, vector, lstrlenW(vector));
		TextOutW(hdc, 340, 603, ls, lstrlenW(ls));
		TextOutW(hdc, 650, 603, ld, lstrlenW(ld));

		break;
	}



	EndPaint(hwnd, &ps);

}

LRESULT CALLBACK VLS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Draw_coordinates(hwnd, 1);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VLD(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Draw_coordinates(hwnd, 2);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK LSLD(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Draw_coordinates(hwnd, 3);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VLSLD(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Draw_coordinates(hwnd, 4);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void deseneaza_iteratie(HWND hwnd, int *numar_elemente, PAINTSTRUCT ps, HDC hdc, HPEN hPenOld, HPEN hLinePen, COLORREF qLineColor, HBRUSH culoare_interior, int k, int nr_itee)
{
	qLineColor = RGB(113, 200, 13);
	hLinePen = CreatePen(PS_SOLID, 6, qLineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	culoare_interior = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, culoare_interior);
	//Luam datele din chenar
	for (int i = 0; i < nr_itee; i++)
	{
		char buff[5];
		sprintf(buff, "%d", numar_elemente[i]);
		CreateWindowEx(WS_EX_CLIENTEDGE, "static", buff, WS_CHILD | WS_VISIBLE, 103 + 133 * (i + 1), 610, 35, 26, hwnd, (HMENU)1, NULL, NULL);
		Rectangle(hdc, 100 + 133 * (i + 1), 600, 140 + 133 * (i + 1), 600 - numar_elemente[i] / k);
	}
	//Setam conturului culoare graficului
	qLineColor = RGB(222, 111, 12);
	hLinePen = CreatePen(PS_SOLID, 6, qLineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);
	//Putem daele pe grafic
	for (int i = 0; i < nr_itee - 1; i++)
	{
		if (numar_elemente[i] < 7)
		{
			MoveToEx(hdc, 120 + 133 * (i + 1), 599 - numar_elemente[i] / k, NULL);
			LineTo(hdc, 120 + 133 * (i + 2), 599 - numar_elemente[i + 1] / k);
		}
		else
		{
			MoveToEx(hdc, 120 + 133 * (i + 1), 605 - numar_elemente[i] / k, NULL);
			LineTo(hdc, 120 + 133 * (i + 2), 605 - numar_elemente[i + 1] / k);
		}
	}
}
void Draw_coordinates2(HWND hwnd) {

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;

	HBITMAP bmpExercising;
	HDC MemDCExercising;

	bmpExercising = LoadBitmap(_hInstace, MAKEINTRESOURCE(IDB_BITMAP2));
	MemDCExercising = CreateCompatibleDC(hdc);
	SelectObject(MemDCExercising, bmpExercising);
	BitBlt(hdc, 0, 0, 1120, 700, MemDCExercising, 0, 0, SRCCOPY);

	DeleteDC(MemDCExercising);
	DeleteObject(bmpExercising);

	qLineColor = RGB(113, 200, 13);
	hLinePen = CreatePen(PS_SOLID, 6, qLineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	//vertical
	MoveToEx(hdc, 100, 600, NULL);
	LineTo(hdc, 100, 100);

	//horizontal
	MoveToEx(hdc, 100, 600, NULL);
	LineTo(hdc, 900, 600);

	//arrows

	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 120, 120);

	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 80, 120);

	MoveToEx(hdc, 900, 600, NULL);
	LineTo(hdc, 880, 580);

	MoveToEx(hdc, 900, 600, NULL);
	LineTo(hdc, 880, 620);

	DWORD color;
	HFONT hFont, holdFont;
	hFont = CreateFontW(50, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = (HFONT)SelectObject(hdc, hFont);
	color = GetSysColor(COLOR_BTNFACE);
	SetBkColor(hdc, color);

	static wchar_t *time = L"Time";
	static wchar_t *data = L"Data";

	HBRUSH      hasura = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 255));
	SelectObject(hdc, hasura);

	TextOutW(hdc, 60, 45, time, lstrlenW(time));
	TextOutW(hdc, 910, 570, data, lstrlenW(data));

	static wchar_t *vector = L"Vector";
	static wchar_t *ls = L"Liste simplu";
	static wchar_t *ld = L"Liste dublu";



	//*******************
	if (okk1 == 1)
		deseneaza_iteratie(hwnd, iint_nr1, ps, hdc, hPenOld, hLinePen, qLineColor, hasura, 5, nr_graf1);
	if (okk2 == 1)
		deseneaza_iteratie(hwnd, iint_nr2, ps, hdc, hPenOld, hLinePen, qLineColor, hasura, 1, nr_graf2);
	if (okk3 == 1)
		deseneaza_iteratie(hwnd, iint_nr3, ps, hdc, hPenOld, hLinePen, qLineColor, hasura, 2, nr_graf3);
	//----------------------

	EndPaint(hwnd, &ps);

}
bool verifica_cifra(char *sir){
	if (strlen(sir) == 0)
		return false;
	for (int i = 0; i < strlen(sir); i++)
	{
		if (!isdigit(sir[i]))
			return false;
	}
	return true;
}

LRESULT CALLBACK Proc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Draw_coordinates2(hwnd);
		break;
	case WM_CREATE:
		//Desenam butnul insert chenarul unde puem introduce text si textul static
		CreateWindowW(L"BUTTON", L"Insert", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 1000, 32, 100, 30, hwnd, (HMENU)IDC_INSERT1, GetModuleHandle(NULL), NULL);
		CreateWindowW(L"static", L"Introdu date pentru a fi inserat in chenar (max 5): ", WS_CHILD | WS_VISIBLE, 600, 4, 396, 26, hwnd, (HMENU)1, NULL, NULL);
		tab1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 1000, 1, 100, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_INSERT1:
			if (nr_graf1 == 5)
			{
				MessageBoxW(hwnd, L"Maxim 5 iteratii", L"ERROR", MB_ICONERROR);
			}
			else
			{
				char buffer[10];
				for (int i = 0; i <= 9; i++)
					buffer[i] = '\0';
				SendMessage(tab1, WM_GETTEXT, sizeof(buffer) / sizeof(char), reinterpret_cast<LPARAM>(buffer));
				if (nr_graf1 < 5)
				{
					if (verifica_cifra(buffer) && atoi(buffer) <= 500)
					{
						iint_nr1[nr_graf1] = atoi(buffer);
						okk1 = 1;
						nr_graf1++;
						//Actualizam desenul
						InvalidateRect(hwnd, NULL, TRUE);
					}
					else
					{
						MessageBoxW(hwnd, L"Introdu un numar intre 0 si 500", L"Error", MB_ICONERROR);
					}
				}
				else
				{
					MessageBoxW(hwnd, L"Maxim 5 iteratii!", L"Error", MB_ICONERROR);
				}
			}
			break;
		}
		break;
	case WM_DESTROY:
		nr_graf1 = 0;
		okk1 = 0;
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Proc3(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Draw_coordinates2(hwnd);
		break;
	case WM_CREATE:
		CreateWindowW(L"BUTTON", L"Insert", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 1000, 32, 100, 30, hwnd, (HMENU)IDC_INSERT1, GetModuleHandle(NULL), NULL);
		CreateWindowW(L"static", L"Introdu date pentru a fi inserat in chenar (max 5): ", WS_CHILD | WS_VISIBLE, 600, 4, 396, 26, hwnd, (HMENU)1, NULL, NULL);
		tab1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 1000, 1, 100, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_INSERT1:
			if (nr_graf3 == 5)
			{
				MessageBoxW(hwnd, L"Maxim 5 iteratii", L"ERROR", MB_ICONERROR);
			}
			else
			{
				char buffer[10];
				for (int i = 0; i <= 9; i++)
					buffer[i] = '\0';
				SendMessage(tab1, WM_GETTEXT, sizeof(buffer) / sizeof(char), reinterpret_cast<LPARAM>(buffer));
				if (nr_graf3 < 5)
				{
					if (verifica_cifra(buffer) && atoi(buffer) <= 500)
					{
						iint_nr3[nr_graf3] = atoi(buffer);
						okk3 = 1;
						nr_graf3++;
						InvalidateRect(hwnd, NULL, TRUE);
					}
					else
					{
						MessageBoxW(hwnd, L"Introdu un numar intre 0 si 500", L"Error", MB_ICONERROR);
					}
				}
				else
				{
					MessageBoxW(hwnd, L"Maxim 5 iteratii!", L"Error", MB_ICONERROR);
				}
			}
			break;
		}
		break;
	case WM_DESTROY:
		nr_graf3 = 0;
		okk3 = 0;
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Proc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_PAINT:
		Draw_coordinates2(hwnd);
		break;
	case WM_CREATE:
		CreateWindowW(L"BUTTON", L"Insert", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 1000, 32, 100, 30, hwnd, (HMENU)IDC_INSERT1, GetModuleHandle(NULL), NULL);
		CreateWindowW(L"static", L"Introdu date pentru a fi inserat in chenar (max 5): ", WS_CHILD | WS_VISIBLE, 600, 4, 396, 26, hwnd, (HMENU)1, NULL, NULL);
		tab1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 1000, 1, 100, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_INSERT1:
			if (nr_graf2 == 5)
			{
				MessageBoxW(hwnd, L"Maxim 5 iteratii", L"ERROR", MB_ICONERROR);
			}
			else
			{
				char buffer[10];
				for (int i = 0; i <= 9; i++)
					buffer[i] = '\0';
				SendMessage(tab1, WM_GETTEXT, sizeof(buffer) / sizeof(char), reinterpret_cast<LPARAM>(buffer));
				if (nr_graf2 < 5)
				{
					if (verifica_cifra(buffer) && atoi(buffer) <= 500)
					{
						iint_nr2[nr_graf2] = atoi(buffer);
						okk2 = 1;
						nr_graf2++;
						InvalidateRect(hwnd, NULL, TRUE);
					}
					else
					{
						MessageBoxW(hwnd, L"Introdu un numar intre 0 si 500", L"Error", MB_ICONERROR);
					}
				}
				else
				{
					MessageBoxW(hwnd, L"Maxim 5 iteratii!", L"Error", MB_ICONERROR);
				}
			}
			break;
		}
		break;
	case WM_DESTROY:
		nr_graf2 = 0;
		okk2 = 0;
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}