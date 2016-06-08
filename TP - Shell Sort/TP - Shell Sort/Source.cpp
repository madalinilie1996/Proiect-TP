#include<stdio.h>
#include<time.h>
#include<Windows.h>
#define maxn 100005
#define n 2000
#define true 1
#define false 0

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
void shell_vector(){

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

void insert_node(int val){

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

void shell_list(){

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

void insert_dnode(int val){

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

WNDCLASSW wc, ws1, ws2, ws3, ws4;
HINSTANCE	ghAppInst = 0;

#include <windows.h>

LRESULT CALLBACK Main(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLS(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LSLD(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VLSLD(HWND, UINT, WPARAM, LPARAM);

clock_t v_start, v_end;
clock_t ls_start, ls_end;
clock_t ld_start, ld_end;
int v_elapse, ls_elapse, ld_elapse;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PWSTR pCmdLine, int nCmdShow) {

	MSG  msg;
	HWND hwnd;

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

	RegisterClassW(&wc);
	hwnd = CreateWindowW(wc.lpszClassName, L"Shell Sort",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 1366, 768, NULL, NULL, hInstance, NULL);

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

	HDC hdc = BeginPaint(hwnd, &ps);

	hFont = CreateFontW(60, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = (HFONT)SelectObject(hdc, hFont);


	static wchar_t *title = L"Shell Sort";
	static wchar_t *vls = L"V-LS";
	static wchar_t *vld = L"V-LD";
	static wchar_t *lsld = L"LS-LD";
	static wchar_t *vlsld = L"V-LS-LD";


	Rectangle(hdc, 350, 200, 600, 300);
	Rectangle(hdc, 350, 400, 600, 500);
	Rectangle(hdc, 766, 200, 1016, 300);
	Rectangle(hdc, 766, 400, 1016, 500);

	TextOutW(hdc, 420, 225, vls, lstrlenW(vls));
	TextOutW(hdc, 420, 425, vld, lstrlenW(vld));
	TextOutW(hdc, 820, 220, lsld, lstrlenW(lsld));
	TextOutW(hdc, 800, 425, vlsld, lstrlenW(vlsld));

	color = GetSysColor(COLOR_BTNFACE);
	SetBkColor(hdc, color);

	TextOutW(hdc, 570, 80, title, lstrlenW(title));

	EndPaint(hwnd, &ps);
}

void open_window(POINT p) {

	if (p.x>350 && p.x<600 && p.y>200 && p.y<300)
		CreateWindowW(ws1.lpszClassName, L"VLS",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 1366, 768, NULL, NULL, ghAppInst, NULL);

	if (p.x>350 && p.x<600 && p.y>400 && p.y<500)
		CreateWindowW(ws2.lpszClassName, L"VLD",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 1366, 768, NULL, NULL, ghAppInst, NULL);

	if (p.x>766 && p.x<1016 && p.y>200 && p.y<300)
		CreateWindowW(ws3.lpszClassName, L"LSLD",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 1366, 768, NULL, NULL, ghAppInst, NULL);

	if (p.x>766 && p.x<1016 && p.y>400 && p.y<500)
		CreateWindowW(ws4.lpszClassName, L"VLSLD",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 1366, 768, NULL, NULL, ghAppInst, NULL);

}

LRESULT CALLBACK Main(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	POINT p;

	switch (msg) {

	case WM_LBUTTONDOWN:

		GetCursorPos(&p);
		open_window(p);

		break;

	case WM_PAINT:
		DrawMenu(hwnd);
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

	//vertical
	MoveToEx(hdc, 200, 600, NULL);
	LineTo(hdc, 200, 100);

	//horizontal
	MoveToEx(hdc, 200, 600, NULL);
	LineTo(hdc, 1000, 600);

	//arrows

	MoveToEx(hdc, 200, 100, NULL);
	LineTo(hdc, 220, 120);

	MoveToEx(hdc, 200, 100, NULL);
	LineTo(hdc, 180, 120);

	MoveToEx(hdc, 1000, 600, NULL);
	LineTo(hdc, 980, 580);

	MoveToEx(hdc, 1000, 600, NULL);
	LineTo(hdc, 980, 620);

	DWORD color;
	HFONT hFont, holdFont;
	hFont = CreateFontW(50, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
		0, 0, 0, 0, L"Font");
	holdFont = (HFONT)SelectObject(hdc, hFont);
	color = GetSysColor(COLOR_BTNFACE);
	SetBkColor(hdc, color);

	static wchar_t *time = L"time";
	static wchar_t *data = L"data";

	TextOutW(hdc, 160, 50, time, lstrlenW(time));
	TextOutW(hdc, 1010, 570, data, lstrlenW(data));

	static wchar_t *vector = L"vector";
	static wchar_t *ls = L"Liste simplu";
	static wchar_t *ld = L"Liste dublu";

	//histograms

	switch (child) {
	case 1:
		if (v_elapse == 0) Rectangle(hdc, 400, 600, 450, 599);
		else Rectangle(hdc, 500, 600, 550, 600 - v_elapse);
		Rectangle(hdc, 700, 600, 750, 600 - ls_elapse);

		TextOutW(hdc, 380, 620, vector, lstrlenW(vector));
		TextOutW(hdc, 620, 620, ls, lstrlenW(ls));

		break;
	case 2:
		if (v_elapse == 0) Rectangle(hdc, 400, 600, 450, 599);
		else Rectangle(hdc, 500, 600, 550, 600 - v_elapse);
		Rectangle(hdc, 700, 600, 750, 600 - ld_elapse);

		TextOutW(hdc, 380, 620, vector, lstrlenW(vector));
		TextOutW(hdc, 620, 620, ld, lstrlenW(ld));

		break;
	case 3:
		Rectangle(hdc, 400, 600, 450, 600 - ls_elapse);
		Rectangle(hdc, 700, 600, 750, 600 - ld_elapse);

		TextOutW(hdc, 320, 620, ls, lstrlenW(ls));
		TextOutW(hdc, 620, 620, ld, lstrlenW(ld));

		break;
	case 4:
		if (v_elapse == 0) Rectangle(hdc, 300, 600, 350, 599);
		else Rectangle(hdc, 300, 600, 350, 600 - v_elapse);
		Rectangle(hdc, 600, 600, 650, 600 - ls_elapse);
		Rectangle(hdc, 900, 600, 950, 600 - ld_elapse);

		TextOutW(hdc, 270, 620, vector, lstrlenW(vector));
		TextOutW(hdc, 520, 620, ls, lstrlenW(ls));
		TextOutW(hdc, 830, 620, ld, lstrlenW(ld));

		break;
	}



	EndPaint(hwnd, &ps);

}



LRESULT CALLBACK VLS(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

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


LRESULT CALLBACK VLD(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

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

LRESULT CALLBACK LSLD(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

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

LRESULT CALLBACK VLSLD(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

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