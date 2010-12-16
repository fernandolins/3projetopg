// Iluminacao.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização
// de objetos 3D com a inserção de uma fonte de luz.
// Este código está baseado nos exemplos disponíveis no livro
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.

#define GLUT_DISABLE_ATEXIT_HACK
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#include <gl/glu.h>
#include <gl/gl.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vetor.h"
#include "tgaload.h"


#define PI 3.1415265359
#define PIdiv180 3.1415265359/180.0

// Qtd máxima de texturas a serem usadas no programa
#define MAX_NO_TEXTURES 10

GLfloat angle, fAspect;

GLdouble x_camera = 0;
GLdouble y_camera = 80;
GLdouble z_camera = 200;


double x_objeto, y_objeto, z_objeto;        // cubo
double x_objeto_2, y_objeto_2, z_objeto_2;  // cubo

GLdouble x_alvo = 0;
GLdouble y_alvo = 0;
GLdouble z_alvo = 0;

int slices = 16;
int stacks = 16;

GLdouble x_translacao = 20.0;
GLdouble y_translacao = 41.0;
GLdouble z_translacao = 40.0;

double ang_rotacao_roda_x = 0;
double ang_rotacao_roda_y = 0;
double ang_rotacao_carro_y = 0;

bool moveu = false;

vetor vetorPosicao = vetor(0,0,0);
vetor vetorSentido = vetor(0,0,0);

bool mudouDirecao = false;

bool seta_cima = false;
bool seta_baixo = false;
bool seta_esquerda = false;
bool seta_direita = false;
bool primeiraImpressao = true;


// vetor com os números das texturas
GLuint texture_id[MAX_NO_TEXTURES];


void initTexture (void)
{

	// Habilita o uso de textura
	glEnable ( GL_TEXTURE_2D );

	// Define a forma de armazenamento dos pixels na textura (1= alihamento por byte)
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	// Define quantas texturas serão usadas no programa
	glGenTextures (1, texture_id);  // 1 = uma textura;
									// texture_id = vetor que guardas os números das texturas

	// Define o número da textura do cubo.
	texture_id[0] = 1001;
	texture_id[1] = 1002;
	texture_id[2] = 1003;
	texture_id[3] = 1004;
	texture_id[4] = 1005;
	texture_id[5] = 1006;
	texture_id[6] = 1007;
	texture_id[7] = 1008;

	// Define que tipo de textura será usada
	// GL_TEXTURE_2D ==> define que será usada uma textura 2D (bitmaps)
	// texture_id[CUBE_TEXTURE]  ==> define o número da textura
	glBindTexture ( GL_TEXTURE_2D, texture_id[0] );

	// carrega a uma imagem TGA
	image_t temp_image;
	tgaLoad  ( "C:\\imagens\\lec.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

	glBindTexture ( GL_TEXTURE_2D, texture_id[1] );
	tgaLoad  ( "C:\\imagens\\lecf.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

	glBindTexture ( GL_TEXTURE_2D, texture_id[2] );
	tgaLoad  ( "C:\\imagens\\ldc.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

    glBindTexture ( GL_TEXTURE_2D, texture_id[3] );
	tgaLoad  ( "C:\\imagens\\ldcf.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

    glBindTexture ( GL_TEXTURE_2D, texture_id[4] );
	tgaLoad  ( "C:\\imagens\\tra.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

    glBindTexture ( GL_TEXTURE_2D, texture_id[5] );
	tgaLoad  ( "C:\\imagens\\frente.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

	glBindTexture ( GL_TEXTURE_2D, texture_id[6] );
	tgaLoad  ( "C:\\imagens\\parabrisa.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

    glBindTexture ( GL_TEXTURE_2D, texture_id[7] );
	tgaLoad  ( "C:\\imagens\\teto.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

    glBindTexture ( GL_TEXTURE_2D, texture_id[8] );
	tgaLoad  ( "C:\\imagens\\capo.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

}

void rotacionarY(GLfloat angulo)
{
    ang_rotacao_roda_y += angulo;

    if(ang_rotacao_roda_y > 60) {
        ang_rotacao_roda_y = 60;

    } else if(ang_rotacao_roda_y < -60) {
        ang_rotacao_roda_y = -60;

    }
    //printf("aslakasa %lf\n\n", ang_rotacao_roda_y);
}

void mover(GLdouble dist)
{
    vetor vetorDirecao;

    if (ang_rotacao_roda_y) {

        //rotacionar em torno de y:
        vetorDirecao.x = cos( (ang_rotacao_roda_y ) * PIdiv180);
        vetorDirecao.z = -sin( (ang_rotacao_roda_y ) * PIdiv180);

   } else {

        //rotacionar em torno de y:
        vetorDirecao.x = cos( (ang_rotacao_roda_y ) * PIdiv180);
        vetorDirecao.z = sin( (ang_rotacao_roda_y ) * PIdiv180);

        //printf("Angulo MENOR y %lf\n",ang_rotacao_roda_y );
        //printf("Angulo MENOR x %lf\n",ang_rotacao_roda_x );

   }
    //ffa2- troquei z por x em vetorCoordCarro!!!!!!!!!!!
    vetorPosicao.x -= vetorDirecao.x * dist;
    vetorPosicao.z += vetorDirecao.z * dist;
}


void verificar_teclas(){

    double inc_t = 90;
    double inc_r = 15;

	bool bateu_f = false;
	bool bateu_a = false;

	bateu_f = (abs(x_objeto - vetorPosicao.x - 3.5) <=1  );

	//bateu_a = (abs(x_objeto_2 - vetorPosicao.x) <=1);

    if(seta_baixo && !bateu_a) {
        ang_rotacao_roda_x += 0.1;
        mover(-0.5);
        moveu = true;

    }

    if(seta_cima && !bateu_f) {
        ang_rotacao_roda_x -= 0.1;
        mover(0.5);
        moveu = true;
    }

    if(seta_esquerda) {
        rotacionarY(inc_r);
    }

    if(seta_direita) {
        rotacionarY(-inc_r);
    }
}
void roda(GLdouble x, GLdouble y, GLdouble z, bool ehRodaFrente)
{
    glPushMatrix();
        glTranslated(x, y, z);
        if(ehRodaFrente)
            glRotated(ang_rotacao_roda_y,0,1,0);
        glRotated(ang_rotacao_roda_x,0,0,1);  //roda a roda =P
        glutSolidTorus(0.4,1.0,slices,stacks);
    glPopMatrix();

}

void desenhar_rodas_carro() {

    glColor3f(0.0f, 0.0f, 0.0f); //deixa as rodas pretas

	roda(20, 42, 40, false);
	roda(20, 42, 40 -10, false);
	roda(20 - 10, 42, 40, true);
	roda(20 - 10, 42, 40 -10, true);
}

void desenhar_plano() {

    glColor3f(0.0f, 0.0f, 1.0f); //deixa o plano azul
    glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0); // Normal do plano
		glVertex3f(-40.0, 40.0, -80.0);
		glVertex3f(-40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, -80.0);
	glEnd();
}

void desenhar_outra_parte_carro() {

	double inc_inf_A_y = 2; // Parte de baixo e de cima do carrinho
	double inc_sup_A_y = 4; // Parte de baixo e de cima do carrinho

	double inc_inf_A_x = 3; // Parte de baixo do carrinho (e algumas de cima tambem)
	double inc_sup_A_x = -13; //Parte de baixo do carrinho

	double inc_inf_B_x = -7; // Parte de cima
	double inc_Z = -9; // todas as partes...

    glColor3f(1.0f, 1.0f, 1.0f);


	glBindTexture ( GL_TEXTURE_2D, texture_id[0] );

    glBegin ( GL_QUADS );
		// Face frontal maior
		glNormal3f(0.0, 0.0, 1.0);  // Normal da face
		glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_inf_B_x,42, 40-1); // M'' - D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_inf_A_x,42, 40-1); // c - c
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_inf_A_x,42+inc_sup_A_y, 40-1); // i - b
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_inf_B_x,42+inc_sup_A_y, 40-1); // M - a
	glEnd();

	glBindTexture ( GL_TEXTURE_2D, texture_id[1] );
	glBegin ( GL_QUADS );
		// Face frontal menor
		glNormal3f(0.0, 0.0, 1.0);  // Normal da face
		glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_sup_A_x,42, 40-1); //	 B -D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_inf_B_x,42, 40-1); // m'' -C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_inf_B_x,42+inc_inf_A_y, 40-1); //	M' -B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_sup_A_x,42+inc_inf_A_y, 40-1); //	 H -A
	glEnd();

    glBindTexture ( GL_TEXTURE_2D, texture_id[2] );
	glBegin ( GL_QUADS );
		// Face posterio maior
		glNormal3f(0.0, 0.0, -1.0);  // Normal da face
		glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_inf_A_x, 42, 40+inc_Z); // D - D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_inf_B_x, 42, 40+inc_Z); // L'' - C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_inf_B_x, 42+inc_sup_A_y, 40+inc_Z); // L - B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_inf_A_x, 42+inc_sup_A_y, 40+inc_Z); // J - A
	glEnd();

    glBindTexture ( GL_TEXTURE_2D, texture_id[3] );
	glBegin ( GL_QUADS );
		// Face posterio menor
		glNormal3f(0.0, 0.0, -1.0);  // Normal da face
		glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_inf_B_x, 42, 40+inc_Z); // L'' - D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_sup_A_x, 42, 40+inc_Z); //  A  - C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_sup_A_x, 42+inc_inf_A_y, 40+inc_Z); //  G - B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_inf_B_x, 42+inc_inf_A_y, 40+inc_Z); //  L'- A
	glEnd();

    glBindTexture ( GL_TEXTURE_2D, texture_id[4] );
	glBegin ( GL_QUADS );
		// Face traseira
		glNormal3f(1.0, 0.0, 0.0);  // Normal da face
        glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_inf_A_x,42, 40-1); //	 C - D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_inf_A_x, 42, 40+inc_Z); //  D  - C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_inf_A_x, 42+inc_sup_A_y, 40+inc_Z); //	J - B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_inf_A_x,42+inc_sup_A_y, 40-1); //  I - A
	glEnd();

    glBindTexture ( GL_TEXTURE_2D, texture_id[5] );
	glBegin ( GL_QUADS );
		// Face frente
		glNormal3f(-1.0, 0.0, 0.0);  // Normal da face
        glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_sup_A_x, 42, 40+inc_Z); //  A  - D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_sup_A_x,42, 40-1); //	 B - C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_sup_A_x,42+inc_inf_A_y, 40-1); //	 H - B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_sup_A_x, 42+inc_inf_A_y, 40+inc_Z); //  G - A
	glEnd();

    glBindTexture ( GL_TEXTURE_2D, texture_id[6] );
	glBegin ( GL_QUADS );
		// Face parabrisa
		glNormal3f(-1.0, 0.0, 0.0);  // Normal da face
        glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_inf_B_x, 42+inc_inf_A_y, 40+inc_Z); //  L'- D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_inf_B_x,42+inc_inf_A_y, 40-1); //	M' -C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_inf_B_x,42+inc_sup_A_y, 40-1); // M - B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_inf_B_x, 42+inc_sup_A_y, 40+inc_Z); // L - A
	glEnd();

    glBindTexture ( GL_TEXTURE_2D, texture_id[7] );
	glBegin ( GL_QUADS );
		// Face teto
		glNormal3f(0.0, 1.0, 0.0);  // Normal da face
        glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_inf_B_x,42+inc_sup_A_y, 40-1); // M - D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_inf_A_x,42+inc_sup_A_y, 40-1); //  I - C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_inf_A_x, 42+inc_sup_A_y, 40+inc_Z); //	J - B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_inf_B_x, 42+inc_sup_A_y, 40+inc_Z); // L - A
	glEnd();

    glBindTexture ( GL_TEXTURE_2D, texture_id[8] );
	glBegin ( GL_QUADS );
		// Face capo
		glNormal3f(0.0, 1.0, 0.0);  // Normal da face
        glTexCoord2d(0.0, 0.0); glVertex3f(20+inc_sup_A_x, 42+inc_inf_A_y, 40+inc_Z); //  G - D
		glTexCoord2d(1.0, 0.0); glVertex3f(20+inc_sup_A_x,42+inc_inf_A_y, 40-1); //	 H - C
		glTexCoord2d(1.0, 1.0); glVertex3f(20+inc_inf_B_x,42+inc_inf_A_y, 40-1); //	M' -B
		glTexCoord2d(0.0, 1.0); glVertex3f(20+inc_inf_B_x, 42+inc_inf_A_y, 40+inc_Z); //  L'- A
	glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
}

void desenhar_obstaculos(){

    glColor3f(10.0f, 0.0f, 1.0f);

    x_objeto = -10;
	y_objeto = 45;
	z_objeto = 40;

	x_objeto_2 = 30;
	y_objeto_2 = 45;
	z_objeto_2 = 40;

	glPushMatrix();
        glTranslated(x_objeto, y_objeto, z_objeto);
        glutSolidCube(10);
    glPopMatrix();

 /*   glPushMatrix();
        glTranslated(30, 45, 40);
        glutSolidCube(10);
    glPopMatrix();*/
}
// Função callback chamada para fazer o desenho
void Desenha(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(moveu) {
            ang_rotacao_carro_y += ang_rotacao_roda_y;
            glRotated(ang_rotacao_carro_y,0,1,0);
            moveu = false;
        }
       glPushMatrix();
            glTranslated(vetorPosicao.x ,vetorPosicao.y,vetorPosicao.z);
            desenhar_rodas_carro();
            desenhar_outra_parte_carro();
       glPopMatrix();

       desenhar_plano();
       desenhar_obstaculos();
       printf("x: %lf , y: %lf , z: %lf\n\n",vetorPosicao.x,vetorPosicao.y - 35,vetorPosicao.z);

	glutSwapBuffers();
}



// iluminação
void light (void)
{
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;

 	// Especifica que a cor de fundo da janela será preta
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	angle=45;
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle,fAspect,0.4,500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

    gluLookAt( x_camera,y_camera,z_camera, x_alvo,y_alvo,z_alvo, 0,1,0);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (angle >= 10) angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (angle <= 130) angle += 5;
		}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;

		case 'j':
			x_camera+=10;
			EspecificaParametrosVisualizacao();
			glutPostRedisplay();
			break;

		case 'k':
			y_camera+=10;
			EspecificaParametrosVisualizacao();
			glutPostRedisplay();
			break;

		case 'l':
			z_camera+=10;
			EspecificaParametrosVisualizacao();
			glutPostRedisplay();
			break;

		case 'u':
			x_camera-=10;
			EspecificaParametrosVisualizacao();
			glutPostRedisplay();
			break;

		case 'i':
			y_camera-=10;
			EspecificaParametrosVisualizacao();
			glutPostRedisplay();
			break;

		case 'o':
			z_camera-=10;
			EspecificaParametrosVisualizacao();
			glutPostRedisplay();
			break;
    }

    glutPostRedisplay();
}

void s_key_up(int key, int x, int y)
{

    switch (key)
    {
        case GLUT_KEY_DOWN:
            seta_baixo = false;
            break;

        case GLUT_KEY_UP:
            seta_cima = false;
            break;

        case GLUT_KEY_LEFT:
            seta_esquerda = false;
            break;

        case GLUT_KEY_RIGHT:
            seta_direita = false;
            break;
    }

    glutPostRedisplay();
}

void s_key(int key, int x, int y)
{

    switch (key)
    {
        case GLUT_KEY_DOWN:
            seta_baixo = true;
            break;

        case GLUT_KEY_UP:
            seta_cima = true;
            break;

        case GLUT_KEY_LEFT:
            seta_esquerda = true;
            break;

        case GLUT_KEY_RIGHT:
            seta_direita = true;
            break;

        case GLUT_KEY_F5:
            x_translacao = 20.0;
            y_translacao = 41.0;
            z_translacao = 40.0;
            break;
    }

    verificar_teclas();

    glutPostRedisplay();
}

// Programa Principal
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    //coisas da janela
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("calhambeque");

    initTexture();
    glColor3f(0.0f, 0.0f, 1.0f);
	//"métodos" principais
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(key);
	glutSpecialUpFunc(s_key_up);
	glutSpecialFunc(s_key);
	light();

	//processa todas as mensagens
	glutMainLoop();
}
