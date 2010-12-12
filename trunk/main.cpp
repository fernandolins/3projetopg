// Iluminacao.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização
// de objetos 3D com a inserção de uma fonte de luz.
// Este código está baseado nos exemplos disponíveis no livro
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.

#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <gl/glut.h>

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

double ang_rotacao_roda_x = 0.9;
double ang_rotacao_roda_y = 0.9;

void rotacionarY(GLfloat angulo)
{
    y_alvo += angulo;
}

void roda(GLdouble x, GLdouble y, GLdouble z, bool ehRodaFrente)
{

    glPushMatrix();
        glTranslated(x, y, z);
        glRotated(15,1,0,0); // isso deixa a roda na posição certa no plano xz
        if(ehRodaFrente)
            glRotated(ang_rotacao_roda_y,0,1,0);
        glRotated(ang_rotacao_roda_x,0,0,1);  //roda a roda =P
        glutSolidTorus(0.4,1.0,slices,stacks);
    glPopMatrix();

}

// Função callback chamada para fazer o desenho
void Desenha(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);

	// Desenha um cubo
/*
//	glBegin(GL_QUADS);			// Face posterior
//		glNormal3f(0.0, 0.0, 1.0);	// Normal da face
//		glVertex3f(40.0, 40.0, 40.0);
//		glVertex3f(-40.0, 40.0, 40.0);
//		glVertex3f(-40.0, -40.0, 40.0);
//		glVertex3f(40.0, -40.0, 40.0);
//	glEnd();
//
//	glBegin(GL_QUADS);			// Face frontal
//		glNormal3f(0.0, 0.0, -1.0); 	// Normal da face
//		glVertex3f(40.0, 40.0, -40.0);
//		glVertex3f(40.0, -40.0, -40.0);
//		glVertex3f(-40.0, -40.0, -40.0);
//		glVertex3f(-40.0, 40.0, -40.0);
//	glEnd();
//
//	glBegin(GL_QUADS);			// Face lateral esquerda
//		glNormal3f(-1.0, 0.0, 0.0); 	// Normal da face
//		glVertex3f(-40.0, 40.0, 40.0);
//		glVertex3f(-40.0, 40.0, -40.0);
//		glVertex3f(-40.0, -40.0, -40.0);
//		glVertex3f(-40.0, -40.0, 40.0);
//	glEnd();
//
//	glBegin(GL_QUADS);			// Face lateral direita
//		glNormal3f(1.0, 0.0, 0.0);	// Normal da face
//		glVertex3f(40.0, 40.0, 40.0);
//		glVertex3f(40.0, -40.0, 40.0);
//		glVertex3f(40.0, -40.0, -40.0);
//		glVertex3f(40.0, 40.0, -40.0);
//	glEnd();
*/
	glBegin(GL_QUADS);			// Face superior
		glNormal3f(0.0, 1.0, 0.0);  	// Normal da face
		glVertex3f(-40.0, 40.0, -80.0);
		glVertex3f(-40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, 80.0);
		glVertex3f(40.0, 40.0, -80.0);
	glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);

	roda(x_translacao, y_translacao, z_translacao, false);
	roda(x_translacao, y_translacao, z_translacao-20, false);
	roda(x_translacao-20, y_translacao, z_translacao, true);
	roda(x_translacao-20, y_translacao, z_translacao-20, true);

//	glBegin(GL_QUADS);			// Face inferior
//		glNormal3f(0.0, -1.0, 0.0); 	// Normal da face
//		glVertex3f(-40.0, -40.0, -40.0);
//		glVertex3f(40.0, -40.0, -40.0);
//		glVertex3f(40.0, -40.0, 40.0);
//		glVertex3f(-40.0, -40.0, 40.0);
//	glEnd();

	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
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

	// Especifica posição do observador e do alvo
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
    double inc_t = 0.1;
    double inc_r = 50;

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

void teclas_especiais(int key, int x, int y)
{
    double inc_t = 90;
    double inc_r = 20;

    switch (key)
    {

        case GLUT_KEY_DOWN:
            x_translacao += 0.1;
            ang_rotacao_roda_x += inc_r;
            break;

        case GLUT_KEY_UP:
            x_translacao -= 0.1;
            ang_rotacao_roda_x -= inc_r;
            break;

        case GLUT_KEY_LEFT:
            ang_rotacao_roda_y += inc_r;
            if(ang_rotacao_roda_y >= 60.9) ang_rotacao_roda_y = 60.9;
            break;

        case GLUT_KEY_RIGHT:
            ang_rotacao_roda_y -= inc_r;
            if(ang_rotacao_roda_y <= -60.9) ang_rotacao_roda_y = -60.9;
            break;

        case GLUT_KEY_F5:
            x_translacao = 20.0;
            y_translacao = 41.0;
            z_translacao = 40.0;
            break;

    }

    glutPostRedisplay();
}

// Programa Principal
int main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(key);
	glutSpecialFunc(teclas_especiais);
	Inicializa();
	glutMainLoop();
}
