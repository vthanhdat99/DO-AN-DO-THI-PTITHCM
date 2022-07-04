#include <graphics.h>
#include <fstream>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include "include/stack.hpp"
#include "include/queue.hpp"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wconversion-null"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#define RADIUS 15
#define MAX 20 

#include <iostream>

using namespace std;

char guideList[8][500] = {
	"DOUBLE CLICK CHUOT TRAI vao vi tri ban muon tao dinh o khu vuc man hinh thao tac voi dinh -> Nhap ten dinh -> Nhan HOAN THANH (hoac nhan ENTER) -> 1 dinh moi se duoc tao.",
	"DI CHUYEN chuot toi vi tri dinh bat dau cua canh -> CLICK CHUOT PHAI vao dinh do, mot hop thoat se hien len -> chon TAO CANH -> DI CHUYEN toi vi tri dinh thu 2 va CLICK CHUOT TRAI vao dinh do -> NHAP trong so (Nhap ky tu * neu muon canh vo huong)-> nhan HOAN THANH (hoac nhan ENTER) -> 1 canh moi se duoc tao.",
	"DI CHUYEN chuot toi vi tri dinh can sua -> CLICK CHUOT PHAI vao dinh do, mot hop thoai se hien len -> chon SUA TEN -> NHAP TEN dinh va nhan HOAN THANH (hoac nhan ENTER).",
	"Giong nhu cach tao canh nhung o phan nhap trong so ta nhap vao trong so ta muon chinh sua va nhan HOAN THANH (hoac nhan ENTER). Neu muon huy sua canh, CLICK CHUOT PHAI. Co the DOUBLE CLICK CHUOT PHAI vao bang ma tran trong so vi tri tuong ung voi canh do de sua trong so",
	"DI CHUYEN chuot toi vi tri dinh can xoa -> CLICK CHUOT PHAI vao dinh do, mot hop thoai se hien len -> chon XOA DINH -> mot thanh thong bao hien len hoi ban co chac muon xoa dinh -> nhan CO (hoac nhan ENTER) de xoa dinh / KHONG de huy xoa.",
	"DI CHUYEN chuot toi vi tri bat dau cua canh do -> CLICK CHUOT PHAI vao dinh do, mot hop thoai se hien len -> chon TAO CANH -> DI CHUYEN chuot toi vi tri cua dinh thu 2 cua canh do -> CLICK CHUOT TRAI vao dinh do -> trong phan tao trong so NHAP SO 0 -> nhan HOAN THANH (hoac nhan ENTER).",
	"DI CHUYEN chuot toi vi tri cua dinh muon di chuyen -> NHAN GIU dinh do va KEO THA CHUOT toi vi tri can dat.",
	"DI CHUYEN cac dinh nam trong mot canh thi se di chuyen canh do."
};


enum key {
	KEY_BACKSPACE = 8,
	KEY_ESC = 27,
	KEY_ENTER = 13,
	KEY_SPACE = 32,
};

enum workingZone {
	W_LEFT = 425,
	W_TOP = 20,
	W_RIGHT = 1259,
	W_BOTTOM = 520
};

struct Point/*cau truc diem*/ {
	int x;
	int y;
};

struct Vertex /*cau truc dinh*/{
	Point coordinates;
	char *name;
	
	void create();
	void draw();
	void createName();
	void highLight();
	void highLight(int tColor, int bColor);
	void defaultVtex();
	bool isDefaultVtex();
	bool isHover();
	bool isClickLMButton();
	void changeName();
};

struct Button /*cau truc nut*/{
	Point coordinates;
	char* name;
	short height;
	short width;
	short tColor;
	short bColor;
	short pattern;//cac kieu gach cheo, cham bi,.. trong nut
	
	void init(short x, short y, short height, short width, short tColor, short bColor, short pattern, char* content);//x, y, height, width, content
	void draw();//Ve nut
	void highLight();//To sang nut (doi mau theo kieu mac dinh)
 	void normal();//Dua nut tro ve hinh dang ban dau
	void highLight(int tColor, int bColor);//To sang nut voi mau chu va mau nen 
	bool isHover();//Kiem tra xem ta co di chuyen chuot toi nut do hay khong
	bool isClickLMButton();//Kiem tra xem chuot trai co click vao o hay khong
	void changeColor(int tColor, int bColor);//doi mau nut
};


/*-------------------------------------GLOBAL VALUE------------------------------------------------------------------------------------------------------------------------*/
int Time;
int n = 0;
Vertex vertices[MAX];
int G[MAX][MAX];
bool visited[MAX];
/**/
Button menuBar/*thanh menu*/, 
		helpBar/*thanh help*/,
		fileBar/*thanh file*/,
		adjacencyMatrixFrame/*khung hien thi ma tran ke*/, 
		pointBarFrame/*khung hien thi dinh va canh*/,
		taskBarFrame/*khung hien thi thanh tac vu gom menu, file va help*/,
		vertexTaskBarFrame/*khung hien thi thanh tac vu */,
		editFrame/*khung hien thi cac cong cu sua ten va xoa dinh*/,
		deleteButton/*Nut xoa dinh*/,
		editNameButton/*Nut sua ten dinh*/,
		addEdgeButton/*Nut tao canh*/, 
		matrixButton;


/*-----------------------------MENU & SETTING-------------------------------------------------------------------------------------------------------------------------------*/
void setTaskBarButtons();
void taskBar();//ham thao tac thanh tac vu 
int menuTools();//ham hien thi bang cong cu menu
int helpTools();//ham hien thi bang cong cu huong dan
int fileTools();//ham hien thi bang cong cu file
void setFrame();//cai dat khung hien thi
void setUserTextStyle();
void process();



/*--------------------------------------FILE---------------------------------------------------------------------------------------------------------------------------------*/
void loadFileStartUp();//khi tat chuong trinh, 
					  //file nay se co chuc nang luu lai thong tin dinh canh cho lan chay tiep theo
void saveFileStartUp();//khi tao hay x oa dinh, file nay se co tac dung cap nhat lai nhung gi chung ta thao tac
void drawGraphInAllFiles();
bool isFileText(char *fileName);
void openFile();
void loadFile(char *fileName);
bool addFile();
void deleteFile();
void saveFile();


/*-----------------------------------VERTEX & EDGE----------------------------------------------------------------------------------------------------------------------------*/
void addVertexToList(Vertex vtex);//ham nay giup chung ta them mot dinh vua tao vao danh sach dinh
void addEdge(int startPos);//tao canh
void clearAllVertices();//ham nay cho phep xoa tat ca cac dinh va cung dang hien thi tren man hinh
int chooseVertex(char * request);//chon dinh
void deleteVertex(int pos);//xoa dinh trong danh sach tai mot vi tri cho truoc
void editVertex();//xoa dinh tren do thi hoac doi ten dinh
int enterWeight();//ham nhap trong so
void helpBox(char *helpStr);
bool isEmptyVertex();//Kiem tra dinh tren man hinh da co hay chua
bool isClickOtherVertex(Vertex vtex);//kiem tra xem chuot trai co click vao dinh hay khong.
void initDefaultVertices();//khi goi ham nay toa do cua cac dinh se tu dong la x = -1, y = -1
void moveVertex();//ham di chuyen dinh
void printWeight(int x, int y, int w);//xuat trong so 
bool isUndirectedGraph();//ham kiem tra do thi co vo huong khong
bool isConnectedGraph();//ham kiem tra do thi co lien thong hay khong


/*--------------------------------------STRING & ARRAY--------------------------------------------------------------------------------------------------------------------------*/
void strnDel(char *s, int pos, int count);//ham xoa ki tu trong chuoi
void upper(char *s);//Doi day s thanh chu in hoa
bool drawYesNoBar(char *question);//ham nay dung de xac nhan truoc khi lam gi do
									  // Ham nay su dung de tranh tao dinh de len dinh khac
void initEditTools();//Khoi tao hop thoai chinh sua dinh
bool isNamesake(char *s);//Ham de kiem tra xem ten cua dinh vua tao co cung ten voi cac dinh khac khong
template <typename Type>
void set2DArrayTo(Type **arr, unsigned height, unsigned width, int value);//cho tat ca cac gia tri cua ma tran ve mot gia tri nao do
template <typename Type>
void setArrayTo(Type *arr, int num, int value);//ham de cho tat ca cac gia tri cua mang ve mot gia tri nao do
int** create2DArray(unsigned height, unsigned width);//tao ma tran
char** create2DStrArray(unsigned height, unsigned width);
void delete2DArray(int **arr, unsigned height, unsigned width);//xoa ma tran
void delete2DStrArray(char **arr, unsigned height, unsigned width);
void enterSubjectName(char listName[MAX][31], bool *passedSubject, bool *regisSubject);
int min(int a, int b);


/*------------------------------------------DRAW---------------------------------------------------------------------------------------------------------------------------------*/
void clearmouseclick();//xoa tat ca cac click chuot
void drawTaskBarButtons();
void drawFrame();//
void drawEditTools(int x, int y);//Ve hop thoai edit dinh
void drawMatrix();//ve ma tran trong so
void drawArrow(Vertex u, Vertex v, int color, int w);//ve mui ten
void drawTriangle(int x1, int y1, int x2, int y2, int color);//ve hinh tam giac dung cho mui ten
void drawLine(Vertex u, Vertex v, int color, int w);//ve duong noi hai dinh, ap dung cho do thi vo huong
void drawAddVertex(int x, int y);//ve mot vong tron khi tao dinh
void drawAllEdges();//ham ve tat ca cac canh
void drawAllEdges(int color);//ham ve tat ca cac canh voi mau do nguoi dung chon
void drawCurvedArrow(Vertex u, Vertex v, int color, int w);//ve mui ten dang cong
void drawCurvedArrow2(Vertex u, Vertex v, int color, int w);
void drawLoading(char * loadingContent);
void drawEnterToExitText();//in ra dong "press ENTER to exit" o goc phai duoi man hinh lam viec
void drawVertices();
void drawKeyToExitText();//in ra dong "press KEY to exit" o goc phai duoi man hinh lam viec
						//ham nay thuong duoc dung cho cac ham khong co ket qua 
void drawUserManualBox(char *guideStr, char *title);//ve bang huong dan su dung
void drawHoverMessengerBox(int x, int y, int color, char *messenger);
void drawGreenTick(int x, int y);
void drawEmptySymbol();
void drawPauseButton(int x, int y, int height, int color, bool border);
void drawPlayButton(int x, int y, int height, int color, bool border);
void drawNextButton(int x, int y, int height, int color, bool border);
void showWelcome();
void showSuccessfullyBox(char * successContent);
void showNoResult(char *resultStr);
void showEmptyVertex();//show man hinh khong co canh nao ca


/*--------------------------------BFS-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void BFS();
void bfsTraveler(int u);
void showResultBFS(int *trace, char *resultText, int count);//show ket qua thuat toan duyet BFS ra man hinh
/*--------------------------------DFS-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void DFS();
void dfsTraveler(int u);
void showResultDFS(int *trace, char *resultText, int count);//show ket qua thuat toan duyet DFS ra man hinh
/*--------------------------------PATHXY----------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pathXY();//tim duong di ngan nhat tu x toi y
void dijkstra(int start, int end);//thuat toan tim duong di ngan nhat
int minDistance(int *dist, bool *sptSet);// mot ham tien ich de tim dinh voi gia tri khoang cach toi thieu, 
void showResultPathXY(int *trace, int *dist, int start, int end);//show ra man hinh duong di ngan nhat tu x toi y
										 						//tu tap hop cac dinh chua duoc bao gom trong cay duong dan ngan nhat


/*--------------------------------EULER------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void eulerCycle(int u);//ham tim chu trinh euler
void showResultEulerCycle(stack CE, char * resultText);//show ket qua chu trinh euler ra man hinh


/*--------------------------------CONNECTED COMPONENT----------------------------------------------------------------------------------------------------------------------------------------------*/
int DFS(int u);//duyet theo chieu sau
void connectedComponents();//thanh phan lien thong
int countConnectedComponents();//tinh thanh phan lien thong cua do thi
int countConnectedComponents(int **connectedComponents);//tinh thanh phan lien thong cua do thi 
														//va tra ve mang cac thanh phan lien thong cua do thi
int countStrongConComponent(int **compnentsList);//dem thanh phan lien thong manh va tim thanh phan lien thong manh
void tarjanAlgo(int u, int *disc, int *lowLink, stack &stk, bool *stkItem, int **componentsList, int &counter);//thuat toan tim thanh phan lien thong manh trong do thi co huong
void showResultConnectedComponents(int **connectedComponents, int count, bool isUndirectedGraph, char *resultText);//show ra man hinh thanh phan lien thong cua do thi


/*--------------------------------BRIDGE EDGE------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bridgeEdges();//duyet canh cau
void bridgeUtil(int u, int *disc, int *lowLink, int *parent, int &counter, int *trace, int &index);//thuat toan tim canh cau
void showResultBridgeEdge(int *trace, int counter);//show ket qua tim canh cau ra man hinh
void tarjanAlgo2(int u, int disc[], int lowLink[], stack &stk, bool stkItem[], int &counter);
int countStrongConComponent();

/*-------------------------------CUT VERTEX---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void cutVertices();//dinh tru
void cutVerticesUtil(int u, int *disc,int *lowLink, int parent, bool *isCutVertex);//thuat toan tim dinh tru
void showResultCutVertices(bool *isCutVertex, int counter);//show ra man hinh ket qua dinh tru


/*-------------------------------HAMILTON-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
//void showResultHamiltonCycle(int *path);//show ket qua chu trinh hamilton ra man hinh
//bool isSafe(int v, int *path, int pos);
//bool hamCycleUtil(int v, int *path, int pos);//mot chuc nang tien ich de quy de giai quyet van de chu trinh Hamilton
//void hamCycle();//Hamilton
void hamilton();
void hamCycle(int *ans, int &counter, int index, bool showScreen);
void hamPath(int *ans, int &counter, int index, bool showScreen);
void showResultHamCycle(int *ans, int counter);
void showResultHamPath(int *ans, int counter);

/*-------------------------------KNOT VERTEX--------------------------------------------------------------------------------------------------------------------------------------------------------*/
void knotVertices();//tim dinh that
bool dfsToCheckKnot(int start, int end, int remove);//ham nay giup kiem tra xem lieu co ton tai duong di giua hai dinh khong 
void showResultKnotVertices(int start, int end, int trace, int counter);//show dinh that tu a toi b len man hinh


/*---------------------------------TOPO SORT---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void topo();//sap xep mon hoc topo
void dfsTopo(int u, queue &topo, int *Visited);
void showResultTopoSort(queue topo,char subjects[MAX][31],bool *passedSubject, bool *regisSubject);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


int main() {
	initwindow(1280, 720, "Do an do thi");
	setUserTextStyle();
	setTaskBarButtons();
	setFrame();
	initDefaultVertices();
	initEditTools();
	loadFileStartUp();
	char c = getcolor();
	showWelcome();
	setcolor(c);
	setUserTextStyle();
	process();
}

void process() {
	Vertex vtex;
	int page = 0;
	while (true) {	
		setactivepage(page);
		setvisualpage(1 - page);
		cleardevice();
		delay(20);
		vtex.defaultVtex();
		setfillstyle(10, GREEN);
		bar(1, 1, 1279, 719);
		drawFrame();
		moveVertex();
		if (n < MAX) {
			vtex.create();
			addVertexToList(vtex);
		}
		drawTaskBarButtons();
		drawVertices();
		drawMatrix();
		taskBar();
		editVertex();
		drawAllEdges();
		if (kbhit()) {
			char key= getch();
			if (key == KEY_ESC) {
				bool confirm = drawYesNoBar("Thoat chuong trinh?");
				if (confirm)
					return;
			}
		}
		page = 1 - page;
//		if (ismouseclick(WM_MOUSEMOVE)) {
//			int x, y;
//			getmouseclick(WM_MOUSEMOVE, x, y);
//			cout << x << " " << y << endl;
//		}
	}
//	getch();
	closegraph();
}

void drawLoading(char * loadingContent) {
	Button frame;
	srand(time(0));
	int height = 100;
	int width = 300;
	int margin = 5;
	frame.init(425 + (834 - width) / 2 , 20 + (500 - height) / 2, height, width, YELLOW, BLACK, 1, "");
	int x = frame.coordinates.x + 18,
		y = frame.coordinates.y + (frame.height - textheight("A")) / 2 + 25;
	int page = getactivepage();
	int interrupt = rand() % 24 + 1, interrupt2, interrupt3;
	char color = getcolor();
	char percentStr[5];
	do {
		interrupt2 = rand() % 24 + 1;
	} while (interrupt2 == interrupt);
	do {
		interrupt3 = rand() % 24 + 1;
	} while (interrupt3 == interrupt || interrupt3 == interrupt2);
	setactivepage(1 - page);
	setvisualpage(1 - page);
	frame.draw();
	rectangle(x - 2, y - 3, x + 254, y + 14);
	outtextxy(x, y - 25, loadingContent);
	for (int i = 1; i < 24; i++) {
		float percent = (float)100 / (float)23 * (float)i;
		itoa(percent, percentStr, 10);
		strcat(percentStr, "%");
		setfillstyle(1, LIGHTBLUE);
		bar(x, y, x + 10, y + 12);
		setfillstyle(1, color);
		setcolor(YELLOW);
		outtextxy(frame.coordinates.x + frame.width - 35, y - 25, percentStr);
		setcolor(color);
		x += 11;
		if (i == interrupt || i == interrupt2)
			delay(300);
		if (i == interrupt3)
			delay(500);
		if (i == 23)
			delay(500);
		delay(30);
	}
}

void drawNextButton(int x, int y, int height, int color, bool border) {
	if (border) 
		rectangle(x, y, x + height, y + height);
	int a[8] = {x + height / 4, y + height / 4, 
			    x + height / 2, y + height / 2, 
				x + height / 4, y + 3 * height / 4,
				x + height / 4, y + height / 4};
	int b[10] = {x + height / 2    , y + height / 4,
				 x + 3 * height / 4, y + height / 4,
				 x + 3 * height / 4, y + 3 * height / 4,
				 x + height / 2    , y + 3 * height / 4, 
				 x + height / 2    , y + height / 4};
	char c = getcolor();
	setfillstyle(1, color);
	fillpoly(4, a);
	fillpoly(5, b);
	setfillstyle(1, c);
}

void drawPlayButton(int x, int y, int height, int color, bool border) {
	if (border) 
		rectangle(x, y, x + height, y + height);
	int a[8] = {x + height / 4    , y + height / 4, 
			    x + 3 * height / 4, y + height / 2, 
				x + height / 4    , y + 3 * height / 4,
				x + height / 4    , y + height / 4};
	char c = getcolor();
	setfillstyle(1, color);
	fillpoly(4, a);
	setfillstyle(1, c);
}

void drawPauseButton(int x, int y, int height, int color, bool border) {
	if (border)
		rectangle(x, y, x + height, y + height);
	int a[10]  = { x + height / 5    , y + height / 6, 
				   x + 2 * height / 5, y + height / 6,
				   x + 2 * height / 5, y + 5 * height / 6,
				   x + height / 5    , y + 5 * height / 6, 
				   x + height / 5    , y + height / 6 };
	char c = getcolor();
	setfillstyle(1, color);
	fillpoly(5, a);
	for (int i = 0; i < 10; i++)
		if (!(i % 2))
			a[i] += (2 * height / 5);
	fillpoly(5, a);	
	setfillstyle(1, c);
}

void drawHoverMessengerBox(int x, int y, int color, char *messenger) {
	Button box;
	box.init(x, y, 40, 390, WHITE, color, 1, messenger);
	box.draw();
}

void drawGreenTick(int x, int y) {
	int a[10] = {x, y + 8, x + 8, y + 12, x + 20, y, x + 8, y + 20, x, y + 8};
	char c = getcolor();
	setfillstyle(1, GREEN);
	fillpoly(5, a);
	setfillstyle(1, c);
}

void hamilton() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;
	}
	Time = 1;
	setArrayTo(visited, n, false);
	int ans[n + 1], counterCycle = 0, counterPath = 0;
	ans[0] = 0;
	hamCycle(ans, counterCycle, 1, false);
	if (!counterCycle) {
		//neu do thi khong co chu trinh hamilton
		//test xem co duong di hamilton hay khong
		for (int i = 1; i < n; i++) {
			counterPath = 0;
			ans[0] = i;
			hamPath(ans, counterPath, 1, false);
			if (counterPath)
				break;
		}
		if (!counterPath) {
			//neu khong co thi in ra ket qua khong co
			showNoResult("Do thi nay khong co chu trinh hay duong di Hamilton nao.");
			return;
			//ket thuc ham
		}
		else {
			//neu co duong di hamilton thi chon dinh bat dau duong di
			//va chon dinh ket thuc duong di
			int start = chooseVertex("Chon dinh bat dau duong di Hamilton");
			ans[0] = start;
			counterPath = 0;
			hamPath(ans, counterPath, 1, false);
			if (!counterPath) {
				char resultText[100] = "Khong ton tai duong di Hamilton tu dinh ";
				strcat(resultText, vertices[start].name);
				showNoResult(resultText);
				return;
			}
			else {
				drawLoading("Dang tai ket qua duong di Hamilton..");
				hamPath(ans, counterPath, 1, true);
			}
		}
	}
	else {
		//neu co chu trinh Hamilton thi in ra ket qua hamilton
		int start = chooseVertex("Chon dinh bat dau chu trinh Hamilton");
		ans[0] = start;
		drawLoading("Dang tai ket qua chu trinh Hamilton..");
		hamCycle(ans, counterCycle, 1, true);
	}
}

void hamCycle(int *ans, int &counter, int index, bool showScreen) {
	for (int i = 0; i < n; i++) {
		if (G[ans[index - 1]][i] && !visited[i]) {
			ans[index] = i;
			visited[i] = true;
			if (index < n)
				hamCycle(ans, counter, index + 1, showScreen);
			else if (ans[index] == ans[0]) {
				if (showScreen)
					showResultHamCycle(ans, counter);
				else
					counter++;
			} 
			visited[i] = false;
		}
	}
}

void hamPath(int *ans, int &counter, int index, bool showScreen) {
	for (int i = 0; i < n; i++) {
		if (G[ans[index - 1]][i] && !visited[i]) {
			ans[index] = i;
			visited[i] = true;
			if (index < n - 1)
				hamPath(ans, counter, index + 1, showScreen);
			else if (!visited[ans[0]]) {
				if (showScreen) 
					showResultHamPath(ans, counter);
				else 
					counter++;
			} 
			visited[i] = false;
		}
	}
}

void showResultHamPath(int *ans, int counter) {
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, "");
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	char resultText[100] = "";
	char order[12] = "", indexStr[5], counterStr[5];
	itoa(Time, indexStr, 10);
	Time++;
	itoa(counter, counterStr, 10);
	strcat(order, indexStr);
	strcat(order, "/");
	strcat(order, counterStr);
	strcat(resultText, vertices[ans[0]].name);
	strcat(resultText, " -> ");
	setactivepage(1);
	setvisualpage(1);
	drawFrame();
	drawVertices();
	drawTaskBarButtons();
	drawEnterToExitText();
	drawAllEdges();
	for (int i = 1; i < n; i++) {
		strcat(resultText, vertices[ans[i]].name);
		if (i < n - 1)
			strcat(resultText, " -> ");
		resultBox.name = resultText;
		resultBox.draw();
		xButton.draw();
		outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua duong di Hamiton")) / 2, 540, "Ket qua duong di Hamiton");
		outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
		drawPauseButton(1135, 530, 25, WHITE, false);
		drawNextButton(1160, 530, 25, WHITE, false);
		vertices[ans[0]].highLight(YELLOW, LIGHTBLUE);
		drawArrow(vertices[ans[i - 1]], vertices[ans[i]], LIGHTGREEN, 0);
		vertices[ans[i]].highLight();
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				return process();
			else if (key == KEY_SPACE) {
				resultBox.draw();
				xButton.draw();
				outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua duong di Hamiton")) / 2, 540, "Ket qua duong di Hamiton");
				outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
				drawPlayButton(1135, 530, 25, WHITE, true);
				drawNextButton(1160, 530, 25, WHITE, false);
 				getch();
			}
			else if (key == KEY_RIGHT) {
				resultBox.draw();
				xButton.draw();
				outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua duong di Hamiton")) / 2, 540, "Ket qua duong di Hamiton");
				outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
				drawPauseButton(1135, 530, 25, WHITE, false);
				drawNextButton(1160, 530, 25, WHITE, true);
				delay(200);
				return;
			}
		}
		if (xButton.isClickLMButton())
			return process();
		delay(200);
	}
	delay(2500);
 	if (kbhit()) {
 		char key = getch();
 		if (key == KEY_ENTER)
 			return process();
 		else if (key == KEY_SPACE) {
			resultBox.draw();
			xButton.draw();
			outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua duong di Hamiton")) / 2, 540, "Ket qua duong di Hamiton");
			outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
			drawPlayButton(1135, 530, 25, WHITE, true);
			drawNextButton(1160, 530, 25, WHITE, false);
 			getch();
 			return;
		}
		else if (key == KEY_RIGHT) {
			resultBox.draw();
			xButton.draw();
			outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua duong di Hamiton")) / 2, 540, "Ket qua duong di Hamiton");
			outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
			drawPauseButton(1135, 530, 25, WHITE, false);
			drawNextButton(1160, 530, 25, WHITE, true);
			delay(200);
			return;
		}
	}
	if (xButton.isClickLMButton())
		return process();
	delay(2500);
}

void showResultHamCycle(int* ans, int counter) {
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, "");
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	char resultText[100] = "";
	char order[12] = "", indexStr[5], counterStr[5];
	itoa(Time, indexStr, 10);
	Time++;
	itoa(counter, counterStr, 10);
	strcat(order, indexStr);
	strcat(order, "/");
	strcat(order, counterStr);
	strcat(resultText, vertices[ans[0]].name);
	strcat(resultText, " -> ");
	setactivepage(1);
	setvisualpage(1);
	drawFrame();
	drawVertices();
	drawTaskBarButtons();
	drawEnterToExitText();
	drawAllEdges();
	for (int i = 1; i < n; i++) {
		
		resultBox.name = resultText;
		resultBox.draw();
		xButton.draw();
		outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua chu trinh Hamiton")) / 2, 540, "Ket qua chu trinh Hamiton");
		outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
		drawPauseButton(1135, 530, 25, WHITE, false);
		drawNextButton(1160, 530, 25, WHITE, false);
		strcat(resultText, vertices[ans[i]].name);
		strcat(resultText, " -> ");
		vertices[ans[0]].highLight(YELLOW, LIGHTBLUE);
		drawArrow(vertices[ans[i - 1]], vertices[ans[i]], LIGHTGREEN, 0);
		vertices[ans[i]].highLight();
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				return process();
			else if (key == KEY_SPACE) {
				resultBox.draw();
				xButton.draw();
				outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua chu trinh Hamiton")) / 2, 540, "Ket qua chu trinh Hamiton");
				outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
				drawPlayButton(1135, 530, 25, WHITE, true);
				drawNextButton(1160, 530, 25, WHITE, false);
 				getch();
			}
			else if (key == KEY_RIGHT) {
				resultBox.draw();
				xButton.draw();
				outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua chu trinh Hamiton")) / 2, 540, "Ket qua chu trinh Hamiton");
				outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
				drawPauseButton(1135, 530, 25, WHITE, false);
				drawNextButton(1160, 530, 25, WHITE, true);
				delay(200);
				return;
			}
		}
		if (xButton.isClickLMButton())
			return process();
		delay(200);
	}
	strcat(resultText, vertices[ans[0]].name);
	resultBox.name = resultText;
	resultBox.draw();
	xButton.draw();
	outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua chu trinh Hamiton")) / 2, 540, "Ket qua chu trinh Hamiton");
	outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
	drawArrow(vertices[ans[n - 1]], vertices[ans[0]], LIGHTGREEN, 0);
	delay(2500);
 	if (kbhit()) {
 		char key = getch();
 		if (key == KEY_ENTER)
 			return process();
 		else if (key == KEY_SPACE) {
			resultBox.draw();
			xButton.draw();
			outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua chu trinh Hamiton")) / 2, 540, "Ket qua chu trinh Hamiton");
			outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
			drawPlayButton(1135, 530, 25, WHITE, true);
			drawNextButton(1160, 530, 25, WHITE, false);
 			getch();
		}
		else if (key == KEY_RIGHT) {
			resultBox.draw();
			xButton.draw();
			outtextxy((W_LEFT + W_RIGHT - textwidth("Ket qua chu trinh Hamiton")) / 2, 540, "Ket qua chu trinh Hamiton");
			outtextxy((W_LEFT + W_RIGHT - textwidth(order)) / 2 , 600, order);
			drawPauseButton(1135, 530, 25, WHITE, false);
			drawNextButton(1160, 530, 25, WHITE, true);
			delay(200);
			return;
		}
	}
	delay(2500);
	if (xButton.isClickLMButton())
		return process();
	//delay(2500);
//	setactivepage(0);
//	setvisualpage(0);
}


void showResultTopoSort(queue topo, char subjects[MAX][31],bool *passedSubject, bool *regisSubject) {
	int index = 0, ans[n], x0, y0, x, y;
	Button displayBox, titleBar, statusBar1, statusBar2, resultBox, successTitleBox;
	while (!topo.isEmpty())
		ans[index++] = topo.pop();
	resultBox.init(425, 525, 170, 834, BLACK, BLACK, 1, "");
	successTitleBox.init(425, 525, 25, 834, YELLOW, BLACK, 1, "DANH SACH MON HOC DANG KY THANH CONG");
	titleBar.init(15, 295, 25, 200, YELLOW, BLACK, 1, "DANH SACH MON HOC");
	statusBar1.init(215, 295, 25, 100, YELLOW, BLACK, 1, "Passed");
	statusBar2.init(315, 295, 25, 100, YELLOW, BLACK, 1, "Registration");
	int page = 0;
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		drawFrame();
		//drawTaskBarButtons();
		drawAllEdges();
		drawEnterToExitText();
		titleBar.draw();
		statusBar1.draw();
		statusBar2.draw();
		resultBox.draw();
		successTitleBox.draw();
		moveVertex();
		x = -1, y = -1;
		if (ismouseclick(WM_LBUTTONDBLCLK)) {
			getmouseclick(WM_LBUTTONDBLCLK, x, y);
		}
		int j;
		for (int i = 0; i < n; i++) {
			if (!passedSubject[ans[i]]) 
				continue;
			for (j = i + 1; j < n; j++) {
				if (G[ans[j]][ans[i]] && !passedSubject[ans[j]]) {
//					passedSubject[ans[j]] = true;
					break;
				}
			}
			if (j != n)
				break;
		}
		y0 = 320;
		x0 = 215;
		for (int i = 0; i < n; i++) {
			displayBox.init(vertices[i].coordinates.x - textwidth(subjects[i]) / 2, vertices[i].coordinates.y - RADIUS, RADIUS * 2, textwidth(subjects[i]), YELLOW, BLACK, 1, subjects[i]);
			displayBox.draw();
			displayBox.init(15, y0, 25, 200, WHITE, BLACK, 1, subjects[i]);
			displayBox.draw();
			displayBox.init(x0, y0, 25, 100, BLACK, BLACK, 1, "");
			displayBox.draw();
			displayBox.init(x0 + (100 - 20) / 2, y0 + 2, 20, 20, BLACK, WHITE, 1, "");
			displayBox.draw();
			if (i == ans[j]) {
				displayBox.highLight(BLACK, RED);
				drawHoverMessengerBox(20, 100, RED, "Khong hop le. Phai qua mon to mau do truoc!");
			}
			if (x >= displayBox.coordinates.x && x <= displayBox.coordinates.x + displayBox.width
			&& y >= displayBox.coordinates.y && y <= displayBox.coordinates.y + displayBox.height) {
				if (passedSubject[i])
					passedSubject[i] = false;
				else 
					passedSubject[i] = true;
				clearmouseclick(WM_LBUTTONDOWN);
			}
			if (passedSubject[i])
				drawGreenTick(displayBox.coordinates.x, displayBox.coordinates.y);
			y0 += 25;
		}
		x0 = 580, y0 = 565;
		for (int i = 0; i < n; i++) {
			if (!regisSubject[ans[i]]) 
				continue;
			bool OK = true;
			for (j = i + 1; j < n; j++) {
				if (G[ans[j]][ans[i]] && !passedSubject[ans[j]]) {
					char regisStatus[60] = "Mon ";
					strcat(regisStatus, subjects[ans[i]]);
					strcat(regisStatus, " dang ky KHONG THANH CONG! vi: ");
					drawHoverMessengerBox(20, 140, BLACK, regisStatus);
					char regisStatus2[60] = "Mon tien quyet ";
					strcat(regisStatus2, subjects[ans[j]]);
					strcat(regisStatus2, " CHUA HOC");
					drawHoverMessengerBox(20, 180, BLACK, regisStatus2);
					OK = false;
					drawArrow(vertices[ans[j]], vertices[ans[i]], LIGHTGREEN, 0);
					break;
				}
			}
			if (OK) {
				drawGreenTick(x0 - 20, y0);
				outtextxy(x0, y0, subjects[ans[i]]);
				x0 += 250;
				if (x0 == 580 + 250 * 3) {
					x0 = 580;
					y0 += 25;
				}
			}
		}
		
		x0 = 215 + 100;
		y0 = 320;
		for (int i = 0; i < n; i++) {
			displayBox.init(x0, y0, 25, 100, BLACK, BLACK, 1, "");
			displayBox.draw();
			displayBox.init(x0 + (100 - 20) / 2, y0 + 2, 20, 20, BLACK, WHITE, 1, "");
			displayBox.draw();
			if (x >= displayBox.coordinates.x && x <= displayBox.coordinates.x + displayBox.width
			&& y >= displayBox.coordinates.y && y <= displayBox.coordinates.y + displayBox.height) {
				if (regisSubject[i])
					regisSubject[i] = false;
				else
					regisSubject[i] = true;
				clearmouseclick(WM_LBUTTONDOWN);
			}
			if (regisSubject[i])
				drawGreenTick(displayBox.coordinates.x, displayBox.coordinates.y);
			y0 += 25;
		}
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;
		}
		delay(100);
		page = 1 - page;
	}
}

void enterSubjectName(char listName[MAX][31], bool *passedSubject, bool *regisSubject) {
	int index = 0, i = 0;
	int page = 0;
	int margin = 5;
	int height = 275 - 3 * margin - 40;
	int width = 400 - 2 * margin;
	char name[31] = "";
	char request[50];
	char c = getcolor();
	Button displayBox;
	Button titleBar;
	Button statusBar1, statusBar2;
	titleBar.init(15, 295, 25, 200, YELLOW, BLACK, 1, "DANH SACH MON HOC");
	statusBar1.init(215, 295, 25, 100, YELLOW, BLACK, 1, "Passed");
	statusBar2.init(315, 295, 25, 100, YELLOW, BLACK, 1, "Registration");
	int x0, y0, x = -1, y = -1;
//		adjacencyMatrixFrame.init(15, 295, 409, 400, 0, 3, 1, "");
	Button frame, enterNameBar;
	frame.init(15 + margin, 15 + 2 * margin + 40, height, width, YELLOW, DARKGRAY, 1, "");
	enterNameBar.init(15 + 2 * margin, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50, 40, (width - 2 * margin), YELLOW, BLACK, 9, "");
	while (index < n) {
		setactivepage(page);
		setvisualpage(1 - page);
		drawFrame();
		drawTaskBarButtons();
//		drawMatrix();
		titleBar.draw();
		statusBar1.draw();
		statusBar2.draw();
		drawAllEdges();
		drawVertices();
		frame.draw();
		enterNameBar.draw();
		moveVertex();
		strcpy(request, "Nhap ten mon hoc tuong ung voi dinh ");
		x = -1, y = -1;
		strcat(request, vertices[index].name);
		if (ismouseclick(WM_LBUTTONDBLCLK)) {
			getmouseclick(WM_LBUTTONDBLCLK, x, y);
		}
		if (strcmp(name, "") == 0)
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
		else 
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);
		if (i < 30 && strcmp(name, "") != 0)
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2 + textwidth(name), 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, "_");
		displayBox.init(vertices[index].coordinates.x - textwidth(name) / 2, vertices[index].coordinates.y - RADIUS, RADIUS * 2, textwidth(name), YELLOW, BLACK, 1, name);
		displayBox.draw();
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER) {
				if (strcmp(name, "") != 0) {	
					strcpy(listName[index], name);
					listName[index][i] = '\0';
				}
				else 
					strcpy(listName[index], vertices[index].name);
				for (int j = 0; j < 30; j++)
					name[j] = '\0';
				name[0] = '\0';
				i = 0;
				index++;
			}
			if ((key >= 'A' && key <= 'Z')
			 || (key >= 'a' && key <= 'z')
			 || (key == KEY_SPACE)
			 || (key >= '0' && key <= '9')
			 || key == '+' || key == '#'
			 || key == '&' || key == '/')
				if (i < 30) {
					name[i] = key;
					i++;
				}
			if (i > 30)
				i = 30;
			if (key == KEY_BACKSPACE) {
				strnDel(name, i - 1, 1);
				i--;
			}
			if (i < 0)
				i = 0;
		}
		y0 = 320;
		x0 = 215;
		for (int i = 0; i < index; i++) {
			displayBox.init(vertices[i].coordinates.x - textwidth(listName[i]) / 2, vertices[i].coordinates.y - RADIUS, RADIUS * 2, textwidth(listName[i]), YELLOW, BLACK, 1, listName[i]);
			displayBox.draw();
			displayBox.init(15, y0, 25, 200, WHITE, BLACK, 1, listName[i]);
			displayBox.draw();
			displayBox.init(x0, y0, 25, 100, BLACK, BLACK, 1, "");
			displayBox.draw();
			displayBox.init(x0 + (100 - 20) / 2, y0 + 2, 20, 20, BLACK, WHITE, 1, "");
			displayBox.draw();
			if (x >= displayBox.coordinates.x && x <= displayBox.coordinates.x + displayBox.width
			&& y >= displayBox.coordinates.y && y <= displayBox.coordinates.y + displayBox.height) {
				if (passedSubject[i])
					passedSubject[i] = false;
				else 
					passedSubject[i] = true;
				clearmouseclick(WM_LBUTTONDOWN);
			}
			if (passedSubject[i])
				drawGreenTick(displayBox.coordinates.x, displayBox.coordinates.y);
			y0 += 25;
		}
		x0 += 100;
		y0 = 320;
		for (int i = 0; i < index; i++) {
			displayBox.init(x0, y0, 25, 100, BLACK, BLACK, 1, "");
			displayBox.draw();
			displayBox.init(x0 + (100 - 20) / 2, y0 + 2, 20, 20, BLACK, WHITE, 1, "");
			displayBox.draw();
			if (x >= displayBox.coordinates.x && x <= displayBox.coordinates.x + displayBox.width
			&& y >= displayBox.coordinates.y && y <= displayBox.coordinates.y + displayBox.height) {
				if (regisSubject[i])
					regisSubject[i] = false;
				else
					regisSubject[i] = true;
				clearmouseclick(WM_LBUTTONDOWN);
			}
			if (regisSubject[i])
				drawGreenTick(displayBox.coordinates.x, displayBox.coordinates.y);
			y0 += 25;
		}
		page = 1 - page;
	}	
}

void dfsTopo(int u, queue &topo, int *Visited) {
	Visited[u] = 1;
	for (int i = 0; i < n; i++) {
		if (Visited[i] == 1 && G[u][i]) {
			showNoResult("Loi: do thi bieu dien cac mon hoc tien quyet ton tai chu trinh");
			return process();
		}
		if (!Visited[i] && G[u][i])
			dfsTopo(i, topo, Visited);
	}
	topo.push(u);
	Visited[u] = 2;
}

void topo() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;
	}
	bool passedSubject[n] = {false};
	bool regisSubject[n] = {false};
	queue Topo;
	int Visited[n];
	setArrayTo(Visited, n, 0);
	for (int i = 0; i < n; i++) 
		if (!Visited[i])
			dfsTopo(i, Topo, Visited);
	char subjects[n][31] = {""};
	enterSubjectName(subjects, passedSubject, regisSubject);
	showResultTopoSort(Topo, subjects, passedSubject, regisSubject);
}

void drawEmptySymbol() {
	char c = getcolor();
	setfillstyle(1, WHITE);
	pieslice(15 + 400 / 2, 295 + 400 / 2, 0, 360, 180);
	setfillstyle(1, c);
	setfillstyle(1, CYAN);
	pieslice(15 + 400 / 2, 295 + 400 / 2, 0, 360, 160);
	setfillstyle(1, c);
	setcolor(CYAN);
	line(15 + 400 / 2, 295 + 400 / 2 - 1, 15 + 400 / 2 + 160, 295 + 400 / 2 - 1);
	setcolor(c);
//	drawpoly()
	int arr[] = {360, 335, 375, 350, 65, 655, 50, 640, 360, 335};
//	adjacencyMatrixFrame.init(15, 295, 409, 400, 0, 3, 1, "");
	setfillstyle(1, WHITE);
	fillpoly(5, arr);
	setfillstyle(1, c);
}

void showResultKnotVertices(int start, int end, int * trace, int counter) {
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, "");
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	char resultText[100] = "";
	if (counter == 0) {
		strcat(resultText, "Khong co dinh that giua dinh ");
		strcat(resultText, vertices[start].name);
		strcat(resultText, " va dinh ");
		strcat(resultText, vertices[end].name);
		strcat(resultText, ".");
	} else {
		drawLoading("Dang tai ket qua dinh that...");
		char counterStr[3];
		itoa(counter, counterStr, 10);
		strcat(resultText, "Co ");
		strcat(resultText, counterStr);
		strcat(resultText, " dinh that giua dinh ");
		strcat(resultText, vertices[start].name);
		strcat(resultText, " va dinh ");
		strcat(resultText, vertices[end].name);
		strcat(resultText, ": ");
		for (int i = 0; i < counter; i++) {
			strcat(resultText, "dinh ");
			strcat(resultText, vertices[trace[i]].name);
			strcat(resultText, ", ");
		}
		strnDel(resultText, strlen(resultText) - 2, 2);
	}
	resultBox.name = resultText;
	int page = 0;
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawTaskBarButtons();
		drawMatrix();
		drawVertices();
		drawAllEdges();
		drawEnterToExitText();
		resultBox.draw();
		xButton.draw();
		if (xButton.isClickLMButton())
			break;
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER);
				break;
		}
		vertices[start].highLight();
		for (int i = 0; i < counter; i++) {
			vertices[trace[i]].highLight(YELLOW, GREEN);
		}
		vertices[end].highLight();
		page = 1 - page;
	}
}

bool dfsToCheckKnot(int start, int end, int remove) {
	setArrayTo(visited, n, false);
	stack s;
	s.push(start);
	visited[start] = true;
	int numTraver = 0;
	while (!s.isEmpty()) {
		start = s.pop();
		if (start == end) return true;
		++numTraver;
		for (int i = 0; i < n; i++) 
			if (!visited[i] && G[start][i] && i != remove && start != remove) {
				visited[i] = true;
				s.push(i);
			}
	}
	return numTraver == n;
}

void knotVertices() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;
	}
	Button instruction;
	instruction.init(425, 525, 40, 834, YELLOW, BLACK, 1, "Chon dinh ket thuc");
	int remove;
	int start = chooseVertex("Chon dinh bat dau duong di");
	int end = start;
	int page = 0;
	while (end == start) {
		setactivepage(page);
		setvisualpage(1 - page);
		pointBarFrame.draw();
		drawVertices();
		instruction.draw();
		int x = mousex();
		int y = mousey();
		if (x >= W_LEFT && x <= W_RIGHT && y >= W_TOP && y <= W_BOTTOM) {
			setlinestyle(DOTTED_LINE, 1, 2);
			line(vertices[start].coordinates.x, vertices[start].coordinates.y, x, y);
			setlinestyle(SOLID_LINE, 1, 1);
		}
		vertices[start].highLight();
		if (ismouseclick(WM_LBUTTONDOWN)) {
			int xL, yL;
			getmouseclick(WM_LBUTTONDOWN, xL, yL); 
			for (int i = 0; i < n; i++) {
				int xV = vertices[i].coordinates.x;
				int yV = vertices[i].coordinates.y;
				if ((xV - xL) * (xV - xL) + (yV - yL) * (yV - yL) <= RADIUS * RADIUS) {
					end = i;
				}
			}
			clearmouseclick(WM_LBUTTONDOWN);
		}
		if (ismouseclick(WM_RBUTTONDOWN))
			clearmouseclick(WM_RBUTTONDOWN);
		if (ismouseclick(WM_LBUTTONDBLCLK))
			clearmouseclick(WM_LBUTTONDBLCLK);
		page = 1 - page;
	}
	if (!dfsToCheckKnot(start, end, -1)) {
		char resultText[60] = "Khong co dinh that giua dinh ";
		strcat(resultText, vertices[start].name);
		strcat(resultText, " va dinh ");
		strcat(resultText, vertices[end].name);
		strcat(resultText, " vi khong co duong di!");
		showNoResult(resultText);
		return;
	}
	int counter = 0, trace[n];
	for (int i = 0; i < n; i++) 
		if (i != start && i != end) 
			if (!dfsToCheckKnot(start, end, i))
				trace[counter++] = i;
	
	showResultKnotVertices(start,end, trace, counter);
}

bool isFileText(char *fileName) {
	int len = strlen(fileName);
	if (fileName[len - 1] != 't') return false;
	if (fileName[len - 2] != 'x') return false;
	if (fileName[len - 3] != 't') return false;
	if (fileName[len - 4] != '.') return false;
	return true;
}

void drawGraphInAllFiles() {
	char fileName[41][100] = {""};
	int index = 0;
	struct dirent *d;
	struct stat dst;
	DIR *dr;
	char path[60] = "filesInProgram";
	dr = opendir(path);
	if (dr != NULL) {
		for (d = readdir(dr); d != NULL; d = readdir(dr)) {
			if (strlen(d->d_name) >= 5 && strcmp(d->d_name, "startUpFile.txt") != 0 && isFileText(d->d_name))
				strcpy(fileName[index++], d->d_name);
			if (index > 40)
				break;
		}
		closedir(dr);	
	}
	if (index == 0) return;
	int page = 0, i = 0;
	while (!ismouseclick(WM_LBUTTONDOWN) 
		&& !ismouseclick(WM_RBUTTONDOWN) 
		&& !ismouseclick(WM_LBUTTONDBLCLK) 
		&& !ismouseclick(WM_RBUTTONDBLCLK)
	) {
		setactivepage(page);
		setvisualpage(1 - page);
		loadFile(fileName[i]);
		drawFrame();
		drawTaskBarButtons();
		drawVertices();
		drawAllEdges(i % 15 + 1);
		i++;
		if (i >= index)
			i = 0;
		page = 1 - page;
		delay(1000);
	} 
	cleardevice();
}

void showSuccessfullyBox(char * successContent) {
	Button successBox;
	successBox.init((W_LEFT + W_RIGHT - 400) / 2, (W_TOP + W_BOTTOM - 200) / 2, 200, 400, YELLOW, BLACK, 1, successContent);
	int page = 0;
	for (int i = 0; i < 3; i++) {
		setactivepage(page);
		setvisualpage(1 - page);
		drawFrame();
		drawTaskBarButtons();
		drawMatrix();
		drawVertices();
		drawAllEdges();
		successBox.draw();
		delay(100);
		page = 1 - page;
	}	
}

void saveFile() {
	char fileName[41][100] = {""};
	int index = 0;
	struct dirent *d;
	DIR *dr;
	char path[30] = "filesInProgram";
	dr = opendir(path);
	if (dr != NULL) {
		for (d = readdir(dr); d != NULL; d = readdir(dr)) 
			if (strlen(d->d_name) >= 5 && strcmp(d->d_name, "startUpFile.txt") != 0 && isFileText(d->d_name))
				strcpy(fileName[index++], d->d_name);
		closedir(dr);	
	}
	if (index) {
		int x0 = W_LEFT + 5, y0 = W_TOP + 50;
		int column = 1;
		Button fileButton[index], displayBox, xButton;
		displayBox.init(x0, y0 - 45, 40, 0, LIGHTBLUE, BLACK, 1, "Chon file muon luu do thi");
		xButton.init(0, y0 - 45, 40, 40, WHITE, RED, 1, "x");
		int yTmp = y0;
		for (int i = 0; i < index; i++) {
			fileButton[i].init(x0, y0, 40, 200, WHITE, BLACK, 1, fileName[i]);
			y0 += 45;
			if (i + 1 == 10 * column) {
				x0 += 205;
				y0 = yTmp;
				column++;
			}
		}
		displayBox.width = (index / 10 + (index % 10 ? 1 : 0)) * 205 - 5;
		xButton.coordinates.x = displayBox.coordinates.x + displayBox.width - 40;
		int page = 0, x = -1, y = -1;
		while (true) {
			setactivepage(page);
			setvisualpage(1 - page);
			drawFrame();
			drawTaskBarButtons();
			drawMatrix();
			drawVertices();
			drawAllEdges();
			displayBox.draw();
			xButton.draw();
			delay(10);
			if (xButton.isHover())
				xButton.highLight(WHITE, LIGHTRED);
			if (ismouseclick(WM_LBUTTONDOWN)) {
				getmouseclick(WM_LBUTTONDOWN, x, y);
				clearmouseclick(WM_LBUTTONDOWN);
			}
			if (x >= xButton.coordinates.x && x <= xButton.coordinates.x + xButton.width
			&& y >= xButton.coordinates.y && y <= xButton.coordinates.y + xButton.height)
				break;
			for (int i = 0; i < index; i++) {
				fileButton[i].draw();
				if (fileButton[i].isHover())
					fileButton[i].highLight(LIGHTBLUE, BLACK);
				if (x >= fileButton[i].coordinates.x && x <= fileButton[i].coordinates.x + fileButton[i].width
				&& y >= fileButton[i].coordinates.y && y <= fileButton[i].coordinates.y + fileButton[i].height) {
					bool confirm = drawYesNoBar("Ban co muon luu do thi vao file nay?");
					if (confirm) {
						confirm = drawYesNoBar("Du lieu file cu se bi mat. Tiep tuc");
						if (confirm) {
							char fileSaveName[60] = "";
							strcat(fileSaveName, "filesInProgram//");
							strcat(fileSaveName, fileButton[i].name);
							//strcat(fileSaveName, ".txt");
							fstream output;
							output.open(fileSaveName, ios::out);
							output << n << endl;
							for (int k = 0; k < n; k++) 
								output << vertices[k].coordinates.x << " " 
									<< vertices[k].coordinates.y << " "
									<< vertices[k].name << endl;
							for (int k = 0; k < n; k++) {
								for (int j = 0; j < n; j++)
									output << G[k][j] << " ";
								output << endl; 
							} 
							output.close();
							showSuccessfullyBox("Luu file thanh cong!");
							return saveFile();
						}
						else 
							return saveFile();
					} 
					else 
						return saveFile();
				}
			}
			clearmouseclick();
			page = 1 - page;
		}
	}
}

void deleteFile() {
//	clearmouseclick();
	char fileName[41][100] = {""};
	int index = 0;
	struct dirent *d;
	struct stat dst;
	DIR *dr;
	char path[30] = "filesInProgram";
	dr = opendir(path);
	if (dr != NULL) {
		for (d = readdir(dr); d != NULL; d = readdir(dr)) 
			if (strlen(d->d_name) >= 5 && strcmp(d->d_name, "startUpFile.txt") != 0 && isFileText(d->d_name))
				strcpy(fileName[index++], d->d_name);
		closedir(dr);	
	}
	if (index) {
		int x0 = W_LEFT + 5, y0 = W_TOP + 50;
		int column = 1;
		Button fileButton[index], displayBox, xButton;
		displayBox.init(x0, y0 - 45, 40, 0, LIGHTRED, BLACK, 1, "Chon file muon xoa");
		xButton.init(0, y0 - 45, 40, 40, WHITE, RED, 1, "x");
		int yTmp = y0;
		for (int i = 0; i < index; i++) {
			fileButton[i].init(x0, y0, 40, 200, WHITE, BLACK, 1, fileName[i]);
			y0 += 45;
			if (i + 1 == 10 * column) {
				x0 += 205;
				y0 = yTmp;
				column++;
			}
		}
		displayBox.width = (index / 10 + (index % 10 ? 1 : 0)) * 205 - 5;
		xButton.coordinates.x = displayBox.coordinates.x + displayBox.width - 40;
		int page = 0, x = -1, y = -1;
		while (true) {
			setactivepage(page);
			setvisualpage(1 - page);
			drawFrame();
			drawTaskBarButtons();
			drawMatrix();
			drawVertices();
			drawAllEdges();
			displayBox.draw();
			xButton.draw();
			delay(10);
			if (xButton.isHover())
				xButton.highLight(WHITE, LIGHTRED);
			if (ismouseclick(WM_LBUTTONDOWN)) {
				getmouseclick(WM_LBUTTONDOWN, x, y);
				clearmouseclick(WM_LBUTTONDOWN);
			}
			if (x >= xButton.coordinates.x && x <= xButton.coordinates.x + xButton.width 
			&& y >= xButton.coordinates.y && y <= xButton.coordinates.y + xButton.height)
				break;
			for (int i = 0; i < index; i++) {
				fileButton[i].draw();
				if (fileButton[i].isHover())
					fileButton[i].highLight(LIGHTBLUE, LIGHTRED);
				if (x >= fileButton[i].coordinates.x && x <= fileButton[i].coordinates.x + fileButton[i].width
				&& y >= fileButton[i].coordinates.y && y <= fileButton[i].coordinates.y + fileButton[i].height) {
					bool confirm = drawYesNoBar("Ban chac muon xoa file nay?");
					if (confirm) {
						confirm = drawYesNoBar("Du lieu se bi mat! tiep tuc");
						if (confirm) {
							char fileDeleteName[100] = "filesInProgram\\";
							strcat(fileDeleteName, fileButton[i].name);
							DeleteFile(fileDeleteName);
							showSuccessfullyBox("Xoa file thanh cong!");
							return deleteFile();
						} else 
							return deleteFile();
					}
					else 
						return deleteFile();
				}	
			}
			if (kbhit()) {
				char key = getch();
				if (key == KEY_ENTER)
					break;
			}
			clearmouseclick();
			page = 1 - page;
		}
	}
}

bool addFile() {
	clearmouseclick();
	int margin = 5;
	int height = 275 - 3 * margin - 40;
	int width = 400 - 2 * margin;
	char name[31] = "";
	char request[] = "Nhap ten file";
	int i = 0, x, y;
	int page = 0;
	char fileName[41][100] = {""};
	int index = 0;
	struct dirent *d;
	struct stat dst;
	DIR *dr;
	char path[60] = "filesInProgram";
	dr = opendir(path);
	if (dr != NULL) {
		for (d = readdir(dr); d != NULL; d = readdir(dr)) {
			if (strlen(d->d_name) >= 5 && strcmp(d->d_name, "startUpFile.txt") != 0 && isFileText(d->d_name))
				strcpy(fileName[index++], d->d_name);
			if (index > 40)
				break;
		}
		closedir(dr);	
	}
	Button frame, finishButton, cancelButton, enterNameBar;
	frame.init(15 + margin, 15 + 2 * margin + 40, height, width, YELLOW, DARKGRAY, 1, "");
	finishButton.init(15 + 2 * margin, 275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HOAN THANH");
	cancelButton.init(15 + 3 * margin + (width - 3 * margin) / 2,275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HUY");
	enterNameBar.init(15 + 2 * margin, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50, 40, (width - 2 * margin), YELLOW, BLACK, 9, "");
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawVertices();
		//drawMatrix();
		drawAllEdges();
		drawTaskBarButtons();
		frame.draw();
		finishButton.draw();
		cancelButton.draw();
		enterNameBar.draw();
		bool isNamesake = false;
		char nameTemp[35];
		strcpy(nameTemp, name);
		strcat(nameTemp, ".txt");
		for (int j = 0; j < index; j++)  
			if (strcmp(fileName[j], nameTemp) == 0) {
				isNamesake = true;
				break;
			}
		if (isNamesake) {
			char existFileText[25];
			strcpy(existFileText, "Ten file da ton tai!");
			outtextxy(frame.coordinates.x + (frame.width - textwidth(existFileText)) / 2, frame.coordinates.y + 25, existFileText);
		}
		if (ismouseclick(WM_LBUTTONDOWN)) {
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x < frame.coordinates.x || x > frame.coordinates.x + frame.width
			|| y < frame.coordinates.y || y > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0)
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
			}
			clearmouseclick(WM_LBUTTONDOWN);
		}
		if (ismouseclick(WM_RBUTTONDOWN)) {
			int xR, yR;
			getmouseclick(WM_RBUTTONDOWN, xR, yR);
			if (xR < frame.coordinates.x || xR > frame.coordinates.x + frame.width
			|| yR < frame.coordinates.y || yR > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0)
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
			}
			clearmouseclick(WM_RBUTTONDOWN);
		}
		if (ismouseclick(WM_RBUTTONDBLCLK)) {
			int xR, yR;
			getmouseclick(WM_RBUTTONDBLCLK, xR, yR);
			if (xR < frame.coordinates.x || xR > frame.coordinates.x + frame.width
			|| yR < frame.coordinates.y || yR > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0)
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
			}
			clearmouseclick(WM_RBUTTONDBLCLK);
		}
		if (ismouseclick(WM_LBUTTONDBLCLK))
			clearmouseclick(WM_LBUTTONDBLCLK);
		if (strcmp(name, "") == 0) 
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
		if (finishButton.isHover())
			finishButton.highLight();
		if (cancelButton.isHover())	
			cancelButton.highLight();
		if (strcmp(name, "") != 0 && !isNamesake
		&& x >= finishButton.coordinates.x && x <= finishButton.coordinates.x + finishButton.width
		&& y >= finishButton.coordinates.y && y <= finishButton.coordinates.y + finishButton.height)
			break;
		if (x >= cancelButton.coordinates.x && x <= cancelButton.coordinates.x + cancelButton.width
		&& y >= cancelButton.coordinates.y && y <= cancelButton.coordinates.y + cancelButton.height) 
			return false;
		if (ismouseclick(WM_LBUTTONDOWN)) {
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x >= finishButton.coordinates.x && x <= finishButton.coordinates.x + finishButton.width
			&& y >= finishButton.coordinates.y && y <= finishButton.coordinates.y + finishButton.height
			&& strcmp(name, "") != 0)
				break; 
		}
		if (kbhit()) {
			char key = getch();
			if ((key >= 'A' && key <= 'z') || (key >= '1' && key <= '9'))
				if (i < 30) {
					name[i] = key;
					i++;
				}
			if (i > 30)
				i = 30;
			if (key == KEY_BACKSPACE) {
				strnDel(name, i - 1, 1);
				i--;
			}
			if (i < 0)
				i = 0;
			if (strcmp(name, "") != 0 && key == KEY_ENTER && !isNamesake)
				break;
			if (strcmp(name, "") == 0 && key == KEY_ENTER) {
				putchar(7);
				frame.highLight(WHITE, RED);
				enterNameBar.draw();
				outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				finishButton.draw();
				cancelButton.draw();
				delay(300);
			}
			if (isNamesake && key == KEY_ENTER) {
				char existFileText[25];
				strcpy(existFileText, "Ten file da ton tai!");
				putchar(7);
				char c = getcolor();
				setcolor(RED);
				outtextxy(frame.coordinates.x + (frame.width - textwidth(existFileText)) / 2, frame.coordinates.y + 25, existFileText);
				delay(300);
				setcolor(c);			
			}
		}
		outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);
		if (i < 30 && strcmp(name, "") != 0)
				outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2 + textwidth(name), 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, "_");
		page = 1 - page;
	}
	if (strcmp(name, "") != 0) {
		char fileName[100] = "filesInProgram//";
		strcat(fileName, name);
		strcat(fileName, ".txt");
		bool comfirm = drawYesNoBar("Luu do thi hien tai vao file?");
		if (comfirm) {
			fstream output;
			output.open(fileName, ios::out);
			output << n << endl;
			for (int i = 0; i < n; i++) {
				output << vertices[i].coordinates.x << " "
					 << vertices[i].coordinates.y << " "
					 << vertices[i].name << endl;
			}
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					output << G[i][j] << " ";
				}
				output << endl;
			}
			output.close();
		}
		else {
			fstream output;
			output.open(fileName, ios::out);
			output << 0;
			output.close();	
		}
	}
	return true;
}

void showNoResult(char *resultStr) {
	int page = 0;
	Button resultBox, xButton;
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, resultStr);
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawTaskBarButtons();
		drawMatrix();
		drawVertices();
		drawAllEdges();
		resultBox.draw();
		drawKeyToExitText();
		xButton.draw();
		if (xButton.isClickLMButton())
			break;
		if (kbhit()) {
			char key = getch();
			break;
		}
		page = 1 - page;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool isSafe(int v, int *path, int pos) {																													//
//	//mot chuc nang tien ich de kiem tra xem dinh v co the 																									//
//	//duoc them vao chi muc pos trong chu ky Hamilton duoc xay dung cho den nay																				//
//	//(duoc luu tru trong path[])																															//
//	//kiem tra xem dinh nay co ke voi dinh da them truoc do khong																							//
//	if (!G[path[pos - 1]][v])																																//
//		return false;            																															//
//	//kiem tra xem dinh da duoc bao gom chua																												//
//	for (int i = 0; i < pos; i++)                                                                                                     						//
//		if (path[i] == v)                                                                                                                                 	//
//			return false;                                                                                                                                 	//
//	return true;																																		  	//
//}																																							//
//																																							//																																					       //
//bool hamCycleUtil(int v, int *path, int pos) {																											//
//	//neu tat ca cac dinh la bao gom trong chu trinh Hamilton																								//
//	if (pos == n) {																																			//
//		//va neu co mot canh tu dinh bao gom dinh cuoi cung den dinh dau tien 																				//
//		if (G[path[pos - 1]][path[v]])																														//
//			return true;																																	//
//		else 																																				//
//			return false;																																	//
//	}																																						//
//	//hay thu cac dinh khac nhau nhu mot dinh co kha nang tiep theo 																						//
//	//trong chu trinh Hamilton. Khong bao gom 0 vi da bao gom dinh 0 																						//
//	//lam diem bat dau trong hamCycle()																														//
//	for (int u = 0; u < n; u++) {																															//
//		//kiem tra xem co the them dinh nay vao chu trinh Hamilton hay khong																				//
//		if (isSafe(u, path, pos) && v != u) {																												//
//			path[pos] = u;																																	//
//			//lap lai de xay dung phan con lai cua duong di																									//
//			if (hamCycleUtil(v, path, pos + 1) == true)																										//
//				return true;																																//
//			//neu dinh them v khong dan den duong di, loai bo no																							//
//			path[pos] = -1;																																	//
//		}																																					//
//	}																																						//
//	//neu khong co dinh nao co the duoc																														//
//	//them vao chu ky Hamilton duoc xay dung																												//
//	//cho den hien tai thi tra ve false																														//
//	return false;																																			//
//}																																							//
//																																							//
//void hamCycle() {																																			//
//	if (isEmptyVertex()) {																																	//
//		showEmptyVertex();																																	//
//		return;																																				//
//	}																																						//
//	int *path = new int[n];																																	//
//	for (int i = 0; i < n; i++)																																//
//		path[i] = -1;																																		//
//	//dat dinh 0 la dinh dau tien trong duong di																											//
//	//Neu co mot chu trinh Hamilton																															//
//	//thi duong di co the la bat dau tu diem bat ky 																										//
//	//diem nao cua chu ky vi do thi la vo huong																												//
//	path[0] = 0;																																			//
//	if (hamCycleUtil(0, path, 1) == false) {																												//
//		showNoResult("Do thi khong ton tai chu trinh Hamilton.");																							//
//		return;																																				//
//	}																																						//			
//	int v = chooseVertex("Chon dinh bat dau chu trinh Hamilton");																							//
//	int index;																																				//
//	for (index = 0; index < n; index++)																														//
//		if (path[index] == v) {																																//
//			break;																																			//
//		}																																					//
//	int *subPath = new int[index];																															//
//	for (int i = 0; i < index; i++) 																														//
//		subPath[i] = path[i];																																//
//	for (int i = index; i < n; i++)																															//
//		path[i - index] = path[i];																															//
//	for (int i = n - index; i < n; i++)																														//
//		path[i] = subPath[i - n + index];																													//
//	showResultHamiltonCycle(path);																															//
//}																																							//
//																																							//
//void showResultHamiltonCycle(int *path) {																													//
//																																							//
//	char resultStr[200] = "";																																//
//	Button resultBox, xButton;																																//
//	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");																									//
//	for (int i = 0; i < n; i++) {																															//
//		strcat(resultStr, vertices[path[i]].name);																											//
//		strcat(resultStr, " -> ");																															//
//	}																																						//
//	strcat(resultStr, vertices[path[0]].name);																												//
//	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, resultStr);																						//
//	int page = 0;																																			//
//	while (true) {																																			//
//		setactivepage(page);																																//
//		setvisualpage(1 - page);																															//
//		drawFrame();																																		//
//		drawTaskBarButtons();																																//
//		drawMatrix();																																		//
//		drawVertices();																																		//
//		drawAllEdges();																																		//
//		resultBox.draw();																																	//
//		drawEnterToExitText();																																//
//		xButton.draw();																																		//
//		setactivepage(1);																																	//
//		setvisualpage(1);																																	//
//		vertices[path[0]].highLight();																														//
//		for (int i = 1; i < n; i++) {																														//
//			drawArrow(vertices[path[i - 1]], vertices[path[i]], LIGHTGREEN, 0);																				//
//			delay(200);																																		//
//			vertices[path[i]].highLight();																													//
//			if (xButton.isClickLMButton())																													//
//				return;																																		//
//			if (kbhit()) {																																	//
//				char key = getch();																															//
//				if (key == KEY_ENTER)																														//
//					return;																																	//
//			}																																				//
//		} 																																					//
//		drawArrow(vertices[path[n - 1]], vertices[path[0]], LIGHTGREEN, 0);																					//
//		setactivepage(page);																																//
//		setvisualpage(1 - page);																															//
//		if (xButton.isClickLMButton())																														//
//			break;																																			//
//		if (kbhit()) {																																		//
//			char key = getch();																																//
//			if (key == KEY_ENTER)																															//
//				break;																																		//
//		}																																					//
//		page = 1 - page;																																	//
//	}																																						//
//}																																							//																
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void loadFile(char *fileName) {
	char path[100] = "filesInProgram//";
	strcat(path, fileName);
	ifstream input (path);
	if (input.is_open()) {
		input >> n;
		for (int i = 0; i < n; i++) {
			input >> vertices[i].coordinates.x >> vertices[i].coordinates.y;
			vertices[i].name = new char[3];
			input >> vertices[i].name;
			if (strcmp(vertices[i].name, "") == 0) {
				n = 0;
				input.close();
				return;
			}
			
		}
		for (int i = 0; i < n; i++) 
			for (int j = 0; j < n; j++) 
				input >> G[i][j];
	} else {
		n = 0;
		FILE * newFile = fopen("filesInProgram//startUpFile.txt", "a");
		fprintf(newFile, "%d", 0);
		fclose(newFile);
	}
 
	input.close();
}

void openFile() {
//	clearmouseclick();
	char fileName[41][100] = {""};
	int index = 0;
	struct dirent *d;
//	struct stat dst;
	DIR *dr;
	char path[60] = "filesInProgram";
	dr = opendir(path);
	if (dr != NULL) {
		for (d = readdir(dr); d != NULL; d = readdir(dr)) {
			if (strlen(d->d_name) >= 5 && strcmp(d->d_name, "startUpFile.txt") != 0 && isFileText(d->d_name))
				strcpy(fileName[index++], d->d_name);
			if (index > 40)
				break;
		}
		closedir(dr);	
	}
	if (index) {
		int x0 = W_LEFT + 5, y0 = W_TOP + 50;
		int column = 1;
		Button fileButton[index], addFileButton, displayBox, xButton;
		displayBox.init(x0, y0 - 45, 40, 0, YELLOW, BLACK, 1, "Danh sach file (path: filesInProgram)");
		xButton.init(0, y0 - 45, 40, 40, WHITE, RED, 1, "x");
		int yTmp = y0;
		for (int i = 0; i < index; i++) {
			fileButton[i].init(x0, y0, 40, 200, WHITE, BLACK, 1, fileName[i]);
			y0 += 45;
			if (i + 1 == 10 * column) {
				x0 += 205;
				y0 = yTmp;
				column++;
			}
		}
		addFileButton.init(x0, y0, 40, 200, WHITE, BLACK, 1, "+");
		displayBox.width = x0 + 200 - displayBox.coordinates.x; 
		xButton.coordinates.x = displayBox.coordinates.x + displayBox.width - 40;  
		int page = 0, x = -1, y = -1;
		while (true) {
			setactivepage(page);
			setvisualpage(1 - page);
			delay(100);
			drawFrame();
			drawTaskBarButtons();
			drawMatrix();
			drawVertices();
			drawAllEdges();
			displayBox.draw();
			xButton.draw();
			if (xButton.isHover())
				xButton.highLight(WHITE, LIGHTRED);
			if (ismouseclick(WM_LBUTTONDOWN)) {
				getmouseclick(WM_LBUTTONDOWN, x, y);
				clearmouseclick(WM_LBUTTONDOWN);
			}
			if (x >= xButton.coordinates.x && x <= xButton.coordinates.x + xButton.width
			&& y >= xButton.coordinates.y && y <= xButton.coordinates.y + xButton.height)
				break;
			if (index < 40) {
				//settextstyle(1, 0, 4);
				addFileButton.draw();
				//setUserTextStyle();
				char tmpAddName[2] = "+";
				if (addFileButton.isHover()) {
					addFileButton.name = "Tao file moi";
					addFileButton.highLight(YELLOW, BLACK);
				}
				addFileButton.name = tmpAddName;
				if (x >= addFileButton.coordinates.x && x <= addFileButton.coordinates.x + addFileButton.width 
				&& y >= addFileButton.coordinates.y && y <= addFileButton.coordinates.y + addFileButton.height) {
					bool confirm = drawYesNoBar("Ban muon tao 1 file moi?");
					if (confirm) {
						bool success = addFile();
						if (success)
							showSuccessfullyBox("Them file thanh cong!");
						return openFile();
					}
					else 
						return openFile();
				}

			}
			for (int i = 0; i < index; i++) {
				fileButton[i].draw();
				if (fileButton[i].isHover())
					fileButton[i].highLight(YELLOW, BLACK);
				if (x >= fileButton[i].coordinates.x && x <= fileButton[i].coordinates.x + fileButton[i].width
				&& y >= fileButton[i].coordinates.y && y <= fileButton[i].coordinates.y + fileButton[i].height) {
					bool confirm = drawYesNoBar("Ban co muon load file nay?");
					if (confirm) {
						loadFile(fileName[i]);
						if (n == 0)
							showSuccessfullyBox("Ops! File nay khong co gi!");
						else
							showSuccessfullyBox("Load file thanh cong!");
						return;
					}
					else 
						return openFile();
				}
				if (fileButton[i].isHover() && kbhit()) {
					char key = getch();
					if (key == KEY_ENTER) {
						bool confirm = drawYesNoBar("Ban co muon load file nay?");
						if (confirm) {
							loadFile(fileName[i]);
							return;
						}
					}
				}					
			}
			if (kbhit()) {
				char key = getch();
				if (key == KEY_ENTER)
					break;
			}
			clearmouseclick();
			page = 1 - page;
		}
	}
	setUserTextStyle();
}

void showResultCutVertices(bool *isCutVertex, int counter) {
	char resultText[100];
	Button resultBox, xButton;
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	if (counter == 0)
		strcpy(resultText, "Do thi khong co dinh tru.");
	else {
		strcpy(resultText, "Do thi co ");
		char counterStr[4];
		itoa(counter, counterStr, 10);
		strcat(resultText, counterStr);
		strcat(resultText, " dinh tru: ");
		for (int i = 0; i < n; i++)
			if (isCutVertex[i]) {
				strcat(resultText, "dinh ");
				strcat(resultText, vertices[i].name);
				strcat(resultText, ", ");
			}
		strnDel(resultText, strlen(resultText) - 2, 2);
	}
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, resultText);
	int page = 0;
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawTaskBarButtons();
		drawVertices();
		drawAllEdges();
		drawMatrix();
		resultBox.draw();
		xButton.draw();
		drawEnterToExitText();
		for (int i = 0; i < n; i++)
			if (isCutVertex[i])
				vertices[i].highLight(BLACK, GREEN);
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;
		}
		if (xButton.isClickLMButton())
			break;
		page = 1 - page;
	}
}

void cutVertices() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;
	}
	int disc[n] = {0};//luu tru so lan tham cua cac dinh da tham
	int low[n];
	int par = -1;
	int counter = 0;
	bool isCutVertex[n] = {false};
	setArrayTo(visited, n, false);
	Time = 0;
	for (int i = 0; i < n; i++)
		if (!visited[i])
			cutVerticesUtil(i, disc, low, par, isCutVertex);
	for (int i = 0; i < n; i++)
		if (isCutVertex[i])
			counter++;
	drawLoading("Dang tim dinh tru...");
	showResultCutVertices(isCutVertex, counter);
}

void cutVerticesUtil(int u, int *disc, int *lowLink, int parent, bool *isCutVertex) {
	//dem so luong con trong cay DFS
	int children = 0;
	//danh dau dinh u da tham
	visited[u] = true;
	disc[u] = lowLink[u] = ++Time;
	for (int v = 0; v < n; v++) {
		if (G[u][v] || G[v][u]) {
			//neu v chua duoc tham, dat no thanh con cua u trong cay DFS va lap lai cho no
			if (!visited[v]) {
				children++;
				cutVerticesUtil(v, disc, lowLink, u, isCutVertex);
				//Kiem tra xem cay con bat nguon tu v c? ket noi v0i mot trong c?c goc cua u hay khong
				lowLink[u] = min(lowLink[u], lowLink[v]);
				//neu u khong phai la goc va lowLink[v] >= disc[u]
				if (parent != -1 && lowLink[v] >= disc[u])
					isCutVertex[u] = true;
			}
			else if (v != parent)
				//cap nhat gia tri lowLink[u] cho lenh goi ham parent
				lowLink[u] = min(lowLink[u], disc[v]);
		}
	}
	//neu u la goc cua cay DFS va co hon 2 con
	if (parent == -1 && children > 1)
		isCutVertex[u] = true;
}

void showEmptyVertex()  {
	Point center;
	Button emptyBox;
//	char emptyStr[25];
//	strcpy(emptyStr, "Chua co dinh nao ca!");
	center.x = (W_LEFT + W_RIGHT) / 2;
	center.y = (W_TOP + W_BOTTOM) / 2;
	emptyBox.init(center.x - 200 / 2, center.y - 80 / 2, 80, 200, YELLOW, BLACK, 1, "Chua co dinh nao ca!");
	int page = 0, startTime, endTime, timeCount;
	time_t now = time(0);
	tm *ltm = localtime(&now);
	startTime =ltm->tm_hour * 3600 + ltm->tm_min * 60 + ltm->tm_sec;
	char c = getcolor();
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		now = time(0);
		ltm = localtime(&now);
		endTime = ltm->tm_hour * 3600 + ltm->tm_min * 60 + ltm->tm_sec;
		timeCount = endTime - startTime;
		if (timeCount >= 3)
			break;
		drawFrame();
		drawTaskBarButtons();
		setlinestyle(SOLID_LINE, 1, 3);
		setcolor(LIGHTRED);	
		line(W_RIGHT, W_TOP, W_LEFT, W_BOTTOM);	
		setlinestyle(SOLID_LINE, 1, 1);
		setcolor(c);
		emptyBox.draw();
		page = 1 - page;
		if (ismouseclick(WM_LBUTTONDOWN) || ismouseclick(WM_RBUTTONDOWN) || ismouseclick(WM_LBUTTONDBLCLK)) {
			clearmouseclick();
			break;
		}
	}
}

void showResultBridgeEdge(int *trace, int counter) {
	char resultText[200];
	Button resultBox, xButton;
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	if (counter == 0)
		strcpy(resultText, "Do thi nay khong co canh cau");
	else {
		strcpy(resultText, "Do thi co ");
		char counterStr[4];
		itoa(counter, counterStr, 10);
		strcat(resultText, counterStr);
		strcat(resultText, " canh cau: ");
		for (int i = 0; i < counter * 2; i += 2) {
			int u = trace[i];
			int v = trace[i + 1];
			strcat(resultText, vertices[u].name);
			strcat(resultText, vertices[v].name);
			strcat(resultText, ", ");
		}
		strnDel(resultText, strlen(resultText) - 2, 2);
	}
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, resultText);
//	cout << resultText << endl;
	int page = 0; 
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawTaskBarButtons();
		drawMatrix();
		drawVertices();
		drawAllEdges();
		drawEnterToExitText();
		resultBox.draw();
		xButton.draw();
		for (int i = 0; i < counter * 2; i += 2) {
			int u = trace[i];
			int v = trace[i + 1];
			if (G[u][v] == -1) 
				drawLine(vertices[u], vertices[v], LIGHTGREEN, 0);
			else if (!G[v][u] && G[u][v])
				drawArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
			else if (G[v][u] && !G[u][v])
				drawArrow(vertices[v], vertices[u], LIGHTGREEN, G[v][u]);
			else {
				if (u < v) {
					drawArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
					drawCurvedArrow(vertices[v], vertices[u], LIGHTGREEN, G[v][u]);
				}
				else {
					drawArrow(vertices[v], vertices[u], LIGHTGREEN, G[v][u]);
					drawCurvedArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);		
				}
			}
		}
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;
		}
		if (xButton.isClickLMButton())
			break;
		page = 1 - page;
	}
	clearmouseclick();
}

void bridgeUtil(int u, int disc[], int lowLink[], int parent[], int &counter, int *trace, int &index) {
	//danh dau dinh do da tham
	visited[u] = true;
	disc[u] = lowLink[u] = ++Time;
	for (int i = 0; i < n; i++) {
		if (G[u][i]) {
			if (!visited[i]) {
				parent[i] = u;
				bridgeUtil(i, disc, lowLink, parent, counter, trace, index);
				lowLink[u] = min(lowLink[u], lowLink[i]);
				if (lowLink[i] > disc[u]) {
					counter++;
					trace[index] = u;
					trace[index + 1] = i;
					index += 2;
				} 
			}
			else if (i != parent[u])
				lowLink[u] = min(lowLink[u], disc[i]);
		}
	}
}

void bridgeEdges() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;	
	}

	int index = 0;
	int trace[n * n];//mang luu cac he so cua dinh
	int componentMax = countStrongConComponent();//so thanh phan lien thong ban dau
	if (!isUndirectedGraph()) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int tmp = G[i][j];
				G[i][j] = 0;
				if (countStrongConComponent() > componentMax) {
					trace[index] = i;
					trace[index + 1] = j;
					index += 2;  
				} 
				G[i][j] = tmp;
			}
		}
	}
	else {
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				int tmp = G[i][j];
				G[i][j] = G[j][i] = 0;
				if (countStrongConComponent() > componentMax) {
					trace[index] = i;
					trace[index + 1] = j;
					index += 2;  
				} 
				G[i][j] = G[j][i] = tmp;
			}
		}	
	}
	drawLoading("Dang tim canh cau...");
	showResultBridgeEdge(trace, index / 2);	
//	cout << index << endl;
}

void drawUserManualBox(char *guideStr, char *title) {	
	Button helpBoxFrame, titleBox;	
	char word[200][10], rowSentences[15][1000] = {""}; 
	
	int wordCounter = 0, index = 0;
	helpBoxFrame.init(20, 65, 220, 390, YELLOW, BLACK, 1, "");
	titleBox.init(20, 65, 40, 390, YELLOW, BLACK, 1, title);
	for (int i = 0; i < strlen(guideStr); i++) {
		//tach tung tu cua cau huong dan ra
		int j = i;
		int k = 0;
		while (guideStr[j] != ' ' && j < strlen(guideStr)) {
			word[wordCounter][k] = guideStr[j];
			k++;
			j++;
		}
		word[wordCounter][k] = '\0';
		wordCounter++;
		i = j;
		if (i >= strlen(guideStr))
			break;
	}
	int h = 0;
	//tach huong dan ra tung dong
	while (h < wordCounter) {
		while (textwidth(rowSentences[index]) < 340) {
			strcat(rowSentences[index], word[h++]);
			strcat(rowSentences[index], " ");
			if (h >= wordCounter)
				break;
		}
		index++;
		if (h >= wordCounter)
			break;
	}
	clearmouseclick();
	int page = 0;
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawTaskBarButtons();
		drawMatrix();
		drawVertices();
		drawAllEdges();
		helpBoxFrame.draw();
		titleBox.draw();
		for (int i = 0; i < index; i++) {
			outtextxy(20 + (390 - textwidth(rowSentences[i])) / 2, 65 + 45 + i * 30, rowSentences[i]);
		}
		if (ismouseclick(WM_LBUTTONDBLCLK)) {
			int x, y;
			getmouseclick(WM_LBUTTONDBLCLK, x, y);
			if (x < helpBoxFrame.coordinates.x || x > helpBoxFrame.coordinates.x +  helpBoxFrame.width
			|| y < helpBoxFrame.coordinates.y || y > helpBoxFrame.coordinates.y +  helpBoxFrame.height)
				break;
		}
		if (ismouseclick(WM_RBUTTONDOWN)) {
			int x, y;
			getmouseclick(WM_RBUTTONDOWN, x, y);
			if (x < helpBoxFrame.coordinates.x || x > helpBoxFrame.coordinates.x +  helpBoxFrame.width
			|| y < helpBoxFrame.coordinates.y || y > helpBoxFrame.coordinates.y +  helpBoxFrame.height)
				break;
		}	
		if (ismouseclick(WM_LBUTTONDOWN)) {
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x < helpBoxFrame.coordinates.x || x > helpBoxFrame.coordinates.x +  helpBoxFrame.width
			|| y < helpBoxFrame.coordinates.y || y > helpBoxFrame.coordinates.y +  helpBoxFrame.height)
				break;
		}
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;
		}
		page = 1 - page;
	}
}


int min(int a, int b) {
	return (a < b) ? a: b;
}


void tarjanAlgo(int u, int disc[], int lowLink[], stack &stk, bool stkItem[], int **componentsList, int &counter) {
	disc[u] = lowLink[u] = ++Time;
	stk.push(u);
	stkItem[u] = true;
	for (int v = 0; v < n; v++) {
		if (G[u][v]) {	
			if (disc[v] == -1) {
				tarjanAlgo(v, disc, lowLink, stk, stkItem, componentsList, counter);
				lowLink[u] = min(lowLink[u], lowLink[v]);
			} 
			else if (stkItem[v])
				lowLink[u] = min(lowLink[u], disc[v]);
		}
	}
	int poppedItem = 0;
	if (lowLink[u] == disc[u]) {
		int k = 0;
		do {
			poppedItem = stk.pop();
			componentsList[counter][k++] = poppedItem;
			stkItem[poppedItem] = false;
		} while (poppedItem != u);
		counter++;
	}
}


void tarjanAlgo2(int u, int disc[], int lowLink[], stack &stk, bool stkItem[], int &counter) {
	disc[u] = lowLink[u] = ++Time;
	stk.push(u);
	stkItem[u] = true;
	for (int v = 0; v < n; v++) {
		if (G[u][v]) {	
			if (disc[v] == -1) {
				tarjanAlgo2(v, disc, lowLink, stk, stkItem, counter);
				lowLink[u] = min(lowLink[u], lowLink[v]);
			} 
			else if (stkItem[v])
				lowLink[u] = min(lowLink[u], disc[v]);
		}
	}
	int poppedItem = 0;
	if (lowLink[u] == disc[u]) {
		do {
			poppedItem = stk.pop();
			stkItem[poppedItem] = false;
		} while (poppedItem != u);
		counter++;
	}
}

int countStrongConComponent(int **componentsList) {
	Time = 0;
	int disc[n], lowLink[n], counter;
	bool stkItem[n];
	counter = 0;
	stack stk;
	for (int i = 0; i < n; i++) {
		disc[i] = lowLink[i] = -1;
		stkItem[i] = false;
	}
	for (int i = 0; i < n; i++)
		if (disc[i] == -1)
			tarjanAlgo(i, disc, lowLink, stk, stkItem, componentsList, counter);
	return counter;
}

int countStrongConComponent() {
	Time = 0;
	int disc[n], lowLink[n], counter;
	bool stkItem[n];
	counter = 0;
	stack stk;
	for (int i = 0; i < n; i++) {
		disc[i] = lowLink[i] = -1;
		stkItem[i] = false;
	}
	for (int i = 0; i < n; i++)
		if (disc[i] == -1)
			tarjanAlgo2(i, disc, lowLink, stk, stkItem, counter);
	return counter;
}


void drawKeyToExitText() {
	char c = getcolor();
	setcolor(BLACK);
	setbkcolor(LIGHTGRAY);
	outtextxy(1259 - textwidth("press KEY to exit"), 520 - textheight("press KEY to exit"), "press KEY to exit");
	setcolor(c);
	setbkcolor(BLACK);	
}

void drawEnterToExitText() {
	char c = getcolor();
	setcolor(BLACK);
	setbkcolor(LIGHTGRAY);
	outtextxy(1259 - textwidth("press ENTER to exit"), 520 - textheight("press ENTER to exit"), "press ENTER to exit");
	setcolor(c);
	setbkcolor(BLACK);
}

template <typename Type>
void set2DArrayTo(Type **arr, unsigned height, unsigned width, int value) {
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			arr[i][j] = value;
}

void delete2DArray(int **arr, unsigned height, unsigned width) {
	for (int h = 0; h < height; h++)
		delete []arr[h];
	delete []arr;
	arr = 0;
}

void delete2DStrArray(char **arr, unsigned height, unsigned width) {
	for (int h = 0; h < height; h++)
		delete []arr[h];
	delete []arr;
}

int** create2DArray(unsigned height, unsigned width) {
    int** array2D = 0;
    array2D = new int*[height];//tao bien ma tran
    
    for (int h = 0; h < height; h++) {
    	//tao cot cho ma tran
        array2D[h] = new int[width];
        for (int w = 0; w < width; w++) 
            //cho gia tri ban dau cua moi phan tu cua ma tran la 0
            array2D[h][w] = 0;
    }
    return array2D;
}

char** create2DStrArray(unsigned height, unsigned width) {
	char** array2DStr;
	array2DStr = new char*[height];
	for (int h = 0; h < height; h++) {
		array2DStr[h] = new char[width];
	}
	return array2DStr;
}

int countConnectedComponents() {
	setArrayTo(visited, n, false);
	int counter = 0;
	for (int i = 0; i < n; i++)
		if (!visited[i]) {
			counter++;
			DFS(i);
		}
	return counter;
}

void connectedComponents() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;	
	}
	int **list = create2DArray(n, n), counter;
	set2DArrayTo(list, n, n, -1);
	bool isUGr = isUndirectedGraph();
	char resultText[100], counterStr[4];
	if (isUGr) {
		counter = countConnectedComponents(list);
		strcpy(resultText, "Do thi vo huong co ");
		itoa(counter, counterStr, 10);
		strcat(resultText, counterStr);
		strcat(resultText, " thanh phan lien thong");
		drawLoading("Dang tim thanh phan lien thong...");
		showResultConnectedComponents(list, counter, 1, resultText);
	}
	else /*if (!isConnectedGraph())*/ {
		counter = countStrongConComponent(list);
		if (counter == 1)
			//neu counter == 1 thi suy ra day la do thi lien thong manh
			strcpy(resultText, "Do thi nay la do thi co huong lien thong manh");
		else {
			strcpy(resultText, "Do thi co huong co ");
			itoa(counter, counterStr, 10);
			strcat(resultText, counterStr);
			strcat(resultText, " thanh phan lien thong manh");
		}
		drawLoading("Dang tim thanh phan lien thong...");
		showResultConnectedComponents(list, counter, 0, resultText);
	}
//	else {
//		counter = countConnectedComponents(list);
//		strcpy(resultText, "Do thi co");
//		itoa(counter, counterStr, 10);
//		strcat(resultText, counterStr);
//		strcat(resultText, " thanh phan lien thong");
//		drawLoading("Dang tim thanh phan lien thong...");
//		showResultConnectedComponents(list, counter, 1, resultText);
//	}
	delete2DArray(list, n, n);
}

void showResultConnectedComponents(int **connectedComponents, int count, bool isUndirectedGraph, char *resultText) {
	int page = 0;
	char componentsStr[n][30] = {""};
	Button resultBox, xButton, components[n];
	resultBox.init(425, 525, 40, 834, YELLOW, BLACK, 1, resultText);
	xButton.init(resultBox.coordinates.x + resultBox.width - 40, resultBox.coordinates.y + resultBox.height - 40, 40, 40, WHITE, RED, 1, "x");
	for (int i = 0; i < count; i++) {
		//duyet tung thanh phan lien thong
		//tuong ung voi so hang cua ma tran connectedComponents
		for (int j = 0; j < n; j++) {
			//moi thanh phan lien thong se co cac dinh
			//tuong uong voi tung cot cua ma tran connectedComponets
			int v = connectedComponents[i][j];
			if (v != -1) {
				//neu v khac -1 thi noi ten dinh v vao sau componentsStr[i]
				strcat(componentsStr[i], vertices[v].name);
				//noi dau phay
				strcat(componentsStr[i], ", ");
			}
		
		}
		//xoa dau phay o sau cua moi chuoi thanh phan lien thong
		strnDel(componentsStr[i], strlen(componentsStr[i]) - 2, 2);
		//sao chep thanh phan lien thong sang moi button cua no
		//de hien thi ra man hinh
		components[i].name = componentsStr[i];
		components[i].height = 40;
		if (count < 5)
			components[i].width = 200;
		else 
			components[i].width = 100;
			
		components[i].bColor = BLACK;
		components[i].tColor = YELLOW;
		components[i].pattern = 1;
	}
	int frag = count / 2,
		x0 = 425 + (834 - frag * components[0].width) / 2,
		y0 = 530 + 40; 
	for (int i = 0; i < frag; i++) {
		components[i].coordinates.x = x0;
		components[i].coordinates.y = y0;
		x0 += (components[i].width + 5);
	}
	x0 = 425 + (834 - (count - frag)* components[0].width) / 2,
	y0 += 45;
	
	for (int i = frag; i < count; i++) {
		components[i].coordinates.x = x0;
		components[i].coordinates.y = y0;
		x0 += (components[i].width + 5);
	}
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawTaskBarButtons();
		drawMatrix();
		drawVertices();
		drawEnterToExitText();
		drawAllEdges();
		resultBox.draw();
		xButton.draw();
		if (xButton.isClickLMButton())
			break;
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < n; j++) {
				int v = connectedComponents[i][j];
				if (v != -1) {
					//tu ham connectedComponents() 
					//ta gan gia tri cho ma tran connectedComponents la -1
					//nen o ham nay ta chi thao tac voi nhung 
					//dinh co gia tri khac -1
					vertices[v].highLight(YELLOW, i + 1);//mau i + 1 tuong tu nhu mau tu 1 toi 11 (voi dieu kien la MAX = 10)
					if (isUndirectedGraph) {
						//neu la do thi vo huong thi ve
						//cac duong thang khong co trong so w = 0;
						for (int k = 0; k < n; k++)
							if (G[v][k])
								drawLine(vertices[v], vertices[k], i, 0);
						}
					else {
						for (int k = 0; k < n; k++) {
							int h = connectedComponents[i][k];
							if (h != -1 && G[v][h]) {
								if (!G[h][v])
									drawArrow(vertices[v], vertices[h], i, G[v][h]);
								else {
									if (v < h) {
										drawCurvedArrow(vertices[h], vertices[v], i, G[h][v]);	
										drawArrow(vertices[v], vertices[h], i, G[v][h]);
									}
									else {
										drawCurvedArrow(vertices[v], vertices[h], i, G[v][h]);	
										drawArrow(vertices[h], vertices[v], i, G[h][v]);
									}
								}
							}
						}
					}
				}
			}
			components[i].draw();
			if (components[i].isHover()) {
				components[i].highLight();
				//neu di chuot vao vi tri o cua thanh phan lien thong 
				//nao thi cac dinh cua thanh phan lien thong do se duoc to sang
				for (int k = 0; k < n; k++) {
					int u = connectedComponents[i][k];
					if (u != -1)
						vertices[u].highLight(WHITE, 13);
				}
			}
		}
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;	
		}
		clearmouseclick();	
		page = 1 - page;
	}
}

int countConnectedComponents(int **connectedComponents) {
	//setArrayTo(visited, n, false);
	int counter = 0;
	bool isAdded[n];//mang nay co tac dung danh dau cac dinh 
					//da duoc them vao thanh phan lien thong truoc
	for (int i = 0; i < n; i++)
		//cho mang ve gia tri false
		isAdded[i] = false, visited[i] = false;
	for (int i = 0; i < n; i++)
		if (!visited[i]) {
			//neu chua tham dinh i thi duyet dfs bat dau tu dinh i
			//dinh tham roi se duoc danh dau la true
			//tu dinh i neu co dinh nao nam trong danh sach dinh ke
			//voi i thi nhung dinh do duoc goi la mot thanh phan lien thong
			//ta se luu no vao mot ma tran de tra ve ket qua
			//nhung thanh phan lien thong cua do thi
			DFS(i);
			int k = 0;
			for (int j = 0; j < n; j++)
				if (visited[j] && !isAdded[j]) {
					//neu dinh da duoc dfs duyet roi 
					//va dinh do chua duoc them vao thanh phan lien thong nao
					//thi them no vao thanh phan lien thong thu counter
					isAdded[j] = true;
					connectedComponents[counter][k] = j;	
					k++;
				}
			counter++;
		}
	return counter;
}

int DFS(int u) {
	int counter = 0;
	stack s;
	s.push(u);
	while (!s.isEmpty()) {
		u = s.pop();
		if (!visited[u]) {
			visited[u] = true;
			for (int i = 0; i < n; i++)
				if ((G[u][i] || G[i][u]) && !visited[i]) {
					counter++;
					s.push(i);
				} 
		}
	} 
	return counter;
}

bool isUndirectedGraph() {
	bool check = false;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			if (G[i][j] == -1) {
				check = true;
				break;
			}
		if (check)
			break;	
	} 
	if (check) 
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (G[i][j]) 
					G[i][j] = G[j][i] = -1;
	return check;	
}

bool isConnectedGraph() {
	//Kiem tra do thi co lien thong hay khong
	if (countConnectedComponents() == 1) return true;
	return false; 
}

void showResultEulerCycle(stack CE, char *resultText) {
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, "");
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	char resultStr[20] = "Chu trinh euler", eulerResultStr[200] = "";
	int trace[n * n];
	int counter = 0;
	while (!CE.isEmpty()) {
		char vertex[3];
		int v = CE.pop();
		strcpy(vertex, vertices[v].name);
		strcat(eulerResultStr, vertex);
		strcat(eulerResultStr, " -> ");
		trace[counter] = v;
		counter++;
	}
	strnDel(eulerResultStr, strlen(eulerResultStr) - 3, 3);
	resultBox.name = eulerResultStr;
	int page = 0;
	drawLoading("Dang tai ket qua chu trinh Euler...");
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(10);
		drawFrame();
		drawTaskBarButtons();
		drawVertices();
		drawAllEdges();
		drawEnterToExitText();
		drawMatrix();
		resultBox.draw();
		xButton.draw();
		outtextxy(425 + (834 - textwidth(resultText)) / 2, 535, resultText);
		vertices[trace[0]].highLight();
		setactivepage(1);
		setvisualpage(1);
		for (int i = 1; i < counter; i++) {
			int u = trace[i - 1];
			int v = trace[i];
			if (!G[v][u] || G[u][v] == -1)
				drawArrow(vertices[u], vertices[v], LIGHTGREEN, 0);
			else {
				if (v > u) 
					drawArrow(vertices[u], vertices[v], LIGHTGREEN, 0);
				else
					drawCurvedArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
			}
			delay(200);
			vertices[v].highLight();
			if (kbhit()) {
				char key = getch();
				if (key == KEY_ENTER)
					return;
			}
			if (xButton.isClickLMButton())
				return;
		}


		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;
		}
		if (xButton.isClickLMButton())
			break;
		clearmouseclick();
		page = 1 - page;			
	}
}

void eulerCycle() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;	
	}
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, "");
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	stack st, CE;
	if (!isConnectedGraph()) {
		char resultStr[100] = "Do thi nay khong co chu trinh Euler vi khong lien thong";
		resultBox.name = resultStr;
		int page = 0;
		while (true) {
			setactivepage(page);
			setvisualpage(1 - page);
			taskBarFrame.draw();
			drawTaskBarButtons();
			resultBox.draw();
			xButton.draw();
			drawKeyToExitText();
			if (xButton.isClickLMButton())
				return;
			if (kbhit()) {
				getch();
				return;
			}
			clearmouseclick();
			page = 1 - page;
		}
	}
	
	
	bool isUGr = isUndirectedGraph();
	if (isUGr) {
		//neu la do thi vo huong
		//dieu kien de do thi co chu trinh euler
		//khi va chi khi moi dinh cua no deu co bac chan
		//hoac co duy nhat 2 dinh bac le (truong hop nay chi tim duoc duong di euler)
		//khong co chu trinh euler
		//mang deg[] dung de luu bac cua tung dinh trong do thi
		int deg[n];
		for (int i = 0; i < n; i++) {
			deg[i] = 0;
			for (int j = 0; j < n; j++)
				if (G[i][j])
					deg[i]++;
		}
		int oddVertexDeg = 0;
		for (int i = 0; i < n; i++)
			//neu bac cua no la le thi do thi khong co chu trinh euler 
			if (deg[i] % 2) {
				oddVertexDeg++;
			}
		if (oddVertexDeg != 0 && oddVertexDeg != 2) {
			//neu do thi khong co chu trinh euler 
			//thi thong bao ra man hinh
			int page = 0;
			char resultStr[100] = "Do thi nay khong co chu trinh Euler vi dinh ";
			for (int i = 0; i < n; i++) {
				if (deg[i] % 2) {
					strcat(resultStr, vertices[i].name);
					strcat(resultStr, " ");
				}
			}
			strcat(resultStr, "co bac le.");
			resultBox.name = resultStr;
			while (true) {
				setactivepage(page);
				setvisualpage(1 - page);
				resultBox.draw();
				xButton.draw();
				drawKeyToExitText();
				taskBarFrame.draw();
				drawTaskBarButtons();
				if (kbhit())
					return;
				if (xButton.isClickLMButton())
					return;
				page = 1 - page;
			}
		}
		else if (oddVertexDeg == 2) {
			//truong hop do thi co hai dinh bac le duy nhat thi
			//co duong di euler
			//dinh bat dau duong di euler la dinh dau tien co dinh bac le
			int tmpMatrx[n][n];
			int u;
			for (u = 0; u < n; u++)
				if (deg[u] % 2)
					break;
			st.push(u);
				//sao chep ma tran G sang ma tran tmp
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					tmpMatrx[i][j] = G[i][j];
			while (!st.isEmpty()) {
				int counter = 0;
				int s = st.get();
				for (int i = 0; i < n; i++) {
					if (tmpMatrx[s][i] == 0)
						counter++;
				}
				if (counter != n) {
					for (int t = 0; t < n; t++) {
						if (tmpMatrx[s][t]) {
							st.push(t);
							tmpMatrx[s][t] /*= tmpMatrx[t][s]*/ = 0;
							if (isUGr)
								tmpMatrx[t][s] = 0;
							break;
						}
					} 
				} 
				else {
					CE.push(s);
					st.pop();
				}
			}
			showResultEulerCycle(CE, "Do thi co hai dinh bac le nen chi co duong di Euler. Khong co chu trinh Euler");
			return;
		}
	}
	else {
		//khi do thi la co huong
		//kiem tra ban bac ra va ban bac vao cua tat ca cac dinh
		//neu degIn != degOut thi khong co chu trinh euler
		int degOut[n], degIn[n], tmpMatrx[n][n];
		char resultStr[100] = "Do thi nay khong co chu trinh Euler vi dinh ";
		//tinh ban bac ra cua moi dinh
		for (int i = 0; i < n; i++) {
			degOut[i] = 0;
			for (int j = 0; j < n; j++)
				if (G[i][j])
					degOut[i]++;	
		} 
		//tinh ban bac vao cua moi dinh
		for (int i = 0; i < n; i++) {
			degIn[i] = 0;
			for (int j = 0; j < n; j++)
				if (G[j][i])
					degIn[i]++;
		}
		
		bool isEulerGraph = true;
		for (int i = 0; i < n; i++)
			//neu ban bac ra cua no khong bang ban bac vao
			if (degOut[i] != degIn[i]) {
				strcat(resultStr, vertices[i].name);
				strcat(resultStr, " ");
				isEulerGraph = false;
			}
		strcat(resultStr, " co ban bac ra khong bang ban bac vao");
		resultBox.name = resultStr;
		if (!isEulerGraph) {
			int page = 0;
			while (true) {
				setactivepage(page);
				setvisualpage(1 - page);
				taskBarFrame.draw();
//				ESCButton.draw();
				resultBox.draw();
				xButton.draw();
				drawKeyToExitText();
				if (kbhit())
					return;
				if (xButton.isClickLMButton())
					return;
				clearmouseclick();
				page = 1 - page;
			}	
		}
	}
	int u = chooseVertex("Chon dinh bat dau chu trinh Euler");
	st.push(u);//cho dinh u vao ngan xep
	int tmpMatrx[n][n];//luu ma tran ke vao mot ma tran tam thoi khac 
					//vi khi tim chu trinh Euler no se bi bien doi
	//sao chep ma tran G sang ma tran tmp
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			tmpMatrx[i][j] = G[i][j];
	
	while (!st.isEmpty()) {
		int counter = 0;
		int s = st.get();
		for (int i = 0; i < n; i++) {
			if (tmpMatrx[s][i] == 0)
				counter++;
		}
		if (counter != n) {
			for (int t = 0; t < n; t++) {
				if (tmpMatrx[s][t]) {
					st.push(t);
					tmpMatrx[s][t] /*= tmpMatrx[t][s]*/ = 0;
					if (isUGr)
						tmpMatrx[t][s] = 0;
					break;
				}
			} 
		} 
		else {
			CE.push(s);
			st.pop();
		}
	}
	showResultEulerCycle(CE, "Chu trinh Euler");
}

void showResultPathXY(int *trace, int *dist, int start, int end) {
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, "");
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	if (dist[end] == INT_MAX) {
		char str[80] = "Khong ton tai duong di tu ";
		strcat(str, vertices[start].name);
		strcat(str, " den ");
		strcat(str, vertices[end].name);
		strcat(str, "! Tu dong thoat sau [     s]");
//		strcpy(resultBox.name, str);
		resultBox.name = str;
		int page = 0, startTime, endTime, timeCount;
		time_t now = time(0);
		tm *ltm = localtime(&now);
		startTime =ltm->tm_hour * 3600 + ltm->tm_min * 60 + ltm->tm_sec;
		while (true) {
			now = time(0);
			ltm = localtime(&now);
			endTime = ltm->tm_hour * 3600 + ltm->tm_min * 60 + ltm->tm_sec;
			timeCount = endTime - startTime;
			
			if (timeCount == 15)
				break;
			setactivepage(page);
			setvisualpage(1 - page);
			drawFrame();
			drawTaskBarButtons();
			drawMatrix();
			drawVertices();
			drawAllEdges();
			resultBox.draw();
			xButton.draw();
			drawKeyToExitText();
			if (kbhit()) 
				break;
			if (xButton.isClickLMButton())
				break;
			char timeCountdownStr[4];
			itoa(15 - timeCount, timeCountdownStr, 10);
			outtextxy(425 + (834 - textwidth(str)) / 2 + textwidth(str) - textwidth(timeCountdownStr) - textwidth("s]"), 525 + (100 - textheight(timeCountdownStr)) / 2, timeCountdownStr);
			page = 1 - page;
		}
		
	} 
	else {
		int traveler[n + n];
		char resultText[100] = "";
		char weightSumText[30] = "Chieu dai duong di: ", weightSumStr[5]; 
		itoa(dist[end], weightSumStr, 10);
		strcat(weightSumText, weightSumStr);
		stack s;
		//sau khi tim duoc duong di ta se truy vet duong di tu 
		//dinh end toi dinh start bang mang trace
		//va push no vao trong stack
		while (end != start) {
			s.push(end);
			end = trace[end];
		}
		s.push(start);
		int count = 0, v;
		do {
			v = s.pop();
			strcat(resultText, vertices[v].name);
			strcat(resultText, " -> ");
			traveler[count] = v;
			count++;
		} while (!s.isEmpty());
		strnDel(resultText, strlen(resultText) - 3, 3);
		resultBox.name = resultText;
		int page = 0;
		bool isUGr = isUndirectedGraph();
		drawLoading("Dang tai ket qua duong di...");
		//show ra man hinh
		while (true) {
			delay(10);
			setactivepage(page);
			setvisualpage(1 - page);
			drawFrame();
			drawTaskBarButtons();
			drawMatrix();
			drawVertices();
			drawAllEdges();
			resultBox.draw();
			xButton.draw();
			outtextxy(425 + (834 - textwidth(weightSumText)) / 2, 525 + (150 - textheight(weightSumText)) / 2, weightSumText);
			drawEnterToExitText();
			setactivepage(1);
			setvisualpage(1);
			int u = traveler[0], v;
			vertices[u].highLight();
			if (isUGr)
				for (int i = 1; i < count; i++) {
					u = traveler[i - 1];
					v = traveler[i];
					drawArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
					delay(200);
					vertices[v].highLight();
					drawEnterToExitText();
					if (kbhit()) {
						char key = getch();
						if (key == KEY_ENTER)
							return;
					}
					if (xButton.isClickLMButton())
						return;
				}
			else 
				for (int i = 1; i < count; i++) {
					u = traveler[i - 1];
					v = traveler[i];
					if (!G[v][u])
						drawArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
					else {
						if (u > v)
							drawCurvedArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
						else 
							drawArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
					}
					delay(200);
					vertices[v].highLight();
					if (kbhit()) {
						char key = getch();
						if (key == KEY_ENTER)
							return;
					}
					if (xButton.isClickLMButton())
						return;
				}
			if (kbhit()) {
				char key = getch();
				if (key == KEY_ENTER)
					return;
			}
			if (xButton.isClickLMButton())
				return;
 			page = 1 - page;
		}
	}
}
 
int minDistance(int *dist, bool *sptSet) {
    // Khoi tao gia tri toi thieu
    int min = INT_MAX, min_index;
    for (int v = 0; v < n; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

void dijkstra(int start, int end) {
	int dist[n];//Mang dau ra. dist [i] se giu khoang cach ngan nhat 
				// tu start den end
	bool sptSet[n];//sptset [i] se dung neu dinh duoc dua vao ngan nhat
					// cay duong dan hoac khoang cach ngan nhat tu start den end duoc hoan thanh
	int trace[n];
	// Khoi tao tat ca cac khoang cach la vo han va stpset [] la false
	for (int i = 0; i < n; i++)
		dist[i] = INT_MAX, sptSet[i] = false;
	dist[start] = 0;//khoang cach cua dinh nguon tu chinh no luon la 0
	// Tim duong di ngan nhat cho tat ca cac dinh
	for (int count = 0; count < n - 1; count++) {
		//Chon dinh khoang cach toi thieu tu tap hop cac dinh chua
		//duoc xu ly. u luon bang start trong lan lap dau tien.
		int u = minDistance(dist, sptSet);
		sptSet[u] = true;//danh dau dinh duoc lua chon la da duoc xu ly
		for (int v = 0; v < n; v++)
			// Cap nhat dist [v] Chi khi khong co trong sptset, co mot canh tu 
			// u den v va tong trong luong cua duong dan tu src den v qua u la 
			// nho hon gia tri hien tai cua dist [v]
			if (!sptSet[v] && G[u][v]
                && abs(dist[u]) + abs(G[u][v]) < dist[v]) {
                	dist[v] = abs(dist[u]) + abs(G[u][v]);
                	trace[v] = u;
				}
	}
	showResultPathXY(trace, dist, start, end);
	return;
}

void pathXY() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;		
	}
	if (n < 2) return;
	Button instruction;
	instruction.init(425, 525, 40, 834, YELLOW, BLACK, 1, "Chon dinh ket thuc");
	int start, end;
	start = chooseVertex("Chon dinh bat dau");
	end = start;
	int page = 0;
	while (end == start) {
		setactivepage(page);
		setvisualpage(1 - page);
		pointBarFrame.draw();
		drawVertices();
		instruction.draw();
		int x = mousex();
		int y = mousey();
		if (x >= W_LEFT && x <= W_RIGHT && y >= W_TOP && y <= W_BOTTOM) {
			setlinestyle(DOTTED_LINE, 1, 2);
			line(vertices[start].coordinates.x, vertices[start].coordinates.y, x, y);
			setlinestyle(SOLID_LINE, 1, 1);
		}
		vertices[start].highLight();
		if (ismouseclick(WM_LBUTTONDOWN)) {
			int xL, yL;
			getmouseclick(WM_LBUTTONDOWN, xL, yL); 
			for (int i = 0; i < n; i++) {
				int xV = vertices[i].coordinates.x;
				int yV = vertices[i].coordinates.y;
				if ((xV - xL) * (xV - xL) + (yV - yL) * (yV - yL) <= RADIUS * RADIUS) {
					end = i;
				}
			}
			clearmouseclick(WM_LBUTTONDOWN);
		}
		if (ismouseclick(WM_RBUTTONDOWN))
			clearmouseclick(WM_RBUTTONDOWN);
		if (ismouseclick(WM_LBUTTONDBLCLK))
			clearmouseclick(WM_LBUTTONDBLCLK);
		page = 1 - page;
	}
	dijkstra(start, end);
	
}

void clearmouseclick() {
	if (ismouseclick(WM_RBUTTONDOWN)) 
		clearmouseclick(WM_RBUTTONDOWN);
	if (ismouseclick(WM_LBUTTONDOWN))
		clearmouseclick(WM_LBUTTONDOWN);
	if (ismouseclick(WM_LBUTTONDBLCLK))
		clearmouseclick(WM_LBUTTONDBLCLK);
}

template <typename Type>

void setArrayTo(Type *arr, int num, int value) {
	for (int i = 0; i < num; i++)
		arr[i] = value;
}

void DFS() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;	
	}
	int start = chooseVertex("Chon dinh bat dau duyet theo chieu sau (DFS)");
	if (start != -1) {
		setArrayTo(visited, n, 0);
		dfsTraveler(start);
	}
}

void showResultDFS(int *trace, char *resultText, int count) {
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, resultText);
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	int page = 0; 
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(10);
		drawFrame();
		drawVertices();
		drawAllEdges();
		drawTaskBarButtons();
		drawMatrix();
		resultBox.draw();
		xButton.draw();
		outtextxy(430, 525 + (100 - textheight("Ket qua thuat toan DFS:")) / 2, "Ket qua thuat toan DFS:");
		drawEnterToExitText();
		setactivepage(1);
		setvisualpage(1);
		for (int i = 1; i < count; i += 2) {
			int u = trace[i - 1];
			int v = trace[i];
			vertices[u].highLight();
			if (G[u][v] == -1 || !G[v][u]) {
				drawArrow(vertices[u], vertices[v], LIGHTGREEN, 0);
			} 
			else {
				if (u < v) 
					drawArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
				else 
					drawCurvedArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
			}
			delay(200);
			vertices[v].highLight();
			if (xButton.isClickLMButton())
				return;
			if (kbhit()) {
				char key = getch();
				if (key == KEY_ENTER)
					return;
			}
		}
		if (xButton.isClickLMButton())
			break;
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;
		}
		//clearmouseclick();
		page = 1 - page;
	}
}

void dfsTraveler(int u) {
	int start = u;
	stack s;
	int trace[n * n];
	stack edgeStart;
	int count = 1;
	char resultText[150] = "";
	s.push(u);
	setactivepage(1);
	setvisualpage(1);
	while (!s.isEmpty()) {
		drawEnterToExitText();
		u = s.pop();//lay dinh o tren ngan xep va xoa no ra khoi danh sach
		if (!visited[u]) {//neu dinh do chua duoc tham
			vertices[u].highLight();
			strcat(resultText, vertices[u].name);
			strcat(resultText, " -> ");
			visited[u] = 1;//danh dau no da tham
			for (int i = n - 1; i >= 0; i--)
				if (G[u][i] && !visited[i]) {
					s.push(i);//them no vao hang doi
					edgeStart.push(u);
				}//neu dinh chua duyet va ke voi dinh u	
		}
		int k = -1, j = -1;
		if (!edgeStart.isEmpty())
			k = edgeStart.pop();
		if (!s.isEmpty())
			j = s.get();
		while (!G[k][j] && !s.isEmpty() && !edgeStart.isEmpty()) {
			k = edgeStart.pop();
		}	
		if (k != -1 && j != -1 && j != k && !visited[j]) {
			if (G[k][j] == -1 || !G[j][k])
				drawArrow(vertices[k], vertices[j], LIGHTGREEN, 0);
			else {
				if (k < j)
					drawArrow(vertices[k], vertices[j], LIGHTGREEN, G[k][j]);
				else
					drawCurvedArrow(vertices[k], vertices[j], LIGHTGREEN, G[k][j]);
			}
			trace[count - 1] = k;
			trace[count] = j;
			count += 2;
			delay(200);
		}
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				return;
		}
	}
	strnDel(resultText, strlen(resultText) - 3, 3);
	drawLoading("Dang tai ket qua DFS...");
	showResultDFS(trace, resultText, count);
}

void BFS() {
	if (isEmptyVertex()) {
		showEmptyVertex();
		return;
	}
	int start = chooseVertex("Chon dinh bat dau duyet theo chieu rong (BFS)");
	if (start != -1) {
		setArrayTo(visited, n, 0);
		bfsTraveler(start);
	}
}

void showResultBFS(int *trace, char *resultText, int count) {
	Button resultBox, xButton;
	resultBox.init(425, 525, 100, 834, YELLOW, BLACK, 1, resultText);
	xButton.init(1219, 525, 100, 40, WHITE, RED, 1, "x");
	int page = 0; 
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(10);
		drawFrame();
		drawVertices();
		drawAllEdges();
		drawTaskBarButtons();
		drawMatrix();
		resultBox.draw();
		xButton.draw();
		outtextxy(430, 525 + (100 - textheight("Ket qua thuat toan BFS:")) / 2, "Ket qua thuat toan BFS:");
		drawEnterToExitText();
		setactivepage(1);
		setvisualpage(1);
		for (int i = 1; i < count; i += 2) {
			int u = trace[i - 1];
			int v = trace[i];
			vertices[u].highLight();
			if (G[u][v] == -1 || !G[v][u])
				drawArrow(vertices[u], vertices[v], LIGHTGREEN, 0);
			else {
				if (u < v)
					drawArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
				else 
					drawCurvedArrow(vertices[u], vertices[v], LIGHTGREEN, G[u][v]);
			}
			delay(200);
			vertices[v].highLight();
			if (xButton.isClickLMButton())
				return;
			if (kbhit()) {
				char key = getch();
				if (key == KEY_ENTER)
					return;
			}
		}
		if (xButton.isClickLMButton())
			break;
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				break;
		}
		page = 1 - page;
	}
}

void bfsTraveler(int u) {
//	int start = u;
	queue q;
	int trace[n * n];
	queue edgeStart;
	int count = 1;
	char resultText[150] = "";
	q.push(u);
	setactivepage(1);
	setvisualpage(1);
	while (!q.isEmpty()) {
		drawEnterToExitText();
		u = q.pop();//lay dinh o tren hang doi va xoa no ra khoi danh sach
		if (!visited[u]) {//neu dinh do chua duoc tham
			vertices[u].highLight();
			strcat(resultText, vertices[u].name);
			strcat(resultText, " -> ");
			visited[u] = 1;//danh dau no da tham
			for (int i = 0; i < n; i++)
				if (G[u][i] && !visited[i]) {
					q.push(i);//them no vao ngan xep
					edgeStart.push(u);
				}//neu dinh chua duyet va ke voi dinh u	
		}
		int k = -1, j = -1;
		if (!edgeStart.isEmpty())
			k = edgeStart.pop();
		if (!q.isEmpty())
			j = q.get();
		while (!G[k][j] && !q.isEmpty() && !edgeStart.isEmpty()) {
			k = edgeStart.pop();
		}	
		if (k != -1 && j != -1 && j != k && !visited[j]) {
			if (G[k][j] == -1 || !G[j][k])
				drawArrow(vertices[k], vertices[j], LIGHTGREEN, 0);
			else {
				if (k < j) 
					drawArrow(vertices[k], vertices[j], LIGHTGREEN, G[k][j]);
				else 
					drawCurvedArrow(vertices[k], vertices[j], LIGHTGREEN, G[k][j]);
			}
			trace[count - 1] = k;
			trace[count] = j;
			count += 2;
			delay(200);
		}
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER)
				return;
		}
	}
	strnDel(resultText, strlen(resultText) - 3, 3);
	drawLoading("Dang tai ket qua BFS...");
	showResultBFS(trace, resultText, count);
}

int chooseVertex(char *request) {
	int x, y;
	int page = 0;
	Button instruction;
	instruction.init(425, 525, 40, 834, YELLOW, BLACK, 1, request);
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(50);
		drawFrame();
		drawTaskBarButtons();
		drawMatrix();
		drawAllEdges();
		drawVertices();
		instruction.draw();
		if (ismouseclick(WM_LBUTTONDOWN)) {
			getmouseclick(WM_LBUTTONDOWN, x, y);
			for (int i = 0; i < n; i++) {
				int x0 = vertices[i].coordinates.x;
				int y0 = vertices[i].coordinates.y;
				if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= RADIUS * RADIUS)
					return i;
			}
		}
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ESC) {
				return -1;
			}
		}
		page = 1 - page;
	}
}

int enterWeight() {
	int margin = 5;
	int height = 275 - 3 * margin - 40;
	int width = 400 - 2 * margin;
	char weightStr[4] = "";
	char request[20] = "Nhap trong so";
	char note[30] = "Nhap * de tao canh vo huong";
	char note2[20] = "Nhap 0 de xoa canh";
	char c = getcolor();
	int i = 0, x, y;
	int page = 0;
	Button frame, finishButton, cancelButton, enterWeightBar;
	frame.init(15 + margin, 15 + 2 * margin + 40, height, width, YELLOW, DARKGRAY, 1, "");
	finishButton.init(15 + 2 * margin, 275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HOAN THANH");
	cancelButton.init(15 + 3 * margin + (width - 3 * margin) / 2,275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HUY");
	enterWeightBar.init(15 + 2 * margin, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50, 40, (width - 2 * margin), YELLOW, BLACK, 9, "");
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(50);
		drawFrame();
		drawVertices();
		//drawMatrix();
		drawAllEdges();
		drawTaskBarButtons();
		frame.draw();
		finishButton.draw();
		cancelButton.draw();
		enterWeightBar.draw();
		setcolor(BLUE);
		setbkcolor(DARKGRAY);
		outtextxy(frame.coordinates.x + (frame.width - textwidth(note)) / 2, 180, note);
		outtextxy(frame.coordinates.x + (frame.width - textwidth(note2)) / 2, 200, note2);
		setcolor(c);
		setbkcolor(BLACK);
		if (ismouseclick(WM_LBUTTONDOWN)) {
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x < frame.coordinates.x || x > frame.coordinates.x + frame.width
			|| y < frame.coordinates.y || y > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterWeightBar.draw();
				if (strcmp(weightStr, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(weightStr)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(weightStr)) / 2, weightStr);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}
			clearmouseclick(WM_LBUTTONDOWN);
		}
		if (ismouseclick(WM_LBUTTONDBLCLK)) {// neu co double click chuot trai khi dang tao ten thi phai xoa di
											//de tranh truong hop sau khi tao ten thi tao luon dinh moi
			int xL, yL;
			getmouseclick(WM_LBUTTONDBLCLK, xL, yL);
			if (xL < frame.coordinates.x || xL > frame.coordinates.x + frame.width
			|| yL < frame.coordinates.y || yL > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterWeightBar.draw();
				if (strcmp(weightStr, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(weightStr)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(weightStr)) / 2, weightStr);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}		
			clearmouseclick(WM_LBUTTONDBLCLK);
		}
		if (ismouseclick(WM_RBUTTONDOWN)) {//neu co click chuot phai khi dang tao ten thi phai xoa di 
										//de tranh truong hop sau khi tao ten thi mo thanh edit dinh
			int xR, yR;
			getmouseclick(WM_RBUTTONDOWN, xR, yR);
			if (xR < frame.coordinates.x || xR > frame.coordinates.x + frame.width
			|| yR < frame.coordinates.y || yR > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterWeightBar.draw();
				if (strcmp(weightStr, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(weightStr)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(weightStr)) / 2, weightStr);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}			
			clearmouseclick(WM_RBUTTONDOWN);
		}
		if (ismouseclick(WM_RBUTTONDBLCLK))
			clearmouseclick(WM_RBUTTONDBLCLK);
		if (strcmp(weightStr, "") == 0) 
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
		if (finishButton.isHover())
			finishButton.highLight();
		if (cancelButton.isHover())	
			cancelButton.highLight();
		if (strcmp(weightStr, "") != 0
		&& x >= finishButton.coordinates.x && x <= finishButton.coordinates.x + finishButton.width
		&& y >= finishButton.coordinates.y && y <= finishButton.coordinates.y + finishButton.height)
			break;
		if (x >= cancelButton.coordinates.x && x <= cancelButton.coordinates.x + cancelButton.width
		&& y >= cancelButton.coordinates.y && y <= cancelButton.coordinates.y + cancelButton.height) 
			return -2;
		if (ismouseclick(WM_LBUTTONDOWN)) {
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x >= finishButton.coordinates.x && x <= finishButton.coordinates.x + finishButton.width
			&& y >= finishButton.coordinates.y && y <= finishButton.coordinates.y + finishButton.height
			&& strcmp(weightStr, "") != 0)
				break; 
		}
		if (kbhit()) {
			char key = getch();
			if ((key >= '0' && key <= '9') || key == '*')
				if (i < 3) {
					weightStr[i] = key;
					i++;
				}
			if (i > 3)
				i = 3;
			if (key == KEY_BACKSPACE) {
				strnDel(weightStr, i - 1, 1);
				i--;
			}
			if (i < 0)
				i = 0;
			if (strcmp(weightStr, "") != 0 && key == KEY_ENTER)
				break;
			if (strcmp(weightStr, "") == 0 && key == KEY_ENTER) {
				frame.highLight(WHITE, RED);
				enterWeightBar.draw();
				finishButton.draw();
				cancelButton.draw();
				delay(50);
			}
		}
		outtextxy(15 + (400 - width) / 2 + (width - textwidth(weightStr)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(weightStr)) / 2, weightStr);
		if (i < 3 && strcmp(weightStr, "") != 0)
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(weightStr)) / 2 + textwidth(weightStr), 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(weightStr)) / 2, "_");
		
		page = 1 - page;
	}
	for (int j = 0; j < i; j++) {
		if (weightStr[j] == '*')
			return -1;
	}
	return atoi(weightStr);//itoa
}

void addEdge(int startPos) {
	int x, y;
	Button instruction;
	char c = getcolor();
	int page = 0;
	int i = startPos;
	int x0 = vertices[i].coordinates.x;
	int y0 = vertices[i].coordinates.y;
	instruction.init(425, 525, 40, 834, YELLOW, BLACK, 1, "Di chuyen chuot toi vi tri dinh con lai va click chuot trai. Click chuot phai vao vi tri bat ky de huy tao canh");
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(10);
		drawFrame();
		drawTaskBarButtons();
		drawVertices();
		drawMatrix();
		drawAllEdges();
		instruction.draw();
		x = mousex(), y = mousey();
		float corner = atan(float(abs(y - y0)) / abs(x - x0));
		float Rsin = RADIUS * sin(corner);
		float Rcos = RADIUS * cos(corner);
		int x11 = x0 + Rcos;
		int y11 = y0 + Rsin;
		int x22 = x - Rcos;
		int y22 = y - Rsin;
		if (x0 > x) {
			x11 = x0 - Rcos;
			x22 = x + Rcos;
		}
		if (y0 > y) {
			y11 = y0 - Rsin;
			y22 = y + Rsin;
		}	
		if (x22 >= 425 && x22 <= 1259 && y22 >= 20 && y22 <= 520) {
			setcolor(WHITE);
			setlinestyle(SOLID_LINE, 1, 2);
			line(x11, y11, x22, y22);
			drawTriangle(2 * x22 - (x + x22) / 2, 2 * y22 - (y22 + y) / 2, x22, y22, WHITE);
			setcolor(c);
			setlinestyle(SOLID_LINE, 0, 1);
		}
		if (ismouseclick(WM_LBUTTONDBLCLK))
			clearmouseclick(WM_LBUTTONDBLCLK);
		if (ismouseclick(WM_RBUTTONDOWN)) {
			clearmouseclick(WM_RBUTTONDOWN);
			break;
		}
		if (ismouseclick(WM_LBUTTONDOWN)) {//click chuot trai vao cac dinh con lai
			bool checked = false;								//-> nhap trong so -> tao canh
			int xL, yL;
			getmouseclick(WM_LBUTTONDOWN, xL, yL);
			for (int j = 0; j < n; j++) {
				int xV, yV;
				xV = vertices[j].coordinates.x;
				yV = vertices[j].coordinates.y;
				if ((x0 - xL) * (x0 - xL) + (y0 - yL) * (y0 - yL) > RADIUS * RADIUS &&
				(xV - xL) * (xV - xL) + (yV - yL) * (yV - yL) <= RADIUS * RADIUS) {
					int tmp = G[i][j];
					G[i][j] = enterWeight();
					if (G[i][j] == -1) 
						G[j][i] = -1;
					checked = true;
					if (G[i][j] == -2)
						G[i][j] = tmp;
				}
			}
			clearmouseclick(WM_LBUTTONDOWN);
			if (checked)
				break;
		}
		page = 1 - page;
	}
	saveFileStartUp();
}

void drawCurvedArrow2(Vertex u, Vertex v, int color, int w) {
	int x1 = u.coordinates.x, 
		y1 = u.coordinates.y,
		x2 = v.coordinates.x,
		y2 = v.coordinates.y;
	char c = getcolor();
	float xO = (x1 + x2) / 2 + (y1 - y2) / M_SQRT2; // or +
	float yO = (y1 + y2) / 2 + (x2 - x1) / M_SQRT2; // or +
	float r = sqrt(pow(xO - x1, 2) + pow(yO - y1, 2));
	float d = sqrt(pow(x1 - xO, 2) + pow(y1 - yO, 2));
	float a = (pow(r, 2) - pow(RADIUS, 2) + pow(d ,2)) / (2 * d);
	float h = sqrt(r * r - a * a);								//					 
	float tmpx1 = xO + a * (x1 - xO) / d;						//					   xT		  
	float tmpx2 = xO + a * (x2 - xO) / d;						//        *  * x3             x4*  *
	float tmpy1 = yO + a * (y1 - yO) / d;						//     *        *            *        *
	float tmpy2 = yO + a * (y2 - yO) / d;						//    *    x1    *          *     x2   *
	int x3 = tmpx1 - h* (y1 - yO) / d;   // -h					//    *          *          *          *
	int x4 = tmpx2 + h* (y2 - yO) / d;   // +h					//     *        *            *        *
	int y3 = tmpy1 + h * (x1 - xO) / d;  // 					//        *  *                  *  *
	int y4 = tmpy2 - h * (x2 - xO) / d;  // 					//				 	   xO          
	float stangle, endangle;									//					 
	float angle1 = float(x1 - xO) / r;
	float angle2 = 1 - float(pow(RADIUS, 2)) / (2 * pow(r, 2));
	if (angle1 < -1 || angle1 > 1) angle1 = int(angle1);
	if (angle2 < -1 || angle2 > 1) angle2 = int(angle2);
	angle1 = acos(angle1) * 180 / M_PI;
	angle2 = acos(angle2) * 180 / M_PI;
	if (y1 >= yO) angle1 = 360 - angle1;
	stangle = angle1 + angle2;
	angle1 = acos(1 - pow(sqrt(pow(x3 - x4, 2) + pow(y3 - y4, 2)), 2) / (2 * pow(r, 2)));
	angle1 = angle1 * 180 / M_PI;
	stangle = stangle - angle1 - 2 * angle2;
	endangle = stangle + angle1;
	float theta = atan2((y1 + y2) / 2 - yO, (x1 + x2) / 2 - xO);
	float xT = xO + r * cos(theta);
	float yT = yO + r * sin(theta);
	setcolor(color);
	setlinestyle(SOLID_LINE, 1, 2);
	arc(xO, yO, stangle, endangle, r);
	drawTriangle(2 * x4 - (x2 + x4) / 2, 2 * y4 - (y2 + y4) / 2, x4, y4, color); // FINALY
	printWeight(xT, yT, w);
	setcolor(c);
}

void drawCurvedArrow(Vertex u, Vertex v, int color, int w) {
	int x1 = u.coordinates.x,
		y1 = u.coordinates.y,
		x2 = v.coordinates.x,
		y2 = v.coordinates.y;
	//M_SQRT2		1.41421356237309504880
	char c = getcolor();
		float xO = (x1 + x2) / 2 + (y1 - y2) / -M_SQRT2; // or +
	float yO = (y1 + y2) / 2 + (x2 - x1) / -M_SQRT2; // or +
	float r = sqrt(pow(xO - x1, 2) + pow(yO - y1, 2));
	float d = sqrt(pow(x1 - xO, 2) + pow(y1 - yO, 2));
	float a = (pow(r, 2) - pow(RADIUS, 2) + pow(d ,2)) / (2 * d);
	float h = sqrt(r * r - a * a);								//					 
	float tmpx1 = xO + a * (x1 - xO) / d;						//					   xT		  
	float tmpx2 = xO + a * (x2 - xO) / d;						//        *  * x3             x4*  *
	float tmpy1 = yO + a * (y1 - yO) / d;						//     *        *            *        *
	float tmpy2 = yO + a * (y2 - yO) / d;						//    *    x1    *          *     x2   *
	int x3 = tmpx1 + h* (y1 - yO) / d;   // -h					//    *          *          *          *
	int x4 = tmpx2 - h* (y2 - yO) / d;   // +h					//     *        *            *        *
	int y3 = tmpy1 - h * (x1 - xO) / d;  // 					//        *  *                  *  *
	int y4 = tmpy2 + h * (x2 - xO) / d;  // 					//				 	   xO          
	float stangle, endangle;									//					 
	float angle1 = float(x1 - xO) / r;
	float angle2 = 1 - float(pow(RADIUS, 2)) / (2 * pow(r, 2));
	if (angle1 < -1 || angle1 > 1) angle1 = int(angle1);
	if (angle2 < -1 || angle2 > 1) angle2 = int(angle2);
	angle1 = acos(angle1) * 180 / M_PI;
	angle2 = acos(angle2) * 180 / M_PI;
	if (y1 >= yO) angle1 = 360 - angle1;
	stangle = angle1 + angle2;
	angle1 = acos(1 - pow(sqrt(pow(x3 - x4, 2) + pow(y3 - y4, 2)), 2) / (2 * pow(r, 2)));
	angle1 = angle1 * 180 / M_PI;
	endangle = stangle + angle1; 
	float theta = atan2((y1 + y2) / 2 - yO, (x1 + x2) / 2 - xO);
	float xT = xO + r * cos(theta);
	float yT = yO + r * sin(theta);
	if (xT < W_LEFT || xT > W_RIGHT || yT < W_TOP || yT > W_BOTTOM)
		return drawCurvedArrow2(u, v, color, w);
	setcolor(color);
	setlinestyle(SOLID_LINE, 1, 2);
	arc(xO, yO, stangle, endangle, r);
	drawTriangle(2 * x4 - (x2 + x4) / 2, 2 * y4 - (y2 + y4) / 2, x4, y4, color); // FINALY
	printWeight(xT, yT, w);
	setcolor(c);
	
}

void drawAllEdges(int color) {
	for (int i = 0; i < n; i++) 
		for(int j = 0; j < n; j++)
			if (G[i][j]) {
				if (!G[j][i]) 
					drawArrow(vertices[i], vertices[j], color, G[i][j]);
				else if (G[i][j] == -1)
					drawLine(vertices[i], vertices[j], color, 0);
				else  {
					if (i < j) {
						drawArrow(vertices[i], vertices[j], color, G[i][j]);
					}
					else
						drawCurvedArrow(vertices[i], vertices[j], color, G[i][j]);//duong cong
				}			
			} 
}

void drawAllEdges() {
	

	for (int i = 0; i < n; i++) 
		for(int j = 0; j < n; j++)
			if (G[i][j]) {
				if (!G[j][i]) 
					drawArrow(vertices[i], vertices[j], RED, G[i][j]);
				else if (G[i][j] == -1)
					drawLine(vertices[i], vertices[j], RED, 0);
				else  {
					if (i < j) {
						drawArrow(vertices[i], vertices[j], RED, G[i][j]);
					}
					else
						drawCurvedArrow(vertices[i], vertices[j], RED, G[i][j]);//duong cong
				}			
			} 
}

void printWeight(int x, int y, int w) {
	char c = getcolor();
	setcolor(BLUE);
	setbkcolor(LIGHTGRAY);
	char str[10];
	itoa(abs(w), str, 10);
//	settextstyle(1, 0, 2);
//	setusercharsize(100, 100, 100, 100);
	outtextxy(x, y, str);
	setcolor(c);
	setbkcolor(BLACK);
	return;
}

void drawTriangle(int x1, int y1, int x2, int y2, int color) {
	char c = getcolor();
	setlinestyle(SOLID_LINE, 0, 3);
	setfillstyle(SOLID_LINE, color);
	int a[8];
	a[0] = x1 + y2 - y1;
	a[1] = y1 + x1 - x2;
	a[2] = x1 - y2 + y1;
	a[3] = y1 - x1 + x2;
	a[4] = x2;
	a[5] = y2;
	a[6] = a[0]; 
	a[7] = a[1];
	setfillstyle(1, color);
	fillpoly(4, a);
	setlinestyle(SOLID_LINE, 0, 1);
	setcolor(c);
	setfillstyle(1, c);
}

void drawLine(Vertex u, Vertex v, int color, int w) {
	int x1, x2, y1, y2;
	char c = getcolor();
	x1 = u.coordinates.x;
	y1 = u.coordinates.y;
	x2 = v.coordinates.x;
	y2 = v.coordinates.y;
	float corner = atan(float(abs(y2 - y1)) / abs(x1 - x2));
	float Rsin = RADIUS * sin(corner);
	float Rcos = RADIUS * cos(corner);
	int x11 = x1 + Rcos;
	int y11 = y1 + Rsin;
	int x22 = x2 - Rcos;
	int y22 = y2 - Rsin;
	if (x1 > x2) {
		x11 = x1 - Rcos;
		x22 = x2 + Rcos;
	}
	if (y1 > y2) {
		y11 = y1 - Rsin;
		y22 = y2 + Rsin;
	}
	setlinestyle(SOLID_LINE, 1, 2);
	setcolor(color);
	line(x11, y11, x22, y22);
	if (w != 0)
		printWeight((x1 + x2) / 2, (y1 + y2) / 2, w);
	setcolor(c);
	setlinestyle(SOLID_LINE, 1, 1);
}

void drawArrow(Vertex u, Vertex v, int color, int w) {
	
	int x1, x2, y1, y2;
	char c = getcolor();
	x1 = u.coordinates.x;
	y1 = u.coordinates.y;
	x2 = v.coordinates.x;
	y2 = v.coordinates.y;
	float corner = atan(float(abs(y2 - y1)) / abs(x1 - x2));
	float Rsin = RADIUS * sin(corner);
	float Rcos = RADIUS * cos(corner);
	int x11 = x1 + Rcos;
	int y11 = y1 + Rsin;
	int x22 = x2 - Rcos;
	int y22 = y2 - Rsin;
	if (x1 > x2) {
		x11 = x1 - Rcos;
		x22 = x2 + Rcos;
	}
	if (y1 > y2) {
		y11 = y1 - Rsin;
		y22 = y2 + Rsin;
	}
	setlinestyle(SOLID_LINE, 1, 2);
	setcolor(color);
	line(x11, y11, x22, y22);
	drawTriangle(2 * x22 - (x2 + x22) / 2, 2 * y22 - (y22 + y2) / 2, x22, y22, color);
	if (w != 0)
		printWeight((x1 + x2) / 2, (y1 + y2) / 2, w);
	setcolor(c);
}

void drawMatrix() {
	if (isEmptyVertex()) return drawEmptySymbol();
	Point center;
	int squareEdge = n < 12 ? 30 : n < 15 ? 25 : n < 18 ? 20 : 18;
	Button square, vertexButton[n], matrxHoverFrame;
	square.init(0, 0, squareEdge, squareEdge, BLACK, BLACK, 1, "");
	matrixButton.draw();
	//bien center de ghi toa do can giua cua khung ma tran
	center.x = 15 + 400 / 2;
	center.y = 304 + 400 / 2;
	matrxHoverFrame.init(center.x - squareEdge * n / 2 - squareEdge / 2, center.y - squareEdge * n / 2, (n + 1) * squareEdge,  (n + 1) * squareEdge, BLACK, BLACK, 1, "");
	int x0 = center.x - n * squareEdge / 2 + squareEdge / 2;
	int y0 = center.y - n * squareEdge / 2 + squareEdge;
	int xDL = -1, yDL = -1;
	if (ismouseclick(WM_RBUTTONDBLCLK)) {
		getmouseclick(WM_RBUTTONDBLCLK, xDL, yDL);
		clearmouseclick(WM_RBUTTONDBLCLK);
	}
	for (int i = 0; i < n; i++) {//ve khung ma tran trong so va in ra gia tri cua ma tran trong so
		int x = x0;//su dung bien x de luu gia tri ban dau cua x0
		vertexButton[i].init(0, 0, squareEdge, squareEdge, YELLOW, BLUE, 1, vertices[i].name);
		//khoi tao gia tri cua cac nut dinh trong ma tran trong so
		for (int j = 0; j < n; j++) {
			char numText[3];
			if (G[i][j] == -1 && G[j][i] != -1)
				G[i][j] = 0;
			if (G[i][j] != -1)
				itoa(G[i][j], numText, 10);//chuyen trong so canh sang dang text
			else 
				strcpy(numText, "1");
			square.init(x0, y0, squareEdge, squareEdge, BLACK, CYAN, 1, numText);
			//square.highLight();
			square.draw();
			if (square.isHover()) {
				square.highLight(YELLOW, LIGHTBLUE);
				vertices[i].highLight(YELLOW, LIGHTBLUE);
				vertices[j].highLight(YELLOW, LIGHTBLUE);

			} 
			if (i != j && 
			xDL >= square.coordinates.x && xDL <= square.coordinates.x + square.width
			&& yDL >= square.coordinates.y && yDL <= square.coordinates.y + square.height) {
				int tmpGij = G[i][j];
				G[i][j] = enterWeight();
				if (!G[i][j])
					G[i][j] = tmpGij;
				else if (G[i][j] == -1)
					G[j][i] = -1;
			}
			
 			x0 += squareEdge;
		}
		x0 = x;
		y0 += squareEdge;
	}
	x0 = center.x - squareEdge * n / 2 - squareEdge / 2;
	y0 = center.y - squareEdge * n / 2;
	int y = y0;
//	matrxHoverFrame.draw();
	if (!isEmptyVertex()) {
		square.coordinates.x = x0;
		square.coordinates.y = y0;
		square.name = "";
		square.draw();
	}
	for (int i = 0; i < n; i++) {
		y0 += squareEdge;
		vertexButton[i].coordinates.x = x0;
		vertexButton[i].coordinates.y = y0;
		vertexButton[i].draw();
		int y = mousey();
		if (y >= vertexButton[i].coordinates.y && y <= vertexButton[i].coordinates.y + vertexButton[i].height
		&& matrxHoverFrame.isHover())
			vertexButton[i].highLight(YELLOW, LIGHTBLUE);
	}
	y0 = y;
	for (int i = 0; i < n; i++) {
		x0 += squareEdge;
		vertexButton[i].coordinates.x = x0;
		vertexButton[i].coordinates.y = y0;
		vertexButton[i].draw();
		int x = mousex();
		if (x >= vertexButton[i].coordinates.x && x <= vertexButton[i].coordinates.x + vertexButton[i].width
		&& matrxHoverFrame.isHover())
			vertexButton[i].highLight(YELLOW, LIGHTBLUE);
	}
}

void moveVertex() {
	int x, y;
	if (ismouseclick(WM_LBUTTONDOWN)) {
		getmouseclick(WM_LBUTTONDOWN, x, y);
		for (int i = 0; i < n; i++) {
			int x0 = vertices[i].coordinates.x;
			int y0 = vertices[i].coordinates.y;
			if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= RADIUS * RADIUS) {
				int page = 0;
				while (true) {
					setactivepage(page);
					setvisualpage(1 - page);
					delay(1);
					if (ismouseclick(WM_MOUSEMOVE)) {
						getmouseclick(WM_MOUSEMOVE, x, y);
						bool check = 0;//check trung dinh
						for (int j = 0; j < n; j++)
							if (j != i) {
								int xB = vertices[j].coordinates.x;
								int yB = vertices[j].coordinates.y;
								if ((x - xB) * (x - xB) + (y - yB) * (y - yB) <= 4 * RADIUS * RADIUS)
									check = 1;
							}
						if (x >= W_LEFT + RADIUS && x <= W_RIGHT - RADIUS
						&& y >= W_TOP + RADIUS && y <= W_BOTTOM - RADIUS && !check) {
							//neu nam trong pham vi bang dieu khien thi moi duoc di chuyen 
							vertices[i].coordinates.x = x;
							vertices[i].coordinates.y = y;
						}
						if (x < W_LEFT + RADIUS && y >= W_TOP + RADIUS && y <= W_BOTTOM - RADIUS) {
							vertices[i].coordinates.x = W_LEFT + RADIUS;
							vertices[i].coordinates.y = y;
						}
						if (x > W_RIGHT - RADIUS && y >= W_TOP + RADIUS && y <= W_BOTTOM - RADIUS) {
							vertices[i].coordinates.x = W_RIGHT - RADIUS;
							vertices[i].coordinates.y = y;
						}
						if (x >= W_LEFT + RADIUS && x <= W_RIGHT - RADIUS
						&& y < W_TOP + RADIUS) {
							vertices[i].coordinates.x = x;
							vertices[i].coordinates.y = W_TOP + RADIUS;
						}
						if (x >= W_LEFT + RADIUS && x <= W_RIGHT - RADIUS
						&& y > W_BOTTOM - RADIUS) {
							vertices[i].coordinates.x = x;
							vertices[i].coordinates.y = W_BOTTOM - RADIUS;
						}
					}					
					drawFrame();
					drawTaskBarButtons();
					drawVertices();
					drawMatrix();
					drawAllEdges();
					if (ismouseclick(WM_LBUTTONUP)) {
						break;
					}
					page = 1 - page;	
				}
			}
		}
	}
	clearmouseclick(WM_LBUTTONDOWN);
	clearmouseclick(WM_LBUTTONUP);
	saveFileStartUp();
}

void Vertex::changeName() {
	Button frame, finishButton, cancelButton, enterNameBar;
	int margin = 5;
	int height = 275 - 3 * margin - 40;
	int width = 400 - 2 * margin;
	int x, y;
	char name[3] = ""; 
	char request[20] = "Nhap ten dinh";
	frame.init(15 + margin, 15 + 2 * margin + 40, height, width, YELLOW, DARKGRAY, 1, "");
	finishButton.init(15 + 2 * margin, 275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HOAN THANH");
	cancelButton.init(15 + 3 * margin + (width - 3 * margin) / 2,275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HUY");
	enterNameBar.init(15 + 2 * margin, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50, 40, (width - 2 * margin), YELLOW, BLACK, 9, "");
	int page = 0;
	int i = 0; 
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(50);
		drawFrame();
		drawTaskBarButtons();
		drawVertices();
		drawMatrix();
		drawAllEdges();
		frame.draw();
		finishButton.draw();
		cancelButton.draw();
		enterNameBar.draw();
		this->highLight();
		if (ismouseclick(WM_LBUTTONDOWN)) {
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x < frame.coordinates.x || x > frame.coordinates.x + frame.width
			|| y < frame.coordinates.y || y > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}
			clearmouseclick(WM_LBUTTONDOWN);
		}
		if (ismouseclick(WM_LBUTTONDBLCLK)) {// neu co double click chuot trai khi dang tao ten thi phai xoa di
											//de tranh truong hop sau khi tao ten thi tao luon dinh moi
			int xL, yL;
			getmouseclick(WM_LBUTTONDBLCLK, xL, yL);
			if (xL < frame.coordinates.x || xL > frame.coordinates.x + frame.width
			|| yL < frame.coordinates.y || yL > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}		
			clearmouseclick(WM_LBUTTONDBLCLK);
		}
		if (ismouseclick(WM_RBUTTONDOWN)) {//neu co click chuot phai khi dang tao ten thi phai xoa di 
										//de tranh truong hop sau khi tao ten thi mo thanh edit dinh
			int xR, yR;
			getmouseclick(WM_RBUTTONDOWN, xR, yR);
			if (xR < frame.coordinates.x || xR > frame.coordinates.x + frame.width
			|| yR < frame.coordinates.y || yR > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}			
			clearmouseclick(WM_RBUTTONDOWN);
		}
		if (ismouseclick(WM_RBUTTONDBLCLK))
			clearmouseclick(WM_RBUTTONDBLCLK);		
		drawAddVertex(this->coordinates.x, this->coordinates.y);
		if (strcmp(name, "") == 0) 
			outtextxy(enterNameBar.coordinates.x + (enterNameBar.width - textwidth(request)) / 2, enterNameBar.coordinates.y + (enterNameBar.height - textheight(request)) / 2, request);
		if (kbhit()) {
			char key = getch();
			if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
				//Neu phim ta nhap la chu cai thi moi them vao chuoi
				if (i < 2) {
					//Neu chieu dai cua chuoi da day thi khong cho them vao chuoi
					name[i] = key;
					i++;//vi tri se duoc nhay len mot don vi sau khi them mot ky tu vao chuoi
				}
			}
			if (i > 2)
				i = 2;
			if (key == KEY_BACKSPACE) {
				strnDel(name, i - 1, 1);//xoa ki tu co vi tri sau cung cua chuoi
				i--;//vi tri se duoc giam xuong 1 don vi
			}
			if (i < 0)
				i = 0;
			if (strcmp(name, "") != 0 && key == KEY_ENTER && (!isNamesake(name) || strcmp(name, this->name) == 0)) {
				break;
			}
			if ((strcmp(name, "") == 0 && key == KEY_ENTER)) {
				frame.highLight(WHITE, RED);
				enterNameBar.draw();
				finishButton.draw();
				cancelButton.draw();
				delay(50);
			}
			if (isNamesake(name) && strcmp(name, this->name) != 0) {
				for (int i = 0; i < n; i++)
					if (strcmp(name, vertices[i].name) == 0 && strcmp(name, this->name) != 0) {
						vertices[i].highLight(YELLOW, LIGHTRED);
						delay(100);
					}
			}
		}
		upper(name);
		outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);
		outtextxy(this->coordinates.x - textwidth(name) / 2, this->coordinates.y - textheight(name) / 2, name);
		if (finishButton.isHover())
			finishButton.highLight();
		if (cancelButton.isHover())
			cancelButton.highLight();
		if (ismouseclick(WM_LBUTTONDBLCLK))// neu co double click chuot trai khi dang tao ten thi phai xoa di
											//de tranh truong hop sau khi tao ten thi tao luon dinh moi
			clearmouseclick(WM_LBUTTONDBLCLK);
		if (ismouseclick(WM_RBUTTONDOWN))//neu co click chuot phai khi dang tao ten thi phai xoa di 
										//de tranh truong hop sau khi tao ten thi mo thanh edit dinh
			clearmouseclick(WM_RBUTTONDOWN);
		if (strcmp(name, "") != 0 && (!isNamesake(name) || strcmp(name, this->name) == 0)
		&& x >= finishButton.coordinates.x && x <= finishButton.coordinates.x + finishButton.width
		&& y >= finishButton.coordinates.y && y <= finishButton.coordinates.y + finishButton.height)
			break;
		if (x >= cancelButton.coordinates.x && x <= cancelButton.coordinates.x + cancelButton.width
		&& y >= cancelButton.coordinates.y && y <= cancelButton.coordinates.y + cancelButton.height) 
			return;
		if (strcmp(name, "") != 0 && i < 2)
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2 + textwidth(name), 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, "_");
		page = 1 - page;
	}
	if (strcmp(name, "") == 0)
		return;
//	this->name = name;
	strcpy(this->name, name);
}

void deleteVertex(int pos) {
	//0 1 2 3 4 5 6 7 8
	//0 1 3 4 5 6 7 8
	for (int i = pos; i < n - 1; i++)
		vertices[i] = vertices[i + 1];//Cap nhat lai danh sach dinh
	for (int i = pos; i < n - 1; i++) 
		for (int j = 0; j < n; j++) 
			G[i][j] = G[i + 1][j];//Xoa hang trong ma tran ke
	for (int i = 0; i < n; i++)
		for (int j = pos; j < n - 1; j++)
			G[i][j] = G[i][j + 1];//Xoa cot trong ma tran ke
	vertices[n - 1].defaultVtex();
	n--;
	saveFileStartUp();
}

void drawEditTools(int x, int y) {
	editFrame.coordinates.x = x, editFrame.coordinates.y = y;
	deleteButton.coordinates.x = x, deleteButton.coordinates.y = y;
	editNameButton.coordinates.x = x, editNameButton.coordinates.y = y + deleteButton.height;
	addEdgeButton.coordinates.x = x, addEdgeButton.coordinates.y = editNameButton.coordinates.y + editNameButton.height;
	
	editFrame.draw();
	deleteButton.draw();
	editNameButton.draw();
	addEdgeButton.draw();
}

void initEditTools() {
	//do hop thoai nay co the dung o bat ky vi tri nao trong chuong trinh nen x y mac dinh cua no la 0
	editFrame.init(0, 0, 120, 100, WHITE, DARKGRAY, 1, "");
	deleteButton.init(0, 0, 40, 100, YELLOW, BLACK, 1, "Xoa dinh");
	editNameButton.init(0, 0, 40, 100, YELLOW, BLACK, 1, "Sua ten");
	addEdgeButton.init(0, 0, 40, 100, YELLOW, BLACK, 1, "Tao canh");
	
}

void editVertex() {
	int x, y;
	if (ismouseclick(WM_RBUTTONDOWN)) {
		getmouseclick(WM_RBUTTONDOWN, x, y);
		for (int i = 0; i < n; i++) {
			int x0 = vertices[i].coordinates.x,
				y0 = vertices[i].coordinates.y;
			if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= RADIUS * RADIUS) {
				int page = 0;
				while (true) {
					setactivepage(page);
					setvisualpage(1 - page);
					delay(10);
					setfillstyle(10, GREEN);
					bar(1, 1, 1279, 719);
					drawFrame();
					drawTaskBarButtons();
					drawVertices();
					drawMatrix();
					drawAllEdges();
					if (x0 + RADIUS > 1259 - editFrame.width && y0 < 520 - editFrame.height)
						drawEditTools(x0 - RADIUS - editFrame.width, y0);//goc tren cung ben phai
					else if (x0 + RADIUS > 1259 - editFrame.width && y0 > 520 - editFrame.height)
						drawEditTools(x0 - RADIUS - editFrame.width, y0 - editFrame.height);//goc duoi cung ben phai
					else if (y0 > 520 - editFrame.height)
						drawEditTools(x0 + RADIUS, y0 - editFrame.height);//goc duoi cung ben trai
					else 
						drawEditTools(x0 + RADIUS, y0);//cac cho con lai
					if (ismouseclick(WM_RBUTTONDOWN)) {
						int xR, yR;
						getmouseclick(WM_RBUTTONDOWN, xR, yR);
						if ((xR - x0) * (xR - x0) + (yR - y0) * (yR - y0) > RADIUS * RADIUS)
							break;
					} 
					if (deleteButton.isHover())
						deleteButton.highLight();
					if (editNameButton.isHover())
						editNameButton.highLight();
					if (addEdgeButton.isHover())
						addEdgeButton.highLight();
					if (ismouseclick(WM_LBUTTONDOWN)) {
						int xL, yL;
						getmouseclick(WM_LBUTTONDOWN, xL, yL);
						if (xL >= deleteButton.coordinates.x && xL <= deleteButton.coordinates.x + deleteButton.width
						&& yL >= deleteButton.coordinates.y && yL <= deleteButton.coordinates.y + deleteButton.height) {
							bool confirm = drawYesNoBar("Ban co chac muon xoa dinh?");
							if (confirm) {
								deleteVertex(i);
							}
							break;
						}
						else if (xL >= editNameButton.coordinates.x && xL <= editNameButton.coordinates.x + editNameButton.width
						&& yL >= editNameButton.coordinates.y && yL <= editNameButton.coordinates.y + editNameButton.height) {
							vertices[i].changeName();	
							break;	
						}
						else if (xL >= addEdgeButton.coordinates.x && xL <= addEdgeButton.coordinates.x + addEdgeButton.width
						&& yL >= addEdgeButton.coordinates.y && yL <= addEdgeButton.coordinates.y + addEdgeButton.height) {
							addEdge(i);
							break;
						}	
						else 
							break;
					}
					page = 1 - page;
				}
			}
		}
	}
}

void drawAddVertex(int x, int y) {
	char c = getcolor();
	setcolor(YELLOW);
	circle(x, y, RADIUS);
	setcolor(c);
}

bool isNamesake(char *s) {
	for (int i = 0; i < n; i++) {
		if(stricmp(s, vertices[i].name) == 0)
			return 1;
	}
	return 0;
}

void upper(char *s) {
	for (int i = 0; i < strlen(s); i++) {
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] -= 32;
	}
}

void strnDel(char s[], int pos, int count) {
	unsigned len = strlen(s);
	if (pos >= len) 
		return;
	if (pos + count > len) 
		count = len - pos;
	for (unsigned i = 0; i <= len - pos - count; i++)
		s[pos + i] = s[pos + count + i];
}

bool Button::isClickLMButton() {
	int x, y;
	if (ismouseclick(WM_LBUTTONDOWN)) {
		getmouseclick(WM_LBUTTONDOWN, x, y);
		clearmouseclick(WM_LBUTTONDOWN);
	}
	if (x >= this->coordinates.x && x <= this->coordinates.x + this->width
	&& y >= this->coordinates.y && y <= this->coordinates.y + this->height)
		return 1;
	return 0;
}

void Button::draw() {
	if (textwidth(this->name) + 10 > this->width)
		this->width = textwidth(this->name) + 10;
	//get mau cua khung hinh chung
	int c = getcolor();
	//tao nut
	//tao mau cho nut
	setfillstyle(this->pattern, this->bColor);
	//bar3d(this->coordinates.x, this->coordinates.y, this->coordinates.x + this->width, this->coordinates.y + this->height, 2, 10);
	bar(this->coordinates.x, this->coordinates.y, this->coordinates.x + this->width, this->coordinates.y + this->height);
	rectangle(this->coordinates.x, this->coordinates.y, this->coordinates.x + this->width, this->coordinates.y + this->height);
	//tao mau chu
	setcolor(this->tColor);
	setbkcolor(this->bColor);
	//in noi dung cua nut ra nut
	//settextstyle(3, 0, 1);
	outtextxy(this->coordinates.x + (this->width - textwidth(this->name)) / 2, this->coordinates.y + (this->height - textheight(this->name)) / 2, this->name);
	//khoi phuc lai mau ban dau khi thoat tao nut
	setcolor(c);
	setbkcolor(BLACK);
	setfillstyle(0, c);
}

void Button::init(short x, short y, short height, short width, short tColor, short bColor, short pattern, char* content) {
	// ham nay de khoi tao nhung thong so cua mot nut
	this->coordinates.x = x;
	this->coordinates.y = y;
	this->height = height;
	this->width = width;
	this->tColor = tColor;
	this->bColor = bColor;
	this->name = content;
	this->pattern = pattern;
}

void Button::highLight() {
	//Voi ham nay chung ta se lay mau vang la mau chu va mau xanh dam la mau nen lam mau mac dinh
	Button highLight;
	highLight = *this;
	highLight.tColor = YELLOW;
	highLight.bColor = BLUE;
	highLight.draw();
	
}

void Button::highLight(int tColor, int bColor) {
	//Voi ham nay mau do nguoi lap trinh chon
	Button highLight;
	highLight.init(this->coordinates.x, this->coordinates.y, this->height, this->width, tColor, bColor, this->pattern, this->name);
	highLight.draw();
}

void Button::normal() {
	this->draw();
}

bool Button::isHover() {
	//ham mousex(), mousey() de lay toa do chuot trong cua so ta dang thao tac
	int x = mousex(), y = mousey();
	//neu con tro chuot nam trong pham vi hinh chu nhat cua nut thi duoc tinh la 1 hover
	if (x >= this->coordinates.x && x <= this->coordinates.x + this->width &&
		y >= this->coordinates.y && y <= this->coordinates.y + this->height)
		return 1;
	return 0;
}

void setTaskBarButtons() {
	int margin = 5, 
		width = (400 - margin * 4) / 3,
		height = 40;
	int xM, yM, xH, yH, xF, yF;
	xM = 15 + margin, yM = 15 + margin;
	xH = xM + margin + width, yH = yM;
	xF = xH + margin + width, yF = yH;
	menuBar.init(xM, yM, height, width, YELLOW, DARKGRAY, 1, "MENU");
	helpBar.init(xH, yH, height, width, YELLOW, DARKGRAY, 1, "TRO GIUP");
	fileBar.init(xF, yF, height, width, YELLOW, DARKGRAY, 1, "FILE");
	matrixButton.init(15, 295, 25, 400, BLACK, CYAN, 1, "MA TRAN TRONG SO");
}

void drawTaskBarButtons() {
	menuBar.draw();
	helpBar.draw();
	fileBar.draw();
}

void taskBar() {
	int option, ad;
	drawTaskBarButtons();
	if (menuBar.isHover()) {//thanh menu bar thi se hien thi ra danh sach cac cong cu o duoi
		option = menuTools();
		switch (option) {
			case 1: {
				bridgeEdges();
				break;
			}
			case 2: {
				cutVertices();
				break;
			}
			case 3: {
				connectedComponents();
				break;
			}
			case 4: {
				DFS();
				break;
			}
			case 5: {
				eulerCycle();
				break;
			}
			case 6: {
				knotVertices();
				break;
			}
			case 7: {
				pathXY();
				break;
			}
			case 8: {
				BFS();
				break;
			}
			case 9: {
				topo();
				break;
			}
			case 10: {
				hamilton();
				break;
			}
			default:
				break;
			}
		}
	else if (helpBar.isHover()) {
		option = helpTools();
		switch (option) {
			case 1: {
				drawUserManualBox(guideList[0], "Huong dan tao dinh");
				break;
			}
			case 2: {
				drawUserManualBox(guideList[1], "Huong dan tao canh");
				break;
			}
			case 3: {
				drawUserManualBox(guideList[2], "Huong dan sua ten dinh");
				break;
			}
			case 4: {
				drawUserManualBox(guideList[3], "Huong dan sua trong so cung");
				break;
			}
			case 5: {
				drawUserManualBox(guideList[4], "Huong dan xoa dinh");
				break;
			}
			case 6: {
				drawUserManualBox(guideList[5], "Huong dan xoa cung");
				break;
			}
			case 7: {
				drawUserManualBox(guideList[6], "Huong dan di chuyen dinh");
				break;
			}
			case 8: {
				drawUserManualBox(guideList[7], "Huong dan di chuyen cung");
				break;
			}
			default: 
				return;
		}
	}
	else if (fileBar.isHover()) {
		option = fileTools();
		switch(option) {
			case 1: {
				openFile();
				break;
			}
			case 2: {
				saveFile();
				break;
			}
			case 3: {
				deleteFile();
				break;
			}
			case 4: {
				bool confirm = drawYesNoBar("Ban co chac muon xoa tat ca?");
				if (confirm) 
					clearAllVertices();			
				break;
			}
		}
	}
}

int helpTools() {
	const short itemsAmount = 8;
	int margin = 5,
		height = 40,
		width = (400 - 3 * margin) / 2;
	int x0 = 15,
		y0 = 15 + height + 2 * margin;
	Button helpTools[8], helpToolsHoverBar /*khung de xu ly hover*/;
	helpToolsHoverBar.init(x0 + margin, y0 - margin, (height + margin) * itemsAmount / 2, 2 * width, BLACK, BLACK, 1, "");
	helpTools[0].name = "Cach tao dinh";
	helpTools[1].name = "Cach tao canh";
	helpTools[2].name = "Cach sua dinh";
	helpTools[3].name = "Cach sua canh";
	helpTools[4].name = "Cach xoa dinh";
	helpTools[5].name = "Cach xoa canh";
	helpTools[6].name = "Cach di chuyen dinh";
	helpTools[7].name = "Cach di chuyen cung";
	for (int i = 0; i < itemsAmount; i++) {
		helpTools[i].pattern = 1;
		helpTools[i].width = width;
		helpTools[i].height = 40;
		helpTools[i].bColor = BLACK;
		helpTools[i].tColor = YELLOW;
		if (i % 2 == 0) {
			helpTools[i].coordinates.x = x0 + margin;
			helpTools[i].coordinates.y = y0;
			helpTools[i + 1].coordinates.y = y0;
			y0 += (height + margin); 
		} else {
			helpTools[i].coordinates.x = x0 + width + 2 * margin; 
		}
	}
	for (int i = 0; i < itemsAmount; i++) {
		helpTools[i].draw();
	}
	int page = 0;
	while (true) {
		delay(10);
		setactivepage(page);
		setvisualpage(1- page);
		taskBarFrame.draw();
		drawTaskBarButtons();
		drawAllEdges();
		helpBar.highLight();
		if (helpBar.isHover() || helpToolsHoverBar.isHover()) {
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);
			for (int i = 0; i < itemsAmount; i++) {
				helpTools[i].draw();
				if (helpTools[i].isHover())
					helpTools[i].highLight();
				if (x >= helpTools[i].coordinates.x && x <= helpTools[i].coordinates.x + helpTools[i].width 
				&& y >= helpTools[i].coordinates.y && y <= helpTools[i].coordinates.y + helpTools[i].height)
					return i + 1;
			}
		}
		else
			break;
		page = 1 - page;
	}
	return -1;
}

int menuTools() {
	const short itemsAmount = 10;
	int margin = 5,
		height = 40;
	Button menuTools[10], menuToolsHoverBar/*khung de xu ly hover*/;
	menuToolsHoverBar.init(20, 60, 225, 395, BLACK, BLACK, 1, "");//Nut gia de xu ly hover
	menuTools[0].name = "Canh cau";
	menuTools[1].name = "Dinh tru";
	menuTools[2].name = "Thanh phan lien thong";
	menuTools[3].name = "DFS";
	menuTools[4].name = "Euler";
	menuTools[5].name = "Dinh that";
	menuTools[6].name = "Tim duong di ngan nhat";
	menuTools[7].name = "BFS";
	menuTools[8].name = "Topo sort";
	menuTools[9].name = "Hamliton";
	int y0 = 65, i;
			//Cap nut dau tien se co toa do nam duoi nut menuBar (y0 = 65)
	//Vong for duoi day se tao cac nut sap xep xen ke
	//Chi ap dung voi danh sach tools co so luong chan (cu the ta cho la 10 items)
	for (i = 0; i < itemsAmount; i++) {
		//Cai mau nen cho cac nut menu tool trong hop thoai menu tools
		menuTools[i].bColor = BLACK;
		//Cai mau chu
		menuTools[i].tColor = YELLOW;
		menuTools[i].height = height;
		menuTools[i].pattern = 1;
		//Cac cap nut (0 1), (2 3), (4 5), (6 7), (8 9) se co chung toa do y
		//Cac nut 0 2 4 6 8 se co chung toa do x
		//Cac nut 0 1 4 5 8 9 se co chung chieu rong 
		//Cac nut 2 6 se co chung chieu rong
		//Cac nut 3 7 se co chung chieu rong
		menuTools[i].coordinates.y = y0;
		if (i % 2 == 0) {
			//Cac nut 0 2 4 6 8 se co chung toa do x
			menuTools[i].coordinates.x = 15 + margin;
			//menuTools[i + 1].coordinates.y = y0;
		} 
		else 
			y0 += (height + margin);
			//Cap nut tiep theo co toa do bang cap nut truoc cong them 45 
			
		if (i % 4 == 0 || i % 4 == 1) {
			menuTools[i].width = (400 - 3 * margin) / 2;
		} 
		if (i % 4 == 1) {
			//Toa do x cua cac nut 1 5 9 la 179
			menuTools[i].coordinates.x = 15 + 2 * margin + (400 - 3 * margin) / 2;
		}
		if (i % 4 == 2) { 
			//Toa do x cua cac nut 3 7 la 227
			menuTools[i + 1].coordinates.x = 15 + 2 * margin + 2 * (400 - 3 * margin) / 3;
			//Chieu rong cua no la 103
			menuTools[i + 1].width = (400 - 3 * margin) / 3;
			//Chieu rong cua nut 2 6 la 200
			menuTools[i].width = 2 * (400 - 3 * margin) / 3;
			
		}
	}
	for (i = 0; i < itemsAmount; i++)
		//ve cac nut ra man hinh
		menuTools[i].draw();
	int page = 0;
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(10);
		taskBarFrame.draw();
		drawAllEdges();
		drawTaskBarButtons();
		menuBar.highLight();
		//Neu con tro chuot dang nam trong pham vi cua thanh menu 
		//hoac no dang nam trong pham vi o chua cac cong cu cua thanh menu thi moi thao tac
		//nguoc lai thi thoat vong lap
		if (menuToolsHoverBar.isHover() || menuBar.isHover()) {
			int x, y;
			//xu ly su kien chuot trai
			getmouseclick(WM_LBUTTONDOWN, x, y);
			for (i = 0; i < itemsAmount; i++) {
				menuTools[i].draw();
				if (menuTools[i].isHover())
					//neu di chuot vao mot trong cac menu tools thi se high light dong do
					menuTools[i].highLight();
				//neu click chuot trai vao mot trong so cac cong cu cua o chua menu tools 
				//thi se tra ve gia tri de xu ly cac ham sau nay (DFS, BFS,...)
				if (x >= menuTools[i].coordinates.x && x <= menuTools[i].coordinates.x + menuTools[i].width
				&& y >= menuTools[i].coordinates.y && y <= menuTools[i].coordinates.y + menuTools[i].height)
					return i + 1;
			}
		}
		else 
			break;
//		if (ismouseclick(WM_LBUTTONDOWN)) {
//			getmouseclick(WM_LBUTTONDOWN) 
//		}
		page = 1 - page;
	}
	return -1;
}

int fileTools() {
	const short itemsAmount = 4;
	int margin = 5,
		height = 40, 
		width = 400 - 2 * margin;
	int y0 = 15 + height + 2 * margin,
		x0 = 15;
	Button fileTools[4], fileToolsHoverBar/*khung x? l? hover*/;
	fileToolsHoverBar.init(x0 + margin, y0 - margin, itemsAmount * (height + margin), width, BLACK, BLACK, 1, "");
	fileTools[0].name = "Mo file";
	fileTools[1].name = "Luu file";
	fileTools[2].name = "Xoa file";
	fileTools[3].name = "Xoa do thi tren man hinh";
	for (int i = 0; i < itemsAmount; i++) {
		fileTools[i].tColor = YELLOW;
		fileTools[i].bColor = BLACK;
		fileTools[i].pattern = 1;
		fileTools[i].coordinates.x = x0 + margin;
		fileTools[i].coordinates.y = y0;
		y0 += (height + margin);
		fileTools[i].width = width;
		fileTools[i].height = height;
	}
	for (int i = 0; i < itemsAmount; i++)
		fileTools[i].draw();
	int page = 0;
	while (true) {
		delay(10);
		setactivepage(page);
		setvisualpage(1 - page);
		taskBarFrame.draw();
		drawTaskBarButtons();
		drawAllEdges();
		fileBar.highLight();
		if (fileToolsHoverBar.isHover() || fileBar.isHover()) {
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);
			for (int i = 0; i < itemsAmount; i++) {
				fileTools[i].draw();
				if (fileTools[i].isHover())
				//neu thanh file tools nao duoc chuot do nguoi dung di chuyen toi thi se duoc to sang
					fileTools[i].highLight();
				if (x >= fileTools[i].coordinates.x && x <= fileTools[i].coordinates.x + fileTools[i].width 
				&& y >= fileTools[i].coordinates.x && y <= fileTools[i].coordinates.y + fileTools[i].height)
					return i + 1;
			}
		}
		else 
			break;
		page = 1 - page;
	}
	return -1;
}

void setFrame() {
	//Khoi tao thanh tac vu gom co nut menu, nut help, nut file va mot so thao tac khac
	taskBarFrame.init(15, 15, 275, 400, 0, 3, 1, "");// width cu = 320
	//Khoi tao cua so thao tac cac tac vu nhu tao dinh, them dinh, xoa dinh, xoa canh,...
	vertexTaskBarFrame.init(420, 15, 689, 844, 0, 3, 1, "");
	//Khoi tai khung cua ma tran ke
	adjacencyMatrixFrame.init(15, 295, 409, 400, 0, 3, 1, "");
	//Khoi tao khung hien thi dinh va canh
	pointBarFrame.init(425, 20, 500, 834, 0, LIGHTGRAY , 1, ""); 
}

void drawFrame() {
	//Ve khung hien thi
	taskBarFrame.draw();
	vertexTaskBarFrame.draw();
	taskBarFrame.draw();
	adjacencyMatrixFrame.draw();
	pointBarFrame.draw();
}

void Vertex::create() {
	int x, y;
	if (ismouseclick(WM_LBUTTONDBLCLK)) {
		//Neu co click double chuot trai
		getmouseclick(WM_LBUTTONDBLCLK, x, y);
		//Neu no nam trong pham vi cua bang hien thi dinh thi moi cho tao dinh
		if (x >= 425 + RADIUS && x <= 1259 - RADIUS
		&& y >= 20 + RADIUS && y <= 520 - RADIUS) {
			for (int i = 0; i < n; i++) {
				int x0 = vertices[i].coordinates.x;
				int y0 = vertices[i].coordinates.y;
				if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= 4 * RADIUS * RADIUS)
					return;
			}
			this->coordinates.x = x;
			this->coordinates.y = y;
			circle(x, y, RADIUS);
			this->createName();
		}
	}
}

void Vertex::createName() {
	Button frame, finishButton, cancelButton, enterNameBar;
	int margin = 5;
	int height = 275 - 3 * margin - 40;
	int width = 400 - 2 * margin;
	int x, y;
	this->name = new char[3];
	char name[3] = ""; 
	char request[] = "Nhap ten dinh";
	frame.init(15 + margin, 15 + 2 * margin + 40, height, width, YELLOW, DARKGRAY, 1, "");
	finishButton.init(15 + 2 * margin, 275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HOAN THANH");
	cancelButton.init(15 + 3 * margin + (width - 3 * margin) / 2,275 - margin - 30, 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "HUY");
	enterNameBar.init(15 + 2 * margin, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50, 40, (width - 2 * margin), YELLOW, BLACK, 9, "");
	int page = 0;
	int i = 0; 
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(50);
		setfillstyle(10, GREEN);
		bar (1, 1, 1279, 719);
		drawFrame();
		drawTaskBarButtons();
		drawVertices();
		drawMatrix();
		frame.draw();
		drawAllEdges();
		finishButton.draw();
		cancelButton.draw();
		enterNameBar.draw();
		drawAddVertex(this->coordinates.x, this->coordinates.y);//ve dau vong tron co dau cong
		if (ismouseclick(WM_LBUTTONDOWN)) {
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x < frame.coordinates.x || x > frame.coordinates.x + frame.width
			|| y < frame.coordinates.y || y > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}
			clearmouseclick(WM_LBUTTONDOWN);
		}
		
		if (ismouseclick(WM_LBUTTONDBLCLK)) {// neu co double click chuot trai khi dang tao ten thi phai xoa di
											//de tranh truong hop sau khi tao ten thi tao luon dinh moi
			int xL, yL;
			getmouseclick(WM_LBUTTONDBLCLK, xL, yL);
			if (xL < frame.coordinates.x || xL > frame.coordinates.x + frame.width
			|| yL < frame.coordinates.y || yL > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}		
			clearmouseclick(WM_LBUTTONDBLCLK);
		}
		if (ismouseclick(WM_RBUTTONDOWN)) {//neu co click chuot phai khi dang tao ten thi phai xoa di 
										//de tranh truong hop sau khi tao ten thi mo thanh edit dinh
			int xR, yR;
			getmouseclick(WM_RBUTTONDOWN, xR, yR);
			if (xR < frame.coordinates.x || xR > frame.coordinates.x + frame.width
			|| yR < frame.coordinates.y || yR > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				enterNameBar.draw();
				if (strcmp(name, "") == 0) 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				else 
					outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);		
				finishButton.draw();
				cancelButton.draw();
				delay(150);
			}			
			clearmouseclick(WM_RBUTTONDOWN);
		}
		if (strcmp(name, "") != 0 && !isNamesake(name)//neu ten khong bi trong va ten khong bi trung voi cac dinh khac thi se duoc nhan enter
		&& x >= finishButton.coordinates.x && x <= finishButton.coordinates.x + finishButton.width
		&& y >= finishButton.coordinates.y && y <= finishButton.coordinates.y + finishButton.height)
		//xu ly click chuot trai vao o hoan thanh
			break;
		if (x >= cancelButton.coordinates.x && x <= cancelButton.coordinates.x + cancelButton.width
		&& y >= cancelButton.coordinates.y && y <= cancelButton.coordinates.y + cancelButton.height) {
			//neu chuot trai co click vao o huy thi ket thuc ham
			this->defaultVtex();
			return;
		}
		if (strcmp(name, "") == 0) {
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
			int x0 = this->coordinates.x, y0 = this->coordinates.y;
			line(x0, y0, x0 - RADIUS + 5, y0);
			line(x0, y0, x0 + RADIUS - 5, y0);
			line(x0, y0, x0, y0 + RADIUS - 5);
			line(x0, y0, x0, y0 - RADIUS + 5);
		}
		if (isNamesake(name)) {
			char c = getcolor();
			char note[20] = "Ten dinh bi trung!";
			setcolor(BLACK);
			setbkcolor(DARKGRAY);
			outtextxy(frame.coordinates.x + (frame.width - textwidth(note)) / 2, 195, note);
			setcolor(c);
			setbkcolor(BLACK);
		}
		if (kbhit()) {
			char key = getch();
			if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
				//Neu phim ta nhap la chu cai thi moi them vao chuoi
				if (i < 2) {
					//Neu chieu dai cua chuoi da day thi khong cho them vao chuoi
					name[i] = key;
					i++;//vi tri se duoc nhay len mot don vi sau khi them mot ky tu vao chuoi
				}
			}
			if (i > 2)
				i = 2;
			if (key == KEY_BACKSPACE) {
				strnDel(name, i - 1, 1);//xoa ki tu co vi tri sau cung cua chuoi
				i--;//vi tri se duoc giam xuong 1 don vi
			}
			if (i < 0)
				i = 0;
			if (strcmp(name, "") != 0 && key == KEY_ENTER && !isNamesake(name)) {
				break;
			}
			if (strcmp(name, "") == 0 && key == KEY_ENTER) {
				putchar(7);
				frame.highLight(WHITE, RED);
				enterNameBar.draw();
				outtextxy(15 + (400 - width) / 2 + (width - textwidth(request)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(request)) / 2, request);
				finishButton.draw();
				cancelButton.draw();
				delay(50);
			}
			if (isNamesake(name) && key == KEY_ENTER) {
				putchar(7);
				char c = getcolor();
				char note[20] = "Ten dinh bi trung!";
				setcolor(LIGHTRED);
				setbkcolor(DARKGRAY);
				outtextxy(frame.coordinates.x + (frame.width - textwidth(note)) / 2, 195, note);
				setcolor(c);
				setbkcolor(BLACK);
				delay(500);
			}
			if (isNamesake(name)) {
				for (int i = 0; i < n; i++)
					if (stricmp(name, vertices[i].name) == 0) {
						vertices[i].highLight(WHITE, RED);
						delay(50);
					}
			}
		}
		upper(name);
		outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2, 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, name);
		setbkcolor(LIGHTGRAY);
		outtextxy(this->coordinates.x - textwidth(name) / 2, this->coordinates.y - textheight(name) / 2, name);
		setbkcolor(BLACK);
		if (strcmp(name, "") != 0 && i < 2)
			outtextxy(15 + (400 - width) / 2 + (width - textwidth(name)) / 2 + textwidth(name), 20 + (275 - height) / 2 + (height - 40 - margin) + margin - 40 - 2 * margin - 50 + (40 - textheight(name)) / 2, "_");
		if (finishButton.isHover())
			finishButton.highLight();
		if (cancelButton.isHover())
			cancelButton.highLight();
		page = 1 - page;
	}
	strcpy(this->name, name);
}

void Vertex::draw() {
	int x = mousex(), y = mousey();
	if (this->coordinates.x == -1 && this->coordinates.y == -1)
		return;
	char c = getcolor();
	setfillstyle(1, BLUE);
	pieslice(this->coordinates.x, this->coordinates.y, 0, 360, RADIUS);
	setcolor(BLACK);
	setlinestyle(0, 1, 3);
	circle(this->coordinates.x, this->coordinates.y, RADIUS);
	setlinestyle(0, 1, 1);
	setcolor(BLUE);
	line(this->coordinates.x, this->coordinates.y - 1, this->coordinates.x + RADIUS, this->coordinates.y - 1);
	setcolor(YELLOW);
	setbkcolor(BLUE);
	outtextxy(this->coordinates.x - textwidth(this->name) / 2, this->coordinates.y - textheight(this->name) / 2, this->name);
	if (this->isHover())
		this->highLight();
	setcolor(c);
	setfillstyle(1, c);
	setbkcolor(BLACK);
}

void Vertex::highLight() {
	char c = getcolor();
	setfillstyle(1, LIGHTBLUE);
	pieslice(this->coordinates.x, this->coordinates.y, 0, 0, RADIUS);
	setcolor(BLACK);
	circle(this->coordinates.x, this->coordinates.y, RADIUS);
	setcolor(LIGHTBLUE);
	line(this->coordinates.x, this->coordinates.y - 1, this->coordinates.x + RADIUS, this->coordinates.y - 1);
	setcolor(YELLOW);
	setbkcolor(LIGHTBLUE);
	outtextxy(this->coordinates.x - textwidth(this->name) / 2, this->coordinates.y - textheight(this->name) / 2, this->name);
	setcolor(c);
	setbkcolor(BLACK);
	setfillstyle(1, c);
}

void Vertex::highLight(int tColor, int bColor) {
	char c = getcolor();
	setfillstyle(1, bColor);
	pieslice(this->coordinates.x, this->coordinates.y, 0, 0, RADIUS);
	setcolor(BLACK);
	circle(this->coordinates.x, this->coordinates.y, RADIUS);
	setcolor(bColor);
	line(this->coordinates.x, this->coordinates.y - 1, this->coordinates.x + RADIUS, this->coordinates.y - 1);
	setcolor(tColor);
	setbkcolor(bColor);
	outtextxy(this->coordinates.x - textwidth(this->name) / 2, this->coordinates.y - textheight(this->name) / 2, this->name);
	setcolor(c);
	setbkcolor(BLACK);
	setfillstyle(1, c);	
}

bool Vertex::isDefaultVtex() {
	if (this->coordinates.x == -1 && this->coordinates.y == -1)
		return 1;
	return 0;
}

bool Vertex::isHover() {
	int x = mousex(), y = mousey();
	if ((x - this->coordinates.x) * (x - this->coordinates.x) + (y - this->coordinates.y) * (y - this->coordinates.y) <= RADIUS * RADIUS)
		// tam I(a,b) phuong trinh (x - a)2 + (y - b)2 <= r2
		return 1;
	return 0;
}

void Button::changeColor(int tColor, int bColor) {
	this->tColor = tColor;
	this->bColor = bColor;
}

bool Vertex::isClickLMButton() {
	int x, y;
	getmouseclick(WM_LBUTTONDOWN, x, y);
	if ((this->coordinates.x - x) * (this->coordinates.x - x) 
	+ (this->coordinates.y - y) * (this->coordinates.y - y) <= RADIUS * RADIUS)
		return 1;
	return 0;
}


void Vertex::defaultVtex() {
	this->coordinates.x = -1;
	this->coordinates.y = -1;
}

void drawVertices() {
	int x, y;
	for (int i = 0; i < n; i++) 
		vertices[i].draw();
}

void initDefaultVertices() {
	for (int i = 0; i < n; i++) {
		vertices[i].defaultVtex();
	}
}

void loadFileStartUp() {
	ifstream input ("filesInProgram//startUpFile.txt");
	if (input.is_open()) {
		input >> n;
		for (int i = 0; i < n; i++) {
			input >> vertices[i].coordinates.x >> vertices[i].coordinates.y;
			vertices[i].name = new char[3];
			input >> vertices[i].name;
			if (strcmp(vertices[i].name, "") == 0) {
				n = 0;
				input.close();
				return;
			}
		}
		for (int i = 0; i < n; i++) 
			for (int j = 0; j < n; j++) {
				input >> G[i][j];
			}
	} else {
		n = 0;
		FILE * newFile = fopen("filesInProgram//startUpFile.txt", "a");
		fprintf(newFile, "%d", 0);
		fclose(newFile);
	}
 
	input.close();
}

void saveFileStartUp() {
	fstream output;
	output.open("filesInProgram//startUpFile.txt", ios::out);
	output << n << endl;
	for (int i = 0; i < n; i++) {
		output << vertices[i].coordinates.x << " "
		 << vertices[i].coordinates.y << " "
		 << vertices[i].name << endl;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			output << G[i][j] << " ";
		}
		output << endl;
	}
	output.close();
}

void addVertexToList(Vertex vtex) {
	if (n < MAX && !vtex.isDefaultVtex()) {
		vertices[n] = vtex;
		for (int i = 0; i <= n; i++) {
			G[n][i] = 0;
			G[i][n] = 0;
		}
		n++;
		saveFileStartUp();
	}
}

void clearAllVertices() {
	n = 0;
	for (int i = 0; i < MAX; i++) {
		vertices[i].defaultVtex();
	}
	saveFileStartUp();
}

bool drawYesNoBar(char *question) {
	putchar(7);
	Button frame, yesButton, noButton, xButton;
	int height = 100;
	int width = 300;
	int margin = 5;
	int x, y;
	frame.init(425 + (834 - width) / 2 , 20 + (500 - height) / 2, height, width, YELLOW, BLACK, 1, "");
	xButton.init(frame.coordinates.x + frame.width - 25, frame.coordinates.y, 25, 25, WHITE, RED, 1, "x");
	yesButton.init(425 + (834 - width) / 2 + margin, 20 + (500 - height) / 2 + (height - 40 - margin), 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "CO");
	noButton.init(425 + (834 - width) / 2 + margin * 2 + (width - 3 * margin) / 2, 20 + (500 - height) / 2 + (height - 40 - margin), 40, (width - 3 * margin) / 2, YELLOW, BLACK, 9, "KHONG");
	int page = 0;
	while (true) {
		setactivepage(page);
		setvisualpage(1 - page);
		delay(100);
		drawFrame();
		drawTaskBarButtons();
		drawVertices();
		drawMatrix();
		drawAllEdges();
		frame.draw();
		xButton.draw();
		noButton.draw();
		yesButton.draw();
		if (ismouseclick(WM_RBUTTONDOWN))//neu co click chuot phai vao man hinh thi phai xoa 
										// de no khong mo hop thoai edit dinh khi ta vo tinh click chuot phai vao dinh bat ky
			clearmouseclick(WM_RBUTTONDOWN);
		if (ismouseclick(WM_LBUTTONDBLCLK))//neu co double click chuot trai vao man hinh thi phai xoa no vi neu khi thoat 
											//ham nay co the tao dinh moi ngay tuc thi
			clearmouseclick(WM_LBUTTONDBLCLK);
		outtextxy(425 + (834 - width) / 2 + (width - textwidth(question)) / 2, 20 + (500 - height) / 2 + 10, question);
		if (kbhit()) {
			char key = getch();
			if (key == KEY_ENTER || key == 'y' || key == 'Y') 
				return 1;	
			if (key == 'n' || key == 'N')
				return 0;
		}
		if (xButton.isHover())
			xButton.highLight(WHITE, LIGHTRED);
		if (yesButton.isHover())//neu chuot di chuyen toi thanh yes thanh yes se sang
			yesButton.highLight();
		else if (noButton.isHover())//neu chuot di chuyen toi thanh no thanh no se sang
			noButton.highLight();
		if (ismouseclick(WM_LBUTTONDOWN)) {
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (x >= yesButton.coordinates.x && x <= yesButton.coordinates.x + yesButton.width
			&& y >= yesButton.coordinates.y && y <= yesButton.coordinates.y + yesButton.height)
				return 1;
			if (x >= noButton.coordinates.x && x <= noButton.coordinates.x + noButton.width
			&& y >= noButton.coordinates.y && y <= noButton.coordinates.y + noButton.height)
				return 0;
			if (x >= xButton.coordinates.x && x <= xButton.coordinates.x + xButton.width
			&& y >= xButton.coordinates.y && y <= xButton.coordinates.y + xButton.height)
				return 0;
			if (x < frame.coordinates.x || x > frame.coordinates.x + frame.width
			|| y < frame.coordinates.y || y > frame.coordinates.y + frame.height) {
				putchar(7);
				frame.highLight(BLACK, RED);
				xButton.highLight(RED, WHITE);
				yesButton.draw();
				noButton.draw();
				outtextxy(425 + (834 - width) / 2 + (width - textwidth(question)) / 2, 20 + (500 - height) / 2 + 10, question);
				delay(150);
			}
		}
 		page = 1 - page;
	}
}

bool isClickOtherVertex(Vertex vtex) {
	int x = vtex.coordinates.x, y = vtex.coordinates.y;
	for (int i = 0; i < n; i++) 
		if ((x - vertices[i].coordinates.x) * (x - vertices[i].coordinates.x) + (y - vertices[i].coordinates.y) * (y - vertices[i].coordinates.y) <= 4 * RADIUS * RADIUS)
		// tam I(a,b) phuong trinh (x - a)2 + (y - b)2 <= r2)
			return 1;
	return 0;
}

bool isEmptyVertex() {
	if (n == 0)
		return 1;
	return 0;
}

void setUserTextStyle() {
	settextstyle(11, 0, 2);
}

void showWelcome() {
	settextstyle(0, 0, 10);	
	char c = getcolor();
	setcolor(YELLOW);
	if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	for (int i = 0; i < 200; i++) {
		outtextxy((1280 - textwidth("WELCOME")) / 2, (720 - textheight("WELCOME")) / 2, "WELCOME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("ELCOME")) / 2, (720 - textheight("ELCOME")) / 2, "ELCOME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("LCOME")) / 2, (720 - textheight("LCOME")) / 2, "LCOME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("COME")) / 2, (720 - textheight("COME")) / 2, "COME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("OME")) / 2, (720 - textheight("OME")) / 2, "OME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("ME")) / 2, (720 - textheight("ME")) / 2, "ME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("E")) / 2, (720 - textheight("E")) / 2, "E");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("E")) / 2, (720 - textheight("E")) / 2, "E");
		delay(10);
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("ME")) / 2, (720 - textheight("ME")) / 2, "ME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("OME")) / 2, (720 - textheight("OME")) / 2, "OME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("COME")) / 2, (720 - textheight("COME")) / 2, "COME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("OME")) / 2, (720 - textheight("OME")) / 2, "OME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("ME")) / 2, (720 - textheight("ME")) / 2, "ME");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	for (int i = 0; i < 20; i++) {
		outtextxy((1280 - textwidth("E")) / 2, (720 - textheight("E")) / 2, "E");
		delay(10);
		if (ismouseclick(WM_LBUTTONDOWN) 
	|| ismouseclick(WM_RBUTTONDOWN) 
	|| ismouseclick(WM_LBUTTONDBLCLK) 
	|| ismouseclick(WM_RBUTTONDBLCLK)
	) return;
	}
	cleardevice();
	setcolor(c);
	setUserTextStyle();
}
