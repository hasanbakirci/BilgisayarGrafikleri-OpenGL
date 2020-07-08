#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <iostream>
#include <GL/Glut.h>
#include "imageloader.h" // Kaplama yapabilmek için gerekli kütüphane
#include <assert.h>
#include <fstream>
#include <stdlib.h>





using namespace std;

float xPiramitHareket = 0, yPiramitHareket = 0, zPiramitHareket = 0, xKupHareket = 0, yKupHareket = 0, zKupHareket = 0;

GLuint textureKopek, textureKedi; // Resimlerin adreslerini tutacaðýmýz deðiþkenler

GLfloat aciPiramit = 0.0f; // Pramitin dönme açýsý
					
GLfloat aciKup = 0.0f; // Kübün dönme açýsý
int t_yenileme = 20; // milisaniye cinsinden yenileme aralýðý


					 // Piramit için hareket fonksiyonlarý
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

// Küp için hareket fonksiyonlarý
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


// Küp için klavye tuþlarý
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
	glutPostRedisplay();//hareket(iþlem) sonrasý güncelleme için
}


// Piramit için klavye tuþlarý
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
	glutPostRedisplay();//hareket(iþlem) sonrasý güncelleme için
}

void Ayarlar() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); /* opaque bir yeþil-mavi. Penceremizin arka alan rengidir; aslýnda ekranýn hangi renkle temizleneceðini belirtir.*/
	glClearDepth(1.0f); /*glClearDepth fonksiyonunu 1.0 deðeri ile çaðýrarak derinlik tamponunun temizlenmesini saðlýyoruz. 1.0 ile Arka plan derinliðini en uzaða ayarlanýyor*/
	glEnable(GL_DEPTH_TEST); //z-ekseni için derinlik testi /* Derinlik testinin yapýlabilmesi için GL_DEPTH_TEST sembolik sabitini, glEnable fonksiyonuna parametre olarak geçerek çaðýrýyoruz.*/
	glDepthFunc(GL_LEQUAL); // Karmaþýk grafiklerde Derinlik testi için kullanýlýr:
							/*Daha sonra glDepthFunc fonksiyonu ile derinlik testinde kullanýlacak fonksiyonu belirtiyoruz.
							Burada parametre olarak geçilen GL_LEQUAL sembolik sabitinin belirttiði derinlik testi fonksiyonu, o an gelen z deðeri, tamponda saklanan z deðerine eþit ise veya ondan daha küçükse baþarýlý olur.*/
	glShadeModel(GL_SMOOTH); // karmaþýk grafiklerde Düzgün gölgelendirmeyi etkinleþtirir
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); /* karmaþýk grafiklerde perspektif ayarlamalarýný(düzeltmelerini) yapar.*/
}

void CizimFonksiyonu()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* belirli bir bellek tamponunu veya tampon bileþenlerini silmek icin kullanilmaktadir.Tampon ile kastimiz bellekte ki bir is icin ayrýlmýþ belirli bir hafiza parcasidir. Bir cizimin kirmizi,yesil,ve mavi bileseleri renk tamponu ve ya piksel tamponu olarak adlandirilir.*/
	glMatrixMode(GL_MODELVIEW); /* Kameranýn pozisyonu ve cisimlerin 3D geometrisi ile ilgili çizme, görüntüleme, konumlandýrma ve yönlendirme iþlemlerinden önce matrislerin hangi iþlem için düzenleneceði belirtilmelidir: bu örnekte -->model-view matrisi kulanýlýyor*/

	glEnable(GL_TEXTURE_2D);  // Kaplama yapmak için her zaman enable olmalýdýr
	

	/// KÜP //////////////////////////////////
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, textureKedi); // Kaplanýlacak resimi gösteriyoruz
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTranslatef(1.5f, 0.0f, -8.0f); // Küpün konumu
	glTranslatef(xKupHareket, yKupHareket, zKupHareket);
	glRotatef(aciKup, 1.0f, 0.5f, 0.0f);  // Küp dönme iþlemi için
	glBegin(GL_QUADS);
	//üst
	// gl TexCoord2f() Bu fonksyion her vertexten önce cagrýlmasý gerekiyor Bu fonksiyon kaplamanýn koordinatlarý olmaktarýr.
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	//alt
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	//ön
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	//arka
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Küpün sol yüzü                                 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Küpün sað yüzü
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	/// PÝRAMÝT ////////////////////////////////////
	glLoadIdentity(); // Orjinal Duruma dönülür (Reset)
	glBindTexture(GL_TEXTURE_2D, textureKopek);   // Kaplanýlacak resimi gösteriyoruz
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTranslatef(-1.5f, 0.0f, -7.0f); // Ekrandaki konumu
	glTranslatef(xPiramitHareket, yPiramitHareket, zPiramitHareket);
	glRotatef(aciPiramit, 0.0f, 1.0f, 0.0f);  // Piramit dönme iþlemi için
	glBegin(GL_TRIANGLES);

	//ön taraf
	glTexCoord2f(0.0f, 1.5f); glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.5f, 0.0f);  glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// sað taraf                                             

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



	glutSwapBuffers(); /*Çerçeveyi çizdikten sonra Ön ve arka tamponlar deðiþir. Burada, biri gösterilen diðeri gizlenen, çift tampon modunda pencere kullanýyoruz. Bu durumda OpenGL komutlarýnýn görüntülenmesi daima gizli tampon içine yapýlýr. glutSwapBuffers(), çaðrýldýðýnda, tamponlarý deðiþtokuþ ederek çizileni pencere içinde görüntüler. Tercih edilmesinin nedeni, çerçeve içeriðinin satýr satýr oluþumunu insan gözünden saklamasýdýr*/
	aciPiramit += 1.0f; 
	aciKup += 1.0f;

	//glDisable(GL_TEXTURE_2D);
}

void zamanlayici(int value) {
	glutPostRedisplay(); //CizimFonksiyonu()'nun yaptýðý iþlem sonrasý güncelleme için
	glutTimerFunc(t_yenileme, zamanlayici, 0); // next timer call milliseconds later
}

/*pencerenin en boy oranýný için: */
void GorunumAyari(GLsizei x, GLsizei y)
{
	// Yeni pencerenin en boy oranýný hesaplama
	if (y == 0) y = 1; // 0'a bölümden korunmak için
	GLfloat oran = (GLfloat)x / (GLfloat)y; //float oran; diyebilirdik
											/* Genellikle pencere geniþliði ile aynýdýr. Eðer viewport geniþlik / yükseklik oraný projeksiyondakinden farklý ise nesneler olduklarýndan farklý biçimde görünebilirler*/
	glViewport(0, 0, x, y);
	// en boy oranýný görünüm penceresiyle eþleþecek þekilde ayarlama:
	glMatrixMode(GL_PROJECTION); //izdüþüm görünümüne ayarlamak için
	glLoadIdentity(); // Orjinal Duruma dönülür (Reset)
	gluPerspective(45.0f, oran, 1.0f, 20.0f);

}


GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Kaplama için yer oluþturuyoruz
	glBindTexture(GL_TEXTURE_2D, textureId); // OpenGL ye hangi dokuyu düzenleyeceðimizi gösteriyoruz

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,                       //OpenGL nin görüntü için kulalndýðý format
		image->width, image->height,  //geniþlik ve yükseklik
		0,
		GL_RGB, // piksellerin RGB formatýnda saklandýðý için
		GL_UNSIGNED_BYTE,

		image->pixels);//gerçek piksel verileri
	return textureId;
}

void resim() {// kaplama yapacaðýmýz resimi open gl ye yüklemek için fonksiyon
	Image* imageKopek = loadBMP("kopek.bmp"); Image* imageKedi = loadBMP("kedi.bmp");
	textureKopek = loadTexture(imageKopek); // Artýk textureKopek textureKedi ile resimlerimize eriþebiliriz
	textureKedi = loadTexture(imageKedi);
	delete imageKopek;
	delete imageKedi;
}


int main(int argc, char** argv) {


	glutInit(&argc, argv); // GLUT'u baþlat
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // çift tampon kullanma
	glutInitWindowPosition(50, 50); //pencere baþlangýç noktasý
	glutInitWindowSize(800, 600); // Pencere geniþlik ve yükseklik ayarlayalým
	glutCreateWindow("3-Boyutlu Sekiller"); //pencere baþlýðý
	glutDisplayFunc(CizimFonksiyonu); // kendisine parametre olarak gönderilen CizimFonksiyonu fonksiyonu çaðýrýr ve çizimi yapar
	glutReshapeFunc(GorunumAyari); // Eðer pencere yeniden boyutlandýrýlýrsa yeni geniþlik ve yükseklik deðerleri atanýr.
	Ayarlar(); //OpenGL Ayarlarýmýz
	glutSpecialFunc(KupHareketTusu);//karakter tuþlarýný kullanmak için
	glutKeyboardFunc(PiramitHareketTusu);//özel iþlev tuþlarýný kullanmak için
	glutTimerFunc(0, zamanlayici, 0); // zamanlayýcý çaðrýmý
	resim(); // resim yükleme
	glutMainLoop(); // çizimi döngüye sokar.

	return 0;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//       AÞAÐIDAKI KODLAR RESIM DOSYASINI OPEN GL ICERISINE EKLEMEK ICINDIR.
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