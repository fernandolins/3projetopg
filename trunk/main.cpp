// Iluminacao.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização
// de objetos 3D com a inserção de uma fonte de luz.
// Este código está baseado nos exemplos disponíveis no livro
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.

#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include "vetor.h"
#define PI 3.1415265359
#define PIdiv180 3.1415265359/180.0

GLfloat angle, fAspect;

GLdouble x_camera = 0;
GLdouble y_camera = 80;
GLdouble z_camera = 200;

GLdouble x_alvo = 0;
GLdouble y_alvo = 0;
GLdouble z_alvo = 0;

int slices = 16;
int stacks = 16;

GLdouble x_translacao = 20.0;
GLdouble y_translacao = 41.0;
GLdouble z_translacao = 40.0;

double ang_rotacao_roda_x = 0;
double ang_rotacao_roda_y_1 = 0;
double ang_rotacao_roda_y_2 = 0;

vetor vetorPosicao = vetor(20.0,41.0,40.0);

bool mudouDirecao = false;

bool seta_cima = false;
bool seta_baixo = false;
bool seta_esquerda = false;
bool seta_direita = false;

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

void verificar_teclas(){

    double inc_t = 90;
    double inc_r = 20;

    if(seta_baixo) {
        //moverPraFrente(0.5);
        vetorPosicao.x += 0.5;
        ang_rotacao_roda_x -= inc_r;
    }

    if(seta_cima) {
        //moverPraFrente(-0.5);
        vetorPosicao.x -= 0.5;
        ang_rotacao_roda_x += inc_r;
    }

    if(seta_esquerda) {
        ang_rotacao_roda_y_1 += inc_r;
        rotacionarY(inc_r);
    }

    if(seta_direita) {
        ang_rotacao_roda_y_1 -= inc_r;
        rotacionarY(-inc_r);
    }
}

//void moverPraFrente(GLdouble dist)
//{
//    vetor vetorDirecao;
//
//	if (mudouDirecao) {
//
//        vetor step1;
//        vetor step2;
//
//        //Rotate around Y-axis:
//        step1.x = cos( (ang_rotacao_roda_y_1 + 90.0) * PIdiv180);
//        step1.z = -sin( (ang_rotacao_roda_y_1 + 90.0) * PIdiv180);
//
//        //Rotate around X-axis:
//        double cosX = cos (ang_rotacao_roda_x * PIdiv180);
//        step2.x = step1.x * cosX;
//        step2.z = step1.z * cosX;
//        step2.y = sin(ang_rotacao_roda_y_1 * PIdiv180);
//
//        //Rotation around Z-axis not yet implemented, so:
//        vetorDirecao = step2;
//    }
//
//	vetorPosicao.x += vetorDirecao.x * dist;
//	//vetorPosicao.y += vetorDirecao.y * dist;
//	vetorPosicao.z += vetorDirecao.z * dist;
//
//    mudouDirecao = false;
//}

void roda(GLdouble x, GLdouble y, GLdouble z, bool ehRodaFrente)
{

    glPushMatrix();
        glTranslated(x, y, z);
        glRotated(15,1,0,0); // isso deixa a roda na posição certa no plano xz
        if(ehRodaFrente)
            glRotated(ang_rotacao_roda_y_1,0,1,0);
        glRotated(ang_rotacao_roda_x,0,0,1);  //roda a roda =P
        glutSolidTorus(0.4,1.0,slices,stacks);
    glPopMatrix();

}

void desenhar_rodas_carro() {

    glColor3f(0.0f, 0.0f, 0.0f); //deixa as rodas pretas

	roda(vetorPosicao.x, vetorPosicao.y, vetorPosicao.z, false);
	roda(vetorPosicao.x, vetorPosicao.y, vetorPosicao.z -10, false);
	roda(vetorPosicao.x - 10, vetorPosicao.y, vetorPosicao.z, true);
	roda(vetorPosicao.x - 10, vetorPosicao.y, vetorPosicao.z -10, true);
}

void desenhar_outra_parte_carro() {

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);	 // Face inferior
        glNormal3f(0.0, -1.0, 0.0);  // Normal da face
        glVertex3f(vetorPosicao.x-23, vetorPosicao.y, z_translacao-20); // sup esq	 A
        glVertex3f(vetorPosicao.x-23,vetorPosicao.y, z_translacao); // inf esq	 B
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y, z_translacao); // inf dir	 C
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y, z_translacao-20); // sup dir	 D
    glEnd();


    glBegin(GL_QUADS);	 // Face posterior
        glNormal3f(0.0, 0.0, -1.0);	// Normal da face
        glVertex3f(vetorPosicao.x-23, vetorPosicao.y, z_translacao-20); //	 A
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y, z_translacao-20); //	 D
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y+5, z_translacao-20); //	 E
        glVertex3f(vetorPosicao.x-23, vetorPosicao.y+5, z_translacao-20); // G
    glEnd();


    glBegin(GL_QUADS);	 // Face lateral direita
        glNormal3f(1.0, 0.0, 0.0);	// Normal da face
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y, z_translacao); //	 C
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y, z_translacao-20); //	 D
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y+5, z_translacao-20); //	 E
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y+5, z_translacao); //	 F
    glEnd();

    glBegin(GL_QUADS);	 // Face lateral esquerda
        glNormal3f(-1.0, 0.0, 0.0); // Normal da face
        glVertex3f(vetorPosicao.x-23,vetorPosicao.y, z_translacao); //	 B
        glVertex3f(vetorPosicao.x-23, vetorPosicao.y, z_translacao-20); //	 A
        glVertex3f(vetorPosicao.x-23, vetorPosicao.y+5, z_translacao-20); //	 G
        glVertex3f(vetorPosicao.x-23,vetorPosicao.y+5, z_translacao); //	 H
    glEnd();

    glBegin(GL_QUADS);	 // Face frontal
        glNormal3f(0.0, 0.0, 1.0); // Normal da face
        glVertex3f(vetorPosicao.x-23,vetorPosicao.y, z_translacao); //	 B
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y, z_translacao); //	 C
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y+5, z_translacao); //	 F
        glVertex3f(vetorPosicao.x-23,vetorPosicao.y+5, z_translacao); //	 H
    glEnd();

    glBegin(GL_QUADS);	 // Face superior
        glNormal3f(0.0, 1.0, 0.0);  // Normal da face
        glVertex3f(vetorPosicao.x-23, vetorPosicao.y+5, z_translacao-20); // sup esq	G
        glVertex3f(vetorPosicao.x-23,vetorPosicao.y+5, z_translacao); // inf esq	 H
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y+5, z_translacao); // inf dir	 F
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y+5, z_translacao-20); // sup dir	 E
    glEnd();

    //Parte de cima

    glBegin(GL_QUADS);	 // Face posterior
        glNormal3f(0.0, 0.0, -1.0);	// Normal da face
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y+4, z_translacao-20-0.2); //	 E
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y+10, z_translacao-20); //	 J
        glVertex3f(vetorPosicao.x-15, vetorPosicao.y+10, z_translacao-20); //	 L
        glVertex3f(vetorPosicao.x-15, vetorPosicao.y+4, z_translacao-20-0.2); //	 L'
    glEnd();

    glColor4f(0.3f, 0.4f, 1.0f, 1.0f);

    glBegin(GL_QUADS);	 // Face lateral esquerda
        glNormal3f(-1.0, 0.0, 0.0); // Normal da face
        glVertex3f(vetorPosicao.x-15, vetorPosicao.y+10, z_translacao-20); //	 L
        glVertex3f(vetorPosicao.x-15,vetorPosicao.y+10, z_translacao); //	 M
        glVertex3f(vetorPosicao.x-15,vetorPosicao.y+5, z_translacao); //	 M'
        glVertex3f(vetorPosicao.x-15, vetorPosicao.y+5, z_translacao-20); //	 L'
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);	 // Face lateral direita
        glNormal3f(1.0, 0.0, 0.0);	// Normal da face
        glVertex3f(vetorPosicao.x+3+0.1, vetorPosicao.y+4, z_translacao-20); //	 E
        glVertex3f(vetorPosicao.x+3+0.1,vetorPosicao.y+4, z_translacao); //	 F
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y+10, z_translacao); //	 I
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y+10, z_translacao-20); //	 J
    glEnd();

    glBegin(GL_QUADS);	 // Face frontal
        glNormal3f(0.0, 0.0, 1.0); // Normal da face
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y+5, z_translacao+0.1); //	 F
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y+10, z_translacao); //	 I
        glVertex3f(vetorPosicao.x-15,vetorPosicao.y+10, z_translacao); //	 M
        glVertex3f(vetorPosicao.x-15,vetorPosicao.y+5, z_translacao+0.1); //	 M'
    glEnd();

    glBegin(GL_QUADS);	 // Face superior
        glNormal3f(0.0, 1.0, 0.0);  // Normal da face
        glVertex3f(vetorPosicao.x-15, vetorPosicao.y+10, z_translacao-20); // sup esq	L
        glVertex3f(vetorPosicao.x-15,vetorPosicao.y+10, z_translacao); // inf esq	 M
        glVertex3f(vetorPosicao.x+3,vetorPosicao.y+10, z_translacao); // inf dir	 I
        glVertex3f(vetorPosicao.x+3, vetorPosicao.y+10, z_translacao-20); // sup dir	J
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);

}

// Função callback chamada para fazer o desenho
void Desenha(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f); //deixa o plano azul

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0); // Normal do plano
		glVertex3f(-40.0, 40.0, -80.0);
		glVertex3f(-40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, -80.0);
	glEnd();

    desenhar_rodas_carro();
    desenhar_outra_parte_carro();

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
int main(void)
{
    //coisas da janela
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("calhambeque");

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
