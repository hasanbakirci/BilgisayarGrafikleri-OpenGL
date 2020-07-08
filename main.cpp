#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <iostream>
#include <GL/Glut.h>
#include "imageloader.h" // Kaplama yapabilmek i�in gerekli k�t�phane
#include <assert.h>
#include <fstream>
#include <stdlib.h>





using namespace std;

float xPiramitHareket = 0, yPiramitHareket = 0, zPiramitHareket = 0, xKupHareket = 0, yKupHareket = 0, zKupHareket = 0;

GLuint textureKopek, textureKedi; // Resimlerin adreslerini tutaca��m�z de�i�kenler

GLfloat aciPiramit = 0.0f; // Pramitin d�nme a��s�
					
GLfloat aciKup = 0.0f; // K�b�n d�nme a��s�
int t_yenileme = 20; // milisaniye cinsinden yenileme aral���


					 // Piramit i�in hareket fonksiyonlar�
void piramitSag(void)
{
	glLoadIdentity();//koordinat sistemimiz etkilenmesin
	xPiramitHareket += 0.1;
}
void piramitSol(void)
{
	glLoadIdentity();
	xPiramitHareket -= 0.1;
}
void piramitYukari(void)
{
	glLoadIdentity();
	yPiramitHareket += 0.1;
}
void piramitAsagi(void)
{
	glLoadIdentity();
	yPiramitHareket -= 0.1;
}
void piramitUzak(void)
{
	glLoadIdentity();
	zPiramitHareket -= 0.1;
}
void piramitYakin(void)
{
	glLoadIdentity();
	zPiramitHareket += 0.1;
}

// K�p i�in hareket fonksiyonlar�
void kupSag(void)
{
	glLoadIdentity();//koordinat sistemimiz etkilenmesin
	xKupHareket += 0.1;
}
void kupSol(void)
{
	glLoadIdentity();
	xKupHareket -= 0.1;
}
void kupYukari(void)
{
	glLoadIdentity();
	yKupHareket += 0.1;
}
void kupAsagi(void)
{
	glLoadIdentity();
	yKupHareket -= 0.1;
}
void kupUzak(void)
{
	glLoadIdentity();
	zKupHareket -= 0.1;
}
void kupYakin(void)
{
	glLoadIdentity();
	zKupHareket += 0.1;
}


// K�p i�in klavye tu�lar�
void KupHareketTusu(int dugme, int x, int y)
{
	switch (dugme)
	{
	case GLUT_KEY_LEFT:kupSol(); break;
	case GLUT_KEY_RIGHT:kupSag(); break;
	case GLUT_KEY_UP:kupYukari(); break;
	case GLUT_KEY_DOWN:kupAsagi(); break;
	case GLUT_KEY_PAGE_UP:kupUzak(); break;
	case GLUT_KEY_PAGE_DOWN:kupYakin(); break;
	}
	glutPostRedisplay();//hareket(i�lem) sonras� g�ncelleme i�in
}


// Piramit i�in klavye tu�lar�
void PiramitHareketTusu(unsigned char dugme, int x, int y)
{
	switch (dugme)
	{
	case'a':piramitSol(); break;
	case'd':piramitSag(); break;
	case'w':piramitYukari(); break;
	case's':piramitAsagi(); break;
	case'+':piramitUzak(); break;
	case'-':piramitYakin(); break;
	}
	glutPostRedisplay();//hareket(i�lem) sonras� g�ncelleme i�in
}

void Ayarlar() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); /* opaque bir ye�il-mavi. Penceremizin arka alan rengidir; asl�nda ekran�n hangi renkle temizlenece�ini belirtir.*/
	glClearDepth(1.0f); /*glClearDepth fonksiyonunu 1.0 de�eri ile �a��rarak derinlik tamponunun temizlenmesini sa�l�yoruz. 1.0 ile Arka plan derinli�ini en uza�a ayarlan�yor*/
	glEnable(GL_DEPTH_TEST); //z-ekseni i�in derinlik testi /* Derinlik testinin yap�labilmesi i�in GL_DEPTH_TEST sembolik sabitini, glEnable fonksiyonuna parametre olarak ge�erek �a��r�yoruz.*/
	glDepthFunc(GL_LEQUAL); // Karma��k grafiklerde Derinlik testi i�in kullan�l�r:
							/*Daha sonra glDepthFunc fonksiyonu ile derinlik testinde kullan�lacak fonksiyonu belirtiyoruz.
							Burada parametre olarak ge�ilen GL_LEQUAL sembolik sabitinin belirtti�i derinlik testi fonksiyonu, o an gelen z de�eri, tamponda saklanan z de�erine e�it ise veya ondan daha k���kse ba�ar�l� olur.*/
	glShadeModel(GL_SMOOTH); // karma��k grafiklerde D�zg�n g�lgelendirmeyi etkinle�tirir
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); /* karma��k grafiklerde perspektif ayarlamalar�n�(d�zeltmelerini) yapar.*/
}

void CizimFonksiyonu()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* belirli bir bellek tamponunu veya tampon bile�enlerini silmek icin kullanilmaktadir.Tampon ile kastimiz bellekte ki bir is icin ayr�lm�� belirli bir hafiza parcasidir. Bir cizimin kirmizi,yesil,ve mavi bileseleri renk tamponu ve ya piksel tamponu olarak adlandirilir.*/
	glMatrixMode(GL_MODELVIEW); /* Kameran�n pozisyonu ve cisimlerin 3D geometrisi ile ilgili �izme, g�r�nt�leme, konumland�rma ve y�nlendirme i�lemlerinden �nce matrislerin hangi i�lem i�in d�zenlenece�i belirtilmelidir: bu �rnekte -->model-view matrisi kulan�l�yor*/

	glEnable(GL_TEXTURE_2D);  // Kaplama yapmak i�in her zaman enable olmal�d�r
	

	/// K�P //////////////////////////////////
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, textureKedi); // Kaplan�lacak resimi g�steriyoruz
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTranslatef(1.5f, 0.0f, -8.0f); // K�p�n konumu
	glTranslatef(xKupHareket, yKupHareket, zKupHareket);
	glRotatef(aciKup, 1.0f, 0.5f, 0.0f);  // K�p d�nme i�lemi i�in
	glBegin(GL_QUADS);
	//�st
	// gl TexCoord2f() Bu fonksyion her vertexten �nce cagr�lmas� gerekiyor Bu fonksiyon kaplaman�n koordinatlar� olmaktar�r.
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	//alt
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	//�n
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	//arka
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// K�p�n sol y�z�                                 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// K�p�n sa� y�z�
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	/// P�RAM�T ////////////////////////////////////
	glLoadIdentity(); // Orjinal Duruma d�n�l�r (Reset)
	glBindTexture(GL_TEXTURE_2D, textureKopek);   // Kaplan�lacak resimi g�steriyoruz
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTranslatef(-1.5f, 0.0f, -7.0f); // Ekrandaki konumu
	glTranslatef(xPiramitHareket, yPiramitHareket, zPiramitHareket);
	glRotatef(aciPiramit, 0.0f, 1.0f, 0.0f);  // Piramit d�nme i�lemi i�in
	glBegin(GL_TRIANGLES);

	//�n taraf
	glTexCoord2f(0.0f, 1.5f); glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.5f, 0.0f);  glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// sa� taraf                                             

	glTexCoord2f(0.0f, 1.5f);  glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.5f, 0.0f);  glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(1.0f, -1.0f, -1.0f);
	// sol taraf

	glTexCoord2f(0.0f, 1.5f); glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.5f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	// arka taraf
	glTexCoord2f(0.0f, 1.5f); glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.5f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();



	glutSwapBuffers(); /*�er�eveyi �izdikten sonra �n ve arka tamponlar de�i�ir. Burada, biri g�sterilen di�eri gizlenen, �ift tampon modunda pencere kullan�yoruz. Bu durumda OpenGL komutlar�n�n g�r�nt�lenmesi daima gizli tampon i�ine yap�l�r. glutSwapBuffers(), �a�r�ld���nda, tamponlar� de�i�toku� ederek �izileni pencere i�inde g�r�nt�ler. Tercih edilmesinin nedeni, �er�eve i�eri�inin sat�r sat�r olu�umunu insan g�z�nden saklamas�d�r*/
	aciPiramit += 1.0f; 
	aciKup += 1.0f;

	//glDisable(GL_TEXTURE_2D);
}

void zamanlayici(int value) {
	glutPostRedisplay(); //CizimFonksiyonu()'nun yapt��� i�lem sonras� g�ncelleme i�in
	glutTimerFunc(t_yenileme, zamanlayici, 0); // next timer call milliseconds later
}

/*pencerenin en boy oran�n� i�in: */
void GorunumAyari(GLsizei x, GLsizei y)
{
	// Yeni pencerenin en boy oran�n� hesaplama
	if (y == 0) y = 1; // 0'a b�l�mden korunmak i�in
	GLfloat oran = (GLfloat)x / (GLfloat)y; //float oran; diyebilirdik
											/* Genellikle pencere geni�li�i ile ayn�d�r. E�er viewport geni�lik / y�kseklik oran� projeksiyondakinden farkl� ise nesneler olduklar�ndan farkl� bi�imde g�r�nebilirler*/
	glViewport(0, 0, x, y);
	// en boy oran�n� g�r�n�m penceresiyle e�le�ecek �ekilde ayarlama:
	glMatrixMode(GL_PROJECTION); //izd���m g�r�n�m�ne ayarlamak i�in
	glLoadIdentity(); // Orjinal Duruma d�n�l�r (Reset)
	gluPerspective(45.0f, oran, 1.0f, 20.0f);

}


GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Kaplama i�in yer olu�turuyoruz
	glBindTexture(GL_TEXTURE_2D, textureId); // OpenGL ye hangi dokuyu d�zenleyece�imizi g�steriyoruz

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,                       //OpenGL nin g�r�nt� i�in kulalnd��� format
		image->width, image->height,  //geni�lik ve y�kseklik
		0,
		GL_RGB, // piksellerin RGB format�nda sakland��� i�in
		GL_UNSIGNED_BYTE,

		image->pixels);//ger�ek piksel verileri
	return textureId;
}

void resim() {// kaplama yapaca��m�z resimi open gl ye y�klemek i�in fonksiyon
	Image* imageKopek = loadBMP("kopek.bmp"); Image* imageKedi = loadBMP("kedi.bmp");
	textureKopek = loadTexture(imageKopek); // Art�k textureKopek textureKedi ile resimlerimize eri�ebiliriz
	textureKedi = loadTexture(imageKedi);
	delete imageKopek;
	delete imageKedi;
}


int main(int argc, char** argv) {


	glutInit(&argc, argv); // GLUT'u ba�lat
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // �ift tampon kullanma
	glutInitWindowPosition(50, 50); //pencere ba�lang�� noktas�
	glutInitWindowSize(800, 600); // Pencere geni�lik ve y�kseklik ayarlayal�m
	glutCreateWindow("3-Boyutlu Sekiller"); //pencere ba�l���
	glutDisplayFunc(CizimFonksiyonu); // kendisine parametre olarak g�nderilen CizimFonksiyonu fonksiyonu �a��r�r ve �izimi yapar
	glutReshapeFunc(GorunumAyari); // E�er pencere yeniden boyutland�r�l�rsa yeni geni�lik ve y�kseklik de�erleri atan�r.
	Ayarlar(); //OpenGL Ayarlar�m�z
	glutSpecialFunc(KupHareketTusu);//karakter tu�lar�n� kullanmak i�in
	glutKeyboardFunc(PiramitHareketTusu);//�zel i�lev tu�lar�n� kullanmak i�in
	glutTimerFunc(0, zamanlayici, 0); // zamanlay�c� �a�r�m�
	resim(); // resim y�kleme
	glutMainLoop(); // �izimi d�ng�ye sokar.

	return 0;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//       A�A�IDAKI KODLAR RESIM DOSYASINI OPEN GL ICERISINE EKLEMEK ICINDIR.
//       AYRI BIR CPP DOSYASIYDI  TEK CPP DOSYASI OLMASI ICIN BURAYA EKLEDIM.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {

}

Image::~Image() {
	delete[] pixels;
}

namespace {
	//Converts a four-character array to an integer, using little-endian form
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
			((unsigned char)bytes[2] << 16) |
			((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	//Converts a two-character array to a short, using little-endian form
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	//Reads the next four bytes as an integer, using little-endian form
	int readInt(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}

	//Reads the next two bytes as a short, using little-endian form
	short readShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}

	//Just like auto_ptr, but for arrays
	template<class T>
	class auto_array {
	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit auto_array(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}

		auto_array(const auto_array<T> &aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		~auto_array() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}

		T* get() const {
			return array;
		}

		T &operator*() const {
			return *array;
		}

		void operator=(const auto_array<T> &aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		T* operator->() const {
			return array;
		}

		T* release() {
			isReleased = true;
			return array;
		}

		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}

		T* operator+(int i) {
			return array + i;
		}

		T &operator[](int i) {
			return array[i];
		}
	};
}

Image* loadBMP(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int dataOffset = readInt(input);

	//Read the header
	int headerSize = readInt(input);
	int width;
	int height;
	switch (headerSize) {
	case 40:
		//V3
		width = readInt(input);
		height = readInt(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
		assert(readShort(input) == 0 || !"Image is compressed");
		break;
	case 12:
		//OS/2 V1
		width = readShort(input);
		height = readShort(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
		break;
	case 64:
		//OS/2 V2
		assert(!"Can't load OS/2 V2 bitmaps");
		break;
	case 108:
		//Windows V4
		assert(!"Can't load Windows V4 bitmaps");
		break;
	case 124:
		//Windows V5
		assert(!"Can't load Windows V5 bitmaps");
		break;
	default:
		assert(!"Unknown bitmap format");
	}

	//Read the data
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);

	//Get the data into the right format
	auto_array<char> pixels2(new char[width * height * 3]);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}

	input.close();
	return new Image(pixels2.release(), width, height);
}