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
#include <gl/glut.h>
#endif

#include <stdio.h>
#include <math.h>
#include "vetor.h"
#define PI 3.1415265359
#define PIdiv180 3.1415265359/180.0

GLfloat angle, fAspect;

vetor vetorCoordCamera = vetor(0,80, 200);
vetor vetorCoordCarro = vetor(20.0,41.0,40.0);
vetor vetorDistPercorrida = vetor(0.0,0.0,0.0);

//GLdouble vetorCoordCamera.x = 0;
//GLdouble vetorCoordCamera.y = 80;
//GLdouble vetorCoordCamera.z = 200;

GLdouble x_alvo = 0;
GLdouble y_alvo = 0;
GLdouble z_alvo = 0;

int slices = 16;
int stacks = 16;

//GLdouble x_translacao = 20.0;
//GLdouble y_translacao = 41.0;
//GLdouble z_translacao = 40.0;

double ang_rotacao_roda_x = 0;
double ang_rotacao_roda_y_1 = 0;
double ang_rotacao_roda_y_2 = 0;


bool mudouDirecao = false;

bool seta_cima = false;
bool seta_baixo = false;
bool seta_esquerda = false;
bool seta_direita = false;

//posição inicial do carro antes de mudar a câmera

GLdouble xi = vetorCoordCarro.x;
GLdouble yi = vetorCoordCarro.y;
GLdouble zi = vetorCoordCarro.z;

void rotacionarY(GLfloat angulo)
{
    ang_rotacao_roda_y_1 += angulo;

    mudouDirecao = true;

    if(angulo > 0) {
        if(ang_rotacao_roda_y_1 > 60) {
            ang_rotacao_roda_y_1 = 60;
        } else if (ang_rotacao_roda_y_1 == 60) {
            mudouDirecao = false;
        }
    } else {
        if(ang_rotacao_roda_y_1 < -60) {
            ang_rotacao_roda_y_1 = -60;
        } else if (ang_rotacao_roda_y_1 == 60) {
            mudouDirecao = false;
        }
    }

}

void moverLado(GLdouble dist, bool esquerda) {

    vetor vetorDirecao;

	if (mudouDirecao) {

        vetor step1;
        vetor step2;

        //Rotate around Y-axis:
        step1.x = sin( (ang_rotacao_roda_y_1 + 90.0) * PIdiv180);
        step1.z = -cos( (ang_rotacao_roda_y_1 + 90.0) * PIdiv180);

//        //Rotate around X-axis:
//        double cosX = cos (ang_rotacao_roda_x * PIdiv180);
//        step2.x = step1.x * cosX;
//        step2.z = step1.z * cosX;
//        step2.y = sin(ang_rotacao_roda_y_1 * PIdiv180);
//
//        //Rotation around Z-axis not yet implemented, so:
        vetorDirecao = step1;
    }

    if(esquerda) {
        vetorCoordCarro.x -= vetorDirecao.x + dist;
        //vetorCoordCarro.y += vetorDirecao.y * dist;
        vetorCoordCarro.z -= vetorDirecao.z + dist;
    } else {
        vetorCoordCarro.x += vetorDirecao.x + dist;
        //vetorCoordCarro.y += vetorDirecao.y * dist;
        vetorCoordCarro.z += vetorDirecao.z + dist;
    }

    mudouDirecao = false;
}

void moverPraFrente(GLdouble dist)
{
    vetor vetorDirecao;

	if (mudouDirecao) {

        vetor step1;
        vetor step2;

        //Rotate around Y-axis:
        step1.x = cos( (ang_rotacao_roda_y_1 + 90.0) * PIdiv180);
        step1.z = -sin( (ang_rotacao_roda_y_1 + 90.0) * PIdiv180);

        //Rotate around X-axis:
        double cosX = cos (ang_rotacao_roda_x * PIdiv180);
        step2.x = step1.x * cosX;
        step2.z = step1.z * cosX;
        step2.y = sin(ang_rotacao_roda_y_1 * PIdiv180);

        //Rotation around Z-axis not yet implemented, so:
        vetorDirecao = step1;
    }

	vetorCoordCarro.x += vetorDirecao.x * dist;
	vetorCoordCarro.y += vetorDirecao.y * dist;
	vetorCoordCarro.z += vetorDirecao.z * dist;

    mudouDirecao = false;
}

void verificar_teclas(){

    double inc_t = 90;
    double angulo = 60;

    if(seta_baixo) {
        //moverPraFrente(0.5);
        vetorCoordCarro.x += 0.5;
        ang_rotacao_roda_x += angulo;
    }

    if(seta_cima) {
        //moverPraFrente(-0.5);
        vetorCoordCarro.x -= 0.5;
        ang_rotacao_roda_x -= angulo;
    }

    if(seta_esquerda) {
        //ang_rotacao_roda_y_1 += inc_r;
        rotacionarY(angulo);
        //moverPraFrente(angulo);
        //moverLado(5, true);
    }

    if(seta_direita) {
        //ang_rotacao_roda_y_1 -= inc_r;
        rotacionarY(-angulo);
    }

}

void roda(GLdouble x, GLdouble y, GLdouble z, bool ehRodaFrente)
{

    glPushMatrix();
        glTranslated(x, y, z);
        if(ehRodaFrente)
            glRotated(ang_rotacao_roda_y_1,0,1,0);
        glRotated(ang_rotacao_roda_x,0,0,1);  //roda a roda =P
        glutSolidTorus(0.5,1.0,slices,stacks);
    glPopMatrix();

}

void desenhar_plano() {

    glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0); // Normal do plano
		glVertex3f(-40.0, 40.0, -80.0);
		glVertex3f(-40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, -80.0);
	glEnd();
}

void desenhar_rodas_carro() {

    glColor3f(0.0f, 0.0f, 0.0f); //deixa as rodas pretas

	roda(vetorCoordCarro.x, vetorCoordCarro.y, vetorCoordCarro.z, false);
	roda(vetorCoordCarro.x, vetorCoordCarro.y, vetorCoordCarro.z -10, false);
	roda(vetorCoordCarro.x - 10, vetorCoordCarro.y, vetorCoordCarro.z, true);
	roda(vetorCoordCarro.x - 10, vetorCoordCarro.y, vetorCoordCarro.z -10, true);
}

void desenhar_outra_parte_carro() {

    double inc_inf_A_y = 2; // Parte de baixo e de cima do carrinho
    double inc_sup_A_y = 4; // Parte de baixo e de cima do carrinho
    double inc_inf_A_x = 3; // Parte de baixo do carrinho (e algumas de cima tambem)
    double inc_sup_A_x = -13; //Parte de baixo do carrinho
    double inc_inf_B_x = -7; // Parte de cima
    double inc_Z = -9; // todas as partes...

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);	 // Face inferior
        glNormal3f(0.0, -1.0, 0.0);  // Normal da face
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x, vetorCoordCarro.y, vetorCoordCarro.z+inc_Z); // sup esq	 A
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x,vetorCoordCarro.y, vetorCoordCarro.z-1); // inf esq	 B
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x,vetorCoordCarro.y, vetorCoordCarro.z-1); // inf dir	 C
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x, vetorCoordCarro.y, vetorCoordCarro.z+inc_Z); // sup dir	 D
    glEnd();

    glBegin(GL_POLYGON);	 // Face poligono frontal
        glNormal3f(0.0, 0.0, 1.0);  // Normal da face
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x,vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z-1); //	 M'
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x,vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z-1); //	 H
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x,vetorCoordCarro.y, vetorCoordCarro.z-1); //	 B
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x,vetorCoordCarro.y, vetorCoordCarro.z-1); //	 C
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x,vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z-1); //	 I
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x,vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z-1); //	 M
    glEnd();

    glBegin(GL_POLYGON);	 // Face poligono posterior
        glNormal3f(0.0, 0.0, -1.0);  // Normal da face
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x, vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z+inc_Z); //	 L'
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x, vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z+inc_Z); // G
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x, vetorCoordCarro.y, vetorCoordCarro.z+inc_Z); //	 A
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x, vetorCoordCarro.y, vetorCoordCarro.z+inc_Z); //	 D
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x, vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z+inc_Z); //	 J
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x, vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z+inc_Z); //	 L
    glEnd();

    glBegin(GL_POLYGON);	 // Face direita
        glNormal3f(1.0, 0.0, 0.0);  // Normal da face
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x,vetorCoordCarro.y, vetorCoordCarro.z-1); //	 C
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x, vetorCoordCarro.y, vetorCoordCarro.z+inc_Z); //	 D
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x, vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z+inc_Z); // sup dir	J
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x,vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z-1); // inf dir	 I
    glEnd();

    glBegin(GL_POLYGON);	 // Face esquerda
        glNormal3f(-1.0, 0.0, 0.0); // Normal da face
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x,vetorCoordCarro.y, vetorCoordCarro.z-1); //	 B
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x, vetorCoordCarro.y, vetorCoordCarro.z+inc_Z); //	 A
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x, vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z+inc_Z); //	 G
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x,vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z-1); //	 H
    glEnd();

    glBegin(GL_QUADS);	 // Face "capô"
        glNormal3f(0.0, 1.0, 0.0);  // Normal da face
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x, vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z+inc_Z); //	 G
        glVertex3f(vetorCoordCarro.x+inc_sup_A_x,vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z-1); //	 H
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x,vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z-1); //	M'
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x, vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z+inc_Z); //	 L'
    glEnd();

    glBegin(GL_QUADS);	 // Face superior
        glNormal3f(0.0, 1.0, 0.0);  // Normal da face
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x,vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z-1); // inf dir	 I
        glVertex3f(vetorCoordCarro.x+inc_inf_A_x, vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z+inc_Z); // sup dir	J
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x, vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z+inc_Z); //	 L
        glVertex3f(vetorCoordCarro.x+inc_inf_B_x,vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z-1); //	 M
    glEnd();

    glColor4f(0.3f, 0.4f, 1.0f, 1.0f);

    glBegin(GL_QUADS);	 // Face "parabrisa"
    glNormal3f(-1.0, 0.0, 0.0);  // Normal da face
    glVertex3f(vetorCoordCarro.x+inc_inf_B_x, vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z+inc_Z); //	 L
    glVertex3f(vetorCoordCarro.x+inc_inf_B_x,vetorCoordCarro.y+inc_sup_A_y, vetorCoordCarro.z-1); //	 M
    glVertex3f(vetorCoordCarro.x+inc_inf_B_x,vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z-1); //	M'
    glVertex3f(vetorCoordCarro.x+inc_inf_B_x, vetorCoordCarro.y+inc_inf_A_y, vetorCoordCarro.z+inc_Z); //	 L'
    glEnd();

}

// Função callback chamada para fazer o desenho
void Desenha(void)
{

    glLoadIdentity();
    gluLookAt( vetorCoordCamera.x, vetorCoordCamera.y, vetorCoordCamera.z, vetorCoordCarro.x - xi, 0 ,vetorCoordCarro.z - zi - 100, 0,1,0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f); //deixa o plano azul

    glPushMatrix();

    desenhar_plano();
    desenhar_rodas_carro();
    desenhar_outra_parte_carro();

    glPopMatrix();

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
//	glLoadIdentity();
//
//    gluLookAt( vetorCoordCamera.x,vetorCoordCamera.y,vetorCoordCamera.z, vetorCoordCarro.x, 0 ,vetorCoordCarro.z - 100, 0,1,0);
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

        case 'i':
            vetorCoordCamera.x += 10;
            x_alvo -= 10;
            EspecificaParametrosVisualizacao();
            glutPostRedisplay();
            break;
        case 'o':
            vetorCoordCamera.y += 10;
            y_alvo -= 10;
            EspecificaParametrosVisualizacao();
            glutPostRedisplay();
            break;
        case 'p':
            vetorCoordCamera.z += 10;
            z_alvo -= 10;
            EspecificaParametrosVisualizacao();
            glutPostRedisplay();
            break;
        case 'j':
            vetorCoordCamera.x -= 10;
            x_alvo += 10;
            EspecificaParametrosVisualizacao();
            glutPostRedisplay();
            break;
        case 'k':
            vetorCoordCamera.y -= 10;
            y_alvo += 10;
            EspecificaParametrosVisualizacao();
            glutPostRedisplay();
            break;
        case 'l':
            vetorCoordCamera.z -= 10;
            z_alvo += 10;
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
            vetorCoordCarro.x = 20.0;
            vetorCoordCarro.y = 41.0;
            vetorCoordCarro.z = 40.0;
            break;
    }

    verificar_teclas();

    glutPostRedisplay();
}

// Programa Principal
int main(void)
{
    //coisas da janela
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Calhambeque");

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
