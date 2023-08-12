/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define LARGURA_JANELA 1020
#define ALTURA_JANELA 720
#define PI 3.14

typedef struct {
    float coordenada_x;
    float coordenada_y;
    float velocidade_x;
    float velocidade_y;
    float angulo;
    bool Vivo;
} Objeto;

Objeto nave;
Objeto tiros[5];
Objeto asteroids[10];
bool GameOver = false;
bool Menu = true;
int score = 0;
int vidas = 3; // Número inicial de vidas

// Desenha a nave
void Nave(Objeto obj) {
    glPushMatrix();
    glTranslatef(obj.coordenada_x, obj.coordenada_y, 0.0f);
    glRotatef(obj.angulo, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.0f, -10.0f);  // Ponta inferior da nave
    glVertex2f(5.0f, 5.0f);    // Canto inferior direito da nave
    glVertex2f(0.0f, 2.0f);    // Parte frontal superior da nave
    glVertex2f(-5.0f, 5.0f);   // Canto inferior esquerdo da nave
    glEnd();
    glPopMatrix();
}

// Desenha os asteroides
void Asteroides(Objeto obj) {
    glPushMatrix();
    glTranslatef(obj.coordenada_x, obj.coordenada_y, 0.0f);
    glRotatef(obj.angulo, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca
    glBegin(GL_LINE_LOOP);
    glVertex2f(-15.0f, 0.0f);
    glVertex2f(-10.0f, 5.0f);
    glVertex2f(-5.0f, 15.0f);
    glVertex2f(5.0f, 15.0f);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(15.0f, 0.0f);
    glVertex2f(10.0f, -10.0f);
    glVertex2f(0.0f, -15.0f);
    glVertex2f(-10.0f, -10.0f);
    glEnd();
    glPopMatrix();
}

void iniciarJogo() {
    nave.coordenada_x = LARGURA_JANELA / 2;
    nave.coordenada_y = ALTURA_JANELA / 2;
    nave.velocidade_x = 0.0f;
    nave.velocidade_y = 0.0f;
    nave.angulo = 0.0f;
    nave.Vivo = true;

    for (int i = 0; i < 5; i++) {
        tiros[i].coordenada_x = 0;
        tiros[i].coordenada_y = 0;
        tiros[i].velocidade_x = 0;
        tiros[i].velocidade_y = 0;
        tiros[i].Vivo = false;
    }

    for (int i = 0; i < 10; i++) {
        asteroids[i].coordenada_x = rand() % LARGURA_JANELA;
        asteroids[i].coordenada_y = rand() % ALTURA_JANELA;
        asteroids[i].velocidade_x = (rand() % 2 == 0) ? -1.0f : 1.0f;
        asteroids[i].velocidade_y = (rand() % 2 == 0) ? -1.0f : 1.0f;
        asteroids[i].angulo = (rand() % 360);
    }
}

// Atualiza o estado do jogo
void atualizarJogo() {
    // Adiciona velocidade nas coordenadas X e Y
    nave.coordenada_x += nave.velocidade_x;
    nave.coordenada_y += nave.velocidade_y;

    // Verifica se a nave saiu da tela
    if (nave.coordenada_x < 0) nave.coordenada_x = LARGURA_JANELA;
    if (nave.coordenada_x > LARGURA_JANELA) nave.coordenada_x = 0;
    if (nave.coordenada_y < 0) nave.coordenada_y = ALTURA_JANELA;
    if (nave.coordenada_y > ALTURA_JANELA) nave.coordenada_y = 0;

    // Atualiza a posição de 10 asteroides
    for (int i = 0; i < 10; i++) {
        asteroids[i].coordenada_x += asteroids[i].velocidade_x;
        asteroids[i].coordenada_y += asteroids[i].velocidade_y;

        // Verifica se cada asteroide saiu da tela
        if (asteroids[i].coordenada_x < 0) asteroids[i].coordenada_x = LARGURA_JANELA;
        if (asteroids[i].coordenada_x > LARGURA_JANELA) asteroids[i].coordenada_x = 0;
        if (asteroids[i].coordenada_y < 0) asteroids[i].coordenada_y = ALTURA_JANELA;
        if (asteroids[i].coordenada_y > ALTURA_JANELA) asteroids[i].coordenada_y = 0;

        // Verifica colisão entre asteroides e os tiros
        for (int j = 0; j < 5; j++) {
            if (tiros[j].Vivo) {
                float dx = asteroids[i].coordenada_x - tiros[j].coordenada_x;
                float dy = asteroids[i].coordenada_y - tiros[j].coordenada_y;
                float distance = sqrt(dx * dx + dy * dy);
                if (distance < 20.0f) { // Raio dos asteroides é 20.0f
                    asteroids[i].coordenada_x = rand() % LARGURA_JANELA;
                    asteroids[i].coordenada_y = rand() % ALTURA_JANELA;
                    tiros[j].Vivo = false;
                    score += 10; // Adiciona 10 pontos à pontuação
                }
            }
        }

        // Verifica colisão entre asteroides e nave
        float dx = asteroids[i].coordenada_x - nave.coordenada_x;
        float dy = asteroids[i].coordenada_y - nave.coordenada_y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < 20.0f) { // Raio dos asteroides é 20.0f
            vidas--; // Reduz o número de vidas
            if (vidas <= 0) {
                GameOver = true;
            }
            // Reiniciar a posição da nave e dos asteroides
            nave.coordenada_x = LARGURA_JANELA / 2;
            nave.coordenada_y = ALTURA_JANELA / 2;
            nave.velocidade_x = 0.0f;
            nave.velocidade_y = 0.0f;
            nave.angulo = 0.0f;
            for (int j = 0; j < 10; j++) {
                asteroids[j].coordenada_x = rand() % LARGURA_JANELA;
                asteroids[j].coordenada_y = rand() % ALTURA_JANELA;
                asteroids[j].velocidade_x = (rand() % 2 == 0) ? -1.0f : 1.0f;
                asteroids[j].velocidade_y = (rand() % 2 == 0) ? -1.0f : 1.0f;
                asteroids[j].angulo = (rand() % 360);
            }
        }
    }

    // Verifica se os tiros sairam da tela
    for (int i = 0; i < 5; i++) {
        if (tiros[i].Vivo) {
            tiros[i].coordenada_x += tiros[i].velocidade_x;
            tiros[i].coordenada_y += tiros[i].velocidade_y;

            if (tiros[i].coordenada_x < 0 || tiros[i].coordenada_x > LARGURA_JANELA || tiros[i].coordenada_y < 0 || tiros[i].coordenada_y > ALTURA_JANELA) {
                tiros[i].Vivo = false;
            }
        }
    }
}

void renderizarCena() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (Menu) {
        // Desenhar menu
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(480.0f, 200.0f);
        char menuText1[] = "Asteroids";
        for (int i = 0; i < sizeof(menuText1) / sizeof(menuText1[0]); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, menuText1[i]);
        }

        glRasterPos2f(450.0f, 300.0f);
        char menuText2[] = "1 - Start Game";
        for (int i = 0; i < sizeof(menuText2) / sizeof(menuText2[0]); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, menuText2[i]);
        }

        glRasterPos2f(450.0f, 325.0f);
        char menuText3[] = "2 - Pause Game";
        for (int i = 0; i < sizeof(menuText3) / sizeof(menuText3[0]); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, menuText3[i]);
        }

        glRasterPos2f(450.0f, 350.0f);
        char menuText4[] = "3 - Resume Game";
        for (int i = 0; i < sizeof(menuText4) / sizeof(menuText4[0]); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, menuText4[i]);
        }

        glRasterPos2f(450.0f, 375.0f);
        char menuText5[] = "6 - Quit Game";
        for (int i = 0; i < sizeof(menuText5) / sizeof(menuText5[0]); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, menuText5[i]);
        }
    } else {
        if (!GameOver) {
            Nave(nave);

            for (int i = 0; i < 10; i++) {
                Asteroides(asteroids[i]);
            }

            glColor3f(1.0f, 1.0f, 1.0f); // Cor tiro vermelho
            glPointSize(2.0f);
            glBegin(GL_POINTS);
            for (int i = 0; i < 5; i++) {
                if (tiros[i].Vivo) {
                    glVertex2f(tiros[i].coordenada_x, tiros[i].coordenada_y);
                }
            }
            glEnd();

            // Exibir a pontuação na tela
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2f(20.0f, 40.0f);  // Posição do texto na tela
            char scoreText[50];
            sprintf(scoreText, "Score: %d", score);  // Formata a pontuação em uma string
            for (int i = 0; i < strlen(scoreText); i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
            }

            // Exibir o número de vidas
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2f(20.0f, 60.0f);
            char vidasText[20];
            sprintf(vidasText, "Vidas: %d", vidas);
            for (int i = 0; i < strlen(vidasText); i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, vidasText[i]);
            }

        } else {
            // Desenhar mensagem de fim de jogo
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2f(450.0f, 375.0f);
            char gameOverText[] = "Game Over!";
            for (int i = 0; i < sizeof(gameOverText) / sizeof(gameOverText[0]); i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOverText[i]);
            }
        }
    }

    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Velocidade da nave
            nave.velocidade_x = 2.0 * sin(nave.angulo * PI / 180);
            nave.velocidade_y = -2.0 * cos(nave.angulo * PI / 180);
            break;
        case 'a':
            nave.angulo -= 5.0f;
            break;
        case 'd':
            nave.angulo += 5.0f;
            break;
        case ' ':
            for (int i = 0; i < 5; i++) {
                if (!tiros[i].Vivo) { // Verifica se o tiro está ativo

                    // Posiciona o tiro de acordo com ângulo da nave
                    tiros[i].coordenada_x = nave.coordenada_x + sinf(nave.angulo * PI / 180.0f) * 10;
                    tiros[i].coordenada_y = nave.coordenada_y - cosf(nave.angulo * PI / 180.0f) * 10;

                    // Permite a velocidade do tiro de acordo com ângulo da nave
                    tiros[i].velocidade_x = sinf(nave.angulo * PI / 180.0f) * 5;
                    tiros[i].velocidade_y = -cosf(nave.angulo * PI / 180.0f) * 5;

                    // Indica que o tiro está em movimento
                    tiros[i].Vivo = true;
                    break;
                }
            }
            break;
        case '1':
            Menu = false;
            iniciarJogo();
            break;
        case '2':
            Menu = true;
            break;
        case '3':
            Menu = false;
            break;
        case '6':
            exit(0);
            break;
    }
}

// Função temporizador
void timerCallback(int value) {
    atualizarJogo();
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);
}

// Responsável por redefinir as configurações de visualização da cena
void reshapeCallback(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutCreateWindow("Asteroids");

    glutDisplayFunc(renderizarCena);
    glutKeyboardFunc(teclado);
    glutReshapeFunc(reshapeCallback);
    glutTimerFunc(0, timerCallback, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    iniciarJogo();

    glutMainLoop();
    return 0;
}
